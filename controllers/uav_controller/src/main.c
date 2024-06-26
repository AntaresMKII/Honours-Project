#include <stdio.h>
#include <math.h>

#include "includes/uav.h"
#include "modules/includes/fds.h"
#include "util/includes/map.h"
#include "util/includes/vec.h"

#define GOAL_X 5.0f
#define GOAL_Y 5.0f
#define TARGT_ALT 2.0f

Uav uav;

void set_start_and_goal(Uav *uav) {
  double *d_start = uav_get_gps_pos(uav);
  
  Vec3d goal = { GOAL_X, GOAL_Y, 0 };
  Vec3d start = { 0 };

  modf(d_start[0], &start.x);
  modf(d_start[1], &start.y);
  
  uav->fds = fds_init(start, goal);
}

int init() {
  int timestep;
  
  printf("Initializing uav\n");

  wb_robot_init();
  timestep = (int)wb_robot_get_basic_time_step();
  
  init_debug_file();

  uav_init(&uav, timestep);

  while (wb_robot_step(timestep) != -1) {
    if (wb_robot_get_time() > 1.0)
      break;
  }
  
  set_start_and_goal(&uav);
  
  return timestep;
}

void main_loop(int timestep) {
  Position next_wp = { GOAL_X, GOAL_Y, TARGT_ALT };
  Vec3d *obs_list;
  int obs_num;
  
  printf("Running main loop\n");
  
  while (wb_robot_step(timestep) != -1) {
    const double time = wb_robot_get_time();
    cm_plan_path(&uav);
    cm_run(&uav, next_wp, time);
  };
}

void clean_up() {
  printf("Cleaninig up\n");
  wb_robot_cleanup();
  cleanup_debug_file();
}

int main(int argc, char *argv[]) {
  int timestep;
  timestep = init();
  main_loop(timestep);
  clean_up();
  return EXIT_SUCCESS;
}
