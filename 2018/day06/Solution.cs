using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        Solve(input);
        Solve2(input);
    }

    static void Solve(string[] input) {
        Dictionary<(int, int), int> points = new Dictionary<(int, int), int>();
        int count = 1;

        int minX = Int32.MaxValue, minY = Int32.MaxValue;
        int maxX = Int32.MinValue, maxY = Int32.MinValue;

        foreach (string line in input) {
            string[] coords = line.Split(", ");
            int x = Int32.Parse(coords[0]), y = Int32.Parse(coords[1]);

            minX = Math.Min(minX, x);
            maxX = Math.Max(maxX, x);
            minY = Math.Min(minY, y);
            maxY = Math.Max(maxY, y);

            points.Add((x, y), count++);
        }

        // Idea: Do 1 pass around the border of the grid, and remove all points that are "closest"
        // Abide by same rule (if multiple are closest, they're safe)
        int[,] grid = new int[maxY + 2, maxX + 2];

        foreach (KeyValuePair<(int, int), int> e in points) {
            (int x, int y) = e.Key;
            grid[y, x] = e.Value;
        }

        HashSet<(int, int)> infinites = new HashSet<(int, int)>();
        int[] counter = new int[count + 1];
        for (int y = 0; y < grid.GetLength(0); y++) {
            for (int x = 0; x < grid.GetLength(1); x++) {
                // If on border, we should track them as infinites
                bool multiple = false;
                int shortest = Int32.MaxValue;

                (int, int) closestPoint = points.Keys.Aggregate<(int, int)>((curr, next) => {
                    (int currX, int currY) = curr;
                    (int nextX, int nextY) = next;
                    int currManhattan = Math.Abs(currX - x) + Math.Abs(currY - y);
                    int nextManhattan = Math.Abs(nextX - x) + Math.Abs(nextY - y);

                    if (nextManhattan == shortest) {
                        multiple = true;
                    }

                    if (nextManhattan < currManhattan) {
                        shortest = nextManhattan;
                        multiple = false;
                        return next;
                    } else {
                        shortest = currManhattan;
                        return curr;
                    }
                });

                if (!multiple) {
                    if (x == 0 || y == 0 || x == grid.GetLength(1) - 1 || y == grid.GetLength(0) - 1) {
                        infinites.Add(closestPoint);
                    }

                    if (!infinites.Contains(closestPoint)) {
                        grid[y, x] = points[closestPoint];
                        counter[grid[y, x]]++;
                    }
                }
            }
        }

        Console.WriteLine(counter.Max());
    }

    static void Solve2(string[] input) {
        HashSet<(int, int)> points = new HashSet<(int, int)>();
        int minX = Int32.MaxValue, minY = Int32.MaxValue;
        int maxX = Int32.MinValue, maxY = Int32.MinValue;
        foreach (string line in input) {
            string[] coords = line.Split(", ");
            int x = Int32.Parse(coords[0]), y = Int32.Parse(coords[1]);

            minX = Math.Min(minX, x);
            maxX = Math.Max(maxX, x);
            minY = Math.Min(minY, y);
            maxY = Math.Max(maxY, y);

            points.Add((x, y));
        }

        int midX = (maxX + minX) / 2, midY = (maxY + minY) / 2;

        // Brute force idea is BFS? from a center point of the grid
        Queue<(int, int)> queue = new Queue<(int, int)>();
        queue.Enqueue((midX, midY));

        HashSet<(int, int)> seen = new HashSet<(int, int)>();
        int total = 0;
        while (queue.Count > 0) {
            (int x, int y) = queue.Dequeue();

            if (seen.Contains((x, y))) {
                continue;
            }

            seen.Add((x, y));

            int sum = 0;
            foreach ((int px, int py) in points) {
                sum += Math.Abs(px - x) + Math.Abs(py - y);
            }

            if (sum > 10000) {
                continue;
            }

            if (sum < 10000) {
                total++;
            }

            queue.Enqueue((x + 1, y));
            queue.Enqueue((x - 1, y));
            queue.Enqueue((x, y + 1));
            queue.Enqueue((x, y - 1));
        }

        Console.WriteLine(total);
    }
}