# WaitGroup

golang中的同步是通过sync.WaitGroup来实现的．WaitGroup的功能：它实现了一个类似队列的结构，可以一直向队列中添加任务，当任务完成后便从队列中删除，如果队列中的任务没有完全完成，可以通过Wait()函数来出发阻塞，防止程序继续进行，直到所有的队列任务都完成为止．WaitGroup总共有三个方法：Add(delta int),　Done(),　Wait()。Add:添加或者减少等待goroutine的数量Done:相当于Add(-1)Wait:执行阻塞，直到所有的WaitGroup数量变成0
