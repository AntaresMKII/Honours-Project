/**
 * \file fds.h
 *
 * This is the header file for the field D* algorithm implementation
 */ 
#ifndef FDS_H
#define FDS_H

#include "../../includes/fds_types.h"
#include "../../util/includes/map.h"
#include "../../util/includes/heap.h"

/**
 * This struct is used to store the variables
 * used by field D*.
 */
typedef struct {
    Map *m;         ///< A map of the environment
    State *start;   ///< The start node
    State *end;     ///< The goal node
    HEAP *OPEN;     ///< The min heap
} Fds;

char compare_keys(void* k1, void* k2);  ///< This functions compares two keys together. Returns 1 if k1 is less than k2 and 0 otherwise
void UpdateState(Fds *fds, State *s);   ///< Updates the state passed in as argument
void ComputeShortestPath(Fds *fds);     ///< Finds the shortest path using field D*

Fds* fds_init(Vec3d start, Vec3d goal);                         ///< Initializes the algorithm
void fds_run(Fds* fds, Cell** changed_cells, int num_cells);    ///< Executes the main loop of the algorithm
Vec3d* fds_extract_path(Fds* fds, int *vec_num);                ///< Extracts the path from the map after filed D* was executed


#endif // !FDS_H
