package main

import (
	"fmt"
	"maps"
	"math"
	"os"
	"strings"
)

func MinCutPhase(graph map[string]map[string]int) (string, string, int, int, int) {
	// Start set with first thing from the graph (i.e. "random" pick)
	nodeSet := make(map[string]bool)
	nodeList := make([]string, len(graph))
	nodeIndex := 0
	toNodeMap := make(map[string]int)
	for node, edges := range graph {
		nodeSet[node] = true
		nodeList[nodeIndex] = node
		nodeIndex++

		maps.Copy(toNodeMap, edges)
		break
	}

	// Go until we process all nodes
	minCut := 0
	size1, size2 := 0, 0

	for nodeIndex < len(graph) {
		// Get the edge with the largest weight
		currWeight := 0
		nodeToMerge := ""
		for toNode, weight := range toNodeMap {
			if weight > currWeight {
				currWeight = weight
				nodeToMerge = toNode
			}
		}

		// Add this to our set
		nodeSet[nodeToMerge] = true
		nodeList[nodeIndex] = nodeToMerge
		nodeIndex++

		// For the nodeToMerge, add their edges/weights to map (while ignoring anything to our set)
		nodeToMergeEdges := graph[nodeToMerge]
		for to, weight := range nodeToMergeEdges {
			if !nodeSet[to] {
				if _, ok := toNodeMap[to]; !ok {
					toNodeMap[to] = 0
				}
				toNodeMap[to] += weight
			}
		}

		// For the very last section, we should calculate the min cut
		if nodeIndex == len(graph) {
			if len(toNodeMap) != 1 {
				panic("Should not happen?")
			}

			for _, v := range toNodeMap {
				minCut = v
			}

			// Get sizes of both the growing node set + the last node we added
			size1 = len(strings.Split(strings.Join(nodeList[:len(nodeList)-1], "/"), "/"))
			size2 = len(strings.Split(nodeToMerge, "/"))
		}

		// Remove the edge to nodeToMerge
		// This is because nodeToMerge is going to be consolidated, we want to remove internal edges
		delete(toNodeMap, nodeToMerge)

		// The edges should now represent all outgoing edges from the consolidated node graph
	}

	return nodeList[len(nodeList)-2], nodeList[len(nodeList)-1], minCut, size1, size2
}

// Stoer-Wagner: https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
// Prefer this to Karger's since it's deterministic
func GetMinCut(graph map[string]map[string]int) (int, int, int) {
	minCut := math.MaxInt
	size1, size2 := 0, 0

	for len(graph) > 1 {
		node1, node2, resultMinCut, resultSize1, resultSize2 := MinCutPhase(graph)

		// Pick the smallest cut returned from phase
		if resultMinCut < minCut {
			minCut = resultMinCut
			size1 = resultSize1
			size2 = resultSize2
		}

		// Officially merge node1 and node2
		combinedNode := node1 + "/" + node2

		// Start with the node1 map, and add over edges for node 2
		toCombinedNodeMap := graph[node1]
		delete(toCombinedNodeMap, node2)
		for to, weight := range graph[node2] {
			if to != node1 {
				if _, ok := toCombinedNodeMap[to]; !ok {
					toCombinedNodeMap[to] = 0
				}
				toCombinedNodeMap[to] += weight
			}
		}

		// Set in graph
		delete(graph, node1)
		delete(graph, node2)
		graph[combinedNode] = toCombinedNodeMap

		// Finally, through through all combinedEdges to re-write to combined node
		for to, weight := range toCombinedNodeMap {
			// Remove old references
			delete(graph[to], node1)
			delete(graph[to], node2)

			// Add combined node
			graph[to][combinedNode] = weight
		}
	}

	return minCut, size1, size2
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	graph := make(map[string]map[string]int)
	for _, line := range input {
		parts := strings.Split(line, ": ")
		from, targets := parts[0], strings.Split(parts[1], " ")

		for _, target := range targets {
			// From - to
			if _, ok := graph[from]; !ok {
				graph[from] = make(map[string]int)
			}
			graph[from][target] = 1

			// To - from
			if _, ok := graph[target]; !ok {
				graph[target] = make(map[string]int)
			}
			graph[target][from] = 1
		}
	}

	minCut, size1, size2 := GetMinCut(graph)
	if minCut != 3 {
		panic("We need to cut 3")
	}
	fmt.Printf("Size 1: %d, Size 2: %d, Result: %d\n", size1, size2, size1*size2)
}
