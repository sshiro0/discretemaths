#include <connectivity.h>
#include <graph.h>
#include <dfs.h>
#include <stddef.h>


int isConnected(Graph* graph, int *excluded) {
    int* visited = (int*)calloc(graph->num_vertices, sizeof(int));

    int start_node = -1;

    if (excluded == NULL) {
        for (int i = 0; i < graph->num_vertices; i++) {
            if (graph->adjacency_lists[i] != NULL) { 
                start_node = i;
                break;
            }
        }
    }
    else {
        for (int i = 0; i < graph->num_vertices; i++) {
            if (!excluded[i]) {
                start_node = i;
                break;
            }
        }
    }

    if (start_node == -1) {
        free(visited);
        return 0;
    }

    dfs(graph, excluded, visited, start_node);

    if (excluded == NULL) {
        for (int i = 0; i < graph->num_vertices; i++) {
            if (visited[i] == 0) {
                free(visited);
                return 0;
            }
        }
    }
    else {
        for (int i = 0; i < graph->num_vertices; i++) {
            if(visited[i] == 0 && !excluded[i]){ 
                return 0;
            }
        }
    }

    free(visited);
    return 1;
}

void combine(Graph* graph, int* excluded, int* combination, int start, int idx, int k, int* min_disconnectivity) {
        if (idx == k) {
            for (int i = 0; i < k; i++) {
                excluded[combination[i]] = 1;
            }
            
            if (!isConnected(graph, excluded)) {
                if (k < *min_disconnectivity) {
                    *min_disconnectivity = k;
                }
            }
            
            for (int i = 0; i < k; i++) {
                excluded[combination[i]] = 0;
            }
            return;
        }

        for (int i = start; i < graph->num_vertices; i++) {
            combination[idx] = i;
            combine(graph, excluded, combination, i + 1, idx + 1, k, min_disconnectivity);
        }
}

int connectivity(Graph* graph) {
    int min_disconnectivity = graph->num_vertices;

    int* excluded = (int*)calloc(graph->num_vertices, sizeof(int));

    for (int k = 1; k <= graph->num_vertices; k++) {
        int combination[k];
        combine(graph, excluded, combination, 0, 0, k, min_disconnectivity);
        if (min_disconnectivity < graph->num_vertices) {
            break;
        }
    }

    free(excluded);
    return min_disconnectivity;
}

int isKConnected(Graph* graph, int k) {
    if (k >= graph->num_vertices) {
        return 0;
    }

    int k_ = k-1;
    int* excluded = (int*)calloc(graph->num_vertices, sizeof(int));
    int combination[k_];
    int is_k_connected = 1;

    combineK(graph, excluded, combination, 0, 0, k_, &is_k_connected);

    free(excluded);
    return is_k_connected;
}

void combineK(Graph* graph, int* excluded, int* combination, int start, int idx, int k_, int* is_k_connected) {
    if (*is_k_connected == 0) {
        return;
    }

    if (idx == k_) {
        for (int i = 0; i < k_; i++) {
            excluded[combination[i]] = 1;
        }
        if (k_ < graph->num_vertices-1) {
            if (!isConnected(graph, excluded)) {
                *is_k_connected = 0;
            }
        }
        
        for (int i = 0; i < k_; i++) {
            excluded[combination[i]] = 0;
        }
        return;
    }

    for (int i = start; i < graph->num_vertices; i++) {
        combination[idx] = i;
        combineK(graph, excluded, combination, i + 1, idx + 1, k_, is_k_connected);
    }
}