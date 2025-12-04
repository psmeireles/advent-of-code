package main

import (
	"bufio"
	"os"
)

func readFile() ([]string, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var lines []string
	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if len(line) > 0 {
			lines = append(lines, string(line))
		} else if err != nil {
			break
		}
	}
	return lines, nil
}

func getNeighbors(x, y int, grid []string) [][2]int {
	var neighbors [][2]int
	directions := [][2]int{
		{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1}, {0, 1},
		{1, -1}, {1, 0}, {1, 1},
	}

	for _, dir := range directions {
		nx, ny := x+dir[0], y+dir[1]
		if nx >= 0 && ny >= 0 && nx < len(grid) && ny < len(grid[0]) && grid[nx][ny] == '@' {
			neighbors = append(neighbors, [2]int{nx, ny})
		}
	}
	return neighbors
}

func main() {
	grid, err := readFile()
	if err != nil {
		panic(err)
	}

	// Part 1
	result1 := 0
	for i := 0; i < len(grid); i++ {
		for j := 0; j < len(grid[0]); j++ {
			if grid[i][j] == '@' {
				neighbors := getNeighbors(i, j, grid)
				if len(neighbors) < 4 {
					result1++
				}
			}
		}
	}

	// Output Part 1: 1553
	println("Part 1:", result1)

	// Part 2
	result2 := 0
	removedThisRun := false
	for removedThisRun || result2 == 0 {
		removedThisRun = false
		for i := 0; i < len(grid); i++ {
			for j := 0; j < len(grid[0]); j++ {
				if grid[i][j] == '@' {
					neighbors := getNeighbors(i, j, grid)
					if len(neighbors) < 4 {
						grid[i] = grid[i][:j] + "." + grid[i][j+1:]
						removedThisRun = true
						result2++
					}
				}
			}
		}
	}

	// Output Part 2:
	println("Part 2:", result2)

}
