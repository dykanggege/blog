关于 beego 的使用，官网给的文档已经足够清晰了，这里主要记一些我在使用中未留意的细节，长长教训

# err
满屏的 err 处理真的很难受，go 对异常的容忍度很低，我们不得不处理所有 err（虽然可以忽略 err，但我不认为那是一个好做法），对 err 的合理处理可以让我们迅速定位错误，我大致对 err 处理分为三个等级

## err
- 意料之内的错误：可以预料到的，用户操作带来的错误，我们应该对错误分析并反馈给用户提示，甚至智能纠错
- 对前端的不信任错误：即使前后端间定义了交互规范，若前端传入给我们的错误的数据格式，程序可可能会出错。这种不是用户犯错的情况，倒像是有人刻意的攻击，可以在日志中记录然后忽略这个错误，并终止当前线程
- 程序运行中出错：这种错误会在程序运行时发现，如果测试时已经覆盖 100% 代码，那么对这种未知错误可以记录日志并忽略，最后终止当前线程 

# 请求数据解析
## delete 方法
前端使用 jquery 与后台交互，起始发现 beego 无法解析 delete 方法中的参数，后来详细的读了 jquery 的文档才知道，delete 和 get 一样都没有请求体，只能在 url 中传递参数，jquery 会将 get 方法中的 data 解析到 url 中，而 delete 则不会，等于 delete 把 data 丢弃了，所以要使用 delete，必须自己手动将参数拼接到 url 中，这样后台才能接收到参数

delete、put 方法，从 IE9 就已经支持了，所以可以放心使用，不用再做隐藏值用 post 模拟 put
## 无法解析到嵌套字段
使用 this.ParseForm(&u) 可以直接将请求参数（无论头部或body中）解析到 struct 中，但是无法对其中的嵌套字段做解析

例如，在 gorm 中，gorm.Model 作为基础 struct 嵌入在 model 中，其中蕴涵了 ID 等基础字段，这些嵌入字段无法被解析到 struct 中

目前我能想到的解决方案就是把 gorm.Model 的字段拿出来在每个 model 中都写一次，经测试不会影响 gorm 的关联映射


# 开发可复用模块
## 常用配置
```
    appname = demo
    # 使用哪个环境下的配置
    runmode = dev

    # 路由区分请求路径大小写
    RouterCaseSensitive = false
    # 开启 session
    SessionOn = true
    # 存在客户端的 cookie 名称
    SessionName = inheritTree
    # 可以处理请求中的 json 数据
    copyrequestbody = true

    # 开发环境下的配置
    [dev]
    # 监听端口
    httpport = 8888
    # 数据库配置
    mysqluser = root
    mysqlpass = kanggege
    mysqlurl = localhost:3306
    mysqldb = demo
    # 这个是 gorm 的是否打印 sql 语句配置
    db_debug = true


    # 生产环境下的配置
    [prod]
    httpport = 9090

```
## models 下的 init 文件
```

    mysqluser := beego.AppConfig.String("mysqluser")
	mysqlpass := beego.AppConfig.String("mysqlpass")
	mysqlurl := beego.AppConfig.String("mysqlurl")
	mysqldb := beego.AppConfig.String("mysqldb")
	// 打开数据库连接
	var err error
	DB,err = gorm.Open("mysql",mysqluser+":"+mysqlpass+"@tcp("+mysqlurl+")/"+mysqldb+"?"+"charset=utf8&parseTime=True")

    //自动迁移，根据结构体生成表（不会删除或修改已存在的表中字段）
	DB.AutoMigrate(&User{})

    //打印 sql 语句
	db_debug,_ := beego.AppConfig.Bool("db_debug")
	DB.LogMode(db_debug)
```

## 控制访问
```//静态文件路径配置
	beego.SetStaticPath("/static", "static")

    //控制的静态目录 static/page，可访问静态目录 static/index
    //控制访问服务前缀 /demo
    var loginFilter = func(ctx *context.Context) {
		_, ok := ctx.Input.Session("uid").(int)

		if ctx.Request.RequestURI == "/" {
			if ok {
				ctx.Redirect(302, "static/page/home.html")
			} else {
				ctx.Redirect(302, "static/index/index.html")
			}
		}

		if !ok {
			ctx.Redirect(302, "static/index/index.html")
		}
	}
	beego.InsertFilter("/", beego.BeforeRouter, loginFilter)
	beego.InsertFilter("/static/page/*", beego.BeforeRouter, loginFilter)
	beego.InsertFilter("/demo/*", beego.BeforeRouter, loginFilter)

```