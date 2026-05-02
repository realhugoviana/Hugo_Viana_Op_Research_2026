#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "negative_cycle.h"

Arc* detect_negative_cycle(Graph* graph, Arc** parent) {
    int n = graph->num_nodes;
    int* dist = calloc(n, sizeof(int));
    int last_updated = -1;

    memset(parent, 0, n * sizeof(Arc*));

    for (int i = 0; i < n; i++) {
        last_updated = -1;
        for (int u = 0; u < n; u++) {
            for (Arc* a = graph->nodes[u].arcs; a; a = a->next) {
                int v = a->node_to;
                if (residual_cap(a) > 0 && dist[u] + a->cost < dist[v]) {
                    dist[v] = dist[u] + a->cost;
                    parent[v] = a;
                    last_updated = v;
                }
            }
        }
    }

    free(dist);
    return (last_updated == -1) ? NULL : parent[last_updated];
}

Arc** extract_cycle(Arc* cycle_arc, Arc** parent, int n, int* cycle_size) {
    Arc* a = cycle_arc;
    for (int i = 0; i < n; i++)
        a = parent[a->residual->node_to];

    int start = a->residual->node_to;
    Arc** cycle = malloc(n * sizeof(Arc*));
    int idx = 0;

    do {
        cycle[idx++] = a;
        a = parent[a->residual->node_to];
    } while (a->residual->node_to != start);

    *cycle_size = idx;
    return cycle;
}