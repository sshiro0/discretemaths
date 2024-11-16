#include "graph.h"
#include "degree.h"
#include <limits.h>

int order(Graph* graph) {
    return graph->num_vertices;
}

int size(Graph* graph) {
    int edge_count = 0;
    for (int i = 0; i < graph->num_vertices; i++) {
        Node* temp = graph->adjacency_lists[i];
        while (temp) {
            edge_count++;
            temp = temp->next_node;
        }
    }
    return edge_count / 2;
}

int maxDegree(Graph* graph) {
    int max_deg = 0;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        int degree = 0;
        Node* temp = graph->adjacency_lists[i];
        
        while (temp) {
            degree++;
            temp = temp->next_node;
        }
        
        if (degree > max_deg) {
            max_deg = degree;
        }
    }
    
    return max_deg;
}

int minDegree(Graph* graph) {
    int min_deg = INT_MAX;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        int degree = 0;
        Node* temp = graph->adjacency_lists[i];
        
        while (temp) {
            degree++;
            temp = temp->next_node;
        }
        
        if (degree < min_deg) {
            min_deg = degree;
        }
    }
    
    if (min_deg == INT_MAX) {
        return 0;
    }
    else {
        return min_deg;
    }
}