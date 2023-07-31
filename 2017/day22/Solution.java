import java.io.BufferedReader;
import java.io.FileReader;
import java.util.HashSet;
import java.util.Set;
import java.util.stream.Collectors;

public class Solution {

    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String input = reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(input.split("\n"));
            solve2(input.split("\n"));

            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void solve(String[] input) {
        int startGridSize = input.length;
        int halfSize = startGridSize / 2;

        Set<Point> infectedPoints = new HashSet<>();
        for (int y = 0; y < input.length; y++) {
            for (int x = 0; x < input[y].length(); x++) {
                if (input[y].charAt(x) == '#') {
                    infectedPoints.add(new Point(x - halfSize, halfSize - y));
                }
            }
        }

        Point position = new Point(0, 0);
        Point currDirection = Point.UP;

        int burstCount = 0;
        int infectBursts = 0;
        while (burstCount < 10000) {
            // Set direction, update node
            if (infectedPoints.contains(position)) {
                currDirection = currDirection.getRightDirection();
                infectedPoints.remove(position);
            } else {
                currDirection = currDirection.getLeftDirection();
                infectedPoints.add(position);
                infectBursts++;
            }

            position.x += currDirection.x;
            position.y += currDirection.y;

            burstCount++;
        }
        System.out.println(infectBursts);
    }

    public static void solve2(String[] input) {
        int startGridSize = input.length;
        int halfSize = startGridSize / 2;

        Set<Point> weakenedPoints = new HashSet<>();
        Set<Point> infectedPoints = new HashSet<>();
        Set<Point> flaggedPoints = new HashSet<>();

        for (int y = 0; y < input.length; y++) {
            for (int x = 0; x < input[y].length(); x++) {
                if (input[y].charAt(x) == '#') {
                    infectedPoints.add(new Point(x - halfSize, halfSize - y));
                }
            }
        }

        Point position = new Point(0, 0);
        Point currDirection = Point.UP;

        int burstCount = 0;
        int infectBursts = 0;
        while (burstCount < 10000000) {
            // Set direction, update node
            if (flaggedPoints.contains(position)) {
                currDirection = currDirection.getReverseDirection();
                flaggedPoints.remove(position);
            } else if (infectedPoints.contains(position)) {
                currDirection = currDirection.getRightDirection();
                infectedPoints.remove(position);
                flaggedPoints.add(position);
            } else if (weakenedPoints.contains(position)) {
                weakenedPoints.remove(position);
                infectedPoints.add(position);
                infectBursts++;
            } else {
                currDirection = currDirection.getLeftDirection();
                weakenedPoints.add(position);
            }

            position.x += currDirection.x;
            position.y += currDirection.y;

            burstCount++;
        }
        System.out.println(infectBursts);
    }

    private static class Point {
        public static Point UP = new Point(0, 1);
        public static Point RIGHT = new Point(1, 0);
        public static Point DOWN = new Point(0, -1);
        public static Point LEFT = new Point(-1, 0);

        public int x;
        public int y;

        public Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        public Point getRightDirection() {
            if (UP.equals(this)) {
                return RIGHT;
            } else if (RIGHT.equals(this)) {
                return DOWN;
            } else if (DOWN.equals(this)) {
                return LEFT;
            } else if (LEFT.equals(this)) {
                return UP;
            }
            return null;
        }

        public Point getLeftDirection() {
            if (UP.equals(this)) {
                return LEFT;
            } else if (LEFT.equals(this)) {
                return DOWN;
            } else if (DOWN.equals(this)) {
                return RIGHT;
            } else if (RIGHT.equals(this)) {
                return UP;
            }
            return null;
        }

        public Point getReverseDirection() {
            if (UP.equals(this)) {
                return DOWN;
            } else if (LEFT.equals(this)) {
                return RIGHT;
            } else if (DOWN.equals(this)) {
                return UP;
            } else if (RIGHT.equals(this)) {
                return LEFT;
            }
            return null;
        }

        @Override
        public int hashCode() {
            return this.toString().hashCode();
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();
            sb.append(this.x);
            sb.append(",");
            sb.append(this.y);
            return sb.toString();
        }

        @Override
        public boolean equals(Object o) {
            if (o instanceof Point) {
                Point p = (Point)o;
                return this.x == p.x && this.y == p.y;
            }
            return false;
        }
    }
}