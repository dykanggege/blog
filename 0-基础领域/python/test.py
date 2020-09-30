#! /usr/bin/python3
# 指定用哪个解释器运行
# -*- coding: utf-8 -*-

'一段注释'

__author__ = 'kanggege'

import subprocess

def test():
    res = subprocess.run(["ls","-al"])
    print(res)



if __name__=='__main__':
    test()