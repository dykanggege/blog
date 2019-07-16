cargo是rust的集成环境，起到管理项目，编译代码等作用

- cargo new project-name 创建项目
- cargo build 编译项目
    - cargo build --release 优化编译，生成用于生产的可执行文件
- cargo run 直接运行项目
- cargo check 确保代码可编译，但是并不输出编译后的文件

如果想要引入外部包，应当修改Cargo.toml文件，填写外部包信息后，cargo build重新编译