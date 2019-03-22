```
type Request struct {
    //client：如果为空，默认使用get，不支持connect
    Method string

    //server：从请求的URI中解析，基本都会被解析到path和rawquery中
    //client：设置请求的url
    URL *url.URL

    
    Proto      string // "HTTP/1.0"
    ProtoMajor int    // 1
    ProtoMinor int    // 0
    
    Header Header

    //server：即使是get请求，body也no-nil，但是一读取就会返回EOF
    Body io.ReadCloser

    //client：用于设置读取body的函数
    GetBody func() (io.ReadCloser, error) // Go 1.8

    // The value -1 indicates that the length is unknown.
    ContentLength int64
    TransferEncoding []string

    //是否关闭tcp连接，或是复用
    Close bool

    Host string

    //server：包含url和body参数
    //client：无用
    Form url.Values

    // PostForm contains the parsed form data from POST, PATCH,
    // or PUT body parameters.
    PostForm url.Values // Go 1.1

    // MultipartForm is the parsed multipart form, including file uploads.
    // This field is only available after ParseMultipartForm is called.
    MultipartForm *multipart.Form

    Trailer Header

    RemoteAddr string

    //path之后的未解码的uri，一般都用url.path代替
    RequestURI string

    TLS *tls.ConnectionState

    //用于client
    Cancel <-chan struct{} // Go 1.5

    //重定向的响应
    Response *Response // Go 1.7
}
```

# url
request结构体中不仅有对整个url的解析信息：URL *url.URL，还有一个 RequestURI 这么多url都代表了啥？

先看一个uri的定义

    <scheme>://<user>:<password>@<host>:<port>/<path>;<params>?<query>#<frag>

由于url是由ASCII编码的，所以非ASCII字符会先用unicode编码，再使用 %码值 编码(记为方式A)，有些还会对 / & ? 等符号编码，也是使用 %ASCII码值 方式(记为方式B)

当我们获取到request结构体时，请求信息都已经被处理好了，我们看下面几个代表url的值

- r.URL.Path：只有/path部分，如果url是编码过的，无论是哪种编码方式，都只返回已经解码过的值
- r.RequestURI：包含/\<path>;\<params>?\<query>#\<frag>，如果是编码过的url，只会对 %2F 即 / 做解码操作
- r.URL.RawPath：编码过的url，一般不用于server端，如果client的request设置了该值，将此值解码得到 r.URL.Path
- r.URL.EscapedPath()：得到编码过的path，只对非ASCII字符编码  

在服务端开发时，一般用r.URL.Path做路由匹配，很少用编码过的uri

# params
对于请求参数的解析，request也提供了多种方式
- r.Form：可以查询url的query参数，和请求体中的param参数，返回的是解码过的值
- r.PostForm：只查询请求体中的param参数，POST、PATCH、PUT这些方法才有请求体，返回过的是解码过的值
- r.MultipartForm：用于文件上传的表单，调用r.ParseMultipartForm后才能使用
- r.Url.RawQuery：未解码的查询参数，返回 ? 之后的字符串，r.Url.Query()可以解析RawQuery，返回解码后的map

