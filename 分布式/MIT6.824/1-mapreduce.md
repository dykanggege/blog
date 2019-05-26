# mapreduce 工作原型

1. master：找到空闲的worker，split分割任务，然后分发任务
2. map worker：调用用户定义的map函数，并缓存中间值在磁盘。周期性的调用partition函数，发送产出给reduce worker可调用到的位置，并将中间值存放位置返回给master节点，master会发送这些位置给reduce worker
3. 当master接受到位置信息后，会调用reduce worker并传递位置信息      