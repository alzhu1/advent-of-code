import java.io.BufferedReader;
import java.io.FileReader;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

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
        List<Integer> offsets = Stream.of(input).map(x -> Integer.parseInt(x)).collect(Collectors.toList());
        int pc = 0;
        int steps = 0;
        while (pc >= 0 && pc < offsets.size()) {
            int prevPc = pc;
            int value = offsets.get(pc);
            pc += value;
            offsets.set(prevPc, isPart2 && value >= 3 ? value - 1 : value + 1);
            steps++;
        }

        System.out.println(steps);
    }
}