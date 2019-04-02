go1.11中引入了mod作为管理工具，它通过一个环境变量GO111MODULE设置，其默认值是auto

- on：不使用GOPATH，只使用go mod
- auto：如果项目里有go.mod文件，则使用go mod，默认值
- off：不使用go mod

如果我们在GOPATH下的项目，且没有修改GO111MODULE，需要自己创建go.mod

或是 export GO111MODULE=on 暂时开启

go.mod文件还可以指定要替换和排除的版本，命令行会自动根据go.mod文件来维护需求声明中的版本。如果想获取更多的有关go.mod文件的介绍，可以使用命令go help go.mod。
go.mod文件用//注释，而不用/**/。文件的每行都有一条指令，由一个动作加上参数组成。例如：

    module my/thing
    require other/thing 	v1.0.2
    require new/thing 		v2.3.4
    exclude old/thing 		v1.2.3
    replace bad/thing 		v1.4.5 	=> good/thing v1.4.5

上面三个动词require、exclude、replace分别表示：项目需要的依赖包及版本、排除某些包的特别版本、取代当前项目中的某些依赖包

- go mod init modulename

初始化一个模块，会自动创建go.mod文件

- go mod download

    go mod download [-dir] [-json] [modules]

下载指定的模块，模块的格式可以根据主模块依赖的形式或者path@version形式指定。如果没有指定参数，此命令会将主模块下的所有依赖下载下来，-json参数会以JSON的格式打印下载的模块对象

将来用于代替 go get命令

- go mod tidy

会下载缺失的模块以及移除不需要的模块，执行后会生成 go.sum 文件（模块下载条目），添加参数-v，例如go mod tidy -v可以将执行的信息，即移除的模块打印到标准输出。

- go mod vendor

用法：go mod vendor [-v]，此命令会将build阶段需要的所有依赖包放到主模块所在的vendor目录中，并且测试所有主模块的包。同理go mod vendor -v会将添加到vendor中的模块打印到标准输出。

- go mod verify

用法：go mod verify。此命令会检查当前模块的依赖是否已经存储在本地下载的源代码缓存中，以及检查自从下载下来是否有修改。如果所有的模块都没有修改，那么会打印all modules verified，否则会打印变化的内容