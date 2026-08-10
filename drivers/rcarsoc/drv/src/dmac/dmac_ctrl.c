/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "interrupts.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "dmac/dmac_common.h"
#include "dmac_reg.h"

#include "FreeRTOS.h"
#include "task.h"

/* Return code of API */
#define drv_OK                                (0U)    /* API completed without any error. */
#define drv_FAIL                              (1U)    /* Failed. */

#define DMACP(X)    ((X) + 4)

extern int printf_delay(const char *format, ...);

uint16_t R_DMAC_RcarDmacExec(DMAC_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg);
uint16_t R_DMAC_RcarDmacStop(DMAC_t dev, uint8_t ch);
static int R_DMAC_SetInterruptCallback(uint8_t irq, IrqHandlerFn handler, void *irq_arg);
void R_DMAC_RcarInterruptHandler(void* p_context);

uint16_t R_DMAC_RcarCallBackSet(dmac_ctrl_t *const p_ctrl, void ( *p_callback)(void *), void * p_context)
{
    rDmacIrqCfg_t *args = (rDmacIrqCfg_t *)p_ctrl;
    Context_t * p_usr_context = (Context_t *)p_context;

    if (args == NULL) {
        return drv_FAIL;
    }

    /* Store callback and context */
    args->p_callback        = p_callback;

	/* Get channel */
    uint16_t irq_ch = args->irq_channel;
    R_DMAC_SetInterruptCallback(irq_ch, (void *)R_DMAC_RcarInterruptHandler, p_context);

    return drv_OK;
}

/*!
 *  @brief          InterruptHandler
 *  @details
 *  @param[in]      device_handle device handle.
 *  @param[in]      irq_channel   irq channel.
 *  @param[in]      irq_arg       user parameter.
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
void R_DMAC_RcarInterruptHandler(void* p_context)
{
    Context_t * p_usr_context = (Context_t *)p_context;
    rDmacIrqCfg_t *args = (rDmacIrqCfg_t *)p_usr_context->ctx;
    uint8_t dev = args->Unit;
    uint8_t ch = args->SubCh;
    uint32_t Value;
    uint8_t dev_p = DMACP(dev);

    /* Select Interrupt Ch */
    Value = R_RTDMAC_Get_RDMOR(dev_p);

    /* Check Address error Flag */
    if (0 != (Value & DRV_RTDMAC_REG_RDMOR_AE))
    {
        /* DMAC address error interrupt occurs during DMA transfer */
        R_RTDMAC_Clear_RDMCHCR_CAE(dev, ch);
        Value = R_RTDMAC_Clear_RDMCHCR_DE(dev, ch);
    }
    else
    {
        /* Check TE Interrupt */
        Value = R_RTDMAC_Get_RDMCHCR(dev, ch);
        if (0 != (Value & DRV_RTDMAC_REG_RDMCHCR_TE))
        {
            R_RTDMAC_Clear_RDMCHCR_TE(dev, ch);
            if (DRV_RTDMAC_REG_RDMCHCR_DPM_REP != (Value & DRV_RTDMAC_REG_RDMCHCR_DPM))
            {
                Value = R_RTDMAC_Clear_RDMCHCR_DE(dev, ch);
            }
        }
        /* Check DSE Interrupt */
        else if (0 != (Value & DRV_RTDMAC_REG_RDMCHCR_DSE))
        {
            R_RTDMAC_Clear_RDMCHCR_DSE(dev, ch);
            if (DRV_RTDMAC_REG_RDMCHCR_DPM_READ == (Value & DRV_RTDMAC_REG_RDMCHCR_DPM))
            {
                Value = R_RTDMAC_Get_RDMDPCR(dev, ch);
            }
        }
    }

    if (args->p_callback != NULL) {
        args->p_callback(args->p_context);
    }
}

static int R_DMAC_SetInterruptCallback(uint8_t irq, IrqHandlerFn handler, void *irq_arg)
{
    (void)irq;
    Context_t * p_usr_context = (Context_t *)irq_arg;
    rDmacIrqCfg_t *args = (rDmacIrqCfg_t *)p_usr_context->ctx;

    /* Set Handler for Irq */
    Irq_SetupEntry(args->irq_channel, handler, (Context_t *)irq_arg);
    /* Set priority for Irq */
    Irq_SetPriority(args->irq_channel, IPRIORITY(3));
    /* Enable Irq */
    Irq_Enable(args->irq_channel);

    // Work around to fix hang IRQ.
    for(int i = 0; i < 1000; i++)
    {
        __asm__ volatile("nop");
    }

    return 0;
}

/*!
 *  @brief          DMA intialize
 *  @details
 *  @param[in]      mode
 *  @return         drv_OK
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_DMAC_RcarDmacCtrlInit(DMAC_t dev, rDmacPriorityMode_t mode)
{
    uint8_t dev_p;

    /* Set Priority Mode */
    dev_p = DMACP(dev);
    R_RTDMAC_Update_RDMOR(dev_p, mode);
    return drv_OK;
}

/*!
 *  @brief          DMA Execute
 *  @details
 *  @param[in]      cfg
 *  @param[in]      descCfg
 *  @param[in]      cb
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @return         drv_RTDMAC_CH_BUSY
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_DMAC_RcarDmacExec(DMAC_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg)
{
    uint32_t Value;
    uint8_t dev_p;
    uint16_t ret = drv_OK;

    dev_p = DMACP(dev);         // Convert to RT_DMAC Public's register
    /* RTDMAC Initial Check */
    Value = R_RTDMAC_Get_RDMOR(dev_p);

    if (0 == (Value & DRV_RTDMAC_REG_RDMOR_DME))
    {
        ret = drv_FAIL;
    }
    else {
        /* CH Busy Check */
        Value = R_RTDMAC_Get_RDMCHCR(dev, ch);
        if (0 != (Value & DRV_RTDMAC_REG_RDMCHCR_DE))
        {
	        LogDebug("-----CH_BUSY-----\n");
            ret = drv_FAIL;
        }
        else
        {
            /* cfg Set */
            R_RTDMAC_Update_RDMCHCLR(dev, ch);
            R_RTDMAC_Update_RDMSAR(dev, ch, cfg->mSrcAddr);
            R_RTDMAC_Update_RDMDAR(dev, ch, cfg->mDestAddr);
            R_RTDMAC_Update_RDMTCR(dev, ch, cfg->mTransferCount);
            R_RTDMAC_Update_RDMRS_ID(dev, ch, cfg->mSourceRequest);

            if (DRV_DMAC_DMA_NO_DESCRIPTOR != (cfg->mDMAMode && descCfg))
            {
                /* descCfg Set */
                R_RTDMAC_Update_RDMCHCRB_RST(dev, ch);
                R_RTDMAC_Update_RDMDPBASE(dev, ch, descCfg->mDescBaseAddr);
            }

            R_RTDMAC_Update_RDMCHCRB(dev, ch, cfg, descCfg);
            if ( DRV_DMAC_DMA_DESC_READOUT == cfg->mDMAMode )
            {
                R_RTDMAC_Update_RDMDPCR(dev, ch, descCfg->mDescIndex);
            }

            /* DMA Start */
            R_RTDMAC_Update_RDMCHCR(dev, ch, cfg, descCfg);

            // LogDebug("DMA Start: SrcAddr=0x%lx, DestAddr=0x%lx\n", cfg->mSrcAddr, cfg->mDestAddr);
        }
    }
    return ret;
}

/*!
 *  @brief          DMA Stop
 *  @details
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_DMAC_RcarDmacStop(DMAC_t dev, uint8_t ch)
{
    uint32_t Value;
    uint8_t dev_p;
    uint16_t ret = drv_OK;

    // Convert to RT_DMAC Public's register
    dev_p = DMACP(dev);
    /* RTDMAC Initial Check */
    Value = R_RTDMAC_Get_RDMOR(dev_p);
    if (0 == (Value & DRV_RTDMAC_REG_RDMOR_DME))
    {
        ret = drv_FAIL;
    }
    else
    {
        /* DMA Stop */
        ret = R_RTDMAC_Clear_RDMCHCR_DE(dev, ch);
        R_RTDMAC_Update_RDMCHCLR(dev, ch);
    }
    return ret;
}
