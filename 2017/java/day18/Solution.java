import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.Map;
import java.util.Queue;
import java.util.stream.Collectors;

public class Solution {
    private static int pc1 = 0;
    private static int pc2 = 0;

    private static int send1 = 0;
    private static int send2 = 0;

    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line.split("\n"));
            solve2(line.split("\n"));

            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void solve(String[] input) {
        Map<String, Long> registers = new HashMap<>();

        long lastSoundPlayed = 0;
        int pc = 0;
        boolean played = false;

        while (pc >= 0 && pc < input.length) {
            String instruction = input[pc];
            String[] parts = instruction.split(" ");

            if (!isNumeric(parts[1]) && !registers.containsKey(parts[1])) {
                registers.put(parts[1], 0L);
            }
            if (parts.length > 2 && !isNumeric(parts[2]) && !registers.containsKey(parts[2])) {
                registers.put(parts[2], 0L);
            }

            switch (parts[0]) {
                case "snd": {
                    lastSoundPlayed = registers.get(parts[1]);
                    break;
                }

                case "set": {
                    long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                    registers.put(parts[1], value);
                    break;
                }

                case "add": {
                    long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                    registers.put(parts[1], registers.get(parts[1]) + value);
                    break;
                }

                case "mul": {
                    long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                    registers.put(parts[1], registers.get(parts[1]) * value);
                    break;
                }

                case "mod": {
                    long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                    registers.put(parts[1], registers.get(parts[1]) % value);
                    break;
                }

                case "rcv": {
                    if (registers.get(parts[1]) != 0) {
                        System.out.println("Last played: " + lastSoundPlayed);
                        played = true;
                    }
                    break;
                }

                case "jgz": {
                    long xNumeric = isNumeric(parts[1]) ? Long.parseLong(parts[1]) : registers.get(parts[1]);
                    long yNumeric = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);

                    if (xNumeric > 0) {
                        pc += yNumeric - 1;
                    }
                    break;
                }
            }

            pc++;

            if (played) {
                break;
            }
        }
    }

    public static void solve2(String[] input) {
        Map<String, Long> registers1 = new HashMap<>();
        Map<String, Long> registers2 = new HashMap<>();

        registers1.put("p", 0L);
        registers2.put("p", 1L);

        Queue<Long> queue1 = new ArrayDeque<>();
        Queue<Long> queue2 = new ArrayDeque<>();

        boolean emptyOnce = false;
        while (true) {
            // Program 1 runs
            while (pc1 >= 0 && pc1 < input.length) {
                boolean shouldProceed = handleInstruction(input[pc1], registers1, queue2, queue1, true);

                if (!shouldProceed) {
                    break;
                }
                pc1++;
            }

            // Program 2 runs
            while (pc2 >= 0 && pc2 < input.length) {
                boolean shouldProceed = handleInstruction(input[pc2], registers2, queue1, queue2, false);

                if (!shouldProceed) {
                    break;
                }
                pc2++;
            }

            if (queue1.isEmpty() && queue2.isEmpty()) {
                if (emptyOnce) {
                    System.out.println("Terminate");
                    break;
                }
                emptyOnce = true;
            } else {
                emptyOnce = false;
            }
        }
        System.out.println("UNUSED: " + send1);
        System.out.println(send2);
    }

    public static boolean handleInstruction(String instruction,
                                            Map<String, Long> registers,
                                            Queue<Long> sendQueue,
                                            Queue<Long> receiveQueue,
                                            boolean isProgramOne) {
        String[] parts = instruction.split(" ");

        if (!isNumeric(parts[1]) && !registers.containsKey(parts[1])) {
            registers.put(parts[1], 0L);
        }
        if (parts.length > 2 && !isNumeric(parts[2]) && !registers.containsKey(parts[2])) {
            registers.put(parts[2], 0L);
        }

        switch (parts[0]) {
            case "snd": {
                long value = isNumeric(parts[1]) ? Long.parseLong(parts[1]) : registers.get(parts[1]);
                sendQueue.offer(value);

                if (isProgramOne) {
                    send1++;
                } else {
                    send2++;
                }
                break;
            }

            case "set": {
                long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                registers.put(parts[1], value);
                break;
            }

            case "add": {
                long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                registers.put(parts[1], registers.get(parts[1]) + value);
                break;
            }

            case "mul": {
                long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                registers.put(parts[1], registers.get(parts[1]) * value);
                break;
            }

            case "mod": {
                long value = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);
                registers.put(parts[1], registers.get(parts[1]) % value);
                break;
            }

            case "rcv": {
                if (receiveQueue.isEmpty()) {
                    // Signal that this register set is waiting
                    return false;
                }
                registers.put(parts[1], receiveQueue.poll());
                break;
            }

            case "jgz": {
                long xNumeric = isNumeric(parts[1]) ? Long.parseLong(parts[1]) : registers.get(parts[1]);
                long yNumeric = isNumeric(parts[2]) ? Long.parseLong(parts[2]) : registers.get(parts[2]);

                if (xNumeric > 0) {
                    // pc += yNumeric - 1;
                    if (isProgramOne) {
                        pc1 += yNumeric - 1;
                    } else {
                        pc2 += yNumeric - 1;
                    }
                }
                break;
            }
        }

        return true;
    }

    public static boolean isNumeric(String strNum) {
        try {
            Long.parseLong(strNum);
        } catch (NumberFormatException nfe) {
            return false;
        }
        return true;
    }
}