#include "includes/net.h"
#include "../includes/uav.h"
#include <stdio.h>

void net_elect_leader(Uav *uav, int timestep) {
   const MHead m = { uav->id, 0 };
    MHead r_m;
    int n;

    uav_send_msg(uav, m);

    uav_wait(timestep, 1.0f);

    n = uav_get_msg_num(uav);

    while (n > 0) {
        r_m = uav_receive_msg(uav, &n);
        if (n != 0) {
            printf("%d\n", r_m.s_id);
        }
    }
}
