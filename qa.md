发行版-rpm&yum-yum配置问题
发行版-rpm&yum-rpm包安装问题
发行版-rpm&yum-yum安装软件包问题
发行版-rpm&yum-软件包文件被篡改修复
发行版-rpm&yum-软件安装依赖问题
发行版-rpm&yum-要求安装指定版本
发行版-rpm&yum-其它问题

发行版-操作系统安装-ISO使用问题
发行版-操作系统安装-cdrom安装、pxe安装、u盘安装问题
发行版-操作系统安装-系统启动失败问题
发行版-操作系统安装-其它问题

发行版-母盘镜像-母盘版本识别
发行版-母盘镜像-母盘版本差异
发行版-母盘镜像-自定义母盘和镜像
发行版-母盘镜像-内外镜像使用
发行版-母盘镜像-其它问题

发行版-系统故障-lib库误操作导致系统异常
发行版-系统故障-lmount不掉
发行版-系统故障-ssh异常
发行版-系统故障-Systemd服务异常
发行版-系统故障-僵尸进程无法杀死
发行版-系统故障-网络异常，网卡命名异常
发行版-系统故障-Grub配置问题
发行版-系统故障-系统启动失败
发行版-系统故障-系统命令缺失或异常
发行版-系统故障-系统配置错误
发行版-系统故障-文件系统异常
发行版-系统故障-删除文件后空间未释放
发行版-系统故障-软件编译问题
发行版-系统故障-密码设置问题
发行版-系统故障-误删系统文件
发行版-系统故障-文件权限问题
发行版-系统故障-其它问题

发行版-软件使用问题-时间同步问题
发行版-软件使用问题-网络配置问题
发行版-软件使用问题-kdump失败
发行版-软件使用问题-gcc升级
发行版-软件使用问题-scl源使用
发行版-软件使用问题-模块签名问题
发行版-软件使用问题-T命令使用
发行版-软件使用问题-Docker使用
发行版-软件使用问题-DNS配置
发行版-软件使用问题-Systemd使用
发行版-软件使用问题-cuda安装
发行版-软件使用问题-软件包版本升级
发行版-软件使用问题-常用软件安装与使用
发行版-软件使用问题-软件包版本升级
发行版-软件使用问题-其它问题

发行版-windows系统问题-激活失败
发行版-windows系统问题-镜像问题
发行版-windows系统问题-使用问题
发行版-windows系统问题-其它问题

内核-咨询-内核版本
内核-咨询-硬件
内核-咨询-系统工具
内核-咨询-参数配置
内核-咨询-其它问题

内核-非宕机类-性能类-cpu高负载
内核-非宕机类-性能类-IO时延问题
内核-非宕机类-性能类-网络时延问题
内核-非宕机类-性能类-软中断性能问题
内核-非宕机类-性能类-其它问题

内核-非宕机类-非性能类-secureboot启动失败
内核-非宕机类-非性能类-数据丢失/文件损坏
内核-非宕机类-非性能类-ping不可通
内核-非宕机类-非性能类-agent上报超时
内核-非宕机类-非性能类-UNKNOWN NMI
内核-非宕机类-非性能类-kdump失败
内核-非宕机类-非性能类-内核模块使用失败
内核-非宕机类-非性能类-僵尸进程
内核-非宕机类-非性能类-io error
内核-非宕机类-非性能类-ceph类问题
内核-非宕机类-非性能类-CBS/NFS类问题
内核-非宕机类-非性能类-xfs, ext类文件系统问题
内核-非宕机类-非性能类-协议栈相关问题
内核-非宕机类-非性能类-内存使用相关问题
内核-非宕机类-非性能类-信号处理相关问题
内核-非宕机类-非性能类-安全问题
内核-非宕机类-非性能类-其它问题

内核-宕机类-硬件故障
内核-宕机类-非硬件故障





curl -d
'{
    "id":1000595,
    "act": "query",
    "task_value": {
        "namespace": "",
        "tke_master_ip_first": "",
    }
}' 
-H "content-type:application/json"
-H "token":"xxx"
-H "staffname":"xxx"
-H "projectname":"xxx"
http://openapi.zhiyan.oa.com/operate/v1/exec_task


curl 'http://cpu.oa.com:8080/business/apis/business.tkestack.io/v1/portal' \
-H "Authorization: Bearer eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2MzU0NDc5MDcsImp0aSI6ImQwZTIiLCJpYXQiOjE1OTk0NDc5MDcsInVzciI6ImZ1a2FuZ2RpbmciLCJ0ZWQiOiJjYXAta2VuZHl3YW5nIn0.iA82ZsVyRzxJVn66qavT_HxxiAKqo_pmy_b77D3NoTtJzgOCgLOAYA64ZYx_LVNWubG22Zq8Q742P9grBz0MjEA0yH3c5xv8wp5NRrwZjeVimm7O9ilGlAESCgQMIo1D_uTy6jWlJUeJcBRadJ0NgdvySKR5mzjLpUbcxL5N3ctBDrUNZfzc0XyPNZ348oZxZSy-4m95pixe3AFRi5_e19Y-OFyI7FEEUAleEe4PkK85teaJg6xF73GeW8NwgFzx0UrvuBujYs0zlXntvLSKCOFr7em4cYfvjCXVMUpLovXRaoBLA4uj8zk1-sTGkmd1REGv0yljmhvw3pGOnfAjFg"

curl 'http://cpu.oa.com:8080/business/apis/business.tkestack.io/v1/namespaces/prj-jn6pkp4w/namespaces' \
-H "Authorization: Bearer eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2MzU0NDc5MDcsImp0aSI6ImQwZTIiLCJpYXQiOjE1OTk0NDc5MDcsInVzciI6ImZ1a2FuZ2RpbmciLCJ0ZWQiOiJjYXAta2VuZHl3YW5nIn0.iA82ZsVyRzxJVn66qavT_HxxiAKqo_pmy_b77D3NoTtJzgOCgLOAYA64ZYx_LVNWubG22Zq8Q742P9grBz0MjEA0yH3c5xv8wp5NRrwZjeVimm7O9ilGlAESCgQMIo1D_uTy6jWlJUeJcBRadJ0NgdvySKR5mzjLpUbcxL5N3ctBDrUNZfzc0XyPNZ348oZxZSy-4m95pixe3AFRi5_e19Y-OFyI7FEEUAleEe4PkK85teaJg6xF73GeW8NwgFzx0UrvuBujYs0zlXntvLSKCOFr7em4cYfvjCXVMUpLovXRaoBLA4uj8zk1-sTGkmd1REGv0yljmhvw3pGOnfAjFg"


cls-sxvk8zl4=9.136.124.195
cls-bx6xqlrx=9.150.192.163
cls-z5kqjrpd=9.130.208.103
cls-86bm4d4q=9.145.39.217
cls-6hwbp8jp=9.208.74.78

1. 防止map内存写爆
