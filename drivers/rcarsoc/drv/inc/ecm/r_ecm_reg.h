/*
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
*
*/

#ifndef R_ECM_REG_H
#define R_ECM_REG_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* Definition of ECM base address */
#define DRV_REG_BASE_ADDR_ECM                       (0x189A0000U)

#define DRV_REG_OFFSET_M_N                          (0x00000004U)
/* Definition of ECM reg offset */
#define DRV_REG_OFFSET_ECM_ECMERRCTLR               (0x00000000U)
#define DRV_REG_OFFSET_ECM_ECMERRSTSR               (0x00000100U)
#define DRV_REG_OFFSET_ECM_ECMERRINCR               (0x00000200U)
#define DRV_REG_OFFSET_ECM_ECMERRRSTR               (0x00000300U)
#define DRV_REG_OFFSET_ECM_ECMERRCNTR               (0x00000400U)
#define DRV_REG_OFFSET_ECM_ECMERROMKR               (0x00000600U)
#define DRV_REG_OFFSET_ECM_ECMPSSTATCTLRA           (0x00000700U)
#define DRV_REG_OFFSET_ECM_ECMPSSTATCTLRB           (0x00000800U)
#define DRV_REG_OFFSET_ECM_ECMPSSTATCTLRM           (0x00000900U)
#define DRV_REG_OFFSET_ECM_ECMGEIIDR                (0x00000928U)
#define DRV_REG_OFFSET_ECM_SAFCLERRENR              (0x00000940U)
#define DRV_REG_OFFSET_ECM_SAFSTERRENR              (0x00000944U)
#define DRV_REG_OFFSET_ECM_SAFCTLR                  (0x00000948U)
#define DRV_REG_OFFSET_ECM_SAFSTSR                  (0x0000094CU)
#define DRV_REG_OFFSET_ECM_ECMDCLSERMON00R          (0x00000984U)
#define DRV_REG_OFFSET_ECM_ECMDCLSERMON01R          (0x00000988U)
#define DRV_REG_OFFSET_ECM_ECMWPCNTR                (0x00000A00U)
#define DRV_REG_OFFSET_ECM_ECMWACNTR                (0x00000A04U)
#define DRV_REG_OFFSET_ECM_ECMEXTRQHLDCNTR          (0x00000A10U)
#define DRV_REG_OFFSET_ECM_ECMEXTRQMSKCNTR          (0x00000A14U)
#define DRV_REG_OFFSET_ECM_ECMEXTRQSTSR             (0x00000A18U)
#define DRV_REG_OFFSET_ECM_ECMERRSTSINR             (0x00000A28U)
#define DRV_REG_OFFSET_ECM_ECMERROUTCTLR            (0x00000A2CU)
#define DRV_REG_OFFSET_ECM_ECMDYNCTRLR              (0x00000A30U)
#define DRV_REG_OFFSET_ECM_ECMDYNFREQSELR           (0x00000A34U)
#define DRV_REG_OFFSET_ECM_ECMERRINTSTSR0           (0x00000A40U)
#define DRV_REG_OFFSET_ECM_ECMERRINTSTSR1           (0x00000A44U)
#define DRV_REG_OFFSET_ECM_ECMDTMCTLR               (0x00000A50U)
#define DRV_REG_OFFSET_ECM_ECMDTMR                  (0x00000A54U)
#define DRV_REG_OFFSET_ECM_ECMDTMCMPR               (0x00000A58U)
#define DRV_REG_OFFSET_ECM_ECMDTMCFGR               (0x00000B00U)

/* Definition of ECM reg address */
#define DRV_REG_ADDR_ECM_ECMERRCTLR(n)              (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRCTLR + (n) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMERRSTSR(n)              (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRSTSR + (n) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMERRINCR(n)              (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRINCR + (n) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMERRRSTR(n)              (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRRSTR + (n) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMERRCNTR(m)              (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRCNTR + (m) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMERROMKR(n)              (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERROMKR + (n) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMPSSTATCTLRA(n)          (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMPSSTATCTLRA + (n) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMPSSTATCTLRB(n)          (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMPSSTATCTLRB + (n) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMPSSTATCTLRM(n)          (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMPSSTATCTLRM + (n) * DRV_REG_OFFSET_M_N)
#define DRV_REG_ADDR_ECM_ECMWPCNTR                  (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMWPCNTR)
#define DRV_REG_ADDR_ECM_ECMWACNTR                  (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMWACNTR)
#define DRV_REG_ADDR_ECM_ECMGEIIDR                  (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMGEIIDR)
#define DRV_REG_ADDR_ECM_SAFCLERRENR                (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_SAFCLERRENR)
#define DRV_REG_ADDR_ECM_SAFSTERRENR                (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_SAFSTERRENR)
#define DRV_REG_ADDR_ECM_SAFCTLR                    (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_SAFCTLR)
#define DRV_REG_ADDR_ECM_SAFSTSR                    (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_SAFSTSR)
#define DRV_REG_ADDR_ECM_ECMDCLSERMON00R            (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMDCLSERMON00R)
#define DRV_REG_ADDR_ECM_ECMDCLSERMON01R            (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMDCLSERMON01R)
#define DRV_REG_ADDR_ECM_ECMEXTRQHLDCNTR            (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMEXTRQHLDCNTR)
#define DRV_REG_ADDR_ECM_ECMEXTRQMSKCNTR            (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMEXTRQMSKCNTR)
#define DRV_REG_ADDR_ECM_ECMEXTRQSTSR               (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMEXTRQSTSR)
#define DRV_REG_ADDR_ECM_ECMERRSTSINR0              (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRSTSINR0)
#define DRV_REG_ADDR_ECM_ECMERRSTSINR1              (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRSTSINR1)
#define DRV_REG_ADDR_ECM_ECMDYNCTRLR                (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMDYNCTRLR)
#define DRV_REG_ADDR_ECM_ECMDYNFREQSELR             (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMDYNFREQSELR)
#define DRV_REG_ADDR_ECM_ECMERRINTSTSR0             (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRINTSTSR0)
#define DRV_REG_ADDR_ECM_ECMERRINTSTSR1             (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMERRINTSTSR1)
#define DRV_REG_ADDR_ECM_ECMDTMCTLR                 (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMDTMCTLR)
#define DRV_REG_ADDR_ECM_ECMDTMR                    (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMDTMR)
#define DRV_REG_ADDR_ECM_ECMDTMCMPR                 (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMDTMCMPR)
#define DRV_REG_ADDR_ECM_ECMDTMCFGR                 (uint32_t)(DRV_REG_BASE_ADDR_ECM + DRV_REG_OFFSET_ECM_ECMDTMCFGR + (n) * DRV_REG_OFFSET_M_N)

#define READ_REGISTER_32(addr)                      (*(volatile uint32_t *)(addr))
#define WRITE_REGISTER_32(addr, value)              (*(volatile uint32_t *)(addr) = (uint32_t)value)

#define ECM_SET_BIT(n)                              (uint32_t)(0x1U << (n))
#define ECM_SET_BIT0                                (uint32_t)(0x00000001U)
#define ECM_SET_BIT1                                (uint32_t)(0x00000002U)
#define ECM_SET_BIT2                                (uint32_t)(0x00000004U)
#define ECM_SET_BIT3                                (uint32_t)(0x00000008U)
#define ECM_SET_BIT4                                (uint32_t)(0x00000010U)
#define ECM_SET_BIT5                                (uint32_t)(0x00000020U)
#define ECM_SET_BIT6                                (uint32_t)(0x00000040U)
#define ECM_SET_BIT7                                (uint32_t)(0x00000080U)
#define ECM_SET_BIT8                                (uint32_t)(0x00000100U)
#define ECM_SET_BIT9                                (uint32_t)(0x00000200U)
#define ECM_SET_BIT10                               (uint32_t)(0x00000400U)
#define ECM_SET_BIT11                               (uint32_t)(0x00000800U)
#define ECM_SET_BIT12                               (uint32_t)(0x00001000U)
#define ECM_SET_BIT13                               (uint32_t)(0x00002000U)
#define ECM_SET_BIT14                               (uint32_t)(0x00004000U)
#define ECM_SET_BIT15                               (uint32_t)(0x00008000U)
#define ECM_SET_BIT16                               (uint32_t)(0x00010000U)
#define ECM_SET_BIT17                               (uint32_t)(0x00020000U)
#define ECM_SET_BIT18                               (uint32_t)(0x00040000U)
#define ECM_SET_BIT19                               (uint32_t)(0x00080000U)
#define ECM_SET_BIT20                               (uint32_t)(0x00100000U)
#define ECM_SET_BIT21                               (uint32_t)(0x00200000U)
#define ECM_SET_BIT22                               (uint32_t)(0x00400000U)
#define ECM_SET_BIT23                               (uint32_t)(0x00800000U)
#define ECM_SET_BIT24                               (uint32_t)(0x01000000U)
#define ECM_SET_BIT25                               (uint32_t)(0x02000000U)
#define ECM_SET_BIT26                               (uint32_t)(0x04000000U)
#define ECM_SET_BIT27                               (uint32_t)(0x08000000U)
#define ECM_SET_BIT28                               (uint32_t)(0x10000000U)
#define ECM_SET_BIT29                               (uint32_t)(0x20000000U)
#define ECM_SET_BIT30                               (uint32_t)(0x40000000U)
#define ECM_SET_BIT31                               (uint32_t)(0x80000000U)

#define ECM_ECMERRCTLR_CLEAR                        (0x00000000U)
#define ECM_ECMERRINCR_CLEAR                        (0x00000000U)
#define ECM_ECMERRSTSR_CLEAR                        (0xFFFFFFFFU)
#define ECM_SAFCLERRENR_CLEAR                       (0xFFFFFFFFU)
#define ECM_ECMERRINTSTSR0_CLEAR                    (0x00000000U)
#define ECM_ECMERRINTSTSR1_CLEAR                    (0x00000000U)
#define ECM_PUSEDO_ERROR_ENABLE                     (0x80000000U)

#define ECM_REGISTER_NUMBER                         (64)

#define ECM_WRITE_PROTECTION_ENABLE                 (0xACCE0000U)
#define ECM_WRITE_PROTECTION_DISABLE                (0xACCE0001U)



/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/



#endif /* R_ECM_REG_H */