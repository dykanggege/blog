远程调用含义很广，客户端使用ajax访问后台的服务，也是远程调用，他们之间默认使用http作为调用协议

- 数据传输协议:当然HTTP了
- 发现服务：使用url定位服务
- 数据编码传输格式：基于http的编码和解析

理论上机器之间都可以通过类似浏览器服务器间的调用实现远程调用，但是他们用于分布式间调用有一些弊端

- 基于http传输效率不高，能不能更直接的基于tcp
- 能不能像调用一个远程函数库一样更直接，如果是调用一个远程库，那么传入参数和返回值应该和远程库的类型保持一致

这就有了各语言间的rpc，抱着设计初衷来看go的rpc实现

```
    func main() {
        go server()
        time.Sleep(time.Second*1)
        go func() {
            for range [10]int{}{
                client()
            }
        }()

        time.Sleep(time.Second*1000)
    }

    //类和方法都必须是可导出的，你也无法访问标准库一个非导出的函数
    type User struct {
        Name string
        Age int
    }

    //调用函数有两个参数
    //第一个是调用者传进来的参数，可以是指针或非指针
    //第二个是设置返回的参数，必须是指针类型，否则服务端修改的值无法传递回去
    //返回一个可能出现的错误
    func (u *User)Set(arg *User,reply *bool) error {
        fmt.Println(*arg)
        *reply = true
        return nil
    }

    func server()  {
        //向默认的rpc服务端注册一个服务
        err := rpc.Register(new(User))
        errHandle(err)

        listener, err := net.Listen("tcp",":8899")
        errHandle(err)
        fmt.Println("开始监听服务")
        //选择服务类型，且将rpc和端口绑定
        rpc.Accept(listener)
        
        //选择使用http作为交互的协议
        //rpc.HandleHTTP()
        //listen, err := net.Listen("tcp", ":8899")
        //err = http.Serve(listen, nil)
    }

    func client()  {
        //创建一个调用client
        dial, err := rpc.Dial("tcp", ":8899")
        errHandle(err)
        
        var flag bool
        u := &User{Name:"kanggege",Age:22}
        //通过注册服务名调用
        err = dial.Call("User.Set", u, &flag)
        errHandle(err)
    }

    func errHandle(err error)  {
        if err != nil{
            log.Println(err)
        }
    }

```