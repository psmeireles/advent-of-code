package main

import (
	"bufio"
	"math"
	"os"
	"slices"
	"strconv"
)

func readFile() ([][]int, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var banks [][]int

	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if len(line) > 0 {
			batteries := []int{}
			for _, numberString := range string(line) {
				number, err := strconv.Atoi(string(numberString))
				if err != nil {
					return nil, err
				}
				batteries = append(batteries, number)
			}
			banks = append(banks, batteries)
		} else if err != nil {
			break
		}
	}
	return banks, nil
}

func main() {
	banks, err := readFile()
	if err != nil {
		panic(err)
	}

	// Part 1
	result1 := 0
	for _, bank := range banks {
		first := bank[0]
		second := bank[1]
		for i := 1; i < len(bank); i++ {
			if bank[i] > first && i != len(bank)-1 {
				first = bank[i]
				second = bank[i+1]
			} else if bank[i] > second {
				second = bank[i]
			}
		}
		var bestBattery = first*10 + second
		// println(bestBattery)
		result1 += bestBattery
	}

	// Output Part 1: 17301
	println("Part 1:", result1)

	// Part 2
	result2 := 0
	for _, bank := range banks {
		remaining := 12
		var result []int
		batteries := bank

		// Reserve the number of remaining batteries and pick the max out of reserve each time
		// Repeat until we have 12
		for remaining > 0 {
			leftEnd := len(batteries) - remaining + 1
			leftSide := batteries[:leftEnd]
			max := slices.Max(leftSide)
			result = append(result, max)
			maxIndex := slices.Index(leftSide, max)
			batteries = batteries[maxIndex+1:]
			remaining--
		}

		bestBattery := 0
		for i, val := range result {
			bestBattery += val * int(math.Pow(10, float64(len(result)-i-1)))
		}
		// println(bestBattery)
		result2 += bestBattery
	}

	// Output Part 2: 172162399742349
	println("Part 2:", result2)
}
