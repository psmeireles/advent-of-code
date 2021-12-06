file = open("../input.txt", "r")

fish = []
for line in file:
    fish += [int(x) for x in line.split(',')]


for day in range(80):
    newFish = 0
    for i in range(len(fish)):
        if fish[i] == 0:
            newFish += 1
            fish[i] = 6
        else:
            fish[i] -= 1
    fish += [8] * newFish

print(len(fish))