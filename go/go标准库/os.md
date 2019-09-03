os提供了跨平台的系统调用接口，接口整体设计风格Unix-like，如果想使用特殊的系统调用，去看看syscall包

# 进程

- os.Stdin、os.Stdout、os.Stderr 当前进程的标准io
- os.Args 该进程的启动命令及参数

我们先写一个程序
```
func main()  {
	fmt.Println("i am the new process")
	fmt.Println(os.Args)
	fmt.Println("finish...")
}
// go build -o nproc main.go 编译为可执行文件
```
再重新写一个
```
process, err := os.StartProcess("./nproc", []string{"yes!"}, &os.ProcAttr{
    Dir:   "./", 
    Files: []*os.File{os.Stdin, os.Stdout, os.Stderr},//让新进程共享旧进程的stdio
})
errHandle(err)
//process.Signal() 给新进程发送信号
//process.Kill() 结束新进程
//process.Release() 释放新进程持有的资源
state, err := process.Wait() //等待子进程结束后获取子进程信息
errHandle(err)
code := state.ExitCode() //获取退出状态吗
fmt.Println(code) //0 正常退出
```

golang标准库中没有直接提供fork-excv的函数，直接加载新文件创建进程，StartProcess 属于偏底层的操作，建议使用exec包中的Command函数，等同于调用shell，可以根据不同的系统有更定制化的操作

几个感觉有用的API

- os.Exit(0) 退出

# 文件操作
