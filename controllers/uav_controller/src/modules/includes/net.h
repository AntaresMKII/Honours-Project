#ifndef NET_H
#define NET_H

#define M_SIZE 2

typedef union {
    struct {
        unsigned char s_id;
        unsigned char r_id;
    };
    unsigned char msg[M_SIZE];
} MHead;

#endif // !NET_H
