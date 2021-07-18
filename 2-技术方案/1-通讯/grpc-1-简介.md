# rpc
rpc 远程服务调用，本质和http协议类似，封装格式传输数据并反射调用，单语言即函数或库调用，而多语言中首选要解决传参和接收值类型一致问题，最简单的做法是将多种语言抽象出公共类型模板配置文件，使用配置文件作为类型的载体

rpc本质就是一个网络协议，主要分两大块

1. 网络数据传输协议，即数据编码解码方式，下面要讲的protobuf就是一种
2. 服务接口协议，将不同语言的可调用接口，和请求、响应数据类型封装跨语言的格式

rpc编程里有三个身份

- 接口制定者：制定了向外暴露接口的规则，无论细节怎么更改，客户端都会遵循这个接口调用
- 实现服务：根据接口实现具体细节
- 调用服务：根据接口调用服务

将多语言抽离出公共类型格式并不难，但为了性能，要保证该配置文件编码解码简单，压缩紧凑

抽离出语言无关、平台无关的类型格式后，再为每个语言提供一个翻译器，能将配置文件自动生成数据代码，爽翻了

# protobuf
首先要有一个编译器将 .proto 文件编译成目标语言文件，即protoc，由于它不支持go编译，所以要安装 protoc-gen-go插件

    protoc -I ${需要导入proto文件的路径，默认当前路径} 
            --go_out=plugins=grpc:${生成文件路径} ${要编译的文件}

--go_out 很有意思，代表使用 protoc-gen-go插件，可以用类似方法使用其他插件

grpc-web编译

    protoc --js_out=import_style=commonjs,binary:. ./frontend/src/api \
   			--grpc-web_out=import_style=commonjs,mode=grpcwebtext:../frontend/src/api */*.proto

## 推荐的go protoc插件
- [github.com/envoyproxy/protoc-gen-validate](github.com/envoyproxy/protoc-gen-validate) 用于rpc之间交互参数校验

- [github.com/favadi/protoc-go-inject-tag](github.com/favadi/protoc-go-inject-tag) 可以给生成的文件inject自定义的tag

# 项目组织
- 把proto文件放在和模块同级的文件夹内统一管理，其他模块导入生成的代码
- 所有模块都可能会用到的基础库封装也抽象为单独的模块

如
```
- service1
- service2
- lib 公共库
- proto pb文件
```

```
require (
	lib v0.0.0-00010101000000-000000000000
	proto v0.0.0-00010101000000-000000000000
)

replace (
	lib => ../lib
	proto => ../proto
)
```

# grpc
## 提供http服务
项目需要提供http接口给前端或其他项目调用，有几种方案

1. 在同一个端口上开启两个服务，区分grpc和其他请求，但是要再封装http handle，麻烦
```
return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
    if r.ProtoMajor == 2 && strings.Contains(r.Header.Get("Content-Type"), "application/grpc") {
        grpcServer.ServeHTTP(w, r)
    } else {
        otherHandler.ServeHTTP(w, r)
    }
})
```

2. grpc-gateway

需要修改pb文件，不够优雅
```
// pb文件中新增
import "google/api/annotations.proto";

// 对rpc调用做额外的option，不够优雅
rpc SayHello(HelloHTTPRequest) returns (HelloHTTPResponse) {
    // http option
    option (google.api.http) = {
        post: "/example/echo"
        body: "*"
    };
}

# 先编译google的pb文件，M指定编译后的包名
protoc -I . --go_out=plugins=grpc,Mgoogle/protobuf/descriptor.proto=github.com/golang/protobuf/protoc-gen-go/descriptor:. google/api/*.proto

# 编译hello_http.proto
protoc -I . --go_out=plugins=grpc,Mgoogle/api/annotations.proto=github.com/jergoo/go-grpc-example/proto/google/api:. hello_http/*.proto

# 编译hello_http.proto gateway
$ protoc --grpc-gateway_out=logtostderr=true:. hello_http/hello_http.proto

```

3. grpc-web 前端也用grpc

- 需要nginx或者其他支持grpc协议的网关代理，本地则调试太麻烦，
- js这种弱类型语言对grpc的支持还不如用axios方便
- pb文件修改前端也要配合改，变更啥都不方便
- 还没有大规模的使用场景，可能有坑

# grpc middle
grpc-go通过注入函数的方式实现aop，可以注入日志、监控、认证等服务

[github.com/grpc-ecosystem/go-grpc-middleware](github.com/grpc-ecosystem/go-grpc-middleware) 这个库里封装了很多好用的插件