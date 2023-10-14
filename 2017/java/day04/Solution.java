import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.HashSet;
import java.util.stream.Collectors;

public class Solution {
    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line.split("\n"), false);
            solve(line.split("\n"), true);

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void solve(String[] input, boolean isPart2) {
        HashSet<String> set = new HashSet<>();
        int invalid = 0;

        for (String line: input) {
            set.clear();
            String[] parts = line.split(" ");

            for (String part: parts) {
                String key = part;
                if (isPart2) {
                    char[] chars = part.toCharArray();
                    Arrays.sort(chars);
                    key = new String(chars);
                }

                if (set.contains(key)) {
                    invalid++;
                    break;
                }
                set.add(key);
            }
        }

        System.out.println(input.length - invalid);
    }
}