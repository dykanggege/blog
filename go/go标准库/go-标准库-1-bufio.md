# bufio
bufio 包实现了缓冲的 IO，它包装了一个 io.Reader 和 io.Writer 对象，实现接口，而且提供了缓冲功能和对文本 IO 的便捷操作

## func ScanBytes(data []byte, atEOF bool) (advance int, token []byte, err error)
```
    //读取一个字节，如果字符能被一个字节编码，变为 string 不会失去本意
    bs := []byte("kanggege")
	advance, token, err := bufio.ScanBytes(bs, true)
	if err == nil{
		fmt.Println(token) //[107]
		fmt.Println("read "+strconv.Itoa(advance)+" byte "+string(token)) //read 1 byte k
	}else{
		fmt.Println(err)
	}

    bs := []byte("康搁搁")
	advance, token, err := bufio.ScanBytes(bs, true) 
	if err == nil{
		fmt.Println(token) //[229] 
		fmt.Println("read "+strconv.Itoa(advance)+" byte "+string(token)) //乱码
	}else{
		fmt.Println(err)
	}

```
## func ScanRunes(data []byte, atEOF bool) (advance int, token []byte, err error)
rune 代表一个 utf-8 字符，他能够完全读取一个 utf-8 字符