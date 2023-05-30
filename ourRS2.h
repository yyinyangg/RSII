#ifndef OURRS_H_
#define OURRS_H_

#include<spi.h>
#include<i2c_master.h>

/*
*Driver of SPI, enable the spi transformation
*/
void spi_peri_enable();

/*
*Driver of SPI, select the Slave
*/
void spi_peri_select();

/*
*Driver of SPI, deselect the Slave
*/
void spi_peri_deselect();

/*
*Driver of SPI, send one Byte to Slave
*/
void spi_peri_write(const unsigned char);

/*
*Inirialize the Oled screen, and show the Title
*/
void startOled();

/*
*Diver of I2C, enable the I2C transformation
*/
void i2c_peri_enable();

/*
*Send one reading command to Sensor, and set the registerpointer to #2
*/
void sendReadCommand();

/*
*read register 2 and 3, store the 
*/
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




