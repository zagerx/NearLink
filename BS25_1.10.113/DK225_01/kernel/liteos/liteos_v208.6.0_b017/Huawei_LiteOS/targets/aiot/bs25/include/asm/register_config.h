/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: liteos platform config files \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25, Create file. \n
 */

#ifndef _ASM_REGISTER_CONFIG_H
#define _ASM_REGISTER_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define REG_BASE_PERI_CTRL              0x20000
#define NMI_REG_BASE                    (REG_BASE_PERI_CTRL + 0xC18)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ASM_REGISTER_CONFIG_H */
