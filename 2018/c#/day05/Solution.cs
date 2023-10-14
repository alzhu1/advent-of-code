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
        Console.WriteLine(GetReducedPolymerSize(input[0]));
    }

    static void Solve2(string[] input) {
        string polymer = input[0];
        char bestChar = 'a';
        int size = Int32.MaxValue;
        for (char c = 'a'; c <= 'z'; c++) {
            string lower = c.ToString();
            string higher = ((char)(c - 32)).ToString();
            int polymerSize = GetReducedPolymerSize(polymer.Replace(lower, String.Empty).Replace(higher, String.Empty));
            
            if (polymerSize < size) {
                size = polymerSize;
                bestChar = c;
            }
        }
        Console.WriteLine($"{bestChar}: smallest length is {size}");
    }

    static int GetReducedPolymerSize(string polymer) {
        while (true) {
            bool noReactions = true;
            int i = 0;
            while (i < polymer.Length - 1) {
                if (Math.Abs(polymer[i] - polymer[i + 1]) == 32) {
                    polymer = $"{polymer.Substring(0, i)}{polymer.Substring(i + 2)}";
                    i -= 2;
                    if (i < 0) {
                        i = 0;
                    }
                    noReactions = false;
                }

                i++;
            }

            if (noReactions) {
                break;
            }
        }

        return polymer.Length;
    }
}