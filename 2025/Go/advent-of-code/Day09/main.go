package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

type Edge struct {
	start Point
	end   Point
}

func (e Edge) ToString() string {
	return fmt.Sprintf("(%s,%s)", e.start.ToString(), e.end.ToString())
}

type Rectangle struct {
	Corner1 Point
	Corner2 Point
}

func (r Rectangle) GetCorners() []Point {
	return []Point{
		r.Corner1,
		r.Corner2,
		{
			r.Corner1.X,
			r.Corner2.Y,
		},
		{
			r.Corner2.X,
			r.Corner1.Y,
		},
	}
}

func (r Rectangle) GetEdges() []Edge {
	corners := r.GetCorners()
	return []Edge{
		{
			start: corners[0],
			end:   corners[1],
		},
		{
			start: corners[1],
			end:   corners[2],
		},
		{
			start: corners[2],
			end:   corners[3],
		},
		{
			start: corners[3],
			end:   corners[0],
		},
	}

}

func (r Rectangle) Area() int {
	dx := abs(r.Corner1.X-r.Corner2.X) + 1
	dy := abs(r.Corner1.Y-r.Corner2.Y) + 1
	return dx * dy
}

func (r Rectangle) ToString() string {
	return fmt.Sprintf("(%s,%s) -> Area: %d", r.Corner1.ToString(), r.Corner2.ToString(), r.Area())
}

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

type Point struct {
	X int
	Y int
}

func (p Point) ToString() string {
	return fmt.Sprintf("(%d,%d)", p.X, p.Y)
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
		})

	}
	return points, nil
}

// Checks if two line segments intersect *properly* (i.e., crossing strictly
// in their interiors, excluding touching at endpoints, collinear overlaps, etc).
// It returns true *only if* the two segments cross each other in a single point
// that is interior to both segments — not sharing endpoints, not touching, not overlapping.
func hasIntersection(e1, e2 Edge) bool {

	a1 := e1.start
	a2 := e1.end
	b1 := e2.start
	b2 := e2.end

	// Orientation test:
	// Given three points (p, q, r), the sign of the cross product:
	//
	//       (q - p) × (r - p)
	//
	// tells whether r is to the left (>0), right (<0), or collinear (=0)
	// relative to the directed segment p → q.
	orient := func(p, q, r Point) int {
		v := (q.X-p.X)*(r.Y-p.Y) - (q.Y-p.Y)*(r.X-p.X)

		if v == 0 {
			return 0 // points are collinear
		}
		if v > 0 {
			return 1 // r is to the left of pq
		}
		return -1 // r is to the right of pq
	}

	// Compute orientations:
	// o1 = orientation(a1, a2, b1)
	// o2 = orientation(a1, a2, b2)
	// → tells whether b1 and b2 lie on opposite sides of segment a1–a2
	//
	// o3 = orientation(b1, b2, a1)
	// o4 = orientation(b1, b2, a2)
	// → tells whether a1 and a2 lie on opposite sides of segment b1–b2
	o1 := orient(a1, a2, b1)
	o2 := orient(a1, a2, b2)
	o3 := orient(b1, b2, a1)
	o4 := orient(b1, b2, a2)

	// Proper intersection criterion:
	//
	// Two segments intersect strictly in their interiors iff:
	//
	//  1. b1 and b2 lie on *opposite sides* of a1→a2  → (o1 * o2 < 0)
	//  2. a1 and a2 lie on *opposite sides* of b1→b2  → (o3 * o4 < 0)
	//
	// This ensures:
	//   - They are not collinear
	//   - They are not touching at endpoints
	//   - They are not just overlapping
	//   - The intersection is a proper "X" crossing

	return o1*o2 < 0 && o3*o4 < 0
}

func isEdgeInsidePolygon(e Edge, allEdges []Edge) bool {
	for _, e2 := range allEdges {
		if hasIntersection(e, e2) {
			return false
		}
	}
	return true
}

func main() {
	redPoints, err := readInput()
	if err != nil {
		panic(err)
	}

	// Part 1
	allRectangles := make([]Rectangle, 0)
	result1 := 0
	for i := 0; i < len(redPoints); i++ {
		for j := i + 1; j < len(redPoints); j++ {
			rectangle := Rectangle{
				Corner1: redPoints[i],
				Corner2: redPoints[j],
			}
			allRectangles = append(allRectangles, rectangle)
			// println(rectangle.ToString())
			result1 = max(result1, rectangle.Area())
		}
	}

	// Output Part 1: 4761736832
	println("Part 1:", result1)

	// Part 2
	allEdges := make([]Edge, 0)
	for i := 0; i < len(redPoints); i++ {
		allEdges = append(allEdges, Edge{
			start: redPoints[i],
			end:   redPoints[(i+1)%len(redPoints)],
		})
	}

	// Sort rectangles descending by area
	sortedRectangles := allRectangles
	sort.Slice(sortedRectangles, func(i, j int) bool {
		return sortedRectangles[i].Area() > sortedRectangles[j].Area()
	})

	// Find first largest valid rectangle
	result2 := 0
	for _, r := range sortedRectangles {
		valid := true

		// Check edges inside polygon
		edges := r.GetEdges()
		for _, edge := range edges {
			if !isEdgeInsidePolygon(edge, allEdges) {
				valid = false
				break
			}
		}
		if !valid {
			continue
		}

		result2 = r.Area()
		break
	}

	// Output Part 2: 1452422268
	println("Part 2:", result2)
}
