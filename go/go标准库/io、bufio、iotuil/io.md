# Reader、Writer
	type Writer interface {
		Write(p []byte) (n int, err error)
	}

UNIX哲学是一切皆文件，go也希望尽可能的把所有都看做文件，而Reader、Writer是IO操作的基本接口，也是文件操作的基石

	type Reader interface {
		Read(p []byte) (n int, err error)
	}

read方法会读取len(p)个字节到p中，但它并不等待读满，而是读取可用字节并返回真正读取的字节数n(0<=n<=len(p))，如果读取发生错误时会返回错误，如果读取到结束，会返回最后读取的字节和EOF错误

正确读取数据的姿势

	listen, err := net.Listen("tcp", "")
	if err != nil {panic(err)}
	for {
		conn, err := listen.Accept()
		data := make([]byte,0)
		buf := make([]byte,20)
		for {
			n, err := conn.Read(buf)
			data = append(data,buf[:n]...)
			if err == io.EOF { break }
			if err != nil {
				log.Error(err)
			}
		}
	}

或者用ioutil

	bytes, err := ioutil.ReadAll(conn)



Write writes len(p) bytes from p to the underlying data stream. It returns the number of bytes written from p (0 <= n <= len(p)) and any error encountered that caused the write to stop early. Write must return a non-nil error if it returns n < len(p). Write must not modify the slice data, even temporarily.


go中的io，在系统层面使用了非阻塞调用，如果一个线程发出 read/write 操作，而内核未准备好，返回一个错误，则调度器会切换线程。

读写的系统调用都是非阻塞的，而线程切换代价极小，使得调度器可以随意切换，达到了异步非阻塞的效果

```
    //异步非阻塞的调度模型
    goroutines := make([]int,100)
	for {
		for _,g := range goroutines{
			read:
				if g.read == false{
					g.sleep() //进入休眠，调度下一个goroutine
					goto read //从休眠中醒来接着read
				}
			//继续执行操作
		}
	}
```

