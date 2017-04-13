#ifndef _io_expander_H    /* Guard against multiple inclusion */
#define _io_expander_H

#include "i2c_master_noint.h"


#define SLAVE_ADDR 0b0100000     // A0, A1, A2 all grounded

void initExpander(void);
void setExpander(char pin, char level);
char getExpander();

#endif 