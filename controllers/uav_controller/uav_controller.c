#include "uav.h"
#include "util.h"
#include <stdio.h>

int main(int argc, char **argv) {
  // Initialize webots controller
  wb_robot_init();
  int timestep = (int)wb_robot_get_basic_time_step();
#ifdef DEBUG
  init_debug_file();
#endif /* ifdef DEBUG */

  // Declare variabels and initialize goal
  Uav uav;
  Position goal;

  goal.x = -5.0;
  goal.y = 6.5;
  goal.z = 2.0; // Target altitude

  /* Initialize the uav */
  uav_init(&uav, timestep);

  // Display the welcome message.
  printf("Starting the drone...\n");

  // Wait one second.
  while (wb_robot_step(timestep) != -1) {
    if (wb_robot_get_time() > 1.0)
      break;
  }
  
  // Initialize the id of the uav

  // Main loop
  while (wb_robot_step(timestep) != -1) {
    const double time = wb_robot_get_time();
    
    cm_run(&uav, goal, time); // Execute drone movements
    
  };

  // Clean up
#ifdef DEBUG
  cleanup_debug_file();
#endif /* ifdef DEBUG */
  wb_robot_cleanup();

  return EXIT_SUCCESS;
}
