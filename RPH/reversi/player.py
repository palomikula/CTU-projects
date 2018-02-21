import random
import time
import copy


class MyPlayer(object):
    '''player chooses the most preferable move by making random simulations'''

    def __init__(self, my_color, opponent_color):
        self.start=time.clock()
        self.name = 'mikulpa6'
        self.my_color = my_color
        self.opponent_color = opponent_color
        pass

    def move(self, board):
        self.board = board
        if (self.find_valid_moves(self.my_color, self.opponent_color, board) == []):
            return None

        else:
            return self.find_best_move()

    def find_all_positions(self, board):
        for r in range(0, len(board)):
            for c in range(0, len(board)):
                yield r, c

    def find_all_moves(self, board):
        for r, c in self.find_all_positions(board):
            if (board[r][c] == -1):
                yield r, c

    def find_valid_moves(self, my_color, opponent_color, board):
        start=time.clock()
        m = 0
        valid_moves_list = []
        self.sequences_list = []
        self.capturable_stones_list = []
        for r, c in self.find_all_moves(board):
            for i in range(-1, 2):
                for j in range(-1, 2):
                    if (0 <= r + i < len(board) \
                    and 0 <= c + j < len(board) \
                    and board[r + i][c + j] == opponent_color):
                        k = 1
                        while ((0 <= r + (k + 1) * i < len(board) \
                        and 0 <= c + (k + 1) * j < len(board)) \
                        and board[r + k * i][c + k * j] == opponent_color):
                            k += 1
                        if (board[r + k * i][c + k * j] == my_color):
                            valid_moves_list.append((r, c))

        return list(set(valid_moves_list))

    def find_best_move(self):
        '''This function first checks valid moves for "sweet spot"
        appearences, prefers the ones with lower index from
        self.sweet_spot list and avoids bad moves. Then if none
        are found, function simulates as many random games as it
        can for eachvalid move and chooses the one with the highest
        overall chance of win'''

        self.sweet_spot = [[(0, 0), (0, 7), (7, 0), (7, 7)],\
                           [(0, 2), (2, 0), (0, 5), (2, 7), (5, 0), (7, 2), (5, 7), (7, 5)],\
                           [(2, 2), (2, 5), (5, 2), (5, 5)]]
        bad_moves = [(1, 1), (1, 6), (6, 1), (6, 6)]

        #some ugly brute-forced self.sweet_spot and bad_moves lists exceptions
        i=0
        while(i<len(self.sweet_spot[0])):
            if (self.board[self.sweet_spot[0][i][0]][self.sweet_spot[0][i][1]] != -1):
                self.sweet_spot[0].remove(self.sweet_spot[0][i])
                self.sweet_spot[1].remove(self.sweet_spot[1][2*i])
                self.sweet_spot[1].remove(self.sweet_spot[1][2*i])
                self.sweet_spot[2].remove(self.sweet_spot[2][i])
                bad_moves.remove(bad_moves[i])
            i+=1
        i=0

        #more exceptions
        while(i<len(self.sweet_spot[1])):

            if (self.board[self.sweet_spot[1][i][0]][self.sweet_spot[1][i][1]] != self.my_color \
            and self.board[self.sweet_spot[1][i][0]][self.sweet_spot[1][i+1][1]] != -1):
                self.sweet_spot[1].remove(self.sweet_spot[1][i])
                self.sweet_spot[1].remove(self.sweet_spot[1][i])


            elif (self.board[self.sweet_spot[1][i][0]][self.sweet_spot[1][i][1]] != -1 \
            and self.board[self.sweet_spot[1][i][0]][self.sweet_spot[1][i+1][1]] != self.my_color):
                self.sweet_spot[1].remove(self.sweet_spot[1][i])
                self.sweet_spot[1].remove(self.sweet_spot[1][i])

            else:
                i+=2


        self.valid_moves_list = self.find_valid_moves(self.my_color, self.opponent_color, self.board)
        sweet_moves = []

        #returns random "sweet move" from the prefered ones
        for i in range(0, len(self.sweet_spot)):
            for j in range(0, len(self.sweet_spot[i])):
                for k in range(0, len(self.valid_moves_list)):
                    if (self.sweet_spot[i][j] == self.valid_moves_list[k]):
                        sweet_moves.append(self.valid_moves_list[k])

            if (len(sweet_moves) != 0):
                best_move = random.choice(sweet_moves)
                time.sleep(0.9)
                return best_move

        '''this part uses functions update_board(), end_game()
         and get_result() to simulate random games and pick a
         move that won by the most'''
        if(len(sweet_moves)==0):
            start = time.clock()
            tmp=0

            while(True):
                check_time=time.clock()
                old_board = copy.deepcopy(self.board)
                valid_moves = copy.deepcopy(self.find_valid_moves(self.my_color, self.opponent_color, old_board))

                # doesn't check for the bad_moves (saves time)
                i=0
                while(i<len(valid_moves)):
                    if((valid_moves[i] in bad_moves) \
                    and len(valid_moves)!=1):
                        valid_moves.remove(valid_moves[i])
                    i+=1

                result = [0] * len(valid_moves)
                tmp+=1

                #runs the simulations for valid moves
                for l in range(0, len(valid_moves)):
                    old_board = copy.deepcopy(self.board)
                    updated_board = self.update_board(old_board, valid_moves[l], self.my_color, self.opponent_color)
                    my_move = False
                    while (not self.end_game(updated_board)):

                        old_board = copy.deepcopy(updated_board)
                        if ((my_move) and (len(self.find_valid_moves(self.my_color, self.opponent_color, old_board)) != 0)):
                            random_move = random.choice(self.find_valid_moves(self.my_color, self.opponent_color, old_board))
                            updated_board = self.update_board(old_board, random_move, self.my_color, self.opponent_color)

                        if ((not my_move) and (len(self.find_valid_moves(self.opponent_color, self.my_color, old_board)) != 0)):
                            random_move = random.choice(self.find_valid_moves(self.opponent_color, self.my_color, old_board))
                            updated_board = self.update_board(old_board, random_move, self.opponent_color, self.my_color)

                        my_move = not my_move

                    result[l] += self.get_result(updated_board)
                check_time = time.clock()-check_time        #checks the time of one simulation

                #breaks if the next simulation wold be over time limit
                if(check_time+(time.clock()-start)>0.99):
                    break

            #chooses and returns the best move
            mx = result.index(max(result))
            return valid_moves[mx]

    def update_board(self, board, move, my_color, opponent_color):
        start = time.clock()
        board[move[0]][move[1]] = my_color
        updated_board = copy.deepcopy(board)

        for i in range(-1, 2):
            for j in range(-1, 2):
                if (0 <= move[0] + i < len(board) \
                and 0 <= move[1] + j < len(board) \
                and board[move[0] + i][move[1] + j] == opponent_color):
                    k = 1
                    while (0 <= move[0] + k * i < len(board) \
                    and 0 <= move[1] + k * j < len(board) \
                    and board[move[0] + k * i][move[1] + k * j] == opponent_color):
                        updated_board[move[0] + k * i][move[1] + k * j] = my_color
                        k += 1
                    if (0 <= move[0] + k * i < len(board) \
                    and 0 <= move[1] + k * j < len(board) \
                    and board[move[0] + k * i][move[1] + k * j] == my_color):
                        board = copy.deepcopy(updated_board)
                    else:
                        updated_board = copy.deepcopy(board)

        return board

    def end_game(self, board):
        if (len(self.find_valid_moves(self.my_color, self.opponent_color, board)) == 0 \
                    and len(self.find_valid_moves(self.opponent_color, self.my_color, board)) == 0):
            return True

        else:
            return False

    def get_result(self, board):
        start = time.clock()
        my_score = 0
        opponent_score = 0

        for r, c in self.find_all_positions(board):
            if (board[r][c] == self.my_color):
                my_score += 1
            if (board[r][c] == self.opponent_color):
                opponent_score += 1

        return my_score-opponent_score

if __name__ == "__main__":
    player = MyPlayer(1, 0)
    print(player.move([ \
        [1, 1, 1, -1, -1, 1, 1, 1], \
        [-1, 1, 1, -1, 1, 0, -1, -1], \
        [-1, 1, 1, 1, 0, 1, 1, 1], \
        [-1, -1, 1, 0, 0, 0, -1, 1], \
        [-1, -1, 0, 0, 0, 1, 1, -1], \
        [1, 1, 0, 1, 1, 0, 1, 1], \
        [0, 0, 1, -1,-1, 1, 0, -1], \
        [-1, 1, -1, -1, -1, 1, 1, 1]]))
