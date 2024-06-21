#include <assert.h>
#include <stdlib.h>

#include "../../src/modules/includes/fds.h"

int main(int argc, char *argv[]) {
    Map m;
    State* s;
    Vec3d v = {50,50,0};
    int num;

    m = map_create();
    s = map_get_state(&m, v);

    assert(0);

    return EXIT_SUCCESS;
}
