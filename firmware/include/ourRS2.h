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
*the first command for reading, set the register pointer to #2
*/
void sendReadCommand();

/*
*send the reading command to Slave, require value of Register 2 and 3
*/
void i2c_peri_read();

/*
 * writing command, set the register pointer to #0
 * */
void i2c_peri_write(const unsigned char);

/*
 * start the Sensor, set up the sensor in CM model
 * */
void startSensor();

/*
 * reading the sensor register 2 and 3, and calculate the result
 * */
void readInCm();

/*
 * initialize the mid pass filter, set all Parameters to 0
 * */
void midPassFilterInit();

/*
 * start the filter
 * */
void startMidPassFilter();

/*
 * sorting function used in filter
 * */
void quicksort();

/*
 * show the result on the OLED screen
 * */
void showResult();

/*
 * set up the Compare model of timer
 * */
void setupCompare();

/*
 * set up the basic Timer
 * */
void setupTimer();

/*
 * stop the timer
 * */
void stopCompare();

/*
 * start the Timer, an Interrupt will be triggered when timer count up to the threshold value, this process takes about 70ms
 * */
void startCompare();


/*
 * encapsulate the Data from sensor, using the struct name sensor to get access of them
 * */
struct SRF02{
    unsigned int resultHigh;
    unsigned int resultLow;
    unsigned int result;

}sensor;

/*
 * encapsulate the Data used in mid pass filter, using the struct name midPassFilter to get access of them
 * */
struct mid_pass{
    unsigned int val[5];
    unsigned int result;
}midPassFilter;


#endif




