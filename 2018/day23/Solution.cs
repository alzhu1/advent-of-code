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
        HashSet<Nanobot> nanobots = new HashSet<Nanobot>();

        Nanobot highRadiusBot = null;

        long minX = long.MaxValue, minY = long.MaxValue, minZ = long.MaxValue;
        long maxX = long.MinValue, maxY = long.MinValue, maxZ = long.MinValue;
        foreach (string line in input) {
            string[] parts = line.Split(">, ");
            string[] posStr = parts[0].Split("<")[1].Split(",");

            (long, long, long) pos = (long.Parse(posStr[0]), long.Parse(posStr[1]), long.Parse(posStr[2]));
            long radius = long.Parse(parts[1].Split("=")[1]);

            minX = Math.Min(minX, pos.Item1);
            maxX = Math.Max(maxX, pos.Item1);

            minY = Math.Min(minY, pos.Item2);
            maxY = Math.Max(maxY, pos.Item2);

            minZ = Math.Min(minZ, pos.Item3);
            maxZ = Math.Max(maxZ, pos.Item3);

            Nanobot bot = new Nanobot(pos, radius);
            nanobots.Add(bot);

            if (highRadiusBot == null || highRadiusBot.radius < radius) {
                highRadiusBot = bot;
            }
        }

        int rangeCount = 0;
        foreach (Nanobot bot in nanobots) {
            if (highRadiusBot.IsTargetInRange(bot)) {
                rangeCount++;
            }
        }
        Console.WriteLine($"Range from highest radius: {rangeCount}");

        // Got some Reddit help on this, idea is splitting the whole space into many points
        // Do a search in that whole space, then repeat but use a smaller space (1/8?)

        // 1st part of alg: broad searching to get within the ballpark of cluster
        // Split a prism containing all points into subdivided points,
        // check for most clusters. Then center around that and continue with smaller space

        // Initial search space reduction
        // long bestOverallCount = 0;
        // long shortestDist = long.MaxValue;
        // while (true) {
        //     long sizeX = maxX - minX;
        //     long sizeY = maxY - minY;
        //     long sizeZ = maxZ - minZ;

        //     long numPointsOnAxis = 25;
        //     long iterX = sizeX / numPointsOnAxis;
        //     long iterY = sizeY / numPointsOnAxis;
        //     long iterZ = sizeZ / numPointsOnAxis;

        //     Console.WriteLine($"iterX = {iterX}, iterY = {iterY}, iterZ = {iterZ}");

        //     // Split the "box" into many points and check for best count

        //     long bestCount = 0;
        //     (long, long, long) posOfBestCount = (0, 0, 0);

        //     for (long x = minX; x <= maxX; x += iterX) {
        //         for (long y = minY; y <= maxY; y += iterY) {
        //             for (long z = minZ; z <= maxZ; z += iterZ) {
        //                 long counter = 0;
        //                 (long, long, long) checkPos = (x, y, z);

        //                 foreach (Nanobot bot in nanobots) {
        //                     if (bot.IsTargetInRange(checkPos)) {
        //                         counter++;
        //                     }
        //                 }

        //                 if (counter > bestCount) {
        //                     bestCount = counter;
        //                     posOfBestCount = checkPos;
        //                 }
        //             }
        //         }
        //     }

        //     (long px, long py, long pz) = posOfBestCount;
        //     if (bestCount > bestOverallCount) {
        //         bestOverallCount = bestCount;
        //         shortestDist = Math.Abs(px) + Math.Abs(py) + Math.Abs(pz);
        //     } else if (bestCount == bestOverallCount) {
        //         shortestDist = Math.Min(shortestDist, Math.Abs(px) + Math.Abs(py) + Math.Abs(pz));
        //     }

        //     Console.WriteLine((minX, maxX));
        //     // Reset min and max to center around posOfBestCount
        //     minX = posOfBestCount.Item1 - (sizeX / 8);
        //     maxX = posOfBestCount.Item1 + (sizeX / 8);
        //     minY = posOfBestCount.Item2 - (sizeY / 8);
        //     maxY = posOfBestCount.Item2 + (sizeY / 8);
        //     minZ = posOfBestCount.Item3 - (sizeZ / 8);
        //     maxZ = posOfBestCount.Item3 + (sizeZ / 8);
        //     Console.WriteLine((minX, maxX));

        //     Console.WriteLine($"Count is {bestCount}, pos is {posOfBestCount}. Best is {bestOverallCount}, shortestDist = {shortestDist}");
        // }

        // 2nd part of alg: locally searching around clusters of overlapping points
        // Do this to find the absolute most overlap
        
        (long, long, long) bestPos = (35689633, 19931583, 44852810);
        long interval = 25;
        long bestCount = 946;
        
        while (true) {
            Console.WriteLine("Checking...");
            bool shouldBreak = true;
            long bestDist = bestPos.Item1 + bestPos.Item2 + bestPos.Item3;

            (long, long, long) checkPos = bestPos;
            (long cx, long cy, long cz) = checkPos;
            for (long x = -interval; x <= interval; x++) {
                for (long y = -interval; y <= interval; y++) {
                    for (long z = -interval; z <= interval; z++) {
                        long counter = 0;
                        (long, long, long) currPos = (cx + x, cy + y, cz);

                        foreach (Nanobot bot in nanobots) {
                            if (bot.IsTargetInRange(currPos)) {
                                counter++;
                            }
                        }

                        (long cux, long cuy, long cuz) = currPos;
                        long dist = Math.Abs(cux) + Math.Abs(cuy) + Math.Abs(cuz);
                        if (counter > bestCount) {
                            Console.WriteLine($"Better count at {counter}, pos = {currPos}");
                            bestCount = counter;
                            bestPos = currPos;
                            bestDist = dist;
                            shouldBreak = false;
                        } else if (counter == bestCount && dist < bestDist) {
                            Console.WriteLine($"Better dist at {dist}, pos = {currPos}");
                            bestDist = dist;
                            bestPos = currPos;
                            shouldBreak = false;
                        }
                    }
                }
            }

            if (shouldBreak) {
                break;
            }
        }
    }

    private class Nanobot {
        public (long, long, long) position;
        public long radius;

        public Nanobot((long, long, long) position, long radius) {
            this.position = position;
            this.radius = radius;
        }

        public bool IsTargetInRange(Nanobot other) {
            (long x, long y, long z) = position;
            (long ox, long oy, long oz) = other.position;

            return (Math.Abs(x - ox) + Math.Abs(y - oy) + Math.Abs(z - oz)) <= radius;
        }

        public bool IsTargetInRange((long, long, long) target) {
            (long x, long y, long z) = position;
            (long ox, long oy, long oz) = target;

            return (Math.Abs(x - ox) + Math.Abs(y - oy) + Math.Abs(z - oz)) <= radius;
        }

        public override bool Equals(object obj) {
            Nanobot n = (Nanobot)obj;
            (long x, long y, long z) = position;
            (long nx, long ny, long nz) = n.position;
            return x == nx && y == ny && z == nz && radius == n.radius;
        }

        public override int GetHashCode() {
            return position.GetHashCode() * radius.GetHashCode();
        }
    }
}