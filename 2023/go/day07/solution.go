package main

import (
	"fmt"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
)

type Play struct {
	hand string
	bid  int
}

func (p Play) GetStrength(withJokers bool) int {
	cardMap := make(map[byte]int)
	for i := range p.hand {
		if _, ok := cardMap[p.hand[i]]; !ok {
			cardMap[p.hand[i]] = 0
		}

		cardMap[p.hand[i]]++
	}

	// Greedy alg: we can always add J count to the highest card count
	if jCount, ok := cardMap['J']; ok && withJokers {
		mostCard, mostCount := byte('z'), 0
		for k, v := range cardMap {
			if k != 'J' && v > mostCount {
				mostCard = k
				mostCount = v
			}
		}

		delete(cardMap, 'J')
		cardMap[mostCard] += jCount
	}

	// Five of a kind
	if len(cardMap) == 1 {
		return 0
	}

	if len(cardMap) == 2 {
		for _, v := range cardMap {
			// Four of a kind
			if v == 4 {
				return 1
			}

			// Full house
			if v == 3 {
				return 2
			}
		}

		panic("Not reachable")
	}

	if len(cardMap) == 3 {
		pairCount := 0
		for _, v := range cardMap {
			// Three of a kind
			if v == 3 {
				return 3
			}

			// 2 pair
			if v == 2 {
				pairCount++
			}
		}

		if pairCount == 2 {
			return 4
		}

		panic("Not reachable")
	}

	// One pair
	if len(cardMap) == 4 {
		return 5
	}

	// High card
	return 6
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	plays := []Play{}

	for _, line := range input {
		parts := strings.Fields(line)
		hand, bidStr := parts[0], parts[1]

		bid, err := strconv.Atoi(bidStr)
		if err != nil {
			log.Fatal("Error: ", err)
		}

		plays = append(plays, Play{hand: hand, bid: bid})
	}

	sum, sum2 := GetWinnings(plays, false), GetWinnings(plays, true)
	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}

func GetWinnings(plays []Play, withJoker bool) int {
	// Sorting fn
	PlayCompare := func(withJokers bool) func(i, j int) bool {
		highCardMap := map[byte]int{
			'A': 0,
			'K': 1,
			'Q': 2,
			'J': 3,
			'T': 4,
			'9': 5,
			'8': 6,
			'7': 7,
			'6': 8,
			'5': 9,
			'4': 10,
			'3': 11,
			'2': 12,
		}
		if withJokers {
			highCardMap['J'] = 999
		}

		return func(i, j int) bool {
			play1, play2 := plays[i], plays[j]

			strength1, strength2 := play1.GetStrength(withJokers), play2.GetStrength(withJokers)

			if strength1 != strength2 {
				return strength1 < strength2
			}

			// Second compare condition
			for i := range 5 {
				if play1.hand[i] != play2.hand[i] {
					return highCardMap[play1.hand[i]] < highCardMap[play2.hand[i]]
				}
			}

			panic("Not reachable")
		}
	}

	sort.SliceStable(plays, PlayCompare(withJoker))

	sum := 0
	for i, play := range plays {
		sum += play.bid * (len(plays) - i)
	}

	return sum
}
