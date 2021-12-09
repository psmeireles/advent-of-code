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

        lowPoints.append(number)

print(sum(lowPoints) + len(lowPoints))