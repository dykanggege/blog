# 接口
go 的接口时隐式实现的，即不需声明结构继承了某个接口，只要提供接口所必需的方法，这样就无需改变类型的实现就可以为这些类型创建新的接口

之前的所有面向对象语言的接口都是像契约一样，我们事先定义好接口，再去耦合继承，但我们可能并不知道需求会出现森么情况，为啥不先开发后再提取接口呢

```

    type Person interface {
        Eat(s []string) int
    }

    type Student interface {
        Person
        Learn(i interface{}) int
    }

    type PersonImpl struct{}

    func (p *PersonImpl) Eat(s []string) int {
        return len(s)
    }
    func (p *PersonImpl) Fight() {
        fmt.Println("fighting!!!")
    }

    func main() {
        var p Person
        pi := &PersonImpl{} //该结构的方法接收者都是指针，所以该结构的指针才实现了 Person 接口
        // 虽然结构变量可以调用指针或非指针接收方法，但是他们是偷偷进行了隐式转换的

        p = pi // 只有实现 p 接口的结构才能赋值
        p.Eat(make([]string, 5, 5))
        //p.Fight() 报错，没有该方法

        var s Student
        p = s // 接口之间也可以相互赋值，必须满足接口
    }

```

接口的类型在编译时就被确定，但是接口实际有两个值：动态类型，动态值
```

    type Test interface {
        Tester()
    }

    type MyFloat float64

    func (m MyFloat) Tester()  {
        fmt.Println(m)
    }

    func describe(t Test)  {
        fmt.Printf("Interface type %T value %v\n",t,t)
    }

    func main() {
	var t Test
	m := MyFloat(66.6)
	t = m
	fmt.Println(t) //动态值
	t.Tester() //动态类型的方法
	describe(t) // Interface type main.MyFloat value 66.6
}

```

注意：我们在讲方法时，方法有两种调用情况，普通调用和指针调用，指针可以调用一切方法，而结构体变量不能调用指针方法，在实现接口时也是如此，如果是用指针方法实现的该接口，那么只能将指针赋值给接口，再调用，可以看做结构体变量没有这个方法

```

    type Test interface {
        Tester()
    }

    type MyFloat float64

    func (m MyFloat) Tester()  {
        fmt.Println(m)
    }

    type MyInt int

    func (this *MyInt) Tester()  {
        fmt.Println(this)
    }

    func main() {
        var t Test
        mf := MyFloat(66.6)
        t = mf
        t.Tester()

        mi := MyInt(100)
        //t = mi  报错，因为 mi 并不能调用 Tester 方法
        t = &mi
        t.Tester()
    }

```

接口的零值，这两个值都是 nil，用 nil 调用接口方法则会宕机，我们可以将接口与 nil 做 == 检查

```

    var w io.Writer
    w = os.Stdin //此时 w 的动态类型是 os.Stdin 实现的接口，他的动态值是指向 os.Stdin 的一个指针
    fmt.Printf("%T",w) // *os.File
    w.Write([]byte("kanggege")) // 调用 w 实际是调用 (*os.File).Write()
    // 但我们在编译时无法确认接口的动态类型是什么，实际还是通过动态分发，以动态值为接受者调用动态类型的方法

```

接口值可以用 == 做比较，如果两个借口值都是 nil，或二者的动态类型完全一致且动态值相等，则接口值相等，即接口值可以作为 map 的 key，或 switch 的操作数

如果接口的动态类型一致，但动态值不可比较，则这个比较会崩溃，在作比较时要小心！

一个易忽略的错误

```

    const debug = true
    var buf *bytes.Buffer
    if debug {
        buf = new(bytes.Buffer)
    }
    f(buf)


    func f (out io.Writer)  {
        if out != nil {
            out.Write([]byte("done\n"))
        }
    }

```
分析下，buf，它是一个结构体指针类型，当作为 io.Write 赋值时，它已经有了动态类型 *file.Write 但是没有动态值，即使这样，在做 nil 判断时，它也是非 nil 类型，如果 debug 为 false 时，也会调用 out.Write 但 out 的动态值为 nil，则会宕机，应该在起始时就将 buf 设置为 io.Write

## 类型断言
在说类型断言之前，我们不得不提一提 interface{} 他可以说是 java 的 object，所有的类型都直接或间接实现了它，所以我们在声明类型时可以这样写

```
    func f (i interface{})  {
        index := strings.LastIndex(i)
        fmt.Println(i)
    }
    map[interface{}]interface{}
```

但是按照第一个函数的写法会报错，因为 i 并不是 string 类型的，也就是说，我们要先确定并转换 i 的类型，才能去更好的使用它，这有点类似于 java 中类的向下转换，将编译类型转换为运行时类型，即，类型断言用于提取接口的底层值

java 中的向下转换是通过强制类型转换，而 go 中使用断言 x.(T) ，即判断或提取出他的类型再使用


使用断言的前提条件就是，x 必须是接口类型的值
1. 当 T 是具体类型时，会检查 x 的动态类型是否就是 T，如果满足则结果就是 x 的动态值和 T 类型
2. 当 T 是接口时，检查 x 的动态类型是否是 T，如果满足返回的仍是接口 T

断言的使用有两种方式
```
    i = i.(string) //如果断言失败，则会宕机，除非你确定，否则并不推荐，不过断言为 string 应该不会失败
    i,ok = i.(string) //通过 ok 判断是否成功，失败也没事
```

接口和类型断言是相辅相成的，有过 java 基础的一定会对结构不能显式标注实现了哪个接口而感到一丝不安，那刚好可以用类型断言来做一个判断，这样虽然虽然会让代码略微冗余，但是有更好的维护性和强健性，多用用就会发现，它远优于 java 的接口

- 使用断言识别错误类型：为错误类型定义好结构后，在出现错误时，可以使用断言识别不同的错误类型，并做出相应处理
- 使用断言判别是否具有特性：当无法确定结构是否实现了某个方法或接口时，可以在方法内定义接口，并使用断言判断是否可以调用

断言的另一种使用方式是提取接口的类型，使用 switch
```
    func findType(i interface{})  {
        switch i.(type) {
        case string:
            fmt.Printf("I am a string and my value is %s\n", i.(string))
        case int:
            fmt.Printf("I am an int and my value is %d\n", i.(int))
        default:
            fmt.Printf("Unknown type\n")
        }
    }

```
注意：只有在 switch 中才能使用 i.(type) 提取类型，而得到的类型也可能是多个不同的值，比如实现了两个接口的结构体
```

    type MyFloat float64

    func (m MyFloat) Tester()  {
        fmt.Println("Tester")
    }

    func (m MyFloat) Tester2()  {
        fmt.Println("Tester2")
    }

    func findType(i interface{})  {
        switch i.(type) {
        case Test:
            fmt.Println("Test")
        case Test2:
            fmt.Println("Test2")
        default:
            fmt.Printf("Unknown type\n")
        }
    }

    func main() {
        m := MyFloat(66.6)

        //更换 switch 下选项的位置，也可以打印出 Test2
        findType(m) //Test
    }
```