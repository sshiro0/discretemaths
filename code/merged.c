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
