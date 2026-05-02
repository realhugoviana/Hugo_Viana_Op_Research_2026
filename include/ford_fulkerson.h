#ifndef FORD_FULKERSON_H
#define FORD_FULKERSON_H

#include "graph.h"

typedef bool (*find_path_fn)(Graph* graph, Arc** parent, bool* visited);

bool dfs(Graph* graph, Arc** parent, bool* visited);

int bottleneck(Graph* graph, Arc** parent);

void augment(Graph* graph, Arc** parent, int delta);

int ford_fulkerson(Graph* graph, find_path_fn find_path, const char* output_dir);

Arc** min_cut(Graph* graph, int* cut_size);

#endif