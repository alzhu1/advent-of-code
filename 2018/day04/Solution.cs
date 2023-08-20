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
        List<string> order = Enumerable.ToList<string>(input);
        order.Sort();

        Dictionary<int, Guard> guards = new Dictionary<int, Guard>();
        int guardId = 0;
        int sleepTime = 60;
        foreach (string line in order) {
            string[] parts = line.Split("] ");

            if (parts[1][0] == 'G') {
                // First, check if previous guard should still be sleeping
                if (sleepTime < 60) {
                    guards[guardId].SetSleepTime(sleepTime, 59);
                }

                // New schedule for guard
                guardId = Int32.Parse(line.Split("#")[1].Split(" ")[0]);
                sleepTime = 0;

                Guard g;
                if (!guards.ContainsKey(guardId)) {
                    g = new Guard(guardId);
                    guards.Add(guardId, g);
                } else {
                    g = guards[guardId];
                }
                g.AddNewSchedule();

                continue;
            }

            Guard guard = guards[guardId];
            int minute = Int32.Parse(parts[0].Split(":")[1]);
            if (parts[1][0] == 'f') {
                sleepTime = minute;
            } else if (parts[1][0] == 'w') {
                guard.SetSleepTime(sleepTime, minute - 1);
                sleepTime = 60;
            }
        }

        // Part 1
        // Guard targetGuard = guards.Values.Aggregate((curr, next) => next.GetTotalSleep() > curr.GetTotalSleep() ? next : curr);

        // Part 2
        Guard targetGuard = guards.Values.Aggregate((curr, next) => next.GetMinuteAndTimesMostSlept().Item2 > curr.GetMinuteAndTimesMostSlept().Item2 ? next : curr);
        Console.WriteLine($"{targetGuard.id}: most slept at {targetGuard.GetMinuteAndTimesMostSlept()}");
    }

    private struct Guard {
        public int id;
        public List<bool[]> schedules;

        public Guard(int id) {
            this.id = id;
            this.schedules = new List<bool[]>();
        }

        public void AddNewSchedule() {
            schedules.Add(new bool[60]);
        }

        public void SetSleepTime(int start, int end) {
            if (start >= 60) {
                return;
            }

            bool[] lastSchedule = schedules.Last();
            for (int i = start; i <= end; i++) {
                lastSchedule[i] = true;
            }
        }

        public int GetTotalSleep() {
            return schedules.Sum<bool[]>(schedule => schedule.Count<bool>(s => s));
        }

        public (int, int) GetMinuteAndTimesMostSlept() {
            int index = 0;
            int sleepCount = 0;
            for (int i = 1; i < 60; i++) {
                int currSleepCount = 0;
                for (int s = 0; s < schedules.Count; s++) {
                    currSleepCount += (schedules[s][i] ? 1 : 0);
                }

                if (currSleepCount > sleepCount) {
                    sleepCount = currSleepCount;
                    index = i;
                }
            }

            return (index, sleepCount);
        }
    }
}