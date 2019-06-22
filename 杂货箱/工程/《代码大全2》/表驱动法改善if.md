在业务开发中，经常会碰到以下几种if的使用情况，我们分别作介绍

# if else if

```
if ... {

}else if ... {

}else if ... {

}else{

}

或

switch ...
    case ...
        处理
    case ...
        处理

```

针对大量 if else 情况，就是在众多条件中，选择首先满足的条件执行

我们尽量把条件，以及满足条件后要执行的代码，封装为一个类，用类的 test() 方法，返回一个布尔值，判断 if 条件是否满足，用 act() 做满足条件后执行的代码，然后将所有条件类的实例放在一个数组里，通过遍历数组来遍历条件

这样第一使得每个条件更直观已读，且条件后的处理也做了很好的封装，最重要的是可维护性好

```
type demoCase interface {
	test() bool
	act()
}

cases := []demoCase{new(case1),new(case2)}

for _,c := range cases {
    if c.test(h) {
        c.act(h)
        break
    }
}
```

如果是简单的 switch，可以选择用工厂模式，或通过 map 映射到要执行的操作

# if,if,if
```
if ... {

}
if ... {

}
if ... {

}
```
这种情况和上面的类似，只不过遍历数组时不需要加 break 而已

如果存在在某个 if　满足后，需要 return 即结束流程，可以选择给 act 加一个布尔返回值，判断是否终止流程，或者再写一个 stop() 方法做判断


