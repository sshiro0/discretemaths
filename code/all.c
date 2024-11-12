#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct Node {
    int vertice;
    struct Node* next_node;
} Node;

typedef struct Graph {
    int num_vertices;
    Node** adjacency_list;
} Graph;

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

void printGraph(Graph* graph) {
    printf("Vertex:  Adjacency List\n");
    for (int v = 0; v < (*graph).num_vertices; v++) {
        struct Node* temp = (*graph).adjacency_list[v];
        printf("%d --->", v);
        while (temp) {
            printf(" %d ->", (*temp).vertice);
            temp = (*temp).next_node;
        }
        printf(" NULL\n");  
    }
}



int main(int argc, char* argv[]) {
    const char* file_name = NULL;
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t buffer_read;

    if (argc > 1) {
        file_name = argv[1];
    }
    else {
        while (file_name == NULL) {
            printf("Enter a valid file name: ");
            buffer_read = getline(&buffer, &buffer_size, stdin);
            if (buffer_read != -1) {
                if (buffer[buffer_read - 1] == '\n') {
                        buffer[buffer_read - 1] = '\0';
                    }
                    file_name = buffer;
            }
            else {
                printf("Error: unable to read a valid file name. Try again.\n");
                clearerr(stdin);
            }
        }
    }
    
    Graph* graph = initializeGraph(0);
    readGraph(file_name, graph);
    printGraph(graph);

    freeGraph(graph);
    free(buffer);
    return 0;
}


