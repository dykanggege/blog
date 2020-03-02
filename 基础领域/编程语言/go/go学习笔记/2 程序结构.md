# 包和文件
- 一个目录下的所有文件必须声明同一个包名
- 目录下的目录可以使用其他的包
- 通过目录路径导入目录，但实际使用仍旧以目录下命名的包名使用
- 最佳实践：目录下的包名和目录名保持一致
- 可能会有不同导入路径，但包名却相同的包，可以为包绑定别名
- 导入路径都是以$GOROOT/bin或$GOPATH/src下查找路径

```
  _ "strconv"
  f "fmt"
```

包的初始化从包级别的变量开始，在依赖解析之后按照依赖的顺序进行初始化

任何文件都可以包含任意多个 init 初始化函数，当程序启动时，初始化顺序如下
- 初始化 import 的包，无论该包是否被使用
- 查找依赖关系，初始化包内全局变量和 init 函数

![](img/1.png)

在main函数执行前，所有的代码都运行在同一个goroutine，如果init中用go启动新的goroutine，只有进入main后才可能被执行到

这里有一些包的使用规范

- 当import多个包时，应该对包进行分组。同一组的包之间不需要有空行，不同组之间的包需要一个空行。标准库的包应该放在第一组

```
import (
    "fmt"
    "hash/adler32"
    "os"

    "appengine/foo"
    "appengine/user"

    "code.google.com/p/x/y"
    "github.com/foo/bar"
)
```

# 注释
godoc是文档工具，我们在代码中写的注释会被显示在文档里，这里有一些注释规范

- 每一个包都应该有包注释，位于文件的顶部，在包名出现之前。
- 如果一个包有多个文件，包注释只需要出现在一个文件的顶部即可。
- 包注释建议使用C注释风格，如果这个包特别简单，需要的注释很少，也可以选择使用C++注释风格。
- 每个public函数都应该有注释，注释以函数名开头，这样便于查找


# 流程控制
## if
```

    //花括号必须和 if 或 else 一行
    if 1 == 1 {
        ...
    } else if 2 == 2 {
        ...
    } else {
        ...
    }

    //在有返回值的函数中，不允许将最终的 return 写在 if else 中
    if 1 == 1 {
        return ...
    } else {
        return ... //这样是编译失败的，必须有最终的 return
    }
    return ...

```

## switch
```
    // s 支持任何类型，在一个条件下可以执行多个语句，且默认执行break，加上 fallthrough 后，会继续执行下一句
    s := "kanggege"
	switch s {
	case "kanggege":
		fmt.Println("yes")
		fmt.Println("yes!")
        fallthrough
	case "luci":
		fmt.Println("no!")
    case "shala"
        fmt.Println("yyyyyes")
	}

    //更牛逼的是 switch 还可以没有条件，等同于一群 if else
    switch {
	case 1 > 2:
		fmt.Println("1>2")
	case 2 > 1:
		fmt.Println("2>1")

	}

```

## for
```

    Loop:
	for i := 0; i<10 ; i++ {
		Loop2:
		for j :=0; j<10;j++  {
			if j == 2 {
				break Loop
			}
			if j == 3 {
				break Loop2
			}
		}
	}

    for {
        .... //等同于 while(true)
    }

```

## goto
```
    //令人惊讶的是 go 竟然也支持 goto
    for i := 0; i<10 ; i++ {
		for j :=0; j<10;j++  {
			if j == 2 {
				goto Loop
                //最好小范围的使用 goto，不要随便乱跳
			}
		}
	}
	Loop:

```