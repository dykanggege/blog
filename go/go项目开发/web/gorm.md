# 数据库与映射关系
## 连接数据库

```

    db, err := gorm.Open("mysql", "user:password@/dbname?charset=utf8&parseTime=True&loc=Local")
    defer db.Close()
    //最好将 db 放在 init 中声明，避免使用时尚未初始化，空指针异常

```

## 迁移与映射
即，自动创建与 struct 对应的表结构
```
    
    db.AutoMigrate(&User{}, &Product{}, &Order{}) //传入要创建表结构的 struct 指针
    //自动建表，并不会对原有的列做修改，而是添加新的列
    //注意：只有大写属性才会被导出

    // 创建表时添加表后缀，和其他设置
    db.Set("gorm:table_options", "ENGINE=InnoDB").AutoMigrate(&User{})

    // 或使用函数定义表前缀，注意，无论添加后缀还是前缀，都应该在自动建表函数前执行
    gorm.DefaultTableNameHandler = func (db *gorm.DB, defaultTableName string) string  {
		return "tb_" + defaultTableName;
	}

    //将结构体映射为表结构时，表名是结构体名的复数形式，可以关闭 User -> users
    //禁用表明变复数
    DB.SingularTable(true)
    //将表和字段映射为数据库表时，使用蛇影对应，即，结构体 User -> 表 user， UserInfo -> user_info，字段也是如此对应 
	
    //可以开启打印 sql 语句，便于调试
	DB.LogMode(true)
    


```

# 模型定义
## 约定 gorm.Model

```
    type Model struct {
        ID        uint `gorm:"primary_key"` // 作为主键，映射为表中自增主键 id
        CreatedAt time.Time
        UpdatedAt time.Time
        DeletedAt *time.Time
        //在增删改查时自动填充，注意此处的删除并不是真正的删除，而是设置了删除时间并且检索不出
    }

```

## 关联
```

    type Person struct {
        gorm.Model
        Name string
        Age int
        Sex string
    }

    type Student struct {
        gorm.Model
        Sorce string
        Garde string
        Person Person
        PersonID int
    }

    func (this *Student)Get()  {
        DB.First(this).Related(this.Person)
    }

```
无论使用正向关联、反向关联，一对多或多对多，默认都是使用 结构体名+ID 作为外键查询，注意要传入指针，也可以自己指定外键，但是我觉得没这个必要