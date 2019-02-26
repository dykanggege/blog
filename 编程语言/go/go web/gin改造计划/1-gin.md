我很喜欢gin。

不同于beego，gin很有golang的风格，功能也是对http标准库进行封装和扩展，贴近与原理层面，如果计算机网络学的好，不需要读太多的文档就能读懂API（这就是API贴近原理的好处）

但是API多而繁杂，最致命的是所有功能统统放在了Context下，这样对命名空间很不友好，我更希望他能区分req、resp，把API再做一个分类

同时，有些离不开的中间件，并没有内置，还需要自己去解决，但是又不能和gin完美融合

所以我希望对gin做一些改造，在他的基础上划分命名空间，并添加一些内置的功能

想要改造，先要看懂，所以这里扒一扒gin的源码

一个简单的demo
```
    engine := gin.New()

	engine.Use(gin.LoggerWithWriter(os.Stdout))
	engine.Use(gin.RecoveryWithWriter(os.Stdout))

	user := engine.Group("/user")
	user.Use(func(c *gin.Context) {
		fmt.Println("可以在这里做session控制")
	})
	{
		user.GET("/name", func(c *gin.Context) {
			c.JSON(200,gin.H{"status":true,"name":"kanggege"})
		})
	}

	engine.GET("/other", func(c *gin.Context) {
		c.String(200,"others")
	})

	log.Fatal(engine.Run(":8899"))

```

# gin.Default
gin的功能实现依赖于Engine，默认的引擎gin.Default()实现如下

```
func Default() *Engine {
    //对golang版本检测，如不合格则打印警告信息
	debugPrintWARNINGDefault()
	engine := New()
    //注册中间件
    //Logger这里使用stdout作为日志打印
    //Recovery对于handle中的Panic做处理，使用500回复给客户端
	engine.Use(Logger(), Recovery())
	return engine
}
```

如果不想要打印日志到控制台，还是自己创建一个engine

# gin.Engine
Engine is the framework's instance, it contains the muxer, middleware and configuration settings.

```
type Engine struct {
    //做路由器和中间件的内部配置
    RouterGroup

    //路由匹配的各种配置信息
    ...
}
```

# engine.Run
我们从入口开始分析，逐步勾勒gin的架构

```
func (engine *Engine) Run(addr ...string) (err error) {
    //如果启动错误的处理
	defer func() { debugPrintError(err) }()

    //端口处理，如果是空，使用 :8080 端口
	address := resolveAddress(addr)
    //由此可见，engine本质还是个路由器，那么最关键的程序还是在 ServerHTTP 中写的
	err = http.ListenAndServe(address, engine)
	return
}
```

```
//主逻辑处理程序
func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
    //复用*Context结构，减小GC的压力
	c := engine.pool.Get().(*Context)
    //重启上下文结构数据
	c.writermem.reset(w)
	c.Request = req
	c.reset()

    //找到匹配的路由，并传入c
	engine.handleHTTPRequest(c)

	engine.pool.Put(c)
}

```

整个Run的处理逻辑:

1. 取出Context复用，Context整合了原生Request和Response信息，提供简单的API
2. handleHTTPRequest
	1. 检测配置，根据engine的配置和url找到匹配的处理函数
		1. 找不到处理函数，调动错误处理函数
	2. 调用Context中注册的中间件
	3. 调用处理函数

我们最常用的是Context的处理函数

# Context
Context用于处理请求函数，返回响应值，中间件之间参数传递

