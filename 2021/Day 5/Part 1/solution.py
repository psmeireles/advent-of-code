file = open("../input.txt", "r")

input = []
for line in file:
    points = line.split(' -> ')
    arr = []
    for point in points:
        coordinates = [int(x) for x in point.split(',')]
        arr += [coordinates]
    # Horizontal or vertical
    if arr[0][0] == arr[1][0] or arr[0][1] == arr[1][1]:
        # Make sure positions will be sorted from lowest to highest
        input.append(
            [
                [min(arr[0][0], arr[1][0]), min(arr[0][1], arr[1][1])],
                [max(arr[0][0], arr[1][0]), max(arr[0][1], arr[1][1])]
            ]
        )

board = [[0 for i in range(1000)] for j in range(1000)]

for row in input:
    if row[0][0] == row[1][0]:
        i = row[0][0]
        for j in range(row[0][1], row[1][1] + 1):
            board[i][j] += 1
    elif row[0][1] == row[1][1]:
        j = row[1][1]
        for i in range(row[0][0], row[1][0] + 1):
            board[i][j] += 1

s = 0
for i in range(len(board)):
    for j in range(len(board[0])):
        if board[i][j] > 1:
            s += 1

print(s)
