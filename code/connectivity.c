#include <connectivity.h>
#include <graph.h>
#include <dfs.h>

int isConnected(Graph* graph){
    int* visited = (int*)calloc(graph->num_vertices, sizeof(int));
    int* excluded = (int*)calloc(graph->num_vertices, sizeof(int));
    dfs(graph, excluded, visited, 0);

    for (int i = 0; i < graph->num_vertices; i++) {
        if (visited[i] == 0) { // alguna parte del arreglo no es visitada
            free(visited);
            free(excluded);
            return 0; // no conexo
        }
    }
    free(visited);
    free(excluded);
    return 1; // conexo
}



