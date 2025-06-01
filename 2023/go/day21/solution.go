package main

import (
	"fmt"
	"os"
	"strings"
)

type Point struct {
	x, y, steps int
}

func Search(spaces map[Point]bool, start Point, targetSteps int) int {
	queue := make([]Point, 0)
	queue = append(queue, start)

	visited := make(map[Point]bool)
	evenVisisted := make(map[Point]bool)
	for len(queue) > 0 {
		point := queue[0]
		queue = queue[1:]

		keyPoint := Point{x: point.x, y: point.y}
		if spaces[keyPoint] && !visited[keyPoint] {
			visited[keyPoint] = true

			if point.steps%2 == targetSteps%2 {
				evenVisisted[keyPoint] = true
			}

			if point.steps < targetSteps {
				queue = append(queue, Point{x: point.x + 1, y: point.y, steps: point.steps + 1})
				queue = append(queue, Point{x: point.x, y: point.y + 1, steps: point.steps + 1})
				queue = append(queue, Point{x: point.x - 1, y: point.y, steps: point.steps + 1})
				queue = append(queue, Point{x: point.x, y: point.y - 1, steps: point.steps + 1})
			}
		}
	}

	return len(evenVisisted)
}

func ExpandDimensions(spaces map[Point]bool, min, max, size Point) map[Point]bool {
	newSpaces := make(map[Point]bool)

	for space := range spaces {
		for x := min.x; x <= max.x; x++ {
			for y := min.y; y <= max.y; y++ {
				newSpace := Point{
					x: space.x + (x * size.x),
					y: space.y + (y * size.y),
				}
				newSpaces[newSpace] = true
			}
		}
	}

	return newSpaces
}

// My sequence: 3847, 94697, 306403, 638965, 1092383, 1666657, 2361787, 3177773, 4114615, 5172313 ...
// ax^2 + bx + c = 0

// x = 0 -> c = 3847
// x = 1 -> a + b + 3847 = 94697
// x = 2 -> 4a + 2b + 3847 = 306403

// a + b = 90850
// 4a + 2b = 302556

// 4a + 2b = 302556
// 2a + 2b = 181700

// 2a = 120856, a = 60428

// 60428 + b = 90850, b = 30422

// Final equation: 60428x^2 + 30422x + 3847 = 0
func ProcessEquation(x int) int {
	return 60428*x*x + 30422*x + 3847
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	spaces := make(map[Point]bool)
	rocks := make(map[Point]bool)
	var start Point

	for y, line := range input {
		for x, c := range line {
			point := Point{
				x: x,
				y: y,
			}

			switch c {
			case '.':
				spaces[point] = true
			case '#':
				rocks[point] = true
			case 'S':
				spaces[point] = true
				start = point
			}
		}
	}

	// These were fetched from doing 10 iterations, step sequence is 65 + (2*131)x
	// Below section was just verifying that this is a quadratic equation
	// testNums := []int{3847, 94697, 306403, 638965, 1092383, 1666657, 2361787, 3177773, 4114615, 5172313}
	// for len(testNums) > 0 {
	// 	fmt.Println(testNums)
	// 	newNums := make([]int, 0)

	// 	for i := 0; i < len(testNums)-1; i++ {
	// 		newNums = append(newNums, testNums[i+1]-testNums[i])
	// 	}
	// 	testNums = newNums
	// }

	// min := Point{x: -2, y: -2}
	// max := Point{x: 2, y: 2}
	// size := Point{x: len(input[0]), y: len(input)}

	// newSpaces := ExpandDimensions(spaces, min, max, size)
	// fmt.Println(Search(newSpaces, start, 65+(2*131)*0))

	// fmt.Println(start, len(spaces), len(newSpaces), Search(spaces, start, 65), Search(newSpaces, start, 65 + (2*131)*2))

	// 101150 = (26501365 - 65) / (2*131)
	// aka, for x = 101150, 65 + (2 * 131)x = 26501365
	fmt.Printf("Part 1: %d, Part 2: %d\n", Search(spaces, start, 64), ProcessEquation(101150))
}
