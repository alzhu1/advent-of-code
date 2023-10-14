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
        string regex = input[0];

        // Possible door/rooms; even coords = room, even/odd coord is door
        HashSet<(int, int)> possibles = new HashSet<(int, int)>();
        possibles.Add((0, 0));

        HashSet<(int, int)> start = new HashSet<(int, int)>();
        start.Add((0, 0));
        ParseRegex(0, regex, possibles, start);

        // BFS on possibles
        Queue<(int, int, int)> queue = new Queue<(int, int, int)>();
        HashSet<(int, int)> seen = new HashSet<(int, int)>();
        int maxDist = 0;
        int largeDoorCounter = 0;
        queue.Enqueue((0, 0, 0));
        while (queue.Count > 0) {
            (int x, int y, int numDoors) = queue.Dequeue();
            (int, int) pos = (x, y);

            if (seen.Contains(pos)) {
                continue;
            }
            seen.Add(pos);
            maxDist = Math.Max(maxDist, numDoors);

            if (numDoors >= 1000) {
                largeDoorCounter++;
            }

            // Check directions for doors
            if (possibles.Contains((x + 1, y))) {
                queue.Enqueue((x + 2, y, numDoors + 1));
            }
            if (possibles.Contains((x, y + 1))) {
                queue.Enqueue((x, y + 2, numDoors + 1));
            }
            if (possibles.Contains((x - 1, y))) {
                queue.Enqueue((x - 2, y, numDoors + 1));
            }
            if (possibles.Contains((x, y - 1))) {
                queue.Enqueue((x, y - 2, numDoors + 1));
            }
        }
        Console.WriteLine(maxDist);
        Console.WriteLine(largeDoorCounter);
    }

    static (HashSet<(int, int)>, int) ParseRegex(int start,
                                              string regex,
                                              HashSet<(int, int)> possibles,
                                              HashSet<(int, int)> startingPositions) {
        // Track current active position (will always move)
        List<(int, int)> trackingPositions = new List<(int, int)>();
        trackingPositions.AddRange(startingPositions);

        // Keep track of ending positions (relevant in recursive calls)
        HashSet<(int, int)> endPositions = new HashSet<(int, int)>();
        while (start < regex.Length) {
            if (regex[start] == '^') {
                start++;
                continue;
            }

            if (regex[start] == '$') {
                break;
            }

            if (regex[start] == '(') {
                // Need to branch starting from our tracking positions
                HashSet<(int, int)> temp = new HashSet<(int, int)>();
                foreach ((int, int) t in trackingPositions) {
                    temp.Add(t);
                }
                (HashSet<(int, int)> finalPositions, int nextIndex) = ParseRegex(start + 1, regex, possibles, temp);

                // At end of branching, update tracking positions with where we ended up
                trackingPositions.Clear();
                trackingPositions.AddRange(finalPositions);
                start = nextIndex;

                // Need to skip to next iteration after traversal (parse rest of regex)
                continue;
            }

            if (regex[start] == '|' || regex[start] == ')') {
                // This means that our current tracking positions are done
                // Move all of them to endPositions, reset tracking to start
                foreach ((int, int) t in trackingPositions) {
                    endPositions.Add(t);
                }

                trackingPositions.Clear();
                trackingPositions.AddRange(startingPositions);

                if (regex[start] == ')') {
                    // In the ) case, return our endPositions and move to next index
                    return (endPositions, start + 1);
                }
            }

            // Handle directions
            for (int i = 0; i < trackingPositions.Count; i++) {
                (int currX, int currY) = trackingPositions[i];

                int dx = 0;
                int dy = 0;
                if (regex[start] == 'N') {
                    dy = 2;
                } else if (regex[start] == 'E') {
                    dx = 2;
                } else if (regex[start] == 'S') {
                    dy = -2;
                } else if (regex[start] == 'W') {
                    dx = -2;
                }

                (int, int) door = (currX + (dx / 2), currY + (dy / 2));
                (int, int) nextPos = (currX + dx, currY + dy);

                possibles.Add(door);
                possibles.Add(nextPos);
                trackingPositions[i] = nextPos;
            }

            start++;
        }

        return (endPositions, regex.Length);
    }
}