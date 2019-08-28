io包下的接口和方法都是将一切IO设备甚至其他抽象(进程、内存)都看做文件操作的通用方法，体现了unix一切皆文件的想法，实际上不同的IO设备或虚拟文件具有自己的特点，我们需要对不同的领域根据其特点抽象出更具有针对性的io方法

# ioutil
ioutil提供了基于io包补充的方法，和对文件操作封装的方法

## NopCloser

    func NopCloser(r io.Reader) io.ReadCloser

把一个不需要close的reader设备封装成ReadCloser，相当于接口间的适配器

## 文件操作
除了上面那个适配器，其他基本都是对文件操作

    func ReadDir(dirname string) ([]os.FileInfo, error)

读取文件夹下所有文件信息

    func ReadFile(filename string) ([]byte, error)
    func WriteFile(filename string, data []byte, perm os.FileMode) error

读取和写入文件

# bufio
bufio就是建立缓冲区，用额外的内存换取io性能

	read := bufio.NewReader(os.Stdin)

默认下使用 4*1024 作为缓冲区，一般情况下这个也是最好的选择

## bufio.Reader
将基础io的操作升级为缓冲io操作，除此之外添加了几个额外的功能

    read.ReadBytes('\n')
    read.ReadSlice('\n')
    read.ReadRune()
    read.ReadLine()

几个基础的扫描分割操作，其中ReadLine是比较低层次的函数，不建议使用，尽量用Scanner

有意思的是下面这个方法

    read.UnreadByte()
	read.ReadByte()

他们两个的功能正好相反

## Scanner

Scanner，总所周知，扫描器

```
scanner := bufio.NewScanner(os.Stdin)
scanner.Split(bufio.ScanWords)
for scanner.Scan() {
    text := scanner.Text()
    fmt.Println(text)
}
if err := scanner.Err(); err != nil{
    log.Fatal(err)
}

```
scanner.Split() 用来指定扫描规则，默认是扫描每一行，即以"\n"为分隔符，bufio包还提供了Scanbytes、ScanLines、ScanRunes、ScanWords等规则

注意：如果使用os.stdin作为reader，你在控制台输入的数据会被放入缓冲区，当你按下回车才会从缓冲区发送到程序中，scann才能获取数据并扫描

## bufio.Writer
writer也是具有缓冲区的写

```
writer := bufio.NewWriter(os.Stdout)
n, err := writer.WriteString("保证尽力写入，如果无法写入返回错误")
//这些数据都在缓冲区中，刷洗缓冲区让他都写入
err = writer.Flush()
```

