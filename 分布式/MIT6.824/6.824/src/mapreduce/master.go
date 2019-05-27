package mapreduce

import (
	"fmt"
	"net"
	"sync"
)

// Master holds all the state that the master needs to keep track of. Of
// particular importance is registerChannel, the channel that notifies the
// master of workers that have gone idle and are in need of new work.
// master记录M和R的状态，并通过registerChannel判断M是否完成任务
type Master struct {
	sync.Mutex

	address         string
	registerChannel chan string // 通知master那些worker处于空闲状态。
	doneChannel     chan bool	// 阻塞等待master调用完成
	workers         []string // protected by the mutex, master下面含有的worker的名字

	// Per-task information
	jobName string   // Name of currently executing job 集群中可能同时处理很多任务，所以jobname是很重要的区分
	files   []string // Input files
	nReduce int      // Number of reduce partitions

	shutdown chan struct{}	//终止程序
	l        net.Listener
	stats    []int
}

// Register is an RPC method that is called by workers after they have started
// up to report that they are ready to receive tasks.
// 一个供worker调用的rpc方法, 告诉master它们已经准备好接受任务。
func (mr *Master) Register(args *RegisterArgs, _ *struct{}) error {
	mr.Lock()
	defer mr.Unlock()
	debug("Register: worker %s\n", args.Worker)
	mr.workers = append(mr.workers, args.Worker)
	go func() {
		mr.registerChannel <- args.Worker // 通知master那些worker处于空闲状态。
	}()
	return nil
}

// newMaster initializes a new Map/Reduce Master
// 创建初始化master
func newMaster(master string) (mr *Master) {
	mr = new(Master)
	mr.address = master
	mr.shutdown = make(chan struct{})
	mr.registerChannel = make(chan string)
	mr.doneChannel = make(chan bool)
	return
}

// Sequential runs map and reduce tasks sequentially, waiting for each task to
// complete before scheduling the next.
// Sequential方法顺序的执行map和reduce任务,在分配下一个任务前需要前面的任务完成。
// 任务名，输入文件，R的数量，M的功能函数，R的功能函数
func Sequential(
	jobName string, files []string, nreduce int,
	mapF func(string, string) []KeyValue,
	reduceF func(string, []string) string,
) (mr *Master) {
	mr = newMaster("master")

	//顺序执行的调度器
	schedule := func(phase jobPhase) {
		switch phase {
		case mapPhase:
			for i, f := range mr.files {
				doMap(mr.jobName, i, f, mr.nReduce, mapF)
			}
		case reducePhase:
			for i := 0; i < mr.nReduce; i++ {
				doReduce(mr.jobName, i, len(mr.files), reduceF)
			}
		}
	}

	finish := func() {
		mr.stats = []int{len(files) + nreduce}
	}
	go mr.run(jobName, files, nreduce, schedule, finish)
	return
}

// Distributed schedules map and reduce tasks on workers that register with the master over RPC.
// 将map和reduc任务分布到通过rpc注册到master的worker。
func Distributed(jobName string, files []string, nreduce int, master string) (mr *Master) {
	mr = newMaster(master)
	mr.startRPCServer()
	go mr.run(jobName, files, nreduce, mr.schedule, func() {
		mr.stats = mr.killWorkers()
		mr.stopRPCServer()
	})
	return
}

// run executes a mapreduce job on the given number of mappers and reducers.
//
// First, it divides up the input file among the given number of mappers, and
// schedules each task on workers as they become available. Each map task bins
// its output in a number of bins equal to the given number of reduce tasks.
// Once all the mappers have finished, workers are assigned reduce tasks.
//
// When all tasks have been completed, the reducer outputs are merged,
// statistics are collected, and the master is shut down.
//
// Note that this implementation assumes a shared file system.

// 用给定的M执行任务，M会将中间值分为R的数量份(可能这样更好的对R做负载均衡)，M完成之后给R分配任务
// 等M和R都执行完成后，使用merge整理最终结果，然后maste shutdown
// 注意：实现假设在一个共享的文件系统之上。
func (mr *Master) run(
	jobName string, files []string, nreduce int,
	schedule func(phase jobPhase),
	finish func(),
) {
	mr.jobName = jobName // job的名字
	mr.files = files     // 输入的文件
	mr.nReduce = nreduce // reduce任务的数量限制

	fmt.Printf("%s: Starting Map/Reduce task %s\n", mr.address, mr.jobName)

	// 这两个函数都需要外面传入
	schedule(mapPhase)    // 安排map任务  schedule即master.go 64行传入的函数
	schedule(reducePhase) // 安排reduce任务
	finish()              // 任务完成

	mr.merge() // 合并结果

	fmt.Printf("%s: Map/Reduce task completed\n", mr.address)

	mr.doneChannel <- true
}

// Wait blocks until the currently scheduled work has completed.
// This happens when all tasks have scheduled and completed, the final output
// have been computed, and all workers have been shut down.
func (mr *Master) Wait() {
	<-mr.doneChannel // 等待run运行完成
}

// killWorkers cleans up all workers by sending each one a Shutdown RPC.
// It also collects and returns the number of tasks each worker has performed.
func (mr *Master) killWorkers() []int {
	mr.Lock()
	defer mr.Unlock()
	ntasks := make([]int, 0, len(mr.workers))
	for _, w := range mr.workers {
		debug("Master: shutdown worker %s\n", w)
		var reply ShutdownReply
		ok := call(w, "Worker.Shutdown", new(struct{}), &reply)
		if ok == false {
			fmt.Printf("Master: RPC %s shutdown error\n", w)
		} else {
			ntasks = append(ntasks, reply.Ntasks)
		}
	}
	return ntasks
}
