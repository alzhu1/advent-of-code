package main

import (
	"fmt"
	"log"
	"os"
	"slices"
	"strconv"
	"strings"
)

func ConsolidateNonogram(nonogramLine string) string {
	splitNonogram := strings.Split(nonogramLine, ".")
	nonogramPieces := make([]string, 0)
	for _, splitPiece := range splitNonogram {
		if len(splitPiece) > 0 {
			nonogramPieces = append(nonogramPieces, splitPiece)
		}
	}
	return strings.Join(nonogramPieces, ".")
}

func CheckValidNonogram(nonogramLine string, nums []int) bool {
	i := 0

	for i < len(nonogramLine) {
		if nonogramLine[i] == '#' {
			if len(nums) == 0 {
				return false
			}

			allHashes := i+nums[0] <= len(nonogramLine) && !strings.Contains(nonogramLine[i:i+nums[0]], ".")
			validEnd := i+nums[0] == len(nonogramLine) || (i+nums[0] < len(nonogramLine) && nonogramLine[i+nums[0]] == '.')

			if !(allHashes && validEnd) {
				return false
			}

			i += nums[0] - 1
			nums = nums[1:]
		}

		i++
	}

	return len(nums) == 0
}

// I guess the caching makes sense, but am not entirely sure why it works
// If you iterate from the start and fill in ?'s, in theory you would've processed
// all the consolidated cases at that point, so there's no net new work? idk
func PermuteNonogramLine(nonogramLine string, originalNums []int, cache map[string]int) int {
	nums := originalNums
	// fmt.Println("[Permute] Begin: ", nonogramLine, originalNums)
	qCount := 0
	for _, c := range nonogramLine {
		if c == '?' {
			qCount++
		}
	}

	nonogramLine = ConsolidateNonogram(nonogramLine)

	if qCount == 0 {
		if CheckValidNonogram(nonogramLine, originalNums) {
			cache[nonogramLine] = 1
			return 1
		}

		cache[nonogramLine] = 0
		return 0
	}

	if permutations, ok := cache[nonogramLine]; ok {
		return permutations
	}

	editableLine := []byte(nonogramLine)

	for i := 0; i < len(editableLine); i++ {
		if editableLine[i] == '.' {
			continue
		}

		if editableLine[i] == '#' {
			if len(nums) == 0 {
				cache[nonogramLine] = 0
				return 0
			}

			allHashes := i+nums[0] <= len(nonogramLine) && !strings.Contains(nonogramLine[i:i+nums[0]], ".")
			validEnd := i+nums[0] == len(nonogramLine) || (i+nums[0] < len(nonogramLine) && nonogramLine[i+nums[0]] != '#')

			// fmt.Println(allHashes, validEnd, string(editableLine), i)

			// Contradiction found already
			if !(allHashes && validEnd) {
				cache[nonogramLine] = 0
				return 0
			}

			for ii := i; ii < i+nums[0]; ii++ {
				editableLine[ii] = '#'
			}

			if i+nums[0] < len(editableLine) {
				editableLine[i+nums[0]] = '.'
			}

			i += nums[0] - 1
			nums = nums[1:]
			continue
		}

		if editableLine[i] == '?' {
			editableLine[i] = '.'
			dotPermute := PermuteNonogramLine(string(editableLine), originalNums, cache)
			editableLine[i] = '#'
			hashPermute := PermuteNonogramLine(string(editableLine), originalNums, cache)

			cache[nonogramLine] = dotPermute + hashPermute

			return dotPermute + hashPermute
		}
	}

	if CheckValidNonogram(string(editableLine), originalNums) {
		cache[nonogramLine] = 1
		return 1
	}

	cache[nonogramLine] = 0
	return 0
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	sum, sum2 := 0, 0
	for _, line := range input {
		parts := strings.Fields(line)
		nonogram, numsStr := parts[0], strings.Split(parts[1], ",")

		nums := make([]int, 0)
		for _, numStr := range numsStr {
			num, err := strconv.Atoi(numStr)
			if err != nil {
				log.Fatal("Error: ", err)
			}
			nums = append(nums, num)
		}

		cache := make(map[string]int)
		sum += PermuteNonogramLine(nonogram, nums, cache)
		// sum += ProcessNonogramLine(nonogram, nums)

		// Part 2
		var builder strings.Builder
		for range 4 {
			builder.WriteString(nonogram)
			builder.WriteByte('?')
		}
		builder.WriteString(nonogram)

		nonogram = builder.String()
		nums = slices.Repeat(nums, 5)

		nonogram = ConsolidateNonogram(nonogram)

		cache2 := make(map[string]int)
		sum2 += PermuteNonogramLine(nonogram, nums, cache2)
		// sum2 += ProcessNonogramLine(nonogram, nums)
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}
