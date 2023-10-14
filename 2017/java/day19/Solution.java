import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayDeque;
import java.util.HashSet;
import java.util.List;
import java.util.Queue;
import java.util.Set;
import java.util.stream.Collectors;

public class Solution {

    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String input = reader.lines().collect(Collectors.joining(System.lineSeparator()));

            String[] lines = input.split("\n");
            char[][] pipes = new char[lines.length][lines[0].length()];
            int startIndex = 0;
            for (int row = 0; row < lines.length; row++) {
                for (int col = 0; col < lines[row].length(); col++) {
                    pipes[row][col] = lines[row].charAt(col);
                    if (row == 0 && pipes[row][col] != ' ') {
                        startIndex = col;
                    }
                }
            }

            search(pipes, startIndex);

            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void search(char[][] pipes, int startIndex) {
        Queue<Pair> queue = new ArrayDeque<>();
        queue.offer(new Pair(0, startIndex, new Pair(1, 0), 0));
        Set<Pair> seen = new HashSet<>();

        StringBuilder sb = new StringBuilder();
        while (!queue.isEmpty()) {
            Pair pair = queue.poll();
            if (seen.contains(pair)) {
                continue;
            }
            seen.add(pair);

            int row = pair.row;
            int col = pair.col;
            Pair currDirection = pair.currDirection;
            int steps = pair.steps;

            // Check if it's a letter
            if (Character.isLetter(pipes[row][col])) {
                sb.append(pipes[row][col]);
            }

            // First, check if same direction works
            int checkRow = row + currDirection.row;
            int checkCol = col + currDirection.col;
            if (checkRow >= 0 && checkRow < pipes.length && checkCol >= 0 && checkCol < pipes[checkRow].length && pipes[checkRow][checkCol] != ' ') {
                queue.offer(new Pair(checkRow, checkCol, currDirection, steps + 1));
            } else {
                // If not, attempt left/right directions
                List<Pair> sides = List.of(currDirection.getLeftDirection(), currDirection.getRightDirection());
                for (Pair side: sides) {
                    checkRow = row + side.row;
                    checkCol = col + side.col;

                    if (checkRow >= 0 && checkRow < pipes.length && checkCol >= 0 && checkCol < pipes[checkRow].length && pipes[checkRow][checkCol] != ' ') {
                        queue.offer(new Pair(checkRow, checkCol, side, steps + 1));
                    }
                }
            }

            // Part 2
            if (queue.isEmpty()) {
                System.out.println("Total traversal steps: " + (steps + 1));
            }
        }
        System.out.println(sb);
    }

    private static class Pair {
        public int row;
        public int col;
        public Pair currDirection;
        public int steps;

        public Pair(int row, int col, Pair currDirection, int steps) {
            this.row = row;
            this.col = col;
            this.currDirection = currDirection;
            this.steps = steps;
        }

        public Pair(int row, int col) {
            this(row, col, null, 0);
        }

        public Pair getLeftDirection() {
            if (row == -1 && col == 0) {
                return new Pair(0, -1);
            }
            if (row == 0 && col == -1) {
                return new Pair(1, 0);
            }
            if (row == 1 && col == 0) {
                return new Pair(0, 1);
            }
            if (row == 0 && col == 1) {
                return new Pair(-1, 0);
            }
            return null;
        }

        public Pair getRightDirection() {
            if (row == -1 && col == 0) {
                return new Pair(0, 1);
            }
            if (row == 0 && col == 1) {
                return new Pair(1, 0);
            }
            if (row == 1 && col == 0) {
                return new Pair(0, -1);
            }
            if (row == 0 && col == -1) {
                return new Pair(-1, 0);
            }
            return null;
        }

        @Override
        public boolean equals(Object o) {
            if (o instanceof Pair) {
                Pair p = (Pair)o;
                return this.row == p.row && this.col == p.col;
            }
            return false;
        }
    }
}