package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type Point struct {
	x, y int
}

func SolveWithBFS(input []string, deltaMap map[string]struct{ x, y int }) int {
	minBounds, maxBounds := Point{x: 0, y: 0}, Point{x: 0, y: 0}

	points := make(map[Point]bool)
	currPoint := Point{x: 0, y: 0}
	points[currPoint] = true

	for _, line := range input {
		parts := strings.Fields(line)
		dir, numStr, _ := parts[0], parts[1], parts[2][1:len(parts[2])-1]

		num, err := strconv.Atoi(numStr)
		if err != nil {
			log.Fatal("Error: ", err)
		}

		// fmt.Println(dir, num, color)
		delta := deltaMap[dir]

		for range num {
			currPoint.x += delta.x
			currPoint.y += delta.y
			points[currPoint] = true

			minBounds.x = min(minBounds.x, currPoint.x)
			minBounds.y = min(minBounds.y, currPoint.y)

			maxBounds.x = max(maxBounds.x, currPoint.x)
			maxBounds.y = max(maxBounds.y, currPoint.y)
		}
	}

	// Check each corner
	startPoints := []Point{
		{x: 1, y: 1},
		{x: -1, y: 1},
		{x: -1, y: -1},
		{x: 1, y: -1},
	}

	for _, startPoint := range startPoints {
		size := GetInteriorSizeBFS(points, startPoint, minBounds, maxBounds)
		if size != -1 {
			fmt.Println("Found size: ", size, len(points))
			return len(points) + size
		}
	}

	return -1
}

func GetInteriorSizeBFS(points map[Point]bool, startPoint, minBounds, maxBounds Point) int {
	queue := make([]Point, 0)
	queue = append(queue, startPoint)

	visited := make(map[Point]bool)
	for len(queue) > 0 {
		point := queue[0]
		queue = queue[1:]

		// If the point is out of bounds, this must be "outside"
		if point.x < minBounds.x || point.x > maxBounds.x || point.y < minBounds.y || point.y > maxBounds.y {
			return -1
		}

		// Skip if it already exists, either as wall point or visited
		if points[point] || visited[point] {
			continue
		}
		visited[point] = true

		// Add neighbors
		queue = append(queue, Point{x: point.x, y: point.y + 1}) // Up
		queue = append(queue, Point{x: point.x - 1, y: point.y}) // Left
		queue = append(queue, Point{x: point.x, y: point.y - 1}) // Down
		queue = append(queue, Point{x: point.x + 1, y: point.y}) // Right
	}

	return len(visited)
}

// Use shoelace formula to get total area first
// Then reverse Pick's theorem to get points inside
func GetInteriorSize(vertices []Point, pointBoundCount int) int {
	// https://en.wikipedia.org/wiki/Shoelace_formula
	sum := 0
	for i := range vertices {
		currVertex, nextVertex := vertices[i], vertices[(i+1)%len(vertices)]
		sum += (currVertex.x * nextVertex.y) - (currVertex.y * nextVertex.x)
	}
	sum = max(sum, -sum)

	// https://en.wikipedia.org/wiki/Pick%27s_theorem
	return sum/2 - pointBoundCount/2 + 1
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	deltaMap := map[string]struct{ x, y int }{
		"U": {x: 0, y: 1},
		"L": {x: -1, y: 0},
		"D": {x: 0, y: -1},
		"R": {x: 1, y: 0},
	}

	digitMap := map[byte]string{
		'0': "R",
		'1': "D",
		'2': "L",
		'3': "U",
	}

	currPoint, currPoint2 := Point{x: 0, y: 0}, Point{x: 0, y: 0}
	vertices, vertices2 := make([]Point, 0), make([]Point, 0)
	pointBoundCount, pointBoundCount2 := 0, 0

	for _, line := range input {
		parts := strings.Fields(line)
		dir, numStr, color := parts[0], parts[1], parts[2][2:len(parts[2])-1]

		num, err := strconv.Atoi(numStr)
		if err != nil {
			log.Fatal("Error: ", err)
		}

		// Part 1
		pointBoundCount += num
		delta := deltaMap[dir]
		currPoint.x += delta.x * num
		currPoint.y += delta.y * num
		vertices = append(vertices, currPoint)

		// Part 2
		num2Str, digit := color[:len(color)-1], color[len(color)-1]
		num2Parsed, err2 := strconv.ParseInt(num2Str, 16, 32)
		if err2 != nil {
			log.Fatal("Error: ", err)
		}

		num2 := int(num2Parsed)
		pointBoundCount2 += num2
		delta2 := deltaMap[digitMap[digit]]
		currPoint2.x += delta2.x * num2
		currPoint2.y += delta2.y * num2
		vertices2 = append(vertices2, currPoint2)
	}

	// Check each corner
	count := GetInteriorSize(vertices, pointBoundCount) + pointBoundCount
	count2 := GetInteriorSize(vertices2, pointBoundCount2) + pointBoundCount2

	fmt.Printf("Part 1: %d, Part 2: %d\n", count, count2)
}
