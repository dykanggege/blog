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



