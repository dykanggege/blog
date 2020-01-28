```
struct User {
    username: String,
    email: String,
}

fn build_user() -> User {
    //必须填充满，不提供默认值，这样很怪
    User{
        username:"kanggege".to_string(),
        email:"@qq.com".to_string()
    }
}


//真心觉得rust语法太丑了
let u1 = build_user();
let u2 = User{
    username:"other".to_string(),
    ..u1
};
```

