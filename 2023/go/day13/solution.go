package main

import (
	"fmt"
	"os"
	"strings"
)

func RotateMatrix(matrix []string) []string {
	newMatrix := make([]string, 0)

	for j := range len(matrix[0]) {
		var builder strings.Builder
		for i := len(matrix) - 1; i >= 0; i-- {
			builder.WriteByte(matrix[i][j])
		}
		newMatrix = append(newMatrix, builder.String())
	}

	return newMatrix
}

func ValidateReflectionLine(matrix []string, i int, smudgeCheck bool) bool {
	// We need to check if this works as expected
	for ii := 0; i-ii >= 0 && i+1+ii < len(matrix); ii++ {
		currCheck, nextCheck := matrix[i-ii], matrix[i+1+ii]

		diffCount := 0
		for j := range len(matrix[i]) {
			if currCheck[j] != nextCheck[j] {
				diffCount++
			}
		}

		if currCheck != nextCheck {
			// Allow it to continue if smudge check
			if smudgeCheck && diffCount == 1 {
				smudgeCheck = false
			} else {
				return false
			}
		}
	}

	// If smudgeCheck is true, that means we didn't find a valid smudge reflector
	return !smudgeCheck
}

func CheckReflection(matrix []string) (int, int) {
	res, smudge := -1, -1
	for i := range len(matrix) - 1 {
		currRow, nextRow := matrix[i], matrix[i+1]

		if currRow == nextRow && ValidateReflectionLine(matrix, i, false) {
			res = i + 1
		}

		// Check for smudge
		if ValidateReflectionLine(matrix, i, true) {
			smudge = i + 1
		}
	}

	return res, smudge
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	// Add extra empty string for it to still work
	input = append(input, "")
	matrix := make([]string, 0)
	sum, sum2 := 0, 0
	for _, line := range input {
		if len(line) > 0 {
			matrix = append(matrix, line)
		} else {
			// Check reflection horizontally
			res, smudge := CheckReflection(matrix)
			res *= 100
			smudge *= 100

			res2, smudge2 := CheckReflection(RotateMatrix(matrix))

			sum += max(res, res2)
			sum2 += max(smudge, smudge2)

			matrix = make([]string, 0)
		}
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}
