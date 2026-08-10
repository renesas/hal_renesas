/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "dmac_ctrl_common.h"

extern int printf_delay(const char *format, ...);

/* Return code of API */
#define drv_OK                                (0U)    /* API completed without any error. */
#define drv_FAIL                              (1U)    /* Failed. */

uint16_t R_SYSDMAC_RcarDmacCtrlInit(DMAC_t dev, rDmacPriorityMode_t mode)
{
    return R_DMAC_RcarDmacCtrlInit(dev, mode);
}

uint16_t R_SYSDMAC_RcarDmacExec(DMAC_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg)
{
    uint8_t Value;
    uint16_t ret = drv_OK;

    Value = R_DMAC_RcarDmacExec(dev, ch, cfg, descCfg);
    if (Value != 0U) {
        LogDebug("SYSDMAC execution failed\n");
        return Value;
    }
    
    return ret;
}

uint16_t R_SYSDMAC_RcarDmacStop(DMAC_t dev, uint8_t ch)
{
    uint32_t Value;
    uint16_t ret = drv_OK;

    Value = R_DMAC_RcarDmacStop(dev, ch);
    if (Value != 0U) {
        LogDebug("SYSDMAC stop failed\n");
        return Value;
    }
    
    return ret;
}

uint16_t R_SYSDMAC_RcarCallBackSet(dmac_ctrl_t * const p_ctrl, void ( *p_callback)(void *), void * const p_context)
{
    uint32_t Value;
    uint16_t ret = drv_OK;

    Value = R_DMAC_RcarCallBackSet(p_ctrl, p_callback, p_context);

    if (Value != 0U) {
        LogDebug("SYSDMAC IRQ callback failed\n");
        return Value;
    }

    return ret;
}
