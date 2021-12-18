import ast
import math

def flatten(list, level, result):
    for item in list:
        if type(item) is int:
            result.append([item, level])
        else:
            flatten(item, level + 1, result)

def explode(arr, i):
    newArr = arr.copy()
    if i > 0:
        newArr[i-1][0] += newArr[i][0]
    if i + 1 < len(arr) - 1:
        newArr[i+2][0] += newArr[i+1][0]
    newArr.pop(i+1)
    newArr[i] = [0, newArr[i][1] - 1]
    return newArr

def split(arr, i):
    newArr = arr.copy()
    value1, value2, newDepth = arr[i][0] // 2, math.ceil(arr[i][0] / 2), arr[i][1] + 1
    newArr.pop(i)
    newArr.insert(i, [value2, newDepth])
    newArr.insert(i, [value1, newDepth])
    return newArr

def getReduceAction(flattenedList):
    for i in range(len(flattenedList)):
        if flattenedList[i][1] > 4 and flattenedList[i+1][1] == flattenedList[i][1]:
            return "explode", i
    for i in range(len(flattenedList)):
        if flattenedList[i][0] > 9:
            return "split", i
    return None, None

def reduce(flattened):
    while True:
        action, index = getReduceAction(flattened)
        if not action:
            return flattened
        newList = []
        if action == 'explode':
            newList = explode(flattened, index)
        else:
            newList = split(flattened, index)
        flattened = newList

def add(arr1, arr2):
    newArr = []
    for value, depth in arr1:
        newArr.append([value, depth + 1])
    for value, depth in arr2:
        newArr.append([value, depth + 1])
    return newArr

def magnitude(arr):
    while len(arr) > 1:
        for i in range(len(arr) - 1):
            if arr[i][1] == arr[i+1][1]:
                newMagnitude = 3 * arr[i][0] + 2 * arr[i+1][0]
                arr[i] = [newMagnitude, arr[i][1] - 1]
                arr.pop(i+1)
                break
    return arr[0][0]


file = open("../input.txt", "r")

input = []
for line in file:
    input.append(ast.literal_eval(line))

flattened = []
flatten(input[0], 1, flattened)
addition = flattened
for number in input[1:]:
    flattenedNumber = [] 
    flatten(number, 1, flattenedNumber)
    addition = add(addition, flattenedNumber)
    addition = reduce(addition)

print(magnitude(addition))