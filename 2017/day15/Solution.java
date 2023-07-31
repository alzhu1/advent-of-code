
public class Solution {
    public static void main(String[] args) {
        long genA = 699;
        long genB = 124;

        long genAFactor = 16807;
        long genBFactor = 48271;
        long divisor = 2147483647;

        int count = 0;

        // Part 1
        // for (int i = 0; i < 40_000_000; i++) {
        //     genA = (genA * genAFactor) % divisor;
        //     genB = (genB * genBFactor) % divisor;

        //     if ((genA & 0xffff) == (genB & 0xffff)) {
        //         count++;
        //     }
        // }

        // Part 2
        for (int i = 0; i < 5_000_000; i++) {
            do {
                genA = (genA * genAFactor) % divisor;
            } while ((genA & 0x3) != 0);

            do {
                genB = (genB * genBFactor) % divisor;
            } while ((genB & 0x7) != 0);

            if ((genA & 0xffff) == (genB & 0xffff)) {
                count++;
            }
        }

        System.out.println(count);
    }
}