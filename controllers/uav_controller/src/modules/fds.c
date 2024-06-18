#include <cmath>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "includes/fds.h"
#include "../util/includes/heap.h"

#define min(a,b) a < b ? a : b

inline double euclid_heuristic(State s1, State s2) {
    return sqrt(pow(fabs(s1.v.x - s2.v.x), 2) + pow(fabs(s1.v.y - s2.v.y), 2));
}

double* key(State s, State s_start) {
    double *k = (double*) malloc(2*sizeof(double));
    if (k == NULL) {
        printf("Failed to allocate memory for key!\n");
        exit(EXIT_FAILURE);
    }

    k[0] = min(s.g, s.rhs) + euclid_heuristic(s_start, s);
    k[1] = min(s.g, s.rhs);

    return k;
}

char compare_keys(void* k1, void* k2) {
    return (((double*)k1)[0] < ((double*)k2)[0]) || ((((double*)k1)[0] == ((double*)k2)[0]) && (((double*)k1)[1] < ((double*)k2)[1]));
}

double compute_cost(Fds *fds, State *s, State *sa, State *sb) {
    State *s1, *s2;
    Cell** cells;
    int num_cells;
    double c, b, f, vs, y, x;

    if (fabs(sa->v.x - s->v.x) == 1 && fabs(sa->v.y - s->v.y) == 1) {
        s1 = sb;
        s2 = sa;
    }
    else {
        s1 = sa;
        s2 = sb;
    }

    cells = map_get_cells_from_states(fds->m, s, s1, s2, &num_cells);

    c = cells[0]->c;
    b = cells[1]->c;

    if (min(c, b) == 1.0f) {
        vs = 1.0f;
    }
    else if (s1->g <= s2->g) {
        vs = min(c, b) + s1->g;
    }
    else {
        f = s1->g - s2->g;
        if (f <= b) {
            if (c <= f) {
                vs = c * sqrt(2.0f) + s2->g;
            }
            else {
                y = min(f / sqrt(pow(c, 2.0f) - pow(f, 2.0f)), 1.0f);
                vs = c * sqrt(1.0f + pow(y, 2.0f)) + f * (1.0f - y) + s2->g;
            }
        }
        else {
            if (c <= b) {
                vs = c * sqrt(2.0f) + s2->g;
            }
            else {
                x = 1.0f - min(b / sqrt(pow(c, 2.0f) - pow(b, 2.0f)), 1.0f);
                vs = c * sqrt(1.0f + pow((1.0f - x), 2.0f)) + b * x + s2->g;
            }
        }
    }

    return vs;
}

void UpdateState(State *s, State s_goal, State s_start, HEAP* OPEN) {
    
}

void ComputeShortestPath(HEAP* OPEN, State s_start, State s_goal, Map *m) {
    
}
