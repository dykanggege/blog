# 基本类型
## 整数
- int8=byte、int16、int32=rune、int64、uint8、uint16、uint32、uint64 内置整数类型，是有点丑，但是的确很直观
- int，根据所在硬件平台，编译为 32 或 64 位
- uintptr，其大小并不明确，但足以存下指针，仅用于底层编程

## 浮点数
- float32、float64、math.MaxFloat32=3.4028234663852886e+38、 math.MaxFloat64=1.7976931348623157e+308，为了避免误差的累积，一般都使用 float64
- math.isNaN()、math.NaN()，这里的 NaN 与 js 中相同

## 布尔值
使用布尔值进行判断时，不得不提短路，如果左边的操作符已经判断出结果，则右边不会进行
```

	  func main()  {
	    if true || getT(){ //getT不会被调用
	      fmt.Println("final")
	    }
	  }
	
	  func getT() bool {
	    fmt.Println(true) 
	    return true
	  }

```
布尔值无法和整数进行直接转换，整数也无法代替布尔值用于判断

## 字符串
字符串是不可变的字节序列，编译时会被直接翻译为 UTF-8 编码的 Unicode 码点，操作字符串时需要注意一下几点：
- len(str) 得到字节数 len("康搁搁") == 9
- str[i] 取得第i个码值(类似于汉字这样的，一个字由多个码值组成，只取出单一码值则会被翻译为其他字)
- 使用 for range 则会将码值完整打印，如果要对字符串中每个字符逐一访问，最好使用这种方式
```

	  s := "康搁搁"
	  for i,v := range s{
	    fmt.Printf("index %d value %q\n",i,v)
	  }
	  index 0 value '康'
	  index 3 value '搁'
	  index 6 value '搁'

```
- 字符串可以通过 == 作比较，< 比较则按字典排序
```

	  s := "康搁"
	  a := "帅帅哒"
	  fmt.Println(s > a) //true

```
- 尽管可以通过 + 连接字符串，但是原字符串是不会变的，这样只是又创建了一个字符串（过多的此类操作可能会浪费太多的内存）
- 字符串是基本类型量，每次赋值底层都是复制，为了避免不必要的内存分配，可以使用 bytes 和 strings 包下的一些函数，可以使用 bytes.Buffer 更高效的处理字符串
- 可以对字符串进行切片操作，实际上使用的是同一个底层字符串，不会再次分配空间，但是注意分割位置，因为下标是字节位置，**字符串可以和字符串切片相互转换**
- 不可变意味着 s[i]="k" 是无法通过编译的，同时也为字符串的复制减少了内存消耗，因为他们使用的是相同的底层内存
- 在使用双引号的字符串字面量中，我们可以直接用 Unicode 码点书写，也可以插入转义字符，都会通过编译生成对应的字符，如果不想要编译而是原生的字符串，使用 \`str\`，它唯一做的处理就是：回车符会被删除

## 字符串操作
```

	//分析文件名
	func baseName(s string) string {
	  for l := len(s) - 1; l >= 0; l-- {
	    if s[l] == '/' {
	      s = s[l+1:] //字符串类型也可以视作切片类型
	      break
	    }
	  }
	  for l := len(s) - 1; l >= 0; l-- {
	    if s[l] == '.' {
	      return s[:l]
	    }
	  }
	  return ""
	}
	
	// 数组转换为字符串
	func intsToString(values []int) string{
	  var buf bytes.Buffer //起始值为空，随着写入数据而增长
	  buf.WriteByte('[')
	  for i,v := range values{
	    if i>0 {
	      buf.WriteByte(',')
	    }
	    fmt.Fprintf(&buf,"%d",v)
	  }
	  buf.WriteByte(']')
	  return buf.String()
	}
	
	//字符串和数字之间的转换，常用 strconv 包
	x, err := strconv.Atoi("123")

```
## 常量
**常量：可以在编译阶段就能计算出的量**
```

	const (
	  b = true
	  pi = 3.1415926
	  str = "i am str!"
	)

```
- 常量可以显式的指定类型，如果没有执行类型，则根据表达式的值自动判断类型
- 对于常量的操作数，所有的数学运算，逻辑运算，比较运算，得到的结果依然是常量
- 常量有些默认的效果
```

	const (
	  a = 1
	  b
	  c = 2
	  d
	  e
	)
	fmt.Println(a, b, c, d, e) //1 1 2 2 2
	
	type weekday int
	const (
	  Sunday weekday = iota //可以看做常量的下标值
	  Monday
	  Tuesday
	  Wednesday
	  Thursday
	  Friday
	  Staturday
	)
	fmt.Println(Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Staturday)
	//0 1 2 3 4 5 6

```
- 无类型常量：go 的类型常量最大特点在于无类型，它在编译器中可以被编译为内置类型精度更高的量，这样它不仅维持了高精度，还可以用作多种表达式中而无需类型转换，字面直接量也是常量