# 命令
- pull：拉取镜像 pull golang:tag 如果没指定版本号，则拉取最新版
- images:列出所有本地镜像
    - -a 列出本地所有镜像，含中间镜像
    - -q 只显示镜像的　id
    - --digests 显示镜像的摘要信息
    - --no-trunc 显示完整的镜像摘要信息
- search golang: 从dockerHub查找镜像
- rmi：删除镜像
    - -f 强制删除（即使正在运行）
- rm：删除容器
    - docker rm `docker ps -a -q`
- run hello-world：根据镜像启动容器，注意容器是和镜像不一样的概念，如果run实质是create container 并 start container，如果 run 后没有删除容器（ｒｍ），容器会一直存在，ps -a 可以查看所有的容器
    - -t 为容器重新分配一个伪终端，通常与　-i 一起使用
    - -i 以交互模式运行容器，通常和 -t 一起使用
    - -p 端口映射　docker run -it -p 8888:8080 tomcat 将　tomcat 的对外公开端口设置为 8888
    - -P 大Ｐ，随机分配映射端口
    - -d 后台访问，在后台运行
- ps：列出所有正在运行的容器
    - -a 列出当前正在运行的和历史上运行过的
    - -l 显示最近创建的容器
- kill id：杀死容器
- logs：日志
- attach centos：进入容器，使用 exit 会退出并杀死容器，ctrl+p+q 只是退出容器，这时候就可以用 docker attach centosID 重新进入容器
- exec：不进入容器直接执行命令（正在运行的容器），得到命令结想｀果
    - -t 分配一个伪终端，针对于长久运行的(centos)，如果只是一次运行(gcc)，则不需要该参数
- cp：拷贝容器内的数据到宿主机
- commit：提交新的容器镜像
- inspect imageID 查看运行容器的配置


# 镜像
在　docker 中的每个镜像都不是单一的文件，而是一层层文件系统叠加而起，类似于 linux 查找依赖的感觉，而已经加载过的依赖也可以被其他镜像安全引用，千层饼一样的叠加

# 数据卷
将容器运行产生的数据或使用的文件和主机交互是很重要的，可以将容器的目录挂载在宿主机上，相当于Ｕ盘挂载一样
- docker run -it -v /myDataVolume:/dataVolumeContainer centos 将本地的一个文件夹像U盘一样挂载在容器中

## dockerfile VOLUME
镜像的描述文件，编写好 dockerfile　文件后，可以直接用命令生产　docker 镜像

- docker build -f dockerfile -t 新的镜像名 .(在当前目录下)
    - -f 指定要使用的 dockerfile 的路径
    - -t　镜像的名字和标签，通常指定 name:tag　如果不指定标签，则默认是最后版本


可以用　VＯLUME　指令给镜像添加一个或多个数据卷，如果只指定了镜像的数据卷，没有指定宿主机的挂载卷，则镜像自己会指定一个默认的挂载点

## --volumes-from 
docker run -it --name k1 kanggege/centos
docker run -it --name k2 --volumes-from k1 kanggege/centos  
将一个容器的数据卷挂载到另一个容器的挂载点上


# dockerfile

## 指令字
- FROM 基础镜像，类似与父类
- MAINTAINER 作者和作者的邮箱
- RUN 容器构建时需要执行的指令
- EXPOSE 当前容器对外暴露的端口
- WORKDIR 创建容器后，终端默认登录进来工作目录，落脚点
- ENV 在构建环境中设置的环境变量
- ADD 将宿主机目录下的文件拷贝进镜像且ＡＤＤ命令会自动处理ＵＲＬ和解压ｔａｒ压缩包
- COPY 类似于ADD，拷贝文件和目录到镜像中，只复制，不做特殊处理
- VOLUME　容器数据卷，持久化容器运行数据
- CMD 制定容器启动时要运行的命令，只有最后一条指令生效
- ENTRYPOINT 所有添加的指令都会生效
- ONBULID　当构建一个被继承的 dockerfile 时运行的命令，父镜像在被子镜像继承后父镜像的build 被触发

dockerfile 中每个指令都是一层，最好不要用太多指令（好像不能超过127层）
