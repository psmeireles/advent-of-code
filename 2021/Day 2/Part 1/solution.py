file = open("../input.txt", "r")

position = 0
depth = 0
for line in file:
    items = line.split()
    direction = items[0]
    steps = int(items[1])
    if direction == 'forward':
        position += steps
    elif direction == 'down':
        depth += steps
    else:
        depth -= steps

print(position * depth)