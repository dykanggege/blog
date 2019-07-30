众所周知，javaee技术依赖于servlet，基于servlet的抽象封装，再加上各种黑科技，有了spring全家桶，spring里面的功能并不都是自己写的，很多都是依赖现有的jar包，有了依赖，就一定需要依赖管理工具，如果他不仅能能管理依赖，还能自动加载依赖、构建、打包

- jar包：已经编译好的class文件，打包成jar格式，便于传输使用，可直接引入项目使用，jvm可直接使用
- war包：一般将javaweb项目打包成war，直接可以放在tomcat容器里部署

maven所有配置都在 pom.xml 文件中

- 分阶段依赖
- 自动构建，complie编译、package打包、install放到本地仓库，install会调用前面几个阶段


