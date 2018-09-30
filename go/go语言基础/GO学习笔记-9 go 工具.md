# go test
test 命令提供了自动化测试的功能
- go test 执行当前包下的测试文件 xxx_test.go
- go test -v 执行测试文件并打印执行过程中的详细信息
- 我们为 name.go 写的测试文件一般命名为 name_test.go，但 go test 并不关心 name，只关心 _test.go 后缀，当前包下以该后缀结尾的 **所有** 文件都会被执行
- 在 _test.go 文件中有三种函数需要特殊对待
    - 功能测试函数：以 Test 前缀命名的函数，用来检测程序逻辑的正确性，输出 Pass 或 Fail 
    - 基准测试函数：以 Benchmark 开头，用于测试某些操作的性能，汇报评价执行时间
    - 示例函数：以 Example 开头，同来提供机器检查过的文档
- go test 工具扫描 *_test.go 文件来寻找特殊函数，并声称一个临时的 main 包来调用他们，编译运行汇报结果后清空临时文件

## TestXXX 功能测试函数
- 在 _test.go 文件中，只会执行以 Test 为前缀的函数，且函数必须传入参数 t *testing.T，使用 t.Error() 打印错误信息
- 使用 go test 默认调用所有的 Test 函数，一个函数测试失败并不会让程序退出，而是测试所有的函数并打印错误的函数，所有函数都成功会打印 PASS，如果真的需要一个函数失败后终止程序，可以使用 t.Fatal
- go test -v 打印所有测试函数的信息，go test -run="reg|other" 只运行那些正则匹配到的函数 
- 在 _test.go 文件中的多个测试函数，go 并不保证他们是顺序执行的，可以使用 t.Run
- 传入 m testing.M 的测试函数会被作为主函数，在一切的测试函数之前执行，在该函数中调用 m.Run() 才会执行其他测试函数，否则不执行

```

    func TestAll(t *testing.T)  {
        t.Run("a1",Print1)
        t.Run("a2",Print2)
    }

    func Print1(t *testing.T)  {
        fmt.Println("print1")
    }

    func Print2(t *testing.T) {
        fmt.Println("print2")
    }

    //函数名必须是 TestMain
    func TestMain(m *testing.M)  {
        fmt.Println("首先被执行，可以用于做初始化操作")
        m.Run() //放行
    }

```

## BenchmarkXXX 函数
- Benchmark 函数用于做多次测试的平均性能，需要在调用时传入测试次数 go test -bench 100 如果次数为 . 则自动测试到稳定性能后停止
```

    func getRes() float64 {
        res := 0.0
        for i := 1.0; i < 10000; i++ {
            for j := 1.0; j < 10000; j++ {
                res += i/j
            }
        }
        return res
    }

    func BenchmarkAll(b *testing.B)  {
        for n := 0; n < b.N; n++ {
            getRes()
        }
    }


```