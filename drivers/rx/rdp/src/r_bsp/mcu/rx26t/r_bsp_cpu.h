/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_bsp_cpu.h
* Description  : This module implements CPU specific functions. An example is enabling/disabling interrupts.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 28.02.2023 1.00     First Release
*         : 21.11.2023 1.01     Added bsp_bus_priority_initialize function.
*         : 26.02.2025 1.02     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef CPU_H
#define CPU_H

/* Control pattern specified as argument of R_BSP_VoltageLevelSetting function. */
#define BSP_VOL_RIIC_4_5V_OROVER            (0x10)
#define BSP_VOL_RIIC_UNDER_4_5V             (0x20)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* The different types of registers that can be protected. */
typedef enum
{
    /* PRC0
       Enables writing to the registers related to the clock generation circuit: SCKCR, SCKCR2, SCKCR3, PLLCR,
       PLLCR2, MOSCCR, LOCOCR, ILOCOCR, HOCOCR, HOCOCR2, OSTDCR, OSTDSR. */
    BSP_REG_PROTECT_CGC = 0,

    /* PRC1
       Enables writing to the registers related to operating modes, low power consumption, the clock generation circuit,
       and software reset: SYSCR1, VOLSR, SBYCR, MSTPCRA, MSTPCRB, MSTPCRC, MSTPCRD, RSTCKCR,
       MOSCWTCR, MOFCR, HOCOPCR, SWRR. */
    BSP_REG_PROTECT_LPC_CGC_SWR,

    /* PRC3
       Enables writing to the registers related to the LVD:LVCMPCR, LVDLVLR, LVD1CR0, LVD1CR1, LVD1SR, LVD2CR0,
       LVD2CR1, LVD2SR. */
    BSP_REG_PROTECT_LVD,

    /* MPC.PWPR
       Enables writing to MPC's PFS registers. */
    BSP_REG_PROTECT_MPC,

    /* This entry is used for getting the number of enum items. This must be the last entry. DO NOT REMOVE THIS ENTRY!*/
    BSP_REG_PROTECT_TOTAL_ITEMS
} bsp_reg_protect_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
void     R_BSP_InterruptsDisable(void);
void     R_BSP_InterruptsEnable(void);
uint32_t R_BSP_CpuInterruptLevelRead(void);
bool     R_BSP_CpuInterruptLevelWrite(uint32_t level);
void     R_BSP_RegisterProtectEnable(bsp_reg_protect_t regs_to_protect);
void     R_BSP_RegisterProtectDisable(bsp_reg_protect_t regs_to_unprotect);
bool     R_BSP_VoltageLevelSetting(uint8_t ctrl_ptn);
void     R_BSP_SoftwareReset(void);

void     bsp_register_protect_open(void); //r_bsp internal function. DO NOT CALL.
void     bsp_ram_initialize(void);
#if BSP_CFG_BUS_PRIORITY_INITIALIZE_ENABLE == 1
void     bsp_bus_priority_initialize(void);
#endif

#endif /* CPU_H */

