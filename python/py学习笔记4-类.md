
# 类
```

class person:
    # _init_ 是初始化函数，创建 person 时会自动调用
    # self 是每个方法的第一个参数，等同于 this，会自动传入
    # 所有的属性需要在 _init_ 中设置,如果只是声明而没有初始化的属性则不能访问
    def __init__(self,name,age):
        self.name = name
        self.age =age
        self.test = 0

    def eat(self,food):
        print('i want to eat ',food)

    def getTest(self):
        self.test = 1
        print(self.test)
        
    def other(self):
		# 这里会报错，只有 init 中可以声明属性
		self.oterVar = 'otherVar'
	
	def otherPrint(self):
		print(self.otherVar)

	p = person('kanggege',21)
	print(p.getTest())
	
	class student(person):
	    def __init__(self,name,age,id):
	        super().__init__(name,age)
	        self.id = id
	
	    def eat(self,food):
	        print('i am a student, i am very hungry, i eat ',food)
	
	    def eat(self):
	        print('i baole')
	
	s = student('kangge',21,666)
	s.eat()

```