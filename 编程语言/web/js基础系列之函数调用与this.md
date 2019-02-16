---
title: js基础系列之函数调用与this
date: 2017-08-28 17:23:55
tags: 
	- js
	- 前端
categories:
    - js

---

>&emsp;谁用了this，this就指向谁 &emsp;&emsp;——康搁搁

<!-- more -->
# 函数调用与this的指向
&emsp;在讲this的指向之前，首先要了解函数的几种调用模式

    fun();
    obj.method();
    fun.call(); / fun.apply(); /fun.bind();
    
&emsp;&emsp;抽象上来看，函数无非就这三种调用模式(new暂且不说)，而前两种都是最后一种的语法糖(语法糖就是复杂语法的简便写法)，即，**任何函数调用的实质都是第三种调用**(第三种调用中放的三个例子调用实质是一样的)

**&emsp;函数调用本质上只有这一种形式 fun(context,p1,p2...)**

## fun()调用
&emsp;&emsp;函数调用中最常见的就是fun();形式，如下

    fun()  ===  fun.call(undefined)
    
看如下例子
    
    function Persion(name){
        this.name=name;
    }         
    Persion('kanggege'); 
    console.log(window.name); //kanggege
    
>&emsp;&emsp;在浏览器中，如果call调用传入的context是null或者undefined，则默认的对象是window(严格模式下，依然是undefined)

## obj.method()调用
&emsp;&emsp;即，方法调用

    obj.menthod()  ===  obj.method.call(obj)

&emsp;&emsp;看下面的例子

    function Persion(name){
      this.name=name;
    }
    Persion.prototype.showName=function(){
      console.log(this.name);
    }
    var persion = new Persion('kanggege');
    persion.showName();  //kanggege

&emsp;&emsp;在我是小白的时候(当然现在也超级菜)，一直认为这很不科学，因为在Persion.prototype.showName中的this应该指向prototype，即Persion函数的原型对象，但是我们知道，对象可以访问到原型链上的共享属性，但是原型访问不到对象的私有属性，那么console.log(this.name)应当是访问不到的，那是为啥访问到了呢，我们看看调用的实质

    persion.showName() === persion.showName.call(persion)
    
&emsp;&emsp;原来this指向了persion，这就很合理了，同时也说明了，**实例化的对象中，this指的是对象本身；未实例化中，this指的是实际调用者的对象**，通俗的说就是，**this的指向是可变的，并不固定，谁调用这个函数，this就指向谁**

## 其他的调用形式
&emsp;&emsp;虽然我们说，函数就这三种调用形式，但是这三种调用形式是穿着皇帝的新衣，也有其他的花枝招展的调用形式，但是俗话说的好，关上灯都一样，今天我们就扒光他们的衣服，与我们坦诚相见，看一样不一样

### []形式

    var i={
      f1:function(){
        console.log(this);
      }
    }
    var arr=[i];
    arr[0].f1(); //对象i

&emsp;&emsp;乍一看，好似this指向的是arr，其实不然，我们扒光衣服再看

    arr[0].f1() === arr.0.f1.call(0)
    
&emsp;&emsp;虽然转化后语法是错的，但是我们这里是揭露个实质

### Event Handler 中的 this

    btn.addEventListener('click' ,function handler(){
      console.log(this) // 请问这里的 this 是什么
    })

>MDN上这样说，通常来说this的值是触发事件的元素的引用，这种特性在多个相似的元素使用同一个通用事件监听器时非常让人满意。 

>当使用 addEventListener() 为一个元素注册事件的时候，句柄里的 this 值是该元素的引用。其与传递给句柄的 event 参数的 currentTarget 属性的值一样。

&emsp;&emsp;不要瞎猜，所有this的指向，要么跑不出我们举例的三种情况，要么是特殊的形式，相应文档会给予说明

## this用法
### 引擎调用
    
    foo = function(){
    	this.myName = "Foo function.";
    }
    foo.prototype.sayHello = function(){
    	console.log(this.myName);
    }
    foo.prototype.bar = function(){
    	setTimeout(this.sayHello, 1000);
    }
    var f = new foo;
    f.bar(); //undefined

&emsp;&emsp;这是因为，当我们用setTimeout来向eventLoop注册函数时，函数是由引擎来发起调用的，因此这个调用者/caller会自动绑定为window

    foo = function(){
    	this.myName = "Foo function.";
    }
    foo.prototype.sayHello = function(){
    	console.log(this.myName);
    }
    foo.prototype.bar = function(){
    	setTimeout(this.sayHello, 1000);
    }
    var f = new foo;
    foo();
    f.bar(); //Foo function
    
&emsp;&emsp;这样就很好的解决了，但是污染了window，不是个好的方式

    foo = function(){
    	this.myName = "Foo function.";
    }
    foo.prototype.sayHello = function(){
    	alert(this.myName);
    }
    foo.prototype.bar = function(){
    	setTimeout(this.sayHello.bind(this), 1000);
      //setTimeout(this.sayHello.call(this), 1000);
      //setTimeout(this.sayHello.apply(this), 1000);
    }
    var f = new foo;
    f.bar()
    
&emsp;&emsp;我们自发的为他绑定上调用的this指向，就不会再被就引擎绑定为window了，很好的解决了这个问题

### 储存this

    var i={};
    i.a=1;
    i.b=2;
    i.num=function(){
      var that=this;
      function add(){
        console.log(that.a+that.b);
      }
      add();
    }
    i.num();
  
&emsp;&emsp;使用that把this储存起来，这样内部变量就可以访问到该方法的对象了，这也是this的一个巧用  
 
### 链式操作
&emsp;&emsp;在jQuery中，我们一般都可以进行$(obj).css().index()等一系列操作，想实现这个原理也极其简单，只需要在调用完后返回调用的对象即可

    var obj={
      name:'kanggege',
      getName:function(){
        console.log(this.name);
        return this;
      },
      setName:function(i){
        this.name=i;
        return this;
      }
    };
    obj.setName('dykanggege').getName(); //dykanggege
    
&emsp;&emsp;就这样，非常简单而又实用的实现了链式操作