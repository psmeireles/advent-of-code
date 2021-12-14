file = open("../input.txt", "r")

template = ''
rules = {}
for line in file:
    if '-' in line:
        words = line.split()
        rules[words[0]] = words[-1]
    elif len(line) > 1:
        template = line.strip()

for _ in range(10):
    newTemplate = ''
    for i in range(1, len(template)):
        firstLetter = template[i-1]
        secondLetter = template[i]
        newTemplate += firstLetter + rules[firstLetter + secondLetter]
    newTemplate += template[-1]
    template = newTemplate
    
counts = {}
for c in template:
    counts[c] = counts.get(c, 0) + 1

print(max(counts.values()) - min(counts.values()))
