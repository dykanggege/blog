有时我们需要一个函数，这个函数能处理所有的类型，即使是我们还未知的用户自定义的类型，我们不知它的接口，不知类型和布局，就如同内置函数 fmt.Sprint() 传入任意类型的数据，都能返回字符串，以我们现有知识可以得到一下的解决方案

```
    func Sprint(x interface{}) string {
        type stringer interface {
            String() string
        }
        
        switch x := x.(type) {
        case stringer:
            return x.String()
        case string:
            return x
        case int:
            return strconv.Itoa(x)
        case bool:
            return strconv.FormatBool(x)
        default:
            //float32、float64、map、slice 等等
        }
        return ""
    }
```

我们发现无法枚举所有类型，更别提用户自定义类型了，这时我们就需要一种方式去处理未知类型

# reflect.Type、reflect.Value
reflect.Type 是一个包含了许多方法的接口，我们可以借助这些方法识别数据类型和透视布局

他只有一个实现，就是类型描述符，通过 reflect.TypeOf(interface{}) 方法，我们知道，将具体数赋值给接口，会发生隐式类型转换，会生成一个具有动态类型和动态值的接口值，例如 把 3 赋值给 interface{} 得到动态类型 int，动态值 3 的接口值，调用 TypeOf 方法返回接口值对应的动态类型（具体类型）

reflect.Value 则是接口类型的动态值，使用 reflect.ValueOf(interface{}) 获取动态值，但 reflect.Value 也包含一个接口值

```
    i := reflect.TypeOf(3)
	fmt.Println(i.String()) //int
	v := reflect.ValueOf(3)
	fmt.Println(v.String()) //<int Value>
```
