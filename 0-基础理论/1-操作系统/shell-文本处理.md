# grep 行处理


# awk 列处理
awk 动作 文件名

awk会根据空格和制表符，将每一行分成若干字段，依次用$1、$2、$3代表第一个字段、第二个字段、第三个字段等等。

```
# 输出a
echo 'this is a test' | awk '{print $3}'

# -F 指定分隔符
rpm -q systemd | awk -F. '{print $1}'
```

除了$ + 数字表示某个字段，awk还提供其他一些变量
- 变量NF表示当前行有多少个字段，因此$NF就代表最后一个字段，$(NF-1) 倒数第二个

