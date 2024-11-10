#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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


    freeGraph(graph);
    free(buffer);
    return 0;
}