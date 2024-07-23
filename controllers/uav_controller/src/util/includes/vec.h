/** \file vec.h
* A simple vector library
*
* Author: Yannick Abouem
*/

#ifndef VEC_H
#define VEC_H

#include <math.h>

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

static inline int vec_equal(Vec3d v, Vec3d u) {
    return v.x == u.x && v.y == u.y;
}

static inline Vec3d vec_rotate(Vec3d v, double alpha) {
    double xp = v.x * cos(alpha) - v.y * sin(alpha);
    double yp = v.x * sin(alpha) + v.y * cos(alpha);
    Vec3d p = { xp, yp, 0 };
    return p;
}

static inline Vec3d vec_translate(Vec3d v, Vec3d u) {
    Vec3d pf = { v.x + u.x, v.y + u.y, v.z + u.z };
    return pf;
}

#endif // !VEC_H
