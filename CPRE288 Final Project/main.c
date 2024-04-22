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
#include "movement.h"
#include "adc.h"
#include "movement.h"
#include "open_interface.h"

// Uncomment or add any include directives that are needed


#define REPLACEME 0

int main(void){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    servo_init();
    ping_init();
    uart_interrupt_init();
    adc_init();

    int target;
    bool hitTarget = false;

        while(!hitTarget){
            char dir = uart_receive();
                if(dir == 'w'){
                    int distMoved = move_forwardF(sensor_data, 100);
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
                else if(dir == 'd'){
                    turn_right(sensor_data, 90);
                    uart_sendStr("Turned Right\n\r");
                }
                else if(dir == 'a'){
                    turn_left(sensor_data, 90);
                    uart_sendStr("Turned Left\n\r");
                }
                else if(dir == 's'){
                    move_backward(sensor_data, 100);
                    uart_sendStr("Moved Backward\n\r");
                }
        }
}
