# request
```
type Request struct {
	//不处理connect方法，使用 Transport 做转发代理
	Method string

	URL *url.URL

	Proto      string // "HTTP/1.0"
	ProtoMajor int    // 1
	ProtoMinor int    // 0

	// If a server received a request with header lines,
	//
	//	Host: example.com
	//	accept-encoding: gzip, deflate
	//	Accept-Language: en-us
	//	fOO: Bar
	//	foo: two
	//
	// then
	//
	//	Header = map[string][]string{
	//		"Accept-Encoding": {"gzip, deflate"},
	//		"Accept-Language": {"en-us"},
	//		"Foo": {"Bar", "two"},
	//	}
	// http协议中没有限制header的大小写，但是解析时默认首字母大写
	Header Header

	// For server requests, the Request Body is always non-nil
	// but will return EOF immediately when no body is present.
	// The Server will close the request body. The ServeHTTP
	// Handler does not need to.
	Body io.ReadCloser

	// For server requests, it is unused.
	GetBody func() (io.ReadCloser, error)

	// ContentLength records the length of the associated content.
	// The value -1 indicates that the length is unknown.
	// Values >= 0 indicate that the given number of bytes may
	// be read from Body.
	ContentLength int64

	TransferEncoding []string

	// For server requests, the HTTP server handles this automatically
	// and this field is not needed by Handlers.
	Close bool

	Host string

	// Form contains the parsed form data, including both the URL
	// field's query parameters and the POST or PUT form data.
	// This field is only available after ParseForm is called.
	// The HTTP client ignores Form and uses Body instead.
	Form url.Values

	// PostForm contains the parsed form data from POST, PATCH,
	// or PUT body parameters.
	//
	// This field is only available after ParseForm is called.
	// The HTTP client ignores PostForm and uses Body instead.
	PostForm url.Values

	// MultipartForm is the parsed multipart form, including file uploads.
	// This field is only available after ParseMultipartForm is called.
	// The HTTP client ignores MultipartForm and uses Body instead.
	MultipartForm *multipart.Form

	// Few HTTP clients, servers, or proxies support HTTP trailers.
	Trailer Header

	RemoteAddr string

	RequestURI string

	TLS *tls.ConnectionState

	// For server requests, this field is not applicable.
	Cancel <-chan struct{}

	// Response is the redirect response which caused this request
	// to be created. This field is only populated during client
	// redirects.
	Response *Response

	// ctx is either the client or server context. It should only
	// be modified via copying the whole Request using WithContext.
	// It is unexported to prevent people from using Context wrong
	// and mutating the contexts held by callers of the same request.
	ctx context.Context
}
```

其实一个request包含的信息也不多，除了url就是header和body了，我们主要讲在server端使用request

## cookie

	r.AddCookie(&http.Cookie{})

这个一般是在client端使用，或者转发时添加cookie

	r.Cookie("cookiename") //查询某个cookie值
	r.Cookies()	//返回所有的cookie值

## 参数值
在 request 中有三个字段存储了参数值

- Form：存了query和body参数,如果query和form字段冲突，form值优先
- PostForm：只存body
- MultipartForm：存储 multipart/form-data 类型表单值

你也可以直接去request.Url.Query()中获取query值

起初上面三个字段的值都是空的，可以使用下面方法解析补充值
	
	r.ParseForm() 

他用来解析表单值，具体解析方式由 Content-Type 决定

- 空：如果为空，则使用 application/octet-stream，二进制类型，不常用
- application/x-www-form-urlencoded：正常解析
 
基本上只能解析这两种，还有一种常见的 multipart/form-data，需要用 ParseMultipartForm() 解析

	r.ParseMultipartForm()

实际上 r.ParseMultipartForm() 也会先调用 r.ParseForm()，简单的说你可以无脑调用它

下面两个获取值的方式就是无脑调用 r.ParseMultipartForm()

	r.FormValue("name")
	r.PostFormValue("name")

multipart/form-data 表单获取文件

	r.FormFile()

它也会调用一下 r.ParseMultipartForm()

总结：只有你调用上面的方法后，request中三个字段才会被填充值，而且标准库中只提供了 application/octet-stream、application/x-www-form-urlencoded、multipart/form-data 三种类型的解析


# response
```
type Response struct {
    Status     string // e.g. "200 OK"
    StatusCode int    // e.g. 200
    Proto      string // e.g. "HTTP/1.0"
    ProtoMajor int    // e.g. 1
    ProtoMinor int    // e.g. 0

    Header Header

    Body io.ReadCloser

    ContentLength int64

    TransferEncoding []string

    Close bool

    Uncompressed bool // Go 1.7

    Trailer Header

    Request *Request

    TLS *tls.ConnectionState // Go 1.3
}

```

response看起来比request简单多了，在server中使用的是 ResponseWriter

```
type ResponseWriter interface {
    Header() Header

    Write([]byte) (int, error)

    WriteHeader(statusCode int)
}
```

如果你想向ResponseWriter写入cookie，应该调用

	func SetCookie(w ResponseWriter, cookie *Cookie)

