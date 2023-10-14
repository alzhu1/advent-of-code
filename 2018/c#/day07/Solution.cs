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
        Dictionary<char, Node> nodes = new Dictionary<char, Node>();

        foreach (string line in input) {
            string[] parts = line.Split(" ");
            char parent = parts[1][0];
            char child = parts[7][0];

            Node parentNode, childNode;
            if (!nodes.ContainsKey(parent)) {
                parentNode = new Node(parent);
                nodes.Add(parent, parentNode);
            } else {
                parentNode = nodes[parent];
            }

            if (!nodes.ContainsKey(child)) {
                childNode = new Node(child);
                nodes.Add(child, childNode);
            } else {
                childNode = nodes[child];
            }

            parentNode.AddChild(childNode);
            childNode.AddParent(parentNode);
        }

        Part1(nodes);
        Part2(nodes);
    }

    static void Part1(Dictionary<char, Node> nodes) {
        List<Node> availableNodes = new List<Node>();
        foreach (Node n in nodes.Values) {
            if (n.GetParents().Count == 0) {
                availableNodes.Add(n);
            }
        }

        HashSet<char> seenChars = new HashSet<char>();
        string sequence = "";
        while (availableNodes.Count > 0) {
            Node currNode = availableNodes[0];
            foreach (Node available in availableNodes) {
                if (available.id < currNode.id) {
                    currNode = available;
                }
            }

            availableNodes.Remove(currNode);
            if (seenChars.Contains(currNode.id)) {
                continue;
            }
            
            sequence += currNode.id;
            seenChars.Add(currNode.id);

            // Only add children that are new and are valid (all its parents have been added)
            availableNodes.AddRange(currNode.GetChildren().Where(x => !seenChars.Contains(x.id) && x.GetParents().All(c => seenChars.Contains(c.id))));
        }

        Console.WriteLine(sequence);
    }

    static void Part2(Dictionary<char, Node> nodes) {
        List<Node> availableNodes = new List<Node>();
        foreach (Node n in nodes.Values) {
            if (n.GetParents().Count == 0) {
                availableNodes.Add(n);
            }
        }

        // Get the smallest character of available nodes, remove it, then add its children
        HashSet<char> seenChars = new HashSet<char>();
        int time = 0;
        int workTime = 60;
        int[] doneByTime = new int[] {
            Int16.MaxValue, Int16.MaxValue, Int16.MaxValue, Int16.MaxValue, Int16.MaxValue
        };
        char[] nodeProcessed = new char[] {
            'z', 'z', 'z', 'z', 'z'
        };
        char a = 'A';
        while (true) {
            // First, sort available nodes by id (get smallest)
            availableNodes.Sort((x, y) => x.id - y.id);

            // Next, put a node in each doneByTime step
            for (int i = 0; i < doneByTime.Length; i++) {
                if (doneByTime[i] == Int16.MaxValue) {
                    if (availableNodes.Count <= 0) {
                        break;
                    }
                    Node workNode = availableNodes[0];
                    availableNodes.RemoveAt(0);

                    doneByTime[i] = time + (workTime + (workNode.id - a)) + 1;
                    nodeProcessed[i] = workNode.id;
                }
            }

            // After adding work items, check if we can break out (cause no more work to do)
            bool doingWork = false;
            for (int i = 0; i < doneByTime.Length; i++) {
                doingWork = doingWork || doneByTime[i] != Int16.MaxValue;
            }

            if (!doingWork) {
                break;
            }

            // Find the smallest done by time, set time to that (so we processed that node)
            int smallestDoneByTime = doneByTime.Min();
            int smallIndex = Array.IndexOf(doneByTime, smallestDoneByTime);

            time = smallestDoneByTime;
            doneByTime[smallIndex] = Int16.MaxValue;
            seenChars.Add(nodeProcessed[smallIndex]);

            // And add that node's valid children to available nodes
            availableNodes.AddRange(nodes[nodeProcessed[smallIndex]].GetChildren().Where(x => !seenChars.Contains(x.id) && x.GetParents().All(c => seenChars.Contains(c.id))));
        }

        Console.WriteLine(time);
    }

    private class Node {
        public char id;
        private List<Node> children;
        private List<Node> parents;

        public Node(char id) {
            this.id = id;
            children = new List<Node>();
            parents = new List<Node>();
        }

        public void AddChild(Node node) {
            this.children.Add(node);
        }

        public void AddParent(Node node) {
            this.parents.Add(node);
        }

        public List<Node> GetChildren() {
            return this.children;
        }

        public List<Node> GetParents() {
            return this.parents;
        }
    }
}