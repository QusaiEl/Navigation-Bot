#ifndef DATA_H
#define DATA_H

#include "uart_extra_help.h"
#include "timer.h"
#include "open_interface.h"
#include "servo.h"
#include "ping.h"
#include "ir.h"

#define max_dist 75

typedef struct{
    char num;
    char angle;
    float dist;
    float width;
} object;


object* scan_CounterClockwise();
object* find_allObjects(float ping_map[], float ir_map[]);
object* scan_objs(object obj_array[]);

#endif
