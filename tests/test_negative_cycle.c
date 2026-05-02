#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "negative_cycle.h"

// Test graphs

// Graph without negative cycle
// 0 → 1 (cost=2), 1 → 2 (cost=3)
static Graph* make_no_negative_cycle_graph() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10, 2);
    add_arc(g, 1, 2, 0, 10, 3);
    return g;
}

// Graph with negative cycle
// 0 → 1 (cost=1), 1 → 2 (cost=1), 2 → 0 (cost=-5)
// cycle 0→1→2→0 of cost -3
static Graph* make_negative_cycle_graph() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10,  1);
    add_arc(g, 1, 2, 0, 10,  1);
    add_arc(g, 2, 0, 0, 10, -5);
    return g;
}

// Graph with unreachable negative cycle
// 0 → 1 (cost=1)
// 2 → 3 (cost=1), 3 → 2 (cost=-5)  ← isolated negative cycle
static Graph* make_unreachable_negative_cycle_graph() {
    Graph* g = create_graph(4, 0, 1);
    add_arc(g, 0, 1, 0, 10,  1);
    add_arc(g, 2, 3, 0, 10,  1);
    add_arc(g, 3, 2, 0, 10, -5);
    return g;
}

// Graph with positive cycle
// 0 → 1 (cost=1), 1 → 2 (cost=1), 2 → 0 (cost=3)
static Graph* make_positive_cycle_graph() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10, 1);
    add_arc(g, 1, 2, 0, 10, 1);
    add_arc(g, 2, 0, 0, 10, 3);
    return g;
}

// Test detect_negative_cycle

void test_detect_no_negative_cycle() {
    Graph* g = make_no_negative_cycle_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));

    Arc* result = detect_negative_cycle(g, parent);

    assert(result == NULL);

    free(parent);
    free_graph(g);
    printf("test_detect_no_negative_cycle OK\n");
}

void test_detect_negative_cycle_found() {
    Graph* g = make_negative_cycle_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));

    Arc* result = detect_negative_cycle(g, parent);

    assert(result != NULL);

    free(parent);
    free_graph(g);
    printf("test_detect_negative_cycle_found OK\n");
}

void test_detect_positive_cycle() {
    Graph* g = make_positive_cycle_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));

    Arc* result = detect_negative_cycle(g, parent);

    assert(result == NULL);

    free(parent);
    free_graph(g);
    printf("test_detect_positive_cycle OK\n");
}

void test_detect_unreachable_negative_cycle() {
    Graph* g = make_unreachable_negative_cycle_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));

    Arc* result = detect_negative_cycle(g, parent);

    assert(result != NULL);

    free(parent);
    free_graph(g);
    printf("test_detect_unreachable_negative_cycle OK\n");
}

// Test extract_cycle

void test_extract_cycle_size() {
    Graph* g = make_negative_cycle_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));

    Arc* entry = detect_negative_cycle(g, parent);
    assert(entry != NULL);

    int cycle_size;
    Arc** cycle = extract_cycle(entry, parent, g->num_nodes, &cycle_size);

    assert(cycle_size == 3);

    free(cycle);
    free(parent);
    free_graph(g);
    printf("test_extract_cycle_size OK\n");
}

void test_extract_cycle_negative_cost() {
    Graph* g = make_negative_cycle_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));

    Arc* entry = detect_negative_cycle(g, parent);
    assert(entry != NULL);

    int cycle_size;
    Arc** cycle = extract_cycle(entry, parent, g->num_nodes, &cycle_size);

    int total_cost = 0;
    for (int i = 0; i < cycle_size; i++)
        total_cost += cycle[i]->cost;
    assert(total_cost < 0);

    free(cycle);
    free(parent);
    free_graph(g);
    printf("test_extract_cycle_negative_cost OK\n");
}

void test_extract_cycle_valid_arcs() {
    Graph* g = make_negative_cycle_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));

    Arc* entry = detect_negative_cycle(g, parent);
    assert(entry != NULL);

    int cycle_size;
    Arc** cycle = extract_cycle(entry, parent, g->num_nodes, &cycle_size);

    for (int i = 0; i < cycle_size; i++) {
        Arc* current = cycle[i];
        Arc* next = cycle[(i + 1) % cycle_size];
        assert(next->node_to == current->residual->node_to);
    }

    free(cycle);
    free(parent);
    free_graph(g);
    printf("test_extract_cycle_valid_arcs OK\n");
}