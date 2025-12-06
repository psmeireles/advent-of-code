package main

import (
	"bufio"
	"os"
	"regexp"
	"strconv"
	"strings"
)

func readFile1() ([][]int, []string, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()

	var numbers [][]int
	var operations []string

	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if err != nil {
			break
		}

		// Normalize spaces and split groups
		splitNumbers := NormalizeSpacesAndSplit(string(line))

		// Handle operations line
		if strings.ContainsAny(string(line), "+*") {
			operations = splitNumbers
			continue
		}

		// Handle numbers lines
		var nums []int
		for _, numStr := range splitNumbers {
			num, err := strconv.Atoi(numStr)
			if err != nil {
				return nil, nil, err
			}
			nums = append(nums, num)
		}
		numbers = append(numbers, nums)
	}
	return numbers, operations, nil
}

func NormalizeSpacesAndSplit(line string) []string {
	whitespaces := regexp.MustCompile(`\s+`)
	singleSpaced := strings.Trim(whitespaces.ReplaceAllString(line, " "), " ")
	numbersStrings := strings.Split(singleSpaced, " ")
	return numbersStrings
}

func Reverse(s string) string {
	runes := []rune(s)
	for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
		runes[i], runes[j] = runes[j], runes[i]
	}
	return string(runes)
}

func readFile2() ([][]int, []string, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, nil, err
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

	numRows := len(grid) - 1
	numCols := len(grid[0])
	operations := NormalizeSpacesAndSplit(Reverse(grid[numRows]))
	var numbers [][]int

	operationNumbers := []int{}
	for j := numCols - 1; j >= 0; j-- {
		numStr := ""
		for i := 0; i < numRows; i++ {
			char := string(grid[i][j])
			if char == " " {
				continue
			}
			numStr += char
		}

		// Found empty column. Store current numbers as a group and continue
		if numStr == "" {
			numbers = append(numbers, operationNumbers)
			operationNumbers = []int{}
			continue
		}

		// Convert to integer
		num, err := strconv.Atoi(numStr)
		if err != nil {
			return nil, nil, err
		}
		operationNumbers = append(operationNumbers, num)

		// If first column, append the last group
		if j == 0 {
			numbers = append(numbers, operationNumbers)
		}
	}

	return numbers, operations, nil
}

func main() {
	numbers, operations, err := readFile1()
	if err != nil {
		panic(err)
	}

	// Part 1
	result1 := 0
	for i, op := range operations {
		result := numbers[0][i]
		for j := 1; j < len(numbers); j++ {
			switch op {
			case "+":
				result += numbers[j][i]
			case "*":
				result *= numbers[j][i]
			}
		}
		// println(result)
		result1 += result
	}

	// Output Part 1: 4878670269096
	println("Part 1:", result1)

	// Part 2
	numbers2, operations2, err := readFile2()
	if err != nil {
		panic(err)
	}

	result2 := 0
	for i, op := range operations2 {
		result := numbers2[i][0]
		for j := 1; j < len(numbers2[i]); j++ {
			switch op {
			case "+":
				result += numbers2[i][j]
			case "*":
				result *= numbers2[i][j]
			}
		}
		// println(result)
		result2 += result
	}

	// Output Part 2: 33451924481920
	println("Part 2:", result2)
}
