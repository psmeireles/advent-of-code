file = open("../input.txt", "r")

positions = []
for line in file:
    positions += [int(x) for x in line.split(',')]

positions.sort()
median = positions[len(positions)//2]

fuel = 0
for n in positions:
    fuel += abs(n - median)
print(fuel)