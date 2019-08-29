一如既往，http库提供了server的默认实现和开箱即用的方法
```
http.HandleFunc("/user", func(w http.ResponseWriter, r *http.Request) {
    w.WriteHeader(200)
    w.Write([]byte("ok"))
})
if err := http.ListenAndServe(":8899", nil); err != nil {
    log.Fatal(err)
}
```

他们都是包装方法，真正的调用如下

# Server http容器

Server只负责基于tcp整合http报文信息，拿到http信息后的处理交给其他模块做，所以server相当于一个http容器

```
type Server struct {
	Addr    string  // TCP address to listen on, ":http" if empty
	Handler Handler // handler to invoke, http.DefaultServeMux if nil
	//tls证书配置
	TLSConfig *tls.Config
	//对整个请求的readline
	ReadTimeout time.Duration
	//只是请求头的readline
	ReadHeaderTimeout time.Duration

	WriteTimeout time.Duration

	// IdleTimeout is the maximum amount of time to wait for the
	// next request when keep-alives are enabled. If IdleTimeout
	// is zero, the value of ReadTimeout is used. If both are
	// zero, ReadHeaderTimeout is used.
	IdleTimeout time.Duration

	// If zero, DefaultMaxHeaderBytes(1 MB) is used.
	MaxHeaderBytes int

	//tls升级策略
	TLSNextProto map[string]func(*Server, *tls.Conn, Handler)

    //当连接状态改变时的回调函数
	ConnState func(net.Conn, ConnState)

	// ErrorLog specifies an optional logger for errors accepting
	// connections, unexpected behavior from handlers, and
	// underlying FileSystem errors.
	// If nil, logging is done via the log package's standard logger.
	ErrorLog *log.Logger

	disableKeepAlives int32     // accessed atomically.
	inShutdown        int32     // accessed atomically (non-zero means we're in Shutdown)
	nextProtoOnce     sync.Once // guards setupHTTP2_* init
	nextProtoErr      error     // result of http2.ConfigureServer if used

	mu         sync.Mutex
	listeners  map[*net.Listener]struct{}
	activeConn map[*conn]struct{}
	doneChan   chan struct{}
	onShutdown []func()
}

```


```
func ListenAndServe(addr string, handler Handler) error {
    //因为你可能同时启动多个server，而每个server都需要单独处理
    //所以没有提供 defaultServer，而是在这里 newServer
	server := &Server{Addr: addr, Handler: handler}
	return server.ListenAndServe()
}

func (srv *Server) ListenAndServe() error {
    ...
    // :http 在 net.Listen 中默认使用 80 端口
    addr := srv.Addr
	if addr == "" {
		addr = ":http"
	}
    //创建一个tcp socket
	ln, err := net.Listen("tcp", addr)
	if err != nil {
		return err
	}
    //tcpKeepAliveListener：把socket升级为长连接，每三分钟发送心跳包维持存活
	return srv.Serve(tcpKeepAliveListener{ln.(*net.TCPListener)})
}

func (srv *Server) Serve(l net.Listener) error {
	...

    //看是否能升级为http2
	if err := srv.setupHTTP2_Serve(); err != nil {
		return err
	}

	...

	var tempDelay time.Duration     // how long to sleep on accept failure
	baseCtx := context.Background() // base is always background, per Issue 16220
	ctx := context.WithValue(baseCtx, ServerContextKey, srv)
	for {
		rw, e := l.Accept()
		if e != nil {
			select {
            //如果已经关闭conn了就结束
			case <-srv.getDoneChan():
				return ErrServerClosed
			default:
			}
            //如果一次连接失败，休眠，然后重新accept，为啥要休眠？？？
			if ne, ok := e.(net.Error); ok && ne.Temporary() {
				if tempDelay == 0 {
					tempDelay = 5 * time.Millisecond
				} else {
					tempDelay *= 2
				}
				if max := 1 * time.Second; tempDelay > max {
					tempDelay = max
				}
				srv.logf("http: Accept error: %v; retrying in %v", e, tempDelay)
				time.Sleep(tempDelay)
				continue
			}
			return e
		}
		tempDelay = 0
		c := srv.newConn(rw)
		c.setState(c.rwc, StateNew) // before Serve can return
		go c.serve(ctx)
	}
}


/****************serve******************/
//serve就是接收tcp包，并整合为http报文，其中有一行需要注意

	serverHandler{c.server}.ServeHTTP(w, w.req)

可以理解为前面都是接收数据包，并整合成http报文，构成request，这里才是获取到http之后的处理

type serverHandler struct {
	srv *Server
}

func (sh serverHandler) ServeHTTP(rw ResponseWriter, req *Request) {
	handler := sh.srv.Handler
	if handler == nil {
		handler = DefaultServeMux //这里默认使用 DefaultServeMux
	}
	if req.RequestURI == "*" && req.Method == "OPTIONS" {
		handler = globalOptionsHandler{}
	}
	handler.ServeHTTP(rw, req)
}

serveHandle
```

整个流程大致如下
1. 创建socket监听端口
2. 接收到请求，建立维持心跳存活的tcp连接
3. 尝试升级为http2
4. tls证书握手
5. 接收数据包，整合为http报文，调用serverHandler处理

# Handler 路由器
handler是真正用于处理http请求的接口，只有一个方法，ServeHTTP(ResponseWriter, *Request)

也就是说只要实现这个方法都能用于处理http请求，当然我们需要根据Request信息做路由，这个handler其实是路由器，标准库提供了一个路由实现

```
type ServeMux struct {
	mu    sync.RWMutex
	//路由查找先在这里查，如果查不到了去下面的es查
	m     map[string]muxEntry
	//es中总是是长的path放在前面，只要前缀匹配了就等于查找到路由，即最长匹配
	es    []muxEntry // slice of entries sorted from longest to shortest.
	hosts bool       // whether any patterns contain hostnames
}

type muxEntry struct {
	h       Handler
	pattern string
}

```

默认提供的路由发现贼简单粗暴，通过hashmap直接查找，查不到了遍历slice最长匹配，如果注册路由特别多性能一定会下降，建议还是用trie tree存储路由

他提供了两个方法注册路由
```
	func (mux *ServeMux) Handle(pattern string, handler Handler)

	func (mux *ServeMux) HandleFunc(pattern string, handler func(ResponseWriter, *Request))
```

向用户提供的要实现的接口依旧是Handler，其实就是基于原来的接口添加了路由发现功能

http库也提供了可直接使用的方法 http.HandleFunc 或 http.Handle

```
func HandleFunc(pattern string, handler func(ResponseWriter, *Request)) {
	DefaultServeMux.HandleFunc(pattern, handler)
}

var DefaultServeMux = &defaultServeMux 

var defaultServeMux ServeMux
```

## 匹配规则

注册路由规则

1. 所有的 path 都会被注册到 m 中
2. 只有以 / 结束的 path 才会被注册到 es 中,es中最长的 path 排在最前面

查找路由规则

1. 如果 /user 匹配不到，且m中注册的有 /user/ 则会 redirect(/user/)
2. 如果 path 在压缩后(清除 .. 或 "" 等) 得到 cleanPath，然后 redirect(cleanPath)
3. 如果注册路由时，存在未以 / 开头的path，则会先math(host+path)，再math(path)

math函数
```
v, ok := mux.m[path]
if ok {
	return v.h, v.pattern
}

// Check for longest valid match.  mux.es contains all patterns
// that end in / sorted from longest to shortest.
for _, e := range mux.es {
	if strings.HasPrefix(path, e.pattern) {
		return e.h, e.pattern
	}
}
```


```
e := muxEntry{h: handler, pattern: pattern}
mux.m[pattern] = e
if pattern[len(pattern)-1] == '/' {
	mux.es = appendSorted(mux.es, e)
}

```

示例规则

1. /user，只能匹配 /user
2. /user/
	1. /user，会被重定向到 /user/ 匹配成功
	2. /user/，匹配成功
	3. /user/other 匹配成功，最长匹配
3. user
4. /
	1. 如果path == ""，则 redirect(/)
	2. 所有匹配不到的path，都会 redirect(/)


http库提供的路由器还是比较简陋的，路由性能不高，没有原生支持restful，没有路径参数解析等，可以使用其他第三方库如httprouter代替ServeMux

有了http容器组件、路由注册发现组件，下面就可以处理request请求了