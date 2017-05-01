#ifndef _DSP_LPF_H    /* Guard against multiple inclusion */
#define _DSP_LPF_H

#define MAFSIZE 8
#define NUMWEIGHTS 8
#define ALPHA 0.3
#define BETA 0.7

char MAF_count;
char MAF_size; // actual number of elements in the array
char IIR_count, FIR_count;
short MAF_array[MAFSIZE];
short FIR_array[NUMWEIGHTS];
float old_average;
static const float weights[NUMWEIGHTS] = {.125, .125, .125, .125, .125, .125, .125, .125};

float MAF(short);
float IIR(short);
float FIR(short);

#endif
