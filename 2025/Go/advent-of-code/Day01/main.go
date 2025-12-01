package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

func readFile() (directions []string, steps []int, err error) {
	file, err := os.Open("input.txt")
	defer file.Close()
	if err != nil {
		return nil, nil, err
	}

	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if len(line) > 0 {
			lineString := string(line)
			directions = append(directions, lineString[0:1])
			step, err := strconv.Atoi(lineString[1:])
			if err != nil {
				return nil, nil, err
			}
			steps = append(steps, step)
		} else if err != nil {
			break
		}
	}
	return directions, steps, nil
}

func main() {
	directions, steps, err := readFile()
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}

	// Part 1
	result1 := 0
	start := 50
	for i, dir := range directions {
		switch dir {
		case "L":
			start = (((start - steps[i]) % 100) + 100) % 100
		case "R":
			start = (start + steps[i]) % 100
		}
		if start == 0 {
			result1++
		}
	}

	// Output Part 1 result: 999
	fmt.Println("Part 1 Result:", result1)

	// Part 2
	result2 := 0
	start = 50
	for i, dir := range directions {
		result2 += steps[i] / 100
		remainder := steps[i] % 100

		switch dir {
		case "L":
			if start != 0 && remainder >= start {
				result2++
			}
			start = (((start - steps[i]) % 100) + 100) % 100
		case "R":
			if start != 0 && remainder >= (100-start) {
				result2++
			}
			start = (start + steps[i]) % 100
		}
	}

	// Output Part 2 result: 6099
	fmt.Println("Part 2 Result:", result2)
}
