#include "timer.h"
#include "open_interface.h"
#include "button.h"

#define l_val 35828
#define r_val 7043

void servo_init(void);
int servo_move(float degrees);
void servo_cal (void);
int get_equ(int start, int end);
