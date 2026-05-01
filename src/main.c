#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "ford_fulkerson.h"

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

    int max_flow = ford_fulkerson(graph, dfs);
    printf("Maximum flow: %d\n", max_flow);

    int cut_size;
    Arc** cut = min_cut(graph, &cut_size);
    printf("Min cut edges (size: %d):\n", cut_size);
    for (int i = 0; i < cut_size; i++) {
        Arc* a = cut[i];
        printf("  %d -> %d (capacity: %d, flow: %d)\n", a->residual->node_to, a->node_to, a->capacity, a->flow);
    }

    free_graph(graph);
    return 0;
}