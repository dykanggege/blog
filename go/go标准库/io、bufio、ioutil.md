# Reader、Writer、Seeker、Closer
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

## Closer
将设备抽象为文件，就不得不考虑临界区的问题，有些资源进程共享，无需持有和释放，有些资源需要临界区限制，必须提供释放资源的方法，即Close，不是所有IO都需要实现Close方法

