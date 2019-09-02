# 驱动
连接数据库需要驱动支持，将sql库中的语句翻译为数据库支持的语句，golang标准库中注册驱动的方式很有意思

通过调用这个方法

    func Register(name string, driver driver.Driver) {
        driversMu.Lock()
        defer driversMu.Unlock()
        if driver == nil {
            panic("sql: Register driver is nil")
        }
        if _, dup := drivers[name]; dup {
            panic("sql: Register called twice for driver " + name)
        }
        drivers[name] = driver
    }

会将驱动注册到 drivers 中，open函数会查找 drivers

    func Open(driverName, dataSourceName string) (*DB, error) {
        driversMu.RLock()
        driveri, ok := drivers[driverName]
        driversMu.RUnlock()
        if !ok {
            return nil, fmt.Errorf("sql: unknown driver %q (forgotten import?)", driverName)
        }
        ...
    }

因此你需要import驱动包，但驱动包会自己调用 sql.Register 把自己注册了，所以你不需要任何显式调用驱动，例如

    import (
        _ "github.com/go-sql-driver/mysql"
    )
    //diver文件里
    func init() {
        sql.Register("mysql", &MySQLDriver{})
    }

# 连接

    db, err := sql.Open("驱动名", datasourcename)

- open函数只会校验参数是否合法，不会连接，如果想知道是否能连接到数据库，应该用 db.Ping()
- 返回的db是并发安全的，而且程序停止会自己关闭，所以一般情况你不需要手动调用Close方法
- 已经內建了连接池

datasourcename有几种形式

- 用户名:密码@tcp(ip:端口)/数据库?charset=utf8

要分清楚下面三个的意思

- db.SetMaxIdleConns(100) //设置连接池中最大空闲链接数量，会随着最大链接数上升，直至到达设置的最大数量
- db.SetConnMaxLifetime(0) //设置空闲连接最大生存时间，如果 <= 0 则空闲连接不会被关闭
- db.SetMaxOpenConns(100) //设置与数据库同时建立连接最大量，如果 <= 0 则无限制，默认0，MaxIdleConn <= MaxOpenConn

# sql
## select
使用Query函数单独用于select

```
func sqlSelect(db *sql.DB) error {
	rows, err := db.Query("SELECT * FROM user")
	if err != nil{ return err}
    defer rows.Close()
	cols, _ := rows.Columns() //得到结果列名称
	types, _ := rows.ColumnTypes() //得到结果列类型
	for rows.Next() {
		rows.Scan(&val) //把每一列结果值映射到值中
	}
	return nil
}
```

多结果值查询你需要自己统计scan了多少值，统计有几行

或者使用 QueryRow() 只查询一行

```
row := db.QueryRow("SELECT * FROM user")
err := row.Scan()
if err == sql.ErrNoRows {
    fmt.Println("没有检索到值")
}
return err
```


## insert、update、delete

```
func execSql(db *sql.DB) error  {
	result, err := db.Exec("INSERT INTO price(price) VALUES('10.5')")
	addid, err := result.LastInsertId() //返回自增主键的id
	rowsAffected, err := result.RowsAffected() //返回影响的行数
	return err
}
```

## context
1.8中给很多函数都提供了 Context 例如：

```
func execSql(db *sql.DB) error  {
	ctx,stop := context.WithCancel(context.Background())
	limit := time.Tick(5 * time.Second)
	done := make(chan struct{})
	go func() {
		db.QueryRowContext(ctx,"",)
		done <- struct{}{}
	}()
	select {
	case <- limit:
		stop()
	case <- done:
	}
	return err
}
```

