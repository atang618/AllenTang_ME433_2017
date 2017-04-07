#include "config.h"
#include "spi.h"
#include <math.h>

#define SINE_SIZE 100
#define TRI_SIZE 200
#define TWO_PI (3.14159 * 2)

static volatile unsigned char sineWave[SINE_SIZE];
static volatile unsigned char triWave[TRI_SIZE];

void makeSine();
void makeTri();

int main () {
    boardInit();
    makeSine();
    makeTri();
    
}

void makeSine() {
    int i;
    for (i = 0; i < SINE_SIZE; i++) {
        sineWave[i] = (sin(TWO_PI*i/SINE_SIZE)+1)*127;
    }
}

void makeTri() {
    int i;
    for (i = 0; i < TRI_SIZE; i++) {
        triWave[i] = i/(TRI_SIZE-1)*254;
    }
}