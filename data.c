#include "data.h"


// scanning TODO need to include servo
object* scan_CounterClockwise(){
    char i = 0;
    char buff[30];
    float ping_map[45];
    float ir_map[45];
    int IR_raw_val;
    object* target;

//    send_puTTy("Degrees Distance (cm)\n"); // removed .r
    // scan and get data
    for(i = 0; i <= 180; i += 4){
        servo_move(i);
//        cyBOT_Scan(i, &scan_ptr);
        IR_raw_val = adc_read();
        ping_map[i / 4] = get_dist(); // TODO
        ir_map[i /4] = 17843 * pow(IR_raw_val, -1);
//        ir_map[i / 4] = (2 * pow(10, 6)) * pow(IR_raw_val, -1.514); bot 1
        sprintf(buff,  "%d   %.2f  %.2f\n",  i, ir_map[i / 4], ping_map[i / 4]); // formats sensor data TODO // removed /r
        uart_printf(buff); // sends sensor data
    }
    // find all objects / return thinnest one
//    target = find_allObjects(ping_map, ir_map);
    send_puTTy("END\n");

    return target;

}

object* find_allObjects(float ping_map[], float ir_map[]){
    object obj_array[10];
    char buff[80];
    float width = 0;
    char i = 0;
    signed char j = 0;

    for(j = 0; j < 10; ++j){
        obj_array[j].num = 0;
        obj_array[j].width = -1;

    }

    j = -1;

    send_puTTy("Object#   Angle   Distance   Width\n\r");

    for(i = 3; i < 45; ++i){ // was 3 for i
        if(ir_map[i] < max_dist){
            ++j;
            obj_array[j].num = j + 1;
            obj_array[j].angle = i * 4;
            obj_array[j].dist = ping_map[i];

            width = 0;
            while(ir_map[i] < max_dist && i < 45){ // threshold condition
                width += 4;
                ++i;
            }
            if(width <= 4){
                obj_array[j].num = 0;
                obj_array[j].angle = 0;
                obj_array[j].dist = 0;
            } else {
                //            width = (2 * obj_array[j].dist) * (sin(0.5 * width));
                obj_array[j].width = width;
            }
        }
    }

    for(j = 0; j < 10; ++j){ // issue inx too high and variable
        if(obj_array[j].num != 0){
            sprintf(buff, "%d         %d        %.2f      %.2f\n\r", obj_array[j].num, obj_array[j].angle, obj_array[j].dist, obj_array[j].width);
            send_puTTy(buff);
        }
    }
    //    cyBOT_Scan_t scan_ptr;
    //    cyBOT_Scan(obj_array[idx].angle, &scan_ptr);
    return scan_objs(obj_array);
}

object* scan_objs(object obj_array[]){
    object* ret = malloc(sizeof(object));
    float curr = 1000;
    char idx = 0;
    char i = 0;
    char buff[8];
//    cyBOT_Scan_t scan; //TODO
    while(obj_array[i].num != 0){
        servo_move(obj_array[i].angle + (obj_array[i].width / 2));
//        cyBOT_Scan(obj_array[i].angle + (obj_array[i].width / 2), &scan);
        obj_array[i].dist = get_dist(); // may cause issue with dist acc
        if(curr > obj_array[i].width){
            curr = obj_array[i].width;
            idx = i;
        }
        sprintf(buff, "%.2f, ", obj_array[i].dist); //TODO
        timer_waitMillis(500);
        send_puTTy(buff);
        ++i;
    }
    *ret = obj_array[idx];
    return ret;
}
