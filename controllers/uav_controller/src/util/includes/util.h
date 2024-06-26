/** \file util.h
* A simple logging library
*
* Author: Yanncik Abouem
*/
#ifndef UTIL_H
#define UTIL_H

void init_debug_file();     ///< Initializes the logging library
void cleanup_debug_file();  ///< Cleans up the logging library

// Log functions
void logvi(int val, char *name);
void log2vi(int val1, char *name1, int val2, char *name2);

void logvf(double val, char *name);
void log2vf(double val1, char *name1, double val2, char *name2);
void logs(char *str);

double to_rad(double alpha);
double to_deg(double alpha);

#endif // !UTIL_H
