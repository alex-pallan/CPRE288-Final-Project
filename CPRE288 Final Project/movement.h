/*
 * movement.h
 *
 *  Created on: Jan 30, 2024
 *      Author: pmyehl
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include <open_interface.h>

double move_forward(oi_t  *sensor_data,   double distance_mm);
double  move_backward (oi_t  *sensor_data,   double distance_mm);
double  turn_left (oi_t  *sensor_data,   double degrees);
double turn_right (oi_t  *sensor_data,   double degrees);

#endif

