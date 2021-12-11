file = open("../input.txt", "r")

input = []
for line in file:
    input.append([int(c) for c in line.split()[0]])

def propagate(board, x, y):
    # Up
    if x - 1 >= 0 and board[x - 1][y] <= 9:
        board[x - 1][y] += 1
        if board[x - 1][y] == 10:
            propagate(board, x - 1, y)
    # Down
    if x + 1 < len(board) and board[x + 1][y] <= 9:
        board[x + 1][y] += 1
        if board[x + 1][y] == 10:
            propagate(board, x + 1, y)
    # Left
    if y - 1 >= 0 and board[x][y - 1] <= 9:
        board[x][y - 1] += 1
        if board[x][y - 1] == 10:
            propagate(board, x, y - 1)
    # Right
    if y + 1 < len(board[x]) and board[x][y + 1] <= 9:
        board[x][y + 1] += 1
        if board[x][y + 1] == 10:
            propagate(board, x, y + 1)
    # Up-Left
    if x - 1 >= 0 and y - 1 >= 0 and board[x - 1][y - 1] <= 9:
        board[x - 1][y - 1] += 1
        if board[x - 1][y - 1] == 10:
            propagate(board, x - 1, y - 1)
    # Up-Right
    if x - 1 >= 0 and y + 1 < len(board[x]) and board[x - 1][y + 1] <= 9:
        board[x - 1][y + 1] += 1
        if board[x - 1][y + 1] == 10:
            propagate(board, x - 1, y + 1)
    # Bottom-Left
    if x + 1 < len(board) and y - 1 >= 0 and board[x + 1][y - 1] <= 9:
        board[x + 1][y - 1] += 1
        if board[x + 1][y - 1] == 10:
            propagate(board, x + 1, y - 1)
    # Bottom-Right
    if x + 1 < len(board) and y + 1 < len(board[x]) and board[x + 1][y + 1] <= 9:
        board[x + 1][y + 1] += 1
        if board[x + 1][y + 1] == 10:
            propagate(board, x + 1, y + 1)

flashes = 0
steps = 100
for step in range(steps):
    for i in range(len(input)):
        for j in range(len(input[i])):
            input[i][j] += 1
            if input[i][j] == 10:
                propagate(input, i, j)
    
    for i in range(len(input)):
        for j in range(len(input[i])):
            if input[i][j] > 9:
                input[i][j] = 0
                flashes += 1

print(flashes)