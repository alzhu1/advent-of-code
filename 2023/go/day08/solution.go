package main

import (
	"fmt"
	"os"
	"strings"
)

type Point struct {
	id           string
	patternIndex int
}

type Node struct {
	id, leftId, rightId string
}

func LCM(x, y int) int {
	num := x * y
	gcd := 1

	for x > 0 && y > 0 {
		if x > y {
			x %= y
		} else {
			y %= x
		}
	}

	gcd = max(x, y)
	return num / gcd
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	pattern := input[0]

	startingIds := []string{}

	nodes := make(map[string]Node)
	for _, line := range input[2:] {
		parts := strings.Split(line, " = ")

		dirs := strings.Split(parts[1][1:len(parts[1])-1], ", ")

		nodes[parts[0]] = Node{
			id:      parts[0],
			leftId:  dirs[0],
			rightId: dirs[1],
		}

		if strings.HasSuffix(parts[0], "A") {
			startingIds = append(startingIds, parts[0])
		}
	}

	// Part 1
	currId := "AAA"
	steps := 0
	for i := 0; currId != "ZZZ"; i = (i + 1) % len(pattern) {
		currNode := nodes[currId]
		if pattern[i] == 'L' {
			currId = currNode.leftId
		} else {
			currId = currNode.rightId
		}
		steps++
	}

	// Part 2
	cycleLengths := []int{}

	for _, id := range startingIds {
		currId = id
		steps = 0

		seen := make(map[Point]int)
		for i := 0; true; i = (i + 1) % len(pattern) {
			point := Point{id: currId, patternIndex: i}

			if cycleStartSteps, ok := seen[point]; ok {
				// Technically I don't think this is a general solution
				// You probably need to know the cycle length + location of Z node
				cycleLengths = append(cycleLengths, steps-cycleStartSteps)
				break
			}

			seen[point] = steps

			currNode := nodes[currId]
			if pattern[i] == 'L' {
				currId = currNode.leftId
			} else {
				currId = currNode.rightId
			}
			steps++
		}
	}

	// LCM of cycles to get first point where they all meet
	steps2 := cycleLengths[0]
	for _, cycleLength := range cycleLengths {
		steps2 = LCM(steps2, cycleLength)
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", steps, steps2)
}
