import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;

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
        List<Particle> particles = new ArrayList<>();

        for (String line: input) {
            String[] parts = line.split(", ");
            
            long[] position = Stream.of(parts[0].substring(3, parts[0].length() - 1).split(",")).mapToLong(Long::parseLong).toArray();
            long[] velocity = Stream.of(parts[1].substring(3, parts[1].length() - 1).split(",")).mapToLong(Long::parseLong).toArray();
            long[] acceleration = Stream.of(parts[2].substring(3, parts[2].length() - 1).split(",")).mapToLong(Long::parseLong).toArray();
        
            Particle particle = new Particle();
            particle.setPosition(position);
            particle.setVelocity(velocity);
            particle.setAcceleration(acceleration);

            particles.add(particle);
        }

        long smallest = Long.MAX_VALUE;
        int smallIndex = -1;
        for (int i = 0; i < particles.size(); i++) {
            Particle p = particles.get(i);
            if (p.getDistanceAtTime(100_000_000) < smallest) {
                smallest = p.getDistanceAtTime(100_000_000);
                smallIndex = i;
            }
        }
        System.out.println(smallIndex);

        // Part 2
        while (true) {
            Map<Long, Set<Particle>> timeToParticleCollision = new HashMap<>();

            long smallestTime = Long.MAX_VALUE;
            for (int i = 0; i < particles.size(); i++) {
                Particle p1 = particles.get(i);
                for (int j = i + 1; j < particles.size(); j++) {
                    Particle p2 = particles.get(j);

                    long collideTime = Particle.getCollisionTime(p1, p2);
                    if (collideTime >= 0) {
                        if (!timeToParticleCollision.containsKey(collideTime)) {
                            timeToParticleCollision.put(collideTime, new HashSet<>());
                        }
                        timeToParticleCollision.get(collideTime).add(p1);
                        timeToParticleCollision.get(collideTime).add(p2);

                        smallestTime = Math.min(smallestTime, collideTime);
                    }
                }
            }
            // System.out.println("Smallest time is " + smallestTime);

            Set<Particle> collidedParticles = timeToParticleCollision.getOrDefault(smallestTime, Collections.emptySet());
            if (collidedParticles.isEmpty()) {
                break;
            }

            for (Particle p: collidedParticles) {
                // System.out.println(p);
                particles.remove(p);
            }
        }
        System.out.println(particles.size());
    }

    private static class Particle {
        private long[] position = new long[3];
        private long[] velocity = new long[3];
        private long[] acceleration = new long[3];

        public static long getCollisionTime(Particle p1, Particle p2) {
            // Alternatively, this works but takes longer as t increases
            // for (int t = 0; t < 100; t++) {
            //     long[] pos1 = p1.getPositionAtTime(t);
            //     long[] pos2 = p2.getPositionAtTime(t);

            //     if (pos1[0] == pos2[0] && pos1[1] == pos2[1] && pos1[2] == pos2[2]) {
            //         if (p1.position[0] == 51 || p2.position[0] == 51) {
            //             System.out.println("DEBUG: " + p1 + " " + p2);
            //         }
            //         return t;
            //     }
            // }
            // return -1;

            // Awful quadratic formula check for collisions... but it works
            long[] timesPos = new long[3];
            long[] timesNeg = new long[3];

            for (int i = 0; i < 3; i++) {
                long a = p1.acceleration[i] - p2.acceleration[i];

                long b1 = p1.acceleration[i] + 2 * p1.velocity[i];
                long b2 = p2.acceleration[i] + 2 * p2.velocity[i];
                long b = b1 - b2;

                long c = 2 * (p1.position[i] - p2.position[i]);

                if (a == 0) {
                    // Not quadratic, just linear
                    if (b == 0) {
                        // This means they're parallel
                        if (p1.position[i] != p2.position[i]) {
                            return -1;
                        } else {
                            timesPos[i] = -1;
                            timesNeg[i] = -1;
                            continue;
                        }
                    }
                    if (Math.abs(c) % Math.abs(b) != 0) {
                        return -1;
                    } else {
                        timesPos[i] = -c / b;
                        timesNeg[i] = -c / b;
                        continue;
                    }
                }

                long determinant = b*b - 4*a*c;
                if (determinant < 0) {
                    // This will never collide
                    return -1;
                }

                long sqrtDet = (long)Math.sqrt(determinant);
                if (Math.pow(sqrtDet, 2) != determinant) {
                    return -1;
                }

                long topPos = sqrtDet - b;
                long topNeg = -(sqrtDet + b);

                topPos /= (2 * a);
                topNeg /= (2 * a);

                timesPos[i] = topPos;
                timesNeg[i] = topNeg;
            }

            // Found all potential colliding times, let's check them all
            boolean possibleTimeFound = false;
            long smallTime = Long.MAX_VALUE;
            for (int i = 0; i < 3; i++) {
                // Check time pos
                long[] pos1 = p1.getPositionAtTime(timesPos[i]);
                long[] pos2 = p2.getPositionAtTime(timesPos[i]);

                if (timesPos[i] >= 0 && pos1[0] == pos2[0] && pos1[1] == pos2[1] && pos1[2] == pos2[2]) {
                    possibleTimeFound = true;
                    smallTime = Math.min(smallTime, timesPos[i]);
                }

                // Check time neg
                long[] neg1 = p1.getPositionAtTime(timesNeg[i]);
                long[] neg2 = p2.getPositionAtTime(timesNeg[i]);
                if (timesNeg[i] >= 0 && neg1[0] == neg2[0] && neg1[1] == neg2[1] && neg1[2] == neg2[2]) {
                    possibleTimeFound = true;
                    smallTime = Math.min(smallTime, timesNeg[i]);
                }
            }

            return possibleTimeFound ? smallTime : -1;
        }

        public void setPosition(long[] position) {
            this.position[0] = position[0];
            this.position[1] = position[1];
            this.position[2] = position[2];
        }

        public void setVelocity(long[] velocity) {
            this.velocity[0] = velocity[0];
            this.velocity[1] = velocity[1];
            this.velocity[2] = velocity[2];
        }

        public void setAcceleration(long[] acceleration) {
            this.acceleration[0] = acceleration[0];
            this.acceleration[1] = acceleration[1];
            this.acceleration[2] = acceleration[2];
        }

        public long[] getPositionAtTime(long time) {
            long[] timePosition = new long[3];
            for (int i = 0; i < 3; i++) {
                timePosition[i] = position[i] + (time * velocity[i]) + (((time + 1) * time / 2) * acceleration[i]);
            }
            return timePosition;
        }

        public long getDistanceAtTime(long time) {
            long[] timePosition = getPositionAtTime(time);
            return Math.abs(timePosition[0]) + Math.abs(timePosition[1]) + Math.abs(timePosition[2]);
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();
            sb.append("p=<");
            sb.append(position[0] + "," + position[1] + "," + position[2] + ">");
            sb.append(", v=<");
            sb.append(velocity[0] + "," + velocity[1] + "," + velocity[2] + ">");
            sb.append(", a=<");
            sb.append(acceleration[0] + "," + acceleration[1] + "," + acceleration[2] + ">");
            return sb.toString();
        }
    }
}