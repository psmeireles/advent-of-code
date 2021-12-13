file = open("../input.txt", "r")

dots = []
folds = []
for line in file:
    if ',' in line:
        dot = [int(x) for x in line.strip().split(',')]
        dots.append(dot)
    elif '=' in line:
        segments = line.strip().split('=')
        axis = segments[0][-1]
        number = int(segments[1])
        folds.append([axis, number])

for fold in folds:
    dotsAfterFold = set()
    for x, y in dots:
        if fold[0] == 'x':
            if x < fold[1]:
                dotsAfterFold.add((x, y))
            else:
                newX = x - 2 * (x - fold[1])
                if newX >= 0:
                    dotsAfterFold.add((newX, y))
        else:
            if y < fold[1]:
                dotsAfterFold.add((x, y))
            else:
                newY = y - 2 * (y - fold[1])
                if newY >= 0:
                    dotsAfterFold.add((x, newY))
    dots = list(dotsAfterFold)

size = [0, 0]
for dot in dots:
    size = [max(size[0], dot[0]), max(size[1], dot[1])]

# Printing inverted coordinates so that we can read as normal human beings
for i in range(size[1] + 1):
    for j in range(size[0] + 1):
        if (j, i) in dotsAfterFold:
            print('#', end='')
        else:
            print('.', end='')
    print()