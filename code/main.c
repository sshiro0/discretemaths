#include <graph.h>
#include <connectivity.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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
        printf("7. Ingresar un nuevo grafo.\n");
        printf("Si desea salir del programa, presione 8.\n");
        scanf("%d", &action);

        switch (action) {
            case 1:
                printGraph(graph);
                break;
            case 2:
                // grados
                break;
            case 3:
                int k;
                printf("Ingrese el valor de k para calcular la k-conexidad: ");
                scanf("%d", &k);
                if (isKConnected(graph, k)) {
                    printf("El grafo es %d-conexo.\n", k);
                }
                else {
                    printf("El grafo no es %d-conexo.\n", k);
                }
                break;
            case 4:
                // tamaño y orden del grafo
                break;
            case 5:
                if (isConnected(graph, NULL)) {
                    printf("El grafo es conexo.\n");
                }
                else {
                    printf("El grafo es no conexo.\n");
                }
                break;
            case 6:
                int connectivity_value = connectivity(graph);
                printf("La conectividad del grafo es %d.\n", connectivity_value);
                break;
            case 7:
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
                    printf("Error: No se logró leer un nombre válido de archivo. El programa se cerrará.\n");
                    return 0;
                }
                fscanf(file, "%d", &num_vertices);
                fclose(file);

                graph = initializeGraph(num_vertices);
                readGraph(file_name, graph);
                break;
            case 8:
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