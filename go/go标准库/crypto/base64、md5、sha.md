# base64
严格来说base64不算加密，只能是一种二进制表示方式。

先创建一个64位的数组，类似这样的['A', 'B', 'C', ... 'a', 'b', 'c', ... '0', '1', ... '+', '/']，然后将二进制每6位作为一个整体得到值，作为下标去访问数组，得到数组里的值，这样就将6位的值变成8位了，数据了增加了33%，但是可读性提升了

通常是将3字节的二进制编码为4字节的base64数据，如果最后不够3字节了，可以选择

1. 往最后补足，补了几个字节就在最后加上几个=，解码的时候会自动去掉=

```
    src := make([]byte,10)
	base64.StdEncoding.Encode(src,[]byte{54,54,54,54})
	fmt.Println(string(src))
```

2. 其实不补=也行，因为生成的一定是4的倍数，如果不是4的倍数，最后一定有=


# md5、sha
md5是一个生成16位摘要的单向加密算法，sha也是单向加密算法，加密强度比md5更高，通常使用sha256生成32位摘要

他们都提供了开箱即用的方法

```
    bs := md5.Sum([]byte("data"))
    fmt.Println("md5 len:",strconv.Itoa(len(bs)))
    fmt.Println(base64.StdEncoding.EncodeToString(bs[:]))

    fmt.Println()

    sum256 := sha256.Sum256([]byte("data"))
    fmt.Println("sha256 len:",strconv.Itoa(len(sum256)))
    fmt.Println(base64.StdEncoding.EncodeToString(sum256[:]))
```

他们本质都是hash函数，所以可以先转换为hash函数接口，并再调用一些hash的方法

```

    hash := sha256.New()
	hash.Write([]byte("加密的数据"))
	sum := hash.Sum([]byte("向加密完成的数据前添加的明文数据"))

```