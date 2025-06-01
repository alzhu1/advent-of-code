package main

import (
	"fmt"
	"os"
	"slices"
	"strings"
)

type Module struct {
	moduleType, id string
	high           bool
	outputs        []string

	inputMemory map[string]bool
}

func (m *Module) ProcessMessage(message Message) []Message {
	messages := make([]Message, 0)

	// Broadcaster should immediately add everything and return
	if m.moduleType == "b" {
		for _, output := range m.outputs {
			messages = append(messages, Message{
				from: m.id,
				to:   output,
				high: message.high,
			})
		}
	}

	// Flip flop ignores high pulses
	if m.moduleType == "%" && !message.high {
		// Flip the module state, and send the module's new state in message
		m.high = !m.high

		for _, output := range m.outputs {
			messages = append(messages, Message{
				from: m.id,
				to:   output,
				high: m.high,
			})
		}
	}

	// Conjunction
	if m.moduleType == "&" {
		// Set input memory
		m.inputMemory[message.from] = message.high
		inputsHigh := true
		for _, v := range m.inputMemory {
			inputsHigh = inputsHigh && v
		}

		pulseSent := !inputsHigh

		for _, output := range m.outputs {
			messages = append(messages, Message{
				from: m.id,
				to:   output,
				high: pulseSent,
			})
		}
	}

	// fmt.Println("End: ", m.id, m.high)

	return messages
}

type Message struct {
	from, to string
	high     bool
}

func LCM(x, y int) int {
	num := x * y
	gcd := 1

	for x > 0 && y > 0 {
		if x > y {
			x %= y
		} else {
			y %= x
		}
	}

	gcd = max(x, y)
	return num / gcd
}

func LCMSlice(nums []int) int {
	lcm := 1
	for _, num := range nums {
		lcm = LCM(lcm, num)
	}
	return lcm
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	moduleMap := make(map[string]Module)

	// First, create all the modules
	for _, line := range input {
		fields := strings.Fields(line)
		moduleType, id := fields[0][0:1], fields[0][1:]
		currModule := Module{
			moduleType: moduleType,
			id:         id,
			high:       false,
			outputs:    make([]string, 0),
		}

		if moduleType == "&" {
			currModule.inputMemory = make(map[string]bool)
		}

		moduleMap[id] = currModule
	}

	// Then get all inputs (used for conjuctions)
	for _, line := range input {
		fields := strings.Split(line, " -> ")

		id, outputs := fields[0][1:], strings.Split(fields[1], ", ")
		for _, output := range outputs {
			// Add output
			currModule := moduleMap[id]
			currModule.outputs = append(currModule.outputs, output)
			moduleMap[id] = currModule

			// Add input
			outputModule := moduleMap[output]
			if outputModule.moduleType == "&" {
				outputModule.inputMemory[id] = false
				moduleMap[output] = outputModule
			}
		}
	}

	// Simulate 1000 runs
	messages := make([]Message, 0)
	presses := 0
	lowCount, highCount := 0, 0
	for presses < 1000 {
		messages = append(messages, Message{from: "", to: "roadcaster", high: false})

		for len(messages) > 0 {
			message := messages[0]
			messages = messages[1:]

			if message.high {
				highCount++
			} else {
				lowCount++
			}

			receiverModule := moduleMap[message.to]
			messages = append(messages, receiverModule.ProcessMessage(message)...)
			moduleMap[message.to] = receiverModule
		}

		presses++
	}
	product := lowCount * highCount

	// Needed to visualize this with a graph (used graphviz via https://dreampuf.github.io/GraphvizOnline)
	// At its core this describes an LCM of 4 different numbers
	// The numbers are 12-bit, and are related to the 4 conjuction modules nearest to broadcaster
	// A series of 12 flip-flop modules describe the number:
	//   Input into conjunction = 1, non-input = 0
	//   First flip-flop from broadcaster is the 1 bit, and it goes up (2, 4, 8, etc)

	lcmNums := make([]int, 0)
	for _, flipFlopId := range moduleMap["roadcaster"].outputs {
		var conjunctionModule Module
		num := 0

		order := make([]string, 0)
		currId := flipFlopId
		for len(currId) > 0 {
			order = append(order, currId)

			flipFlopModule := moduleMap[currId]
			currId = ""

			for _, flipFlopOutput := range flipFlopModule.outputs {
				if moduleMap[flipFlopOutput].moduleType == "&" {
					conjunctionModule = moduleMap[flipFlopOutput]
				} else if moduleMap[flipFlopOutput].moduleType == "%" {
					currId = moduleMap[flipFlopOutput].id
				}
			}
		}

		// We want MSB first
		slices.Reverse(order)

		for _, orderId := range order {
			num <<= 1
			if _, ok := conjunctionModule.inputMemory[orderId]; ok {
				num |= 1
			}
		}

		lcmNums = append(lcmNums, num)
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", product, LCMSlice(lcmNums))
}
