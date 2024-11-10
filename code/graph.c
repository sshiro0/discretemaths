#include <stdio.h>
#include <graph.h>


Graph* initializeGraph(int num_vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));

    if (!graph) {
        perror("Failed to allocate memory for graph.");
        return NULL;
    }

    (*graph).num_vertices = num_vertices;
    (*graph).adjacency_list = (Node**)malloc(num_vertices * sizeof(Node*));

    if (!(*graph).adjacency_list) {
        perror("Failed to allocate memory for adjacency list.");
        free(graph);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        (*graph).adjacency_list[i] = NULL;
    }

    return graph;
}

void readGraph(const char* file_name, Graph* graph) {
    FILE* file = fopen(file_name, "r");

    if (!file) {
        perror("Error opening file.");
        return;
    }

    int num_vertices;
    fscanf(file, "%d", &num_vertices);

    (*graph).num_vertices = num_vertices;
    (*graph).adjacency_list = (Node**)malloc(num_vertices * sizeof(Node*));

    if (!(*graph).adjacency_list) {
        perror("Failed to allocate memory for adjacency list in readGraph.");
        fclose(file);
        return;
    }

    for (int i = 0; i < num_vertices; i++) {
        (*graph).adjacency_list[i] = NULL;
    }

    fclose(file);
}

void addEdge(Graph* graph, int start, int end) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    if (!new_node) {
        perror("Failed to allocate memory for new node.");
        return;
    }

    (*new_node).vertice = end;
    (*new_node).next_node = (*graph).adjacency_list[start];
    (*graph).adjacency_list[start] = new_node;

    Node* new_node_backwards = (Node*)malloc(sizeof(Node));

    if (!new_node_backwards) {
        perror("Failed to allocate memory for new node (backwards).");
        return;
    }

    (*new_node_backwards).vertice = start;
    (*new_node_backwards).next_node = (*graph).adjacency_list[end];
    (*graph).adjacency_list[end] = new_node_backwards;
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < (*graph).num_vertices; i++) {
        Node* temp = (*graph).adjacency_list[i];
        while (temp) {
            Node* next_node = (*temp).next_node;
            free(temp);
            temp = next_node;
        }
    }
    free((*graph).adjacency_list);
    free(graph);
}







