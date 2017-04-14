#include "LCD.h"
#include "config.h"
#include "timer.h"
#include <stdio.h>

void boardInit();

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void) {
    static char message[20];
    static int n = 0;
    sprintf(message, "Hello world %d!", n);
    LCD_clearScreen(WHITE);
    LCD_drawString(28,32, message, GREEN);
    LCD_drawBar(28,45,n,5,GREEN);
    
    n++;
    if (n > 100) {
        n = 0;
    }
            
    IFS0bits.T2IF = 0;
}


int main() {
    boardInit();
    SPI1_init();
    LCD_init();
    interrupt_init();
    
    while (1) {;}
}

void boardInit() {
     __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    
    // initialize LED
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 1; 
    
    __builtin_enable_interrupts();
}