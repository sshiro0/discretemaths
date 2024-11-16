#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

Node* createNode(int v) {
  Node* new_node = (Node*)malloc(sizeof(Node));
  new_node->vertice = v;
  new_node->next_node = NULL;
  return new_node;
}

Graph* initializeGraph(int num_vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));

    if (!graph) {
        perror("Failed to allocate memory for graph.");
        return NULL;
    }

    graph->num_vertices = num_vertices;
    graph->adjacency_lists= (Node**)malloc(num_vertices * sizeof(Node*));

    if (!graph->adjacency_lists) {
        perror("Failed to allocate memory for adjacency list.");
        free(graph);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        graph->adjacency_lists[i] = NULL;
    }

    return graph;
}

void addEdge(Graph* graph, int start, int end) {
    Node* temp = graph->adjacency_lists[start];
    while (temp) {
        if (temp->vertice == end){
            return;
        }
        temp = temp->next_node;
    }

    Node* new_node = createNode(end);
    new_node->next_node = graph->adjacency_lists[start];
    graph->adjacency_lists[start] = new_node;

    new_node = createNode(start);
    new_node->next_node = graph->adjacency_lists[end];
    graph->adjacency_lists[end] = new_node;
}

void readGraph(const char* file_name, Graph* graph) {
    FILE* file = fopen(file_name, "r");

    if (!file) {
        perror("Error opening file.");
        return;
    }

    int num_vertices;
    fscanf(file, "%d", &num_vertices);

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int start;
        char* token = strtok(line, ":");
        if (token) {
            start = atoi(token) - 1;
            token = strtok(NULL, ", ");
            while (token) {
                int end = atoi(token) - 1;
                addEdge(graph, start, end);
                token = strtok(NULL, ", ");
            }
        }
    }

    fclose(file);
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        Node* temp = graph->adjacency_lists[i];
        while (temp) {
            Node* next_node = temp->next_node;
            free(temp);
            temp = next_node;
        }
    }
    free(graph->adjacency_lists);
    free(graph);
}

void printGraph(Graph* graph) {
    printf("Vertex:  Adjacency List\n");
    for (int v = 0; v < graph->num_vertices; v++) {
        Node* temp = graph->adjacency_lists[v];
        printf("%d:", v + 1);
        while (temp) {
            printf(" %d ", temp->vertice + 1);
            temp = temp->next_node;
        }
        printf("\n");
    }
}