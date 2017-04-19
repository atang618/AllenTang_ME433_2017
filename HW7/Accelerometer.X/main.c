#include "LSM6.h"
#include "config.h"
#include "LCD.h"
#include "timer.h"
#include <stdio.h>

#define CONV 60.0/16383.0
#define BARWIDTH 10

static volatile unsigned char raw[20];
static volatile short final[10];

void boardInit();
void dataFormat(unsigned char *, short *, int);

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void) {
    I2C_read_multiple(SLAVE_ADDR,0x20,raw,14);
    dataFormat(raw,final,7);
    IFS0bits.T2IF = 0;
}


int main() {
    boardInit();
    SPI1_init();
    LCD_init();
    interrupt_init();
    initIMU();
    LCD_clearScreen(WHITE);
            
    int start;
    float FPS = 0;
    
    char x, y;
    char x_prev, y_prev;
    while (1) {
        start = _CP0_GET_COUNT();
        // x direction (-y direction of LCD)
        if (final[4] < 0) {
            x = final[4]*CONV*-1;
            if (x_prev == 1) {
                LCD_drawBar(0,64,128,BARWIDTH,WHITE); // clear bar on crossover
            }
            LCD_drawBar(0,64,64-x,BARWIDTH,WHITE);
            LCD_drawBar(64-x,64,x,BARWIDTH,RED);
            x_prev = 0;
        } 
        else {
            x = final[4]*CONV;
            if (x_prev == 0) {
                LCD_drawBar(0,64,128,BARWIDTH,WHITE); // clear bar on crossover
            }
            LCD_drawBar(64,64,x,BARWIDTH,RED);
            LCD_drawBar(64+x,64,64-x,BARWIDTH,WHITE);
            x_prev = 1;
        }
        // y direction (-x direction of LCD)
        if (final[5] < 0) {
            y = final[5]*CONV*-1;
            if (y_prev == 1) {
                LCD_drawBar(64,0,BARWIDTH,128,WHITE); // clear bar on crossover
            }
            LCD_drawBar(64,0,BARWIDTH,64-y,WHITE);
            LCD_drawBar(64,64-y,BARWIDTH,y,BLUE);
            y_prev = 0;
        } 
        else {
            y = final[5]*CONV;
            if (y_prev == 0) {
                LCD_drawBar(64,0,BARWIDTH,128,WHITE); // clear bar on crossover
            }
            LCD_drawBar(64,64,BARWIDTH,y,BLUE);
            LCD_drawBar(64,64+y,BARWIDTH,64-y,WHITE);
            y_prev = 1;
        }
        
        
        while (_CP0_GET_COUNT() - start < 4800000) {;} // update screen at 5 Hz
    } 
    
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

void dataFormat(unsigned char * raw, short * final, int length) {
    int i;
    unsigned char low, high;
    for (i = 0; i < length; i++) {
        low = raw[2*i];
        high = raw[2*i+1];
        final[i] = (high << 8) | low;
    }
}