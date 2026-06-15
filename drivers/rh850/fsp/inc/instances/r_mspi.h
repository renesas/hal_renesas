/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_MSPI_H
#define R_MSPI_H

/*******************************************************************************************************************//**
 * @addtogroup SPI
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_spi_api.h"
#include "r_mspi_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define MSPI_CS_CHANNEL_0     (0U)     ///< Chip select channel 0
#define MSPI_CS_CHANNEL_1     (1U)     ///< Chip select channel 1
#define MSPI_CS_CHANNEL_2     (2U)     ///< Chip select channel 2
#define MSPI_CS_CHANNEL_3     (3U)     ///< Chip select channel 3
#define MSPI_CS_CHANNEL_4     (4U)     ///< Chip select channel 4
#define MSPI_CS_CHANNEL_5     (5U)     ///< Chip select channel 5
#define MSPI_CS_CHANNEL_6     (6U)     ///< Chip select channel 6
#define MSPI_CS_CHANNEL_7     (7U)     ///< Chip select channel 7
#define MSPI_CS_CHANNEL_8     (8U)     ///< Chip select channel 8
#define MSPI_CS_CHANNEL_9     (9U)     ///< Chip select channel 9
#define MSPI_CS_CHANNEL_10    (10U)    ///< Chip select channel 10
#define MSPI_CS_CHANNEL_11    (11U)    ///< Chip select channel 11

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Active or inactive the MSPInCS[11:0] */
typedef enum e_mspi_chip_select_state
{
    MSPI_CHIP_SELECT_STATE_INACTIVE,   ///< Chip select signal is inactive.
    MSPI_CHIP_SELECT_STATE_ACTIVE      ///< Chip select signal is active.
} mspi_chip_select_state_t;

/** Enables or disables the slave select signal(MSPInCSI) in slave mode (MSPInCSIE). */
typedef enum e_mspi_slave_select
{
    MSPI_SLAVE_SELECT_IGNORED,         ///< Input CS signal(MSPInCSI) is ignored in slave mode.
    MSPI_SLAVE_SELECT_RECONGNIZED      ///< Input CS signal(MSPInCSI) is recognized in slave mode.
} mspi_slave_select_t;

/** Select sampling time of Master (MSPInSAMP) */
typedef enum e_mspi_internal_sampling
{
    MSPI_INTERNAL_SAMPLING_STANDARD,   ///< Standard sampling mode (MSPInSAMP)
    MSPI_INTERNAL_SAMPLING_NEXT_EDGE   ///< Next edge sampling mode (MSPInSAMP)
} mspi_internal_sampling_t;

/** Select communications clock frequency division in master mode */
typedef enum e_mspi_clock_mode
{
    MSPI_CLOCK_MODE_CLK_2  = (uint16_t)(0x1 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/2 (Master mode)
    MSPI_CLOCK_MODE_CLK_4  = (uint16_t)(0x2 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/4 (Master mode)
    MSPI_CLOCK_MODE_CLK_6  = (uint16_t)(0x3 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/6 (Master mode)
    MSPI_CLOCK_MODE_CLK_8  = (uint16_t)(0x4 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/8 (Master mode)
    MSPI_CLOCK_MODE_CLK_10 = (uint16_t)(0x5 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/10 (Master mode)
    MSPI_CLOCK_MODE_CLK_12 = (uint16_t)(0x6 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/12 (Master mode)
    MSPI_CLOCK_MODE_CLK_14 = (uint16_t)(0x7 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/14 (Master mode)
    MSPI_CLOCK_MODE_CLK_16 = (uint16_t)(0x8 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/16 (Master mode)
    MSPI_CLOCK_MODE_CLK_18 = (uint16_t)(0x9 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/18 (Master mode)
    MSPI_CLOCK_MODE_CLK_20 = (uint16_t)(0xA << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/20 (Master mode)
    MSPI_CLOCK_MODE_CLK_22 = (uint16_t)(0xB << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/22 (Master mode)
    MSPI_CLOCK_MODE_CLK_24 = (uint16_t)(0xC << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/24 (Master mode)
    MSPI_CLOCK_MODE_CLK_26 = (uint16_t)(0xD << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/26 (Master mode)
    MSPI_CLOCK_MODE_CLK_28 = (uint16_t)(0xE << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/28 (Master mode)
    MSPI_CLOCK_MODE_CLK_30 = (uint16_t)(0xF << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos),  ///< Unit operates as MSPI  MSPInCLK/30 (Master mode)
    MSPI_CLOCK_MODE_CLK_32 = (uint16_t)(0x10 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/32 (Master mode)
    MSPI_CLOCK_MODE_CLK_34 = (uint16_t)(0x11 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/34 (Master mode)
    MSPI_CLOCK_MODE_CLK_36 = (uint16_t)(0x12 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/36 (Master mode)
    MSPI_CLOCK_MODE_CLK_38 = (uint16_t)(0x13 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/38 (Master mode)
    MSPI_CLOCK_MODE_CLK_40 = (uint16_t)(0x14 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/40 (Master mode)
    MSPI_CLOCK_MODE_CLK_42 = (uint16_t)(0x15 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/42 (Master mode)
    MSPI_CLOCK_MODE_CLK_44 = (uint16_t)(0x16 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/44 (Master mode)
    MSPI_CLOCK_MODE_CLK_46 = (uint16_t)(0x17 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/46 (Master mode)
    MSPI_CLOCK_MODE_CLK_48 = (uint16_t)(0x18 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/48 (Master mode)
    MSPI_CLOCK_MODE_CLK_50 = (uint16_t)(0x19 << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/50 (Master mode)
    MSPI_CLOCK_MODE_CLK_52 = (uint16_t)(0x1A << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/52 (Master mode)
    MSPI_CLOCK_MODE_CLK_54 = (uint16_t)(0x1B << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/54 (Master mode)
    MSPI_CLOCK_MODE_CLK_56 = (uint16_t)(0x1C << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/56 (Master mode)
    MSPI_CLOCK_MODE_CLK_58 = (uint16_t)(0x1D << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/58 (Master mode)
    MSPI_CLOCK_MODE_CLK_60 = (uint16_t)(0x1E << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/60 (Master mode)
    MSPI_CLOCK_MODE_CLK_62 = (uint16_t)(0x1F << R_MSPI0_MSPI0_CH_CFG3_CDIV_Pos), ///< Unit operates as MSPI  MSPInCLK/62 (Master mode)
} mspi_clock_mode_t;

/** Selects MSPIn Clock default level (MSPInCKR) */
typedef enum e_mspi_clock_level
{
    MSPI_CLOCK_LEVEL_LOW,              ///< The default level of MSPInSCK is low.
    MSPI_CLOCK_LEVEL_HIGH              ///< The default level of MSPInSCK is high.
} mspi_clock_level_t;

/** Select the MSPInSOUT function in Master mode (MSPInSOLS) */
typedef enum e_mspi_sout_func
{
    MSPI_SOUT_FUNC_LOW_0,              ///< Set MSPInSOUT to low after macro enable, and holds the level after each transfer.
    MSPI_SOUT_FUNC_LOW_1,              ///< Set MSPInSOUT to low after macro enable and low during idle time.
    MSPI_SOUT_FUNC_HIGH_0,             ///< Set MSPInSOUT to high after macro enable, and holds the level after each transfer
    MSPI_SOUT_FUNC_HIGH_1              ///< Set MSPInSOUT to high after macro enable and high during idle time.
} mspi_sout_func_t;

/** Control the polarity of MSPInCS (MSPInCSP) */
typedef enum e_mspi_control_polarity
{
    MSPI_CONTROL_POLARITY_LOW,         ///< Signal is active low.
    MSPI_CONTROL_POLARITY_HIGH         ///< Signal is active high.
} mspi_control_polarity_t;

/** Enables/disables loop back mode (MSPInLBM) */
typedef enum e_mspi_loopback_mode
{
    MSPI_LOOPBACK_MODE_DISABLE,        ///< Default disable Loop-back mode
    MSPI_LOOPBACK_MODE_ENABLE          ///< Enable loop-back mode
} mspi_loopback_mode_t;

/** Enables/disables data consistency check (MSPInDCS) */
typedef enum e_mspi_consistency_check
{
    MSPI_CONSISTENCY_CHECK_DISABLE,    ///< Disables data consistency check.
    MSPI_CONSISTENCY_CHECK_ENABLE      ///< Enables data consistency check.
} mspi_consistency_check_t;

/** Transmit Only, Transmit/Receive or Receive Only. */
typedef enum e_mspi_selectable_mode
{
    MSPI_SELECTABLE_MODE_TRANSMIT_ONLY,    ///< MSPI in transmit only mode serial communication
    MSPI_SELECTABLE_MODE_TRANSMIT_RECEIVE, ///< MSPI in transmit/receive mode serial communication
    MSPI_SELECTABLE_MODE_RECEIVE_ONLY      ///< MSPI in receive-only mode serial communication
} mspi_selectable_mode_t;

/** Setting memory access mode */
typedef enum e_mspi_transfer_mode
{
    MSPI_TRANSFER_MODE_DIRECT_ACCESS,  ///< Direct memory mode
    MSPI_TRANSFER_MODE_BUFFER_MEMORY,  ///< Buffer memory mode
    MSPI_TRANSFER_MODE_FIFO_MEMORY     ///< FIFO memory mode
} mspi_transfer_mode_t;

/** Enable transmission */
typedef enum e_mspi_transmission
{
    MSPI_TRANSMISSION_DISABLE,         ///< Disable transmission
    MSPI_TRANSMISSION_ENABLE           ///< Enable transmission
} mspi_transmission_t;

/** Enable reception */
typedef enum e_mspi_enable_reception
{
    MSPI_RECEPTION_DISABLE,            ///< Disable reception
    MSPI_RECEPTION_ENABLE              ///< Enable reception
} mspi_reception_t;

/** Select channel priority */
typedef enum e_mspi_channel_priority
{
    MSPI_CHANNEL_PRIORITY_1,           ///< Channel priority 1
    MSPI_CHANNEL_PRIORITY_2,           ///< Channel priority 2
    MSPI_CHANNEL_PRIORITY_3,           ///< Channel priority 3
    MSPI_CHANNEL_PRIORITY_4,           ///< Channel priority 4
    MSPI_CHANNEL_PRIORITY_5,           ///< Channel priority 5
    MSPI_CHANNEL_PRIORITY_6,           ///< Channel priority 6
    MSPI_CHANNEL_PRIORITY_7,           ///< Channel priority 7
    MSPI_CHANNEL_PRIORITY_8            ///< Channel priority 8
} mspi_channel_priority_t;

/** Enable channel lock operation */
typedef enum e_mspi_channel_lock
{
    MSPI_CHANNEL_LOCK_DISABLE,         ///< Disable channel lock operation
    MSPI_CHANNEL_LOCK_ENABLE           ///< Enable channel lock operation
} mspi_channel_lock_t;

/** Select frame count operation */
typedef enum e_mspi_channel_frame_end
{
    MSPI_CHANNEL_FRAME_END_CLEAR,      ///< When a last frame ends, MSPInCHENm is cleared and the channel operation ends
    MSPI_CHANNEL_FRAME_END_KEEP,       ///< When a last frame ends, MSPInCHENm is not cleared and the channel operation continues
} mspi_channel_frame_end_t;

/** Enable output of INTTMSPInERR interrupt */
typedef enum e_mspi_interrupt_out
{
    MSPI_INTERRUPT_OUT_DISABLE,        ///< Disable output of interrupt
    MSPI_INTERRUPT_OUT_ENABLE          ///< Enable output of interrupt
} mspi_interrupt_out_t;

/** Select the polarity of the chip select signal */
typedef enum e_mspi_polarity
{
    MSPI_POLARITY_LOW,                 ///< MSPInSCK is low during idle time
    MSPI_POLARITY_HIGH                 ///< MSPInSCK is high during idle time
} mspi_polarity_t;

/** Select the clock phase selection bit */
typedef enum e_mspi_phase_select
{
    MSPI_PHASE_SELECT_ODD,             ///< Shifting bits out for transmission takes place on even-numbered edges, and
    ///< sampling for reception takes place on odd-numbered edges
    MSPI_PHASE_SELECT_EVEN             ///< Shifting bits out for transmission takes place on odd-numbered edges, and
    ///< sampling for reception takes place on even-numbered edges.
} mspi_phase_select_t;

/** Select communication direction */
typedef enum e_mspi_data_direction
{
    MSPI_DATA_DIRECTION_MSB_FIRST,     ///< Data is sent and received in MSB first order
    MSPI_DATA_DIRECTION_LSB_FIRST      ///< Data is sent and received in LSB first order
} mspi_data_direction_t;

/** Select bit controls the MSPInCS level for idle time in the Direct memory mode */
typedef enum e_mspi_idle_level
{
    MSPI_IDLE_LEVEL_LOW,               ///< MSPInCS level is inactive for idle time
    MSPI_IDLE_LEVEL_HIGH               ///< MSPInCS level holds active level for idle time
} mspi_idle_level_t;

/** Select bit controls whether to insert an idle period after each end of a frame */
typedef enum e_mspi_insert_idle_end
{
    MSPI_INSERT_IDLE_END_DISABLE,      ///< Idle period is not inserted each end of a frame
    MSPI_INSERT_IDLE_END_ENABLE        ///< Idle period is forcibly inserted after each end of a frame
} mspi_insert_idle_end_t;

/** Select bit controls whether to insert an idle period after the last frame end */
typedef enum e_mspi_insert_idle_last
{
    MSPI_INSERT_IDLE_LAST_ACTIVE,      ///< Holds MSPInCS at the active level
    MSPI_INSERT_IDLE_LAST_INACTIVE     ///< MSPInCS returns to the inactive level
} mspi_insert_idle_last_t;

/** Masks the CRC error of the first frame in Safe-SPI protocol function */
typedef enum e_mspi_crc_mask_enable
{
    MSPI_CRC_MASK_DISABLE,             ///< Does not mask the CRC error of the first frame
    MSPI_CRC_MASK_ENABLE               ///< Masks the CRC error of the first frame
} mspi_crc_mask_enable_t;

/** Select the format of the Safe-SPI protocol function */
typedef enum e_mspi_safe_format
{
    MSPI_SAFE_FORMAT_IN,               ///< In-frame format
    MSPI_SAFE_FORMAT_OUT               ///< Out-of-frame format
} mspi_safe_format_t;

/** Enables/disables the safe format for data transfer */
typedef enum e_mspi_safe
{
    MSPI_SAFE_DISABLE,                 ///< Safe format is disabled
    MSPI_SAFE_ENABLE                   ///< Safe format is enabled
} mspi_safe_t;

/** Odd parity check */
typedef enum e_mspi_parity
{
    MSPI_ODD_PARITY,                   ///< Adds odd parity
    MSPI_EVEN_PARITY,                  ///< Adds even parity
    MSPI_NO_PARITY                     ///< No parity
} mspi_parity_t;

/** Enable the parity check */
typedef enum e_mspi_parity_check
{
    MSPI_PARITY_CHECK_DISABLE,         ///< Parity check is disabled
    MSPI_PARITY_CHECK_ENABLE           ///< Parity check is enabled
} mspi_parity_check_t;

/** FIFO size stage */
typedef enum e_mspi_fifo_stage
{
    MSPI_FIFO_STAGE_SIZE_8,            ///< The stage size of Buffer is 8 in Fixed FIFO memory mode
    MSPI_FIFO_STAGE_SIZE_16,           ///< The stage size of Buffer is 16 in Fixed FIFO memory mode
    MSPI_FIFO_STAGE_SIZE_32            ///< The stage size of Buffer is 32 in Fixed FIFO memory mode
} mspi_fifo_stage_t;

/** Activate Job function */
typedef enum e_mspi_job
{
    MSPI_JOB_DISABLE,                  ///< Disable Job function
    MSPI_JOB_ENABLE                    ///< Enable Job function
} mspi_job_t;

/** Interrupt mask */
typedef enum e_mspi_mask_interrupt
{
    MSPI_MASKED_INTERRUPT_DISABLE,     ///< Unmasked interrupt output
    MSPI_MASKED_INTERRUPT_ENABLE       ///< Masked interrupt output
} mspi_interrupt_mask_t;

/** Transfer support trigger source */
typedef enum e_mspi_transfer_trigger
{
    MSPI_TRANSFER_TRIGGER_DMA,         ///< Transfer on DMA
    MSPI_TRANSFER_TRIGGER_DTS          ///< Transfer on DTSC
} mspi_transfer_trigger_t;

/** Hardware trigger source */
typedef enum e_mspi_hardware_trigger
{
    MSPI_HARDWARE_TRIGGER_DISABLE = 0x0U,       ///< Disable hardware reset
#if (0 == BSP_FEATURE_ATU_GTM_SEL)
 #if (defined(BSP_MCU_GROUP_RH850U2Bx) || defined(BSP_MCU_GROUP_RH850U2Cx))
    MSPI_HARDWARE_TRIGGER_IRQ0 = 0x1U,          ///< External Interrupt 0
    MSPI_HARDWARE_TRIGGER_IRQ1 = 0x2U,          ///< External Interrupt 1
    MSPI_HARDWARE_TRIGGER_IRQ2 = 0x3U,          ///< External Interrupt 2
    MSPI_HARDWARE_TRIGGER_IRQ3 = 0x4U,          ///< External Interrupt 3
 #elif defined(BSP_MCU_GROUP_RH850U2Ax)
    MSPI_HARDWARE_TRIGGER_INTP0 = 0x1U,         ///< External Interrupt 0
    MSPI_HARDWARE_TRIGGER_INTP1 = 0x2U,         ///< External Interrupt 1
    MSPI_HARDWARE_TRIGGER_INTP2 = 0x3U,         ///< External Interrupt 2
    MSPI_HARDWARE_TRIGGER_INTP3 = 0x4U,         ///< External Interrupt 3
 #endif
    MSPI_HARDWARE_TRIGGER_INTTPTMU00 = 0x5U,    ///< TPTM0 an up timer interrupt for comparison value 0
    MSPI_HARDWARE_TRIGGER_INTTPTMU01 = 0x6U,    ///< TPTM0 an up timer interrupt for comparison value 1
    MSPI_HARDWARE_TRIGGER_INTTPTMU02 = 0x7U,    ///< TPTM0 an up timer interrupt for comparison value 2
    MSPI_HARDWARE_TRIGGER_INTTPTMU03 = 0x8U,    ///< TPTM0 an up timer interrupt for comparison value 3
    MSPI_HARDWARE_TRIGGER_INTTPTMU10 = 0x9U,    ///< TPTM1 an up timer interrupt for comparison value 0
    MSPI_HARDWARE_TRIGGER_INTTPTMU11 = 0xAU,    ///< TPTM1 an up timer interrupt for comparison value 1
    MSPI_HARDWARE_TRIGGER_INTTPTMU12 = 0xBU,    ///< TPTM1 an up timer interrupt for comparison value 2
    MSPI_HARDWARE_TRIGGER_INTTPTMU13 = 0xCU,    ///< TPTM1 an up timer interrupt for comparison value 3
 #if (defined(BSP_MCU_GROUP_RH850U2Ax) || defined(BSP_MCU_GROUP_RH850U2Bx))
    MSPI_HARDWARE_TRIGGER_INTTPTMU20 = 0xDU,    ///< TPTM2 an up timer interrupt for comparison value 0
    MSPI_HARDWARE_TRIGGER_INTTPTMU21 = 0xEU,    ///< TPTM2 an up timer interrupt for comparison value 1
    MSPI_HARDWARE_TRIGGER_INTTPTMU22 = 0xFU,    ///< TPTM2 an up timer interrupt for comparison value 2
    MSPI_HARDWARE_TRIGGER_INTTPTMU23 = 0x10U,   ///< TPTM2 an up timer interrupt for comparison value 3
    MSPI_HARDWARE_TRIGGER_INTTPTMU30 = 0x11U,   ///< TPTM3 an up timer interrupt for comparison value 0
    MSPI_HARDWARE_TRIGGER_INTTPTMU31 = 0x12U,   ///< TPTM3 an up timer interrupt for comparison value 1
    MSPI_HARDWARE_TRIGGER_INTTPTMU32 = 0x13U,   ///< TPTM3 an up timer interrupt for comparison value 2
    MSPI_HARDWARE_TRIGGER_INTTPTMU33 = 0x14U,   ///< TPTM3 an up timer interrupt for comparison value 3
 #endif
 #if (defined(BSP_MCU_GROUP_RH850U2Bx) || defined(BSP_MCU_GROUP_RH850U2Cx))
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG00 = 0x15U, ///< GTM Interrupt trigger channel 0 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG01 = 0x16U, ///< GTM Interrupt trigger channel 1 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG02 = 0x17U, ///< GTM Interrupt trigger channel 2 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG10 = 0x18U, ///< GTM Interrupt trigger channel 3 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG11 = 0x19U, ///< GTM Interrupt trigger channel 4 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG12 = 0x1AU, ///< GTM Interrupt trigger channel 5 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG20 = 0x1BU, ///< GTM Interrupt trigger channel 6 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG21 = 0x1CU, ///< GTM Interrupt trigger channel 7 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG22 = 0x1DU, ///< GTM Interrupt trigger channel 8 through PIC_TOP
 #elif (defined(BSP_MCU_GROUP_RH850U2Bx))
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG30 = 0x1EU, ///< GTM Interrupt trigger channel 9 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG31 = 0x1FU, ///< GTM Interrupt trigger channel 10 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG32 = 0x20U, ///< GTM Interrupt trigger channel 11 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG40 = 0x21U, ///< GTM Interrupt trigger channel 12 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG41 = 0x22U, ///< GTM Interrupt trigger channel 13 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG42 = 0x23U, ///< GTM Interrupt trigger channel 14 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG50 = 0x24U, ///< GTM Interrupt trigger channel 15 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG51 = 0x25U, ///< GTM Interrupt trigger channel 16 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG52 = 0x26U, ///< GTM Interrupt trigger channel 17 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG60 = 0x27U, ///< GTM Interrupt trigger channel 18 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG61 = 0x28U, ///< GTM Interrupt trigger channel 19 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG62 = 0x29U, ///< GTM Interrupt trigger channel 20 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG70 = 0x2AU, ///< GTM Interrupt trigger channel 21 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG71 = 0x2BU, ///< GTM Interrupt trigger channel 22 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG72 = 0x2CU, ///< GTM Interrupt trigger channel 23 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG80 = 0x2DU, ///< GTM Interrupt trigger channel 24 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG81 = 0x2EU, ///< GTM Interrupt trigger channel 25 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG82 = 0x2FU, ///< GTM Interrupt trigger channel 26 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG90 = 0x30U, ///< GTM Interrupt trigger channel 27 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG91 = 0x31U, ///< GTM Interrupt trigger channel 28 through PIC_TOP
    MSPI_HARDWARE_TRIGGER_GTMMSPICFG92 = 0x32U, ///< GTM Interrupt trigger channel 29 through PIC_TOP
 #elif defined(BSP_MCU_GROUP_RH850U2Ax)
    MSPI_HARDWARE_TRIGGER_MCS0_IRQ0 = 0x15U,    ///< MCS0 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS0_IRQ1 = 0x16U,    ///< MCS0 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS0_IRQ2 = 0x17U,    ///< MCS0 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS0_IRQ3 = 0x18U,    ///< MCS0 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS0_IRQ4 = 0x19U,    ///< MCS0 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS0_IRQ5 = 0x1AU,    ///< MCS0 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS0_IRQ6 = 0x1BU,    ///< MCS0 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS0_IRQ7 = 0x1CU,    ///< MCS0 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS2_IRQ0 = 0x1DU,    ///< MCS2 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS2_IRQ1 = 0x1EU,    ///< MCS2 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS2_IRQ2 = 0x1FU,    ///< MCS2 Interrupt for channel
    MSPI_HARDWARE_TRIGGER_MCS2_IRQ3 = 0x20U,    ///< MCS2 Interrupt for channel
 #endif
#else
    MSPI_HARDWARE_TRIGGER_ATU0    = 0x1U,       ///< ATU Interrupt trigger channel 0
    MSPI_HARDWARE_TRIGGER_ATU1    = 0x2U,       ///< ATU Interrupt trigger channel 1
    MSPI_HARDWARE_TRIGGER_ATU2    = 0x3U,       ///< ATU Interrupt trigger channel 2
    MSPI_HARDWARE_TRIGGER_ATU3    = 0x4U,       ///< ATU Interrupt trigger channel 3
    MSPI_HARDWARE_TRIGGER_ATU4    = 0x15U,      ///< ATU Interrupt trigger channel 4
    MSPI_HARDWARE_TRIGGER_ATU5    = 0x16U,      ///< ATU Interrupt trigger channel 5
    MSPI_HARDWARE_TRIGGER_ATU6    = 0x17U,      ///< ATU Interrupt trigger channel 6
    MSPI_HARDWARE_TRIGGER_ATU7    = 0x18U,      ///< ATU Interrupt trigger channel 7
    MSPI_HARDWARE_TRIGGER_ATU8    = 0x19U,      ///< ATU Interrupt trigger channel 8
    MSPI_HARDWARE_TRIGGER_ATU9    = 0x1AU,      ///< ATU Interrupt trigger channel 9
    MSPI_HARDWARE_TRIGGER_ATU10   = 0x1BU,      ///< ATU Interrupt trigger channel 10
    MSPI_HARDWARE_TRIGGER_ATU11   = 0x1CU,      ///< ATU Interrupt trigger channel 11
    MSPI_HARDWARE_TRIGGER_ATU12   = 0x1DU,      ///< ATU Interrupt trigger channel 12
    MSPI_HARDWARE_TRIGGER_ATU13   = 0x1EU,      ///< ATU Interrupt trigger channel 13
    MSPI_HARDWARE_TRIGGER_ATU14   = 0x1FU,      ///< ATU Interrupt trigger channel 14
    MSPI_HARDWARE_TRIGGER_ATU15   = 0x20U,      ///< ATU Interrupt trigger channel 15
    MSPI_HARDWARE_TRIGGER_ATU16   = 0x21U,      ///< ATU Interrupt trigger channel 16
    MSPI_HARDWARE_TRIGGER_ATU17   = 0x22U,      ///< ATU Interrupt trigger channel 17
    MSPI_HARDWARE_TRIGGER_ATU18   = 0x23U,      ///< ATU Interrupt trigger channel 18
    MSPI_HARDWARE_TRIGGER_ATU19   = 0x24U,      ///< ATU Interrupt trigger channel 19
    MSPI_HARDWARE_TRIGGER_ATU20   = 0x25U,      ///< ATU Interrupt trigger channel 20
    MSPI_HARDWARE_TRIGGER_ATU21   = 0x26U,      ///< ATU Interrupt trigger channel 21
    MSPI_HARDWARE_TRIGGER_ATU22   = 0x27U,      ///< ATU Interrupt trigger channel 22
    MSPI_HARDWARE_TRIGGER_ATU23   = 0x28U,      ///< ATU Interrupt trigger channel 23
    MSPI_HARDWARE_TRIGGER_ATU24   = 0x29U,      ///< ATU Interrupt trigger channel 24
    MSPI_HARDWARE_TRIGGER_ATU25   = 0x2AU,      ///< ATU Interrupt trigger channel 25
    MSPI_HARDWARE_TRIGGER_ATU26   = 0x2BU,      ///< ATU Interrupt trigger channel 26
    MSPI_HARDWARE_TRIGGER_ATU27   = 0x2CU,      ///< ATU Interrupt trigger channel 27
    MSPI_HARDWARE_TRIGGER_ATU28   = 0x2DU,      ///< ATU Interrupt trigger channel 28
    MSPI_HARDWARE_TRIGGER_ATU29   = 0x2EU,      ///< ATU Interrupt trigger channel 29
    MSPI_HARDWARE_TRIGGER_ATU30   = 0x2FU,      ///< ATU Interrupt trigger channel 30
    MSPI_HARDWARE_TRIGGER_ATU31   = 0x30U,      ///< ATU Interrupt trigger channel 31
    MSPI_HARDWARE_TRIGGER_TAUD08  = 0x31U,      ///< TAUD Interrupt trigger
    MSPI_HARDWARE_TRIGGER_TAUD19  = 0x32U,      ///< TAUD Interrupt trigger
    MSPI_HARDWARE_TRIGGER_TAUD210 = 0x33U,      ///< TAUD Interrupt trigger
    MSPI_HARDWARE_TRIGGER_TAUD311 = 0x34U,      ///< TAUD Interrupt trigger
    MSPI_HARDWARE_TRIGGER_TAUD412 = 0x35U,      ///< TAUD Interrupt trigger
    MSPI_HARDWARE_TRIGGER_TAUD513 = 0x36U,      ///< TAUD Interrupt trigger
    MSPI_HARDWARE_TRIGGER_TAUD614 = 0x37U,      ///< TAUD Interrupt trigger
#endif
    MSPI_HARDWARE_TRIGGER_INTTSG30IVLY = 0x38U, ///< TSG30 valley interrupt
    MSPI_HARDWARE_TRIGGER_INTTSG30IPEK = 0x39U, ///< TSG30 peak interrupt
    MSPI_HARDWARE_TRIGGER_INTTSG30I12  = 0x3AU, ///< TSG30 compare match interrupt 12
    MSPI_HARDWARE_TRIGGER_INTTSG30I11  = 0x3BU, ///< TSG30 compare match interrupt 11
    MSPI_HARDWARE_TRIGGER_INTTSG31IVLY = 0x3CU, ///< TSG31 valley interrupt
    MSPI_HARDWARE_TRIGGER_INTTSG31IPEK = 0x3DU, ///< TSG31 peak interrupt
    MSPI_HARDWARE_TRIGGER_INTTSG31I12  = 0x3EU, ///< TSG31 compare match interrupt 12
    MSPI_HARDWARE_TRIGGER_INTTSG31I11  = 0x3FU  ///< TSG31 compare match interrupt 11
} mspi_hardware_trigger_t;

/** Transfer alternative operation */
typedef enum e_mspi_transfer_alternative
{
    MSPI_TRANSFER_ALTERNATIVE_DISABLE, ///< Disable DMA alternative trigger
    MSPI_TRANSFER_ALTERNATIVE_ENABLE   ///< Enable DMA alternative trigger
} mspi_transfer_alternative_t;

/** Transfer trigger source */
typedef enum e_mspi_activation_source
{
    MSPI_TRIGGER_SOURCE_NONE    = 0,
    MSPI_TRIGGER_SOURCE_TX1     = 1,
    MSPI_TRIGGER_SOURCE_RX1     = 2,
    MSPI_TRIGGER_SOURCE_FE1     = 3,
    MSPI_TRIGGER_SOURCE_TX2     = 4,
    MSPI_TRIGGER_SOURCE_RX2     = 5,
    MSPI_TRIGGER_SOURCE_FE2     = 6,
    MSPI_TRIGGER_SOURCE_TX1_RX2 = 7,
} mspi_trigger_source_t;

/** Configure the chip selection function 0*/
typedef union u_mspi_chip_select_cfg_0
{
    uint32_t chip_select_config_0_u32;

    /** Struct contain bit field of CFGx0 register  */
    struct
    {
        mspi_interrupt_out_t mspi_txe_inr       : 1; ///< Select enables or disables the output of INTMSPInTXEm interrupt.
        mspi_interrupt_out_t mspi_rxe_inr       : 1; ///< Select enables or disables the output of INTMSPInRXEm interrupt.
        mspi_interrupt_out_t mspi_fee_inr       : 1; ///< Select enables or disables the output of INTMSPInFEEm interrupt.
        mspi_interrupt_out_t mspi_ere_inr       : 1; ///< Select enables or disables the output of INTMSPInERRm interrupt.
        uint32_t                                : 8; ///< Unused
        mspi_channel_frame_end_t mspi_frame_end : 1; ///< Select the number of frames to be transferred
        uint32_t                                : 3; ///< Unused
        mspi_channel_lock_t mspi_lock           : 1; ///< Select channel lock operation
        uint32_t                                : 3; ///< Unused
        mspi_channel_priority_t mspi_priority   : 3; ///< Select channel priority
        uint32_t                                : 1; ///< Unused
        mspi_transfer_mode_t mspi_transfer_mode : 2; ///< Select channel mode
        uint32_t                                : 2; ///< Unused
        mspi_reception_t    mspi_enable_rec     : 1; ///< Enable or disable reception
        mspi_transmission_t mspi_enable_trans   : 1; ///< Enable or disable transmission
        uint32_t                                : 2; ///< Unused
    } mspi_chip_select_cfg0;
} mspi_chip_select_cfg0_t;

/** Configure the chip selection function 1*/
typedef union u_mspi_chip_select_cfg1
{
    uint32_t chip_select_config_1_u32;

    /** Struct contain bit field of CFGx1 register  */
    struct
    {
        mspi_parity_check_t mspi_parity_check       : 1; ///< Enables/disables the parity check
        uint32_t                                    : 3; ///< Unused
        mspi_parity_t mspi_odd_parity               : 2; ///<
        uint32_t                                    : 2; ///< Unused
        mspi_safe_t        mspi_safe_enable         : 1; ///< Enables/disables the safe format for data transfer
        mspi_safe_format_t mspi_safe_format         : 1; ///< Selects the safe format for data transfer
        uint32_t                                    : 1; ///< Unused
        mspi_crc_mask_enable_t mspi_crc_mask_enable : 1; ///< Enables/disables CRC function
        uint32_t                                    : 4; ///< Unused
        mspi_insert_idle_last_t mspi_idle_last      : 1; ///< Select bit controls whether to insert an idle period after the last frame end
        mspi_insert_idle_end_t  mspi_idle_end       : 1; ///< Select bit controls whether to insert an idle period after each end of a frame
        mspi_idle_level_t       mspi_idle_level     : 1; ///< Select bit controls the MSPInCS level for idle time in the Direct memory mode
        uint32_t                                    : 1; ///< Unused
        spi_bit_order_t mspi_direction              : 1; ///< Select the communication direction
        uint32_t                                    : 3; ///< Unused
        mspi_phase_select_t mspi_phase_select       : 1; ///< Select the clock phase selection bit
        mspi_polarity_t     mspi_sck_polarity       : 1; ///< Select the polarity of the chip select signal
        uint32_t                                    : 6; ///< Unused
    } mspi_chip_select_cfg1;
} mspi_chip_select_cfg1_t;

typedef union u_mspi_chip_select_cfg4
{
    uint32_t chip_select_config_4_u32;
    struct
    {
        mspi_fifo_stage_t mspi_fifo_stage_size  : 2;  ///< FIFO size stage
        uint32_t                                : 14; ///< Unused
        mspi_hardware_trigger_t mspi_hw_trigger : 6;  ///< Hardware trigger source
        uint32_t                                : 10; ///< Unused
    } mspi_chip_select_cfg4;
} mspi_chip_select_cfg4_t;

/** Configure register control 1 */
typedef union u_mspi_control_register_1
{
    uint32_t control_register_1;

    /** Struct contain bit field of CTL1 register  */
    struct
    {
        uint16_t mspi_control_polarity                  : 12; ///< Select the control polarity of MSPInCS
        uint32_t                                        : 12; ///< Unused
        mspi_sout_func_t mspi_sout_function             : 2;  ///< Select the SOUT function in Master mode
        uint32_t                                        : 1;  ///< Unused
        mspi_clock_level_t       mspi_clock_level       : 1;  ///< Selects MSPIn Clock default level
        mspi_internal_sampling_t mspi_internal_sampling : 1;  ///< Select the internal sampling mode
        mspi_slave_select_t      mspi_slave_select      : 1;  ///< Enables/disables the slave select signal in slave mode
        uint32_t                                        : 1;  ///< Unused
        spi_mode_t mspi_mode_selection                  : 1;  ///< Enables/disables the slave select function
    } mspi_bit_control_register_1;
} mspi_control_register_1_t;

/** Extended MSPI interface configuration */
typedef struct st_mspi_extended_cfg
{
    mspi_selectable_mode_t    mspi_select;             ///< Select Transmit Only, Transmit/Receive or Receive Only communication
    mspi_clock_mode_t         mspi_clock_mode;         ///< Master or slave operating mode and the reference clock value
    uint32_t                  mspi_baudrate_channel;   ///< Value setting for CFG03 register.
    mspi_chip_select_cfg0_t * p_mspi_config_cfg0;      ///< Cfg0 configuration for channel operation
    mspi_chip_select_cfg1_t * p_mspi_config_cfg1;      ///< Cfg1 configuration for channel operation
    mspi_chip_select_cfg4_t * p_mspi_config_cfg4;      ///< Cfg4 configuration for channel operation
    mspi_chip_select_state_t  mspi_chip_selection;     ///< Select chip selection active or inactive
    mspi_job_t                mspi_job_enable;         ///< Enable Job function
    mspi_control_register_1_t mspi_ctl1;               ///< Control register 1
    mspi_loopback_mode_t      mspi_loopback;           ///< Enables/disables loop-back mode
    mspi_consistency_check_t  mspi_consistency_check;  ///< Enables/disables data consistency check
    uint16_t                  mspi_setup_time;         ///< Setup time for the delay between MSPInCS assertion and MSPInSCK operation start
    uint16_t                  mspi_hold_time;          ///< Hold time for the delay between MSPInSCK operation stop and MSPInCS negation delay time.
    uint16_t                  mspi_idle_time;          ///< Idle time for the delay between MSPInCS negation and MSPInCS assertion between two frames.
    uint16_t                  mspi_inter_data;         ///< Inter-data time for the delay between MSPInSCK operation stop and MSPInSCK operation start between two frames with same MSPInCS configuration
    uint16_t                  mspi_ram_start;          ///< Start address of the RAM area used for data transfer in the Fixed buffer memory mode
    mspi_interrupt_mask_t     mspi_err_isr_mask;       ///< Mask MSPIn error interrupts output to INTC2
    mspi_interrupt_mask_t     mspi_fe_isr_mask;        ///< Mask MSPIn frame end interrupts output to INTC
    mspi_interrupt_mask_t     mspi_rx_isr_mask;        ///< Mask MSPIn reception interrupts output to INTC
    mspi_interrupt_mask_t     mspi_tx_isr_mask;        ///< Mask MSPIn transmission interrupts output to INTC
    uint8_t                     mspi_number_of_cs;     ///< Number of chip select for each unit
    uint8_t                     unit;                  ///< MSPI unit number
    mspi_transfer_trigger_t     mspi_transfer_trigger; ///< MSPI transfer trigger source
    mspi_trigger_source_t       mspi_trigger_source;   ///< MSPI activation source
    mspi_transfer_alternative_t mspi_dma_alternative;  ///< MSPI DMA Trigger Alternative Select Register
    mspi_transfer_alternative_t mspi_dts_alternative;  ///< MSPI DTS Trigger Alternative Select Register
    uint32_t                    buffer_length;         ///< Length in fixed buffer mode
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
    bsp_ored_t com_ored_int;                           ///< ORED interrupt for MSPI communication
    bsp_ored_t rec_ored_int;                           ///< ORED interrupt for MSPI reception
    bsp_ored_t frame_ored_int;                         ///< ORED interrupt for MSPI frame end
    bsp_ored_t error_ored_int;                         ///< ORED interrupt for MSPI error
#endif
} mspi_extended_cfg_t;

/** MSPI instance control block. */
typedef struct st_mspi_instance_ctrl
{
    uint32_t            open;          ///< Indicates whether the open() API has been successfully called.
    spi_cfg_t const   * p_cfg;         ///< Pointer to instance configuration
    R_MSPI0_Type      * p_regs;        ///< Base register for this unit
    R_MSPI0_INTF_Type * p_regs_intf;   ///< Base register for interrupt register
    R_MSPITG_Type     * p_regs_tg;     ///< Base register for trigger generator register
    void const        * p_tx_data;     ///< Buffer to transmit
    void              * p_rx_data;     ///< Buffer to receive
    uint32_t            tx_count;      ///< Number of Data Frames to transfer (2-bit,3-bit...16-bit)
    uint32_t            rx_count;      ///< Number of Data Frames to receiver (2-bit,3-bit...16-bit)
    spi_bit_width_t     bitwidth;      ///< Length of data frame (up to 128-bit)
    uint32_t            length;        ///< Number of frames
    uint8_t             stage_size;    ///< Stage size in FIFO mode

    /* Pointer to callback and optional working memory */
    void (* p_callback)(spi_callback_args_t *);
    spi_callback_args_t * p_callback_memory;

    /* Pointer to context to be passed into callback function */
    void * p_context;
} mspi_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const spi_api_t g_spi_on_mspi;

/** @endcond */
fsp_err_t R_MSPI_Open(spi_ctrl_t * p_ctrl, spi_cfg_t const * const p_cfg);
fsp_err_t R_MSPI_Close(spi_ctrl_t * const p_ctrl);
fsp_err_t R_MSPI_Read(spi_ctrl_t * const p_ctrl, void * p_dest, uint32_t const length, spi_bit_width_t const bit_width);
fsp_err_t R_MSPI_Write(spi_ctrl_t * const    p_ctrl,
                       void const          * p_src,
                       uint32_t const        length,
                       spi_bit_width_t const bit_width);
fsp_err_t R_MSPI_WriteRead(spi_ctrl_t * const    p_ctrl,
                           void const          * p_src,
                           void                * p_dest,
                           uint32_t const        length,
                           spi_bit_width_t const bit_width);
fsp_err_t R_MSPI_ChipSelect(spi_ctrl_t * const p_ctrl, uint32_t chip_select);
fsp_err_t R_MSPI_CallbackSet(spi_ctrl_t * const          p_ctrl,
                             void (                    * p_callback)(spi_callback_args_t *),
                             void * const                p_context,
                             spi_callback_args_t * const p_callback_memory);

/*******************************************************************************************************************//**
 * @} (end addtogroup SPI)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif
