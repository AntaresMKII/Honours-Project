/*
 * File: uav.h
 * Author: Yannick Abouem
 * Date: 25/01/2024
 * Description: uav structure
*/

#ifndef UAV_H
#define UAV_H

// libc includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// webots robot includes
#include <webots/robot.h>

// webots devices includes
#include <webots/compass.h>
#include <webots/gps.h>
#include <webots/gyro.h>
#include <webots/inertial_unit.h>
#include <webots/keyboard.h>
#include <webots/led.h>
#include <webots/motor.h>
#include <webots/camera.h>
#include <webots/emitter.h>
#include <webots/receiver.h>
#include <webots/radar.h>

#include "../util/includes/util.h"
#include "../modules/includes/comm_module.h"

// Uncomment for debug functionality
//#define DEBUG

// Constants

/// \def Roll constant for PID
#define ROLL_P 50.0

/// \def Putch constant for PID
#define PITCH_P 30.0

/// \def Vertical constant for PID
#define VERTICAL_P 3.0

#define VERTICAL_T 68.5
#define VERTICAL_O 0.6

/// \def Precision for target navigation
#define TARGET_PRECISION 0.5

/// \def Maximum yaw allowed
#define MAX_YAW_DIST 0.4

/// \def Maximum pitch allowed
#define MAX_PITCH_DIST -1

/// Used for storing position and attitude informations
typedef struct Position {
    double x; 
    double y;
    double z;
    double pitch;
    double roll;
    double yaw;
} Position;

typedef struct uav
{
    /* Devices */
    WbDeviceTag camera,
        imu,
        gps,
        compass,
        gyro;
    
    /* Led Lights */
    WbDeviceTag front_left_led,
        front_right_led;

    /* Motors */
    WbDeviceTag camera_roll_motor,
        camera_pitch_motor;

    WbDeviceTag motors[4];

    WbDeviceTag emitter;
    WbDeviceTag receiver;

    WbDeviceTag radar;

    /* Variables */
    double target_alt; // Targeted altitude
    bool tagetReached;
    Position pos; // position + pitch, roll and yaw attitude
    
    double t; // Time

    double pitch_disturbance;
    double yaw_disturbance;

    int target_reached;

    // Communication
    struct comm_module comm_mod;
} Uav;

void uav_init(Uav* uav, int timestep);

/* Getters */
double uav_get_roll(Uav* uav);
double uav_get_pitch(Uav* uav);
double uav_get_yaw(Uav* uav);

double uav_get_gps_altitude(Uav* uav);
double* uav_get_gps_pos(Uav* uav);

double uav_get_roll_velocity(Uav* uav);
double uav_get_pitch_velocity(Uav* uav);
double uav_get_yaw_velocity(Uav* uav);

double uav_get_heading(Uav* uav);

int uav_get_radar_targets_num(Uav* uav);
WbRadarTarget* uav_get_radar_targets(Uav* uav);
int uav_get_obstacles(Uav* uav, WbRadarTarget* targets);

/* Setters */
void uav_set_position(Uav* uav, Position position);

/* Other methods */
void uav_actuate_motors(Uav* uav, double roll, double pitch, double yaw, double altitude);
void cm_run(Uav *uav, Position goal, double time);
Position* cm_detect_obstacles(Uav *uav, int *num);

#endif // !UAV_H
