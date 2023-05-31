#include "../include/ourRS2.h"
#include "peripherals.h"
#include <display.h>
#include <stdio.h>

#define ResetMask 0x00000U
#define SPI_SLAVE_ADDRESS (1<<4) //Address of SPI_Slave1
#define I2C_SLAVE_ADDRESS 0x70 // Address of I2C_Slave
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
    char *content = "Entfernung: ";
    spi_peri_enable();
    spi_peri_select();
    OLED_Init_25664();
    Checkerboard_25664();
    Show_String_25664(1,head,15,0);
    Show_String_25664(1,content,5,15);
}

void i2c_peri_enable(){
    I2C_MASTER.ctrl &= ResetMask;
    I2C_MASTER.ctrl |= I2C_CTRL_EN | I2C_PRESCALER;
    I2C_MASTER.ctrl |= I2C_CTRL_INTR_EN;
}

void sendReadCommand(){
    I2C_MASTER.data[0]= (I2C_SLAVE_ADDRESS<<1); //set the last bit to 0,indicating for writing
    I2C_MASTER.data[1] = 2; // set registerpointer to #2
    I2C_MASTER.cmd = I2C_CMD_STA | (2<<3) | I2C_CMD_STO;
    //while (I2C_MASTER.stat & I2C_STA_TIP);
    //printf("Reading Command sent\n");
    //while(I2C_MASTER.stat & I2C_STA_NO_ACK);
    //printf("got Acknowledge from Slave\n");
}
void i2c_peri_read(){

    I2C_MASTER.data[0]= (I2C_SLAVE_ADDRESS<<1)+1;//set the last bit to 0,indicating for reading
    I2C_MASTER.cmd = I2C_CMD_STA | (3<<3) | I2C_CMD_RD;// read 2 bytes from sensor

    //while(I2C_MASTER.stat & I2C_STA_TIP);	//wait for not busy
    //printf("transfer complete\n");


    //printf("get result %u, %u\n", sensor.resultHigh,sensor.resultLow);
}


void i2c_peri_write(const unsigned char data){
    I2C_MASTER.data[0]= I2C_SLAVE_ADDRESS<<1; //set the last bit to 0, indicating for writing
    I2C_MASTER.data[1] = 0; // set registerpointer to #0
    I2C_MASTER.data[2] = data;
    I2C_MASTER.cmd = I2C_CMD_STA | (3<<3) | I2C_CMD_STO;

    //while (I2C_MASTER.stat & I2C_STA_TIP);
    //printf("Transfer ends\n");

    //while(I2C_MASTER.stat & I2C_STA_NO_ACK);
    //printf("got Acknowledge from Slave\n");
}

void startSensor(){

    i2c_peri_write(0x51);
}
void readInCm(){

    //i2c_peri_read();
        sensor.resultHigh = I2C_MASTER.data[1];
    sensor.resultLow = I2C_MASTER.data[2];
    sensor.result = (sensor.resultHigh)*256 +sensor.resultLow ;
    //printf("result from Sensor is  %u\n", sensor.result);
}

void midPassFilterInit(){

    midPassFilter.result =0;
    for(unsigned int i =0;i < ( sizeof(midPassFilter.val) / sizeof(unsigned int) ) ; i++){
        midPassFilter.val[i] = 0;
    }
}
void startMidPassFilter(){
    int length = sizeof(midPassFilter.val)/sizeof (unsigned int);
    midPassFilter.val[0] = sensor.result;
    for( int i = 0; i<length-1; i++){
        midPassFilter.val[length-1-i] = midPassFilter.val[length-2-i];
    }

    quicksort(0,4);
    midPassFilter.result = midPassFilter.val[2];
    /*
    for( int i = 0; i<length; i++){
        printf("midPassFilter.val[%d] = %u \n",i,midPassFilter.val[i]);
    }*/
}
void quicksort(int leftPosi, int rightPosi) {
    if (leftPosi < rightPosi) {
        int pivot = midPassFilter.val[(leftPosi + rightPosi) / 2];
        int left = leftPosi;
        int right = rightPosi;
        while (left <= right) {
            while ((int)midPassFilter.val[left] < pivot) left = left + 1;
            while ((int)midPassFilter.val[right] > pivot) right = right - 1;
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
    char * unit="   CM";
    char getResult[5];


    snprintf(getResult,5,"%u",midPassFilter.result);
    //snprintf(getResult,5,"%u",sensor.result);
    Show_String_25664(1,unit,30,15);
    Show_String_25664(1,getResult,28,15);
}

void readFrimeWork(){
    
	I2C_MASTER.data[0] = I2C_SLAVE_ADDRESS<<1; // slave address 
	I2C_MASTER.data[1] = 0;			//Register nummer in slave
	I2C_MASTER.cmd = I2C_CMD_STA | (2 << 3);
	while(I2C_MASTER.stat & I2C_STA_TIP); 	//wait for not busy
	while(I2C_MASTER.stat & I2C_STA_NO_ACK);	//wait for slave's ackownledgement
	I2C_MASTER.cmd |= I2C_CMD_STO;
	
	I2C_MASTER.data[0] = (I2C_SLAVE_ADDRESS<<1) + 1; //the last bit set 1
	I2C_MASTER.cmd = I2C_CMD_STA | (2 << 3) | I2C_CMD_RD;	
	while(I2C_MASTER.stat & I2C_STA_TIP); 	//wait for not busy
	while(I2C_MASTER.stat & I2C_STA_NO_ACK);	//wait for slave's ackownledgement
	I2C_MASTER.cmd |= I2C_CMD_STO;
	printf("firmware:%u\n", I2C_MASTER.data[1]);

}
void setupTimer(){
    TIMER.control &= ResetMask;
    TIMER.limit = 16410*2;
    TIMER.control |= TIMER_EN;
    TIMER.control |= TIMER_PRE_EN;
    TIMER.control |= TIMER_PRE_256;
}

void setupCompare(){
    COMPARE.CMP_CTRL &=ResetMask;
    COMPARE.CMP_CTRL |= COMPARE_CLEAR_OUT;
    COMPARE.CMP_DAT = 16410;
    COMPARE.CMP_CTRL |= COMPARE_EN;
    COMPARE.CMP_CTRL |= COMPARE_EN_INT;
    COMPARE.CMP_CTRL |= COMPARE_EN_OUT;
}

void stopCompare(){
    COMPARE.CMP_CTRL &= ~COMPARE_EN;
    COMPARE.CMP_CTRL &= ~COMPARE_EN_INT;
    TIMER.control &= ~TIMER_EN;
}
void startCompare(){
    setupTimer();
    setupCompare();
}
