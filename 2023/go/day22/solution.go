package main

import (
	"fmt"
	"log"
	"os"
	"slices"
	"strconv"
	"strings"
)

type Point struct {
	x, y, z int
}

type Brick struct {
	min, max Point
}

func (b Brick) CheckPointInBrick(point Point) bool {
	return b.min.x <= point.x && b.max.x >= point.x && b.min.y <= point.y && b.max.y >= point.y && b.min.z <= point.z && b.max.z >= point.z
}

func (b Brick) ContainsAnyPointInBrick(points []Point) bool {
	// fmt.Println("Checking: ", b, points)
	for _, point := range points {
		if b.CheckPointInBrick(point) {
			return true
		}
	}
	return false
}

func (b Brick) GetPointsAtHeight(z int) []Point {
	points := make([]Point, 0)

	for x := b.min.x; x <= b.max.x; x++ {
		for y := b.min.y; y <= b.max.y; y++ {
			// nextZ := 0
			// if deltaZ < 0 {
			// 	nextZ = b.min.z
			// } else {
			// 	nextZ = b.max.z
			// }

			points = append(points, Point{
				x: x,
				y: y,
				z: z,
			})
		}
	}

	return points
}

func CheckStable(maxZBrickMap map[int][]Brick) {
	// Create the minZBrickMap
	minZBrickMap := make(map[int][]Brick)
	for _, bricks := range maxZBrickMap {
		for _, brick := range bricks {
			if _, ok := minZBrickMap[brick.min.z]; !ok {
				minZBrickMap[brick.min.z] = make([]Brick, 0)
			}
			minZBrickMap[brick.min.z] = append(minZBrickMap[brick.min.z], brick)
		}
	}

	keys := make([]int, 0)
	for k := range minZBrickMap {
		keys = append(keys, k)
	}
	slices.Sort(keys)

	// For each brick:
	//   1. Get points 1 above
	//   2. Get any overlapping bricks
	//   3. For each of those checkBricks:
	//     3a. Get points 1 below
	//     3b. Check if other bricks that are not the current one from step 1 is there
	//     3c. If there are, the checkBrick is supported. If not, the checkBrick is not supported
	//   4. Increase count if all checkBricks are supported

	sum := 0
	for _, z := range keys {
		bricks := minZBrickMap[z]

		for _, brick := range bricks {
			abovePoints := brick.GetPointsAtHeight(brick.max.z + 1)
			checkBricks := make([]Brick, 0)

			potentialBricks, ok := minZBrickMap[brick.max.z+1]
			if !ok {
				// Nothing should exist above, so it's safe to disintegrate
				// fmt.Println("Nothing above: ", brick)
				sum++
				continue
			}

			for _, potentialBrick := range potentialBricks {
				if potentialBrick.ContainsAnyPointInBrick(abovePoints) {
					checkBricks = append(checkBricks, potentialBrick)
				}
			}

			// if len(checkBricks) == 0 {
			// 	// fmt.Println("Nothing directly above: ", brick)
			// 	sum++
			// 	continue
			// }

			overallSupported := true
			for _, checkBrick := range checkBricks {
				supported := false
				belowPoints := checkBrick.GetPointsAtHeight(checkBrick.min.z - 1)

				belowBricks := maxZBrickMap[checkBrick.min.z-1]

				for _, supportCheckBrick := range belowBricks {
					if supportCheckBrick == brick {
						continue
					}

					if supportCheckBrick.ContainsAnyPointInBrick(belowPoints) {
						supported = true
						break
					}
				}

				if !supported {
					overallSupported = false
					break
				}
			}

			if overallSupported {
				// fmt.Println("Should be supported: ", brick)
				sum++
			}
		}
	}

	fmt.Println(sum)
}

func SimulateFall(maxZBrickMap map[int][]Brick) (map[int][]Brick, int) {
	simulatedMap := make(map[int][]Brick)
	for z, bricks := range maxZBrickMap {
		simulatedMap[z] = make([]Brick, 0)
		simulatedMap[z] = append(simulatedMap[z], bricks...)
	}

	keys := make([]int, 0)
	for k := range simulatedMap {
		keys = append(keys, k)
	}
	slices.Sort(keys)

	fallCount := 0
	for _, z := range keys {
		// Grounded
		if z == 1 {
			continue
		}

		bricks := simulatedMap[z]
		delete(simulatedMap, z)

		for _, brick := range bricks {
			// Go down as much as possible
			deltaZ := 0

			for brick.min.z+deltaZ > 1 {
				belowPoints := brick.GetPointsAtHeight(brick.min.z + deltaZ - 1)
				checkBricks, ok := simulatedMap[brick.min.z+deltaZ-1]

				if !ok {
					deltaZ--
				} else {
					canMove := true
					for _, checkBrick := range checkBricks {
						if checkBrick.ContainsAnyPointInBrick(belowPoints) {
							canMove = false
							break
						}
					}

					if canMove {
						deltaZ--
					} else {
						break
					}
				}
			}

			// Change the brick height and re-set in map
			brick.min.z += deltaZ
			brick.max.z += deltaZ

			if deltaZ < 0 {
				fallCount++
			}

			if _, ok := simulatedMap[brick.max.z]; !ok {
				simulatedMap[brick.max.z] = make([]Brick, 0)
			}
			simulatedMap[brick.max.z] = append(simulatedMap[brick.max.z], brick)
		}
	}

	return simulatedMap, fallCount
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	maxZBrickMap := make(map[int][]Brick)
	for _, line := range input {
		parts := strings.Split(line, "~")
		p1, p2 := strings.Split(parts[0], ","), strings.Split(parts[1], ",")

		x1, x1Err := strconv.Atoi(p1[0])
		y1, y1Err := strconv.Atoi(p1[1])
		z1, z1Err := strconv.Atoi(p1[2])
		x2, x2Err := strconv.Atoi(p2[0])
		y2, y2Err := strconv.Atoi(p2[1])
		z2, z2Err := strconv.Atoi(p2[2])
		if x1Err != nil || y1Err != nil || z1Err != nil || x2Err != nil || y2Err != nil || z2Err != nil {
			log.Fatal("Error: ", x1Err, y1Err, z1Err, x2Err, y2Err, z2Err)
		}

		maxZ := max(z1, z2)

		if _, ok := maxZBrickMap[maxZ]; !ok {
			maxZBrickMap[maxZ] = make([]Brick, 0)
		}

		maxZBrickMap[maxZ] = append(maxZBrickMap[maxZ], Brick{
			min: Point{
				x: min(x1, x2),
				y: min(y1, y2),
				z: min(z1, z2),
			},
			max: Point{
				x: max(x1, x2),
				y: max(y1, y2),
				z: maxZ,
			},
		})
	}

	maxZBrickMap, _ = SimulateFall(maxZBrickMap)

	keys := make([]int, 0, len(maxZBrickMap))
	for k := range maxZBrickMap {
		keys = append(keys, k)
	}
	slices.Sort(keys)

	// fmt.Println("Checking falls now")

	stableCount, fellTotal := 0, 0
	for _, key := range keys {
		bricks := make([]Brick, 0)
		bricks = append(bricks, maxZBrickMap[key]...)

		// Count fallers
		for i := range bricks {
			maxZBrickMap[key] = make([]Brick, 0)
			maxZBrickMap[key] = append(maxZBrickMap[key], bricks[0:i]...)
			maxZBrickMap[key] = append(maxZBrickMap[key], bricks[i+1:]...)

			_, count := SimulateFall(maxZBrickMap)

			if count == 0 {
				stableCount++
			}
			fellTotal += count
		}

		maxZBrickMap[key] = make([]Brick, 0)
		maxZBrickMap[key] = append(maxZBrickMap[key], bricks...)
	}
	fmt.Printf("Part 1: %d, Part 2: %d\n", stableCount, fellTotal)
}
