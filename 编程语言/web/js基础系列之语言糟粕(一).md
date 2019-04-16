---
title: js基础系列之语言糟粕(一)
date: 2017-08-31 14:41:55
tags: 
	- js
	- 前端
categories:
    - js

---

>历史遗留问题，总能成为语言的累赘&emsp;&emsp;&emsp;&emsp;——康搁搁

<!-- more -->

## 全局变量
&emsp;&emsp;js没有链接器，所有的编译单元都载入一个公共全局对象中，而在任何一个单元中，以下这三种方式都会定义全局变量

+ 在函数外声明变量   var i;
+ 直接给全局对象添加属性  window.i=1;
+ 在函数外，未声明直接使用变量  i=1;

&emsp;&emsp;**每个单独的js文件最终被载入到一个js文件中**，如果每个单独的js文件中使用了大量的全局变量，很容易造成变量名冲突，而造成各种错误，为了避免这种错误，在每个js文件中都要使用IIFE或者ES6下的块级作用域，来保护变量

## 变量提升
&emsp;&emsp;起始，js为了照顾新手，所以才有了变量无需声明也可以直接使用，但是js引擎中存在着变量提升，所以没有声明直接使用很容易造成错误

    console.log(i); //undefined
    var i=0;
    console.log(i); // 0
    
    console.log(j); //报错
    j=0;
    console.log(j); // 0
    
&emsp;&emsp;第一个例子中，先调用了i得到undefined，再声明i，没有报错，说**明js引擎会将所有的变量声明，提升到每个函数的头部或者全局作用域的头部**，而js中，所有声明但是未赋值的变量都默认为undefined，所以得到了undefined，等到执行到赋值语句后，i才有了值，才能得到0

&emsp;&emsp;第二个例子中，先调用了j，报错了，因为j自始至终没有声明，所以不存在变量提升，j不存在就会报错，等到解析到j=0，存在赋值语句并检测到没有声明，这才对j进行了声明(引擎会帮助你声明)，后面就可以调用了，再看多次声明同一变量

    var i=0;
    console.log(i); // 0
    var i=1;
    console.log(i); // 1

&emsp;&emsp;有些童鞋会说，这又不科学了，这里声明了两次i，如果被提升到头部了，那么第一次输出i也应该得到1，那你就搞混了声明和赋值，虽然有两次声明，**但引擎会在代码执行前进行预处理，碰到第一次声明进行变量提升**，碰到var&emsp;i=1发现已经提升过了，就不再理会，等到开始执行代码，再碰到i=0和i=1才进行赋值

&emsp;&emsp;js中对于变量这样漫不经心的设定，可能造成变量未声明就使用或者变量重复声明而被覆盖，而发生的各种匪夷所思的八阿哥。因为没有块级作用域，面对变量问题最好的做法就是**在每个函数的开头部分，声明所有函数中要用到的变量。在每个文件的IIFE头部，声明该文件用到的所有变量，并调用函数，将该文件的所有函数声明放在下面(毕竟js中存在着函数提升)**，或者使用ES6中的let和const

## 自动插入分号
&emsp;&emsp;js有个自动修复机制，他试图自动插入分号来修正有缺陷的程序，但是别指望他能给你修复什么八阿哥，不添乱就是好的了

    return
    {
      status:true
    };

&emsp;&emsp;可能你的本意是返回一个对象，js可不这么认为，他会在return后加上一个分号，你的程序就从返回一个对象变为返回一个undefined了。如果把{放在return后就可以避免这种错误

    return {
        status : true
    };

## 保留字
&emsp;&emsp;js中的保留字多的你难以想象(即使他们大多没有被用在语言中)，幸好ES5进行了缩减(我们面向IE9+，所以ES5之前的就不考虑了)，ES5中非严格模式下保留字缩减为class&emsp;enum&emsp;extends&emsp;super&emsp;const&emsp;export&emsp;import，严格模式下还有这些，implements&emsp;package&emsp;public&emsp;interface&emsp;private&emsp;static&emsp;let&emsp;protected&emsp;yield

&emsp;&emsp;不能使用保留字作为变量名和参数，如果用作键值要用引号括起来(最好还是别用)，并且不能用在点表示法中，所以有时只能用中括号表示法

## typeof
&emsp;&emsp;typeof用于识别类型，但是它绝不是你想象的那么天真无邪，看下面的检测结果

    console.log(typeof Array);
    console.log(typeof String);
    console.log(typeof Function);
    console.log(typeof Object);
    var fun=new Function();
    console.log(typeof fun);
    //上面得到的统统是function，这可以理解，毕竟都是构造函数  

    var str=new String();
    console.log(typeof str); //object
    var arr=new Array();
    console.log(typeof arr); //Object
    var reg=new RegExp();
    console.log(typeof reg); //Object
    //这里得到的都是Object，也可以理解，都是某种类型的对象
    
    var sstr='woccc';
    console.log(typeof sstr); //string
    var char='a';
    console.log(typeof char); //string
    var boo=true;
    console.log(typeof boo); //boolean
    var i=1;
    console.log(typeof i); //number
    console.log(typeof undefined); //undefined 这可以理解
    //字面量能得到正确的结果，也挺放心的
    
    console.log(typeof null); //object  这特么就不能忍了
    console.log(typeof NaN); //number 说好的NaN表示非数字值呢
    
&emsp;&emsp;鉴于typeof这混乱不堪的设定，我们最好避免使用typeof来检验(你甚至无法想象什么东西都能通过检验)，即使要用，也要加上多重检验标准(除非你检验标准十分松散)，下面给出一个标准的数字检验方法

    Function.add(isNumber, function(val) {
      return typeof val === 'number' && isFinite(val);
    });

## parseInt
&emsp;&emsp;看到这个标题时你可能会想，"这是个很有用的工具啊，你别是在故意挑刺吧"，好吧，被你看出来了...parseInt会把字符串转换为整数，碰到非数组字符时停止解析，返回解析的结果，如下
    
    console.log(parseInt('1024'));  //1024
    console.log(parseInt("1024kangege"));  //1024

&emsp;&emsp;呀，挺好的，还能帮我们摘出非数字字符呢(但是变量出现了额外的非数字字符你也不知道)，再看下面的例子

    console.log(parseInt('09')); //0

 &emsp;&emsp;可能你在自己的浏览器上测试得到的是9，实际上在parseInt()中，如果输入string以“0”开始，则解析基数为八(八进制)，但是在ES5中已经强制设定默认基数为十，即，parseInt('09',10)，最好的做法是总带上这个参数，避免老版浏览器中出错，而且如果用parseInt来解析日期字符串，前面的0总是被忽略的，这点要注意

## 浮点数
&emsp;&emsp;二进制的浮点数无法处理好十进制的小数，因此在进行浮点数的运算时总会出现一些奇怪的东西

    console.log(0.1+0.2); //0.300000000000004
    
&emsp;&emsp;怎样解决这个八阿哥？你听说过放大法吗，看下面例子：

    var i=1+2;
    i/=10;  //0.3
    
## 伪数组
&emsp;&emsp;js中没有真正的数组，内置的数组对象只能算是个伪数组，性能相比较于真正的数组，相差太远。而且用typeof检验数组得到的是object，甚至用constructor检验是否为数组都不太可靠，下面提供了个更可靠的方法

    if(Object.prototype.toString(arr) === "[object Array]"){
      console.log("确实是个数组");
    }


    



