```
//config/config.go
type Source struct {
}

func NewSource() *Source {
	return &Source{}
}


func (p *Source) Load(cpath string) (content string, err error) {
	en := p.extendName(cpath)
	parser, err := parserFactory(en)
	if err != nil {
		return
	}
	content = parser.Content(cpath)
	return
}

func (p *Source) extendName(cpath string) string {
	splitedWords := strings.Split(cpath, ".")
	l := len(splitedWords)
	if l == 0 {
		return ""
	}
	return splitedWords[l-1]
}


//config/parser.go
type parser interface {
	Content(fpath string) string
}

var parserMap = map[string]parser {
	"json" : jsonParser{},
}

func parserFactory(extendName string) (p parser, err error) {
	p, existed := parserMap[extendName]
	if existed {
		return p, nil
	}
	return nil, fmt.Errorf("parser %s don't exist", extendName)
}

type jsonParser struct {
}

func (j jsonParser) Content(fpath string) string {
	panic("implement me")
}
```

工厂模式有几个需要注意的地方

1. factory返回接口而非实现
2. 通过map管理注册的接口实现