# 模块
每个 py 文件都是一个模块，多个模块可以放在一个目录下，如 a.pu、b.py 放在 dir 下，则路径为 dir.a、dir.b

包目录下必须有一个 _init_.py 文件，否则 py 会把这个目录视为一个普通的目录

_init_.py 可以是一个空文件，也可以写代码，通过包名引用 import dir

```
	#!/usr/bin/env python3
	# -*- coding: utf-8 -*-
	
	# 文档注释，包的作用
	'a test module'
	
	__author__ = 'kanggege' # 注明包作者
	
	import sys
	
	def test():
		args = sys.argv
		if len(args) == 1:
			print('hello')
	
	if __name__=='__main__'
		test()
	
	# 当运行某个 py 文件时，会把文件的 _name_ 设置为 '_main_' 通过这种方式标明主程序的入口，同样也可以用于非程序入口文件做测试
	# 注意是两个 _
```

# 封装
py 模块中的全局变量和函数是 public，类似于 _xxx_ 的变量是特殊变量，也可以直接引用，通过 _abc 或 __abc 定义 private，这只是语义上的定义，实际仍可以访问

# 导入模块
import 模块，py 解释器会搜索当前目录、所有已安装的内置模块、第三方模块，搜索路径存放在 sys.path 中，如果当前目录的模块名和内置模块重复，则会覆盖内置模块