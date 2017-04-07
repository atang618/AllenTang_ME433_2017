#ifndef _SPI_H    /* Guard against multiple inclusion */
#define _SPI_H


#include <xc.h>

#define CS LATBbits.LATB15

unsigned char spi_io(unsigned char o);
void initSPI1();
void setVoltage(unsigned char channel, unsigned char voltage);


#endif 

