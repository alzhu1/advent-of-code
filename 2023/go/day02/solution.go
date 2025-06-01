package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	sum, sum2 := 0, 0
	for _, value := range input {
		gameId, countMap := process(value)

		if countMap["red"] <= 12 && countMap["green"] <= 13 && countMap["blue"] <= 14 {
			sum += gameId
		}

		power := 1
		for _, v := range countMap {
			power *= v
		}
		sum2 += power
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}

func process(line string) (int, map[string]int) {
	// Omit first word as it's just "Game"
	fields := strings.Fields(line)[1:]

	gameId, _ := strconv.Atoi(strings.TrimSuffix(fields[0], ":"))

	sets := strings.Split(strings.Join(fields[1:], " "), "; ")

	countMap := map[string]int{
		"red":   0,
		"green": 0,
		"blue":  0,
	}

	for _, set := range sets {
		parts := strings.SplitSeq(set, ", ")

		for part := range parts {
			parsedPart := strings.Fields(part)
			num, _ := strconv.Atoi(parsedPart[0])
			countMap[parsedPart[1]] = max(countMap[parsedPart[1]], num)
		}
	}

	return gameId, countMap
}
