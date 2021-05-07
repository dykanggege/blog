# GO语言编码规范V1.0（试行稿）

## 1 目的

为形成PCG统一的Go编码风格，以保障公司项目代码的易维护性和编码安全性，特制定本规范。

## 2 适用范围及推荐等级

本标准适用于PCG下所有使用Go语言作为开发语言的软件产品。

### 规则等级划分

| 规则 | 等级 |
| --- | --- |
| [3 编码规范](#3-编码规范) |  |
| [3.1 代码风格](#31-代码风格) |  |
| [3.1.1 格式化](#311-格式化) | 必须 |
| [3.1.2 换行](#312-换行) | 必须 |
| [3.1.3 括号和空格](#313-括号和空格) | 必须 |
| [3.1.4 import规范](#314-import规范) | 必须 |
| [3.1.5 错误处理](#315-错误处理) | 必须 |
| [3.1.6 测试](#316-测试) | 必须 |
| [3.2 注释](#32-注释) | 必须 |
| [3.3 命名规范](#33-命名规范) | 必须 |
| [3.3.1 包命名](#331-包命名) | 必须 |
| [3.3.2 文件命名](#332-文件命名) | 必须 |
| [3.3.3 结构体命名](#333-结构体命名) | 建议 |
| [3.3.4 接口命名](#334-接口命名) | 建议 |
| [3.3.5 变量命名](#335-变量命名) | 必须 |
| [3.3.6 常量命名](#336-常量命名) | 必须 |
| [3.4 控制结构](#34-控制结构) | 必须 |
| [3.5 函数](#35-函数) | 必须 |
| [3.6 defer](#36-defer) | 必须 |
| [3.7 参数传递](#37-参数传递) | 必须 |
| [3.8 方法的接收器method receiver](#38-方法的接收器method-receiver) | 必须 |
| [3.9 文件](#39-文件) | 必须 |
| [4 方法](#4-方法) | 必须 |
| [4.1 嵌套](#41-嵌套) | 必须 |
| [4.2 应用服务](#42-应用服务) | 建议 |
| [4.3 go mod](#43-go-mod) | 建议 |
| [4.4 魔法数字](#44-魔法数字) | 必须 |
| [4.5 变量声明](#45-变量声明) | 建议 |

## 3 编码规范

### 3.1 代码风格

#### 3.1.1 格式化

代码都需要用gofmt格式化。

#### 3.1.2 换行

一行代码不能超过100列，超过的情况，使用合理的换行方法换行。更少的列数，方便在terminal下查看，也方便两个代码框同屏编码。

#### 3.1.3 括号和空格

遵循gofmt的逻辑。关于操作符。一般情况，运算符和操作数之间要留空格。作为输入参数或者数组下标时，运算符和运算数之间不需要空格，紧凑展示。

#### 3.1.4 import规范

使用goimports自动格式化引入的包名。
引入单个包，应该采用如下格式：
```go
import (
    "fmt"
)
```
而不是这样：
```go
// DON'T DO THIS
import "fmt"
```

如果你引入了多种类型的包，应该对包进行分组管理，将包分为标准库包、程序内部包、第三方包，并将标准库作为第一组。三组包，用空行间隔，goimports或者gofmt会自动帮你把三组按首字母排序。
应该采用如下方式进行组织你的包：
```go
import (
    // standard package
    "encoding/json"
    "strings"

    // inner package
    "myproject/models"
    "myproject/controller"

    // third-party package
    "git.obc.im/obc/utils"
    "git.obc.im/dep/beego"
    "git.obc.im/dep/mysql"
)
```
不要使用相对路径引入包：
```go
// DON'T DO THIS
import (
    "../net"
)
```
应该使用完整的路径引入包：
```go
import (
    "xxxx.com/proj/net"
)
```

#### 3.1.5 错误处理
- **error**
  1. error作为函数的值返回，必须对error进行处理, 或将返回值赋值给明确忽略。
  2. 错误描述不需要标点结尾。
  3. 采用独立的错误流进行处理。
      不要采用这种方式
      ```go
      // DON'T DO THIS
      if err != nil {
          // error handling
      } else {
          // normal code
      }
      ```
      而要采用下面的方式
      ```go
      if err != nil {
          // error handling
          return // or continue, etc.
      }
      // normal code
      ```
        如果返回值需要初始化，则采用下面的方式
      ```go
      x, err := f()
      if err != nil {
          // error handling
          return // or continue, etc.
      }
      // use x
      ```
-	**Panic**
在业务逻辑处理中禁止使用panic。在main包中只有当完全不可运行的情况可使用panic，例如：文件无法打开，数据库无法连接导致程序无法正常运行。
对于其它的包，可导出的接口不能有panic，只能在包内使用。 建议在main包中使用log.Fatal来记录错误，这样就可以由log来结束程序。
panic捕获只能到goroutine最顶层，每个自行启动的goroutine，必须在入口处捕获panic，并打印详细堆栈信息或进行其它处理。
-	**Recover**
recover用于捕获runtime的异常，禁止滥用recover，在开发测试阶段尽量不要用recover。

#### 3.1.6 测试
1. 单元测试文件名命名规范为example_test.go。
2. 测试用例的函数名称必须以Test开头，例如TestExample。
3. 每个重要的可导出函数都要首先编写测试用例，测试用例和正规代码一起提交方便进行回归测试。

### 3.2 注释
-	**包注释**
每个包都应该有一个包注释。包如果有多个go文件，只需要出现在一个go文件中（一般是和包同名的文件）即可。
    ```go
    // Package math provides basic constants and mathematical functions.
    package math
    ```
    或
    ```go
    /*
    Package template implements data-driven templates for generating textual
    output such as HTML.
    ....
    */
    package template
    ```
-	**需要导出的结构体（接口）注释**
每个需要导出的自定义结构体或者接口都必须有注释说明，该注释对结构进行简要介绍，放在结构体定义的前一行，格式为：“结构体名 结构体描述说明”。同时结构体内的每个需要导出的成员变量都要有说明，该说明放在成员变量的前一行。示例如下：
    ```go
    // User 用户结构定义了用户基础信息
    type User struct {
    	// UserName 用户名
    	UserName string
    	// Email 邮箱
    	Email string
    }
    ```
-	**函数（方法）注释**
每个需要导出的函数或者方法（结构体或者接口下的函数称为方法）都必须有注释，描述函数或方法功能、调用方等信息。示例如下：
    ```go
    // NewtAttrModel 是属性数据层操作类的工厂方法
    func NewAttrModel(ctx *common.Context) *AttrModel {
    	// TODO
    }
    ```

### 3.3 命名规范
命名是代码规范中很重要的一部分，统一的命名规范有利于提高代码的可读性，好的命名仅仅通过命名就可以获取到足够多的信息。
#### 3.3.1 包命名
保持 package 的名字和目录保持一致，尽量采取有意义、简短的包名，尽量不要和标准库冲突。包名应该为小写单词，不要使用下划线或者混合大小写，使用中划线或分目录来划分层级。
简单明了的包命名，如：time、list、http。
不要使用无意义的包名，如：util、common、misc。
#### 3.3.2 文件命名
采用有意义，简短的文件名，文件名应该采用小写，并且使用下划线分割各个单词。
#### 3.3.3 结构体命名
1. 采用驼峰命名方式，首字母根据访问控制采用大写或者小写。
2. 结构体名应该是名词或名词短语，如 Customer、WikiPage、Account、AddressParser。
3. 避免使用Manager、Processor、Data、Info 这样的结构体名，结构体名不应当是动词。
4.  结构体的申明和初始化格式采用多行，例如：
    ```go
    // 多行申明
    type User struct{
    	UserName string
        Email string
    }

    // 多行初始化
    u := User{
    	UserName: "astaxie",
    	Email: "astaxie@gmail.com",
    }
    ```

#### 3.3.4 接口命名
1. 命名规则基本保持和结构体命名规则一致。
2. 单个函数的结构名以 "er" 作为后缀，例如Reader，Writer。
```go
type Reader interface {
    Read(p []byte) (n int, err error)
}
```
3. 两个函数的接口名综合两个函数名（建议）。
4. 三个以上函数的接口名，类似于结构体名。
```go
type Car interface {
    Start([]byte)
    Stop() error
    Recover()
}
5. 本身以er结尾的单词接口在后面添加able
```

#### 3.3.5 变量命名
1. 和结构体类似，变量名一般遵循驼峰式，首字母根据访问控制原因大写或小写，但遇到特有名词时，需要遵循以下规则：
    - 如果变量为私有，且特有名词为首个单词，则使用小写，如apiClient；
    - 其他情况都应该使用该名词原有的写法，如APIClient、repoID、UserID；
    - 错误示例：UrlArray，应该写成urlArray或者URLArray；
2. 若变量类型为bool类型，则名称应以Has，Is，Can或者Allow开头。

#### 3.3.6 常量命名
1. 常量均需遵循驼峰式。如：
    ```go
    const AppVersion = "1.0.0"
    ```
2. 如果是枚举类型的常量，需要先创建相应类型：
    ```go
    type Scheme string
    const (
        HTTP Scheme = "http"
        HTTPS Scheme = "https"
    )
    ```

**[⬆ 返回规则等级划分](#规则等级划分)**    

### 3.4 控制结构
- **if**
if接受初始化语句，约定如下方式建立局部变量：
  ```go
  if err := file.Chmod(0664); err != nil {
    return err
  }
  ```
-	**for**
采用短声明建立局部变量：
    ```go
    sum := 0
    for i := 0; i < 10; i++ {
        sum += 1
    }
    ```
-	**range**
如果只需要第一项（key），就丢弃第二个：
    ```go
    for key := range m {
        if key.expired() {
            delete(m, key)
        }
    }
    ```
    如果只需要第二项，则把第一项置为下划线：
    ```go
    sum := 0
    for _, value := range array {
        sum += value
    }
    ```
-	**switch**
要求必须有 default：
    ```go
    switch os := runtime.GOOS; os {
        case "darwin":
            fmt.Println("OS X.")
        case "linux":
            fmt.Println("Linux.")
    	default:
            // freebsd, openbsd,
            // plan9, windows...
            fmt.Printf("%s.\n", os)
    }
    ```
- **return**
尽早return：一旦有错误发生，马上返回：
  ```go
  f, err := os.Open(name)
  if err != nil {
      return err
  }

  d, err := f.Stat()
  if err != nil {
    f.Close()
    return err
  }

  codeUsing(f, d)
  ```

### 3.5 函数
1. 函数返回相同类型的两个或三个参数，或者如果从上下文中不清楚结果的含义，使用命名返回，其它情况不建议使用命名返回。
    ```go
    func (n *Node) Parent1() *Node
    func (n *Node) Parent2() (*Node, error)

    func (f *Foo) Location() (lat, long float64, err error)
    ```
2. 传入变量和返回变量以小写字母开头，数量均不能超过5个。

### 3.6 defer
1. 当存在资源管理时，应紧跟defer函数进行资源的释放。

### 3.7 参数传递
1. 尽量用值传递，非指针传递。
2. 传入参数是map，slice，chan，interface不要传递指针。

### 3.8 方法的接收器method receiver
1. 接收器的命名在函数超过20行的时候不要用单字符。
2. 命名不能采用“me”，“this”，“self”这类易混淆名称。

### 3.9 文件
1. 文件长度不能超过800行。

**[⬆ 返回规则等级划分](#规则等级划分)**

## 4 方法
1. 方法长度建议不超过80行。

### 4.1 嵌套
1. 嵌套深度不能超过4层（补充样例）：
```go
//	添加成功或出错
func (s *BookingService) AddArea(areas ...string) error {
	s.Lock()
	defer s.Unlock()
	for _, area := range areas {
		for _, has := range s.areas {
			if area == has {
				return srverr.ErrAreaConflict
			}
		}
		s.areas = append(s.areas, area)
		s.areaOrders[area] = new(order.AreaOrder)
	}
	return nil
}
```
建议调整：
```go
//	添加成功或出错
func (s *BookingService) AddArea(areas ...string) error {
	s.Lock()
	defer s.Unlock()
	for _, area := range areas {
		if s.HasArea(area) {
			return srverr.ErrAreaConflict
		}
		s.areas = append(s.areas, area)
		s.areaOrders[area] = new(order.AreaOrder)
	}
	return nil
}

func (s *BookingService) HasArea(area string) bool {
	for _, has := range s.areas {
		if area == has {
			return true
		}
	}
	return false
}
```

### 4.2 应用服务
应用服务接口建议有readme.md，其中建议包括服务基本描述、使用方法、部署时的限制与要求、基础环境依赖（例如最低go版本、最低外部通用包版本）等。
​​![](http://imgcache.oa.com/photos/56848/o_a9921fc6200fa0baa2ea2897cb04850a.png)

### 4.3 go mod
建议所有项目使用go modules：
```go
go mod init my-project
```

### 4.4 魔法数字
如果魔法数字出现超过2次，则禁止使用。
```go
func getArea(r float64) float64 {
	return 3.14 * r * r

func getLength(r float64) float64 {
	return 3.14 * 2 * r
}
```

建议用一个常量代替：

```go
const PI = 3.14

func getArea(r float64) float64 {
	return PI * r * r
}

func getLength(r float64) float64 {
	return PI * 2 * r
}
```

### 4.5 变量声明
变量声明尽量放在变量第一次使用前面。


## 附：常用工具
go 语言本身在代码规范性这方面也做了很多努力，很多限制都是强制语法要求，例如左大括号不换行，引用的包或者定义的变量不使用会报错，此外 go 还是提供了很多好用的工具帮助我们进行代码的规范。
- gofmt ，大部分的格式问题可以通过gofmt解决， gofmt 自动格式化代码，保证所有的 go 代码与官方推荐的格式保持一致，于是所有格式有关问题，都以 gofmt 的结果为准。
- goimports ，此工具在 gofmt 的基础上增加了自动删除和引入包。
- go vet ，vet工具可以帮我们静态分析我们的源码存在的各种问题，例如多余的代码，提前return的逻辑，struct的tag是否符合标准等。编译前先执行代码静态分析。

**[⬆ 返回规则等级划分](#规则等级划分)**
- golint ，类似javascript中的jslint的工具，主要功能就是检测代码中不规范的地方。
