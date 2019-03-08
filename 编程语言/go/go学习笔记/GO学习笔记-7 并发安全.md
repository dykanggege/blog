前面已经学了 channel 和 sync，现在我们学习如何使用它们解决并发安全问题

在标准库中，包变量除非在文档中标注是并发安全的，我们才能相信，否则应该理所当然的认为它是不安全的，但是包导出的函数通常可认为是并发安全的（这要求函数不依赖外部变量，或者使用外部变量时加锁，即不产生副作用）

对变量的安全操作，要么限制变量的使用在一个goroutine内（多个goroutine通过一个chan去修改这个变量，这就是go所说的通过通讯共享内存），要么就应该加锁

# 以通信共享内存
看下面的例子

```
    type bank struct {
        balance int
    }

    func (this *bank) Deposit(amount int)  {
        this.balance += amount
        //b := this.balance
        //time.Sleep(time.Second)
        //this.balance = b + amount
    }
    func (b bank) GetBalance() int {
        return b.balance
    }

    func main() {
        b := new(bank)
        go func() {
            b.Deposit(200)
            fmt.Println(b.GetBalance())
        }()
        go func() {
            b.Deposit(100)
        }()
        time.Sleep(time.Second)
        fmt.Println(b.GetBalance())
    }
```

实际上，最终的余额可能是 300 也可能是 200，当大量并发同时存在时，就可能会出现我们注释的那种情况，一个协程获取到 balance 是 0，而后该协程被切换走，其他协程存入了 100，再回到原协程时，就会在 0 的基础上 +200 并再赋值给 balance，100 块就这样莫名其妙的消失了

当对数据做只读操作时，并发是安全的，如果多个并发项对同一个数据做读写操作，称之为数据竞态，我们应该默认它是不安全的

在其他语言中（比如 java）并发程序共享内存，使用锁的方式保护数据，即通过共享内存实现通讯，如下

```
    var mu = new(sync.Mutex)
    type Bank struct {
        balance int
    }
    func (this *Bank)Deposit(amount int)  {
        mu.Lock()
        this.balance += amount
        mu.Unlock()
    }
    //或用通道，达到互斥锁模式
    var same = make(chan struct{},1)
    type Bank struct {
        balance int
    }
    func (b Bank) GetBalance() int {
        return b.balance
    }
    func (this *Bank)Deposit(amount int)  {
        same <- struct{}{}
        this.balance += amount
        <- same
    }
```


而 go 推荐的做法是：通过通讯实现共享内存，即，数据只放在一个协程内，其他的并发程序使用通道访问该数据

我们使用这种思想改编上面的代码
```
    var deposits = make(chan int)
    var balances = make(chan int)

    func taller() {
        var balance int
        select {
        case v := <-deposits:
            balance += v
        case balances <- balance:
        }
    }

    func deposit(amount int) {
        deposits <- amount
    }
    func balance() int {
        return <- balances
    }
    func main() {
        go taller()
    }
```
这样的话，其他并发程序想要修改数据只能通过通道，而真正对数据进行读写操作只在一个协程中进行，就避免了并发安全的问题

# 以共享内存通信
## 互斥锁
go 提供了 sync 去为共享内存加锁，让我们再次审视下面这段代码
```
    type bank struct {
        balance int
        mu *sync.Mutex
    }

    func NewBank() *bank {
        b := new(bank)
        b.mu = new(sync.Mutex)
        return b
    }

    func (this *bank) GetBalance() int {
        this.mu.Lock()
        v := this.balance
        this.mu.Unlock()
        return v
    }
    func (this *bank) Deposit(amount int) {
        this.mu.Lock()
        this.balance += amount
        this.mu.Unlock()
    }
    func (this *bank) WithDraw(amount int) bool {
        this.Deposit(-amount)
        if this.GetBalance() < 0 {
            this.Deposit(amount)
            return false
        }
        return true
    }
```
首先要了解 mu.Lock 加锁的机制，实际它是对 mu 内的一个变量加上锁，当其他程序再次试图对它 Lock 时会被拒绝，直到 Unlock 为止，这样就实现了临界条件

但上面的 WithDraw 有一个问题，在第一次Deposit之后，和第二次Deposit之前，有一个小小的空隙，在这个空隙里可能会被切到其他的goroutine操作，查询操作将是错误的

如果将代码抽离如下
```
    func (this *bank) Deposit(amount int) {
        this.mu.Lock()
        this.deposit(amount)
        this.mu.Unlock()
    }
    func (this *bank) WithDraw(amount int) bool {
        this.mu.Lock()
        defer this.mu.Unlock()
        this.deposit(amount)
        if this.balance < 0 {
            this.deposit(amount)
            return false
        }
        return true
    }
    func (this *bank) deposit(amount int)  {
        this.balance += amount
    }
```
第一次锁之后，调用deposit，这就造成了永久的阻塞，所以用锁是一个很考验心智的东西，在并发操作时，宁愿损失一些性能，使用通讯共享内存，也要保证可读性

## 读写锁
GetBalance，为什么一个读取操作需要加锁？这需要了解 cpu 的工作原理

现代计算机一般都有多个处理器，每个处理器都有自己的寄存器，为了提高效率，对内存的写入都是缓存在寄存器中，只有必要的时候才会再次刷回内存，甚至刷回内存的顺序和程序执行的顺序并不相同，像通信通道或互斥原语都会让处理器把之前累计的写草走刷回内存并提交，在提交之前运行在其他处理器的 goroutine 并不可见，如果 GetBalance 没有加锁，他可能会插在 withDraw 中间执行，而 withDraw 可能还没有刷回内存

但使用互斥锁可能会造成 GetBalance 调用次数太多而其他方法无法调用，着对于一个简单的读操作来说未免太过浪费了，可以使用 sync.RWMutex 读写锁。读锁相当于共享锁，多个读操作并不会相互影响，写锁则是排它锁，写时不能读或者写。读写锁的粒度更小，但是可能

```
    var murw sync.RWMutex
    func (this *bank) GetBalance() int {
        murw.RLock()
        v := this.balance
        murw.RUnlock()
        return v
    }
```

看如下程序
```

    var x, y int

	go func() {
		x = 1
		fmt.Print("y:",y)
	}()

	go func() {
		y = 1
		fmt.Print("x:",x)
	}()

```
对于这样一个并发程序，我们可能期望到多种结果，但是你可能没预料到这样两种结果 x:0 y:0 或 y:0 x:0，事实上在某些编译器和处理器上的确有这种结果，因为赋值和 print 函数对应不同变量，所以调换了他们的顺序，或者他们在两个处理器上执行，因为没有回刷内存而导致彼此不可见

关于并发通信，有着各种各样的可能以及扯淡的结果，我们应该尽可能的将共享变量限制在一个顺序执行的 goroutine，避免这些难以预料的问题