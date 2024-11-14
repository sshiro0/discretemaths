#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

int isConnected(Graph* graph, int *excluded);
void combine(Graph* graph, int* excluded, int* combination, int start, int idx, int k, int* min_disconnectivity);
void generate_combinations(Graph* graph, int* excluded, int k, int* min_disconnectivity);
int connectivity(Graph* graph);
int isKConnected(Graph* graph, int k);
void combineK(Graph* graph, int* excluded, int* combination, int start, int idx, int k, int* is_k_connected);

#endif