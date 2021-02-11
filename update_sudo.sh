#!/bin/bash

rpm_sudo='sudo-1.8.23-10.tl2.1.x86_64.rpm'
release=`cat /etc/tlinux-release`
is_tl2=`$release|grep 'Tencent tlinux release 2'`

if [[ "$is_tl2" = "" ]];then
    echo '$release，非Tencent tlinux release 2.x'
    exit 0
fi

code=`rpm -Uvh $rpm_sudo`

if [[ $code -ne 0 ]];then
    echo '更新失败'
fi

