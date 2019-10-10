开发一个工程，首要重要的就是管理工程

# 项目管理

    http://tapd.oa.com //主要写项目开发管理文档，项目背景，开发情况，记录进度等

skyflow wiki：

    http://tapd.oa.com/CDB_DEV_OPS/markdown_wikis/view/#1010142841009531103

# 代码管理
工蜂管理平台

    https://git.code.oa.com

本机生成公钥私钥，在gitcode上配置公钥

skyflow代码地址(需要被拉入项目，获取权限)

    https://git.code.oa.com/matthewmu/skyflow

拉取整个项目后，应当有 README.md 介绍项目结构、启动等等，针对项目代码的文档

## 分支与版本
代码管理最重要的就是分支和版本

    主版本号.次版本号.release

- 主版本号是重大版本发布，可能包含很多功能
- 功能演进，如性能优化、小功能迭代
- release，修补bug或小功能点改进，可以用日期作为版本号

分支管理也很重要

- master 稳定版本
- dev 开发分支，包含具体版本号 dev_

# 云开发
云开发平台

    http://devcloud.oa.com

申请一个vscode云主机

本机安装WeTEAM，ssh客户端

## 配置云开发环境
- 本机和云协同vscode
- 配置git和gitcode
- 拉取代码
- 安装开发依赖
- 配置数据库
- 设置数据库用户

# 测试、生产环境
大家在自己的环境开发，但是应该有统一的测试环境和生产环境

# 前端
使用vue-cli启动项目，前端架构基于已有模板

    https://git.code.oa.com/omc-dev/vue_cli_oss


# 后台

## 启动项目
提供项目cli工具，/build/build.sh 生成cli工具在/bin目录下
