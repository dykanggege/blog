字符串在go中有两种表现方式，string和[]byte，一般将string看作数据量较小的字符处理，把[]byte看作数据量大的字节流处理

对字符串的处理都在strings和bytes两个包中，这两个包的api高度相似

# Builder、Buffer
在各种语言中都会将string看作不可变量，如果想改变就要重新复制，对于过多次数的改变会造成频繁内存复制开销，所以提供了这两个方法

如果使用 []byte 保存字符串，扩容会造成复制开销，但改变不会，Builder和Buffer都是基于 []byte

注意：他们两个也实现了Writer接口

```
builder := &strings.Builder{}
builder.Grow(4*1024) //扩容cap
builder.WriteString("nb")
builder.Write([]byte("nb"))

type Builder struct {
	addr *Builder // of receiver, to detect copies by value
	buf  []byte
}

//注意有这样一个小技巧
buf := []byte{}
str := "i am str"
buf = append(buf,str...)
```

Buffer也是对 []byte 的基本操作做了包装，并额外提供了reader和writer的功能
```
type Buffer
    func NewBuffer(buf []byte) *Buffer
    func NewBufferString(s string) *Buffer
    func (b *Buffer) Bytes() []byte
    func (b *Buffer) Cap() int
    func (b *Buffer) Grow(n int)
    func (b *Buffer) Len() int
    func (b *Buffer) Next(n int) []byte
    func (b *Buffer) Read(p []byte) (n int, err error)
    func (b *Buffer) ReadByte() (byte, error)
    func (b *Buffer) ReadBytes(delim byte) (line []byte, err error)
    func (b *Buffer) ReadFrom(r io.Reader) (n int64, err error)
    func (b *Buffer) ReadRune() (r rune, size int, err error)
    func (b *Buffer) ReadString(delim byte) (line string, err error)
    func (b *Buffer) Reset()
    func (b *Buffer) String() string
    func (b *Buffer) Truncate(n int)
    func (b *Buffer) UnreadByte() error
    func (b *Buffer) UnreadRune() error
    func (b *Buffer) Write(p []byte) (n int, err error)
    func (b *Buffer) WriteByte(c byte) error
    func (b *Buffer) WriteRune(r rune) (n int, err error)
    func (b *Buffer) WriteString(s string) (n int, err error)
    func (b *Buffer) WriteTo(w io.Writer) (n int64, err error)
```

# Reader
strings和bytes都提供了Reader对象用作字符串和Reader接口之间的适配器，几乎所有数据对象都有围绕着IO接口的操作或适配器，例如：os、net等

```
//将string或 []byte 包装为 Reader 对象
bytes.NewReader([]byte{})
strings.NewReader("")

type Reader
    func NewReader(s string) *Reader
    func (r *Reader) Len() int
    func (r *Reader) Read(b []byte) (n int, err error)
    func (r *Reader) ReadAt(b []byte, off int64) (n int, err error)
    func (r *Reader) ReadByte() (byte, error)
    func (r *Reader) ReadRune() (ch rune, size int, err error)
    func (r *Reader) Reset(s string)
    func (r *Reader) Seek(offset int64, whence int) (int64, error)
    func (r *Reader) Size() int64
    func (r *Reader) UnreadByte() error
    func (r *Reader) UnreadRune() error
    func (r *Reader) WriteTo(w io.Writer) (n int64, err error)
```

# 字符串操作
其他对于字符串的操作都可以在这两个库里去找