#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "ford_fulkerson.h"
#include "min_cost.h"

// test graphs

// 0 → 1 → 2 (cost 1 + 2 = 3)
static Graph* make_simple_cost_graph() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10, 1);
    add_arc(g, 1, 2, 0, 10, 2);
    return g;
}

// Two paths:
// 0→1→3 (cost 2)
// 0→2→3 (cost 6)
static Graph* make_choice_graph() {
    Graph* g = create_graph(4, 0, 3);
    add_arc(g, 0, 1, 0, 5, 1);
    add_arc(g, 1, 3, 0, 5, 1);

    add_arc(g, 0, 2, 0, 5, 5);
    add_arc(g, 2, 3, 0, 5, 1);

    return g;
}

// Negative cost allowed (no negative cycle)
// 0→1 (-2), 1→2 (3)
static Graph* make_negative_graph() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10, -2);
    add_arc(g, 1, 2, 0, 10, 3);
    return g;
}

// Test Bellman-Ford

void test_bellman_ford_simple() {
    Graph* g = make_simple_cost_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    int* dist = malloc(g->num_nodes * sizeof(int));

    bool ok = bellman_ford(g, parent, dist);

    assert(ok == true);
    assert(dist[2] == 3);

    free(parent);
    free(dist);
    free_graph(g);

    printf("test_bellman_ford_simple OK\n");
}

void test_bellman_ford_negative() {
    Graph* g = make_negative_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    int* dist = malloc(g->num_nodes * sizeof(int));

    bool ok = bellman_ford(g, parent, dist);

    assert(ok == true);
    assert(dist[2] == 1);

    free(parent);
    free(dist);
    free_graph(g);

    printf("test_bellman_ford_negative OK\n");
}

// Test Dijkstra

void test_dijkstra_simple() {
    Graph* g = make_simple_cost_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    int* dist = malloc(g->num_nodes * sizeof(int));
    int* potentials = calloc(g->num_nodes, sizeof(int));

    bool ok = dijkstra(g, parent, dist, potentials);

    assert(ok == true);
    assert(parent[2] != NULL);

    free(parent);
    free(dist);
    free(potentials);
    free_graph(g);

    printf("test_dijkstra_simple OK\n");
}

//
// =======================
// MIN COST FLOW (BF)
// =======================
//

void test_mcf_bf_simple() {
    Graph* g = make_simple_cost_graph();

    int cost = 0;
    int flow = min_cost_flow_bf(g, &cost);

    assert(flow == 10);
    assert(cost == 30); // 10 * (1+2)

    free_graph(g);
    printf("test_mcf_bf_simple OK\n");
}

void test_mcf_bf_choice() {
    Graph* g = make_choice_graph();

    int cost = 0;
    int flow = min_cost_flow_bf(g, &cost);

    assert(flow == 10);

    // optimal: 5 on each path
    assert(cost == 5 * 2 + 5 * 6);

    free_graph(g);
    printf("test_mcf_bf_choice OK\n");
}

void test_mcf_bf_negative() {
    Graph* g = make_negative_graph();

    int cost = 0;
    int flow = min_cost_flow_bf(g, &cost);

    assert(flow == 10);
    assert(cost == 10 * 1); // -2 + 3 = 1 per unit

    free_graph(g);
    printf("test_mcf_bf_negative OK\n");
}

//
// =======================
// MIN COST FLOW (DIJKSTRA)
// =======================
//

void test_mcf_dijkstra_simple() {
    Graph* g = make_simple_cost_graph();

    int cost = 0;
    int flow = min_cost_flow_dijkstra(g, &cost);

    assert(flow == 10);
    assert(cost == 30);

    free_graph(g);
    printf("test_mcf_dijkstra_simple OK\n");
}

void test_mcf_dijkstra_choice() {
    Graph* g = make_choice_graph();

    int cost = 0;
    int flow = min_cost_flow_dijkstra(g, &cost);

    assert(flow == 10);
    assert(cost == 5 * 2 + 5 * 6);

    free_graph(g);
    printf("test_mcf_dijkstra_choice OK\n");
}