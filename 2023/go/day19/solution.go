package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type Part struct {
	fields [4]int // x, m, a, s
}

type Rule struct {
	fieldIdx          int
	condition, target string
	num               int
	bypass            bool
}

func (r Rule) GetField(p Part) int {
	return p.fields[r.fieldIdx]
}

func (r Rule) GetNextNodeLabel(p Part) (bool, string) {
	if r.bypass {
		return true, r.target
	}

	partField := r.GetField(p)
	if r.condition == "<" {
		return partField < r.num, r.target
	} else {
		return partField > r.num, r.target
	}
}

func (r Rule) ProcessBounds(minPart, maxPart Part) (Part, Part, Part, Part) {
	affectedPart, replacePart, delta := Part{}, Part{}, 0
	if r.condition == "<" {
		affectedPart = maxPart
		replacePart = minPart
		delta = -1
	} else {
		affectedPart = minPart
		replacePart = maxPart
		delta = 1
	}

	affectedPart.fields[r.fieldIdx] = r.num + delta
	replacePart.fields[r.fieldIdx] = r.num

	// The first 2 should go in the recurse; the last 2 should replace the params
	if r.condition == "<" {
		return minPart, affectedPart, replacePart, maxPart
	} else {
		return affectedPart, maxPart, minPart, replacePart
	}
}

type Node struct {
	label string
	rules []Rule
}

func GetPermutations(nodeMap map[string]Node, currLabel string, minPart, maxPart Part) int {
	if currLabel == "R" {
		// Ignore R
		return 0
	}

	if currLabel == "A" {
		// Multiply total counts for each value for permutations
		res := 1
		for i := range 4 {
			res *= (maxPart.fields[i] - minPart.fields[i] + 1)
		}
		return res
	}

	// Recurse per rule
	node := nodeMap[currLabel]
	sum := 0
	for _, rule := range node.rules {
		if rule.bypass {
			sum += GetPermutations(nodeMap, rule.target, minPart, maxPart)
			continue
		}

		// Get new bounds based on the current rule
		recurseMin, recurseMax, replaceMin, replaceMax := rule.ProcessBounds(minPart, maxPart)
		sum += GetPermutations(nodeMap, rule.target, recurseMin, recurseMax)
		minPart = replaceMin
		maxPart = replaceMax
	}

	return sum
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	fieldMap := map[string]int{
		"x": 0,
		"m": 1,
		"a": 2,
		"s": 3,
	}

	nodeMap := make(map[string]Node)

	// A and R
	nodeMap["A"] = Node{label: "A"}
	nodeMap["R"] = Node{label: "R"}

	i := 0
	for i < len(input) {
		if len(input[i]) == 0 {
			i++
			break
		}

		labelSplit := strings.Split(input[i], "{")
		label, rest := labelSplit[0], labelSplit[1][:len(labelSplit[1])-1]

		rules := make([]Rule, 0)
		rulesStr := strings.SplitSeq(rest, ",")
		for ruleStr := range rulesStr {
			if !strings.Contains(ruleStr, ":") {
				rules = append(rules, Rule{
					target: ruleStr,
					bypass: true,
				})
				continue
			}

			parts := strings.Split(ruleStr, ":")
			check, target := parts[0], parts[1]

			field, condition, numStr := string(check[0]), string(check[1]), check[2:]
			num, err := strconv.Atoi(numStr)
			if err != nil {
				log.Fatal("Error: ", err)
			}

			rules = append(rules, Rule{
				fieldIdx:  fieldMap[field],
				condition: condition,
				target:    target,
				num:       num,
				bypass:    false,
			})
		}

		nodeMap[label] = Node{
			label: label,
			rules: rules,
		}

		i++
	}

	// Parts
	count := 0
	for i < len(input) {
		pieces := strings.Split(input[i][1:len(input[i])-1], ",")

		x, xErr := strconv.Atoi(pieces[0][2:])
		m, mErr := strconv.Atoi(pieces[1][2:])
		a, aErr := strconv.Atoi(pieces[2][2:])
		s, sErr := strconv.Atoi(pieces[3][2:])

		if xErr != nil || mErr != nil || aErr != nil || sErr != nil {
			log.Fatal("Error: ", xErr, mErr, aErr, sErr)
		}

		part := Part{
			fields: [4]int{x, m, a, s},
		}

		node := nodeMap["in"]
		for node.label != "A" && node.label != "R" {
			for _, rule := range node.rules {
				result, target := rule.GetNextNodeLabel(part)

				if result {
					node = nodeMap[target]
					break
				}
			}
		}

		if node.label == "A" {
			count += x + m + a + s
		}

		i++
	}

	count2 := GetPermutations(nodeMap, "in", Part{fields: [4]int{1, 1, 1, 1}}, Part{fields: [4]int{4000, 4000, 4000, 4000}})
	fmt.Printf("Part 1: %d, Part 2: %d\n", count, count2)
}
