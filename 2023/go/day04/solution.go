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

	cardCountMap := map[int]int{
		1: 1,
	}

	sum, sum2 := 0, 0
	for _, value := range input {
		cardNum, numWin, cardSum := process(value)

		if _, ok := cardCountMap[cardNum]; !ok {
			cardCountMap[cardNum] = 1
		}

		for i := 1; i <= numWin; i++ {
			if _, ok := cardCountMap[cardNum+i]; !ok {
				cardCountMap[cardNum+i] = 1
			}

			cardCountMap[cardNum+i] += cardCountMap[cardNum]
		}

		sum += cardSum
	}

	for _, v := range cardCountMap {
		sum2 += v
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}

func process(line string) (int, int, int) {
	pieces := strings.Split(line, ": ")
	cardNum, _ := strconv.Atoi(strings.Fields(pieces[0])[1])
	card := strings.Split(pieces[1], " | ")

	winners, nums := strings.Fields(card[0]), strings.Fields(card[1])

	winnerMap := make(map[string]bool)
	for _, winner := range winners {
		winnerMap[winner] = true
	}

	n := 0
	for _, num := range nums {
		if _, ok := winnerMap[num]; ok {
			n++
		}
	}

	if n == 0 {
		return cardNum, n, 0
	}

	return cardNum, n, 1 << (n - 1)
}
