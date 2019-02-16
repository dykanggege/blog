- strings 包提供了很多操作字符串的简单函数，通常一般的字符串操作需求都可以在这个包中找到。
- strconv 包提供了基本数据类型和字符串之间的转换。在 Go 中，没有隐式类型转换，一般的类型转换可以这么做：int32(i)，将 i （比如为 int 类型）转换为 int32，然而，字符串类型和 int、float、bool 等类型之间的转换却没有这么简单。
- 进行复杂的文本处理必然离不开正则表达式。regexp 包提供了正则表达式功能，它的语法基于 RE2 ，regexp/syntax 子包进行正则表达式解析。
- Go 代码使用 UTF-8 编码（且不能带 BOM），同时标识符支持 Unicode 字符。在标准库 unicode 包及其子包 utf8、utf16中，提供了对 Unicode 相关编码、解码的支持，同时提供了测试 Unicode 码点（Unicode code points）属性的功能。
- 在开发过程中，可能涉及到字符集的转换，作为补充，本章最后会讲解一个第三方库：mahonia — 纯 Go 语言实现的字符集转换库

# strings
## 子串出现次数(字符串匹配)
```
num := strings.Count("kanggege","ge")
fmt.Println(num) //2
//如果子字符串为空，则匹配到的数量为	utf8.RuneCountInString(str)
num2 := strings.Count("康哥哥", "")
fmt.Println(num2) //4

```


# bytes 与 strings
[]byte 类型是在 go 程序中常用的类型，本来 byte 作为字节类型，并不常用，但是我们知道，string 类型每一次变化都是重新创建另一个值，需要再次分配空间，如果对一个 string 类型做多次的修改操作，就会生成了大量额外的变量，对垃圾回收器造成负担。

在 go 中字符默认使用 UTF-8 编码，字符的本质就是边长字节，如果使用 []byte 类型存储字符串，存储的是经过 UTF-8 编码过后的字节，每次对字符串的修改操作，直接修改底层字节，并不需要再生成额外的变量，提高了程序的性能

bytes 为 []byte 类型提供了常用的操作，其主要操作还是针对字符串的，与之相对应的包是 strings，两个包的很多函数用法都相同

对于基本的函数操作，可以查看文档使用

两个包中都有 Reader 结构体和 NewReader 方法，他实现了io.Reader、io.Seeker、io.ReaderAt、io.WriterTo、io.ByteScanner、io.RuneScanner接口，作为一个包装结构体，为这两种类型实现了这些接口的方法 

在 bytes 包下还有一个 Buffer 类型，它实现了字节缓冲，其本质和切片区别不大，而是将对切片的常用操作封装在结构体的函数中，更符合面向对象思想，并且实现了读写接口，便于其他流式操作
