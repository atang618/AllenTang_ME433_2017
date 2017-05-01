#include "DSP_LPF.h"



float MAF(short new) {
    if (MAF_count == MAFSIZE) { // Roll over if reached max size
        MAF_count = 0;
    }
    MAF_array[MAF_count] = new;
    MAF_count++;
    if (MAF_size < MAFSIZE) {
        MAF_size++;
    }
    int i, sum = 0;
    for (i = 0; i < MAF_size; i++) {
        sum += MAF_array[i]; 
    }
    return sum/MAF_size; 
}

float IIR(short new) {
    if (IIR_count == 0) {
        old_average = new;
        IIR_count++;
    } else {
        old_average = ALPHA*old_average+BETA*new;
    }
    return old_average;
}

float FIR(short new) {
    if (FIR_count < NUMWEIGHTS) { // if not all the array has been filled
        FIR_array[FIR_count] = new;
        FIR_count++;
    } else { // when FIR_count = 8                                   
        int i;
        for (i = 0; i < NUMWEIGHTS-1 ; i++) { // shift all elements to the left
            FIR_array[i] = FIR_array[i+1]; 
        }
        FIR_array[NUMWEIGHTS-1] = new;
    }
    int i;
    float sum = 0;
    for (i = 0; i < FIR_count; i++) { // take weighted sum
        sum += (FIR_array[i]*weights[i]);
    }
    return sum;
}