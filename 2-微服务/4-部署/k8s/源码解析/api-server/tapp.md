# controller逻辑分析
都是通过 enqueueTApp 加入队列，监听函数(AddFunc)传递的参数为 object key，一般是 namespace/name 的形式

New函数中
```
tappInformer.Informer().AddEventHandler(cache.ResourceEventHandlerFuncs{
    AddFunc: controller.enqueueTApp,
    UpdateFunc: func(old, cur interface{}) {
        oldTApp := old.(*tappv1.TApp)
        curTApp := cur.(*tappv1.TApp)
        if oldTApp.Status.Replicas != curTApp.Status.Replicas {
            klog.V(4).Infof("Observed updated replica count for tapp %s: %d->%d",
                util.GetTAppFullName(curTApp), oldTApp.Status.Replicas, curTApp.Status.Replicas)
        }
        controller.enqueueTApp(cur)
    },
    DeleteFunc: controller.enqueueTApp,
})
```

清除信息，传递给queue namespace/name 值
```
func (c *Controller) enqueueTApp(obj interface{}) {
	key, err := cache.DeletionHandlingMetaNamespaceKeyFunc(obj)
	if err != nil {
		klog.Errorf("Could not get key for object %+v: %v", obj, err)
		return
	}
	c.workqueue.Add(key)
}
```

New函数创建queue，监听事件，将key放入queue，Run函数中会创建多个goroutinue从queue取出事件处理

Run
```
for i := 0; i < threadiness; i++ {
    go wait.Until(c.runWorker, time.Second, stopCh)
}
```

runWorker也只是从queue中取出消息处理

runWorker
```
for {
    func() {
        key, quit := c.workqueue.Get()
        if quit {
            return
        }
        defer c.workqueue.Done(key)

        //TODO forget 又是干嘛
        if err := c.syncHandler(key.(string)); err != nil {
            klog.Errorf("Error syncing TApp %v, re-queuing: %v", key.(string), err)
            c.workqueue.AddRateLimited(key)
        } else {
            // forget 标记处理过了
            c.workqueue.Forget(key)
        }
    }()
}
```

Sync 真正处理函数

前面经过了，设置监听事件，处理监听获得的key，将事件放入队列，从队列中获取key，处理事件，queue done

处理事件又包含，确定缓存同步完成，获取tapp

上面这些步骤除了处理事件，即kubebuilder中的Reconcile，其他步骤基本固定，可以封装隐藏
