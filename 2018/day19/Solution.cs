using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

public class Solution {
    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        Solve(input);
        Console.WriteLine($"Part 1: r0 = {SumFactors(898)}");
        Console.WriteLine($"Part 2: r0 = {SumFactors(10551298)}");
    }

    /**
        At a high level, this program attempts to find the sum of factors of register 2

        When r0 = 0 starting out, reg2 = 898
        When r0 = 1 starting out, more setup work occurs, manual run -> reg2 = 10551298

        Translating from instructions in input:
        r0 = 0, reg2 = ((2 * 2) * 19 * 11) + ((2 * 22) + 18)
        r0 = 1, reg2 = ((2 * 2) * 19 * 11) + ((2 * 22) + 18) + (((27 * 28) + 29) * 30 * 14 * 32)
    */

    static void Solve(string[] input) {
        int[] registers = new int[6];
        int boundRegister = Int16.Parse(input[0].Split(" ")[1]);
        int instructionPointer = 0;

        string[] instructions = new string[input.Length - 1];
        for (int i = 0; i < instructions.Length; i++) {
            instructions[i] = input[i + 1];
        }

        while (instructionPointer >= 0 && instructionPointer < instructions.Length) {
            // Write ip to register bound
            registers[boundRegister] = instructionPointer;
            string[] instr = instructions[instructionPointer].Split(" ");
            int a = Int16.Parse(instr[1]);
            int b = Int16.Parse(instr[2]);
            int c = Int16.Parse(instr[3]);

            // Execute instruction
            Operate(registers, instr[0], a, b, c);

            // Register back to ip, and add one
            instructionPointer = registers[boundRegister];
            instructionPointer++;
        }

        Console.WriteLine(registers[0]);
    }

    static void Operate(int[] registers, string opToTest, int a, int b, int c) {
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

    static int SumFactors(int num) {
        int sum = 0;
        for (int i = 1; i < Math.Sqrt(num); i++) {
            if (num % i == 0) {
                sum += (i + (num / i));
            }
        }
        return sum;
    }
}