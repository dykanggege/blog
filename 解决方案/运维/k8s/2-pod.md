# 概念
- container 被设计为进程隔离单位，一个服务可能需要多个进程相互作用，所以设计了pod
- pod 是个逻辑划分，由多个相互依赖的 container 组成，master 只调度 pod，不直接操作 container
- node作为物理主机，一般运行多个pod，但pod中所有container一定在一个node中，不会跨物理主机
- pod 是最小的 scale 单位，应该将所有需要水平扩展的组件单独创建 pod


# 构成
## 共享
- 每个 pod 都有一个 pause container，依靠 pause 做 pod 的逻辑划分，pause 和其他逻辑中的 container 共享 Network、UTS、IPC，还可以设置开启共享 PID，可以通过挂载 volume 共享文件

# label
pod需要有一个标签 label 作为它的唯一标识，他是一个key=val键值对，val中存储了其他信息，这样就可以用 label selector 对 pod 做筛选

# 分类
pod通过操作划分有两种创建方式

### 自主式pod
由用户自己创建，然后交给kubelet管理，kubelet访问scheduler请求调度

如果有pod故障，由kubelet重启pod，但是有node故障，则pod就消失了

### 控制器管理的pod
ReplicationController 在这里面定义运行的pod，自动调度，维持pod的数量和运行

并支持滚动更新，先创建新服务，再停掉旧服务，保持服务不中断

还有各种各样的用于管理不同pod声明周期的控制器

- ReplicationController & ReplicatSet & Deployment