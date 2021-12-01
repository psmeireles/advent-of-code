file = open("../input.txt", "r")

input = []
for line in file:
    input.append(int(line))

result = 0
for i in range(1, len(input)):
    if input[i] > input[i-1]:
        result += 1
print(result)