import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Solution {
    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line.split("\n")[0]);

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void solve(String input) {
        List<Integer> banks = Stream.of(input.split("\t")).map(x -> Integer.parseInt(x)).collect(Collectors.toList());
        Map<String, Integer> seenStates = new HashMap<>();

        seenStates.put(getStringElement(banks), 0);

        int maxIndex = banks.indexOf(Collections.max(banks));
        int cycles = 0;
        int loopSize = 0;

        while (true) {
            int blocks = banks.get(maxIndex);
            int baseAmount = blocks / banks.size();
            banks.set(maxIndex, baseAmount);

            int leftover = blocks % banks.size();

            int startIndex = (maxIndex + 1) % banks.size();
            while (startIndex != maxIndex) {
                int additional = leftover-- > 0 ? 1 : 0;
                banks.set(startIndex, banks.get(startIndex) + baseAmount + additional);
                startIndex = (startIndex + 1) % banks.size();
            }

            String element = getStringElement(banks);

            cycles++;
            if (seenStates.containsKey(element)) {
                loopSize = cycles - seenStates.get(element);
                break;
            }
            seenStates.put(element, cycles);

            maxIndex = banks.indexOf(Collections.max(banks));
        }

        System.out.println(cycles);
        System.out.println(loopSize);
    }

    public static String getStringElement(List<Integer> banks) {
        return banks.stream().map(x -> x.toString()).reduce("", (acc, x) -> acc + "," + x);
    }
}