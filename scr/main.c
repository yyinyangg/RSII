#include<stdio.h>
#include"peripherals.h"
#include "../include/ourRS2.h"

FILE *stdout = &UART_LIGHT_FILE;

void version1();



void main(){
    // Version 1, with sleep function
    version1();
}



void version1(){
    startOled();
    startSensor();
    midFilterInit();
    while(1){
        sleep_ms(70);
        readInCm();
        print("Entfernung :%d",midPassFilter.result);
        startMidFilter();
        showResult();
    }
    spi_peri_deselect();
}