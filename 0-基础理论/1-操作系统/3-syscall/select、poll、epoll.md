无论是阻塞、非阻塞、异步IO，这些都是对单个文件描述符的IO操作，如果想要同时对很多文件描述符做IO操作，例如对3K个文件做监控读取数据，无论使用这里面的哪种都不合适，就需要更高级的IO复用函数

# select

    int select(int n, fd_set *readfds, fd_set *writefds, fs_set *expectfds, struct timeval *timeout)

第一个参数是监控文件的个数，后面都是要传入监控文件的描述符，调用后发生阻塞，如果任何一个 可读、可写、异常 或者 timeout，函数返回，等到select函数返回后，可以通过遍历fdset找到就绪的描述符，n存在一个宏限制，在linux上一般是1024

使用select存在很多缺点，例如：

- 想向监控中添加新的描述符，就要重新传入数组，再从用户态复制到内核态一次
- 当IO操作或者异常发生后，需要自己遍历数组查询
- 监控数量有限制

# poll

    int poll(struct pollfd fds[], unsigned int nfds, int timeout)

    struct pollfd {
        int fd; //文件描述符
        short events; //要监控的事件
        short revents; //发生的事件
    }

poll也是传入数组监控，同样存在select存在的添加问题、遍历问题，虽然没有数量限制，监控太多会很耗费性能

# epoll
epoll和select、poll从本质上就不同，epoll采用事件驱动，后两者采用阻塞

    int epoll_create(int size) //size要监控文件数量
    
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
    //epfd是epoll的描述符，op代表操作，fd文件描述符，epoll_event要监听的事件

    int epoll_wait(int epdfd, struct epoll_event *events, int maxevents, int timeout)
    //epoll_event 得到事件的集合、maxevents事件数量

- 可以监控的描述符没有限制
- 添加或删除监控不需要重新复制数组到内核
- 直接返回发生的IO事件集合，不需要自己遍历，底层是通过红黑树维持文件描述符查找，性能很好


与 select&poll 相比，epoll 分清了高频调用和低频调用。例如，epoll_ctl 相对来说就是非频繁调用的，而 epoll_wait 则是会被高频调用的。所以 epoll 利用 epoll_ctl 来插入或者删除一个 fd，实现用户态到内核态的数据拷贝，这确保了每一个 fd 在其生命周期只需要被拷贝一次，而不是每次调用 epoll_wait 的时候都拷贝一次。 epoll_wait 则被设计成几乎没有入参的调用，相比 select&poll 需要把全部监听的 fd 集合从用户态拷贝至内核态的做法，epoll 的效率就高出了一大截。

在实现上 epoll 采用红黑树来存储所有监听的 fd，而红黑树本身插入和删除性能比较稳定，时间复杂度 O(logN)。通过 epoll_ctl 函数添加进来的 fd 都会被放在红黑树的某个节点内，所以，重复添加是没有用的。当把 fd 添加进来的时候时候会完成关键的一步：该 fd 会与相应的设备（网卡）驱动程序建立回调关系，也就是在内核中断处理程序为它注册一个回调函数，在 fd 相应的事件触发（中断）之后（设备就绪了），内核就会调用这个回调函数，该回调函数在内核中被称为： ep_poll_callback ，这个回调函数其实就是把这个 fd 添加到 rdllist 这个双向链表（就绪链表）中。epoll_wait 实际上就是去检查 rdllist 双向链表中是否有就绪的 fd，当 rdllist 为空（无就绪 fd）时挂起当前进程，直到 rdllist 非空时进程才被唤醒并返回。

相比于 select&poll 调用时会将全部监听的 fd 从用户态空间拷贝至内核态空间并线性扫描一遍找出就绪的 fd 再返回到用户态，epoll_wait 则是直接返回已就绪 fd，因此 epoll 的 I/O 性能不会像 select&poll 那样随着监听的 fd 数量增加而出现线性衰减，是一个非常高效的 I/O 事件驱动技术。

由于使用 epoll 的 I/O 多路复用需要用户进程自己负责 I/O 读写，从用户进程的角度看，读写过程是阻塞的，所以 select&poll&epoll 本质上都是同步 I/O 模型，而像 Windows 的 IOCP 这一类的异步 I/O，只需要在调用 WSARecv 或 WSASend 方法读写数据的时候把用户空间的内存 buffer 提交给 kernel，kernel 负责数据在用户空间和内核空间拷贝，完成之后就会通知用户进程，整个过程不需要用户进程参与，所以是真正的异步 I/O

