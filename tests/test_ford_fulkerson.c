#include <assert.h>
#include <stdio.h>

#include "graph.h"
#include "ford_fulkerson.h"

// test graphs

// simple graph: 0 → 1 → 2, cap=10, s=0, t=2
static Graph* make_simple_graph() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10, 0);
    add_arc(g, 1, 2, 0, 10, 0);
    return g;
}

// Graph with two parallel paths
//   0 → 1 → 3  (cap 10)
//   0 → 2 → 3  (cap 5)
static Graph* make_parallel_graph() {
    Graph* g = create_graph(4, 0, 3);
    add_arc(g, 0, 1, 0, 10, 0);
    add_arc(g, 1, 3, 0, 10, 0);
    add_arc(g, 0, 2, 0, 5, 0);
    add_arc(g, 2, 3, 0, 5, 0);
    return g;
}

// Graph with bottleneck
//   0 → 1 (cap 10)
//   1 → 2 (cap 3)   ← bottleneck
//   2 → 3 (cap 10)
static Graph* make_bottleneck_graph() {
    Graph* g = create_graph(4, 0, 3);
    add_arc(g, 0, 1, 0, 10, 0);
    add_arc(g, 1, 2, 0, 3, 0);
    add_arc(g, 2, 3, 0, 10, 0);
    return g;
}

// Test DFS

void test_dfs_finds_path() {
    Graph* g = make_simple_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    bool* visited = calloc(g->num_nodes, sizeof(bool));

    bool found = dfs(g, parent, visited);

    assert(found == true);
    assert(parent[2] != NULL);
    assert(parent[1] != NULL);

    free(parent);
    free(visited);
    free_graph(g);
    printf("test_dfs_finds_path OK\n");
}

void test_dfs_no_path() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10, 0);
    
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    bool* visited = calloc(g->num_nodes, sizeof(bool));

    bool found = dfs(g, parent, visited);

    assert(found == false);

    free(parent);
    free(visited);
    free_graph(g);
    printf("test_dfs_no_path OK\n");
}

void test_dfs_saturated_arc() {
    Graph* g = make_simple_graph();
    
    g->nodes[0].arcs->flow = 10;

    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    bool* visited = calloc(g->num_nodes, sizeof(bool));

    bool found = dfs(g, parent, visited);

    assert(found == false);

    free(parent);
    free(visited);
    free_graph(g);
    printf("test_dfs_saturated_arc OK\n");
}

// Test Bottleneck

void test_bottleneck_simple() {
    Graph* g = make_simple_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    bool* visited = calloc(g->num_nodes, sizeof(bool));

    dfs(g, parent, visited);
    int delta = bottleneck(g, parent);

    assert(delta == 10);

    free(parent);
    free(visited);
    free_graph(g);
    printf("test_bottleneck_simple OK\n");
}

void test_bottleneck_limited() {
    Graph* g = make_bottleneck_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    bool* visited = calloc(g->num_nodes, sizeof(bool));

    dfs(g, parent, visited);
    int delta = bottleneck(g, parent);

    assert(delta == 3);

    free(parent);
    free(visited);
    free_graph(g);
    printf("test_bottleneck_limited OK\n");
}

// Test Augment

void test_augment_updates_flow() {
    Graph* g = make_simple_graph();
    Arc** parent = calloc(g->num_nodes, sizeof(Arc*));
    bool* visited = calloc(g->num_nodes, sizeof(bool));

    dfs(g, parent, visited);
    int delta = bottleneck(g, parent);
    augment(g, parent, delta);

    Arc* fwd = g->nodes[0].arcs;
    assert(fwd->flow == 10);
    assert(fwd->residual->flow == -10);

    free(parent);
    free(visited);
    free_graph(g);
    printf("test_augment_updates_flow OK\n");
}

// Test Ford-Fulkerson

void test_ff_simple() {
    Graph* g = make_simple_graph();
    int flow = ford_fulkerson(g, dfs);
    assert(flow == 10);
    free_graph(g);
    printf("test_ff_simple OK\n");
}

void test_ff_parallel() {
    Graph* g = make_parallel_graph();
    int flow = ford_fulkerson(g, dfs);
    assert(flow == 15);
    free_graph(g);
    printf("test_ff_parallel OK\n");
}

void test_ff_bottleneck() {
    Graph* g = make_bottleneck_graph();
    int flow = ford_fulkerson(g, dfs);
    assert(flow == 3);
    free_graph(g);
    printf("test_ff_bottleneck OK\n");
}

void test_ff_no_path() {
    Graph* g = create_graph(3, 0, 2);
    add_arc(g, 0, 1, 0, 10, 0);
    int flow = ford_fulkerson(g, dfs);
    assert(flow == 0);
    free_graph(g);
    printf("test_ff_no_path OK\n");
}

// Test Min Cut

void test_min_cut_equals_max_flow() {
    Graph* g = make_parallel_graph();
    int max_flow = ford_fulkerson(g, dfs);
    int cut_size = 0;
    Arc** cut = min_cut(g, &cut_size);

    int cut_capacity = 0;
    for (int i = 0; i < cut_size; i++)
        cut_capacity += cut[i]->capacity;

    assert(cut_capacity == max_flow);

    free(cut);
    free_graph(g);
    printf("test_min_cut_equals_max_flow OK\n");
}