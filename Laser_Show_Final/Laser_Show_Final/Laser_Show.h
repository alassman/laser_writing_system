//
//  Laser_Show.h
//  Laser_Show_Final
//
//  Created by Adam Lassman on 5/4/16.
//  Copyright © 2016 Adam Lassman. All rights reserved.
//

#ifndef Laser_Show_h
#define Laser_Show_h
#include "Arduino.h"

class Laser_Show {
private:
    int laser_pin;
    int x_speaker_pin;
    int y_speaker_pin;
    
    
    float x_level; //figure these two out
    float y_level;
    
    float move_scale;
    float exp_move;
    float min_move;
    float max_move;
    float laser_toggle_delay;
    float line_delay;
    float x_min_value;
    float y_min_value;
    float x_max_value;
    float y_max_value;
public:
    Laser_Show(int laser_pin_in, int x_in, int y_in);
    
    void marquee(String text, int characters);
    
    
    void move (float x_movedir, float y_movedir);
    
    void sendto (float x_target, float y_target);
    
    void draw_string(String text, int x, int y, int count);
    
    void draw_letter(byte letter);
    
    void setPwmFrequency(int pin, int divisor);
    
    void laser_on();
    void laser_off();
    void hold(long length);
    void do_pwm();
    
};

#endif /* Laser_Show_h */
