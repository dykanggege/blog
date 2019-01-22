http包下主要有两大类，一个是作为客户端发起请求，一个是作为服务器处理请求，他们的核心点和交叉点都是Request和Reponse

先从 http.Get() 开始看

```
//调用包中默认的Client的Get方法
func Get(url string) (resp *Response, err error) {
	return DefaultClient.Get(url)
}

//空的对象
var DefaultClient = &Client{}

//再看http.Post、http.Head 都是调用DefaultClient，再往下看 http.Client

//字段很简单，看它具有的方法
type Client struct {
	Transport RoundTripper
	CheckRedirect func(req *Request, via []*Request) error
	Jar CookieJar
	Timeout time.Duration
}

//向外暴露的还是Get、Post、PostForm、Head、Do这几个方法，我们从Get开始一个个看

//它使用了 Request 结构体，我们看Request
func (c *Client) Get(url string) (resp *Response, err error) {
	req, err := NewRequest("GET", url, nil)
	if err != nil {
		return nil, err
	}
	return c.Do(req)
}
```

无论是get/post等，都是要先NewRequest，然后调用do方法，看request结构体

我们看do方法

```

```


# http.Request
```
//http包中有两大作用，作为客户端发起请求，作为服务端处理请求
//他们都用到了 Request，反而造成他的字段对于这两个功能中任意一个是冗余的
//但是约定大于配置，没必要搞个超类去继承，keep simple，keep stupid

type Request struct {
	// 请求方法，用于客户端，如果是""默认为Get
	Method string
	URL *url.URL
	Proto string
	ProtoMajor int
	ProtoMinor int
	Header http.Header
	Body io.ReadCloser
	GetBody func() (io.ReadCloser,error)
	ContentLength int64
	//重定向后的编码方式
	TransferEncoding []string
	// 请求完成之后是否断开连接
	Close bool
	Host string
	//map[string][]string，第一个是有效值
	//包含url中的参数和body中的参数
	Form url.Values
	PostForm url.Values
	//多种文件上传表单
	MultipartForm *multipart.Form
	//表示传输报文主体后面跟上的首部字段
	Trailer http.Header
	//IP:port形式
	RemoteAddr string
	//请求资源标示
	//url = proto + remoteAddr + uri
	RequestURI string
	//https
	TLS *tls.ConnectionState
	//用于终止请求
	Cancel <-chan struct{}
	//重定向的响应
	Response *http.Response
	//上下文，我也不知道是啥
	ctx context.Context
}
//了解他的结构体后，从使用上看他的方法
//先从Client看他的使用
```

# NewRequest
```
//根据传入的参数，尽可能的填充结构体内的字段
func NewRequest(method, urlstr string, body io.Reader) (*Request,error) {
	if method == ""{
		//这些一定固定的量，可变的概率比http消失的概率还小，就直接写死了
		method = "GET"
	}
	if !validMethod(method){
		return nil, fmt.Errorf("net/http: invalid method %q", method)
	}

	u,err := url.Parse(urlstr)
	if err != nil{
		return nil,err
	}

	//如果不是空，且有read，加上close，避免调用出错
	rc,ok := body.(io.ReadCloser)
	if !ok && body != nil{
		rc = ioutil.NopCloser(body)
	}
	//url解析的host是host:port形式，移除端口号
	u.Host = removeEmptyPort(u.Host)

	req := &Request{
		Method:method,
		URL:u,
		Proto: "HTTP/1.1",
		ProtoMajor:1,
		ProtoMinor:1,
		Header:make(http.Header),
		Body:rc,
		Host:u.Host,
	}

	// 如果不是空，通过类型判断，取得它的长度
	if body != nil{
		switch v := body.(type) {
		case *bytes.Buffer:
			req.ContentLength = int64(v.Len())
			buf := v.Bytes()
			req.GetBody = func() (io.ReadCloser,error) {
				r := bytes.NewReader(buf)
				return ioutil.NopCloser(r),nil
			}
		case *bytes.Reader:
			req.ContentLength = int64(v.Len())
			req.GetBody = func() (io.ReadCloser, error) {
				return ioutil.NopCloser(&*v),nil
			}
		case *strings.Reader:
			req.ContentLength = int64(v.Len())
			snapshot := *v
			req.GetBody = func() (io.ReadCloser, error) {
				r := snapshot
				return ioutil.NopCloser(&r), nil
			}
		default:
			// 这是干啥的？？？
			 if req.GetBody != nil && req.ContentLength == 0{
				 req.Body = http.NoBody
				 req.GetBody = func() (io.ReadCloser, error)
			 }
		}
	}
	
	return req,nil
}
```

