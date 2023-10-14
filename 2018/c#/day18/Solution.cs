using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

public class Solution {
    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        Solve(input);
    }

    static void Solve(string[] input) {
        (int, int)[] directions = new (int, int)[] {
            (0, 1), (1, 0), (0, -1), (-1, 0),
            (1, 1), (1, -1), (-1, -1), (-1, 1)
        };
        HashSet<(int, int)> trees = new HashSet<(int, int)>();
        HashSet<(int, int)> lumberyards = new HashSet<(int, int)>();

        for (int y = 0; y < input.Length; y++) {
            for (int x = 0; x < input[y].Length; x++) {
                if (input[y][x] == '|') {
                    trees.Add((x, y));
                } else if (input[y][x] == '#') {
                    lumberyards.Add((x, y));
                }
            }
        }

        long t = 0;
        long iters = 1_000_000_000;
        Dictionary<string, long> keyToTime = new Dictionary<string, long>();
        keyToTime.Add(GetHashKey(trees, lumberyards), 0);
        while (t < iters) {
            HashSet<(int, int)> tempTrees = new HashSet<(int, int)>();
            HashSet<(int, int)> tempLumberyards = new HashSet<(int, int)>();

            for (int x = 0; x < 50; x++) {
                for (int y = 0; y < 50; y++) {
                    (int, int) checkPos = (x, y);
                    int treeCount = 0;
                    int lumberyardCount = 0;

                    bool isTree = trees.Contains(checkPos);
                    bool isLumberyard = lumberyards.Contains(checkPos);

                    foreach ((int dx, int dy) in directions) {
                        (int, int) neighborPos = (x + dx, y + dy);

                        if (trees.Contains(neighborPos)) {
                            treeCount++;
                        } else if (lumberyards.Contains(neighborPos)) {
                            lumberyardCount++;
                        }
                    }

                    if (!isTree && !isLumberyard && treeCount >= 3) {
                        tempTrees.Add(checkPos);
                    }

                    if (isTree && lumberyardCount >= 3) {
                        tempLumberyards.Add(checkPos);
                    } else if (isTree) {
                        tempTrees.Add(checkPos);
                    }

                    if (isLumberyard && (treeCount >= 1 && lumberyardCount >= 1)) {
                        tempLumberyards.Add(checkPos);
                    }
                }
            }

            trees = tempTrees;
            lumberyards = tempLumberyards;

            string key = GetHashKey(trees, lumberyards);
            if (keyToTime.ContainsKey(key)) {
                Console.WriteLine($"Loop found, start: {keyToTime[key]}, end: {t}");
                long cycle = t - keyToTime[key];
                long numCyclesLeft = (iters - t) / cycle;
                if (numCyclesLeft > 0) {
                    t += cycle * numCyclesLeft;
                }
            } else {
                keyToTime.Add(key, t);
            }

            t++;
        }

        Console.WriteLine($"Trees {trees.Count} * lumberyards {lumberyards.Count}");
        Console.WriteLine(trees.Count * lumberyards.Count);
    }

    static string GetHashKey(HashSet<(int, int)> trees, HashSet<(int, int)> lumberyards) {
        StringBuilder sb = new StringBuilder();
        sb.Append("T:");
        foreach ((int, int) t in trees) {
            sb.Append($"({t})");
        }
        sb.Append("L:");
        foreach ((int, int) l in lumberyards) {
            sb.Append($"({l})");
        }

        return sb.ToString();
    }
}