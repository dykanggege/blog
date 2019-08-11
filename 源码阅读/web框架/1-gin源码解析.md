http框架很多很多，但是大部分功能高度重合，我想他们解决思路也大径相同，所以先从一个老牌的gin开始阅读

任何一个http框架，都应该有下面这些最基本的功能

```
	app := gin.New()
	app.Use(gin.Logger(), gin.Recovery())

	user := app.Group("/user", func(c *gin.Context) {
		fmt.Println("path: user/")
	})
	{
		user.GET("/", func(c *gin.Context) {
			c.Query("id")
		})
		user.POST("/", func(c *gin.Context) {
			file, _ := c.FormFile("file")
			c.SaveUploadedFile(file,"/var/gin")
		})
	}

	app.Run(":1234")
```

从run这个源头去看gin的整体架构，再逐步拆分每个模块，最后关注其他功能的实现

```
func (engine *Engine) Run(addr ...string) (err error) {
	defer func() { debugPrintError(err) }()

	address := resolveAddress(addr)
	debugPrint("Listening and serving HTTP on %s\n", address)
	err = http.ListenAndServe(address, engine)
	return
}
```

看到http.ListenAndServe(address, engine)这句，说明Engine应该有一个ServeHTTP方法

```
func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	c := engine.pool.Get().(*Context)
	c.writermem.reset(w)
	c.Request = req
	c.reset()

	engine.handleHTTPRequest(c)

	engine.pool.Put(c)
}
```

很明显pool是一个context的缓存池，Context则是一个单独的模块，保存了请求的上下文信息，包括handle函数、请求参数等等，下面会逐一分析

接着调用了engine.handleHTTPRequest(c)

```
//我把下面代码里与主线无关的都删掉了
func (engine *Engine) handleHTTPRequest(c *Context) {
	httpMethod := c.Request.Method
	rPath := c.Request.URL.Path
	unescape := false

	// Find root of the tree for the given HTTP method
	t := engine.trees //engine.trees 负责路由发现的模块
	for i, tl := 0, len(t); i < tl; i++ {
		if t[i].method != httpMethod {
			continue
		}
		root := t[i].root
		// Find route in tree
		value := root.getValue(rPath, c.Params, unescape) //返回路由匹配的handle和路由参数

		//将这些值交给context处理，即交给使用者处理
		if value.handlers != nil {
			c.handlers = value.handlers
			c.Params = value.params
			c.fullPath = value.fullPath
			c.Next()
			c.writermem.WriteHeaderNow()
			return //如果正常获取到值，调用在这里就结束了
		}

		//后面都是对非正常情况的处理，都省略了
	}
}

```

到现在请求已经结束了，除了各种配置参数外，我们主要见到了两大模块，context 和 tree，把Engine其他参数去掉，就很简单了

```
type Engine struct {
	RouterGroup
	pool             sync.Pool
	trees            methodTrees
}
```

逐一看看这仨模块

# RouterGroup 
type RouterGroup struct {
	Handlers HandlersChain
	basePath string
	engine   *Engine
	root     bool
}