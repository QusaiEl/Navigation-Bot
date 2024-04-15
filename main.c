
#include "lcd.h"
#include "uart_extra_help.h"
#include "open_interface.h"
#include "movement.h"
#include "timer.h"
#include "button.h"
#include "ir.h"
#include "ping.h"
#include "servo.h"
#include <math.h>
#include "data.h"

int main(void)
{

    oi_t *sensor_data = oi_alloc();
    timer_init();
    uart_init(115200);
    oi_init(sensor_data); //should turn the iRobot Create's "Dirt Detect" LED to blue
    adc_init();
    button_init();
    ping_init();
    lcd_init();
    servo_init();

//    servo_cal();

//    servo_move(90);
//    lcd_printf("%f, %d", get_dist(), adc_read());
    manual_drive_val(sensor_data);

    oi_free(sensor_data);
    return 0;
}


