#include "includes/od_test.h"
#include "util/includes/vec.h"


void add_obst(Map *m) {
  Vec3d obs = { 2.0f, 2.0f, 0 };
  int num_cells, n;
  Cell **cells;

  cells = map_set_cells_cost(m, obs, 1.0f, &num_cells);
/*
  for (int i = 0; i < num_cells; i++) {
    map_set_cells_cost(m, cells[i]->s0->v, 0.5f, &n);
    map_set_cells_cost(m, cells[i]->s1->v, 0.5f, &n);
    map_set_cells_cost(m, cells[i]->s2->v, 0.5f, &n);
    map_set_cells_cost(m, cells[i]->s3->v, 0.5f, &n);
  }
  */
}
