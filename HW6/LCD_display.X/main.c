#include "LCD.h"
#include "config.h"
#include <stdio.h>

void boardInit();

int main() {
    boardInit();
    SPI1_init();
    LCD_init();
    
    char message[20];
    sprintf(message, "Hello world!");
    LCD_clearScreen(WHITE);
    LCD_drawString(50,50,message, GREEN);
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