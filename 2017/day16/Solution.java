import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Solution {

    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            // solve("s1,x3/4,pe/b");
            solve(line);

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void solve(String input) {
        List<Character> programs = new ArrayList<>();
        for (char c = 'a'; c <= 'p'; c++) {
            programs.add(c);
        }

        Map<String, Integer> seen = new HashMap<>();
        String[] instructions = input.split(",");

        int iters = 1_000_000_000;
        int i;
        int cycle = 0;
        int beginIndex = 0;
        for (i = 0; i < iters; i++) {
            for (String instruction: instructions) {
                char c = instruction.charAt(0);

                if (c == 's') {
                    int iterations = Integer.parseInt(instruction.substring(1));
                    while (iterations > 0) {
                        programs.add(0, programs.remove(programs.size() - 1));
                        iterations--;
                    }
                } else if (c == 'x') {
                    String[] parts = instruction.substring(1).split("/");
                    int indexA = Integer.parseInt(parts[0]);
                    int indexB = Integer.parseInt(parts[1]);
                    char temp = programs.get(indexA);
                    programs.set(indexA, programs.get(indexB));
                    programs.set(indexB, temp);
                } else if (c == 'p') {
                    String[] parts = instruction.substring(1).split("/");
                    int indexA = programs.indexOf(parts[0].charAt(0));
                    int indexB = programs.indexOf(parts[1].charAt(0));
                    char temp = programs.get(indexA);
                    programs.set(indexA, programs.get(indexB));
                    programs.set(indexB, temp);
                }
            }

            StringBuilder sb = new StringBuilder();
            for (char c: programs) {
                sb.append(c);
            }
            String s = sb.toString();

            if (seen.containsKey(s)) {
                cycle = i - seen.get(s) + 1;
                beginIndex = seen.get(s);
                break;
            }
            seen.put(s, i + 1);
        }

        i = (beginIndex + (iters / cycle) * cycle);

        for (; i < iters; i++) {
            for (String instruction: instructions) {
                char c = instruction.charAt(0);

                if (c == 's') {
                    int iterations = Integer.parseInt(instruction.substring(1));
                    while (iterations > 0) {
                        programs.add(0, programs.remove(programs.size() - 1));
                        iterations--;
                    }
                } else if (c == 'x') {
                    String[] parts = instruction.substring(1).split("/");
                    int indexA = Integer.parseInt(parts[0]);
                    int indexB = Integer.parseInt(parts[1]);
                    char temp = programs.get(indexA);
                    programs.set(indexA, programs.get(indexB));
                    programs.set(indexB, temp);
                } else if (c == 'p') {
                    String[] parts = instruction.substring(1).split("/");
                    int indexA = programs.indexOf(parts[0].charAt(0));
                    int indexB = programs.indexOf(parts[1].charAt(0));
                    char temp = programs.get(indexA);
                    programs.set(indexA, programs.get(indexB));
                    programs.set(indexB, temp);
                }
            }
        }

        StringBuilder sb = new StringBuilder();
        for (char c: programs) {
            sb.append(c);
        }
        String s = sb.toString();
        System.out.println(s);
    }
}