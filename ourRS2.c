#include "ourRS2.h"
#include "peripherals.h"
#include <display.h>
#include <stdio.h>

#define ResetMask 0x00000U
#define SLAVE_ADDRESS (1<<4)

void spi_peri_enable(){
	SPI_MASTER.spi.spi_control &= ResetMask;
	SPI_MASTER.spi.spi_control |= SPI_MASTER_CTRL_EN;
	SPI_MASTER.spi.spi_control &= ~SPI_MASTER_CTRL_BITCNT;
	SPI_MASTER.spi.spi_control |= 0x02000U;
	SPI_MASTER.spi.spi_control |= 0x00900U;
}

void spi_peri_select(){

	SPI_MASTER.spi.spi_control |= SLAVE_ADDRESS;
}

void spi_peri_deselect(){
	SPI_MASTER.spi.spi_control &= ~SPI_MASTER_CTRL_SLAVE;
}

void spi_peri_write(const unsigned char data){
	while(SPI_MASTER.spi.spi_status & SPI_MASTER_STAT_FILL);
	SPI_MASTER.spi.spi_data_out=data;
}

void startOled(){
        char *dataPointer = "RS2 Versuch 1";
        spi_peri_enable();
        spi_peri_select();
        OLED_Init_25664();
        //Checkerboard_25664();
        Show_String_25664(1,dataPointer,15,0);
        printf("checkPoint1\n");
}
