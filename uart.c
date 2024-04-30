///*
//*
//*   uart.c
//*
//*
//*
//*   @author
//*   @date
//*/
//
//#include <inc/tm4c123gh6pm.h>
//#include <stdint.h>
//#include "uart.h"
//#include <cyBot_uart.h>
//
//void uart_init(void){
//	//TODO
//  //enable clock to GPIO port B
//  SYSCTL_RCGCGPIO_R |= 0b000010;
//
//  //enable clock to UART1
//  SYSCTL_RCGCUART_R |= 0b00000010;
//
//  //wait for GPIOB and UART1 peripherals to be ready
//  while ((SYSCTL_PRGPIO_R & 0b000010) == 0) {};
//  while ((SYSCTL_PRUART_R & 0b00000010) == 0) {};
//
//  //enable alternate functions on port B pins
//  GPIO_PORTB_AFSEL_R |= 0x03;
//
//  //enable digital functionality on port B pins
//  GPIO_PORTB_DEN_R |= 0x03;
//
//  //enable UART1 Rx and Tx on port B pins
//  GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the desired locations
//  GPIO_PORTB_PCTL_R |= 0x00000011; //Force 1's
//  //GPIO_PORTB_PCTL_R = 0x00000011;
//
//  //cyBot_uart_init_last_half();
//
//  //calculate baud rate
//  //double BRD = (16000000 / ((115200) * 16));
//  //uint16_t iBRD = (int) BRD; //use equations
//  //uint16_t fBRD = (int)(((BRD - iBRD) * 64) + 0.5); //use equations
//  uint16_t iBRD = 0x8;
//  uint16_t fBRD = 0x2C;
//
//
//  //turn off UART1 while setting it up
// // UART1_CTL_R &= ~0b0000001100000001; //TODO
//  UART1_CTL_R &= 0xFFFFFFFE;
//  //set baud rate
//  //note: to take effect, there must be a write to LCRH after these assignments
//  UART1_IBRD_R = iBRD;
//  UART1_FBRD_R = fBRD;
//
//  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
//  //note: this write to LCRH must be after the BRD assignments
//  UART1_LCRH_R = 0x00000060;
//
//  //use system clock as source
//  //note from the datasheet UARTCCC register description:
//  //field is 0 (system clock) by default on reset
//  //Good to be explicit in your code
//  UART1_CC_R = 0x00;
//
//  //re-enable UART1 and also enable RX, TX (three bits)
//  //note from the datasheet UARTCTL register description:
//  //RX and TX are enabled by default on reset
//  //Good to be explicit in your code
//  //Be careful to not clear RX and TX enable bits
//  //(either preserve if already set or set them)
//  //UART1_CTL_R = 0b0000001100000001;
//  //UART1_CTL_R &= 0xFFFFFFF0;
//  UART1_CTL_R |= 0x00000301;
//
//}
//
//void uart_sendChar(char data){
//    while ((UART1_FR_R & 0x20) != 0){} // holds until no data in transmit buffer
//
//       UART1_DR_R = data; // puts data in transmission buffer
//
//}
//
//char uart_receive(void){
//        uint32_t tempData; //used for error checking
//        char data;
//
//        while ((UART1_FR_R & 0x10) != 0){} // wait here until data is recieved
//
//        tempData = UART1_DR_R;
//
//        if(tempData & 0xF00){
//            GPIO_PORTB_DATA_R & 0xFF;
//        }
//
//        else{
//            data = (char)(UART1_DR_R & 0xFF);
//        }
//
//        // ToDo: Implement error checking
//        return data;
//}
//
//char uart_receive_nonblocking(void){
//        uint32_t tempData; //used for error checking
//        char data;
//
//        while ((UART1_FR_R & 0x10) != 0){} // wait here until data is recieved
//
//        if((char)(UART1_DR_R & 0xFF) == 's'){
//            while((char)(UART1_DR_R & 0xFF) != 'g'){
//                tempData = UART1_DR_R;
//
//                       if(tempData & 0xF00){
//                           GPIO_PORTB_DATA_R & 0xFF;
//                       }
//
//                       else{
//                           data = (char)(UART1_DR_R & 0xFF);
//                       }
//            }
//        }
//
//        tempData = UART1_DR_R;
//
//        if(tempData & 0xF00){
//            GPIO_PORTB_DATA_R & 0xFF;
//        }
//
//        else{
//            data = (char)(UART1_DR_R & 0xFF);
//        }
//
//        // ToDo: Implement error checking
//        return data;
//}
//
//
//void uart_sendStr(const char *data){
//	//TODO for reference see lcd_puts from lcd.c file
//
//    while (*data != '\0') {
//            uart_sendChar(*data);
//            data++;
//        }
//}
