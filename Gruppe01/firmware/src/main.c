#include<stdio.h>
#include"peripherals.h"
#include "../include/ourRS2.h"
#include <sleep.h>
#include "interrupt.h"
#include <timer.h>
FILE *stdout = &UART_LIGHT_FILE;

#define IDLE (unsigned  int) 0
#define START (unsigned int) 1
#define FINISH (unsigned int) 2
#define READING (unsigned int) 3
unsigned int flag = START;


void version1();
void version2();


void main(){
    // Version 1, with sleep function
    //version1();

    //Version 2, with ISR
    version2();
}

/*
 * using sleep function to suspend the Thread, until the messing is completed,
 * the While Loop inside the Diver should be uncommented
 * */
void version1(){
    startOled();
    midPassFilterInit();
    i2c_peri_enable();
    while(1){

        startSensor();
        sleep(0.08);
        sendReadCommand();
        i2c_peri_read();
        readInCm();
        startMidPassFilter();
        showResult();
    }
    spi_peri_deselect();
}

/*
 * using ISR and State Machine to read the data register in Sensor
 * */
void version2(){
    startOled();
    interrupt_enable();
    midPassFilterInit();
    i2c_peri_enable();
    startSensor();
    while(1){

        startMidPassFilter();
        showResult();

    }
}

/*
 * the ISR of I2C Interrupt source
 * */
ISR(0)(){
printf("get in ISR 0 with flag %u\n",flag); // man braucht die Funktion die ISR zu verzögern, sonst wird der Sensor nicht richtig gestarted
switch(flag){
case IDLE:
flag = START;
I2C_MASTER.data[0]=0;//stop the ISR
readInCm();
startSensor();
break;

case START:
flag = FINISH;
I2C_MASTER.data[0]=0;
startCompare();
break;

case READING:
flag = IDLE;
I2C_MASTER.data[0]=0;
i2c_peri_read();

break;

default:
flag = IDLE;
I2C_MASTER.data[0]=0;
printf("Invalid Parameter got ! \n");
break;
}
}

/*
 * the ISR of Timer interrupt source
 * */
ISR(1)(){
//printf("get in ISR 1 with flag %u\n",flag);
if(flag == FINISH){
flag =READING;
stopCompare();
sendReadCommand();
}
else{
printf("wrong Flag\n");
}
}
