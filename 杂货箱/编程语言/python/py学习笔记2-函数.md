# 函数语法
```

    def power(x):
        return x*x

    # enroll('Bob', 'M', 7)
    # enroll('Adam', 'M', city='Tianjin')
    def enroll(name, gender, age=6, city='Beijing'):
        print('name:', name)
        print('gender:', gender)
        print('age:', age)
        print('city:', city)

    # L 是一个已经被计算出的参数，  默认参数应该指向不变的对象
    def add_end(L=[]):
        L.append('END')
        return L
    
    # >>> add_end()
    # ['END', 'END']
    # >>> add_end()
    # ['END', 'END', 'END']
    # 改进如下
    def add_end(L=None):
        if L is None:
            L = []
        L.append('END')
        return L
    
    #  calc(1, 3, 5, 7) numbers会被作为一个 list 解析
    def calc(*numbers):
        sum = 0
        for n in numbers:
            sum = sum + n * n
        return sum

    def person(name,age,**kw):
        print('name:', name, 'age:', age, 'other:', kw)
    # >>> person('Bob', 35, city='Beijing')
    # name: Bob age: 35 other: {'city': 'Beijing'}
    # >>> person('Adam', 45, gender='M', job='Engineer')
    # name: Adam age: 45 other: {'gender': 'M', 'job': 'Engineer'}

    # 命名关键字参数需要一个特殊分隔符*，*后面的参数被视为命名关键字参数
    def person(name, age, *, city, job):
        print(name, age, city, job)
    # >>> person('Jack', 24, city='Beijing', job='Engineer')
    # Jack 24 Beijing Engineer

    def f1(a, b, c=0, *args, **kw):
        print('a =', a, 'b =', b, 'c =', c, 'args =', args, 'kw =', kw)

    def f2(a, b, c=0, *, d, **kw):
        print('a =', a, 'b =', b, 'c =', c, 'd =', d, 'kw =', kw)
    
    >>> f1(1, 2)
    a = 1 b = 2 c = 0 args = () kw = {}
    >>> f1(1, 2, c=3)
    a = 1 b = 2 c = 3 args = () kw = {}
    >>> f1(1, 2, 3, 'a', 'b')
    a = 1 b = 2 c = 3 args = ('a', 'b') kw = {}
    >>> f1(1, 2, 3, 'a', 'b', x=99)
    a = 1 b = 2 c = 3 args = ('a', 'b') kw = {'x': 99}
    >>> f2(1, 2, d=99, ext=None)
    a = 1 b = 2 c = 0 d = 99 kw = {'ext': None}

    >>> args = (1, 2, 3, 4)
    >>> kw = {'d': 99, 'x': '#'}
    >>> f1(*args, **kw)
    a = 1 b = 2 c = 3 args = (4,) kw = {'d': 99, 'x': '#'}
    >>> args = (1, 2, 3)
    >>> kw = {'d': 88, 'x': '#'}
    >>> f2(*args, **kw)
    a = 1 b = 2 c = 3 d = 88 kw = {'x': '#'}
```

# 函数式编程
函数名本身就是一个变量，指向一个函数，因此函数也可以作为变量使用赋值给其他变量

## map
```
    def f(x):
        return x*x

    arr = [1,2,3,4,5]
    ir = map(f,arr)
    print(list(ir))
```

map(fun,Iterable) 它将会把 fun 用在 Iterable 每一个元素上，生成一个迭代器 Iterator,Ierator 是惰性的，用 list(Iterator) 将强制生成一个 Iterable 

## reduce
map 只能传入一个参数，如果我们想传入两个参数呢
在 Python3 中，reduce() 函数已经被从全局名字空间里移除了，它现在被放置在 fucntools 模块里，如果想要使用它，则需要通过引入 functools 模块来调用 reduce() 函数
```

    from functools import reduce
    def sum(x,y):
        return  x + y

    print(reduce(sum,[1,2,3,4,5])) # return sum(sum(sum(sum(1,2),3),4),5)

```

## filter
filter()把传入的函数依次作用于每个元素，然后根据返回值是True还是False决定保留还是丢弃该元素
```
    def is_odd(v):
        return v % 2 == 0

    f = filter(is_odd,[1,2,3,4,5])
    print(list(f))
```
filter 得到的也是一个惰性的 Iterator

## sorted
key指定的函数将作用于list的每一个元素上，并根据key函数返回的结果进行排序,并按照对应关系返回list相应的元素
```

    print(sorted([-5,-88,9,4,33,84]))
    print(sorted([-5,-88,9,4,33,84],key=abs))
    print(sorted([-5,-88,9,4,33,84],key=abs,reverse=False))
    print(sorted(['bob', 'about', 'Zoo', 'Credit'],key=str.lower))
    # [-88, -5, 4, 9, 33, 84]
    # [4, -5, 9, 33, 84, -88]
    # [4, -5, 9, 33, 84, -88]
    # ['about', 'bob', 'Credit', 'Zoo']

```

## 闭包
```
    def lazy_sum(*args):
        def sum():
            n = 0
            for v in args:
                n += v
            return n

        return sum

    s = lazy_sum(1,2,3,4,5,6,7,8,9)
    print(s()) # 45 闭包与惰性求和

```

## 匿名函数
```
    f = lambda x: x*x

    def fun(x):
        return x*x

```
如上，两个函数等价

## 装饰器
所有的函数都可以通过 fun.__name__ 得到定义时的函数名，即使被赋值给其他变量，也会得到原来的函数名
```
	def log(fun):
		def warpper(*args,**kw): # 对原来的函数进行包装，类似于切面
			print('like sprint aop')
			fun(*args,**kw)
		return warpper
		
	def p(*args,**kw):
		for v in args:
			print(v)
	
	f = log(p)
	f(1,2,3)
```

我们首先创建了一个函数修饰器，它为函数的功能做了扩充，然后传入要被装饰的函数，并得到一个被装饰后的函数并调用它

使用 python 的语法糖可以简化这一过程

```
	def log(fun):
		def warpper(*args,**kw):
			print('like sprint aop')
			fun(*args,**kw)
		return warpper
		
	@log # 相当于 p = log(p)
	def p(*args,**kw):
		for v in args:
			print(v)
	
	p(1,2,3)
```

在做包装时传入参数

```

	import functools
	
	def log(b,l):
		def decorator(func):
			def warpper(*args,**kw):
				print(b,' middle ',l)
				func(*args,**kw)
			return warpper
		return decorator
	
	@log('begin','last') # 等同于 p = log('begin','last')(p)，两层修饰
	def p(*args,**kw):
		for v in args:
			print(v)
		
	print(p.__name__) # warpper 其实现在的 p 是指向 warpper 函数的，我们要修改回来，避免出问题
	p.__name__ = p
	p(1,2,3,a='a')
	
	# 修改后的函数如下
	def log(b,l):
		def decorator(func):
			@functools.wraps(func)
			def warpper(*args,**kw):
				print(b,' middle ',l)
				func(*args,**kw)
			return warpper
		return decorator

```

## 偏函数
py 提供的内置函数大多都可以传入其他参数以满足需求，如果每次调用都需要传入一个额外参数，我们可以把这参数固定下来

```
	import functools
	
	print(int('12345'))
	print(int('01011',base=2))
	
	def int2(x):
		return int(x,base=2)
	
	print(int2('001010'))
	
	int2 = functools.partial(int,base=2) # 和上面 int2 的效果相同
```

