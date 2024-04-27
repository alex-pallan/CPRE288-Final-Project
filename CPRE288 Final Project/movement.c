#include <movement.h>
#include <open_interface.h>
#include "uart-interrupt.h"


   // oi_t *sensor_data = oi_alloc(); // do this only once at start of main()

    //oi_init(sensor_data); // do this only once at start of main()


double  move_forwardF (oi_t  *sensor_data,   double distance_mm){

        double sum = 0;
        oi_setWheels(100,100);
        bool bumped;
        while (sum < distance_mm) {
            oi_update(sensor_data);
            sum += sensor_data -> distance; // use -> notation since pointer
            bumped = sensor_data -> bumpLeft || sensor_data -> bumpRight;
            if(bumped){
                bool turnLeft = sensor_data -> bumpRight;
                move_backward(sensor_data, 150);
                sum -= 150;
                //if bumped object on right
                if(turnLeft){
                    uart_sendChar('e');
                    turn_left(sensor_data, 90);
                    move_forward(sensor_data, 250, true);
                    turn_right(sensor_data, 90);
                    return sum;
                }
                //if bumped object on left
                else {
                    uart_sendChar('q');
                    turn_right(sensor_data, 90);
                    move_forward(sensor_data, 250, true);
                    turn_left(sensor_data, 90);
                    return sum;
                }
                int frontL = sensor_data -> cliffFrontLeftSignal;
                int frontR = sensor_data -> cliffFrontRightSignal;
                int left = sensor_data -> cliffLeftSignal;
                int right = sensor_data -> cliffRightSignal;

                //cliff found AHEAD
                if(frontL < 500 || frontL > 2600 || frontR  < 500 || frontR > 2600){
                    oi_setWheels(0, 0);
                    move_backward(sensor_data, 75);
                    turn_left(sensor_data, 180);
                    //cliff hit
                    if(frontL < 500 || frontR < 500){
                        return -250;
                    }
                    //edge hit
                    return -200;
                }
                //cliff found LEFT
                if(left < 500 || left > 2600){
                    oi_setWheels(0, 0);
                    move_backward(sensor_data, 75);
                    turn_right(sensor_data, 90);
                    //cliff hit
                    if(left < 500){
                        return -250;
                    }
                    //edge hit
                    return -200;
                }
                //cliff found RIGHT
                if(right < 500 || right > 2600){
                    oi_setWheels(0, 0);
                    move_backward(sensor_data, 75);
                    turn_left(sensor_data, 90);
                    //cliff hit
                    if(right < 500){
                        return -250;
                    }
                    //edge hit
                    return -200;
                }

            }
        }
        oi_setWheels(0,0); //stop
        return sum;
}

double  move_forward (oi_t  *sensor_data,   double distance_mm, bool stop){

        double sum = 0;
        oi_setWheels(100,100);
        bool bumped;
        while (sum < distance_mm) {
            oi_update(sensor_data);
            sum += sensor_data -> distance; // use -> notation since pointer
            bumped = sensor_data -> bumpLeft || sensor_data -> bumpRight;
            if(bumped){
                bool turnLeft = sensor_data -> bumpRight;
                move_backward(sensor_data, 150);
                sum -= 150;
                if(turnLeft){
                    turn_left(sensor_data, 90);
                    move_forward(sensor_data, 250, true);
                    turn_right(sensor_data, 90);
                    if(stop){
                        return sum;
                    }
                } else {
                    turn_right(sensor_data, 90);
                    move_forward(sensor_data, 250, true);
                    turn_left(sensor_data, 90);
                    if(stop){
                        return sum;
                    }
                }
                int frontL = sensor_data -> cliffFrontLeftSignal;
                int frontR = sensor_data -> cliffFrontRightSignal;
                int left = sensor_data -> cliffLeftSignal;
                int right = sensor_data -> cliffRightSignal;

                //cliff found AHEAD
                if(frontL < 500 || frontL > 2600 || frontR  < 500 || frontR > 2600){
                    oi_setWheels(0, 0);
                    move_backward(sensor_data, 75);
                    turn_left(sensor_data, 180);
                    //cliff hit
                    if(frontL < 500 || frontR < 500){
                        return -250;
                    }
                    //edge hit
                    return -200;
                }
                //cliff found LEFT
                if(left < 500 || left > 2600){
                    oi_setWheels(0, 0);
                    move_backward(sensor_data, 75);
                    turn_right(sensor_data, 90);
                    //cliff hit
                    if(left < 500){
                        return -250;
                    }
                    //edge hit
                    return -200;
                }
                //cliff found RIGHT
                if(right < 500 || right > 2600){
                    oi_setWheels(0, 0);
                    move_backward(sensor_data, 75);
                    turn_left(sensor_data, 90);
                    //cliff hit
                    if(right < 500){
                        return -250;
                    }
                    //edge hit
                    return -200;
                }

            }
        }
        oi_setWheels(0,0); //stop
        return sum;
}

double  move_backward (oi_t  *sensor_data,   double distance_mm){


    double sum = 0;
    oi_setWheels(-100,-100);
    while (sum < distance_mm) {
        oi_update(sensor_data);
        sum -= sensor_data -> distance; // use -> notation since pointer
    }
    oi_setWheels(0,0); //stop
    return sum;
}

void  turn_left (oi_t  *sensor_data,   double degrees){

    double turn = 0;
    oi_setWheels(75,-75);

    while (turn < degrees){
        oi_update(sensor_data);
        turn += sensor_data -> angle;
    }
    oi_setWheels(0,0);

}

void  turn_right (oi_t  *sensor_data,   double degrees){

    double turn = 0;
    oi_setWheels(-75,75);

    while (turn < degrees){
        oi_update(sensor_data);
        turn -= sensor_data -> angle;
    }
    oi_setWheels(0,0);

}

