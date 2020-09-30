influxdb是一个时序数据库，主要解决监控痛点

- 数据量大，但大量指标是重复的（压缩数据）
- 计算数据量大，但通常针对几个纬度计算（快速计算）
- 数据通常和特定时间关联，且时间密度大


# 安装

```
#! /bin/bash

mkdir -p data

docker run -d --name influxdbserver \
    --mount type=bind,source=$PWD/data,target=/var/lib/influxdb \
    --mount type=bind,source=$PWD/influxdb.conf,target=/etc/influxdb/influxdb.conf \
    --network host \
    influxdb
```

- 数据位置：/var/lib/influxdb
- 配置文件：/etc/influxdb/influxdb.conf
- influxd：server端，使用8086监听HTTP连接，8088监听TCP
- influx：client端，influx -database 'metrics' -host 'localhost' -port '8086' 默认连接localhost:8086

