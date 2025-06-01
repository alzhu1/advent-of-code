package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func GetEndValues(sequence []int) (int, int) {
	diffs := make([]int, 0)
	constant := true

	for i := range len(sequence) - 1 {
		diffs = append(diffs, sequence[i+1]-sequence[i])

		if sequence[i] != sequence[i+1] {
			constant = false
		}
	}

	if constant {
		return sequence[0], sequence[0]
	}

	headDiff, tailDiff := GetEndValues(diffs)
	return sequence[0] - headDiff, sequence[len(sequence)-1] + tailDiff
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	sum, sum2 := 0, 0
	for _, line := range input {
		parts := strings.Split(line, " ")

		sequence := make([]int, 0)
		for _, part := range parts {
			num, err := strconv.Atoi(part)
			if err != nil {
				log.Fatal("Error: ", err)
			}
			sequence = append(sequence, num)
		}

		head, tail := GetEndValues(sequence)
		sum += tail
		sum2 += head
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}
