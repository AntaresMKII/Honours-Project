/*
 * control_module.c
 * Author: Yannick Abouem
 * Date: 2024-02-05
 * Description:
 * Module that control uav movements
 */

#include "uav.h"
#include "vector.h"
#include <stdio.h>

#define DEBUG

// Compute the heading relative to the UAV's current position
double compute_heading(double* gpsCoord, double* goalCoord) {
    double relVec[2];
    double heading;
    double northVec[2] = {0,1};
    
    relVec[0] = goalCoord[0] - gpsCoord[0];
    relVec[1] = goalCoord[1] - gpsCoord[1];

    heading = angle_between(relVec, northVec);

    heading = (heading - 1.5708) / M_PI * 180.0;

    return heading;
}

// Compute the yaw displacement required to turn nicely to the desired heading
double turn_to_heading(double targetHeading, double curHeading) {
    double relHeading = targetHeading;
    double yaw = 0.1;

    #ifdef DEBUG
    printf("Relative Heading: %f\n", relHeading);
    #endif /* ifdef DEBUG */

    if (relHeading <= 0) {
        relHeading += 360.0;
    }

    if (relHeading > 180.0) {
        // turn left otherwise turn right
        yaw *= -1.0;
        relHeading = (relHeading - 360.0) * -1.0;
    }

    return yaw * (relHeading / 180.0);
}

// Move the UAV towards the predefined goal
void cm_move_to_goal(Uav* uav, double* goalCoord, double altitude){
    double *gpsCoord;
    double dist, heading, curHeading;
    double yaw = 0.0;
    double pitch = 0.0;

    gpsCoord = uav_get_gps_pos(uav);
    curHeading = uav_get_heading(uav);

    heading = compute_heading(gpsCoord, goalCoord);
#ifdef DEBUG
    printf("Computed Heading: %f\tCurrent Heading: %f\n", heading + curHeading, curHeading);
#endif /* ifdef DEBUG */

    yaw = turn_to_heading(heading, curHeading);

    uav_actuate_motors(uav, 0.0, pitch, yaw, altitude);
}
