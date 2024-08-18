#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <csv.h>

#include <webots/robot.h>

FILE *file_ptr;
FILE *wp_file;

void init_debug_file() {
    int time = wb_robot_get_time();
    file_ptr = fopen("uav_debug.txt", "w+");

    if (file_ptr == NULL) {
        printf("Unable to open debug file! Quitting controller!");
        exit(EXIT_FAILURE);
    }

    fprintf(file_ptr, "[%d] Controller started\n", time);
}

void logvi(int val, char *name) {
    int time = wb_robot_get_time();
    fprintf(file_ptr, "[%d] %s %d\n", time, name, val);
}

void log2vi(int val1, char *name1, int val2, char *name2) {
    int time = wb_robot_get_time();
    fprintf(file_ptr, "[%d] %s %d\t%s %d\n", time, name1, val1, name2, val2);
}

void logvf(double val, char *name) {
    int time = wb_robot_get_time();
    fprintf(file_ptr, "[%d] %s %f\n", time, name, val);
}

void log2vf(double val1, char *name1, double val2, char *name2) {
    int time = wb_robot_get_time();
    fprintf(file_ptr, "[%d] %s %f\t%s %f\n", time, name1, val1, name2, val2);
}

void logs(char *str) {
    int time = wb_robot_get_time();
    fprintf(file_ptr, "[%d] %s\n", time, str);
}

void cleanup_debug_file() {
    fclose(file_ptr);
}

double to_deg(double alpha) {
    return alpha * 180.0f / M_PI;
}

double to_rad(double alpha) {
    return alpha / 180.0f * M_PI;
}

void dtouc(double d, unsigned char* c) {
    union conv {
        double d;
        unsigned char c[8];
    };

    union conv conv;

    conv.d = d;

    for (int i = 0; i < 8; i++) {
        c[i] = conv.c[i];
    }
}

void print_csv(double** data, int n) {
    FILE* csv_file = fopen("wps.csv", "wb+");
    double infty = INFINITY;
    for (int i = 0; i < n; i++) {
        csv_fwrite(csv_file, &data[i][0], sizeof(double));
        fprintf(csv_file, ",");
        csv_fwrite(csv_file, &data[i][1], sizeof(double));
        fprintf(csv_file, "\n");
    }
    fclose(file_ptr);
}
