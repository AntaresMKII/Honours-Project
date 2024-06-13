/** \file map.h
 *
 * Header file for the map
 *
 * Author: Yannick Abouem
 */

#include "vec.h"
#ifndef MAP_H

#include "../../includes/uav.h"
#include "../../modules/includes/fds.h"

#define MAP_H
#define MAP_SIZE 100

typedef Cell** Map;

typedef struct {
    State* fst;
    State* snd;
} Tuple;

Map map_create();

/// Returns adjacent cells of a given point
/** This function returns one or more
* cells from a point vector. If the
* point falls on the edge between two
* cells both cells will be returned.
* If the point lies on a vertex of
* a cell, it will return all four
* adjacent cells
*/
Cell** map_get_cells(Map *m, Vec3d v, int *num_cells);

/// Returns the neighbor of a specific state
State** map_get_nbrs(Map *m, State *s, int *num_nbrs);

/// Returns the contiguous neighbors of a state s
Tuple* map_get_connbrs(Map*m, State *s, int *num_nbrs);

#endif // !MAP_H
