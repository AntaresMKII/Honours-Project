#ifndef MAP_H

#include "../../includes/uav.h"
#include "../../modules/includes/fds.h"

#define MAP_H
#define MAP_SIZE 100

typedef Cell** Map;

Map map_create();
void map_set_cell(Map *m, Position p, char v);
void map_add_obstacles(Map *m, Position *p, int p_size);
void map_print(Map *m);

#endif // !MAP_H
