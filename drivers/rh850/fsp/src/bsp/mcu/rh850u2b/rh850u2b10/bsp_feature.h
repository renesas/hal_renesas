/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2BX
 * @defgroup BSP_MCU_RH850U2B10 BSP MCU U2B10
 * @brief This module provides the macros, data types, and definition constants required for U2B10 feature.
 **********************************************************************************************************************/

#ifndef BSP_FEATURE_H
#define BSP_FEATURE_H

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2B10
 * @defgroup BSP_FEATURE_U2B10 BSP U2B10 Specific Feature
 * @brief This file defines all feature-related macros for the U2B10 device, including module availability and
 * hardware capability constants.
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Barrier Synchronization */
/** PE mask bit that is used in R_BSP_PESyncEnable */
#define BSP_BARRIER_SYNC_CPU0                                                 (1U << 0U)                           ///< PE mask bit for CPU0 in R_BSP_PESyncEnable
#define BSP_BARRIER_SYNC_CPU1                                                 (1U << 1U)                           ///< PE mask bit for CPU1 in R_BSP_PESyncEnable
#define BSP_BARRIER_SYNC_CPU2                                                 (1U << 2U)                           ///< PE mask bit for CPU2 in R_BSP_PESyncEnable

/** PE value that is used in R_BSP_PESyncComplete */
#define BSP_CPU0                                                              (0U)                                 ///< CPU identifier value for CPU0
#define BSP_CPU1                                                              (1U)                                 ///< CPU identifier value for CPU1
#define BSP_CPU2                                                              (2U)                                 ///< CPU identifier value for CPU2

/** BSP FEATURE */
/** U2B10 */
#define BSP_FEATURE_MMCA_IS_AVAILABLE                                         (0x01U)                              ///< MMC-A feature availability (1 = available)
#define BSP_FEATURE_PSI5_IS_AVAILABLE                                         (0x01U)                              ///< PSI5 module availability (1 = available)
#define BSP_FEATURE_PSI5S_IS_AVAILABLE                                        (0x01U)                              ///< PSI5S module availability (1 = available)
#define BSP_FEATURE_RHSIF_IS_AVAILABLE                                        (0x01U)                              ///< RHSIF module availability (1 = available)
#define BSP_FEATURE_RIIC_IS_AVAILABLE                                         (0x01U)                              ///< RIIC (I2C) module availability (1 = available)
#define BSP_FEATURE_EMU_IS_AVAILABLE                                          (0x00U)                              ///< EMU module availability (0 = not available)
#define BSP_FEATURE_DFP_IS_AVAILABLE                                          (0x01U)                              ///< DFP module availability (1 = available)
#define BSP_FEATURE_DSMIF_IS_AVAILABLE                                        (0x01U)                              ///< DSMIF feature availability (1 = available)
#define BSP_FEATURE_RDC3AS_IS_AVAILABLE                                       (0x01U)                              ///< RDC3AS feature availability (1 = available)
#define BSP_FEATURE_RDC3AL_IS_AVAILABLE                                       (0x00U)                              ///< RDC3AL feature availability (0 = not available)

#define BSP_FEATURE_FEINT_CPU0                                                (1U)                                 ///< FEINT interrupt feature for CPU0
#define BSP_FEATURE_FEINT_CPU1                                                (1U)                                 ///< FEINT interrupt feature for CPU1
#define BSP_FEATURE_FEINT_CPU2                                                (1U)                                 ///< FEINT interrupt feature for CPU2
#define BSP_FEATURE_FEINT_CPU3                                                (1U)                                 ///< FEINT interrupt feature for CPU3

/** MODULE RESET */
#define BSP_RESET_SOURCE_POWER_OFF_STANDBY_IS_AVAILABLE                       (0x01U)                              ///< Power-off standby reset source support

/** STANDBY MODE */
#define BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_IS_AVAILABLE                        (0x00U)                              ///< DeepStop mode availability (0 = not supported)
#define BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_NUM_OF_WAKEUP_SOURCE                (0x07U)                              ///< Number of DeepStop wake-up sources
#define BSP_FEATURE_CHIP_STANDBY_IOHOLD_REG_NAME                              (0x01U)                              ///< IOHOLD register index for standby state retention

/** GTM */
#define BSP_FEATURE_GTM_VALID_CHANNEL_MASK                                    (0x1FU)                              ///< Mask of available GTM channels

/** OSTM */
#define BSP_FEATURE_OSTM_VALID_CHANNEL_MASK                                   (0x0FU)                              ///< Mask of available OSTM channels

/** ADC */
#define BSP_FEATURE_ADC_VALID_UNIT_MASK                                       (3U)                                 ///< ADC valid unit mask (units 0 and 1)

/** GUARD */
#define BSP_FEATURE_PEGCAP_M_IS_AVAILABLE                                     (0x01U)                              ///< PEGCAP_M guard availability
#define BSP_FEATURE_PE1_GUARD_IS_AVAILABLE                                    (1U)                                 ///< PE1 guard enable
#define BSP_FEATURE_PE2_GUARD_IS_AVAILABLE                                    (1U)                                 ///< PE2 guard enable
#define BSP_FEATURE_PE3_GUARD_IS_AVAILABLE                                    (1U)                                 ///< PE3 guard enable
#define BSP_FEATURE_CRAM0_GUARD_IS_AVAILABLE                                  (1U)                                 ///< CRAM0 guard enable
#define BSP_FEATURE_CRAM1_GUARD_IS_AVAILABLE                                  (1U)                                 ///< CRAM1 guard enable
#define BSP_FEATURE_CRAM_GUARD_STATUS_MAX                                     (8U)                                 ///< Maximum CRAM guard region count
#define BSP_FEATURE_PE_GUARD_STATUS_MAX                                       (5U)                                 ///< Maximum PE guard region count
#define BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE                                   (1U)                                 ///< IPIR guard availability
#define BSP_FEATURE_BARR_GUARD_IS_AVAILABLE                                   (1U)                                 ///< Barrier guard availability
#define BSP_FEATURE_TPTMn_GUARD_MAX_CHANNEL                                   (4U)                                 ///< Max TPTMn guard channels
#define BSP_FEATURE_INTC2_GUARD_MAX_CHANNEL                                   (1023U)                              ///< Max INTC2 guard channels
#define BSP_FEATURE_INTC2_GUARD_MIN_CHANNEL                                   (32U)                                ///< Min INTC2 guard channel index
#define BSP_FEATURE_DTS_GUARD_MAX_CHANNEL                                     (128U)                               ///< Max DTS guard channels
#define BSP_FEATURE_DMAC_GUARD_MAX_CHANNEL                                    (16U)                                ///< Max DMAC guard channels

/** DMAC */
#define BSP_FEATURE_DMAC_MAX_CHANNEL                                          (16U)                                ///< Maximum DMAC channels
#define BSP_FEATURE_DMAC_MAX_UNIT                                             (2U)                                 ///< Maximum DMAC units

/** DTS */
#define BSP_FEATURE_DTS_MAX_CHANNEL                                           (128U)                               ///< Maximum DTS channels

/** MSPI */
#define BSP_FEATURE_MSPI_MAX_UNIT                                             (10U)                                ///< Number of MSPI units
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_0                            (12U)                                ///< Chip selects for MSPI unit 0
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_1                            (12U)                                ///< Chip selects for MSPI unit 1
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_2                            (12U)                                ///< Chip selects for MSPI unit 2
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_3                            (4U)                                 ///< Chip selects for MSPI unit 3
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_4                            (4U)                                 ///< Chip selects for MSPI unit 4
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_5                            (4U)                                 ///< Chip selects for MSPI unit 5
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_6                            (1U)                                 ///< Chip selects for MSPI unit 6
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_7                            (4U)                                 ///< Chip selects for MSPI unit 7
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_8                            (4U)                                 ///< Chip selects for MSPI unit 8
#define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_9                            (1U)                                 ///< Chip selects for MSPI unit 9

#define BSP_FEATURE_ATU_GTM_SEL                                               (BSP_OPBT_SETTING_OPBT8 & (1 << 24)) ///< ATU->GTM trigger select bit

#define BSP_FEATURE_MSPI_ALT_DMA_UNIT                                         (0xCCCU)                             ///< ALT-DMA unit mapping bitmask
#define BSP_FEATURE_MSPI_ALT_DTS_UNIT                                         (0xF00U)                             ///< ALT-DTS unit mapping bitmask
#define BSP_FEATURE_MSPI_ALT_DMA_MAX_UNIT                                     (3U)                                 ///< Max MSPI units supporting ALT DMA
#define BSP_FEATURE_MSPI_ALT_DTS_MAX_UNIT                                     (2U)                                 ///< Max MSPI units supporting ALT DTS
#define BSP_FEATURE_MSPI_ALT_GAP_CHANNEL                                      (4U)                                 ///< Gap between ALT channel groups
#define BSP_FEATURE_MSPI_ALT_GAP_UNIT                                         (6U)                                 ///< Gap between ALT-enabled MSPI units
#define BSP_FEATURE_MSPI_ALT_PAIR_CHANNEL                                     (2U)                                 ///< ALT channel pairing size
#define BSP_FEATURE_MSPI_ALT_MAX_CHANNEL                                      (11U)                                ///< Maximum ALT-enabled channel index
#define BSP_FEATURE_MSPI_ALT_DMA_MAX_BIT                                      (17U)                                ///< Highest ALT DMA register bit used
#define BSP_FEATURE_MSPI_ALT_DTS_MAX_BIT                                      (9U)                                 ///< Highest ALT DTS register bit used

#define BSP_FEATURE_DTSAS06                                                   (0U)                                 ///< ALT DMA unit2 channel3 mapping bit position
#define BSP_FEATURE_DTSAS07                                                   (1U)                                 ///< ALT DMA unit2 channel1 mapping bit position
#define BSP_FEATURE_UNIT_HAS_MORE_CHANNEL                                     (2U)                                 ///< Number of MSPI units with > 7 channels

/** ENCA */
#define BSP_FEATURE_ENCA_MAX_CHANNEL                                          (2U)                                 ///< Maximum ENCA channels supported

/** TSG3 */
#define BSP_FEATURE_TSG3_MAX_UNIT                                             (3U)                                 ///< Maximum TSG3 units
#define BSP_FEATURE_TSG3_COMPARE_MATCH_INTERRUPT_SKIPPING_FUNCTION_SUPPORT    (1U)                                 ///< Compare-match interrupt skip supported
#define BSP_FEATURE_TSG3_OUTPUT_INACTIVE_FUNCTION_SUPPORT                     (1U)                                 ///< Inactive-output function supported
#define BSP_FEATURE_TSG3_OUTPUT_CONTROL_BY_INTPUT_TERMINAL_SUPPORT            (0U)                                 ///< No input-terminal output control support
#define BSP_FEATURE_TSG3_EMU3S_IS_AVAILABLE                                   (0U)                                 ///< EMU3S not available for TSG3

/** TAUD */
#define BSP_FEATURE_TAUD_MAX_UNIT                                             (4U)                                 ///< Maximum TAUD units

/** CANFD */
#define BSP_FEATURE_CANFD_NUM_CHANNELS                                        (8UL)                                ///< Number of CANFD channels
#define BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL                                   (3UL)                                ///< Number of CFIFO channels
#define BSP_FEATURE_CANFD_NUM_RXFIFO                                          (8UL)                                ///< Number of CANFD RX FIFOs
#define BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT                                  (24UL)                               ///< Total CFIFO entries per unit
#define BSP_FEATURE_CANFD_NUM_TXQUEUE_CHANNEL                                 (4UL)                                ///< Number of TX queue channels
#define BSP_FEATURE_CANFD_NUM_RX_FIFO_FFI_VM                                  (8UL)                                ///< Number of RX FIFO for FFI/VM
#define BSP_FEATURE_CANFD_TXMB_SUPPORTED_SLOT                                 (0x0000FFFF0000FFFFULL)              ///< Supported TXMB slots (0–15, 32–47)
#define BSP_FEATURE_CANFD_CLKC_SRC                                            (FSP_PRIV_CLOCK_CLK_CANFD_C)         ///< CANFD CLKC source
#define BSP_FEATURE_CANFD_PN_MODE                                             (1U)                                 ///< Pretended Network Mode supported
#define BSP_FEATURE_CANFD_FFI_MODE                                            (1U)                                 ///< FFI Mode supported

/** RIIC */
#define BSP_FEATURE_IIC_HAS_SYS_CLK                                           (0U)                                 ///< IIC system-clock support unavailable
#define BSP_FEATURE_IIC_VALID_CHANNEL_MASK                                    (0x03U)                              ///< Valid RIIC channels bitmask

/** INTSEL */
#define BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ                                     (1U)                                 ///< INTSEL IRQ selection supported

/** RSENT */
#define BSP_FEATURE_RSENT_SENT_NUM_OF_UNIT                                    (30U)                                ///< Number of RSENT units

/** MSPI */
#define BSP_FEATURE_MSPI_RAM_WORDS_MAX                                        (512U)                               ///< MSPI RAM size

/** TAUD3P */
#define BSP_FEATURE_TAUD_THREE_PHASE_NUMBER_OF_UNIT                           (3U)                                 ///< TAUD three-phase control units

/** PIC */
#define BSP_FEATURE_PIC_ADC_NUMBER_UNIT                                       (4U)                                 ///< Number of PIC ADC units
#define BSP_FEATURE_PIC_ADC_NUMBER_SG                                         (6U)                                 ///< Number of ADC scan groups
#define BSP_FEATURE_PIC_TAUDTINSEL_NUMBER                                     (4U)                                 ///< Number of TAUDTINSEL signals
#define BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT                                    (2U)                                 ///< TAUD units in PIC20
#define BSP_FEATURE_PIC21_TAUD_NUMBER_UNIT                                    (2U)                                 ///< TAUD units in PIC21
#define BSP_FEATURE_PIC_TSG3_NUMBER_UNIT                                      (3U)                                 ///< TSG3 units in PIC
#define BSP_FEATURE_PIC_ADTCFG_NUMBER                                         (10U)                                ///< ADTCFG count

#define BSP_FEATURE_PIC_DSADTCFG_NUMBER                                       (14U)                                ///< DSADTCFG entries
#define BSP_FEATURE_PIC_DFETSEN_NUMBER                                        (22U)                                ///< DFETSEN entries
#define BSP_FEATURE_PIC_FCMPISEN_NUMBER_REG                                   (8U)                                 ///< FCMPISEN registers
#define BSP_FEATURE_PIC_GTMSEN_NUMBER_REG                                     (8U)                                 ///< GTMSEN registers
#define BSP_FEATURE_PIC_PSI5S_NUMBER_UNIT                                     (2U)                                 ///< PSI5S units
#define BSP_FEATURE_PIC_PSI5SEN_NUMBER                                        (7U)                                 ///< PSI5SEN entries
#define BSP_FEATURE_PIC_PSI5EN_NUMBER                                         (6U)                                 ///< PSI5EN entries
#define BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP                                  (7U)                                 ///< GTMINEN groups
#define BSP_FEATURE_PIC_CANIOSEN_NUMBER                                       (3U)                                 ///< CANIOSEN entries
#define BSP_FEATURE_PIC_GTMINTIOEN_NUMBER_GROUP                               (8U)                                 ///< GTM INTIO enable groups
#define BSP_FEATURE_PIC_GTMMSPI_NUMBER                                        (10U)                                ///< GTM MSPI link entries
#define BSP_FEATURE_PIC_ABFGODSEN_NUMBER                                      (14U)                                ///< ABFGODSEN entries

/** FLXA */
#define BSP_FEATURE_FLXA_MAX_UNIT                                             (1U)                                 ///< Maximum FLXA units supported

/** ETHTSN */
#define BSP_FEATURE_ETHTSN_DUAL_CHANNEL_SUPPORT                               (0UL)                                ///< TSN supports only single channel
#define ETHER_TSN_TOTAL_CTRL_CONFIG                                           (1UL)                                ///< TSN total controller configuration (single)

/** CRC */
#define BSP_FEATURE_CRC_COMMON_GROUP                                          (1U)                                 ///< CRC common group supported

/** IOPORT */
#define BSP_FEATURE_IOPORT_NF_RLIN30                                          (1U)                                 ///< IOPORT Noise filter RLIN30 signal
#define BSP_FEATURE_IOPORT_NF_RLIN31                                          (1U)                                 ///< IOPORT Noise filter RLIN31 signal
#define BSP_FEATURE_IOPORT_NF_RSCAN                                           (1U)                                 ///< IOPORT Noise filter RSCAN signal
#define BSP_FEATURE_IOPORT_NF_FLXA0                                           (1U)                                 ///< IOPORT Noise filter FLXA0 signal
#define BSP_FEATURE_IOPORT_NF_FLXA1                                           (0U)                                 ///< IOPORT Noise filter FLXA1 signal
#define BSP_FEATURE_IOPORT_NF_RSENT0                                          (1U)                                 ///< IOPORT Noise filter RSENT0 signal
#define BSP_FEATURE_IOPORT_NF_RSENT1                                          (1U)                                 ///< IOPORT Noise filter RSENT1 signal
#define BSP_FEATURE_IOPORT_NF_RSENT2                                          (1U)                                 ///< IOPORT Noise filter RSENT2 signal
#define BSP_FEATURE_IOPORT_NF_RSENT3                                          (1U)                                 ///< IOPORT Noise filter RSENT3 signal
#define BSP_FEATURE_IOPORT_NF_RSENT4                                          (1U)                                 ///< IOPORT Noise filter RSENT4 signal
#define BSP_FEATURE_IOPORT_NF_RSENT5                                          (1U)                                 ///< IOPORT Noise filter RSENT5 signal
#define BSP_FEATURE_IOPORT_NF_RSENT6                                          (1U)                                 ///< IOPORT Noise filter RSENT6 signal
#define BSP_FEATURE_IOPORT_NF_RSENT7                                          (1U)                                 ///< IOPORT Noise filter RSENT7 signal
#define BSP_FEATURE_IOPORT_NF_RSENT8                                          (1U)                                 ///< IOPORT Noise filter RSENT8 signal
#define BSP_FEATURE_IOPORT_NF_RSENT9                                          (1U)                                 ///< IOPORT Noise filter RSENT9 signal
#define BSP_FEATURE_IOPORT_NF_RSENT10                                         (1U)                                 ///< IOPORT Noise filter RSENT10 signal
#define BSP_FEATURE_IOPORT_NF_RSENT11                                         (1U)                                 ///< IOPORT Noise filter RSENT11 signal
#define BSP_FEATURE_IOPORT_NF_RSENT12                                         (1U)                                 ///< IOPORT Noise filter RSENT12 signal
#define BSP_FEATURE_IOPORT_NF_RSENT13                                         (1U)                                 ///< IOPORT Noise filter RSENT13 signal
#define BSP_FEATURE_IOPORT_NF_RSENT14                                         (1U)                                 ///< IOPORT Noise filter RSENT14 signal
#define BSP_FEATURE_IOPORT_NF_RSENT15                                         (1U)                                 ///< IOPORT Noise filter RSENT15 signal
#define BSP_FEATURE_IOPORT_NF_RSENT16                                         (1U)                                 ///< IOPORT Noise filter RSENT16 signal
#define BSP_FEATURE_IOPORT_NF_RSENT17                                         (1U)                                 ///< IOPORT Noise filter RSENT17 signal
#define BSP_FEATURE_IOPORT_NF_RSENT18                                         (1U)                                 ///< IOPORT Noise filter RSENT18 signal
#define BSP_FEATURE_IOPORT_NF_RSENT19                                         (1U)                                 ///< IOPORT Noise filter RSENT19 signal
#define BSP_FEATURE_IOPORT_NF_RSENT20                                         (1U)                                 ///< IOPORT Noise filter RSENT20 signal
#define BSP_FEATURE_IOPORT_NF_RSENT21                                         (1U)                                 ///< IOPORT Noise filter RSENT21 signal
#define BSP_FEATURE_IOPORT_NF_RSENT22                                         (1U)                                 ///< IOPORT Noise filter RSENT22 signal
#define BSP_FEATURE_IOPORT_NF_RSENT23                                         (1U)                                 ///< IOPORT Noise filter RSENT23 signal
#define BSP_FEATURE_IOPORT_NF_RSENT24                                         (1U)                                 ///< IOPORT Noise filter RSENT24 signal
#define BSP_FEATURE_IOPORT_NF_RSENT25                                         (1U)                                 ///< IOPORT Noise filter RSENT25 signal
#define BSP_FEATURE_IOPORT_NF_RSENT26                                         (1U)                                 ///< IOPORT Noise filter RSENT26 signal
#define BSP_FEATURE_IOPORT_NF_RSENT27                                         (1U)                                 ///< IOPORT Noise filter RSENT27 signal
#define BSP_FEATURE_IOPORT_NF_RSENT28                                         (1U)                                 ///< IOPORT Noise filter RSENT28 signal
#define BSP_FEATURE_IOPORT_NF_RSENT29                                         (1U)                                 ///< IOPORT Noise filter RSENT29 signal
#define BSP_FEATURE_IOPORT_NF_PSI50                                           (1U)                                 ///< IOPORT Noise filter PSI50 signal
#define BSP_FEATURE_IOPORT_NF_PSI51                                           (1U)                                 ///< IOPORT Noise filter PSI51 signal
#define BSP_FEATURE_IOPORT_NF_PSI52                                           (1U)                                 ///< IOPORT Noise filter PSI52 signal
#define BSP_FEATURE_IOPORT_NF_PSI53                                           (1U)                                 ///< IOPORT Noise filter PSI53 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA0                                       (1U)                                 ///< IOPORT Noise filter PIC_TAPA0 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA1                                       (1U)                                 ///< IOPORT Noise filter PIC_TAPA1 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA2                                       (1U)                                 ///< IOPORT Noise filter PIC_TAPA2 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA3                                       (1U)                                 ///< IOPORT Noise filter PIC_TAPA3 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA4                                       (1U)                                 ///< IOPORT Noise filter PIC_TAPA4 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA5                                       (1U)                                 ///< IOPORT Noise filter PIC_TAPA5 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ0                                           (0U)                                 ///< IOPORT Noise filter ADCJ0 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ1                                           (0U)                                 ///< IOPORT Noise filter ADCJ1 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ2                                           (0U)                                 ///< IOPORT Noise filter ADCJ2 signal
#define BSP_FEATURE_IOPORT_NF_ADCK0                                           (1U)                                 ///< IOPORT Noise filter ADCK0 signal
#define BSP_FEATURE_IOPORT_NF_ADCK1                                           (1U)                                 ///< IOPORT Noise filter ADCK1 signal
#define BSP_FEATURE_IOPORT_NF_ADCK2                                           (1U)                                 ///< IOPORT Noise filter ADCK2 signal
#define BSP_FEATURE_IOPORT_NF_ADCK3                                           (1U)                                 ///< IOPORT Noise filter ADCK3 signal
#define BSP_FEATURE_IOPORT_NF_ADCKA                                           (1U)                                 ///< IOPORT Noise filter ADCKA signal
#define BSP_FEATURE_IOPORT_NF_RHSB0                                           (1U)                                 ///< IOPORT Noise filter RHSB0 signal
#define BSP_FEATURE_IOPORT_NF_RHSB1                                           (1U)                                 ///< IOPORT Noise filter RHSB1 signal
#define BSP_FEATURE_IOPORT_NF_RHSB2                                           (1U)                                 ///< IOPORT Noise filter RHSB2 signal
#define BSP_FEATURE_IOPORT_NF_RHSB3                                           (1U)                                 ///< IOPORT Noise filter RHSB3 signal
#define BSP_FEATURE_IOPORT_NF_ENCA0                                           (1U)                                 ///< IOPORT Noise filter ENCA0 signal
#define BSP_FEATURE_IOPORT_NF_ENCA1                                           (1U)                                 ///< IOPORT Noise filter ENCA1 signal
#define BSP_FEATURE_IOPORT_NF_TAUD0                                           (1U)                                 ///< IOPORT Noise filter TAUD0 signal
#define BSP_FEATURE_IOPORT_NF_TAUD1                                           (1U)                                 ///< IOPORT Noise filter TAUD1 signal
#define BSP_FEATURE_IOPORT_NF_TAUD2                                           (1U)                                 ///< IOPORT Noise filter TAUD2 signal
#define BSP_FEATURE_IOPORT_NF_TAUD3                                           (1U)                                 ///< IOPORT Noise filter TAUD3 signal
#define BSP_FEATURE_IOPORT_NF_TSG30                                           (1U)                                 ///< IOPORT Noise filter TSG30 signal
#define BSP_FEATURE_IOPORT_NF_TSG31                                           (1U)                                 ///< IOPORT Noise filter TSG31 signal
#define BSP_FEATURE_IOPORT_NF_TSG32                                           (1U)                                 ///< IOPORT Noise filter TSG32 signal
#define BSP_FEATURE_IOPORT_NF_IRQ0                                            (1U)                                 ///< IOPORT Noise filter IRQ0 signal
#define BSP_FEATURE_IOPORT_NF_IRQ1                                            (1U)                                 ///< IOPORT Noise filter IRQ1 signal
#define BSP_FEATURE_IOPORT_NF_IRQ2                                            (1U)                                 ///< IOPORT Noise filter IRQ2 signal
#define BSP_FEATURE_IOPORT_NF_IRQ3                                            (1U)                                 ///< IOPORT Noise filter IRQ3 signal
#define BSP_FEATURE_IOPORT_NF_IRQ4                                            (1U)                                 ///< IOPORT Noise filter IRQ4 signal
#define BSP_FEATURE_IOPORT_NF_IRQ5                                            (1U)                                 ///< IOPORT Noise filter IRQ5 signal
#define BSP_FEATURE_IOPORT_NF_IRQ6                                            (1U)                                 ///< IOPORT Noise filter IRQ6 signal
#define BSP_FEATURE_IOPORT_NF_IRQ7                                            (1U)                                 ///< IOPORT Noise filter IRQ7 signal
#define BSP_FEATURE_IOPORT_NF_IRQ8                                            (1U)                                 ///< IOPORT Noise filter IRQ8 signal
#define BSP_FEATURE_IOPORT_NF_IRQ9                                            (1U)                                 ///< IOPORT Noise filter IRQ9 signal
#define BSP_FEATURE_IOPORT_NF_IRQ10                                           (1U)                                 ///< IOPORT Noise filter IRQ10 signal
#define BSP_FEATURE_IOPORT_NF_IRQ11                                           (1U)                                 ///< IOPORT Noise filter IRQ11 signal
#define BSP_FEATURE_IOPORT_NF_IRQ12                                           (1U)                                 ///< IOPORT Noise filter IRQ12 signal
#define BSP_FEATURE_IOPORT_NF_IRQ13                                           (1U)                                 ///< IOPORT Noise filter IRQ13 signal
#define BSP_FEATURE_IOPORT_NF_IRQ14                                           (1U)                                 ///< IOPORT Noise filter IRQ14 signal
#define BSP_FEATURE_IOPORT_NF_IRQ15                                           (1U)                                 ///< IOPORT Noise filter IRQ15 signal
#define BSP_FEATURE_IOPORT_NF_IRQ16                                           (1U)                                 ///< IOPORT Noise filter IRQ16 signal
#define BSP_FEATURE_IOPORT_NF_IRQ17                                           (1U)                                 ///< IOPORT Noise filter IRQ17 signal
#define BSP_FEATURE_IOPORT_NF_IRQ18                                           (1U)                                 ///< IOPORT Noise filter IRQ18 signal
#define BSP_FEATURE_IOPORT_NF_IRQ19                                           (1U)                                 ///< IOPORT Noise filter IRQ19 signal
#define BSP_FEATURE_IOPORT_NF_IRQ20                                           (1U)                                 ///< IOPORT Noise filter IRQ20 signal
#define BSP_FEATURE_IOPORT_NF_IRQ21                                           (1U)                                 ///< IOPORT Noise filter IRQ21 signal
#define BSP_FEATURE_IOPORT_NF_IRQ22                                           (1U)                                 ///< IOPORT Noise filter IRQ22 signal
#define BSP_FEATURE_IOPORT_NF_IRQ23                                           (1U)                                 ///< IOPORT Noise filter IRQ23 signal
#define BSP_FEATURE_IOPORT_NF_IRQ24                                           (1U)                                 ///< IOPORT Noise filter IRQ24 signal
#define BSP_FEATURE_IOPORT_NF_IRQ25                                           (1U)                                 ///< IOPORT Noise filter IRQ25 signal
#define BSP_FEATURE_IOPORT_NF_IRQ26                                           (1U)                                 ///< IOPORT Noise filter IRQ26 signal
#define BSP_FEATURE_IOPORT_NF_IRQ27                                           (1U)                                 ///< IOPORT Noise filter IRQ27 signal
#define BSP_FEATURE_IOPORT_NF_IRQ28                                           (1U)                                 ///< IOPORT Noise filter IRQ28 signal
#define BSP_FEATURE_IOPORT_NF_IRQ29                                           (1U)                                 ///< IOPORT Noise filter IRQ29 signal
#define BSP_FEATURE_IOPORT_NF_IRQ30                                           (1U)                                 ///< IOPORT Noise filter IRQ30 signal
#define BSP_FEATURE_IOPORT_NF_IRQ31                                           (1U)                                 ///< IOPORT Noise filter IRQ31 signal
#define BSP_FEATURE_IOPORT_NF_IRQ32                                           (1U)                                 ///< IOPORT Noise filter IRQ32 signal
#define BSP_FEATURE_IOPORT_NF_IRQ33                                           (1U)                                 ///< IOPORT Noise filter IRQ33 signal
#define BSP_FEATURE_IOPORT_NF_IRQ34                                           (1U)                                 ///< IOPORT Noise filter IRQ34 signal
#define BSP_FEATURE_IOPORT_NF_IRQ35                                           (1U)                                 ///< IOPORT Noise filter IRQ35 signal
#define BSP_FEATURE_IOPORT_NF_IRQ36                                           (1U)                                 ///< IOPORT Noise filter IRQ36 signal
#define BSP_FEATURE_IOPORT_NF_IRQ37                                           (1U)                                 ///< IOPORT Noise filter IRQ37 signal
#define BSP_FEATURE_IOPORT_NF_IRQ38                                           (1U)                                 ///< IOPORT Noise filter IRQ38 signal
#define BSP_FEATURE_IOPORT_NF_IRQ39                                           (1U)                                 ///< IOPORT Noise filter IRQ39 signal
#define BSP_FEATURE_IOPORT_NF_IRQ_NMI                                         (1U)                                 ///< IOPORT Noise filter IRQ_NMI signal
#define BSP_FEATURE_IOPORT_NF_IRQ40                                           (1U)                                 ///< IOPORT Noise filter IRQ40 signal
#define BSP_FEATURE_IOPORT_NF_IRQ41                                           (1U)                                 ///< IOPORT Noise filter IRQ41 signal
#define BSP_FEATURE_IOPORT_NF_IRQ42                                           (1U)                                 ///< IOPORT Noise filter IRQ42 signal
#define BSP_FEATURE_IOPORT_NF_IRQ43                                           (1U)                                 ///< IOPORT Noise filter IRQ43 signal
#define BSP_FEATURE_IOPORT_NF_IRQ44                                           (0U)                                 ///< IOPORT Noise filter IRQ44 signal
#define BSP_FEATURE_IOPORT_NF_IRQ45                                           (0U)                                 ///< IOPORT Noise filter IRQ45 signal
#define BSP_FEATURE_IOPORT_NF_IRQ_ESR                                         (0U)                                 ///< IOPORT Noise filter IRQ_ESR signal
#define BSP_FEATURE_IOPORT_NF_INTP_NMI                                        (0U)                                 ///< IOPORT Noise filter INTP_NMI signal
#define BSP_FEATURE_IOPORT_NF_INTP0                                           (0U)                                 ///< IOPORT Noise filter INTP0 signal
#define BSP_FEATURE_IOPORT_NF_INTP1                                           (0U)                                 ///< IOPORT Noise filter INTP1 signal
#define BSP_FEATURE_IOPORT_NF_INTP2                                           (0U)                                 ///< IOPORT Noise filter INTP2 signal
#define BSP_FEATURE_IOPORT_NF_INTP3                                           (0U)                                 ///< IOPORT Noise filter INTP3 signal
#define BSP_FEATURE_IOPORT_NF_INTP4                                           (0U)                                 ///< IOPORT Noise filter INTP4 signal
#define BSP_FEATURE_IOPORT_NF_INTP5                                           (0U)                                 ///< IOPORT Noise filter INTP5 signal
#define BSP_FEATURE_IOPORT_NF_INTP6                                           (0U)                                 ///< IOPORT Noise filter INTP6 signal
#define BSP_FEATURE_IOPORT_NF_INTP7                                           (0U)                                 ///< IOPORT Noise filter INTP7 signal
#define BSP_FEATURE_IOPORT_NF_INTP8                                           (0U)                                 ///< IOPORT Noise filter INTP8 signal
#define BSP_FEATURE_IOPORT_NF_INTP9                                           (0U)                                 ///< IOPORT Noise filter INTP9 signal
#define BSP_FEATURE_IOPORT_NF_INTP10                                          (0U)                                 ///< IOPORT Noise filter INTP10 signal
#define BSP_FEATURE_IOPORT_NF_INTP11                                          (0U)                                 ///< IOPORT Noise filter INTP11 signal
#define BSP_FEATURE_IOPORT_NF_INTP12                                          (0U)                                 ///< IOPORT Noise filter INTP12 signal
#define BSP_FEATURE_IOPORT_NF_INTP13                                          (0U)                                 ///< IOPORT Noise filter INTP13 signal
#define BSP_FEATURE_IOPORT_NF_INTP14                                          (0U)                                 ///< IOPORT Noise filter INTP14 signal
#define BSP_FEATURE_IOPORT_NF_INTP15                                          (0U)                                 ///< IOPORT Noise filter INTP15 signal
#define BSP_FEATURE_IOPORT_NF_INTP16                                          (0U)                                 ///< IOPORT Noise filter INTP16 signal
#define BSP_FEATURE_IOPORT_NF_INTP17                                          (0U)                                 ///< IOPORT Noise filter INTP17 signal
#define BSP_FEATURE_IOPORT_NF_INTP18                                          (0U)                                 ///< IOPORT Noise filter INTP18 signal
#define BSP_FEATURE_IOPORT_NF_INTP19                                          (0U)                                 ///< IOPORT Noise filter INTP19 signal
#define BSP_FEATURE_IOPORT_NF_INTP20                                          (0U)                                 ///< IOPORT Noise filter INTP20 signal
#define BSP_FEATURE_IOPORT_NF_INTP21                                          (0U)                                 ///< IOPORT Noise filter INTP21 signal
#define BSP_FEATURE_IOPORT_NF_INTP22                                          (0U)                                 ///< IOPORT Noise filter INTP22 signal
#define BSP_FEATURE_IOPORT_NF_INTP23                                          (0U)                                 ///< IOPORT Noise filter INTP23 signal
#define BSP_FEATURE_IOPORT_NF_INTP24                                          (0U)                                 ///< IOPORT Noise filter INTP24 signal
#define BSP_FEATURE_IOPORT_NF_INTP25                                          (0U)                                 ///< IOPORT Noise filter INTP25 signal
#define BSP_FEATURE_IOPORT_NF_INTP26                                          (0U)                                 ///< IOPORT Noise filter INTP26 signal
#define BSP_FEATURE_IOPORT_NF_INTP27                                          (0U)                                 ///< IOPORT Noise filter INTP27 signal
#define BSP_FEATURE_IOPORT_NF_INTP28                                          (0U)                                 ///< IOPORT Noise filter INTP28 signal
#define BSP_FEATURE_IOPORT_NF_INTP29                                          (0U)                                 ///< IOPORT Noise filter INTP29 signal
#define BSP_FEATURE_IOPORT_NF_INTP30                                          (0U)                                 ///< IOPORT Noise filter INTP30 signal
#define BSP_FEATURE_IOPORT_NF_INTP31                                          (0U)                                 ///< IOPORT Noise filter INTP31 signal
#define BSP_FEATURE_IOPORT_NF_INTP32                                          (0U)                                 ///< IOPORT Noise filter INTP32 signal
#define BSP_FEATURE_IOPORT_NF_INTP33                                          (0U)                                 ///< IOPORT Noise filter INTP33 signal
#define BSP_FEATURE_IOPORT_NF_INTP34                                          (0U)                                 ///< IOPORT Noise filter INTP34 signal
#define BSP_FEATURE_IOPORT_NF_INTP35                                          (0U)                                 ///< IOPORT Noise filter INTP35 signal
#define BSP_FEATURE_IOPORT_NF_INTP36                                          (0U)                                 ///< IOPORT Noise filter INTP36 signal
#define BSP_FEATURE_IOPORT_NF_INTP37                                          (0U)                                 ///< IOPORT Noise filter INTP37 signal
#define BSP_FEATURE_IOPORT_NF_INTP38                                          (0U)                                 ///< IOPORT Noise filter INTP38 signal
#define BSP_FEATURE_IOPORT_NF_INTP39                                          (0U)                                 ///< IOPORT Noise filter INTP39 signal
#define BSP_FEATURE_IOPORT_NF_ECM0                                            (1U)                                 ///< IOPORT Noise filter ECM0 signal
#define BSP_FEATURE_IOPORT_NF_ECM1                                            (1U)                                 ///< IOPORT Noise filter ECM1 signal
#define BSP_FEATURE_IOPORT_NF_ECM2                                            (1U)                                 ///< IOPORT Noise filter ECM2 signal
#define BSP_FEATURE_IOPORT_NF_ECM3                                            (1U)                                 ///< IOPORT Noise filter ECM3 signal
#define BSP_FEATURE_IOPORT_NF_ECM4                                            (1U)                                 ///< IOPORT Noise filter ECM4 signal
#define BSP_FEATURE_IOPORT_NF_ECM5                                            (1U)                                 ///< IOPORT Noise filter ECM5 signal
#define BSP_FEATURE_IOPORT_NF_ECM6                                            (1U)                                 ///< IOPORT Noise filter ECM6 signal
#define BSP_FEATURE_IOPORT_NF_ECM7                                            (1U)                                 ///< IOPORT Noise filter ECM7 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ0                                           (0U)                                 ///< IOPORT Noise filter TAUJ0 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ1                                           (0U)                                 ///< IOPORT Noise filter TAUJ1 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ2                                           (1U)                                 ///< IOPORT Noise filter TAUJ2 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ3                                           (1U)                                 ///< IOPORT Noise filter TAUJ3 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TSG                                         (1U)                                 ///< IOPORT Noise filter PIC_TSG signal
#define BSP_FEATURE_IOPORT_NF_EMU0                                            (1U)                                 ///< IOPORT Noise filter EMU0 signal
#define BSP_FEATURE_IOPORT_NF_EMU1                                            (1U)                                 ///< IOPORT Noise filter EMU1 signal
#define BSP_FEATURE_IOPORT_NF_SSIF                                            (0U)                                 ///< IOPORT Noise filter SSIF signal
#define BSP_FEATURE_IOPORT_NF_OSPI0                                           (0U)                                 ///< IOPORT Noise filter OSPI0 signal

#define BSP_FEATURE_IOPORT_LVDSA                                              (1U)                                 ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSB                                              (1U)                                 ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSC                                              (1U)                                 ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSD                                              (1U)                                 ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSE                                              (1U)                                 ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSF                                              (1U)                                 ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDS_CTRLA_CONFIGURATION                           (0x00090000)                         ///< IOPORT LVDSCTRLA configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLB_CONFIGURATION                           (0x00090000)                         ///< IOPORT LVDSCTRLB configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLC_CONFIGURATION                           (0x00030000)                         ///< IOPORT LVDSCTRLC configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLD_CONFIGURATION                           (0x00010000)                         ///< IOPORT LVDSCTRLD configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLE_CONFIGURATION                           (0x00090000)                         ///< IOPORT LVDSCTRLE configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLF_CONFIGURATION                           (0x00100000)                         ///< IOPORT LVDSCTRLF configuration mask register

#define BSP_FEATURE_IOPORT_LVDS_CTRLA_ENABLE                                  (0x00090009)                         ///< IOPORT LVDSCTRLA enable and configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLB_ENABLE                                  (0x00090039)                         ///< IOPORT LVDSCTRLB enable and configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLC_ENABLE                                  (0x0003001B)                         ///< IOPORT LVDSCTRLC enable and configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLD_ENABLE                                  (0x0001000B)                         ///< IOPORT LVDSCTRLD enable and configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLE_ENABLE                                  (0x00090009)                         ///< IOPORT LVDSCTRLE enable and configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLF_ENABLE                                  (0x00100038)                         ///< IOPORT LVDSCTRLF enable and configuration mask register

#define BSP_FEATURE_IOPORT_PST                                                (7U)                                 ///< Number of ECM error trigger
#define BSP_FEATURE_IOPORT_PEIODC                                             (1U)                                 ///< IOPORT enable/disable ERRORIN Open-drain feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_PULVSEL5                                           (1U)                                 ///< IOPORT enable/disable PULVSEL5 feature (0U): Disable/ (1U): Enable

/** HRPWM */
#define BSP_FEATURE_HRPWM_VALID_CHANNEL_MASK                                  (0x1U)                               ///< Valid HRPWM channel mask

/** PIC — advanced signals */
#define BSP_FEATURE_PIC_TAUD_INT_SIGNAL_OUT_NUMBER                            (3U)                                 ///< TAUD interrupt outputs
#define BSP_FEATURE_PIC_TAUD_TRIANGLE_WAVE_NUMBER                             (4U)                                 ///< Number of triangle-wave generators
#define BSP_FEATURE_PIC_TAUD_PWM_OUTPUT_NUMBER                                (4U)                                 ///< TAUD PWM output count
#define BSP_FEATURE_PIC_TAUD_DELAY_PULSE_NUMBER                               (4U)                                 ///< TAUD delay pulse count
#define BSP_FEATURE_PIC_ENCA_FEATURE_NUMBER_UNIT                              (2U)                                 ///< ENCA-capable units

#define BSP_FEATURE_PIC_ADCK_TRIG_SEL                                         (1U)                                 ///< ADCK trigger select supported
#define BSP_FEATURE_PIC_ADCK_TRIG_SEL_ATU                                     (1U)                                 ///< ADCK trigger via ATU supported
#define BSP_FEATURE_PIC_SWITCH_TSG_OUT                                        (1U)                                 ///< TSG output switch supported
#define BSP_FEATURE_PIC_SWITCH_TSG_OUT_TSG32                                  (1U)                                 ///< TSG32 output switch supported
#define BSP_FEATURE_PIC_EMU_FEATURE                                           (0U)                                 ///< EMU feature not supported on PIC
#define BSP_FEATURE_PIC_TSG_START_TRIG                                        (1U)                                 ///< TSG start trigger supported
#define BSP_FEATURE_PIC_TSG_START_TRIG_TSG32                                  (1U)                                 ///< TSG32 start trigger supported
#define BSP_FEATURE_PIC_3PHASE_ENCODER_TSG31                                  (1U)                                 ///< 3-phase encoder on TSG31 supported
#define BSP_FEATURE_PIC_2PHASE_SEL_INTENCA1_TO_TSG3                           (1U)                                 ///< ENCA1 -> TSG3 2-phase input supported
#define BSP_FEATURE_PIC_ENC_CAPT_TRIG_ENCA1                                   (1U)                                 ///< ENCA1 capture trigger supported
#define BSP_FEATURE_PIC_INT_SIG_OUT_TAUD1                                     (1U)                                 ///< TAUD1 interrupt output supported
#define BSP_FEATURE_PIC_INT_SIG_OUT_TAUD2                                     (1U)                                 ///< TAUD2 interrupt output supported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT12                         (1U)                                 ///< Pulse-width measurement via DT12 supported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT00                         (0U)                                 ///< DT00 pulse-width measurement unsupported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT10                         (0U)                                 ///< DT10 pulse-width measurement unsupported
#define BSP_FEATURE_PIC_ENCA_INPUT_SEL                                        (1U)                                 ///< ENCA input selection supported
#define BSP_FEATURE_PIC_FCMP_FEATURE                                          (1U)                                 ///< FCMP feature supported
#define BSP_FEATURE_PIC_HRPWM_FEATURE                                         (1U)                                 ///< HRPWM feature supported
#define BSP_FEATURE_PIC_TIMER_INPUT_MONITOR                                   (0U)                                 ///< Timer input monitor not supported
#define BSP_FEATURE_PIC_TAUD_INPUT_SEL                                        (1U)                                 ///< TAUD input selection supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL                                         (1U)                                 ///< ENCA trigger selection supported
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL                                    (1U)                                 ///< ENCA encoder input selection supported
#define BSP_FEATURE_PIC_RDC_FEATURE                                           (1U)                                 ///< RDC feature supported
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE                                      (1U)                                 ///< CAN/GTM IO sharing supported
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL                                         (1U)                                 ///< MSPI trigger selection supported
#define BSP_FEATURE_PIC_GTM_DTM_INPUT_SEL                                     (1U)                                 ///< GTM DTM input selection supported
#define BSP_FEATURE_PIC_TSG3_INPUT_GTM                                        (0U)                                 ///< TSG3 GTM input unsupported
#define BSP_FEATURE_PIC_ADCK_TRIG_SEL_GTM_PIC24                               (1U)                                 ///< ADCK trigger via GTM PIC24 supported
#define BSP_FEATURE_PIC_ADCK_TRIG_SEL_GTM_PIC2_SELB                           (0U)                                 ///< ADCK trigger via GTM PIC2 SELB unsupported
#define BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC13                            (1U)                                 ///< TSG sync/start via PIC13 supported
#define BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC1_SELB                        (0U)                                 ///< TSG sync/start via PIC1 SELB unsupported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD2                                  (0U)                                 ///< TIMER IN SEL B -> TAUD2 unsupported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD_TAUJ                              (0U)                                 ///< TIMER IN SEL B -> TAUD/TAUJ unsupported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B                                        (0U)                                 ///< TIMER IN SEL B unsupported
#define BSP_FEATURE_PIC_TAUD_IN_SEL_PIC13                                     (1U)                                 ///< TAUD input via PIC13 supported
#define BSP_FEATURE_PIC_TAUD_IN_SEL_PIC1_SELB                                 (0U)                                 ///< TAUD input via PIC1 SELB unsupported
#define BSP_FEATURE_PIC_TIMER_OUT_MON_PIC13                                   (1U)                                 ///< Timer output monitor PIC13 supported
#define BSP_FEATURE_PIC_TIMER_OUT_MON_PIC1_SELB                               (0U)                                 ///< Timer output monitor PIC1 SELB unsupported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC22                          (1U)                                 ///< ENCATSEL via PIC22 supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC24                          (1U)                                 ///< ENCATCFG via PIC24 supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC21                          (0U)                                 ///< ENCATSEL via PIC21 unsupported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC22                          (0U)                                 ///< ENCATCFG via PIC22 unsupported
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC24                                      (1U)                                 ///< GTM TIM input via PIC24 supported
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC2_SELB                                  (0U)                                 ///< GTM TIM input via PIC2 SELB unsupported
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC22                                      (0U)                                 ///< GTM TIM input via PIC22 unsupported
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC24                                  (1U)                                 ///< ENCA encoder input via PIC24 supported
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC22                                  (0U)                                 ///< ENCA encoder input via PIC22 unsupported
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC24                                    (1U)                                 ///< Hi-Z control via GTM PIC24 supported
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC2_SELB                                (0U)                                 ///< Hi-Z via GTM PIC2 SELB unsupported
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC20                                    (0U)                                 ///< Hi-Z control via GTM PIC20 unsupported
#define BSP_FEATURE_PIC_RDC_FEATURE_RDC3AL                                    (1U)                                 ///< RDC3AL feature supported
#define BSP_FEATURE_PIC_RDC_FEATURE_RDC3AS                                    (1U)                                 ///< RDC3AS feature supported
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_PIC24                                (1U)                                 ///< CAN/GTM IO share via PIC24 supported
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_PIC2_SELB                            (0U)                                 ///< CAN/GTM IO share via PIC2 SELB unsupported
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL_PIC24                                   (1U)                                 ///< PIC24 MSPI trigger supported
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL_PIC2_SELB                               (0U)                                 ///< PIC2 SELB MSPI trigger unsupported
#define BSP_FEATURE_PIC_TAUD_INPUT_SEL_PIC13                                  (1U)                                 ///< TAUD input select via PIC13 supported
#define BSP_FEATURE_PIC_TAUD_INPUT_SEL_PIC1_SELB                              (0U)                                 ///< TAUD input select via PIC1 SELB unsupported
#define BSP_FEATURE_PIC_ADCK_TRIG_SEL_PIC21                                   (1U)                                 ///< ADCK trigger select via PIC21
#define BSP_FEATURE_PIC_SST_PIC1_MULTI_UNIT                                   (1U)                                 ///< Multi-unit SST on PIC1 supported
#define BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN0                                      (1U)                                 ///< HIZCEN0 supported
#define BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN1                                      (1U)                                 ///< HIZCEN1 supported
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_CAN_NUMBER                           (10U)                                ///< Number of CAN/GTM IO share signals
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL_SIGNAL_NUMBER                           (10U)                                ///< Number of MSPI trigger signals supported
#define BSP_FEATURE_PIC_FEATURE_U2BX                                          (1U)                                 ///< PIC U2Bx feature set supported

/** GTM */
#define BSP_FEATURE_GTM_MCS_NUMBER_INSTANCE                                   (7U)                                 ///< Number of GTM MCS instances
#define BSP_FEATURE_GTM_MCS_NUMBER_CHANNEL                                    (8U)                                 ///< Channels per MCS instance
#define BSP_FEATURE_GTM_MCS_NUMBER_SINT                                       (8U)                                 ///< Number of SINT outputs
#define BSP_FEATURE_GTM_MCS_NUMBER_HBP                                        (2U)                                 ///< Number of HBP outputs
#define BSP_FEATURE_GTM_MCS_NUMBER_TRIGGER                                    (24U)                                ///< Number of GTM trigger sources

/** TAUD */
#define BSP_FEATURE_TAUD_MAX_UNIT                                             (4U)                                 ///< Maximum TAUD units

/** CLMA */
#define BSP_FEATURE_CLMA_MAX_CHANNEL                                          (18U)                                ///< Maximum CLMA channels supported

/** VMON */
#define BSP_FEATURE_VMON_VDD2_GROUP                                           (0U)                                 ///< Group index for VDD2 voltage monitoring
#define BSP_FEATURE_VMON_AWOVDD_GROUP                                         (0U)                                 ///< Group index for AWOVDD voltage monitoring

/** RLIN3 */
#define BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK                                   (0x80003FUL)                         ///< Bitmask of supported RLIN3 channels
#define BSP_FEATURE_RLIN3_LIN_NUM_OF_CHANNELS                                 (7U)                                 ///< Number of LIN channels supported

/** DMAC */
#define BSP_FEATURE_DMAC_SWITCHABLE_REDUNDANT_OPERATION                       (1U)                                 ///< DMAC redundant operation supported
#define BSP_FEATURE_DMAC_DESCRIPTOR_START_BIT                                 (1U)                                 ///< DMAC descriptor start bit supported

/** DTSC */
#define BSP_FEATURE_DTSC_TRANSFER_RQ_OVERFLOW_FLAG                            (1U)                                 ///< Transfer request overflow flag supported

/** ECM */
#define BSP_FEATURE_ECM_ERROR_SOURCE_CHECK_VALID                              (351U)                               ///< Maximum ECM error source index supported

/** ICU */
#define BSP_FEATURE_FCLA5_IS_AVAILABLE                                        (1U)                                 ///< FCLA5 register availability (1 = available)

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU_RH850U2B10) */

#endif                                 /* BSP_FEATURE_H */
