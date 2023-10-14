import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class Solution {
    private static int longest = 0;
    private static int bestScore = 0;

    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line.split("\n"));

            System.out.println("Longest: " + longest);
            System.out.println("Best score: " + bestScore);

            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void solve(String[] input) {
        List<Bridge> bridges = new ArrayList<>();

        for (String line: input) {
            String[] parts = line.split("/");
            bridges.add(new Bridge(Integer.parseInt(parts[0]), Integer.parseInt(parts[1])));
        }

        System.out.println(search(0, bridges, new ArrayList<>()));
    }

    public static int search(int currPort, List<Bridge> remainingBridges, List<Bridge> usedBridges) {
        if (usedBridges.size() >= longest) {
            if (usedBridges.size() > longest) {
                longest = usedBridges.size();
                bestScore = 0;
            }
            bestScore = Math.max(bestScore, usedBridges.stream().reduce(0, (acc, next) -> acc + next.getScore(), Integer::sum));
        }

        if (remainingBridges.isEmpty()) {
            return 0;
        }

        // Loop through remaining bridges
        int maxScore = 0;
        for (Bridge bridge: remainingBridges) {
            if (currPort == bridge.port1 || currPort == bridge.port2) {
                // Use this bridge
                List<Bridge> nextRemaining = new ArrayList<>(remainingBridges);
                List<Bridge> nextUsed = new ArrayList<>(usedBridges);
                nextRemaining.remove(bridge);
                nextUsed.add(bridge);

                int nextPort = currPort == bridge.port1 ? bridge.port2 : bridge.port1;
                maxScore = Math.max(maxScore, bridge.getScore() + search(nextPort, nextRemaining, nextUsed));
            }
        }

        return maxScore;
    }

    private static class Bridge {
        public int port1;
        public int port2;

        public Bridge(int port1, int port2) {
            this.port1 = port1;
            this.port2 = port2;
        }

        public int getScore() {
            return this.port1 + this.port2;
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();
            sb.append(this.port1);
            sb.append("/");
            sb.append(this.port2);
            return sb.toString();
        }

        @Override
        public int hashCode() {
            return this.toString().hashCode();
        }

        @Override
        public boolean equals(Object o) {
            if (o instanceof Bridge) {
                Bridge b = (Bridge)o;
                return this.port1 == b.port1 && this.port2 == b.port2;
            }
            return false;
        }
    }
}