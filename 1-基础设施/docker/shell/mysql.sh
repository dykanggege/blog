#!/bin/bash

pwd=`pwd`

touch my.cnf
mkdir -p data
mkdir -p mysql-files
mkdir -p backup

echo `[mysqld]
user = mysql
character-set-server=utf8
init_connect='SET NAMES utf8'
default_authentication_plugin = mysql_native_password

[client]
default-character-set = utf8mb4

[mysql]
default-character-set = utf8mb4` > my.cnf

docker run -d \
        --privileged=true \
        --network host \
        -e MYSQL_ROOT_PASSWORD=kanggege \
        --mount type=bind,src=${pwd}/my.cnf,dst=/etc/mysql/my.cnf,readonly \
        --mount type=bind,src=${pwd}/data,dst=/var/lib/mysql \
        --mount type=bind,src=${pwd}/mysql-files,dst=/var/lib/mysql-files \
        --name mysqlserver \
        mysql