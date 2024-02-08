/*
 * control_module.c
 * Author: Yannick Abouem
 * Date: 2024-02-05
 * Description:
 * Module that control uav movements
 */

#include "uav.h"

#define DEBUG

// Euclidian distance between two points in a coorinate plane
double cm_euclidian_distance(double* uavPos, double* goalPos){
    double xDiff = uavPos[0] - goalPos[0];
    double yDiff = uavPos[1] - goalPos[1];

    return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}

// Computes the heading of the goal from distance and current position
double cm_compute_heading(double* uavPos, double* goalPos, double dist){
    double opp = fabs(goalPos[1] - uavPos[1]);
    
    return asin(opp / dist);
}

void cm_move_to_goal(Uav* uav, double* goalCoord, double altitude){
    double *gpsCoord;
    double dist, heading, curHeading;
    double yaw = 0.0;
    double pitch = 0.0;

    gpsCoord = uav_get_gps_pos(uav);
    dist = cm_euclidian_distance(gpsCoord, goalCoord);
    heading = cm_compute_heading(gpsCoord, goalCoord, dist);

    curHeading = uav_get_heading(uav);

    #ifdef DEBUG
    printf("Distance: %f\tHeading: %f to %f\n", dist, curHeading, heading);
    #endif /* ifdef DEBUG */

    if (curHeading != heading) {
        yaw = 1.3;
        if (curHeading < heading || curHeading > heading + 180.0)
            yaw *= -1.0;

    }

    if (dist > 0) {
        pitch = -1.3;
    }

    uav_actuate_motors(uav, 0.0, pitch, yaw, altitude);
}
