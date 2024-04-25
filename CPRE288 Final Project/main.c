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

// Uncomment or add any include directives that are needed
float pingDistance;
float irDist;

#define REPLACEME 0

void pingScan(uint32_t degree){
    servo_move(degree);
    ping_trigger();
    pingDistance = ping_getDistance();
    irDist = adc_distance();
}

int scan (int begin, int end){
    //cyBOT_init_Scan(0b111);

   // right_calibration_value = 311500;
    //left_calibration_value = 1256500;


   // cyBOT_Scan_t scan;

   char buffer[25];
   int j;
   int i = 0;
   char header[30];
   sprintf(header, "Degrees\tPING Distance (cm)\n\r");
   for (j = 0; j < sizeof(header); j++){        //LOOP FOR ALL
       uart_sendChar(header[j]);
   }

   int firstAngle[10];
   int lastAngle[10];
   int firstIR[10];
   int objNum = 0;
   float objDist[10];
   bool measuringObj = false;
   int stop = end;
   int start = begin;

   pingScan(0);
   pingScan(0);
   for(i = start; i <= stop; i += 2){
       pingScan(i);
       float pingDist = 0;
       int IRVal = 0;
       for(j = 0; j < 1; j++){
           //Detects an object but ping distance is inaccurate
           IRVal += irDist;
           pingDist += pingDistance;
       }

       IRVal /= 1;
       pingDist /= 1;

       /*if(pingDist > 100 && !measuringObj){
           continue;
       }*/

       if(pingDist < 150){
           sprintf(buffer, "%d\t\t%f\t%d\n\r", i, pingDist, IRVal);
           for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
               uart_sendChar(buffer[j]);
           }
           if(!measuringObj){
               firstAngle[objNum] = i;
               objDist[objNum] = pingDist;
               measuringObj = true;
           }
           else{
               if(IRVal < firstIR[objNum] - 150){
                   lastAngle[objNum] = i - 2;
                   //Ignores an object found only at one location, should help eliminate ghost objects
                   objNum++;

                   measuringObj = false;
               }
           }
       }
       else if(measuringObj){
           lastAngle[objNum] = i - 2;
           //Ignores an object found only at one location, should help eliminate ghost objects
           objNum++;
           measuringObj = false;
       }
   }

   //print stats
   sprintf(header, "Obj#\tAngle\tDistance\tWidth\n\r");
   for (j = 0; j < sizeof(header); j++){        //LOOP FOR ALL
       uart_sendChar(header[j]);
   }

   int smallestWidth = stop;
   int smallestLocation = 90;

   for(i = 0; i < objNum; i++){
       int angularWidth = lastAngle[i] - firstAngle[i];
       //Ignores an object found only at one location, should help eliminate ghost objects
       if(angularWidth <= 2){
           continue;
       }
       int location = firstAngle[i] + (angularWidth / 2);
       float dist = objDist[i];
       float linearWidth = 2 * 3.14 * dist * (angularWidth / 360.0);
       if(linearWidth > 2.8 || linearWidth < 3.0){
           smallestWidth = linearWidth;
           smallestLocation = location;
       }
       else if(linearWidth < smallestWidth){
           smallestWidth = linearWidth;
           smallestLocation = location;
       }
       sprintf(buffer, "%d\t%d\t%.3f\t%d\t%.3f\n\r", i, location, dist, angularWidth, linearWidth);
       for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
           uart_sendChar(buffer[j]);
       }
   }

   return smallestLocation;
}


bool move (int target, oi_t  *sensor_data){
    //cyBOT_init_Scan(0b111);

    //right_calibration_value = 311500;
    //left_calibration_value = 1309000;

    //cyBOT_Scan_t scan;

    int turnAngle = 90 - target;
    if(turnAngle > 0){
        turnAngle *= 0.65;
        lcd_clear();
        lcd_printf("Right, %d", turnAngle);
        turn_right(sensor_data, turnAngle);
    }
    else{
        turnAngle *= -0.65;
        lcd_clear();
        lcd_printf("Left, %d", turnAngle);
        turn_left(sensor_data, turnAngle);
    }

    float targetDistance = 0;
    int i;
    timer_waitMillis(1000);
    pingScan(90);
    pingScan(90);
    for(i = 0; i < 5; i++){
       pingScan(90);
       while(pingDistance > 150){
          pingScan(90);
       }
       targetDistance += pingDistance;
       //lcd_clear();
       //lcd_printf("%d", i);
    }
    //Average
    targetDistance /= 5;
    //Convert from cm to mm
    targetDistance *= 10;
    //Stop just before hitting object
    targetDistance -= 90;

    float distanceMoved = (float)move_forward(sensor_data, (double)targetDistance, true);

    return (distanceMoved >= targetDistance);
}

int main(void){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    button_init();
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    servo_init();
    ping_init();
    uart_interrupt_init();
    adc_init();

    bool hitTarget = false;
    int i;
    char arr[4];
    int toMove;
    int start;
    char firstArr[4];

    /*INPUTS
        l for rotate left
        r for rotate right
        f for drive forward
        b for drive backward
        s for scan

        OUTPUTS
        q for bumped left
        e for bumped right*/

        while(!hitTarget){
            /**sketchy, DRAFT
             *
             */
            uart_sendChar('S');
            char dir = uart_receive();
            timer_waitMillis(50);
            uart_sendChar(dir);
            if(dir == 's'){
                for(i = 0; i < 3; i++){
                    firstArr[i] = uart_receive();
                    timer_waitMillis(50);
                    uart_sendChar('A');
                }
                firstArr[3] = '\0';
                start = atoi(firstArr);
            }
            for(i = 0; i < 3; i++){
                arr[i] = uart_receive();
                timer_waitMillis(50);
                uart_sendChar('A');
            }
            arr[3] = '\0';
            toMove = atoi(arr);
            uart_sendChar('X');

                if(dir == 'f'){
                    int distMoved = move_forwardF(sensor_data, toMove);
                    if(distMoved < -240){
                        uart_sendStr("Hit a Cliff\n\r");
                    }
                    else if(distMoved < -190){
                        uart_sendStr("Hit an Edge\n\r");
                    }
                    else if(distMoved < 100){
                        uart_sendStr("Hit an Object\n\r");
                    }
                    else{
                        uart_sendStr("Moved Forward\n\r");
                    }
                }
                else if(dir == 'r'){
                    turn_right(sensor_data, toMove);
                    uart_sendStr("Turned Right\n\r");
                }
                else if(dir == 'l'){
                    turn_left(sensor_data, toMove);
                    uart_sendStr("Turned Left\n\r");
                }
                else if(dir == 'b'){
                    move_backward(sensor_data, toMove);
                    uart_sendStr("Moved Backward\n\r");
                }
                else if(dir == 's'){
                    int target = scan(start, toMove);
                    hitTarget = move(target, sensor_data);
                }
        }
        oi_free(sensor_data);
}
