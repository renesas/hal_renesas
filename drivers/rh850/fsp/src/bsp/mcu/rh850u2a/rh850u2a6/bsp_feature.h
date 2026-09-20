/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2AX
 * @defgroup BSP_MCU_RH850U2A6 BSP MCU U2A6
 * @brief This module provides the macros, data types, and definition constants required for U2A6 feature.
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

#ifndef BSP_FEATURE_H
#define BSP_FEATURE_H

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2A6
 * @defgroup BSP_FEATURE_U2A6 BSP U2A6 Specific Feature
 * @brief This file defines all feature-related macros for the U2A6 device, including module availability and
 * hardware capability constants.
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_PACKAGE_PINS_BGA292                                   (292U)                                                                        ///< Package pin count: BGA292
#define BSP_PACKAGE_PINS_QFP176                                   (176U)                                                                        ///< Package pin count: QFP176
#define BSP_PACKAGE_PINS_BGA156                                   (156U)                                                                        ///< Package pin count: BGA156
#define BSP_PACKAGE_PINS_QFP144                                   (144U)                                                                        ///< Package pin count: QFP144

/** Barrier Synchronization */
/** PE mask bit that is used in R_BSP_PESyncEnable */
#define BSP_BARRIER_SYNC_CPU0                                     (1U << 0U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< PE mask bit for CPU0 in R_BSP_PESyncEnable
#define BSP_BARRIER_SYNC_CPU1                                     (1U << 1U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< PE mask bit for CPU1 in R_BSP_PESyncEnable

/** PE value that is used in R_BSP_PESyncComplete */
#define BSP_CPU0                                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CPU identifier value for CPU0 in R_BSP_PESyncComplete
#define BSP_CPU1                                                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CPU identifier value for CPU1 in R_BSP_PESyncComplete

/** BSP FEATURE */
#define BSP_FEATURE_FEINT_CPU0                                    (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FEINT feature availability on CPU0 (1 = available)
#define BSP_FEATURE_FEINT_CPU1                                    (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FEINT feature availability on CPU1 (1 = available)

/** U2A6 */
#if (BSP_PACKAGE_PINS_BGA156 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_ETNB_IS_AVAILABLE                            (0x00U)                                                                       ///< ETNB feature availability (BGA156: not available)
#else
 #define BSP_FEATURE_ETNB_IS_AVAILABLE                            (0x01U)                                                                       ///< ETNB feature availability (not BGA156: available)
#endif

#define BSP_FEATURE_CXPI_IS_AVAILABLE                             (0x00U)                                                                       ///< CXPI peripheral availability (0 = not available)

#if (BSP_PACKAGE_PINS_BGA156 == BSP_PACKAGE_PINS) || (BSP_PACKAGE_PINS_QFP144 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_MMCA_IS_AVAILABLE                            (0x00U)                                                                       ///< MMCA availability (BGA156/QFP144: not available)
#else
 #define BSP_FEATURE_MMCA_IS_AVAILABLE                            (0x01U)                                                                       ///< MMCA availability (other packages: available)
#endif

#if (BSP_PACKAGE_PINS_BGA156 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_ENCA_IS_AVAILABLE                            (0x00U)                                                                       ///< ENCA availability (BGA156: not available)
#else
 #define BSP_FEATURE_ENCA_IS_AVAILABLE                            (0x01U)                                                                       ///< ENCA availability (other packages: available)
#endif

#define BSP_FEATURE_RHSIF_IS_AVAILABLE                            (0x00U)                                                                       ///< RHSIF availability (0 = not available)
#define BSP_FEATURE_SCI3_IS_AVAILABLE                             (0x00U)                                                                       ///< SCI3 availability (0 = not available)

#if (BSP_PACKAGE_PINS_BGA156 == BSP_PACKAGE_PINS) || (BSP_PACKAGE_PINS_QFP144 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_SFMA_IS_AVAILABLE                            (0x00U)                                                                       ///< Octa/Serial Flash (SFMA) availability (BGA156/QFP144: not available)
#else
 #define BSP_FEATURE_SFMA_IS_AVAILABLE                            (0x01U)                                                                       ///< Octa/Serial Flash (SFMA) availability (other packages: available)
#endif

#if (BSP_PACKAGE_PINS_BGA156 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_ADCKA_IS_AVAILABLE                           (0x00U)                                                                       ///< ADCKA availability (BGA156: not available)
#else
 #define BSP_FEATURE_ADCKA_IS_AVAILABLE                           (0x01U)                                                                       ///< ADCKA availability (other packages: available)
#endif

#define BSP_FEATURE_DFP_IS_AVAILABLE                              (0x00U)                                                                       ///< DFP feature availability (0 = not available)
#define BSP_FEATURE_DFA_IS_AVAILABLE                              (0x00U)                                                                       ///< DFA availability (0 = not available)
#define BSP_FEATURE_CLMA_STOP_DETECTION_IS_AVAILABLE              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Clock Stop Detection Mode support
#define BSP_FEATURE_BSP_PRODUCT_NUMBER_POINTER                    (0x08030040U)                                                                 ///< Address of the Product number register.
#define BSP_FEATURE_BSP_PRODUCT_NAME_POINTER                      (0x080300A0U)                                                                 ///< Address of the Product name register.
#define BSP_FEATURE_BSP_UNIQUE_ID_POINTER                         (0x08031000U)                                                                 ///< Address of the MCU Unique ID register.
#define BSP_FEATURE_FPU_IMPRECISE_EXCEPTION_IS_AVAILABLE          (0U)                                                                          ///< FPU imprecise exception is not available
#define BSP_FEATURE_FPEC_REGISTER_IS_AVAILABLE                    (0U)                                                                          ///< FPEC system register is not available
#define BSP_IO_PORT_TYPE_MAX                                      (3U)                                                                          ///< IO Port Type max value

/** MODULE RESET */
#define BSP_RESET_SOURCE_POWER_OFF_STANDBY_IS_AVAILABLE           (0x00U)                                                                       ///< Power-off standby reset source support (0 = not available)
#define BSP_RESET_ECM_SELECT_RESET_SOURCE_IS_AVAILABLE            (0x01U)                                                                       ///< ECM-selectable reset source support (1 = available)
#define BSP_RESET_MODULE_RSCFD                                    RSCFD /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Module reset name: RSCFD
#define BSP_RESET_MODULE_FLXA                                     FLXA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: FLXA
#define BSP_RESET_MODULE_GTM                                      GTM /* PRQA S 1534 # Advisory R-2.5 */                                        ///< Module reset name: GTM
#if (BSP_FEATURE_ETNB_IS_AVAILABLE)
 #define BSP_RESET_MODULE_ETNB                                    ETNB /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: ETNB
#endif
#define BSP_RESET_MODULE_RSENT                                    RSENT /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Module reset name: RSENT
#define BSP_RESET_MODULE_MSPI                                     MSPI /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: MSPI
#define BSP_RESET_MODULE_RLIN3                                    RLIN3 /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Module reset name: RLIN3
#define BSP_RESET_MODULE_ADCJ_ISO                                 ADCJ_ISO /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Module reset name: ADCJ_ISO
#define BSP_RESET_MODULE_CXPI                                     CXPI /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: CXPI
#if (BSP_FEATURE_MMCA_IS_AVAILABLE)
 #define BSP_RESET_MODULE_MMCA                                    MMCA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: MMCA
#endif
#if (BSP_FEATURE_ENCA_IS_AVAILABLE)
 #define BSP_RESET_MODULE_ENCA                                    ENCA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: ENCA
#endif
#define BSP_RESET_MODULE_PSI5                                     PSI5 /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: PSI5
#define BSP_RESET_MODULE_PSI5S                                    PSI5S /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Module reset name: PSI5S
#define BSP_RESET_MODULE_PWMD                                     PWMD /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: PWMD
#define BSP_RESET_MODULE_RIIC                                     RIIC /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: RIIC
#define BSP_RESET_MODULE_SCI3                                     SCI3 /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: SCI3
#if (BSP_FEATURE_SFMA_IS_AVAILABLE)
 #define BSP_RESET_MODULE_SFMA                                    SFMA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: SFMA
#endif
#define BSP_RESET_MODULE_TAPA                                     TAPA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: TAPA
#define BSP_RESET_MODULE_TAUD                                     TAUD /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: TAUD
#define BSP_RESET_MODULE_TAUJ_ISO                                 TAUJ_ISO /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Module reset name: TAUJ_ISO
#define BSP_RESET_MODULE_TPBA                                     TPBA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: TPBA
#define BSP_RESET_MODULE_TSG3                                     TSG3 /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: TSG3
#define BSP_RESET_MODULE_OSTM                                     OSTM /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: OSTM

/** STANDBY MODE */
#define BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_IS_AVAILABLE            (0x01U)                                                                       ///< DeepStop mode availability (1 = available)
#define BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_NUM_OF_WAKEUP_SOURCE    (0x05U)                                                                       ///< Number of DeepStop wakeup sources
#define BSP_FEATURE_CHIP_STANDBY_IOHOLD_REG_NAME                  (0x02U)                                                                       ///< IOHOLD register selector for standby retention

/** OPBT */
#define BSP_FEATURE_OPBT_BVA1_AREA_IS_AVAILABLE                   (0x00U)                                                                       ///< BVA1 Area is available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_1_AVAILABLE            (0x00U)                                                                       ///< OPBT Cluster 1 switching is available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_2_AVAILABLE            (0x00U)                                                                       ///< OPBT Cluster 2 switching is not available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_3_AVAILABLE            (0x00U)                                                                       ///< OPBT Cluster 3 switching is not available

/** QOS */
#define BSP_FEATURE_QOS_IS_AVAILABLE                              (0U)                                                                          ///< QoS feature is not available on this device

/** INTSEL */
#define BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ                         (0U)                                                                          ///< INTSEL IRQ selection is not supported

/** INT MASK */
#define BSP_FEATURE_DEVICE_HAS_ORED_IRQ                           (0U)                                                                          ///< ORed IRQ support is not available

/** CANFD */
#if (BSP_PACKAGE_PINS_QFP144 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS                           (7UL)                                                                         ///< Number of CAN FD channels (QFP144)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT0                     (6UL)                                                                         ///< Number of CAN FD Channels in Unit 0
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT1                     (1UL)                                                                         ///< Number of CAN FD Channels in Unit 1
#elif (BSP_PACKAGE_PINS_BGA156 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS                           (8UL)                                                                         ///< Number of CAN FD channels (BGA156)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT0                     (4UL)                                                                         ///< Number of CAN FD Channels in Unit 0
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT1                     (4UL)                                                                         ///< Number of CAN FD Channels in Unit 1
#elif (BSP_PACKAGE_PINS_QFP176 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS                           (11UL)                                                                        ///< Number of CAN FD channels (QFP176)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT0                     (7UL)                                                                         ///< Number of CAN FD Channels in Unit 0
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT1                     (4UL)                                                                         ///< Number of CAN FD Channels in Unit 1
#else
 #define BSP_FEATURE_CANFD_NUM_CHANNELS                           (12UL) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Number of CAN FD channels (default/other packages)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT0                     (8UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CAN FD Channels in Unit 0
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT1                     (4UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CAN FD Channels in Unit 1
#endif

#define BSP_FEATURE_CANFD_NUM_UNIT                                (2UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CANFD units

#define BSP_FEATURE_CANFD_NUM_TXQUEUE_CHANNEL                     (4UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CAN FD TX queue channels
#define BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL                       (3UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CAN FD CFIFO channels per unit
#define BSP_FEATURE_CANFD_NUM_RXFIFO                              (8UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Total number of CAN FD RX FIFOs
#define BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT                      (24UL) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Total number of CAN FD CFIFOs on the MCU
#define BSP_FEATURE_CANFD_TXMB_SUPPORTED_SLOT                     (0x0000FFFF0000FFFFULL) /* PRQA S 1534 # Advisory R-2.5 */                    ///< TX message buffer supported slots (0-15 and 32-47)
#define BSP_FEATURE_CANFD_TXMB_MAX_NUM                            (32UL) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Maximum number of TX message buffers
#define BSP_FEATURE_CANFD_CLKC_SRC                                (FSP_PRIV_CLOCK_CLK_HSB) /* PRQA S 1534 # Advisory R-2.5 */                   ///< Clock source for CAN FD CLKC
#define BSP_FEATURE_CANFD_AFLMRAM_MAX_PAGE                        (305UL) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Number of AFL MRAM page
#define BSP_FEATURE_CANFD_AFLMRAM_PER_CHANNEL                     (9760UL) /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Number of AFL MRAM area size

/** GUARD */
#define BSP_FEATURE_PEGCAP_M_IS_AVAILABLE                         (0x01U)                                                                       ///< PEGCAP_M feature availability (1 = available)
#define BSP_FEATURE_PE1_GUARD_IS_AVAILABLE                        (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PE1 guard availability
#define BSP_FEATURE_PE2_GUARD_IS_AVAILABLE                        (0U)                                                                          ///< PE2 guard availability
#define BSP_FEATURE_PE3_GUARD_IS_AVAILABLE                        (0U)                                                                          ///< PE3 guard availability
#define BSP_FEATURE_CRAM0_GUARD_IS_AVAILABLE                      (1U)                                                                          ///< CRAM0 guard availability
#define BSP_FEATURE_CRAM1_GUARD_IS_AVAILABLE                      (0U)                                                                          ///< CRAM1 guard availability
#define BSP_FEATURE_CRAM2_GUARD_IS_AVAILABLE                      (0U)                                                                          ///< CRAM2 guard availability
#define BSP_FEATURE_CRAM3_GUARD_IS_AVAILABLE                      (1U)                                                                          ///< CRAM3 guard availability
#define BSP_FEATURE_CRAM_GUARD_STATUS_MAX                         (6U)                                                                          ///< Maximum CRAM guard status index/value
#define BSP_FEATURE_PE_GUARD_STATUS_MAX                           (3U)                                                                          ///< Maximum PE guard status index/value
#define BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IPIR guard availability
#define BSP_FEATURE_BARR_GUARD_IS_AVAILABLE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Barrier guard availability
#define BSP_FEATURE_TPTMn_GUARD_MAX_CHANNEL                       (2U)                                                                          ///< Maximum TPTMn guarded channel count
#define BSP_FEATURE_INTC2_GUARD_MAX_CHANNEL                       (767U)                                                                        ///< Maximum INTC2 guarded channel index
#define BSP_FEATURE_INTC2_GUARD_MIN_CHANNEL                       (32U)                                                                         ///< Minimum INTC2 guarded channel index
#define BSP_FEATURE_DTS_GUARD_MAX_CHANNEL                         (128U)                                                                        ///< Maximum DTS guarded channel count
#define BSP_FEATURE_DMAC_GUARD_MAX_CHANNEL                        (16U)                                                                         ///< Maximum DMAC guarded channel count

/** MSPI */
#define BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT                   (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum chip selects per MSPI unit
#define BSP_FEATURE_MSPI_TRIG_TPTM_NUMBER_UNIT                    (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of TPTM units usable as an MSPI hardware trigger source
#define BSP_FEATURE_MSPI_DMA_ALT_SPECIAL_MAPPING_SUPPORT          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Special DMA ALT mapping is not supported
#define BSP_FEATURE_MSPI_DTS_ALT_SPECIAL_MAPPING_SUPPORT          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Special DTS ALT mapping is not supported
#if (BSP_PACKAGE_PINS_QFP144 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_MSPI_MAX_UNIT                                (4U)                                                                          ///< Number of MSPI units (QFP144)
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_0               (8U)                                                                          ///< Number of chip selects in MSPI unit 0
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_1               (8U)                                                                          ///< Number of chip selects in MSPI unit 1
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_2               (8U)                                                                          ///< Number of chip selects in MSPI unit 2
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_3               (8U)                                                                          ///< Number of chip selects in MSPI unit 3
 #define BSP_FEATURE_ATU_GTM_SEL                                  (0U)                                                                          ///< ATU GTM trigger select bit (0 = default)
#else
 #define BSP_FEATURE_MSPI_MAX_UNIT                                (6U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of MSPI units (non-QFP144)
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_0               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of chip selects in MSPI unit 0
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_1               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of chip selects in MSPI unit 1
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_2               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of chip selects in MSPI unit 2
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_3               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of chip selects in MSPI unit 3
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_4               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of chip selects in MSPI unit 4
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_5               (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of chip selects in MSPI unit 5
 #define BSP_FEATURE_ATU_GTM_SEL                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ATU GTM trigger select bit (0 = default)
#endif
#define BSP_FEATURE_MSPI_ALT_DMA_UNIT                             (0xCCU) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Bitmask/sequence for MSPI units supporting ALT DMA
#define BSP_FEATURE_MSPI_ALT_DTS_UNIT                             (0xF0U) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Bitmask/sequence for MSPI units supporting ALT DTS
#define BSP_FEATURE_MSPI_ALT_DMA_MAX_UNIT                         (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum number of MSPI units with ALT DMA
#define BSP_FEATURE_MSPI_ALT_DTS_MAX_UNIT                         (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum number of MSPI units with ALT DTS
#define BSP_FEATURE_MSPI_ALT_GAP_CHANNEL                          (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Channel gap per ALT grouping
#define BSP_FEATURE_MSPI_ALT_GAP_UNIT                             (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Unit gap between ALT-capable units
#define BSP_FEATURE_MSPI_ALT_PAIR_CHANNEL                         (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Channel pairing size for ALT
#define BSP_FEATURE_MSPI_ALT_MAX_CHANNEL                          (7U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum channel index having ALT support
#define BSP_FEATURE_MSPI_ALT_DMA_MAX_BIT                          (9U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Highest ALT DMA bit position used
#define BSP_FEATURE_MSPI_ALT_DTS_MAX_BIT                          (13U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Highest ALT DTS bit position used

/** IIC */
#define BSP_FEATURE_IIC_HAS_SYS_CLK                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IIC can be clocked from system clock
#define BSP_FEATURE_IIC_VALID_CHANNEL_MASK                        (0x03U) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Valid IIC channel bitmask

/** RSENT */
#define BSP_FEATURE_RSENT_SENT_NUM_OF_UNIT                        (5U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of RSENT units on this MCU
#define BSP_FEATURE_RSENT_SENT_SPCO_SUPPORT                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RSENT SPCO select function is not supported
#define BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD                     (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RSENT unit index threshold between MSSEL register groups
#define BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS              (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of bits per unit in the RSENT MSSEL register
#define BSP_FEATURE_RSENT_SENT_HAS_EXTENDED_TSSEL                 (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RSENT does not have extended TSSEL2-TSSEL7 registers

/** RLIN3 */
#define BSP_FEATURE_RLIN3_RLIN323_HAS_PBUS_GROUP_8L               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RLIN323 channel is not on PBUS group 8L

/** TAUD3P */
#define BSP_FEATURE_TAUD_THREE_PHASE_NUMBER_OF_UNIT               (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of TAUD three-phase control units

/** MSPI */
#define BSP_FEATURE_MSPI_RAM_WORDS_MAX                            (512U) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< MSPI RAM size

/** PIC */
#define BSP_FEATURE_PIC_ADC_NUMBER_UNIT                           (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of PIC-ADC units
#define BSP_FEATURE_PIC_ADC_NUMBER_SG                             (5U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of PIC-ADC scan groups per unit
#define BSP_FEATURE_PIC_PSI5S_NUMBER_UNIT                         (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of PIC PSI5S units
#define BSP_FEATURE_PIC_PSI5SEN_NUMBER                            (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of PSI5SEN instances
#define BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP                      (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTMINEN groups

/** PWM DIAG */
#define BSP_FEATURE_PWM_DIAG_NUM_OF_CHANNEL                       (96U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of PWM diagnostic channels

/** FR */
#define BSP_FEATURE_FLXA_MAX_UNIT                                 (1U)                                                                          ///< Maximum number of FLXA units
#define BSP_FEATURE_FLXA_IS_AVAILABLE                             (1U)                                                                          ///< FLXA availability (1 = available)

/** PIC (additional) */ /* PRQA S 2053 # Advisory D-4.4 */
#define BSP_FEATURE_PIC_TAUD_INT_SIGNAL_OUT_NUMBER                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of TAUD interrupt signal outputs via PIC
#define BSP_FEATURE_PIC_TAUD_TRIANGLE_WAVE_NUMBER                 (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of TAUD triangle wave generators
#define BSP_FEATURE_PIC_TAUD_PWM_OUTPUT_NUMBER                    (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of TAUD PWM outputs
#define BSP_FEATURE_PIC_TAUD_DELAY_PULSE_NUMBER                   (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of TAUD delay pulse generators
#define BSP_FEATURE_PIC_ENCA_FEATURE_NUMBER_UNIT                  (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of ENCA-featured units
#define BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT                        (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of TAUD units in PIC20
#define BSP_FEATURE_PIC_ADCJ_TRIG_SEL                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCJ trigger selection support (1 = supported)
#define BSP_FEATURE_PIC_SWITCH_TSG_OUT                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG output switching support (0 = not supported)
#define BSP_FEATURE_PIC_EMU_FEATURE                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< EMU feature within PIC (0 = not available)
#define BSP_FEATURE_PIC_TSG_START_TRIG                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG start trigger support (0 = not supported)
#define BSP_FEATURE_PIC_3PHASE_ENCODER_TSG31                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< 3-phase encoder with TSG31 support
#define BSP_FEATURE_PIC_2PHASE_SEL_INTENCA1_TO_TSG3               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< 2-phase INTENCA1 selection to TSG3 supported
#define BSP_FEATURE_PIC_ENC_CAPT_TRIG_ENCA1                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA1 capture trigger supported
#define BSP_FEATURE_PIC_INT_SIG_OUT_TAUD1                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD1 interrupt signal output (0 = not available)
#define BSP_FEATURE_PIC_INT_SIG_OUT_TAUD2                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD2 interrupt signal output (0 = not available)
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT12             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Pulse width measurement via DT12 (0 = not supported)
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT00             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Pulse width measurement via DT00 supported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT10             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Pulse width measurement via DT10 supported
#define BSP_FEATURE_PIC_ENCA_INPUT_SEL                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA input selection (0 = fixed/limited)
#define BSP_FEATURE_PIC_FCMP_FEATURE                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FCMP feature (0 = not available)
#define BSP_FEATURE_PIC_HRPWM_FEATURE                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< HRPWM feature (0 = not available)
#define BSP_FEATURE_PIC_TIMER_INPUT_MONITOR                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Timer input monitor support
#define BSP_FEATURE_PIC_TAUD_INPUT_SEL                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD input selection feature (0 = not available)
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA trigger selection supported
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL                        (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA encoder input selection supported
#define BSP_FEATURE_PIC_RDC_FEATURE                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RDC feature (0 = not available)
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CAN/GTM I/O sharing (0 = not available)
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI trigger selection (0 = not available)
#define BSP_FEATURE_PIC_GTM_DTM_INPUT_SEL                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM DTM input selection (0 = not available)
#define BSP_FEATURE_PIC_TSG3_INPUT_GTM                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG3 input from GTM (0 = not available)
#define BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC13                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG sync start/clear via PIC13 (0 = not available)
#define BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC1_SELB            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG sync start/clear via PIC1 SELB supported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD2                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TIMER IN SEL B to TAUD2 (U2Cx) (0 = not supported)
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD_TAUJ                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TIMER IN SEL B to TAUD/TAUJ (U2Bx) supported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TIMER IN SEL B feature availability
#define BSP_FEATURE_PIC_TIMER_OUT_MON_PIC13                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Timer out monitor via PIC13 (0 = not available)
#define BSP_FEATURE_PIC_TIMER_OUT_MON_PIC1_SELB                   (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Timer out monitor via PIC1 SELB supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC22              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATSEL on PIC22 (U2Bx) (0 = not supported)
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC24              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATCFG on PIC24 (0 = not supported)
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC2_SELB          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATCFG on PIC2 SELB (0 = not supported)
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC21              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATSEL on PIC21 (U2Ax) supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC22              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATCFG on PIC22 supported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC24                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S trigger via GTM PIC24 unsupported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC2_SELB              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S trigger via GTM PIC2 SELB unsupported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC22                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S trigger via GTM PIC22 supported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S trigger selection supported
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC24                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIO input via PIC24 supported
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC2_SELB                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIO input via PIC2 SELB unavailable
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC22                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIO input via PIC22 unavailable
#define BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIO input selection unsupported
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC24                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIM input on PIC24 (0 = not available)
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC2_SELB                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIM input on PIC2 SELB (0 = not available)
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC22                          (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIM input on PIC22 supported
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC24                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA encoder input on PIC24 (0 = not available)
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC22                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA encoder input on PIC22 supported
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC24                        (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Hi-Z control via GTM on PIC24 (0 = not available)
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC2_SELB                    (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Hi-Z control via GTM on PIC2 SELB (0 = not available)
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC20                        (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Hi-Z control via GTM on PIC20 supported
#define BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN                           (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< HIZCEN control supported
#define BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Single SST mode on PIC1 supported
#define BSP_FEATURE_PIC_FEATURE_U2AX                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PIC feature set U2Ax supported

/** GTM */
#define BSP_FEATURE_GTM_ARU_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM ARU instances
#define BSP_FEATURE_GTM_BRC_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM BRC instances
#define BSP_FEATURE_GTM_PSM_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM PSM instances
#define BSP_FEATURE_GTM_CMU_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM CMU instances
#define BSP_FEATURE_GTM_CCM_NUMBER_INSTANCE                       (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM CCM instances
#define BSP_FEATURE_GTM_TBU_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM TBU instances
#define BSP_FEATURE_GTM_TIM_NUMBER_INSTANCE                       (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM TIM instances
#define BSP_FEATURE_GTM_TOM_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM TOM instances
#define BSP_FEATURE_GTM_ATOM_NUMBER_INSTANCE                      (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM ATOM instances
#define BSP_FEATURE_GTM_DTM_NUMBER_INSTANCE                       (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM DTM instances
#define BSP_FEATURE_GTM_MCS_NUMBER_INSTANCE                       (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCS instances
#define BSP_FEATURE_GTM_MCFG_NUMBER_INSTANCE                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCFG instances
#define BSP_FEATURE_GTM_MAP_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MAP instances
#define BSP_FEATURE_GTM_DPLL_NUMBER_INSTANCE                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM DPLL instances
#define BSP_FEATURE_GTM_SPE_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM SPE instances
#define BSP_FEATURE_GTM_ICM_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM ICM instances
#define BSP_FEATURE_GTM_CMP_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM CMP instances
#define BSP_FEATURE_GTM_MON_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MON instances
#define BSP_FEATURE_GTM_TIO_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM TIO instances
#define BSP_FEATURE_GTM_INTSEL_SUPPORT                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Interconnections of Interrupt Factors Support

/** GTM_MCS */
#define BSP_FEATURE_GTM_MCS_NUMBER_CHANNEL                        (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM MCS channel count per instance
#define BSP_FEATURE_GTM_MCS_NUMBER_TRIGGER                        (24U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< GTM MCS trigger sources
#define BSP_FEATURE_GTM_MCS_NUMBER_SINT                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCS SINT sources
#define BSP_FEATURE_GTM_MCS_NUMBER_HBP                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCS HBP units
#define BSP_FEATURE_GTM_MCS_SP_CNT_AVAILABLE                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Stack Pointer Counter support
#define BSP_FEATURE_GTM_MCS_EXT_IRQ_AVAILABLE                     (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< External Interrupt Selection support

/** FREERTOS Port */
#define BSP_FEATURE_FREERTOS_PORT_OSTM0_EIC_ADDRESS               (0xFFF8018E) /* PRQA S 1534 # Advisory R-2.5 */                               ///< OSTM0 EI level interrupt control register address
#define BSP_FEATURE_FREERTOS_PORT_OSTM0_CMP_ADDRESS               (0xFFBF0000) /* PRQA S 1534 # Advisory R-2.5 */                               ///< OSTM0 compare register address
#define BSP_FEATURE_FREERTOS_PORT_OSTM0_CTL_ADDRESS               (0xFFBF0020) /* PRQA S 1534 # Advisory R-2.5 */                               ///< OSTM0 control register address
#define BSP_FEATURE_FREERTOS_PORT_OSTM0_TS_ADDRESS                (0xFFBF0014) /* PRQA S 1534 # Advisory R-2.5 */                               ///< OSTM0 count start trigger register address

/** TAUD */
#define BSP_FEATURE_TAUD_MAX_UNIT                                 (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum number of TAUD units

/** TAUJ */
#define BSP_FEATURE_TAUJ_FOUR_UNITS_V1_AVAILABLE                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUJ four units v1 are available
#define BSP_FEATURE_TAUJ_SINGLE_UNIT_AVAILABLE                    (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUJ single unit is available
#define BSP_FEATURE_TAUJ_FOUR_UNITS_V2_AVAILABLE                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUJ four units v2 are available

/** CLMA */
#define BSP_FEATURE_CLMA_MAX_CHANNEL                              (10U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Maximum CLMA channels
#define BSP_FEATURE_CLMA_SSCG1_UNDIVIDED_REQUIRED                 (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CLMA CKS_SSCG1C Requirement

/** VMON */
#define BSP_FEATURE_VMON_VDD2_AVAILABLE                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< VMON VDD2 group index/support (0 = none or group 0)
#define BSP_FEATURE_VMON_AWOVDD_AVAILABLE                         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< VMON AWOVDD group index/support (group 1)
#define BSP_FEATURE_VMON_FC_ENABLE                                (0xFFU) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Bit positions for enabling VMON functionality
#define BSP_FEATURE_VMON_DMASK_ENABLE                             (0xFFU) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Enable/disable VMONDMASK functionality
#define BSP_FEATURE_VMON_OUT_DMASK_ENABLE                         (0xF0U) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Enable/disable VMON_OUT_DMASK functionality
#define BSP_FEATURE_VMON_RESET_DMASK_ENABLE                       (0x0FU) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Enable/disable VMON_RESET_DMASK functionality

/** RLIN3 */
#if (BSP_PACKAGE_PINS_BGA292 == BSP_PACKAGE_PINS) || (BSP_PACKAGE_PINS_QFP176 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK                      (0xFFFUL) /* PRQA S 1534 # Advisory R-2.5 */                                  // Bitmask of available RLIN3 LIN channels
 #define BSP_FEATURE_RLIN3_LIN_NUM_OF_CHANNELS                    (12U) /* PRQA S 1534 # Advisory R-2.5 */                                      // Number of RLIN3 LIN channels on this MCU
#elif (BSP_PACKAGE_PINS_BGA156 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK                      (0xF39UL)                                                                     // Bitmask of available RLIN3 LIN channels
 #define BSP_FEATURE_RLIN3_LIN_NUM_OF_CHANNELS                    (8U)                                                                          // Number of RLIN3 LIN channels on this MCU
#elif (BSP_PACKAGE_PINS_QFP144 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK                      (0xFBDUL)                                                                     // Bitmask of available RLIN3 LIN channels
 #define BSP_FEATURE_RLIN3_LIN_NUM_OF_CHANNELS                    (10U)                                                                         // Number of RLIN3 LIN channels on this MCU
#endif

/** IOPORT */
#define BSP_FEATURE_IOPORT_NF_RLIN30                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RLIN30 signal
#define BSP_FEATURE_IOPORT_NF_RLIN31                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RLIN31 signal
#define BSP_FEATURE_IOPORT_NF_RSCAN                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSCAN signal
#define BSP_FEATURE_IOPORT_NF_FLXA0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter FLXA0 signal
#define BSP_FEATURE_IOPORT_NF_FLXA1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter FLXA1 signal
#define BSP_FEATURE_IOPORT_NF_RSENT0                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT0 signal
#define BSP_FEATURE_IOPORT_NF_RSENT1                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT1 signal
#define BSP_FEATURE_IOPORT_NF_RSENT2                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT2 signal
#define BSP_FEATURE_IOPORT_NF_RSENT3                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT3 signal
#define BSP_FEATURE_IOPORT_NF_RSENT4                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT4 signal
#define BSP_FEATURE_IOPORT_NF_RSENT5                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT5 signal
#define BSP_FEATURE_IOPORT_NF_RSENT6                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT6 signal
#define BSP_FEATURE_IOPORT_NF_RSENT7                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT7 signal
#define BSP_FEATURE_IOPORT_NF_RSENT8                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT8 signal
#define BSP_FEATURE_IOPORT_NF_RSENT9                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT9 signal
#define BSP_FEATURE_IOPORT_NF_RSENT10                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT10 signal
#define BSP_FEATURE_IOPORT_NF_RSENT11                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT11 signal
#define BSP_FEATURE_IOPORT_NF_RSENT12                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT12 signal
#define BSP_FEATURE_IOPORT_NF_RSENT13                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT13 signal
#define BSP_FEATURE_IOPORT_NF_RSENT14                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT14 signal
#define BSP_FEATURE_IOPORT_NF_RSENT15                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT15 signal
#define BSP_FEATURE_IOPORT_NF_RSENT16                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT16 signal
#define BSP_FEATURE_IOPORT_NF_RSENT17                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT17 signal
#define BSP_FEATURE_IOPORT_NF_RSENT18                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT18 signal
#define BSP_FEATURE_IOPORT_NF_RSENT19                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT19 signal
#define BSP_FEATURE_IOPORT_NF_RSENT20                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT20 signal
#define BSP_FEATURE_IOPORT_NF_RSENT21                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT21 signal
#define BSP_FEATURE_IOPORT_NF_RSENT22                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT22 signal
#define BSP_FEATURE_IOPORT_NF_RSENT23                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT23 signal
#define BSP_FEATURE_IOPORT_NF_RSENT24                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT24 signal
#define BSP_FEATURE_IOPORT_NF_RSENT25                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT25 signal
#define BSP_FEATURE_IOPORT_NF_RSENT26                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT26 signal
#define BSP_FEATURE_IOPORT_NF_RSENT27                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT27 signal
#define BSP_FEATURE_IOPORT_NF_RSENT28                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT28 signal
#define BSP_FEATURE_IOPORT_NF_RSENT29                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSENT29 signal
#define BSP_FEATURE_IOPORT_NF_PSI50                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PSI50 signal
#if (BSP_PACKAGE_PINS_QFP144 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_IOPORT_NF_PSI51                              (0U)                                                                          ///< IOPORT Noise filter PSI51 signal
#else
 #define BSP_FEATURE_IOPORT_NF_PSI51                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PSI51 signal
#endif
#define BSP_FEATURE_IOPORT_NF_PSI52                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PSI52 signal
#define BSP_FEATURE_IOPORT_NF_PSI53                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PSI53 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA0                           (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA0 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA1                           (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA1 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA2                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA2 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA3                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA3 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA4                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA4 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA5                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA5 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCJ0 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCJ1 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ2                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCJ2 signal
#define BSP_FEATURE_IOPORT_NF_ADCK0                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCK0 signal
#define BSP_FEATURE_IOPORT_NF_ADCK1                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCK1 signal
#define BSP_FEATURE_IOPORT_NF_ADCK2                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCK2 signal
#define BSP_FEATURE_IOPORT_NF_ADCK3                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCK3 signal
#define BSP_FEATURE_IOPORT_NF_ADCKA                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCKA signal
#define BSP_FEATURE_IOPORT_NF_RHSB0                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RHSB0 signal
#define BSP_FEATURE_IOPORT_NF_RHSB1                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RHSB1 signal
#define BSP_FEATURE_IOPORT_NF_RHSB2                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RHSB2 signal
#define BSP_FEATURE_IOPORT_NF_RHSB3                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RHSB3 signal
#define BSP_FEATURE_IOPORT_NF_ENCA0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ENCA0 signal
#define BSP_FEATURE_IOPORT_NF_ENCA1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ENCA1 signal
#define BSP_FEATURE_IOPORT_NF_TAUD0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUD0 signal
#define BSP_FEATURE_IOPORT_NF_TAUD1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUD1 signal
#define BSP_FEATURE_IOPORT_NF_TAUD2                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUD2 signal
#define BSP_FEATURE_IOPORT_NF_TAUD3                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUD3 signal
#define BSP_FEATURE_IOPORT_NF_TSG30                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TSG30 signal
#define BSP_FEATURE_IOPORT_NF_TSG31                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TSG31 signal
#define BSP_FEATURE_IOPORT_NF_TSG32                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TSG32 signal
#define BSP_FEATURE_IOPORT_NF_IRQ0                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ0 signal
#define BSP_FEATURE_IOPORT_NF_IRQ1                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ1 signal
#define BSP_FEATURE_IOPORT_NF_IRQ2                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ2 signal
#define BSP_FEATURE_IOPORT_NF_IRQ3                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ3 signal
#define BSP_FEATURE_IOPORT_NF_IRQ4                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ4 signal
#define BSP_FEATURE_IOPORT_NF_IRQ5                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ5 signal
#define BSP_FEATURE_IOPORT_NF_IRQ6                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ6 signal
#define BSP_FEATURE_IOPORT_NF_IRQ7                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ7 signal
#define BSP_FEATURE_IOPORT_NF_IRQ8                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ8 signal
#define BSP_FEATURE_IOPORT_NF_IRQ9                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ9 signal
#define BSP_FEATURE_IOPORT_NF_IRQ10                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ10 signal
#define BSP_FEATURE_IOPORT_NF_IRQ11                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ11 signal
#define BSP_FEATURE_IOPORT_NF_IRQ12                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ12 signal
#define BSP_FEATURE_IOPORT_NF_IRQ13                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ13 signal
#define BSP_FEATURE_IOPORT_NF_IRQ14                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ14 signal
#define BSP_FEATURE_IOPORT_NF_IRQ15                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ15 signal
#define BSP_FEATURE_IOPORT_NF_IRQ16                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ16 signal
#define BSP_FEATURE_IOPORT_NF_IRQ17                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ17 signal
#define BSP_FEATURE_IOPORT_NF_IRQ18                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ18 signal
#define BSP_FEATURE_IOPORT_NF_IRQ19                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ19 signal
#define BSP_FEATURE_IOPORT_NF_IRQ20                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ20 signal
#define BSP_FEATURE_IOPORT_NF_IRQ21                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ21 signal
#define BSP_FEATURE_IOPORT_NF_IRQ22                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ22 signal
#define BSP_FEATURE_IOPORT_NF_IRQ23                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ23 signal
#define BSP_FEATURE_IOPORT_NF_IRQ24                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ24 signal
#define BSP_FEATURE_IOPORT_NF_IRQ25                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ25 signal
#define BSP_FEATURE_IOPORT_NF_IRQ26                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ26 signal
#define BSP_FEATURE_IOPORT_NF_IRQ27                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ27 signal
#define BSP_FEATURE_IOPORT_NF_IRQ28                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ28 signal
#define BSP_FEATURE_IOPORT_NF_IRQ29                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ29 signal
#define BSP_FEATURE_IOPORT_NF_IRQ30                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ30 signal
#define BSP_FEATURE_IOPORT_NF_IRQ31                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ31 signal
#define BSP_FEATURE_IOPORT_NF_IRQ32                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ32 signal
#define BSP_FEATURE_IOPORT_NF_IRQ33                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ33 signal
#define BSP_FEATURE_IOPORT_NF_IRQ34                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ34 signal
#define BSP_FEATURE_IOPORT_NF_IRQ35                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ35 signal
#define BSP_FEATURE_IOPORT_NF_IRQ36                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ36 signal
#define BSP_FEATURE_IOPORT_NF_IRQ37                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ37 signal
#define BSP_FEATURE_IOPORT_NF_IRQ38                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ38 signal
#define BSP_FEATURE_IOPORT_NF_IRQ39                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ39 signal
#define BSP_FEATURE_IOPORT_NF_IRQ_NMI                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ_NMI signal
#define BSP_FEATURE_IOPORT_NF_IRQ40                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ40 signal
#define BSP_FEATURE_IOPORT_NF_IRQ41                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ41 signal
#define BSP_FEATURE_IOPORT_NF_IRQ42                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ42 signal
#define BSP_FEATURE_IOPORT_NF_IRQ43                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ43 signal
#define BSP_FEATURE_IOPORT_NF_IRQ44                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ44 signal
#define BSP_FEATURE_IOPORT_NF_IRQ45                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ45 signal
#define BSP_FEATURE_IOPORT_NF_IRQ_ESR                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ_ESR signal
#define BSP_FEATURE_IOPORT_NF_INTP_NMI                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP_NMI signal
#define BSP_FEATURE_IOPORT_NF_INTP0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP0 signal
#define BSP_FEATURE_IOPORT_NF_INTP1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP1 signal
#define BSP_FEATURE_IOPORT_NF_INTP2                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP2 signal
#define BSP_FEATURE_IOPORT_NF_INTP3                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP3 signal
#define BSP_FEATURE_IOPORT_NF_INTP4                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP4 signal
#define BSP_FEATURE_IOPORT_NF_INTP5                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP5 signal
#define BSP_FEATURE_IOPORT_NF_INTP6                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP6 signal
#define BSP_FEATURE_IOPORT_NF_INTP7                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP7 signal
#define BSP_FEATURE_IOPORT_NF_INTP8                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP8 signal
#define BSP_FEATURE_IOPORT_NF_INTP9                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP9 signal
#define BSP_FEATURE_IOPORT_NF_INTP10                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP10 signal
#define BSP_FEATURE_IOPORT_NF_INTP11                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP11 signal
#define BSP_FEATURE_IOPORT_NF_INTP12                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP12 signal
#define BSP_FEATURE_IOPORT_NF_INTP13                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP13 signal
#define BSP_FEATURE_IOPORT_NF_INTP14                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP14 signal
#define BSP_FEATURE_IOPORT_NF_INTP15                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP15 signal
#define BSP_FEATURE_IOPORT_NF_INTP16                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP16 signal
#define BSP_FEATURE_IOPORT_NF_INTP17                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP17 signal
#define BSP_FEATURE_IOPORT_NF_INTP18                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP18 signal
#define BSP_FEATURE_IOPORT_NF_INTP19                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP19 signal
#define BSP_FEATURE_IOPORT_NF_INTP20                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP20 signal
#define BSP_FEATURE_IOPORT_NF_INTP21                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP21 signal
#define BSP_FEATURE_IOPORT_NF_INTP22                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP22 signal
#define BSP_FEATURE_IOPORT_NF_INTP23                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP23 signal
#define BSP_FEATURE_IOPORT_NF_INTP24                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP24 signal
#define BSP_FEATURE_IOPORT_NF_INTP25                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP25 signal
#define BSP_FEATURE_IOPORT_NF_INTP26                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP26 signal
#define BSP_FEATURE_IOPORT_NF_INTP27                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP27 signal
#define BSP_FEATURE_IOPORT_NF_INTP28                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP28 signal
#define BSP_FEATURE_IOPORT_NF_INTP29                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP29 signal
#define BSP_FEATURE_IOPORT_NF_INTP30                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP30 signal
#define BSP_FEATURE_IOPORT_NF_INTP31                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP31 signal
#define BSP_FEATURE_IOPORT_NF_INTP32                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP32 signal
#define BSP_FEATURE_IOPORT_NF_INTP33                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP33 signal
#define BSP_FEATURE_IOPORT_NF_INTP34                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP34 signal
#define BSP_FEATURE_IOPORT_NF_INTP35                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP35 signal
#define BSP_FEATURE_IOPORT_NF_INTP36                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP36 signal
#define BSP_FEATURE_IOPORT_NF_INTP37                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP37 signal
#define BSP_FEATURE_IOPORT_NF_INTP38                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP38 signal
#define BSP_FEATURE_IOPORT_NF_INTP39                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP39 signal
#define BSP_FEATURE_IOPORT_NF_ECM0                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ECM0 signal
#define BSP_FEATURE_IOPORT_NF_ECM1                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ECM1 signal
#define BSP_FEATURE_IOPORT_NF_ECM2                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ECM2 signal
#define BSP_FEATURE_IOPORT_NF_ECM3                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ECM3 signal
#define BSP_FEATURE_IOPORT_NF_ECM4                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ECM4 signal
#define BSP_FEATURE_IOPORT_NF_ECM5                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ECM5 signal
#define BSP_FEATURE_IOPORT_NF_ECM6                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ECM6 signal
#define BSP_FEATURE_IOPORT_NF_ECM7                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ECM7 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUJ0 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUJ1 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ2                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUJ2 signal
#define BSP_FEATURE_IOPORT_NF_TAUJ3                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUJ3 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TSG                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TSG signal
#define BSP_FEATURE_IOPORT_NF_EMU0                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter EMU0 signal
#define BSP_FEATURE_IOPORT_NF_EMU1                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter EMU1 signal
#define BSP_FEATURE_IOPORT_NF_SSIF                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter SSIF signal
#define BSP_FEATURE_IOPORT_NF_OSPI0                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter OSPI0 signal

#define BSP_FEATURE_IOPORT_LVDSA                                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSB                                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSC                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSD                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSE                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSF                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDS_CTRLA_CONFIGURATION               (0x00110000) /* PRQA S 1534 # Advisory R-2.5 */                               ///< IOPORT LVDSCTRLA configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLB_CONFIGURATION               (0x00110000) /* PRQA S 1534 # Advisory R-2.5 */                               ///< IOPORT LVDSCTRLB configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLA_ENABLE                      (0x00110039) /* PRQA S 1534 # Advisory R-2.5 */                               ///< IOPORT LVDSCTRLA enable and configuration mask register
#define BSP_FEATURE_IOPORT_LVDS_CTRLB_ENABLE                      (0x00110039) /* PRQA S 1534 # Advisory R-2.5 */                               ///< IOPORT LVDSCTRLB enable and configuration mask register

#define BSP_FEATURE_IOPORT_PST                                    (3U)                                                                          ///< Number of ECM error trigger
#define BSP_FEATURE_IOPORT_PEIODC                                 (0U)                                                                          ///< IOPORT enable/disable ERRORIN Open-drain feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_PULVSEL5                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable PULVSEL5 feature (0U): Disable/ (1U): Enable

/** OSTM */
#define BSP_FEATURE_OSTM_VALID_CHANNEL_MASK                       (0x33FU) /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Bitmask of valid OSTM channels

/** LTSC */
#define BSP_FEATURE_LTSC_CLOCK_SOURCE                             (FSP_PRIV_CLOCK_CLK_HSB) /* PRQA S 1534 # Advisory R-2.5 */                   ///< LTSC uses CLK_HSB clock as the clock source.

/** ECM */
#define BSP_FEATURE_ECM_ERROR_SOURCE_CHECK_VALID                  (287U) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Maximum ECM error source index supported (0..287)

/** ADCJ */
#if (BSP_PACKAGE_PINS_BGA156 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_ADCJ_NUM_OF_UNIT                             (2U)                                                                          ///< Number of ADCJ units (BGA156)
#else
 #define BSP_FEATURE_ADCJ_NUM_OF_UNIT                             (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of ADCJ units (others)
#endif

/** ICU */
#define BSP_FEATURE_FCLA5_IS_AVAILABLE                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FCLA5 register availability (1 = available)
#define BSP_FEATURE_ICU_SIGNAL_INTP                               (1U)                                                                          /* PRQA S 1534 # Advisory R-2.5 */

/** ETNB */
#if (BSP_FEATURE_ETNB_IS_AVAILABLE)
 #define BSP_FEATURE_ETH_AVB_NUM_CHANNELS                         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum ETH_AVB channel supported
#endif

/** ECM */
#define BSP_FEATURE_ECM_INSTANCE                                  (9U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ECM feature instance

/** CLMA */
#define BSP_FEATURE_CLMA_BACKUP_CLOCK_IS_AVAILABLE                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Backup Clock support

/** PSI5 */
#if (BSP_PACKAGE_PINS_QFP144 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_PSI5_MAX_CHANNEL                             (3U)                                                                          ///< Maximum PSI5 unit supported
#else
 #define BSP_FEATURE_PSI5_MAX_CHANNEL                             (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum PSI5 unit supported
#endif
#define BSP_FEATURE_PSI5_NUM_SLOTS_EACH_UNIT                      (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum PSI5 slot per unit supported

/** PSI5S */
#define BSP_FEATURE_PSI5S_NUM_UNIT                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum PSI5S unit supported

/** CGC */
#define BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of CPU system clock domains supported
#define BSP_FEATURE_CGC_HAS_CLK_SYS                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< System clock CLK_SYS is available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< System clock CLK_SYS_CLEAN is not available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< System clock CLK_SYS_SSCG is not available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< System clock CLK_SYS_SSCG1 is not available
#define BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Peripheral input clock source selectors
#define BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Sub oscillator is not available
#define BSP_FEATURE_CGC_HVIOSC_IS_AVAILABLE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< High Voltage Internal Oscillator is available
#define BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of repetition cases for System Clock gear up/down shifting

/** ECM ERRORIN NF */
#define BSP_FEATURE_ECM_ERRORIN_NF_MAX                            IOPORT_FILTER_SIGNAL_ERRORIN3 /* PRQA S 1534 # Advisory R-2.5 */              ///< ECM ERRORIN NF Max
#define BSP_FEATURE_ECM_ERRORIN_NF_0                              IOPORT_FILTER_SIGNAL_ERRORIN0 /* PRQA S 1534 # Advisory R-2.5 */              ///< ECM ERRORIN NF0

/** DMAC */
#define BSP_FEATURE_DMAC_MAX_CHANNEL                              (16U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Maximum DMAC channels
#define BSP_FEATURE_DMAC_MAX_UNIT                                 (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum DMAC units

/** LPS */
#define BSP_FEATURE_LPS_PIN_RH850U2Ax                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS unsupport DPIN13-DSELDP
#define BSP_FEATURE_LPS_PIN0_RH850U2Ax                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS unsupport DPIN3 - DPIN10

/** GTM_ATOM */
#define BSP_FEATURE_GTM_ATOM_HRES_MODE_SUPPORT                    (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< High-Resolution PWM Support

/** IPI */
#define BSP_FEATURE_IPI_VALID_PE                                  (0x03U) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< IPI valid PE mask
#define BSP_FEATURE_IPI_VALID_REQ                                 (0x0003U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< IPI valid interrupt request mask

/** FLASH */
#define BSP_FEATURE_FLASH_CF_WRITE_SIZE                           (512U) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Code flash minimum write size in bytes
#define BSP_FEATURE_FLASH_CF_REGION0_BLOCK_SIZE                   (0x4000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Small CF block size: 16 KB
#define BSP_FEATURE_FLASH_CF_REGION1_BLOCK_SIZE                   (0x10000U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< Large CF block size: 64 KB
#define BSP_FEATURE_FLASH_CF_REGION0_SIZE                         (0x00020000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Region 0 total size: 128 KB (8 x 16 KB)
#define BSP_FEATURE_FLASH_CF_BLANK_CHECK_AREA_START               (0x0C000000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Code Flash Blank Check Area base address

#define BSP_FEATURE_FLASH_CF_BANK_A_START                         (0x00000000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Bank A start address
#define BSP_FEATURE_FLASH_CF_BANK_A_SIZE                          (0x00300000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Bank A size: 3 MB (U2A6)
#define BSP_FEATURE_FLASH_CF_BANK_B_START                         (0x00400000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Bank B start address
#define BSP_FEATURE_FLASH_CF_BANK_B_SIZE                          (0x00300000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Bank B size: 3 MB (U2A6)

#define BSP_FEATURE_FLASH_CODE_FLASH_START                        (0x00000000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Code flash start address
#define BSP_FEATURE_FLASH_DF_BLOCK_SIZE                           (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Data flash block (erase unit): 4 KB
#define BSP_FEATURE_FLASH_DF_WRITE_SIZE                           (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Data flash minimum write size in bytes

#define BSP_FEATURE_FLASH_DATA_FLASH_AREA0_START                  (0xFF200000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Data Area 0 start (FPSYS0)
#define BSP_FEATURE_FLASH_DATA_FLASH_AREA0_SIZE                   (0x20000U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< Data Area 0 size: 128 KB
#define BSP_FEATURE_FLASH_DATA_FLASH_AREA1_START                  (0xFF220000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Data Area 1 start (FPSYS1)
#define BSP_FEATURE_FLASH_DATA_FLASH_AREA1_SIZE                   (0x10000U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< Data Area 1 size: 64 KB
#define BSP_FEATURE_FLASH_DATA_FLASH_AREA2_START                  (0xFF240000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Data Area 2 start (FPSYS2) - ICUMB unit only
#define BSP_FEATURE_FLASH_DATA_FLASH_AREA2_SIZE                   (0x10000U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< Data Area 2 size: 64 KB - ICUMB unit only
#define BSP_FEATURE_FLASH_DATA_FLASH_START                        (BSP_FEATURE_FLASH_DATA_FLASH_AREA0_START) /* PRQA S 1534 # Advisory R-2.5 */ ///< DF start address

/* PRQA S 1534 ++ # Advisory R-2.5 */
#define BSP_DATA_FLASH_SIZE_BYTES                                 (BSP_FEATURE_FLASH_DATA_FLASH_AREA0_SIZE + \
                                                                   BSP_FEATURE_FLASH_DATA_FLASH_AREA1_SIZE)                                     ///< CPU-accessible DF: 192 KB

/* PRQA S 1534 -- */
#define BSP_FEATURE_FLASH_CF_FACI1_BANK_START                     (BSP_FEATURE_FLASH_CF_BANK_B_START) /* PRQA S 1534 # Advisory R-2.5 */        ///< FACI1 bank start address
#define BSP_FEATURE_RHSB_VALID_CHANNEL_MASK                       (0x00000000UL) /* PRQA S 1534 # Advisory R-2.5 */                             ///< RHSB implemented-unit mask: no RHSB unit on this device

/** SCI3 */
#define BSP_FEATURE_SCI3_CHANNEL_NUM                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of SCI3 channels available

#define BSP_FEATURE_R_ENCA0_TYPE_USED                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< R_ENCA0_Type is used

#define BSP_FEATURE_IOPORT_NF_TAUD2_USE_TAUD_REG                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Indicates whether TAUD2 noise filter uses the TAUD-specific control register.
#define BSP_FEATURE_IOPORT_NF_TAUJ_FCLA_AVAILABLE                 (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Indicates whether TAUJ noise filter has the TAUJ-specific FCLACTL edge detector register.

#define BSP_FEATURE_TPTM_COMPARISON_STATUS                        (0) /* PRQA S 1534 # Advisory R-2.5 */                                        ///< Comparison status is not available

#define BSP_FEATURE_TPTM_INTERRUPT_UP_TIMER                       (0) /* PRQA S 1534 # Advisory R-2.5 */                                        ///< Interrupt up timer is not available

#define BSP_FEATURE_ADCJ_HWIP_SUPPORT                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCJ peripheral support
#define BSP_FEATURE_ADCK_HWIP_SUPPORT                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCK peripheral support

#define BSP_FEATURE_MOTOR_SUPPORTED_VARIANT                       (0x0001U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 hex presents 1 MCU variant using alphabet order
#define BSP_FEATURE_MOTOR_IOPORT_PIN_NUMBER                       (BSP_PACKAGE_PINS) /* PRQA S 1534 # Advisory R-2.5 */                         ///< Pin profile number
#if (BSP_PACKAGE_PINS_BGA292 > BSP_PACKAGE_PINS)
 #define BSP_FEATURE_MOTOR_TAUD_THREE_PHASE_SUPPORT               (0U)                                                                          ///< Taud three phase feature support
#else
 #define BSP_FEATURE_MOTOR_TAUD_THREE_PHASE_SUPPORT               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Taud three phase feature support
#endif

/** TAPA */
#define BSP_FEATURE_TAPA_SUPPORTED_VARIANT                        (0x0001U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order

/** PWM_DIAG */
#define BSP_FEATURE_PWM_DIAG_SUPPORTED_VARIANT                    (0x0001U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order

/** OSTM */
#define BSP_FEATURE_OSTM_SIMPLE_CHANNEL_LAYOUT_AVAILABLE          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< OSTM simple channel layout is not available
#define BSP_FEATURE_OSTM_EXTENDED_CHANNEL_LAYOUT_AVAILABLE        (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< OSTM extended channel layout is available

/** PIC */
#define BSP_FEATURE_PIC_SUPPORTED_VARIANT                         (0x0001U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order
#define BSP_FEATURE_PIC_AVAILABLE                                 (0x0001U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order

/** LPS */
#define BSP_FEATURE_LPS_SUPPORTED_VARIANT                         (0x0001U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order
#define BSP_FEATURE_LPS_ADCJ_SUPPORT                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS support pin for U2A

/** LPS */
#define BSP_FEATURE_LPS_ANALOG_MODE_SUPPORT                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS analog mode support
#define BSP_FEATURE_LPS_DIGITAL_MODE_SUPPORT                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS digital mode support
#define BSP_FEATURE_LPS_DIGITAL_INPUT_SUPPORT                     (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS digital input mode unsupport

/** GTM */
#define BSP_FEATURE_GTM_INTSEL_SUPPORT                            (0U)                                                                          ///< Interconnections of Interrupt Factors for INTC Support
#define BSP_FEATURE_GTM_INTSEL_NUMBER                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of interrupt selection control for INTC registers
#define BSP_FEATURE_GTM_DMASEL_SUPPORT                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Interconnections of Interrupt Factors for DMA/DTS Support
#define BSP_FEATURE_GTM_DMASEL_NUMBER                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of interrupt selection control for DMA/DTS registers
#define BSP_FEATURE_GTM_IP_GEN_3                                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM_IP generation 3 in used
#define BSP_FEATURE_GTM_IP_GEN_4                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM_IP generation 4 in used
#define BSP_FEATURE_GTM_ADCI_RDC3A_SUPPORT                        (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Stored RDC3A data in ADC interface support
#define BSP_FEATURE_GTM_ADCI_EMU3S_DFE_SUPPORT                    (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Stored DFE or EMU3S data in ADC interface support

/** GTM_DTM */
#define BSP_FEATURE_GTM_DTM_HRES_MODE_SUPPORT                     (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< High-Resolution PWM Support

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU_RH850U2A6) */

#endif                                 /* BSP_FEATURE_H */
