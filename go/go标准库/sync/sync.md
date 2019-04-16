# WaitGroup

golang中的同步是通过sync.WaitGroup来实现的．WaitGroup的功能：它实现了一个类似队列的结构，可以一直向队列中添加任务，当任务完成后便从队列中删除，如果队列中的任务没有完全完成，可以通过Wait()函数来出发阻塞，防止程序继续进行，直到所有的队列任务都完成为止．WaitGroup总共有三个方法：Add(delta int),　Done(),　Wait()。Add:添加或者减少等待goroutine的数量Done:相当于Add(-1)Wait:执行阻塞，直到所有的WaitGroup数量变成0


# Pool
如果不断的创建临时相同的对象，我们就需要考虑重用，这样不但减轻了GC压力，还能增强性能，类似于Groutine pool

**Pool是多线程并发安全的**

```
    //创建一个pool的时候，需要完善New函数，返回要重用的数据类型，注意一定要返回数据的引用
    pool := sync.Pool{New: func() interface{} {
		return new(bytes.Buffer)
	}}
    //现在就可以愉快的使用了
    //如果池子是空的，他也会为你创建一个
    buffer := pool.Get().(*bytes.Buffer)
    //使用完了再把它放回去
    pool.Put(buffer)

```