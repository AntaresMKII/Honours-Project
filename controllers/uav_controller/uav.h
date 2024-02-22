/*
 * File: uav.h
 * Author: Yannick Abouem
 * Date: 25/01/2024
 * Description: uav structure
*/

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

#include "util.h"

#define DEBUG

// Constants
#define ROLL_P 50.0
#define PITCH_P 30.0
#define VERTICAL_P 3.0
#define VERTICAL_T 68.5
#define VERTICAL_O 0.6
#define TARGET_PRECISION 0.5
#define MAX_YAW_DIST 0.4
#define MAX_PITCH_DIST -1

typedef struct Position {
    double x, y, z, pitch, roll, yaw;
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

    /* Variables */
    double target_alt; // Targeted altitude
    bool tagetReached;
    Position pos; // position + pitch, roll and yaw attitude
    
    double t; // Time

    double pitch_disturbance;
    double yaw_disturbance;

    int target_reached;
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

/* Setters */
void uav_set_position(Uav* uav, Position position);

/* Other methods */
void uav_actuate_motors(Uav* uav, double roll, double pitch, double yaw, double altitude);
void cm_run(Uav *uav, Position goal, double time);
