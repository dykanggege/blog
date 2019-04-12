# ifconfig
ifconfig 或 ip addr 用来查看本机网络地址，推荐使用ip系列命令

```
    ip addr //打印所有网络信息
    1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
    2: enp3s0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast state DOWN group default qlen 1000
        link/ether 8c:16:45:4a:0b:a3 brd ff:ff:ff:ff:ff:ff
    3: wlp5s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP group default qlen 1000
        link/ether 70:c9:4e:dd:11:c1 brd ff:ff:ff:ff:ff:ff
        inet 192.168.1.107/24 brd 192.168.1.255 scope global dynamic noprefixroute wlp5s0
        valid_lft 6522sec preferred_lft 6522sec
        inet6 fe80::413f:f12c:b7a2:a95/64 scope link noprefixroute 
        valid_lft forever preferred_lft forever
    4: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default 
        link/ether 02:42:bd:17:57:17 brd ff:ff:ff:ff:ff:ff
        inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
        valid_lft forever preferred_lft forever
```
- lo/enp3s0：网络名称
- mtu：最大传输单元
- state：网络状态,UP打开，DOWN关闭
- link/：网络类型
- sope：范围，host对内，只能在本机使用，global对外，可以接受来自任意地方的包
- link/ether：mac地址
- inet：网络地址、brd：广播地址

每个主机都有一个回环网络接口lo用来连接自身，用来测试网络程序。固定的IP地址是127.0.0.1(localhost)

# ipv4
最早在制定IP协议考虑较少，使用32位，每8位用 . 分隔，史称ipv4

他被分为五类，每类又分为网络号和主机号

![](img/1.jpg)

![](img/2.jpg)

看着都头疼，幸好大家根本不用五类地址

这个网络号一般用于路由器，主机用于路由器下每个主机，但是分配的非常扯淡，大的大小的小，所以就有新的解决方案

## 无类型域间选路CIDR
它打破了固定的五类，将 32 位的 IP 地址一分为二，前面是网络号，后面是主机号，以/作为分隔

    10.100.122.2/24 表示，32位中前24位是网络号，后8位是主机号

伴随CIDR的是广播地址和子网掩码

我们知道网络号是 10.100.122 那么 10.100.122.255 (即主机号的最大值)则是该网络下的广播地址，所有在该网络下的主机都能收到

子网掩码是 255.255.255.0 将ip地址和子网掩码做AND运算，可以得到网络地址

# 公有ip和私有ip
前面表格中的五类都有对应的私有ip，例如我们的路由器被分配了一个公有ip 192.6.6.6，即在c类网络中，那么路由器下的所有主机访问192.168.0.0-192.168.255.255都会被路由器拦截，作为内网ip访问，而不会向外转播

即公网ip用于全球访问或路由器访问，私网ip是路由器下局域网内的访问，只会被路由器转接到内网主机，不会被发送出去

一般192.168.0.x是最常用的局域网内访问地址，很少有家庭局域网超过255个主机，所以家庭路由器内网ip通常是192.168.0.1/24，那么广播地址就是192.168.0.255

# mac地址
mac地址即网卡地址，每个网卡都有一个固定的地址，这是他被生产出来时就写好了，他是48位6字节，用16进制表示，如：8c:16:45:4a:0b:a3

既然有了ip，为什么还需要网卡呢。就像每个人都有一个唯一的身份证号，这是我们的mac地址，但是人在不停的转移地点，如果A想去找B，首先要打探到B所在的位置，位置一般不容易变化，比如上海陆家嘴再过几年还是陆家嘴，我们知道到这里后告诉其他人自己的地址，等他找到我后，再确定我的身份。所以我们需要定位和确定的身份，ip和mac分别提供了对应的功能

# 状态标识
```
    enp3s0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast state DOWN group default qlen 1000
            link/ether 8c:16:45:4a:0b:a3 brd ff:ff:ff:ff:ff:ff
```
<BROADCAST,MULTICAST,UP,LOWER_UP>UP 表示网卡处于启动的状态；BROADCAST 表示这个网卡有广播地址，可以发送广播包；MULTICAST 表示网卡可可以发送多播包；UP 表示它是启动的，也即连上网了

MTU表示链路层的最大传输单元，他是第二层MAC层的概念。以太网规定连带MAC头加正文最大传输数据长度不能超过1500字节。这里面有MAC头，IP头，TCP头、HTTP头、HTTP体，如果传输不下，就需要分片传输，一般tcp会提前获取MTU的最大长度，将报文段长度控制在MTU之内

qdisc pfifo_fast 是什么意思呢？qdisc 全称是queueing discipline，即排队规则。内核如果要通过某个网络接口发送数据包，他都需要按照为这个接口配置的排队规则把数据包加入队列。

最简单的qdisc是pfifo，它不对数据包做任何处理，采用先进先出的方式，pfifo_fast略复杂，它分为三个波段(band)，每个波段都是先进先出但优先级不同，band0>band1>band2。数据包按照IP报里TOS字段被分配高优先级还是低优先级


# 配置ip地址
```

    sudo ifconfig eth1 10.0.0.1/24 //配置网卡的ip地址
    sudo ifconfig eth1 up //重启网卡

```

ip的确可以随意配置，但是配置了能否生效呢，假设我们配置完成之后ping 192.168.1.6(旁边的电脑)

首先linux会看要访问的地址和本机地址是否再一个网段，如果在一个网段下，发送ARP请求，向网关请求对方的MAC地址，然后就可以发送包了

linux默认的逻辑是：如果这是一个跨网段的传输，他就不会把包发到网络上(而且还没有MAC地址，没法发送)，而是企图交给网关处理

这时候就要在本机查询网关地址，如果没配置网关，那么包压根就发不出去，如果想要配置网关为192.168.1.6，linux根本不会让你配置成功的，因为你的ip和网关不在一个网段下

## DHCP动态主机配置协议
既然配置IP这么麻烦，而我们经常要换地方，每次配置一次要累死，这时就用到了DHCP

当一个机器加入网络并没有IP地址，它会使用0.0.0.0地址(因为他现在还没有IP地址)发送一个广播包，目的地址为255.255.255.255(它也不知道DHCP Server的地址)，本机端口号68，目的端口号67，并使用自己的MAC地址，目的MAC地址为FFFF.FFFF.FFFF

可以发现，区域广播有两种方式，第一种是本网段下的主机最大值，第二种是255.255.255.255，它会被发送到局域网下所有主机，但是网关不会将它发送到外部网络。广播只能用UDP包

区域网中有一个或多个主机是用来做HDCP Server，它接受到这个UDP包后，解析其MAC地址，就知道来了个新人，这时就体会到了MAC地址唯一的重要性了。HDCP会给它分配一个IP地址，并附带子网掩码、网关、IP租用信息等广播出去

幸运的话，这个机器能收到多个响应，它会选择一个HDCP offer，并回复一个广播，携带客户端的MAC和接受的HDCP服务器的地址，告诉他我接收了这个IP，并谢谢其他HDCP，HDCP回复一个ACK表示把这个IP分给他了，拢共四个报文

当租期超过50%时，主机会向HDCP服务器发一个续约的报文

有一个有意思的细节是：网络管理员不仅能自动分配IP地址，还能帮你安装操作系统！

