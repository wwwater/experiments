import random
import time

################################################################
## basic operations
################################################################
def create_new_board():
    return [[0, 0, 0]]*3


def available_moves(board):
    return [(row, col) for row in range(3)
                        for col in range(3)
                        if board[row][col] == 0]


def player_sign(player):
    return "X" if player == 1 else "0" if player == 2 else "."


def print_board(board):
    for row in board:
        print(" ".join(map(player_sign, row)))


def make_move(board, player, move):
    new_board = [row[:] for row in board]
    new_board[move[0]][move[1]] = player
    return new_board


def valid_move(board, move):
    if 0 <= move[0] < 3 and 0 <= move[1] < 3:
        if board[move[0]][move[1]] == 0:
            return True
    return False


def toggle_player(player):
    #return 3 - player
    #return player ^ 3
    return 2**(player % 2)


def if_draw(board):
    return not available_moves(board)


def if_won(board, player):
    win = [player]*3
    for row in board:
        if row == win:
            return True
    for col in range(3):
        if [board[0][col], board[1][col], board[2][col]] == win:
            return True
    if [board[0][0], board[1][1], board[2][2]] == win:
        return True
    if [board[2][0], board[1][1], board[0][2]] == win:
        return True
    return False


def read_line(board, line):
    return [board[line[0][0]][line[0][1]],
            board[line[1][0]][line[1][1]],
            board[line[2][0]][line[2][1]]]


def lines():
    for row in range(3):
        yield [(row, 0), (row, 1), (row, 2)]
    for col in range(3):
        yield [(0, col), (1, col), (2, col)]
    yield [(0, 0), (1, 1), (2, 2)]
    yield [(0, 2), (1, 1), (2, 0)]


################################################################
## players and strategies
################################################################
def human(board, player):
    return input("Player "+ player_sign(player) + " makes a move: ")


def ai_rand(board, player):
    free_cells = available_moves(board)
    move = random.choice(free_cells)
    return move


def ai_Johnny(board, player):
    two_free = []
    for line in lines():
        board_line = read_line(board,line)
        s = sorted(board_line)
        if s[0] == 0 and s[1] == s[2] == player:
            for i in range(3):
                if board_line[i] == 0:
                    return line[i]
    free_cells = available_moves(board)
    move = random.choice(free_cells)
    return move


def utility(board, player):
    if if_won(board, player):
        return (1.0, 1)
    elif if_draw(board):
        return (0.5, 1)
    max_other_utility = -1.0
    best_other_steps = None
    other = toggle_player(player)
    for move in available_moves(board):
        (other_utility, steps) = utility(make_move(board, other, move), other)
        if other_utility > max_other_utility:
            max_other_utility = other_utility
            best_other_steps = steps
        elif other_utility == max_other_utility:
            if other_utility < 0.5:
                best_other_steps = max(best_other_steps, steps)
            elif other_utility > 0.5:
                best_other_steps = min(best_other_steps, steps)
    return (1 - max_other_utility, best_other_steps + 1)


def best_move(board, player):
    max_utility = -1.0
    my_best_move = None
    my_best_steps = None
    moves = available_moves(board)
    random.shuffle(moves)
    for move in moves:
        (my_utility, steps) = utility(make_move(board, player, move), player)
        #print(str(move) + str(my_utility) + " " + str(steps))
        if my_utility > max_utility:
            my_best_move = move
            my_best_steps = steps
            max_utility = my_utility
        elif my_utility == max_utility:
            if my_utility < 0.5:
                if steps > my_best_steps:
                    my_best_move = move
                    my_best_steps = steps
            elif my_utility > 0.5:
                if steps < my_best_steps:
                    my_best_steps = steps
                    my_best_move = move
            
    return my_best_move
    

def ai_genius(board, player):
    return best_move(board, player)

################################################################
##high-level operations
################################################################
def decorate_ai(ai):
    def decorated_ai(board, player):
        time.sleep(random.uniform(0.1, 0.2))
        move = ai(board, player)
        print("Player " + str(ai.__name__) + " make a move as "\
            + player_sign(player) + ": " \
            + str(move[0]) + ", " + str(move[1]))
        return move
    decorated_ai.__name__ = ai.__name__    
    return decorated_ai


def play_game(player1, player2, p):
    if player1 == human or player2 == human:
        p = 1
    if p and player1 != human:
        player1 = decorate_ai(player1)
    if p and player2 != human:
        player2 = decorate_ai(player2)
    board = create_new_board()
    if p: print_board(board)
    player_id = 1
    while True:
        if player_id == 1:
            player = player1
        else:
            player = player2
        move = player(board, player_id)
        if not valid_move(board, move):
            if p: print("Your move is not valid!")
            continue
        board = make_move(board, player_id, move)
        if p: print_board(board)
        if if_won(board, player_id):
            if p: print("Player " + player.__name__ + " has won!")
            return player_id
        if if_draw(board):
            if p: print("It is a draw! Everybody wins!")
            return 0
        player_id = toggle_player(player_id)

   
def tournament(player1, player2, n, p):
    other = [0, 2, 1]
    results = {0: 0, 1: 0, 2: 0}
    for _ in range(n):
        results[play_game(player1, player2, p)] += 1
        results[other[play_game(player2, player1, p)]] += 1
    print("Draws: " + str(results[0]))
    print(player1.__name__ + ": " + str(results[1]))
    print(player2.__name__ + ": " + str(results[2]))
    return results


################################################################
## testing
################################################################
def test():
    board = [[1, 1, 1], [2, 0, 2], [1, 0, 2]]
    assert (make_move(board, 1, (1, 1)) == [[1, 1, 1], [2, 1, 2], [1, 0, 2]])
    assert (board != make_move(board, 1, (1, 1)))
    assert (if_won(board, 1) is True)
    print(available_moves(board))
    assert (available_moves(board) == [(1, 1), (2, 1)])
    print_board(board)


play_game(ai_genius, ai_Johnny, 1)
#tournament(ai_Johnny, ai_genius, 10, 0)
#tournament(ai_Johnny, ai_rand, 100, 0)
