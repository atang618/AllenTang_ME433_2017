#include "LSM6.h"
#include "config.h"

void boardInit();
void dataFormat(unsigned char *, unsigned short);

int main() {
    boardInit();
    initIMU();
    while (1) {
    ;
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

void dataFormat(unsigned char * raw, unsigned short * final, int length) {
    int i;
    unsigned char low, high;
    for (i = 0; i < length; i++) {
        low = raw[2*i];
        high = raw[2*i+1];
        final[i] = (high << 8) | low;
    }
}