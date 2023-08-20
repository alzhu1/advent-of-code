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
        List<(int, int, int, int)> points = new List<(int, int, int, int)>();
        List<Constellation> constellations = new List<Constellation>();

        foreach (string line in input) {
            string[] parts = line.Split(",");
            (int, int, int, int) point = (
                Int16.Parse(parts[0]),
                Int16.Parse(parts[1]),
                Int16.Parse(parts[2]),
                Int16.Parse(parts[3])
            );
            points.Add(point);
        }

        while (points.Count > 0) {
            // Create new constellation using first point
            Constellation currConstellation = new Constellation(points[0]);
            points.RemoveAt(0);
            constellations.Add(currConstellation);

            if (points.Count == 0) {
                break;
            }

            // Now loop over all points to check if in constellation
            int i;
            bool updatedConstellation;

            do {
                i = 0;
                updatedConstellation = false;
                while (i < points.Count) {
                    (int, int, int, int) pointToCheck = points[i];
                    if (currConstellation.AddIfInConstellation(pointToCheck)) {
                        updatedConstellation = true;
                        points.RemoveAt(i);
                        i--;
                    }
                    i++;
                }
            } while (updatedConstellation);
        }

        Console.WriteLine(constellations.Count);
    }

    private class Constellation {
        public List<(int, int, int, int)> points;

        public Constellation((int, int, int, int) firstPoint) {
            this.points = new List<(int, int, int, int)>();
            this.points.Add(firstPoint);
        }

        public bool AddIfInConstellation((int, int, int, int) point) {
            (int pw, int px, int py, int pz) = point;
            bool shouldAdd = false;
            foreach ((int w, int x, int y, int z) in points) {
                if (Math.Abs(pw - w) + Math.Abs(px - x) + Math.Abs(py - y) + Math.Abs(pz - z) <= 3) {
                    shouldAdd = true;
                    break;
                }
            }

            if (shouldAdd) {
                this.points.Add(point);
            }
            return shouldAdd;
        }
    }
}