# io
io 包抽象了标准库中所有 io 操作的底层接口和函数，它的很多方法属于底层方法，所以并不全部保证线程安全，go 的 io 操作，都是基于以下的几个接口展开的

```
type Reader interface {
    Read(p []byte) (n int, err error)
}

type Writer interface {
    Write(p []byte) (n int, err error)
}

type Closer interface {
    Close() error
}

type Seeker interface {
    Seek(offset int64, whence int) (int64, error)
}

```
不同的底层 io 方法对这些接口的实现也有所不同，但大多都是异步的，这也保证了 go 语言 io 的异步非阻塞

## func Pipe() (*PipeReader, *PipeWriter)
提供了同步的 io 操作，它希望 write 后就立刻被 read，否则线程将被阻塞，它是线程安全的

它有点像无缓冲的 channel

下面的代码将会堵死
```
    reader, writer := io.Pipe()
	fmt.Fprintf(writer,"这必须是同步的操作")
	err := writer.Close()
	if err != nil{
		panic(err)
	}

	buf := new(bytes.Buffer)
	buf.ReadFrom(reader)
	fmt.Println(buf.String())
```
必须保持再启动其他的线程同步接收或写入
```
    reader, writer := io.Pipe()
	go func() {
		fmt.Fprintf(writer,"这必须是同步的操作")
		err := writer.Close()
		if err != nil{
			panic(err)
		}
	}()

	buf := new(bytes.Buffer)
	buf.ReadFrom(reader)
	fmt.Println(buf.String())
```

# ioutil
ioutil 提供了封装过的对文件和目录的简单 io 操作

# bufio
## func NewReader(rd io.Reader) *Reader
从一个普通的 reader 变成一个具有缓冲的 reader，默认的缓冲大小是 4096，如果打开文件，或者读取太长的字节流，可以让他变成一个 buff.Reader，否则，像读取几个字符这样的操作，就别再为它分配 4kb 的空间了

如果要读取的太大，而且我们能预先估测到大概大小，最好使用 func NewReaderSize(rd io.Reader, size int) *Reader

bufio 的缓冲大小是很重要的，Discard、Peek 方法都是以缓冲区作为基准，如果跳过或预读的字节大小是超过缓冲区大小的，就会失败，所以在处理时要注意预估大文本的缓冲，避免失败

bufio 里的 reader 适合大文本大数据的具有缓冲的读取（缓冲的分配也很有意思），它原有的方法都比较底层，包装为 Scanner 后可以对大文本进行自定义的切割扫描

## func NewWriter(w io.Writer) *Writer
writer 和 reader 的方法都比较类似，简直是一对好基友

# fmt
fmt 提供了更接近控制台的输入输出，非常类似于 c 的 print 和 scan，但是封装的更巧妙