package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	sum, sum2 := 0, 0
	for _, value := range input {
		s1, s2 := process(value)
		sum += s1
		sum2 += s2
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}

func process(line string) (int, int) {
	wordToNum := map[string]int{
		"one":   1,
		"two":   2,
		"three": 3,
		"four":  4,
		"five":  5,
		"six":   6,
		"seven": 7,
		"eight": 8,
		"nine":  9,
	}

	numDigit, numDigitPos := 0, len(line)
	numDigit2, numDigitPos2 := 0, -1

	wordDigit, wordDigitPos := 0, len(line)
	wordDigit2, wordDigitPos2 := 0, -1

	for i := range line {
		start, end := i, len(line)-i

		// Word digit search
		for k, v := range wordToNum {
			// First digit
			if wordDigit == 0 && strings.HasPrefix(line[start:], k) {
				wordDigit = v
				wordDigitPos = start
			}

			// if wordDigit == 0 && start+len(k) <= len(line) {
			//     runeSlice := line[start : start+len(k)]

			//     if val, ok := wordToNum[runeSlice]; ok {
			//         wordDigit = val
			//         wordDigitPos = start
			//     }
			// }

			// Last digit
			if wordDigit2 == 0 && strings.HasSuffix(line[:end], k) {
				wordDigit2 = v
				wordDigitPos2 = end - len(k)
			}
			// if wordDigit2 == 0 && end-len(k) > 0 {
			//     runeSlice := line[end-len(k) : end]

			//     if val, ok := wordToNum[runeSlice]; ok {
			//         wordDigit2 = val
			//         wordDigitPos2 = end - len(k)
			//     }
			// }
		}

		// Num digit search
		head, tail := line[start], line[end-1]
		if numDigit == 0 && head >= '0' && head <= '9' {
			numDigit = int(head - '0')
			numDigitPos = start
		}
		if numDigit2 == 0 && tail >= '0' && tail <= '9' {
			numDigit2 = int(tail - '0')
			numDigitPos2 = end - 1
		}
	}

	lineSum := 0
	if numDigitPos < wordDigitPos {
		lineSum += numDigit * 10
	} else {
		lineSum += wordDigit * 10
	}

	if numDigitPos2 > wordDigitPos2 {
		lineSum += numDigit2
	} else {
		lineSum += wordDigit2
	}

	return numDigit*10 + numDigit2, lineSum
}
