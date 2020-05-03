依赖管理的目的是限制项目使用的包，保证其安全性和可用性，附加功能是便于其他使用者加载依赖

# 引入包
在原来的$GOPATH设定中，包被缓存在src目录下，和项目在一起，且无法指定版本

项目中以相对路径引入包，而相对的是 $GOPATH/src，一旦项目目录改变，就会对项目包的导入路径有影响，很难受

官方建议使用v文件夹隔离版本，但对于其他用户加载依赖很不舒服，依赖更改时需要该引入路径等等不便

go在1.5版本引入了vendor属性(默认关闭，需要设置go环境变量GO15VENDOREXPERIMENT=1)，并在1.6版本中默认开启了vendor属性。

简单来说，vendor属性就是让go编译时，优先从项目源码树根目录下的vendor目录查找代码(可以理解为切了一次GOPATH)，如果vendor中有，则不再去GOPATH中去查找，但仍不能解决版本依赖问题

借助于vendor的功能，陆续出现了dep等包管理工具，但是在1.112中，将vendor功能禁了

# GO111MODULE
go1.11中引入了mod作为管理工具，它通过一个环境变量GO111MODULE设置，其默认值是auto

- on：不使用GOPATH，只使用go mod
- auto：如果项目里有go.mod文件，则使用go mod，默认值
- off：不使用go mod

如果我们在GOPATH下的项目，且没有修改GO111MODULE，需要自己创建go.mod

或是 export GO111MODULE=on 暂时开启

建议不要再把项目放在$GOPATH/src下，以后$GOPATH也会被逐渐弃用

# 依赖配置
- go mod init tmod //包名最好和文件夹名相同，包名是作为给其他项目引入的名字

在$GOPATH/src外创建一个文件夹，初始化一个模块，会自动创建go.mod文件用来管理项目依赖配置

包名为 tmod,则项目内部的包就是以 tmod/ 为前缀的

独立项目中无法引用到 $GOPATH/src 下的包，可以使用go get github/...@版本(或者不加版本号默认使用最新版) 加载依赖到项目中，或者手动写入(不建议)

    go get github.com/gorilla/mux           #匹配最新的一个tag
    go get github.com/gorilla/mux@latest    #跟上面一样
    go get github.com/gorilla/mux@master    #匹配master分支
    go get github.com/gorilla/mux@v1.6.2    #匹配v1.6.2
    go get github.com/gorilla/mux@c856192   #匹配commit的SHA-1码的版本

此时的get不会将依赖加载到 $GOPATH/src 下，而是将包信息配置在 go.mod 中，将下载信息放在 go.sum 中，并且将包按版本号缓存在 $GOPAH/pkg/mod 中

- go mod vendor

goland如果开启vgo模式，在这种情况下无法生成代码提示，需要 go mod vendor 加载依赖包到当前目录下后，才有代码提示

加载依赖时最容易碰到的问题就是 golang.org 下包加载问题，可以设置环境变量 GOPROXY=https://goproxy.io 官方代理

- go list -m all 查看项目的所有依赖

## 给已有项目加载依赖
将项目依赖的包手动写到 go.mod 中，编译、运行项目或 go mod vendor，

## 依赖

    module my/thing
    require (
        other/thing 	v1.0.2
        new/thing 		v2.3.4
    )
    exclude old/thing 		latest
    replace bad/thing 		v1.4.5 	=> good/thing v1.4.5


require指依赖某个包的版本，replace将对某个包的依赖更换为另一个包，exclude排除某些包的特别版本

## 版本控制
go.mod文件还可以指定要替换和排除的版本，命令行会自动根据go.mod文件来维护需求声明中的版本。如果想获取更多的有关go.mod文件的介绍，可以使用命令go help go.mod。

go.mod文件用//注释，而不用/**/。文件的每行都有一条指令，由一个动作加上参数组成。例如：


# 缓存依赖
- go mod download

    go mod download [-dir] [-json] [modules]

下载指定的模块，模块的格式可以根据主模块依赖的形式或者path@version形式指定。如果没有指定参数，此命令会将主模块下的所有依赖下载下来，-json参数会以JSON的格式打印下载的模块对象

将来用于代替 go get命令

- go mod tidy

会下载缺失的模块以及移除不需要的模块，执行后会生成 go.sum 文件（模块下载条目），添加参数-v，例如go mod tidy -v可以将执行的信息，即移除的模块打印到标准输出。

- go mod vendor

用法：go mod vendor [-v]，此命令会将build阶段需要的所有依赖包放到主模块所在的vendor目录中，并且测试所有主模块的包。同理go mod vendor -v会将添加到vendor中的模块打印到标准输出。

使用mod后，包依赖会被本机缓存到$GOPATH/src/pkg/mod中，go build默认会使用本机缓存，我们可以使用 go build -mod=vendor 更改编译依赖倒vendor文件夹下

- go mod verify

用法：go mod verify。此命令会检查当前模块的依赖是否已经存储在本地下载的源代码缓存中，以及检查自从下载下来是否有修改。如果所有的模块都没有修改，那么会打印all modules verified，否则会打印变化的内容

