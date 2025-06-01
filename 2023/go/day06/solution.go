package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	times := strings.Fields(strings.Split(input[0], ":")[1])
	distances := strings.Fields(strings.Split(input[1], ":")[1])

	// Part 2
	times = append(times, strings.Join(times, ""))
	distances = append(distances, strings.Join(distances, ""))

	margin := 1
	finalWays := 0
	for i := range times {
		time, e1 := strconv.Atoi(times[i])
		distance, e2 := strconv.Atoi(distances[i])

		if e1 != nil || e2 != nil {
			log.Fatal("Error: ", e1, e2)
		}

		// Minimum time should be the time where remaining time * elapsed time >= dist
		minTime := 0
		for ; (time-minTime)*minTime <= distance; minTime++ {
		}

		// Times are symmetrical, get the range between elapsed time + time - elapsed time
		ways := time - 2*minTime + 1
		if i < len(times)-1 {
			margin *= ways
		} else {
			finalWays = ways
		}
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", margin, finalWays)
}
