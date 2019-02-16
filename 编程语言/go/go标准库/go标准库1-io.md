	ｇｏ中的ＩＯ操作主要由ｉｏ、ｉｏ/ｉｏｕｔｉｌ、ｆｍｔ、ｂｕｆｉｏ实现，他们各有自己的特点
	- io 为 ＩＯ原语提供基本的接口，他的方法都是异步ｉｏ，并不保证所有的方法都是并发安全的
	- io/ioutil　主要提供了对文件操作的辅助ｉｏ和一些常用、方便的IO操作函数
	- bufio 基本的ｉｏ原语函数调用都需要传入一个缓冲字节，bufio　提供了一个默认的缓冲字节长度，并实现了 io 中常用的接口，可以看做是对原语ｉｏ操作包装，实现　io 包的接口
	- fmt　提供了格式化的ｉｏ

	**并不列举所有的ＡＰＩ，只对我频繁用到的做介绍**

	# io

	io 包抽象了标准库中所有 io 操作的底层接口和函数，它的很多方法属于底层方法，所以并不全部保证线程安全，go 的 io 操作，都是基于以下的两个接口展开的

	```
	type Reader interface {
		Read(p []byte) (n int, err error)
	}

	type Writer interface {
		Write(p []byte) (n int, err error)
	}

	```
	## Reader

	>Read 将 len(p) 个字节读取到 p 中。它返回读取的字节数 n（0 <= n <= len(p)） 以及任何遇到的错误。即使 Read 返回的 n < len(p)，它也会在调用过程中占用 len(p) 个字节作为暂存空间。若可读取的数据不到 len(p) 个字节，Read 会返回可用数据，而不是等待更多数据。
	>
	>当 Read 在成功读取 n > 0 个字节后遇到一个错误或 EOF (end-of-file)，它会返回读取的字节数。它可能会同时在本次的调用中返回一个non-nil错误,或在下一次的调用中返回这个错误（且 n 为 0）。 一般情况下, Reader会返回一个非0字节数n, 若 n = len(p) 个字节从输入源的结尾处由 Read 返回，Read可能返回 err == EOF 或者 err == nil。并且之后的 Read() 都应该返回 (n:0, err:EOF)。
	>
	>调用者在考虑错误之前应当首先处理返回的数据。这样做可以正确地处理在读取一些字节后产生的 I/O 错误，同时允许EOF的出现。
	>
	>io包下，var EOF = errors.New("EOF")


	看一个简单的例子

	```
	//从一个输入源中读取 num 字节
	func readNumFrom(r io.Reader,num int) ([]byte,error) {
		b := make([]byte,num)
		n, err := r.Read(b)
		if n > 0{
			return b[:n],nil
		}
		return b,err
	}
	```

	## Writer

	>Write 将 len(p) 个字节从 p 中写入到基本数据流中。它返回从 p 中被写入的字节数 n（0 <= n <= len(p)）以及任何遇到的引起写入提前停止的错误。若 Write 返回的 n < len(p)，它就必须返回一个 非nil 的错误。

	```
	func println(d ...interface{}) (int,error) {
		return fmt.Fprintln(os.Stdout,d...)
	}
	//print等方法，底层调用的都是 Fprint
	```

	ｇｏ中ｉｏ的思想就是，不管你是啥，只要你实现了这个接口，我就能 Write/Read，屏蔽底层的实现细节，很简洁的ｉｏ流

	即使是不同的高层次ＩＯ操作，都同样要实现这两个基本接口，他们从根本上就确定是异步缓存的，所以ｇｏ中的ｉｏ天生异步


	## func Pipe() (*PipeReader, *PipeWriter)
	提供了同步的 io 操作，它希望 write 后就立刻被 read，否则线程将被阻塞，它是线程安全的，并希望被用于两个以上的线程

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

	## MultiReader 和 MultiWriter 函数

	>func MultiReader(readers ...Reader) Reader
	>func MultiWriter(writers ...Writer) Writer

	它们接收多个 Reader 或 Writer，返回一个 Reader 或 Writer。我们可以猜想到这两个函数就是操作多个 Reader 或 Writer 就像操作一个

	```
	readers := []io.Reader{
		strings.NewReader("string reader"),
		bytes.NewBufferString("[]bytes reader"),
	}

	multireader := io.MultiReader(readers...)

	r := bufio.NewReader(multireader)

	buf := new(bytes.Buffer)

	buf.ReadFrom(r)
	fmt.Println(buf.String())
	```

	```
	func multiWriter()  {
		file, err := os.Create("test.txt")
		if err != nil{
			panic(err)
		}
		defer file.Close()

		writers := []io.Writer{
			file,
			os.Stdout,
		}

		mulwri := io.MultiWriter(writers...)
		writer := bufio.NewWriter(mulwri)
		writer.WriteString("康搁搁")
		writer.Flush()
		
	}
	```

	## TeeReader

	>func TeeReader(r Reader, w Writer) Reader

	TeeReader 返回一个 Reader，它将从 r 中读到的数据写入 w 中。所有经由它处理的从 r 的读取都匹配于对应的对 w 的写入。它没有内部缓存，即写入必须在读取完成前完成。任何在写入时遇到的错误都将作为读取错误返回

	它不同于 io.copy 的地方是，TeeReader 是流式的数据传输，无缓存

	```
	reader := io.TeeReader(os.Stdin, os.Stdout)
	reader.Read(make([]byte,200))
	```



