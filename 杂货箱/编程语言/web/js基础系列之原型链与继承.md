---
title: js基础系列之原型链与继承
date: 2017-08-31 14:41:55
tags: 
	- js
	- 前端
categories:
    - js

---


>原型链并不难&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;——康搁搁

<!-- more -->

## 原型链
&emsp;&emsp;在理解原型链之前，首先要明白，**当一个函数被创建时，Function构造器产生的函数对象会运行类似这样的一些代码**

    this.prototype={ constructor:this }

&emsp;&emsp;即，创建的函数会有一个prototype(原型对象)属性，这个原型对象也有个属性是"constructor===函数名"，而每个被构造函数创实例，都有一个_proto_，来连接到创建他的函数的原型对象

>prototype

>是什么：每个函数(方法)都会有个prototype属性(除了bind方法建的函数)，指向其原型对象，我们可以叫做显式原型                这个对象伴随着函数而生

>作用：	用实现基于原型的共享属性

>\_proto\_

>是什么： 每创建一个对象(所有的对象都是某个函数的实例)，就会有个[[prototype]]属性，指向它的创建函数的原型对象(也称之为显式原型)，我们称为\_proto\_为隐式原型，在ES5之前没有明确方法可以通过实例，来访问创建实例的函数的显式原型，在主流浏览器中可以通过\_proto\_来进行访问，所以也可以把\_proto\_看做对象的隐式原型，通过\_proto\_链接到原型上，可以访问原型上的属性和方法

>作用：	构成原型链，实现基于原型链的继承

>constuctor

>是什么：每个原型对象都有的属性，在原型对象刚被创建时，值为产生原型对象的函数名，可修改

>作用：除了知道该原型对象对应的函数外，没什么卵用

&emsp;&emsp;知道了上面的那些基础概念后，我们来看看js的数据结构。是时候祭出这张图了!
![原型链](../img/yuanlinglian.png)
&emsp;&emsp;我们来理一理js数据类型的继承关系

+ js中创建了一个Function类型，由Function类型创建了Object类型

+ js说，要有原型对象，于是以Object为构造函数，创建了每个函数的原型对象,即，有了Function.prototype，Object.prototype

+ Object类型由Function创建，所以Object.\_proto\_指向Function.prototype

+ 而所有的prototype由Object创建，所以，Function.prototype.\_proto\_指向Object.prototype，而Object.prototype.\_proto\_不可能再指向自己，就被js设置为指向null

+ 再由Function创建了js中其他的内置类型，和我们自己创建的函数，由于Function.prototype._proto_指向Object.prototype，所以**我们创建的任何函数（包括内置函数），都直接的继承了Function.prototype，而且通过原型链间接继承了Object.prototype，而我们创建的函数（包括内置函数）的原型对象都直接继承了Object.prototype，函数的实例都直接继承了fun.prototype（我们创建的函数或者内置函数的原型对象），间接的继承了Object.prototype**

+ 嗯，就这样了，简单吧，不懂就对照着文字看图，这么详细了，实在不懂我也没办法了

&emsp;&emsp;呱唧呱唧的说了半天的原型链概念，那怎么检验我说的对不对呢，instanceof可以检测是否为某类型，检测的实质就是通过检测原型链来判断是否是此类型的

    console.log(String instanceof Function);  //true
    //每个构造函数，包括内置函数，都直接的继承了Function.prototype，所以返回true
    console.log(Array instanceof Object);
    //每个构造函数，包括内置函数，也间接的继承了Object.prototype，所以返回true
    function fun(){}
    console.log(fun instanceof Function); //true
    console.log(fun instanceof Object); //true
    
    var arr=new Array();
    var str=new Array();
    var i=new fun();
    
    console.log(arr instanceof Function); //false
    console.log(arr instanceof Object);  //true
    console.log(arr instanceof Array);  //true
    
    console.log(str instanceof Function); //false
    console.log(str instanceof Object);  //true
    console.log(str instanceof String);  //true
    
    console.log(i instanceof Function); //false
    console.log(i instanceof Object); //true
    console.log(i instanceof fun); //true
    //每个实例都直接继承了其构造函数的原型对象，间接继承了Object.prototype，而与Function无关系
    
## 继承
### 原型链语法糖
&emsp;&emsp;如果觉得原型链单词太长，懒得写，不如这里写一个语法糖，来简化为原型链添加属性或者方法的书写，并提供链式操作，减少代码量

    Function.prototype.add=function(name,func){
      if(!(this.prototype[name])){
        this.prototype[name]=func;
      }
      return this;
    }
    
 &emsp;&emsp;不仅提供了链式，而且还能检测函数原型中是否已经存在该属性，避免覆盖（如果有意覆盖，可以去掉if语句）   
    
    function a(){};
    a.add('name',function(){
      console.log(1);
    }).add('name',function(){
      console.log(2);
    });
    var i=new a();
    i.name(); // 1 避免了覆盖
    
### new操作符
&emsp;&emsp;当我们理解了原型后，不如来看看new操作符背后的故事，人们都说，每个成功的基佬后面都有一个腐女，我们来试试用add来实现new操作符

    Function.prototype.add = function(name, func) {
      this.prototype[name] = func;
    }
    Function.add('new', function() {
      var that = Object.create(this.prototype);
      //创建一个新对象，并继承了构造函数的原型
      var other = this.apply(that, arguments);
      //在这个新对象上调用构造函数
      return (typeof other === 'object' && other) || that;
      //如果返回值不是一个对象，就返回一个新对象
    });

&emsp;&emsp;使用new操作符，相当于是在每个函数上调用了这个new方法

### 继承的两种方式
&emsp;&emsp;在ES5中给了Object.create()方法（IE9+兼容）来帮助实现继承，那么我们就看看Object.create的底层和其他实现继承的方法

&emsp;&emsp;先看一个Object.create()的使用例子

    var obj={
        name:'kanggege',
        showName:function(){
        console.log(this.name);
    }
    }
    function f(){};
    f.prototype=Object.create(obj);
    f.prototype.showName(); //kanggege
    //f的原型对象继承了obj
    console.log(f.prototype.hasOwnProperty('showName')); //false
    //说明只是通过_proto_继承了，并没有复制

&emsp;&emsp;于是我们猜想，Object.create的实现应该是这样的

    Object.add('create',function(obj){
      function F(){}
      F.prototype=obj;
      return new F();
    });
    
&emsp;&emsp;有一点不舒服的是，在使用Object.create时，必须先调用函数原型fun.prototype=Object.create(fun)，我们可以自己写一个继承的函数，来减少代码量，并且让他支持链式书写

    Function.add('inherit',function(fun){
      this.prototype=new fun();
      return this; //实现链式
    });
    function Father(){}
    Father.add('getName',function(){
      console.log('kanggege');
    });
    var Child=function(){}
              .inherit(Father)
              .add('getNewName',function(){
                console.log(dykanggege);
              });
    var i=new Child(); // kanggege
    console.log(i.hasOwnProperty('getName')); //false

### 对象说明符
&emsp;&emsp;有时候构造器要接受一大堆参数，这很令人恶心，因为要记住参数的顺序和个数非常困难，如果我们在编写构造器时让它接受一个简单的对象说明符，这样就简便许多了
    
    function Person(per){
      this.name=per.name || 'kanggege';
      this.age=per.age || 19;
      this.show=per.show || function(){
        console.log(this.name);
      }
    }
    var person1=new Person({age:20});
    console.log(person1.name); //kanggege
    person1.show(); //kanggege

&emsp;&emsp;我们甚至可以为没有传入的值提供默认值，这样在传入参数时只用考虑要传入的值，不用考虑传入的个数和顺序                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           