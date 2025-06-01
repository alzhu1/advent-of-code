package main

import "strings"

// The stuff here is for more math-y/logic techniques for solving nonograms

func FillInEdges(editableNonogramPtr *[]byte, numsPtr *[]int) {
	editableNonogram := *editableNonogramPtr
	nums := *numsPtr

	// fmt.Println("Start edges: ", string(editableNonogram), nums)

	// Check for first guaranteed pieces
	for i := 0; i < len(editableNonogram); i++ {
		if len(nums) == 0 {
			continue
		}

		if editableNonogram[i] == '?' {
			// Check if we can mark it as empty
			count := 0
			for i+count < len(editableNonogram) && editableNonogram[i+count] != '#' {
				count++
			}

			// fmt.Println(editableNonogram[i:i+count], nums)

			if i+count < len(editableNonogram) && count == nums[0] && strings.Contains(string(editableNonogram[i:i+count]), strings.Repeat("?", nums[0])) {
				editableNonogram[i] = '.'
			}

			// Check if we can mark it as #
			nonDotCount := 0
			for i+nonDotCount < len(editableNonogram) && editableNonogram[i+nonDotCount] != '.' {
				nonDotCount++
			}

			hashExists := strings.Contains(string(editableNonogram[i:i+nonDotCount]), "#")
			if hashExists {
				// If the whole non dot substring (so ? and #) contains a # and is exactly nums[0]
				if nonDotCount == nums[0] {
					// fmt.Println("FORWARD PASS before: ", string(editableNonogram), nums)
					editableNonogram[i] = '#'
					// fmt.Println("FORWARD PASS after: ", string(editableNonogram), nums)
				}
			}

			// If still a ?, break
			if editableNonogram[i] == '?' {
				break
			}
		}

		if editableNonogram[i] == '.' {
			continue
		}

		num := nums[0]
		nums = nums[1:]
		for range num {
			editableNonogram[i] = '#'
			i++
		}

		if i < len(editableNonogram) {
			editableNonogram[i] = '.'
		}
	}

	// Check for last guaranteed pieces
	for i := len(editableNonogram) - 1; i >= 0; i-- {
		// fmt.Println("Backwards: ", string(editableNonogram))

		if len(nums) == 0 {
			continue
		}

		if editableNonogram[i] == '?' {
			// Check if we can mark it as empty
			count := 0
			for i-count >= 0 && editableNonogram[i-count] != '#' {
				count++
			}

			// fmt.Println(i, count, i-count)

			// fmt.Println("BBB", string(editableNonogram[i-count+1:i+1]), nums)

			if i-count >= 0 && count == nums[len(nums)-1] && strings.Contains(string(editableNonogram[i-count+1:i+1]), strings.Repeat("?", nums[len(nums)-1])) {
				editableNonogram[i] = '.'
			}

			// Check if we can mark it as #
			nonDotCount := 0
			for i-nonDotCount >= 0 && editableNonogram[i-nonDotCount] != '.' {
				nonDotCount++
			}

			// If the whole non dot substring (so ? and #) contains a # and is exactly nums[len(nums)-1]
			if nonDotCount == nums[len(nums)-1] && strings.Contains(string(editableNonogram[i-nonDotCount+1:i+1]), "#") {
				// fmt.Println("BACKWARD PASS before: ", string(editableNonogram), nums)
				editableNonogram[i] = '#'
				// fmt.Println("BACKWARD PASS before: ", string(editableNonogram), nums)
			}

			// If still a ?, break
			if editableNonogram[i] == '?' {
				break
			}
		}

		if editableNonogram[i] == '.' {
			continue
		}

		num := nums[len(nums)-1]
		nums = nums[:len(nums)-1]
		for range num {
			editableNonogram[i] = '#'
			i--
		}

		if i >= 0 {
			editableNonogram[i] = '.'
		}
	}
}

func ProcessNonogramLine(nonogramLine string, nums []int) int {
	originalNums := nums

	// https://en.wikipedia.org/wiki/Nonogram#Mathematical_approach
	numSum := 0
	for _, num := range nums {
		numSum += num + 1
	}
	numSum -= 1

	// fmt.Println("Processingg: ", simpleNonogram)

	if numSum > len(nonogramLine) {
		panic("This should never happen")
	} else if numSum == len(nonogramLine) {
		// TODO: Return 1
		// fmt.Println("There is only 1 solution: ", simpleNonogram, nums)
		return 1
	}

	editableNonogram := []byte(nonogramLine)

	// fmt.Println(simpleNonogram, len(simpleNonogram), numSum)
	// fmt.Println("Before pass: ", string(editableNonogram), nums)

	// Pack things in
	minDiff := len(nonogramLine) - numSum
	for numI, num := range nums {
		if num-minDiff <= 0 {
			continue
		}

		// fmt.Println("Startt pass: ", string(editableNonogram), nums, num)

		// This one is fillable
		start, end := 0, len(editableNonogram)-1
		startSlice, endSlice := nums[:numI], nums[numI+1:]

		for _, startFillNum := range startSlice {
			start += startFillNum + 1
		}
		for j := len(endSlice) - 1; j >= 0; j-- {
			end -= (endSlice[j] + 1)
		}

		editablePiece := editableNonogram[start : end+1]

		// fmt.Println("Editable piece: ", string(editablePiece), start, end)

		for i := num - 1; i >= minDiff; i-- {
			editablePiece[i] = '#'
		}

		// fmt.Println("Iterrr pass: ", string(editableNonogram), nums)
	}

	// fmt.Println("Preeee pass: ", string(editableNonogram), nums)
	FillInEdges(&editableNonogram, &nums)

	// fmt.Println("Afterr pass: ", string(editableNonogram), nums)

	splitNonogram := strings.Split(string(editableNonogram), ".")
	processedPieces := make([]string, 0)
	hasQ := false
	for _, splitPiece := range splitNonogram {
		if len(splitPiece) > 0 {
			processedPieces = append(processedPieces, splitPiece)

			if strings.Contains(splitPiece, "?") {
				hasQ = true
			}
		}
	}

	if !hasQ {
		return 1
	}

	// qCount := 0
	// for i := range len(editableNonogram) {
	// 	if editableNonogram[i] == '?' {
	// 		qCount++
	// 	}
	// }
	// fmt.Println("qCount: ", qCount, processedPieces)

	cache := make(map[string]int)
	return PermuteNonogramLine(strings.Join(processedPieces, "."), originalNums, cache)
}
