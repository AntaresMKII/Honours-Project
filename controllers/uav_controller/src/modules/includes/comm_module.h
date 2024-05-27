/*
 * comm_module.h
 * Author: Yannick Abouem
 * Date: 2024-03-11
 * Description: Header file of comm_module.c
*/

#ifndef COMM_MODULE_H
#define COMM_MODULE_H

#include <stdlib.h>
#include <math.h>

struct comm_module {
    unsigned char id;
};

// comm_gen_id randomly generates an id and stores it in the comm_module structure
void comm_gen_id(struct comm_module *module, unsigned int seed);

#endif // !COMM_MODULE_H
