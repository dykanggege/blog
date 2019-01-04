# 数据类型
## 基本类型
### 整形
- 赋值为啥种类型就是啥类型，py 中整形没有大小限制，浮点型超出限制就是 inf（无限大）
- 无论是两个整形还是浮点型，使用 / 都得到浮点型结果
- // 等同于 %，但整形使用 // 得到整形结果，浮点型使用 // 得到整数的浮点型结果

### bool
- True 或 False
- 布尔值可以使用 and or not 运算

### 字符串
- 使用 “” 或 '' 创建字符串
- 使用 + 可以拼接字符串，但是字符串和非字符串之间不能拼接，需要使用 str(val) 进行类型转换
- 可以在字符串中使用转义字符，会被解析
- val = input('alert提示') 用于获取用户输入，获取的是字符串、int() 可以将字符串转为 int 类型,还有 bool()、float()
- 使用 '''val''' 可以输入多行字符串，换行符会被保留
- ord('val') 获取单个字符的 Unicode 十进制编码，chr() 将十进制编码转为字符，使用 "str".encode('ascii') 返回字符串按照指定编码方式的十六进制代码，'str'.decode('utf-8') 解码
- len() 获取字符串 **字符** 数
- 由于Python源代码也是一个文本文件，所以，当你的源代码中包含中文的时候，在保存源代码时，就需要务必指定保存为UTF-8编码。当Python解释器读取源代码时，为了让它按UTF-8编码读取，我们通常在文件开头写上这两行

```
    #!/usr/bin/env python3  
    # -*- coding: utf-8 -*-
    # 第一行注释是为了告诉Linux/OS X系统，这是一个Python可执行程序，Windows系统会忽略这个注释
    # 二行注释是为了告诉Python解释器，按照UTF-8编码读取源代码，否则，你在源代码中写的中文输出可能会有乱码
```
- print('%d5.2',v) 输出格式化方式与 c 相同

### 空值
- None

## list
- 创建列表 arr = ["a",2,6.66] 可以是多数据类型混合，类数组操作，引用类型
- 增删改查，类似于 arr.fun() 格式的函数，都是对原数组进行的操作
    - arr.apend(val) 在数组末尾追加数据
    - arr.insert(index,value) 在 index 处插入 val，被插入元素后的元素都会向右移动，如果 index 大于数组长度，会将 val 添加在末尾
    - del arr[index] 删除元素，其后元素向左移动
    - arr.pop() 弹出顶元素，并删除，arr.pop(index) 弹出并删除第 index 个元素，其后元素向左移动
    - arr.remove(val) 删除 val 元素，而后元素会左移，如果 val 不存在，会报错，它和 pop 的区别是，remove 不会返回被删除的元素值，pop 会
    - v in arr / v not in arr 可以查询 v 是否在 arr 数组中，返回 True/False
- 排序
    - arr.sort() 在原数组上排序， arr.sort(reverse=True) 反向排序，只有数组里都是同类型元素才能进行排序
    - sorted(arr) 返回排序后的数组，并不改变数组本身
- 其他
    - arr.reverse() 翻转元素
    - len(arr) 获取长度
    - 如果数组长度不为 0 ，都可以通过 arr[-1] 访问最后一个元素，类似 arr[-2] 访问倒数第二个元素
- 遍历
    - for v in arr:
    - range()函数，可以生成一个整数序列，再通过list()函数可以转换为list，for v in list(range())
    - 列表解析 arr = [value**2 for value in range(1,11)]、**2 是求平方
    - from collections import Iterable，通过 isinstance(arr, Iterable) 可以判断是否是可迭代的对象

```
    from collections import Iterable

    arr = list(range(100))

    # enumerate 使得 list 变成一个索引对
    if isinstance(arr, Iterable):
        for i,v in enumerate(arr): 
            print('i:',i,' ','v',v)
```

- 数字数组处理
    - min(arr)、max(arr)、sum(arr) 如果数组里有非数字元素，则会出错
- 切片
    - arr[:] py的切片不是引用类型的，而是进行了复制
- 元组
    - ct = (1,2) 常量数组，虽然不能修改其中的元素，但是可以重新给元祖赋值，真扯淡


## dict
- 键值对而已 map = {"":"",} 引用类型
- 操作
    - del map[index] 删除字典元素，map.pop(key) 删除并弹出
    - 如果访问不存在的 key 会报错，使用 key in map 会返回 key 是否存在，也可以使用 map.get(key)，如果不存在会返回 None，或指定值 map.get(key,default)
    - for i,v in map.items(): 遍历字典键值对，for i in map.keys() 便利字典的键，for v in map.values()遍历字典的值，
    - for i in sorted(map.keys())遍历排序后的，注意只有同一种类型才能排序，for v in set(map.values()) 正如你想的 set 那样，独一无二

## set
- set(list) 必须传入一个 list 用户创建 set，会自动过滤相同元素
- s.add(v) 添加元素 s.remove(key) 移除元素

## 列表生成式
```

    arr1 = [x*x for x in range(1,11)]
    arr2 = [x*x for x in range(1,11) if x%2==0]
    arr3 = [n+m for n in 'abc' for m in 'qwe']
    print(arr1) #[1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
    print(arr2) #[4, 16, 36, 64, 100]
    print(arr3) #['aq', 'aw', 'ae', 'bq', 'bw', 'be', 'cq', 'cw', 'ce']

```

上述的生成器会在一次性生成一个 list，又是并不想一次性生成那么多，而是随情况而生成，可以使用生成器，只需要把 [] 换成 ()

```

    arr1 = (x*x for x in range(1,11))
    n = 10
    while n:
        # 每调用一次 next() 就生成一次
        print(next(arr1))
        n+=1

```

列表生成式的语法虽然简洁，但是不适合于复杂情况，如果碰到复杂的处理，最好还是用函数
```
    def fib(max):
	    n,a,b = 0,0,1
	    while n<max:
	        a,b = b, a+b
	        n+=1
	        print(b)
	        yield b # 每一次调用到这里都会停止，并且返回数据
	    
	    f = fib(100)
	    i=0
	    while i<10:
	        next(f)
	        i+=1 

```


## 迭代类型
一类是集合数据类型，如list、tuple、dict、set、str等
```
    s = '康搁搁' # 按照字符迭代
    for i in s:
        print(i)
```

一类是generator，包括生成器和带yield的generator function。

这些可以直接作用于for循环的对象统称为可迭代对象：Iterable。

可以使用isinstance()判断一个对象是否是Iterable对象：
```

    >>> from collections import Iterable
    >>> isinstance([], Iterable)
    True
    >>> isinstance({}, Iterable)
    True
    >>> isinstance('abc', Iterable)
    True
    >>> isinstance((x for x in range(10)), Iterable)
    True
    >>> isinstance(100, Iterable)
    False

```

# 流程语句
## if

```
    age = 20
    if age >= 6:
        print('teenager')
    elif age >= 18:
        print('adult')
    else:
        print('kid')
    //通过缩进和换行控制 if 的范围

    if x:
        print('True')
    //只要x是非零数值、非空字符串、非空list等，就判断为True，否则为False。
```

## while
    while con:
        可以用 break,continue

## for
for 主要用户迭代
