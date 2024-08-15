/** 
 * \file vec.h
 *
 * A simple vector library
 */

#ifndef VEC_H
#define VEC_H

#include <math.h>

/**
 * A three-dimensional vector
 */ 
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

/**
 * Inline function to find if two vectors are equal
 */ 
static inline int vec_equal(Vec3d v, Vec3d u) {
    return v.x == u.x && v.y == u.y;
}

/**
 * Inline function to rotate a 3D vector using a 2D rotation matrix
 */ 
static inline Vec3d vec_rotate(Vec3d v, double alpha) {
    double xp = v.x * cos(alpha) - v.y * sin(alpha);
    double yp = v.x * sin(alpha) + v.y * cos(alpha);
    Vec3d p = { xp, yp, 0 };
    return p;
}

/**
 * Inline function to translate a vector v by another vector u
 */ 
static inline Vec3d vec_translate(Vec3d v, Vec3d u) {
    Vec3d pf = { v.x + u.x, v.y + u.y, v.z + u.z };
    return pf;
}

#endif // !VEC_H
