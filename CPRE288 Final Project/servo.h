/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */
#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

/**
 * Initialize Servo. Uses PB5 and Timer 1B
 */
void servo_init (void);

/**
 * Move Servo.
 */
void servo_move(uint16_t);


#endif /* PING_H_ */
