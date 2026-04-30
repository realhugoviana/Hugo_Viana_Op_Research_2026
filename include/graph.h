#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Arc {
    int node_to;
    int capacity;
    int cost;
    int flow;
    struct Arc* residual;
    struct Arc* next;
} Arc;

typedef struct Node {
    Arc* arcs;
} Node;

typedef struct Graph {
    int num_nodes;
    int s,t;
    Node* nodes;
} Graph;

Graph* create_graph(int num_nodes, int s, int t);

Arc* allocate_arc(Graph* graph, int from, int to, int capacity, int cost, int flow);

void add_arc(Graph* graph, int from, int to, int capacity, int cost);

void free_arcs(Arc* arc);

void free_graph(Graph* graph);

Graph* read_graph_stdin();

#endif