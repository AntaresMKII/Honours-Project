/*
 * control_module.c
 * Author: Yannick Abouem
 * Date: 2024-02-05
 * Description:
 * Module that control uav movements
 */

#include <math.h>

// Euclidian distance between two points in a coorinate plane
void cm_euclidian_distance(double* uavPos, double* goalPos){
    double xDiff = uavPos[0] - goalPos[0];
    double yDiff = uavPos[1] - goalPos[1];

    return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}

// Computes the heading of the goal from distance and current position
void cm_compute_heading(double* uavPos, double* goalPos, double dist){
    double adj = abs(uavPos[0] - goalPos[0]);
    
    return acos(adj / dist);
}


