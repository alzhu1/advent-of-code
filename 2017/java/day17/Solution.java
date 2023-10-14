import java.util.ArrayList;
import java.util.List;

public class Solution {
    public static void main(String[] args) {
        // Part 1
        solve(2018, 2017, 386);

        // Part 2
        solve2(50_000_001, 386);
    }

    public static void solve(int values, int target, int stepSize) {
        List<Integer> buffer = new ArrayList<>();
        buffer.add(0);

        int currPos = 0;
        for (int i = 1; i < values; i++) {
            currPos = (currPos + stepSize) % buffer.size();
            buffer.add(++currPos, i);
        }

        int indexAfter = (buffer.indexOf(target) + 1) % buffer.size();
        System.out.println(buffer.get(indexAfter));
    }

    public static void solve2(int values, int stepSize) {
        int currPos = 0;
        int afterFirst = 0;
        for (int i = 1; i < values; i++) {
            currPos = (currPos + stepSize) % i;

            if (currPos++ == 0) {
                afterFirst = i;
            }
        }

        System.out.println(afterFirst);
    }
}