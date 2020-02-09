# Dockerfile

Dockerfile 是一个文本文件,其内包含了一条条的指令(Instruction),每一条指令构建一层,因此每一条指令的内容,就是描述该层应当如何构建。

Dockerfile 支持 Shell 类的行尾添加 \ 的命令换行方式,以及行首 # 进行注释的格式。良好的格式,比如换行、缩进、注释等,会让维护、排障更为容易,这是一个比较好的习惯

## FROM
docker的镜像是层层构建的，所以新的镜像可以依赖于已存在的镜像，FROM就是用来说明依赖的镜像

scratch 镜像是虚拟的概念,并不实际存在,它表示一个空白的镜像，没有依赖就可以用 FROM scratch

## COPY
复制文件到镜像中

copy的源目录由构建时指定，目的根目录由WORKDIR指定

<源路径> 可以是多个,甚至可以是通配符,其通配符规则要满足 Go 的filepath.Match 规则,如:

    COPY hom* /mydir/
    COPY hom?.txt /mydir/

此外，还需要注意一点，使用 COPY 指令，源文件的各种元数据都会保留。比如读、写、执行权限、文件变更时间等。这个特性对于镜像定制很有用。特别是构建相关文件都在使用 Git 进行管理的时候。

在使用该指令的时候还可以加上 --chown=<user>:<group> 选项来改变文件的所属用户及所属组。

## ADD
add是更高级的文件复制，被复制对象可以是url，也可以是压缩包（自动解压），但是能用COPY就不要用ADD，因为ADD的语义比较模糊

## RUN
RUN 指令是用来执行命令行命令的，它会在编译镜像时执行，执行带来的改变被封装在镜像内了，其格式有两种

1. shell 格式: RUN <命令> ,就像直接在命令行中输入的命令一样。RUN echo '\<h1>Hello, Docker!\</h1>' > /usr/share/nginx/html/index.html
2. exec 格式: RUN \["可执行文件", "参数1", "参数2"\] ,这更像是函数调用中的格式。

每一个命令都会构建一层，太多的层毫无意义，所以尽可能的把所有的run命令写在一个命令中，RUN echo 1 && echo 2

记得每一层构建的最后一定要清理掉无关文件

## CMD
我们知道启动一个容器的本质就是启动一个进程，启动一个进程时可以携带附加参数，CMD就是容器启动附加参数，相当于 docker run centos CMD，只不过我们设置的CMD是个启动默认值，如果run时加了参数，就会代替CMD

容器启动的默认命令来源于FROM镜像，通过CMD可以修改这个启动命令

它和RUN的区别是，RUN在编译时执行，带来的改变被永久封装在镜像中了，CMD是镜像每次启动容器时执行

在运行时可以指定新的命令来替代镜像设置中的这个默认命令,比如, ubuntu镜像默认的 CMD 是 /bin/bash ,如果我们直接 docker run -it ubuntu 的话,会直接进入 bash 。我们也可以在运行时指定运行别的命令,如 docker run -it ubuntu cat /etc/os-release 。这就是用 cat/etc/os-release命令替换了默认的 /bin/bash 命令了,输出了系统版本信息。

在指令格式上,一般推荐使用 exec 格式,这类格式在解析时会被解析为 JSON数组,因此一定要使用双引号 " ,而不要使用单引号

如果使用shell的形式，实际的命令会被包装为 sh -c 的参数的形式进行执行，比如：

    CMD echo $HOME
    CMD [ "sh", "-c", "echo $HOME" ]

这个脚本执行完进程就结束了

尤其需要注意的是，容器是个进程，当主线程里的任务完成后进程就会退出，如果我们在CMD里命令后台运行一个程序，比如

    CMD service nginx start

程序就会立刻退出，因为这个主线程执行完成了，所以应该要求前台运行的形式

## ENTRYPOINT
ENTRYPOINT是容器的入口点，是“容器即服务”的关键，当指定了 ENTRYPOINT 后，CMD 的含义就发生了改变，不再是直接的运行其命令，而是将 CMD 的内容作为参数传给 ENTRYPOINT 指令

    <ENTRYPOINT> "<CMD>"

如果我们要在容器内封装服务，将ENTRYPOINT设置为启动服务的命令，CMD将变成服务启动时传入的参数，甚至根据CMD的值做出不同的选择

ENTRYPOINT 的格式和 RUN 指令格式一样，分为 exec 格式和 shell 格式。

## ENV
设置环境变量，设置完之后后面就可以直接使用

    ENV <key> <value>
    ENV <key1>=<value1> <key2>=<value2>...

## VOLUME匿名数据卷

    VOLUME ["<路径1>", "<路径2>"...]
    VOLUME <路径>

容器运行时，应该尽量保证容器存储层不发生写操作，对于数据库类需要保存动态数据的应用，其数据库文件应该放在卷中。为了防止运行时用户忘记将动态文件所保存目录挂载为卷，在 Dockerfile 中，我们可以事先指定某些目录挂载为匿名卷，这样在运行时如果用户不指定挂载，其应用也可以正常运行，不会向容器存储层写入大量数据。

但是我们指定的路径也不是以宿主机的相对路径，而是以docker程序目录下某个文件夹的相对路径，即这个数据卷并不能挂载到我们希望的位置，真正想要挂载数据卷还是要靠 run 时的传入

## EXPOSE 声明端口
EXPOSE 指令是声明运行时容器提供服务端口，这只是一个声明，在运行时并不会因为这个声明应用就会开启这个端口的服务。在 Dockerfile 中写入这样的声明有两个好处，一个是帮助镜像使用者理解这个镜像服务的守护端口，以方便配置映射；另一个用处则是在运行时使用随机端口映射时，也就是 docker run -P 时，会自动随机映射 EXPOSE 的端口。

## WORKDIR
格式为 WORKDIR <工作目录路径>。

使用 WORKDIR 指令可以来指定工作目录（或者称为当前目录），以后各层的当前目录就被改为指定的目录，如该目录不存在，WORKDIR 会帮你建立目录。

    RUN cd /app
    RUN echo "hello" > world.txt

如果你这样写了Dockerfile，那么会发现在app下找不到world.txt文件，因为他们是分层构建的，两层是两个运行环境，命令互不影响的，应该使用WORKDIR指定工作目录

## USER 指定当前用户
USER 指令和 WORKDIR 相似，都是改变环境状态并影响以后的层。WORKDIR 是改变工作目录，USER 则是改变之后层的执行 RUN, CMD 以及 ENTRYPOINT 这类命令的身份。

当然，和 WORKDIR 一样，USER 只是帮助你切换到指定用户而已，这个用户必须是事先建立好的，否则无法切换。

## HEALTHCHECK 健康检查
在之前我们都是通过容器内主线程退出与否判断容器状态，如果容器内的核心功能已死，但主线程未推出，我们就无法检测到容器异常，且浪费了资源，所以提供了这种机制去检测功能健康与否

    HEALTHCHECK [选项] CMD <命令>：设置检查容器健康状况的命令,
    HEALTHCHECK NONE：如果基础镜像有健康检查指令，使用这行可以屏蔽掉其健康检查指令
    选项：
        --interval=<间隔>：两次健康检查的间隔，默认为 30 秒；
        --timeout=<时长>：健康检查命令运行超时时间，如果超过这个时间，本次健康检查就被视为失败，默认 30 秒；
        --retries=<次数>：当连续失败指定次数后，则将容器状态视为 unhealthy，默认 3 次。
        --start-period=<间隔>: 应用的启动的初始化时间，在启动过程中的健康检查失效不会计入，默认 0 秒（从 V17.05 引入）
    命令的返回值决定了该次健康检查的成功与否：0：成功；1：失败；2：保留，不要使用这个值。

当在一个镜像指定了 HEALTHCHECK 指令后，用其启动容器，初始状态会为 starting，在 HEALTHCHECK 指令检查成功后变为 healthy，如果连续一定次数失败，则会变为 unhealthy。

为了帮助排障，健康检查命令的输出（包括 stdout 以及 stderr）都会被存储于健康状态里，可以用 docker inspect 来查看。

    FROM elasticsearch:5.5
    HEALTHCHECK --interval=5s --timeout=2s --retries=12 \
    CMD curl --silent --fail localhost:9200/_cluster/health || exit 1

## ONBUILD 为他人做嫁衣裳
    
    格式：ONBUILD <其它指令>。

ONBUILD 是一个特殊的指令，它后面跟的是其它指令，比如 RUN, COPY 等，而这些指令，在当前镜像构建时并不会被执行。只有当以当前镜像为基础镜像，去构建下一级镜像的时候才会被执行。

Dockerfile 中的其它指令都是为了定制当前镜像而准备的，唯有 ONBUILD 是为了帮助别人定制自己而准备的。

## 构建镜像
    docker build [选项] <上下文路径/URL/->
    -t 镜像名[：tag]
    -f 指定某个文件为 Dockerfile

最后的上下文路径一般我们会用 . 即当前路径下，它代表的不是Dockerfile的存放路径，而是COPY、ADD等命令的根路径

在docker中分为client和server，我们使用client书写命令，调用server的远程API，在调用build命令时，其实是交给server来编译Dockerfile文件，所以client需要把上下文路径中所有文件打包给server，server才能找到编辑中命令需要的文件。所以我们应该把所需文件复制到Dockerfile所在文件夹中，否则可能会打包很多无用的资源，如果用 / 作为上下文编译，那还需要打包整个硬盘文件！同时也在说明：**copy中无法使用 .. 路径**

如果没有用 -f 指定文件名，默认使用当前文件夹下的 dockerfile/Dockerfile 文件

如果有文件想被忽略，可以写一个 .dockerignore

也可以使用远程文件直接构建

    docker build https://github.com/twang2218/gitlab-ce-zh.git#:11.1

# 多层次构建
如果想隐藏一些细节，可以分两次构建，第一次构建依赖及其配置，第二次用第一次构建的镜像作为基础镜像，用于生产

但是构建两个Dockerfile太麻烦，甚至还要写一个sh脚本整合，现在docker支持在一个Dockerfile中做多次构建

```
FROM golang:1.12 as builder

WORKDIR /var

COPY main.go .

RUN CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .

FROM alpine:latest as prod

RUN apk add --no-cache ca-certificates

WORKDIR /var

COPY --from=0 /var/app .

CMD ["./app"]
```
- --from=0，0代表第一阶段构建的镜像，也可以是1、2，即从 0 阶段镜像中拿一些数据出来
- 如果使用 as 对某一阶段命名，也可以用 --from=builder
- 也可以复制任意镜像中的文件。

    COPY --from=nginx:latest /etc/nginx/nginx.conf /nginx.conf

- 当我们只想构建 builder 阶段的镜像时，增加 --target=builder 参数即可

    $ docker build --target builder -t username/imagename:tag .

注意

    CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .


CGO_ENABLED=0 代表禁止使用CGO做交叉编译，即将依赖库静态链接在二进制中，build -a 强制所有文件重新编译，

## 构建最小镜像
开发人员一般会用dockerfile打包好镜像，然后将镜像及其用法交给运维部署

虽然镜像是分层构建，且多个镜像可公用中间层，若其他镜像没有用到这个中间层，那么你构建的镜像就切切实实的浪费了这么大的空间

所有我们要在保证dockerfile可读的情况下，尽量减少镜像大小，避免浪费太多空间

## .dockerignore
dockerfile build时，会首先将其所在文件夹下所有文件复制倒docker server，如果复制了无用的文件，就会浪费很多时间

同.gitignore，在dockerfile build时，设置忽略某些文件

## alpine
golang可以将程序编译成一个二进制文件，除了动态链接库，所有运行时依赖都被编译在二进制文件中

scratch是一个空白镜像，它是个虚拟的概念，并不存在，不提供任何运行依赖

alpine是docker官方出的一个精简过的linux最小子集，只有5M，一般建议直接用它，相比scratch只多了5M，但省了很多事

所以go项目可以在编译后再构建镜像，避免打包依赖进去

