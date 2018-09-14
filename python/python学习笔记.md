## 字符串
- 使用 “” 或 '' 创建字符串
- 使用 + 可以拼接字符串，但是字符串和非字符串之间不能拼接，需要使用 str(val) 进行类型转换
- 可以在字符串中使用转义字符。会被解析
- input('alert提示') 用于获取用户输入，获取的是字符串、int() 可以将字符串转为 int 类型

## 列表
- 创建列表 arr = ["a",2,6.66] 可以是多数据类型混合，类数组操作，引用类型
- 增删改查，都是对原数组进行的操作
    - arr.apend(val) 在数组末尾追加数据
    - arr.insert(index,value) 在 index 处插入 val，被插入元素后的元素都会向右移动，如果 index 大于数组长度，会将 val 添加在末尾
    - del arr[index] 删除元素，其后元素向左移动
    - arr.pop() 弹出顶元素，并删除
    - arr.remove(val) 删除 val 元素，而后元素会左移，如果 val 不存在，会报错
    - v in arr / v not in arr 可以查询 v 是否在 arr 数组中，返回 True/False
- 排序
    - arr.sort() 在原数组上排序， arr.sort(reverse=True) 反向排序，只有数组里都是同类型元素才能进行排序
    - sorted(arr) 返回排序后的数组，并不改变数组本身
- 其他
    - arr.reverse() 翻转元素
    - len(arr) 获取长度
    - 如果数组长度不为 0 ，都可以通过 arr[-1] 访问最后一个元素
    - arr.fun() 对数组本身进行操作，fun(arr) 并不改变数组本身
- 遍历
    - for v in arr:
    - list(range())
    - 列表解析 arr = [value**2 for value in range(1,11)]、**2 是求平方
- 数字数组处理
    - min(arr)、max(arr)、sum(arr) 如果数组里有非数字元素，则会出错
- 切片
    - arr[:] py的切片不是引用类型的，而是进行了复制
- 元组
    - ct = (1,2) 常量数组，虽然不能修改其中的元素，但是可以重新给元祖赋值，真扯淡

## 逻辑语句
- if c1: elif c2: else:
- True / False
- and / or

## 字典
- 键值对而已 map = {"":"",} 引用类型
- del map[index] 删除字典元素
- for i,v in map.items(): 遍历字典键值对，for i in map.keys() 便利字典的键，for v in map.values()遍历字典的值，
- for i in sorted(map.keys())遍历排序后的，注意只有同一种类型才能排序，for v in set(map.values()) 正如你想的 set 那样，独一无二

## 类
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