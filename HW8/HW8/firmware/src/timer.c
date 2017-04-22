#include "timer.h"

void interrupt_init() {
    __builtin_disable_interrupts();
    T2CONbits.TCKPS = 0b111;    // set prescaler to 1:256
    PR2 = 37499;             // interrupt of 5 Hz
    TMR2 = 0;               // initialize timer 2
    T2CONbits.ON = 1;       // turn on timer 2
    IPC2bits.T2IP = 5;      // set priority 5
    IFS0bits.T2IF = 0;      // clear flag
    IEC0bits.T2IE = 1;      // enable ISR
    __builtin_enable_interrupts();
}
