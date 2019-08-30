时至今日，http已经发展成了一个庞然大物，里面的各种细节和实现真是让人头疼，首先就是对url的解析

    <scheme>://<user>:<password>@<host>:<port>/<path>;<params>?<query>#<frag>

```
type URL struct {
	Scheme     string
	Opaque     string    // encoded opaque data
	User       *Userinfo // username and password information
	Host       string    // host or host:port
	Path       string    // path (relative paths may omit leading slash)
	RawPath    string    // encoded path hint (see EscapedPath method)
	ForceQuery bool      // append a query ('?') even if RawQuery is empty
	RawQuery   string    // encoded query values, without '?'
	Fragment   string    // fragment for references, without '#'
}

func main() {
	path := "https://www.google.com:80/请求+(nb)=路径/path?name=康搁搁&age=20&param=%26"
	parse, err := url.Parse(path)
	errHanle(err)
	fprintUrl(parse)
}

scheme:  https
host:  www.google.com:80 
path:  /请求+(nb)=路径/path    //得到的就是解码后的path和query
rawPath:  /请求+(nb)=路径/path
rawQuery:  name=康搁搁&age=20&param=%26
forceQuery:  false
Opaque: 

//获取未解码的path，一般情况我们不会使用未解码的path
parse.EscapedPath() => /%E8%AF%B7%E6%B1%82+%28nb%29=%E8%B7%AF%E5%BE%84/path


//把query参数解析成 key-val 形式
parse.Query() => map[age:[20] name:[康搁搁] param:[&]]

println(parse.IsAbs()) //scheme是否为空
println(parse.Hostname()) //只返回host
println(parse.Port()) //只返回port
println(parse.RequestURI()) // path+query
```

除了这些，还提供了其他的辅助函数
```
//对path做编码和解码
func PathEscape(s string) string 
func PathUnescape(s string) (string, error)
//对query参数做编码和解码
func QueryEscape(s string) string
func QueryUnescape(s string) (string, error)
```

启示我们发送请求时，要对path和query做编码转义，否则后台可能无法正常解析