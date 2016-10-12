void sendto (float x_target, float y_target);
void do_pwm();
void hold(long length);
void laser_on();
void laser_off();
void draw_letter(byte letter);
void draw_square();
void draw_triangle();
void draw_star();

String input;


float x_level;
float y_level;
float move_scale = 2.5 ; //size of letters

int laser_pin = 10;
int x_speaker_pin = 6;
int y_speaker_pin = 5;


float exp_move = 1.6;  //main value to adjust to change laser speed
                         //controls exponential laser movement
                        //lower values = faster drawing
                        //drawing too fast will decrease quality
                        //try setting it around 2.0 to 6
                        //this is the easiest variable to adjust to tweak quality vs. speed

float min_move = .5;  //if the laser gets this close to its target location - (on both axis)
                        //it's considered close enough 
                        //larger values increase drawing speed (at expense of quality)
                        //try setting it to about 0.5

float max_move = 20;     //maximum allowed movement - helps keep things from moving too fast
                        //when drawing long lines
                        
float laser_toggle_delay = 11; //11  //extra delay in units of 20 us before toggling laser on / off - try 0 to 100
float line_delay = 8; //8  //extra delay in units of 20 us at end of each line - try 0 to 100                      

float x_min_value = 1; //these set the minimum value for X and Y channels (on a scale of 0 to 255)
float y_min_value = 1; //this helps compensate for non-linear response some speakers have right off "center"
                        //these values must be below the corresponding max values
                        //increasing these values decrease image size
                        //try values of 0-30 - increase if you are having quality issues


float x_max_value = 240; //240 //these set the minimum voltage for X and Y channels (on a scale of 0 to 255)
float y_max_value = 240; //240 //this helps prevent the speaker from going outside its linear response range / distorting
                         //these values must be about the corresponding min values
                         //decreasing these values decrease image size
                         //try values of about 190 - decrease if you are having quality issues
                         //these values may also be manipulated to adjust the "aspect ratio"                                     

void setup()   {  

  Serial.begin(9600);
 
  pinMode(laser_pin, OUTPUT);
  pinMode(x_speaker_pin, OUTPUT);
  pinMode(y_speaker_pin, OUTPUT);

  //resets the default arduino PWM stuff to much higher speed
  setPwmFrequency(x_speaker_pin, 1);
  setPwmFrequency(y_speaker_pin, 1);
  
}

  //sendto (X,Y) - sends laser to X and Y coordinates - scale is always 0 to 255 (although actual resolution may be lower than this depending on above config values)
  //move (X,Y) - moves laser to X and Y coordinates -relative- to current position
  //move_scale = X - sets scaling - only used by "move" and "draw_letter" functions
                     //scaling of 2.8 allows for 5 characters across
  //draw_letter("X") - draws single letter at single position (uppercase only - see routine for other supported characters)
  //draw_string("STRING", X, Y, COUNT) - draws a string at X,Y for COUNT times
  //marquee("HELLO WORLD", CHARACTERS) - draws a scrolling marquee CHARACTERS long - text is autoscaled to fit (4 to 7 characters long works best)
  //laser_off() - turns laser off
  //laser_on() - turns laser on
  //hold(time) - tells laser to hold current position for roughly 20 microseconds (useful if you need to force laser to "catch up" after moving a large distance)

//put your laser control code here!
String selction;


void loop() {
  //x_level = 0;
  //void draw_string(String text, int x, int y, int count)
  //HERE
  int wait = 10;
//  draw_string("B", 1, 1, wait);
//  draw_string("A", 1, 1, wait);
//  draw_string("R", 1, 1, wait);
//  draw_string("T", 1, 1, wait);
//  draw_string("O", 1, 1, wait);
//  draw_string("N", 1, 1, wait);
//  draw_string("H", 1, 1, wait);

  int i=0;
  char input[100];

  if(Serial.available()){
     delay(100);
     while( Serial.available() && i< 99) {
        input[i++] = Serial.read();
        delay(100);
     }
     input[i++]='\0';
  }

  if(i>0){
    if(input[0] == 'S') {
      String letter = &input[1];
      draw_string(letter, 1, 1, 20);
       //Serial.println((char*)input);
       Serial.println(input);
       Serial.println("here");
    }
    else if (input[0] == 'M') {
      String letter = &input[1];
      marquee(letter, 4);
    }
    else {
      int x = input[0] - '0';
      int y = input[2] - '0';
      laser_on();
      sendto(x,y);
    }
  }

  draw_star();
  draw_square();
//  draw_string("?", 1, 1, wait);

//  draw_string("BAT", 1, 1, 20);
//  marquee("BARTON LAB   UROP", 4);
//  draw_string("ALL",1,1,20);
 
}


void marquee(String text, int characters)
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


void move (float x_movedir, float y_movedir)
{
  //move_scale is a value we set to set the size
  x_movedir = x_movedir * move_scale;
  y_movedir = y_movedir * move_scale;
  
  sendto (x_level + x_movedir, y_level + y_movedir);
  
}

void sendto (float x_target, float y_target)
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
void draw_string(String text, int x, int y, int count)
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

void draw_letter(byte letter)
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
        laser_on();  
        move (7,0);
        move (0,16);
        move(-12,2);
        break;
    case 'D':
        move(15,0);
        laser_on();  
        move (-10,0);
        move (0,16);
        move(12,4);
        laser_off();
        move(5,-19);
        laser_on();
        move(0,15);
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
        laser_on();  
        move (6,1);
        laser_off();
        move (-4,5);
        laser_on();
        move(-2,-1);
        laser_off();
        move (2,-5);
        laser_on();
        move(1,16);
        break;

    case 'G':
        laser_on();  
        move (7,0);
        move (0,16);
        move(-12,2);
        move(10,-3);
        break;
//HERE
   case 'H':
       laser_on();
       move(-10,16);
       
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
void setPwmFrequency(int pin, int divisor) {
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

void laser_on() {
  hold(laser_toggle_delay);
  digitalWrite(laser_pin, HIGH);
}
void laser_off() {
  hold(laser_toggle_delay);
  digitalWrite(laser_pin, LOW);
}
void hold(long length)
{
    long loop2 = 0;
     length = length * 2;  
        while (loop2 < length)
        {
            delay (1); //we've messed with timers - so this is a bunch smaller than 1ms delay
            loop2 ++;
        }
}
void do_pwm()
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






