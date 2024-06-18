#ifndef FDS_H
#define FDS_H

#include "../../util/includes/vec.h"
#include "../../util/includes/map.h"

typedef struct {
    Vec3d v;
    double rhs;
    double g;
    char visited;
} State;

typedef struct {
    double c;
    State* s0;
    State* s1;
    State* s2;
    State* s3;
} Cell;

typedef struct {
    Map *m;
} Fds;

#endif // !FDS_H
