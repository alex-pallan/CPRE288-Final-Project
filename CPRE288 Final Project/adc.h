//Lab 8

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>


void adc_init (void);

uint32_t adc_read (void);

uint32_t adc_read_avg (void);

#endif


