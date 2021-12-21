file = open("../input.txt", "r")


positions = []
for line in file:
    positions.append(int(line.split(': ')[1]))

def getNextPosition(currentPosition, diceRoll):
    newPosition = (currentPosition + diceRoll) % 10
    if newPosition == 0:
        newPosition = 10
    return newPosition

wins = [0, 0]
scores = [0, 0]
numbersRolled = 0
currentPlayer = 0
while scores[0] < 1000 and scores[1] < 1000:
    rolledValue = numbersRolled * 3 + 6
    positions[currentPlayer] = getNextPosition(positions[currentPlayer], rolledValue)
    scores[currentPlayer] += positions[currentPlayer]
    currentPlayer = (currentPlayer + 1) % 2
    numbersRolled += 3
print(min(scores) * numbersRolled)