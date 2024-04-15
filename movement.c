#include "lcd.h"
#include "uart_extra_help.h"
#include "open_interface.h"
#include "timer.h"
#include "movement.h"
#include "data.h"
#include "song.h"

void manual_drive(){
    char data;
    while(1){
        data = uart_receive();
        if(data == 'w'){
            oi_setWheels(250, 250);
            while(uart_receive() == 'w'){
                continue;
            }
            oi_setWheels(0, 0);

        } else if(data == 'a'){
            oi_setWheels(-250, 250);
            while(uart_receive() == 'a'){
                continue;
            }
            oi_setWheels(0, 0);

        } else if(data == 's'){
            oi_setWheels(-250, -250);
            while(uart_receive() == 's'){
                continue;
            }
            oi_setWheels(0, 0);

        } else if(data == 'd'){
            oi_setWheels(250, -250);
            while(uart_receive() == 'd'){
                continue;
            }
            oi_setWheels(0, 0);

        } else if(data == 'm'){
            scan_CounterClockwise();

        } else if(data == 'q'){
            return;

        }
    }
}

void manual_drive_val(oi_t* sensor){
    char data;
    char buff[8];
    int val;
    char i;



    while(1){
        battery_power(sensor);
        for(i = 0; i < 8; ++i){
            buff[i] = 0;
        }
        data = uart_receive();

        if(data == 'w'){
            move_forward(sensor, 10);

        } else if(data == 'e'){
            move_forward(sensor, 5);

        } else if(data == 'r'){
            move_back(sensor, -5);

        } else if(data == 't'){
            turn(sensor, 90);

        } else if(data == 'y'){
            turn(sensor, -90);

        } else if(data == 'u'){
            turn(sensor, 45);

        } else if(data == 'i'){
            turn(sensor, -45);

        } else if(data == 'o'){
            turn(sensor, 10);

        } else if(data == 'p'){
            turn(sensor, -10);

        } else if(data == 'm'){
            scan_CounterClockwise();

        } else if(data == 'x'){
            bear(sensor);

        } else if(data == 'q'){
            return;

        } else {

        }
    }
}

void battery_power(oi_t *sensor_data){
   int battery=0;
    oi_update(sensor_data);
    battery= 100*sensor_data->batteryCharge/sensor_data->batteryCapacity;
    lcd_printf("battery: %d", battery);
}

void move_forward(oi_t *sensor, int centimeters){
    int sum = 1;
    oi_update(sensor);
    int cliffLeftList[2], cliffFrontLeftList[2] ,cliffFrontRightList[2] ,cliffRightList[2];
    int i=0;
    cliffLeftList[i] = sensor->cliffLeftSignal;
    cliffFrontLeftList[i] = sensor->cliffFrontLeftSignal;
    cliffFrontRightList[i] =sensor->cliffFrontRightSignal;
    cliffRightList[i] = sensor->cliffRightSignal;
    ++i;

    oi_setWheels(150, 150);
    while(1) {
        if(sum > (centimeters * 10)){ // maybe TODO
            oi_setWheels(0, 0);
            timer_waitMillis(100);
            uart_printf("done moving!\n");
            return;
        }
        oi_update(sensor);


        sum += sensor->distance;
        cliffLeftList[i] = sensor->cliffLeftSignal;
        cliffFrontLeftList[i] = sensor->cliffFrontLeftSignal;
        cliffFrontRightList[i] =sensor->cliffFrontRightSignal;
        cliffRightList[i] = sensor->cliffRightSignal;
        ++i;
        i %= 2;

        if(abs(cliffLeftList[i] - cliffLeftList[(i+1)%2]) >= 650 || abs(cliffFrontLeftList[i] - cliffFrontLeftList[(i+1)%2]) >= 650
                || abs(cliffFrontRightList[i] - cliffFrontRightList[(i+1)%2]) >= 650 || abs(cliffRightList[i] - cliffRightList[(i+1)%2])  >= 650)
        {
            //            oi_setWheels(0, 0);
            //            oi_setWheels(-10,-10);
            //            timer_waitMillis(100);
            oi_setWheels(0, 0);
            cliffLeftList[i] = sensor->cliffLeftSignal;
            cliffFrontLeftList[i] = sensor->cliffFrontLeftSignal;
            cliffFrontRightList[i] =sensor->cliffFrontRightSignal;
            cliffRightList[i] = sensor->cliffRightSignal;

            //            move_back(sensor, 10);
            //            lcd_printf("Cliff detected\nL: %-6d  FL:  %-6d\nFR: %-6d  R:  %-6d\n", sensor->cliffLeftSignal, sensor->cliffFrontLeftSignal,
            //                       sensor->cliffFrontRightSignal, sensor->cliffRightSignal);
            uart_printf("Cliff detected L: %-6d, FL: %-6d FR: %-6d, R:  %-6d\n", cliffLeftList[i], cliffFrontLeftList[i],
                        cliffFrontRightList[i], cliffRightList[i]);
            collision_procedure(sensor, 0);
            return;

        } else if(sensor->bumpLeft){
            oi_setWheels(0,0);
            uart_printf("bump left\n");
            collision_procedure(sensor, 0);
            return;
        } else if(sensor->bumpRight){
            oi_setWheels(0,0);
            uart_printf("bump right\n");
            collision_procedure(sensor, 0);
            return;
        }


    }
}

void move_back(oi_t *sensor, int centimeters){
    double sum = 0;
    oi_setWheels(-150, -150);
    while(sum > centimeters * 10) {
        oi_update(sensor);
        sum += sensor->distance;
    }
    oi_setWheels(0, 0);
}


// positive angle is left negative is right
void turn(oi_t *sensor, int degrees){ //problem
    double sum = 0;

    if(degrees > 0){
        oi_setWheels(-150, 150);
        while(sum > (-degrees)) {
            oi_update(sensor);
            sum += sensor->angle;
        }
    } else {
        oi_setWheels(150, -150);
        while(sum > degrees) {
            oi_update(sensor);
            sum -= sensor->angle;
        }
    }
    oi_setWheels(0, 0);
    timer_waitMillis(500);
}

void collision_procedure(oi_t *sensor, int dir){
    oi_setWheels(0,0);

    move_back(sensor, -15);
    //    if(dir == 0){
    //        turn(sensor, 80);
    //        move_forward(sensor, 25);
    //        turn(sensor, -80);
    //    } else if(dir == 1){
    //        turn(sensor, -80);
    //        move_forward(sensor, 25);
    //        turn(sensor, 80);
    //    }
    //    oi_setWheels(250, 250);
}

// driving
void drive_to_object(oi_t *sensor_data, object* target){

    turn(sensor_data, (target->angle - 100) * (-1));
    move_forward(sensor_data, target->dist - 10);
    timer_waitMillis(200);
    while(target->dist > 20) {         //min proximity
        target = scan_CounterClockwise();
        if(target->dist < 20){
            break;
        }
        turn(sensor_data, (target->angle - 100) * (-1));
        move_forward(sensor_data, 5);
    }
}
