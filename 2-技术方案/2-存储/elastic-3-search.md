URI search和request body，这里主要讲解第二种

- ignore_unavailable=true，可以忽略尝试访问不存在的索引“404_idx”导致的报错
- profile true 性能分析
- query 查询参数

# query  
## 查询字段
- match_all 空查询，匹配所有
- match 单字段的精确匹配，用 . 匹配json嵌套字段
- multi_match 多字段的精确匹配
```
GET /_search
{
    "query": {
        "match": {
            "tweet": "elasticsearch"
        }
    }
}
```

## 合并查询
- bool 用于组合多个查询语句

```
POST /movies,404_idx/_search?ignore_unavailable=true
{
    "profile": true,
    "query": {
        "match_all": {}
    }
}
```

- match_all 查询所有文档



```

# 查询movies分页
POST /movies,404_idx/_search?ignore_unavailable=true
{
   "profile": true,
    "query": {
        "match_all": {}
    }
}
```



```
POST /kibana_sample_data_ecommerce/_search
{
  "from":10,
  "size":20,
  "query":{
    "match_all": {}
  }
}


#对日期排序
POST kibana_sample_data_ecommerce/_search
{
  "sort":[{"order_date":"desc"}],
  "query":{
    "match_all": {}
  }

}

#source filtering
POST kibana_sample_data_ecommerce/_search
{
  "_source":["order_date"],
  "query":{
    "match_all": {}
  }
}


#脚本字段
GET kibana_sample_data_ecommerce/_search
{
  "script_fields": {
    "new_field": {
      "script": {
        "lang": "painless",
        "source": "doc['order_date'].value+'hello'"
      }
    }
  },
  "query": {
    "match_all": {}
  }
}


POST movies/_search
{
  "query": {
    "match": {
      "title": "last christmas"
    }
  }
}

POST movies/_search
{
  "query": {
    "match": {
      "title": {
        "query": "last christmas",
        "operator": "and"
      }
    }
  }
}

POST movies/_search
{
  "query": {
    "match_phrase": {
      "title":{
        "query": "one love"

      }
    }
  }
}

POST movies/_search
{
  "query": {
    "match_phrase": {
      "title":{
        "query": "one love",
        "slop": 1

      }
    }
  }
}
```