import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Solution {
    private static List<Integer> lengths;
    private static int[] nums;

    private static int startPos, skip;

    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            lengths = new ArrayList<>();
            nums = IntStream.rangeClosed(0, 255).toArray();
            // Part 1
            // String[] stringLengths = line.split(",");
            // for (String s: stringLengths) {
            //     lengths.add(Integer.parseInt(s));
            // }
            // hash();
            // System.out.println(nums[0] * nums[1]);

            // Part 2
            for (int i = 0; i < line.length(); i++) {
                lengths.add((int)line.charAt(i));
            }
            lengths.addAll(List.of(17, 31, 73, 47, 23));

            for (int i = 0; i < 64; i++) {
                hash();
            }

            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < nums.length; i += 16) {
                int value = nums[i];
                for (int j = 1; j < 16; j++) {
                    value ^= nums[i + j];
                }
                String hex = Integer.toHexString(value);
                if (hex.length() < 2) {
                    hex = "0" + hex;
                }
                sb.append(hex);
            }
            System.out.println(sb);

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void hash() {        
        for (int len: lengths) {
            List<Integer> temp = new ArrayList<>();

            for (int i = 0; i < len; i++) {
                temp.add(nums[(startPos + i) % nums.length]);
            }
            Collections.reverse(temp);
            for (int i = 0; i < len; i++) {
                nums[(startPos + i) % nums.length] = temp.get(i);
            }

            startPos += len + skip;
            startPos %= nums.length;
            skip++;
        }
    }
}