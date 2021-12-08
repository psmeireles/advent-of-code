'''
# segments -> possible digits
2 -> 1
3 -> 7
4 -> 4
5 -> 2, 3, 5
6 -> 0, 6, 9
7 -> 8
'''

file = open("../input.txt", "r")

count = 0
for line in file:
    output = line.split('|')[1].split()
    for word in output:
        size = len(word)
        if size < 5 or size == 7:
            count += 1
print(count)