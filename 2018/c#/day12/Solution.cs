using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        Solve(input);
    }

    static void Solve(string[] input) {
        HashSet<long> plantLocations = new HashSet<long>();
        Dictionary<long, long> plantMapping = new Dictionary<long, long>();

        string initialLocations = input[0].Split(": ")[1];
        for (int i = 0; i < initialLocations.Length; i++) {
            if (initialLocations[i] == '#') {
                plantLocations.Add((long)i);
            }
        }

        foreach (string line in input) {
            if (line.IndexOf("=") != -1) {
                string[] parts = line.Split(" => ");
                long key = 0;
                for (int i = 0; i < parts[0].Length; i++) {
                    if (parts[0][parts[0].Length - 1 - i] == '#') {
                        key += (1 << i);
                    }
                }
                plantMapping.Add(key, parts[1][0] == '#' ? 1 : 0);
            }
        }

        Dictionary<string, (long, long)> seen = new Dictionary<string, (long, long)>();
        (string firstValue, long firstOffset) = GetCheckedValue(plantLocations);
        seen.Add(firstValue, (0, firstOffset));

        long generation = 0;
        while (generation++ < 100) {
            HashSet<long> tempPlants = new HashSet<long>();

            foreach (long plantLocation in plantLocations) {
                for (long center = plantLocation - 2; center <= plantLocation + 2; center++) {
                    // Check 2 before, center, and 2 after for a 5 piece

                    long value = 0;
                    for (int i = 0; i < 5; i++) {
                        if (plantLocations.Contains(center + 2 - i)) {
                            value += (1 << i);
                        }
                    }

                    if (plantMapping.ContainsKey(value) && plantMapping[value] == 1) {
                        tempPlants.Add(center);
                    } 
                }
            }

            plantLocations = tempPlants;
            (string checkedValue, long offset) = GetCheckedValue(plantLocations);

            if (seen.ContainsKey(checkedValue)) {
                (long foundGeneration, long foundOffset) = seen[checkedValue];
                Console.WriteLine("Loop detected");
                Console.WriteLine($"Found gen {foundGeneration}, found offset {foundOffset}, curr gen {generation}, curr offset {offset}");
                Console.WriteLine($"Sum is {plantLocations.Sum()}");
                break;
            } else {
                seen.Add(checkedValue, (generation, offset));
            }

            Console.WriteLine(generation);
        }

        // Looks like a 1 cycle that increases offset by 1, with 15 points
        Console.WriteLine(plantLocations.Sum() + (15L * (50_000_000_000 - generation)));
    }

    static (string, long) GetCheckedValue(HashSet<long> locations) {
        List<long> temp = locations.ToList();
        temp.Sort();

        // Idea: what if I localize everything to 0
        long firstElement = temp[0];
        for (int i = 0; i < temp.Count; i++) {
            temp[i] -= firstElement;
        }

        return (String.Join(",", temp.Select(x => x.ToString()).ToArray()), firstElement);
    }
}