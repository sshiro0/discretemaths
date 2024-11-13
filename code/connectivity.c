#include <connectivity.h>
#include <graph.h>
#include <dfs.h>
#include <stddef.h>

int isConnected(Graph* graph){
    int* visited = (int*)calloc(graph->num_vertices, sizeof(int));

    int start_node = -1;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adjacency_lists[i] != NULL) { 
            start_node = i;
            break;
        }
    }

    if (start_node == -1) {
        free(visited);
        return 0;
    }

    dfs(graph, NULL, visited, start_node);

    for (int i = 0; i < graph->num_vertices; i++) {
        if (visited[i] == 0) {
            free(visited);
            return 0;
        }
    }

    free(visited);
    return 1;
}



