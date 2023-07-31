import java.util.HashSet;
import java.util.Set;

/**
 *  Begin in state A.
    Perform a diagnostic checksum after 12172063 steps.

    In state A:
    If the current value is 0:
        - Write the value 1.
        - Move one slot to the right.
        - Continue with state B.
    If the current value is 1:
        - Write the value 0.
        - Move one slot to the left.
        - Continue with state C.

    In state B:
    If the current value is 0:
        - Write the value 1.
        - Move one slot to the left.
        - Continue with state A.
    If the current value is 1:
        - Write the value 1.
        - Move one slot to the left.
        - Continue with state D.

    In state C:
    If the current value is 0:
        - Write the value 1.
        - Move one slot to the right.
        - Continue with state D.
    If the current value is 1:
        - Write the value 0.
        - Move one slot to the right.
        - Continue with state C.

    In state D:
    If the current value is 0:
        - Write the value 0.
        - Move one slot to the left.
        - Continue with state B.
    If the current value is 1:
        - Write the value 0.
        - Move one slot to the right.
        - Continue with state E.

    In state E:
    If the current value is 0:
        - Write the value 1.
        - Move one slot to the right.
        - Continue with state C.
    If the current value is 1:
        - Write the value 1.
        - Move one slot to the left.
        - Continue with state F.

    In state F:
    If the current value is 0:
        - Write the value 1.
        - Move one slot to the left.
        - Continue with state E.
    If the current value is 1:
        - Write the value 1.
        - Move one slot to the right.
        - Continue with state A.
 */
public class Solution {

    public enum State {
        A, B, C, D, E, F
    }

    public static void main(String[] args) {
        int cursor = 0;
        State state = State.A;
        Set<Integer> ones = new HashSet<>();
        int steps = 0;

        while (steps++ < 12172063) {
            switch (state) {
                case A: {
                    if (!ones.contains(cursor)) {
                        ones.add(cursor);
                        cursor++;
                        state = State.B;
                    } else {
                        ones.remove(cursor);
                        cursor--;
                        state = State.C;
                    }
                    break;
                }

                case B: {
                    if (!ones.contains(cursor)) {
                        ones.add(cursor);
                        cursor--;
                        state = State.A;
                    } else {
                        cursor--;
                        state = State.D;
                    }
                    break;
                }

                case C: {
                    if (!ones.contains(cursor)) {
                        ones.add(cursor);
                        cursor++;
                        state = State.D;
                    } else {
                        ones.remove(cursor);
                        cursor++;
                        state = State.C;
                    }
                    break;
                }

                case D: {
                    if (!ones.contains(cursor)) {
                        cursor--;
                        state = State.B;
                    } else {
                        ones.remove(cursor);
                        cursor++;
                        state = State.E;
                    }
                    break;
                }

                case E: {
                    if (!ones.contains(cursor)) {
                        ones.add(cursor);
                        cursor++;
                        state = State.C;
                    } else {
                        cursor--;
                        state = State.F;
                    }
                    break;
                }

                case F: {
                    if (!ones.contains(cursor)) {
                        ones.add(cursor);
                        cursor--;
                        state = State.E;
                    } else {
                        cursor++;
                        state = State.A;
                    }
                    break;
                }

                default: {
                    break;
                }
            }
        }

        System.out.println(ones.size());
    }
}
