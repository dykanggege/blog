net包下提供了 tcp/ip、udp、dns 的编程接口，以及Unix域的socket

虽然本包提供了对网络原语的访问，大部分使用者只需要Dial、Listen和Accept函数提供的基本接口；以及相关的Conn和Listener接口。crypto/tls包提供了相同的接口和类似的Dial和Listen函数。

# 简述
一个简单的demo来帮助入门net包编程

server端

```

	listener, err := net.Listen("tcp", ":2000")
	fmt.Println("listen 2000...")
	if err != nil{
		panic(err)
	}
	for{
		conn, err := listener.Accept()
		if err != nil{
			log.Println(err)
			continue
		}

		go func(conn net.Conn) {
			reader := bufio.NewReader(conn)
			bytes, err := reader.ReadBytes('\r')
			if err != nil{
				log.Println(err)
				return
			}
			_, _ = conn.Write([]byte(bytes))
			_, _ = conn.Write([]byte("bye!\t"))
		}(conn)

	}

```

client端
```

	conn, err := net.Dial("tcp", "127.0.0.1:2000")
	if err != nil{
		panic(err)
	}
	_, err = conn.Write([]byte("client:kanggege\n"))
	_, err = conn.Write([]byte(conn.LocalAddr().String()+"\n"))
	_, err = conn.Write([]byte("\r"))
	if err != nil {
		log.Println(err)
		return
	}

	reader := bufio.NewReader(conn)
	s, err := reader.ReadString('\t')
	if err != nil{
		log.Println(err)
		return
	}
	fmt.Println(s)

```

这个demo简单的实现了tcp编程（udp也是一样的）。有意思的一点是数据的流动，双方建立连接后，就可以接受或发送对方的数据了，直到双方任意一方结束连接前，数据都会被接受并缓存，有意思的就是对数据的处理

在http协议中，以\r\n\r\n作为报文结束标识符，同时满足报文体长度=content-length，接收端就可以断开连接了。

我们在用tcp传输数据时，要么发送方发送完成数据后主动断开，要么接收方通过某种标志检测到数据结束，主动断开连接