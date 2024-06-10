#ifndef FDS_H
#define FDS_H

#include "../../util/includes/vec.h"

typedef struct {
    Vec3d v;
    double rhs;
    double g;
    char visited;
} State;

typedef struct {
    char c;
    State* s0;
    State* s1;
    State* s2;
    State* s3;
} Cell;

#endif // !FDS_H
