#include "../include/ourRS2.h"
#include "peripherals.h"
#include <display.h>
#include <stdio.h>

#define ResetMask 0x00000U
#define SPI_SLAVE_ADDRESS (1<<4) //Address of SPI_Slave1
#define I2C_SLAVE_ADDRESS 0x00 // Address of I2C_Slave
#define I2C_PRESCALER 399 // prescaler 399 with desired SCL 30KHz

void spi_peri_enable(){
	SPI_MASTER.spi.spi_control &= ResetMask;
	SPI_MASTER.spi.spi_control |= SPI_MASTER_CTRL_EN;
	SPI_MASTER.spi.spi_control &= ~SPI_MASTER_CTRL_BITCNT;
    SPI_MASTER.spi.spi_control |= 0x00900U; // 9 bit SPI frame
	SPI_MASTER.spi.spi_control |= 0x02000U; // set the frequency of Display to 7.5 MHz

}

void spi_peri_select(){

	SPI_MASTER.spi.spi_control |= SPI_SLAVE_ADDRESS;
}

void spi_peri_deselect(){
	SPI_MASTER.spi.spi_control &= ~SPI_MASTER_CTRL_SLAVE;
}

void spi_peri_write(const unsigned char data){
	while(SPI_MASTER.spi.spi_status & SPI_MASTER_STAT_FILL);
	SPI_MASTER.spi.spi_data_out=data;
}

void startOled(){
        char *head = "RS2 Versuch 1";
        char *content = "Entfernung : "
        spi_peri_enable();
        spi_peri_select();
        OLED_Init_25664();
        Show_String_25664(1,head,15,0);
        Show_String_25664(1,content,5,15);
        //printf("checkPoint1\n");
}

void i2c_peri_enable(){
    I2C_MASTER.ctrl &= ResetMask;
    I2C_MASTER.ctrl |= I2C_CTRL_EN | I2C_PRESCALER;
    I2C_MASTER.ctrl |= I2C_CTRL_INTR_EN;
}


void i2c_peri_read(){
    /*
    I2C_MASTER.data[0]= (I2C_SLAVE_ADDRESS<<1); //set the last bit to 0,indicating for writing
    I2C_MASTER.data[1] = 2 // set registerpointer to #2
    I2C_MASTER.cmd = I2C_CMD_STA | (2<<3) | I2C_CMD_STO;
    while (I2C_MASTER.stat & I2C_STA_TIP);
    print("Reading Command sent\n");
    while(I2C_MASTER.stat & I2C_STA_NO_ACK);
    print("got Acknowledge from Slave\n");
    */
    i2c_peri_write(2);// set registerpointer to #2
    I2C_MASTER.data[0]= (I2C_SLAVE_ADDRESS<<1)+1;//set the last bit to 0,indicating for reading
    I2C_MASTER.cmd = I2C_CMD_STA | (3<<3) | I2C_CMD_RD;// read 2 bytes from sensor

    while(I2C_MASTER.stat & I2C_STA_TIP);	//wait for not busy
    printf("transfer complete");

    sensor.resultHigh = I2C_MASTER.data[1];
    sensor.resultLow = I2C_MASTER.data[2];
}


void i2c_peri_write(const unsigned char data){
    I2C_MASTER.data[0]= I2C_SLAVE_ADDRESS<<1; //set the last bit to 0, indicating for writing
    //I2C_MASTER.data[1] = 0; // set registerpointer to #0
    I2C_MASTER.data[1] = data;
    I2C_MASTER.cmd = I2C_CMD_STA | (2<<3) | I2C_CMD_STO;

    while (I2C_MASTER.stat & I2C_STA_TIP);
    print("Transfer ends\n");

    while(I2C_MASTER.stat & I2C_STA_NO_ACK);
    print("got Acknowledge from Slave\n");
}

void startSensor(){
    i2c_peri_enable();
    i2c_peri_write(0x51);
}
void readInCm(){

    i2c_peri_read();
    sensor.result = (sensor.resultHigh)*256 +sensor.resultLow ;
}

void lowPassFilterInit(double sample_rate, double time_coefficient)
{
    lowFilter.sample_rate = sample_rate;
    lowFilter.time_coefficient = time_coefficient;
    lowFilter.filtered[0] = 0;
    lowFilter.filtered[1] = 0;
    lowFilter.unfiltered[0] = 0;
    lowFilter.unfiltered[1] = 0;
}

void startLowPassFilter()
{
    double tau_s = lowPassFilter.time_coefficient;
    double tau_sw = lowPassFilter.sample_rate;

    lowPassFilter.filtered[0] = tau_s / (tau_s + 2 * tau_sw) *
                            (lowPassFilter.unfiltered[0] + lowPassFilter.unfiltered[1]) -
                            ((tau_s - 2 * tau_sw) / (tau_s + 2 * tau_sw)) * lowPassFilter.filtered[1];
    lowPassFilter.filtered[1] = lowPassFilter.filtered[0];
    lowPassFilter.unfiltered[1] = lowPassFilter.unfiltered[0];
}

void midFilterInit(){

    midPassFilter.result =0;
    for(int i =0;i < ( sizeof(midPassFilter.val) / sizeof(unsigned int) ) ; i++){
        midPassFilter.val[i] = 0;
    }
}
void startMidFilter(){
    int length = sizeof(midPassFilter.val)/sizeof (unsigned int);

    for( int i = 0; i<length-1; i++){
        midPassFilter.val[length-1-i] = midPassFilter.val[length-2-i];
    }
    midPassFilter.val[0] = sensor.result;
    quicksort(0,4);
    midPassFilter.result = midPassFilter.val[2];
}
void quicksort(int leftPosi, int rightPosi) {
    if (leftPosi < rightPosi) {
        int pivot = midPassFilter.val[(leftPosi + rightPosi) / 2];
        int left = leftPosi;
        int right = rightPosi;
        while (left <= right) {
            while (midPassFilter.val[left] < pivot) left = left + 1;
            while (midPassFilter.val[right] > pivot) right = right - 1;
            if (left <= right) {
                int temp = midPassFilter.val[left];
                midPassFilter.val[left] = midPassFilter.val[right];
                midPassFilter.val[right] = temp;
                left = left + 1;
                right = right - 1;
            }
        }
        quicksort(leftPosi, right);
        quicksort(left, rightPosi);
    }
}

void showResult(){
    char * unit="_______CM";
    char * getResult;
    sprintf(getResult,"%d",midPassFilter.result);
    Show_String_25664(1,unit,5,25);
    Show_String_25664(1,getResult,5,25);
}