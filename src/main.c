#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

int main() {
    Graph* graph = read_graph_stdin();
    
    printf("Graph read successfully: %d nodes, source = %d, sink = %d\n", graph->num_nodes, graph->s, graph->t);
    for (int i = 0; i < graph->num_nodes; i++) {
        printf("Node %d:\n", i);
        Arc* arc = graph->nodes[i].arcs;
        while (arc) {
            printf("  -> Node %d (capacity: %d, cost: %d)\n", arc->node_to, arc->capacity, arc->cost);
            arc = arc->next;
        }
    }

    free_graph(graph);
    return 0;
}