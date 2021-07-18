mongo4.0提供副本集多文档事务，4.2提供分片集多文档事务

1.  多文档事务，必须建立在复制集的基础上，仅在wiredTiger 数据引擎支持
2.  不可操作collection。进行操作时，collection必须已经存在，对collection的创建和删除操作，不能再事务中出现
3.  不可操作索引。对INDEX的创建，删除操作不可以在事务中出现
4.  仅仅支持CRUD 操作，其他操作不能再事务中出现
5.  仅仅对用户级别的collection 可以操作，系统级别的collection 或db 是不能操作的
6.  对事务的大小的限制在 16MB，对事务的操作整体不允许超过60秒
7. 虽然是事务，但也要尽快的操作完成，否则WireTiger中使用快照来操作维护事务，会造成大量的内存的使用不被释放。
8. 事务不能再会话外运行，一个session 只能一次运行一个事务，但可以运行多个session 并行运行事务
9. 针对冲突的情况，WiredTiger会自动重试，超时失败

Multiple granularity locking (我把它翻译为多粒度锁机制): MongoDB采用的是所谓的MGL多粒度锁机制，具体可以参考文末的wiki链接。简单来说就是结合了多种不同粒度的锁，包括S锁（Shared lock），X锁（Exclusive lock), IS锁(Intent Share lock), IX(Intent Exclusive lock)，这几种锁的互斥关系如下表所示：

当我们设置Read Concern为majority时，可以保证返回的数据是大多数节点所持有的数据，这种情况是不会发生回滚的，也就避免了脏读。默认是local读级别，未提交读，则可能脏读

还有一种情况可能出现脏读，就是当writer写数据时，虽然已经写入到了内存当中，但是并没有写入到磁盘中，这时reader读取到了更新后的数据，但当Mongodb将内存中的数据写入磁盘时可能会产生错误，从而导致磁盘写入失败，这时就可能导致该数据丢失，这种情况下也会产生脏读，而为了避免这种情况，我们需要在Write Concern设置的时候使用j:1，这样实际是在写入journal之后才返回写入成功，保证不会出现上述的脏读现象
