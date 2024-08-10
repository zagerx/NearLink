/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Mouse pin config port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-04-04， Create file. \n
 */

#ifndef MOUSE_SENSOR_PORT_H
#define MOUSE_SENSOR_PORT_H

#include "pinctrl_porting.h"

#define PIN_LEFT        S_MGPIO24
#define PIN_RIGHT       S_MGPIO25
#define PIN_MID         S_MGPIO29

#define QDEC_A          S_MGPIO27
#define QDEC_B          S_MGPIO28

#define MOUSE_SPI       SPI_BUS_2
#define SPI_PIN_MISO    S_MGPIO16
#define SPI_PIN_MOSI    S_MGPIO17
#define SPI_PIN_CLK     S_MGPIO18
#define SPI_PIN_CS      S_MGPIO19

#define SPI_PIN_MISO_PINMUX   HAL_PIO_FUNC_SPI2_M1
#define SPI_PIN_MOSI_PINMUX   HAL_PIO_FUNC_SPI2_M1
#define SPI_PIN_CLK_PINMUX    HAL_PIO_FUNC_SPI2_M1
#define SPI_PIN_CS_PINMUX     HAL_PIO_FUNC_SPI2_M1

#endif