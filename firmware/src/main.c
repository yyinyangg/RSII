#include<stdio.h>
#include"peripherals.h"
#include "../include/ourRS2.h"
#include <sleep.h>

FILE *stdout = &UART_LIGHT_FILE;

void version1();



void main(){
    // Version 1, with sleep function
    version1();
}



void version1(){
    startOled();
    //startSensor();
    midPassFilterInit();
    while(1){
    	startSensor();
        sleep(0.08);
        readInCm();
        //printf("Entfernung :%d\n",midPassFilter.result);
        startMidPassFilter();
        showResult();
    }
    spi_peri_deselect();
}
