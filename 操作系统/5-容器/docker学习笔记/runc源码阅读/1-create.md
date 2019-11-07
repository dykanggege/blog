# 配置
容器标准包 bundle 与配置

config.json： 基本配置文件，包括与宿主机独立的和应用相关的特定信息，如安全权限、环境变量和参数等。具体如下：

- 容器版本
- rootfs 路径及权限（ro / rw）
- 各类文件挂载点及相应容器内挂载目录（必须与 state.json 一致）
- 初始进程配置信息，包括是否绑定终端、工作目录、环境变量配置、可执行文件参数、uid、gid 以及额外需要加入的 hostname 等

state.json： 运行时配置文件（运行时主机相关的信息，如内存限制、设备访问权限、挂载点等）

rootfs：根文件系统目录，容器执行的环境依赖，如/bin、/var、/lib、/dev、/usr等目录及相应文件

# create

运行参数

```
Flags: []cli.Flag{
    cli.StringFlag{
        Name:  "bundle, b", //配置文件所在路径
        Value: "",
        Usage: `path to the root of the bundle directory, defaults to the current directory`,
    },
    cli.StringFlag{
        Name:  "console-socket", //用unix socket作为stdin
        Value: "",
        Usage: "path to an AF_UNIX socket which will receive a file descriptor referencing the master end of the console's pseudoterminal",
    },
    cli.StringFlag{
        Name:  "pid-file", //使用该文件指定pid
        Value: "",
        Usage: "specify the file to write the process id to",
    },
    cli.BoolFlag{
        Name:  "no-pivot",
        Usage: "do not use pivot root to jail process inside rootfs.  This should be used whenever the rootfs is on top of a ramdisk",
    },
    cli.BoolFlag{
        Name:  "no-new-keyring",
        Usage: "do not create a new session keyring for the container.  This will cause the container to inherit the calling processes session key",
    },
    cli.IntFlag{
        Name:  "preserve-fds",
        Usage: "Pass N additional file descriptors to the container (stdio + $LISTEN_FDS + N in total)",
    },
},

```

运行行为

```
    Action: func(context *cli.Context) error {
		//验证参数个数是否合法
		if err := checkArgs(context, 1, exactArgs); err != nil {
			return err
		}
		//把 pid-file 修改为绝对路径
		if err := revisePidFile(context); err != nil {
			return err
		}
		//bound为配置文件的路径，加载配置文件
		spec, err := setupSpec(context)
		if err != nil {
			return err
		}
		status, err := startContainer(context, spec, CT_ACT_CREATE, nil)
		if err != nil {
			return err
		}
		// exit with the container's exit status so any external supervisor is
		// notified of the exit with the correct exit status.
		os.Exit(status)
		return nil
	},
```

## startContainer

