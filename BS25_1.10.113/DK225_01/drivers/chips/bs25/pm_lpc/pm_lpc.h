/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides low power control header \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25， Create file. \n
 */

#ifndef PM_SYS_H
#define PM_SYS_H

#include <stdbool.h>
#include "common_def.h"
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup middleware_utils_pm_lpc PM lpc
 * @ingroup  middleware_utils_pm
 * @{
 */

typedef enum lpc_ccrg_clk_spi_used {
    LPC_CCRG_SPI_USED_SPI0 = 0,
    LPC_CCRG_SPI_USED_SPI1,
    LPC_CCRG_SPI_USED_SPI2,
    LPC_CCRG_SPI_USED_SPI3,
    LPC_CCRG_SPI_USED_SPI4,
} lpc_ccrg_clk_spi_used_t;

typedef enum lpc_mclken_sec_used {
    LPC_MCLKEN_SEC_USED_HASH,
    LPC_MCLKEN_SEC_USED_TRNG,
} lpc_sec_clken_used_t;

typedef enum lpc_ccrg_clk_pdm_used {
    LPC_CCRG_CLK_PDM_USED_PDM,
    LPC_CCRG_CLK_PDM_USED_I2S,
} lpc_ccrg_clk_pdm_used_t;

typedef enum lpc_ulp_clkldo_used {
    LPC_ULP_CLKLDO_USED_FNPLL,
    LPC_ULP_CLKLDO_USED_ULPFLL,
} lpc_ulp_clkldo_used_t;

/**
 * @brief  Enable of disable ccrg_clk_spi.
 * @param  dev Device of using ccrg_clk_spi.
 * @param  en true: enable, false: disable.
 */
void pm_lpc_ccrg_clk_spi_enable(lpc_ccrg_clk_spi_used_t dev, bool en);

/**
 * @brief  Enable of disable mclken_sec.
 * @param  dev Device of using mclken_sec.
 * @param  en true: enable, false: disable.
 */
void pm_lpc_mclken_sec_enable(lpc_sec_clken_used_t dev, bool en);

/**
 * @brief  Enable of disable ccrg_clk_pdm.
 * @param  dev Device of using ccrg_clk_pdm.
 * @param  en true: enable, false: disable.
 */
void pm_lpc_ccrg_clk_pdm_enable(lpc_ccrg_clk_pdm_used_t dev, bool en);

/**
 * @brief  Enable of disable ulp_clkldo.
 * @param  dev Device of using ulp_clkldo.
 * @param  en true: enable, false: disable.
 */
void pm_lpc_ulp_clkldo_enable(lpc_ulp_clkldo_used_t dev, bool en);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif