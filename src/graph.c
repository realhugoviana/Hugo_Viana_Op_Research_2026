#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

Graph* create_graph(int num_nodes, int s, int t) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = num_nodes;
    graph->s = s;
    graph->t = t;
    graph->nodes = (Node*)calloc(num_nodes, sizeof(Node));
    return graph;
}

Arc* allocate_arc(Graph* graph, int from, int to, int capacity, int cost, int flow) {
    Arc* arc = (Arc*)malloc(sizeof(Arc));
    arc->node_to = to;
    arc->capacity = capacity;
    arc->cost = cost;
    arc->flow = flow;
    arc->residual = NULL;
    arc->next = graph->nodes[from].arcs;
    graph->nodes[from].arcs = arc;
    return arc;
}

void add_arc(Graph* graph, int from, int to, int capacity, int cost) {
    Arc* forward_arc = allocate_arc(graph, from, to, capacity, cost, 0);
    Arc* backward_arc = allocate_arc(graph, to, from, 0, -cost, 0);
    forward_arc->residual = backward_arc;
    backward_arc->residual = forward_arc;
}

void free_arcs(Arc* arc) {
    while (arc) {
        Arc* next = arc->next;
        free(arc);
        arc = next;
    }
}

void free_graph(Graph* graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        free_arcs(graph->nodes[i].arcs);
    }
    free(graph->nodes);
    free(graph);
}

Graph* read_graph_stdin() {
    int num_nodes, num_arcs, s, t;
    scanf("%d %d %d %d", &num_nodes, &num_arcs, &s, &t);
    Graph* graph = create_graph(num_nodes, s, t);
    for (int i = 0; i < num_arcs; i++) {
        int from, to, capacity, cost;
        scanf("%d %d %d %d", &from, &to, &capacity, &cost);
        add_arc(graph, from, to, capacity, cost);
    }
    return graph;
}