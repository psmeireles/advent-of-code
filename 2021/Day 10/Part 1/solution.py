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

scores = {
    ')': 3,
    ']': 57,
    '}': 1197,
    '>': 25137
}

result = 0
for string in input:
    stack = []
    for c in string:
        if c in closing:
            stack.append(c)
        else:
            if c != closing[stack.pop()]:
                result += scores[c]
                break



print(result)