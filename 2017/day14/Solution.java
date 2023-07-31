import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.IntStream;

public class Solution {
    public static void main(String[] args) {
        String input = "jzgqcdpd";

        int[][] grid = new int[128][128];

        int bits = 0;
        for (int i = 0; i < 128; i++) {
            bits += knotHashBits(grid, input + "-" + i, i);
        }
        System.out.println(bits);

        int regions = 0;
        for (int row = 0; row < grid.length; row++) {
            for (int col = 0; col < grid[row].length; col++) {
                if (grid[row][col] < 0) {
                    bfs(grid, row, col, ++regions);
                }
            }
        }

        System.out.println(regions);
    }

    public static int knotHashBits(int[][] grid, String input, int row) {
        List<Integer> lengths = new ArrayList<>();
        int[] nums = IntStream.rangeClosed(0, 255).toArray();
        int startPos = 0, skip = 0;

        for (int i = 0; i < input.length(); i++) {
            lengths.add((int)input.charAt(i));
        }
        lengths.addAll(List.of(17, 31, 73, 47, 23));

        for (int i = 0; i < 64; i++) {
            startPos = hash(lengths, nums, startPos, skip);
            skip += lengths.size();
        }

        int bits = 0;

        for (int i = 0; i < nums.length; i += 16) {
            int value = nums[i];
            for (int j = 1; j < 16; j++) {
                value ^= nums[i + j];
            }

            // Compute bits
            for (int shift = 7; shift >= 0; shift--) {
                if ((value & (1 << shift)) > 0) {
                    bits++;
                    grid[row][(i / 2) + 7 - shift] = -1;
                }
            }
        }

        return bits;
    }

    public static int hash(List<Integer> lengths, int[] nums, int startPos, int skip) {
        for (int len: lengths) {
            List<Integer> temp = new ArrayList<>();

            for (int i = 0; i < len; i++) {
                temp.add(nums[(startPos + i) % nums.length]);
            }

            Collections.reverse(temp);
            for (int i = 0; i < len; i++) {
                nums[(startPos + i) % nums.length] = temp.get(i);
            }

            startPos += len + skip;
            startPos %= nums.length;
            skip++;
        }

        return startPos;
    }

    public static void bfs(int[][] grid, int startRow, int startCol, int regionNumber) {
        List<Pair> queue = new ArrayList<>();
        queue.add(new Pair(startRow, startCol));

        while (!queue.isEmpty()) {
            Pair pair = queue.remove(0);
            int row = pair.row, col = pair.col;

            if (row < 0 || row >= grid.length || col < 0 || col >= grid[row].length) {
                continue;
            }

            if (grid[row][col] >= 0) {
                continue;
            }

            grid[row][col] = regionNumber;
            queue.add(new Pair(row + 1, col));
            queue.add(new Pair(row, col + 1));
            queue.add(new Pair(row - 1, col));
            queue.add(new Pair(row, col - 1));
        }
    }

    private static class Pair {
        public int row;
        public int col;

        public Pair(int row, int col) {
            this.row = row;
            this.col = col;
        }
    }
}