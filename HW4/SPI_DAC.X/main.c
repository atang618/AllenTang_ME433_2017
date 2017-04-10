#include "config.h"
#include "spi.h"
#include <math.h>

#define SINE_SIZE 100
#define TRI_SIZE 200
#define TWO_PI (3.14159 * 2)

static volatile unsigned char sineWave[SINE_SIZE];
static volatile unsigned char triWave[TRI_SIZE];

void boardInit();
void makeSine();
void makeTri();
void interrupt_init();

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void) {
    static int Acnt = 0; 
    static int Bcnt = 0;
    /* if (CS == 0) {
        CS = 1;
    }
    else if (CS == 1) {
        CS = 0;
    }
    */
    setVoltage('A', sineWave[Acnt]);
    setVoltage('B', triWave[Bcnt]);
    
    Acnt++;
    Bcnt++;
    if (Acnt == SINE_SIZE) {
        Acnt = 0;
    }
    if (Bcnt == TRI_SIZE) {
        Bcnt = 0;
    }
    
    IFS0bits.T2IF = 0;
}


int main () {
    boardInit();
    initSPI1();
    makeSine();
    makeTri();
    interrupt_init();
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

    __builtin_enable_interrupts();
}


void makeSine() {
    int i;
    for (i = 0; i < SINE_SIZE; i++) {
        /* if (i < 50) {
            sineWave[i] = 0;
        } else {
            sineWave[i] = 1;
        }
        */
        sineWave[i] = (sin(TWO_PI*i/SINE_SIZE)+1)*127;
    }
}

void makeTri() {
    int i;
    for (i = 0; i < TRI_SIZE; i++) {
        triWave[i] = (i*254/(TRI_SIZE-1));
    }
}

void interrupt_init() {
    __builtin_disable_interrupts();
    T2CONbits.TCKPS = 0b100;    // set prescaler to 1:16
    PR2 = 2999;             // interrupt of 1 kHz
    TMR2 = 0;               // initialize timer 2
    T2CONbits.ON = 1;       // turn on timer 2
    IPC2bits.T2IP = 5;      // set priority 5
    IFS0bits.T2IF = 0;      // clear flag
    IEC0bits.T2IE = 1;      // enable ISR
    __builtin_enable_interrupts();
}