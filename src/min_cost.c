#include <string.h>
#include <limits.h>

#include "min_cost.h"
#include "ford_fulkerson.h"

bool bellman_ford(Graph* graph, Arc** parent, int* dist) {
    int n = graph->num_nodes;

    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX;
    dist[graph->s] = 0;

    for (int i = 0; i < n - 1; i++) {
        bool updated = false;
        for (int u = 0; u < n; u++) {
            if (dist[u] == INT_MAX) continue;
            for (Arc* a = graph->nodes[u].arcs; a; a = a->next) {
                int v = a->node_to;
                if (residual_cap(a) > 0 && dist[u] + a->cost < dist[v]) {
                    dist[v] = dist[u] + a->cost;
                    parent[v] = a;
                    updated = true;
                }
            }
        }
        if (!updated) break;
    }

    return dist[graph->t] != INT_MAX;
}

int min_cost_flow_bf(Graph* graph, int* total_cost) {
    Arc** parent = calloc(graph->num_nodes, sizeof(Arc*));
    int* dist = malloc(graph->num_nodes * sizeof(int));
    int flow = 0;
    *total_cost = 0;

    while (bellman_ford(graph, parent, dist)) {
        int delta = bottleneck(graph, parent);
        augment(graph, parent, delta);
        flow += delta;
        *total_cost += delta * dist[graph->t];

        memset(parent, 0, graph->num_nodes * sizeof(Arc*));
    }

    free(parent);
    free(dist);
    return flow;
}

bool dijkstra(Graph* graph, Arc** parent, int* dist, int* potentials) {
    int n = graph->num_nodes;
    bool* visited = calloc(n, sizeof(bool));

    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX;
    dist[graph->s] = 0;

    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++)
            if (!visited[j] && (u == -1 || dist[j] < dist[u]))
                u = j;

        if (u == -1 || dist[u] == INT_MAX) break;
        visited[u] = true;

        for (Arc* a = graph->nodes[u].arcs; a; a = a->next) {
            int v = a->node_to;
            if (residual_cap(a) <= 0) continue;

            int reduced_cost = a->cost + potentials[u] - potentials[v];

            if (dist[u] + reduced_cost < dist[v]) {
                dist[v] = dist[u] + reduced_cost;
                parent[v] = a;
            }
        }
    }

    for (int i = 0; i < n; i++)
        if (dist[i] != INT_MAX)
            potentials[i] += dist[i];

    free(visited);
    return dist[graph->t] != INT_MAX;
}

int min_cost_flow_dijkstra(Graph* graph, int* total_cost) {
    Arc** parent = calloc(graph->num_nodes, sizeof(Arc*));
    int* dist = malloc(graph->num_nodes * sizeof(int));
    int* potentials = calloc(graph->num_nodes, sizeof(int));
    int flow = 0;
    *total_cost = 0;

    while (dijkstra(graph, parent, dist, potentials)) {
        int delta = bottleneck(graph, parent);
        augment(graph, parent, delta);
        flow += delta;
        *total_cost += delta * potentials[graph->t];

        memset(parent, 0, graph->num_nodes * sizeof(Arc*));
    }

    free(parent);
    free(dist);
    free(potentials);
    return flow;
}