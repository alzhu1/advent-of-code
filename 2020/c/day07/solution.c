#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Node Node;
typedef struct Edge Edge;

struct Node {
    char* key;
    Edge** edges;
    int edgeCount;
    int checked;
    int hasShinyGoldBag;
    int totalBags;
};

// One way edge
struct Edge {
    Node* dest;
    int weight;
};

Node* initNode(char* key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = (char*)calloc(strlen(key) + 1, sizeof(char));
    strcpy(node->key, key);

    // Let's just allocate enough for 10 edges (use calloc so we can NULL check)
    node->edges = (Edge**)calloc(10, sizeof(Edge*));
    node->edgeCount = 0;

    node->checked = 0;
    node->hasShinyGoldBag = 0;
    node->totalBags = 0;
    return node;
}

int dfs(Node* n) {
    if (n->checked) {
        return n->hasShinyGoldBag;
    }

    n->checked = 1;

    int hasShinyGoldBag = 0;
    for (int i = 0; i < n->edgeCount; i++) {
        // Recurse, then check for gold bag + bags within
        hasShinyGoldBag |= dfs(n->edges[i]->dest);
        n->totalBags += n->edges[i]->weight * (n->edges[i]->dest->totalBags + 1);
    }
    n->hasShinyGoldBag = hasShinyGoldBag;

    if (strcmp(n->key, "shiny gold bag") == 0) {
        n->hasShinyGoldBag = 1;
    }

    return n->hasShinyGoldBag;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        // Create node list
        Node** nodes = (Node**)calloc(1000, sizeof(Node*));
        int nodeCount = 0;

        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            // Remove period
            line[strlen(line) - 1] = '\0';

            char* test;
            char nodeKey[100];
            memset(nodeKey, '\0', 100);
            do {
                test = strsep(&line, " ");
                strcat(nodeKey, test);
                strcat(nodeKey, " ");
            } while (strcmp(test, "contain"));

            // Remove the plural from "bags" (i.e. set to null)
            nodeKey[strlen(nodeKey) - strlen("s contain ")] = '\0';

            Node* node = NULL;
            int nodeIndex = 0;
            while (nodeIndex < nodeCount) {
                if (strcmp(nodes[nodeIndex]->key, nodeKey) == 0) {
                    node = nodes[nodeIndex];
                    break;
                }

                nodeIndex++;
            }

            if (node == NULL) {
                node = initNode(nodeKey);
                nodes[nodeIndex] = node;
                nodeCount++;
            }

            if (line[0] == 'n') {
                // No edges, so continue
                continue;
            }

            do {
                // Each iteration = parse 1 edge
                char* edgeStr = strsep(&line, ",");
                if (line != NULL) {
                    line++;
                }

                // Remove plural for key consistency
                if (edgeStr[strlen(edgeStr) - 1] == 's') {
                    edgeStr[strlen(edgeStr) - 1] = '\0';
                }

                int weight = strtol(strsep(&edgeStr, " "), NULL, 10);
                Edge* edge = (Edge*)malloc(sizeof(Edge));
                edge->weight = weight;

                Node* destNode = NULL;
                nodeIndex = 0;
                while (nodeIndex < nodeCount) {
                    if (strcmp(nodes[nodeIndex]->key, edgeStr) == 0) {
                        destNode = nodes[nodeIndex];
                        break;
                    }

                    nodeIndex++;
                }
                if (destNode == NULL) {
                    destNode = initNode(edgeStr);
                    nodes[nodeIndex] = destNode;
                    nodeCount++;
                }

                // Assign edge and dest node
                edge->dest = destNode;
                node->edges[node->edgeCount++] = edge;
            } while (line != NULL);
        }

        // Search at each node to find bags
        for (int i = 0; i < nodeCount; i++) {
            dfs(nodes[i]);
        }

        int carryCount = 0;
        for (int i = 0; i < nodeCount; i++) {
            if (strcmp(nodes[i]->key, "shiny gold bag") == 0) {
                printf("Shiny gold bag contains %d bags\n", nodes[i]->totalBags);
                continue;
            }

            if (nodes[i]->hasShinyGoldBag) {
                carryCount++;
            }
        }

        printf("Carry count: %d\n", carryCount);

        // Fix leaks
        for (int i = 0; i < nodeCount; i++) {
            for (int j = 0; j < nodes[i]->edgeCount; j++) {
                free(nodes[i]->edges[j]);
            }
            free(nodes[i]->edges);
            free(nodes[i]->key);
            free(nodes[i]);
        }
        free(nodes);
    }

    free(startBuffer);
}
