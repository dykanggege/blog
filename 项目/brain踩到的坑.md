# beego
## POST
小程序的 POST 方法必须将 header 设置为 application/x-www-form-urlencoded 后台才能拦截到数据

application/x-www-form-urlencoded 是所有浏览器支持的 form 解析方式，也是默认的 form 编码方式，它将表单中所有数据转换成键值对，application/json 则是序列化后的 json 字符串，如果含有文件，则使用 multipart/form-data，它使用二进制编码，用于有文件的情况