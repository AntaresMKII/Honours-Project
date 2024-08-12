#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "includes/uav.h"
#include "includes/od_test.h"
#include "modules/includes/fds.h"
#include "util/includes/map.h"
#include "util/includes/util.h"
#include "util/includes/vec.h"

#define GOAL_X 20.0f
#define GOAL_Y 0.0f
#define TARGT_ALT 2.0f

Uav uav;

void set_id(Uav *uav, Vec3d start) {
  srand(start.x);
  int r = (rand() % 100) + 1;
  srand(start.y);
  for (int i = 0; i < r; i++) {
    uav->id = (rand() % 255) + 1;
  }

  printf("Assigned id: %d to uav at pos (%f,%f)\n", uav->id, start.x, start.y);
}

void set_start_and_goal(Uav *uav) {
  const double *d_start = uav_get_gps_pos(uav);
  
  Vec3d goal = { GOAL_X, GOAL_Y, 0 };
  Vec3d start = { 0 };
  double rest_x, rest_y;

  rest_x = modf(d_start[0], &start.x);
  rest_y = modf(d_start[1], &start.y);

  if (rest_x >= 0.5) {
    start.x++;
  }
  else if (rest_x <= -0.5) {
    start.x--;
  }

  if (rest_y >= 0.5) {
    start.y++;
  }
  else if (rest_y <= -0.5) {
    start.y--;
  }
 
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
  return timestep;
}
/*
void run() {

  static int curr_wp = 0;

  Vec3d *wps_list;
  State* s;
  int wps_num = -1;
  int wp_reached = 0;

  wps_list = cm_plan_path(&uav, &wps_num);
  cm_followers_path(&uav, wps_list, wps_num);
  net_send_wp(&uav, curr_wp);
  wp_reached = cm_run(&uav, wps_list[curr_wp], TARGT_ALT, time);
  if (wp_reached) {
    s = map_get_state(uav.fds->m, wps_list[curr_wp]);
    s->visited = 1;
    curr_wp++;
    printf("%d : Next Waypoint: (%f,%f)\n", uav.id, wps_list[curr_wp].x, wps_list[curr_wp].y);
    if (curr_wp == wps_num)
      uav.state = END;
  }
}
*/
void run() {
  const double time = wb_robot_get_time();

  static int n = 0;

  Vec3d *wps_list;
  State *s;
  int wps_num = 0;
  int wp_reached = 0;

  wps_list = cm_plan_path(&uav, &wps_num);
  cm_followers_path(&uav, wps_list, wps_num);
  net_send_wp(&uav, n);
  wp_reached = cm_run(&uav, wps_list[n], TARGT_ALT, time);
  if (wp_reached) {
    //s = map_get_state(uav.fds->m, wps_list[0]);
    //uav.fds->start = s;
    n++;
    printf("Waypoint reached! Next wp: (%f, %f)\n", wps_list[n].x, wps_list[n].y);
    //if (states_are_equal(uav.fds->start, uav.fds->end)) {
    if (n == wps_num) {
      printf("Goal reached!\n");
      uav.state = END;
    }
  }
}

void follower_wait() {
  const double time = wb_robot_get_time();
  cm_run(&uav, uav.fds->start->v, TARGT_ALT, time); 
}

void follower_run() {
  const double time = wb_robot_get_time();

  static Vec3d wp;
  static int curr_wp = 0;

  int wp_reaced = 0;

  net_recieve_wp(&uav, &wp);

  if (uav.state == F_RUN) {
    wp_reaced = cm_run(&uav, wp, TARGT_ALT, time);
    if (wp_reaced) {
      curr_wp++;
      //net_ask_next_wp(&uav, curr_wp);
    }
  }
}

void main_loop(int timestep) {
    
  printf("Running main loop\n");

  while (wb_robot_step(timestep) != -1) {
    switch (uav.state) {
      case INIT:
        net_elect_leader(&uav, timestep);
        uav.state = (uav.id == uav.l_id) ? RUN : F_WAIT;
        net_share_init_pos(&uav, timestep);
        break;
      case RUN:
        run();
        break;
      case F_WAIT:
        follower_wait();
      case F_RUN:
        follower_run();
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
