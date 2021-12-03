file = open("../input.txt", "r")

input = []
for line in file:
    input.append([char for char in line])

o2 = input
i = 0
while len(o2) > 1 and i < len(input[0]):
    relevantDigits = [int(x[i]) for x in o2]
    avg = sum(relevantDigits)/len(relevantDigits)
    mostFrequent = '1' if avg >= 0.5 else '0'
    o2 = [x for x in o2 if x[i] == mostFrequent]
    i += 1
numberO2 = int(''.join(o2[0]).strip(), 2)


co2 = input
i = 0
while len(co2) > 1 and i < len(input[0]):
    relevantDigits = [int(x[i]) for x in co2]
    avg = sum(relevantDigits)/len(relevantDigits)
    lessFrequent = '1' if avg < 0.5 else '0'
    co2 = [x for x in co2 if x[i] == lessFrequent]
    i += 1
numberCO2 = int(''.join(co2[0]).strip(), 2)

print(numberCO2 * numberO2)