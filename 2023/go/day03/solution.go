package main

import (
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

type pair struct {
	i int
	j int
}

type point struct {
	i int
	j int
	c byte
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	sum, sum2 := process(input)
	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}

func process(input []string) (int, int) {
	symbolIndices := make([]point, 1)

	for i := range input {
		for j := range input[i] {
			c := input[i][j]

			if c != '.' && (c < '0' || c > '9') {
				symbolIndices = append(symbolIndices, point{
					i: i,
					j: j,
					c: c,
				})
			}
		}
	}

	sort.Slice(symbolIndices, func(i, j int) bool {
		if symbolIndices[i].c == '*' {
			return true
		}

		if symbolIndices[j].c == '*' {
			return false
		}

		return symbolIndices[i].i < symbolIndices[j].i && symbolIndices[i].j < symbolIndices[j].j
	})

	numMap := make(map[pair]int)

	gearSum := 0
	for _, index := range symbolIndices {
		symbolI, symbolJ, symbolC := index.i, index.j, index.c

		if symbolI == 0 && symbolJ == 0 {
			continue
		}

		partCount := 0
		gearFactor := 1

		for i := symbolI - 1; i <= symbolI+1; i++ {
			for j := symbolJ - 1; j <= symbolJ+1; j++ {
				if i < 0 || i >= len(input) || j < 0 || j >= len(input[i]) {
					continue
				}

				if input[i][j] >= '0' && input[i][j] <= '9' {
					start, end := j+1, j
					for jj := j; jj >= 0 && input[i][jj] >= '0' && input[i][jj] <= '9'; jj-- {
						start--
					}

					for jj := j; jj < len(input[i]) && input[i][jj] >= '0' && input[i][jj] <= '9'; jj++ {
						end++
					}

					num, _ := strconv.Atoi(input[i][start:end])

					// If it didn't exist before, it's a new part
					// Technically I don't think this should've worked?
					// e.g. 2 * gears next to each other should be double counted
					if _, ok := numMap[pair{i: i, j: start}]; !ok {
						partCount++
						gearFactor *= num
						numMap[pair{i: i, j: start}] = num
					}
				}
			}
		}

		if symbolC == '*' && partCount == 2 {
			gearSum += gearFactor
		}
	}

	sum := 0
	for _, v := range numMap {
		sum += v
	}

	return sum, gearSum
}
