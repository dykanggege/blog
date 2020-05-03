并发会出现各种意料之外的情况，从CPU的寄存器，到一二三级缓存，再到内存，这里面有太多的意外，我们无法猜测和预估到所有情况，但是我们可以只做正确的事情

# atomic
atomic提供了最基础的原子操作，在CPU上只用一个指令完成，所以不会被中断或重入，一般这些操作都建议使用sync或channel代替

- 从内存中原子性的加载值

    func LoadInt64(addr *int64) (val int64)

- 从内存中原子性存储值

    func StoreInt64(addr *int64, val int64)

- 原子性存储值且返回旧值

    func SwapInt64(addr *int64, new int64) (old int64)

- 原子加

    func AddInt64(addr *int64, delta int64) (new int64)

- 比较addr指向的值是否和old相同，如果相同则将new交换给旧值，返回是否交换

    func CompareAndSwapInt64(addr *int64, old, new int64) (swapped bool)


原子锁虽然更高效简洁，但过于底层，不建议使用

# sync
sync提供了基础的同步原语，一般线程间的同步尽可能的使用channel

## once
单例模式，并发安全保证只执行一次
```
var once = &sync.Once{}

func main() {
	print1 := func() {
		print(1)
	}
	for i := 0; i < 100; i++{
		go once.Do(print1)
	}
	time.Sleep(time.Second*5)
}
```
源码
```
type Once struct {
	m    Mutex
	done uint32
}
func (o *Once) Do(f func()) {
	if atomic.LoadUint32(&o.done) == 1 {
		return
	}
	// Slow-path.
	o.m.Lock()
	defer o.m.Unlock()
	if o.done == 0 { 
		defer atomic.StoreUint32(&o.done, 1)
		f()
	}
}
```

# pool
pool池用于资源重复利用，在该池子中维持的对象不会被GC回收，如果需要频繁分配和回收对象，可以使用pool，类似线程池
```
var pool sync.Pool
pool.New = func() interface{} {
    return struct {}{}
}
pool.Get()
pool.Put(struct {}{})
```


# Map
并发安全下的map操作，但是没有泛型，要不停的转换interface{}
```
sm := &sync.Map{}
sm.Store("key","val")
sm.LoadOrStore("key2","val2")
sm.Range(func(k, v interface{}) bool {
    fmt.Println(k,v)
    return true
})
sm.Delete("key")
```

# Mutex
sync下提供了锁，一种是无差别的排他锁，另一种是粒度更小的读写锁，多个读下可重入，写操作会排他
```
    rwmu := &sync.RWMutex{}
	rf := func() {
		rwmu.RLock()
		fmt.Println("read")
		time.Sleep(time.Second*1)
		rwmu.RUnlock()
	}
	for i := 0; i < 10; i++ {
		go rf()
	}
	time.Sleep(time.Millisecond*50)
	go func() {
		rwmu.Lock()
		defer rwmu.Unlock()
		fmt.Println("write")
		time.Sleep(time.Second*3)
	}()
	for i := 0; i < 10; i++ {
		go rf()
	}

	time.Sleep(time.Second * 5)
```

# WaitGroup
用于管理线程执行流
```
wg := sync.WaitGroup{}
for i := 0; i < 10; i++{
    wg.Add(1)
    go func(i int) {
        time.Sleep(time.Duration(i)*time.Second) //注意这里是个闭包
        fmt.Println(i," done")
        wg.Done()
    }(i)
}
wg.Wait()
```
