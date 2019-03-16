---
title: js基础系列之作用域与闭包
date: 2017-08-30 23:23:55
tags: 
	- js
	- 前端
categories:
    - js

---
> 闭包是js中最简单实用而又强大的功能&emsp;&emsp;&emsp;&emsp;——康搁搁
<!-- more -->
# 作用域与闭包
## js中的作用域
&emsp;&emsp;在诸如C语言这样严格的语言，{}内常常有自己的作用域，譬如

    #include<stdio.h>
    int main(){
        for(int i=0;i<10;i++){
            print("%d",i);
        }
    }

&emsp;&emsp;在这里，for的()和{}都是个块级作用域，但是，**在ES5和ES5之前，除了函数不存在块级作用域**
### for的块级作用域
&emsp;&emsp;**在ES5甚至之前，for，while等有{}的，并没有块级作用域，在其中声明的任何变量都会暴露在全局作用域下**

    for(var i=0;i<10;i++){}
    console.log(i); //10

&emsp;&emsp;而在ES6标准中引入了let和const，来声明块级作用域(具体会在以后，另写一篇博文讲解)

### 函数的作用域
&emsp;&emsp;每个执行环境都有一个变量对象(执行环境都有对象了，我还特么是个单身狗)，储存着这个执行环境的所有变量和函数，虽然代码无法直接访问，但是浏览器引擎需要它进行数据处理，通常在web浏览器中，全局变量对象可以用window访问到(实际上window并不是全局变量对象)，**在全局下声明的任何变量或函数，都可以看做window的属性**，每个函数也有自己的变量对象(这个是真访问不到了)，储存着在这个函数中声明的所有变量和函数。

&emsp;&emsp;每当一个函数执行，就会被推进一个环境栈中(关于js的单线程和异步，会在以后开博文详细解释)，然后会根据该函数的变量对象创建一个作用域链，在作用域链最前端永远是该函数的变量对象，然后依次向外查找扩充作用域链，直到全局变量对象，即，**函数可以访问到其外函数的变量，而外函数访问不到内函数的变量**

    function valI(){
      var i='i';
      function valJ(){
        var j='j';
        console.log(i); //i 内函数可以访问到外函数变量对象
      }
      function valK(){
        var k='k';
        console.log(j); //undefined 访问不到兄弟函数的变量对象
      }
      console.log(j+' '+k); //undefined 外函数访问不到内函数变量对象
    }
    valI();
    
## 闭包
&emsp;&emsp;上面我们知道了函数作用域与作用域链，那么闭包就变得容易理解了。先看下面这个例子

    var arr=[];
    for(var i=0;i<10;i++){
      arr[i]=function(){
        console.log(i);
      }
    }
    arr[3](); // 10
    arr[9](); // 10
    
&emsp;&emsp;这明显不是我们想要的结果，我们想要的是调用arr[index]()得到index，这是什么情况！对，一定是引擎出bug了，emmm....，那，我们就讲讲这个bug的原理是什么...

&emsp;&emsp;我们为每个arr[index]绑定了一个函数，这个函数的作用是输出i，实际上这里并没有得到i的值，只是单纯的在这里说明，我要用变量i，当这个函数被调用时，才真正的到i的值，这时i已经变为10了，旧梦依稀，物是人非啊..咳咳，那么怎么解决这个问题，看下面的代码。

    var arr=[];
    for(var i=0;i<10;i++){
      arr[i]=(function(index){
        return function(){
          console.log(index);
        }
      }(i));
    }
    arr[0]();  //0
    arr[6]();  //6
    
&emsp;&emsp;哎，这就很舒服，这样写就不忘初心了。这两种写法有什么区别呢，为什么会出现不同结果呢。首先，我们要再进一步了解函数调用的过程

&emsp;&emsp;之前已经说过每个函数被推入执行环境时会根据该函数的变量对象(以及外函数的变量对象，这里为了区分，把外函数的变量对象称作活动对象)创建作用域链，**每个函数执行完成后，都只会保留该函数的变量对象，而销毁作用域链**，我们看第二个代码，这里使用了个IIFE（立即执行函数），并把i作为参数传入，当我们把i作为参数index传入时，实际上就是在该函数内声明了变量index=i，然后该函数执行完成，理应销毁该函数的作用域链（index放在作用域链上），但是，由于内函数（即返回的那个函数）调用了index，如果销毁了index的链接，那么在使用到内函数时岂不是找不到index了，所以js就机智的保留了index的值，这就是闭包

    function clickEvent(node){
      function helper(i){
        return function(){
          console.log(i);
        }
      }
      for(var i=0;i<node.length;i++){
        node[i].onclick=helper(i);
      }
    }

&emsp;&emsp;这里创建了一个帮助函数helper来实现闭包，本来helper(i)执行完后，传入的i就应该被销毁的，但是返回的匿名函数要用到i，所以i就这样侥幸的存活了下来

&emsp;&emsp;闭包虽然很便于使用，但是一定小心内存泄漏，出现大量未被销毁的变量就会占据很多空间，所以，当使用完成后要及时用null切断与函数的联系，回收这些变量。

## 闭包的用法
### 保护变量
    
    var myObj = (function() {
      var name = 'kanggege';
      return {
        showName: function() {
          console.log(name);
        },
        changeName: function(newName) {
          name = newName;
        }
      }
    }());
    myObj.showName(); // kanggege
    myObj.changeName('dykanggege');
    myObj.showName(); //dykanggege

&emsp;&emsp;这样用闭包，除了我们给定的方法，就无法访问和修改name这个私有变量，仿佛看见整个代码都充满着原谅色，呸呸呸，保护色，上面是最简单的做法，其实跟好的做法是将私有变量的访问方法放在一个对象中

	var myObj=function(secret){
	  var that={};
	  var get=function(){
	    console.log(secret);
	  }
	  var set=function(name,val){
	    secret[name]=val;
	  }
	  that.get=get; //分两步写的好处是，that.get/set可能会被修改，而函数内部也可能再调用set
	  that.set=set; //分开写后，即使that.set被修改了，也不影响函数内部调用set
	  return that;
	}

&emsp;&emsp;这样传入私有对象，得到含有访问和修改私有对象方法的对象，我们甚至可以再为返回的对象添加其他的，对私有对象的处理方法(要先get私有对象)

### 增强记忆
&emsp;&emsp;别想多，这里增强的不是你的记忆，首先看下面的代码

    var num2=function(n){
      return n<2?n:n*num2(n-1);
    }
    console.log(num2(5)); //120

    var num1=function(n){
      return n<2?n:num1(n-1)+num1(n-2);
    }
    for(var i=0;i<=10;i++)
    console.log(num1(i)); //0 1 1 2 3 4 8 13 21 34 55
    
&emsp;&emsp;使用递归可以很好的帮助我们解决一些数学问题，但是这里存在个问题，num1一共被调用了453次，我们调用了它11次，而它自身调用了442次用于已经计算过的值，如果我们让函数具有记忆功能，就能显著减少运算量

    var num1 = (function() {
      var val = [0, 1];
      return function(n) {
        if (typeof val[n] === 'number') {
          return val[n];
        } else {
          val[n] = n < 2 ? n : num1(n - 1) + num1(n - 2);
          return val[n];
        }
      }
    }());
    console.log(num1(10)); //55
    
&emsp;&emsp;我们用val进行了值的储存，这样在以后求值，都可以显著的减少运算量
