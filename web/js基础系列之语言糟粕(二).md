---
title: js基础系列之语言糟粕(二)
date: 2017-09-02 15:17:55
tags: 
	- js
	- 前端
categories:
    - js

---

&emsp;&emsp;继语言糟粕一，继续讲一讲js要避免的糟粕

<!-- more -->
## 假值
&emsp;&emsp;js中的假值就像保留字一样，多的令人无语，0,NaN,'',false,null,undefined，跟恐怖的是**js中对声明而未赋值的变量，或者不存在的属性给定的默认值为undefined**(null："你置老子于何地")，如果你更骚的用了==来进行判定

    console.log(null == undefined); //true
    
&emsp;&emsp;执手相看代码，竟无语凝噎，还特么有这种操作。

&emsp;&emsp;undefined和NaN是window的一个属性，甚至在ES5之前undefined和NaN还不是个常量(可以更改)，算了吧，无力吐槽了。

## ==和！=
&emsp;&emsp;在js中提供了两种判断是否相等的方法，==和===，===会进行严格的判定，不仅比较值，也要确保类型相同

    var str='1';
    var num=1;
    console.log(str === num); //false
    console.log(str == num); //true
    
&emsp;&emsp;如果说===是有着铮铮铁骨的君子，那==就是个偷奸耍滑，卖主求荣的小人，如果两个运算数是不同的类型，它试图先去强制转换值的类型，转换的规则复杂而难以记忆，如果你不想在程序中出现莫名其妙的八阿哥(就好像你明明从来没有过女朋友，却忽然有人抱个孩子过来说这是你私生子)，就老老实实的抛弃了==和!=，而使用跟稳定的===