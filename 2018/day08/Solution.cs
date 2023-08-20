using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

public class Solution {
    public static void Main(string[] args) {
        string[] input = File.ReadAllLines("input.txt");

        Solve(input);
    }

    static void Solve(string[] input) {
        HashSet<Node> nodes = new HashSet<Node>();
        (int i, Node n) = BuildTree(input[0].Split(" "), 0, nodes);

        while (n.GetParent() != null) {
            n = n.GetParent();
        }

        Console.WriteLine(SumMetadata(n));
        Console.WriteLine(GetNodeValue(n));
    }

    static int SumMetadata(Node n) {
        int sum = n.GetMetadata().Sum();
        foreach (Node child in n.GetChildren()) {
            sum += SumMetadata(child);
        }
        return sum;
    }

    static int GetNodeValue(Node n) {
        if (n.GetChildren().Count == 0) {
            return n.GetMetadata().Sum();
        }

        int value = 0;
        foreach (int index in n.GetMetadata()) {
            int nodeIndex = index - 1;
            if (nodeIndex >= 0 && nodeIndex < n.GetChildren().Count) {
                value += GetNodeValue(n.GetChildren()[nodeIndex]);
            }
        }
        return value;
    }

    static (int, Node) BuildTree(string[] values, int currIndex, HashSet<Node> nodes) {
        if (currIndex >= values.Length) {
            return (values.Length - 1, null);
        }

        int children = Int16.Parse(values[currIndex++]);
        int metadata = Int16.Parse(values[currIndex++]);
        Node node = new Node(currIndex - 2);
        nodes.Add(node);

        // Handle children
        while (children-- > 0) {
            (int i, Node n) = BuildTree(values, currIndex, nodes);
            if (n != null) {
                node.AddChild(n);
                n.SetParent(node);
            }
            currIndex = i;
        }

        // Handle metadata entries
        while (metadata-- > 0) {
            node.AddMetadata(Int16.Parse(values[currIndex++]));
        }
        return (currIndex, node);
    }

    private class Node {
        public int id;
        private List<Node> children;
        private Node parent;

        private List<int> metadata;

        public Node(int id) {
            this.id = id;
            this.children = new List<Node>();
            this.metadata = new List<int>();
        }

        public void AddChild(Node node) {
            this.children.Add(node);
        }

        public void SetParent(Node node) {
            this.parent = node;
        }

        public void AddMetadata(int m) {
            this.metadata.Add(m);
        }

        public List<Node> GetChildren() {
            return this.children;
        }

        public Node GetParent() {
            return this.parent;
        }

        public List<int> GetMetadata() {
            return this.metadata;
        }
    }
}