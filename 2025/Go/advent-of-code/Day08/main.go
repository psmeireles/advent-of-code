package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

type Point struct {
	X int
	Y int
	Z int
}

func (p Point) DistanceSquared(other Point) int {
	dx := p.X - other.X
	dy := p.Y - other.Y
	dz := p.Z - other.Z
	return dx*dx + dy*dy + dz*dz
}

func (p Point) ToString() string {
	return fmt.Sprintf("(%d,%d,%d)", p.X, p.Y, p.Z)
}

func atoi(s string) int {
	i, _ := strconv.Atoi(s)
	return i
}

func readInput() ([]Point, error) {
	file, err := os.Open("input.txt")
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var points []Point
	r := bufio.NewReader(file)
	for {
		line, _, err := r.ReadLine()
		if err != nil {
			break
		}
		coords := strings.Split(string(line), ",")
		points = append(points, Point{
			X: atoi(coords[0]),
			Y: atoi(coords[1]),
			Z: atoi(coords[2]),
		})

	}
	return points, nil
}

// Union-Find data structure for clustering
type UnionFind struct {
	parent map[Point]Point
	rank   map[Point]int
}

func NewUnionFind() *UnionFind {
	return &UnionFind{
		parent: make(map[Point]Point),
		rank:   make(map[Point]int),
	}
}

func (uf *UnionFind) Find(p Point) Point {
	if _, exists := uf.parent[p]; !exists {
		uf.parent[p] = p
		uf.rank[p] = 0
	}
	if uf.parent[p] != p {
		uf.parent[p] = uf.Find(uf.parent[p])
	}
	return uf.parent[p]
}
func (uf *UnionFind) Union(p1, p2 Point) {
	root1 := uf.Find(p1)
	root2 := uf.Find(p2)
	if root1 != root2 {
		if uf.rank[root1] < uf.rank[root2] {
			uf.parent[root1] = root2
		} else if uf.rank[root1] > uf.rank[root2] {
			uf.parent[root2] = root1
		} else {
			uf.parent[root2] = root1
			uf.rank[root1]++
		}
	}
}

func main() {
	points, err := readInput()
	if err != nil {
		panic(err)
	}

	// Part 1
	sortedPairs := make([][2]Point, 0)
	for i := 0; i < len(points); i++ {
		for j := i + 1; j < len(points); j++ {
			sortedPairs = append(sortedPairs, [2]Point{points[i], points[j]})
		}
	}

	// Sort pairs by distance squared
	sort.Slice(sortedPairs, func(i, j int) bool {
		return sortedPairs[i][0].DistanceSquared(sortedPairs[i][1]) < sortedPairs[j][0].DistanceSquared(sortedPairs[j][1])
	})

	// Group into circuits combining closest pairs together
	uf := NewUnionFind()
	for i, pair := range sortedPairs {
		if i == 1000 {
			break
		}

		parent1 := uf.Find(pair[0])
		parent2 := uf.Find(pair[1])

		// Only union if they are in different sets
		if parent1 != parent2 {
			uf.Union(pair[0], pair[1])
			// println(pair[0].ToString(), "-", pair[1].ToString())
		}
	}

	// Find circuit sizes
	circuitSizes := make(map[Point]int)
	for _, p := range points {
		root := uf.Find(p)
		circuitSizes[root]++
	}

	// Find 3 largest circuits and multiply their sizes
	sizes := make([]int, 0, len(circuitSizes))
	for _, size := range circuitSizes {
		sizes = append(sizes, size)
	}
	sort.Sort(sort.Reverse(sort.IntSlice(sizes)))
	result1 := sizes[0] * sizes[1] * sizes[2]

	// Output Part 1: 97384
	println("Part 1:", result1)

	// Part 2

	// Continue from i = 1000 and keep connecting
	lastUnionMultiplication := 0
	for i := 1000; i < len(sortedPairs); i++ {
		pair := sortedPairs[i]

		parent1 := uf.Find(pair[0])
		parent2 := uf.Find(pair[1])

		// Only union if they are in different sets
		if parent1 != parent2 {
			uf.Union(pair[0], pair[1])
			lastUnionMultiplication = pair[0].X * pair[1].X
		}
	}

	// Output Part 2: 9003685096
	println("Part 2:", lastUnionMultiplication)
}
