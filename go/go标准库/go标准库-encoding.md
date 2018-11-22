# god
## god 编码
我们通常说的编码是为了让数据以二进制的形式保存在计算机，而为了让数据结构在网络上传输或保存在文件，并且不丢失原来的结构信息，我们还需要一种编码，常见的编码方式有：json、XML 等，go 也提供了一种编码方式，由 gob 包定义

gob是Golang包自带的一个数据结构序列化的编码/解码工具，编码使用Encoder，解码使用Decoder

gob 和 json 的 pack 之类的方法一样，由发送端使用 Encoder 对数据结构进行编码。在接收端收到消息之后，接收端使用 Decoder 将序列化的数据变化成本地变量

发送方的数据结构和接收方的数据结构可以不必完全相同，结构相似也可以接收，但是最好还是使用相同的结构，避免出现不必要的错误

## god的编码规则

整型：分为sign int和usign int， 其中从上面例子也看到，int和uint是不能互相编解码的。float和int也是不能互相编解码的。

Struct，array，slice是可以被编码的。但是function和channel是不能被编码的。

bool类型是被当作uint来编码的，0是false，1是true。

浮点类型的值都是被当作float64类型的值来编码的

String和[]byte传递是uint(byte个数) + byte[]的形式编码的

Slice和array是按照uint(array个数) + 每个array编码 这样的形式进行编码的

Maps是按照 uint(Map个数) + 键值对 这样的形式进行编码的

Struct是按照一对对（属性名 + 属性值）来进行编码的。其中属性值是其自己对应的gob编码。前面说过，如果有一个属性值为0或空，则这个属性直接被忽略。每个属性的序号是由编码时候顺序决定的，从0开始顺序递增。Struct在序列化前会以-1代表序列化的开始，以0代表序列化结束。即Struct的序列化是按照 “-1 （0 属性1名字 属性1值） （1 属性2名字 属性2值） 0 ”来进行编码的。


非常重要的一点：

**Struct中的属性应该是public的，即应该是大写字母开头**

## 总结

golang 可以通过 json 或 gob 来序列化 struct 对象,虽然 json 的序列化更为通用,但利用 gob 编码可以实现 json 所不能支持的 struct 的方法序列化,利用 gob 包序列化 struct 保存到本地也十分简单

这里需要明确一点，gob只能用在golang中，所以在实际工程开发过程中，如果与其他端，或者其他语言打交道，那么gob是不可以的，我们就要使用json了

# json
## Encode
将一个对象编码成JSON数据，接受一个interface{}对象，返回[]byte和error：

func Marshal(v interface{}) ([]byte, error)

Marshal函数将会递归遍历整个对象，依次按成员类型对这个对象进行编码，类型转换规则如下：

bool类型 转换为JSON的Boolean
整数，浮点数等数值类型 转换为JSON的Number
string 转换为JSON的字符串(带""引号)
struct 转换为JSON的Object，再根据各个成员的类型递归打包
数组或切片 转换为JSON的Array
[]byte 会先进行base64编码然后转换为JSON字符串
map 转换为JSON的Object，key必须是string
interface{} 按照内部的实际类型进行转换，它利用反射逐一检验该数据满足何种类型，再使用该类型编码
nil 转为JSON的null
channel,func等类型 会返回UnsupportedTypeError

如果想自定义（非 struct 定义）一个 json 类型数据，最好使用 map 去做转换（少了定义 struct 的过程）

## struct
对 struct 做转化时，小写字符开头的属性将被默认隐藏不导出，使用 StructTag 可以设置对应关系

```
    type Message struct {
        Name string `json:"msg_name"`       // 解析为 json 或从 json 反向解析都将使用 msg_name 做映射
        Body string `json:"body,omitempty"` // omitempty,如果为空置则忽略字段
        Time int64  `json:"-"`              // 直接忽略字段
    }
    var m = Message{
        Name: "Alice",
        Body: "",
        Time: 1294706395881547000,
    }
    data, err := json.Marshal(m)
    if err != nil {
        fmt.Printf(err.Error())
        return
    }
    fmt.Println(string(data))
    
    Output:
    {"msg_name":"Alice"}

```

## interface{}
interface{}类型在Unmarshal时，会自动将JSON转换为对应的数据类型：

- JSON的boolean 转换为bool
- JSON的数值 转换为float64
- JSON的字符串 转换为string
- JSON的Array 转换为[]interface{}
- JSON的Object 转换为map[string]interface{}
- JSON的null 转换为nil