PromQL (Prometheus Query Language) 是 Prometheus 自己开发的数据查询 DSL 语言，语言表现力非常丰富，内置函数很多，在日常数据可视化以及rule 告警中都会使用到它。

# http查询
所有的API请求均返回以下的JSON格式：
```
{
  "status": "success" | "error",
  "data": <data>,

  // Only set if status is "error". The data field may still hold
  // additional data.
  "errorType": "<string>",
  "error": "<string>"
}
```

- 404 Bad Request：当参数错误或者缺失时。
- 422 Unprocessable Entity 当表达式无法执行时。
- 503 Service Unavailiable 当请求超时或者被中断时。

## 瞬时数据查询
- GET /api/v1/query
- query=：PromQL表达式。
- time=：用于指定用于计算PromQL的时间戳。可选参数，默认情况下使用当前系统时间。
- timeout=：超时设置。可选参数，默认情况下使用-query,timeout的全局设置。




## 区间数据查询
- GET /api/v1/query_range
- query=: PromQL表达式。
- start=: 起始时间。
- end=: 结束时间。
- step=: 查询步长。
- timeout=: 超时设置。可选参数，默认情况下使用-query,timeout的全局设置。

## 响应数据类型


# 基础查询
## 时间序列
1. 当我们直接使用监控指标名称查询时，可以查询该指标下的所有时间序列，如

    http_requests_total == http_requests_total{}

2. 使用 label = value 或 label != value 匹配满足条件的时间序列
3. 使用label=~regx表示选择那些标签符合正则表达式定义的时间序列，!~表示不符合，多个表达式之间用 | 分隔，如

    http_requests_total{environment=~"staging|testing|development",method!="GET"}

# 操作符

# 聚合操作

# 内置函数

