rpc 远程服务调用，本质和http协议类似，封装格式传输数据并反射调用，单语言中rpc容易实现，而多语言中首选要解决传参和接收值类型一致问题，最简单的做法是将多种语言抽象出公共类型模板配置文件，使用配置文件作为类型的载体

rpc编程里有三个身份

- 接口制定者：制定了向外暴露接口的规则，无论细节怎么更改，客户端都会遵循这个接口调用
- 实现服务：根据接口实现具体细节
- 调用服务：根据接口调用服务

# protocol buffers

将多语言抽离出公共类型格式并不难，但为了性能，要保证该配置文件编码解码简单，压缩紧凑

抽离出语言无关、平台无关的类型格式后，再为每个语言提供一个翻译器，能将配置文件自动生成数据代码，爽翻了

```
syntax = "proto3";  //声明使用proto3协议，默认为2

//java的配置信息
option java_multiple_files = true;
option java_package = "io.grpc.examples.routeguide";
option java_outer_classname = "RouteGuideProto";

package test;  //包名,防止命名冲突，要使用其他proto文件中数据，要加上命名空间

Protocol Buffers //导入其他包

enum PhoneType{
    option allow_alias = true;
    HOME = 0; //第一个枚举值必须是0，枚举值不能重复，除非使用 option allow_alias = true 选项来开启别名
    WORK = 1;
    ALISE = 1;
}
PhoneType pt = 0;

//最小消息体，定义数据结构
message Phone{  //消息定义的关键字
    PhoneType type = 1; //定义字段的编号,每个字段都有唯一的编号，1-15只用1字节编码，应该将常用的编码为1-15，不能用19000-19999
    string number = 2;
}

message Person{
    int32 id = 1;
    string name = 2;
    repeated Phone phones = 3;  //字段可以被重复任意多次（包括0次）
}

message ContactBook{
    repeated Person persons = 1; //可以嵌套或者组合
}

//定义一个服务接口
service RouteGuide {
    //一个简单的rpc调用
    rpc GetFeature(Point) returns (Feature) {}

    //流式rpc
    rpc ListFeatures(Rectangle) returns (stream Feature) {}

    rpc RecordRoute(stream Point) returns (RouteSummary) {}

    rpc RouteChat(stream RouteNote) returns (stream RouteNote) {}
}


message Point {
  int32 latitude = 1;
  int32 longitude = 2;
}


message Rectangle {
  // One corner of the rectangle.
  Point lo = 1;

  // The other corner of the rectangle.
  Point hi = 2;
}

// A feature names something at a given point.
//
// If a feature could not be named, the name is empty.
message Feature {
  // The name of the feature.
  string name = 1;

  // The point where the feature is detected.
  Point location = 2;
}

// A RouteNote is a message sent while at a given point.
message RouteNote {
  // The location from which the message is sent.
  Point location = 1;

  // The message to be sent.
  string message = 2;
}

// A RouteSummary is received in response to a RecordRoute rpc.
//
// It contains the number of individual points received, the number of
// detected features, and the total distance covered as the cumulative sum of
// the distance between each point.
message RouteSummary {
  // The number of points received.
  int32 point_count = 1;

  // The number of known features passed while traversing the route.
  int32 feature_count = 2;

  // The distance covered in metres.
  int32 distance = 3;

  // The duration of the traversal in seconds.
  int32 elapsed_time = 4;
}
```