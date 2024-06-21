#include <assert.h>
#include <stdlib.h>

#include "../../src/modules/includes/fds.h"

int main(int argc, char *argv[]) {
    Fds *fds;
    Vec3d start = {0, 0, 0};
    Vec3d end = {10, 10, 0};
    Vec3d v = {1.5, 1.5, 0};
    Cell** c;

    int num_cells;

    fds = fds_init(start, end);

    c = map_get_cells(fds->m, v, &num_cells);

    assert(num_cells == 1);

    c[0]->c = 1.0f;

    ComputeShortestPath(fds);

    return EXIT_SUCCESS;
}
