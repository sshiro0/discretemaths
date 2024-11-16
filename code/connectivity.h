#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include "graph.h"
#include "dfs.h"

int isConnected(Graph* graph, int *excluded);
void combine(Graph* graph, int* excluded, int* combination, int start, int idx, int k, int* min_disconnectivity);
int connectivity(Graph* graph);
void combineK(Graph* graph, int* excluded, int* combination, int start, int idx, int k, int* is_k_connected);
int isKConnected(Graph* graph, int k);

#endif