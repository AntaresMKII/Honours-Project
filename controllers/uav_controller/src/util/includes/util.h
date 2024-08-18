/** 
 * \file util.h
 *
 * A simple utility library
 */
#ifndef UTIL_H
#define UTIL_H

void init_debug_file();     ///< Initializes the logging library
void cleanup_debug_file();  ///< Cleans up the logging library

// Log functions
void logvi(int val, char *name);                            ///< Write to the log file one integer value
void log2vi(int val1, char *name1, int val2, char *name2);  ///< Write to the log file two integer values

void logvf(double val, char *name);                                 ///< Write to the log file one float value
void log2vf(double val1, char *name1, double val2, char *name2);    ///< Write to the log file two float values

void logs(char *str);   ///< Write a string to the log file

double to_rad(double alpha);    ///< Converts an angle in degrees into radians
double to_deg(double alpha);    ///< Converts an angle in radians into degrees

unsigned char dtouc(double d);  ///< Convert a double value to unsigned character (deprecated)

void print_csv(double** data, int n);

#endif // !UTIL_H
