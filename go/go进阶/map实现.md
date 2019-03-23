go中的map操作起来太方便了，使得我大量情况无脑的就用上了map，但有map的查找性能不一定总是强过arr的，如果小范围遍历情况，还是用数组

这篇文章尝试着简析map的源码，在$GOROOT/src/runtime/map.go可以看到map的源码

```


```