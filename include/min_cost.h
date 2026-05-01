#ifndef MIN_COST_FLOW_H
#define MIN_COST_FLOW_H

#include "graph.h"

bool bellman_ford(Graph* graph, Arc** parent, int* dist);

bool dijkstra(Graph* graph, Arc** parent, int* dist, int* potentials);

int min_cost_flow_bf(Graph* graph, int* total_cost);

int min_cost_flow_dijkstra(Graph* graph, int* total_cost);

#endif