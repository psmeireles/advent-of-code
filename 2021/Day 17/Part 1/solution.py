file = open("../input.txt", "r")

target = []
for line in file:
    xRange, yRange = line.split(':')[1].strip().split(',')
    x = [int(s) for s in xRange.split('=')[1].split('..')]
    y = [int(s) for s in yRange.split('=')[1].split('..')]
    target.append(x)
    target.append(y)

print(target[1][0]*(target[1][0]+1)//2)