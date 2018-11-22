gin　的风格更接近　go　的标准库，在 api 粒度上也比　beego　细，对 restful　风格支持更好，很有 go　风格的一个框架

# Using GET, POST, PUT, PATCH, DELETE and OPTIONS
```
    func main() {
        //禁用控制台输出
        // gin.DisableConsoleColor()

        //创建一个 gin 的 router 
        router := gin.Default()
    
        router.GET("/someGet", getting)
        router.POST("/somePost", posting)
        router.PUT("/somePut", putting)
        router.DELETE("/someDelete", deleting)
        router.PATCH("/somePatch", patching)
        router.HEAD("/someHead", head)
        router.OPTIONS("/someOptions", options)

        //默认跑在 8080 端口
        router.Run()
        // router.Run(":3000")
    }
```

# Parameters in path

```
    func main() {
        router := gin.Default()

        // This handler will match /user/john but will not match /user/ or /user
        router.GET("/user/:name", func(c *gin.Context) {
            name := c.Param("name")
            c.String(http.StatusOK, "Hello %s", name)
        })

        // However, this one will match /user/john/ and also /user/john/send
        // If no other routers match /user/john, it will redirect to /user/john/
        //　它匹配到的是完整的路径，如　/user/john/en -> action = /en
        router.GET("/user/:name/*action", func(c *gin.Context) {
            //c.Param　只能获取到　Parameters in path
            name := c.Param("name")
            action := c.Param("action")
            message := name + " is " + action
            c.String(http.StatusOK, message)
        })

        router.Run(":8080")
    }
```

# Querystring parameters

```

    func main() {
        router := gin.Default()

        // Query string parameters are parsed using the existing underlying request object.
        // The request responds to a url matching:  /welcome?firstname=Jane&lastname=Doe
        router.GET("/welcome", func(c *gin.Context) {
            // c.Query 只能获取到 url 参数，无论是自己拼接，还是使用 get/delete 请求后浏览器拼接
            firstname := c.DefaultQuery("firstname", "Guest")
            lastname := c.Query("lastname") // shortcut for c.Request.URL.Query().Get("lastname")

            c.String(http.StatusOK, "Hello %s %s", firstname, lastname)
        })
        router.Run(":8080")
    }

```

# Multipart/Urlencoded Form
```
    func main() {
        router := gin.Default()

        router.POST("/form_post", func(c *gin.Context) {
            // httpBody　的 Content-Type 有多种，application/json，application/x-www-form-urlencoded, application/xml　和　multipart/form-data
            //　c.PostForm 只能获取 x-www-form-urlencoded 或 from-data　中的参数
            message := c.PostForm("message")
            nick := c.DefaultPostForm("nick", "anonymous")

            c.JSON(200, gin.H{
                "status":  "posted",
                "message": message,
                "nick":    nick,
            })
        })
        router.Run(":8080")
    }
```

# query + post form
```
    POST /post?id=1234&page=1 HTTP/1.1
    Content-Type: application/x-www-form-urlencoded

    name=manu&message=this_is_great

    func main() {
        router := gin.Default()

        router.POST("/post", func(c *gin.Context) {
            //　各自获取各自的数据
            id := c.Query("id")
            page := c.DefaultQuery("page", "0")
            name := c.PostForm("name")
            message := c.PostForm("message")

            fmt.Printf("id: %s; page: %s; name: %s; message: %s", id, page, name, message)
        })
        router.Run(":8080")
    }

```

# Map as querystring or postform parameters
```

    POST /post?ids[a]=1234&ids[b]=hello HTTP/1.1
    Content-Type: application/x-www-form-urlencoded

    names[first]=thinkerou&names[second]=tianou
    func main() {
        router := gin.Default()

        router.POST("/post", func(c *gin.Context) {

            ids := c.QueryMap("ids")
            names := c.PostFormMap("names")

            fmt.Printf("ids: %v; names: %v", ids, names)
        })
        router.Run(":8080")
    }

    ids: map[b:hello a:1234], names: map[second:tianou first:thinkerou]
```

# Upload files
单文件
```
    func main(){
        router := gin.Default()
        
        router.POST("/upload", func(c *gin.Context) {
            name := c.PostForm("name")
            fmt.Println(name)
            file, header, err := c.Request.FormFile("upload")
            if err != nil {
                c.String(http.StatusBadRequest, "Bad request")
                return
            }
            //如果　filaname　是文件要存储的路径，必须是绝对路径
            filename := header.Filename

            fmt.Println(file, err, filename)

            out, err := os.Create(filename)
            if err != nil {
                log.Fatal(err)
            }
            defer out.Close()
            _, err = io.Copy(out, file)
            if err != nil {
                log.Fatal(err)
            }
            c.String(http.StatusCreated, "upload successful")
        })
        router.Run(":8000")
    }
```
多文件
```
    func main() {
        router := gin.Default()
        // Set a lower memory limit for multipart forms (default is 32 MiB)
        // router.MaxMultipartMemory = 8 << 20  // 8 MiB
        router.POST("/upload", func(c *gin.Context) {
            // Multipart form
            form, _ := c.MultipartForm()
            files := form.File["upload[]"]

            for _, file := range files {
                log.Println(file.Filename)

                // Upload the file to specific dst.
                // c.SaveUploadedFile(file, dst)
            }
            c.String(http.StatusOK, fmt.Sprintf("%d files uploaded!", len(files)))
        })
        router.Run(":8080")
    }
```

# 参数绑定


# 分组路由
```
    Grouping routes
    func main() {
        router := gin.Default()

        // Simple group: v1
        v1 := router.Group("/v1")
        {
            v1.POST("/login", loginEndpoint)
            v1.POST("/submit", submitEndpoint)
            v1.POST("/read", readEndpoint)
        }

        // Simple group: v2
        v2 := router.Group("/v2")
        {
            v2.POST("/login", loginEndpoint)
            v2.POST("/submit", submitEndpoint)
            v2.POST("/read", readEndpoint)
        }

        router.Run(":8080")
    }

```

# 日志
```
    func main() {
        // Disable Console Color, you don't need console color when writing the logs to file.
        gin.DisableConsoleColor()

        // Logging to a file.
        f, _ := os.Create("gin.log")
        gin.DefaultWriter = io.MultiWriter(f)

        // Use the following code if you need to write the logs to file and console at the same time.
        // gin.DefaultWriter = io.MultiWriter(f, os.Stdout)

        router := gin.Default()
        router.GET("/ping", func(c *gin.Context) {
            c.String(200, "pong")
        })

        router.Run(":8080")
    }
```