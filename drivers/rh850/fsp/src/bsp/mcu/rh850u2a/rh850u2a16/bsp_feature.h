/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2AX
 * @defgroup BSP_MCU_RH850U2A16 BSP MCU U2A16
 * @brief This module provides the macros, data types, and definition constants required for U2A16 feature.
 **********************************************************************************************************************/

#ifndef BSP_FEATURE_H
#define BSP_FEATURE_H

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2A16
 * @defgroup BSP_FEATURE_U2A16 BSP U2A16 Specific Feature
 * @brief This file defines all feature-related macros for the U2A16 device, including module availability and
 * hardware capability constants.
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_PACKAGE_PINS_BGA516                                               (516U)                        ///< Package pin count: BGA516
#define BSP_PACKAGE_PINS_BGA373                                               (373U)                        ///< Package pin count: BGA373
#define BSP_PACKAGE_PINS_BGA292                                               (292U)                        ///< Package pin count: BGA292
#define BSP_PACKAGE_PINS_BGA156                                               (156U)                        ///< Package pin count: BGA156

/** Barrier Synchronization */
/** PE mask bit that is used in R_BSP_PESyncEnable */
#define BSP_BARRIER_SYNC_CPU0                                                 (1U << 0U)                    ///< PE mask bit for CPU0 in R_BSP_PESyncEnable
#define BSP_BARRIER_SYNC_CPU1                                                 (1U << 1U)                    ///< PE mask bit for CPU1 in R_BSP_PESyncEnable
#define BSP_BARRIER_SYNC_CPU2                                                 (1U << 2U)                    ///< PE mask bit for CPU2 in R_BSP_PESyncEnable
#define BSP_BARRIER_SYNC_CPU3                                                 (1U << 3U)                    ///< PE mask bit for CPU3 in R_BSP_PESyncEnable

/** PE value that is used in R_BSP_PESyncComplete */
#define BSP_CPU0                                                              (0U)                          ///< CPU identifier value for CPU0
#define BSP_CPU1                                                              (1U)                          ///< CPU identifier value for CPU1
#define BSP_CPU2                                                              (2U)                          ///< CPU identifier value for CPU2
#define BSP_CPU3                                                              (3U)                          ///< CPU identifier value for CPU3

/** BSP FEATURE */
/** U2A16 */
#define BSP_FEATURE_ETNB_IS_AVAILABLE                                         (0x01U)                       ///< ETNB feature availability
#define BSP_FEATURE_CXPI_IS_AVAILABLE                                         (0x01U)                       ///< CXPI peripheral availability
#define BSP_FEATURE_MMCA_IS_AVAILABLE                                         (0x01U)                       ///< MMC-A/eMMC availability
#define BSP_FEATURE_ENCA_IS_AVAILABLE                                         (0x01U)                       ///< ENCA feature availability
#define BSP_FEATURE_RHSIF_IS_AVAILABLE                                        (0x01U)                       ///< RHSIF availability
#define BSP_FEATURE_SCI3_IS_AVAILABLE                                         (0x01U)                       ///< SCI3 availability
#define BSP_FEATURE_SFMA_IS_AVAILABLE                                         (0x01U)                       ///< Octa/Serial Flash SFMA availability
#define BSP_FEATURE_ADCKA_IS_AVAILABLE                                        (0x01U)                       ///< ADCKA availability
#define BSP_FEATURE_FEINT_CPU0                                                (1U)                          ///< FEINT availability on CPU0
#define BSP_FEATURE_FEINT_CPU1                                                (1U)                          ///< FEINT availability on CPU1
#define BSP_FEATURE_FEINT_CPU2                                                (1U)                          ///< FEINT availability on CPU2
#define BSP_FEATURE_FEINT_CPU3                                                (1U)                          ///< FEINT availability on CPU3
#define BSP_FEATURE_CLMA_STOP_DETECTION_IS_AVAILABLE                          (0U)                          ///< Clock Stop Detection Mode support

/** MODULE RESET */
#define BSP_RESET_SOURCE_POWER_OFF_STANDBY_IS_AVAILABLE                       (0x00U)                       ///< Power-Off Standby reset source not available
#define BSP_RESET_ECM_SELECT_RESET_SOURCE_IS_AVAILABLE                        (0x01U)                       ///< ECM-selectable reset source available
#define BSP_RESET_MODULE_RSCFD                                                RSCFD                         ///< Module reset name: RSCFD
#define BSP_RESET_MODULE_FLXA                                                 FLXA                          ///< Module reset name: FLXA
#define BSP_RESET_MODULE_GTM                                                  GTM                           ///< Module reset name: GTM
#define BSP_RESET_MODULE_ETNB                                                 ETNB                          ///< Module reset name: ETNB
#define BSP_RESET_MODULE_RSENT                                                RSENT                         ///< Module reset name: RSENT
#define BSP_RESET_MODULE_MSPI                                                 MSPI                          ///< Module reset name: MSPI
#define BSP_RESET_MODULE_RLIN3                                                RLIN3                         ///< Module reset name: RLIN3
#define BSP_RESET_MODULE_ADCJ_ISO                                             ADCJ_ISO                      ///< Module reset name: ADCJ_ISO
#define BSP_RESET_MODULE_CXPI                                                 CXPI                          ///< Module reset name: CXPI
#define BSP_RESET_MODULE_MMCA                                                 MMCA                          ///< Module reset name: MMCA
#define BSP_RESET_MODULE_ENCA                                                 ENCA                          ///< Module reset name: ENCA
#define BSP_RESET_MODULE_PSI5                                                 PSI5                          ///< Module reset name: PSI5
#define BSP_RESET_MODULE_PSI5S                                                PSI5S                         ///< Module reset name: PSI5S
#define BSP_RESET_MODULE_PWMD                                                 PWMD                          ///< Module reset name: PWMD
#define BSP_RESET_MODULE_RHSIF                                                RHSIF                         ///< Module reset name: RHSIF
#define BSP_RESET_MODULE_RIIC                                                 RIIC                          ///< Module reset name: RIIC
#define BSP_RESET_MODULE_SCI3                                                 SCI3                          ///< Module reset name: SCI3
#define BSP_RESET_MODULE_SFMA                                                 SFMA                          ///< Module reset name: SFMA
#define BSP_RESET_MODULE_TAPA                                                 TAPA                          ///< Module reset name: TAPA
#define BSP_RESET_MODULE_TAUD                                                 TAUD                          ///< Module reset name: TAUD
#define BSP_RESET_MODULE_TAUJ_ISO                                             TAUJ_ISO                      ///< Module reset name: TAUJ_ISO
#define BSP_RESET_MODULE_TPBA                                                 TPBA                          ///< Module reset name: TPBA
#define BSP_RESET_MODULE_TSG3                                                 TSG3                          ///< Module reset name: TSG3
#define BSP_RESET_MODULE_OSTM                                                 OSTM                          ///< Module reset name: OSTM

/** STANDBY MODE */
#define BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_IS_AVAILABLE                        (0x01U)                       ///< DeepStop mode available
#define BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_NUM_OF_WAKEUP_SOURCE                (0x05U)                       ///< Number of DeepStop wakeup sources
#define BSP_FEATURE_CHIP_STANDBY_IOHOLD_REG_NAME                              (0x02U)                       ///< IOHOLD register index

/** OPBT */
#define BSP_FEATURE_OPBT_BVA1_AREA_IS_AVAILABLE                               (0x01U)                       ///< BVA1 Area is available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_1_AVAILABLE                        (0x01U)                       ///< OPBT Cluster 1 switching is available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_2_AVAILABLE                        (0x00U)                       ///< OPBT Cluster 2 switching is not available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_3_AVAILABLE                        (0x00U)                       ///< OPBT Cluster 3 switching is not available

/** IIC */
#define BSP_FEATURE_IIC_VALID_CHANNEL_MASK                                    (0x03U)                       ///< Valid IIC channel mask

/** GUARD */
#define BSP_FEATURE_PEGCAP_M_IS_AVAILABLE                                     (0x01U)                       ///< PEGCAP_M availability
#define BSP_FEATURE_PE1_GUARD_IS_AVAILABLE                                    (1U)                          ///< PE1 guard availability
#define BSP_FEATURE_PE2_GUARD_IS_AVAILABLE                                    (1U)                          ///< PE2 guard availability
#define BSP_FEATURE_PE3_GUARD_IS_AVAILABLE                                    (1U)                          ///< PE3 guard availability
#define BSP_FEATURE_CRAM0_GUARD_IS_AVAILABLE                                  (1U)                          ///< CRAM0 guard availability
#define BSP_FEATURE_CRAM1_GUARD_IS_AVAILABLE                                  (1U)                          ///< CRAM1 guard availability
#define BSP_FEATURE_CRAM2_GUARD_IS_AVAILABLE                                  (1U)                          ///< CRAM2 guard availability
#define BSP_FEATURE_CRAM3_GUARD_IS_AVAILABLE                                  (1U)                          ///< CRAM3 guard availability
#define BSP_FEATURE_CRAM_GUARD_STATUS_MAX                                     (8U)                          ///< Maximum CRAM guard status index
#define BSP_FEATURE_PE_GUARD_STATUS_MAX                                       (5U)                          ///< Maximum PE guard status index
#define BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE                                   (1U)                          ///< IPIR guard availability
#define BSP_FEATURE_BARR_GUARD_IS_AVAILABLE                                   (1U)                          ///< Barrier guard availability
#define BSP_FEATURE_TPTMn_GUARD_MAX_CHANNEL                                   (3U)                          ///< Maximum TPTMn guard channel
#define BSP_FEATURE_INTC2_GUARD_MAX_CHANNEL                                   (767U)                        ///< Maximum INTC2 guard channel index
#define BSP_FEATURE_INTC2_GUARD_MIN_CHANNEL                                   (32U)                         ///< Minimum INTC2 guard channel index
#define BSP_FEATURE_DTS_GUARD_MAX_CHANNEL                                     (128U)                        ///< Maximum DTS guard channel
#define BSP_FEATURE_DMAC_GUARD_MAX_CHANNEL                                    (16U)                         ///< Maximum DMAC guard channel

/** QOS */
#define BSP_FEATURE_QOS_IS_AVAILABLE                                          (0U)                          ///< QoS feature is not available on this device

/** DMAC */
#define BSP_FEATURE_DMAC_MAX_CHANNEL                                          (16U)                         ///< Maximum DMAC channels
#define BSP_FEATURE_DMAC_MAX_UNIT                                             (2U)                          ///< Maximum DMAC units

/** DTSC */
#define BSP_FEATURE_DTSC_MAX_CHANNEL                                          (128U)                        ///< Maximum DTSC channels

/** ENCA */
#define BSP_FEATURE_ENCA_MAX_CHANNEL                                          (2U)                          ///< Maximum ENCA channels

/** PWM */
#define BSP_FEATURE_PWM_DIAG_NUM_OF_CHANNEL                                   (96U)                         ///< Number of PWM diagnostic channels

/** TSG3 */
#define BSP_FEATURE_TSG3_MAX_UNIT                                             (2U)                          ///< Maximum TSG3 units
#define BSP_FEATURE_TSG3_COMPARE_MATCH_INTERRUPT_SKIPPING_FUNCTION_SUPPORT    (0U)                          ///< Compare-match interrupt skipping not supported
#define BSP_FEATURE_TSG3_OUTPUT_INACTIVE_FUNCTION_SUPPORT                     (0U)                          ///< Output inactive function not supported
#define BSP_FEATURE_TSG3_OUTPUT_CONTROL_BY_INTPUT_TERMINAL_SUPPORT            (0U)                          ///< Output control by input terminal not supported
#define BSP_FEATURE_TSG3_EMU3S_IS_AVAILABLE                                   (0U)                          ///< EMU3S availability for TSG3

/** MSPI */
#if (BSP_PACKAGE_PINS_BGA516 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_MSPI_MAX_UNIT                                            (10U)                         ///< Number of MSPI units (BGA516)
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_0                           (8U)                          ///< Chip selects on MSPI unit 0
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_1                           (8U)                          ///< Chip selects on MSPI unit 1
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_2                           (8U)                          ///< Chip selects on MSPI unit 2
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_3                           (8U)                          ///< Chip selects on MSPI unit 3
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_4                           (8U)                          ///< Chip selects on MSPI unit 4
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_5                           (4U)                          ///< Chip selects on MSPI unit 5
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_6                           (4U)                          ///< Chip selects on MSPI unit 6
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_7                           (4U)                          ///< Chip selects on MSPI unit 7
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_8                           (4U)                          ///< Chip selects on MSPI unit 8
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_9                           (4U)                          ///< Chip selects on MSPI unit 9
 #define BSP_FEATURE_ATU_GTM_SEL                                              (0U)                          ///< ATU GTM trigger selection bit
#elif (BSP_PACKAGE_PINS_BGA373 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_MSPI_MAX_UNIT                                            (9U)                          ///< Number of MSPI units (BGA373)
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_0                           (8U)                          ///< Chip selects on MSPI unit 0
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_1                           (8U)                          ///< Chip selects on MSPI unit 1
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_2                           (8U)                          ///< Chip selects on MSPI unit 2
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_3                           (8U)                          ///< Chip selects on MSPI unit 3
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_4                           (8U)                          ///< Chip selects on MSPI unit 4
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_5                           (4U)                          ///< Chip selects on MSPI unit 5
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_6                           (4U)                          ///< Chip selects on MSPI unit 6
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_7                           (4U)                          ///< Chip selects on MSPI unit 7
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_8                           (4U)                          ///< Chip selects on MSPI unit 8
 #define BSP_FEATURE_ATU_GTM_SEL                                              (0U)                          ///< ATU GTM trigger selection bit
#elif (BSP_PACKAGE_PINS_BGA292 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_MSPI_MAX_UNIT                                            (6U)                          ///< Number of MSPI units (BGA292)
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_0                           (8U)                          ///< Chip selects on MSPI unit 0
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_1                           (8U)                          ///< Chip selects on MSPI unit 1
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_2                           (8U)                          ///< Chip selects on MSPI unit 2
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_3                           (8U)                          ///< Chip selects on MSPI unit 3
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_4                           (8U)                          ///< Chip selects on MSPI unit 4
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_5                           (4U)                          ///< Chip selects on MSPI unit 5
 #define BSP_FEATURE_ATU_GTM_SEL                                              (0U)                          ///< ATU GTM trigger selection bit
#endif

#define BSP_FEATURE_MSPI_ALT_DMA_UNIT                                         (0xCCU)                       ///< Bitmask for MSPI units supporting ALT DMA
#define BSP_FEATURE_MSPI_ALT_DTS_UNIT                                         (0xF0U)                       ///< Bitmask for MSPI units supporting ALT DTS
#define BSP_FEATURE_MSPI_ALT_DMA_MAX_UNIT                                     (2U)                          ///< Max units supporting ALT DMA
#define BSP_FEATURE_MSPI_ALT_DTS_MAX_UNIT                                     (4U)                          ///< Max units supporting ALT DTS
#define BSP_FEATURE_MSPI_ALT_GAP_CHANNEL                                      (4U)                          ///< Channel gap per unit for ALT
#define BSP_FEATURE_MSPI_ALT_GAP_UNIT                                         (4U)                          ///< Unit gap between ALT units
#define BSP_FEATURE_MSPI_ALT_PAIR_CHANNEL                                     (2U)                          ///< Channel pair grouping for ALT
#define BSP_FEATURE_MSPI_ALT_MAX_CHANNEL                                      (7U)                          ///< Maximum ALT channel index
#define BSP_FEATURE_MSPI_ALT_DMA_MAX_BIT                                      (9U)                          ///< Maximum ALT DMA bit index
#define BSP_FEATURE_MSPI_ALT_DTS_MAX_BIT                                      (13U)                         ///< Maximum ALT DTS bit index

/** OSTM */
#define BSP_FEATURE_OSTM_VALID_CHANNEL_MASK                                   (0x3FFU)                      ///< Valid OSTM channel mask

/** TAUD */
#define BSP_FEATURE_TAUD_MAX_UNIT                                             (3U)                          ///< Maximum TAUD units

/** CANFD */
#define BSP_FEATURE_CANFD_NUM_UNIT                                            (2UL)                         ///< Number of CANFD units
#define BSP_FEATURE_CANFD_NUM_CHANNELS                                        (16UL)                        ///< Number of CAN FD channels
#define BSP_FEATURE_CANFD_NUM_TXQUEUE_CHANNEL                                 (4UL)                         ///< Number of CAN FD TXQUEUE channels
#define BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL                                   (3UL)                         ///< Number of CAN FD CFIFO channels
#define BSP_FEATURE_CANFD_NUM_RXFIFO                                          (8UL)                         ///< Number of CAN FD RX FIFOs
#define BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT                                  (24UL)                        ///< Total CAN FD CFIFOs
#define BSP_FEATURE_CANFD_TXMB_SUPPORTED_SLOT                                 (0xFFFFFFFFFFFFFFFFULL)       ///< Supported CAN FD TXMB slots (0–63)
#define BSP_FEATURE_CANFD_TXMB_MAX_NUM                                        (64UL)                        ///< Maximum CAN FD TXMB count
#define BSP_FEATURE_CANFD_CLKC_SRC                                            (FSP_PRIV_CLOCK_CLK_HSB)      ///< CAN FD CLKC source
#define BSP_FEATURE_CANFD_NOT_CHECK_TXMB_RANGE                                (1)                           ///< Skip TXMB range check
#define BSP_FEATURE_CANFD_AFLMRAM_MAX_PAGE                                    (869UL)                       ///< Number of AFL MRAM page
#define BSP_FEATURE_CANFD_AFLMRAM_PER_CHANNEL                                 (27808UL)                     ///< Number of AFL MRAM area size

/** RIIC */
#define BSP_FEATURE_IIC_HAS_SYS_CLK                                           (1U)                          ///< IIC supports system clock source
#define BSP_FEATURE_IIC_VALID_CHANNEL_MASK                                    (0x03U)                       ///< Valid IIC channel mask

/** MSPI */
#define BSP_FEATURE_MSPI_RAM_WORDS_MAX                                        (128U)                        ///< MSPI RAM size

/** RSENT */
#define BSP_FEATURE_RSENT_SENT_NUM_OF_UNIT                                    (8U)                          ///< Number of RSENT units

/** TAUD3P */
#define BSP_FEATURE_TAUD_THREE_PHASE_NUMBER_OF_UNIT                           (2U)                          ///< Number of TAUD three-phase units

/** PIC — ADC / PSI5S / GTM */
#define BSP_FEATURE_PIC_ADC_NUMBER_UNIT                                       (2U)                          ///< Number of PIC ADC units
#define BSP_FEATURE_PIC_ADC_NUMBER_SG                                         (5U)                          ///< Number of scan groups per PIC ADC
#define BSP_FEATURE_PIC_PSI5S_NUMBER_UNIT                                     (2U)                          ///< Number of PSI5S units
#define BSP_FEATURE_PIC_PSI5SEN_NUMBER                                        (4U)                          ///< Number of PSI5SEN instances
#define BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP                                  (4U)                          ///< Number of GTMINEN groups

/** FR */
#define BSP_FEATURE_FLXA_MAX_UNIT                                             (2U)                          ///< Maximum FLXA units
#define BSP_FEATURE_FLXA_IS_AVAILABLE                                         (1U)                          ///< FLXA availability

/** PIC — detailed feature set */
#define BSP_FEATURE_PIC_TAUD_INT_SIGNAL_OUT_NUMBER                            (1U)                          ///< TAUD interrupt signal outputs from PIC
#define BSP_FEATURE_PIC_TAUD_TRIANGLE_WAVE_NUMBER                             (2U)                          ///< TAUD triangle wave outputs
#define BSP_FEATURE_PIC_TAUD_PWM_OUTPUT_NUMBER                                (2U)                          ///< TAUD PWM output count
#define BSP_FEATURE_PIC_TAUD_DELAY_PULSE_NUMBER                               (2U)                          ///< TAUD delay pulse count
#define BSP_FEATURE_PIC_ENCA_FEATURE_NUMBER_UNIT                              (2U)                          ///< Number of ENCA-capable units
#define BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT                                    (2U)                          ///< Number of TAUD units in PIC20
#define BSP_FEATURE_PIC_ADCJ_TRIG_SEL                                         (1U)                          ///< ADCJ trigger selection support
#define BSP_FEATURE_PIC_SWITCH_TSG_OUT                                        (0U)                          ///< TSG output switching unavailable
#define BSP_FEATURE_PIC_EMU_FEATURE                                           (0U)                          ///< EMU feature unavailable
#define BSP_FEATURE_PIC_TSG_START_TRIG                                        (0U)                          ///< TSG start trigger unavailable
#define BSP_FEATURE_PIC_3PHASE_ENCODER_TSG31                                  (1U)                          ///< 3-phase encoder via TSG31 available
#define BSP_FEATURE_PIC_2PHASE_SEL_INTENCA1_TO_TSG3                           (1U)                          ///< 2-phase ENCA1 input selectable for TSG3
#define BSP_FEATURE_PIC_ENC_CAPT_TRIG_ENCA1                                   (1U)                          ///< ENCA1 capture trigger supported
#define BSP_FEATURE_PIC_INT_SIG_OUT_TAUD1                                     (0U)                          ///< TAUD1 interrupt output unavailable
#define BSP_FEATURE_PIC_INT_SIG_OUT_TAUD2                                     (0U)                          ///< TAUD2 interrupt output unavailable
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT12                         (0U)                          ///< Pulse width measure via DT12 unsupported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT00                         (1U)                          ///< Pulse width measure via DT00 supported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT10                         (1U)                          ///< Pulse width measure via DT10 supported
#define BSP_FEATURE_PIC_ENCA_INPUT_SEL                                        (0U)                          ///< ENCA input selection not supported
#define BSP_FEATURE_PIC_FCMP_FEATURE                                          (0U)                          ///< FCMP feature unavailable
#define BSP_FEATURE_PIC_HRPWM_FEATURE                                         (0U)                          ///< HRPWM feature unavailable
#define BSP_FEATURE_PIC_TIMER_INPUT_MONITOR                                   (1U)                          ///< Timer input monitor available
#define BSP_FEATURE_PIC_TAUD_INPUT_SEL                                        (0U)                          ///< TAUD input selection unavailable
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL                                         (1U)                          ///< ENCA trigger selection supported
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL                                    (1U)                          ///< ENCA encoder input selection supported
#define BSP_FEATURE_PIC_RDC_FEATURE                                           (0U)                          ///< RDC feature unavailable
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE                                      (0U)                          ///< CAN/GTM IO sharing unsupported
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL                                         (0U)                          ///< MSPI trigger selection unavailable
#define BSP_FEATURE_PIC_GTM_DTM_INPUT_SEL                                     (0U)                          ///< GTM DTM input selection unavailable
#define BSP_FEATURE_PIC_TSG3_INPUT_GTM                                        (0U)                          ///< TSG3 GTM input unavailable
#define BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC13                            (0U)                          ///< TSG sync via PIC13 unsupported
#define BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC1_SELB                        (1U)                          ///< TSG sync via PIC1 SELB supported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD2                                  (0U)                          ///< TIMER IN SEL B to TAUD2 (U2Cx) unsupported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD_TAUJ                              (1U)                          ///< TIMER IN SEL B to TAUD/TAUJ (U2Bx) supported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B                                        (1U)                          ///< TIMER IN SEL B available
#define BSP_FEATURE_PIC_TIMER_OUT_MON_PIC13                                   (0U)                          ///< Timer out monitor via PIC13 unavailable
#define BSP_FEATURE_PIC_TIMER_OUT_MON_PIC1_SELB                               (1U)                          ///< Timer out monitor via PIC1 SELB available
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC22                          (0U)                          ///< ENCATSEL via PIC22 unavailable
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC24                          (0U)                          ///< ENCATCFG via PIC24 unavailable
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC2_SELB                      (0U)                          ///< ENCATCFG via PIC2 SELB unavailable
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC21                          (1U)                          ///< ENCATSEL via PIC21 available
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC22                          (1U)                          ///< ENCATCFG via PIC22 available
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC24                              (0U)                          ///< PSI5S trigger via GTM PIC24 unsupported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC2_SELB                          (0U)                          ///< PSI5S trigger via GTM PIC2 SELB unsupported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC22                              (1U)                          ///< PSI5S trigger via GTM PIC22 supported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL                                        (1U)                          ///< PSI5S trigger selection supported
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC24                                      (0U)                          ///< GTM TIO input via PIC24 supported
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC2_SELB                                  (0U)                          ///< GTM TIO input via PIC2 SELB unavailable
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC22                                      (0U)                          ///< GTM TIO input via PIC22 unavailable
#define BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL                                     (0U)                          ///< GTM TIO input selection unsupported
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC24                                      (0U)                          ///< GTM TIM input via PIC24 unavailable
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC2_SELB                                  (0U)                          ///< GTM TIM input via PIC2 SELB unavailable
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC22                                      (1U)                          ///< GTM TIM input via PIC22 available
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC24                                  (0U)                          ///< ENCA input via PIC24 unavailable
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC22                                  (1U)                          ///< ENCA input via PIC22 available
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC24                                    (0U)                          ///< Hi-Z via GTM PIC24 unavailable
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC2_SELB                                (0U)                          ///< Hi-Z via GTM PIC2 SELB unavailable
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC20                                    (1U)                          ///< Hi-Z via GTM PIC20 supported
#define BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN                                       (1U)                          ///< HIZCEN control supported
#define BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST                                   (1U)                          ///< Single SST mode on PIC1 supported
#define BSP_FEATURE_PIC_FEATURE_U2AX                                          (1U)                          ///< PIC U2Ax feature set supported

/** GTM */
#define BSP_FEATURE_GTM_ARU_NUMBER_INSTANCE                                   (1U)                          ///< Number of GTM ARU instances
#define BSP_FEATURE_GTM_BRC_NUMBER_INSTANCE                                   (0U)                          ///< Number of GTM BRC instances
#define BSP_FEATURE_GTM_PSM_NUMBER_INSTANCE                                   (0U)                          ///< Number of GTM PSM instances
#define BSP_FEATURE_GTM_CMU_NUMBER_INSTANCE                                   (1U)                          ///< Number of GTM CMU instances
#define BSP_FEATURE_GTM_CCM_NUMBER_INSTANCE                                   (4U)                          ///< Number of GTM CCM instances
#define BSP_FEATURE_GTM_TBU_NUMBER_INSTANCE                                   (1U)                          ///< Number of GTM TBU instances
#define BSP_FEATURE_GTM_TIM_NUMBER_INSTANCE                                   (4U)                          ///< Number of GTM TIM instances
#define BSP_FEATURE_GTM_TOM_NUMBER_INSTANCE                                   (0U)                          ///< Number of GTM TOM instances
#define BSP_FEATURE_GTM_ATOM_NUMBER_INSTANCE                                  (4U)                          ///< Number of GTM ATOM instances
#define BSP_FEATURE_GTM_DTM_NUMBER_INSTANCE                                   (8U)                          ///< Number of GTM DTM instances
#define BSP_FEATURE_GTM_MCS_NUMBER_INSTANCE                                   (4U)                          ///< Number of GTM MCS instances
#define BSP_FEATURE_GTM_MCFG_NUMBER_INSTANCE                                  (1U)                          ///< Number of GTM MCFG instances
#define BSP_FEATURE_GTM_MAP_NUMBER_INSTANCE                                   (0U)                          ///< Number of GTM MAP instances
#define BSP_FEATURE_GTM_DPLL_NUMBER_INSTANCE                                  (0U)                          ///< Number of GTM DPLL instances
#define BSP_FEATURE_GTM_SPE_NUMBER_INSTANCE                                   (0U)                          ///< Number of GTM SPE instances
#define BSP_FEATURE_GTM_ICM_NUMBER_INSTANCE                                   (1U)                          ///< Number of GTM ICM instances
#define BSP_FEATURE_GTM_CMP_NUMBER_INSTANCE                                   (1U)                          ///< Number of GTM CMP instances
#define BSP_FEATURE_GTM_MON_NUMBER_INSTANCE                                   (1U)                          ///< Number of GTM MON instances
#define BSP_FEATURE_GTM_TIO_NUMBER_INSTANCE                                   (0U)                          ///< Number of GTM TIO instances
#define BSP_FEATURE_GTM_MCS_NUMBER_CHANNEL                                    (8U)                          ///< GTM MCS channel count per instance
#define BSP_FEATURE_GTM_MCS_NUMBER_TRIGGER                                    (24U)                         ///< GTM MCS trigger sources

/** CLMA */
#define BSP_FEATURE_CLMA_MAX_CHANNEL                                          (10U)                         ///< Maximum CLMA channels
#define BSP_FEATURE_CLMA_CHANNEL_CONFIG_U2A                                   (1U)                          ///< Clock monitor channel config

/** VMON */
#define BSP_FEATURE_VMON_VDD2_AVAILABLE                                       (0U)                          ///< VDD2 monitoring group
#define BSP_FEATURE_VMON_AWOVDD_AVAILABLE                                     (1U)                          ///< AWOVDD monitoring group
#define BSP_FEATURE_VMON_FC_ENABLE                                            (0xFFU)                       ///< Bit positions for enabling VMON functionality
#define BSP_FEATURE_VMON_DMASK_ENABLE                                         (0xFFU)                       ///< Enable/disable VMONDMASK functionality
#define BSP_FEATURE_VMON_OUT_DMASK_ENABLE                                     (0xF0U)                       ///< Enable/disable VMON_OUT_DMASK functionality
#define BSP_FEATURE_VMON_RESET_DMASK_ENABLE                                   (0x0FU)                       ///< Enable/disable VMON_RESET_DMASK functionality

/** RLIN3 */
#if (BSP_PACKAGE_PINS_BGA292 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK                                  (0xFFFUL)                     ///< RLIN3 LIN channel bitmask (12 channels)
 #define BSP_FEATURE_RLIN3_LIN_NUM_OF_CHANNELS                                (12U)                         ///< Number of RLIN3 LIN channels (BGA292)
#else
 #define BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK                                  (0xFFFFFFUL)                  ///< RLIN3 LIN channel bitmask (24 channels)
 #define BSP_FEATURE_RLIN3_LIN_NUM_OF_CHANNELS                                (24U)                         ///< Number of RLIN3 LIN channels (others)
#endif

/** IOPORT */
#define BSP_FEATURE_IOPORT_NF_RLIN30                                          (1U)                          ///< IOPORT Noise filter RLIN30 signal
#define BSP_FEATURE_IOPORT_NF_RLIN31                                          (1U)                          ///< IOPORT Noise filter RLIN31 signal
#define BSP_FEATURE_IOPORT_NF_RSCAN                                           (1U)                          ///< IOPORT Noise filter RSCAN signal
#define BSP_FEATURE_IOPORT_NF_FLXA0                                           (1U)                          ///< IOPORT Noise filter FLXA0 signal
#define BSP_FEATURE_IOPORT_NF_FLXA1                                           (1U)                          ///< IOPORT Noise filter FLXA1 signal
#define BSP_FEATURE_IOPORT_NF_RSENT0                                          (1U)                          ///< IOPORT Noise filter RSENT0 signal
#define BSP_FEATURE_IOPORT_NF_RSENT1                                          (1U)                          ///< IOPORT Noise filter RSENT1 signal
#define BSP_FEATURE_IOPORT_NF_RSENT2                                          (1U)                          ///< IOPORT Noise filter RSENT2 signal
#define BSP_FEATURE_IOPORT_NF_RSENT3                                          (1U)                          ///< IOPORT Noise filter RSENT3 signal
#define BSP_FEATURE_IOPORT_NF_RSENT4                                          (1U)                          ///< IOPORT Noise filter RSENT4 signal
#define BSP_FEATURE_IOPORT_NF_RSENT5                                          (1U)                          ///< IOPORT Noise filter RSENT5 signal
#define BSP_FEATURE_IOPORT_NF_RSENT6                                          (1U)                          ///< IOPORT Noise filter RSENT6 signal
#define BSP_FEATURE_IOPORT_NF_RSENT7                                          (1U)                          ///< IOPORT Noise filter RSENT7 signal
#define BSP_FEATURE_IOPORT_NF_RSENT8                                          (0U)                          ///< IOPORT Noise filter RSENT8 signal
#define BSP_FEATURE_IOPORT_NF_RSENT9                                          (0U)                          ///< IOPORT Noise filter RSENT9 signal
#define BSP_FEATURE_IOPORT_NF_RSENT10                                         (0U)                          ///< IOPORT Noise filter RSENT10 signal
#define BSP_FEATURE_IOPORT_NF_RSENT11                                         (0U)                          ///< IOPORT Noise filter RSENT11 signal
#define BSP_FEATURE_IOPORT_NF_RSENT12                                         (0U)                          ///< IOPORT Noise filter RSENT12 signal
#define BSP_FEATURE_IOPORT_NF_RSENT13                                         (0U)                          ///< IOPORT Noise filter RSENT13 signal
#define BSP_FEATURE_IOPORT_NF_RSENT14                                         (0U)                          ///< IOPORT Noise filter RSENT14 signal
#define BSP_FEATURE_IOPORT_NF_RSENT15                                         (0U)                          ///< IOPORT Noise filter RSENT15 signal
#define BSP_FEATURE_IOPORT_NF_RSENT16                                         (0U)                          ///< IOPORT Noise filter RSENT16 signal
#define BSP_FEATURE_IOPORT_NF_RSENT17                                         (0U)                          ///< IOPORT Noise filter RSENT17 signal
#define BSP_FEATURE_IOPORT_NF_RSENT18                                         (0U)                          ///< IOPORT Noise filter RSENT18 signal
#define BSP_FEATURE_IOPORT_NF_RSENT19                                         (0U)                          ///< IOPORT Noise filter RSENT19 signal
#define BSP_FEATURE_IOPORT_NF_RSENT20                                         (0U)                          ///< IOPORT Noise filter RSENT20 signal
#define BSP_FEATURE_IOPORT_NF_RSENT21                                         (0U)                          ///< IOPORT Noise filter RSENT21 signal
#define BSP_FEATURE_IOPORT_NF_RSENT22                                         (0U)                          ///< IOPORT Noise filter RSENT22 signal
#define BSP_FEATURE_IOPORT_NF_RSENT23                                         (0U)                          ///< IOPORT Noise filter RSENT23 signal
#define BSP_FEATURE_IOPORT_NF_RSENT24                                         (0U)                          ///< IOPORT Noise filter RSENT24 signal
#define BSP_FEATURE_IOPORT_NF_RSENT25                                         (0U)                          ///< IOPORT Noise filter RSENT25 signal
#define BSP_FEATURE_IOPORT_NF_RSENT26                                         (0U)                          ///< IOPORT Noise filter RSENT26 signal
#define BSP_FEATURE_IOPORT_NF_RSENT27                                         (0U)                          ///< IOPORT Noise filter RSENT27 signal
#define BSP_FEATURE_IOPORT_NF_RSENT28                                         (0U)                          ///< IOPORT Noise filter RSENT28 signal
#define BSP_FEATURE_IOPORT_NF_RSENT29                                         (0U)                          ///< IOPORT Noise filter RSENT29 signal
#define BSP_FEATURE_IOPORT_NF_PSI50                                           (1U)                          ///< IOPORT Noise filter PSI50 signal
#define BSP_FEATURE_IOPORT_NF_PSI51                                           (1U)                          ///< IOPORT Noise filter PSI51 signal
#define BSP_FEATURE_IOPORT_NF_PSI52                                           (1U)                          ///< IOPORT Noise filter PSI52 signal
#define BSP_FEATURE_IOPORT_NF_PSI53                                           (1U)                          ///< IOPORT Noise filter PSI53 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA0                                       (1U)                          ///< IOPORT Noise filter PIC_TAPA0 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA1                                       (1U)                          ///< IOPORT Noise filter PIC_TAPA1 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA2                                       (0U)                          ///< IOPORT Noise filter PIC_TAPA2 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA3                                       (0U)                          ///< IOPORT Noise filter PIC_TAPA3 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA4                                       (0U)                          ///< IOPORT Noise filter PIC_TAPA4 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA5                                       (0U)                          ///< IOPORT Noise filter PIC_TAPA5 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ0                                           (1U)                          ///< IOPORT Noise filter ADCJ0 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ1                                           (1U)                          ///< IOPORT Noise filter ADCJ1 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ2                                           (1U)                          ///< IOPORT Noise filter ADCJ2 signal
#define BSP_FEATURE_IOPORT_NF_ADCK0                                           (0U)                          ///< IOPORT Noise filter ADCK0 signal
#define BSP_FEATURE_IOPORT_NF_ADCK1                                           (0U)                          ///< IOPORT Noise filter ADCK1 signal
#define BSP_FEATURE_IOPORT_NF_ADCK2                                           (0U)                          ///< IOPORT Noise filter ADCK2 signal
#define BSP_FEATURE_IOPORT_NF_ADCK3                                           (0U)                          ///< IOPORT Noise filter ADCK3 signal
#define BSP_FEATURE_IOPORT_NF_ADCKA                                           (0U)                          ///< IOPORT Noise filter ADCKA signal
#define BSP_FEATURE_IOPORT_NF_RHSB0                                           (0U)                          ///< IOPORT Noise filter RHSB0 signal
#define BSP_FEATURE_IOPORT_NF_RHSB1                                           (0U)                          ///< IOPORT Noise filter RHSB1 signal
#define BSP_FEATURE_IOPORT_NF_RHSB2                                           (0U)                          ///< IOPORT Noise filter RHSB2 signal
#define BSP_FEATURE_IOPORT_NF_RHSB3                                           (0U)                          ///< IOPORT Noise filter RHSB3 signal
#define BSP_FEATURE_IOPORT_NF_ENCA0                                           (1U)                          ///< IOPORT Noise filter ENCA0 signal
#define BSP_FEATURE_IOPORT_NF_ENCA1                                           (1U)                          ///< IOPORT Noise filter ENCA1 signal
#define BSP_FEATURE_IOPORT_NF_TAUD0                                           (1U)                          ///< IOPORT Noise filter TAUD0 signal
#define BSP_FEATURE_IOPORT_NF_TAUD1                                           (1U)                          ///< IOPORT Noise filter TAUD1 signal
#define BSP_FEATURE_IOPORT_NF_TAUD2                                           (1U)                          ///< IOPORT Noise filter TAUD2 signal
#define BSP_FEATURE_IOPORT_NF_TAUD3                                           (0U)                          ///< IOPORT Noise filter TAUD3 signal
#define BSP_FEATURE_IOPORT_NF_TSG30                                           (1U)                          ///< IOPORT Noise filter TSG30 signal
#define BSP_FEATURE_IOPORT_NF_TSG31                                           (1U)                          ///< IOPORT Noise filter TSG31 signal
#define BSP_FEATURE_IOPORT_NF_TSG32                                           (0U)                          ///< IOPORT Noise filter TSG32 signal
#define BSP_FEATURE_IOPORT_NF_IRQ0                                            (0U)                          ///< IOPORT Noise filter IRQ0 signal
#define BSP_FEATURE_IOPORT_NF_IRQ1                                            (0U)                          ///< IOPORT Noise filter IRQ1 signal
#define BSP_FEATURE_IOPORT_NF_IRQ2                                            (0U)                          ///< IOPORT Noise filter IRQ2 signal
#define BSP_FEATURE_IOPORT_NF_IRQ3                                            (0U)                          ///< IOPORT Noise filter IRQ3 signal
#define BSP_FEATURE_IOPORT_NF_IRQ4                                            (0U)                          ///< IOPORT Noise filter IRQ4 signal
#define BSP_FEATURE_IOPORT_NF_IRQ5                                            (0U)                          ///< IOPORT Noise filter IRQ5 signal
#define BSP_FEATURE_IOPORT_NF_IRQ6                                            (0U)                          ///< IOPORT Noise filter IRQ6 signal
#define BSP_FEATURE_IOPORT_NF_IRQ7                                            (0U)                          ///< IOPORT Noise filter IRQ7 signal
#define BSP_FEATURE_IOPORT_NF_IRQ8                                            (0U)                          ///< IOPORT Noise filter IRQ8 signal
#define BSP_FEATURE_IOPORT_NF_IRQ9                                            (0U)                          ///< IOPORT Noise filter IRQ9 signal
#define BSP_FEATURE_IOPORT_NF_IRQ10                                           (0U)                          ///< IOPORT Noise filter IRQ10 signal
#define BSP_FEATURE_IOPORT_NF_IRQ11                                           (0U)                          ///< IOPORT Noise filter IRQ11 signal
#define BSP_FEATURE_IOPORT_NF_IRQ12                                           (0U)                          ///< IOPORT Noise filter IRQ12 signal
#define BSP_FEATURE_IOPORT_NF_IRQ13                                           (0U)                          ///< IOPORT Noise filter IRQ13 signal
#define BSP_FEATURE_IOPORT_NF_IRQ14                                           (0U)                          ///< IOPORT Noise filter IRQ14 signal
#define BSP_FEATURE_IOPORT_NF_IRQ15                                           (0U)                          ///< IOPORT Noise filter IRQ15 signal
#define BSP_FEATURE_IOPORT_NF_IRQ16                                           (0U)                          ///< IOPORT Noise filter IRQ16 signal
#define BSP_FEATURE_IOPORT_NF_IRQ17                                           (0U)                          ///< IOPORT Noise filter IRQ17 signal
#define BSP_FEATURE_IOPORT_NF_IRQ18                                           (0U)                          ///< IOPORT Noise filter IRQ18 signal
#define BSP_FEATURE_IOPORT_NF_IRQ19                                           (0U)                          ///< IOPORT Noise filter IRQ19 signal
#define BSP_FEATURE_IOPORT_NF_IRQ20                                           (0U)                          ///< IOPORT Noise filter IRQ20 signal
#define BSP_FEATURE_IOPORT_NF_IRQ21                                           (0U)                          ///< IOPORT Noise filter IRQ21 signal
#define BSP_FEATURE_IOPORT_NF_IRQ22                                           (0U)                          ///< IOPORT Noise filter IRQ22 signal
#define BSP_FEATURE_IOPORT_NF_IRQ23                                           (0U)                          ///< IOPORT Noise filter IRQ23 signal
#define BSP_FEATURE_IOPORT_NF_IRQ24                                           (0U)                          ///< IOPORT Noise filter IRQ24 signal
#define BSP_FEATURE_IOPORT_NF_IRQ25                                           (0U)                          ///< IOPORT Noise filter IRQ25 signal
#define BSP_FEATURE_IOPORT_NF_IRQ26                                           (0U)                          ///< IOPORT Noise filter IRQ26 signal
#define BSP_FEATURE_IOPORT_NF_IRQ27                                           (0U)                          ///< IOPORT Noise filter IRQ27 signal
#define BSP_FEATURE_IOPORT_NF_IRQ28                                           (0U)                          ///< IOPORT Noise filter IRQ28 signal
#define BSP_FEATURE_IOPORT_NF_IRQ29                                           (0U)                          ///< IOPORT Noise filter IRQ29 signal
#define BSP_FEATURE_IOPORT_NF_IRQ30                                           (0U)                          ///< IOPORT Noise filter IRQ30 signal
#define BSP_FEATURE_IOPORT_NF_IRQ31                                           (0U)                          ///< IOPORT Noise filter IRQ31 signal
#define BSP_FEATURE_IOPORT_NF_IRQ32                                           (0U)                          ///< IOPORT Noise filter IRQ32 signal
#define BSP_FEATURE_IOPORT_NF_IRQ33                                           (0U)                          ///< IOPORT Noise filter IRQ33 signal
#define BSP_FEATURE_IOPORT_NF_IRQ34                                           (0U)                          ///< IOPORT Noise filter IRQ34 signal
#define BSP_FEATURE_IOPORT_NF_IRQ35                                           (0U)                          ///< IOPORT Noise filter IRQ35 signal
#define BSP_FEATURE_IOPORT_NF_IRQ36                                           (0U)                          ///< IOPORT Noise filter IRQ36 signal
#define BSP_FEATURE_IOPORT_NF_IRQ37                                           (0U)                          ///< IOPORT Noise filter IRQ37 signal
#define BSP_FEATURE_IOPORT_NF_IRQ38                                           (0U)                          ///< IOPORT Noise filter IRQ38 signal
#define BSP_FEATURE_IOPORT_NF_IRQ39                                           (0U)                          ///< IOPORT Noise filter IRQ39 signal
#define BSP_FEATURE_IOPORT_NF_IRQ_NMI                                         (0U)                          ///< IOPORT Noise filter IRQ_NMI signal
#define BSP_FEATURE_IOPORT_NF_IRQ40                                           (0U)                          ///< IOPORT Noise filter IRQ40 signal
#define BSP_FEATURE_IOPORT_NF_IRQ41                                           (0U)                          ///< IOPORT Noise filter IRQ41 signal
#define BSP_FEATURE_IOPORT_NF_IRQ42                                           (0U)                          ///< IOPORT Noise filter IRQ42 signal
#define BSP_FEATURE_IOPORT_NF_IRQ43                                           (0U)                          ///< IOPORT Noise filter IRQ43 signal
#define BSP_FEATURE_IOPORT_NF_IRQ44                                           (0U)                          ///< IOPORT Noise filter IRQ44 signal
#define BSP_FEATURE_IOPORT_NF_IRQ45                                           (0U)                          ///< IOPORT Noise filter IRQ45 signal
#define BSP_FEATURE_IOPORT_NF_IRQ_ESR                                         (0U)                          ///< IOPORT Noise filter IRQ_ESR signal
#define BSP_FEATURE_IOPORT_NF_INTP_NMI                                        (1U)                          ///< IOPORT Noise filter INTP_NMI signal
#define BSP_FEATURE_IOPORT_NF_INTP0                                           (1U)                          ///< IOPORT Noise filter INTP0 signal
#define BSP_FEATURE_IOPORT_NF_INTP1                                           (1U)                          ///< IOPORT Noise filter INTP1 signal
#define BSP_FEATURE_IOPORT_NF_INTP2                                           (1U)                          ///< IOPORT Noise filter INTP2 signal
#define BSP_FEATURE_IOPORT_NF_INTP3                                           (1U)                          ///< IOPORT Noise filter INTP3 signal
#define BSP_FEATURE_IOPORT_NF_INTP4                                           (1U)                          ///< IOPORT Noise filter INTP4 signal
#define BSP_FEATURE_IOPORT_NF_INTP5                                           (1U)                          ///< IOPORT Noise filter INTP5 signal
#define BSP_FEATURE_IOPORT_NF_INTP6                                           (1U)                          ///< IOPORT Noise filter INTP6 signal
#define BSP_FEATURE_IOPORT_NF_INTP7                                           (1U)                          ///< IOPORT Noise filter INTP7 signal
#define BSP_FEATURE_IOPORT_NF_INTP8                                           (1U)                          ///< IOPORT Noise filter INTP8 signal
#define BSP_FEATURE_IOPORT_NF_INTP9                                           (1U)                          ///< IOPORT Noise filter INTP9 signal
#define BSP_FEATURE_IOPORT_NF_INTP10                                          (1U)                          ///< IOPORT Noise filter INTP10 signal
#define BSP_FEATURE_IOPORT_NF_INTP11                                          (1U)                          ///< IOPORT Noise filter INTP11 signal
#define BSP_FEATURE_IOPORT_NF_INTP12                                          (1U)                          ///< IOPORT Noise filter INTP12 signal
#define BSP_FEATURE_IOPORT_NF_INTP13                                          (1U)                          ///< IOPORT Noise filter INTP13 signal
#define BSP_FEATURE_IOPORT_NF_INTP14                                          (1U)                          ///< IOPORT Noise filter INTP14 signal
#define BSP_FEATURE_IOPORT_NF_INTP15                                          (1U)                          ///< IOPORT Noise filter INTP15 signal
#define BSP_FEATURE_IOPORT_NF_INTP16                                          (1U)                          ///< IOPORT Noise filter INTP16 signal
#define BSP_FEATURE_IOPORT_NF_INTP17                                          (1U)                          ///< IOPORT Noise filter INTP17 signal
#define BSP_FEATURE_IOPORT_NF_INTP18                                          (1U)                          ///< IOPORT Noise filter INTP18 signal
#define BSP_FEATURE_IOPORT_NF_INTP19                                          (1U)                          ///< IOPORT Noise filter INTP19 signal
#define BSP_FEATURE_IOPORT_NF_INTP20                                          (1U)                          ///< IOPORT Noise filter INTP20 signal
#define BSP_FEATURE_IOPORT_NF_INTP21                                          (1U)                          ///< IOPORT Noise filter INTP21 signal
#define BSP_FEATURE_IOPORT_NF_INTP22                                          (1U)                          ///< IOPORT Noise filter INTP22 signal
#define BSP_FEATURE_IOPORT_NF_INTP23                                          (1U)                          ///< IOPORT Noise filter INTP23 signal
#define BSP_FEATURE_IOPORT_NF_INTP24                                          (1U)                          ///< IOPORT Noise filter INTP24 signal
#define BSP_FEATURE_IOPORT_NF_INTP25                                          (1U)                          ///< IOPORT Noise filter INTP25 signal
#define BSP_FEATURE_IOPORT_NF_INTP26                                          (1U)                          ///< IOPORT Noise filter INTP26 signal
#define BSP_FEATURE_IOPORT_NF_INTP27                                          (1U)                          ///< IOPORT Noise filter INTP27 signal
#define BSP_FEATURE_IOPORT_NF_INTP28                                          (1U)                          ///< IOPORT Noise filter INTP28 signal
#define BSP_FEATURE_IOPORT_NF_INTP29                                          (1U)                          ///< IOPORT Noise filter INTP29 signal
#define BSP_FEATURE_IOPORT_NF_INTP30                                          (1U)                          ///< IOPORT Noise filter INTP30 signal
#define BSP_FEATURE_IOPORT_NF_INTP31                                          (1U)                          ///< IOPORT Noise filter INTP31 signal
#define BSP_FEATURE_IOPORT_NF_INTP32                                          (1U)                          ///< IOPORT Noise filter INTP32 signal
#define BSP_FEATURE_IOPORT_NF_INTP33                                          (1U)                          ///< IOPORT Noise filter INTP33 signal
#define BSP_FEATURE_IOPORT_NF_INTP34                                          (1U)                          ///< IOPORT Noise filter INTP34 signal
#define BSP_FEATURE_IOPORT_NF_INTP35                                          (1U)                          ///< IOPORT Noise filter INTP35 signal
#define BSP_FEATURE_IOPORT_NF_INTP36                                          (1U)                          ///< IOPORT Noise filter INTP36 signal
#define BSP_FEATURE_IOPORT_NF_INTP37                                          (1U)                          ///< IOPORT Noise filter INTP37 signal
#define BSP_FEATURE_IOPORT_NF_INTP38                                          (1U)                          ///< IOPORT Noise filter INTP38 signal
#define BSP_FEATURE_IOPORT_NF_INTP39                                          (1U)                          ///< IOPORT Noise filter INTP39 signal
#define BSP_FEATURE_IOPORT_NF_ECM0                                            (1U)                          ///< IOPORT Noise filter ECM0 signal
#define BSP_FEATURE_IOPORT_NF_ECM1                                            (1U)                          ///< IOPORT Noise filter ECM1 signal
#define BSP_FEATURE_IOPORT_NF_ECM2                                            (1U)                          ///< IOPORT Noise filter ECM2 signal
#define BSP_FEATURE_IOPORT_NF_ECM3                                            (1U)                          ///< IOPORT Noise filter ECM3 signal
#define BSP_FEATURE_IOPORT_NF_ECM4                                            (0U)                          ///< IOPORT Noise filter ECM4 signal
#define BSP_FEATURE_IOPORT_NF_ECM5                                            (0U)                          ///< IOPORT Noise filter ECM5 signal
#define BSP_FEATURE_IOPORT_NF_ECM6                                            (0U)                          ///< IOPORT Noise filter ECM6 signal
#define BSP_FEATURE_IOPORT_NF_ECM7                                            (0U)                          ///< IOPORT Noise filter ECM7 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ0                                           (1U)                          ///< IOPORT Noise filter TAUJ0 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ1                                           (1U)                          ///< IOPORT Noise filter TAUJ1 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ2                                           (1U)                          ///< IOPORT Noise filter TAUJ2 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ3                                           (1U)                          ///< IOPORT Noise filter TAUJ3 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TSG                                         (0U)                          ///< IOPORT Noise filter PIC_TSG signal
#define BSP_FEATURE_IOPORT_NF_EMU0                                            (0U)                          ///< IOPORT Noise filter EMU0 signal
#define BSP_FEATURE_IOPORT_NF_EMU1                                            (0U)                          ///< IOPORT Noise filter EMU1 signal
#define BSP_FEATURE_IOPORT_NF_SSIF                                            (0U)                          ///< IOPORT Noise filter SSIF signal
#define BSP_FEATURE_IOPORT_NF_OSPI0                                           (0U)                          ///< IOPORT Noise filter OSPI0 signal

#define BSP_FEATURE_IOPORT_LVDSA                                              (1U)                          ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSB                                              (1U)                          ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSC                                              (0U)                          ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSD                                              (0U)                          ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSE                                              (0U)                          ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSF                                              (0U)                          ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDS_CTRLA_CONFIGURATION                           (0x00110000)                  ///< IOPORT LVDSCTRLA configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLB_CONFIGURATION                           (0x00110000)                  ///< IOPORT LVDSCTRLB configuration mask register

#define BSP_FEATURE_IOPORT_LVDS_CTRLA_ENABLE                                  (0x00110039)                  ///< IOPORT LVDSCTRLA enable and configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLB_ENABLE                                  (0x00110039)                  ///< IOPORT LVDSCTRLB enable and configuration mask register

#define BSP_FEATURE_IOPORT_PST                                                (3U)                          ///< Number of ECM error trigger
#define BSP_FEATURE_IOPORT_PEIODC                                             (0U)                          ///< IOPORT enable/disable ERRORIN Open-drain feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_PULVSEL5                                           (0U)                          ///< IOPORT enable/disable PULVSEL5 feature (0U): Disable/ (1U): Enable

/** ADCJ */
#define BSP_FEATURE_ADCJ_NUM_OF_UNIT                                          (3U)                          ///< Number of ADCJ units (others)

/** ECM */
#define BSP_FEATURE_ECM_ERROR_SOURCE_CHECK_VALID                              (351U)                        ///< Maximum ECM error source index supported

/** ICU */
#define BSP_FEATURE_FCLA5_IS_AVAILABLE                                        (0U)                          ///< FCLA5 register availability (1 = available)
#define BSP_FEATURE_ICU_SIGNAL_INTP                                           (1U)

/** CXPI */
#define BSP_FEATURE_CXPI_NUM_CHANNELS                                         (4U)                          ///< Maximum CXPI channel supported

/** ETNB */
#define BSP_FEATURE_ETH_AVB_NUM_CHANNELS                                      (2U)                          ///< Maximum ETH_AVB channel supported

/** RHSIF */
#define BSP_FEATURE_RHSIF_UNIT_SUPPORTED                                      (1U)                          ///< Maximum RHSIF channel supported

/** ECM */
#define BSP_FEATURE_ECM_INSTANCE                                              (11U)                         ///< ECM feature instance

/** CLMA */
#define BSP_FEATURE_CLMA_BACKUP_CLOCK_IS_AVAILABLE                            (0U)                          ///< Backup Clock support

/** PSI5 */
#define BSP_FEATURE_PSI5_MAX_CHANNEL                                          (4U)                          ///< Maximum PSI5 unit supported
#define BSP_FEATURE_PSI5_NUM_SLOTS_EACH_UNIT                                  (8U)                          ///< Maximum PSI5 slot per unit supported

/** PSI5S */
#define BSP_FEATURE_PSI5S_NUM_UNIT                                            (2U)                          ///< Maximum PSI5S unit supported

/** SCI3 */
#define BSP_FEATURE_SCI3_CLOCK                                                (FSP_PRIV_CLOCK_CLK_HSB)      ///< SCI3 Clock source HSB

/** CGC */
#define BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK                                   (1U)                          ///< Number of CPU system clock domains supported
#define BSP_FEATURE_CGC_HAS_CLK_SYS                                           (1U)                          ///< System clock CLK_SYS is available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN                                     (0U)                          ///< System clock CLK_SYS_CLEAN is not available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG                                      (0U)                          ///< System clock CLK_SYS_SSCG is not available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1                                     (0U)                          ///< System clock CLK_SYS_SSCG1 is not available
#define BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS                     (0U)                          ///< Peripheral input clock source selectors
#define BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE                                  (0U)                          ///< Sub oscillator is not available
#define BSP_FEATURE_CGC_HVIOSC_IS_AVAILABLE                                   (1U)                          ///< High Voltage Internal Oscillator is available
#define BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE                     (1U)                          ///< Number of repetition cases for System Clock gear up/down shifting

/** ECM ERRORIN NF */
#define BSP_FEATURE_ECM_ERRORIN_NF_MAX                                        IOPORT_FILTER_SIGNAL_ERRORIN3 ///< ECM ERRORIN NF Max
#define BSP_FEATURE_ECM_ERRORIN_NF_0                                          IOPORT_FILTER_SIGNAL_ERRORIN0 ///< ECM ERRORIN NF0

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU_RH850U2A16) */

#endif                                 /* BSP_FEATURE_H */
