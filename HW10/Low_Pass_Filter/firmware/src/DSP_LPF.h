#ifndef _DSP_LPF_H    /* Guard against multiple inclusion */
#define _DSP_LPF_H

#define MAFSIZE 12
#define NUMWEIGHTS 12
#define ALPHA 0.9
#define BETA 0.1

char MAF_count;
char MAF_size; // actual number of elements in the array
char IIR_count, FIR_count;
short MAF_array[MAFSIZE];
short FIR_array[NUMWEIGHTS];
float old_average;
static const float weights[NUMWEIGHTS] = {.0128, .0249, .0574, .1004, .1404, .1642, 
                                        .1642, .1404, .1004, .0574, .0249, .0128};

float MAF(short);
float IIR(short);
float FIR(short);

#endif
