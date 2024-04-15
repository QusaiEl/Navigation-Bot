#include "uart_extra_help.h"
#include "timer.h"
#include "open_interface.h"

#define s_speed 34300.0


float get_dist(void);
void ping_init();
void send_pulse();
void ping_isr();
