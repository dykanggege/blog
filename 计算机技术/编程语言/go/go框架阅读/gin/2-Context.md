看完了路由，进入Run流程看看
```
func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	c := engine.pool.Get().(*Context)
	c.writermem.reset(w)
	c.Request = req
	c.reset()

	engine.handleHTTPRequest(c)

	engine.pool.Put(c)
}

func (engine *Engine) handleHTTPRequest(c *Context) {
	httpMethod := c.Request.Method
	path := c.Request.URL.Path
	unescape := false
	if engine.UseRawPath && len(c.Request.URL.RawPath) > 0 {
		path = c.Request.URL.RawPath
		unescape = engine.UnescapePathValues
	}

	// Find root of the tree for the given HTTP method
	t := engine.trees
	for i, tl := 0, len(t); i < tl; i++ {
		if t[i].method != httpMethod {
			continue
		}
		root := t[i].root
		// Find route in tree
		handlers, params, tsr := root.getValue(path, c.Params, unescape)
		if handlers != nil {
			c.handlers = handlers
			c.Params = params
			c.Next() //这个很有意思
			c.writermem.WriteHeaderNow()
			return
		}
		if httpMethod != "CONNECT" && path != "/" {
			if tsr && engine.RedirectTrailingSlash {
				redirectTrailingSlash(c)
				return
			}
			if engine.RedirectFixedPath && redirectFixedPath(c, root, engine.RedirectFixedPath) {
				return
			}
		}
		break
	}

	if engine.HandleMethodNotAllowed {
		for _, tree := range engine.trees {
			if tree.method == httpMethod {
				continue
			}
			if handlers, _, _ := tree.root.getValue(path, nil, unescape); handlers != nil {
				c.handlers = engine.allNoMethod
				serveError(c, http.StatusMethodNotAllowed, default405Body)
				return
			}
		}
	}
	c.handlers = engine.allNoRoute
	serveError(c, http.StatusNotFound, default404Body)
}
```

整体看来超级简单，核心的只有 c.Next() 一句，其他的都是根据配置做调整，通过Next调用就把控制流程交给Context了，上下文实现了很多的功能

```
type Context struct {
	writermem responseWriter
	Request   *http.Request
	Writer    ResponseWriter

	Params   Params
	handlers HandlersChain
	index    int8

	engine *Engine

	// Keys is a key/value pair exclusively for the context of each request.
	Keys map[string]interface{} //中间件中存储的值

	// Errors is a list of errors attached to all the handlers/middlewares who used this context.
	Errors errorMsgs

	// Accepted defines a list of manually accepted formats for content negotiation.
	Accepted []string
}
```

# Next()
先看下Next，再看其他功能

```
//可以说是很精妙了
func (c *Context) Next() {
	c.index++
	for s := int8(len(c.handlers)); c.index < s; c.index++ {
		c.handlers[c.index](c)
	}
}

user := app.Group("/user", func(c *gin.Context) {
    fmt.Println("我是第一个调用的")
    c.Next()
    fmt.Println("我又成为最后一个调用的")
})
```

中间件还可以通过向后面的请求传值

    c.Set("key","val")
    c.Get("key")

# 参数解析

```
//所有的Query查询都包装自这个函数
//它调用的URL.Query，而这个函数每次都是用rawQuery解析的，没有缓存，性能不是很友好，
func (c *Context) GetQueryArray(key string) ([]string, bool) {
	if values, ok := c.Request.URL.Query()[key]; ok && len(values) > 0 {
		return values, true
	}
	return []string{}, false
}

//form值查询都包装自这个函数
//只支持 application/octet-stream、application/x-www-form-urlencoded、multipart/form-data 三种表单
func (c *Context) GetPostFormArray(key string) ([]string, bool) {
	req := c.Request
	req.ParseForm()
	req.ParseMultipartForm(c.engine.MaxMultipartMemory)
	if values := req.PostForm[key]; len(values) > 0 {
		return values, true
	}
	if req.MultipartForm != nil && req.MultipartForm.File != nil {
		if values := req.MultipartForm.Value[key]; len(values) > 0 {
			return values, true
		}
	}
	return []string{}, false
}

//还有路径参数param
func (c *Context) Param(key string) string {
	return c.Params.ByName(key)
}
```

除了这些基本的参数查询，还有 表单-对象 的映射，通过bind函数实现

```
//一切bind的根源都是这个函数
func (c *Context) ShouldBindWith(obj interface{}, b binding.Binding) error {
	return b.Bind(c.Request, obj)
}
//b的选择由头部字段决定ContentType
func (c *Context) Bind(obj interface{}) error {
	b := binding.Default(c.Request.Method, c.ContentType())
	return c.MustBindWith(obj, b)
}

//其实常用的只有json了
func Default(method, contentType string) Binding {
	if method == "GET" {
		return Form
	}

	switch contentType {
	case MIMEJSON:
		return JSON
	case MIMEXML, MIMEXML2:
		return XML
	case MIMEPROTOBUF:
		return ProtoBuf
	case MIMEMSGPACK, MIMEMSGPACK2:
		return MsgPack
	default: //case MIMEPOSTForm, MIMEMultipartPOSTForm:
		return Form
	}
}

//具体绑定的方式是通过反射实现的，这里就不看了
```

# 响应
提供了多种返回值，下面逐一介绍(除了模板)

## json
使用json返回，会把 Content-Type 设置为 "application/json".
- JSON(code int, obj interface{})  //通常会对html标签做转义
- PureJSON(code int, obj interface{}) //不会对obj里面的html特殊字符做转义
- JSONP(code int, obj interface{}) //应对跨域，这个函数有点东西

```
//jsonp请求应该自带callback，这里不默认任何callback
func (c *Context) JSONP(code int, obj interface{}) {
	callback := c.DefaultQuery("callback", "")
	if callback == "" {
		c.Render(code, render.JSON{Data: obj})
		return
	}
	c.Render(code, render.JsonpJSON{Callback: callback, Data: obj})
}
```

- SecureJSON(code int, obj interface{}) //防止json劫持，具体还不太懂

AsciiJSON serializes the given struct as JSON into the response body with unicode to ASCII string.

- AsciiJSON(code int, obj interface{}) 

