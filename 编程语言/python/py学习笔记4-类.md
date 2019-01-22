
# 类
```

	class person:
		#　在__init__外声明的属性是静态属性，类和实例都可以访问
		jio = 'person'

		# _init_ 是初始化函数，创建 person 时会自动调用
		# self 是每个方法的第一个参数，等同于 this，会自动传入
		# 所有的属性必须在 __init__ 中声明并初始化,如果只是声明而没有初始化的属性则不能访问
		# 如果没有在 __init__ 中声明并初始化的，在其他方法里赋值了也没用，访问了会报错
		def __init__(self,name,age):
			self.name = name
			self.age =age
			self.test = 0
			#假装出一个私有属性，其实。。。
			self.__private = ""
		
		＃ 普通的方法
		def eat(self,food):
			print('i want to eat ',food)

		# 静态方法，不能访问 self 原因同　java
		@staticmethod
		def get_jio():
			return jio

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
## 封装
在类中， \_\_xx\_\_ 是特殊变量，可以访问的，而 __xx 是我们自己定义的私有变量，直接访问不到（目的就不让别人访问，其实也可以通过 _class__name 访问，但是不同的 py 解释器效果不同，一般不这样访问）

同样，以 __fun 开头的函数也是不可访问的

### 属性装饰器
一般为了封装某个属性，都会将其变为私有，并加上　getter, setter 方法做访问控制，但是　py 的私有属性其实是也可以访问的，可以使用属性装饰器弥补（我觉得让事情更糟糕了）
```
	class person:
		# 等同于创建了一个属性　name，并声明它的　getter　方法
		@property
		def name(self):
			return self._name

		# name　的　setter　方法
		@name.setter
		def name(self,val):
			self._name = val


	p = person()
	# 等同于调用它的　setter 方法
	p.name = 'kanggege'　
	# 如果不调用　setter 直接调用 getter 会报错
	print(p.name)
```

## 继承
py是多继承，class cla(father,father2): 继承父类所有的方法和变量，如果直接写 class name: 默认继承了 Object == class name(Object):，多个父类的方法冲突了，则默认继承第一个父类

子类继承父类所有的 public 属性和方法,__xxx 无法访问，子类可以重写父类的方法或属性

##　多态
isinstance(var,type) 检测　var 是否是　type 类型的，子类可以看做父类的类型，type(val)　可以获取值的类型

理论上说，多态一定是传入子类或者实现了接口的类，但是在　py 这种动态语言中，满足‘鸭子类型’即可，一个对象只要“看起来像鸭子，走起路来像鸭子”，那它就可以被看做是鸭子

```
	class animate:
		def eat(self):
			print('eat')
		
		def run(self):
			print('runnnnnn')

	class cat:
		def run(self):
			print('cat runnnnnnn')

	def t(animate):
		animate.run()

	t(cat())
```

## 其他操作
dir(obj) 可以获取对象的所有属性和方法，配合 getattr(obj,attr)、setattr(obj,attr,val) 以及 hasattr(obj,attr)，我们可以直接操作一个对象的状态