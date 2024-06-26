/*
 * control_module.c
 * Author: Yannick Abouem
 * Date: 2024-02-05
 * Description:
 * Module that control uav movements
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/uav.h"
#include "includes/fds.h"

//#define DEBUG

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
int move_to_waypoint(Uav *uav, Position wp) {

    double angle = compute_angle(uav, wp);

    uav->yaw_disturbance = MAX_YAW_DIST * angle / (2 * M_PI);
    uav->pitch_disturbance = compute_pitch_disturbance(angle);

    #ifdef DEBUG
    double distance = sqrt(pow((wp.x - uav->pos.x),2) + pow((wp.y - uav->pos.y), 2));
    log2vf(angle, "Angle left", distance, "Distance left");
    #endif /* ifdef DEBUG */

    return 0;
}

Vec3d obstacle_relative_pos(double obs_dist, double obs_azimuth) {
    double alpha = obs_azimuth;
    double xo = cos(alpha) * obs_dist;
    double yo = sin(alpha) * obs_dist;

    if (obs_azimuth > 0) {
        yo = yo * (-1.0f);
    }

    Vec3d p = {xo, yo, 0};

    return p;
}

Vec3d* cm_detect_obstacles(Uav *uav, int *num) {
    int target_num;
    WbRadarTarget *targets;
    double heading;
    const double *gps_pos;
    Vec3d uav_pos = { 0 };

    target_num = uav_get_radar_targets_num(uav);
    targets = uav_get_radar_targets(uav);
    heading = uav_get_heading(uav);
    heading = to_rad(heading);
    gps_pos = uav_get_gps_pos(uav);

    uav_pos.x = gps_pos[0];
    uav_pos.y = gps_pos[1];

    heading = (2*M_PI) - heading;

    Vec3d *targets_pos = (Vec3d *) malloc(sizeof(Vec3d) * target_num);
    if (targets_pos == NULL) {
        printf("Failed to allocate memeory in cm_detect_obstacle!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < target_num; i++) {
        targets_pos[i] = obstacle_relative_pos(targets[i].distance, targets[i].azimuth);
        targets_pos[i] = vec_rotate(targets_pos[i], heading);
        targets_pos[i] = vec_translate(uav_pos, targets_pos[i]);

        #ifdef DEBUG
        logs("Obstacle detected at:");
        log2vf(targets_pos[i].x, "x:", targets_pos[i].y, "y:");
        logvi(i, "Target ID:");
        #endif /* ifdef DEBUG */
    }

    *num = target_num;
    return targets_pos;
}

void cm_plan_path(Uav *uav) {
    Vec3d *obs_arr;
    int obs_num = 0;
    int num_cells = 0;
    Cell **mod_cells, **curr_cells;

    int mod_cells_num = 0;

    obs_arr = cm_detect_obstacles(uav, &obs_num);

    mod_cells = (Cell**) malloc(sizeof(Cell*) * obs_num * 4);
    if (mod_cells == NULL) {
        printf("Failed to allocate memory in cm_plan_path\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < obs_num; i++) {
        curr_cells = map_set_cells_cost(uav->fds->m, obs_arr[i], 1.0f, &num_cells);

        for (int j =0; j < num_cells; j++) {
            mod_cells[mod_cells_num] = curr_cells[j];
            mod_cells_num++;
        }
    }

    fds_run(uav->fds, mod_cells, mod_cells_num);

}

// update variables in the UAV struct, compute the disturbance and actuate motors
void cm_run(Uav *uav, Position wp, double time) {

    Position newPos;

    const double *gpsPos = uav_get_gps_pos(uav);

    newPos.x = gpsPos[0];
    newPos.y = gpsPos[1];

    newPos.z = uav_get_gps_altitude(uav);
    newPos.yaw = uav_get_yaw(uav);
    newPos.roll = uav_get_roll(uav);
    newPos.pitch = uav_get_pitch(uav);

    uav_set_position(uav, newPos);

    if (fabs((uav->pos.x - wp.x)) < TARGET_PRECISION
        && fabs((uav->pos.y - wp.y)) < TARGET_PRECISION) {
        uav->target_reached = 1;
        #ifdef DEBUG 
        logs("Target Reached!");
        #endif /* ifdef DEBUG */
    }

    if (newPos.z > (wp.z - 1) && time - uav->t > 0.01 && !uav->target_reached) {
        move_to_waypoint(uav, wp); 
        uav->t = wb_robot_get_time();
    }

    uav_actuate_motors(uav, 0.0, uav->pitch_disturbance, uav->yaw_disturbance, wp.z);
}
