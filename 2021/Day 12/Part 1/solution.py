file = open("../input.txt", "r")

g = {}
for line in file:
    cave1, cave2 = line.strip().split('-')
    if cave1 not in g:
        g[cave1] = set()
    if cave2 not in g:
        g[cave2] = set()
    g[cave1].add(cave2)
    g[cave2].add(cave1)

def dfs(node, visited):
    if node == 'end':
        return 1
    if node == 'start' and visited:
        return 0
    if node.islower() and node in visited:
        return 0
    visited = visited | {node}
    result = 0
    for neighbor in g[node]:
        result += dfs(neighbor, visited)
    return result


print(dfs('start', set()))