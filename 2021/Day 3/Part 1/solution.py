file = open("../input.txt", "r")

input = []
for line in file:
    input.append([char for char in line])

counts = [{} for i in range(len(input[0]) - 1)]
for line in input:
    for i in range(len(line) - 1):
        counts[i][line[i]] = counts[i].get(line[i], 0) + 1

gamma = [max(dict, key=dict.get) for dict in counts]
epsilon = ['0' if x == '1' else '1' for x in gamma]

gamma = int(''.join(gamma), 2)
epsilon = int(''.join(epsilon), 2)

print(gamma * epsilon)