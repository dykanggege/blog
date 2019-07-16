# JSON
## JSON可存储值类型

- 简单值：数值，字符串，null和布尔值，**注意不支持undefined，而且字符串要用双引号**
- 对象：注意对象的属性定义也要加双引号 { "var" : "value" } 
- 数组：["a","b",5,null,true]

## js使用JSON
ES5中定义了全局对象JSON，用于JSON操作
- JSON.stringify()：把js对象转化为JSON对象
- JSON.parse()：把JSON对象转化为js对象

### JSON.stringify()
stringify在序列化js对象时会忽略所有的函数,原型对象和值为undefined的属性

**除了要接受的对象外，还有两个参数，第一个为过滤器，第二个用来控制缩进**

过滤器可以是一个数组或函数

JSON.stringify(obj, ["attr1","attr2"])  只会将obj中的attr1和attr2属性保留

JSON.stringify(obj, function(attr, val){}) 函数可以接收到对象中所有的键值对作为参数，再进行处理返回

第三个参数为字符串缩进格数，可以为null，最大为10（超过10则默认为10）

如果第三个函数为字符串而非数值，则会用该字符串进行缩进

### toJSON()
可以给对象定义toJSON方法进行自定义序列化要求，例如Date原生toJSON方法会将其转换为 ISO 8601 日期字符串

使用stringify序列化会依次进行一下步骤
- 如果对象存在toJSON方法并可以取得有效值，调用该方法并取得值，否则返回对象本身
- 如果存在第二个参数，应用这个对象过滤器
- 对上一步返回的值进行序列化
- 如果存在第三个参数，使用第三个参数再次序列化

### JSON.parse()
也可以接受第二个参数，必须为函数类型，将在对象的每个键值对上调用

## java生成和解析json

有很多方法，这里主要介绍Jackson类库的使用方法

**Jackson的方法都是基于javabean的**

+ jackson-core.jar (核心jar) 
+ jackson-annotations.jar (该包提供Json注解支持)
+ jackson-databind.jar (对象序列化支持)

**Jackson提供了一系列注解，方便对JSON序列化和反序列化进行控制，下面介绍一些常用的注解**

- @JsonIgnore 此注解用于属性上，作用是进行JSON操作时忽略该属性。
- @JsonFormat 此注解用于属性上，作用是把Date类型直接转化为想要的格式，如@JsonFormat(pattern = "yyyy年MM月dd日")
- @JsonProperty 此注解用于属性上，作用是把该属性的名称序列化为另外一个名称，如把eMail属性序列化为mail，@JsonProperty("mail")

**其他常用方法**
- ObjectMapper mapper = new ObjectMapper(); //一切方法都是基于该对象
- string mapper.writeValueAsString() //可以将任何对象或集合、数组转化为json字符串

# ajax
>**同源政策规定，AJAX请求只能发给同源的网址，否则就报错**

浏览器安全的基石是"同源政策"（same-origin policy）,最初，它的含义是指，A网页设置的 Cookie，B网页不能打开，除非这两个网页"同源"。**所谓"同源"指的是"三个相同"**
- 协议相同
- 域名相同
- 端口相同

在一个页面中使用ajax，他的url是基于当前项目的地址（ip和端口）,所以一定是同源

最简单的解决同源问题的方法是使用jsonp

## jsonp

jsonp解决跨域问题的原理是，浏览器的script标签是不受同源策略限制的，我们可以在script标签中访问任何域名下的资源文件。利用这一特性，用script标签从服务器中请求数据，同时服务器返回一个带有方法和数据的js代码，请求完成，调用本地的js方法，来完成数据的处理

eg：

    function addScriptTag(src) {
        var script = document.createElement('script');
        script.setAttribute("type","text/javascript");
        script.src = src;
        document.body.appendChild(script);
    }
    //将访问地址包装为一个script标签
    window.onload = function () {
        addScriptTag('http://example.com?callback=foo');
    }
    //传输应该包含一个参数callback，使服务端知道此处使用的回调函数名(foo)
    function foo(data){
        console.log(data)
    }
    服务端应该返回一段js代码
    foo({val:'val'})

**分析：**

将访问地址包装为一个script标签后，加上参数callback（这也限制了jsonp只能用于get请求），当服务端得到callback参数的值之后应该将其视为js函数名，将意图传递的数据（一般都是json对象）放在函数中作为参数，而后返回此段js代码给前端，前端接受到js代码并执行，即调用了callback函数，就获得了数据




