#ifndef GRAPH_H
#define GRAPH_H

typedef struct Node {
    int vertice;
    struct Node* next_node;
} Node;

typedef struct Graph {
    int num_vertices;
    Node** adjacency_lists;
} Graph;

Node* createNode(int v);
Graph* initializeGraph(int num_vertices);
void addEdge(Graph* graph, int start, int end);
void readGraph(const char* file_name, Graph* graph);
void freeGraph(Graph* graph);
void printGraph(Graph* graph);

#endif