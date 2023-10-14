using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

public class Solution {
    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");
        int freq = 0;

        HashSet<int> seen = new HashSet<int>();
        seen.Add(freq);

        bool foundRepeat = false;
        int repeat = 0;
        while (!foundRepeat) {
            foreach (string s in input) {
                freq += Int32.Parse(s);

                if (!foundRepeat && seen.Contains(freq)) {
                    repeat = freq;
                    foundRepeat = true;
                }
                seen.Add(freq);
            }

            Console.WriteLine($"End frequency: {freq}");
        }

        Console.WriteLine($"Repeat: {repeat}");
    }
}