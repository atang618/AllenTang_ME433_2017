#include "LSM6.h"


void initIMU() {
    i2c_master_setup();
    // Turn on accelerometer
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR << 1); // Write 
    i2c_master_send(0x10); // CTRL1_XL
    i2c_master_send(0x82); // Set the sample rate to 1.66 kHz, with 2g sensitivity, and 100 Hz filter
    i2c_master_stop();
    // Turn on gyroscrope
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR << 1); // Write
    i2c_master_send(0x11); // CTRL2_G
    i2c_master_send(0x88); // Set the sample rate to 1.66 kHz, with 1000 dps sensitivity
    i2c_master_stop();
    // Enable reading multiple registers in a row
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR << 1); // Write
    i2c_master_send(0x12); // CTRL3_C
    i2c_master_send(0x04); // Set IF_INC bit 1
    i2c_master_stop();
}

void I2C_read_multiple(unsigned char addr, unsigned char reg, unsigned char * data, int length) {
    int i;
    i2c_master_start();
    i2c_master_send(addr << 1); // Write
    i2c_master_send(reg); // starting register
    i2c_master_restart();
    i2c_master_send((addr << 1) | 1); // Read
    for (i = 0; i < length; i++) {
        data[i] = i2c_master_recv();
        if (i < length - 1) {
            i2c_master_ack(0);
        }
        else {
            i2c_master_ack(1);
        }
    }
    i2c_master_stop();
} 