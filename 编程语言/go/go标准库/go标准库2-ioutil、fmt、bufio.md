
# ioutil
ioutil 提供了封装过的对文件和目录的简单 io 操作

看一个遍历目录的操作
```
func allDir(path string)  {
	fileInfos, err := ioutil.ReadDir(path)

	if err != nil{
		fmt.Println(err)
		return
	}

	for _,f := range fileInfos{
		if !f.IsDir(){
			fmt.Println("file ",f.Name())
			return
		}
		fmt.Println("dir ",f.Name())
		allDir(path+"/"+f.Name())
	}
}
```

```
bufferString := bytes.NewBufferString("哈哈哈哈哈哈")
//读取并抛弃
bufferString.WriteTo(ioutil.Discard)
```

# fmt
fmt 提供了格式化的输入输出，非常类似于 c 的 print 和 scan，但是封装的更巧妙

# bufio
## func NewReader(rd io.Reader) *Reader
从一个普通的 reader 变成一个具有缓冲的 reader，默认的缓冲大小是 4096，如果打开文件，或者读取太长的字节流，可以让他变成一个 buff.Reader，否则，像读取几个字符这样的操作，就别再为它分配 4kb 的空间了

如果要读取的太大，而且我们能预先估测到大概大小，最好使用 func NewReaderSize(rd io.Reader, size int) *Reader

bufio 的缓冲大小是很重要的，Discard、Peek 方法都是以缓冲区作为基准，如果跳过或预读的字节大小是超过缓冲区大小的，就会失败，所以在处理时要注意预估大文本的缓冲，避免失败

bufio 里的 reader 适合大文本大数据的具有缓冲的读取（缓冲的分配也很有意思），它原有的方法都比较底层，包装为 Scanner 后可以对大文本进行自定义的切割扫描



## ReadSlice、ReadBytes、ReadString、ReadLine 
他们的作用相似，从包装过的 reader 中读取内容，但　ReaderSlice　和其他又有不同
```
reader := bufio.NewReader(strings.NewReader("我自\n横刀\n向天笑，哈哈\n哈哈\n哈哈哈"))
for i := 0; i < 4; i++ {
	line, err := reader.ReadSlice(byte('\n'))
	if err != nil{
		panic(err)
	}
	fmt.Print(string(line))
}
//我自
//横刀
//向天笑，哈哈
//哈哈
```

- ReadSlice返回的[]byte是指向Reader中的buffer，而不是copy一份返回
- ReadBytes、ReadString、ReadLine 都是指向 buffer，他们底层使用　ReadSlice　
- 使用 UnreadRune 可以在将指针移动回去
- 如果ReadSlice在找到界定符之前遇到了error，它就会返回缓存中所有的数据和错误本身（经常是 io.EOF）
- 如果在找到界定符之前缓存已经满了，ReadSlice会返回bufio.ErrBufferFull错误

```
reader := bufio.NewReader(os.Stdin)
isPrefix := true
for isPrefix {
	var line []byte
	var err error
	line, isPrefix, err = reader.ReadLine()
	if err != nil{
		panic(err)
	}
	fmt.Println(len(line))
	fmt.Print(string(line))
}
```
- ReadLine尝试返回单独的行，不包括行尾的换行符
- 如果一行大于缓存，isPrefix会被设置为true，同时返回该行的开始部分（等于缓存大小的部分）。该行剩余的部分就会在下次调用的时候返回

这个 ReadLine 有点智障，如果要读取的数据超过 4096 还要自己去拼接，所以要从　reader　中读取一行数据，可以这样
```
func easyReadLine()  {
	bufreader := bufio.NewReader(strings.NewReader("康搁搁"))
	line, err := bufreader.ReadBytes('\n')
	if err != nil{
		panic(err)
	}
	line = bytes.TrimRight(line,"\r\n")
}
```
这样既读取了一行，有去除了行尾的符号

在 go1.1 中使用了 Scanner 可以更方便的读取一行
```
file, err := os.Create("text.txt")
if err != nil{
	panic(err)
}
defer file.Close()
file.WriteString("哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈")
file.Seek(0,os.SEEK_SET)
fileScann := bufio.NewScanner(file)
fileScann.Split(bufio.ScanRunes)
for fileScann.Scan() {
	fmt.Println(fileScann.Text())
}
```
注意偏移量，所有指向 buf 并非 copy 的操作，底层都使用了偏移量，所以要在必要时清除偏移量

## func NewWriter(w io.Writer) *Writer
writer 和 reader 的方法都比较类似，简直是一对好基友

writer 也是有缓冲的，对他的写入先是在缓冲区内，当缓冲区满了自己调用　Flush　将缓存写入，很多写入方法都会在写入完成后　Flush　写入缓存

# demo
```
func readerMenu() {
	fmt.Println("请选择不同的输入源：")
	fmt.Println("1 控制台输入")
	fmt.Println("2 文件输入")
	fmt.Println("3 网络输入")
	fmt.Println("4 字符串输入")
	fmt.Println("b 返回上一菜单")
	fmt.Println("q 退出程序")
}

func readFromAny()  {
	for {
		readerMenu()

		var opt rune
		fmt.Scanf("%c\n", &opt)

		data := make([]byte,0)
		var err error

		switch opt {
		case '1':
			_, err = fmt.Scanln(&data)

		case '2':
			WRITEFILENAME:
			fmt.Println("请输入文件名\n")

			fname := ""
			_, e := fmt.Scanln(&fname)
			if e != nil{
				fmt.Println("文件名输入错误！\n")
				goto WRITEFILENAME
			}

			data, err = ioutil.ReadFile(fname)

		case '3':
			WRITEPORT:
			fmt.Println("使用TCP连接，请输入监听端口号\n")

			port := ""
			_, e := fmt.Scanln(&port)
			if e != nil{
				fmt.Println("端口号输入错误\n")
				break
			}

			listener, e := net.Listen("tcp", port)
			if e != nil{
				fmt.Println("端口号格式错误\n")
				goto WRITEPORT
			}

			fmt.Println("等待连接\n")

			conn, e := listener.Accept()
			if e != nil{
				fmt.Println(e,'\n')
				break
			}
			_, err = conn.Read(data)

		case '4':
			fmt.Println("请输入要读取的字符串，回车键结束\n")

			str := ""
			fmt.Scanln(&str)

			reader := strings.NewReader(str)
			ch, _, e := reader.ReadRune()
			data = []byte(strconv.QuoteRune(ch))
			err = e

		case 'b':
			return

		case 'q':
			os.Exit(2)

		default:
			fmt.Println("输入一个字符！！！！\n")
			break
		}

		if err != nil{
			fmt.Println("数据读取失败")
			fmt.Println(err)
			fmt.Println()
			continue
		}else{
			fmt.Println()
			fmt.Println("读取到数据：",string(data))
			fmt.Println()
		}
	}
}


```