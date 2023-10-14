import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Stack;
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
        Stack<Character> braceStack = new Stack<>();
        int totalScore = 0;
        int garbageScore = 0;

        int i = 0;
        while (i < input.length()) {
            char c = input.charAt(i);
            if (c == '{') {
                braceStack.push(c);
                i++;
            } else if (c == '}') {
                totalScore += braceStack.size();
                braceStack.pop();
                i++;
            } else if (c == '!') {
                i += 2;
            } else if (c == '<') {
                // Garbage mode, need to move i to the first > with no !
                boolean garbageEnd = false;
                int tempI = i;

                while (!garbageEnd) {
                    tempI = input.indexOf('>', tempI);

                    int temp = tempI - 1;
                    int exCount = 0;
                    while (input.charAt(temp) == '!') {
                        temp--;
                        exCount++;
                    }

                    if (exCount % 2 == 0) {
                        // If even, we can exit loop
                        garbageEnd = true;
                    }
                    tempI++;
                }

                garbageScore += analyzeGarbage(input.substring(i + 1, tempI - 1));

                i = tempI;
            } else {
                i++;
            }
        }

        System.out.println(totalScore);
        System.out.println(garbageScore);
    }

    public static int analyzeGarbage(String garbageString) {
        int score = 0;
        int i = 0;
        while (i < garbageString.length()) {
            char c = garbageString.charAt(i);
            if (c == '!') {
                i += 2;
            } else {
                score++;
                i++;
            }
        }

        return score;
    }
}