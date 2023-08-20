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
        HashSet<(int, int)> clay = new HashSet<(int, int)>();
        HashSet<(int, int)> stillWater = new HashSet<(int, int)>();

        int minY = Int16.MaxValue, maxY = Int16.MinValue;
        foreach (string line in input) {
            string[] parts = line.Split(", ");

            string baseCoordinate = parts[0].Substring(parts[0].IndexOf("=") + 1);
            string[] rangeCoordinates = parts[1].Substring(parts[1].IndexOf("=") + 1).Split("..");

            int baseNum = Int16.Parse(baseCoordinate);
            int rangeStart = Int16.Parse(rangeCoordinates[0]);
            int rangeEnd = Int16.Parse(rangeCoordinates[1]);

            for (int i = rangeStart; i <= rangeEnd; i++) {
                int x = parts[0][0] == 'x' ? baseNum : i;
                int y = parts[0][0] == 'y' ? baseNum : i;

                clay.Add((x, y));
                minY = Math.Min(minY, y);
                maxY = Math.Max(maxY, y);
            }
        }

        // foreach ((int, int) p in clay) {
        //     Console.WriteLine(p);
        // }

        // Keep calling Traverse until stillWater's count has not changed
        HashSet<(int, int)> seen = new HashSet<(int, int)>();
        while (true) {
            int stillWaterCount = stillWater.Count;
            seen.Clear();
            Traverse((500, 0), (500, -1), maxY, clay, stillWater, seen, false, false);

            if (stillWaterCount == stillWater.Count) {
                break;
            }
        }

        int minX = Int16.MaxValue, maxX = Int16.MinValue;
        foreach (var e in seen) {
            minX = Math.Min(minX, e.Item1);
            maxX = Math.Max(maxX, e.Item1);
        }
        foreach (var e in clay) {
            minX = Math.Min(minX, e.Item1);
            maxX = Math.Max(maxX, e.Item1);
        }

        // Technically for debug, but it looks cool
        int currY = 0;
        using (StreamWriter sw = new StreamWriter("out.txt")) {
            while (currY < maxY) {
                if (currY < 10) {
                    sw.Write("   ");
                } else if (currY < 100) {
                    sw.Write("  ");
                } else if (currY < 1000) {
                    sw.Write(" ");
                }
                sw.Write($"{currY}: ");
                for (int i = minX - 1; i <= maxX + 1; i++) {
                    (int, int) wPos = (i, currY);

                    if (i == 500 && currY == 0) {
                        sw.Write("+");
                    } else if (clay.Contains(wPos)) {
                        sw.Write("#");
                    } else if (stillWater.Contains(wPos)) {
                        sw.Write("~");
                    } else if (seen.Contains(wPos)) {
                        sw.Write("|");
                    } else {
                        sw.Write(".");
                    }
                }
                sw.WriteLine();

                currY++;
            }
        }

        seen.RemoveWhere(x => x.Item2 < minY);
        Console.WriteLine($"Part 1 (all wet): {seen.Count + stillWater.Count}");
        Console.WriteLine($"Part 2 (only still water): {stillWater.Count}");
    }

    static (int, int, bool) Traverse((int, int) pos,
                               (int, int) prev,
                               int deepest,
                               HashSet<(int, int)> clay,
                               HashSet<(int, int)> stillWater,
                               HashSet<(int, int)> seen,
                               bool left,
                               bool side) {
        // Base case is going past deepest or we saw it already
        (int x, int y) = pos;
        if (y > deepest || clay.Contains(pos) || stillWater.Contains(pos)) {
            return (prev.Item1, prev.Item2, true);
        }

        if (seen.Contains(pos)) {
            return (prev.Item1, prev.Item2, false);
        }
        seen.Add(pos);

        // If down not blocked, traverse down
        if (!clay.Contains((x, y + 1)) && !stillWater.Contains((x, y + 1))) {
            return Traverse((x, y + 1), pos, deepest, clay, stillWater, seen, false, false);
        }

        // If we're moving by sides, auto move
        if (side) {
            (int, int) nextPos = left ? (x - 1, y) : (x + 1, y);

            return Traverse(nextPos, pos, deepest, clay, stillWater, seen, left, true);
        }

        // Otherwise, go left and right
        (int, int, bool) leftPos = Traverse((x - 1, y), pos, deepest, clay, stillWater, seen, true, true);
        (int, int, bool) rightPos = Traverse((x + 1, y), pos, deepest, clay, stillWater, seen, false, true);

        if (leftPos.Item3 && rightPos.Item3 && leftPos.Item2 == rightPos.Item2 && leftPos.Item2 == y) {
            for (int i = leftPos.Item1; i <= rightPos.Item1; i++) {
                stillWater.Add((i, y));
            }
        }

        if (left) {
            return leftPos;
        } else {
            return rightPos;
        }
    }
}