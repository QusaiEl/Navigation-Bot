#include "ir.h"
#include "timer.h"

void adc_init(void){
    // enable clocks and set GPIO registers
    SYSCTL_RCGCGPIO_R|= 0b10; //enable clocks for the corresponding ports used
    SYSCTL_RCGCADC_R |= 0b1; //select ADC module 0 or 1
    timer_waitMillis(300);
    GPIO_PORTB_DEN_R &= 0xEF; //disable digital functions
 //   GPIO_PORTB_DIR_R &= 0b11101111; // input
    GPIO_PORTB_AFSEL_R |= 0x10; //set the alternate functions
    GPIO_PORTB_AMSEL_R |= 0x10; //enable the corresponding analog functions
    // initialize and configure ADC


    ADC0_ACTSS_R &= 0xE; //disable the sample sequencer that you want, in this case SS0
    ADC0_EMUX_R &= 0xFFF0; //use the default trigger; initiated by setting bits in the ADCPSSI register when you need it to sample/scan
    ADC0_SSMUX0_R |= 0xA; //set the analog input number that you are using, this case AIN10
    ADC0_SAC_R = 0x4; //set the amount of sampling (oversampling) that you want the hardware to perform
    ADC0_SSCTL0_R |= 0x2; // may cause problems......  sets the ctl register to use the specific 'extra instructions' you want
    ADC0_ACTSS_R |= 0b1;

}

int adc_read(void){
    ADC0_PSSI_R |= 0b1;// to trigger
    while(ADC0_SSFSTAT0_R & 0x100){}
    timer_waitMillis(5);
    return ADC0_SSFIFO0_R;

}
