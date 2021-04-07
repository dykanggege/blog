client-go用于和api-server通讯，kubectl就是基于client-go封装的命令

# 创建client
将以operator和api-server建立连接的过程分析整个源码包

```
// 将kubeconfig转化为认证配置文件，即/tools/clientcmd/api/types/Config结构
kubeconfigCf, err := clientcmd.Load([]byte{})
So(err, ShouldBeNil)

// 用kubeconfig文件填充clientConfig
// NewDefaultClientConfig creates a DirectClientConfig using the config.CurrentContext as the context name
restCf, err := clientcmd.NewDefaultClientConfig(
    *kubeconfigCf,
    &clientcmd.ConfigOverrides{
        ClusterDefaults: clientcmdapi.Cluster{Server: ""},
    },
).ClientConfig()

kubeclient, err := kubernetes.NewForConfig(restCf)
```

## NewDefaultClientConfig
```
// client-go/kubernetes/clientset.go

// NewDefaultClientConfig creates a DirectClientConfig using the config.CurrentContext as the context name
func NewDefaultClientConfig(config clientcmdapi.Config, overrides *ConfigOverrides) ClientConfig {
	return &DirectClientConfig{config, config.CurrentContext, overrides, nil, NewDefaultClientConfigLoadingRules(), promptedCredentials{}}
}

// DirectClientConfig is a ClientConfig interface that is backed by a clientcmdapi.Config, options overrides, and an optional fallbackReader for auth information
type DirectClientConfig struct {
	config         clientcmdapi.Config // 即kubeconfig信息
	contextName    string              // 使用的kubeconfig哪个context
	overrides      *ConfigOverrides    // 
	fallbackReader io.Reader
	configAccess   ConfigAccess        // kubeconfig默认存放位置和加载方式
	// promptedCredentials store the credentials input by the user
	promptedCredentials promptedCredentials // 用户提供的凭证
}

// NewDefaultClientConfigLoadingRules returns a ClientConfigLoadingRules object with default fields filled in.  You are not required to
// use this constructor
// 返回kubeconfig默认存放的地址和解析规则
func NewDefaultClientConfigLoadingRules() *ClientConfigLoadingRules {
	chain := []string{}

	envVarFiles := os.Getenv(RecommendedConfigPathEnvVar)
	if len(envVarFiles) != 0 {
		fileList := filepath.SplitList(envVarFiles)
		// prevent the same path load multiple times
		chain = append(chain, deduplicate(fileList)...)

	} else {
		chain = append(chain, RecommendedHomeFile)
	}

	return &ClientConfigLoadingRules{
		Precedence:     chain,
		MigrationRules: currentMigrationRules(),
	}
}

ClientConfig() // 校验配置填充的值格式是否正确，并转化为restclient.Config
```

## kubernetes.NewForConfig
```
// NewForConfig creates a new Clientset for the given config.
func NewForConfig(c *rest.Config) (*Clientset, error) {
	configShallowCopy := *c
	if configShallowCopy.RateLimiter == nil && configShallowCopy.QPS > 0 {
		configShallowCopy.RateLimiter = flowcontrol.NewTokenBucketRateLimiter(configShallowCopy.QPS, configShallowCopy.Burst)
	}
	var cs Clientset
	var err error
	cs.admissionregistrationV1beta1, err = admissionregistrationv1beta1.NewForConfig(&configShallowCopy)
	if err != nil {
		return nil, err
	}
	...
	return &cs, nil
}

```


# tools
提供查询和缓存机制，减少向api-server发起请求

## clientcmd
在kubernetes基础上封装更方便使用的创建client方法

kubeconfig是常用的认证文件

- /clientcmd/api/types.go 声明了kubeconfig结构
- /clientcmd/loader 加载kubeconfig文件
    - Load(data []byte) (*clientcmdapi.Config, error) 将kubeconfig文件转为 /clientcmd/api/types/Config结构
    - LoadFromFile(filename string) (*clientcmdapi.Config, error) 

与api-server通讯的client

- /clientcmd/client_config.go // ClientConfig is used to make it easy to get an api server client
    - ClientConfig 配置结构接口
    - DirectClientConfig 从kubeconfig或者其他认证文件生成为kube-client的中转配置


# util 
工具集
- util/flowcontrol 实现了两个qps速率控制器

# kubernetes

- typed 所有内置api的go-client
- clientset.go 生成包含所有内置api go-client的client，即clientset

# rest
用于通讯的rest客户端