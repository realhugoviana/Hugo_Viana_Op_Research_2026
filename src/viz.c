#include <stdio.h>

#include "viz.h"
#include "graph.h"

void dump_dot(Graph* graph, int step, Arc** parent, Arc** cut, int cut_size, const char* output_dir) {
    if (!output_dir) return;

    char filename[256];
    snprintf(filename, sizeof(filename), "%s/step_%03d.dot", output_dir, step);

    FILE* f = fopen(filename, "w");
    if (!f) return;

    fprintf(f, "digraph {\n");
    fprintf(f, "    rankdir=LR\n");
    fprintf(f, "    node [shape=circle]\n");
    fprintf(f, "    %d [style=filled fillcolor=green label=\"s=%d\"]\n", graph->s, graph->s);
    fprintf(f, "    %d [style=filled fillcolor=red   label=\"t=%d\"]\n", graph->t, graph->t);

    for (int u = 0; u < graph->num_nodes; u++) {
        for (Arc* a = graph->nodes[u].arcs; a; a = a->next) {
            if (a->capacity == 0) continue;

            bool in_path = false;
            bool in_cut  = false;

            if (parent)
                for (int v = 0; v < graph->num_nodes; v++)
                    if (parent[v] == a) { in_path = true; break; }

            if (cut)
                for (int i = 0; i < cut_size; i++)
                    if (cut[i] == a) { in_cut = true; break; }

            const char* color    = in_cut  ? "orange" : in_path ? "red" : "black";
            const char* penwidth = in_cut  ? "3.0"    : in_path ? "2.0" : "1.0";
            const char* style    = in_cut  ? "dashed" : "solid";

            fprintf(f, "    %d -> %d [label=\"%d/%d (cost=%d)\" color=%s penwidth=%s style=%s]\n",
                u, a->node_to,
                a->flow, a->capacity, a->cost,
                color, penwidth, style);
        }
    }

    fprintf(f, "}\n");
    fclose(f);
}