make解决的是查找依赖，执行命令

    <target> : <prerequisites> 
    [tab]  <commands>

- make 默认使用当前目录下的 Makefile，或 make -f file
- 如果Make命令运行时没有指定目标，默认会执行Makefile文件的第一个目标。
- target是必须的，prerequisites 和 commands 非必须，但至少存在一个
- 只要有一个前置文件不存在，或者有过更新，target就需要重新构建

# target
target 可以是多个文件或操作的名字(伪目标)

注意，如果当前目录下有和target相同文件，则构建失败，为了避免这种情况，可以明确声明clean是"伪目标"

    .PHONY: run
    run:
        go run main.go

# prerequisites

    all: manager

    manager:
        echo 'build manager'

构建all依赖mmanager，如果当前路径不存在该文件，会在文件内查找构建，有函数调用内味了

# commands
多行command会在不同的进程内执行，如果依赖临时环境变量，用 \ 在单进程内执行

# 语法
- \# 注释
- 编译时会打印每个commands，即回声，@commands 可避免回声
- var = val $(var)，使用shell变量需要 $$var
- VARIABLE = value 在执行时扩展，允许递归扩展。
- VARIABLE := value 在定义时扩展。
- VARIABLE ?= value 只有在该变量为空时才设置值。
- VARIABLE += value 将值追加到变量的尾端。