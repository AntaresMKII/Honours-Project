#include "includes/map.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Map map_create() {
    Map m = (char**) malloc(MAP_SIZE);
    if (m == NULL) {
        printf("Failed to allocate memory for Map in map_create()\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAP_SIZE; i++) {
        m[i] = (char*) malloc(MAP_SIZE);
        if (m[i] == NULL) {
            printf("Failed to allocate memory for Map subarray in map_create()\n");
            exit(EXIT_FAILURE);
        }
    }

    return m;
}

void map_set_cell(Map *m, Position p, char v) {
    int x,y;
    x = floor(p.x);
    y = floor(p.y);

    x += 50;
    y += 50;

    if (x >= MAP_SIZE || x < 0 || y >= MAP_SIZE || y < 0) {
        printf("Map coordinates out of bounds\n");
    }
    else {
        *m[y][x] = v;
    }
}
