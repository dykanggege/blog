# 四大基本接口
linux下将一切IO设备抽象为文件，不讨论这种做法的好坏，文件一定会有read和write，所以IO包的基础是这两个接口

一般会加上 er 表示这是一个接口

read本质是将内核读取到的数据数组复制到用户空间，同样write将用户空间数据数组复制到内核

## reader
```
    type Reader interface {
        Read(p []byte) (n int, err error)
    }
```

read向一个数组中读入数据，可能不会把数组读满，**无论是否读满会使用整个数组**，当读取到文件末尾 err == io.EOF 或 其他错误 err != nil 时，停止读取

安全的read调用如下
```
func getData(reader bufio.Reader) ([]byte,error){
	buf := make([]byte,20)
	res := make([]byte,0)
	for {
		n, err := reader.Read(buf)
		if n < 0 {
			return res,errors.New("读取到小于0的字节数")
		}
        res = append(res,buf...)
		if err == io.EOF{
			return res,nil
		}
		if err != nil{
			return res,err
		}
	}
}
```

如果是你自己实现整个接口，你不应该让read方法返回小于0的值，同时为了避免实现返回的 n < 0，你要处理这种情况

## writer
```
type Writer interface {
    Write(p []byte) (n int, err error)
}
```
write方法将数据中数据输出，返回数据数据大小 0 <= n <= len(p)，无法写入时返回错误，无论如何不应该修改p中数据

安全的write调用如下

```
func sendData(writer io.Writer,data []byte) error {
	l,sum := len(data),0
	for sum < l {
		n, err := writer.Write(data[sum:])
		if err != nil{
			return err
		}
		sum += n
	}
	return nil
}
```
## Seeker
```
type Seeker interface {
    Seek(offset int64, whence int) (int64, error)
}
```
设定偏移位置和偏移量，然后偏移一定的位置


## Closer
将设备抽象为文件，就不得不考虑临界区的问题，有些资源进程共享，无需持有和释放，有些资源需要临界区限制，必须提供释放资源的方法，即Close，不是所有IO都需要实现Close方法

## 其他
除了四大基本接口，还提供了其他特定场景用途的接口如ByteReader、RuneReader等


# 包装方法
上面的四个接口是IO操作最基本的接口，io包基于四大接口实现了其他的一些包装方法

## ReadAtLeast、ReadFull、WriteString
基本的reader反映了最真实的内核调用，但是丑陋且使用不方便，所以封装了上层这些方法

虽然丑陋和原始，但也代表了本质、通用和坚挺。自己写库也应该这样，要有虽然丑陋但反应本质的api，面向通用和未来，也要有对特定场景的封装

看下源码，其实和我们自己实现的差不多
```
func ReadAtLeast(r Reader, buf []byte, min int) (n int, err error) {
	if len(buf) < min {
		return 0, ErrShortBuffer
	}
	for n < min && err == nil {
		var nn int
		nn, err = r.Read(buf[n:])
		n += nn
	}
	if n >= min {
		err = nil
	} else if n > 0 && err == EOF {
		err = ErrUnexpectedEOF
	}
	return
}

```

	func ReadFull(r Reader, buf []byte) (n int, err error)

ReadFull是对ReadAtLeast的进一步封装简化，

对于writer，并没有实现WriteAtLeast，而是提供了WriteString

WriteString希望writer实现了StringWriter接口，然后能方便的一次调用全部写入，如果没有则还是调用write方法

```
type StringWriter interface {
    WriteString(s string) (n int, err error)
}
```

如果要写入的是byte，一般是从其他reader中读取的，可以使用copy

如果写入的是string，一般使我们自己要写入的，我们不得不考察writer是否实现了StringWriter接口，否则不得不处理没有完全写入的情况

总体来说不如提供WriteAtleast方法

## Copy、CopyBuffer

	func Copy(dst Writer, src Reader) (written int64, err error)

读取reader中数据写入writer中，当reader读取到末尾EOF后，不返回错误，只要切实发生异常才返回 err != nil

reader和writer是基础的io方法，unix提供了缓冲io：在用户空间分配额外的空间用于io缓冲，占用了额外的内存，单减少了系统调用，提高性能，根据《unix高级编程》测试，缓冲区在4kb之后性能不再提升，所以一般使用 4*1024 作为缓冲区大小(更大则浪费了内存还对性能提升无用)

copy使用32*1024作为缓冲，如果你实在怕浪费这一丁点的性能，可以用CopyBuffer自己定义缓冲区大小



# 包装对象
除了包装方法，还包装了一些对象，提供其他场景下的io
## pipe

pipe用于线程间共享内存的通讯，看起来类似unix的pipe，其底层使用的channel

本质就是将channel封装成对象实现了reader、writer接口，在channel和io之间搭建了桥梁，但操作仍偏底层，需要其他包做更高层的封装

```
func main() {
	wg := &sync.WaitGroup{}
	wg.Add(2)
	pr, pw := io.Pipe()
	go func() {
		defer wg.Done()
		buf := make([]byte,4*1024)
		n, err := pr.Read(buf)
		if err != nil {
			log.Fatal(err)
		}
		fmt.Println(string(buf[:n]))
	}()

	go func() {
		defer wg.Done()
		time.Sleep(time.Second * 3)
		pw.Write([]byte("send you"))
		pw.Close()
	}()
	wg.Wait()
}
```