#include <movement.h>
#include <open_interface.h>
#include "uart-interrupt.h"


   // oi_t *sensor_data = oi_alloc(); // do this only once at start of main()

    //oi_init(sensor_data); // do this only once at start of main()


double  move_forward (oi_t  *sensor_data,   double distance_mm){

    char buffer[3];
    int j;
        double sum = 0;
        oi_setWheels(100,100);
        bool bumped;
        while (sum < distance_mm) {
            oi_update(sensor_data);
            sum += sensor_data -> distance; // use -> notation since pointer
            bumped = sensor_data -> bumpLeft || sensor_data -> bumpRight;
            if(bumped){
                bool turnLeft = sensor_data -> bumpRight;
                move_backward(sensor_data, sum);
                //sum -= 150;
                //if bumped object on right
                if(turnLeft){
                    uart_sendChar('e');
                    return sum;
                }
                //if bumped object on left
                else {
                    uart_sendChar('q');
                    return sum;
                }
            }
                //White is 2800
                //Black < 500
                int frontL = sensor_data -> cliffFrontLeftSignal;
                int frontR = sensor_data -> cliffFrontRightSignal;
                int left = sensor_data -> cliffLeftSignal;
                int right = sensor_data -> cliffRightSignal;

                if(frontL > 2700 || frontR > 2700){
                    move_backward(sensor_data, sum);
                    uart_sendChar('b');
                    uart_sendChar('f');
                    //int newSum = (int)sum;
                    sprintf(buffer, "%.0f", sum);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                         uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                    return -200;
                }

                else if(right > 2700){
                    move_backward(sensor_data, sum);
                    uart_sendChar('b');
                    uart_sendChar('r');
                    //int newSum = (int)sum;
                    sprintf(buffer, "%.0f", sum);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                         uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                    return -200;
                }

                else if(left > 2700){
                    move_backward(sensor_data, sum);
                    uart_sendChar('b');
                    uart_sendChar('l');
                    //int newSum = (int)sum;
                    sprintf(buffer, "%.0f", sum);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                         uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                    return -200;
                }

                else if(frontL < 500 || frontR < 500){
                    move_backward(sensor_data, sum);
                    uart_sendChar('h');
                    uart_sendChar('f');
                    //int newSum = (int)sum;
                    sprintf(buffer, "%.0f", sum);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                         uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                    return -250;
                }

                else if(right < 500){
                    move_backward(sensor_data, sum);
                    uart_sendChar('h');
                    uart_sendChar('r');
                    //int newSum = (int)sum;
                    sprintf(buffer, "%.0f", sum);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                         uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                    return -250;
                }

                else if(left < 500){
                    move_backward(sensor_data, sum);
                    uart_sendChar('h');
                    uart_sendChar('l');
                    //int newSum = (int)sum;
                    sprintf(buffer, "%.0f", sum);
                    for (j = 0; j < sizeof(buffer); j++){        //LOOP FOR ALL
                         uart_sendChar(buffer[j]);
                    }
                    uart_sendChar('\n');
                    return -250;
                }
            }
        oi_setWheels(0,0); //stop
        uart_sendChar('f');
        return sum;
}

double move_backward (oi_t  *sensor_data,   double distance_mm){

    double sum = 0;
    oi_setWheels(-100,-100);
    while (sum < distance_mm) {
        oi_update(sensor_data);
        sum -= sensor_data -> distance; // use -> notation since pointer
    }
    oi_setWheels(0,0); //stop
    return sum;
}

double turn_left (oi_t  *sensor_data,   double degrees){

    double turn = 0;
    oi_setWheels(75,-75);

    while (turn < degrees){
        oi_update(sensor_data);
        turn += sensor_data -> angle;
    }
    oi_setWheels(0,0);
    return turn;
}

double turn_right (oi_t  *sensor_data,   double degrees){

    double turn = 0;
    oi_setWheels(-75,75);

    while (turn < degrees){
        oi_update(sensor_data);
        turn -= sensor_data -> angle;
    }
    oi_setWheels(0,0);
    return turn;
}

