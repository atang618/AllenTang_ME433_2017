#include "spi.h"

unsigned char spi_io(unsigned char o) {
    SPI1BUF = o;
    while (!SPI1STATbits.SPIRBF) {
        ;
    }
    return SPI1BUF;
}

void initSPI1() {
    /* Initialize chip select pin as digital output pin*/
    TRISBbits.TRISB8 = 0;
    CS = 1;
    
    /* Set up SPI1*/
    SPI1CON = 0;                // turn off the spi module and reset it            
    SPI1BUF = 0;                // clear the rx buffer by reading from it
    SPI1BRG = 0x1;              // baud rate to 12 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0;    // clear the overflow bit
    SPI1CONbits.CKE = 1;        // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1;      // master operation
    SPI1CONbits.ON = 1;         // turn on spi 1
    
}

void setVoltage(unsigned char channel, unsigned char voltage) {
    if (channel == 'A') {
        CS = 0;
        spi_io((0b0011 << 4) | ((voltage & 0xF0) >> 4));
        spi_io((voltage & 0x0F) << 4);
        CS = 1;
    }
    else if (channel = 'B') {
        CS = 0;
        spi_io((0b1011 << 4) | ((voltage & 0xF0) >> 4));
        spi_io((voltage & 0x0F) << 4);
        CS = 1;
    }
    
}


