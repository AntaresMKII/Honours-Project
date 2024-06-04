/*! main.c
 *
 * Main file of the controllecr. It contains the main function
 */
#include "includes/uav.h"
#include "util/includes/map.h"
#include <stdio.h>

/*!
 * \brief Main function of the controller
 *
 * This function contains the main loop of the controller
 */
int main(int argc, char **argv) {
  // Declare variabels and initialize goal
  Uav uav;
  Position goal;
  Position *obstacles;
  int timestep, obstacles_num;
  Map m;

  goal.x = -5.0;
  goal.y = 6.5;
  goal.z = 2.0; // Target altitude

  // Initialize webots controller
  wb_robot_init();
  timestep = (int)wb_robot_get_basic_time_step();

#ifdef DEBUG
  init_debug_file();
#endif /* ifdef DEBUG */

  /* Initialize the uav */
  uav_init(&uav, timestep);

  // Display the welcome message.
  printf("Starting the drone...\n");

  // Wait one second.
  while (wb_robot_step(timestep) != -1) {
    if (wb_robot_get_time() > 1.0)
      break;
  }
  
  // Main loop
  while (wb_robot_step(timestep) != -1) {
    const double time = wb_robot_get_time();
    
    //obstacles = cm_detect_obstacles(&uav, &obstacles_num);
    //cm_run(&uav, goal, time); // Execute drone movements
  };

  // Clean up
#ifdef DEBUG
  cleanup_debug_file();
#endif /* ifdef DEBUG */

  wb_robot_cleanup();

  return EXIT_SUCCESS;
}
