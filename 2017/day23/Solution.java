import java.io.BufferedReader;
import java.io.FileReader;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

public class Solution {
    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line.split("\n"));
            solve2();

            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void solve(String[] input) {
        Map<String, Long> registers = new HashMap<>();

        int pc = 0;
        int mulCount = 0;

        while (pc >= 0 && pc < input.length) {
            String instruction = input[pc];
            String[] parts = instruction.split(" ");

            if (!isNumeric(parts[1]) && !registers.containsKey(parts[1])) {
                registers.put(parts[1], 0L);
            }
            if (parts.length > 2 && !isNumeric(parts[2]) && !registers.containsKey(parts[2])) {
                registers.put(parts[2], 0L);
            }

            switch (parts[0]) {
                case "set": {
                    long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                    registers.put(parts[1], value);
                    break;
                }

                case "sub": {
                    long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                    registers.put(parts[1], registers.get(parts[1]) - value);
                    break;
                }

                case "mul": {
                    long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                    registers.put(parts[1], registers.get(parts[1]) * value);
                    mulCount++;
                    break;
                }

                case "jnz": {
                    long xNumeric = isNumeric(parts[1]) ? Long.parseLong(parts[1]) : registers.get(parts[1]);
                    long yNumeric = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);

                    if (xNumeric != 0) {
                        pc += yNumeric - 1;
                    }
                    break;
                }
            }

            pc++;
        }

        System.out.println(mulCount);
    }

    public static void solve2() {
        // I believe this program is checking if numbers starting from b to c (in 17-increments) are composite
        int start = 108400;
        int end = 125400;

        int compositeCount = 0;
        for (int i = start; i <= end; i += 17) {
            if (!isPrime(i)) {
                compositeCount++;
            }
        }
        System.out.println(compositeCount);
    }

    // https://www.geeksforgeeks.org/java-prime-number-program/#
    public static boolean isPrime(int n) {
        // Corner case
        if (n <= 1)
            return false;

        // Check from 2 to n-1
        for (int i = 2; i < n; i++)
            if (n % i == 0)
                return false;
  
        return true;
    }

    public static boolean isNumeric(String strNum) {
        try {
            Long.parseLong(strNum);
        } catch (NumberFormatException nfe) {
            return false;
        }
        return true;
    }
}