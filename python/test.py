#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'a test module'

__author__ = 'kanggege'

__name__ = '__main__'

class Student(object):
	def __init__(self,name,score):
		self.name = name
		self.score =score
		
	def print(self):
		print("%s %s",self.name,self.score)
	
	def other(self):
		# 这里会报错，只有 init 中可以声明属性
		self.oterVar = 'otherVar'
	
	def otherPrint(self):
		print(self.otherVar)

stu = Student('kanggege','666')
stu.print()


print(Student)
print(stu)
stu.other = 'kanggege'
print(stu.other)
