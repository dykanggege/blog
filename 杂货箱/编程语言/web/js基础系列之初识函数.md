---
title:  js基础系列之初识函数
date: 2017-08-24 10:23:55
tags: 
	- js
	- 前端
categories:
    - js

---

>JavaScript设计得最出色的就是它的函数的实现。它几乎接近于完美
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;——《js语言精粹》

**&emsp;本文并不是对js函数各个特性进行详解，事实上，一篇文章是根本写不完的，这里只是对函数进行粗略的介绍，让刚接触js函数的人对其有所了解**
<!-- more -->
## 函数对象
>&emsp;js中的函数就是对象。

&emsp;&emsp;注意了，对象这个概念很重要，我要重点说明一下。

&emsp;&emsp;**在js中，对象指的不是女朋友！**
&emsp;&emsp;**在js中，对象指的不是女朋友！**
&emsp;&emsp;**在js中，对象指的不是女朋友！**

&emsp;&emsp;对象是“名/值”对的集合并拥有一个连到原型对象的隐藏连接

&emsp;&emsp;因为函数是对象(即函数名可以看做一个对象变量)，所以它可以像其他的值一样被使用，可以保存变量，可以作为值被传递，**可以拥有对象和方法**

&emsp;&emsp;**而函数的与众不同之处，在于他们可以被调用**

        function test() {
          console.log(1);
        }
        test.val='val';
        test.fun=function(){
          console.log('fun');
        }
        test(); //1
        console.log(test.val); //val
        test.fun(); //fun
    
&emsp;&emsp;**js创建一个函数对象时，会给该对象设置一个“调用”属性，当js调用一个函数时，可以理解为调用了此函数的“调用”属性**

&emsp;&emsp;如果看不懂上面那句话，没关系。我们可以把创建函数看做创建一个对象，而函数名test就是指向那个对象的变量，这个对象一被创建就拥有了一个方法，通过“变量名()”来调用此方法，即“ console.log(1);”。既然它是个对象，自然可以拥有属性和方法，即test.val和test.fun。这样我们就明白了，**除了用“函数名()”的方式来调用函数(的调用属性)，函数名本身是可以作为一个对象使用的。**

        function test(){};
        test.i='i';
        function test2(obj,index){
          console.log(obj[index]);
        }
        test2(test,'i'); //i

##函数声明提升
&emsp;&emsp;**函数声明提升，即在执行代码之前会前读取函数声明**

         test(); //1
         function test() {
           console.log(1);
         }

&emsp;&emsp;这意味着，可以把函数声明放在它的语句后边，可以更加集中的处理函数的放置的位置

**&emsp;但是，要区分函数声明与函数表达式**

        i(); //报错
        var i = function() {
        console.log(1);
        }
    
&emsp;&emsp;这里声明了一个匿名函数并且赋值给i，我们之前说过，先读取函数声明再执行代码，而函数声明必须是以function开头的，当函数声明读取完成后，代码从上而下执行，到i()时，i还没有被声明。

看下面的例子

        if(true){
          function say(){
            console.log("first");
          }
        }else{
          function say(){
            console.log("second");
          }
        }
        say();
    
&emsp;&emsp;这样的做法是非常糟糕的，由于存在函数声明提升，所以需要浏览器来智能修复你的代码错误，而且很多浏览器会得到“second”

        if (true) {
          var say = function() {
            console.log("first");
          }
        } else {
          var say = function() {
            console.log("second");
          }
        }
        say(); //first
    
&emsp;&emsp;用表达式的写法，就准确无误了

##函数的参数
&emsp;&emsp;参数的灵活性，为构建函数提供了极大的灵活性

&emsp;&emsp;声明函数时，可以声明将来可能被传入的形参，并在函数中对形参进行处理。当传入实际参数多余形参时，多余的参数会被忽略，而实际参数少于形参时，未被传入值的形参默认为undefined。对参数的值不会进行类型检查，任何类型的值都可以被传递给任何参数

        function test(i,j,k) {
          console.log(i+' '+j+' '+k);
        }
        test('a','b'); //a b undefined
        test('a','b','c','d'); //a b c
    
**&emsp;&emsp;这是函数的一些基本特性，至于函数调用和this，函数作用域和闭包，继承和原型链，会在以后另开博文详细讲解**

&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;*转载请指明出处*