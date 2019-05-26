package mapreduce

import (
	"encoding/json"
	"os"
)

// doReduce does the job of a reduce worker: it reads the intermediate
// key/value pairs (produced by the map phase) for this task, sorts the
// intermediate key/value pairs by key, calls the user-defined reduce function
// (reduceF) for each key, and writes the output to disk.
func doReduce(
	jobName string, // the name of the whole MapReduce job
	reduceTaskNumber int, // which reduce task this is
	nMap int, // the number of map tasks that were run ("M" in the paper)
	reduceF func(key string, values []string) string,
) {
	// TODO:
	// You will need to write this function.
	// You can find the intermediate file for this reduce task from map task number
	// m using reduceName(jobName, m, reduceTaskNumber).
	// Remember that you've encoded the values in the intermediate files, so you
	// will need to decode them. If you chose to use JSON, you can read out
	// multiple decoded values by creating a decoder, and then repeatedly calling
	// .Decode() on it until Decode() returns an error.
	//
	// You should write the reduced output in as JSON encoded KeyValue
	// objects to a file named mergeName(jobName, reduceTaskNumber). We require
	// you to use JSON here because that is what the merger than combines the
	// output from all the reduce tasks expects. There is nothing "special" about
	// JSON -- it is just the marshalling format we chose to use. It will look
	// something like this:
	//
	// enc := json.NewEncoder(mergeFile)
	// for key in ... {
	// 	enc.Encode(KeyValue{key, reduceF(...)})
	// }
	// file.Close()

	// 你需要完成这个函数。你可与获取到来自map任务生产的中间数据，通过reduceName获取到文件名。
	//  记住你应该编码了值到中间文件,所以你需要解码它们。如果你选择了使用JSON,你通过创建decoder读取到多个
	// 解码之后的值，直接调用Decode直到返回错误。
	//
	// 你应该将reduce输出以JSON编码的方式保存到文件，文件名通过mergeName获取。我们建议你在这里使用JSON,

	// key是中间文件里面键值，value是字符串,这个map用于存储相同键值元素的合并

	keymap := make(map[string]*[]string)

	//读取所有M产生的中间值中属于当前R的那一份
	data := make([][]KeyValue,nMap)
	for i := 0; i < nMap; i++{
		filename := reduceName(jobName,i,reduceTaskNumber)
		file, err := os.Open(filename)
		errPanic(err)

		decoder := json.NewDecoder(file)
		kvs := make([]KeyValue,0)
		errPanic(decoder.Decode(&kvs))
		data = append(data,kvs)
		errPanic(file.Close())
	}

	//整合key-val
	for _, kvs := range data {
		for _, kv := range kvs{
			if arr,ok := keymap[kv.Key]; ok{
				temp := append(*arr,kv.Value)
				arr = &temp
			}else{
				arr := make([]string,0)
				arr = append(arr,kv.Value)
				keymap[kv.Key] = &arr
			}
		}
	}

	mergeName := mergeName(jobName, reduceTaskNumber)
	mergeFile, err := os.Create(mergeName)
	errPanic(err)
	defer mergeFile.Close()
	mergeEncoder := json.NewEncoder(mergeFile)

	for key,val := range keymap{
		err := mergeEncoder.Encode(KeyValue{key, reduceF(key, *val)})
		errPanic(err)
	}
}
