file = open("../input.txt", "r")

hex = ''
for line in file:
    hex = line.strip()

versionSum = 0
def parsePacket(number, depth):
    version, packetType = number[:3], number[3:6]
    global versionSum
    versionSum += int(version, base=2)
    
    if packetType == '100': # Literal
        literal = ''
        i = 6
        while number[i] != '0':
            literal += number[i + 1: i + 5]
            i += 5
        literal += number[i + 1: i + 5]
        return i + 5
    else:
        lengthType = number[6]
        if lengthType == '0':
            numberOfBits = int(number[7:22], base=2)
            start = 22
            while start - 22 < numberOfBits:
                start += parsePacket(number[start:], depth + 1)
            return start
        else:
            numberOfPackets = int(number[7:18], base=2)
            start = 18
            for i in range(numberOfPackets):
                start += parsePacket(number[start:], depth + 1)
            return start



    
int_value = int(hex, base=16)
binary = str(bin(int_value))[2:].zfill(4*len(hex))
parsePacket(binary, 0)
print(versionSum)