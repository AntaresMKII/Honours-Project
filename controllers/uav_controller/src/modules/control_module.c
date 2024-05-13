/*
 * control_module.c
 * Author: Yannick Abouem
 * Date: 2024-02-05
 * Description:
 * Module that control uav movements
 */

#include "../includes/uav.h"

#define CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

// Compute the angle in radians between the goal and the uav in [-pi, pi]
double compute_angle(Uav *uav, Position goal) {
    double angle;
    angle = atan2(goal.y - uav->pos.y, goal.x - uav->pos.x);
    angle = angle - uav->pos.yaw;
    angle = fmod((angle + 2 * M_PI), (2 * M_PI));

    if (angle > M_PI) {
        angle -= 2 * M_PI;
    }

    return angle;
}

// computes the pitch disturbance from the computed angle
double compute_pitch_disturbance(double angle) {
    return CLAMP(log10(fabs(angle)), MAX_PITCH_DIST, 0.1);
}

// Given a goal compute the pitch and yaw distrubance to reach the goal
int move_to_goal(Uav *uav, Position goal) {

    double angle = compute_angle(uav, goal);

    uav->yaw_disturbance = MAX_YAW_DIST * angle / (2 * M_PI);
    uav->pitch_disturbance = compute_pitch_disturbance(angle);

    #ifdef DEBUG
    double distance = sqrt(pow((goal.x - uav->pos.x),2) + pow((goal.y - uav->pos.y), 2));
    log2vf(angle, "Angle left", distance, "Distance left");
    #endif /* ifdef DEBUG */

    return 0;
}

// update variables in the UAV struct, compute the disturbance and actuate motors
void cm_run(Uav *uav, Position goal, double time) {

    Position newPos;

    double *gpsPos = uav_get_gps_pos(uav);

    newPos.x = gpsPos[0];
    newPos.y = gpsPos[1];

    newPos.z = uav_get_gps_altitude(uav);
    newPos.yaw = uav_get_yaw(uav);
    newPos.roll = uav_get_roll(uav);
    newPos.pitch = uav_get_pitch(uav);

    uav_set_position(uav, newPos);

    if (fabs((uav->pos.x - goal.x)) < TARGET_PRECISION
        && fabs((uav->pos.y - goal.y)) < TARGET_PRECISION) {
        uav->target_reached = 1;
        #ifdef DEBUG 
        logs("Target Reached!");
        #endif /* ifdef DEBUG */
    }

    if (uav->target_reached) {
        uav->yaw_disturbance = 0.0;
        uav->pitch_disturbance = 0.0;
        goal.z = 0;
    }
    else if (newPos.z > (goal.z - 1) && time - uav->t > 0.01) {
        move_to_goal(uav, goal); 
        uav->t = wb_robot_get_time();
    }

    uav_actuate_motors(uav, 0.0, uav->pitch_disturbance, uav->yaw_disturbance, goal.z);
}
