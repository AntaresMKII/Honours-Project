/*
* util.c
* Author: Yannick Abouem
* Date: 12/02/2024
*/

double clamp(double val, double min, double max) {
    if (val < min) {
        return min;
    }
    else if (val > max) {
        return max;
    }

    return val;
}
