package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type Lens struct {
	label, foc string
}

func HASH(s string) int {
	val := 0
	for _, c := range s {
		val = (val + int(c)) * 17
	}
	return val % 256
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	line := input[0]

	parts := strings.Split(line, ",")

	sum := 0
	hashmap := make(map[int][]Lens)
	for i := range 256 {
		hashmap[i] = make([]Lens, 0)
	}

	for _, part := range parts {
		sum += HASH(part)

		if part[len(part)-1] == '-' {
			label := part[:len(part)-1]
			hash := HASH(label)

			slice := make([]Lens, 0)
			for _, l := range hashmap[hash] {
				if l.label != label {
					slice = append(slice, l)
				}
			}
			hashmap[hash] = slice
		} else {
			pieces := strings.Split(part, "=")
			label, foc := pieces[0], pieces[1]
			hash := HASH(label)

			contained := false
			for i, l := range hashmap[hash] {
				if l.label == label {
					hashmap[hash][i].foc = foc
					contained = true
				}
			}

			if !contained {
				hashmap[hash] = append(hashmap[hash], Lens{
					label: label,
					foc:   foc,
				})
			}
		}
	}

	sum2 := 0
	for box, lenses := range hashmap {
		for pos, lens := range lenses {
			foc, err := strconv.Atoi(lens.foc)
			if err != nil {
				log.Fatal("Error: ", err)
			}

			sum2 += (box + 1) * (pos + 1) * foc
		}
	}

	fmt.Printf("Part 1: %d, Part 2: %d\n", sum, sum2)
}
