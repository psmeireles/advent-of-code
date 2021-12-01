file = open("../input.txt", "r")

input = []
for line in file:
    input.append(int(line))

value = sum(input[:3])
result = 0
for i in range(4, len(input)):
    newValue = value + input[i] - input[i-3]
    if newValue > value:
        result += 1
    value = newValue
print(result)