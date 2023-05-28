#ifndef OURRS_H_
#define OURRS_H_

#include<spi.h>
#include<i2c_master.h>

void spi_peri_enable();
void spi_peri_select();
void spi_peri_deselect();
void spi_peri_write(const unsigned char);
void startOled();

void i2c_peri_enable();
void sendReadCommand();
void i2c_peri_read();
void i2c_peri_write(const unsigned char);
void startSensor();
void readInCm();

void lowPassFilterInit(double sample_rate, double time_coefficient);
void startLowPassFilter();

void midPassFilterInit();
void startMidPassFilter();
void quicksort();
void showResult();
void searchSlave();

void setupCompare();
void setupTimer();
void stopCompare();
void startCompare();

struct SRF02{
    unsigned int resultHigh;
    unsigned int resultLow;
    unsigned int result;

}sensor;

struct low_pass{
    double unfiltered[2];
    double filtered[2];
    double sample_rate;
    double time_coefficient;
} lowPassFilter;

struct mid_pass{
    unsigned int val[5];
    unsigned int result;
}midPassFilter;


#endif




