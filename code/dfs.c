#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"
#include "graph.h"

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
