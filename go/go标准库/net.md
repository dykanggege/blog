# 传输层
顶层操作，一般都建议使用这个接口

```
//"tcp", "tcp4", "tcp6", "unix" or "unixpacket"
//if the host in the address parameter is empty or a literal unspecified IP address, Listen listens on all available
//If the port in the address parameter is empty or "0", as in  "127.0.0.1:" or "[::1]:0", a port number is automatically chosen.
//注意不能监听udp
ln, err := net.Listen("tcp", "")
errHandle(err)
fmt.Println(ln.Addr())
for {
    conn, err := ln.Accept()
    if err != nil { continue }
}
```

所有关于传输层的接口，地址、socket、conn
```
type Addr interface {
	Network() string // name of the network (for example, "tcp", "udp")
	String() string  // string form of address (for example, "192.0.2.1:25", "[2001:db8::1]:80")
}

type Listener interface {
	Accept() (Conn, error)

	Close() error

	Addr() Addr
}

type Conn interface {
	Read(b []byte) (n int, err error)

	Write(b []byte) (n int, err error)

	Close() error

	LocalAddr() Addr

	RemoteAddr() Addr

	SetDeadline(t time.Time) error

	SetReadDeadline(t time.Time) error

	SetWriteDeadline(t time.Time) error
}
```

## tcp
上面的三大接口是将tcp、udp共同抽象出来通用的接口
```
//解析tcpaddr
func ResolveTCPAddr(network, address string) (*TCPAddr, error) {
	switch network {
	case "tcp", "tcp4", "tcp6":
	case "": // a hint wildcard for Go 1.0 undocumented behavior
		network = "tcp"
	default:
		return nil, UnknownNetworkError(network)
	}
	...
}

func ListenTCP(network string, laddr *TCPAddr) (*TCPListener, error) {
	switch network {
	case "tcp", "tcp4", "tcp6":
	default:
		return nil, &OpError{Op: "listen", Net: network, Source: nil, Addr: laddr.opAddr(), Err: UnknownNetworkError(network)}
	}
	if laddr == nil {
		laddr = &TCPAddr{}
	}
	...
}


addr, err := net.ResolveTCPAddr("tcp","127.0.0.1:8899")
errHandle(err)
ln, err := net.ListenTCP("tcp", addr)
errHandle(err)
```

除了通用接口的API实现，socket还提供一些额外的功能

```
//AcceptTcp和Accept一毛一样，可以直接在Accept中调用AcceptTcp，或者可以继承重写Accept，用包装过的AcceptTcp实现通用接口
func (l *TCPListener) AcceptTCP() (*TCPConn, error) {
	if !l.ok() {
		return nil, syscall.EINVAL
	}
	c, err := l.accept()
	if err != nil {
		return nil, &OpError{Op: "accept", Net: l.fd.net, Source: nil, Addr: l.fd.laddr, Err: err}
	}
	return c, nil
}

func (l *TCPListener) Accept() (Conn, error) {
	if !l.ok() {
		return nil, syscall.EINVAL
	}
	c, err := l.accept()
	if err != nil {
		return nil, &OpError{Op: "accept", Net: l.fd.net, Source: nil, Addr: l.fd.laddr, Err: err}
	}
	return c, nil
}


ln.File() //返回底层的io文件
ln.SetDeadline() //设置accept的deadline
ln.SyscallConn() //使用系统调用，不建议使用
```

conn也有额外的功能，而且很多...
```
tcpConn, err := ln.AcceptTCP()

//加了个语法糖
tcpConn.ReadFrom(os.Stdin)
//如果closeWrite，对于还未发送完的数据的处理方式
tcpConn.SetLinger(0)
//众所周知，tcp是全双工模式
tcpConn.CloseWrite()
tcpConn.CloseRead()
```

最有意思的是从系统层面支持了keepalive

建立tcp练级的双方如果有一方网线断了，tcp连接并不会中断，如果使用类似SetReadDeadline的方法，就要定时发送自定义的心跳包避免空闲连接被挂掉

TCP keepalive定义于RFC 1122，但并不是规范的一部分，它可以在个别的连接中启用，但默认必需是关闭的，启用它会使网络栈在空闲了特定时间后(不能低于2小时)探测连接的连接状况。探测包不能包含数据，并且一个探测包的回复的失败不能将连接看作已中断，因为探测包的传输是不可靠的。

GO 可以通过 net.TCPConn 的 SetKeepAlive 来启用 TCP keepalive。在 OS X 和 Linux 系统上，当一个连接空间了2个小时时，会以75秒的间隔发送8个TCP keepalive探测包。换句话说， 在两小时10分钟后(7200+8*75)Read将会返回一个 io.EOF 错误.

对于你的应用，这个超时间隔可能太长了。在这种情况下你可以调用SetKeepAlivePeriod方法，限制一个空闲连接存活时间

```
tcpConn.SetKeepAlive(true)
tcpConn.SetKeepAlivePeriod(time.Minute*3)
```


