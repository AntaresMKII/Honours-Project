#include "includes/map.h"
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            m[i][j].c = 0;
            if (i == 0 && j == 0) {
                m[i][j].s0 = (State*) malloc(sizeof(State));
                m[i][j].s0->v.x = -25.0f;
                m[i][j].s0->v.y = 25.0f;
                m[i][j].s1 = (State*) malloc(sizeof(State));
                m[i][j].s1->v.x = -24.0f;
                m[i][j].s1->v.y = 25.0f;
                m[i][j].s2 = (State*) malloc(sizeof(State));
                m[i][j].s2->v.x = -24.0f;
                m[i][j].s2->v.y = 24.0f;
                m[i][j].s3 = (State*) malloc(sizeof(State));
                m[i][j].s3->v.x = -25.0f;
                m[i][j].s3->v.y = 24.0f;
            }
            else if (i == 0){
                m[i][j].s0 = m[i][j-1].s1; 
                m[i][j].s1 = (State*) malloc(sizeof(State));
                m[i][j].s1->v.x = m[i][j].s0->v.x + 1;
                m[i][j].s1->v.y = m[i][j].s0->v.y;
                m[i][j].s2 = (State*) malloc(sizeof(State));
                m[i][j].s2->v.x = m[i][j].s1->v.x;
                m[i][j].s2->v.y = m[i][j].s1->v.y - 1;
                m[i][j].s3 = m[i][j-1].s2;
            }
            else if(j == 0){
                m[i][j].s0 = m[i-1][j].s3;
                m[i][j].s1 = m[i-1][j].s2;
                m[i][j].s2 = (State*) malloc(sizeof(State));
                m[i][j].s2->v.x = m[i][j].s1->v.x;
                m[i][j].s2->v.y = m[i][j].s1->v.y - 1;
                m[i][j].s3 = (State*) malloc(sizeof(State));
                m[i][j].s3->v.x = m[i][j].s0->v.x;
                m[i][j].s3->v.y = m[i][j].s0->v.y - 1;
            }
            else {
                m[i][j].s0 = m[i][j-1].s1;
                m[i][j].s1 = m[i-1][j].s2;
                m[i][j].s2 = (State*) malloc(sizeof(State));
                m[i][j].s2->v.x = m[i][j].s1->v.x;
                m[i][j].s2->v.y = m[i][j].s1->v.y - 1;
                m[i][j].s3 = m[i][j-1].s2;
            }
        }
    }

    return m;
}

void map_set_cell(Map *m, Position p, char v) {
    int x,y;
    x = floor(p.x);
    y = floor(p.y);

    x += 25;
    y += 25;

    if (x >= MAP_SIZE || x < 0 || y >= MAP_SIZE || y < 0) {
        printf("Map coordinates out of bounds\n");
    }
    else {
        (*m)[y][x] = v;
    }
}

void map_add_obstacles(Map *m, Position *p, int p_size) {
    for (int i = 0; i < p_size; i++) {
        map_set_cell(m, p[i], 1);
    }
}

void map_print(Map *m) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            printf("%d ", (*m)[i][j]);
        }
        printf("\n");
    }
}
