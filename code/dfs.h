#ifndef DFS_H
#define DFS_H

typedef struct Stack {
    int* items;
    int top;
} Stack;

Stack* createStack(int max_size);
int isStackEmpty(Stack* stack);
void push(Stack* stack, int value);
int pop(Stack* stack);
void freeStack(Stack* stack);
void dfs(Graph* graph, int* excluded, int* visited, int start_node);

#endif