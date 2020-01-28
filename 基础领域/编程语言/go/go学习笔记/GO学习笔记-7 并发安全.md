前面已经学了 channel 和 sync，现在我们学习如何使用它们解决并发安全问题

在标准库中，包变量除非在文档中标注是并发安全的，我们才能相信，否则应该理所当然的认为它是不安全的，但是包导出的函数通常可认为是并发安全的（这要求函数不依赖外部变量，或者使用外部变量时加锁，即不产生副作用）

对变量的安全操作，要么限制变量的使用在一个goroutine内（多个goroutine通过一个chan去修改这个变量，这就是go所说的通过通讯共享内存），要么就应该加锁

# 静态检测
在并发情况下，管理资源的抢占和死锁总是很费脑力，在运行或编译时加上 --race 如 go run --race main.go 能在一定程度上帮助我们做并发的分析

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

但使用互斥锁可能会造成 GetBalance 调用次数太多而其他方法无法调用，着对于一个简单的读操作来说未免太过浪费了，可以使用 sync.RWMutex 读写锁。读锁相当于共享锁，多个读操作并不会相互影响，写锁则是排它锁，写时不能读或者写。

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

# 并发非阻塞缓存
如果我们要设计一个如下的缓存操作
```
type memo struct {
	f Func
	cache map[string]result
}

type result struct {
	res interface{}
	err error
}

type Func func(string)(interface{},error)

func parseUrl(url string) (interface{},error) {
	resp, err := http.Get(url)
	if err != nil{
		return nil,err
	}
	defer resp.Body.Close()
	return ioutil.ReadAll(resp.Body)
}

func NewMemo(f Func) *memo {
	return &memo{f:f,cache: make(map[string]result)}
}

func (m *memo)Get(url string) (interface{},error) {
	res,ok := m.cache[url]
	if !ok{
		res.res,res.err = m.f(url)
		m.cache[url] = res
	}
	return res.res,res.err
}
```

这样会有一个很明显的问题，在并发情况下 m.cache 是不安全的，可能有多个url相同的都没查取到缓存然后去调用函数，这是大大的浪费，最简单的做法是为 m.cache加锁

更新如下
```
type memo struct {
	f Func
	cache map[string]result
	mu sync.Mutex
}

func (m *memo)Get(url string) (interface{},error) {
	m.mu.Lock()
	defer m.mu.Unlock()
	res,ok := m.cache[url]
	if !ok{
		res.res,res.err = m.f(url)
		m.cache[url] = res
	}
	return res.res,res.err
}
```
如果这样做，m.cache将成为一个瓶颈，多个线程将不得不排队等待访问cache，产生了拥塞。如果一个线程访问不到缓存，则去请求，这个线程请求的时候，其他线程不得不等待，因为锁只有在请求完成之后才会打开，这个瓶颈有点堵，我们试试改造一下

```
func (m *memo)Get(url string) (interface{},error) {
	m.mu.Lock()
	res,ok := m.cache[url]
	m.mu.Unlock()
	if !ok{
		m.mu.Lock()
		res.res,res.err = m.f(url)
		m.cache[url] = res
		m.mu.Unlock()
	}
	return res.res,res.err
}
```

这样做锁性能会略微的提升，但是带来了更大的问题，一个url可能会被请求多次，这反而又造成更大的性能损耗

现在最大的问题是堵在 cache 的并发访问上，造成了阻塞，能否有办法在这里不阻塞，让每个线程访问cache之后再去解决查询不到缓存的问题，或者仅仅是加锁查询cache，如果查询不到则解锁并放在线程里处理

```
type entry struct{
	ready chan struct{}
	val result
}

type memo struct {
	f Func
	cache map[string]*entry
	mu sync.Mutex
}

func (m *memo)Get(url string) (interface{},error) {
	m.mu.Lock()
	e := m.cache[url]
	if e == nil{
		e = &entry{ready:make(chan struct{})}
		m.cache[url] = e
		m.mu.Unlock()

		e.val.res,e.val.err = m.f(url)
		close(e.ready)
	}else{
		m.mu.Unlock()
		<- e.ready
	}
	return e.val.res,e.val.err
}
```

这样虽然也有锁，但是阻塞度变小了，一旦查询不到缓存，会放到线程中去解析并存入，这样就减轻了堵塞，锁查询的堵塞瓶颈不大了。


# context.Context
go func(){}() 用着很爽，但是开起来的线程就像泼出去的水，一旦启动后就难以管理，甚至出现goroutine泄露，所以官方推出了context.Context包，用于管理goroutine生命周期和在父子线程间传值，并建议开启线程都要传入context

context包用于线程的管理，通常是一个父线程创建子线程时传入context，用于管理后代(一般是那种一直在后台跑的线程)

在没有context时，如果我们在创建了一个监控线程，想要主动去杀死这个线程，主要靠 select + channel

```
    done := make(chan struct{})
	go func() {
		for{
			select {
			case <-done:
				fmt.Println("运行结束，清理线程")
				return
			default:
				fmt.Println("working...")
			}
		}
	}()
```

当我们创建一个守护线程时，还要保留管理它的channel，当创建的线程太多，甚至子线程再创建，守护线程变得庞大，管理也变得复杂起来

使用context就可以让管理变得简单起来

```
    //context.Background() 创建一个空的context
    //context.WithCancel() 将一个空的context添加cancel功能
    ctx, cancel := context.WithCancel(context.Background())
	go func(ctx context.Context) {
		go func(ctx context.Context) {
			for {
				select {
				case <-ctx.Done():
					fmt.Println("*******我也完事了")
					return
				default:
					fmt.Println("我也运行一下")
					time.Sleep(time.Second*2)
				}
			}
		}(ctx)
		for {
			select {
			case <- ctx.Done():
				fmt.Println("********我完事了")
				return
			default:
				fmt.Println("运行一下")
				time.Sleep(time.Second*2)
			}
		}
	}(ctx)
	time.Sleep(time.Second*5)
	
    //调用cancel函数，所有使用ctx.Done()的线程都退出了
	cancel()
	
	time.Sleep(time.Second*5)
	fmt.Println("******看看谁是最持久的")

```

先看一看Context的接口

```
    type Context interface {
        Deadline() (deadline time.Time, ok bool)
        Done() <-chan struct{}
        Err() error
        Value(key interface{}) interface{}
    }
```

context包为我们提供了这个接口的两个实现

    context.Background()
	context.TODO()

他们都来自一个空的实现

    background = new(emptyCtx)
	todo       = new(emptyCtx)

backgroun代表一个没有实现任何功能的父context，todo则是还没想要做什么，先传入再说

有了根context，就可以调用其他的函数衍生更多的子context

## 使用原则
- 不要把Context放在结构体中，要以参数的方式传递
- 以Context作为参数的函数方法，应该把Context作为第一个参数，放在第一位。
- 给一个函数方法传递Context的时候，不要传递nil，如果不知道传递什么，就使用context.TODO
- Context的Value相关方法应该传递必须的数据，不要什么数据都使用这个传递
- Context是线程安全的，可以放心的在多个goroutine中传递


```
type cancelCtx struct {
	Context

	mu       sync.Mutex            // protects following fields
	done     chan struct{}         // created lazily, closed by first cancel call
	children map[canceler]struct{} // set to nil by the first cancel call
	err      error                 // set to non-nil by the first cancel call
}
```