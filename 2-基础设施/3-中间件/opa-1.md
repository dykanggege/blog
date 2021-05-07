opa是开源的云原生规则引擎

# 核心概念
- input  输入json值
- query  规则
- output 规则判断结果

# 规则
- 规则中用input代表输入，类js访问值，input.user[0].name["china"]
- 如果访问不存在的值，cmd中返回undefined decision，go sdk中返回空
- 多个表达式分行书写，或在一行中用 ; 分割，每个表达式都有自己的结果，声明变量成功也会返回true

## expression
支持 ==、!=、>、>=、

## var

user := input.user

## iteration
```
input.servers[_].protocols[_] == "http"

some i, j; input.servers[i].protocols[j] == "http"

some i, j
id := input.ports[i].id
input.ports[i].network == input.networks[j].id
input.networks[j].public
```

## rule