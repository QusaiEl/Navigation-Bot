#ifndef PING_H
#define PING_H
#include "ping.h"

enum level {
    LOW,
    HIGH,
    DONE
};

volatile int start;
volatile char status;
volatile int end;
int num_overflows = 0;


void ping_init(){
    //port init
    SYSCTL_RCGCGPIO_R|= 0b10; //enable clock
    GPIO_PORTB_DEN_R |= 0b1000;
    GPIO_PORTB_AFSEL_R &= 0xF7;
    GPIO_PORTB_DATA_R &= 0xF7;

    //timer init
    SYSCTL_RCGCTIMER_R |= 0b1000;
    TIMER3_CTL_R &= 0xFEFF;
    TIMER3_CFG_R |= 0x00000004;
    TIMER3_TBMR_R |= 0x7;
    TIMER3_TBMR_R &= 0xFFEF;
    TIMER3_TBILR_R = 0xFFFF;
    TIMER3_TBPR_R = 0xFF;
    TIMER3_CTL_R |= 0xC00;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;
    TIMER3_ICR_R |= 0x400;
    NVIC_EN1_R |= 0x10;
    IntRegister(INT_TIMER3B, ping_isr);
    status = LOW;
}

void send_pulse(){
    // resetting regs
    TIMER3_CTL_R &= 0xFEFF;
    GPIO_PORTB_AFSEL_R &= 0xF7;
    GPIO_PORTB_DIR_R |= 0b1000;
    // pilse and i/o
    GPIO_PORTB_DATA_R |= 0x08;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= 0xF7;
    GPIO_PORTB_DIR_R &= 0xF7;
    //setinput
    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_PCTL_R |= 0x7000;
    TIMER3_CTL_R |= 0x100;
}

float get_dist(){
    status = LOW;
    send_pulse();
    while(status != DONE){}
    int time = (start - end);
    if(end > start)
    {
        num_overflows++;
        time = start + (0xFFFFFF - end);
    }
    return ((float) ((time * s_speed) / (2.0 * 16000000.0)));
}

int get_overflows()
{
    return num_overflows;
}

void ping_isr(){
   // TIMER3_ICR_R |= 0x400;
    if(TIMER3_MIS_R & 0x400) {
        if(status == LOW){
            start = TIMER3_TBR_R;
            status = HIGH;
        } else if(status == HIGH) {
            end = TIMER3_TBR_R;
            status = DONE;
        }
        TIMER3_ICR_R |= 0x400;
    }
}

#endif
