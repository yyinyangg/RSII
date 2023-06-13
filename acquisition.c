
#include "acquisition.h"

#include <malloc.h>
#include <string.h>
#include <math.h>

typedef struct {
    int32_t codePhase;
    int32_t dopplerFrequency;
    // internal state for acquisition goes here
    // for example the current index used to add code and sample entries

    // Example:
    int32_t sampleCount;
    float* samples;
    
    int32_t codeCount;
    float* codes;
} acquisitionInternal_t;

acquisition_t* allocateAcquisition(int32_t nrOfSamples) {
    acquisitionInternal_t * a = malloc(sizeof(acquisitionInternal_t));

    memset(a, 0, sizeof(acquisitionInternal_t)); // to initialize everything into a definitive state (=0)

    a->samples = malloc(nrOfSamples * 2 * sizeof(float));
    a->codes = malloc(nrOfSamples * 2 * sizeof(float));

    return (acquisition_t*)a;
}

void deleteAcquisition(acquisition_t* acq) {
    acquisitionInternal_t * a = (acquisitionInternal_t*) acq;

    // free also everything else that was allocated in [allocateAcquisition]
    free(a->samples);
    free(a->codes);

    // after freeing all contained structures on heap, free acq itself
    free(acq);
}

void enterSample(acquisition_t* acq, float real, float imag) {
    acquisitionInternal_t * a = (acquisitionInternal_t*) acq;

    // put a sample-entry into the state in [a]

    // Example
    a->samples[a->sampleCount] = real;
    a->samples[a->sampleCount+1] = imag;
    a->sampleCount += 2; 
}

void enterCode(acquisition_t* acq, float real, float imag) {
    acquisitionInternal_t * a = (acquisitionInternal_t*) acq;

    // put a code-entry into the state in [a]
    a->codes[a->codeCount] = real;
    a->codes[a->codeCount+1] = imag;
    a->sampleCount += 2; 
}

__attribute__((noipa))
bool startAcquisition(acquisition_t* acq, int32_t testFreqCount, const int32_t* testFrequencies) {
    acquisitionInternal_t * a = (acquisitionInternal_t*) acq;
    int length = sizeof(a->codes)/sizeof(float);
    float sum_real = 0;
    float sum_img =0;
    float angle =0;
    for(int k =0;k<length;k++){
    	sum_real = 0;
    	sum_img =0;
    	for(int n =0;n<length;n++){
    		angle = 2*3.14159267*k*n/length;
    		sum_real += a->samples[n][0]*cos(angle) + a->samples[n][1]*sin(angle);
    		sum_img += -a->samples[n][0]*sin(angle) + a->samples[n][1]*cos(angle);
    	}
    	
    } 


	bool result;

	return result; // return whether acquisition was achieved or not!
}
