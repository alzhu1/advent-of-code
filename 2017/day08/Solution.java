import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

public class Solution {
    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line.split("\n"));

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void solve(String[] input) {
        Registers registers = new Registers();

        int largest = 0;
        for (String line: input) {
            String[] parts = line.split(" ");

            // By definition
            String register = parts[0];
            int incAmount = Integer.parseInt(parts[2]);
            if ("dec".equals(parts[1])) {
                incAmount *= -1;
            }

            String registerCheck = parts[4];
            String condition = parts[5];
            int valueCheck = Integer.parseInt(parts[6]);

            boolean passed = false;
            switch (condition) {
                case ">": {
                    passed = registers.getRegisterValue(registerCheck) > valueCheck;
                    break;
                }

                case ">=": {
                    passed = registers.getRegisterValue(registerCheck) >= valueCheck;
                    break;
                }

                case "<": {
                    passed = registers.getRegisterValue(registerCheck) < valueCheck;
                    break;
                }

                case "<=": {
                    passed = registers.getRegisterValue(registerCheck) <= valueCheck;
                    break;
                }

                case "==": {
                    passed = registers.getRegisterValue(registerCheck) == valueCheck;
                    break;
                }

                case "!=": {
                    passed = registers.getRegisterValue(registerCheck) != valueCheck;
                    break;
                }
            }

            if (passed) {
                registers.updateRegister(register, incAmount);
                largest = Math.max(largest, registers.getLargestValue());
            }
        }

        System.out.println(registers.getLargestValue());
        System.out.println(largest);
    }

    private static class Registers {
        private Map<String, Integer> registers;

        public Registers() {
            registers = new HashMap<>();
        }

        public void updateRegister(String register, Integer incAmount) {
            if (!registers.containsKey(register)) {
                registers.put(register, 0);
            }

            registers.put(register, registers.get(register) + incAmount);
        }

        public int getLargestValue() {
            return Collections.max(registers.values());
        }

        public int getRegisterValue(String register) {
            return registers.getOrDefault(register, 0);
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();
            for (Map.Entry<String, Integer> entry: registers.entrySet()) {
                sb.append("(");
                sb.append(entry.getKey());
                sb.append(",");
                sb.append(entry.getValue());
                sb.append("), ");
            }

            return sb.toString();
        }
    }
}