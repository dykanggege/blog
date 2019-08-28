go在网络这块非常强大，标准库支持非常好，甚至不用借助第三方框架，把http库封装一下就可以做web开发了

# client
```
type Client struct {
    //传输层
    Transport RoundTripper
    //重定向处理方式    
    CheckRedirect func(req *Request, via []*Request) error
    //请求携带的cookie
    Jar CookieJar
    //超时限制
    Timeout time.Duration
}
```

go标准库中会很常见的提供默认实现，client的默认实现

    var DefaultClient = &Client{}

同时提供了基于默认实现的开箱即用的方法，用于发送请求

    func Get(url string) (resp *Response, err error)
    func Head(url string) (resp *Response, err error)
    func Post(url, contentType string, body io.Reader) (resp *Response, err error)
    func PostForm(url string, data url.Values) (resp *Response, err error)

客户端不是讲解的重点，写俩demo示范下怎么用，主要看的是server

```
vals := url.Values{}
vals.Add("key","val")
resp, err := http.PostForm("www.baidu.com", vals)

path := ""
client := &http.Client{}
req, _ := http.NewRequest("POST",path,nil)
req.AddCookie(&http.Cookie{})
client.Do(req)
```