#include <stdio.h>

#include "includes/uav.h"
#include "modules/includes/fds.h"
#include "util/includes/map.h"
#include "util/includes/vec.h"

#define GOAL_X -5.0f
#define GOAL_Y 6.0f
#define TARGT_ALT 2.0f

void set_start_and_goal(Uav *uav) {
  double *d_start = uav_get_gps_pos(uav);

  Vec3d start = {d_start[0], d_start[1], 0};
  Vec3d goal = {GOAL_X, GOAL_Y, 0};

  uav->fds = fds_init(start, goal);
}

void init() {
  Uav uav;
  Position goal = {GOAL_X, GOAL_Y, TARGT_ALT};
  int timestep;

  wb_robot_init();
  timestep = (int)wb_robot_get_basic_time_step();

  uav_init(&uav, timestep);

  while (wb_robot_step(timestep) != -1) {
    if (wb_robot_get_time() > 1.0)
      break;
  }
}

void main_loop() {
  while (wb_robot_step(timestep) != -1) {
    const double time = wb_robot_get_time();
    
  };
}

void clean_up() {
  wb_robot_cleanup();
}

int main(int argc, char *argv[]) {
  init();
  main_loop();
  clean_up();
  return EXIT_SUCCESS;
}
