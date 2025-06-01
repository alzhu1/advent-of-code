package main

import (
	"fmt"
	"os"
	"strings"
)

func EncodeRocks(rockMap map[int]map[int]bool, rowCount int) int {
	encoding := 0

	for row, v := range rockMap {
		for col := range v {
			encoding += row*rowCount + col
		}
	}

	return encoding
}

func GetLoad(rockMap map[int]map[int]bool, rowCount int) int {
	sum := 0
	for row, rowRocks := range rockMap {
		sum += (rowCount - row) * len(rowRocks)
	}

	return sum
}

func DebugRocks(rockMap, staticMap map[int]map[int]bool, rowCount, colCount int) {
	// Debug
	for i := range rowCount {
		for j := range colCount {
			if rockMap[i] != nil && rockMap[i][j] {
				fmt.Print("O")
			} else if staticMap[i] != nil && staticMap[i][j] {
				fmt.Print("#")
			} else {
				fmt.Print(".")
			}
		}
		fmt.Println()
	}
}

// This could probably be way more efficient but eh
func RunCycle(rockMap, staticMap map[int]map[int]bool, deltaI, deltaJ, rowCount, colCount int) map[int]map[int]bool {
	movedRocks := true
	for movedRocks {
		movedRocks = false

		rockMapNext := make(map[int]map[int]bool)
		for i := range rockMap {
			for j := range rockMap[i] {
				nextI, nextJ := i+deltaI, j+deltaJ

				if nextI >= 0 && nextI < rowCount && nextJ >= 0 && nextJ < colCount {
					rockEmpty := rockMap[nextI] == nil || !rockMap[nextI][nextJ]
					staticEmpty := staticMap[nextI] == nil || !staticMap[nextI][nextJ]

					if rockEmpty && staticEmpty {
						if _, ok := rockMapNext[nextI]; !ok {
							rockMapNext[nextI] = make(map[int]bool)
						}

						rockMapNext[nextI][nextJ] = true
						movedRocks = true
					} else {
						if _, ok := rockMapNext[i]; !ok {
							rockMapNext[i] = make(map[int]bool)
						}

						rockMapNext[i][j] = true
					}
				} else {
					if _, ok := rockMapNext[i]; !ok {
						rockMapNext[i] = make(map[int]bool)
					}

					rockMapNext[i][j] = true
				}
			}
		}

		if len(rockMapNext) == 0 {
			break
		}

		rockMap = rockMapNext
	}

	return rockMap
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	rockMap := make(map[int]map[int]bool)
	staticMap := make(map[int]map[int]bool)

	rowCount, colCount := len(input), len(input[0])

	for i, line := range input {
		for j := 0; j < len(line); j++ {
			if line[j] == '#' {
				if _, ok := staticMap[i]; !ok {
					staticMap[i] = make(map[int]bool)
				}

				staticMap[i][j] = true
			}

			if line[j] == 'O' {
				if _, ok := rockMap[i]; !ok {
					rockMap[i] = make(map[int]bool)
				}
				rockMap[i][j] = true
			}
		}
	}

	cycles := 0
	deltas := []struct{ i, j int }{
		{i: -1, j: 0}, // North
		{i: 0, j: -1}, // West
		{i: 1, j: 0},  // South
		{i: 0, j: 1},  // East
	}

	cache := make(map[int]int)
	loadMap := make(map[int]int)

	load := GetLoad(RunCycle(rockMap, staticMap, -1, 0, rowCount, colCount), rowCount)
	load2 := 0
	for cycles < 1000000000 {
		encoding := EncodeRocks(rockMap, rowCount)
		if prevCycle, ok := cache[encoding]; ok {
			fmt.Println("Found a repeat: ", prevCycle, cycles)

			diff := cycles - prevCycle
			gap := (1000000000 - cycles) / diff * diff

			remaining := 1000000000 - cycles - gap
			load2 = loadMap[prevCycle+remaining]
			break
		}
		cache[encoding] = cycles
		loadMap[cycles] = GetLoad(rockMap, rowCount)

		for _, delta := range deltas {
			rockMap = RunCycle(rockMap, staticMap, delta.i, delta.j, rowCount, colCount)
		}

		cycles++
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", load, load2)
}
