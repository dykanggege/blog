```
# 重要，配置http
http {
   include       /etc/nginx/mime.types;
   default_type  application/octet-stream;

   # 设置使用的字符集，如果该网站可能有多个字符集，不应该在这里设置，而是在页面meta中设置
   # charset utf-8; 
   
   log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                     '$status $body_bytes_sent "$http_referer" '
                     '"$http_user_agent" "$http_x_forwarded_for"';

   # 设置客户端能够上传文件大小
   client_max_body_size 8m;
   
   access_log  /var/log/nginx/access.log  main;
   
   sendfile        on; # 直接在内核中的文件描述符之间传输，不需要copy数据到用户缓冲区
   tcp_nopush     on;

   #连接超时时间，默认为75s，可以在http，server，location块。
   keepalive_timeout  90;

   # cpu换网络
   gzip  on;

   include /etc/nginx/conf.d/*.conf;
    
   #设定实际的服务器列表
   upstream svc_backup {   
     server 127.0.0.1:7878;
     server 192.168.10.121:3333 backup; #热备(其它所有的非backup机器down或者忙的时候，请求backup机器))
   }

   upstream svc_weight {
   #weigth参数表示权值，权值越高被分配到的几率越大
     server 192.168.1.11:80 weight=5;
     server 192.168.1.12:80 weight=1;
     server 192.168.1.13:80 weight=6;
   }

   upstream svc_hash {
   #每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题
     ip_hash;
     server 192.168.1.11:90;
     server 192.168.1.12:90;
   }

   upstream svc_fair {
     fair; # 优先分配给响应最快的服务器
     server 192.168.1.11:90;
     server 192.168.1.12:90;
   }

   # 服务是最小监听单元    
   server{
      # server_name:listen/location 三者共同决定了分发路径
      listen 80;
      server_name localhost;
      
      # 所有以 /static 开头的 uri 都会从下面文件夹里找静态资源,最长匹配
      # 访问路径 /static/css/index.css 转发路径 html/css/index.css，即转发不会带前缀
      # 注意从上到下优先匹配
      location /static { 
         deny 192.168.1.1; # 禁止这个ip访问
         allow all;
         root  html; #  该配置文件所在文件夹下的相对路径 
         index index.html; # 默认返回页面
         expires     10h; # 设置过期时间为10小时  
      }

      access_log  /var/log/nginx/localhost.access.log;    
   }
}
```

- 文件分发由 server_name:listen/location 三者共同决定

- 使用 docker 时注意文件路径使用容器内的挂载路径，root相对路径是以容器内配置文件路径为根

- 如果多个IP但同一个端口，或者多个域名同一个端口，映射到不同的网站，好像将一台主机的端口重用了，这就是虚拟主机，主要通过 server 配置


# upstream
```
#http服务器
http {
   #连接超时时间，默认为75s，可以在http，server，location块。
   keepalive_timeout 65; 

   #gzip压缩开关
   gzip on;

   tcp_nodelay on;

   #设定实际的服务器列表
   upstream mysvr1 {   
     server 127.0.0.1:7878;
     server 192.168.10.121:3333 backup; #热备(其它所有的非backup机器down或者忙的时候，请求backup机器))
   }
   upstream mysvr2 {
   #weigth参数表示权值，权值越高被分配到的几率越大
     server 192.168.1.11:80 weight=5;
     server 192.168.1.12:80 weight=1;
     server 192.168.1.13:80 weight=6;
   }
   upstream https-svr {
   #每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题
     ip_hash;
     server 192.168.1.11:90;
     server 192.168.1.12:90;
   }

#error_page 404 https://www.baidu.com; #错误页

#HTTP服务器

# 静态资源一般放在nginx所在主机
   server {
       listen 80; #监听HTTP端口
       server_name 127.0.0.1; #监听地址  
       keepalive_requests 120; #单连接请求上限次数
       set $doc_root_dir "/Users/doing/IdeaProjects/edu-front-2.0"; #设置server里全局变量
       #index index.html;  #定义首页索引文件的名称
       location ~*^.+$ { #请求的url过滤，正则匹配，~为区分大小写，~*为不区分大小写。
          root $doc_root_dir; #静态资源根目录
          proxy_pass http://mysvr1; #请求转向“mysvr1”定义的服务器列表
          #deny 127.0.0.1; #拒绝的ip
          #allow 172.18.5.54; #允许的ip           
       } 
   }

#http
   server {
       listen 80;
       server_name www.helloworld.com; #监听基于域名的虚拟主机。可有多个，可以使用正则表达式和通配符
       charset utf-8; #编码格式
       set $static_root_dir "/Users/doing/static";
       location /app1 { #反向代理的路径（和upstream绑定），location后面设置映射的路径 
           proxy_pass http://zp_server1;
       } 
       location /app2 {  
           proxy_pass http://zp_server2;
       } 
       location ~ ^/(images|javascript|js|css|flash|media|static)/ {  #静态文件，nginx自己处理
           root $static_root_dir;
           expires 30d; #静态资源过时间30天
       }
       location ~ /\.ht {  #禁止访问 .htxxx 文件
           deny all;
       }
       location = /do_not_delete.html { #直接简单粗暴的返回状态码及内容文本
           return 200 "hello.";
       }

      # 指定某些路径使用https访问(使用正则表达式匹配路径+重写uri路径)
      location ~* /http* { #路径匹配规则：如localhost/http、localhost/httpsss等等
      #rewrite只能对域名后边的除去传递的参数外的字符串起作用，例如www.c.com/proxy/html/api/msg?method=1&para=2只能对/proxy/html/api/msg重写。
      #rewrite 规则 定向路径 重写类型;
      #rewrite后面的参数是一个简单的正则。$1代表正则中的第一个()。
      #$host是nginx内置全局变量，代表请求的主机名
      #重写规则permanent表示返回301永久重定向
      rewrite ^/(.*)$ https://$host/$1 permanent;
   }

   #错误处理页面（可选择性配置）
   #error_page 404 /404.html;
   #error_page 500 502 503 504 /50x.html;

   #以下是一些反向代理的配置(可选择性配置)
   #proxy_redirect off;
   #proxy_set_header Host $host; #proxy_set_header用于设置发送到后端服务器的request的请求头
   #proxy_set_header X-Real-IP $remote_addr;
   #proxy_set_header X-Forwarded-For $remote_addr; #后端的Web服务器可以通过X-Forwarded-For获取用户真实IP
   #proxy_connect_timeout 90; #nginx跟后端服务器连接超时时间(代理连接超时)
   #proxy_send_timeout 90; #后端服务器数据回传时间(代理发送超时)
   #proxy_read_timeout 90; #连接成功后，后端服务器响应时间(代理接收超时)
   #proxy_buffer_size 4k; #设置代理服务器（nginx）保存用户头信息的缓冲区大小
   #proxy_buffers 4 32k; #proxy_buffers缓冲区，网页平均在32k以下的话，这样设置
   #proxy_busy_buffers_size 64k; #高负荷下缓冲大小（proxy_buffers*2）
   #proxy_temp_file_write_size 64k; #设定缓存文件夹大小，大于这个值，将从upstream服务器传

   #client_max_body_size 10m; #允许客户端请求的最大单文件字节数
   #client_body_buffer_size 128k; #缓冲区代理缓冲用户端请求的最大字节数
}

#https
#(1)HTTPS的固定端口号是443，不同于HTTP的80端口；
#(2)SSL标准需要引入安全证书，所以在 nginx.conf 中你需要指定证书和它对应的 key
   server {
      listen 443;
      server_name  www.hellohttps1.com www.hellohttps2.com;
      set $geek_web_root "/Users/doing/IdeaProjects/backend-geek-web";
      ssl_certificate      /usr/local/etc/nginx/ssl-key/ssl.crt; #ssl证书文件位置(常见证书文件格式为：crt/pem)
      ssl_certificate_key  /usr/local/etc/nginx/ssl-key/ssl.key; #ssl证书key位置
      location /passport {
         send_timeout 90;
         proxy_connect_timeout 50;
         proxy_send_timeout 90;
         proxy_read_timeout 90;
         proxy_pass http://https-svr;
      }
      location ~ ^/(res|lib)/ {
         root $geek_web_root; 
         expires 7d;
         #add_header用于为后端服务器返回的response添加请求头，这里通过add_header实现CROS跨域请求服务器
         add_header Access-Control-Allow-Origin *; 
      }
      #ssl配置参数（选择性配置）
      ssl_session_cache shared:SSL:1m;
      ssl_session_timeout 5m;
   }

   #配置访问控制：每个IP一秒钟只处理一个请求，超出的请求会被delayed
   #语法：limit_req_zone  $session_variable  zone=name:size  rate=rate (为session会话状态分配一个大小为size的内存存储区，限制了每秒（分、小时）只接受rate个IP的频率)
   limit_req_zone  $binary_remote_addr zone=req_one:10m   rate=1r/s nodelay;
   location /pay {
        proxy_set_header Host $http_host;
        proxy_set_header X-Real_IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        #访问控制：limit_req zone=name [burst=number] [nodelay];
        limit_req zone=req_one burst=5; #burst=5表示超出的请求(被delayed)如果超过5个，那些请求会被终止（默认返回503）
        proxy_pass http://mysvr1;
   }

   #可以把子配置文件放到/usr/local/etc/nginx/servers/路径下，通过include引入
   include /usr/local/etc/nginx/servers/*.conf;
}
```

    $args ：这个变量等于请求行中的参数，同$query_string
    $content_length ： 请求头中的Content-length字段。
    $content_type ： 请求头中的Content-Type字段。
    $document_root ： 当前请求在root指令中指定的值。
    $host ： 请求主机头字段，否则为服务器名称。
    $http_user_agent ： 客户端agent信息
    $http_cookie ： 客户端cookie信息
    $limit_rate ： 这个变量可以限制连接速率。
    $request_method ： 客户端请求的动作，通常为GET或POST。
    $remote_addr ： 客户端的IP地址。
    $remote_port ： 客户端的端口。
    $remote_user ： 已经经过Auth Basic Module验证的用户名。
    $request_filename ： 当前请求的文件路径，由root或alias指令与URI请求生成。
    $scheme ： HTTP方法（如http，https）。
    $server_protocol ： 请求使用的协议，通常是HTTP/1.0或HTTP/1.1。
    $server_addr ： 服务器地址，在完成一次系统调用后可以确定这个值。
    $server_name ： 服务器名称。
    $server_port ： 请求到达服务器的端口号。
    $request_uri ： 包含请求参数的原始URI，不包含主机名，如：”/foo/bar.php?arg=baz”。
    $uri ： 不带请求参数的当前URI，$uri不包含主机名，如”/foo/bar.html”。
    $document_uri ： 与$uri相同。

    
# tcp_nodelay
TCP/IP网络有一个“小数据包”的问题，如果一条信息中只有一个字符，那么网络可能会堵塞。这样的数据包大小为41字节，其中TCP信头40字节，内容为1字节。像这种小数据包，它们的开销是4000%。大量的小数据包可以迅速让网络饱和。

John Nagle发明了Nagle算法，它在一定的时间段，将小数据包暂存，将这些小数据包集合起来，整合为一个数据包发送，在下一个时间段又是如此。这改善了网络传输的效率。时间段通常为200ms。

但值得注意的是，小数据包的问题在telnet的通信过程中仍然存在。在telnet中，每一次敲键都分别在网络发送。不过这跟web服务器没有关联。web服务器发送的文件大多是大数据包，所以它们会被立即发送，而不需要等待200ms。

TCP_NODELAY可以用来关闭Nagle的缓冲算法，将数据尽快地发送。Nginx可以在http, server, location当中定义

 http {
 tcp_nodelay on;
 }
Nginx默认启用了tcp_nodelay。Nginx在keep-alive模式下会使用tcp_nodelay。

# tcp_nopush
除了Nagle算法外，Linux内核提供了一个TCP_CORK选项，也可以解决小数据包问题。TCP_CORK告诉TCP栈将数据包附加在另外一个数据包，当数据包饱和时或程序明确地指示发送时再发送。在FreeBSD和Mac OS系统上，TCP_NOPUSH选项相当于TCP_CORK。

Nginx可以在http, server和location模块中定义tcp_nopush。

http {
 tcp_nopush on;
 }
Nginx默认启用了tcp_nopush

上面的两个directives负责不同的任务。tcp_nodelay可以降低网络延迟，而tcp_nopush可以优化发送的数据包。

同时启用tcp_nopush和sendfile可以确保在传输文件时，内核总是生成最大量的完整TCP数据包以供传输，而最后一个TCP数据包可以是不完整的，这个不完整的数据包