#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
            if (!visited[neighbor_id] && !excluded[neighbor_id]) {
                visited[neighbor_id] = 1;
                push(stack, neighbor_id);
            }
            neighbor = neighbor->next_node;
        }
    }
    freeStack(stack);
}

int main(int argc, char* argv[]) {
    const char* file_name = NULL;
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t buffer_read;
    FILE *file = NULL;
    int action = NULL;

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
        printf("3. Obtener k-conexidad del Grafo.\n");
        printf("4. Obtener Tamaño y Orden del Grafo.\n");
        printf("5. Obtener conexidad.\n");
        printf("6. Ingresar un nuevo grafo.\n");
        printf("Si desea salir del programa, presione 7.\n");
        scanf("%d", &action);

        switch (action) {
            case 1:
                printGraph(graph);
                break;
            case 2:
                // grados
                break;
            case 3:
                // k-conexidad
            case 4:
                // tamaño y orden del grafo
            case 5:
                // es conexo?
            case 6:
                freeGraph(graph);
                graph = NULL;

                printf("Ingrese el nombre del nuevo archivo: ");
                buffer_read = getline(&buffer, &buffer_size, stdin);
                if (buffer_read != -1) {
                    if (buffer[buffer_read - 1] == '\n') {
                        buffer[buffer_read - 1] = '\0';
                    }
                    file_name = buffer;
                }
                file = fopen(file_name, "rb");
                if(file == NULL) {
                    printf("Error: No se logró leer un nombre válido de archivo. El programa se cerrará.");
                    return 0;
                }
                fscanf(file, "%d", &num_vertices);
                fclose(file);

                graph = initializeGraph(num_vertices);
                readGraph(file_name, graph);
                break;

            case 7:
                return 0;
        }
    }

    freeGraph(graph);
    free(buffer);
    return 0;
}