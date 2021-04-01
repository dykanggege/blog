In short, a proxy is a wrapper or agent object that is being called by the client to access the real serving object behind the scenes. Use of the proxy can simply be forwarding to the real object, or can provide additional logic. In the proxy, extra functionality can be provided, for example caching when operations on the real object are resource intensive, or checking preconditions before operations on the real object are invoked. For the client, usage of a proxy object is similar to using the real object, because both implement the same interface.

```

type UserServicer interface {
	Login(name, pwd string) bool
}

type UserService struct {
}

func (u *UserService) Login(name, pwd string) (succ bool) {
	realPwd := u.findPwdByName(name)
	succ = u.equalPwd(pwd, realPwd)
	u.updateLoginTime(name)

	return
}

func (u *UserService) findPwdByName(name string) string {
	return ""
}

func (u *UserService) equalPwd(pwd, realPwd string) bool {
	return true
}

func (u *UserService) updateLoginTime(name string) {
}


type Counterable interface {
	Add(key string)
	Sub(key string)
}


type Counter struct {
	record map[string]int
}

func NewCounter() *Counter {
	return &Counter{record: make(map[string]int)}
}

func (c Counter) Add(key string) {
	c.record[key]++
}

func (c Counter) Sub(key string) {
	c.record[key]--
}


type LoginService struct {
	us UserServicer
	ca Counterable
}

func NewLoginService(us UserServicer, ca Counterable) *LoginService {
	return &LoginService{us: us, ca: ca}
}

func (l *LoginService) Login(name, pwd string) error {
	loginSuccess := l.us.Login(name, pwd)
	if loginSuccess {
		l.ca.Add(name)
		return nil
	}
	return fmt.Errorf("登陆失败")
}

```

上面是个简单的登陆验证逻辑，如果验证之后修改登陆频率等非login操作，放在login函数中违反了单一职责，可以在基础上新加函数，封装多个逻辑

proxy更像设计理念

- 每个类（方法）只做一件事，不添加任何包含副作用功能，在原有方法上集成其他功能
- 每个函数保持一个抽象层次

