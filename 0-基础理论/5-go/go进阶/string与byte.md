string和[]byte都可以表示字符串，都有对应的处理函数，探究下具体结构和相互转换

# byte
type byte = uint8

可以把byte看作ascii码的集合，而slice是以下结构
```
type slice struct {
    array unsafe.Pointer
    len   int
    cap   int
}
```

则[]byte是指向ascii码数组的指针

对slice修改和拼接的成本是比较低的

# string
// string is the set of all strings of 8-bit bytes, conventionally but not
// necessarily representing UTF-8-encoded text. A string may be empty, but
// not nil. Values of string type are immutable.
type string string

是8位字符的集合，值无法改变，修改操作会先复制，再修改

string本身也是指针，指向不可改变的内存，结构和slice类似
```
type stringStruct struct {
    str unsafe.Pointer
    len int
}

// 实例化方法
func gostringnocopy(str *byte) string {
 ss := stringStruct{str: unsafe.Pointer(str), len: findnull(str)}
 s := *(*string)(unsafe.Pointer(&ss))
 return s
}
```
本质是string这个指针struct可以变，但是底层pointer内存区域不可变

在其他语言中也类似，将string设计为不可改变，只能copy内存修改，可以避免并发竞态问题

# 相互转换
string转[]byte

```
const tmpStringBufSize = 32

type tmpBuf [tmpStringBufSize]byte

func stringtoslicebyte(buf *tmpBuf, s string) []byte {
    var b []byte
    if buf != nil && len(s) <= len(buf) {
        *buf = tmpBuf{}
        b = buf[:len(s)]
    } else {
        b = rawbyteslice(len(s))
    }
    copy(b, s)
    return b
}
// rawbyteslice allocates a new byte slice. The byte slice is not zeroed.
func rawbyteslice(size int) (b []byte) {
    cap := roundupsize(uintptr(size))
    p := mallocgc(cap, nil, false)
    if cap != uintptr(size) {
        memclrNoHeapPointers(add(p, uintptr(size)), cap-uintptr(size))
    }

    *(*slice)(unsafe.Pointer(&b)) = slice{p, size, int(cap)}
    return
}
```

如果长度小于32b，声明数组，否则会分配内存后，最后copy转换

[]byte转string也会发生内存分配和copy

[]byte和string的结构类似，可以0 copy的方式转换，但可能异常崩溃
```
func stringtoslicebytetmp(s string) []byte {
    str := (*reflect.StringHeader)(unsafe.Pointer(&s))
    ret := reflect.SliceHeader{Data: str.Data, Len: str.Len, Cap: str.Len}
    return *(*[]byte)(unsafe.Pointer(&ret))
}
```

如果在高性能场景全部用[]byte就好，不要试图将[]byte和string无损转换