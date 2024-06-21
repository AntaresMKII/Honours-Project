/** \file fds_types.h
* 
* Author: Yannick
*/

#include "../util/includes/vec.h"

#ifndef FDS_TYPES_H
#define FDS_TYPES_H

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

#endif // !FDS_TYPES_H
