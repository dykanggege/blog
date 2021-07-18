nginx 主要用于静态资源分发、反向代理、负载均衡，并且高性能、占用资源少、特性可配置

在 /usr/share/nginx/html/index.html 可以看到nginx的欢迎页

实际安装的时候有很多参数，可以指定安装位置或静态文件位置等

先把nginx的html文件和配置文件以及日志映射到本地，再修改配置，否则容器删除后自己的修改都没了

    docker run -d --name mynginx \
    --mount type=bind,source=$PWD/nginx/html,target=/usr/share/nginx/html \
    --mount type=bind,source=$PWD/nginx/conf/nginx.conf,target=/etc/nginx/nginx.conf \
    --mount type=bind,source=$PWD/nginx/log,target=/var/log/nginx \
    --network host \
    nginx 


# nginx概览
ngxin主要作用是静态资源的分发、反向代理，提供的功能简单但又多样化，所以操作命令很简单，主要是配置文件复杂

    nginx -s stop       快速关闭Nginx，可能不保存相关信息，并迅速终止web服务。
    nginx -s quit       平稳关闭Nginx，保存相关信息，有安排的结束web服务
    nginx -s reload     因改变了Nginx相关配置，需要重新加载配置而重载。
    nginx -s reopen     重新打开日志文件。
    nginx -c filename   为 Nginx 指定一个配置文件，来代替缺省的。
    nginx -t            不运行，而仅仅测试配置文件。nginx 将检查配置文件的语法的正确性，并尝试打开配置文件中所引用到的文件。
    nginx -v            显示 nginx 的版本。
    nginx -V            显示 nginx 的版本，编译器版本和配置参数。

这些命令也都可以以kill形式发出，如 kill -QUIT nginx主进程号

## work
nginx会先启动的进程称为master，然后他会fork出work进程，master做配置文件加载，维持work存活等管理工作，而work才是真正处理连接的进程

多进程的模式可以避免单进程死亡服务停止，同时热部署时只需要再创建新进程，待旧进程处理完工作后杀死

![](img/1.png)

# 配置文件
他的配置文件一般在 /etc/nginx/，nginx下有两个配置文件，conf.d/default.conf，用来配置不常改变的，整个服务运行时的配置信息，例如监听端口号，另一个是nginx.conf，默认配置文件，主要用来设置http服务等经常改变的配置信息，可以使用nginx -c指定其他配置文件

nginx.conf 主要分为四部分，main全局设置、server主机设置、upstream上游服务器设置、location URL匹配特定位置后的设置 

## 基本配置
```
user  nginx;
worker_processes  4; # work进程，建议设置auto，和核心值相同

include /usr/share/nginx/modules/*.conf; # 导入其他配置文件

error_log  /var/log/nginx/error.log warn; # 日志文件和日志级别
pid        /var/run/nginx.pid; # 存放主进程pid文件的位置

events {
   #设置网路连接序列化，防止惊群现象发生，默认为on
   accept_mutex on; 
   #设置一个进程是否同时接受多个网络连接，默认为off
   multi_accept on; 
   #事件驱动模型，select|poll|kqueue|epoll|resig|/dev/poll|eventport
   #use epoll; # socket处理函数，默认用epoll就好
   #单个work进程允许的最大连接数，默认为512
   worker_connections 1024; 
   worker_rlimit_nofile 10000; # 重新设定可监听文件描述符数量，避免ulimit -c影响
}
```

- 惊群效应，惊群效应（thundering herd）是指多进程（多线程）在同时阻塞等待同一个事件的时候（休眠状态），如果等待的这个事件发生，那么他就会唤醒等待的所有进程（或者线程），但是最终却只能有一个进程（线程）获得这个时间的“控制权”，对该事件进行处理，而其他进程（线程）获取“控制权”失败，只能重新进入休眠状态，这种现象和性能浪费就叫做惊群效应。


## 内置变量
通过 $var 使用变量
```
$host	         请求信息中的 Host，如果请求中没有 Host 行，则等于设置的服务器名
$request_method	 客户端请求类型，如 GET、POST
$remote_addr 	 客户端的 IP 地址
$args	         请求中的参数
$content_length  请求头中的 Content-length 字段
$http_user_agent 客户端 agent 信息
$http_cookie	 客户端cookie信息
$remote_port	 客户端的端口
$server_protocol 请求使用的协议，如 HTTP/1.1
$server_addr	 服务器地址
$server_name	 服务器名称
$server_port	 服务器的端口号
```