/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "dmac/dmac_common.h"
#include "dmac_reg.h"

extern int printf_delay(const char *format, ...);

/* Return code of API */
#define drv_OK			(0U)    /* API completed without any error. */
#define drv_FAIL		(1U)    /* Failed. */

/***********************************************************
Macro definitions
***********************************************************/

/***********************************************************
Typedef definitions
***********************************************************/

/***********************************************************
Exported global variables (to be accessed by other files)
***********************************************************/

/***********************************************************
Private global variables and functions
***********************************************************/
static volatile uint32_t fBaseAddress[DRV_RTDMAC_UNIT_NUM] = {
    DRV_RTDMAC_REG_ADDR_DMAC0,
    DRV_RTDMAC_REG_ADDR_DMAC1,
    DRV_RTDMAC_REG_ADDR_DMAC2,
    DRV_RTDMAC_REG_ADDR_DMAC3,
    DRV_RTDMAC_REG_ADDR_DMACP0,
    DRV_RTDMAC_REG_ADDR_DMACP1,
    DRV_RTDMAC_REG_ADDR_DMACP2,
    DRV_RTDMAC_REG_ADDR_DMACP3,
    DRV_SYSDMAC_REG_ADDR_DMAC0,
    DRV_SYSDMAC_REG_ADDR_DMAC1,
    DRV_SYSDMAC_REG_ADDR_DMAC2,
    DRV_SYSDMAC_REG_ADDR_DMAC3,
    DRV_SYSDMAC_REG_ADDR_DMACP0,
    DRV_SYSDMAC_REG_ADDR_DMACP1,
    DRV_SYSDMAC_REG_ADDR_DMACP2,
    DRV_SYSDMAC_REG_ADDR_DMACP3
#ifdef CONFIG_USE_RCARSOC_DRV_DMAC
    ,
    DRV_ADMAC_REG_ADDR_DMAC0,
    DRV_ADMAC_REG_ADDR_DMAC1,
    DRV_ADMAC_REG_ADDR_DMAC2,
    DRV_ADMAC_REG_ADDR_DMAC3,
    DRV_ADMAC_REG_ADDR_DMACP0,
    DRV_ADMAC_REG_ADDR_DMACP1,
    DRV_ADMAC_REG_ADDR_DMACP2,
    DRV_ADMAC_REG_ADDR_DMACP3,
    DRV_SCPDMAC_REG_ADDR_DMAC0,
    DRV_SCPDMAC_REG_ADDR_DMAC1,
    NULL,
    NULL,
    DRV_SCPDMAC_REG_ADDR_DMACP0,
    DRV_SCPDMAC_REG_ADDR_DMACP1,
    NULL,
    NULL,
#endif /* CONFIG_USE_RCARSOC_DRV_DMAC */
};

const uint32_t rDmacCH_MaskTable[] = {
    DRV_RTDMAC_REG_MASK_CH0,      /* CH0 use bit mask */
    DRV_RTDMAC_REG_MASK_CH1,      /* CH1 use bit mask */
    DRV_RTDMAC_REG_MASK_CH2,      /* CH2 use bit mask */
    DRV_RTDMAC_REG_MASK_CH3,      /* CH3 use bit mask */
    DRV_RTDMAC_REG_MASK_CH4,      /* CH4 use bit mask */
    DRV_RTDMAC_REG_MASK_CH5,      /* CH5 use bit mask */
    DRV_RTDMAC_REG_MASK_CH6,      /* CH6 use bit mask */
    DRV_RTDMAC_REG_MASK_CH7,      /* CH7 use bit mask */
    DRV_RTDMAC_REG_MASK_CH8,      /* CH8 use bit mask */
    DRV_RTDMAC_REG_MASK_CH9,      /* CH9 use bit mask */
    DRV_RTDMAC_REG_MASK_CH10,     /* CH10 use bit mask */
    DRV_RTDMAC_REG_MASK_CH11,     /* CH11 use bit mask */
    DRV_RTDMAC_REG_MASK_CH12,     /* CH12 use bit mask */
    DRV_RTDMAC_REG_MASK_CH13,     /* CH13 use bit mask */
    DRV_RTDMAC_REG_MASK_CH14,     /* CH14 use bit mask */
    DRV_RTDMAC_REG_MASK_CH15,     /* CH15 use bit mask */
};

static const uint32_t rDmacPriorityMode_Table[] = {
    DRV_RTDMAC_REG_PRIO_FIX,
    DRV_RTDMAC_REG_PRIO_RR,
};

static const uint32_t rDmacTransferUnit_Table[] = {
    DRV_RTDMAC_REG_TRANS_UNIT_1BYTE,
    DRV_RTDMAC_REG_TRANS_UNIT_2BYTE,
    DRV_RTDMAC_REG_TRANS_UNIT_4BYTE,
    DRV_RTDMAC_REG_TRANS_UNIT_8BYTE,
    DRV_RTDMAC_REG_TRANS_UNIT_16BYTE,
    DRV_RTDMAC_REG_TRANS_UNIT_32BYTE,
    DRV_RTDMAC_REG_TRANS_UNIT_64BYTE,
};


/*!
 *  @brief          Write RTDMAC register with the SubChannel number
 *  @details
 *  @param[in]      dev  : dev number
 *  @param[in]      ch   : Sub Ch number
 *  @param[in]      reg  : RTDMAC register
 *  @param[in]      data : data to write
 *  @return         None
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static void R_RTDMAC_WriteReg32(uint8_t dev, uint8_t ch, uint32_t reg, uint32_t data)
{
    uint32_t regAddr = fBaseAddress[dev] + ch * DRV_RTDMAC_CH_REG_ADDR + reg;
    *(volatile uint32_t*)regAddr = data;
}

/*!
 *  @brief          Read RTDMAC register with the SubChannel number
 *  @details
 *  @param[in]      dev  : dev number
 *  @param[in]      ch   : Sub Ch number
 *  @param[in]      reg  : RTDMAC register
 *  @return         data : data to be read
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static uint32_t R_RTDMAC_ReadReg32(uint8_t dev, uint8_t ch, uint32_t reg)
{
    uint32_t regAddr = fBaseAddress[dev] + ch * DRV_RTDMAC_CH_REG_ADDR + reg;
    uint32_t data;

    data = *(volatile uint32_t*)regAddr;
    return data;
}

/*!
 *  @brief          Write RTDMAC register with the dev number
 *  @details
 *  @param[in]      dev  : dev number
 *  @param[in]      reg  : RTDMAC register
 *  @param[in]      data : data to write
 *  @return         None
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static void R_RTDMAC_UnitWriteReg16(uint8_t dev, uint32_t reg, uint32_t data)
{
    uint32_t regAddr = fBaseAddress[dev] + reg;
    *(volatile uint16_t*)regAddr = data;
}

/*!
 *  @brief          Read RTDMAC register with the dev number
 *  @details
 *  @param[in]      dev  : dev number
 *  @param[in]      reg  : RTDMAC register
 *  @return         data : data to be read
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static uint16_t R_RTDMAC_UnitReadReg16(uint8_t dev, uint32_t reg)
{
    uint32_t regAddr = fBaseAddress[dev] + reg;
    uint16_t data;
    data = *(volatile uint16_t*)regAddr;
    return data;
}

/*!
 *  @brief          Write RTDMAC register with the SubChannel number
 *  @details
 *  @param[in]      dev  : dev number
 *  @param[in]      ch   : Sub Ch number
 *  @param[in]      reg  : RTDMAC register
 *  @param[in]      data : data to write
 *  @return         None
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static void R_RTDMAC_WriteReg16(uint8_t dev, uint8_t ch, uint32_t reg, uint32_t data)
{
    uint32_t regAddr = fBaseAddress[dev] + ch * DRV_RTDMAC_CH_REG_ADDR + reg;
    *(volatile uint16_t*)regAddr = data;
}

/**
 * Get operation register RDMOR.
 *
 * The R_RTDMAC_Get_RDMOR() function.
 * @param[in]  dev : dev number
 * @return     operation register RDMOR.
 * @attention
 */
uint32_t R_RTDMAC_Get_RDMOR(uint8_t dev)
{
    return (uint32_t)R_RTDMAC_UnitReadReg16(dev, RDMOR);
}

/**
 * Get channel control register RDMCHCR.
 *
 * The R_RTDMAC_Get_RDMCHCR() function.
 * @param[in]  dev : dev number
 * @param[in]  ch  : Sub Ch number
 * @return     channel control register RDMCHCR.
 * @attention
 */
uint32_t R_RTDMAC_Get_RDMCHCR(uint8_t dev, uint8_t ch)
{
    return R_RTDMAC_ReadReg32(dev, ch, RDMCHCR);
}

/**
 * @brief       The R_RTDMAC_Get_RDMDPCR() function.
 * @details     Get descriptor control register RDMDPCR.
 * @param[in]   dev : dev number
 * @param[in]   ch  : Sub Ch number
 * @return      channel control register RDMDPCR.
 * @attention
 */
uint32_t R_RTDMAC_Get_RDMDPCR(uint8_t dev, uint8_t ch)
{
    return R_RTDMAC_ReadReg32(dev, ch, RDMDPCR);
}

/**
 *  @brief          Update RDMOR
 *  @details
 *  @param[in]      dev  : dev number
 *  @param[in]      mode : priority mode
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMOR(uint8_t dev, rDmacPriorityMode_t mode)
{
    uint8_t Value;
    regRDMOR_t rRDMOR;

    Value = rDmacPriorityMode_Table[mode];
    rRDMOR.INT = R_RTDMAC_UnitReadReg16(dev, RDMOR);

    rRDMOR.bit.PR = Value;
    rRDMOR.bit.DME = DRV_RTDMAC_RDMOR_DME_ENABLE;
    R_RTDMAC_UnitWriteReg16(dev, RDMOR, rRDMOR.INT);
}

/**
 *  @brief          Update RDMCHCLR
 *  @details
 *  @param[in]      dev : dev number
 *  @param[in]      ch  : Sub Ch number
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMCHCLR(uint8_t dev, uint8_t ch)
{
    regRDMCHCLR_t rRDMCHCLR;

    rRDMCHCLR.INT = 1;
    R_RTDMAC_WriteReg32(dev, ch, RDMCHCLR, rRDMCHCLR.INT);
}

/**
 *  @brief          Update RDMSAR
 *  @details
 *  @param[in]      dev   : dev number
 *  @param[in]      ch    : Sub Ch number
 *  @param[in]      Value : src data address
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMSAR(uint8_t dev, uint8_t ch, uint64_t Value)
{
    R_RTDMAC_WriteReg32(dev, ch, RDMSAR, (uint32_t)(Value & DRV_RTDMAC_REG_MASK_LOWER_RDMSAR));
    R_RTDMAC_WriteReg32(dev, ch, RDMFIXSAR, ((Value >> 32) & DRV_RTDMAC_REG_MASK_UPPER_RDMSAR));
}

/**
 *  @brief          Update RDMDAR
 *  @details
 *  @param[in]      dev   : dev number
 *  @param[in]      ch    : Sub Ch number
 *  @param[in]      Value : dst data address
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMDAR(uint8_t dev, uint8_t ch, uint64_t Value)
{
    R_RTDMAC_WriteReg32(dev, ch, RDMDAR, (uint32_t)(Value & DRV_RTDMAC_REG_MASK_LOWER_RDMDAR));
    R_RTDMAC_WriteReg32(dev, ch, RDMFIXDAR, ((Value >> 32) & DRV_RTDMAC_REG_MASK_UPPER_RDMDAR));
}

/**
 *  @brief          Update RDMTCR
 *  @details
 *  @param[in]      dev   : dev number
 *  @param[in]      ch    : Sub Ch number
 *  @param[in]      Value : dst data address
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMTCR(uint8_t dev, uint8_t ch, uint32_t Value)
{
    regRDMTCR_t rRDMTCR;

    rRDMTCR.INT = R_RTDMAC_ReadReg32(dev, ch, RDMTCR);
    rRDMTCR.bit.TCR = Value;
    R_RTDMAC_WriteReg32(dev, ch, RDMTCR,  rRDMTCR.INT);
}


/**
 *  @brief          Update RDMCHCR
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @param[in]      cfg     : dmac config
 *  @param[in]      descCfg : desc config
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMCHCR(uint8_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg)
{
    uint8_t Value;
    regRDMCHCR_t rRDMCHCR;

    rRDMCHCR.INT = R_RTDMAC_ReadReg32(dev, ch, RDMCHCR);
    Value = cfg->mDMAMode;
    if (DRV_DMAC_DMA_DESC_INFINITE == Value)
    {
        Value = DRV_DMAC_DMA_DESC_READOUT;
    }

    if ((DRV_DMAC_DMA_DESC_READOUT != Value))
    {
        rRDMCHCR.bit.IE = DRV_RTDMAC_RDMCHCR_IE_ENABLE;
    }

    rRDMCHCR.bit.DPM = Value;
    rRDMCHCR.bit.SM = cfg->mSrcAddrMode;
    rRDMCHCR.bit.DM = cfg->mDestAddrMode;
    Value = rDmacTransferUnit_Table[cfg->mTransferUnit];
    rRDMCHCR.bit.TS01 = Value & DRV_RTDMAC_REG_MASK_RDMCHCR_TS;
    rRDMCHCR.bit.TS23 = (Value >> 2) & DRV_RTDMAC_REG_MASK_RDMCHCR_TS;
    if (DRV_RTDMAC_MEMORY == cfg->mResource)
    {
        rRDMCHCR.bit.RS = DRV_RTDMAC_RDMCHCR_RS_AUTO_REQEST;
    }
    else
    {
        rRDMCHCR.bit.RS = DRV_RTDMAC_RDMCHCR_RS_EXTENDED_RESOUSE;
    }

    if (DRV_DMAC_DMA_NO_DESCRIPTOR != cfg->mDMAMode)
    {
        rRDMCHCR.bit.RPT3 = descCfg->mDescUpdate.mSrcAddrUpdate;
        rRDMCHCR.bit.RPT2 = descCfg->mDescUpdate.mDestAddrUpdate;
        rRDMCHCR.bit.RPT1 = descCfg->mDescUpdate.mTransCountUpdate;
        rRDMCHCR.bit.RPT0 = descCfg->mDescUpdate.mCHCRUpdate;
        rRDMCHCR.bit.DPB = descCfg->mDescRead1st;
        rRDMCHCR.bit.DSIE = descCfg->mStateEndEnable;
    }
    rRDMCHCR.bit.CAIE = DRV_RTDMAC_RDMCHCR_CAIE_ENABLE;
#ifndef CONFIG_USE_RCARSOC_DRV_DMAC
    rRDMCHCR.bit.DE = DRV_RTDMAC_RDMCHCR_DE_ENABLE;
#endif
    R_RTDMAC_WriteReg32(dev, ch, RDMCHCR, rRDMCHCR.INT);
}

/**
 *  @brief          Clear RDMCHCR_DE
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @return         drv_OK
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
uint16_t R_RTDMAC_Clear_RDMCHCR_DE(uint8_t dev, uint8_t ch)
{
    uint8_t Retry;
    uint32_t Value;
    uint16_t ret;
    regRDMCHCR_t rRDMCHCR;

    Retry = DRV_RTDMAC_CLR_DE_RETRY;
    while (1)
    {
        rRDMCHCR.INT = R_RTDMAC_ReadReg32(dev, ch, RDMCHCR);
        rRDMCHCR.bit.DE = DRV_RTDMAC_RDMCHCR_DE_DISABLE;
        R_RTDMAC_WriteReg32(dev, ch, RDMCHCR, rRDMCHCR.INT);

        Value = R_RTDMAC_Get_RDMCHCR(dev, ch);
        if (0 == (Value & DRV_RTDMAC_REG_RDMCHCR_DE))
        {
            ret = drv_OK;
            break;
        }
        if (--Retry == 0)
        {
            ret = drv_FAIL;
            break;
        }
    }
    return ret;
}

/**
 *  @brief          Clear RDMCHCR_CAE
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Clear_RDMCHCR_CAE(uint8_t dev, uint8_t ch)
{
    regRDMCHCR_t rRDMCHCR;

    rRDMCHCR.INT = R_RTDMAC_ReadReg32(dev, ch, RDMCHCR);
    rRDMCHCR.bit.CAE = DRV_RTDMAC_RDMCHCR_CAE_CLEAR;
    R_RTDMAC_WriteReg32(dev, ch, RDMCHCR, rRDMCHCR.INT);
}

/**
 *  @brief          Clear RDMCHCR_DSE
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Clear_RDMCHCR_DSE(uint8_t dev, uint8_t ch)
{
    regRDMCHCR_t rRDMCHCR;

    rRDMCHCR.INT = R_RTDMAC_ReadReg32(dev, ch, RDMCHCR);
    rRDMCHCR.bit.DSE = DRV_RTDMAC_RDMCHCR_DSE_CLEAR;
    R_RTDMAC_WriteReg32(dev, ch, RDMCHCR, rRDMCHCR.INT);
}

/**
 *  @brief          Clear RDMCHCR_TE
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Clear_RDMCHCR_TE(uint8_t dev, uint8_t ch)
{
    regRDMCHCR_t rRDMCHCR;

    rRDMCHCR.INT = R_RTDMAC_ReadReg32(dev, ch, RDMCHCR);
    rRDMCHCR.bit.TE = DRV_RTDMAC_RDMCHCR_TE_CLEAR;
    R_RTDMAC_WriteReg32(dev, ch, RDMCHCR, rRDMCHCR.INT);
}

/**
 *  @brief          Update RDMCHCRB
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @param[in]      cfg     : dmac config
 *  @param[in]      descCfg : desc config
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */

void R_RTDMAC_Update_RDMCHCRB(uint8_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg)
{
    regRDMCHCRB_t rRDMCHCRB;

    rRDMCHCRB.INT = R_RTDMAC_ReadReg32(dev, ch, RDMCHCRB);
    rRDMCHCRB.bit.SLM = cfg->mLowSpeed;
    rRDMCHCRB.bit.PRI = cfg->mPrioLevel;
    if (DRV_DMAC_DMA_NO_DESCRIPTOR != cfg->mDMAMode) {
        rRDMCHCRB.bit.DSIEEN = 1U;
        rRDMCHCRB.bit.DCNT = descCfg->mDescCount - 1;
    }
    R_RTDMAC_WriteReg32(dev, ch, RDMCHCRB, rRDMCHCRB.INT);
}

/**
 *  @brief          Update RDMCHCRB_RST
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */

void R_RTDMAC_Update_RDMCHCRB_RST(uint8_t dev, uint8_t ch)
{
    regRDMCHCRB_t rRDMCHCRB;

    /* Update RDMCHCRB.DRST */
    rRDMCHCRB.INT = R_RTDMAC_ReadReg32(dev, ch, RDMCHCRB);
    rRDMCHCRB.bit.DRST = 1;
    R_RTDMAC_WriteReg32(dev, ch, RDMCHCRB, rRDMCHCRB.INT);
}

/**
 *  @brief          Update RDMRS.ID
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @param[in]      Value   : write data
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMRS_ID(uint8_t dev, uint8_t ch, uint8_t Value)
{
    R_RTDMAC_WriteReg16(dev, ch, RDMRS, Value);
}

/**
 *  @brief          Update RDMDPBASE
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @param[in]      Value   : write data
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMDPBASE(uint8_t dev, uint8_t ch, uint64_t Value)
{
    regRDMDPBASE_t rRDMDPBASE;

    rRDMDPBASE.INT = (uint32_t)(Value & DRV_RTDMAC_REG_MASK_LOWER_RDMDPBASE);

    /* Update RDMDPBASE.SEL to 1: External memory is used */
    rRDMDPBASE.bit.SEL = 1U;
    R_RTDMAC_WriteReg32(dev, ch, RDMDPBASE, rRDMDPBASE.INT);
    R_RTDMAC_WriteReg32(dev, ch, RDMFIXDPBASE, ((Value >> 32) & DRV_RTDMAC_REG_MASK_UPPER_RDMDPBASE));
}

/**
 *  @brief          Update RDMDPCR
 *  @details
 *  @param[in]      dev     : dev number
 *  @param[in]      ch      : Sub Ch number
 *  @param[in]      Value   : write data
 *  @return         void
 *  @par    Refer   (none)
 *  @par    Modify  (none)
 */
void R_RTDMAC_Update_RDMDPCR(uint8_t dev, uint8_t ch, uint8_t Value)
{
    regRDMDPCR_t rRDMDPCR;

    rRDMDPCR.INT = R_RTDMAC_ReadReg32(dev, ch, RDMDPCR);
    rRDMDPCR.bit.DIPT_7_0 = Value;
    R_RTDMAC_WriteReg32(dev, ch, RDMDPCR, rRDMDPCR.INT);
}

