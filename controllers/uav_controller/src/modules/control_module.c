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
#include "../util/includes/util.h"

//#define DEBUG

#define CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

// Compute the angle in radians between the goal and the uav in [-pi, pi]
double compute_angle(Uav *uav, Vec3d goal) {
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
int move_to_waypoint(Uav *uav, Vec3d wp) {

    double angle = compute_angle(uav, wp);

    uav->yaw_disturbance = MAX_YAW_DIST * angle / (2 * M_PI);
    uav->pitch_disturbance = compute_pitch_disturbance(angle);

    #ifdef DEBUG
    double distance = sqrt(pow((wp.x - uav->pos.x),2) + pow((wp.y - uav->pos.y), 2));
    log2vf(angle, "Angle left", distance, "Distance left");
    #endif /* ifdef DEBUG */

    return 0;
}

Vec3d compute_target_pos(double alpha, double theta, double v_mag) {
    Vec3d v;
    double beta = alpha - theta;

    theta = theta < 0 ? theta * -1 : theta;

    v.x = v_mag * cos(theta) / (cos(alpha) + sin(alpha) * tan(beta));
    v.y = v.x * tan(beta);

    return v;
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

    Vec3d *targets_pos = (Vec3d *) malloc(sizeof(Vec3d) * target_num);
    if (targets_pos == NULL) {
        printf("Failed to allocate memeory in cm_detect_obstacle!\n");
        exit(EXIT_FAILURE);
    }

    heading -= 0.5 * M_PI;
    heading *=  -1;

    for (int i = 0; i < target_num; i++) {
        targets_pos[i] = compute_target_pos(heading, targets[i].azimuth, targets[i].distance);
        targets_pos[i].x += uav_pos.x;
        targets_pos[i].y += uav_pos.y;
        #ifdef DEBUG
        logs("---=== Obstacle Detected! ===---");
        log2vf(targets[i].azimuth, "Alpha: ", targets[i].distance, "Distance: ");
        log2vf(heading, "Beta: ", heading - targets[i].azimuth, "Gamma: ");
        log2vf(targets_pos[i].x, "Vx: ", targets_pos[i].y, "Vy: ");
        #endif /* ifdef DEBUG
                logs("---=== Obstacle Detected! ===---");
         */
    }

    *num = target_num;
    return targets_pos;
}

void cm_followers_path(Uav *uav, Vec3d *wps, int wps_num) {
    Vec3d prev_wp = uav->fds->start->v;
    double angle = 0;
    Cell **cells;
    int num_cells = 0;

    for (int i = 0; i < uav->f_num; i++) {
        uav->followers[i].wps = (Vec3d*) malloc(sizeof(Vec3d) * wps_num);
        if (uav->followers[i].wps == NULL) {
            printf("Failed to allocate memory in cm_followers_path()\n");
        }

        prev_wp = uav->fds->start->v;

        for (int j = 0; j < wps_num; j++) {

            angle = acos((wps[j].x - prev_wp.x) / (sqrt(pow(wps[j].x - prev_wp.x, 2) + pow(wps[j].y - prev_wp.y, 2))));

            uav->followers[i].wps[j] = vec_rotate(uav->followers[i].pos, angle);
            uav->followers[i].wps[j] = vec_translate(uav->followers[i].wps[j], wps[j]);
            prev_wp = wps[j];
        }
    }
}

Vec3d* cm_plan_path(Uav *uav, int *wps_num) {
    const double *gps_pos;

    Vec3d *obs_arr;
    Vec3d *wps;
    int obs_num = 0;
    int num_cells = 0;
    Cell **mod_cells, **curr_cells;

    int mod_cells_num = 0;

    obs_arr = cm_detect_obstacles(uav, &obs_num);

    mod_cells = (Cell**) malloc(sizeof(Cell*));
    if (mod_cells == NULL) {
        printf("Failed to allocate memory in cm_plan_path\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < obs_num; i++) {
        curr_cells = map_set_cells_cost(uav->fds->m, obs_arr[i], 4, &num_cells);

        for (int j = 0; j < num_cells; j++) {
            mod_cells_num++;
            mod_cells = (Cell**) realloc(mod_cells, sizeof(Cell*) * mod_cells_num);
            if (mod_cells == NULL) {
                printf("Failed to reallocate memory in cm_plan_path\n");
                exit(EXIT_FAILURE);
            }
            mod_cells[mod_cells_num - 1] = curr_cells[j];
        }
        free(curr_cells);
    }

    fds_run(uav->fds, mod_cells, mod_cells_num);

    wps = fds_extract_path(uav->fds, wps_num);

    free(mod_cells);
    free(obs_arr);

    return wps;
}

// update variables in the UAV struct, compute the disturbance and actuate motors
int cm_run(Uav *uav, Vec3d wp, double target_alt, double time) {

    Position newPos;

    int rc = 0;

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
        rc = 1;
    }

    if (newPos.z > (target_alt - 1) && time - uav->t > 0.01 && !uav->target_reached) {
        move_to_waypoint(uav, wp); 
        uav->t = wb_robot_get_time();
    }

    uav_actuate_motors(uav, 0.0, uav->pitch_disturbance, uav->yaw_disturbance, target_alt);

    return rc;
}
