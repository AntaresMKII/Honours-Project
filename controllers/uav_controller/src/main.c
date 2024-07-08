#include <stdio.h>
#include <math.h>

#include "includes/uav.h"
#include "includes/od_test.h"
#include "modules/includes/fds.h"
#include "util/includes/map.h"
#include "util/includes/util.h"
#include "util/includes/vec.h"

#define GOAL_X 5.0f
#define GOAL_Y 5.0f
#define TARGT_ALT 2.0f

Uav uav;

void set_id(Uav *uav, Vec3d start) {
  uav->id = dtouc(start.x + start.y);
  printf("%d\n", uav->id);
}

void set_start_and_goal(Uav *uav) {
  const double *d_start = uav_get_gps_pos(uav);
  
  Vec3d goal = { GOAL_X, GOAL_Y, 0 };
  Vec3d start = { 0 };

  modf(d_start[0], &start.x);
  modf(d_start[1], &start.y);
  
  uav->fds = fds_init(start, goal);

  set_id(uav, start);
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

  add_obst(uav.fds->m);
  
  return timestep;
}

void run() {
  const double time = wb_robot_get_time();

  static int curr_wp = 0;

  Vec3d *wps_list;
  int wps_num = -1;
  int wp_reached = 0;

  wps_list = cm_plan_path(&uav, &wps_num);
  wp_reached = cm_run(&uav, wps_list[curr_wp], TARGT_ALT, time);
  if (wp_reached) {
    curr_wp++;
    if (curr_wp == wps_num)
      uav.state = END;
  }
}

void main_loop(int timestep) {
    
  printf("Running main loop\n");

  while (wb_robot_step(timestep) != -1) {
    switch (uav.state) {
      case INIT:
        net_elect_leader(&uav, timestep);
        uav.state = RUN;
        break;
      case RUN:
        run();
        break;
      default:
        return;
    }
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
