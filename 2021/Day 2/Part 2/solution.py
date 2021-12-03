file = open("../input.txt", "r")

position = 0
depth = 0
aim = 0
for line in file:
    items = line.split()
    direction = items[0]
    steps = int(items[1])
    if direction == 'forward':
        position += steps
        depth += aim * steps
    elif direction == 'down':
        aim += steps
    else:
        aim -= steps

print(position * depth)