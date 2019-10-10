# cgroup
先放一些定义，看不懂没关系，后面慢慢就知道了

**cgroup ，控制组，它提供了一套机制用于控制一组特定进程对资源的使用。cgroup绑定一个进程集合到一个或多个子系统上** 

cgroup有v1和v2两个版本，一般不会将两个版本混用，docker目前使用v1，所以这里主要介绍v1

cgroup可以通过挂载到任何文件夹下使用

通常系统已经将cgroup挂载到 /sys/fs/cgroup 下，并且挂载了所有的subsystem。如果没有挂载，可以通过下面的命令手动挂载

    //挂载一颗和所有subsystem相关的cgroup树
    mount -t cgroup 起个名 /sys/fs/cgroup   

    //挂载cpuset subsystem到/sys/fs/cgroup/cpuset
    mount -t cgroup -o cpu,cpuset /sys/fs/cgroup/cpuset 

    //挂载一棵cgroup树，但不关联任何subsystem，下面就是systemd所用到的方式
    mount -t cgroup -o none,name=systemd xxx /sys/fs/cgroup/systemd

    //卸载一个cgroup或subsystem可以用 umount


## 挂载
**只能有一个和所有子系统相关的cgroup**，我们可以创建其他的没有任何subsystem的cgroup，但你创建挂载subsystem的cgroup就会出错

    root@kanggege-PC:~/tcgroup# mkdir demo
    root@kanggege-PC:~/tcgroup# mount -t cgroup none ./demo/
    mount: /root/tcgroup/demo: none already mounted on /proc.

无法在创建和所有子系统相关的cgroup，但可以将subsystem挂载到其他任何文件夹下

    root@kanggege-PC:~/tcgroup# mount -t cgroup -o memory none ./demo/
    root@kanggege-PC:~/tcgroup# cd demo/
    root@kanggege-PC:~/tcgroup/demo# ls
    2@dde				memory.kmem.slabinfo		    memory.pressure_level
    cgroup.clone_children		memory.kmem.tcp.failcnt		    memory.soft_limit_in_bytes
    cgroup.event_control		memory.kmem.tcp.limit_in_bytes	    memory.stat
    cgroup.procs			memory.kmem.tcp.max_usage_in_bytes  memory.swappiness
    cgroup.sane_behavior		memory.kmem.tcp.usage_in_bytes	    memory.usage_in_bytes
    docker				memory.kmem.usage_in_bytes	    memory.use_hierarchy
    memory.failcnt			memory.limit_in_bytes		    notify_on_release
    memory.force_empty		memory.max_usage_in_bytes	    release_agent
    memory.kmem.failcnt		memory.move_charge_at_immigrate     system.slice
    memory.kmem.limit_in_bytes	memory.numa_stat		    tasks
    memory.kmem.max_usage_in_bytes	memory.oom_control		    user.slice
    root@kanggege-PC:~/tcgroup/demo# cd /sys/fs/cgroup/memory/
    root@kanggege-PC:/sys/fs/cgroup/memory# mkdir exit
    root@kanggege-PC:/sys/fs/cgroup/memory# ls ~/tcgroup/demo/ | grep exit
    exit

即使你把一个subsystem挂载到其他文件夹下，会发现他还是原来的subsystem又在这个文件夹下挂载了一遍，并不是新的subsystem

接着演示怎么挂载cgroup

由于linux已经挂载过cgroup，所以这里演示挂载一个不和任何subsystem绑定的cgroup树，演示怎么挂载和卸载cgroup

    root@kanggege-PC:~/tcgroup# mkdir cgroup
    
    //挂载一个没有任何subsystem，名为demo的cgroup到 ./cgroup 下
    root@kanggege-PC:~/tcgroup# mount -t cgroup -o none,name=demo demo ./cgroup/

    //查一下看看
    root@kanggege-PC:~/tcgroup# mount | grep cgroup
    tmpfs on /sys/fs/cgroup type tmpfs (ro,nosuid,nodev,noexec,mode=755)
    cgroup2 on /sys/fs/cgroup/unified type cgroup2 (rw,nosuid,nodev,noexec,relatime,nsdelegate)
    cgroup on /sys/fs/cgroup/systemd type cgroup (rw,nosuid,nodev,noexec,relatime,xattr,name=systemd)
    cgroup on /sys/fs/cgroup/blkio type cgroup (rw,nosuid,nodev,noexec,relatime,blkio)
    cgroup on /sys/fs/cgroup/cpu,cpuacct type cgroup (rw,nosuid,nodev,noexec,relatime,cpu,cpuacct)
    cgroup on /sys/fs/cgroup/pids type cgroup (rw,nosuid,nodev,noexec,relatime,pids)
    cgroup on /sys/fs/cgroup/cpuset type cgroup (rw,nosuid,nodev,noexec,relatime,cpuset)
    cgroup on /sys/fs/cgroup/devices type cgroup (rw,nosuid,nodev,noexec,relatime,devices)
    cgroup on /sys/fs/cgroup/memory type cgroup (rw,nosuid,nodev,noexec,relatime,memory)
    cgroup on /sys/fs/cgroup/hugetlb type cgroup (rw,nosuid,nodev,noexec,relatime,hugetlb)
    cgroup on /sys/fs/cgroup/net_cls,net_prio type cgroup (rw,nosuid,nodev,noexec,relatime,net_cls,net_prio)
    cgroup on /sys/fs/cgroup/freezer type cgroup (rw,nosuid,nodev,noexec,relatime,freezer)
    cgroup on /sys/fs/cgroup/perf_event type cgroup (rw,nosuid,nodev,noexec,relatime,perf_event)
    cgroup on /sys/fs/cgroup/rdma type cgroup (rw,nosuid,nodev,noexec,relatime,rdma)
    demo on /root/tcgroup/cgroup type cgroup (rw,relatime,name=demo) //我们刚刚挂载的

    root@kanggege-PC:~/tcgroup/cgroup# ls
    cgroup.clone_children  cgroup.procs  cgroup.sane_behavior  notify_on_release  release_agent  tasks

- cgroup.clone_children 这个文件只对cpuset（subsystem）有影响，当该文件的内容为1时，新创建的cgroup将会继承父cgroup的配置，即从父cgroup里面拷贝配置文件来初始化新cgroup
- cgroup.procs 当前cgroup中的所有进程ID，系统不保证ID是顺序排列的，且ID有可能重复
- cgroup.sane_behavior 具体功能不详
- notify_on_release 该文件的内容为1时，当cgroup退出时（不再包含任何进程和子cgroup），将调用release_agent里面配置的命令。新cgroup被创建时将默认继承父cgroup的这项配置。
- release_agent 里面包含了cgroup退出时将会执行的命令，系统调用该命令时会将相应cgroup的相对路径当作参数传进去。 注意：这个文件只会存在于root cgroup下面，其他cgroup里面不会有这个文件。
- tasks 当前cgroup中的所有线程ID，系统不保证ID是顺序排列的

当前的tasks和cgroup.procs包含系统里所有的进程和线程id

    //创建文件夹就是创建一个子cgroup
    root@kanggege-PC:~/tcgroup/cgroup# mkdir subcgroup

    //子cgroup默认有一些文件
    root@kanggege-PC:~/tcgroup/cgroup# cd subcgroup/ && ls
    cgroup.clone_children  cgroup.procs  notify_on_release	tasks

    //没有托管任何进程或线程
    root@kanggege-PC:~/tcgroup/cgroup/subcgroup# cat tasks && cat cgroup.procs 
    root@kanggege-PC:~/tcgroup/cgroup/subcgroup# 

    //子cgroup中还可以再创建子cgroup
    root@kanggege-PC:~/tcgroup/cgroup/subcgroup# mkdir subsubcp
    root@kanggege-PC:~/tcgroup/cgroup/subcgroup# ls ./subsubcp/
    cgroup.clone_children  cgroup.procs  notify_on_release	tasks

    //删除文件夹就等于删除cgroup
    root@kanggege-PC:~/tcgroup/cgroup/subcgroup# rmdir subsubcp/

    //将进程加入cgroup中
    root@kanggege-PC:~/tcgroup/cgroup# echo $$ >> cgroup.procs 

一个进程可以存在于多个子系统中，但在一个子系统中只能被加入一个cgroup里，如果加入到新的cgroup，就会自动从原来的cgroup中清除，进程的子进程也会被默认加到新的cgroup中

![](img/4.png)


cgroup.procs是用来放入进程，而tasks用来放入线程，但实际线程在v2中已经弃用了，以后把进程放到cgroup.procs中就好

当cgroup中最后一个进程离去，并且没没有子cgroup，就会执行 notify_on_release 中记录的脚本

# subsystem

在cgroup下创建文件夹会发现它是一个只读系统，他下面的每个文件夹都是对一种资源的限制，就是传说中的subsystem

看下根cgroup里所有的subsystem

    kanggege@kanggege-PC:/sys/fs/cgroup$ ls
    blkio  cpuacct      cpuset   freezer  memory   net_cls,net_prio  perf_event  rdma     unified
    cpu    cpu,cpuacct  devices  hugetlb  net_cls  net_prio          pids        systemd


## pids

pids可以限制进程总数

    root@kanggege-PC:/sys/fs/cgroup/pids/test# ls
    cgroup.clone_children  cgroup.procs  notify_on_release	pids.current  pids.events  pids.max  tasks

在pids下面创建文件夹并进入，多了两个特殊文件

- pids.current：用来限制cgroup及其所有子孙cgroup当前运行的进程总数，在向cgroup内移入进程时检查该文件
- pids.max：跟上面一样，但是只有在fork时才会检查pids.max，current可能大于max


子cgroup会受祖先cgroup的current和max限制，取min(c1,c2,c3)

## memory
    //在memory下创建了一个cgroup，查看文件
    root@kanggege-PC:/sys/fs/cgroup/memory/test# ls
    cgroup.clone_children  memory.kmem.failcnt	       memory.kmem.tcp.limit_in_bytes	   memory.max_usage_in_bytes	    memory.soft_limit_in_bytes	notify_on_release
    cgroup.event_control   memory.kmem.limit_in_bytes      memory.kmem.tcp.max_usage_in_bytes  memory.move_charge_at_immigrate  memory.stat			tasks
    cgroup.procs	       memory.kmem.max_usage_in_bytes  memory.kmem.tcp.usage_in_bytes	   memory.numa_stat		    memory.swappiness
    memory.failcnt	       memory.kmem.slabinfo	       memory.kmem.usage_in_bytes	   memory.oom_control		    memory.usage_in_bytes
    memory.force_empty     memory.kmem.tcp.failcnt	       memory.limit_in_bytes		   memory.pressure_level	    memory.use_hierarchy

memory提供了很多功能，但很多我们用不到

    cgroup.event_control       #用于eventfd的接口
    memory.usage_in_bytes      #显示当前已用的内存
    memory.limit_in_bytes      #设置/显示当前限制的内存额度
    memory.failcnt             #显示内存使用量达到限制值的次数
    memory.max_usage_in_bytes  #历史内存最大使用量
    memory.soft_limit_in_bytes #设置/显示当前限制的内存软额度
    memory.stat                #显示当前cgroup的内存使用情况
    memory.use_hierarchy       #设置/显示是否将子cgroup的内存使用情况统计到当前cgroup里面
    memory.force_empty         #触发系统立即尽可能的回收当前cgroup中可以回收的内存
    memory.pressure_level      #设置内存压力的通知事件，配合cgroup.event_control一起使用
    memory.swappiness          #设置和显示当前的swappiness
    memory.move_charge_at_immigrate #设置当进程移动到其他cgroup中时，它所占用的内存是否也随着移动过去
    memory.oom_control         #设置/显示oom controls相关的配置
    memory.numa_stat           #显示numa相关的内存

下面示例具体怎么用

    //在第一个tty中，将自己加入并启动top命令
    root@kanggege-PC:/sys/fs/cgroup/memory/test# echo $$ >> cgroup.procs 
    root@kanggege-PC:/sys/fs/cgroup/memory/test# top
    
    //再打开一个命令窗口
    root@kanggege-PC:/sys/fs/cgroup/memory/test# cat cgroup.procs 
    6586
    18860
    //查看下已经使用的内存量，以字节为单位
    root@kanggege-PC:/sys/fs/cgroup/memory/test# cat memory.usage_in_bytes 
    3571712

    #--------------------------第一个shell窗口----------------------
    #回到第一个shell窗口
    dev@dev:/sys/fs/cgroup/memory$ cd test
    #这里两个进程id分别时第二个窗口的bash和top进程
    dev@dev:/sys/fs/cgroup/memory/test$ cat cgroup.procs
    4589
    4664
    #开始设置之前，看看当前使用的内存数量，这里的单位是字节
    dev@dev:/sys/fs/cgroup/memory/test$ cat memory.usage_in_bytes
    835584

    #设置1M的限额
    dev@dev:/sys/fs/cgroup/memory/test$ sudo sh -c "echo 1M > memory.limit_in_bytes"
    #设置完之后记得要查看一下这个文件，因为内核要考虑页对齐, 所以生效的数量不一定完全等于设置的数量
    dev@dev:/sys/fs/cgroup/memory/test$ cat memory.limit_in_bytes
    1048576

    #如果不再需要限制这个cgroup，写-1到文件memory.limit_in_bytes即可
    dev@dev:/sys/fs/cgroup/memory/test$ sudo sh -c "echo -1 > memory.limit_in_bytes"
    #这时可以看到limit被设置成了一个很大的数字
    dev@dev:/sys/fs/cgroup/memory/test$ cat memory.limit_in_bytes
    9223372036854771712

    
## cpu

    root@kanggege-PC:/sys/fs/cgroup/cpu# ls
    cgroup.clone_children  cpuacct.usage_all	  cpuacct.usage_user  docker		 tasks
    cgroup.procs	       cpuacct.usage_percpu	  cpu.cfs_period_us   mycpu		 test
    cgroup.sane_behavior   cpuacct.usage_percpu_sys   cpu.cfs_quota_us    notify_on_release  user.slice
    cpuacct.stat	       cpuacct.usage_percpu_user  cpu.shares	      release_agent
    cpuacct.usage	       cpuacct.usage_sys	  cpu.stat	      system.slice

