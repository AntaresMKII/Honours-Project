/*
* vector.c
* Author: Yannick Abouem
* Date: 10/02/2024
* Description:
* A simple vector library
*/

#include "vector.h"

// Take two double arrays as vector and compute the dot product
// Returns an angle
double dot(double *u, double *v) {
    return (u[0] * v[0]) + (u[1] * v[1]);
}

// Returns the magnitude of a vector
double magnitude(double *u) {
    return sqrt(pow(u[0],2) + pow(u[1],2));
}

// Returns the distance between two vectors
double distance(double *u, double *v) {
    const double x = pow(u[0] - v[0],2);
    const double y = pow(u[1] - v[1],2);

    return sqrt(x + y);
}

// returns an angle betweent the two vertices
double angle_between(double *u, double *v) {
    const double d = dot(u,v);
    const double magU = magnitude(u);
    const double magV = magnitude(v);

    return acos(d / (magU * magV));
}
