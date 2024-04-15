#ifndef Song_H_
#define Song_H_


#include <math.h>
#include "Timer.h"
#include "lcd.h"
#include "button.h"
#include "PING.h"
#include "uart_extra_help.h"
#include "Servo.h"
#include <stdio.h>
#include "open_interface.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>



void bear(oi_t *sensor_data); //Bare Essentials Song
void wonder(oi_t *sensor_data); //Wonder Pets Song
void gummy(oi_t *sensor_data); //Gummy Bear Song




#endif /* Song_H_ */
