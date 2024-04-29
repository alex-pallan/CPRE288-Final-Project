#include "adc.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <math.h>
#include "Timer.h"
#include "lcd.h"

void adc_init(void){
    SYSCTL_RCGCADC_R = 0x1;
    SYSCTL_RCGCGPIO_R |= 0x2;
    while((SYSCTL_RCGCGPIO_R & 0x2) != 0x2){};
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R = ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    ADC0_ACTSS_R &= ~0x7;
    ADC0_EMUX_R &= ~0xF000;
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10;
    ADC0_SSCTL3_R = 0x6;
    ADC0_ACTSS_R |= 0x8;
}

uint32_t adc_read(void){
    ADC0_SAC_R = 3;
    ADC0_PSSI_R = 0x8;
    while((ADC0_RIS_R & 0x8) == 0){};
    uint32_t result = ADC0_SSFIFO3_R & 0xFFF;
    ADC0_ISC_R = 0x0008;
    return result;
}


uint32_t adc_read_avg(void){
    int i;
    uint32_t avgVal = 0;
    int scans = 16;
    for(i = 0; i < scans; i++){
        avgVal += adc_read();
    }
    avgVal /= scans;
    return avgVal;
}

float adc_distance(void){
    uint32_t adcVal = adc_read_avg();
    //BOT 13
    float dist = 689720 * (powf(adcVal, -1.42));
    char buffer[20];
    sprintf(buffer, "%d, %.2f", adcVal, dist);
    //lcd_puts(buffer);
    timer_waitMillis(50);
    lcd_clear();
    return dist;
}





