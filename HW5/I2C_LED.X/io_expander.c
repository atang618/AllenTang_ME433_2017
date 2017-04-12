#include "io_expander.h"

void initExpander(void) {
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    i2c_master_setup();
    // set GPIO 0-3 as outputs
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR << 1);
    i2c_master_send(0x00);              // I/O Direction register
    i2c_master_send(0x0F);
    i2c_master_stop();
    // could potentially set SEQOP bit here
    // enable pull up on GP7 for pushbutton
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR << 1);
    i2c_master_send(0x06);              // GPPU register
    i2c_master_send(0x80);      
    i2c_master_stop();
    
}

void setExpander(char pin, char level) {
    i2c_master_start();                     
    i2c_master_send(SLAVE_ADDR << 1);       // write
    i2c_master_send(0x09);                  // GPIO register
    i2c_master_send(level << pin);          // ideally should read first to figure out mask
    i2c_master_stop();
}

char getExpander() {
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR << 1);       // write
    i2c_master_send(0x09);                  // GPIO register
    i2c_master_restart();
    i2c_master_send((SLAVE_ADDR << 1) | 1); // read
    char r =i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return r;
}