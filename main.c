/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "servo.h"
#include "stdio.h"
#include "ping_template.h"
#include "uart-interrupt.h"
#include <movement.h>
#include "button.h"
#include "adc.h"
#include <open_interface.h>
#include "cyBot_uart.h"
#include "cyBot_scan.h"
#include "music.h"

// Uncomment or add any include directives that are needed
float pingDistance;
float irDist;

#define REPLACEME 0

void pingScan(uint32_t degree){
    servo_move(degree);
    pingDistance = ping_getDistance();
    irDist = adc_distance();
    //uint32_t adcVal = adc_read_avg();
    //irDist = 689720 * (powf(adcVal, -1.42));
}

void scan (int begin, int end, int pul){
    char buffer[3];
    int j;
    int i = 0;
    int pulse = pul;
    int stop = end;
    int start = begin;

   pingScan(start);
   pingScan(start);
   for(i = start; i <= stop; i += pulse){
       pingScan(i);
       float pingDist = 0;
       int IRVal = 0;
       for(j = 0; j < 5; j++){
           //Detects an object but ping distance is inaccurate
           IRVal += irDist;
           pingDist += pingDistance;
       }

       IRVal /= 5;
       pingDist /= 5;

       //int intFloat = (int)pingDist;
       if(IRVal > 999){
           IRVal = 999;
       }
       uart_sendChar('d');
       sprintf(buffer, "%d", i);
       for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
           uart_sendChar(buffer[j]);
        }
       uart_sendChar('\n');

       uart_sendChar('p');
       sprintf(buffer, "%.0f", pingDist);
       for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
           uart_sendChar(buffer[j]);
       }
       uart_sendChar('\n');

       uart_sendChar('i');
        sprintf(buffer, "%d", IRVal);
        for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
          uart_sendChar(buffer[j]);
        }
        uart_sendChar('\n');
   }
}

int main(void){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    button_init();
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    uart_interrupt_init();
    servo_init();
    ping_init();
    adc_init();
    bool inTarget = false;
    int i;
    char arr[4];
    int toMove;
    int start;
    char startArr[4];
    int end;
    char endArr[4];
    int pulse;
    char pulseArr[4];
    char buffer[3];
    int j;

     /* Note from Alex:
     * Here is the notation that will be used for communication over UART.
     * Each "command" will contain one letter followed by three numbers (Ex. s002, l030, j135, ...).
     *
     * ~ Inputs ~
     * l000 for rotate left
     * r000 for rotate right
     * f000 for drive forward
     * b000 for drive backward
     * j000 to set the left (higher-degree) scan boundary
     * k000 to set the right (lower-degree) scan boundary
     * s000 to execute the scan (number will be how many degrees between each scan pulse)
     * m to play music (numbers irrelivent, will be 000)
     *
     * ~ Outputs ~
     * l000 for rotate left (approx same as input)
     * r000 for rotate right (approx same as input)
     * f000 for drive forward (approx same as input)
     * b000 for drive backward (approx same as input)
     * q000 for bumped object on left at distance
     * e000 for bumped right on right at distance
     * hf000 for hole in front
     * hr000 for hole on right
     * hl000 for hole on left
     * bf000 for out of bounds in front
     * br000 for out of bounds on right
     * bl000 for out of bounds on left
     * d000 for current degree
     * p000 for ping distance (cm)
     * i000 for ir distance (cm)
     *
     * If the bot thinks it missed part of a command, it can send XXXX and the client will re-send the most recent command
     */

    while(!inTarget){
        char dir = uart_echo();
        if(dir != 'u' && dir != 'm'){
            if(dir == 'j'){
                //scan end degree
                for(i = 0; i < 3; i++){
                    endArr[i] = uart_echo();
                }
                uart_sendChar('\n');
                endArr[3] = '\0';
                end = atoi(endArr);

                //scan start degree (dir should be k)
                dir = uart_echo();
                for(i = 0; i < 3; i++){
                    startArr[i] = uart_echo();
                }
                uart_sendChar('\n');
                startArr[3] = '\0';
                start = atoi(startArr);

                //scan pulse (dir should be s)
                dir = uart_echo();
                for(i = 0; i < 3; i++){
                    pulseArr[i] = uart_echo();
                }
                uart_sendChar('\n');
                pulseArr[3] = '\0';
                pulse = atoi(pulseArr);
            }
            //if action isn't scan
            else{
                for(i = 0; i < 3; i++){
                    arr[i] = uart_echo();
                }
            uart_sendChar('\n');
            arr[3] = '\0';
            toMove = atoi(arr);
            }
        }

                if(dir == 'f'){
                    double distMoved = move_forward(sensor_data, toMove);
                    if(distMoved < -240){
                            //do nothing but hit out of bounds (barrier)
                    }
                    else if(distMoved < -190){
                        //do nothing but hit hole
                    }
                    //didn't actually move forward since it returned to starting spot
                    //object is at distMoved from robot
                    else if(distMoved < toMove){
                        sprintf(buffer, "%.0f", distMoved);
                        for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                             uart_sendChar(buffer[j]);
                        }
                        uart_sendChar('\n');
                    }
                    else{
                        sprintf(buffer, "%.0f", distMoved);
                        for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                           uart_sendChar(buffer[j]);
                        }
                        uart_sendChar('\n');
                    }
                }
                else if(dir == 'r'){
                    double turned = turn_right(sensor_data, toMove);
                    uart_sendChar('r');
                    sprintf(buffer, "%.0f", turned);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                       uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                }
                else if(dir == 'l'){
                    double turned = turn_left(sensor_data, toMove);
                    uart_sendChar('l');
                    sprintf(buffer, "%.0f", turned);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                       uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                }
                else if(dir == 'b'){
                    double distMoved = move_backward(sensor_data, toMove);
                    uart_sendChar('b');
                    sprintf(buffer, "%.0f", distMoved);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                       uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                }
                else if(dir == 's'){
                    scan(start, end, pulse);
                }
                else if(dir == 'u'){
                    scan(0, 180, 2);
                }
                else if(dir == 'm'){
                    int num = load_song_SW();
                    playSong(num);
                    uart_sendChar('m');
                }
        }
        oi_free(sensor_data);
}
