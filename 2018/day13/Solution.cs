using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    // Up is negative
    private static (int, int) UP = (-1, 0);
    private static (int, int) RIGHT = (0, 1);
    private static (int, int) DOWN = (1, 0);
    private static (int, int) LEFT = (0, -1);

    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        Solve(input);
    }

    static void Solve(string[] input) {
        char[,] grid = new char[input.Length, input[0].Length];
        Dictionary<(int, int), Cart> carts = new Dictionary<(int, int), Cart>();

        for (int y = 0; y < input.Length; y++) {
            for (int x = 0; x < input[y].Length; x++) {
                grid[y, x] = input[y][x];
                char c = grid[y, x];

                if (c == '^' || c == '>' || c == 'v' || c == '<') {
                    Cart cart = new Cart((y, x), c);
                    carts.Add(cart.GetPosition(), cart);
                }
            }
        }

        int ticks = 0;
        while (true) {
            ticks++;
            List<(int, int)> order = carts.Keys.ToList();
            order.Sort((p1, p2) => {
                int result = p1.Item1.CompareTo(p2.Item1);
                return result == 0 ? p1.Item2.CompareTo(p2.Item2) : result;
            });

            HashSet<(int, int)> crashLocations = new HashSet<(int, int)>();
            foreach ((int, int) pos in order) {
                if (crashLocations.Contains(pos)) {
                    continue;
                }
                Cart cart = carts[pos];

                // First remove from dict, then move
                carts.Remove(pos);
                cart.Move();

                // Check for collisions
                (int cartY, int cartX) = cart.GetPosition();
                (int, int) cartPos = (cartY, cartX);
                if (carts.ContainsKey(cartPos)) {
                    Console.WriteLine($"Collision at {cartPos}");

                    // Need to keep track of crashed carts
                    crashLocations.Add(cartPos);
                    continue;
                }

                // Otherwise, set back in dict
                carts.Add(cartPos, cart);

                // Handle turns (check grid)
                (int, int) cartDir = cart.GetDirection();
                if (grid[cartY, cartX] == '+') {
                    cart.Turn();
                } else if (grid[cartY, cartX] == '\\') {
                    if (cartDir == UP || cartDir == DOWN) {
                        cart.TurnLeft();
                    } else {
                        cart.TurnRight();
                    }
                } else if (grid[cartY, cartX] == '/') {
                    if (cartDir == UP || cartDir == DOWN) {
                        cart.TurnRight();
                    } else {
                        cart.TurnLeft();
                    }
                }
            }

            foreach ((int, int) crashPos in crashLocations) {
                if (carts.ContainsKey(crashPos)) {
                    carts.Remove(crashPos);
                }
            }

            if (carts.Count == 1) {
                Console.WriteLine($"Last cart at: {carts.First().Key}");
                break;
            }
        }
    }

    private class Cart {
        private static (int, int)[] DIR = new (int, int)[] {
            UP, RIGHT, DOWN, LEFT
        };
        private (int, int) position;
        private int directionIndex;
        private int turnOrder;

        public Cart((int, int) position, char pointer) {
            this.position = position;
            this.turnOrder = 0;
            switch (pointer) {
                case '^': {
                    this.directionIndex = 0;
                    break;
                }
                case '>': {
                    this.directionIndex = 1;
                    break;
                }
                case 'v': {
                    this.directionIndex = 2;
                    break;
                }
                case '<': {
                    this.directionIndex = 3;
                    break;
                }
                default: {
                    this.directionIndex = 0;
                    break;
                }
            }
        }

        public void Move() {
            (int, int) direction = DIR[this.directionIndex];
            this.position.Item1 += direction.Item1;
            this.position.Item2 += direction.Item2;
        }

        public (int, int) GetPosition() {
            return this.position;
        }

        public (int, int) GetDirection() {
            return DIR[this.directionIndex];
        }

        public void Turn() {
            int turnType = this.turnOrder++ % 3;
            if (turnType == 1) {
                return;
            }

            bool leftTurn = turnType == 0;
            if (leftTurn) {
                TurnLeft();
            } else {
                TurnRight();
            }
        }

        public void TurnLeft() {
            this.directionIndex--;
            if (this.directionIndex < 0) {
                this.directionIndex += 4;
            }
        }

        public void TurnRight() {
            this.directionIndex = (this.directionIndex + 1) % 4;
        }
    }
}