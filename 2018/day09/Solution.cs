using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    public static void Main(string[] args) {
        if (args.Length != 2) {
            return;
        }

        Solve(long.Parse(args[0]), long.Parse(args[1]));
    }

    static void Solve(long players, long marbles) {
        // Idea for LinkedList came from Reddit (i.e. O(1) insertion is important)
        LinkedList<int> marblePlaces = new LinkedList<int>();
        long[] scores = new long[players];

        // Simulate first few runs
        marblePlaces.AddFirst(0);
        LinkedListNode<int> curr = marblePlaces.First;
        marblePlaces.AddAfter(curr, 2);
        marblePlaces.AddAfter(curr.Next, 1);
        int currTurn = 3;
        int currMarbleToPlace = 3;

        while (currMarbleToPlace <= marbles) {
            if (currMarbleToPlace % 23 == 0) {
                int scoreToAdd = 0;
                scoreToAdd += currMarbleToPlace;

                // Remove the marble 7 before
                int prev = 7;
                while (prev-- > 0) {
                    curr = curr.Previous != null ? curr.Previous : marblePlaces.Last;
                }

                LinkedListNode<int> toRemove = curr.Next != null ? curr.Next : marblePlaces.First;
                scoreToAdd += toRemove.Value;

                scores[currTurn] += scoreToAdd;
                marblePlaces.Remove(toRemove);
                currMarbleToPlace++;
            } else {
                int next = 2;
                while (next-- > 0) {
                    curr = curr.Next != null ? curr.Next : marblePlaces.First;
                }

                marblePlaces.AddAfter(curr, currMarbleToPlace++);
            }

            currTurn = (currTurn + 1) % scores.Length;
        }

        Console.WriteLine(scores.Max());
    }
}