server是用作服务器，不像java之类，go自带的服务器用着很舒服，简单几行就启动了一个服务器

```
func main() {
    //直接注册一个函数处理
	http.Handle("/struct",new(handle))
    //注册一个结构体分发处理
	http.HandleFunc("/func", func(rw http.ResponseWriter, r *http.Request) {
        /*do something*/
	})
    //传入nil，使用默认的路由器
	err := http.ListenAndServe(":8080", nil)
	if err != nil{fmt.Println(err)}
}

type handle struct {
}

func (h *handle) ServeHTTP(http.ResponseWriter, *http.Request) {
}
```

看看源码

```
func ListenAndServe(addr string, handler Handler) error {
	server := &Server{Addr: addr, Handler: handler}
	return server.ListenAndServe()
}
```

再看看Server

# Server

```
type Server struct {
	Addr    string  // TCP address to listen on, ":http" if empty
	Handler Handler //路由器 handler to invoke, http.DefaultServeMux if nil，

	//tls证书配置，用于https
	TLSConfig *tls.Config

	//request请求的最大允许时间
	ReadTimeout time.Duration
	ReadHeaderTimeout time.Duration
	WriteTimeout time.Duration
	IdleTimeout time.Duration

	// 头部允许的最大大小，If zero, DefaultMaxHeaderBytes(1MB) is used.
	MaxHeaderBytes int
    //tls协议相关
	TLSNextProto map[string]func(*Server, *tls.Conn, Handler)
	ConnState func(net.Conn, ConnState)


	//错误日志处理，If nil, logging is done via the log package's standard logger.
	ErrorLog *log.Logger

	//隐藏字段
}
```

从字段可以看出，server中主要是各种配置相关，做一些超时限制之类的，真正的处理放在路由器handle中

我们在使用 http.ListenAndServe(":8080", nil) 传入nil，代表使用默认的路由器DefaultServeMux

# ServeMux

```
var DefaultServeMux = &defaultServeMux
var defaultServeMux ServeMux
```

而我们使用的http.Handle、http.HandleFunc都是注册到DefaultServeMux中
```
func HandleFunc(pattern string, handler func(ResponseWriter, *Request)) {
	DefaultServeMux.HandleFunc(pattern, handler)
}

func Handle(pattern string, handler Handler) { DefaultServeMux.Handle(pattern, handler) }

```

我们也可以实现自己的路由器，并注册

```
myMux := http.NewServeMux()
myMux.HandleFunc("/", func(rw http.ResponseWriter, r	 *http.Request) {
    fmt.Println(r.URL)
})
err := http.ListenAndServe(":8080", myMux)
if err != nil{
    log.Panic(err)
}
```
