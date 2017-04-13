#include "io_expander.h"
#include "config.h"

void boardInit();


int main() {
    boardInit();
    initExpander();
    while (1) {
        if (!(getExpander() >> 7)) {            // if GP7 is low
            setExpander(0,0);
            while (!(getExpander() >> 7)) {;}   // wait until GP7 is high
        } else {                                // if GP7 is high
            setExpander(0,1);   
            while ((getExpander() >> 7)) {;}      // wait until GP7 is low  
        }
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