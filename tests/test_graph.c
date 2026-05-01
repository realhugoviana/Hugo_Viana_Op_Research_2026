#include <assert.h>
#include <stdio.h>

#include "graph.h"

void test_create_graph() {
    Graph* g = create_graph(5, 0, 4);

    assert(g != NULL);
    assert(g->num_nodes == 5);
    assert(g->s == 0);
    assert(g->t == 4);

    free_graph(g);
    printf("test_create_graph OK\n");
}

void test_add_arc() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10, 5);
    Arc* arc = g->nodes[0].arcs;

    assert(arc != NULL);
    assert(arc->node_to == 1);
    assert(arc->capacity == 10);
    assert(arc->cost == 5);
    assert(arc->flow == 0);

    free_graph(g);
    printf("test_add_arc OK\n");
}

void test_residual_link() {
    Graph* g = create_graph(2, 0, 1);
    add_arc(g, 0, 1, 0, 10, 5);
    Arc* forward = g->nodes[0].arcs;
    Arc* backward = g->nodes[1].arcs;

    assert(forward->residual == backward);
    assert(backward->residual == forward);

    free_graph(g);
    printf("test_residual_link OK\n");
}