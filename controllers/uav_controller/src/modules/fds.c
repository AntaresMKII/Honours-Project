#include "includes/fds.h"
#include "../util/includes/heap.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

void UpdateState(State *s, State s_goal, State s_start, HEAP* OPEN) {
    if (!s->visited) {
        s->g = INFINITY;
    }

    if ((s->v.x != s_goal.v.x) || (s->v.y != s_goal.v.y)) {
        //s->rhs = min cost of path to conneigbors
    }

    // if s is in OPEN remove from OPEN
    heap_remove(OPEN, (void*) &s);

    if (s->g == s->rhs) {
        heap_add(OPEN, (void*) &s, (void*) (key(*s, s_start)));
    }
}

void ComputeShortestPath(HEAP* OPEN, State s_start) {
   while(compare_keys(get_root_key(OPEN), (void*) key(s_start, s_start))) {
        State* s = (State*) pop_root_val(OPEN);

        if (s->g > s->rhs) {
            s->g = s->rhs;
        }
    }
}
