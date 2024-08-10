/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: System sleep manager header.
 * Author:
 * Create: 2021-06-16
 */
#ifndef CONNECTIVITY_SLEEP_H
#define CONNECTIVITY_SLEEP_H

#include "std_def.h"
#include "chip_core_definition.h"
#include "connectivity_sleep_internal.h"

/**
 * @defgroup connectivity_libs_connectivity_sleep CONNECTIVITRY Sleep
 * @ingroup  connectivity_libs_connectivity
 * @{
 */
/**
 * @brief  Make the system suppress and sleep.
 */
void connectivity_sys_suppress_and_sleep(void);

/**
 * @}
 */
#endif