using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    public static void Main(string[] args) {
        if (args.Length != 1) {
            return;
        }

        Solve(Int32.Parse(args[0]));
    }

    static void Solve(int serialNumber) {
        // Part 1
        Console.WriteLine($"Largest power for 3x3: {GetSquareOfLargest(3, serialNumber)}");

        // Part 2
        int currLargestPower = Int32.MinValue;
        (int, int) lowestCoords = (-1, -1);
        int squareSize = -1;
        for (int i = 1; i <= 300; i++) {
            (int x, int y, int powerAtSize) = GetSquareOfLargest(i, serialNumber);

            if (powerAtSize > currLargestPower) {
                currLargestPower = powerAtSize;
                lowestCoords = (x, y);
                squareSize = i;
            }
        }

        Console.WriteLine($"Best power is with square size {squareSize}, at {lowestCoords}");
    }

    static (int, int, int) GetSquareOfLargest(int squareSize, int serialNumber) {
        int currLargestPower = Int32.MinValue;
        (int, int) lowestCoords = (-1, -1);

        int[,] cache = new int[301, 301];
        int row = cache.GetLength(0);
        int col = cache.GetLength(1);      

        for (int i = 0; i < row * col; i++) {
            cache[i / col , i % col] = Int32.MinValue;
        }

        for (int x = 1; x <= 300 - squareSize + 1; x++) {
            for (int y = 1; y <= 300 - squareSize + 1; y++) {
                int powerSum;
                if (cache[x - 1, y] != Int32.MinValue) {
                    // If previous in x direction is calculated, remove previous col and add next
                    powerSum = cache[x - 1, y];
                    for (int yy = 0; yy < squareSize; yy++) {
                        powerSum -= GetPowerAtPos(x - 1, y + yy, serialNumber);
                        powerSum += GetPowerAtPos(x + squareSize - 1, y + yy, serialNumber);
                    }
                } else if (cache[x, y - 1] != Int32.MinValue) {
                    // Same for y direction
                    powerSum = cache[x, y - 1];
                    for (int xx = 0; xx < squareSize; xx++) {
                        powerSum -= GetPowerAtPos(x + xx, y - 1, serialNumber);
                        powerSum += GetPowerAtPos(x + xx, y + squareSize - 1, serialNumber);
                    }
                } else {
                    // If uncached, calculate whole square
                    powerSum = 0;
                    for (int xx = 0; xx < squareSize; xx++) {
                        for (int yy = 0; yy < squareSize; yy++) {
                            powerSum += GetPowerAtPos(x + xx, y + yy, serialNumber);
                        }
                    }
                }
                cache[x, y] = powerSum;

                if (powerSum > currLargestPower) {
                    currLargestPower = powerSum;
                    lowestCoords = (x, y);
                }
            }
        }

        return (lowestCoords.Item1, lowestCoords.Item2, currLargestPower);
    }

    static int GetPowerAtPos(int x, int y, int serialNumber) {
        int rackId = x + 10;
        int powerLevel = ((rackId * y) + serialNumber) * rackId;
        powerLevel /= 100;
        powerLevel %= 10;
        return powerLevel - 5;
    }
}