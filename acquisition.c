
#include "acquisition.h"

#include <malloc.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159267
#define gamma 0.015
#define fs 2000000

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
    a->codeCount += 2;
}

__attribute__((noipa))
bool startAcquisition(acquisition_t* acq, int32_t testFreqCount, const int32_t* testFrequencies) {
    acquisitionInternal_t * a = (acquisitionInternal_t*) acq;
    bool result = false;
    int sizeOfC = 2000;
    int sizeOfX = 2000;

    float P = 0;
    for(int i = 0; i < sizeOfX; i++){
        P += a->samples[i] * a->samples[i];
    }
    P = P/sizeOfX*2;

    for(int32_t index_fd = 0; index_fd < testFreqCount; index_fd++){
    if(result) break;
        int32_t fd = testFrequencies[index_fd];
        float X_fd[sizeOfX];
        float X_fd_DFT[sizeOfX];
        float C_DFT[sizeOfC];
        for(int i = 0; i < sizeOfX; i+=2){
            X_fd[i] = a->samples[i] * cos(2*PI*fd*i/fs) + a->samples[i+1]*sin(2*PI*fd*i/fs);
            X_fd[i+1] = a->samples[i+1] * cos(2*PI*fd*i/fs) - a->samples[i] * sin(2*PI*fd*i/fs);
        }

        float angle =0;
        for(int k =0;k<sizeOfX;k+=2){
            X_fd_DFT[k] = 0;
            X_fd_DFT[k+1] =0;
            for(int n =0;n<sizeOfX;n+=2){
                angle = 2*PI*k*n/sizeOfX*2;
                X_fd_DFT[k] += a->samples[n]*cos(angle) + a->samples[n+1]*sin(angle);
                X_fd_DFT[k+1] += -a->samples[n]*sin(angle) + a->samples[n+1]*cos(angle);
            }
        }

        angle =0;
        for(int k =0;k<sizeOfC;k+=2){
            C_DFT[k] = 0;
            C_DFT[k+1] =0;
            for(int n =0;n<sizeOfC;n+=2){
                angle = 2*PI*k*n/sizeOfC*2;
                C_DFT[k] += a->codes[n]*cos(angle) + a->codes[n+1]*sin(angle);
                C_DFT[k+1] += -a->codes[n]*sin(angle) + a->codes[n+1]*cos(angle);
            }
        }
        float value[sizeOfX];
        for(int i  = 0; i<sizeOfX; i++){
            value[i] = X_fd_DFT[i] * C_DFT[i];
        }
        float value_IDFT[sizeOfX];
        int32_t N = sizeOfX /2;
        for (int n = 0; n < sizeOfX; n+=2) {
            value_IDFT[n] = 0.0;
            value_IDFT[n+1] = 0.0;
            for (int k = 0; k < sizeOfX; k+=2) {
                float angle = 2 * PI * k * n / N;
                float cos_val = cos(angle);
                float sin_val = sin(angle);

                value_IDFT[n] += (value[k] * cos_val - value[k+1] * sin_val);
                value_IDFT[n+1] += (value[k] * sin_val + value[k+1] * cos_val);
            }
            value_IDFT[n] /= N;
            value_IDFT[n+1] /= N;
            printf("Re IDFT %f \n",value_IDFT[n]);
            printf("Im IDFT %f \n",value_IDFT[n+1]);
        }
        float max = 0;

        for (int n = 0; n < sizeOfX; n+=2) {
            float temp =  (value_IDFT[n]*value_IDFT[n] +value_IDFT[n+1]*value_IDFT[n+1])/P;
            printf("found temp %f \n",temp);
            if(temp>max){
                max = temp;
                    printf("found max %f \n",max);
            }
            if(max >gamma){
             a->codePhase = n/2;
            a->dopplerFrequency =fd;
                result = true;
                break;
            }

        }
    }
    return result; // return whether acquisition was achieved or not!
}
