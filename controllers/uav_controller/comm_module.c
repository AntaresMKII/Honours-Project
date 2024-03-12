/*
* comm_module.c
* Author: Yannick Abouem
* Date: 2024-03-11
* Description: Communication and distributed algorithms
*/

#include "comm_module.h"

void comm_gen_id(struct comm_module *module, unsigned int seed) {
    srand(seed);
    module->id = (unsigned char) floor(((double) rand() / RAND_MAX) * 256);
}
