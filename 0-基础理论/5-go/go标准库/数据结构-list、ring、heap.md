go并没有像java一样提供很多数据结构，内置的slice和map已经能解决绝大部分问题，container下提供了其他三种结构

# list
双向链表，list包中有两个结构，List代表链表结构，Element是链表中节点持有指向链表的指针，这种设计为链表提供了更灵活的操作

我们可以创建List，而Element不能直接创建，只有放入值后才会返回Element

```
    pool := &sync.Pool{}
	pool.New = func() interface{} {
		return list.New()
	}
	l := pool.Get().(*list.List)
	l.Init() //重新初始化链表，可以用作复用

	e1 := l.PushBack(1)
	e2 := l.PushFront(2)
	l.MoveAfter(e1,e2)
	fmt.Println(l.Front().Value) // 2
	fmt.Println(l.Front().Next().Value) // 1
```

# ring
ring是一个环形链表，环是没有begin和end，所以Ring只是环中的一个元素

```
	r1 := ring.New(3) //创建具有3个元素的环
	r2 := ring.New(5)

	for i := 0; i < r1.Len(); i++{
		r1.Value = i
		r1 = r1.Next()
	}
	for i := 0; i < r2.Len(); i++{
		r2.Value = i+100
		r2 = r2.Next()
	}

	r1.Link(r2)
	r1.Do(func(v interface{}) { //遍历环
		fmt.Println(v.(int))
	})

	fmt.Println()

	r1.Unlink(r1.Len()-1)
	fmt.Println(r1.Len())

```

# heap
如果前面两种结构只是说明没有泛型全靠约定，那么heap的用法就看出来没泛型有多难受

```
type MinIntHeap []int

func (m MinIntHeap) Len() int {
	return len(m)
}

func (m MinIntHeap) Less(i, j int) bool {
	return m[i] < m[j]
}

func (m MinIntHeap) Swap(i, j int) {
	m[i],m[j] = m[j],m[i]
}

func (m *MinIntHeap) Push(x interface{}) {
	*m = append(*m,x.(int))
}

func (m *MinIntHeap) Pop() interface{} {
	l := len(*m)
	v := (*m)[l-1]
	*m = (*m)[:l-1]
	return v
}

func main() {
	h := &MinIntHeap{55,2,7}
	heap.Init(h)
	heap.Push(h,8)
	for i,l := 0,h.Len(); i < l; i++{
		v := heap.Pop(h)
		fmt.Println(v)
	}
}
```

你需要自己创建结构，实现heap.Interface接口，然后就是类c的容器调用，即使没有泛型无法实现通用对象，为啥标准库不提供几种常用类型的实现;)


