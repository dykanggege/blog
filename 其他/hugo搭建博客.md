# hugo 安装
我们可以直接从 [Github Release](https://github.com/gohugoio/hugo/releases) 页面下载对应的二进制文件,然后把它放在你的任意目录并加入到环境变量中即可。这个支持任何平台，根据自己的平台选择相应的二进制包即可。

# hugo 常用命令
- hugo new site blog 在任意目录下使用，生成一个 blog 目录作为创建的站点
- 在站点目录中创建主题

```
    cd blog;\
    git init;\
    git submodule add https://github.com/budparr/gohugo-theme-ananke.git themes/ananke;\
    # Edit your config.toml configuration file
    # and add the Ananke theme.
    echo 'theme = "ananke"' >> config.toml
```

- 添加文章

```
    hugo new posts/my-first-post.md
```
- 运行查看效果
```
    hugo server -D
```
然后我们就可以打开浏览器，输入http://localhost:1313查看我们新创建的站点了。