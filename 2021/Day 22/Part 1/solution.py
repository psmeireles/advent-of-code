file = open("../input.txt", "r")

steps = []
for line in file:
    command, ranges = line.split()
    xRange, yRange, zRange = ranges.split(',')
    steps.append([command, [int(x) for x in xRange.split('=')[1].split('..')], [int(y) for y in yRange.split('=')[1].split('..')], [int(z) for z in zRange.split('=')[1].split('..')]])

def isValidRegion(xRange, yRange, zRange):
    return xRange[0] > -50 and xRange[1] < 50 and yRange[0] > -50 and yRange[1] < 50 and zRange[0] > -50 and zRange[1] < 50

grid = {}
for command, xRange, yRange, zRange in steps:
    if not isValidRegion(xRange, yRange, zRange):
        continue
    for x in range(xRange[0], xRange[1] + 1):
        for y in range(yRange[0], yRange[1] + 1):
            for z in range(zRange[0], zRange[1] + 1):
                grid[x, y, z] = command

print(len([command for command in grid.values() if command == 'on']))