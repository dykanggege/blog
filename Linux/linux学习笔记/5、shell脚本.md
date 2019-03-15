shell就是linux下一个批处理文件，用于写指令并一次性执行

# 基本格式
- 命令太长了一行放不下可以用 \ 换行放置
- # 注释

    #!/bin/bash
    PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
    export PATH
    echo -e "Hello World! \a \n"
    exit 0

- #!/bin/bash 是告诉linux用哪个shell执行该脚本，一般也会默认/bin/bash

可以直接使用 ./test.sh 执行脚本(如果有执行权限的话)，或者使用 sh ./test.sh
