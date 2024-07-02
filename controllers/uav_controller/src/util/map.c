#include "includes/map.h"
#include "includes/vec.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static inline Vec3d convert(Vec3d v) {
    Vec3d u = { v.x + MAP_SIZE / 2.0f, MAP_SIZE / 2.0f - v.y, v.z };
    return u;
}

Map map_create() {
    Map m = (Cell**) malloc(MAP_SIZE * sizeof(Cell*));
    if (m == NULL) {
        printf("Failed to allocate memory for Map in map_create()\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAP_SIZE; i++) {
        m[i] = (Cell*) malloc(MAP_SIZE * sizeof(Cell));
        if (m[i] == NULL) {
            printf("Failed to allocate memory for Map subarray in map_create()\n");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < MAP_SIZE; j++) {
            m[i][j].c = 0.0f;
            if (i == 0 && j == 0) {
                m[i][j].s0 = (State*) malloc(sizeof(State));
                m[i][j].s0->v.x = -1 * (MAP_SIZE / 2.0f);
                m[i][j].s0->v.y = (MAP_SIZE / 2.0f);
                m[i][j].s0->g = INFINITY;
                m[i][j].s0->rhs = INFINITY;
                m[i][j].s0->visited = 0;
                m[i][j].s1 = (State*) malloc(sizeof(State));
                m[i][j].s1->v.x = m[i][j].s0->v.x + 1;
                m[i][j].s1->v.y = m[i][j].s0->v.y;
                m[i][j].s1->g = INFINITY;
                m[i][j].s1->rhs = INFINITY;
                m[i][j].s1->visited = 0;
                m[i][j].s2 = (State*) malloc(sizeof(State));
                m[i][j].s2->v.x = m[i][j].s1->v.y;
                m[i][j].s2->v.y = m[i][j].s1->v.y - 1;
                m[i][j].s2->g = INFINITY;
                m[i][j].s2->rhs = INFINITY;
                m[i][j].s2->visited = 0;
                m[i][j].s3 = (State*) malloc(sizeof(State));
                m[i][j].s3->v.x = m[i][j].s0->v.x;
                m[i][j].s3->v.y = m[i][j].s2->v.y;
                m[i][j].s2->g = INFINITY;
                m[i][j].s2->rhs = INFINITY;
                m[i][j].s2->visited = 0;
            }
            else if (i == 0){
                m[i][j].s0 = m[i][j-1].s1; 
                m[i][j].s1 = (State*) malloc(sizeof(State));
                m[i][j].s1->v.x = m[i][j].s0->v.x + 1;
                m[i][j].s1->v.y = m[i][j].s0->v.y;
                m[i][j].s1->g = INFINITY;
                m[i][j].s1->rhs = INFINITY;
                m[i][j].s1->visited = 0;
                m[i][j].s2 = (State*) malloc(sizeof(State));
                m[i][j].s2->v.x = m[i][j].s1->v.x;
                m[i][j].s2->v.y = m[i][j].s1->v.y - 1;
                m[i][j].s2->g = INFINITY;
                m[i][j].s2->rhs = INFINITY;
                m[i][j].s2->visited = 0;
                m[i][j].s3 = m[i][j-1].s2;
            }
            else if(j == 0){
                m[i][j].s0 = m[i-1][j].s3;
                m[i][j].s1 = m[i-1][j].s2;
                m[i][j].s2 = (State*) malloc(sizeof(State));
                m[i][j].s2->v.x = m[i][j].s1->v.x;
                m[i][j].s2->v.y = m[i][j].s1->v.y - 1;
                m[i][j].s2->g = INFINITY;
                m[i][j].s2->rhs = INFINITY;
                m[i][j].s2->visited = 0;
                m[i][j].s3 = (State*) malloc(sizeof(State));
                m[i][j].s3->v.x = m[i][j].s0->v.x;
                m[i][j].s3->v.y = m[i][j].s0->v.y - 1;
                m[i][j].s3->g = INFINITY;
                m[i][j].s3->rhs = INFINITY;
                m[i][j].s3->visited = 0;
            }
            else {
                m[i][j].s0 = m[i][j-1].s1;
                m[i][j].s1 = m[i-1][j].s2;
                m[i][j].s2 = (State*) malloc(sizeof(State));
                m[i][j].s2->v.x = m[i][j].s1->v.x;
                m[i][j].s2->v.y = m[i][j].s1->v.y - 1;
                m[i][j].s2->g = INFINITY;
                m[i][j].s2->rhs = INFINITY;
                m[i][j].s2->visited = 0;
                m[i][j].s3 = m[i][j-1].s2;
            }

            if(m[i][j].s0 == NULL || m[i][j].s1 == NULL || m[i][j].s2 == NULL || m[i][j].s3 == NULL) {
                printf("Failed to allocate memory for states");
                exit(EXIT_FAILURE);
            }
        }
    }

    return m;
}

Cell** map_get_cells(Map *m, Vec3d v, int *num_cells) {
    Vec3d u = convert(v);
    Vec3d idx, frac;
    Cell **rv;

    if (u.x >= MAP_SIZE)
        u.x = u.x - 0.1f;

    if (u.y >= MAP_SIZE)
        u.y = u.y - 0.1f;

    frac.x = modf(u.x, &idx.x);
    frac.y = modf(u.y, &idx.y);

    if ((frac.x != 0.0f && frac.y != 0.0f) || (idx.x == 0.0f || idx.y == 0.0f)) {
        rv = (Cell**) malloc(sizeof(Cell*));
        if (rv == NULL) {
            printf("Failed to allocate Cell in map_get_cell");
            exit(EXIT_FAILURE);
        }
        *rv = &((*m)[(int)idx.y][(int)idx.x]);
        *num_cells = 1;
    }
    else if (frac.x == 0.0f && frac.y != 0.0f) {
       rv = (Cell**) malloc(sizeof(Cell*) * 2);
        if (rv == NULL) {
            printf("Failed to allocate Cell in map_get_cell");
            exit(EXIT_FAILURE);
        }

        rv[0] = &((*m)[(int)idx.y][(int)idx.x-1]);
        rv[1] = &((*m)[(int)idx.y][(int)idx.x]);
        *num_cells = 2;
    }
    else if (frac.x != 0.0f && frac.y == 0.0f) {
       rv = (Cell**) malloc(sizeof(Cell*) * 2);
        if (rv == NULL) {
            printf("Failed to allocate Cell in map_get_cell");
            exit(EXIT_FAILURE);
        }

        rv[0] = &((*m)[(int)idx.y-1][(int)idx.x]);
        rv[1] = &((*m)[(int)idx.y][(int)idx.x]);
        *num_cells = 2;
    }
    else {
        rv = (Cell**) malloc(sizeof(Cell*) * 4);
        if (rv == NULL) {
            printf("Failed to allocate Cell in map_get_cell");
            exit(EXIT_FAILURE);
        }

        rv[0] = &((*m)[(int)idx.y-1][(int)idx.x-1]);
        rv[1] = &((*m)[(int)idx.y-1][(int)idx.x]);
        rv[2] = &((*m)[(int)idx.y][(int)idx.x-1]);
        rv[3] = &((*m)[(int)idx.y][(int)idx.x]);
        *num_cells = 4;
    }

    return rv;
}

State** map_get_nbrs(Map *m, State *s, int *num_nbrs) {
    Cell** cells;
    int num_cells;
    State** nbrs;

    cells = map_get_cells(m, s->v, &num_cells);

    if (num_cells == 4) {
        nbrs = (State**) malloc(sizeof(State*) * 8);
        if (nbrs == NULL) {
            printf("Unable to allocate memory in map_get_nbrs\n");
        }

        nbrs[0] = cells[0]->s0;
        nbrs[1] = cells[0]->s1;
        nbrs[2] = cells[1]->s1;
        nbrs[3] = cells[1]->s2;
        nbrs[4] = cells[3]->s2;
        nbrs[5] = cells[3]->s3;
        nbrs[6] = cells[2]->s3;
        nbrs[7] = cells[2]->s0;

        *num_nbrs = 8;
    }
    else if (num_cells == 2) {
        nbrs = (State**) malloc(sizeof(State*) * 5);
        if (nbrs == NULL) {
            printf("Unable to allocate memory in map_get_nbrs\n");
        }

        if (s->v.x == 0.0f) {
            nbrs[0] = cells[0]->s0;
            nbrs[1] = cells[0]->s1;
            nbrs[2] = cells[0]->s2;
            nbrs[3] = cells[1]->s2;
            nbrs[4] = cells[1]->s3;
        }
        else if (s->v.y == 0.0f) {
            nbrs[0] = cells[0]->s0;
            nbrs[1] = cells[1]->s1;
            nbrs[2] = cells[1]->s2;
            nbrs[3] = cells[1]->s3;
            nbrs[4] = cells[0]->s3;
        }
        else if (s->v.x == MAP_SIZE) {
            nbrs[0] = cells[0]->s0;
            nbrs[1] = cells[0]->s1;
            nbrs[2] = cells[1]->s2;
            nbrs[3] = cells[1]->s3;
            nbrs[4] = cells[1]->s0;
        }
        else if (s->v.y == MAP_SIZE) {
            nbrs[0] = cells[0]->s0;
            nbrs[1] = cells[0]->s1;
            nbrs[2] = cells[1]->s1;
            nbrs[3] = cells[1]->s2;
            nbrs[4] = cells[0]->s3;
        }

        *num_nbrs = 5;
    }
    else {
        nbrs = (State**) malloc(sizeof(State*) * 3);
        if (nbrs == NULL) {
            printf("Unable to allocate memory in map_get_nbrs\n");
        }

        if (s->v.x == 0.0f && s->v.y == 0.0f) {
            nbrs[0] = cells[0]->s1;
            nbrs[1] = cells[0]->s2;
            nbrs[2] = cells[0]->s3;
        }
        else if (s->v.x == 0.0f && s->v.y == MAP_SIZE) {
            nbrs[0] = cells[0]->s0;
            nbrs[1] = cells[0]->s1;
            nbrs[2] = cells[0]->s2;
        }
        else if (s->v.x == MAP_SIZE && s->v.y == 0.0f) {
            nbrs[0] = cells[0]->s0;
            nbrs[1] = cells[0]->s2;
            nbrs[2] = cells[0]->s3;
        }
        else if (s->v.x == MAP_SIZE && s->v.y == MAP_SIZE) {
            nbrs[0] = cells[0]->s0;
            nbrs[1] = cells[0]->s1;
            nbrs[2] = cells[0]->s3;
        }

        *num_nbrs = 3;
    }

    return nbrs;
}

Tuple* map_get_connbrs(Map *m, State *s, int *num_nbrs) {
    State** nbrs;
    Tuple* tuple_arr;
    
    nbrs = map_get_nbrs(m, s, num_nbrs);

    tuple_arr = (Tuple*) malloc(sizeof(Tuple) * *num_nbrs);
    if (tuple_arr == NULL) {
        printf("Failure to allocate space for tuple array\n");
    }

    for (int i = 0; i < *num_nbrs; i++) {
        tuple_arr[i].fst = nbrs[i];
        if (i + 1 < *num_nbrs) {
            tuple_arr[i].snd = nbrs[i+1];
        }
        else {
            tuple_arr[i].snd = nbrs[0];
        }
    }

    if (*num_nbrs != 8) {
        for (int i = 0; i < *num_nbrs; i++) {
            if ((fabs(tuple_arr[i].fst->v.x - tuple_arr[i].snd->v.x) > 1 || fabs(tuple_arr[i].fst->v.y - tuple_arr[i].snd->v.y) > 1) || (fabs(tuple_arr[i].fst->v.x - tuple_arr[i].snd->v.x) > 1 && fabs(tuple_arr[i].fst->v.y - tuple_arr[i].snd->v.y) > 1)) {
                tuple_arr[i].fst = NULL;
                tuple_arr[i].snd = NULL;
                (*num_nbrs)--;
            } 
        }
    }

    return tuple_arr;
}

Cell** map_get_cells_from_states(Map *m, State *s1, State *s2, State *s3, int *num_cells) {
    Cell **cells, **sel_cells, *tmp;
    int n_cells, j;
    
    cells = map_get_cells(m, s1->v, &n_cells);
 
    sel_cells = (Cell**) malloc(sizeof(Cell*) * 2);
    if (sel_cells == NULL) {
        printf("Unabnle to allocate memory in map_get_cells_from_states\n");
        exit(EXIT_FAILURE);
    }

    j = 0;

    for (int i = 0; i < n_cells; i++) {
        if (states_are_equal(s2, cells[i]->s0) || states_are_equal(s2, cells[i]->s1) || states_are_equal(s2, cells[i]->s2) || states_are_equal(s2, cells[i]->s3)) {
            sel_cells[j] = cells[i];
            j++;
        }
    }

    for (int i = 0; i < j; i++) {
        if (states_are_equal(s3, sel_cells[i]->s0) || states_are_equal(s3, sel_cells[i]->s1) || states_are_equal(s3, sel_cells[i]->s3) || states_are_equal(s3, sel_cells[i]->s3)) {
            if (i != 0) {
                //flip 
                tmp = sel_cells[i];
                sel_cells[i] = sel_cells[i-1];
                sel_cells[i-1] = tmp;
            }
            break;
        }

    }

    *num_cells = j;
    return sel_cells;
}

State* map_get_state(Map *m, Vec3d v) {
    Cell** cells;
    int num_cells;

    cells = map_get_cells(m, v, &num_cells);

    for (int i = 0; i < num_cells; i++) {
        if (vec_equal(v, cells[i]->s0->v)) {
            return cells[i]->s0; 
        }
        else if (vec_equal(v, cells[i]->s1->v)) {
            return cells[i]->s1; 
        }
        else if (vec_equal(v, cells[i]->s2->v)) {
            return cells[i]->s2; 
        }
        else if (vec_equal(v, cells[i]->s3->v)) {
            return cells[i]->s3; 
        }
    }

    return NULL;
}

Cell** map_set_cells_cost(Map *m, Vec3d v, double cost, int *num_cells) {
    Cell** cells;
    int n;

    cells = map_get_cells(m, v, num_cells);

    for (int i = 0; i < *num_cells; i++) {
        if (cost > cells[i]->c) {
            cells[i]->c = cost;
        }
    }

    return cells;
}
