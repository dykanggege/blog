# 常用命令

1. mongo mongodb://ip:27017 -u "user" -p "pwd"
2. use 切换数据库，也可以创建数据库，db查看当前数据库，show dbs 查看所有数据库

# 索引
1. 不可重复创建索引

    db.coll.createIndex({test:1}) // 这个1就是占位的

# golang使用mongo的坑
1. 直接查询表返回错误，必须先插入值，再清空值表不会被删
2. 存储字段不是用json，而是bson，再单独定义标签