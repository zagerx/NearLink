/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: liteos platform config files \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25, Create file. \n
 */

#ifndef _MEMMAP_CONFIG_H
#define _MEMMAP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void *g_intheap_begin;
extern void *g_intheap_size;
extern void *g_usb_ram_begin;
extern void *g_usb_ram_size;

#define OS_SYS_MEM_ADDR         (&g_intheap_begin)
#define OS_SYS_MEM_SIZE         (unsigned int)(&g_intheap_size)
#define OS_USB_MEM_ADDR         ((void *)&g_usb_ram_begin)
#define OS_USB_MEM_SIZE         ((uintptr_t)&g_usb_ram_size)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MEMMAP_CONFIG_H */
