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

// Constants
#define ROLL_P 50.0
#define PITCH_P 30.0
#define VERTICAL_P 3.0
#define VERTICAL_T 68.5
#define VERTICAL_O 0.6

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

void uav_actuate_motors(Uav* uav, double roll, double pitch, double yaw, double altitude);

double uav_get_heading(Uav* uav);

void cm_move_to_goal(Uav* uav, double* goalCoord, double altitude);
