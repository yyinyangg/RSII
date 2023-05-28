#include<stdio.h>
#include"peripherals.h"
#include "../include/ourRS2.h"
#include <sleep.h>
#include "interrupt.h"
#include <timer.h>
FILE *stdout = &UART_LIGHT_FILE;

#define IDLE (unsigned  int) 0
//#define START (unsigned int) 1
#define ON (unsigned int) 2
//#define FINISH (unsigned int) 3
//#define READING (unsigned int) 4
unsigned int flag = IDLE;


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
    startSensor();
}

ISR(0)(){
    printf("get in ISR 0\n");
    printf("flag is %u",flag);
    if(flag == IDLE){
    printf("Start Timer\n");
    flag =ON;
    startCompare();
    }
}
ISR(1)(){
    printf("get in ISR 1 \n");
    switch(flag){

        case IDLE:
            flag = ON;
            printf("switch on Sensor\n");
            startSensor();
            break;

        case ON:
            flag = IDLE;
            printf("start reading\n");
            sendReadCommand();
            readInCm();
            startMidPassFilter();
            showResult();
            printf("finish reading and restart\n");
            break;

        default:
            flag = IDLE;
            printf("Invalid Parameter got ! \n");
            break;
    }
}