package main

import (
	"fmt"
	"os"
	"strings"
)

type Point struct {
	i, j, deltaI, deltaJ int
}

type PointCoordinates struct {
	i, j int
}

func GetEnergizedTiles(input []string, startI, startJ int) int {
	deltas := []struct{ deltaI, deltaJ int }{
		{deltaI: -1, deltaJ: 0}, // North
		{deltaI: 0, deltaJ: -1}, // West
		{deltaI: 1, deltaJ: 0},  // South
		{deltaI: 0, deltaJ: 1},  // East
	}

	deltaIdxMap := map[struct{ deltaI, deltaJ int }]int{
		{deltaI: -1, deltaJ: 0}: 0,
		{deltaI: 0, deltaJ: -1}: 1,
		{deltaI: 1, deltaJ: 0}:  2,
		{deltaI: 0, deltaJ: 1}:  3,
	}

	// Figure out deltas
	startPoint := Point{
		i: startI,
		j: startJ,
	}

	if startI < 0 {
		// Start top, go south
		startPoint.deltaI = deltas[2].deltaI
		startPoint.deltaJ = deltas[2].deltaJ
	} else if startI >= len(input) {
		// Start bottom, go north
		startPoint.deltaI = deltas[0].deltaI
		startPoint.deltaJ = deltas[0].deltaJ
	} else if startJ < 0 {
		// Start left, go right
		startPoint.deltaI = deltas[3].deltaI
		startPoint.deltaJ = deltas[3].deltaJ
	} else if startJ >= len(input[startI]) {
		// Start right, go left
		startPoint.deltaI = deltas[1].deltaI
		startPoint.deltaJ = deltas[1].deltaJ
	} else {
		panic("This should not happen")
	}

	queue := make([]Point, 0)
	queue = append(queue, startPoint)

	highlighted := make(map[PointCoordinates]bool)
	visited := make(map[Point]bool)
	for len(queue) > 0 {
		point := queue[0]
		queue = queue[1:]

		i, j, deltaI, deltaJ := point.i, point.j, point.deltaI, point.deltaJ

		if visited[point] {
			continue
		}
		visited[point] = true

		// Only count highlights within
		if i >= 0 && i < len(input) && j >= 0 && j < len(input[i]) {
			highlighted[PointCoordinates{i: i, j: j}] = true
		}

		nextPoint := Point{
			i:      i + deltaI,
			j:      j + deltaJ,
			deltaI: deltaI,
			deltaJ: deltaJ,
		}

		nextI, nextJ := nextPoint.i, nextPoint.j

		if nextI < 0 || nextI >= len(input) || nextJ < 0 || nextJ >= len(input[nextI]) {
			continue
		}

		deltaIdx := deltaIdxMap[struct {
			deltaI int
			deltaJ int
		}{deltaI: deltaI, deltaJ: deltaJ}]

		switch input[nextI][nextJ] {
		case '.':
			queue = append(queue, nextPoint)

		case '|':
			if deltaJ != 0 {
				queue = append(queue, Point{i: nextI, j: nextJ, deltaI: 1, deltaJ: 0})
				queue = append(queue, Point{i: nextI, j: nextJ, deltaI: -1, deltaJ: 0})
			} else {
				queue = append(queue, nextPoint)
			}

		case '-':
			if deltaI != 0 {
				queue = append(queue, Point{i: nextI, j: nextJ, deltaI: 0, deltaJ: 1})
				queue = append(queue, Point{i: nextI, j: nextJ, deltaI: 0, deltaJ: -1})
			} else {
				queue = append(queue, nextPoint)
			}

		case '/':
			nextDelta := struct{ deltaI, deltaJ int }{deltaI: 0, deltaJ: 0}
			if deltaIdx%2 == 0 {
				nextDelta = deltas[(len(deltas)+deltaIdx-1)%len(deltas)]
			} else {
				nextDelta = deltas[(deltaIdx+1)%len(deltas)]
			}
			queue = append(queue, Point{i: nextI, j: nextJ, deltaI: nextDelta.deltaI, deltaJ: nextDelta.deltaJ})

		case '\\':
			nextDelta := struct{ deltaI, deltaJ int }{deltaI: 0, deltaJ: 0}
			if deltaIdx%2 == 0 {
				nextDelta = deltas[(deltaIdx+1)%len(deltas)]
			} else {
				nextDelta = deltas[(len(deltas)+deltaIdx-1)%len(deltas)]
			}
			queue = append(queue, Point{i: nextI, j: nextJ, deltaI: nextDelta.deltaI, deltaJ: nextDelta.deltaJ})
		}
	}

	return len(highlighted)
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	startEnergized := GetEnergizedTiles(input, 0, -1)

	mostEnergized := 0

	// Process left and right outside edges
	for i := range input {
		mostEnergized = max(mostEnergized, GetEnergizedTiles(input, i, -1))
		mostEnergized = max(mostEnergized, GetEnergizedTiles(input, i, len(input[i])))
	}

	// Proces top and bottom outside edges
	for j := range len(input[0]) {
		mostEnergized = max(mostEnergized, GetEnergizedTiles(input, -1, j))
		mostEnergized = max(mostEnergized, GetEnergizedTiles(input, len(input), j))
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", startEnergized, mostEnergized)
}
