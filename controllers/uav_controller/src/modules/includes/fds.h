#ifndef FDS_H
#define FDS_H

#include "../../includes/fds_types.h"
#include "../../util/includes/map.h"
#include "../../util/includes/heap.h"

typedef struct {
    Map *m;
    State *start;
    State *end;
    HEAP *OPEN;
} Fds;

char compare_keys(void* k1, void* k2);
void UpdateState(Fds *fds, State *s);
void ComputeShortestPath(Fds *fds);
Fds* fds_init(Vec3d start, Vec3d goal);
void fds_run(Fds* fds, Cell** changed_cells, int num_cells);


#endif // !FDS_H
