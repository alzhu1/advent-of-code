public class Solution {
    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Require numeric arg");
            System.exit(1);
        }

        // solve(Integer.parseInt(args[0]));
        solve2(Integer.parseInt(args[0]));
    }

    public static void solve(int input) {
        int outerLevel = (int)Math.sqrt(input);
        // Check corner
        if (outerLevel % 2 == 1 && outerLevel * outerLevel == input) {
            System.out.println(outerLevel - 1);
            return;
        }

        if (outerLevel % 2 == 0) {
            outerLevel--;
        }

        int leftover = input - (outerLevel * outerLevel);

        outerLevel = (outerLevel / 2) + 1;
        int numLeaps = (leftover - 1) / outerLevel;
        int offset = (leftover - 1) % outerLevel;

        if (numLeaps % 2 == 0) {
            System.out.println((2 * outerLevel - 1) - offset);
        } else {
            System.out.println(outerLevel + 1 + offset);
        }
    }

    // NOTE: This is an awful solution lol, but it works
    public static void solve2(int input) {
        int gridSize = 101;
        int[][] grid = new int[gridSize][gridSize];
        
        int x = gridSize / 2, y = gridSize / 2;
        int offset = 1;
        int offsetChange = offset;
        int mode = 0;

        grid[y][x] = 1;

        while (grid[y][x] < input) {
            switch (mode) {
                case 0:
                    x++;
                    break;
                case 1:
                    y--;
                    break;
                case 2:
                    x--;
                    break;
                case 3:
                    y++;
                    break;
            }
            offsetChange--;

            if (offsetChange == 0) {
                mode = (mode + 1) % 4;

                if (mode % 2 == 0) {
                    offset++;
                }
                offsetChange = offset;
            }

            grid[y][x] = grid[y-1][x-1] + grid[y-1][x] + grid[y-1][x+1] + grid[y][x-1] + grid[y][x+1] + grid[y+1][x-1] + grid[y+1][x] + grid[y+1][x+1];
        
            System.out.println(grid[y][x]);
        }

        System.out.println(grid[y][x]);
        System.out.println(y);
        System.out.println(x);
    }
}