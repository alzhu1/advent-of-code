package main

import (
	"container/heap"
	"fmt"
	"os"
	"strings"
)

type Point struct {
	i, j, steps, index, deltaI, deltaJ, straightCount int
}

// https://pkg.go.dev/container/heap#example__priorityQueue

// A PriorityQueue implements heap.Interface and holds Items.
type PriorityQueue []*Point

func (pq PriorityQueue) Len() int { return len(pq) }

func (pq PriorityQueue) Less(i, j int) bool {
	return pq[i].steps < pq[j].steps
}

func (pq PriorityQueue) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
	pq[i].index = i
	pq[j].index = j
}

func (pq *PriorityQueue) Push(x any) {
	n := len(*pq)
	item := x.(*Point)
	item.index = n
	*pq = append(*pq, item)
}

func (pq *PriorityQueue) Pop() any {
	old := *pq
	n := len(old)
	item := old[n-1]
	old[n-1] = nil  // don't stop the GC from reclaiming the item eventually
	item.index = -1 // for safety
	*pq = old[0 : n-1]
	return item
}

// update modifies the priority and value of an Item in the queue.
// func (pq *PriorityQueue) update(item *Point, value string, priority int) {
//     item.value = value
//     item.steps = priority
//     heap.Fix(pq, item.index)
// }

func PerformSearch(input []string, straightLimit, minStraightTurns int) int {
	deltas := []struct{ deltaI, deltaJ int }{
		{deltaI: -1, deltaJ: 0}, // North
		{deltaI: 0, deltaJ: -1}, // West
		{deltaI: 1, deltaJ: 0},  // South
		{deltaI: 0, deltaJ: 1},  // East
	}

	deltaIdxMap := map[struct{ deltaI, deltaJ int }]int{
		{deltaI: -1, deltaJ: 0}: 0,
		{deltaI: 0, deltaJ: -1}: 1,
		{deltaI: 1, deltaJ: 0}:  2,
		{deltaI: 0, deltaJ: 1}:  3,
	}

	pq := PriorityQueue{
		// Going right
		&Point{
			i:             0,
			j:             0,
			steps:         0,
			index:         0,
			deltaI:        0,
			deltaJ:        1,
			straightCount: 0,
		},
		// Going down
		&Point{
			i:             0,
			j:             0,
			steps:         0,
			index:         1,
			deltaI:        1,
			deltaJ:        0,
			straightCount: 0,
		},
	}
	heap.Init(&pq)

	dist := -1
	visited := make(map[Point]int)
	for pq.Len() > 0 {
		point := *heap.Pop(&pq).(*Point)

		keyPoint := Point{i: point.i, j: point.j, deltaI: point.deltaI, deltaJ: point.deltaJ, straightCount: point.straightCount}
		if point.i < 0 || point.i >= len(input) || point.j < 0 || point.j >= len(input[point.i]) {
			continue
		}

		if currSteps, ok := visited[keyPoint]; ok && currSteps <= point.steps {
			continue
		}

		visited[keyPoint] = point.steps

		if point.straightCount >= minStraightTurns && point.i == len(input)-1 && point.j == len(input[point.i])-1 {
			dist = point.steps
			break
		}

		// Move straight
		if point.straightCount+1 <= straightLimit {
			straightI, straightJ := point.i+point.deltaI, point.j+point.deltaJ
			if straightI >= 0 && straightI < len(input) && straightJ >= 0 && straightJ < len(input[straightI]) {
				pq.Push(&Point{
					i:             straightI,
					j:             straightJ,
					steps:         point.steps + int(input[straightI][straightJ]-'0'),
					deltaI:        point.deltaI,
					deltaJ:        point.deltaJ,
					straightCount: point.straightCount + 1,
				})
			}
		}

		// Turn left or right
		if point.straightCount >= minStraightTurns {
			deltaIdx := deltaIdxMap[struct {
				deltaI int
				deltaJ int
			}{deltaI: point.deltaI, deltaJ: point.deltaJ}]
			turns := []struct{ deltaI, deltaJ int }{
				deltas[(len(deltas)+deltaIdx-1)%len(deltas)],
				deltas[(deltaIdx+1)%len(deltas)],
			}
			for _, turn := range turns {
				turnI, turnJ := point.i+turn.deltaI, point.j+turn.deltaJ
				if turnI >= 0 && turnI < len(input) && turnJ >= 0 && turnJ < len(input[turnI]) {
					pq.Push(&Point{
						i:             turnI,
						j:             turnJ,
						steps:         point.steps + int(input[turnI][turnJ]-'0'),
						deltaI:        turn.deltaI,
						deltaJ:        turn.deltaJ,
						straightCount: 1,
					})
				}
			}
		}
	}

	return dist
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	count := PerformSearch(input, 3, 0)
	count2 := PerformSearch(input, 10, 4)

	fmt.Printf("Part 1: %d, Part 2: %d\n", count, count2)
}
