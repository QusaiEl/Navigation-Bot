#ifndef IR_H
#define IR_H

#include "uart_extra_help.h"
#include "timer.h"
#include "open_interface.h"

void adc_init(void);

int adc_read(void);

int get_equ(int start, int end);

#endif
