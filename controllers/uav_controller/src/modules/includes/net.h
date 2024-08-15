/**
 * \file net.h
 *
 * This is the header file for most networking functionalities.
 * It contains the message structure.
 */
#ifndef NET_H
#define NET_H

#define MH_SIZE 3                       ///< The size of the head of the message in bytes
#define MD_SIZE sizeof(double) * 2      ///< The size of the data of the message in bytes
#define M_SIZE MH_SIZE + MD_SIZE + 5    ///< The full size of a message with 5 bytes of padding after the head

/**
 * The enum of the message type.
 * Possible values are ID, POS, WP and NEXT_WP
 */
enum MType {
    ID,     ///< Used to send ids
    POS,    ///< Used to share position
    WP,     ///< Used for sending and receiving waypoints
    NEXT_WP ///< Used to request next waypoint
};

/**
 * The header of the message
 */
typedef union {
    struct {
        unsigned char s_id; ///< The sender's id
        unsigned char r_id; ///< The receiver's id
        unsigned char type; ///< The type of message. See MType enum
    };
    unsigned char bytes[MH_SIZE];
} MHead;

/**
 * The data part of the message.
 * It accepst either a set of coordinates or an
 * integer as the waypoint number
 */
typedef union {
    struct {
        double x;
        double y;
    };
    int wp_num;
    unsigned char bytes[MD_SIZE];
} MData;

/**
 * The full message union
 */
typedef union {
    struct {
        MHead head;
        unsigned char padding[5];
        MData data;
    };
    unsigned char bytes[M_SIZE];
} Message;

#endif // !NET_H
