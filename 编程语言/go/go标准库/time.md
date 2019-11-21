时间本来是一件很单纯很简单的事情，但纷闹的世界让他复杂了起来

```
	now := time.Now()    // 获取系统时间

	localTime := now.Local() // 返回时间转化为系统时区对应的时间值

	now.MarshalJSON() // 默认使用 RFC 3339 格式作为json编码标准，可以自建类型覆盖该方法

	now.Format("2006-01-02 15:04:05") //YYYY-MM-DD HH-MM-SS 类型必须按照这个时间格式化

	newDate := time.Date(now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second(), now.Nanosecond(), now.Location())

	now.Unix() // 返回时间戳

	now.UTC()

```


	tick := time.Tick(time.Second * 1)
	for range tick {
		fmt.Println("nb")
	}

	time.AfterFunc(time.Second*1, func() {
		fmt.Println("call back")
	})

	after := time.After(time.Second*5)
	<- after