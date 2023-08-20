using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

public class Solution {
    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        Solve(input);
        Solve2(input);
    }

    static void Solve(string[] input) {
        /**
            Slick C# solution with Linq from Reddit solution thread, should do more of this
            var ids = input.Split("\r\n");
            var twos =   ids.Count(id => id.GroupBy(c => c).Any(g => g.Count() == 2));
            var threes = ids.Count(id => id.GroupBy(c => c).Any(g => g.Count() == 3));
            var checksum = twos * threes;
            return checksum.ToString();
        */
        int twoCount = 0;
        int threeCount = 0;
        foreach (string line in input) {
            Dictionary<char, int> charCount = new Dictionary<char, int>();
            for (int i = 0; i < line.Length; i++) {
                if (!charCount.ContainsKey(line[i])) {
                    charCount.Add(line[i], 0);
                }

                charCount[line[i]] += 1;
            }

            bool foundTwo = false;
            bool foundThree = false;
            foreach (int count in charCount.Values) {
                if (!foundTwo && count == 2) {
                    foundTwo = true;
                    twoCount++;
                }

                if (!foundThree && count == 3) {
                    foundThree = true;
                    threeCount++;
                }
            }
        }

        Console.WriteLine(twoCount * threeCount);
    }

    static void Solve2(string[] input) {
        // There's probably a better alg than this O(n^2 * L), but eh
        for (int i = 0; i < input.Length; i++) {
            for (int j = i + 1; j < input.Length; j++) {
                string checkA = input[i];
                string checkB = input[j];

                if (checkA.Length != checkB.Length) {
                    continue;
                }

                int offCount = 0;
                for (int c = 0; c < checkA.Length; c++) {
                    if (checkA[c] != checkB[c]) {
                        offCount++;
                    }
                    if (offCount > 1) {
                        break;
                    }
                }

                if (offCount == 1) {
                    Console.WriteLine($"Correct boxes: {checkA} and {checkB}");
                }
            }
        }
    }
}