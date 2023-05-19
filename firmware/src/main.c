#include<stdio.h>
#include"peripherals.h"
#include "../include/ourRS2.h"
#include <sleep.h>
#include "interrupt.h"
FILE *stdout = &UART_LIGHT_FILE;

#define START (unsigned int 0)
#define FINISH (unsigned int 1)
#define READING (unsigned int 2)
unsigned int flag = START


void version1();
void version2();


void main(){
    // Version 1, with sleep function
    //version1();

    //Version 2, with ISR
    version2();
}



void version1(){
    startOled();
    midPassFilterInit();
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
    searchSlave();
    while(1) printf("Processor is now free ");

}

ISR(0)(){
    printf("get in ISR");
    switch (flag) {
        case START:
            printf("find Slave");
            flag = FINISH;
            startSensor();
            break;
        case FINISH:
            flag = READING;
            sendReadCommand();
            break;
        case READING:
            flag = START;
            readInCm();
            startMidPassFilter();
            showResult();
            break;
        default:
            flag = START;
            printf("Invalid Parameter got ! \n");
            break;
    }
}