# 函数
## 函数声明
```

    //可变参数
    func ex1(i int, j ...int) {
        for index, _ := range j {
            j[index] *= i
        }
    }
    
    j := []int{1, 2, 3, 4, 5, 6, 7}
    // slice 解析调用
	ex1(2, j...)
	fmt.Println(j) //[2 4 6 8 10 12 14]

    //预设返回值
    func ex2(s string) (i int) {
        i, _ = strconv.Atoi(s)
        return
    }

    //多返回值
    func ex3(s string) (int, error) {
        i, err := strconv.Atoi(s)
        return i, err
    }
```
- 可变参数，多返回值，裸返回，这是 go 函数的基本特点
- 不支持重载，在大型工程中，重载函数只会让你调用到你意料之外的函数
- 递归栈很大，一般的语言函数递归所能提供的最大栈大概 2k，而 go 函数递归提供动态的栈大小，最大有 2g

## error
go 函数中最大的特点就是 error，在 go 的内置函数中，大多数函数第二个会返回一个 error 值，代表了调用函数失败的错误信息

通常我们并不能保障我们的函数一定会被成功调用，就需要返回 error 来明确提示错误信息，通常他会被作为第二个参数返回，如果调用结果只有两种情况，通常返回一个 bool 表示调用成功或失败

go 在设计时，希望能尽可能的处理错误信息，或尽可能的给出严谨的错误，如果说 java 只做正确的情况，对于所有错误情况通过异常抛出，那么 go 则是希望除了正确情况外，尽可能去处理意料之内的错误情况

提供几个 error 处理的函数
- fmt.Errorf() //返回格式化后的错误信息
- log.Fatal() //打印错误信息，默认带上服务器时间做前缀，可以自定义前缀
- 很多错误常量有利于我们对错误的处理，比如 io.EOF 代表文件读取到尾部了，读取不到了


## 函数使用
在 go 中，函数是一等公民，它也有类型，也可以作为变量赋值传递，特有的就是可以调用

```

    var f0 func(n int) int
    func f1(n int) int {
        return n*n
    }
    func f2(n int) int {
        return n+n
    }
    func f3(n,m int) int {
        return m*n
    }

    f := f1
    fmt.Println(reflect.TypeOf(f)) //func(int) int
    f = f2
    fmt.Println(f(10)) // 20
    //f = f3 报错，因为类型不同
    fmt.Println(reflect.TypeOf(f3)) //func(int, int) int
    
    fmt.Println(reflect.TypeOf(f0) == reflect.TypeOf(f1)) //true
    // fmt.Println(f2 == f1) 函数之间无法直接比较，所以不能作为 map 的 key，但是函数可以和 nil 比较

    //闭包
    func squares() func() int {
        var x int
        return func() int {
            x++
            return x * x
        }
    }
    f := squares()
    fmt.Println(f())  // 1
    fmt.Println(f())  // 4
    fmt.Println(f())  // 9

    //匿名函数与自执行
    func (){
		fmt.Println(1)
	}()
    (func (){
		fmt.Println(1)
	})()
```
- 命名函数只能在包级别声明，但是我们可以在任意地方声明函数变量或使用函数字面量（匿名函数）
- 函数的零值是 nil，如果调用 nil 函数变量，会宕机
- go 的函数用着非常舒服，比动态语言的函数还舒服

## defer
```

    func def() string{
        defer fmt.Print("1")
        defer fmt.Print("2")
        defer fmt.Print("3")
        panic(4)
        return "ok"
    }
    def() //4321
}

```
正常情况下，defer 修饰的语句会在 return 执行之后，逆序执行（堆），如果发生宕机，会在执行完 defer 后关闭程序
- 使用 os.Exit() 立即退出，则 defer 不会被执行
- 使用 panic 则 defer 会被执行

与其他编程语言不同，这是 go 独有的特性，这使得我们可以把本需要放在最后的操作提前进行    

## panic 与 recover
退出程序方式有多种，如os.Exit(1)，如果使用panic，则一定是要和defer结合这用，如果在defer中，我们想把宕机，或错误截取下来并恢复，就可以用 recover
```
func main() {
    defer func{
        if err:=recover();err!=nil{
            //检测错误，如果符合预期，就把他从错误中恢复出来
        }
    }()

    panic("我是个木得感情的错误")
}


```

# oop
go 并不是直接支持 oop，而是提供了组装的方式，让我们自己选择合适的编程方式，go 也可以很好的实现 oop

## oop的本质
在 c++、java 等面向对象语言中，所谓的对象，只是一个语法糖，他们的实质还是 c 的面向过程编程，成员变量如同结构体中的成员，而方法只是隐式的传入了 this 指针

```

    // java 代码
    class Integer{
        private int val;
        public boolean Less(Integer  b){
            reutrn this.val < b
        }
    }

    // go 代码
    func Less(a Integer,b int) bool {
        return a.Val < b
    }

```
上面的两个函数相同，但是第一个隐式的传入了结构体本身，就成了“对象”这一语法糖

go 中的面向对象更为直观，你必须显式的传入结构体，无论式值传递还是引用传递。严格来说，go 是结构化语言，但是也可以作为 oop 语言使用，就像高乐积木一样去拼凑

## 方法
```

    type F func(int) int

    func (f *F) hello()  {
        fmt.Println("hello")
    }
    type S struct {
        name string
        //getName func() string 报错，在结构体中，属性和方法同用一个命名空间，不能重复
        S string //没问题
    }
    // 注意：方法也遵循首字符小写不可导出包外的原则
    func (s *S) getName() string {
        return s.name
    }

```

- 在 go 中，可以将方法绑定在任何 type 修饰的自定义类型上（除了指针类型和接口类型），相比较于 java 的类才能有方法，即一切皆对象的思想，go 的做法更让我感到舒服
- 由于调用函数会复制传递每一个形参，所以在传入参数结构太大时我们选择用指针传递，避免复制浪费不必要的性能，它同样适用于方法的接受者，所以我们一般也选用指针作为接受者，这也是指针类型无法拥有方法的原因（避免太过混淆）

```

    type Person struct {
        name string
        age  int
    }

    func (p *Person) SetName(n string) {
        p.name = n
    }
    func (p Person) SetAge(a int) {
        p.age = a
    }

    p1 := &Person{}
    p1.SetName("kanggege") //可以调用指针接受者的方法
    p1.SetAge(21)          // 也可以调用非指针接受者的方法
    fmt.Println(p1)        //&{kanggege 0}

    p2 := Person{}
    p2.SetName("dyk") //可以调用指针接受者的方法
    p2.SetAge(21)		// 也可以调用非指针接受者的方法
    fmt.Println(p2)		// {dyk 0}

    Person{}.SetAge(22) // 调用非指针接受者方法，没问题
    //Person{}.SetName("hahah") 报错，因为找不到字面量的地址

    p3 := &Person{}
    fmt.Println(p3) // &{ 0}
    p4 := p3
    p4.SetName("kanggege")
    fmt.Println(p3) // &{kanggege 0}
```
- 通过首字符是否大写控制是否导出
- 字面量无法使用指针接受者的方法，因为字面量无法寻址，实际上这样的调用意义也不大
- 结构体指针，谨慎复制，很容易破坏原结构体内部数据，除非你是有意为之
- nil 是一个合法的接受者，这个的意义重大，意味着它作为接受者来说不会报错，但是可能做其他操作会宕机
- 有了访问权限，属性，方法，就自然有了 oop 三大特征之一的：封装

## 组合
```

    type User struct {
        Name string
        Age  int8
    }

    func (u *User) UserHight() {
        fmt.Println("UserHight")
    }
    func (u *User) String() {
        fmt.Printf("User:{Name:%s, Age:%d}", u.Name, u.Age)
    }

    type Student struct {
        User
        Id    string
        Sroce float32
    }

    func (s *Student) StudentHight() {
        fmt.Println("StudentHight")
    }
    func (s *Student) String() {
        fmt.Printf("Student:{User:%v, Id:%s, Sroce:%v", s.User, s.Id, s.Sroce)
    }

    s := Student{User: User{"kanggege", 21}, Id: "007", Sroce: 44.9}
    fmt.Println(s.Name) //等同于调用 s.User.Name
    s.String() // 会优先调用 Student 的方法，找不到了再去 User 内寻找，如果在同一级别找到两个相同的方法，则会报错
    s.UserHight() // 等同于调用 s.User.UserHight()

    var cache = struct {
    sync.Mutex
    mapping map[string]string
    }{
    mapping: make(map[string]string),
    }
```

go 是通过组合的方式实现 oop 的继承，但是它不仅实现了继承基本的功能，而且又提供了很大的灵活度



