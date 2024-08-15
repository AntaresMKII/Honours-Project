#include "includes/uav.h"
#include "modules/includes/net.h"
#include <stdlib.h>

#define CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

/* Initialization of the UAV */
void uav_init(Uav* uav, int timestep){

    uav->state = INIT;

    // Initialize devices
    uav->camera     = wb_robot_get_device("camera");            wb_camera_enable(uav->camera, timestep);
    uav->imu        = wb_robot_get_device("inertial unit");     wb_inertial_unit_enable(uav->imu, timestep);
    uav->gps        = wb_robot_get_device("gps");               wb_gps_enable(uav->gps, timestep);
    uav->compass    = wb_robot_get_device("compass");           wb_compass_enable(uav->compass, timestep);
    uav->gyro       = wb_robot_get_device("gyro");              wb_gyro_enable(uav->gyro, timestep);

    // Initialize leds
    uav->front_left_led = wb_robot_get_device("front left led");
    uav->front_right_led = wb_robot_get_device("front right led");

    // Initalize camera motors
    uav->camera_pitch_motor = wb_robot_get_device("camera pitch");
    uav->camera_roll_motor = wb_robot_get_device("camera roll");

    // Initialize emitter and receiver
    uav->emitter = wb_robot_get_device("emitter");
    uav->receiver = wb_robot_get_device("receiver");
    wb_receiver_enable(uav->receiver, timestep);

    uav->radar= wb_robot_get_device("radar");
    wb_radar_enable(uav->radar, timestep);

    // Get motors of the porpellers
    WbDeviceTag front_left_motor = wb_robot_get_device("front left propeller");
    WbDeviceTag front_right_motor = wb_robot_get_device("front right propeller");
    WbDeviceTag rear_left_motor = wb_robot_get_device("rear left propeller");
    WbDeviceTag rear_right_motor = wb_robot_get_device("rear right propeller");

    uav->motors[0] = front_left_motor;
    uav->motors[1] = front_right_motor;
    uav->motors[2] = rear_left_motor;
    uav->motors[3] = rear_right_motor;

    // Initialize other variables

    uav->t = wb_robot_get_time();

    uav->pitch_disturbance = 0.0;
    uav->yaw_disturbance = 0.0;

    uav->target_reached = 0;

    // initialize UAV position
    uav->pos.x = wb_gps_get_values(uav->gps)[0];
    uav->pos.y = wb_gps_get_values(uav->gps)[1];
    uav->pos.z = wb_gps_get_values(uav->gps)[2];

    // Set motors to velocity mode
    for(int i = 0; i < 4; i++){
        wb_motor_set_position(uav->motors[i], INFINITY);
        wb_motor_set_velocity(uav->motors[i], 1.0);
    }
}

/* Get UAV roll */
double uav_get_roll(Uav* uav){
    return wb_inertial_unit_get_roll_pitch_yaw(uav->imu)[0];
}

/* Get UAV pitch */
double uav_get_pitch(Uav* uav){
    return wb_inertial_unit_get_roll_pitch_yaw(uav->imu)[1];
}

/* Get UAV yaw */
double uav_get_yaw(Uav* uav){
    return wb_inertial_unit_get_roll_pitch_yaw(uav->imu)[2];
}
/* Get UAV gps altitude */
double uav_get_gps_altitude(Uav* uav){
    return wb_gps_get_values(uav->gps)[2];
}

/* Get UAV GPS coordinates */
const double* uav_get_gps_pos(Uav* uav){
    return wb_gps_get_values(uav->gps);
}

/* Get UAV roll velocity */
double uav_get_roll_velocity(Uav* uav){
    return wb_gyro_get_values(uav->gyro)[0];
}

/* Get UAV pitch velocity */
double uav_get_pitch_velocity(Uav* uav){
    return wb_gyro_get_values(uav->gyro)[1];
}

/* Get UAV yaw velocity */
double uav_get_yaw_velocity(Uav* uav){
    return wb_gyro_get_values(uav->gyro)[2];
}

/* Compute roll input */
double uav_compute_roll_input(Uav* uav, double roll_disturbance){
    const double c_roll_p = ROLL_P;
    const double c_roll = CLAMP(uav_get_roll(uav), -1.0, 1.0);
    const double c_roll_velocity = uav_get_roll_velocity(uav);

    return c_roll_p * c_roll + c_roll_velocity + roll_disturbance;
}

/* Compute pitch input */
double uav_compute_pitch_input(Uav* uav, double pitch_disturbance){
    const double c_pitch_p = PITCH_P;
    const double c_pitch = CLAMP(uav_get_pitch(uav), -1.0, 1.0);
    const double c_pitch_velocity = uav_get_pitch_velocity(uav);

    return c_pitch_p * c_pitch + c_pitch_velocity + pitch_disturbance;
}

/* Compute yaw input */
double uav_compute_yaw_input(Uav* uav, double yaw_disturbance){
    return yaw_disturbance;
}

/* Compute vertical input */
double uav_compute_vertical_input(Uav* uav, double target_altitude){
    const double c_altitude = uav_get_gps_altitude(uav);
    const double c_vertical_offset = VERTICAL_O;
    const double c_vertical_p = VERTICAL_P;

    const double c_clamped_diff_alt = CLAMP(target_altitude - c_altitude + c_vertical_offset, -1.0, 1.0);

    return c_vertical_p * pow(c_clamped_diff_alt, 3.0);
}

/* Motor actuation */
void uav_actuate_motors(Uav* uav, double roll, double pitch, double yaw, double altitude){
    const double roll_input = uav_compute_roll_input(uav, roll);
    const double pitch_input = uav_compute_pitch_input(uav, pitch);
    const double yaw_input = uav_compute_yaw_input(uav, yaw);
    const double vertical_input = uav_compute_vertical_input(uav, altitude);
    const double c_vertical_thrust = VERTICAL_T;

    const double front_left_motor_input = c_vertical_thrust + vertical_input - roll_input + pitch_input - yaw_input;
    const double front_right_motor_input = c_vertical_thrust + vertical_input + roll_input + pitch_input + yaw_input;
    const double rear_left_motor_input = c_vertical_thrust + vertical_input - roll_input - pitch_input + yaw_input;
    const double rear_right_motor_input = c_vertical_thrust + vertical_input + roll_input - pitch_input - yaw_input;

    wb_motor_set_velocity(uav->motors[0], front_left_motor_input);
    wb_motor_set_velocity(uav->motors[1], -1 * front_right_motor_input);
    wb_motor_set_velocity(uav->motors[2], -1 * rear_left_motor_input);
    wb_motor_set_velocity(uav->motors[3], rear_right_motor_input);
}

// Get the current heading of the UAV
double uav_get_heading(Uav* uav){
    const double *north = wb_compass_get_values(uav->compass);
    double rad = atan2(north[1], north[0]);
    double heading = (rad / M_PI) * 180.0;
    if (heading < 0.0)
        heading = heading + 360.0;
    return heading;
}

// Return the number of targets detected by the radar
int uav_get_radar_targets_num(Uav *uav) {
    return wb_radar_get_number_of_targets(uav->radar);
}

// Return the array of detected targets by the radar
const WbRadarTarget* uav_get_radar_targets(Uav* uav) {
    return wb_radar_get_targets(uav->radar);
}

// Returns both the number and the array of obstacles identified by the radar
int uav_get_obstacles(Uav* uav, WbRadarTarget* targets) {
    targets = uav_get_radar_targets(uav);
    return uav_get_radar_targets_num(uav);
}

// Kind of pointless to use these functions
// Get position of UAV
Position uav_get_position(Uav* uav) {
    return uav->pos;
}

// Set UAV position
void uav_set_position(Uav* uav, Position position) {
    uav->pos = position;
}

// send a message to other uavs
void uav_send_msg(Uav *uav, const Message m) {
    int status;
    do {
        status = wb_emitter_send(uav->emitter, (void*) m.bytes, M_SIZE);
    }while (!status);
}

// receives messages from other uavs
Message uav_receive_msg(Uav *uav, int *queue_len) {
    Message m;
    unsigned char *c;
    *queue_len = wb_receiver_get_queue_length(uav->receiver);

    if (*queue_len > 0) {
        c = (unsigned char*) wb_receiver_get_data(uav->receiver);
        for (int i = 0; i < M_SIZE; i++) {
            m.bytes[i] = c[i];
        }
        wb_receiver_next_packet(uav->receiver);
        *queue_len--;
    }

    return m;
}

// returns the number of messages
int uav_get_msg_num(Uav *uav) {
   return wb_receiver_get_queue_length(uav->receiver); 
}

// wait for x seconds
void uav_wait(int timestep, double x) {
    int t = wb_robot_get_time();
    while (wb_robot_step(timestep) != -1) {
        if (wb_robot_get_time() - t > x)
        break;
    }
}

// peek at next msg
int uav_peek_msg(Uav *uav) {
    Message m;
    unsigned char *c;

    int q_len = wb_receiver_get_queue_length(uav->receiver);

    if (q_len > 0) {
        c = (unsigned char*) wb_receiver_get_data(uav->receiver);
        for (int i = 0; i < M_SIZE; i++) {
            m.bytes[i] = c[i];
        }
        if (m.head.r_id == uav->id) {
            return 1; 
        }
        else {
            wb_receiver_next_packet(uav->receiver);
            return 0;
        }
    }
}

// clean up fnction
void uav_cleanup(Uav *uav) {
    fds_cleanup(uav->fds);
    free(uav->fds);
    free(uav->followers);
}
