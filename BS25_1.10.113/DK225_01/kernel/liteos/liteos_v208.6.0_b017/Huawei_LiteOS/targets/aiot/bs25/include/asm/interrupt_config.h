/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: liteos platform config files \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25, Create file. \n
 */

#ifndef _ASM_INTERRUPT_CONFIG_H
#define _ASM_INTERRUPT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*  26 + 27 */
#define NUM_HAL_INTERRUPT_UART           53

#define NUM_HAL_INTERRUPT_TIMER          7UL

#define OS_TICK_INT_NUM                  NUM_HAL_INTERRUPT_TIMER

#define NUM_HAL_INTERRUPT_NMI            12

#define OS_RISCV_SYS_VECTOR_CNT          26

#define OS_RISCV_MIE_IRQ_VECTOR_CNT      6

#define OS_RISCV_LOCAL_IRQ_VECTOR_CNT   (OS_RISCV_MIE_IRQ_VECTOR_CNT + OS_RISCV_SYS_VECTOR_CNT)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ASM_INTERRUPT_CONFIG_H */
