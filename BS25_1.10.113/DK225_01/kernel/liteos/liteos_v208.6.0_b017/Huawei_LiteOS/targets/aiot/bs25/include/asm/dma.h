/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: liteos platform config files \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-25, Create file. \n
 */

#ifndef _DMA_H
#define _DMA_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void dma_cache_clean(uintptr_t start, uintptr_t end);
extern void dma_cache_inv(uintptr_t start, uintptr_t end);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DMA_H */
