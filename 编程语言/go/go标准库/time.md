
	tick := time.Tick(time.Second * 1)
	for range tick {
		fmt.Println("nb")
	}

	time.AfterFunc(time.Second*1, func() {
		fmt.Println("call back")
	})

	after := time.After(time.Second*5)
	<- after