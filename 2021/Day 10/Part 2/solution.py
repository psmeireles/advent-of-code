file = open("../input.txt", "r")

input = []
for line in file:
    input.append(line.strip())

closing = {
    '(': ')',
    '[': ']',
    '{': '}',
    '<': '>'
}

values = {
    ')': 1,
    ']': 2,
    '}': 3,
    '>': 4
}

scores = []
for string in input:
    stack = []
    isIncorrect = False
    for c in string:
        if c in closing:
            stack.append(c)
        else:
            if c != closing[stack.pop()]:
                isIncorrect = True
                break

    if isIncorrect:
        continue

    score = 0
    while stack:
        c = stack.pop()
        score *= 5
        score += values[closing[c]]
    scores.append(score)

print(sorted(scores)[len(scores)//2])