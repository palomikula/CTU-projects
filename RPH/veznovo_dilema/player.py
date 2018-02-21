class MyPlayer():
	'''player detects some strategies, if none detected, then plays his own strategy'''
	def __init__(self, payoff_matrix, number_of_iterations = 0):
		self.payoff_matrix = payoff_matrix
		self.number_of_iterations = number_of_iterations
		self.opponent_move_list = []

	def move(self):
		if (self.number_of_iterations !=0):
			if (len(self.opponent_move_list) + 3 >= self.number_of_iterations):
				return True

		#first seven moves to detect some strategies
		if (len(self.opponent_move_list) <= 6 ):
			move_one_to_seven = [False,True,False,False,True,True,False]
			return move_one_to_seven[len(self.opponent_move_list)]

		def get_opponents_strategy():	
			#list of some possible opponent's strategies
			strategy=[[False,False,False,False,False,False],[True,True,True,True,True,True],[False,True,False,False,True,True],[False,False,False,False,False,True],[True,True,True,False,False,True],[True,False,False,True,True,False]]

			#compares strategies from the list with opponent's moves 2 to 7
			for i in range (0,6):  											
				if(strategy[i]==self.opponent_move_list[1:7]):
				 	return i #if strategy detected, function returns index of opponent's strategy from the strategy list
				else:
					pass
		opponents_strategy = get_opponents_strategy()
			
		#if opponent always returns the same
		if (opponents_strategy==(0 or 1)):							
			return True

		#if opponent always repeats Myplayer's last move
		if (opponents_strategy==2):
			return False

		#if changing opponent's move requires two consecutive moves of the opposite value (after two Myplayer's Trues returns True until two Myplayer's Falses) 
		if (opponents_strategy==(3 or 4)):
			# returns True,False,True,False....
			if(len(self.opponent_move_list)%2 == 1):
				return False
			else:
				return True 
		#if MyPlayer plays against himself
		if (opponents_strategy==5):
			return False

		#if no strategy detected MyPlayer plays his own strategy
		else:
			if(self.opponent_move_list[-1]==False and self.opponent_move_list[-2]== False):
				return False
		
			else:
				return True
				
			
	def record_opponents_move(self, opponent_move):
		self.opponent_move = opponent_move
		self.opponent_move_list.append(self.opponent_move)
		return self.opponent_move_list
		

if __name__ == "__main__":
	pl1 = MyPlayer([ [(4,4),(1,6)] , [(6,1),(2,2)] ], 55)
	pl2 = MyPlayer([ [(4,4),(1,6)] , [(6,1),(2,2)] ], 55)
	print (pl1.move()) 

	

	
	
