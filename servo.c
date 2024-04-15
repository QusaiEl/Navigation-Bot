#include "servo.h"

void servo_init(void){
    SYSCTL_RCGCGPIO_R |= 0b10;
    SYSCTL_RCGCTIMER_R |= 0b10;
    timer_waitMillis(2);
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x700000;
    GPIO_PORTB_PCTL_R &= ~0x800000;

    TIMER1_CTL_R &= 0xFEFF;
    TIMER1_CFG_R = 0x4;
    TIMER1_TBMR_R = 0xA;
    TIMER1_CTL_R &= 0xBFFF;
    TIMER1_TBPR_R |= 0x4;
    TIMER1_TBILR_R = 0xE200;
    TIMER1_TBPMR_R = 0x4;
    TIMER1_TBMATCHR_R = 0xA380;
    TIMER1_CTL_R |= 0x100;
    timer_waitMillis(2);

}

int servo_move(float degrees){
    float cycles = 0;
    int val = 0;
    cycles = (((l_val - r_val)/180.0) * degrees + r_val);
    val = 320000 - (int)cycles;
    TIMER1_TBPMR_R = val >> 16;
    TIMER1_TBMATCHR_R = ((val) & 0xFFFF);
    timer_waitMillis(100);
    return (int) cycles;
}

void servo_cal (void){
    char dir = 0;
    float pos = 0;
    char button;
    int cycles;
    while(1){
        button = 10;
        button = button_getButton();
        if(button == 1){
            if(dir == 0){
                ++pos;

            } else {
                --pos;
            }
        } else if(button == 2){
            if(dir == 0){
                pos += 5;
            } else {
                pos -= 5;
            }
        }  else if(button == 3){
            if(dir == 1){
                dir = 0;
            } else {
                dir = 1;
            }
        } else if(button == 4) {
            if(dir == 0){
                pos = 180;
            } else {
                pos = 0;
            }
        }
        cycles=servo_move(pos);
        lcd_printf("pos: %.0f\nreg: %d\ndir: %d", pos, cycles, dir);
        timer_waitMillis(250);
    }
}
