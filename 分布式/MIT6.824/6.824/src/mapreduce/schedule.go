package mapreduce

import (
	"fmt"
)

// schedule starts and waits for all tasks in the given phase (Map or Reduce).
func (mr *Master) schedule(phase jobPhase) {
	var ntasks int
	var nios int // number of inputs (for reduce) or outputs (for map)

	switch phase {
	case mapPhase:
		ntasks = len(mr.files)
		nios = mr.nReduce
		fmt.Println("start map tasks")

	case reducePhase:
		ntasks = mr.nReduce
		nios = len(mr.files)

		fmt.Println("start reduce tasks")
	}
	fmt.Printf("Schedule: %v %v tasks (%d I/Os)\n", ntasks, phase, nios)

	// All ntasks tasks have to be scheduled on workers, and only once all of
	// them have been completed successfully should the function return.
	// Remember that workers may fail, and that any given worker may finish
	// multiple tasks.
	//
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
	//

	//把要处理的任务放在channel中
	taskch, taskfinch,taskfin := make(chan int,ntasks),make(chan int),make([]int,ntasks)
	for i := 0; i < ntasks; i++{
		taskch <- i
	}

	//将已经注册过的work重新放入mr.registerChannel中
	//在reduce过程，或渐进增加work有效
	mr.Lock()
	exitRegister := make([]string,len(mr.workers))
	copy(exitRegister,mr.workers)
	mr.Unlock()

	go func() {
		for _, work := range exitRegister{
			mr.registerChannel <- work
		}
	}()

	for {
		select {
		case work := <- mr.registerChannel:
			//对每个刚注册的work分一个线程，让他们循环完成任务
			go func(work string) {
				for itask := range taskch{
					args := &DoTaskArgs{
						JobName:mr.jobName,
						Phase:phase,
						TaskNumber:itask,
						NumOtherPhase:nios,
					}
					if phase == mapPhase {
						args.File = mr.files[itask]
					}
					ok := call(work, "Worker.DoTask", args, nil)
					//成功则完成任务，不成功将任务放回未完成的队列
					if ok {
						taskfinch <- itask
					}else{
						taskch <- itask
					}
				}
			}(work)
		case itask := <- taskfinch:
			taskfin[itask]++
			flag := true
			for _,v := range taskfin{
				if v == 0{
					flag = false
					break
				}
				//要是超过1了，说明有的任务被完成了多次！！！
				if v > 1{
					panic(itask)
				}
			}
			if flag {
				close(taskch)
				close(taskfinch)
				goto FINAL
			}
		}
	}
	FINAL:
	fmt.Printf("Schedule: %v phase done\n", phase)
}
