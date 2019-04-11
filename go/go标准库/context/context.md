# 介绍
context包用于线程的管理，通常是一个父线程创建子线程时传入context，用于管理后代(一般是那种一直在后台跑的线程)

在没有context时，如果我们在创建了一个监控线程，想要主动去杀死这个线程，主要靠 select + channel

```
    done := make(chan struct{})
	go func() {
		for{
			select {
			case <-done:
				fmt.Println("运行结束，清理线程")
				return
			default:
				fmt.Println("working...")
			}
		}
	}()
```

当我们创建一个守护线程时，还要保留管理它的channel，当创建的线程太多，甚至子线程再创建，守护线程变得庞大，管理也变得复杂起来

使用context就可以让管理变得简单起来

```
    //context.Background() 创建一个空的context
    //context.WithCancel() 将一个空的context添加cancel功能
    ctx, cancel := context.WithCancel(context.Background())
	go func(ctx context.Context) {
		go func(ctx context.Context) {
			for {
				select {
				case <-ctx.Done():
					fmt.Println("*******我也完事了")
					return
				default:
					fmt.Println("我也运行一下")
					time.Sleep(time.Second*2)
				}
			}
		}(ctx)
		for {
			select {
			case <- ctx.Done():
				fmt.Println("********我完事了")
				return
			default:
				fmt.Println("运行一下")
				time.Sleep(time.Second*2)
			}
		}
	}(ctx)
	time.Sleep(time.Second*5)
	
    //调用cancel函数，所有使用ctx.Done()的线程都退出了
	cancel()
	
	time.Sleep(time.Second*5)
	fmt.Println("******看看谁是最持久的")

```

先看一看Context的接口

```
    type Context interface {
        Deadline() (deadline time.Time, ok bool)
        Done() <-chan struct{}
        Err() error
        Value(key interface{}) interface{}
    }
```

context包为我们提供了这个接口的两个实现

    context.Background()
	context.TODO()

他们都来自一个空的实现

    background = new(emptyCtx)
	todo       = new(emptyCtx)

backgroun代表一个没有实现任何功能的父context，todo则是还没想要做什么，先传入再说

有了根context，就可以调用其他的函数衍生更多的子context

# 使用原则
- 不要把Context放在结构体中，要以参数的方式传递
- 以Context作为参数的函数方法，应该把Context作为第一个参数，放在第一位。
- 给一个函数方法传递Context的时候，不要传递nil，如果不知道传递什么，就使用context.TODO
- Context的Value相关方法应该传递必须的数据，不要什么数据都使用这个传递
- Context是线程安全的，可以放心的在多个goroutine中传递


```
type cancelCtx struct {
	Context

	mu       sync.Mutex            // protects following fields
	done     chan struct{}         // created lazily, closed by first cancel call
	children map[canceler]struct{} // set to nil by the first cancel call
	err      error                 // set to non-nil by the first cancel call
}
```