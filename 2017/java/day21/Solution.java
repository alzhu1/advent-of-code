import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

public class Solution {

    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String input = reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(input.split("\n"));

            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void solve(String[] input) {
        int[][] grid = new int[][] {
            {0, 1, 0},
            {0, 0, 1},
            {1, 1, 1}
        };

        Map<Matrix, Matrix> rules = new HashMap<>();
        for (String line: input) {
            String[] parts = line.split(" => ");
            String[] leftSide = parts[0].split("/");
            String[] rightSide = parts[1].split("/");

            int[][] leftGrid = new int[leftSide.length][leftSide[0].length()];
            for (int i = 0; i < leftGrid.length; i++) {
                for (int j = 0; j < leftGrid[i].length; j++) {
                    leftGrid[i][j] = leftSide[i].charAt(j) == '#' ? 1 : 0;
                }
            }
            int[][] rightGrid = new int[rightSide.length][rightSide[0].length()];
            for (int i = 0; i < rightGrid.length; i++) {
                for (int j = 0; j < rightGrid[i].length; j++) {
                    rightGrid[i][j] = rightSide[i].charAt(j) == '#' ? 1 : 0;
                }
            }

            rules.put(new Matrix(leftGrid), new Matrix(rightGrid));
        }

        int iters = 0;
        // Part 1 uses 5
        while (iters++ < 18) {
            int currSize = grid.length;
            int subSize = (currSize % 2 == 0) ? 2 : 3;
            int[][] nextGrid = new int[(subSize + 1) * currSize / subSize][(subSize + 1) * currSize / subSize];

            for (int i = 0; i < currSize; i += subSize) {
                for (int j = 0; j < currSize; j += subSize) {
                    // i and j represents the top left corner of a subgrid of current grid
                    int[][] currSubgrid = new int[subSize][subSize];
                    for (int r = 0; r < subSize; r++) {
                        for (int c = 0; c < subSize; c++) {
                            currSubgrid[r][c] = grid[i + r][j + c];
                        }
                    }
                    Matrix subMatrix = new Matrix(currSubgrid);
                    Matrix flippedSubMatrix = subMatrix.getFlippedMatrix();

                    // Now check if any permutations of subMatrix matches a rule
                    Matrix target = null;
                    while (target == null) {
                        if (rules.containsKey(subMatrix)) {
                            target = rules.get(subMatrix);
                        } else if (rules.containsKey(flippedSubMatrix)) {
                            target = rules.get(flippedSubMatrix);
                        }

                        subMatrix = subMatrix.getRotatedMatrix();
                        flippedSubMatrix = flippedSubMatrix.getRotatedMatrix();
                    }

                    // We have the target, need to fill in the next grid
                    int startRow = (subSize + 1) * i / subSize;
                    int startCol = (subSize + 1) * j / subSize;

                    for (int sr = startRow; sr < startRow + subSize + 1; sr++) {
                        for (int sc = startCol; sc < startCol + subSize + 1; sc++) {
                            nextGrid[sr][sc] = target.grid[sr - startRow][sc - startCol];
                        }
                    }
                }
            }

            grid = nextGrid;
        }

        int sum = 0;
        for (int[] row: grid) {
            for (int value: row) {
                sum += value;
            }
        }
        System.out.println(sum);
    }

    // 2x2 or 3x3
    private static class Matrix {
        public int[][] grid;

        public Matrix(int[][] grid) {
            this.grid = grid;
        }

        public Matrix getFlippedMatrix() {
            // Flip across vertically
            int[][] matrix = new int[grid.length][grid[0].length];

            for (int i = 0; i < grid.length; i++) {
                for (int j = 0; j < grid[i].length; j++) {
                    matrix[i][j] = grid[i][grid[i].length - 1 - j];
                }
            }
            return new Matrix(matrix);
        }

        public Matrix getRotatedMatrix() {
            // First transpose
            int[][] matrix = new int[grid.length][grid[0].length];
            for (int i = 0; i < grid.length; i++) {
                int[] row = grid[i];
                for (int j = 0; j < row.length; j++) {
                    matrix[j][i] = row[j];
                }
            }

            // Then flip (simulates clockwise turn)
            return new Matrix(matrix).getFlippedMatrix();
        }

        @Override
        public int hashCode() {
            return Arrays.deepHashCode(grid);
        }

        @Override
        public boolean equals(Object o) {
            if (!(o instanceof Matrix)) {
                return false;
            }

            Matrix m = (Matrix)o;

            if (this.grid.length != m.grid.length || this.grid[0].length != m.grid[0].length) {
                return false;
            }

            for (int i = 0; i < this.grid.length; i++) {
                for (int j = 0; j < this.grid[i].length; j++) {
                    if (this.grid[i][j] != m.grid[i][j]) {
                        return false;
                    }
                }
            }
            return true;
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();
            for (int[] row: grid) {
                for(int value: row) {
                    sb.append(value + " ");
                }
                sb.append("\n");
            }
            return sb.toString();
        }
    }
}