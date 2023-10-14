import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

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
        Map<String, Node> nodeMap = new HashMap<>();
        List<String> items = Stream.of(input).collect(Collectors.toList());
        Node startingNode = null;

        // First preprocess leaves
        int i = 0;
        while (i < items.size()) {
            if (items.get(i).indexOf("->") == -1) {
                String[] leaf = items.get(i).split(" ");
                int weight = Integer.parseInt(leaf[1].substring(1, leaf[1].length() - 1));

                Node leafNode = new Node(leaf[0], weight);
                if (startingNode == null) {
                    startingNode = leafNode;
                }

                nodeMap.put(leaf[0], leafNode);

                items.remove(i);
                i--;
            }
            i++;
        }

        // Now process nodes with children, only if all children are currently defined
        // Basically create tree bottom up
        i = 0;
        while (items.size() > 0) {
            String[] nodeAndChildren = items.get(i).split(" -> ");

            String[] children = nodeAndChildren[1].split(", ");
            boolean canProcess = true;
            for (String child: children) {
                if (!nodeMap.containsKey(child)) {
                    canProcess = false;
                    break;
                }
            }
            if (!canProcess) {
                i = (i + 1) % items.size();
                continue;
            }

            // We can process this node
            String[] nodeInfo = nodeAndChildren[0].split(" ");
            int weight = Integer.parseInt(nodeInfo[1].substring(1, nodeInfo[1].length() - 1));

            Node node = new Node(nodeInfo[0], weight);

            for (String child: children) {
                Node childNode = nodeMap.get(child);
                node.addChild(childNode);
                childNode.setParent(node);
            }

            nodeMap.put(nodeInfo[0], node);

            items.remove(i);
            if (items.size() > 0) {
                i %= items.size();
            }
        }
        
        while (startingNode.parent != null) {
            startingNode = startingNode.parent;
        }
        System.out.println(startingNode.name);

        // Now check weights
        checkWeights(startingNode);
    }

    public static int checkWeights(Node node) {
        // Get weights of all associated children (leaf node is empty)
        List<Node> nodes = new ArrayList<>();
        List<Integer> weights = new ArrayList<>();
        for (Node child: node.children) {
            nodes.add(child);
            weights.add(checkWeights(child));
        }

        // Check for unbalanced weight
        for (int i = 0; i < weights.size() - 1; i++) {
            if (!weights.get(i).equals(weights.get(i + 1))) {
                StringBuilder sb = new StringBuilder();

                // Ugly but it works
                sb.append("Curr node: ");
                sb.append(node.name);
                sb.append(", one weight: ");
                sb.append(nodes.get(i).name);
                sb.append(" (");
                sb.append(nodes.get(i).weight);
                sb.append("): ");
                sb.append(weights.get(i));
                sb.append(", other weight: ");
                sb.append(nodes.get(i + 1).name);
                sb.append(" (");
                sb.append(nodes.get(i + 1).weight);
                sb.append("): ");
                sb.append(weights.get(i + 1));

                System.out.println(sb);
                break;
            }
        }

        // Regardless, return weight
        return node.weight + weights.stream().reduce(0, Integer::sum);
    }

    private static class Node {
        public String name;
        public int weight;
        public List<Node> children;
        public Node parent;

        public Node(String name, int weight) {
            this.name = name;
            this.weight = weight;
            this.children = new ArrayList<>();
        }

        public void addChild(Node child) {
            this.children.add(child);
        }

        public void setParent(Node parent) {
            this.parent = parent;
        }
    }
}