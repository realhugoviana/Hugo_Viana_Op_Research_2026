#include <stdio.h>

// graph
void test_create_graph();
void test_add_arc();
void test_residual_link();

// ford_fulkerson
void test_dfs_finds_path();
void test_dfs_no_path();
void test_dfs_saturated_arc();
void test_bottleneck_simple();
void test_bottleneck_limited();
void test_augment_updates_flow();
void test_ff_simple();
void test_ff_parallel();
void test_ff_bottleneck();
void test_ff_no_path();
void test_min_cut_equals_max_flow();

int main() {
    test_create_graph();
    test_add_arc();
    test_residual_link();

    test_dfs_finds_path();
    test_dfs_no_path();
    test_dfs_saturated_arc();
    test_bottleneck_simple();
    test_bottleneck_limited();
    test_augment_updates_flow();
    test_ff_simple();
    test_ff_parallel();
    test_ff_bottleneck();
    test_ff_no_path();
    test_min_cut_equals_max_flow();

    printf("ALL TESTS PASSED ✅\n");
    return 0;
}