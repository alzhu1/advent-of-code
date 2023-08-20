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

        // Solve(Int32.Parse(args[0]));
        Solve2(Int32.Parse(args[0]));
    }

    static void Solve(int recipes) {
        LinkedList<int> recipeList = new LinkedList<int>();
        recipeList.AddFirst(3);
        recipeList.AddLast(7);

        LinkedListNode<int> elf1 = recipeList.First;
        LinkedListNode<int> elf2 = elf1.Next;

        while (recipeList.Count < recipes + 10) {
            int sum = elf1.Value + elf2.Value;
            bool firstInsert = true;
            do {
                if (firstInsert) {
                    recipeList.AddLast(sum % 10);
                    firstInsert = false;
                } else {
                    recipeList.AddBefore(recipeList.Last, sum % 10);
                }
                sum /= 10;
            } while (sum > 0);

            int elf1Move = elf1.Value + 1;
            int elf2Move = elf2.Value + 1;

            while (elf1Move-- > 0) {
                elf1 = elf1.Next != null ? elf1.Next : recipeList.First;
            }
            while (elf2Move-- > 0) {
                elf2 = elf2.Next != null ? elf2.Next : recipeList.First;
            }
        }

        LinkedListNode<int> endNode = recipeList.Last;
        int recipeCounter = recipeList.Count;
        while (recipeCounter-- > recipes + 10) {
            endNode = endNode.Previous;
        }

        string final = "";
        for (int i = 0; i < 10; i++) {
            // Console.Write(endNode.Value);
            final = $"{endNode.Value}{final}";
            endNode = endNode.Previous;
        }
        final.Reverse();
        Console.WriteLine(final);
    }

    static void Solve2(int target) {
        List<int> targetDigits = new List<int>();
        int t = target;
        while (t > 0) {
            targetDigits.Add(t % 10);
            t /= 10;
        }
        targetDigits.Reverse();

        LinkedList<int> recipeList = new LinkedList<int>();
        recipeList.AddFirst(3);
        recipeList.AddLast(7);

        LinkedListNode<int> elf1 = recipeList.First;
        LinkedListNode<int> elf2 = elf1.Next;

        LinkedListNode<int> checker = recipeList.First;
        int count = 0;
        int correctDigits = 0;

        while (true) {
            int sum = elf1.Value + elf2.Value;
            bool firstInsert = true;
            do {
                if (firstInsert) {
                    recipeList.AddLast(sum % 10);
                    firstInsert = false;
                } else {
                    recipeList.AddBefore(recipeList.Last, sum % 10);
                }
                sum /= 10;
            } while (sum > 0);

            int elf1Move = elf1.Value + 1;
            int elf2Move = elf2.Value + 1;

            while (elf1Move-- > 0) {
                elf1 = elf1.Next != null ? elf1.Next : recipeList.First;
            }
            while (elf2Move-- > 0) {
                elf2 = elf2.Next != null ? elf2.Next : recipeList.First;
            }

            // Now check for correctness
            bool found = false;
            while (checker.Next != null) {
                count++;
                if (checker.Value != targetDigits[correctDigits]) {
                    correctDigits = 0;
                } else {
                    correctDigits++;

                    if (correctDigits >= targetDigits.Count) {
                        Console.WriteLine($"Found it at {count - targetDigits.Count}");
                        found = true;
                        break;
                    }
                }

                checker = checker.Next;
            }

            if (found) {
                break;
            }
        }
    }
}