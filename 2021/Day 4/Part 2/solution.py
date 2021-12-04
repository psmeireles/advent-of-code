from utils import *

numbers, tables = readFile()
dicts = initializeDicts(tables)

tableSize = len(tables[0])
numberOfTables = len(tables)
markedNumbersByRow = [[0]*tableSize for table in tables]
markedNumbersByColumn = [[0]*tableSize for table in tables]
markedNumbersInTable = [set() for table in tables]

completedTables = set()
for n in numbers:
    for tableIndex in range(numberOfTables):
        row, column = dicts[tableIndex].get(n, [-1, -1])
        if row == -1 or column == -1:
            continue

        markedNumbersInTable[tableIndex].add(n)
        markedNumbersByRow[tableIndex][row] += 1
        markedNumbersByColumn[tableIndex][column] += 1
        if markedNumbersByColumn[tableIndex][column] == tableSize or markedNumbersByRow[tableIndex][row] == tableSize:
            completedTables.add(tableIndex)
            if len(completedTables) == numberOfTables:
                print(sumUnmarkedNumbers(tables[tableIndex], markedNumbersInTable[tableIndex]) * n)
                exit()


