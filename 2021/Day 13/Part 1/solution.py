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

dotsAfterFold = set()
for x, y in dots:
    if folds[0][0] == 'x':
        if x < folds[0][1]:
            dotsAfterFold.add((x, y))
        else:
            newX = x - 2 * (x - folds[0][1])
            if newX >= 0:
                dotsAfterFold.add((newX, y))
    else:
        if y < folds[0][1]:
            dotsAfterFold.add((x, y))
        else:
            newY = y - 2 * (y - folds[0][1])
            if newY >= 0:
                dotsAfterFold.add((x, newY))

print(len(dotsAfterFold))