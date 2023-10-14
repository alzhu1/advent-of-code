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
        HashSet<Point> points = new HashSet<Point>();

        foreach (string line in input) {
            string[] parts = line.Split("> v");
            string[] pos = parts[0].Substring(parts[0].IndexOf("<") + 1).Split(",");
            (long, long) position = (long.Parse(pos[0].Trim()), long.Parse(pos[1].Trim()));

            string[] vel = parts[1].Split(">")[0].Substring(parts[1].IndexOf("<") + 1).Split(",");
            (long, long) velocity = (long.Parse(vel[0].Trim()), long.Parse(vel[1].Trim()));

            points.Add(new Point(position, velocity));
        }

        (long, long) smallestWidthAtTime = (long.MaxValue, -1);
        (long, long) smallestHeightAtTime = (long.MaxValue, -1);
        for (int i = 0; i < 100000; i++) {
            long minX = long.MaxValue, minY = long.MaxValue;
            long maxX = long.MinValue, maxY = long.MinValue;
            foreach (Point p in points) {
                (long px, long py) = p.GetPosAtTime(i);
                minX = Math.Min(minX, px);
                minY = Math.Min(minY, py);

                maxX = Math.Max(maxX, px);
                maxY = Math.Max(maxY, py);
            }

            long width = maxX - minX;
            long height = maxY - minY;

            if (width < smallestWidthAtTime.Item1) {
                smallestWidthAtTime = (width, i);
            }

            if (height < smallestHeightAtTime.Item1) {
                smallestHeightAtTime = (height, i);
            }
        }

        Console.WriteLine("Try smallest width:");
        PrintPoints(points, smallestWidthAtTime.Item2);

        Console.WriteLine("Try smallest height:");
        PrintPoints(points, smallestHeightAtTime.Item2);
    }

    static void PrintPoints(HashSet<Point> points, long t) {
        HashSet<(long, long)> positions = new HashSet<(long, long)>();
        foreach (Point p in points) {
            positions.Add(p.GetPosAtTime(t));
        }
        long minX = long.MaxValue, minY = long.MaxValue;
        long maxX = long.MinValue, maxY = long.MinValue;

        foreach ((long x, long y) in positions) {
            minX = Math.Min(minX, x);
            minY = Math.Min(minY, y);

            maxX = Math.Max(maxX, x);
            maxY = Math.Max(maxY, y);
        }

        using (StreamWriter sw = new StreamWriter($"output{t}.txt")) {
            for (long y = minY; y <= maxY; y++) {
                for (long x = minX; x <= maxX; x++) {
                    (long, long) pos = (x, y);
                    sw.Write(positions.Contains(pos) ? '#' : ' ');
                }
                sw.WriteLine();
            }
        }
    }

    private class Point {
        private (long, long) position;
        private (long, long) velocity;

        public Point((long, long) position, (long, long) velocity) {
            this.position = position;
            this.velocity = velocity;
        }

        public (long, long) GetPosition() {
            return position;
        }

        public (long, long) GetPosAtTime(long t) {
            return (position.Item1 + velocity.Item1 * t, position.Item2 + velocity.Item2 * t);
        }
    }
}