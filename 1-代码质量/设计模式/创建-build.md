The intent of the Builder design pattern is to separate the construction of a complex object from its representation. By doing so the same construction process can create different representations.

创建组建通常需要很多配置，可能会有一下需求

1. New有众多配置参数，有的必填有的选填
2. 需要对配置参数做校验，当填一个参数时候可能会检验另一个参数也必填
3. 配置一旦确定不能修改

这样一个数据库连接池
```
type DBPool struct {
	// 必填
	addr    string

	// 下面选填，有默认值，maxConn和maxIdle必须同时设置
	maxConn int
	maxIdle int
	
	timeout int
}
```

最简单的实现场景如下

```
// 创建构造方法，依赖必填项
func NewDBPool(addr string) *DBPool {
	return &DBPool{
		addr: addr,
		maxConn: 100,
		maxIdle: 60,
		timeout: 60,
	}
}

func (d *DBPool) SetMaxConnAndMaxIdle(maxConn, maxIdle int) {
	if maxConn <= 0 {
		maxConn = 1
	}
	if maxIdle < 0 {
		maxIdle = 0
	}
	d.maxConn = maxConn
	d.maxIdle = maxIdle
}

func (d *DBPool) SetTimeout(timeout int) {
	if timeout <= 0 {
		timeout = 1
	}
	d.timeout = timeout
}
```

满足了基础特点
1. 强制要求必填参数
2. set方法中提示选填参数，且分离每个参数的校验逻辑

同时带来如下问题

1. 必填项太多会导致New参数太多
2. 不希望在运行过程中被Set动态修改，即初始化后确定参数，不允许动态修改
3. 假如timeout会根据maxConn是否设置有不同的校验会导致Set函数太多且复杂

可以把构建的逻辑单独拆出

```
type DBPoolOptions struct {
	// 必填
	addr    string

	// 有默认值
	maxConn int
	maxIdle int

	timeout int
}

func NewDBPoolOptions(addr string) (options *DBPoolOptions, err error) {
	options = &DBPoolOptions{addr: addr}
	err = options.checkOptions()
	return
}

func (d *DBPoolOptions) SetMaxConnAndMaxIdle(maxConn, maxIdle int) {
	if maxConn <= 0 {
		maxConn = 1
	}
	if maxIdle < 0 {
		maxIdle = 0
	}
	d.maxConn = maxConn
	d.maxIdle = maxIdle
}

func (d *DBPoolOptions) SetTimeout(timeout int) {
	if timeout <= 0 {
		timeout = 1
	}
	d.timeout = timeout
}

func (d *DBPoolOptions) checkOptions() error {
	return nil
}
```

build模式用于配置组建，具体使用方法按照组建参数的需求，不需要过度复杂化，但是也要保证方法易用