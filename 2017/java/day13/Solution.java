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

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void solve(String[] input) {
        // List<Integer> depthsWithWall = new ArrayList<>();
        Map<Integer, Integer> depthToCycle = new HashMap<>();
        int severity = 0;
        for (String line: input) {
            String[] values = line.split(": ");
            int depth = Integer.parseInt(values[0]);
            int range = Integer.parseInt(values[1]);
            int cycle = (range - 1) * 2;

            // depthsWithWall.add(depth);
            depthToCycle.put(depth, cycle);

            if (depth % cycle == 0) {
                severity += depth * range;
            }
        }

        System.out.println("t=0 severity: " + severity);

        int t = 1;
        while (true) {
            boolean found = true;

            for (Map.Entry<Integer, Integer> pair: depthToCycle.entrySet()) {
                if ((t + pair.getKey()) % pair.getValue() == 0) {
                    found = false;
                    break;
                }
            }

            if (found) {
                break;
            }
            t++;
        }

        System.out.println("Success time: " + t);
    }
}