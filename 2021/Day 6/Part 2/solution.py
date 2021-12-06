file = open("../input.txt", "r")

fish = []
for line in file:
    fish += [int(x) for x in line.split(',')]

dict = {}
for n in fish:
    dict[n] = dict.get(n, 0) + 1


for day in range(256):
    newDict = {}
    newDict[0] = dict.get(1, 0)
    newDict[1] = dict.get(2, 0)
    newDict[2] = dict.get(3, 0)
    newDict[3] = dict.get(4, 0)
    newDict[4] = dict.get(5, 0)
    newDict[5] = dict.get(6, 0)
    newDict[6] = dict.get(7, 0) + dict.get(0, 0)
    newDict[7] = dict.get(8, 0)
    newDict[8] = dict.get(0, 0)

    dict = newDict

s = 0
for value in dict.values():
    s += value
print(s)