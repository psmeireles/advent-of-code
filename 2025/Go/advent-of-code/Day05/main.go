package main

import (
	"bufio"
	"os"
	"sort"
	"strconv"
	"strings"
)

func readFile() ([][2]int, []int, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()

	var freshRanges [][2]int
	var ingredients []int

	parsingRanges := true
	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if len(line) > 0 {
			if parsingRanges {
				split := strings.Split(string(line), "-")
				start, err := strconv.Atoi(split[0])
				if err != nil {
					return nil, nil, err
				}
				end, err := strconv.Atoi(split[1])
				if err != nil {
					return nil, nil, err
				}

				freshRanges = append(freshRanges, [2]int{start, end})
			} else {
				num, err := strconv.Atoi(string(line))
				if err != nil {
					return nil, nil, err
				}
				ingredients = append(ingredients, num)
			}
		} else if err != nil {
			break
		} else if len(line) == 0 {
			parsingRanges = false
		}
	}
	return freshRanges, ingredients, nil
}

func main() {
	freshRanges, ingredients, err := readFile()
	if err != nil {
		panic(err)
	}

	// Part 1
	result1 := 0
	for _, ingredient := range ingredients {
		for _, freshRange := range freshRanges {
			if ingredient >= freshRange[0] && ingredient <= freshRange[1] {
				result1++
				break
			}
		}
	}

	// Output Part 1: 511
	println("Part 1:", result1)

	// Part 2
	result2 := 0

	// Sort freshRanges ascending by start value, then end value
	sort.Slice(freshRanges, func(i, j int) bool {
		return freshRanges[i][0] < freshRanges[j][0] || (freshRanges[i][0] == freshRanges[j][0] && freshRanges[i][1] < freshRanges[j][1])
	})

	// For each fresh range, add the number of new values it covers
	maxEnd := -1
	for _, freshRange := range freshRanges {
		start := freshRange[0]
		end := freshRange[1]

		newRangeStart := max(maxEnd+1, start)
		newRangeEnd := max(end, maxEnd)
		result2 += max(0, newRangeEnd-newRangeStart+1)
		maxEnd = newRangeEnd
	}

	// Output Part 2: 350939902751909
	println("Part 2:", result2)
}
