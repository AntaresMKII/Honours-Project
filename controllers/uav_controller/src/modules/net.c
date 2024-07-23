#include "includes/net.h"
#include "../includes/uav.h"
#include <math.h>
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

    uav_wait(timestep, 5.0f);

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
                    uav->followers[i].pos.x = m.data.x - uav->fds->start->v.x;
                    uav->followers[i].pos.y = m.data.y - uav->fds->start->v.y;

                    i++;
                }
            }
        }
    }
}

// Compute the wayponts for the followers relative to their initial positions
void net_send_wp(Uav *uav, Vec3d wp, int curr_wp) {
    Vec3d l_init_pos = uav->fds->start->v;
    Vec3d diff;
    //const double *curr_pos;
    //double tmp;

    for (int i = 0; i < uav->f_num; i++) {
        if (uav->followers[i].wp_num == curr_wp) {
            continue;
        }

        //curr_pos = uav_get_gps_pos(uav);

        Message m;
        m.head.s_id = uav->id;
        m.head.r_id = uav->followers[i].id;
        m.head.type = WP;
        m.data.x = uav->followers[i].wps[curr_wp].x;
        m.data.y = uav->followers[i].wps[curr_wp].y;

        uav->followers[i].wp_num = curr_wp;

        printf("%d : Wp sent (%f,%f) to %d\n", uav->id, m.data.x, m.data.y, m.head.r_id);

        uav_send_msg(uav, m);
    }
}

void net_recieve_wp(Uav *uav, Vec3d *wp) {
    int n;
    Message m;

    n = uav_get_msg_num(uav);

    while (n > 0) {
        m = uav_receive_msg(uav, &n);
        if (n != 0) {
            if (m.head.r_id == uav->id && m.head.type == WP) {
                uav->state = F_RUN;
                wp->x = m.data.x;
                wp->y = m.data.y;
                printf("%d : Wp received (%f,%f) from %d\n", uav->id, m.data.x, m.data.y, m.head.s_id);
            }
        }
    }
}
