- GO111MODULE   默认开启，使用go mod模式
- GOARCH        指定编译的指令集架构
- GOBIN         go install 安装的二进制放置的位置，建议把GOBIN加入环境变量
- GOCACHE       golang是增量编译，用于缓存编译信息
- GOENV         配置go环境变量的文件
- GOEXE         编译后执行文件的后缀
- GOFLAGS       是个"-flag=value"的命令行参数列表，当执行go命令时，默认会添加这个环境变量里设置的参数
- GOHOSTOS      编译操作系统
- GOINSECURE    添加 -insecure 参数即表示更新依赖时可以不去校验CA证书，但是这会带来一个问题：范围无法界定(overkill)，所有与要更新依赖相关联的依赖，均不会去做校验，可能会意外更新到不安全的依赖，-insecure 仅支持 go get 命令，不支持 go mod 命令，因此使用 go mod 命令时是无法更新不支持https协议的私有库的
- GOMODCACHE    依赖缓存的地方
- GONOPROXY     配置获取依赖时不使用代理的地址
- GONOSUMDB     配置获取依赖时不校验md5值的地址
- GOOS          编译目标环境
- GOPATH        你懂的
- GOPRIVATE     私有仓库，mod时不要求https
- GOPROXY       加载依赖时走这个代理
- GOROOT        go源码、文档、工具存放的地址
- GOSUMDB       用于校验md5值的地址
- GOTMPDIR      run时产生临时文件地址
- GOTOOLDIR     go tool地址
- GOVCS         支持从非git、svn版本管理工具拉依赖
- GCCGO         表示"go build -compiler=gccgo"，编译时指定的-compiler参数
- AR            打包工具
- CC            编译c的工具
- CGO_ENABLED   是否使用cgo

# GODEBUG
这个变量用于runtime调试，很多参数，要详细讲下

## inittrace
GODEBUG=inittrace=1 go run main.go 

用于init方法的分析排查