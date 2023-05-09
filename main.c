#include<stdio.h>
#include"peripherals.h"
#include "ourRS2.h"

FILE *stdout = &UART_LIGHT_FILE;

void main(){
printf("in MAIN\n");
startOled();
}
