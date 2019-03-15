go默认使用github作为包管理工具，这样是为了鼓励开源，但对于生产版本来说，使用不确定的依赖则是一个灾难

# 关于Godep
发现好多golang项目都使用到godep作为包管理的工具，像比较大型的项目，比如kubernetes这种，都是使用的是godep来进行依赖管理操作的了，看了一下有点像maven的感觉

# 安装：
go get github.com/tools/godep
# 编译和运行
项目用godep管理后，要编译和运行项目的时候再用go run和go build显然就不行了，因为go命令是直接到GOPATH目录下去找第三方库。 而使用godep下载的依赖库放到Godeps/workspace目录下的；

godep go run main.go
godep go build
godep go install
godep go test
godep中的go命令，就是将原先的go命令加了一层壳，执行godep go的时候，会将当前项目的workspace目录加入GOPATH变量中；

# godep save
godep save将项目中使用到的第三方库复制到项目的Godeps目录下。

godep save 会自动扫描当前目录所属包中import的所有外部依赖库（非系统库），并查看其是否属于某个代码管理工具（比如git，hg）。若是，则把此库获取路径和当前对应的revision（commit id）记录到当前目录Godeps下的Godeps.json，同时，把不含代码管理信息（如.git目录）的代码拷贝到Godeps/_workspace/src下，用于后继godep go build等命令执行时固定查找依赖包的路径。

因此，godep save能否成功执行需要有两个要素： 当前或者需扫描的包均能够编译成功：因此所有依赖包事先都应该已经或go get或手工操作保存到当前GOPATH路径下 依赖包必须使用了某个代码管理工具（如git，hg）：这是因为godep需要记录revision

# godep restore
如果下载的项目中只有Godeps.json文件，而没有包含第三库则可以使用godep restore这个命令将所有的依赖库下来下来到GOPATH的src中。

godep restore
godep restore执行时，godep会按照Godeps/Godeps.json内列表，依次执行go get -d -v 来下载对应依赖包到GOPATH路径下，因此，如果某个原先的依赖包保存路径（GOPATH下的相对路径）与下载url路径不一致，比如kuberbetes在github上路径是github.com/kubernetes，而代码内import则是k8s.io，则会导致无法下载成功，也就是说godep restore不成功。这种只能手动，比如手动创建$GOPATH/k8s.io目录，然后git clone。

# golang自带包管理工具
自带工具：go get go get可以将依赖的第三方库下载本GOPATH目录，在代码中直接import相应的代码库就可以了。 与godep相比，如果项目引用的第三方库没有列入到项目里面，安装项目时，针对第三方库需要使用go get一个个下载，比较麻烦；

注：使用godep restore可能导致部分库无法下载下来；编译会报错： cmd/decode.go:16:2: cannot find package "github.com/CodisLabs/redis-port/pkg/libs/atomic2" in any of:

此时针对报错的特定库再go get一般都能下载： go get github.com/CodisLabs/redis-port/pkg/libs/atomic2

# godep支持的命令
复制代码
save     list and copy dependencies into Godeps
go       run the go tool with saved dependencies
get      download and install packages with specified dependencies
path     print GOPATH for dependency code
restore  check out listed dependency versions in GOPATH
update   update selected packages or the go version
diff     shows the diff between current and previously saved set of dependencies
version  show version info
复制代码


执行godep save,此时会生成Godeps文件夹，同时将引用的放入vendor文件夹中。