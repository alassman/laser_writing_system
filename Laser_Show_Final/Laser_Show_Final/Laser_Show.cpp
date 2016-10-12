//
//  Laser_Show.cpp
//  Laser_Show_Final
//
//  Created by Adam Lassman on 5/4/16.
//  Copyright © 2016 Adam Lassman. All rights reserved.
//

#include "Arduino.h"
#include <string>
#include "Laser_Show.h"
using namespace std;

Laser_Show::Laser_Show(int laser_pin_in, int x_in, int y_in)
:laser_pin(laser_pin_in), x_speaker_pin(x_in), y_speaker_pin(y_in), move_scale(2.5), exp_move(1.6), min_move(.5), max_move(20), laser_toggle_delay(11), line_delay(8), x_min_value(1), y_min_value(1), x_max_value(240), y_max_value(240) {
    pinMode(laser_pin, OUTPUT);
    pinMode(x_speaker_pin, OUTPUT);
    pinMode(y_speaker_pin, OUTPUT);
    
    //resets the default arduino PWM stuff to much higher speed
    setPwmFrequency(x_speaker_pin, 1);
    setPwmFrequency(y_speaker_pin, 1);
}


void Laser_Show::marquee(string text, int characters)
{
    
    float move_scale_holder;
    int i = 0;
    int loop = 0;
    
    move_scale_holder = move_scale;
    
    move_scale = 256 / (characters * 18);
    
    for (int x=0; x < characters + 2; x++)
    {
        text = String(" " + text + " ");
    }
    //text.length() == 30;
    for (loop = 0; loop < (text.length() - 5); loop ++)
    {
        
        draw_string(text.substring(loop,loop + characters),move_scale * 9,1,1);
        draw_string(text.substring(loop,loop + characters),1,1,1);
        
    }
    
    move_scale = move_scale_holder;
    
}


void Laser_Show::move (float x_movedir, float y_movedir)
{
    //move_scale is a value we set to set the size
    x_movedir = x_movedir * move_scale;
    y_movedir = y_movedir * move_scale;
    
    sendto (x_level + x_movedir, y_level + y_movedir);
    
}

void Laser_Show::sendto (float x_target, float y_target)
{
    float x_move;
    float y_move;
    float temp_exp_move = exp_move;
    
    while (fabs(x_target - x_level) > min_move || fabs(y_target - y_level) > min_move)
    {
        
        x_move = (x_target - x_level) / temp_exp_move;
        y_move = (y_target - y_level) / temp_exp_move;
        //fabs() is absolute value
        while (fabs(x_move) > max_move || fabs(y_move) > max_move)
        {
            x_move = x_move *.5;
            y_move = y_move *.5;
            temp_exp_move = temp_exp_move * 1.5;
        }
        x_level = x_level + x_move;
        y_level = y_level + y_move;
        do_pwm();
    }//while
    
    x_level = x_target;
    y_level = y_target;
    do_pwm();
    hold(line_delay);
    
}
void Laser_Show::draw_string(String text, int x, int y, int count)
{
    int hold = 0;
    
    while (text.charAt(hold) != '\0')
    {
        hold++;
    }
    hold--;
    
    for (int loop = 0; loop < count; loop ++)
    {
        sendto (x,y);
        int i = hold;
        while (i != -1)
        {
            draw_letter(text.charAt(i));
            --i;
        }
        
        
    }
}

void Laser_Show::draw_letter(byte letter)
{
    
    float temp_x_level = x_level;
    //float temp_x_level = x_level; //CHANGE
    float temp_y_level = y_level;
    
    //all characters 16x16 (leave -plenty- of space on right side to prevent overlap with next character)
    //characters are drawn starting from upper left corner
    //"cursor" is automatically returned to top right corner
    
    laser_off();
    switch (letter)
    {
            
            //draws from the top right and upside down
        case 'A':
            move (2,16); //move down
            laser_on();
            move (7,-15); //move left and up
            move (7,15);  //move left and down
            laser_off();
            move (-9,-2); //move right and up
            laser_on();
            move (8,-4); //move left and down
            break;
        case 'B':
            //TODO
            move(15,15); //start  bottom left
            laser_on();
            move(5,-10); //move up
            //hold(100);
            move(-12,-5); //move right
            //hold(100);
            move(6,3);  //move down
            //hold(100);
            move(-9,7); //move down
            //hold(100);
            move(12,15); //move to the left
            //hold(100);
            break;
            
        case 'C':
            move (0,20);
            laser_on();
            move (-11,-2);
            move (0,-12);
            move (11,-2);
            break;
            
        case 'E':
            
            move (10,0);
            laser_on();
            move (-10,0);
            move (0,14);
            move (10,2);
            laser_off();
            move (-6,-10);
            laser_on();
            move (-6,0);
            break;
            
        case 'F':
            move (10,0);
            laser_on();
            move (-10,0);
            move (0,16);
            laser_off();
            move (0,-8);
            laser_on();
            move (5,0);
            break;
        case 'L':
            move(15,0);
            laser_on();
            move(2,17);
            move(-15,3);
            break;
        case 'N':
            move(-1,0);//hold
            laser_on(); //start at top right
            move(0, 17); //move down
            move(8,-14); //move left_up
            move(1,15); //move up
            break;
        case 'O':
            laser_on();
            move (8,0);
            move (0,16);
            move (-7,0);
            move (0,-16);
            break;
        case 'P':
            //TODO
            move(7,15); //start  bottom left
            laser_on();
            move(5,-14); //move up
            move(-10,-3); //move right
            move(0,5);  //move down
            move(5, 0);
            break;
        case 'R':
            //TODO
            move(9,15); //start  bottom left
            laser_on();
            move(5,-14); //move up
            move(-10,-3); //move right
            move(0,5);  //move down
            move(5, 0);
            move(-7,15); //move down_right
            break;
        case 'T':
            //TODO
            move(0,-2);
            laser_on(); //start top right
            move(13,0); //move left
            //hold(20000);
            laser_off();
            move(-5,-1); //move right
            laser_on();
            //laser_on();
            move(-5,17);  //move down
            break;
        case 'U':
            move (15,0);
            laser_on();
            move (0,16);
            move (-11,0);
            move (0,-16);
            break;
            
        case 'V':
            laser_on(); //start top right
            move(0,17);  //move down
            move(5,-17);  //move up
            break;
        case '?':
            //TODO
            move(1,17); //start  top left
            laser_on();
            move(-15,0); //move right
            move(7,0); //move left
            move(0,15);  //move down
            laser_off();
            move(0,10);
            laser_on();
            hold(150);
            break;
            
    }
    //return to bottom right corner of character "cell" - adds 2 spaces
    laser_off();
    //sendto (temp_x_level + (40 * move_scale) , temp_y_level);
    sendto (temp_x_level + (18 * move_scale) , temp_y_level); //CHANGE
    
}
void Laser_Show::setPwmFrequency(int pin, int divisor) {
    byte mode;
    if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
        
        /*
         * Pins 5,6 are faster than all other PWM pins
         * (roughly twice as fast)
         * Setting   Divisor   Frequency
         * 0x01      1         62500
         * 0x02      8         7812.5
         * 0x03      64        976.5625   <--DEFAULT
         * 0x04      256       244.140625
         * 0x05      1024      61.03515625
         */
        switch(divisor) {
            case 1: mode = 0x01; break;
            case 8: mode = 0x02; break;
            case 64: mode = 0x03; break;
            case 256: mode = 0x04; break;
            case 1024: mode = 0x05; break;
            default: return;
        }
        
        //TCCRnB, where 'n' is the number for the timer.
        if(pin == 5 || pin == 6) {
            TCCR0B = TCCR0B & 0b11111000 | mode;
            //timer 0 (controls pin 13, 4)
            //sets Arduino Mega's pin 13 and 4 to frequency 31250.
            //code typically inserted in setup()
        } else {
            TCCR1B = TCCR1B & 0b11111000 | mode;
            //timer 1 (controls pin 12, 11)
        }
    } else if(pin == 3 || pin == 11) {
        switch(divisor) {
            case 1: mode = 0x01; break;
            case 8: mode = 0x02; break;
            case 32: mode = 0x03; break;
            case 64: mode = 0x04; break;
            case 128: mode = 0x05; break;
            case 256: mode = 0x06; break;
            case 1024: mode = 0x7; break;
            default: return;
        }
        TCCR2B = TCCR2B & 0b11111000 | mode;
    }
}

void Laser_Show::laser_on() {
    hold(laser_toggle_delay);
    digitalWrite(laser_pin, HIGH);
}
void Laser_Show::laser_off() {
    hold(laser_toggle_delay);
    digitalWrite(laser_pin, LOW);
}
void Laser_Show::hold(long length)
{
    long loop2 = 0;
    length = length * 2;
    while (loop2 < length)
    {
        delay (1); //we've messed with timers - so this is a bunch smaller than 1ms delay
        loop2 ++;
    }
}
void Laser_Show::do_pwm()
{
    //actual PWM code
    float x_adjust;
    float y_adjust;
    
    //adds any offset (but still maintains 0-255 range) / handles scaling
    x_adjust = x_min_value + (x_level * ((x_max_value - x_min_value) / 255));
    y_adjust = y_min_value + ((y_level) * ((y_max_value - y_min_value) / 255));
    
    if (x_adjust < 1) x_adjust = 1;
    if (y_adjust < 1) y_adjust = 1;
    if (x_adjust > x_max_value) x_adjust = x_max_value;
    if (y_adjust > y_max_value) y_adjust = y_max_value;
    
    analogWrite(y_speaker_pin, y_adjust);
    analogWrite(x_speaker_pin, x_adjust);
}

