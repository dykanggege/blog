如果一条sql语句执行的很慢，该怎么办?当然是要先查查他是否用到了索引，然后分析一下sql语句怎么执行的，我们可以用mysql的explain解释器查看sql语句怎么被解析器分析执行

# explain
```
mysql> explain select * from users;
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------+
| id | select_type | table | partitions | type | possible_keys | key  | key_len | ref  | rows | filtered | Extra |
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------+
|  1 | SIMPLE      | users | NULL       | ALL  | NULL          | NULL | NULL    | NULL |    1 |   100.00 | NULL  |
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------+
1 row in set, 1 warning (0.00 sec)
```

## id
id代表查询顺序
- id相同，顺序从上往下执行
- id不同，id越大优先级越高

## select_type
查询类型

