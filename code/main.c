#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    const char* file_name = NULL;
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t buffer_read;
    FILE *file = NULL;

    if (argc > 1) {
        file_name = argv[1];
    }
    while (file == NULL) {
        if (file_name == NULL) {
            printf("Enter a valid file name: ");
            buffer_read = getline(&buffer, &buffer_size, stdin);
            if (buffer_read != -1) {
                if (buffer[buffer_read - 1] == '\n') {
                    buffer[buffer_read - 1] = '\0';
                }
                file_name = buffer;
            }
            else {
                printf("Error: unable to read a valid content. Try again.\n");
                clearerr(stdin);
                continue;
            }
        }

        file = fopen(file_name, "rb");
        if (file == NULL) {
            printf("Error: unable to read a valid file name. Try again.\n");
            file_name = NULL;
        }
    }

    int num_vertices;
    fscanf(file, "%d", &num_vertices);

    fclose(file);
    
    Graph* graph = initializeGraph(num_vertices);
    readGraph(file_name, graph);
    printGraph(graph);

    freeGraph(graph);
    free(buffer);
    return 0;
}