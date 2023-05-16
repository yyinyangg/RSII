/*
 * Automatically generated by libJConfig-4.0-alpha0 @ rs2[fddccd3]
 * Generated From: peripherals.h (/home/rs01/RS2/Versuch1/system/subsystems/spartanmc/peripherals.h)
 * Generated at Tue May 16 14:11:25 CEST 2023
 * jConfig Built on Tue Nov 26 09:26:22 CET 2019, Build Hash E3AA1574
 *
 * Variable Declarations that can be used to access peripherals
 */


#pragma once

#include <uart.h>
#include <i2c_master.h>
#include <port_out.h>
#include <spi.h>

struct file;

extern i2c_master_regs_t spartanmc_i2c_master;
extern port_out_regs_t spartanmc_port_out;
extern spi_master_regs_t spartanmc_spi_master;
extern uart_light_regs_t spartanmc_uart_light;
extern const struct file spartanmc_uart_light_file;

#define I2C_MASTER		spartanmc_i2c_master
#define PORT_OUT		spartanmc_port_out
#define SPI_MASTER		spartanmc_spi_master
#define UART_LIGHT		spartanmc_uart_light
#define UART_LIGHT_FILE		spartanmc_uart_light_file
