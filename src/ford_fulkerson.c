#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#include "ford_fulkerson.h"
#include "graph.h"
#include "viz.h"


bool dfs(Graph* graph, Arc** parent, bool* visited) {
    int* stack = malloc(graph->num_nodes * sizeof(int));
    int  top = 0;

    stack[top++] = graph->s;
    visited[graph->s] = true;

    while (top > 0) {
        int u = stack[--top];

        if (u == graph->t) {
            free(stack);
            return true;
        }

        for (Arc* a = graph->nodes[u].arcs; a; a = a->next) {
            int v = a->node_to;
            if (!visited[v] && residual_cap(a) > 0) {
                visited[v] = true;
                parent[v] = a;
                stack[top++] = v;
            }
        }
    }

    free(stack);
    return false;
}

int bottleneck(Graph* graph, Arc** parent) {
    int delta = INT_MAX;
    int v = graph->t;

    while (v != graph->s) {
        Arc* a = parent[v];
        delta = delta < residual_cap(a) ? delta : residual_cap(a);
        v = a->residual->node_to;
    }

    return delta;
}

void augment(Graph* graph, Arc** parent, int delta) {
    int v = graph->t;

    while (v != graph->s) {
        Arc* a = parent[v];
        push_flow(a, delta);
        v = a->residual->node_to;
    }
}

Arc** min_cut(Graph* graph, int* cut_size) {
    Arc** cut    = malloc(graph->num_nodes * graph->num_nodes * sizeof(Arc*));
    int   idx    = 0;
    bool* visited = calloc(graph->num_nodes, sizeof(bool));
    Arc** dummy  = calloc(graph->num_nodes, sizeof(Arc*));

    dfs(graph, dummy, visited);
    free(dummy);

    for (int u = 0; u < graph->num_nodes; u++) {
        if (!visited[u]) continue;
        for (Arc* a = graph->nodes[u].arcs; a; a = a->next) {
            if (!visited[a->node_to] && a->capacity > 0 && residual_cap(a) == 0)
                cut[idx++] = a;
        }
    }

    free(visited);
    *cut_size = idx;
    return cut;
}

int ford_fulkerson(Graph* graph, find_path_fn find_path, const char* output_dir) {
    Arc** parent = calloc(graph->num_nodes, sizeof(Arc*));
    bool* visited = calloc(graph->num_nodes, sizeof(bool));
    int max_flow = 0;
    int step = 0;

    dump_dot(graph, step++, NULL, NULL, 0, output_dir);

    while (find_path(graph, parent, visited)) {
        dump_dot(graph, step++, parent, NULL, 0, output_dir);

        int delta = bottleneck(graph, parent);
        augment(graph, parent, delta);
        max_flow += delta;

        dump_dot(graph, step++, NULL, NULL, 0, output_dir);

        memset(parent,  0, graph->num_nodes * sizeof(Arc*));
        memset(visited, 0, graph->num_nodes * sizeof(bool));
    }

    int cut_size;
    Arc** cut = min_cut(graph, &cut_size);
    dump_dot(graph, step, NULL, cut, cut_size, output_dir);
    printf("Min cut capacity : %d\n", max_flow);
    free(cut);

    free(parent);
    free(visited);
    return max_flow;
}