#ifndef NET_H
#define NET_H

#define MH_SIZE 3
#define MD_SIZE sizeof(double) * 2
#define M_SIZE MH_SIZE + MD_SIZE + 5 // 5 bytes of padding after MHead

enum MType {
    ID,
    POS,
    WP,
    NEXT_WP
};

typedef union {
    struct {
        unsigned char s_id;
        unsigned char r_id;
        unsigned char type;
    };
    unsigned char bytes[MH_SIZE];
} MHead;

typedef union {
    struct {
        double x;
        double y;
    };
    int wp_num;
    unsigned char bytes[MD_SIZE];
} MData;

typedef union {
    struct {
        MHead head;
        unsigned char padding[5];
        MData data;
    };
    unsigned char bytes[M_SIZE];
} Message;

#endif // !NET_H
