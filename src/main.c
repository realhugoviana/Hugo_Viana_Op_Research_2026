// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "ford_fulkerson.h"
#include "min_cost.h"
#include "negative_cycle.h"
#include "viz.h"

typedef struct {
    char* algo;
    char* output_dir;
} Options;

static void usage(const char* prog) {
    fprintf(stderr, "Usage: %s --algo <algo> [--output <dir>] < graphe.txt\n", prog);
    fprintf(stderr, "Algos disponibles:\n");
    fprintf(stderr, "  ff             Ford-Fulkerson\n");
    fprintf(stderr, "  mcf-bf         Min Cost Flow Bellman-Ford\n");
    fprintf(stderr, "  mcf-dijkstra   Min Cost Flow Dijkstra\n");
    fprintf(stderr, "  negative-cycle Detection de cycle negatif\n");
    exit(EXIT_FAILURE);
}

static Options parse_args(int argc, char** argv) {
    Options opts = { .algo = NULL, .output_dir = "steps" };

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--algo") == 0) {
            if (i + 1 >= argc) usage(argv[0]);
            opts.algo = argv[++i];
        } else if (strcmp(argv[i], "--output") == 0) {
            if (i + 1 >= argc) usage(argv[0]);
            opts.output_dir = argv[++i];
        } else {
            usage(argv[0]);
        }
    }

    if (!opts.algo) usage(argv[0]);
    return opts;
}

int main(int argc, char** argv) {
    Options opts  = parse_args(argc, argv);
    Graph*  graph = read_graph_stdin();

    // créer le dossier de sortie
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", opts.output_dir);
    system(cmd);

    if (strcmp(opts.algo, "ff") == 0) {
        int flow = ford_fulkerson(graph, dfs, opts.output_dir);
        printf("Max flow : %d\n", flow);

    } else if (strcmp(opts.algo, "mcf-bf") == 0) {
        int total_cost = 0;
        int flow = min_cost_flow_bf(graph, &total_cost, opts.output_dir);
        printf("Max flow : %d\n", flow);
        printf("Min cost : %d\n", total_cost);

    } else if (strcmp(opts.algo, "mcf-dijkstra") == 0) {
        int total_cost = 0;
        int flow = min_cost_flow_dijkstra(graph, &total_cost, opts.output_dir);
        printf("Max flow : %d\n", flow);
        printf("Min cost : %d\n", total_cost);

    } else if (strcmp(opts.algo, "negative-cycle") == 0) {
        Arc** parent = calloc(graph->num_nodes, sizeof(Arc*));
        Arc*  entry  = detect_negative_cycle(graph, parent);

        if (entry) {
            int   cycle_size;
            Arc** cycle = extract_cycle(entry, parent, graph->num_nodes, &cycle_size);
            printf("Cycle negatif detecte :\n");
            int total_cost = 0;
            for (int i = 0; i < cycle_size; i++) {
                printf("  %d -> %d (cost=%d)\n",
                    cycle[i]->residual->node_to, cycle[i]->node_to, cycle[i]->cost);
                total_cost += cycle[i]->cost;
            }
            printf("Cout total : %d\n", total_cost);
            free(cycle);
        } else {
            printf("Pas de cycle negatif\n");
        }
        free(parent);

    } else {
        fprintf(stderr, "Algo inconnu : %s\n", opts.algo);
        usage(argv[0]);
    }

    free_graph(graph);
    return 0;
}