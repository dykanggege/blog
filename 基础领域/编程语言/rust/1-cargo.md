1. rustup，rust版本以及工具管理器，rustc编译器，cargo依赖管理工具
2. rustc、cargo都可以编译，通常建议使用cargo，除了对rust升级管理，一般都建议使用cargo


# cargo

- cargo new hello //创建以为名为hello的项目，自动生成hello文件夹
- cargo run 运行项目，自动编译运行
- cargo build 编译但不运行
- cargo doc 查看文档


- src 放置源码
    - main.rs 入口文件
- target 目标生成代码
    - debug 运行时产生的代码
- Cargo.lock 项目依赖锁定，不用管它
- Cargo.toml 项目配置目录