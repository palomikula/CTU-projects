#!/bin/python3
# -*- encoding: utf-8 -*-

def solve(weigh):
	heavier = weigh([1,2,3,4],[5,6,7,8]) 	
	if  heavier == 'none':
		heavier = weigh([1,2,3],[9,10,11])		
		if heavier == 'none':
			if weigh([1],[12]) == 'left':
				return (12, '-')
			else:
				return (12, '+')
		if heavier == 'left':
			heavier = weigh([9],[10])
			if heavier == 'none':
				return (11, '-')
			if heavier == 'left':
				return (10, '-')
			else:
				return (9, '-')
		else:
			heavier = weigh([9],[10])
			if heavier == 'none':
				return (11, '+')
			if heavier == 'left':
				return (9, '+')
			else:
				return (10, '+')
###################################################
	if heavier == 'left':
		heavier = weigh([1,5,9],[2,6,7])		
		if  heavier == 'none':
			heavier = weigh([3],[4])
			if  heavier == 'none':
				return (8, '-')
			if heavier == 'left':
				return  (3, '+')
			else:
				return (4, '+')
		if heavier == 'left':
			heavier = weigh([6],[7])
			if heavier == 'none':
				return (1, '+')
			if heavier == 'left':
				return (7, '-')
			else:
				return (6, '-')
		else:
			if weigh([2],[12]) == 'none':
				return (5, '-')
			else:
				return (2, '+')
###################################################
	else:		
		heavier = weigh([1,5,9],[2,6,7])
		if heavier == 'none':
			heavier = weigh([3],[4])
			if heavier == 'none':
				return (8, '+')
			if heavier == 'left':
				return  (4, '-')
			else:
				return (3, '-')
		if heavier == 'left':
			if weigh([2],[12]) == 'none':
				return (5, '+')
			else:
				return (2, '-')
		else:
			heavier = weigh([6],[7])
			if heavier == 'none':
				return (1, '-')
			if heavier == 'left':
				return (6, '+')
			else:
				return (7, '+')



		
