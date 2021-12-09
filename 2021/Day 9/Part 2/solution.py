file = open("../input.txt", "r")

input = []
for line in file:
    input.append([int(c) for c in line.split()[0]])

lowPoints = []
for i in range(len(input)):
    for j in range(len(input[0])):
        number = input[i][j]
        
        # Up
        if i - 1 >= 0 and input[i - 1][j] <= number:
            continue
        # Down
        if i + 1 < len(input) and input[i + 1][j] <= number:
            continue
        # Left
        if j - 1 >= 0 and input[i][j - 1] <= number:
            continue
        # Right
        if j + 1 < len(input[0]) and input[i][j + 1] <= number:
            continue

        lowPoints.append([i, j])

basinSizes = []
for i, j in lowPoints:
    visited = set()
    visited.add((i, j))
    queue = [(i, j)]
    while queue:
        x, y = queue.pop(0)
        number = input[x][y]
        # Up
        if x - 1 >= 0 and input[x - 1][y] > number and input[x - 1][y] < 9 and (x - 1, y) not in visited:
            queue.append((x - 1, y))
            visited.add((x - 1, y))
        # Down
        if x + 1 < len(input) and input[x + 1][y] > number and input[x + 1][y] < 9 and (x + 1, y) not in visited:
            queue.append((x + 1, y))
            visited.add((x + 1, y))
        # Left
        if y - 1 >= 0 and input[x][y - 1] > number and input[x][y - 1] < 9 and (x, y - 1) not in visited:
            queue.append((x, y - 1))
            visited.add((x, y - 1))
        # Right
        if y + 1 < len(input[0]) and input[x][y + 1] > number and input[x][y + 1] < 9 and (x, y + 1) not in visited:
            queue.append((x, y + 1))
            visited.add((x, y + 1))
    basinSizes.append(len(visited))

basinSizes.sort()
print(basinSizes[-1] * basinSizes[-2] * basinSizes[-3])