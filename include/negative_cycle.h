#ifndef NEGATIVE_CYCLE_H
#define NEGATIVE_CYCLE_H

#include "graph.h"

Arc* detect_negative_cycle(Graph* graph, Arc** parent);

Arc** extract_cycle(Arc* cycle_arc, Arc** parent, int n, int* cycle_size);

#endif