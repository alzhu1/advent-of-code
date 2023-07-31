import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

public class Solution {
    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
            String line =  reader.lines().collect(Collectors.joining(System.lineSeparator()));

            solve(line.split("\n"));

            reader.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void solve(String[] input) {
        Map<Integer, Node> nodeMap = new HashMap<>();

        for (String line: input) {
            String[] parts = line.split(" <-> ");
            int baseNodeId = Integer.parseInt(parts[0]);

            if (!nodeMap.containsKey(baseNodeId)) {
                nodeMap.put(baseNodeId, new Node(baseNodeId));
            }
            Node baseNode = nodeMap.get(baseNodeId);

            String[] rest = parts[1].split(", ");
            for (String s: rest) {
                int nodeId = Integer.parseInt(s);
                if (!nodeMap.containsKey(nodeId)) {
                    nodeMap.put(nodeId, new Node(nodeId));
                }
                Node node = nodeMap.get(nodeId);

                baseNode.connections.add(node);
                node.connections.add(baseNode);
            }
        }

        Set<Node> traversed = new HashSet<>();

        int groupCount = 0;
        for (int i: nodeMap.keySet()) {
            Node checkNode = nodeMap.get(i);
            if (traversed.contains(checkNode)) {
                continue;
            }

            traversed.addAll(search(checkNode));
            groupCount++;
        }

        System.out.println("Group count: " + groupCount);
    }

    public static Set<Node> search(Node startNode) {
        List<Node> queue = new ArrayList<>();
        Set<Node> found = new HashSet<>();

        queue.add(startNode);

        while (!queue.isEmpty()) {
            Node node = queue.remove(0);
            if (found.contains(node)) {
                continue;
            }

            found.add(node);

            for (Node n: node.getConnections()) {
                queue.add(n);
            }
        }

        // Part 1: call on 0 only
        if (startNode.id == 0) {
            System.out.println("Group with node 0 size: " + found.size());
        }

        return found;
    }

    private static class Node {
        public int id;
        public Set<Node> connections;

        public Node(int id) {
            this.id = id;
            this.connections = new HashSet<>();
        }

        public Set<Node> getConnections() {
            return this.connections;
        }
    }
}