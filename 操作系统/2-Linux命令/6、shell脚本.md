shell就是linux里批处理文件，用于写指令并一次性执行

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
- 用 $var 或 ${var} 获取变量值

## 字符串
字符串可以用 ' 或 " 限定

- ' 不做任何转义，不接受转义符，所以里面不能再出现 '
- " 会对变量或转义符做解释

字符串操作

- echo ${#str} 输出字符串长度
- echo ${str:1:4} 切片

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


