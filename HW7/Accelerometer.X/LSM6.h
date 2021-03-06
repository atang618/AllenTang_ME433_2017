#ifndef _LSM6_H    /* Guard against multiple inclusion */
#define _LSM6_H

#include "i2c_master_noint.h"

#define SLAVE_ADDR 0b1101011

void initIMU();
void I2C_read_multiple(unsigned char, unsigned char, unsigned char *, int);

#endif 
