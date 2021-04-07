
# 内存缓存 store.go
client-go/tools/cache/store.go
```
type Store interface {
	Add(obj interface{}) error
	Update(obj interface{}) error
	Delete(obj interface{}) error
	List() []interface{}
	ListKeys() []string
	Get(obj interface{}) (item interface{}, exists bool, err error)
	GetByKey(key string) (item interface{}, exists bool, err error)
	Replace([]interface{}, string) error
	Resync() error
}
func NewStore(keyFunc KeyFunc) Store {...}

// KeyFunc knows how to make a key from an object. Implementations should be deterministic.
type KeyFunc func(obj interface{}) (string, error)

类似与key-val数据库，此处的key可以从val中解析出
```

- MetaNamespaceKeyFunc(obj interface{}) (string, error) 从api的object meta中获取namespace和name组成key
- SplitMetaNamespaceKey(key string) (namespace, name string, err error)  从上面函数key再解析出namespace和name