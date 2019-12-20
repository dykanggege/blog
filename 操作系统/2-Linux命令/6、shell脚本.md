shell就是linux里批处理文件，用于写指令并一次性执行

虽然有了命令行或系统调用，但仅仅函数并不能满足很多运维需求，还要有简单的变量、逻辑处理等。类似存储过程，shell也算是一个mini编程语言

# 基本格式
- 命令太长了一行放不下可以用 \ 换行放置
- #注释

    #!/bin/bash
    PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
    export PATH
    echo -e "Hello World! \a \n"
    exit 0

- 第一行告诉bash用哪个命令执行脚本，一般会默认/bin/bash
- 如果是python会看到 #!/bin/python

可以直接使用 ./test.sh 执行脚本(如果有执行权限的话)，或者使用 sh ./test.sh


# 变量

    var=value

- 直接声明变量、可重复声明
- =两侧不能有空格
- 由于命令和变量无法明显的区分，所以以 $var 或 ${var} 区分使用变量

## 字符串
字符串可以用 ' 或 " 限定

- ' 不做任何转义，不接受转义符，所以里面不能再出现 '
- " 会对变量或转义符做解释

字符串操作

- echo ${#str} 输出字符串长度
- echo ${str:1:4} 切片

# 函数调用
变量用于缓存，逻辑控制流程，函数才是shell中最重要的处理单元，主要是命令行函数

```
cpath = `pwd` //``用于调用命令行获取返回值
```

## 传递参数
运行脚本时可以传递参数，使用 $n 获取，如$1、$2...

还有一些特殊参数

- $#	传递到脚本的参数个数
- $$	脚本运行的当前进程ID号
- $?	显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。

## 常用函数
- echo 输出
- printf 格式化输出
- test 用于高逻辑判断，主要用于 if 或 for 中

# 流程控制

    if cond1
    then
        command
    elseif cond2
        command
    else
        command
    fi

    if cond; then command; fi

    for (( i=1; i < 10; i++))
    do
        command
    done


