file = open("../input.txt", "r")

algorithm = []
input = []
for line in file:
    if len(line) == 513:
        algorithm += [c for c in line.strip()]
    elif len(line) > 1:
        input.append([c for c in line.strip()])

def extendImage(img, value='.'):
    extendedImage = []
    extendedImage.append([value for _ in range(len(img[0])+2)])
    for row in img:
        extendedImage.append([value] + row + [value])
    extendedImage.append([value for _ in range(len(img[0])+2)])
    return extendedImage

def symbolToInt(symbol):
    return 0 if symbol == '.' else 1

def enhanceImage(img, infiniteValue='.'):
    newImage = []
    for i in range(len(img)):
        newRow = []
        for j in range(len(img[i])):
            binaryPixel = []
            
            # up-left
            binaryPixel.append(img[i-1][j-1] if i-1 > 0 and j-1 > 0 else infiniteValue)
            # up
            binaryPixel.append(img[i-1][j] if i-1 > 0 else infiniteValue)
            # up-right
            binaryPixel.append(img[i-1][j+1] if i-1 > 0 and j+1 < len(img[i]) else infiniteValue)
            # left
            binaryPixel.append(img[i][j-1] if j-1 > 0 else infiniteValue)
            # center
            binaryPixel.append(img[i][j])
            # right
            binaryPixel.append(img[i][j+1] if j+1 < len(img[i]) else infiniteValue)
            # down-left
            binaryPixel.append(img[i+1][j-1] if i+1 < len(img) and j-1 > 0 else infiniteValue)
            # down
            binaryPixel.append(img[i+1][j] if i+1 < len(img) else infiniteValue)
            # down-right
            binaryPixel.append(img[i+1][j+1] if i+1 < len(img) and j+1 < len(img[i]) else infiniteValue)

            
            index = int("".join(str(x) for x in [symbolToInt(symbol) for symbol in binaryPixel]), 2)
            newRow.append(algorithm[index])
        newImage.append(newRow)
    return newImage


image = extendImage(input)
infiniteValue = '.'
for _ in range(2):
    image = enhanceImage(image, infiniteValue)
    if algorithm[0] == '#' and infiniteValue == '.':
        infiniteValue = '#'
    elif algorithm[511] == '.' and infiniteValue == '#':
        infiniteValue = '.'
    image = extendImage(image, infiniteValue)

lit = 0
for row in image:
    for c in row:
        if c == '#':
            lit += 1
print(lit)
