/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

/**
 * @defgroup RTDMA_PRIVATE RTDMA PRIVATE
 * @{
 * @brief Work around for RTDMA issue on X5H platform.
 *
 */

#ifndef R_RTDMA_PRIVATE_H
#define R_RTDMA_PRIVATE_H

#include "dmac_ctrl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Logging Function include. */
#include "logging_stack.h"

/* RTDMA Workaround functions */
static inline uint16_t R_RTDMAC_WA_16_BYTES(rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg)
{
    /* Workaround for temporarily limitation of rt-dmac.
     * Workaround: transfer unit must be 16 bytes or less.
     */
    if ((cfg->mTransferUnit == DRV_RTDMAC_TRANS_UNIT_32BYTE) ||
        (cfg->mTransferUnit == DRV_RTDMAC_TRANS_UNIT_64BYTE) ||
        (cfg->mTransferUnit == DRV_RTDMAC_TRANS_UNIT_MAX))
    {
        LogError(("RTDMAC error: transfer unit must be 16 bytes or less\n"));
        return drv_FAIL;
    }

    /* Workaround for temporarily limitation of rt-dmac.
     * Workaround: source address must be 16-byte aligned to avoid 16-byte boundary crossing.
     */
    /* Workaround for no descriptor mode */
    if (cfg->mDMAMode == DRV_DMAC_DMA_NO_DESCRIPTOR)
    {
        if ((cfg->mSrcAddr & 0xFU) != 0U)
        {
            LogError(("RTDMAC error: source address 0x%08x must be 16-byte aligned\n", cfg->mSrcAddr));
            return drv_FAIL;
        }
    }
    /* Workaround for descriptor mode */
    else
    {
        rDmacDescMemCfg_t *p_desc_table = (rDmacDescMemCfg_t *)(uintptr_t)descCfg->mDescBaseAddr; // descriptor table base address
        for (int i = 0; i < descCfg->mDescCount; i++)
        {
            if ((p_desc_table[i].SAR & 0xFU) != 0U)
            {
                LogError(("RTDMAC error: descriptor[%d].SAR=0x%08x must be 16-byte aligned\n", i, p_desc_table[i].SAR));
                return drv_FAIL;
            } 
        }
    }

    return drv_OK;
}

#ifdef __cplusplus
}
#endif

#endif /* R_RTDMA_PRIVATE_H */
