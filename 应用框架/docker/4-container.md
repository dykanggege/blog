# 启动容器
根据镜像创建的容器就像是根据代码跑一次进程，但是docker中还可以将在终止状态的容器重新启动

    docker [container] run
    等同于
    docker container create IMAGE
    docker container start CONTAINER

1. 检查本地是否存在指定的镜像，不存在就从公有仓库下载
2. 利用镜像创建并启动一个容器
3. 分配一个文件系统，并在只读的镜像层外面挂载一层可读写层
4. 从宿主主机配置的网桥接口中桥接一个虚拟接口到容器中去
5. 从地址池配置一个 ip 地址给容器
6. 执行用户指定的应用程序
7. 执行完毕后容器被终止

注意：docker启动容器的本质是启动并隔离镜像中的进程，并不是重新启动操作系统，所以在创建新容器时候需要注意一下几点

- 诸如: /etc/rcN.d 或 /etc/rc.local 文件，都会母机复制并覆盖容器内原有的内容
- /etc/resolve.conf、/ect/hostname 类似的网络配置，如果docker中没有特殊配置，也会从母机复制并覆盖容器文件

# 查看容器
查看正在运行的容器    

    docker container ls
    docekr ps
        -a 查看所有容器信息，已退出的和正在运行的

要获取容器的输出信息，可以通过 docker container logs 命令

    docker container logs [container ID or NAMES]

# 终止容器

    docker container stop
    docker stop 

stop会给容器内进程发送信号，使其结束并做清理工作，可能不会立即停止。此外，当 Docker 容器中指定的应用终结时，容器也自动终止。

只启动了一个终端的容器，用户通过 exit 命令或 Ctrl+d 来退出终端时，所创建的容器立刻终止

终止状态的容器可以用 docker container ls -a 命令看到

docker kill 会强制杀死容器，立刻停止

## 启动后台容器
容器启动后，默认是在前台运行，打印所有日志消息，加上 -d 的是在后台运行，如果想从前台切换到后台，可以使用Ctrl + P + Q组合键

再次进入一个在后台运行的容器有两种方式

### attach

docker attach dreamy_ritchie或者用ID 

使用attach进入后台运行的程序，如果从这个 stdin 中 exit，会导致容器的停止

一般建议中 exec

### exec
    
    docker exec [OPTIONS] CONTAINER COMMAND [ARG...]
    -d :分离模式: 在后台运行
    -i :即使没有附加也保持STDIN 打开
    -t :分配一个伪终端

使用exec进入后台运行的程序，如果从这个 stdin 中 exit，不会导致容器的停止，一般建议用exec

## 启动已终止的容器

    docker container start 容器id
    docker start
    docker container restart id 重新启动
    docker restart

容器终止后，只要不是被删除了，就还保留上次的信息，重新启动容器是以上次为基准

# 导出与导入容器
如果要导出本地某个容器，可以使用 docker export 命令

    docker export 7691a814370e > ubuntu.tar

可以使用 docker import 从容器快照文件中再导入为镜像，例如

    docker import [OPTIONS] file|URL|- [REPOSITORY[:TAG]]
    docker import http://example.com/exampleimage.tgz example/imagerepo

# 删除容器
    docker rm id

    docker container prune  清理所有处于终止状态的容器

# 其他
docker container

- attach：将一个container的io重定向到stdio上
- cp：从本地copy文件到container中
- diff：查看container对filesystem做了啥改动
- export：将container打包导出，import导入
- pause：暂停容器内所有进程，unpause 恢复运行
- wait：调用后会阻塞，知道某个container结束退出
- update：用于更新对container限制的资源