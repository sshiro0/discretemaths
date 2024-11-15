#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

typedef struct Node {
    int vertice;
    struct Node* next_node;
} Node;

typedef struct Graph {
    int num_vertices;
    Node** adjacency_lists;
} Graph;

typedef struct Stack {
    int* items;
    int top;
} Stack;

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
        printf("%d --->", v + 1);
        while (temp) {
            printf(" %d ->", temp->vertice + 1);
            temp = temp->next_node;
        }
        printf(" NULL\n");  
    }
}

Stack* createStack(int max_size) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->items = (int*)malloc(max_size * sizeof(int));
    stack->top = -1;
    return stack;
}

int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, int value) {
    stack->top += 1;
    stack->items[stack->top] = value;
}

int pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        return -1;
    }
    int aux = stack->items[stack->top];
    stack->top -= 1;
    return aux;
}

void freeStack(Stack* stack) {
    free(stack->items);
    free(stack);
}

void dfs(Graph* graph, int* excluded, int* visited, int start_node) {
    Stack* stack = createStack(graph->num_vertices);
    push(stack, start_node);
    visited[start_node] = 1;

    while (!isStackEmpty(stack)) {
        int node_id = pop(stack);
        Node* neighbor = graph->adjacency_lists[node_id];

        while (neighbor) {
            int neighbor_id = neighbor->vertice;
            if (!visited[neighbor_id] && (excluded == NULL || !excluded[neighbor_id])) {
                visited[neighbor_id] = 1;
                push(stack, neighbor_id);
            }
            neighbor = neighbor->next_node;
        }
    }
    freeStack(stack);
}

int isConnected(Graph* graph, int *excluded){
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
    int min_disconnectivity = graph->num_vertices-1;

    int* excluded = (int*)calloc(graph->num_vertices, sizeof(int));

    for (int k = 1; k <= graph->num_vertices; k++) {
        int combination[k];
        combine(graph, excluded, combination, 0, 0, k, &min_disconnectivity);
        if (min_disconnectivity < graph->num_vertices) {
            break;
        }
    }

    free(excluded);
    return min_disconnectivity;
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

int main(int argc, char* argv[]) {
    const char* file_name = NULL;
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t buffer_read;
    FILE *file = NULL;
    int action = 0;

    if(argc > 1) {
        file_name = argv[1];
    }
    while(file == NULL) {
        if (file_name == NULL) {
            printf("Ingrese un nombre válido de archivo: ");
            buffer_read = getline(&buffer, &buffer_size, stdin);
            if (buffer_read != -1) {
                if (buffer[buffer_read - 1] == '\n') {
                    buffer[buffer_read - 1] = '\0';
                }
                file_name = buffer;
            }
            else {
                printf("Error: No se logró leer contenido válido, inténtelo de nuevo.\n");
                clearerr(stdin);
                continue;
            }
        }

        file = fopen(file_name, "rb");
        if (file == NULL) {
            printf("Error: No es un nombre válido de archivo, inténtelo de nuevo.\n");
            file_name = NULL;
        }
    }

    int num_vertices;
    fscanf(file, "%d", &num_vertices);
    fclose(file);
    
    Graph* graph = initializeGraph(num_vertices);
    readGraph(file_name, graph);
    
    while (1) {
        printf("Bienvenid@. Ingrese alguno de los siguientes números para realizar una acción:\n");
        printf("1. Mostrar Grafo ingresado en formato de Lista de Adyacencia.\n");
        printf("2. Obtener grados mínimos y máximos del Grafo.\n");
        printf("3. Obtener k-conexidad del Grafo con un k ingresado.\n");
        printf("4. Obtener Tamaño y Orden del Grafo.\n");
        printf("5. Obtener conexidad.\n");
        printf("6. Obtener conectividad.\n");
        printf("Si desea salir del programa, presione 7.\n");
        scanf("%d", &action);

        system("clear");
        switch (action) {
            case 1:
                printGraph(graph);
                break;
            case 2:
                int min = minDegree(graph);
                int max = maxDegree(graph);
                printf("Grado mínimo del grado: %d\n", min);
                printf("Grado máximo del grado: %d\n", max);
                break;
            case 3:
                if (isConnected(graph, NULL)) {
                    int k;
                    clock_t start, end;
                    start = clock();
                    printf("Ingrese el valor de k para calcular la k-conexidad: ");
                    scanf("%d", &k);
                    if (isKConnected(graph, k)) {
                        printf("El grafo es %d-conexo.\n", k);
                    }
                    else {
                        printf("El grafo no es %d-conexo.\n", k);
                    }
                    end = clock();
                    double time = (double)(end - start) / CLOCKS_PER_SEC;
                    printf("Tiempo ejecución: %f\n", time);
                }
                else {
                    printf("El grafo es no conexo, por lo tanto, no es posible calcular la k-conexidad.\n");
                }
                break;
            case 4:
                printf("Orden del grafo: %d\n", order(graph));
                printf("Tamaño del grafo: %d\n", size(graph));
    
                break;
            case 5:
                clock_t start, end;
                start = clock();
                if (isConnected(graph, NULL)) {
                    printf("El grafo es conexo.\n");
                }
                else {
                    printf("El grafo es no conexo.\n");
                }
                end = clock();
                double time = (double)(end - start) / CLOCKS_PER_SEC;
                printf("Tiempo ejecución: %f\n", time);
                break;
            case 6:
                if (isConnected(graph, NULL)) {
                    clock_t start, end;
                    start = clock();
                    if (graph->num_vertices == 1) {
                        printf("La conectividad del grafo es 0.\n");
                    }
                    else {
                        int connectivity_value = connectivity(graph);
                        printf("La conectividad del grafo es %d.\n", connectivity_value);
                    }
                    end = clock();
                    double time = (double)(end - start) / CLOCKS_PER_SEC;
                    printf("Tiempo ejecución: %f\n", time);
                    }
                else {
                    printf("El grafo es no conexo, por lo tanto, no es posible calcular la conectividad.\n");
                }
                break;
            case 7:
                freeGraph(graph);
                free(buffer);
                return 0;
            default:
                printf("Ingrese una opción válida.\n");
                break;
        }
    }

    freeGraph(graph);
    free(buffer);
    return 0;
}