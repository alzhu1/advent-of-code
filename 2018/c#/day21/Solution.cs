using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

public class Solution {
    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        // Solve(input);
        Calculator();
    }

    static void Solve(string[] input) {
        long[] registers = new long[6];
        long boundRegister = long.Parse(input[0].Split(" ")[1]);
        long instructionPointer = 0;

        string[] instructions = new string[input.Length - 1];
        for (int i = 0; i < instructions.Length; i++) {
            instructions[i] = input[i + 1];
        }

        while (instructionPointer >= 0 && instructionPointer < instructions.Length) {
            // Write ip to register bound
            registers[boundRegister] = instructionPointer;
            string[] instr = instructions[instructionPointer].Split(" ");
            long a = long.Parse(instr[1]);
            long b = long.Parse(instr[2]);
            long c = long.Parse(instr[3]);

            // if (instructionPointer == 28) {
            //     Console.Write($"ip={instructionPointer} [");
            //     foreach (long r in registers) {
            //         Console.Write($"{r},");
            //     }
            //     Console.Write("] ");
            //     Console.Write($"{instr[0]} {instr[1]} {instr[2]} {instr[3]}");
            // }

            // Execute instruction
            Operate(registers, instr[0], a, b, c);

            // if (instructionPointer == 28) {
            //     Console.Write($" [");
            //     foreach (long r in registers) {
            //         Console.Write($"{r},");
            //     }
            //     Console.Write("]");
            //     Console.Read();
            // }

            // Register back to ip, and add one
            instructionPointer = registers[boundRegister];
            instructionPointer++;
        }

        Console.WriteLine(registers[0]);
    }

    static void Operate(long[] registers, string opToTest, long a, long b, long c) {
        switch (opToTest) {
            case "addr": { registers[c] = registers[a] + registers[b]; break; }
            case "addi": { registers[c] = registers[a] + b; break; }
            case "mulr": { registers[c] = registers[a] * registers[b]; break; }
            case "muli": { registers[c] = registers[a] * b; break; }
            case "banr": { registers[c] = registers[a] & registers[b]; break; }
            case "bani": { registers[c] = registers[a] & b; break; }
            case "borr": { registers[c] = registers[a] | registers[b]; break; }
            case "bori": { registers[c] = registers[a] | b; break; }
            case "setr": { registers[c] = registers[a]; break; }
            case "seti": { registers[c] = a; break; }
            case "gtir": { registers[c] = a > registers[b] ? 1 : 0; break; }
            case "gtri": { registers[c] = registers[a] > b ? 1 : 0; break; }
            case "gtrr": { registers[c] = registers[a] > registers[b] ? 1 : 0; break; }
            case "eqir": { registers[c] = a == registers[b] ? 1 : 0; break; }
            case "eqri": { registers[c] = registers[a] == b ? 1 : 0; break; }
            case "eqrr": { registers[c] = registers[a] == registers[b] ? 1 : 0; break; }
        }
    }

    /**
        Not sure if this algorithm has semantic meaning, but this is a code version
        of the assembly program. To break out of it, reg0 = reg5 when reg4 < 256 in loop
    */
    static void Calculator() {
        // Bottom loop: find min value x such that (x + 1) * 256 is greater than a target number
        long reg4 = 65536;
        long reg5 = 13159625;

        long small = long.MaxValue;
        int i = 0;

        HashSet<long> seen = new HashSet<long>();
        while (true) {
            reg5 += (reg4 & 255);
            reg5 &= 16777215;
            reg5 *= 65899;
            reg5 &= 16777215;

            if (reg4 < 256) {
                if (!seen.Contains(reg5)) {
                    Console.WriteLine($"First occurrence: {reg5}");
                    seen.Add(reg5);
                }
                if (reg5 < small) {
                    Console.WriteLine($"Next smallest: {reg5}");
                    small = reg5;
                }
                reg4 = reg5 | 65536;
                reg5 = 13159625;
                continue;
            }

            reg4 /= 256;
        }
    }
}