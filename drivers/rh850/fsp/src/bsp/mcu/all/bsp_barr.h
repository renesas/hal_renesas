/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_BARR_H
#define BSP_BARR_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU
 * @defgroup BSP_BARR BSP Barrier-Synchronization
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Barrier Synchronization control values */
#define BSP_BARRIER_SYNCHRONIZATION_INITIALIZE    0x01U                                                   ///< Barrier initialize value
#define BSP_BARRIER_SYNCHRONIZATION_COMPLETION    0x01U                                                   ///< Barrier completion value
#define BSP_BARRIER_SYNCHRONIZATION_CLEAR         0x00U                                                   ///< Barrier clear value
#define BSP_BARRIER_NOT_ENABLE_PE                 0x00U                                                   ///< Barrier not enabled for PE

/** Barrier register base address */
#define R_BARR_BASE                               0xFFFB8000UL                                            ///< Base address of barrier registers

/** Barrier register access macros */
#define BSP_REG_BRnINIT(n)        *(volatile uint8_t *) (R_BARR_BASE + 0x000 + 0x010 * (n))               ///< BRnINIT register for channel n
#define BSP_REG_BRnEN(n)          *(volatile uint8_t *) (R_BARR_BASE + 0x004 + 0x010 * (n))               ///< BRnEN register for channel n
#define BSP_REG_BRnCHKS(n)        *(volatile uint8_t *) (R_BARR_BASE + 0x100 + 0x010 * (n))               ///< BRnCHKS register for channel n
#define BSP_REG_BRnSYNCS(n)       *(volatile uint8_t *) (R_BARR_BASE + 0x104 + 0x010 * (n))               ///< BRnSYNCS register for channel n
#define BSP_REG_BRnCHKm(n, m)     *(volatile uint8_t *) (R_BARR_BASE + 0x800 + 0x010 * (n) + 0x100 * (m)) ///< BRnCHKm register for channel n and PE m
#define BSP_REG_BRnSYNCm(n, m)    *(volatile uint8_t *) (R_BARR_BASE + 0x804 + 0x010 * (n) + 0x100 * (m)) ///< BRnSYNCm register for channel n and PE m

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Barrier Synchronization channel */
typedef enum e_bsp_barrier_sync_channel
{
    BSP_BARRIER_SYNC_CHANNEL_0  = (0U),  ///< Barrier sync channel 0
    BSP_BARRIER_SYNC_CHANNEL_1  = (1U),  ///< Barrier sync channel 1
    BSP_BARRIER_SYNC_CHANNEL_2  = (2U),  ///< Barrier sync channel 2
    BSP_BARRIER_SYNC_CHANNEL_3  = (3U),  ///< Barrier sync channel 3
    BSP_BARRIER_SYNC_CHANNEL_4  = (4U),  ///< Barrier sync channel 4
    BSP_BARRIER_SYNC_CHANNEL_5  = (5U),  ///< Barrier sync channel 5
    BSP_BARRIER_SYNC_CHANNEL_6  = (6U),  ///< Barrier sync channel 6
    BSP_BARRIER_SYNC_CHANNEL_7  = (7U),  ///< Barrier sync channel 7
    BSP_BARRIER_SYNC_CHANNEL_8  = (8U),  ///< Barrier sync channel 8
    BSP_BARRIER_SYNC_CHANNEL_9  = (9U),  ///< Barrier sync channel 9
    BSP_BARRIER_SYNC_CHANNEL_10 = (10U), ///< Barrier sync channel 10
    BSP_BARRIER_SYNC_CHANNEL_11 = (11U), ///< Barrier sync channel 11
    BSP_BARRIER_SYNC_CHANNEL_12 = (12U), ///< Barrier sync channel 12
    BSP_BARRIER_SYNC_CHANNEL_13 = (13U), ///< Barrier sync channel 13
    BSP_BARRIER_SYNC_CHANNEL_14 = (14U), ///< Barrier sync channel 14
    BSP_BARRIER_SYNC_CHANNEL_15 = (15U)  ///< Barrier sync channel 15
} bsp_barrier_sync_channel_t;

/***********************************************************************************************************************
 * Exported global variable (to be accessed by other files)
 **********************************************************************************************************************/

uint8_t R_BSP_GetCoreID(void);
void    R_BSP_PESyncCheck(bsp_barrier_sync_channel_t channel);
void    R_BSP_PESyncEnable(bsp_barrier_sync_channel_t channel, uint32_t pe_bitmask);
void    R_BSP_PESyncComplete(bsp_barrier_sync_channel_t channel, uint8_t peid);

void    R_BSP_BarrChannelInit(bsp_barrier_sync_channel_t channel);
void    R_BSP_BarrChannelEnableSet(bsp_barrier_sync_channel_t channel, uint32_t pe_bitmask);
uint8_t R_BSP_BarrChannelEnableGet(bsp_barrier_sync_channel_t channel);
void    R_BSP_BarrChannelStatusSet(bsp_barrier_sync_channel_t channel, uint8_t peid, uint8_t status);
uint8_t R_BSP_BarrChannelStatusGet(bsp_barrier_sync_channel_t channel, uint8_t peid);
void    R_BSP_BarrChannelStatusClear(bsp_barrier_sync_channel_t channel, uint8_t peid);

/** @} (end addtogroup BSP_BARR) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif
