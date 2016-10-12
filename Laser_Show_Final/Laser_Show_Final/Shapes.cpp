//
//  Shapes.cpp
//  Laser_Show_Final
//
//  Created by Adam Lassman on 5/4/16.
//  Copyright © 2016 Adam Lassman. All rights reserved.
//

#include "Shapes.h"

void draw_square() {
    laser_on();
    int wait = 1;
    int holdtime = 1;
    int corner;
    for(corner = 40; corner < 200; corner += 5) {
        for (int i = 0; i < wait; i++) {
            sendto(0,0);
            hold(holdtime);
            sendto(corner,0);
            hold(holdtime);
        }
        for (int i = 0; i < wait; ++i) {
            sendto(corner,0);
            hold(holdtime);
            sendto(corner,corner);
            hold(holdtime);
        }
        for (int i = 0; i < wait; i++) {
            sendto(corner,corner);
            hold(holdtime);
            sendto(0,corner);
            hold(holdtime);
        }
        for (int i = 0; i < wait; i++) {
            sendto(0,corner);
            hold(holdtime);
            sendto(0,0);
            hold(holdtime);
        }
    }
}

void draw_triangle() {
    laser_on();
    int wait = 1;
    int holdtime = 1;
    int corner = 10;
    sendto(1,1);
    for(corner = 10; corner < 150; corner += 2) {
        move(0,corner);
        move(corner/2,-corner/2);
        move(-corner/2, -corner/2);
    }
}

void draw_star() {
    for (float loop = 0; loop < 25; loop ++)
    {
        move_scale = loop / 5;
        laser_off();
        sendto (1,1);
        move (17,5);
        laser_on();
        move (13,24);
        move (-30,-18);
        move (34,0);
        move (-30,18);
        move (13,-24);
    }
    
    for (float loop = 25; loop > 0; loop --)
    {
        move_scale = loop / 5;
        laser_off();
        sendto (1,1);
        move (17,5);
        laser_on();
        move (13,24);
        move (-30,-18);
        move (34,0);
        move (-30,18);
        move (13,-24);
    }
}
