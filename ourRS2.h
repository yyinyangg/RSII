#ifndef OURRS_H_
#define OURRS_H_

#include<spi.h>

void spi_peri_enable();
void spi_peri_select();
void spi_peri_deselect();
void spi_peri_write(const unsigned char);
void startOled();


#endif




