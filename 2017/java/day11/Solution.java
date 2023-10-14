import java.io.BufferedReader;
import java.io.FileReader;
import java.util.stream.Collectors;

public class Solution {
    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line);

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void solve(String input) {
        String[] directions = input.split(",");
        int nw = 0, n = 0, ne = 0;

        int max = 0;
        for (String dir: directions) {
            switch (dir) {
                case "nw":
                    nw++;
                    break;
                case "n":
                    n++;
                    break;
                case "ne":
                    ne++;
                    break;
                case "se":
                    nw--;
                    break;
                case "s":
                    n--;
                    break;
                case "sw":
                    ne--;
                    break;
            }

            max = Math.max(max, getDistance(n, ne, nw));
        }

        System.out.println(getDistance(n, ne, nw));
        System.out.println(max);
    }

    // Convert directional movements into the same sign, then add up
    public static int getDistance(int n, int ne, int nw) {
        // n + se = ne
        if (n > 0 && nw < 0) {
            n += nw;
            ne -= nw;
            nw = 0;
        }

        // ne + s = se
        if (ne > 0 && n < 0) {
            ne += n;
            nw += n;
            n = 0;
        }

        // nw + s = sw
        if (nw > 0 && n < 0) {
            nw += n;
            ne += n;
            n = 0;
        }

        // n + sw = nw
        if (n > 0 && ne < 0) {
            n += ne;
            nw -= ne;
            ne = 0;
        }

        // nw + ne = n
        if (nw > 0 && ne > 0) {
            int lower = Math.min(nw, ne);
            nw -= lower;
            ne -= lower;
            n += lower;
        }

        // se + sw = s
        if (nw < 0 && ne < 0) {
            int higher = Math.max(nw, ne);
            ne -= higher;
            nw -= higher;
            n -= higher;
        }

        return Math.abs(n + nw + ne);
    }
}