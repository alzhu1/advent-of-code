package main

import (
	"fmt"
	"os"
	"strings"
)

type Point struct {
	x, y int
}

type Edge struct {
	from, to Point
	dist     int
}

func Search(pointEdgeMap map[Point][]Edge, curr, end Point, visited map[Point]bool, steps int) int {
	if curr == end {
		// fmt.Println("Soln: ", steps)
		return steps
	}

	if visited[curr] {
		return 0
	}

	nextVisited := make(map[Point]bool)
	for k := range visited {
		nextVisited[k] = true
	}
	nextVisited[curr] = true

	// For a regular tile, we want the max value
	maxValue := 0
	edges := pointEdgeMap[curr]
	for _, edge := range edges {
		maxValue = max(maxValue, Search(pointEdgeMap, edge.to, end, nextVisited, steps+edge.dist))
	}

	return maxValue
}

func FollowEdge(input []string, points map[Point]bool, origin, curr Point) Edge {
	path := make(map[Point]bool)
	path[origin] = true

	// There should only be 1 possible direction
	for !points[curr] {
		path[curr] = true

		neighbors := []Point{
			{x: curr.x, y: curr.y - 1},
			{x: curr.x, y: curr.y + 1},
			{x: curr.x - 1, y: curr.y},
			{x: curr.x + 1, y: curr.y},
		}

		validCount := 0
		nextNeighbor := neighbors[0]
		for _, neighbor := range neighbors {
			if !path[neighbor] && input[neighbor.y][neighbor.x] != '#' {
				validCount++
				nextNeighbor = neighbor
			}
		}

		if validCount > 1 {
			panic("This should not happen")
		}

		curr = nextNeighbor
	}

	return Edge{from: origin, to: curr, dist: len(path)}
}

func FillEdges(input []string, points map[Point]bool, ignoreSlopes bool) map[Point][]Edge {
	pointEdgeMap := make(map[Point][]Edge)

	for point := range points {
		edges := make([]Edge, 0)

		x, y := point.x, point.y

		// North
		if y-1 > 0 && input[y-1][x] != '#' && (ignoreSlopes || input[y-1][x] != 'v') {
			edges = append(edges, FollowEdge(input, points, point, Point{x: x, y: y - 1}))
		}

		// South
		if y+1 < len(input) && input[y+1][x] != '#' && (ignoreSlopes || input[y+1][x] != '^') {
			edges = append(edges, FollowEdge(input, points, point, Point{x: x, y: y + 1}))
		}

		// West
		if x-1 > 0 && input[y][x-1] != '#' && (ignoreSlopes || input[y][x-1] != '>') {
			edges = append(edges, FollowEdge(input, points, point, Point{x: x - 1, y: y}))
		}

		// East
		if x+1 < len(input[y]) && input[y][x+1] != '#' && (ignoreSlopes || input[y][x+1] != '<') {
			edges = append(edges, FollowEdge(input, points, point, Point{x: x + 1, y: y}))
		}

		pointEdgeMap[point] = edges
	}

	return pointEdgeMap
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	points := make(map[Point]bool)
	var start, end Point
	for y, line := range input {
		for x, c := range line {
			if c != '#' {
				point := Point{x: x, y: y}
				if y == 0 {
					start = point
					points[point] = true
				} else if y == len(input)-1 {
					end = point
					points[point] = true
				} else {
					// Check for junctions
					openNeighbors := 0
					if input[y-1][x] != '#' {
						openNeighbors++
					}

					if input[y+1][x] != '#' {
						openNeighbors++
					}

					if input[y][x-1] != '#' {
						openNeighbors++
					}

					if input[y][x+1] != '#' {
						openNeighbors++
					}

					if openNeighbors >= 3 {
						points[point] = true
					}
				}

			}
		}
	}

	// Construct edges (directed)
	pointEdgeMap := FillEdges(input, points, false)
	length := Search(pointEdgeMap, start, end, make(map[Point]bool), 0)

	fmt.Println("p1 done, on to p2 (p1 res:)", length)

	// Ignore slopes to get all edges (i.e. undirected)
	pointEdgeMap2 := FillEdges(input, points, true)
	length2 := Search(pointEdgeMap2, start, end, make(map[Point]bool), 0)

	fmt.Printf("Part 1: %d, Part 2: %d\n", length, length2)
}
