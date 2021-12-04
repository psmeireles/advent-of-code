# Reads file and returns list of numbers and list of tables
def readFile():
    file = open("../input.txt", "r")

    numbers = [int(n) for n in file.readline().split(',')]

    tables = []
    while file.readline():
        table = []
        for i in range(5):
            line = file.readline().strip().split()
            table += [[int(x) for x in line]]
        tables += [table]
    return numbers, tables

# Returns list of dicts with indexes of each number in table
def initializeDicts(tables):
    dicts = [{} for table in tables]
    numberOfTables = len(tables)
    tableSize = len(tables[0])

    for n in range(numberOfTables):
        table = tables[n]
        for i in range(tableSize):
            for j in range(tableSize):
                number = table[i][j]
                dicts[n][number] = [i, j]
    return dicts