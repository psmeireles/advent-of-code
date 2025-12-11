package main

import (
	"bufio"
	"os"
	"strings"
)

func readInput() (map[string][]string, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, err
	}
	defer file.Close()

	graph := make(map[string][]string)
	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if err != nil {
			break
		}
		split := strings.Split(string(line), " ")
		from := split[0][:3]
		to := split[1:]

		for _, neighbor := range to {
			graph[from] = append(graph[from], neighbor)
		}
	}
	return graph, nil
}

func main() {
	graph, err := readInput()
	if err != nil {
		panic(err)
	}

	// Part 1
	result1 := getNumPathsBetweenPoints(graph, "you", "out", map[string]int{})

	// Output Part 1: 607
	println("Part 1:", result1)

	// Part 2
	svr_dac := getNumPathsBetweenPoints(graph, "svr", "dac", map[string]int{})
	// println("svr_dac:", svr_dac)
	svr_fft := getNumPathsBetweenPoints(graph, "svr", "fft", map[string]int{})
	// println("svr_fft:", svr_fft)
	dac_fft := getNumPathsBetweenPoints(graph, "dac", "fft", map[string]int{})
	// println("dac_fft:", dac_fft)
	fft_dac := getNumPathsBetweenPoints(graph, "fft", "dac", map[string]int{})
	// println("fft_dac:", fft_dac)
	fft_out := getNumPathsBetweenPoints(graph, "fft", "out", map[string]int{})
	// println("fft_out:", fft_out)
	dac_out := getNumPathsBetweenPoints(graph, "dac", "out", map[string]int{})
	// println("dac_out:", dac_out)

	// Output Part 2: 506264456238938
	println("Part 2:", svr_dac*dac_fft*fft_out+svr_fft*fft_dac*dac_out)
}

func getNumPathsBetweenPoints(graph map[string][]string, start, end string, memo map[string]int) int {
	if start == end {
		return 1
	}

	if numPaths, exists := memo[start]; exists {
		return numPaths
	}

	numPaths := 0
	for _, neighbor := range graph[start] {
		numPaths += getNumPathsBetweenPoints(graph, neighbor, end, memo)
	}
	memo[start] = numPaths
	return numPaths
}
