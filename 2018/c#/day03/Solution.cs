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
        int[,] grid = new int[1000, 1000];
        foreach (string line in input) {
            string[] parsed = line.Split("@ ")[1].Split(": ");

            int[] indices = parsed[0].Split(",").Select(index => Int32.Parse(index)).ToArray();
            int[] dimensions = parsed[1].Split("x").Select(dimension => Int32.Parse(dimension)).ToArray();

            for (int y = 0; y < dimensions[1]; y++) {
                for (int x = 0; x < dimensions[0]; x++) {
                    int yIndex = y + indices[1];
                    int xIndex = x + indices[0];

                    if (grid[yIndex, xIndex] < 2) {
                        grid[yIndex, xIndex]++;
                    }
                }
            }
        }

        int idCheck = 1;
        foreach (string line in input) {
            string[] parsed = line.Split("@ ")[1].Split(": ");

            int[] indices = parsed[0].Split(",").Select(index => Int32.Parse(index)).ToArray();
            int[] dimensions = parsed[1].Split("x").Select(dimension => Int32.Parse(dimension)).ToArray();

            bool overlap = false;
            for (int y = 0; y < dimensions[1]; y++) {
                for (int x = 0; x < dimensions[0]; x++) {
                    int yIndex = y + indices[1];
                    int xIndex = x + indices[0];

                    if (grid[yIndex, xIndex] == 2) {
                        overlap = true;
                    }
                }
            }

            if (!overlap) {
                break;
            }
            idCheck++;
        }

        int count = grid.Cast<int>().Count(v => v == 2);
        Console.WriteLine(count);
        Console.WriteLine(idCheck);
    }
}