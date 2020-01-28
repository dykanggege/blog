#! /usr/bin/python3
# -*- coding: utf-8 -*-

'a test module'

import sys


class Solution:
	def Find(self, target, arr):
		w = len(arr)
		
		if w == 0:
			return False
		
		h = len(w[0])

		if h == 0:
			return False
		
		lw, rw = 0, w-1


if __name__ == "__main__":
	Solution().Find(1, [1,2,3])