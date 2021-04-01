# 单例模式
全局只需要一个或只产生一次的变量，如日志，数据库连接池

- 饱汉式：加载进程时即生产对象
- 饥汉式：第一次使用时生产对象
    - 双重检验提高性能且避免重入
    
```
// 数据库连接池
type DBPooler interface {
	Query(string) error
	Exec(string) error
}

type mysqlPool struct {
}

func newMysqlPool() *mysqlPool {
	return &mysqlPool{}
}

func (m *mysqlPool) Query(s string) error {
	panic("implement me")
}

func (m *mysqlPool) Exec(s string) error {
	panic("implement me")
}

// 直接简单
var SingleInstance DBPooler = new(mysqlPool)

// 并不初始化的单例
var mysqlPoolInstance DBPooler

// 不想暴露这个全局变量
var lock sync.Mutex
func MysqlPool() DBPooler {
	// 双重检测避免并发竞争性能下降，且确保并发安全
	if has(mysqlPoolInstance) {
		return mysqlPoolInstance
	}

	lock.Lock()
	defer lock.Unlock()
	if !has(mysqlPoolInstance) {
		mysqlPoolInstance = newMysqlPool()
	}

	return mysqlPoolInstance
}

func has(v interface{}) bool {
	if v == nil {
		return false
	}
	return true
}
```

# 使用
- 注意不要违背了基于接口而非实现的原则，即满足开闭
    - 不应该直接使用单例对象，而是满足接口的对象

- 代码可读性差，隐藏了类之间的引用关系，且部分服务可能更改依赖
    - 通过依赖注入的方式引用单例

- 考虑变为非单例的可能性和扩展实现
    - 实现通用的抽象类，按需声明全局单例对象

- 单例的可测试性差，无法模拟替代单例数据
