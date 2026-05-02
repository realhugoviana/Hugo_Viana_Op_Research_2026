#ifndef VIZ_H
#define VIZ_H

#include "graph.h"

void dump_dot(Graph* graph, int step, Arc** parent, Arc** cut, int cut_size, const char* output_dir);

#endif