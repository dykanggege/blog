# 数组
```

	var a [3]int //[3]int 是一个数组类型，即使是同一type，但不同长度的数组，类型也不相同
	for _, v := range a {
	  fmt.Println(v) // 0 0 0 数字会使用类型的默认值初始化
	}
	
	var b [3]int = [3]int{1, 2, 3} // 声明时初始化，数组的长度必须是常量表达式
	//可以使用 {} 赋值，意味着它的本质也像是结构体（从汇编来看和结构体的访问相同，都是使用地址的偏移）
	//看下面的用法

	type intarr [10]int  //使用类型定义
	intarrs := intarr{1, 2, 3, 4, 5}  //翻译过来就是 [10]int{1,2,3,4,5}
	fmt.Println(intarrs) //[1 2 3 4 5 0 0 0 0 0]

	var intArr [5]int
	intArr = [5]int{1,2,3,4,5}
	// 先声明后再初始化，即使已经声明过了，一次性初始化还是要用 [5]int{} 这样好像略为死板，但是这也提示了最好声明时直接初始化，否则使用 intArr[i] = val
	
	c := [...]int{1, 2, 3, 4, 5} //根据初始化的值动态决定长度
	fmt.Println(len(b), len(c))  // 3 5
	
	// c = b 会报错，数组长度不同类型也不同
	
	d := [...]int{99: 99}          //根据索引位置动态赋值
	fmt.Println(d[99]+1 == len(d)) // true
	fmt.Println(d[0])              // 0
	
	fmt.Println(a == b) //相同数组类型间可以比较，如果数组元素都相同，返回 true
	
	func changeArr(arr [3]int) [3]int {
	  for i, _ := range arr {
	    arr[i] = i + 1
	  }
	  return arr
	}
	
	e := changeArr(a)
	fmt.Println(a == e) // false 数组赋值是值传递，即，数组并非引用类型
	
	f := [3]int{1,2,3}
	p1 := &f
	f = [3]int{4,5,6} //单纯的赋值操作
	fmt.Println(&f == p1) // true

```

数组的定长类型和值传递，使得我们并不常用它，而是选择更好用的 slice

# slice
- 切片：相同类型元素的可变长度序列,[]T
- slice的底层还是数组，它的本质是一个数据结构，有三个属性
  - 以 arr [30]int; sli = arr[5:10] 做例子
  - 指针：指向底层数组，sli保存的指针指向 arr[5]，以地址偏移访问
  - len：切片的长度，len(slice)得到长度，len = 10 - 5 = 5，对切片的下标访问不能超过 len
  - cap: 从slice的起始位置，到最底层数组最后一个元素的长度，cap(slice)得到容量，即 cap = 30 - 5 = 25，cap是个很有意思的值，后面会专门介绍它
- slice := s[i:j]，0 <= i <= j <= len(s)， 省略 i 时，默认 i 为 0，省略 j 时，j 为 len(s)，arr[0:len(arr)] 拷贝整个数组，请记住它
- 因为 slice 是指向数组的某个位置，所以不能用 == 比较两个 slice 是否有相同的元素，这也使得 slice 无法作为 map 的 key，可以使用 bytes.Equal()比较 []byte，其他的需要自己写函数了

```
	
	intArr := [10]int{5:0}
	intSlice := intArr[2:5]
	fmt.Println("len:",len(intSlice)) // len: 3
	fmt.Println("cap:",cap(intSlice)) // cap: 8

```

这里有一个很容易让新手搞乱的地方（至少我曾乱过...）

- 声明数组：[10]int{}
- 得到切片：intArr[5:10]

我曾把切片写成 [5:10]intArr

golang 选择把类型的声明放在后面，这样就避免了前缀太多而造成识别命名困难,因为大多时候是想先找到名再看类型

```

	int (*(*fp)(int (*)(int, int), int))(int, int)
	//c 代码，至少我一眼难以看出函数名是 fp
	public static List<Map<String, String>> funcName () {} 
	//java 代码，其实大多时候我们更想先找到函数名或变量名，再去看他便秘一样的前缀

```

操作则在变量后面放着，而切片是对数组的操作，所以是 intArr[5:10]

接着看切片的例子

```

	func reverse(s []int) {
		for i, j := 0, len(s)-1; i < j; i, j = i+1, j-1 {
			s[i], s[j] = s[j], s[i]
		}
	}
	
	arr := [...]int{1,2,3,4,5,6,7}
	// reverse(arr) 错误，因为数组类型并不是slice类型
	s := arr[:]
	//或者直接可以写为 s := []int{1,2,3}，它的底层自动分配了数组，但我们不用关心
	reverse(s)
	fmt.Println(s)
	
	str := "kanggege康搁搁"
	fmt.Println(str[8])
	strS := str[:]
	fmt.Println(strS) //"kanggege康搁搁" 还是字符串，并不能算作 slice 类型
	//fmt.Printf(strS == nil) 错误，对字符串做 slice 操作，得到的还是字符串，字符串不是引用类型不能和 nil 比较
	fmt.Println(s == nil) // slice 唯一可以做的比较
	fmt.Println(reflect.TypeOf(strS[13])) //uint8

```
## slice 操作
### make
```

	s := make([]int,5,10) //或 make([]T,len,cap) cap可以省略，只写len
	for i,v := range s{
		print(i) //01234
	}

```

使用 make 创建 slice 的本质还是创建一个指定长度的匿名数组，并返回一个指向该匿名数组的 slice

### append
```

	func appendInt(x []int, y int) []int {
		var z []int
		zlen := len(x) + 1
		if zlen <= cap(x) {
			// 本来是不允许访问超过 len 的下标，但是对 append 的访问没有做限制，他可以超过 len 但要小于 cap
			// 这会超过len下标的改变原数组，要小心使用
			z = x[:zlen]
		} else {
			// 使用新的数组了！
			zcap := zlen
			if zcap < 2*len(x) {
				zcap = 2 * len(x)
			}
			z = make([]int, zlen, zcap)
			copy(z, x)
		}
		z[len(x)] = y
		return z
	}

	s := make([]int, 0)
	s = append(s,1,2,3,4,5)
	s2 := []int{6,7,8,9}
	s = append(s,s2...)
	print(len(s)) // 9

```

append([]Type, ...Type)

从上面源码分析可以看出来，对 sli 做 append 操作时，有太多不稳定性，比如：可能会越过 len 修改原数组的值，可能会被分配的新的数组

建议：尽量对 sli = make([]int,10) 这种切片做 append 操作，对指向已存在数组的切片做 append 有太大不稳定性

当切片容量不够会从新分配底层数组使其增长，如果这样的次数太多势必会影响性能，所以我们在 make 切片时，尽量一步到位

### copy
copy(s1,s2) 将 s2 的内容复制到 s1 中
```
	s1 := []int{1,2,3,4,5}
	s2 := []int{3,2,1}

	copy(s1,s2)
	fmt.Println(s1) //[3 2 1 4 5]

	s3 := []int{1,2,3,4,5}
	copy(s2,s3)
	fmt.Println(s2)	//[1 2 3]

```

### 传递
我们之前说过 slice 是引用类型，让我们在详细的去审视一下这个引用类型，它和我们在 java 中说的引用类型不太一样
```
	func changeLen(s []int) []int {
		return s[:len(s)/2]
	}

	func main() {
		s := make([]int,20)
		fmt.Println(len(s)) //20
		fmt.Println(s[0]) //0
		s2 := changeLen(s)
		fmt.Println(len(s)) //20
		fmt.Println(s[0]) //666
		fmt.Println(len(s2)) //10
		fmt.Println(s2[0]) //666
	}


```
如果安装 java 中引用类型的定义，s 的长度也会改变，实际上只改变了底层数组而长度没变。

我们可以简单的将 slice 看作这样的结构体
	
	type slice struct{
		arr *[len]type
		len int
		cap int
	}

它是这样的一个结构体，再赋值时依旧是复制传递，由于内部使用的是数组指针，所以多个 slice 共同操作一个数组，一旦一个 slice 改变底层数组值，其他的随之改变，这样就造成了 slice 是引用类型的假象，**slice 的本质还是复制传递**

# map[k]v
```

	//下面两种初始化方式相同
	m1 := make(map[string]int)
	m1["index1"] = 1
	m1["index2"] = 2
	m1["index3"] = 3
	m2 := map[string]int{
		"index1":1,
		"index2":2,
	}

	delete(m2,"index1") //删除 map 中的元素
	fmt.Println(len(m2))  // 1
	fmt.Printf("m2[index3]=%v\n",m2["index3"]) // 访问不存在的下标会得到默认值

	//_ = &m1["index1"] map 中的元素可能会因为数量的增长而重新分配地址，所以获取元素地址无任何意义

	for key,val := range m1{
		fmt.Printf("key:%v, val:%v\n",key,val)
		//没有固定的迭代顺序，据说是为了安全，emmm，不太懂
	}
	
	//如果想要按一定顺序迭代，可以先对 key 排列，再得到值
	keys := make([]string,0)
	for key := range m1{
		keys = append(keys, key)
	}
	sort.Strings(keys)
	for _,key := range keys{
		fmt.Printf("m1[%v]=%v",key,m1[key])
	}

	//因为 map 是引用类型，所以除了和 nil 做比较无法和其他的 map 直接做比较，可以迭代比较
	func mapEqual(m1, m2 map[string]int) bool {
		if len(m1) != len(m2) {
			return false
		}
		for key := range m1 {
			if _, ok := m2[key]; !(ok && (m1[key] == m2[key])) {
				return false
			}
		}
		return true
	}

	//如果不确定 map 中是否存在一个值
	v,ok := m1["99999"]
	//如果存在，则 ok 是 true，否则 ok 是 false，v 是其类型的零值

```

就像分析 slice 的引用传递一样，看看 map 的引用传递。简单来说，可以视作如下结构体

type map_key_val struct{
	....
}

type map_impl struct{
	impl *map_key_val
}


# struct
- 结构体是由基本类型组成的聚集数据类型，它是值语义，同样以首字母是否大写控制是否可导出，其成员变量也是
- 结构体指针也可以直接使用结构体成员，且结构体是复制传递，所以经常使用结构体的指针
- 如果结构体的所有成员都可以比较，那么这个结构体就是可比较的，== 会按照顺序比较两个结构体变量的成员变量
- struct 和 java 中的 class 地位相同，但是 struct 只有组合的特性，通过组合可以灵活的构建 oop 特性，亦可不使用 oop

```

	type Person struct {
		Name   string  //public
		Age    int
		Sex    string
		person *Person // 包内可访问
	}
	
	type Student struct {
		Person //不能定义两个相同的匿名成员，否则会冲突
		Id    int
		Score float64
	}
	
	func main() {
		p1 := &Person{Name:"kanggege"}
		fmt.Println(p1.Name)

		p := Person{"kanggege",20,"man",p1}
		s := Student{Person:p,Id:20,}
		fmt.Println(p.person) //&{kanggege 0  <nil>}
		fmt.Println(s.person) //&{kanggege 0  <nil>}
		fmt.Println(s.Name)	//dyk
	}

```

# 类型系统
程序是用来处理数据的，所以数据类型是一门语言的基石，一门典型语言的类型系统应该包含以下部分

- 基础类型，如 int，bool，float 等
- 复合类型，如数组、结构体、指针等
- 可以指向任意对象的类型（any 类型）
- 值语义和引用语义
- 具有面向对象的特征的类型
- 接口

go 中的类型大多都是值语义，以 interface{} 作为 any 类型，使用结构体的组装实现 oop

## 值语义与引用语义
slice、map、channel、interface，比较特殊，他们看起来像是引用类型，实际只是底层数据结构维护着指针，依旧是值语义



# JSON
```
	type Movie struct{
		Title string
		Year int `json:"year"` //起别名，year
		Color bool `json:",omitempty"` //如果未赋值，则不会转换，注意是未赋值，只要赋值了，哪怕和数据零值相同，都会转换
		Actors []string
	}

	mv := Movie{Title:"success",Year:10}
	mvJson,_ := json.MarshalIndent(mv,"","	")
	//使用 json.Marshal() 将其转换为 json 字符串
	//使用 json.Unmarshal() 将 json 转换为 go 类型，转换时会忽略 json 数据的大小写
	fmt.Printf("%s",mvJson)
	//{
	//	"Title": "success",
	//	"year": 10,
	//	"Actors": null
	//}

	// 字段被本包忽略
	Field int `json:"-"`
	// 字段在json里的键为"myName"
	Field int `json:"myName"`
	// 字段在json里的键为"myName"且如果字段为空值将在对象中省略掉
	Field int `json:"myName,omitempty"`
	// 字段在json里的键为"Field"（默认值），但如果字段为空值会跳过；注意前导的逗号
	Field int `json:",omitempty"`
```

# 文本和 html 模板
补充一个知识点：

很多类型都定义了自己的 String 方法，当使用 fmt 包中的方法输出时，就是调用该类型的 String 方法
```
	type T struct {
		Name string
	}

	func (this T) String()  string{
		return "do string"
	}

	func main() {
		t := T{Name:"dyk"}
		fmt.Println(t) // do string
	}
```
text/template 和 html/template 中的方法类似，但是 html/template 会将一切传入变量的字符串中特殊字符进行同义转义，避免被视作 html/js 解析，这样可以减少很多安全问题，如果想要对传入字符串添加信任，可以声明为 template.HTML

	template.Must(template.New("tem").Funcs(template.FuncMap{"func",func}).Parse(tem)).Execute(os.Stdin,params)
