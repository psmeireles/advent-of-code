import sys
import heapq
file = open("../input.txt", "r")

board = []
for line in file:
    board.append([int(x) for x in line.strip()])

g = {}
for x in range(len(board)):
    for y in range(len(board[x])):
        neighbors = set()

        # Up
        if x - 1 >= 0:
            neighbors.add((x - 1, y))
        # Down
        if x + 1 < len(board):
            neighbors.add((x + 1, y))
        # Left
        if y - 1 >= 0:
            neighbors.add((x, y - 1))
        # Right
        if y + 1 < len(board[x]):
            neighbors.add((x, y + 1))
        g[(x, y)] = neighbors


distances = [[sys.maxsize for j in range(len(board[0]))] for i in range(len(board))]
distances[0][0] = 0
visited = set()
queue = [(0, 0)]
heapq.heappush(queue := [], (0, (0,0)))
while(queue):
    x, y = heapq.heappop(queue)[1]
    visited.add((x, y))
    for i, j in g[(x, y)]:
        if (i, j) not in visited:
            newDistance = distances[x][y] + board[i][j]
            if newDistance < distances[i][j]:
                distances[i][j] = newDistance
                heapq.heappush(queue, (newDistance, (i, j)))

print(distances[-1][-1])