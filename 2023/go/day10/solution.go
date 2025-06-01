package main

import (
	"fmt"
	"os"
	"strings"
)

type Point struct {
	i, j, steps int
}

type BetweenPoint struct {
	i1, i2, j1, j2 int
}

func (p Point) GetKey(offset int) int {
	return p.i*offset + p.j
}

func GetKey(i, j, offset int) int {
	return i*offset + j
}

func SearchBetweens(input []string, wallCheck map[int]bool, initialPoint BetweenPoint) (int, error) {
	betweenQueue := []BetweenPoint{initialPoint}

	processed := make(map[BetweenPoint]bool)
	inPointSet := make(map[int]bool)

	keyOffset := len(input[0])

	// Do a BFS on points "in between"
	// This should fill in all spaces within the maze
	for len(betweenQueue) > 0 {
		betweenPoint := betweenQueue[0]
		betweenQueue = betweenQueue[1:]

		if processed[betweenPoint] {
			continue
		}
		processed[betweenPoint] = true

		i1, i2, j1, j2 := betweenPoint.i1, betweenPoint.i2, betweenPoint.j1, betweenPoint.j2

		if (i1 < 0 || i1 >= len(input) || i2 < 0 || i2 >= len(input)) ||
			(j1 < 0 || j1 >= len(input[i1]) || j2 < 0 || j2 >= len(input[i2])) {
			return 0, fmt.Errorf("out of bounds")
		}

		// Add corner points not on wall to set
		if !wallCheck[GetKey(i1, j1, keyOffset)] {
			inPointSet[GetKey(i1, j1, keyOffset)] = true
		}
		if !wallCheck[GetKey(i1, j2, keyOffset)] {
			inPointSet[GetKey(i1, j2, keyOffset)] = true
		}
		if !wallCheck[GetKey(i2, j1, keyOffset)] {
			inPointSet[GetKey(i2, j1, keyOffset)] = true
		}
		if !wallCheck[GetKey(i2, j2, keyOffset)] {
			inPointSet[GetKey(i2, j2, keyOffset)] = true
		}

		// Rules for adding neighbors
		// If the neighbor edge is part of a wall, do not add it to the queue

		// North
		if !(wallCheck[GetKey(i1, j1, keyOffset)] && wallCheck[GetKey(i1, j2, keyOffset)]) ||
			(input[i1][j1] == '|' || input[i1][j1] == 'J' || input[i1][j1] == '7') {
			betweenQueue = append(betweenQueue, BetweenPoint{
				i1: i1 - 1,
				i2: i2 - 1,
				j1: j1,
				j2: j2,
			})
		}

		// West
		if !(wallCheck[GetKey(i1, j1, keyOffset)] && wallCheck[GetKey(i2, j1, keyOffset)]) ||
			(input[i1][j1] == '-' || input[i1][j1] == 'L' || input[i1][j1] == 'J') {
			betweenQueue = append(betweenQueue, BetweenPoint{
				i1: i1,
				i2: i2,
				j1: j1 - 1,
				j2: j2 - 1,
			})
		}

		// South
		if !(wallCheck[GetKey(i2, j2, keyOffset)] && wallCheck[GetKey(i2, j1, keyOffset)]) ||
			(input[i2][j2] == '|' || input[i2][j2] == 'L' || input[i2][j2] == 'F') {
			betweenQueue = append(betweenQueue, BetweenPoint{
				i1: i1 + 1,
				i2: i2 + 1,
				j1: j1,
				j2: j2,
			})
		}

		// East
		if !(wallCheck[GetKey(i2, j2, keyOffset)] && wallCheck[GetKey(i1, j2, keyOffset)]) ||
			(input[i2][j2] == '-' || input[i2][j2] == 'F' || input[i2][j2] == '7') {
			betweenQueue = append(betweenQueue, BetweenPoint{
				i1: i1,
				i2: i2,
				j1: j1 + 1,
				j2: j2 + 1,
			})
		}
	}

	return len(inPointSet), nil
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	startI, startJ := -1, -1
	for i, line := range input {
		for j, c := range line {
			if c == 'S' {
				startI = i
				startJ = j
				break
			}
		}
	}

	fmt.Println(startI, startJ)

	// Search
	queue := make([]Point, 0)
	queue = append(queue, Point{
		i:     startI,
		j:     startJ,
		steps: 0,
	})

	visited := make(map[int]bool)
	farthestPoint := Point{i: -1, j: -1, steps: -1}

	keyOffset := len(input[0])

	for len(queue) > 0 {
		point := queue[0]
		queue = queue[1:]

		i, j, steps := point.i, point.j, point.steps

		if visited[point.GetKey(keyOffset)] {
			continue
		}
		visited[point.GetKey(keyOffset)] = true

		if steps > farthestPoint.steps {
			farthestPoint = point
		}

		n, w, s, e := false, false, false, false

		// North
		if i-1 >= 0 &&
			(steps == 0 || input[i][j] == '|' || input[i][j] == 'L' || input[i][j] == 'J') &&
			(input[i-1][j] == '|' || input[i-1][j] == '7' || input[i-1][j] == 'F') {
			queue = append(queue, Point{i: i - 1, j: j, steps: steps + 1})
			n = true
		}

		// West
		if j-1 >= 0 &&
			(steps == 0 || input[i][j] == '-' || input[i][j] == 'J' || input[i][j] == '7') &&
			(input[i][j-1] == '-' || input[i][j-1] == 'L' || input[i][j-1] == 'F') {
			queue = append(queue, Point{i: i, j: j - 1, steps: steps + 1})
			w = true
		}

		// South
		if i+1 < len(input) &&
			(steps == 0 || input[i][j] == '|' || input[i][j] == '7' || input[i][j] == 'F') &&
			(input[i+1][j] == '|' || input[i+1][j] == 'L' || input[i+1][j] == 'J') {
			queue = append(queue, Point{i: i + 1, j: j, steps: steps + 1})
			s = true
		}

		// East
		if j+1 < len(input[i]) &&
			(steps == 0 || input[i][j] == '-' || input[i][j] == 'L' || input[i][j] == 'F') &&
			(input[i][j+1] == '-' || input[i][j+1] == '7' || input[i][j+1] == 'J') {
			queue = append(queue, Point{i: i, j: j + 1, steps: steps + 1})
			e = true
		}

		// TODO: I hate this
		if steps == 0 {
			temp := []rune(input[i])

			if n && s {
				temp[j] = '|'
			}
			if e && w {
				temp[j] = '-'
			}
			if n && e {
				temp[j] = 'L'
			}
			if n && w {
				temp[j] = 'J'
			}
			if s && w {
				temp[j] = '7'
			}
			if s && e {
				temp[j] = 'F'
			}

			input[i] = string(temp)

			fmt.Println("S becomes: ", string(temp[j]))
		}
	}

	fmt.Println(farthestPoint)

	// TODO: Figure this out programmatically

	inTiles, err := SearchBetweens(input, visited, BetweenPoint{
		i1: startI,
		i2: startI + 1,
		j1: startJ,
		j2: startJ + 1,
	})

	if err == nil {
		fmt.Printf("Part 1: %d, Part 2: %d\n", farthestPoint.steps, inTiles)
	}
}
