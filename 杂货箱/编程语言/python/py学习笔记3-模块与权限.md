# 模块
每个 py 文件都是一个模块，多个模块可以放在一个目录下，如 a.pu、b.py 放在 dir 下，则路径为 dir.a、dir.b

**包目录下必须有一个 \_init\_.py 文件，否则 py 会把这个目录视为一个普通的目录**

\_\_init\_\_.py 可以是一个空文件，也可以写代码，通过包名引用 import dir

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
	
	if __name__=='__main__':
		test()
	
	# 当运行某个 py 文件时，会把文件的 _name_ 设置为 '_main_' 通过这种方式标明主程序的入口，同样也可以用于非程序入口文件做测试
	# 注意是两个 _
```

**项目入口，if \_\_name\_\_=='\_\_main\_\_'**

# 导入模块


import my.pack.demo 则使用时必须补全路径 my.pack.demo.param

from my.pack import demo 导入，则直接使用 demo，不用补全路径，等同于 import my.pack.demo as demo

导入的最小单位是文件

import 模块，py 解释器会搜索当前目录、所有已安装的内置模块、第三方模块，搜索路径存放在 sys.path 中，如果当前目录的模块名和内置模块重复，则会覆盖内置模块

python 至少在导入依赖比java强一点，不用将所有依赖都放在一个项目内，必须项目内导入，python根据路径共享第三方依赖，但是依赖版本管理又成一个问题了

# 封装
py 模块中的全局变量和函数是 public，类似于 \_xxx\_ 的变量是特殊变量，也可以直接引用，通过 _abc 或 __abc 定义 private，这只是语义上的定义，实际仍可以访问（看编译器，总的来说这是不好的行为）

注意，每个文件内都有一些 \_\_xx\_\_ 的变量，自己不要这样，避免冲突



