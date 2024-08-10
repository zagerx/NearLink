/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: lcd pin config port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-04， Create file. \n
 */

#ifndef LCD_SPI_PORT_H
#define LCD_SPI_PORT_H

#include "pinctrl_porting.h"
#include "spi_porting.h"

#define LCD_SPI         SPI_BUS_3

#define SPI_PIN_MISO    S_MGPIO20
#define SPI_PIN_MOSI    S_MGPIO21
#define SPI_PIN_CLK     S_MGPIO22
#define SPI_PIN_CS      S_MGPIO23

#define LCD_TE          S_MGPIO25
#define LCD_RESERT      S_MGPIO19
#define LCD_DC          S_MGPIO18

#define LCD_POWN        S_AGPIO4
#define LCD_BACK        S_AGPIO5

#define SPI_PIN_MISO_PINMUX   HAL_PIO_FUNC_SPI2_M1
#define SPI_PIN_MOSI_PINMUX   HAL_PIO_FUNC_SPI2_M1
#define SPI_PIN_CLK_PINMUX    HAL_PIO_FUNC_SPI2_M1
#define SPI_PIN_CS_PINMUX     HAL_PIO_FUNC_SPI2_M1
#define dma_remap(addr) ((addr) - 0x80000000)
#endif