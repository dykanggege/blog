# + 拼接
```
    func stringPlus() string {
        s := "k"+"a"+"n"+"g"
        s += "g"+"e"
        s += "g"+"e"
        s += "\n"
        return s
    }

    func BenchmarkStringPlus(b *testing.B) {
        for i := 0; i < b.N; i++ {
            stringPlus()
        }
    }

    go test -bench=. -benchmem

    BenchmarkStringPlus-8           10000000               111 ns/op              32 B/op          3 allocs/op

    //改为这样，再做拼接，发现性能没有变法，说明 go 在编译时就会对字符串拼接做优化，常量在编译时就已经拼接在一起了
    func stringPlus() string {
        s := "kang"
        s += "ge"
        s += "ge"
        s += "\n"
        return s
    }
```
# fmt.Sprint
```
    func stringFmt() string {
        s := fmt.Sprint("kang","ge","ge","\n")
        return s
    }

    BenchmarkStringPlus-8           10000000               151 ns/op              16 B/op          1 allocs/op
    //运行效率虽然变低了，但是内存分配变少了，但是这样做是不公平的，改为下面的做法

    func stringFmt() string {
        s := fmt.Sprint("kang")
        s = fmt.Sprint(s,"ge")
        s = fmt.Sprint(s,"ge")
        s = fmt.Sprint(s,"\n")
        return s
    }

    BenchmarkStringPlus-8            3000000               454 ns/op              80 B/op          7 allocs/op
    //无论是性能还是内存分配都远远不如 + 
```