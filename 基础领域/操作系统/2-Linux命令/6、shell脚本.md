shell就是linux里批处理文件，用于写指令并一次性执行

虽然有了命令行或系统调用，但仅仅函数并不能满足很多运维需求，还要有简单的变量、逻辑处理等。类似存储过程，shell也算是一个mini编程语言

# 基本格式
- 命令太长了一行放不下可以用 \ 换行放置
- 多个命令可以用 ; 分隔放在同一行
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
用户变量
- kanggege=nb; echo $kanggege
- 等号前后不能出现空格
- $[内部用户四则运算],只支持整数运算
- 可以给值加
    

## 字符串
- "" 做特殊符号的转义和变量取值
- '' 不做转义，保留原来样子
- `` 当作命令执行取得结果，效果等同于 $()

字符串操作
- echo ${#str} 输出字符串长度
- echo ${str:1:4} 切片

## 传递参数
运行脚本时可以传递参数，使用 $n 获取，如$1、$2...

还有一些特殊参数

- $#	传递到脚本的参数个数
- $$	脚本运行的当前进程ID号
- $?	显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。
- $0    shell 本身的文件名

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

## test
if 只能验证exit为0的函数，可以用 test 验证条件

- test 直接使用则失败返回1，test;echo $?
- test $var 如果变量是空的，则失败
- 在if中使用 [] 具有和 test 一样的效果，注意方括号和内容之间要有空格
- 数字可以用 -eq、-ne(不等)、
- [ -e $var] 不存在为真、[ -n $var] 存在为真


# 退出
- exit 用于结束退出
- 0 执行成功，非0则是失败的含义
- $? 获取上一个命令执行后的退出码

# 路径问题
当使用shell执行命令时，程序接受到的是当前shell文件所在的路径

1. 程序的配置文件路径等不能依赖于调用程序的路径，而是依赖于程序本身路径，如

```
func CurrentDirectory() string {
	dir, err := filepath.Abs(filepath.Dir(os.Args[0]))
	if err != nil {
		log.Fatal(err)
	}
	return strings.Replace(dir, "\\", "/", -1)
}
```

2. shell也不能信任相对路径，而是根据shell文件本身找到绝对路径，如
```
tools_dir=$(dirname $(readlink -f "$0"))
```

3. 总的说，确定好项目结构后，程序只能依赖结构本身路径，通过文件本身找到绝对路径，脚本要进入所在目录执行，确保路径间的依赖没问题