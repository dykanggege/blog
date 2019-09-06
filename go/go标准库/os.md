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
	Env:   os.Environ(),
	Files: []*os.File{os.Stdin, os.Stdout, os.Stderr}, //让新进程共享旧进程的stdio,
	Sys:   &syscall.SysProcAttr{}, //给新进程设置其他属性，可能不跨平台
})
errHandle(err)
//process.Signal() 给新进程发送信号
//process.Kill() 结束新进程
//process.Release() 释放新进程持有的资源
state, err := process.Wait() //等待子进程结束后获取子进程信息
errHandle(err)
code := state.ExitCode() //获取退出状态吗
fmt.Println(code)        //0 正常退出

//os包中提供的两种信号
var (
    Interrupt Signal = syscall.SIGINT //ctrl+c
    Kill      Signal = syscall.SIGKILL //kill信号，处理方式无法改写
)
```

os包是跨平台的，只能从各个平台中抽象出通用的API，甚至信号也是平台通用的，这样的代价是大大阉割了进程管理调用

几个感觉有用的API

- os.Exit(0) 退出
- Pipe() (r *File, w *File, err error) 创建管道，但是不如socket有用
- FindProcess(pid int) (*Process, error) 



# exec
为了提供更用户友好的API，exec包将StartProcess方法包装了一下，建议调用shell文件，再执行系统相关的命令，这样更简单

注意这个包假定运行在Unix平台上，并不确保window兼容

```
cmd := exec.Command("/bin/bash","-c","ls") //对StartProcess做了包装
//没有继承文件描述符，要自己设置
cmd.Stdin = os.Stdin
cmd.Stdout = os.Stdout
cmd.Stderr = os.Stderr
cmd.SysProcAttr = &syscall.SysProcAttr{} //如果还想设置其他特殊系统调用
err := cmd.Run() //等同于start() 再 wait()
errHandle(err)
```

os包中没有直接提供exec的方法，只能通过syscall，但是牺牲了跨平台性

