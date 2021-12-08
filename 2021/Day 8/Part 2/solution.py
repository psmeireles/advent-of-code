'''
# segments -> possible digits
2 -> 1
3 -> 7
4 -> 4
5 -> 2, 3, 5
6 -> 0, 6, 9
7 -> 8
'''

'''
segment -> count
a -> 8
b -> 6
c -> 8
d -> 7
e -> 4
f -> 9
g -> 7
'''

'''
count
b (count = 6)
e (count = 4)
f (count = 9)
c (digit that is not f in 1)
a (compare length 2 with legth 3, check diff digit)
d (digit that is not discovered in 4)
g (remaining digit)
'''

file = open("../input.txt", "r")

numbers = {
    'abcefg': 0,
    'cf': 1,
    'acdeg': 2,
    'acdfg': 3,
    'bcdf': 4,
    'abdfg': 5,
    'abdefg': 6,
    'acf': 7,
    'abcdefg': 8,
    'abcdfg': 9
}

input = []
output = []
for line in file:
    lineInput, lineOutput = line.split('|')
    input.append(lineInput.split())
    output.append(lineOutput.split())

results = []
for i in range(len(input)):
    countsDict = {}
    conversionDict = {}
    wordsByLengthDict = {}
    for word in input[i]:
        for c in word:
            countsDict[c] = countsDict.get(c, 0) + 1
        wordsByLengthDict[len(word)] = wordsByLengthDict.get(len(word), []) + [word]

    # Find e, b and f
    for key, value in countsDict.items():
        if value == 4:
            conversionDict[key] = 'e'
        elif value == 6:
            conversionDict[key] = 'b'
        elif value == 9:
            conversionDict[key] = 'f'
    
    # Find c
    for c in wordsByLengthDict[2][0]:
        if c not in conversionDict:
            conversionDict[c] = 'c'

    # Find a
    a = [c for c in wordsByLengthDict[3][0] if c not in wordsByLengthDict[2][0]][0]
    conversionDict[a] = 'a'

    # Find d
    for c in wordsByLengthDict[4][0]:
        if c not in conversionDict:
            conversionDict[c] = 'd'

    # Find g
    for c in wordsByLengthDict[7][0]:
        if c not in conversionDict:
            conversionDict[c] = 'g'

    # Convert output
    value = 0
    for j in range(len(output[i])):
        convertedString = ''.join(sorted([conversionDict[c] for c in output[i][j]]))
        value += numbers[convertedString] * 10 ** (3 - j)
    results.append(value)

print(sum(results))