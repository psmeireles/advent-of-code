package main

import (
	"bufio"
	"os"
)

func readInput() ([]string, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var grid []string
	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if err != nil {
			break
		}
		grid = append(grid, string(line))
	}
	return grid, nil
}

var memoizedResults = make(map[[2]int]int)

func countTimelines(grid []string, startI, startJ int) int {
	if startI < 0 || startI >= len(grid) || startJ < 0 || startJ >= len(grid[0]) {
		return 0
	}

	if val, exists := memoizedResults[[2]int{startI, startJ}]; exists {
		return val
	}

	i := startI
	j := startJ
	pos := [2]int{i, j}
	memoizedResults[pos] = 1

	for i < len(grid) {
		if grid[i][j] == '^' {
			memoizedResults[pos] = countTimelines(grid, i, j-1) + countTimelines(grid, i, j+1)
			break
		}
		i++
	}

	return memoizedResults[pos]
}

func main() {
	grid, err := readInput()
	if err != nil {
		println("Error reading input:", err)
		return
	}

	// Part 1

	// Find start position
	startI, startJ := -1, -1
	for i, line := range grid {
		for j, char := range line {
			if char == 'S' {
				startI, startJ = i, j
				break
			}
		}
	}

	// Create queue of positions to trace from and count when split
	queue := [][2]int{{startI, startJ}}
	mapSplit := make(map[[2]int]bool)
	mapVisited := make(map[[2]int]bool)
	splitCount := 0
	for len(queue) > 0 {
		pos := queue[0]
		queue = queue[1:]
		if pos[1] < 0 || pos[1] >= len(grid[0]) {
			continue
		}

		if mapVisited[pos] {
			continue
		}
		mapVisited[pos] = true

		for i := pos[0]; i < len(grid); i++ {
			newPos := [2]int{i, pos[1]}
			if grid[i][pos[1]] == '^' {
				if !mapSplit[newPos] {
					mapSplit[newPos] = true
					splitCount++
					queue = append(queue, [2]int{i, pos[1] - 1})
					queue = append(queue, [2]int{i, pos[1] + 1})
				}
				break
			}
		}
	}

	// Output Part 1: 1656
	println("Part 1:", splitCount)

	// Part 2
	result2 := countTimelines(grid, startI, startJ)

	// Output Part 2: 76624086587804
	println("Part 2:", result2)
}
