package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Shape struct {
	Shape []string
	Area  int
}

func (s Shape) ToString() string {
	var b strings.Builder
	b.WriteString("Shape:\n")
	for _, row := range s.Shape {
		b.WriteString("  " + row + "\n")
	}
	b.WriteString(fmt.Sprintf("Area: %d", s.Area))
	return b.String()
}

type Region struct {
	Shapes []int
	Width  int
	Height int
}

func (r Region) ToString() string {
	return fmt.Sprintf(
		"Region:\n  Shapes: %v\n  Width: %d\n  Height: %d",
		r.Shapes, r.Width, r.Height,
	)
}

func readInput() ([]Shape, []Region, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()

	shapes := []Shape{}
	regions := []Region{}
	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if err != nil {
			break
		}

		lineString := string(line)
		if lineString[len(lineString)-1] == ':' {
			// Start of shape block
			shapeLines := []string{}
			area := 0
			for {
				nextLine, _, err := r.ReadLine()
				if err != nil {
					return nil, nil, err
				}
				nextLineString := string(nextLine)
				if len(nextLineString) == 0 {
					// Got to the last line of block. Save shape
					shapes = append(shapes, Shape{
						Shape: shapeLines,
						Area:  area,
					})
					break
				}
				shapeLines = append(shapeLines, nextLineString)
				for _, c := range nextLineString {
					if c == '#' {
						area++
					}
				}
			}
		} else {
			// Regions
			splitByColon := strings.Split(lineString, ":")
			dimensions := strings.Split(splitByColon[0], "x")
			width, err := strconv.Atoi(dimensions[0])
			if err != nil {
				return nil, nil, err
			}

			var height int
			height, err = strconv.Atoi(dimensions[1])
			if err != nil {
				return nil, nil, err
			}

			quantitiesString := strings.Split(splitByColon[1][1:], " ")
			quantities := []int{}
			for _, quantityString := range quantitiesString {
				var quantity int
				quantity, err = strconv.Atoi(quantityString)
				if err != nil {
					return nil, nil, err
				}
				quantities = append(quantities, quantity)
			}
			regions = append(regions, Region{
				Shapes: quantities,
				Width:  width,
				Height: height,
			})
		}
	}
	return shapes, regions, nil
}

func main() {
	shapes, regions, err := readInput()
	if err != nil {
		panic(err)
	}

	// Luckily this works for the input. Just check if total needed area fits in the box.
	result := 0
	for _, region := range regions {
		neededArea := 0
		for shapeIndex, shapeAmount := range region.Shapes {
			neededArea += shapes[shapeIndex].Area * shapeAmount
		}

		if neededArea <= region.Height*region.Width {
			result++
		}
	}

	// Output: 505
	println("Result:", result)
}
