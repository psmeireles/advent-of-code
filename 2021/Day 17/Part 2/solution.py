file = open("../input.txt", "r")

target = []
for line in file:
    xRange, yRange = line.split(':')[1].strip().split(',')
    x = [int(s) for s in xRange.split('=')[1].split('..')]
    y = [int(s) for s in yRange.split('=')[1].split('..')]
    target.append(x)
    target.append(y)

count = 0
for vx in range(target[0][1]+1):
    for vy in range(target[1][0], -target[1][0]+1):
        t = 0
        x = 0
        y = 0
        while x <= target[0][1] and y >= target[1][0]:
            t += 1
            newVx = vx - (t-1) if vx - (t-1) >= 0 else 0
            x = x + newVx
            newVy = vy - (t-1)
            y = y + newVy
            if x <= target[0][1] and x >= target[0][0] and y <= target[1][1] and y >= target[1][0]:
                count += 1
                break
print(count)