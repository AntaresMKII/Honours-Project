/** \file: uav.h
 * This file contains most strucutres and function definition
 * for the uav and the command module
 *
 * Author: Yannick Abouem
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
#include "../modules/includes/fds.h"

// Uncomment for debug functionality
//#define DEBUG

// Constants
#define ROLL_P 50.0

#define PITCH_P 30.0

#define VERTICAL_P 3.0

#define VERTICAL_T 68.5

#define VERTICAL_O 0.6

#define TARGET_PRECISION 0.5

#define MAX_YAW_DIST 0.4

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

/// The UAV structure containing the UAV data.
/** This data structure contains all device tags
* to interface with the Webots robot. It also
* contains other varaibles needed in multiple functions
*/
typedef struct uav
{
    /* Devices */
    WbDeviceTag camera;             ///< The camera device tag (unused)
    WbDeviceTag imu;                ///< The inertial reference unit device tag
    WbDeviceTag gps;                ///< The gps device tag
    WbDeviceTag compass;            ///< The compass device tag
    WbDeviceTag gyro;               ///< The gyro device tag
    WbDeviceTag emitter;            ///< The emitter device tag
    WbDeviceTag receiver;           ///< The receiver device tag
    WbDeviceTag radar;              ///< The radar device tag
    
    /* Led Lights */
    WbDeviceTag front_left_led;     ///< The front left led device tag (unused)
    WbDeviceTag front_right_led;    ///< The front right led device tag (unused)

    /* Motors */
    WbDeviceTag camera_roll_motor;  ///< The camera roll motor device tag (unused)
    WbDeviceTag camera_pitch_motor; ///< The camera pitch motor device tag (unused)

    WbDeviceTag motors[4];          ///< The quadcopter motors device tag

    /* Variables */
    double target_alt;              ///< Target altitude
    Position pos;                   ///< The UAV current position. It also stores the registered attitude
    
    double t;                       ///< Time mesured in ms

    double pitch_disturbance;       ///< Computed pitch disturbance
    double yaw_disturbance;         ///< Computed yaw disturbance

    int target_reached;             ///< Traged reached flag

    /* Communication */
    struct comm_module comm_mod;    ///< Communication module structure
    
    /* Path planning */
    Fds *fds; ///< Pointer to the field D* global variables
} Uav;

/// UAV initialization function
void uav_init(Uav* uav, int timestep);

/* Getters */
double uav_get_roll(Uav* uav);                                                              ///< Returns the current roll
double uav_get_pitch(Uav* uav);                                                             ///< Returns the current pitch
double uav_get_yaw(Uav* uav);                                                               ///< Returns the current yaw

double uav_get_gps_altitude(Uav* uav);                                                      ///< Returns the current registered altitude
double* uav_get_gps_pos(Uav* uav);                                                          ///< Returns the current gps position

double uav_get_roll_velocity(Uav* uav);                                                     ///< Returns the roll velocity
double uav_get_pitch_velocity(Uav* uav);                                                    ///< Returns the pitch velocity
double uav_get_yaw_velocity(Uav* uav);                                                      ///< Returns the yaw velocity

double uav_get_heading(Uav* uav);                                                           ///< Returns the current heading in degrees

int uav_get_radar_targets_num(Uav* uav);                                                    ///< Returns the number of trargets
WbRadarTarget* uav_get_radar_targets(Uav* uav);                                             ///< Returns an array of tragets
int uav_get_obstacles(Uav* uav, WbRadarTarget* targets);                                    ///< Given an empty array of targets, this function will return the numebr of targets and the targets themselves

/* Setters */
void uav_set_position(Uav* uav, Position position);                                         ///< Sets the position of the UAV

/* Other methods */
void uav_actuate_motors(Uav* uav, double roll, double pitch, double yaw, double altitude);  ///< Actuates the motors of the UAV
void cm_run(Uav *uav, Position goal, double time);                                          ///< Runs the automated movement of the uav
Position* cm_detect_obstacles(Uav *uav, int *num);                                          ///< Detects the obstacles and returns the number and the position of the obstacles

#endif // !UAV_H
