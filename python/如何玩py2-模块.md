# 模块
在 py 中，一个文件就是一个模块，一个文件夹是一个包，通过 import 包.模块 导入模块中所有函数 包.模块.fun，可以直接导入模块，也可以在包中再嵌套包

注意，在每一个目录下都有一个 __init__.py 文件，这个文件可以有代码或是空文件，他的作用是标明这个目录是一个包结构

```
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'a test module'

__author__ = 'kanggege' # 标注该模块的作者

import sys

def test():
	args = sys.argv
	if len(args) == 1:
		print('hello')

if __name__='__main__'
	test()

```
第一行注释是让它可以直接在 unix 上运行，第二行标明使用 utf-8 编码

当 py 编译器运行一个 py 文件时，会将该模块的 __name__ 设置为 '__main__' 因此可以使用它作为测试使用，当它被作为模块导入时就不会执行

# 作用域
正如 py 中没有常量，只能定义大写变量大家默认它是常量不去修改它，py 的模块中也没有访问限制，我们可以通过约定实现 private 和 public

一般如 __name__ 格式的，都是特殊变量，可以引用但是不要修改，也不要创建类似格式的变量，如：__author__ 模块作者，__doc__ 文档注释

类似 _xxx 的变量或方法，是 private 的，不要直接去使用它