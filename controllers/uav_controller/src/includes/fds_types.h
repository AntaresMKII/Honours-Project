/**
 * \file fds_types.h
 *
 * This file contains types used by Field D*
 */

#include "../util/includes/vec.h"

#ifndef FDS_TYPES_H
#define FDS_TYPES_H

/**
 * The state structure is usde to represent a 
 * vertex in Field D* and is used to compute the
 * shortest path
 */
typedef struct State {
    Vec3d v;            ///< The position of the State defined by a vector v
    double rhs;         ///< The one-step lookahead value of the state
    double g;           ///< The g-value of the state. Represents the path cost of this state
    char visited;       ///< Equals one if the state is visited, otherwise it will be zero
    struct State *s1;   ///< Used for path extraction
    struct State *s2;   ///< Used for path extraction
} State;

/**
 * A cell in the map.
 * It has a traversal cost c and four states
 */
typedef struct {
    double c;
    State* s0;
    State* s1;
    State* s2;
    State* s3;
} Cell;

#endif // !FDS_TYPES_H
