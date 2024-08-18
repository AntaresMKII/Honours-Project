/** 
 * \file map.h
 *
 * Map functionality
 */

#include "vec.h"
#include "../../includes/fds_types.h"

#ifndef MAP_H

#define MAP_H
#define MAP_SIZE 200

typedef Cell** Map;

typedef struct {
    State* fst;
    State* snd;
} Tuple;

Map map_create();

/// Returns adjacent cells of a given point
/** This function returns one or more cells from a point vector. If the
* point falls on the edge between two cells both cells will be returned.
* If the point lies on a vertex of a cell, it will return all four
* adjacent cells.
*/
Cell** map_get_cells(Map *m, Vec3d v, int *num_cells);

/// Returns the neighbor of a specific state
State** map_get_nbrs(Map *m, State *s, int *num_nbrs);

/// Returns the contiguous neighbors of a state s
Tuple* map_get_connbrs(Map *m, State *s, int *num_nbrs);

/// Finds a specific cell given the adjacent states
/** Given three distinct states, this function returns a pointer to a Cell
* adjacent to all three states or NULL if such cell does not exits.
*/
Cell** map_get_cells_from_states(Map *m, State *s1, State *s2, State *s3, int *num_cells);

/// Returns a state given its position as a vector
State* map_get_state(Map *m, Vec3d v);

/// Sets the cost of one or more cells given a vector v and the cost. Sets the number of returned cells into num_cells
Cell** map_set_cells_cost(Map *m, Vec3d v, double cost, int *num_cells);

static inline int states_are_equal(State *s1, State *s2) {
    return s1->v.x == s2->v.x && s1->v.y == s2->v.y;
}

/// Cleanup function
void map_cleanup(Map *m);

void print_obs_to_map(Map *m, int n);

#endif // !MAP_H
