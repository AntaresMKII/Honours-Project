/*
 * control_module.c
 * Author: Yannick Abouem
 * Date: 2024-02-05
 * Description:
 * Module that control uav movements
 */

#include "uav.h"
#include <stdio.h>
#include <stdlib.h>

double compute_yaw_angle(Uav *uav, Position goal) {
    double angle;
    angle = atan2(uav->pos.y - goal.y, uav->pos.x - goal.x);
    angle = angle - uav->pos.roll;
    angle = (angle + 2 * M_PI) % (2 * M_PI);

    if (angle > M_PI) {
        angle -= 2 * M_PI;
    }

    return MAX_YAW_DIST * angle / (2 * M_PI);
}

double compute_pitch_angle(double yaw) {
    return clamp(log10(fabs(yaw)), MAX_PITCH_DIST, 0.1);
}

int move_to_goal(Uav *uav, Position goal, double *disturbance) {
    double yaw_distrubance, pitch_disturbance;

    // Check if UAV is at goal
    if ((uav->pos.x - goal.x) < TARGET_PRECISION) {
        // Can be modified to allow for multiple waypoints
        return 1;
    }

    yaw_distrubance = compute_yaw_angle(uav, goal);
    pitch_disturbance = compute_pitch_angle(yaw_distrubance);

    disturbance = (double *) malloc(sizeof(double[2]));
    if (disturbance == NULL) {
        perror("Error: Failed to allocate memory!");
        return -1;
    }

    return 0;
}


