#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "includes/fds.h"
#include "../util/includes/util.h"

#define min(a,b) a < b ? a : b

static inline double euclid_heuristic(State *s1, State *s2) {
    return sqrt(pow(fabs(s1->v.x - s2->v.x), 2) + pow(fabs(s1->v.y - s2->v.y), 2));
}

double* key(State *s, State *s_start) {
    double *k = (double*) malloc(2*sizeof(double));
    if (k == NULL) {
        printf("Failed to allocate memory for key!\n");
        exit(EXIT_FAILURE);
    }

    k[0] = min(s->g, s->rhs) + euclid_heuristic(s_start, s);
    k[1] = min(s->g, s->rhs);

    return k;
}

char compare_keys(void* k1, void* k2) {
    return (((double*)k1)[0] < ((double*)k2)[0]) || ((((double*)k1)[0] == ((double*)k2)[0]) && (((double*)k1)[1] < ((double*)k2)[1]));
}

char compare_states(void* s1, void* s2) {
    return s1 == s2;
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

    if (min(c, b) == INFINITY) {
        vs = INFINITY;
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

void UpdateState(Fds *fds, State *s) {
    Tuple *connbrs;
    int num_nbrs;
    double vs;

    if (!s->visited) {
        s->g = INFINITY;
        s->visited = 1;
    }

    if (!states_are_equal(s, fds->end)) {
        connbrs = map_get_connbrs(fds->m, s, &num_nbrs);
        s->rhs = INFINITY;
        for (int i = 0; i < num_nbrs; i++) {
            vs = compute_cost(fds, s, connbrs[i].fst, connbrs[i].snd);
            if (vs < s->rhs) {
                s->rhs = vs;
                if (fabs(connbrs[i].fst->v.x - s->v.x) == 1 && fabs(connbrs[i].fst->v.y - s->v.y) == 1) {
                    s->s1 = connbrs[i].snd;
                    s->s2 = connbrs[i].fst;
                }
                else {
                    s->s1 = connbrs[i].fst;
                    s->s2 = connbrs[i].snd;
                }
            }
        }
    }

    heap_remove(fds->OPEN, (void*) s);

    if (s->g != s->rhs) {
        heap_add(fds->OPEN, (void*) s, (void *) key(s, fds->start));
    }
}

void ComputeShortestPath(Fds *fds) {
    State *s, **nbrs;
    int num_nbrs;

    while ((compare_keys(get_root_key(fds->OPEN), key(fds->start, fds->start))) || fds->start->g != fds->start->rhs) {
        s = (State*) pop_root_val(fds->OPEN);
        nbrs = map_get_nbrs(fds->m, s, &num_nbrs);
        s->visited = 1;

        if (s->g > s->rhs) {
            s->g = s->rhs;
            for (int i = 0; i < num_nbrs; i++) {
                UpdateState(fds, nbrs[i]);
            }
        }
        else {
            s->g = INFINITY;
            for (int i = 0; i < num_nbrs; i++) {
                UpdateState(fds, nbrs[i]);
            }
            UpdateState(fds, s);
        }
    }
}

Fds* fds_init(Vec3d start, Vec3d goal) {
    Map *m;
    State *s_start;
    State *s_goal;
    HEAP *h;
    Fds *fds;

    double *max_val;

    m = (Map*) malloc(sizeof(Map));
    if (m == NULL) {
        printf("Failure to allocate memory in fds_init!\n");
    }

    *m = map_create();

    s_start = map_get_state(m, start);
    s_goal = map_get_state(m, goal);

    s_start->g = INFINITY;
    s_start->rhs = INFINITY;
    s_goal->g = INFINITY;
    s_goal->rhs = 0.0f;

    max_val = (double*) malloc(sizeof(double) * 2);
    if (max_val == NULL) {
        printf("Error allocating memory for max_val in fds_init()\n");
        exit(EXIT_FAILURE);
    }

    max_val[0] = INFINITY;
    max_val[1] = INFINITY;

    h = new_heap(NULL, NULL, 0, compare_keys, compare_states, max_val);

    heap_add(h, (void*) s_goal, (void*) key(s_goal, s_start));

    fds = (Fds*) malloc(sizeof(Fds));
    if (fds == NULL) {
        printf("Error allocating memory for fds in fds_init()\n");
        exit(EXIT_FAILURE);
    }

    fds->m = m;
    fds->start = s_start;
    fds->end = s_goal;
    fds->OPEN = h;

    return fds;
}

void fds_run(Fds* fds, Cell** changed_cells, int num_cells) {
    ComputeShortestPath(fds);
    
    if (num_cells > 0) {
        for (int i = 0; i < num_cells; i++) {
            UpdateState(fds, changed_cells[i]->s3);
            UpdateState(fds, changed_cells[i]->s2);
            UpdateState(fds, changed_cells[i]->s1);
            UpdateState(fds, changed_cells[i]->s0);
        }
        ComputeShortestPath(fds);
    }
}

Vec3d* fds_extract_path(Fds* fds, int *wp_num) {
    State *s;
    Cell **cells;
    Vec3d next_wp, *wps;
    double f, c, b, y;
    int num_cells = 0;

    s = fds->start;

    wps = (Vec3d*) malloc(sizeof(Vec3d));
    if (wps == NULL) {
        exit(EXIT_FAILURE);
    }

    *wp_num = 0;

    while (!states_are_equal(s, fds->end)) {
        cells = map_get_cells_from_states(fds->m, s, s->s1, s->s2, &num_cells);
        c = cells[0]->c;
        b = cells[1]->c;
        f = s->s1->g - s->s2->g;

        if (f <= 0) {
            // next wp is at s1
            next_wp = s->s1->v;
        }
        else if (f > b) {
            // next wp is at s2
            next_wp = s->s2->v;
        }
        else {
            // next wp is at some point sy
            y = min(f / sqrt(pow(c, 2) - pow(f, 2)), 1);
            next_wp.x = s->s1->v.x;
            if (s->s1->v.y - s->s2->v.y < 0) {
                next_wp.y = s->s1->v.y + y;
            }
            else {
                next_wp.y = s->s1->v.y - y;
            }
        }

        *wp_num += 1;

        wps = (Vec3d*) realloc(wps, sizeof(Vec3d) * *wp_num);

        wps[*wp_num - 1].x = next_wp.x;
        wps[*wp_num - 1].y = next_wp.y;

        if (s->s1->g < s->s2->g) {
            s = s->s1;
        }
        else {
            s = s->s2;
        }
    }

    return wps;
}
