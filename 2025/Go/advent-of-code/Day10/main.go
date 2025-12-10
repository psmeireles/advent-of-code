package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Machine struct {
	Lights  string
	Buttons [][]int

	Bits           int
	LightsBitmask  int
	ButtonsBitmask []int
	Requirements   []int
}

// Construtor que recebe strings e transforma em bitmasks
func NewMachine(lights string, wiring [][]int, requirements []int) Machine {
	lightsBin := 0
	for i, c := range lights {
		if c == '#' {
			lightsBin |= 1 << i
		}
	}

	// Convert each button to bitmask
	wiringMasks := make([]int, len(wiring))
	for i, button := range wiring {
		mask := 0
		for _, idx := range button {
			mask |= 1 << idx
		}
		wiringMasks[i] = mask
	}

	return Machine{
		Lights:         lights,
		Buttons:        wiring,
		LightsBitmask:  lightsBin,
		ButtonsBitmask: wiringMasks,
		Requirements:   requirements,
		Bits:           len(lights),
	}
}

func (m Machine) ToString() string {
	numBits := m.Bits

	wiringStr := "["
	for i, mask := range m.ButtonsBitmask {
		wiringStr += fmt.Sprintf("%0*b", numBits, mask)
		if i < len(m.ButtonsBitmask)-1 {
			wiringStr += ", "
		}
	}
	wiringStr += "]"

	// Formata Lights
	return fmt.Sprintf(
		"Machine{Lights: %0*b, Wiring: %s, Requirements: %v}",
		numBits, m.LightsBitmask, wiringStr, m.Requirements,
	)
}

func readInput() ([]Machine, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var machines []Machine
	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if err != nil {
			break
		}
		items := strings.Split(string(line), " ")
		lights := items[0][1 : len(items[0])-1]
		requirementsString := items[len(items)-1][1 : len(items[len(items)-1])-1]
		requirementsSplit := strings.Split(requirementsString, ",")
		requirements := make([]int, 0)
		for _, r := range requirementsSplit {
			rInt, err := strconv.Atoi(r)
			if err != nil {
				return nil, err
			}
			requirements = append(requirements, rInt)
		}

		wires := make([][]int, 0)
		for i := 1; i < len(items)-1; i++ {
			wiresSplit := strings.Split(items[i][1:len(items[i])-1], ",")
			wire := make([]int, 0)
			for _, w := range wiresSplit {
				wireInt, err := strconv.Atoi(w)
				if err != nil {
					return nil, err
				}
				wire = append(wire, wireInt)
			}
			wires = append(wires, wire)
		}
		machines = append(machines, NewMachine(lights, wires, requirements))

	}
	return machines, nil
}

func main() {
	machines, err := readInput()
	if err != nil {
		panic(err)
	}

	// Part 1
	result1 := 0

	// For each machine, use dp to find minimum number of steps to each state
	for _, machine := range machines {
		dp := make([]int, 2<<machine.Bits)
		for i := range dp {
			dp[i] = -1
		}

		dp[0] = 0
		queue := []int{0}
		for len(queue) > 0 {
			curr := queue[0]
			queue = queue[1:]

			for _, button := range machine.ButtonsBitmask {
				newState := curr ^ button
				if dp[newState] > dp[curr]+1 || dp[newState] == -1 {
					dp[newState] = dp[curr] + 1
					queue = append(queue, newState)
				}
			}
		}
		//println(dp[machine.Lights])
		result1 += dp[machine.LightsBitmask]
	}

	// Output Part 1: 390
	println("Part 1:", result1)

	// Part 2
	result2 := 0

	for _, machine := range machines {
		type State []int

		copyState := func(s State) State {
			newS := make(State, len(s))
			copy(newS, s)
			return newS
		}

		stateKey := func(s State) string {
			return fmt.Sprint(s)
		}

		dp := make(map[string]int)
		start := make(State, len(machine.Requirements))
		dp[stateKey(start)] = 0
		queue := []State{start}

		for len(queue) > 0 {
			curr := queue[0]
			queue = queue[1:]

			for _, button := range machine.ButtonsBitmask {
				newState := copyState(curr)
				for i := 0; i < len(newState); i++ {
					// Incrementa coordenada se o botão influencia
					if (button>>i)&1 == 1 {
						newState[i]++
						// Se passar da quantidade requerida, ignora
						if newState[i] > machine.Requirements[i] {
							newState = nil
							break
						}
					}
				}
				if newState == nil {
					continue
				}
				key := stateKey(newState)
				if dp[key] == 0 && key != stateKey(start) || dp[key] > dp[stateKey(curr)]+1 {
					dp[key] = dp[stateKey(curr)] + 1
					queue = append(queue, newState)
				}
			}
		}

		target := machine.Requirements
		println(dp[stateKey(target)])
		result2 += dp[stateKey(target)]
	}

	fmt.Println("Part 2:", result2)
}
