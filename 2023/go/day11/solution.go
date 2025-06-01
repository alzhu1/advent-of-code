package main

import (
    "container/heap"
    "fmt"
    "os"
    "strings"
)

type Point struct {
    i, j, steps, index int
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

// Dijkstra's alg to search using weighted graph approach
func GetPairDistancesSum(input []string, galaxies []Point, emptyRows, emptyCols map[int]bool, spacing int) int {
    distances := make(map[Point](map[Point]int))

    for _, galaxy := range galaxies {
        visited := make(map[Point]int)

        pq := PriorityQueue{
            &Point{
                i:     galaxy.i,
                j:     galaxy.j,
                steps: 0,
                index: 0,
            },
        }
        heap.Init(&pq)

        for pq.Len() > 0 {
            point := *heap.Pop(&pq).(*Point)

            keyPoint := Point{i: point.i, j: point.j}
            if point.i < 0 || point.i >= len(input) || point.j < 0 || point.j >= len(input[point.i]) {
                continue
            }

            if currSteps, ok := visited[keyPoint]; ok && currSteps <= point.steps {
                continue
            }

            visited[keyPoint] = point.steps

            if input[point.i][point.j] != '.' {
                if _, ok := distances[galaxy]; !ok {
                    distances[galaxy] = make(map[Point]int)
                }

                distances[galaxy][keyPoint] = point.steps
            }

            nSteps, wSteps, sSteps, eSteps := 1, 1, 1, 1
            if emptyRows[point.i-1] {
                nSteps = spacing
            }

            if emptyCols[point.j-1] {
                wSteps = spacing
            }

            if emptyRows[point.i+1] {
                sSteps = spacing
            }

            if emptyCols[point.j+1] {
                eSteps = spacing
            }

            // Push neighbors with appropriate steps
            pq.Push(&Point{
                i:     point.i - 1,
                j:     point.j,
                steps: point.steps + nSteps,
            })
            pq.Push(&Point{
                i:     point.i,
                j:     point.j - 1,
                steps: point.steps + wSteps,
            })
            pq.Push(&Point{
                i:     point.i + 1,
                j:     point.j,
                steps: point.steps + sSteps,
            })
            pq.Push(&Point{
                i:     point.i,
                j:     point.j + 1,
                steps: point.steps + eSteps,
            })
        }
    }

    sum := 0
    for _, distMap := range distances {
        for _, v := range distMap {
            sum += v
        }
    }

    return sum / 2
}

func main() {
    data, _ := os.ReadFile("input.txt")
    input := strings.Split(string(data), "\n")

    galaxies := make([]Point, 0)

    emptyRows := make(map[int]bool)
    emptyCols := make(map[int]bool)

    for i, line := range input {
        isEmpty := true

        for j, c := range line {
            if c != '.' {
                galaxies = append(galaxies, Point{i: i, j: j})
                isEmpty = false
            }
        }

        if isEmpty {
            emptyRows[i] = true
        }
    }

    for j := range len(input[0]) {
        isEmpty := true
        for i := range input {
            if input[i][j] != '.' {
                isEmpty = false
            }
        }

        if isEmpty {
            emptyCols[j] = true
        }
    }

    // sum := GetPairDistancesSum(input, galaxies, emptyRows, emptyCols, 2)
    // sum2 := GetPairDistancesSum(input, galaxies, emptyRows, emptyCols, 1_000_000)

    // Alt approach (reddit hints)
    // Just do manhattan dist, and add in extra spaces when crossing an empty field
    sum, sum2 := 0, 0
    for _, galaxy := range galaxies {
        for _, galaxy2 := range galaxies {
            // Init manhattan dist
            rowDist, colDist := galaxy.i-galaxy2.i, galaxy.j-galaxy2.j

            // Add absolute value distances
            sum += max(rowDist, -rowDist) + max(colDist, -colDist)
            sum2 += max(rowDist, -rowDist) + max(colDist, -colDist)

            rowDelta, colDelta := 1, 1
            if galaxy.i > galaxy2.i {
                rowDelta = -1
            }
            if galaxy.j > galaxy2.j {
                colDelta = -1
            }

            // Add relative factors (e.g. num spaces - 1)
            for i := galaxy.i; i != galaxy2.i; i += rowDelta {
                if emptyRows[i] {
                    sum += 1
                    sum2 += 999999
                }
            }
            for j := galaxy.j; j != galaxy2.j; j += colDelta {
                if emptyCols[j] {
                    sum += 1
                    sum2 += 999999
                }
            }
        }
    }

    // Sums are double counted
    sum /= 2
    sum2 /= 2

    fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}
