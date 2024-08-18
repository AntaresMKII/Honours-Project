/** 
 * \file: uav.h
 *
 * This file contains most strucutres and function definition
 * for the uav and the command module
*/

#ifndef UAV_H
#define UAV_H

/// libc includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/// webots robot includes
#include <webots/robot.h>

/// webots devices includes
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

/// local header files
#include "../util/includes/util.h"
#include "../modules/includes/fds.h"
#include "../modules/includes/net.h"

// Uncomment for debug functionality
//#define DEBUG

// Constants
#define ROLL_P 50.0f             ///< Roll costant for PID
#define PITCH_P 30.0f            ///< Pitch constant for PID
#define VERTICAL_P 3.0f          ///< Vertical constant for PID
#define VERTICAL_T 68.5f         ///< Vertical constant for PID
#define VERTICAL_O 0.6f          ///< Vertical constant for PID
#define TARGET_PRECISION 0.5f    ///< This constant determines how close the UAV must be to the waypoint for it to be reaced
#define MAX_YAW_DIST 0.4f        ///< Maximum Yaw Displacement
#define MAX_PITCH_DIST -1        ///< Maximum Pitch Displacement

/// Used for storing position and attitude informations
typedef struct Position {
    double x; 
    double y;
    double z;
    double pitch;
    double roll;
    double yaw;
} Position;

/// The Possible states of the UAV
enum Uav_State {
    INIT,   ///< Set the UAV to intialization phase
    RUN,    ///< Set the UAV to run phase and declares the UAV as leader
    END,    ///< Set the UAV to cleanup phase and terminate the program
    F_RUN,  ///< Set the UAV to run pahse as a follower
    F_WAIT, ///< Set the UAV to wait phase and wait for leader's instructions
};

/// The Follower structure
typedef struct {
    unsigned char id;   ///< The id of the follower
    Vec3d pos;          ///< The initial position of the follower
    int wp_num;         ///< The number of the waypoint the follower is currently following
    Vec3d *wps;         ///< List of waypoints computed for the follower
} Follower;

/// The UAV structure containing the UAV data.
/** This data structure contains all device tags
* to interface with the Webots robot. It also
* contains other varaibles needed in multiple functions
*/
typedef struct uav {
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

    /* Path planning */
    Fds *fds;                       ///< Pointer to the field D* global variables
    
    /* State */
    char state;                     ///< The current state the UAV is in. Used to control the program

    /* Networking */
    unsigned char id;               ///< The id of the UAV. Used for inter-UAV connection
    unsigned char l_id;             ///< The id of the elected leader. If id == l_id then UAV is leader

    Follower *followers;            ///< An array of all detected followers
    int f_num;                      ///< The number of followers in the array
} Uav;

/// UAV initialization function
void uav_init(Uav* uav, int timestep);

/* Getters */
double uav_get_roll(Uav* uav);                                                              ///< Returns the current roll
double uav_get_pitch(Uav* uav);                                                             ///< Returns the current pitch
double uav_get_yaw(Uav* uav);                                                               ///< Returns the current yaw

double uav_get_gps_altitude(Uav* uav);                                                      ///< Returns the current registered altitude
const double* uav_get_gps_pos(Uav* uav);                                                    ///< Returns the current gps position

double uav_get_roll_velocity(Uav* uav);                                                     ///< Returns the roll velocity
double uav_get_pitch_velocity(Uav* uav);                                                    ///< Returns the pitch velocity
double uav_get_yaw_velocity(Uav* uav);                                                      ///< Returns the yaw velocity

double uav_get_heading(Uav* uav);                                                           ///< Returns the current heading in degrees

int uav_get_radar_targets_num(Uav* uav);                                                    ///< Returns the number of trargets
const WbRadarTarget* uav_get_radar_targets(Uav* uav);                                       ///< Returns an array of tragets
int uav_get_obstacles(Uav* uav, WbRadarTarget* targets);                                    ///< Given an empty array of targets, this function will return the numebr of targets and the targets themselves

/* Setters */
void uav_set_position(Uav* uav, Position position);                                         ///< Sets the position of the UAV

/* Other methods */
void uav_actuate_motors(Uav* uav, double roll, double pitch, double yaw, double altitude);  ///< Actuates the motors of the UAV
void uav_wait(int timestep, double x);                                                      ///< Waits for x seconds
void uav_cleanup(Uav *uav);                                                                 ///< Cleanup function

/* Message passing functons */
void uav_send_msg(Uav *uav, const Message m);       ///< Send a message m
Message uav_receive_msg(Uav *uav, int *queue_len);  ///< Receive a message and get the number of messages in the queue
int uav_get_msg_num(Uav *uav);                      ///< Returns the number of messages currently on the queue
int uav_peek_msg(Uav *uav);                         ///< Peek at next message but do not remove from queue

/* Control Module Functions */
int cm_run(Uav *uav, Vec3d wp, double target_alt, double time); ///< Runs the automated movement of the uav
Vec3d* cm_detect_obstacles(Uav *uav, int *num);                 ///< Detects the obstacles and returns the number and the position of the obstacles
Vec3d* cm_plan_path(Uav *uav, int *wps_num);                    ///< Plans the path using Filed D*
void cm_followers_path(Uav *uav, Vec3d *wps, int wps_num);      ///< Plans the path of the followers

/* Network function */
void net_elect_leader(Uav *uav, int timestep);                  ///< Leader election function
void net_share_init_pos(Uav *uav, int timestep);                ///< Sends inital position to leader. If the calling UAV is the leader then it will receive the followers psoitions
void net_send_wp(Uav *uav, int curr_wp);                        ///< Send the current waypoint to the follower
void net_recieve_wp(Uav *uav, Vec3d *wp);                       ///< Receive waypoint from leader
void net_ask_next_wp(Uav *uav, int curr_wp);                    ///< Ask next waypoint from leader (unused)

/* Field D* functions */
void fds_cleanup(Fds *fds); ///< Cleanup fds structure

#endif // !UAV_H
