#include <graph.h>
#include <degree.h>
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