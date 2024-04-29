/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "stdlib.h"

unsigned long pwm_period = 0x4E200;
int lastAngle;

void servo_init (void){

  // YOUR CODE HERE
    SYSCTL_RCGCGPIO_R |= 0x02; //enable portb
    SYSCTL_RCGCTIMER_R |= 0x02; //Timer 1
    while((SYSCTL_PRGPIO_R & 0x02) != 0x02){};

    GPIO_PORTB_DIR_R &= ~0x20;
    GPIO_PORTB_AFSEL_R |= 0x20; //alt function
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF;
    GPIO_PORTB_PCTL_R |= 0x00700000; //7th digital function on mux5
    GPIO_PORTB_DEN_R |= 0x20; //enable digital function on pin 5

    //Initialize Timer1
    TIMER1_CTL_R &= ~0x100; //timer b disable
    TIMER1_CFG_R = 0x4; // 16/32 bit mode
    TIMER1_TBMR_R = 0x0A; //pwm and periodic mode

    TIMER1_TBPR_R = pwm_period >> 16; //8bit prescaler
    TIMER1_TBILR_R = pwm_period & 0xFFFF; //interval load

    // Configure and enable the timer
    TIMER1_CTL_R |= 0x100;
}

void servo_move(uint16_t degrees){
    int start = degrees;
    degrees *= 1.9;
    float edgeTime = (degrees + 100)/180.0;
    float edge = (pwm_period - (edgeTime *14250));

    TIMER1_TBMATCHR_R = ((int)edge & 0xFFFF);
    TIMER1_TBPMR_R = (int)edge >> 16;
    int wait;
    if(lastAngle == -1){
        wait = 1000;
    }
    else{
        wait = 200 + ((abs(lastAngle - start)) * 3.2);
    }
    timer_waitMillis(50);
    lastAngle = start;
}
