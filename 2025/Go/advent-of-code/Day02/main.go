package main

import (
	"bufio"
	"os"
	"strconv"
	"strings"
)

func readFile() ([]int, []int, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()

	var starts []int
	var ends []int

	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if len(line) > 0 {
			lineString := string(line)
			ranges := strings.Split(lineString, ",")
			for _, r := range ranges {
				bounds := strings.Split(r, "-")
				start, err := strconv.Atoi(bounds[0])
				if err != nil {
					return nil, nil, err
				}
				end, err := strconv.Atoi(bounds[1])
				if err != nil {
					return nil, nil, err
				}
				starts = append(starts, start)
				ends = append(ends, end)
			}
		} else if err != nil {
			break
		}
	}
	return starts, ends, nil
}

func main() {
	starts, ends, err := readFile()
	if err != nil {
		panic(err)
	}

	// Part 1
	result1 := 0
	for i := range starts {
		start := starts[i]
		end := ends[i]

		// For each number in the interval, check if the first half equals the second half
		for j := start; j <= end; j++ {
			jString := strconv.Itoa(j)
			if len(jString)%2 == 1 {
				continue
			}

			firstHalf := jString[0 : len(jString)/2]
			secondHalf := jString[len(jString)/2:]
			if firstHalf == secondHalf {
				result1 += j
				println(j)
			}
		}
	}

	// Output Part 1 result: 30599400849
	println("Part 1 Result:", result1)

	// Part 2
	result2 := 0
	for i := range starts {
		start := starts[i]
		end := ends[i]

		// For each number in the interval, check if it is made of repeated segments
		for j := start; j <= end; j++ {
			jString := strconv.Itoa(j)

			// Try all possible segment lengths until half of the string
			for k := 1; k <= len(jString)/2; k++ {
				if len(jString)%k != 0 {
					continue
				}

				segment := jString[:k]
				isInvalid := true
				for l := 0; l < len(jString); l++ {
					if jString[l] != segment[l%k] {
						isInvalid = false
						break
					}
				}

				// If found, add to result and break
				if isInvalid {
					result2 += j
					println(j)
					break
				}
			}
		}
	}

	// Output Part 2 result: 46270373595
	println("Part 2 Result:", result2)
}
