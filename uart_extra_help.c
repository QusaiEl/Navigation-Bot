/*
 *
 *   uart_extra_help.c
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code, and followig the Datasheet.
 */

#include "uart_extra_help.h"

#define REPLACE_ME 0x00


void uart_init(int baud)
{
    SYSCTL_RCGCGPIO_R |= 0b10;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b10;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R = 0b0011;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  = 0x11;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   = 0b11;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   = 0b01;        // sets pb0 as output, pb1 as input

    //compute baud values [UART clock= 16 MHz] 
    double fbrd;
    double brd;
    int    ibrd;

    brd = (float)16000000 / ((float)16 * (float)baud);
    ibrd = (int) brd;
    fbrd = brd - ibrd; // page 903
    fbrd = (int)(fbrd * 64 + .5);

    UART1_CTL_R &= 0xFCFF;      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R = 0b1100000;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = 0x0;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0x301;        // enable UART1

}

void uart_sendChar(char data)
{
//    while(UART1_FR_R & UART_FR_RXFF){}
    UART1_DR_R = data;

    timer_waitMillis(5); // wonky sol
}

char uart_receive(void)
{
    while(UART1_FR_R & UART_FR_RXFE){}
    return UART1_DR_R;
}

void uart_sendStr(const char *data)
{
    char i = 0;
    while(*(data + i) != '\r'){
        uart_sendChar(*(data + i));
        ++i;

    }
}

void send_puTTy(char val[]){
    while(*val != '\0'){
        uart_sendChar(*val);
        ++val;
    }
}


void uart_printf(const char *format, ...) {
    static char buffer[150];
    va_list arglist;
    va_start(arglist, format);
    vsnprintf(buffer, 150, format, arglist);

    char *str = buffer;
    while (*str) {
        if (*str == '\n') {
            uart_sendChar('\n');
        } else {
            uart_sendChar(*str);
        }

        str++;
    }
    va_end(arglist);
}

// _PART3

//
//void uart_interrupt_init()
//{
//    // Enable interrupts for receiving bytes through UART1
//    UART1_IM_R |= 0x10; //enable interrupt on receive - page 924
//
//    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
//    // Note: NVIC register descriptions are found in chapter 3.4
//    NVIC_EN0_R |= 0x40; //enable uart1 interrupts - page 104
//
//    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
//    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number
//
//}

//void uart_interrupt_handler()
//{
////    if(GPIO_PORTB_RIS_R & 0b1){
////        flag = 1;
////        uart_data = UART1_DR_R;
////        GPIO_PORTB_ICR_R = 0xFF;
//
//    }
//    //STEP2:  Copy the data
//
//    //STEP3:  Clear the interrup
//
//}

