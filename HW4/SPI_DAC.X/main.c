#include "config.h"
#include "spi.h"
#include <math.h>

#define SINE_SIZE 100
#define TWO_PI (3.14159 * 2)

static volatile int sineWave[SINE_SIZE];

void makeSINE();

int main () {
    boardinit();
    makeSINE();
    
    
}

void makeSINE() {
    int i;
    for (i = 0; i < SINE_SIZE; i++) {
        sineWave[i] = sin(TWO_PI*i/SINE_SIZE);
    }
}