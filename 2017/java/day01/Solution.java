import java.io.BufferedReader;
import java.io.FileReader;
import java.util.stream.Collectors;

public class Solution {
    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line.split("\n"));
            solve2(line.split("\n"));

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void solve(String[] input) {
        String num = input[0];

        int sum = 0;
        for (int i = 0; i < num.length(); i++) {
            int next = (i + 1) % num.length();
            if (num.charAt(i) == num.charAt(next)) {
                sum += Character.getNumericValue(num.charAt(i));
            }
        }

        System.out.println(sum);
    }

    public static void solve2(String[] input) {
        String num = input[0];

        int sum = 0;
        for (int i = 0; i < num.length(); i++) {
            int next = (i + num.length() / 2) % num.length();
            if (num.charAt(i) == num.charAt(next)) {
                sum += Character.getNumericValue(num.charAt(i));
            }
        }

        System.out.println(sum);
    }
}