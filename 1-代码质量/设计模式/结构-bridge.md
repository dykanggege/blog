Decouple an abstraction from its implementation so that the two can vary independently

将抽象和实现解藕，让两者都可以独立变化

比如 VFS，对外提供的使用接口和磁盘要求的接口是分开存在的，文件系统就是在其中做整合的算法

对外提供的接口是抽象，底层磁盘接口是实现，文件系统将实现变为抽象