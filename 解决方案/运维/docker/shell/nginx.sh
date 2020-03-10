#!/bin/bash

mkdir -p html
mkdir -p log
touch conf

docker run -d \
        --network host \
        --mount type=bind,src=$PWD/conf,dst=/etc/nginx/nginx.conf \
        --mount type=bind,src=$PWD/log,dst=/var/log/nginx \
        --mount type=bind,src=$PWD/html,dst=/usr/share/nginx/html \
        --name nginxserver \
        nginx