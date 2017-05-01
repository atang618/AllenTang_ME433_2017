#ifndef _DSP_LPF_H    /* Guard against multiple inclusion */
#define _DSP_LPF_H

#define MAFSIZE 8
#define NUMWEIGHTS 8
#define ALPHA 0.3
#define BETA 0.7

char MAF_count = 0;
char MAF_size = 0; // actual number of elements in the array
char FIR_count = 0;
short MAF_array[MAFSIZE] = {0};
short FIR_array[NUMWEIGHTS] = {0};
float old_average = 0;
float weights[NUMWEIGHTS] = {.125, .125, .125, .125, .125, .125, .125, .125};

float MAF(short);
float IIR(short);
float FIR(short);

#endif
