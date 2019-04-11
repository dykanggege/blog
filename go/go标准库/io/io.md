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

