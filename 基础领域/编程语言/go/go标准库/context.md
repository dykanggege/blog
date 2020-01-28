# 源码
context用于对调用的控制，具体的说可以用于线程的管理

```
type cctx context.Context //为了兼容context.Context，在1.9中加入了别名

func main() {
	blank := context.Background() //创建一个空白的ctx
	//todo := context.TODO()        //创建了空白的ctx，但是还没想好要做啥，占个坑

	blank = context.WithValue(blank, "key", "wait...") //上下文传值
    //设定死亡周期，实际等同于 	context.WithDeadline(blank,time.Now().Add(time.Second*5))
    //五秒后应该结束调用，你可以提前手动调用stop结束调用
	blank, stop := context.WithTimeout(blank, time.Second*5)
	
    go func() {
		<- time.After(time.Second * 3)
		stop()
	}()

	go Go(blank)

	time.Sleep(time.Minute)
}

func Go(ctx cctx)  {
	defer fmt.Println("done...")
	val := ctx.Value("key").(string)
	for {
		select {
		case <- ctx.Done():
			return
		case <- time.After(time.Second*1):
			fmt.Println(val)
		}
	}
}
```

1. 不要把Context放在结构体中，要以参数的方式传递
2. 以Context作为参数的函数方法，应该把Context作为第一个参数，放在第一位。
3. 给一个函数方法传递Context的时候，不要传递nil，如果不知道传递什么，就使用context.TODO
4. Context的Value相关方法应该传递必须的数据，不要什么数据都使用这个传递
5. Context是线程安全的，可以放心的在多个goroutine中传递

# 源码
```
var (
	background = new(emptyCtx)
	todo       = new(emptyCtx)
)
func Background() Context {
	return background
}

func TODO() Context {
	return todo
}

//空的Context都是emptyCtx

type emptyCtx int //实现了空接口，没用struct{},因为所有的struct{}地址是相同的
```

withCancel
```
type cancelCtx struct {
	Context

	mu       sync.Mutex            // protects following fields
	done     chan struct{}         // created lazily, closed by first cancel call
	children map[canceler]struct{} // set to nil by the first cancel call
	err      error                 // set to non-nil by the first cancel call
}

func newCancelCtx(parent Context) cancelCtx {
	return cancelCtx{Context: parent}
}

func WithCancel(parent Context) (ctx Context, cancel CancelFunc) {
	c := newCancelCtx(parent)
	propagateCancel(parent, &c)
	return &c, func() { c.cancel(true, Canceled) }
}




``