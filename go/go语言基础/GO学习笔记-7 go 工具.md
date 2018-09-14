# test
test 命令提供了自动化测试的功能
- go test 执行当前包下的测试文件 xxx_test.go
- go test -v 执行测试文件并打印执行过程中的详细信息
- 我们为 name.go 写的测试文件一般命名为 name_test.go，但 go test 并不关心 name，只关心 _test.go 后缀，当前包下以该后缀结尾的 **所有** 文件都会被执行

## TestXXX 函数
- 在 _test.go 文件中，只会执行以 Test 为前缀的函数，且函数必须传入参数 t *testing.T 或 b *testing.B，t 可以用于打印错误信息并终止测试等 
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