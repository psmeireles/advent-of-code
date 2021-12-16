import math

file = open("../input.txt", "r")

hex = ''
for line in file:
    hex = line.strip()

def parsePacket(number, depth):
    _, packetType = int(number[:3], base=2), int(number[3:6], base=2)
    
    if packetType == 4: # Literal
        literal = ''
        i = 6
        while number[i] != '0':
            literal += number[i + 1: i + 5]
            i += 5
        literal += number[i + 1: i + 5]
        return int(literal, base=2), i + 5
    else:
        lengthType = number[6]
        if lengthType == '0':
            numberOfBits = int(number[7:22], base=2)
            start = 22
            numbers = []
            while start - 22 < numberOfBits:
                literal, newStart = parsePacket(number[start:], depth + 1)
                numbers.append(literal)
                start += newStart
            return operate(packetType, numbers), start
        else:
            numberOfPackets = int(number[7:18], base=2)
            start = 18
            numbers = []
            for i in range(numberOfPackets):
                literal, newStart = parsePacket(number[start:], depth + 1)
                numbers.append(literal)
                start += newStart
            return operate(packetType, numbers), start

def operate(type, numbers):
    if type == 0:
        return sum(numbers)
    if type == 1:
        return math.prod(numbers)
    if type == 2:
        return min(numbers)
    if type == 3:
        return max(numbers)
    if type == 5:
        return 1 if numbers[0] > numbers[1] else 0
    if type == 6:
        return 1 if numbers[0] < numbers[1] else 0
    if type == 7:
        return 1 if numbers[0] == numbers[1] else 0
   
int_value = int(hex, base=16)
binary = str(bin(int_value))[2:].zfill(4*len(hex))
value, _ = parsePacket(binary, 0)
print(value)