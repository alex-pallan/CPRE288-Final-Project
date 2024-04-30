#include "Timer.h"
#include "lcd.h"


/*int main (void) {

    char s [21];
    int displayLength = 20;
    int stringLength = 54;

    char str [] = "                    Microcontrollers are a lot of fun!";

    int i, j, k;

    int loopAmount = 50;

	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	// Print "Hello, world" on the LCD
	for (i = 0; i < loopAmount; i++){
	    for(j = 0; j < stringLength; j++){
	        str[stringLength] = str[0];

	        for(k = 0; k < stringLength; k++){
	            str[k] = str[k+1];
	        }
	        str[stringLength] = '\0';
	        strncpy(s, str, displayLength);
	        s[displayLength] = '\0';

	        lcd_printf(s);
	        timer_waitMillis(300);
	    }
	}

	// lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

	return 0;
}*/
