# chroot
chroot，即 change root directory (更改 root 目录)。在 linux 系统中，系统默认的目录结构都是以 `/`，即是以根 (root) 开始的。而在使用 chroot 之后，系统的目录结构将以指定的位置作为 `/` 位置。

在经过 chroot 之后，系统读取到的目录和文件将不在是旧系统根下的而是新根下(即被指定的新的位置)的目录结构和文件

要注意的是，切换到一个新目录作为根目录下，要能够找到bash，否则无法执行命令行

# source
source /etc/profil，将该脚本里命令重新执行一次

source 也被称为点命令，作用是执行脚本命令