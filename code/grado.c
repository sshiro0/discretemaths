#include <stdio.h>
#include <stdlib.h>

//estructura de los vertices
typedef struct {
	int id;
	int *neighbours;
	int neighbour_count;
} node;

void dfs(node *nodes, int num_nodes, int *excluded, int *visited, int node_id){
	visited[node_id] = 1;
	for(int i=0; i<nodes[node_id].neighbour_count;i++){
		int neighbour = nodes[node_id].neighbours[i];
		if(!visited[neighbour] && !excluded[neighbour]){
			dfs(nodes, num_nodes, excluded, visited, neighbour);
		}
	}
}

int is_connected(node *nodes, int num_nodes, int *excluded){
	int visited[num_nodes];
	for(int i=0; i<num_nodes; i++){
		visited[i] = 0;
	}

	int start_node = -1;

	for(int i=0; i<num_nodes; i++){
		if(!excluded[i]){
			start_node=i;
			break;
		}
	}

	if(start_node == -1){
		return 0;
	}

	dfs(nodes, num_nodes, excluded, visited, start_node);

	for(int i=0; i<num_nodes; i++){
		if(visited[i] == 0 && !excluded[i]){ //Si encuentra un vertice que no fue visitado y no fue eliminado, entonces el grafo no es conexo
			return 0;
		}
	}
    return 1; //El grafo es conexo
}


void generate_combinations(node *nodes, int *excluded, int num_nodes, int k, int *min_disconnectivity) {
    int combination[k];
    
    // Función recursiva para generar combinaciones
    void combine(int start, int idx) {
        if (idx == k) {
            // Excluir los nodos de la combinación actual
            for (int i = 0; i < k; i++) {
                excluded[combination[i]] = 1;
            }
            
            // Verificar la conexidad después de excluir los nodos
            if (!is_connected(nodes, num_nodes, excluded)) {
                // Si el grafo no es conexo, actualizar el mínimo grado de desconexión
                if (k < *min_disconnectivity) {
                    *min_disconnectivity = k;
                }
            }
            
            // Restaurar los nodos excluidos
            for (int i = 0; i < k; i++) {
                excluded[combination[i]] = 0;
            }
            return;
        }

        for (int i = start; i < num_nodes; i++) {
            combination[idx] = i;
            combine(i + 1, idx + 1);
        }
    }

    // Generar combinaciones de k nodos
    combine(0, 0);
}

int connectivity_degree(node *nodes, int num_nodes){
	int min_disconnectivity = num_nodes;

	int excluded[num_nodes];
	for(int i=0;i<num_nodes;i++){
		excluded[i]=0;
	}

	if(!is_connected(nodes, num_nodes, excluded)){
		return 0;
	}

        for (int k = 1; k <= num_nodes; k++) {
        // generar combinaciones de k nodos a eliminar y verificar la conexidad
        generate_combinations(nodes, excluded, num_nodes, k, &min_disconnectivity);
    }

	return min_disconnectivity;

}

void init_graph(node *nodes, int num_nodes){
	for(int i=0; i<num_nodes; i++){
		nodes[i].id = i;
		nodes[i].neighbours = NULL;
		nodes[i].neighbour_count = 0;
	}
}

void free_graph(node *nodes, int num_nodes){
	for(int i=0; i<num_nodes; i++){
		free(nodes[i].neighbours);
	}
}

void read_edges(node *nodes, int num_nodes) {
    int in;
    int out;
    printf("Ingrese las aristas del grafo (in out), -1 -1 para terminar:\n");

    while (1) {
        printf("Ingrese los nodos de la arista (in out): ");
        scanf("%d %d", &in, &out);

        if (in == -1 || out == -1){
	    break;  // Terminar si se ingresa -1 -1
	}
        // Verificar que los vertices estén dentro del rango
        if (in >= 0 && in < num_nodes && out >= 0 && out < num_nodes) {
            // Agregar la arista en ambas direcciones
            nodes[in].neighbours = realloc(nodes[in].neighbours, (nodes[in].neighbour_count + 1) * sizeof(int));
            nodes[out].neighbours = realloc(nodes[out].neighbours, (nodes[out].neighbour_count + 1) * sizeof(int));

            nodes[in].neighbours[nodes[in].neighbour_count] = out;
            nodes[out].neighbours[nodes[out].neighbour_count] = in;

            nodes[in].neighbour_count++;
            nodes[out].neighbour_count++;
        } else {
            printf("IDs de vertice inválidos, por favor intente de nuevo.\n");
        }
    }
}

int main(){
        int num_nodes = 0;
	printf("Indique cantidad de vértices: ");
	scanf("%d", &num_nodes);
	node nodes[num_nodes];

	init_graph(nodes, num_nodes);
	read_edges(nodes, num_nodes);

	int degree = connectivity_degree(nodes, num_nodes);
	printf("El grado de conexidad del grafo es: %d\n", degree);

	free_graph(nodes, num_nodes);

	return 0;
}
