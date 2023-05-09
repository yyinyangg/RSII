//
// Created by YINYANG on 2023/5/8.
//
#ifndef OUR_SPI_
#define OUR_SPI_

#ifdef __cplusplus
extern "C" {
#endif

#include <spi.h>

void spi_peri_enable();
void spi_peri_select();
void spi_peri_deselect();
void spi_peri_write(const unsigned char);

#ifdef __cplusplus
}
#endif

#endif
