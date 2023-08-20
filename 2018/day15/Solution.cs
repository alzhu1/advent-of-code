using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    private static (int, int) UP = (-1, 0);
    private static (int, int) RIGHT = (0, 1);
    private static (int, int) DOWN = (1, 0);
    private static (int, int) LEFT = (0, -1);

    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        Solve(input);
    }

    static void Solve(string[] input) {
        bool done = false;
        for (int elfPower = 3; elfPower <= 200; elfPower++) {
            Dictionary<(int, int), Unit> units = new Dictionary<(int, int), Unit>();
            char[,] grid = new char[input.Length, input[0].Length];
            for (int y = 0; y < input.Length; y++) {
                for (int x = 0; x < input[y].Length; x++) {
                    char c = input[y][x];
                    if (c == 'E' || c == 'G') {
                        // Manually test with higher elf power
                        units.Add((y, x), new Unit((y, x), c, c == 'E' ? elfPower : 3));
                        c = '.';
                    }
                    grid[y, x] = c;
                }
            }

            int totalElfCount = units.Values.Count(x => x.type == 'E');

            int round = 0;
            while (true) {
                List<(int, int)> order = units.Keys.ToList();
                order.Sort((p1, p2) => {
                    int result = p1.Item1.CompareTo(p2.Item1);
                    return result == 0 ? p1.Item2.CompareTo(p2.Item2) : result;
                });

                foreach (Unit u in units.Values) {
                    u.acted = false;
                }

                foreach ((int, int) pos in order) {
                    if (units.ContainsKey(pos)) {
                        Unit unitToAct = units[pos];
                        if (!unitToAct.acted && unitToAct.Move(grid, units)) {
                            unitToAct.Attack(grid, units);
                        }

                        unitToAct.acted = true;
                    }
                }

                int gCount = units.Values.Count(x => x.type == 'G');
                int eCount = units.Values.Count(x => x.type == 'E');

                if (gCount == 0 || eCount == 0) {
                    if (eCount == totalElfCount) {
                        // Console.WriteLine("No elves died!");
                        done = true;
                    }
                    // Console.WriteLine($"Done at round {round}");
                    break;
                }

                round++;
            }

            int leftoverHp = units.Values.Sum(unit => unit.hp);

            if (elfPower == 3) {
                Console.WriteLine($"Part 1: Round ({round}) * HP {leftoverHp} = {round * leftoverHp}");
            }

            if (done) {
                Console.WriteLine($"Part 2: Elf power is {elfPower}, Round ({round}) * HP {leftoverHp} = {round * leftoverHp}");
                break;
            }
        }
    }

    private class Unit {
        private static (int, int)[] DIR = new (int, int)[] {
            UP, LEFT, RIGHT, DOWN
        };

        public (int, int) position;
        public char type;
        public int hp = 200;
        public int attack;
        public bool acted;

        public Unit((int, int) position, char type, int attackPower) {
            this.position = position;
            this.type = type;
            this.attack = attackPower;
        }

        public bool Move(char[,] grid, Dictionary<(int, int), Unit> units) {
            // Don't move if an enemy unit in range already exists
            foreach ((int yy, int xx) in DIR) {
                (int, int) checkPos = (position.Item1 + yy, position.Item2 + xx);
                if (units.ContainsKey(checkPos) && this.IsEnemy(units[checkPos])) {
                    return true;
                }
            }

            // Otherwise, do a BFS
            Queue<Node> queue = new Queue<Node>();
            queue.Enqueue(new Node((position.Item1, position.Item2), 0));

            HashSet<Node> seen = new HashSet<Node>();
            List<Node> enemyLocations = new List<Node>();
            while (queue.Count > 0) {
                Node currNode = queue.Dequeue();
                (int currY, int currX) = currNode.position;
                int currDist = currNode.dist;

                // If here before or encounter a wall, skip
                if (seen.Contains(currNode) || grid[currY, currX] == '#') {
                    continue;
                }
                seen.Add(currNode);

                // If position is occupied by a unit, skip
                bool unitBlock = currDist > 0 && units.ContainsKey(currNode.position);
                if (unitBlock) {
                    continue;
                }

                // Check if this position's NEIGHBORS are enemies
                foreach ((int yy, int xx) in DIR) {
                    (int, int) checkPos = (currY + yy, currX + xx);
                    if (units.ContainsKey(checkPos) && this.IsEnemy(units[checkPos])) {
                        enemyLocations.Add(currNode);
                        break;
                    }
                }

                // Enqueue neighbors
                foreach ((int yy, int xx) in DIR) {
                    (int, int) nextPos = (currY + yy, currX + xx);
                    Node nextNode = new Node(nextPos, currDist + 1);
                    nextNode.prev = currNode;
                    queue.Enqueue(nextNode);
                }
            }

            if (enemyLocations.Count == 0) {
                // No enemies, so stop
                return false;
            }

            // Pick the closest enemy location in reading order
            enemyLocations.Sort((p1, p2) => {
                int result = p1.position.Item1.CompareTo(p2.position.Item1);
                return result == 0 ? p1.position.Item2.CompareTo(p2.position.Item2) : result;
            });
            Node goToEnemy = enemyLocations.Aggregate((curr, next) => curr.dist > next.dist ? next : curr);

            // Do another BFS from neighbors of curr to find direction to minimize dist
            int minDistFromAdj = Int16.MaxValue;
            (int, int) nextOne = (-1, -1);
            foreach ((int yyy, int xxx) in DIR) {
                queue.Clear();
                seen.Clear();
                (int, int) checkStartPos = (position.Item1 + yyy, position.Item2 + xxx);
                queue.Enqueue(new Node(checkStartPos, 0));

                while (queue.Count > 0) {
                    Node currNode = queue.Dequeue();
                    (int currY, int currX) = currNode.position;
                    int currDist = currNode.dist;

                    if (seen.Contains(currNode) || grid[currY, currX] == '#') {
                        continue;
                    }
                    seen.Add(currNode);

                    bool unitBlock = units.ContainsKey(currNode.position);
                    if (unitBlock) {
                        continue;
                    }

                    if (currY == goToEnemy.position.Item1 && currX == goToEnemy.position.Item2) {
                        if (currDist < minDistFromAdj) {
                            minDistFromAdj = currDist;
                            nextOne = checkStartPos;
                        }
                        break;
                    }

                    foreach ((int yy, int xx) in DIR) {
                        (int, int) nextPos = (currY + yy, currX + xx);
                        Node nextNode = new Node(nextPos, currDist + 1);
                        nextNode.prev = currNode;
                        queue.Enqueue(nextNode);
                    }
                }
            }

            // Move!
            units.Remove(this.position);
            this.position = nextOne;
            units.Add(this.position, this);

            // One final check to see if we can attack
            foreach ((int yy, int xx) in DIR) {
                (int, int) checkPos = (position.Item1 + yy, position.Item2 + xx);
                if (units.ContainsKey(checkPos) && this.IsEnemy(units[checkPos])) {
                    return true;
                }
            }

            return false;
        }

        public void Attack(char[,] grid, Dictionary<(int, int), Unit> units) {
            Unit unitToAttack = null;
            foreach ((int yy, int xx) in DIR) {
                (int, int) checkPos = (position.Item1 + yy, position.Item2 + xx);
                if (units.ContainsKey(checkPos) && this.IsEnemy(units[checkPos])) {
                    int currUnitHp = unitToAttack?.hp ?? Int16.MaxValue;

                    if (units[checkPos].hp < currUnitHp) {
                        unitToAttack = units[checkPos];
                    }
                }
            }

            if (unitToAttack == null) {
                return;
            }

            // Attack!
            unitToAttack.hp -= this.attack;
            if (unitToAttack.hp <= 0) {
                units.Remove(unitToAttack.position);
            }
        }

        public bool IsEnemy(Unit other) {
            return this.type != other.type;
        }
    }

    private class Node {
        public (int, int) position;
        public int dist;
        public Node prev;

        public Node((int, int) position, int dist) {
            this.position = position;
            this.dist = dist;
        }

        public override bool Equals(object obj) {
            Node n = (Node)obj;
            return position.Item1 == n.position.Item1 && position.Item2 == n.position.Item2;
        }

        public override int GetHashCode() {
            return position.GetHashCode();
        }
    }
}