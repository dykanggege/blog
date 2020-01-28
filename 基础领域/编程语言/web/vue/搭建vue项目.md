先按照官网给的安装方式 npm 后，在要创建项目的文件夹下 vue ui 会打开一个面板用于管理项目，对于不常做 vue　的我，不用再去查那些毫无意义的命令

在控制面板里可以做一切，编译，部署，等等

# 目录结构
- dist 放置编译压缩后的文件，用于生产环境
- node_modules　依赖的模块，注意上传 github 不要传这个，通过 package.json 的配置可以再下载
- public 该文件夹下的资源不会被　webpack 打包，通过绝对路径引用
- src 生产目录
    - assets　放置静态资源，图片之类的，会被webpack打包
    - components 组件
    - App.vue　真正的入口文件
    - main.js 项目启动入口，多用于配置，绑定渲染了 App.vue
- package.json 项目依赖
- 其他依赖文件

    