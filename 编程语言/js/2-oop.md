js没有采用通用意义上的类继承，而是采用原型链，ES6引入了class语法糖，一般使用class

# 原型链
js中类的概念基于函数实现

```
//正常调用就是一个函数，用new调用就是类
function Myclass() {
    this.a = 1
    this.b = function() {}
}

let obj = new Myclass() //创建一个对象
```

只是一个函数，当使用 new 调用这个函数，会返回一个对象，函数中的this都指向返回的对象

Myclass除了作为函数拥有的属性，还有一些用于类继承的属性

- prototype：是一个对象，即原型链
- \_proto\_：ES中未规定，浏览器提供的属性，指向他的父类

先理解下怎样实现类继承，再考虑上面两个属性的意义

```
function SuperType() {
    this.a = 1
}
SuperType.prototype.a = 1
SuperType.prototype.b = 2

var o = new SuperType()
o.a //1
o.b //2
```

可以看出基于SuperType类创建的对象查找值的顺序 this => prototype

可以利用这个特点实现继承
```

function SubType() {
    this.b = 'sub b'
}

// 这里是关键，创建SuperType的实例，并将该实例赋值给SubType.prototype
SubType.prototype = new SuperType()

var o = new SubType()
o.a //1
o.b //"sub b"
```

o.a查找顺序 SubType函数内this(未找到) => SubType.prototype

而SubType.prototype是SuperType类的一个对象，所以接着查找就是

SuperType函数内的this(找到)，如果未找到还会继续查找 SuperType.prototype,就像一个链条一样层层向上查找

还有其他的继承实现，但其根本原理都是借助 this 和 prototype，如下面组合继承

```
function SuperType(name){
  this.name = name;
  this.colors = ["red", "blue", "green"];
}
SuperType.prototype.sayName = function(){
  alert(this.name);
};

function SubType(name, age){
  // 继承this，把SuperType换做其他类就能继承其this上的属性
  SuperType.call(this, name); 
  this.age = age;
}

// 构建原型链
SubType.prototype = new SuperType(); 
```

至于其他 \_proto_  或 constructor 都是作为辅助，后面先说js内置类的继承关系

