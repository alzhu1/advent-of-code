import java.io.BufferedReader;
import java.io.FileReader;
import java.util.IntSummaryStatistics;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

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
        int sum = 0;
        for (String line: input) {
            String[] nums = line.split("\t");
            IntSummaryStatistics stats = Stream.of(nums).map(x -> Integer.parseInt(x)).collect(Collectors.summarizingInt(Integer::intValue));
            sum += stats.getMax() - stats.getMin();
        }
        System.out.println(sum);
    }

    public static void solve2(String[] input) {
        int sum = 0;
        for (String line: input) {
            String[] nums = line.split("\t");
            List<Integer> parsedNums = Stream.of(nums).map(x -> Integer.parseInt(x)).sorted().collect(Collectors.toList());
            for (int i = 0; i < parsedNums.size(); i++) {
                boolean found = false;

                for (int j = i + 1; j < parsedNums.size(); j++) {
                    if (parsedNums.get(j) % parsedNums.get(i) == 0) {
                        found = true;
                        sum += (parsedNums.get(j) / parsedNums.get(i));
                        break;
                    }
                }

                if (found) {
                    break;
                }
            }
        }
        System.out.println(sum);
    }
}