# java8

## 增强了基本包装类的功能
自己查API文档

## 增强了接口的功能
### 在接口中定义默认方法
该方法必须用 default 修饰，且不能用static修饰，可以定义方法体
### 在接口中定义静态类方法
使用 static 修饰，可以定义静态类方法

注意：接口里的成员变量默认使用 public static final 修饰

## 匿名内部类
当一个接口中只定义了一些方法(比较少)，当我们想调用这些方法时，正常的流程是这样的

- 创建该接口实现类文件，并实现接口中的方法
- 在使用的地方创建类实例
- 使用实例调用方法

实际上，可以用匿名内部类简化这一流程

  Interface A {
    void method1();
    void method2();
  }

  Class B {
    public A a = new A(){
      
      public void method1(){
        System.out.println("在此处创建了实现该接口的类");
        //在编译后还是会生成相应的类文件
      }

      public void method2()...//省略实现
    };

    //或者在方法中作为参数时
    public static void test(A a){
      a.method1();
    }

    public static void main(String[] args) {
      final String name = "康搁搁";

      test(new A() {
        public void method1(){
          System.out.println(name);
        }
        public void method2()...
      });
	  }

  }

匿名内部类虽然使用方便，但是仅适用于那些只使用一次的接口，如果要多次在不同地方使用该接口，那还是老老实实的用类文件实现它

**注意：匿名内部类使用类外的变量，该变量必须用 final 修饰，java8之后，可以不用final修饰(编译时会自动判别)**

## Lambda表达式
Lambda表达式是匿名内部类的升级版，当接口只有一个抽象方法时(注意：**只有一个抽象方法，可以有其他的default方法**)，可以简写

  Interface A {
    void method();
  }

  Class B{
    public static void test(A a){
      a.method();
    }

    public static void main(String[] args){
      test( () -> {
        System.out.println("甚至类都不需要声明，直接初始化函数就行了");
      })
    }

    //实质上，它返回的是一个对象，实现了该方法的对象
    A a = () ->  System.out.println("are you ok");

  }

这就是Lambda表达式，可以传入Lambda表达式的参数称为函数式接口，注意一下几点

- 如果参数只有一个，可以省略()
- 如果函数体语句只有一句，可以省略{}
- 如果函数体只有一句，且函数需要返回值，可以省略{}，和 return

java8还为Lambda表达式提供了方法引用和构造器引用，但是鄙人认为并不美观，且不符合java语法特点，反倒是很像C++，所以在此不做介绍

## 增强 Arrays 工具类
提供了一些函数式接口的API，在原有的API基础上，扩充支持多CPU并行运算