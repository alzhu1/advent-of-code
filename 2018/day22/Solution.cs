using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    public static void Main(string[] args) {
        if (args.Length != 3) {
            return;
        }

        Solve(long.Parse(args[0]), long.Parse(args[1]), long.Parse(args[2]));
    }

    static void Solve(long depth, long targetX, long targetY) {
        Dictionary<(long, long), long> erosionLevels = new Dictionary<(long, long), long>();

        long totalRisk = 0;
        for (long y = 0; y <= targetY; y++) {
            for (long x = 0; x <= targetX; x++) {
                // Calculate geologic index

                long geologicIndex;
                if ((x == 0 && y == 0) || (x == targetX && y == targetY)) {
                    geologicIndex = 0;
                    // geologicIndices.Add((x, y), 0);
                } else if (y == 0) {
                    geologicIndex = x * 16807;
                    // geologicIndices.Add((x, y), geologicIndex);
                } else if (x == 0) {
                    geologicIndex = y * 48271;
                    // geologicIndices.Add((x, y), geologicIndex);
                } else {
                    geologicIndex = erosionLevels[(x-1, y)] * erosionLevels[(x, y-1)];
                }

                long erosionLevel = (geologicIndex + depth) % 20183;
                erosionLevels.Add((x, y), erosionLevel);

                totalRisk += (erosionLevel % 3);
            }
        }

        // Console.WriteLine(totalRisk);

        // Now we search
        // Last number represents tool: 0 for neither, 1 for torch, 2 for climbing
        Queue<(long, long, long, int)> queue = new Queue<(long, long, long, int)>();
        queue.Enqueue((0, 0, 0, 1));

        // Max is if we have to switch tools every traversal to get close
        long maxMinute = (targetX + targetY) * 8;

        (long, long)[] directions = new (long, long)[]{
            (0, -1), // up
            (1, 0), // right
            (0, 1), // down
            (-1, 0) // left
        };

        long smallestMin = long.MaxValue;

        Dictionary<(long, long, int), long> posMinute = new Dictionary<(long, long, int), long>();
        while (queue.Count > 0) {
            (long x, long y, long min, int toolSet) = queue.Dequeue();
            (long, long, int) pos = (x, y, toolSet);

            if (min >= maxMinute) {
                continue;
            }

            if (posMinute.ContainsKey(pos)) {
                long storedMin = posMinute[pos];

                if (storedMin <= min) {
                    // We'v been here before with a better minute, so skip it
                    continue;
                }
                posMinute[pos] = min;
            } else {
                posMinute.Add(pos, min);
            }

            if (x == targetX && y == targetY) {
                // Console.WriteLine($"Arrived at target at min = {min}");
                if (min < smallestMin) {
                    Console.WriteLine($"Smallest min found at {min}");
                    smallestMin = min;
                }
                continue;
            }

            long distToTarget = Math.Abs(x - targetX) + Math.Abs(y - targetY);
            // Assuming best case (no tool switching), if we can't beat, don't try
            if (min + distToTarget >= smallestMin) {
                continue;
            }

            // Traverse and/or switch

            // Can only switch with 1 tool in our terrain
            long currType = GetType(x, y, targetX, targetY, depth, erosionLevels);
            int switchableTool = (currType + 1) % 3 == toolSet ? (toolSet + 1) % 3 : (toolSet + 2) % 3;
            bool couldSwitch = false;

            foreach ((long dx, long dy) in directions) {
                long nextType = GetType(x + dx, y + dy, targetX, targetY, depth, erosionLevels);
                if (nextType >= 0) {
                    // If you can go there now, enqueue
                    if (IsToolValid(nextType, toolSet)) {
                        queue.Enqueue((x + dx, y + dy, min + 1, toolSet));
                    }

                    // Otherwise, if we can switch, mark it down
                    couldSwitch = couldSwitch || IsToolValid(nextType, switchableTool);
                }
            }

            // If could switch, enqueue that
            if (couldSwitch) {
                queue.Enqueue((x, y, min + 7, switchableTool));
            }
        }
    }

    static long GetErosionLevel(long x, long y, long targetX, long targetY, long depth, Dictionary<(long, long), long> erosionLevels) {
        if (x < 0 || y < 0) {
            return -1;
        }

        long geologicIndex;
        if ((x == 0 && y == 0) || (x == targetX && y == targetY)) {
            geologicIndex = 0;
        } else if (y == 0) {
            geologicIndex = x * 16807;
        } else if (x == 0) {
            geologicIndex = y * 48271;
        } else {
            long leftErosion = erosionLevels.ContainsKey((x-1, y)) ? erosionLevels[(x-1, y)] : GetErosionLevel(x-1, y, targetX, targetY, depth, erosionLevels);
            long upErosion = erosionLevels.ContainsKey((x, y-1)) ? erosionLevels[(x, y-1)] : GetErosionLevel(x, y-1, targetX, targetY, depth, erosionLevels);
            geologicIndex = leftErosion * upErosion;
        }

        long erosionLevel = (geologicIndex + depth) % 20183;
        if (!erosionLevels.ContainsKey((x, y))) {
            erosionLevels.Add((x, y), erosionLevel);
        }

        return erosionLevel;
    }

    static long GetType(long x, long y, long targetX, long targetY, long depth, Dictionary<(long, long), long> erosionLevels) {
        return GetErosionLevel(x, y, targetX, targetY, depth, erosionLevels) % 3;
    }

    static bool IsToolValid(long type, int tool) {
        return type != tool;
    }
}