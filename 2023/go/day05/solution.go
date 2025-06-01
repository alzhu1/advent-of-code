package main

import (
	"fmt"
	"log"
	"math"
	"os"
	"strconv"
	"strings"
)

type Chunk struct {
	start, end int
	translated bool
}

type Mapper struct {
	dest, start, end int
}
type MapperSlice []Mapper

func (mm MapperSlice) GetMappedValue(check int) int {
	for _, mapper := range mm {
		v := mapper.GetMappedValue(check)
		if v != -1 {
			return v
		}
	}

	return check
}

func (m Mapper) GetMappedValue(check int) int {
	if check >= m.start && check <= m.end {
		return check - (m.start - m.dest)
	}

	return -1
}

func (mm MapperSlice) GetMappedChunks(chunks []Chunk) []Chunk {
	for _, mapper := range mm {
		chunks = mapper.GetMappedChunks(chunks)
	}

	for i := range chunks {
		chunks[i].translated = false
	}

	return chunks
}

func (m Mapper) GetMappedChunks(chunks []Chunk) []Chunk {
	mappedChunks := []Chunk{}
	for _, chunk := range chunks {
		// Case 1: mapper is outside chunk range (or was previously translated)
		if chunk.translated || chunk.end < m.start || chunk.start > m.end {
			mappedChunks = append(mappedChunks, chunk)
			continue
		}

		// Case 2: mapper is larger
		if chunk.start >= m.start && chunk.end <= m.end {
			mappedChunks = append(mappedChunks, Chunk{
				start:      chunk.start - (m.start - m.dest),
				end:        chunk.end - (m.start - m.dest),
				translated: true,
			})
			continue
		}

		// Case 3: partial overlap
		if chunk.start < m.start && chunk.end <= m.end {
			mappedChunks = append(mappedChunks, Chunk{
				start: chunk.start,
				end:   m.start - 1,
			}, Chunk{
				start:      m.dest,
				end:        chunk.end - (m.start - m.dest),
				translated: true,
			})
		}
		if chunk.start >= m.start && chunk.end > m.end {
			mappedChunks = append(mappedChunks, Chunk{
				start:      chunk.start - (m.start - m.dest),
				end:        m.end - (m.start - m.dest),
				translated: true,
			}, Chunk{
				start: m.end + 1,
				end:   chunk.end,
			})
		}

		// Case 4: Chunk is larger
		if chunk.start < m.start && chunk.end > m.end {
			mappedChunks = append(mappedChunks, Chunk{
				start: chunk.start,
				end:   m.start - 1,
			}, Chunk{
				start:      m.dest,
				end:        m.end - (m.start - m.dest),
				translated: true,
			}, Chunk{
				start: m.end + 1,
				end:   chunk.end,
			})
		}
	}

	return mappedChunks
}

func main() {
	data, _ := os.ReadFile("input.txt")
	input := strings.Split(string(data), "\n")

	seeds := strings.Fields(strings.Split(input[0], ": ")[1])

	mappers, mapperIndex := make([]MapperSlice, 7), -1

	for _, line := range input[1:] {
		if len(line) == 0 {
			continue
		}

		fields := strings.Fields(line)
		if len(fields) != 3 {
			mapperIndex++
		} else {
			dest, e1 := strconv.Atoi(fields[0])
			start, e2 := strconv.Atoi(fields[1])
			length, e3 := strconv.Atoi(fields[2])

			if e1 != nil || e2 != nil || e3 != nil {
				log.Fatal("Error: ", e1, e2, e3)
			}

			mappers[mapperIndex] = append(mappers[mapperIndex], Mapper{
				dest:  dest,
				start: start,
				end:   start + length - 1,
			})
		}
	}

	location := math.MaxInt
	for _, seed := range seeds {
		num, e := strconv.Atoi(seed)
		if e != nil {
			log.Fatal("Error: ", e)
		}

		for _, mapperSlice := range mappers {
			num = mapperSlice.GetMappedValue(num)
		}

		location = min(location, num)
	}

	fmt.Println(location)

	// location2 := bruteForcePart2(seeds, mappers)
	location2 := chunkPart2(seeds, mappers)

	fmt.Printf("Part 1: %d, Part 2: %d\n", location, location2)
}

func bruteForcePart2(seeds []string, mappers []MapperSlice) int {
	location2 := math.MaxInt
	for i := 0; i < len(seeds); i += 2 {
		seed, length := seeds[i], seeds[i+1]

		num1, e1 := strconv.Atoi(seed)
		num2, e2 := strconv.Atoi(length)
		if e1 != nil || e2 != nil {
			log.Fatal("Error: ", e1, e2)
		}

		for j := num1; j < num1+num2; j++ {
			n := j
			for _, mapperSlice := range mappers {
				n = mapperSlice.GetMappedValue(n)
			}

			location2 = min(location2, n)
		}
	}

	return location2
}

func chunkPart2(seeds []string, mappers []MapperSlice) int {
	location2 := math.MaxInt
	for i := 0; i < len(seeds); i += 2 {
		seed, length := seeds[i], seeds[i+1]

		num1, e1 := strconv.Atoi(seed)
		num2, e2 := strconv.Atoi(length)
		if e1 != nil || e2 != nil {
			log.Fatal("Error: ", e1, e2)
		}

		chunks := []Chunk{
			{
				start: num1,
				end:   num1 + num2 - 1,
			},
		}

		for _, mapperSlice := range mappers {
			chunks = mapperSlice.GetMappedChunks(chunks)
		}

		for _, chunk := range chunks {
			if chunk.start != 0 && chunk.end != 0 {
				location2 = min(location2, chunk.start)
			}
		}
	}

	return location2
}
