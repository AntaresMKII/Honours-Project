#include "includes/net.h"
#include "../includes/uav.h"
#include <stdio.h>
#include <stdlib.h>

void net_elect_leader(Uav *uav, int timestep) {
   Message m;
    Message r_m;
    int n;
    unsigned char max_id = uav->id; 

    m.head.s_id = uav->id;
    m.head.r_id = 0;
    m.head.type = ID;

    uav_send_msg(uav, m);

    uav_wait(timestep, 1.0f);

    n = uav_get_msg_num(uav);

    while (n > 0) {
        r_m = uav_receive_msg(uav, &n);
        if (n != 0) {
            if ((r_m.head.r_id == 0 || r_m.head.r_id == uav->id) && r_m.head.type == ID) {
               max_id = (max_id > r_m.head.s_id) ? max_id : r_m.head.s_id;
                uav->f_num++;
            }
        }
    }

    uav->l_id = max_id;
}

void net_share_init_pos(Uav *uav, int timestep) {
    if (uav->state == F_WAIT) {
        Vec3d init_pos = uav->fds->start->v;
        Message m;
        m.head.s_id = uav->id;
        m.head.r_id = uav->l_id;
        m.head.type = POS;
        m.data.x = init_pos.x;
        m.data.y = init_pos.y;

        uav_send_msg(uav, m);
    }
    else {
        Message m;
        int n, i = 0;

        uav->followers = (Follower*) malloc(sizeof(Follower) * uav->f_num);
        if (uav->followers == NULL) {
            printf("Failed to allocate memory in net_share_init_pos.\n");
            exit(EXIT_FAILURE);
        }

        uav_wait(timestep, 1.0f);
        
        n = uav_get_msg_num(uav);
        while (n > 0) {
            printf("Collecting followers position\n");
            m = uav_receive_msg(uav, &n);
            if (n != 0) {
                if (m.head.r_id == uav->id && m.head.type == POS) {
                    uav->followers[i].id = m.head.s_id;
                    uav->followers[i].pos.x = m.data.x;
                    uav->followers[i].pos.y = m.data.y;
                    i++;
                }
            }
        }
    }
}
