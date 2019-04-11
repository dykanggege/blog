## 匿名管道
管道也是一个io接口，可以用最基本的io方法
```
    //command用于输入一条命令
	cmd0 := exec.Command("ps", "-e")
	//该命令的输出管道 | 
	stdoutPipe, err := cmd0.StdoutPipe()
	errHandle(err)
	
	//执行命令
	errHandle(cmd0.Start())

	bf := make([]byte,20)
	for {
		//从管道中读取
		n, err := stdoutPipe.Read(bf)
		if err != nil{
			if err == io.EOF{
				break
			}else{
				log.Println(err)
			}
		}
		if n > 0 {
			fmt.Print(string(bf))
		}
	}

```

好的用法应该是两个管道之间的通讯
```
	cmd0 := exec.Command("ps", "-e")
	cmd1 := exec.Command("grep","go")
    //输出只能被设置一次，如果用了StdoutPipe，两次重复设置就会报错
	cmd1.Stdout = os.Stdout

	stdoutPipe, err := cmd0.StdoutPipe()
	errHandle(err)
	stdinPipe, err := cmd1.StdinPipe()
	errHandle(err)

	//必须启动命令后才会有输出
	errHandle(cmd0.Start())
	errHandle(cmd1.Start())

	bufio.NewReader(stdoutPipe).WriteTo(stdinPipe)
```

## 命名管道
命名管道在linux中会以文件的形式存在于文件系统中，任何进程都可以通过命名管道交换数据，它的操作和文件类似

	mkfifo fname 创建命名管道

	mkfifo -m 664 tfifo
	tee fname.log < tfifo &
	ls > tfifo

命名管道可以用于多个进程之间，但是如果多个进程监听输出，而只有一个进程输入，那么只有一个进程获得输出，类似于channel

并且命名管道是阻塞的，它要求发送和接受在不同的线程下，和无缓存的channel一样

```
    r, w, err := os.Pipe()
	errHandle(err)
	for i := range [10][0]int{}{
		go func(i int) {
			w.Write([]byte("我是"+strconv.Itoa(i)+"号线程，bala "))
			i++
		}(i)
	}

	for range [10][0]int{}{
		go func() {
			b := make([]byte,20)
			r.Read(b)
			fmt.Println(string(b))
		}()
	}

	time.Sleep(time.Second*50)
```

多个线程同事操作输入，就会出现并发写的情况，可能一句没写完整就被其他线程抢了，所以在io包下提供了并发安全的命名管道，也是使用 io.Pipe

而且io.Pipe不是基于文件的，直接使用内存区域，不需要把数据来回复制，也提高了性能

