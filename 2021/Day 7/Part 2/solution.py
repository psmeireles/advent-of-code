file = open("../input.txt", "r")

positions = []
for line in file:
    positions += [int(x) for x in line.split(',')]

avg = round(sum(positions)/len(positions))


# Check for rounding up or down and pick min value
fuel1 = 0
for n in positions:
    distance = abs(n - avg)
    fuel1 += distance * (distance + 1) // 2

fuel2 = 0
for n in positions:
    distance = abs(n - (avg-1))
    fuel2 += distance * (distance + 1) // 2
print(min(fuel1, fuel2))