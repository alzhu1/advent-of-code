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
        string[] ops = new string[] {
            "addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori",
            "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"
        };
        int[] beforeRegisters = new int[4];
        int[] operationNums = new int[4];
        int[] afterRegisters = new int[4];

        Dictionary<int, string> correctOpcodeMap = new Dictionary<int, string>();
        Dictionary<int, List<string>> possibleOpcodeMap = new Dictionary<int, List<string>>();

        Dictionary<int, int> possibleToTotal = new Dictionary<int, int>();
        for (int i = 0; i < input.Length; i += 4) {
            int possibleCount = 0;
            string beforeState = input[i].Split("[")[1];
            string operation = input[i + 1];
            string afterState = input[i + 2].Split("[")[1];
            // i + 3 is blank line

            string[] beforeValues = beforeState.Substring(0, beforeState.Length - 1).Split(", ");
            string[] operationValues = operation.Split(" ");
            string[] afterValues = afterState.Substring(0, afterState.Length - 1).Split(", ");

            for (int r = 0; r < 4; r++) {
                beforeRegisters[r] = Int16.Parse(beforeValues[r]);
                operationNums[r] = Int16.Parse(operationValues[r]);
                afterRegisters[r] = Int16.Parse(afterValues[r]);
            }

            // Don't need to check this opcode
            if (correctOpcodeMap.ContainsKey(operationNums[0])) {
                continue;
            }

            // Test registers
            List<string> possibleOpcodes = new List<string>();
            foreach (string o in ops) {
                int p = EqualRegisters(beforeRegisters, afterRegisters, operationNums, o);
                if (p == 1) {
                    possibleOpcodes.Add(o);
                }
                possibleCount += p;
            }

            if (possibleOpcodes.Count == 1) {
                // Opcode must match
                correctOpcodeMap.Add(operationNums[0], possibleOpcodes[0]);
            } else {
                // These are possible opcodes
                if (!possibleOpcodeMap.ContainsKey(operationNums[0])) {
                    possibleOpcodeMap.Add(operationNums[0], new List<string>());
                }
                possibleOpcodeMap[operationNums[0]].AddRange(possibleOpcodes);
            }

            if (!possibleToTotal.ContainsKey(possibleCount)) {
                possibleToTotal.Add(possibleCount, 0);
            }
            possibleToTotal[possibleCount]++;
        }

        Console.WriteLine(possibleToTotal.Sum(x => x.Key >= 3 ? x.Value : 0));

        // Need to process possibles
        while (correctOpcodeMap.Count < 16) {
            List<(int, string)> correctOnes = new List<(int, string)>();
            foreach (string opcode in correctOpcodeMap.Values) {
                foreach (var p in possibleOpcodeMap) {
                    List<string> possibles = p.Value;
                    possibles.RemoveAll(x => x.Equals(opcode));

                    if (possibles.Distinct().Count() == 1) {
                        correctOnes.Add((p.Key, possibles[0]));
                        possibles.Clear();
                    }
                }
            }

            foreach ((int op, string opcode) in correctOnes) {
                correctOpcodeMap.Add(op, opcode);
            }
        }

        string[] lastOps = File.ReadAllLines("part2Input.txt");
        int[] finalRegisters = new int[] { 0, 0, 0, 0 };
        foreach (string line in lastOps) {
            string[] operationString = line.Split(" ");
            int[] opNums = new int[4];
            for (int i = 0; i < 4; i++) {
                opNums[i] = Int16.Parse(operationString[i]);
            }

            Operate(finalRegisters, opNums, correctOpcodeMap);
        }

        Console.WriteLine(finalRegisters[0]);
    }

    static void Operate(int[] registers, int[] op, Dictionary<int, string> opcodeMap) {
        string opToTest = opcodeMap[op[0]];
        switch (opToTest) {
            case "addr": { registers[op[3]] = registers[op[1]] + registers[op[2]]; break; }
            case "addi": { registers[op[3]] = registers[op[1]] + op[2]; break; }
            case "mulr": { registers[op[3]] = registers[op[1]] * registers[op[2]]; break; }
            case "muli": { registers[op[3]] = registers[op[1]] * op[2]; break; }
            case "banr": { registers[op[3]] = registers[op[1]] & registers[op[2]]; break; }
            case "bani": { registers[op[3]] = registers[op[1]] & op[2]; break; }
            case "borr": { registers[op[3]] = registers[op[1]] | registers[op[2]]; break; }
            case "bori": { registers[op[3]] = registers[op[1]] | op[2]; break; }
            case "setr": { registers[op[3]] = registers[op[1]]; break; }
            case "seti": { registers[op[3]] = op[1]; break; }
            case "gtir": { registers[op[3]] = op[1] > registers[op[2]] ? 1 : 0; break; }
            case "gtri": { registers[op[3]] = registers[op[1]] > op[2] ? 1 : 0; break; }
            case "gtrr": { registers[op[3]] = registers[op[1]] > registers[op[2]] ? 1 : 0; break; }
            case "eqir": { registers[op[3]] = op[1] == registers[op[2]] ? 1 : 0; break; }
            case "eqri": { registers[op[3]] = registers[op[1]] == op[2] ? 1 : 0; break; }
            case "eqrr": { registers[op[3]] = registers[op[1]] == registers[op[2]] ? 1 : 0; break; }
        }
    }

    static int EqualRegisters(int[] before, int[] after, int[] op, string opToTest) {
        int[] test = new int[4];
        for (int i = 0; i < 4; i++) {
            test[i] = before[i];
        }

        switch (opToTest) {
            case "addr": { test[op[3]] = test[op[1]] + test[op[2]]; break; }
            case "addi": { test[op[3]] = test[op[1]] + op[2]; break; }
            case "mulr": { test[op[3]] = test[op[1]] * test[op[2]]; break; }
            case "muli": { test[op[3]] = test[op[1]] * op[2]; break; }
            case "banr": { test[op[3]] = test[op[1]] & test[op[2]]; break; }
            case "bani": { test[op[3]] = test[op[1]] & op[2]; break; }
            case "borr": { test[op[3]] = test[op[1]] | test[op[2]]; break; }
            case "bori": { test[op[3]] = test[op[1]] | op[2]; break; }
            case "setr": { test[op[3]] = test[op[1]]; break; }
            case "seti": { test[op[3]] = op[1]; break; }
            case "gtir": { test[op[3]] = op[1] > test[op[2]] ? 1 : 0; break; }
            case "gtri": { test[op[3]] = test[op[1]] > op[2] ? 1 : 0; break; }
            case "gtrr": { test[op[3]] = test[op[1]] > test[op[2]] ? 1 : 0; break; }
            case "eqir": { test[op[3]] = op[1] == test[op[2]] ? 1 : 0; break; }
            case "eqri": { test[op[3]] = test[op[1]] == op[2] ? 1 : 0; break; }
            case "eqrr": { test[op[3]] = test[op[1]] == test[op[2]] ? 1 : 0; break; }
        }

        for (int i = 0; i < 4; i++) {
            if (test[i] != after[i]) {
                return 0;
            }
        }
        return 1;
    }
}