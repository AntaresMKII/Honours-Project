#ifndef MAP_H

#include "../../includes/uav.h"

#define MAP_H
#define MAP_SIZE 100

typedef char** Map;

Map map_create();
void map_set_cell(Map *m, Position p, char v);

#endif // !MAP_H
