/*
 * Automatically generated by libJConfig-4.0-alpha0 @ rs2[fddccd3]
 * Generated From: ourSpi.jc4 (/home/rs01/RS2/Versuch1/ourSpi.jc4)
 * Generated at Tue May 09 15:10:20 CEST 2023
 * jConfig Built on Tue Nov 26 09:26:22 CET 2019, Build Hash E3AA1574
 *
 * Simulation Testbench Verilog Module
 */

`timescale 1ns / 1ps 
module testbench();

// ======
// Inputs
// ======
reg             clk_xilinx_clk_gen;
reg             clk_xilinx_reset;
reg             uart_light_rx;

// =======
// Outputs
// =======
wire            port_out_pin;
wire            spi_master_mosi;
wire            spi_master_sclk;
wire            spi_master_ss;
wire            uart_light_tx;

// ======
// Inouts
// ======
wire            i2c_master_scl;
wire            i2c_master_sda;

// =============
// Instantiation
// =============
configuration UUT (
	/* input  */ 		 .pin_CLOCK_USER       ( clk_xilinx_clk_gen                                 ),
	/* output */ 		 .pin_FMC_LA00_CC_N    ( port_out_pin                                       ),
	/* output */ 		 .pin_FMC_LA02_N       ( spi_master_mosi                                    ),
	/* output */ 		 .pin_FMC_LA02_P       ( spi_master_ss                                      ),
	/* output */ 		 .pin_FMC_LA03_P       ( spi_master_sclk                                    ),
	/* inout  */ 		 .pin_FMC_LA27_N       ( i2c_master_scl                                     ),
	/* inout  */ 		 .pin_FMC_LA27_P       ( i2c_master_sda                                     ),
	/* input  */ 		 .pin_GPIO_CPU_RESET   ( clk_xilinx_reset                                   ),
	/* output */ 		 .pin_USB_1_RX         ( uart_light_tx                                      ),
	/* input  */ 		 .pin_USB_1_TX         ( uart_light_rx                                      )
);

// ======
// Clocks
// ======
// 27.0MHz clock on clk_xilinx_clk_gen
initial clk_xilinx_clk_gen=0;
always #(18.519) clk_xilinx_clk_gen=~clk_xilinx_clk_gen;

endmodule

