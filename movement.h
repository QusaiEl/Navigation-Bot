#include "lcd.h"
#include "uart_extra_help.h"
#include "open_interface.h"
#include <stdint.h>
#include "timer.h"
#include "data.h"

#define l_factor
#define r_factor

void move_forward(oi_t *sensor, int centimeters);
void move_back(oi_t *sensor, int centimeters);
void turn(oi_t *sensor, int degrees);
void collision_procedure(oi_t *sensor, int dir);
void drive_to_object(oi_t *sensor_data, object* target);
void manual_drive();
void battery_power(oi_t *sensor_data);
void manual_drive_val(oi_t* sensor);

