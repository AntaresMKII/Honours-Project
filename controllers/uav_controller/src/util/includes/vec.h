/** \file vec.h
* A simple vector library
*
* Author: Yannick Abouem
*/

#ifndef VEC_H
#define VEC_H

typedef union Vec3d {
    struct {
        double x;
        double y;
        double z;
    };

    struct {
        double pitch;
        double roll;
        double yaw;
    };
} Vec3d;

#endif // !VEC_H
