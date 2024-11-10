#ifndef GRAPH_H
#define GRAPH_H

typedef struct Node {
    int vertice;
    struct Node* next_node;
} Node;

typedef struct Graph {
    int num_vertices;
    Node** adjacency_list
} Graph;

Graph* initializeGraph(int num_vertices);
void readGraph(const char* file_name, Graph* graph);
void addEdge(Graph* graph, int start, int end);
void freeGraph(Graph* graph);


#endif