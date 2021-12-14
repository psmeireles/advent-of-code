file = open("../input.txt", "r")

template = ''
rules = {}
for line in file:
    if '-' in line:
        words = line.split()
        rules[words[0]] = words[-1]
    elif len(line) > 1:
        template = line.strip()

letterCounts = {}
for c in template:
    letterCounts[c] = letterCounts.get(c, 0) + 1

pairCounts = {}
for i in range(1, len(template)):
    firstLetter = template[i-1]
    secondLetter = template[i]
    pair = firstLetter + secondLetter
    pairCounts[pair] = pairCounts.get(pair, 0) + 1


for _ in range(40):
    newPairCounts = {}
    for key, value in pairCounts.items():
        firstLetter = key[0]
        secondLetter = key[1]
        middleLetter = rules[firstLetter + secondLetter]
        letterCounts[middleLetter] = letterCounts.get(middleLetter, 0) + value

        pair1 = firstLetter + middleLetter
        pair2 = middleLetter + secondLetter
        newPairCounts[pair1] = newPairCounts.get(pair1, 0) + value
        newPairCounts[pair2] = newPairCounts.get(pair2, 0) + value
    pairCounts = newPairCounts

print(max(letterCounts.values()) - min(letterCounts.values()))