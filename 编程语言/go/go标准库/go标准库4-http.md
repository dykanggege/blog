http库主要实现两大功能，作为客户端发送http请求，作为服务器处理http请求

# 客户端
http提供了默认的直接请求方法
```
    func Get(url string) (resp *Response, err error)
    func Head(url string) (resp *Response, err error)
    func Post(url, contentType string, body io.Reader) (resp *Response, err error)
    func PostForm(url string, data url.Values) (resp *Response, err error)
    func ReadResponse(r *bufio.Reader, req *Request) (*Response, error)
```
他们底层公用一个var DefaultClient = &Client{}，并对应调用DefaultClient.Get(u)，它们都会先NewRequest，所以对于Request是并发安全的，而Client中的数据并不是安全的

标准库默认提供的是一个空的Client，我们可以创建自己的Client作为发送请求的代理

```
type Client struct {
    //主要用于请求转发，可以做：管理代理、TLS配置、keep-alive、压缩和其他设置
	Transport RoundTripper
    //对请求的重定向做处理，如果不是nil，会在执行重定向之前调用该函数
    //req重定向的请求，via所有请求
	CheckRedirect func(req *Request, via []*Request) error
    //管理cookie，可以添加或更改
	Jar CookieJar
    //设置请求时间限制
    //该超时限制包括连接时间、重定向和读取回复主体的时间。
    //零值代表不设置超时
	Timeout time.Duration
}
```

## cookie
一般用http.get之类就足够用了，如果做爬虫经常需要增加cookie，我们看看如何创建带cookie的client

### CookieJar
通过实现cookieJar接口，可以管理cookie

这些get/post请求最底层的调用都是client.send方法，我们看看send方法是如何使用cookie的
```
//每个请求都会携带jar中查询到的cookie(根据req.URL查询，注意每个URL是不同的)
if client.Jar != nil {
    //req.URL是我们发出请求时填写的url，被parse得到的
    for _, cookie := range client.Jar.Cookies(req.URL) {
        req.AddCookie(cookie)
    }
}

//如果重定向，会根据URL添加重定向新的cookie
if c.Jar != nil {
    if rc := resp.Cookies(); len(rc) > 0 {
        c.Jar.SetCookies(req.URL, rc)
    }
}
```