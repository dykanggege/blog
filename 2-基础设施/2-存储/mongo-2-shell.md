# 基本命令

1. mongo mongodb://ip:27017 -u "user" -p "pwd"
2. use 切换数据库，也可以创建数据库，db查看当前数据库，show dbs 查看所有数据库

# insert
- 插入时有校验
    - 如果没有 _id，自动生成
    - doc大于16M插入失败
- db.table.insert(obj)

# find
- db.table.find(filter)

# update
- db.table.update(filter, newobj)

## $set
直接update是全量更新，即用新代替旧，可以用set只更新某个字段，如果不存在，则创建这个字段，或者修改已有字段的类型，$unset可以删除某个字段

## $inc
原子增加某个数值类的键，如果不存在就自动创建
```
{
    "$inc": {"age":1}
}
// 如果是如下形式，用 . 连接
{
    "name": "test",
    "count": {
        "a": 0
    }
}

db.table.update({"name":"test"}, {"$inc": {"count.a": 1}})
```

## upsert
如果存在就更新，不存在则新增

## 数组操作
- $push 向数组后追加
- 
## remove
- db.table.remove(filter)
- db.table.drop 直接删表更快更刺激


# 索引
doc上创建的索引不能超过64个

1. 不可重复创建索引

    db.coll.createIndex({test:1}) // 这个1就是占位的
