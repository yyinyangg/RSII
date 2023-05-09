//
// Created by YINYANG on 2023/5/9.
//
#include "ourRS2.h"
#define CommandMask 0x0FF
#define DataMask 0x100
//--------------------------------------------------------------------------
//send Command to OLED
//--------------------------------------------------------------------------
void oled_Command_25664(unsigned char Data)
{
    spi_peri_write((Data & CommandMask));
}

//--------------------------------------------------------------------------
//send Data to OLED
//--------------------------------------------------------------------------
void oled_Data_25664(unsigned char Data)
{
    spi_peri_write((Data | DataMask));
}