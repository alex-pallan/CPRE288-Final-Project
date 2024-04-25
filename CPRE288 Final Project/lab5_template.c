/**
 * lab5_template.c
 *
 * Template file for CprE 288 Lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021, 2/17/2022
 */
/*
#include "button.h"
#include "timer.h"
#include "lcd.h"

//#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors

#include "uart-interrupt.h"


//#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
    button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_interrupt_init();

	while(1){

	    if(command_byte == 's'){
	        lcd_printf("%c", command_byte);
	        command_flag = 1;
	        if(command_byte == 'g'){
	            command_byte = -1;
	            break;
	        }

	    }
	}



	      /*  int j;
	        char buffer[20];
	        while(1){
	            char i = uart_receive_nonblocking();


	            if(command_flag == 1){
	                lcd_printf("FLAG");
	                while(i != 'g'){
	                    char i = uart_receive_nonblocking();
	                }
	                command_flag = 0;
	            }

	            sprintf(buffer, "Got a %c \t", i);
	            lcd_printf("%c", i);

	            for(j = 0; j < sizeof(buffer); j++){
	                uart_sendChar(buffer[j]);
	            }
	        }*/


	//}
