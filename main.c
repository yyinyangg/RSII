#include<stdio.h>
#include"peripherals.h"
#include "../include/ourRS2.h"
#include <sleep.h>
#include "interrupt.h"
#include <timer.h>
FILE *stdout = &UART_LIGHT_FILE;

#define IDLE (unsigned  int) 0
#define START (unsigned int) 1
#define RESTART (unsigned int) 2
#define FINISH (unsigned int) 3
#define READING (unsigned int) 4
unsigned int flag = IDLE;


void version1();
void version2();


void main(){
    // Version 1, with sleep function
    version1();

    //Version 2, with ISR
    //version2();
}

void version1(){
    startOled();
    midPassFilterInit();
    i2c_peri_enable();
    while(1){
    
        startSensor();
        sleep(0.08);
        sendReadCommand();
        readInCm();
        startMidPassFilter();
        showResult();
    }
    spi_peri_deselect();
}

void version2(){
    startOled();
    interrupt_enable();
    midPassFilterInit();
    i2c_peri_enable();
    startSensor();
    //searchSlave();
   
    printf("processor is free\n");
}

ISR(0)(){
    printf("get in ISR 0 with flag %u\n",flag);
    switch(flag){
    
        case IDLE:
        flag = START;
        I2C_MASTER.data[0]=0;
        printf("start Timer *******************\n");
        
	startCompare();
	
        break;
        
        case FINISH:
        flag = READING;
        I2C_MASTER.data[0]=0;
        readInCm();
        
        break;
        
        case READING:
        flag = IDLE;
        I2C_MASTER.data[0]=0;
        printf("reading and showing\n");
        startMidPassFilter();
        showResult();
        startSensor();
            
        break;

        default:
            flag = IDLE;
            printf("Invalid Parameter got ! \n");
            i2c_peri_enable();
            break;
    }
}
ISR(1)(){
    printf("get in ISR 1 with flag %u\n",flag);
    
    if(flag == START){
    printf("complete messing\n");

    flag =FINISH;
    stopCompare();
    sendReadCommand();
    }
    else{
    printf("wrong Flag\n");
    }

}
