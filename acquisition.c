
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

    float *X_fd;
    float *X_fd_DFT;
    float *C_DFT;
    float *values;
    float *values_IDFT;
} acquisitionInternal_t;

acquisition_t* allocateAcquisition(int32_t nrOfSamples) {
    acquisitionInternal_t * a = malloc(sizeof(acquisitionInternal_t));

    memset(a, 0, sizeof(acquisitionInternal_t)); // to initialize everything into a definitive state (=0)

    a->samples = malloc(nrOfSamples * 2 * sizeof(float));
    a->codes = malloc(nrOfSamples * 2 * sizeof(float));

    a->X_fd = malloc(nrOfSamples * 2 * sizeof(float));
    a->X_fd_DFT = malloc(nrOfSamples * 2 * sizeof(float));
    a->C_DFT = malloc(nrOfSamples * 2 * sizeof(float));
    a->values = malloc(nrOfSamples * 2 * sizeof(float));
    a->values_IDFT = malloc(nrOfSamples * 2 * sizeof(float));

    return (acquisition_t*)a;
}

void deleteAcquisition(acquisition_t* acq) {
    acquisitionInternal_t * a = (acquisitionInternal_t*) acq;

    // free also everything else that was allocated in [allocateAcquisition]
    free(a->samples);
    free(a->codes);
    free(a->X_fd);
    free(a->X_fd_DFT);
    free(a->C_DFT);
    free(a->values);
    free(a->values_IDFT);

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

        for(int i = 0; i < sizeOfX; i+=2){
            float alpha = PI*fd*i/fs;
            float cos_val = cos(alpha);
            float sin_val = sin(alpha);
            a->X_fd[i] = a->samples[i] * cos_val + a->samples[i+1]*sin_val;
            a->X_fd[i+1] = a->samples[i+1] * cos_val - a->samples[i] * sin_val;
        }

        for(int k =0;k<sizeOfX;k+=2){
            for(int n =0;n<sizeOfX;n+=2){
                float angle1 = PI*k*n/sizeOfX;
                float cos_val1 = cos(angle1);
                float sin_val1 = sin(angle1);
                a->X_fd_DFT[k] += a->X_fd[n]*cos_val1 + a->X_fd[n+1]*sin_val1;
                a->X_fd_DFT[k+1] += -a->X_fd[n]*sin_val1 + a->X_fd[n+1]*cos_val1;
            }
        }

        for(int kk =0;kk<sizeOfC;kk+=2){
            for(int nn =0;nn<sizeOfC;nn+=2){
                float angle2 = PI*kk*nn/sizeOfC;
                float cos_val2 = cos(angle2);
                float sin_val2 = sin(angle2);
                a->C_DFT[kk] += a->codes[nn]*cos_val2 + a->codes[nn+1]*sin_val2;
                a->C_DFT[kk+1] += -a->codes[nn]*sin_val2 + a->codes[nn+1]*cos_val2;
            }
        }

        for(int j  = 0; j<sizeOfX; j+=2){
            a->values[j] = a->X_fd_DFT[j] * a->C_DFT[j] + a->X_fd_DFT[j+1] * a->C_DFT[j+1];
            a->values[j+1] = a->X_fd_DFT[j+1] * a->C_DFT[j] - a->X_fd_DFT[j] * a->C_DFT[j+1];
        }

        float N = sizeOfX / 2;
        for (int n = 0; n < sizeOfX; n+=2) {
            for (int k = 0; k < sizeOfX; k+=2) {
                float angle3 =  PI * k * n / sizeOfX;
                float cos_val3 = cos(angle3);
                float sin_val3 = sin(angle3);

                a->values_IDFT[n] += (a->values[k] * cos_val3 - a->values[k+1] * sin_val3);
                a->values_IDFT[n+1] += (a->values[k] * sin_val3 + a->values[k+1] * cos_val3);
            }
            a->values_IDFT[n] /= N;
            a->values_IDFT[n+1] /= N;
            //printf("Re IDFT %f \n",value_IDFT[n]);
            //printf("Im IDFT %f \n",value_IDFT[n+1]);
        }
        float max = 0;

        for (int n = 0; n < sizeOfX; n+=2) {

            float temp =  (a->values_IDFT[n]*a->values_IDFT[n] +a->values_IDFT[n+1]*a->values_IDFT[n+1])/P;
            printf("found temp %f \n",temp);
            if(temp > max){
                max = temp;
                printf("found max %f \n",max);
                if(max > gamma){
                    a->codePhase = (int32_t)(n/2);
                    a->dopplerFrequency =fd;
                    result = true;
                    break;
                }
            }
        }
    }
    return result; // return whether acquisition was achieved or not!
}
