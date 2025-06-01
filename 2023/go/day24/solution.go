package main

import (
	"fmt"
	"log"
	"math/big"
	"os"
	"strconv"
	"strings"
)

type Vector struct {
	x, y, z int
}

func DetermineIntersection(h1, h2 [2]Vector, min, max Vector) bool {
	p1, v1 := h1[0], h1[1]
	p2, v2 := h2[0], h2[1]
	// y - y1 = m(x - x1) -> y = m(x - x1) + y1
	// you have a point: x1, y1, and rate of change, x'1, y'1 (m1 = y'1 / x'1)

	// Solve for x given same y: m1(x - x1) + y1 = m2(x - x2) + y2
	// m1x - m1x1 + y1 = m2x - m2x2 + y2
	// (m1 - m2)x = m1x1 - m2x2 + y2 - y1
	// x = (m1x1 - m2x2 + y2 - y1) / (m1 - m2)

	// So solve for this X, get a rough approximate value. Same for y
	// If it's not within min and max, return 0
	// Otherwise, check if both equations h1 and h2 reach this number with positive time
	//   Should be enough to check curr hpx, sign of hvx, and if it will eventually reach target x

	m1 := big.NewRat(int64(v1.y), int64(v1.x))
	m2 := big.NewRat(int64(v2.y), int64(v2.x))

	// fmt.Println(m1, m2, v1, v2)
	if m1.Cmp(m2) == 0 {
		// fmt.Println("Parallel: ", m1, m2, v1, v2)
		return false
	}

	yDiff := p2.y - p1.y

	m1x1 := big.NewRat(int64(p1.x), 1)
	m1x1 = m1x1.Mul(m1x1, m1)

	m2x2 := big.NewRat(int64(p2.x), 1)
	m2x2 = m2x2.Mul(m2x2, m2)

	ratX := big.NewRat(1, 1).Sub(m1x1, m2x2)
	ratX = ratX.Add(ratX, big.NewRat(int64(yDiff), 1))

	den := big.NewRat(1, 1).Sub(m1, m2)

	ratX = ratX.Quo(ratX, den)

	// y = m(x - x1) + y1
	ratY := big.NewRat(int64(p1.y), 1)
	ratY = ratY.Add(ratY, big.NewRat(1, 1).Mul(m1, ratX))
	ratY = ratY.Sub(ratY, m1x1)

	solutionX, _ := ratX.Float64()
	solutionY, _ := ratY.Float64()

	// fmt.Println("Solution (x, y): ", m1x1, solutionX, solutionY, exactX, exactY)

	if float64(min.x) < solutionX && float64(max.x) > solutionX && float64(min.y) < solutionY && float64(max.y) > solutionY {
		// Now we need to check if the crossing is forwards in time for both

		// solve for t so that px1 + vx1 * t = solutionX
		t1 := big.NewRat(int64(p1.x), 1)
		t1 = t1.Sub(ratX, t1)
		t1 = t1.Quo(t1, big.NewRat(int64(v1.x), 1))

		t2 := big.NewRat(int64(p2.x), 1)
		t2 = t2.Sub(ratX, t2)
		t2 = t2.Quo(t2, big.NewRat(int64(v2.x), 1))

		// fmt.Println(h1, h2, t1, t2)

		if t1.Sign() > 0 && t2.Sign() > 0 {
			return true
		}
	}

	return false
}

// Another idea (rough n^2 inspo from reddit)
// Do check on vx and vy only, solve for ratX and ratY
// Ignore parallel lines first, just get a list of ratX and ratY that matches for all xy-plane non-parallel lines
// For these vx,vy pairs, then do a vz check
// Technically still O(n^3), but effectively would be n^2 cause the possibility of matches across all hailstones is unlikely

// Credit to Reddit for this main observation:
//  If you subtract rock velocity v from hailstone, all the lines must now be concurrent (at rock pos for t = 0)
func GetIntersectingPoint(h1, h2 [2]Vector, v Vector, ignoreZ bool) (*big.Rat, *big.Rat, *big.Rat) {
	p1, v1 := h1[0], h1[1]
	p2, v2 := h2[0], h2[1]

	// Offset by the input velocity, to simulate the path that will intersect with the start position
	v1.x -= v.x
	v1.y -= v.y
	v1.z -= v.z
	v2.x -= v.x
	v2.y -= v.y
	v2.z -= v.z

	// x1 + v1t1 vs x2 + v2t2, ditto for y and z, solve for t1 and t2
	// Once solved, find the x, y, and z, and return that for position

	// x1 + vx1t1 = x2 + vx2t2 -> vx1t1 - vx2t2 = x2 - x1
	// y1 + vy1t1 = y2 + vy2t2 -> vy1t1 - vy2t2 = y2 - y1

	// (vx1/vx2)t1 - t2 = (x2 - x1) / vx2
	// (vy1/vy2)t1 - t2 = (y2 - y1) / vy2
	// (vx1/vx2 - vy1/vy2)*t1 = (x2 - x1) / vx2 - (y2 - y1) / vy2
	// t1 = ((x2 - x1) / vx2 - (y2 - y1) / vy2) / (vx1/vx2 - vy1/vy2)

	// Find a non-parallel plane
	// fmt.Println(v1, v2)

	dim1, dim2 := [4]int{p1.x, v1.x, p2.x, v2.x}, [4]int{p1.y, v1.y, p2.y, v2.y}
	if v1.y*v2.x == v2.y*v1.x {
		if ignoreZ {
			return nil, nil, nil
		}

		// Try x and z
		dim2 = [4]int{p1.z, v1.z, p2.z, v2.z}

		if v1.z*v2.x == v2.z*v1.x {
			// Try y and z
			dim1 = [4]int{p1.y, v1.y, p2.y, v2.y}

			if v1.z*v2.y == v2.z*v1.y {
				// All dimensions are parallel
				return nil, nil, nil
			}
		}
	}

	p1Dim1, v1Dim1, p2Dim1, v2Dim1 := dim1[0], dim1[1], dim1[2], dim1[3]
	p1Dim2, v1Dim2, p2Dim2, v2Dim2 := dim2[0], dim2[1], dim2[2], dim2[3]

	// Special cases exist for velocities = vx2 = 0 or vy2 = 0
	// vx2 = 0: vx1t1 = x2 - x1, t1 = (x2 - x1) / vx1
	// vy2 = 0: vy1t1 = y2 - y1
	// var t1 *big.Rat
	t1 := big.NewRat(1, 1)
	if v2Dim1 == 0 {
		t1 = big.NewRat(int64(p2Dim1-p1Dim1), int64(v1Dim1))
	} else if v2Dim2 == 0 {
		t1 = big.NewRat(int64(p2Dim2-p1Dim2), int64(v1Dim2))
	} else {

		dim1Piece := big.NewRat(int64(p2Dim1-p1Dim1), int64(v2Dim1))
		dim2Piece := big.NewRat(int64(p2Dim2-p1Dim2), int64(v2Dim2))

		veloDiff := big.NewRat(int64(v1Dim1), int64(v2Dim1))
		veloDiff = veloDiff.Sub(veloDiff, big.NewRat(int64(v1Dim2), int64(v2Dim2)))

		t1 = t1.Sub(dim1Piece, dim2Piece).Quo(t1, veloDiff)
	}

	// Calculate here
	ratX := big.NewRat(int64(v1.x), 1)
	ratX = ratX.Mul(ratX, t1).Add(ratX, big.NewRat(int64(p1.x), 1))

	ratY := big.NewRat(int64(v1.y), 1)
	ratY = ratY.Mul(ratY, t1).Add(ratY, big.NewRat(int64(p1.y), 1))

	ratZ := big.NewRat(int64(v1.z), 1)
	ratZ = ratZ.Mul(ratZ, t1).Add(ratZ, big.NewRat(int64(p1.z), 1))

	// fmt.Println("Intersecting: ", ratX, ratY, ratZ)

	return ratX, ratY, ratZ
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	hailstones := make([][2]Vector, 0)
	for _, line := range input {
		parts := strings.Split(line, " @ ")
		position, velocity := strings.Split(parts[0], ","), strings.Split(parts[1], ",")

		px, pxErr := strconv.Atoi(strings.Trim(position[0], " "))
		py, pyErr := strconv.Atoi(strings.Trim(position[1], " "))
		pz, pzErr := strconv.Atoi(strings.Trim(position[2], " "))
		vx, vxErr := strconv.Atoi(strings.Trim(velocity[0], " "))
		vy, vyErr := strconv.Atoi(strings.Trim(velocity[1], " "))
		vz, vzErr := strconv.Atoi(strings.Trim(velocity[2], " "))

		if pxErr != nil || pyErr != nil || pzErr != nil || vxErr != nil || vyErr != nil || vzErr != nil {
			log.Fatal("Error: ", pxErr, pyErr, pzErr, vxErr, vyErr, vzErr)
		}

		hailstones = append(hailstones, [2]Vector{
			{x: px, y: py, z: pz},
			{x: vx, y: vy, z: vz},
		})
	}

	// fmt.Println(hailstones)

	minDimension := 200000000000000
	maxDimension := 400000000000000

	sum := 0
	for i := range hailstones {
		for j := i + 1; j < len(hailstones); j++ {
			hailstone1, hailstone2 := hailstones[i], hailstones[j]

			res := DetermineIntersection(hailstone1, hailstone2, Vector{x: minDimension, y: minDimension}, Vector{x: maxDimension, y: maxDimension})

			if res {
				sum++
			}
		}
	}

	veloRange := 1000
	var finalX, finalY, finalZ *big.Rat

	// First, check for vx and vy that could potentially work
	// This means returning x and y positions that match for hailstone pairs, or nil (parallel, we can ignore)
	for x := -veloRange; x <= veloRange; x++ {
		for y := -veloRange; y <= veloRange; y++ {
			var ratX, ratY *big.Rat

			worksXY := true
			for i := range len(hailstones) - 1 {
				// Ignore parallels, we just care about same XY points
				nextRatX, nextRatY, _ := GetIntersectingPoint(hailstones[i], hailstones[i+1], Vector{x: x, y: y}, true)
				if nextRatX == nil || nextRatY == nil {
					continue
				}

				if (ratX != nil && nextRatX.Cmp(ratX) != 0) || (ratY != nil && ratY.Cmp(nextRatY) != 0) {
					worksXY = false
					break
				}
				ratX = nextRatX
				ratY = nextRatY
			}

			if worksXY {
				fmt.Println("Found a possible XY: ", x, y)

				// With a potential vx and vy found, now let's look through vz
				// This should return the z pos for us assuming everything matches
				for z := -veloRange; z <= veloRange; z++ {
					var ratZ *big.Rat

					works := true
					for i := range len(hailstones) - 1 {
						_, _, nextRatZ := GetIntersectingPoint(hailstones[i], hailstones[i+1], Vector{x: x, y: y, z: z}, false)

						if nextRatZ == nil {
							works = false
							break
						}

						if ratZ != nil && ratZ.Cmp(nextRatZ) != 0 {
							works = false
							break
						}

						ratZ = nextRatZ
					}

					if works {
						fmt.Println("Found: ", ratX, ratY, ratZ)
						finalX = ratX
						finalY = ratY
						finalZ = ratZ
						goto done
					}
				}
			}
		}
	}
done:
	res := big.NewRat(1, 1)
	res = res.Add(finalX, finalY).Add(res, finalZ)

	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, res.Num())
}
