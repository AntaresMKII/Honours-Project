#include "includes/net.h"
#include "../includes/uav.h"
#include <stdio.h>
#include <stdlib.h>

void net_elect_leader(Uav *uav, int timestep) {
   const MHead m = { uav->id, 0 };
    MHead r_m;
    int n;
    unsigned char max_id = uav->id;

    uav->f_id_num = 0;
    uav->f_id = (unsigned char*) malloc(1);
    if (uav->f_id == NULL) {
        printf("Failure to allocate memory in net_elect_leader!\n");
        exit(EXIT_FAILURE);
    }

    uav_send_msg(uav, m);

    uav_wait(timestep, 1.0f);

    n = uav_get_msg_num(uav);

    while (n > 0) {
        r_m = uav_receive_msg(uav, &n);
        if (n != 0) {
            if (r_m.r_id == 0 || r_m.r_id == uav->id) {
               max_id = (max_id > r_m.s_id) ? max_id : r_m.s_id;
                uav->f_id_num++;
                uav->f_id = realloc(uav->f_id, uav->f_id_num);
                if (uav->f_id == NULL) {
                    printf("Failure to allocate memory in net_elect_leader!\n");
                    exit(EXIT_FAILURE);
                }
                uav->f_id[uav->f_id_num - 1] = r_m.s_id;
            }
        }
    }

    uav->l_id = max_id;
}
