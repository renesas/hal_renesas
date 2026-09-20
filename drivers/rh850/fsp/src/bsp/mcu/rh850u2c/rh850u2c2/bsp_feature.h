/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2CX
 * @defgroup BSP_MCU_RH850U2C2 BSP MCU U2C2
 * @brief This module provides the macros, data types, and definition constants required for U2C2 feature.
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

#ifndef BSP_FEATURE_H
#define BSP_FEATURE_H

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2C2
 * @defgroup BSP_FEATURE_U2C2 BSP U2C2 Specific Feature
 * @brief This file defines all feature-related macros for the U2C2 device, including module availability and
 * hardware capability constants.
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_PACKAGE_PINS_QFP100                                   (100U)                                                                        ///< Package pin count: QFP100
#define BSP_PACKAGE_PINS_QFP144                                   (144U) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Package pin count: QFP144

/** Barrier Synchronization */
/** PE mask bit that is used in R_BSP_PESyncEnable */
#define BSP_BARRIER_SYNC_CPU0                                     (1U << 0U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< PE mask bit for CPU0

/** PE value that is used in R_BSP_PESyncComplete */
#define BSP_CPU0                                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CPU identifier for CPU0

/** BSP FEATURE */
#define BSP_FEATURE_FEINT_CPU0                                    (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FEINT feature available on CPU0

/** U2C2 */
#define BSP_FEATURE_FLXA_IS_AVAILABLE                             (0x00U)                                                                       ///< FLXA availability (0 = not available)
#define BSP_FEATURE_ETND_IS_AVAILABLE                             (0x00U)                                                                       ///< ETND feature availability (0 = not available)
#define BSP_FEATURE_ETNF_IS_AVAILABLE                             (0x00U) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< ETNF module not available
#define BSP_FEATURE_MMCA_IS_AVAILABLE                             (0x00U)                                                                       ///< MMC-A availability (0 = not available)
#define BSP_FEATURE_PSI5S_IS_AVAILABLE                            (0x00U)                                                                       ///< PSI5S module availability (0 = not available)
#define BSP_FEATURE_CANXL_IS_AVAILABLE                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CAN XL available
#define BSP_FEATURE_CANXL_CH1_IS_AVAILABLE                        (0x00U)                                                                       ///< CAN XL channel 1 availability (0 = not available)
#define BSP_FEATURE_DFP_IS_AVAILABLE                              (0x00U)                                                                       ///< DFP feature availability (0 = not available)
#define BSP_FEATURE_DFA_IS_AVAILABLE                              (0x00U)                                                                       ///< DFA availability (0 = not available)
#define BSP_FEATURE_CLMA_STOP_DETECTION_IS_AVAILABLE              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Clock Stop Detection Mode support
#define BSP_FEATURE_BSP_PRODUCT_NUMBER_POINTER                    (0x08030040U)                                                                 ///< Address of the Product number register.
#define BSP_FEATURE_BSP_PRODUCT_NAME_POINTER                      (0x080300A0U)                                                                 ///< Address of the Product name register.
#define BSP_FEATURE_BSP_UNIQUE_ID_POINTER                         (0x08031000U)                                                                 ///< Address of the MCU Unique ID register.
#define BSP_FEATURE_FPU_IMPRECISE_EXCEPTION_IS_AVAILABLE          (1U)                                                                          ///< FPU imprecise exception is available
#define BSP_FEATURE_FPEC_REGISTER_IS_AVAILABLE                    (1U)                                                                          ///< FPEC system register is available
#define BSP_IO_PORT_TYPE_MAX                                      (3U)                                                                          ///< IO Port Type max value

/** MODULE RESET */
#define BSP_RESET_SOURCE_POWER_OFF_STANDBY_IS_AVAILABLE           (0x00U)                                                                       ///< Power-off standby reset not supported
#define BSP_RESET_ECM_SELECT_RESET_SOURCE_IS_AVAILABLE            (0x00U)                                                                       ///< ECM-selectable reset source not supported
#define BSP_RESET_MODULE_RSCFD                                    RSCFD /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Module reset name: RSCFD
#define BSP_RESET_MODULE_GTM                                      GTM /* PRQA S 1534 # Advisory R-2.5 */                                        ///< Module reset name: GTM
#define BSP_RESET_MODULE_RSENT                                    RSENT /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Module reset name: RSENT
#define BSP_RESET_MODULE_MSPI                                     MSPI /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: MSPI
#define BSP_RESET_MODULE_RLIN3                                    RLIN3 /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Module reset name: RLIN3
#define BSP_RESET_MODULE_ADCK_ISO                                 ADCK_ISO /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Module reset name: ADCK_ISO
#define BSP_RESET_MODULE_CXPI                                     CXPI /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: CXPI
#define BSP_RESET_MODULE_ENCA                                     ENCA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: ENCA
#define BSP_RESET_MODULE_PSI5                                     PSI5 /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: PSI5
#define BSP_RESET_MODULE_PWMD                                     PWMD /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: PWMD
#define BSP_RESET_MODULE_SFMA                                     SFMA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: SFMA
#define BSP_RESET_MODULE_TAPA                                     TAPA /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: TAPA
#define BSP_RESET_MODULE_TAUD                                     TAUD /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: TAUD
#define BSP_RESET_MODULE_TAUJ_ISO                                 TAUJ_ISO /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Module reset name: TAUJ_ISO
#define BSP_RESET_MODULE_TSG3                                     TSG3 /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: TSG3
#define BSP_RESET_MODULE_OSTM                                     OSTM /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: OSTM
#define BSP_RESET_MODULE_PE                                       PE /* PRQA S 1534 # Advisory R-2.5 */                                         ///< Module reset name: PE
#define BSP_RESET_MODULE_WDTB_ISO                                 WDTB_ISO /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Module reset name: WDTB_ISO
#define BSP_RESET_MODULE_CANXL                                    CANXL /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Module reset name: CANXL
#define BSP_RESET_MODULE_RI3C                                     RI3C /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: RI3C
#define BSP_RESET_MODULE_SSIF                                     SSIF /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: SSIF
#define BSP_RESET_MODULE_ETNF                                     ETNF /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Module reset name: ETNF

/** STANDBY MODE */
#define BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_IS_AVAILABLE            (0x01U)                                                                       ///< DeepStop mode availability
#define BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_NUM_OF_WAKEUP_SOURCE    (0x06U)                                                                       ///< Number of DeepStop wakeup sources
#define BSP_FEATURE_CHIP_STANDBY_IOHOLD_REG_NAME                  (0x02U)                                                                       ///< IOHOLD register index

/** OPBT */
#define BSP_FEATURE_OPBT_BVA1_AREA_IS_AVAILABLE                   (0x00U)                                                                       ///< BVA1 Area is not available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_1_AVAILABLE            (0x00U)                                                                       ///< OPBT Cluster 1 switching is not available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_2_AVAILABLE            (0x00U)                                                                       ///< OPBT Cluster 2 switching is not available
#define BSP_FEATURE_OPBT_SWITCHING_CLUSTER_3_AVAILABLE            (0x00U)                                                                       ///< OPBT Cluster 3 switching is not available

/** GUARD */
#define BSP_FEATURE_PEGCAP_M_IS_AVAILABLE                         (0x00U)                                                                       ///< PEGCAP_M guard availability (0 = not available)
#define BSP_FEATURE_PE1_GUARD_IS_AVAILABLE                        (0U)                                                                          ///< PE1 guard not available
#define BSP_FEATURE_CRAM00_GUARD_IS_AVAILABLE                     (1U)                                                                          ///< CRAM00 guard available
#define BSP_FEATURE_CRAM01_GUARD_IS_AVAILABLE                     (0U)                                                                          ///< CRAM01 guard not available
#define BSP_FEATURE_CRAM02_GUARD_IS_AVAILABLE                     (0U)                                                                          ///< CRAM02 guard not available
#define BSP_FEATURE_CRAM_GUARD_STATUS_MAX                         (2U)                                                                          ///< Maximum CRAM guard status index
#define BSP_FEATURE_PE_GUARD_STATUS_MAX                           (1U)                                                                          ///< Maximum PE guard status index
#define BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE                       (0U)                                                                          ///< IPIR guard not available
#define BSP_FEATURE_BARR_GUARD_IS_AVAILABLE                       (0U)                                                                          ///< Barrier guard not available
#define BSP_FEATURE_TPTMn_GUARD_MAX_CHANNEL                       (1U)                                                                          ///< Max TPTMn guardable channels
#define BSP_FEATURE_INTC2_GUARD_MAX_CHANNEL                       (527U)                                                                        ///< Max INTC2 guardable channel
#define BSP_FEATURE_INTC2_GUARD_MIN_CHANNEL                       (32U)                                                                         ///< Min INTC2 guardable channel
#define BSP_FEATURE_DTS_GUARD_MAX_CHANNEL                         (128U)                                                                        ///< Max DTS guard channels
#define BSP_FEATURE_DMAC_GUARD_MAX_CHANNEL                        (16U)                                                                         ///< Max DMAC guard channels

/** QOS */
#define BSP_FEATURE_QOS_IS_AVAILABLE                              (0U)                                                                          ///< QoS feature is not available on this device

/** INTSEL */
#define BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ                         (0U)                                                                          ///< INTSEL IRQ selection is not supported

/** INT MASK */
#define BSP_FEATURE_DEVICE_HAS_ORED_IRQ                           (1U)                                                                          ///< ORed IRQ support available
#define BSP_FEATURE_VIRQ_MAX_ENTRIES                              (672UL)                                                                       ///< Maximum virtual IRQ entries

/** CANFD */
#if (BSP_PACKAGE_PINS_QFP100 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS                           (4UL)                                                                         ///< Number of CANFD channels (QFP100)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT0                     (4UL)                                                                         ///< Number of CAN FD Channels in Unit 0
#else
 #define BSP_FEATURE_CANFD_NUM_CHANNELS                           (6UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CANFD channels (QFP144)
 #define BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT0                     (6UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CAN FD Channels in Unit 0
#endif

#define BSP_FEATURE_CANFD_NUM_UNIT                                (1UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CANFD units

#define BSP_FEATURE_CANFD_NUM_TXQUEUE_CHANNEL                     (4UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CANFD TXQUEUE channels
#define BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL                       (3UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CANFD CFIFO channels
#define BSP_FEATURE_CANFD_NUM_RXFIFO                              (8UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CANFD RX FIFOs
#define BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT                      (24UL) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Total CFIFO count on MCU
#define BSP_FEATURE_CANFD_TXMB_SUPPORTED_SLOT                     (0x0000FFFF0000FFFFULL) /* PRQA S 1534 # Advisory R-2.5 */                    ///< Supported TXMB slots (0-15 & 32-47)
#define BSP_FEATURE_CANFD_TXMB_MAX_NUM                            (32UL) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Maximum TXMB count
#define BSP_FEATURE_CANFD_CLKC_SRC                                (FSP_PRIV_CLOCK_CLK_CANFD_C) /* PRQA S 1534 # Advisory R-2.5 */               ///< CANFD CLKC source
#define BSP_FEATURE_CANFD_PN_MODE                                 (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Pretended Network mode supported
#define BSP_FEATURE_CANFD_FFI_MODE                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FFI mode supported
#define BSP_FEATURE_CANFD_NUM_RX_FIFO_FFI_VM                      (8UL) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of RX FIFO for FFI/VM
#define BSP_FEATURE_CANFD_AFLMRAM_MAX_PAGE                        (305UL) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Number of AFL MRAM page
#define BSP_FEATURE_CANFD_AFLMRAM_PER_CHANNEL                     (9760UL) /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Number of AFL MRAM area size
#define BSP_FEATURE_CANFD_PFLRAM_MAX_PAGE                         (22UL) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Number of PFL page
#define BSP_FEATURE_CANFD_PFLRAM_PER_CHANNEL                      (720UL) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Number of PFL MRAM area size

#define BSP_FEATURE_FREERTOS_PORT_OSTM0_EIC_ADDRESS               (0xFFF8007E) /* PRQA S 1534 # Advisory R-2.5 */                               ///< OSTM0 EI level interrupt control register address
#define BSP_FEATURE_FREERTOS_PORT_OSTM0_CMP_ADDRESS               (0xFFBF0000) /* PRQA S 1534 # Advisory R-2.5 */                               ///< OSTM0 compare register address
#define BSP_FEATURE_FREERTOS_PORT_OSTM0_CTL_ADDRESS               (0xFFBF0020) /* PRQA S 1534 # Advisory R-2.5 */                               ///< OSTM0 control register address
#define BSP_FEATURE_FREERTOS_PORT_OSTM0_TS_ADDRESS                (0xFFBF0014) /* PRQA S 1534 # Advisory R-2.5 */                               ///< OSTM0 count start trigger register address

/** TAUD */
#define BSP_FEATURE_TAUD_MAX_UNIT                                 (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum TAUD units

/** TAUJ */
#define BSP_FEATURE_TAUJ_FOUR_UNITS_V1_AVAILABLE                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUJ four units v1 are available
#define BSP_FEATURE_TAUJ_SINGLE_UNIT_AVAILABLE                    (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUJ single unit is available
#define BSP_FEATURE_TAUJ_FOUR_UNITS_V2_AVAILABLE                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUJ four units v2 are available

/** MSPI */
#define BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT                   (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum chip selects per MSPI unit
#define BSP_FEATURE_MSPI_TRIG_TPTM_NUMBER_UNIT                    (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of TPTM units usable as an MSPI hardware trigger source
#define BSP_FEATURE_MSPI_DMA_ALT_SPECIAL_MAPPING_SUPPORT          (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Special DMA ALT mapping is supported
#define BSP_FEATURE_MSPI_DTS_ALT_SPECIAL_MAPPING_SUPPORT          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Special DTS ALT mapping is not supported
#if (BSP_PACKAGE_PINS_QFP100 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_MSPI_MAX_UNIT                                (6U)                                                                          ///< Number of MSPI units (QFP100)
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_0               (8U)                                                                          ///< MSPI unit 0 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_1               (8U)                                                                          ///< MSPI unit 1 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_2               (8U)                                                                          ///< MSPI unit 2 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_3               (8U)                                                                          ///< MSPI unit 3 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_4               (4U)                                                                          ///< MSPI unit 4 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_5               (4U)                                                                          ///< MSPI unit 5 chip selects
 #define BSP_FEATURE_ATU_GTM_SEL                                  (0)                                                                           ///< ATU GTM trigger select bit
#else
 #define BSP_FEATURE_MSPI_MAX_UNIT                                (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of MSPI units (QFP144)
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_0               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI unit 0 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_1               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI unit 1 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_2               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI unit 2 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_3               (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI unit 3 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_4               (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI unit 4 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_5               (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI unit 5 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_6               (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI unit 6 chip selects
 #define BSP_FEATURE_MSPI_NUMBER_CHIP_SELECT_UNIT_7               (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI unit 7 chip selects
 #define BSP_FEATURE_ATU_GTM_SEL                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ATU GTM trigger select bit
#endif

#define BSP_FEATURE_MSPI_ALT_DMA_UNIT                             (0xCCU) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< ALT DMA unit bitmask
#define BSP_FEATURE_MSPI_ALT_DTS_UNIT                             (0xF0U) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< ALT DTS unit bitmask
#define BSP_FEATURE_MSPI_ALT_DMA_MAX_UNIT                         (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Max ALT DMA-capable units
#define BSP_FEATURE_MSPI_ALT_DTS_MAX_UNIT                         (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Max ALT DTS-capable units
#define BSP_FEATURE_MSPI_ALT_GAP_CHANNEL                          (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Channel gap for ALT
#define BSP_FEATURE_MSPI_ALT_GAP_UNIT                             (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Unit gap for ALT
#define BSP_FEATURE_MSPI_ALT_PAIR_CHANNEL                         (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ALT channel pairing
#define BSP_FEATURE_MSPI_ALT_MAX_CHANNEL                          (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum ALT-supported channel index
#define BSP_FEATURE_MSPI_ALT_DMA_MAX_BIT                          (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Highest ALT DMA register bit
#define BSP_FEATURE_MSPI_ALT_DTS_MAX_BIT                          (13U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Highest ALT DTS register bit
#define BSP_FEATURE_DMAAS23                                       (9U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Bit position ALT DMA unit2, ch3
#define BSP_FEATURE_DMAAS21                                       (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Bit position ALT DMA unit2, ch1

/** I3C */
#define BSP_FEATURE_I3C_MAX_DEV_COUNT                             (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum I3C devices
#define BSP_FEATURE_I3C_MSTP_OFFSET                               (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< I3C MSTP register offset
#define BSP_FEATURE_I3C_NTDTBP0_DEPTH                             (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< NTDTBP0 FIFO depth
#define BSP_FEATURE_I3C_NUM_CHANNELS                              (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of I3C channels
#define BSP_FEATURE_I3C_HAS_HDR                                   (0U)                                                                          /* PRQA S 1534 # Advisory R-2.5 */
#define BSP_FEATURE_I3C_HAS_BCTL_BMDS                             (0U)                                                                          /* PRQA S 1534 # Advisory R-2.5 */

#define BSP_FEATURE_IIC_B_VALID_CHANNEL_MASK                      (0x07UL) /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Valid IIC-B channel mask
#define BSP_FEATURE_IIC_B_FAST_MODE_PLUS                          (0x00UL) /* PRQA S 1534 # Advisory R-2.5 */                                   ///< Fast Mode Plus not supported

/** RSENT */
#define BSP_FEATURE_RSENT_SENT_NUM_OF_UNIT                        (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of RSENT units
#define BSP_FEATURE_RSENT_SENT_SPCO_SUPPORT                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RSENT SPCO select function is supported
#define BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD                     (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RSENT unit index threshold between MSSEL register groups
#define BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS              (5U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of bits per unit in the RSENT MSSEL register
#define BSP_FEATURE_RSENT_SENT_HAS_EXTENDED_TSSEL                 (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RSENT does not have extended TSSEL2-TSSEL7 registers

/** RLIN3 */
#define BSP_FEATURE_RLIN3_RLIN323_HAS_PBUS_GROUP_8L               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RLIN323 channel is on PBUS group 8L

/** MSPI */
#define BSP_FEATURE_MSPI_RAM_WORDS_MAX                            (512U) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< MSPI RAM size

/** TAUD3P */
#define BSP_FEATURE_TAUD_THREE_PHASE_NUMBER_OF_UNIT               (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD 3-phase units

/** PIC */
#define BSP_FEATURE_PIC_ADC_NUMBER_UNIT                           (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PIC ADC units
#define BSP_FEATURE_PIC_ADC_NUMBER_SG                             (6U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PIC ADC scan groups
#define BSP_FEATURE_PIC_TAUDTINSEL_NUMBER                         (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUDTINSEL signals
#define BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT                        (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD units in PIC20
#define BSP_FEATURE_PIC21_TAUD_NUMBER_UNIT                        (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD units in PIC21
#define BSP_FEATURE_PIC_TSG3_NUMBER_UNIT                          (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG3 units
#define BSP_FEATURE_PIC_ADTCFG_NUMBER                             (10U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< ADTCFG entries
#define BSP_FEATURE_PIC_PSI5S_NUMBER_UNIT                         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S units
#define BSP_FEATURE_PIC_PSI5SEN_NUMBER                            (7U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5SEN entries
#define BSP_FEATURE_PIC_PSI5EN_NUMBER                             (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5EN entries
#define BSP_FEATURE_PIC_PSI5EN_NUMBER_UNIT                        (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5EN units
#define BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP                      (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTMINEN groups
#define BSP_FEATURE_PIC_CANIOSEN_NUMBER                           (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CANIOSEN entries
#define BSP_FEATURE_PIC_GTMINTIOEN_NUMBER_GOURP                   (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM INTIO enable groups
#define BSP_FEATURE_PIC_GTMMSPI_NUMBER                            (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM-MSPI links
#define BSP_FEATURE_PIC_TSG3TRG_NUMBER_UNIT                       (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG3 trigger units
#define BSP_FEATURE_PIC_TSG3TRG_NUMBER_TRIGGER                    (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG3 trigger signals
#define BSP_FEATURE_PIC_GTMINTIOEN_NUMBER_GROUP                   (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM GTMINTIOEN enable groups

/** FLXA */
#define BSP_FEATURE_FLXA_MAX_UNIT                                 (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FLXA not available

/** CRC */
#define BSP_FEATURE_CRC_COMMON_GROUP                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CRC common group supported

/** PIC - additional TAUD functions */
#define BSP_FEATURE_PIC_TAUD_INT_SIGNAL_OUT_NUMBER                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD interrupt output signals
#define BSP_FEATURE_PIC_TAUD_TRIANGLE_WAVE_NUMBER                 (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD triangle-wave outputs
#define BSP_FEATURE_PIC_TAUD_PWM_OUTPUT_NUMBER                    (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD PWM outputs
#define BSP_FEATURE_PIC_TAUD_DELAY_PULSE_NUMBER                   (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD delay pulse outputs

#define BSP_FEATURE_PIC_ENCA_FEATURE_NUMBER_UNIT                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA-capable PIC units
#define BSP_FEATURE_PIC_ADCK_TRIG_SEL                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCK trigger selection supported
#define BSP_FEATURE_PIC_SWITCH_TSG_OUT                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG output switch supported
#define BSP_FEATURE_PIC_SWITCH_TSG_OUT_TSG32                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG32 switch not supported
#define BSP_FEATURE_PIC_EMU_FEATURE                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< EMU feature not supported
#define BSP_FEATURE_PIC_TSG_START_TRIG                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG start trigger supported
#define BSP_FEATURE_PIC_TSG_START_TRIG_TSG32                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG32 start trigger not supported
#define BSP_FEATURE_PIC_3PHASE_ENCODER_TSG31                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< 3-phase encoder on TSG31 not supported
#define BSP_FEATURE_PIC_2PHASE_SEL_INTENCA1_TO_TSG3               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< 2-phase ENCA1 selection to TSG3 not supported
#define BSP_FEATURE_PIC_ENC_CAPT_TRIG_ENCA1                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA1 capture trigger not supported
#define BSP_FEATURE_PIC_INT_SIG_OUT_TAUD1                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD1 interrupt output not supported
#define BSP_FEATURE_PIC_INT_SIG_OUT_TAUD2                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD2 interrupt output not supported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT12             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< DT12 pulse width measurement not supported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT00             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< DT00 pulse width measurement supported
#define BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT10             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< DT10 pulse width measurement not supported
#define BSP_FEATURE_PIC_ENCA_INPUT_SEL                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA input selection not supported
#define BSP_FEATURE_PIC_FCMP_FEATURE                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FCMP feature not supported
#define BSP_FEATURE_PIC_HRPWM_FEATURE                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< HRPWM feature not supported
#define BSP_FEATURE_PIC_TIMER_INPUT_MONITOR                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Timer input monitor not supported
#define BSP_FEATURE_PIC_TAUD_INPUT_SEL                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD input selection supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA trigger selection supported
#define BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL                        (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCA encoder input selection not supported
#define BSP_FEATURE_PIC_RDC_FEATURE                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< RDC feature not supported
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE                          (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CAN/GTM IO sharing supported
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI trigger select supported
#define BSP_FEATURE_PIC_GTM_DTM_INPUT_SEL                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM DTM input selection not supported
#define BSP_FEATURE_PIC_TSG3_INPUT_GTM                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG3 GTM input supported
#define BSP_FEATURE_PIC_ADCK_TRIG_SEL_GTM_PIC24                   (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCK via PIC24 not supported
#define BSP_FEATURE_PIC_ADCK_TRIG_SEL_GTM_PIC2_SELB               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCK via PIC2 SELB supported
#define BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC13                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG sync via PIC13 not supported
#define BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC1_SELB            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TSG sync via PIC1 SELB supported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD2                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TIMER IN SEL B -> TAUD2 supported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD_TAUJ                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TIMER IN SEL B -> TAUD/TAUJ not supported
#define BSP_FEATURE_PIC_TIMER_IN_SEL_B                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TIMER IN SEL B supported
#define BSP_FEATURE_PIC_TAUD_IN_SEL_PIC13                         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD input via PIC13 supported
#define BSP_FEATURE_PIC_TAUD_IN_SEL_PIC1_SELB                     (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD input via PIC1 SELB not supported
#define BSP_FEATURE_PIC_TIMER_OUT_MON_PIC13                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Timer output monitor via PIC13 not supported
#define BSP_FEATURE_PIC_TIMER_OUT_MON_PIC1_SELB                   (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Timer output monitor via PIC1 SELB supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC22              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATSEL via PIC22 supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC24              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATCFG via PIC24 not supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC2_SELB          (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATCFG via PIC2 SELB supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC21              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATSEL via PIC21 not supported
#define BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC22              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ENCATCFG via PIC22 not supported
#define BSP_FEATURE_PIC_PSI5_TRIG_SEL                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5 trigger selection supported
#define BSP_FEATURE_PIC_PSI5_TRIG_SEL_GTM_PIC24                   (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5 trigger via GTM PIC24 unsupported
#define BSP_FEATURE_PIC_PSI5_TRIG_SEL_GTM_PIC2_SELB               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5 trigger via GTM PIC2 SELB supported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC24                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S trigger via GTM PIC24 unsupported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC2_SELB              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S trigger via GTM PIC2 SELB supported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC22                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S trigger via GTM PIC22 unsupported
#define BSP_FEATURE_PIC_PSI5S_TRIG_SEL                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PSI5S trigger selection supported
#define BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL                         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIO input selection supported
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC24                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIO input via PIC24 unsupported
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC2_SELB                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIO input via PIC2 SELB supported
#define BSP_FEATURE_PIC_GTM_TIO_IN_PIC22                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIO input via PIC22 unsupported
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC24                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIM input PIC24 not supported
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC2_SELB                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIM input PIC2 SELB supported
#define BSP_FEATURE_PIC_GTM_TIM_IN_PIC22                          (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM TIM input PIC22 not supported
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC24                        (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Hi-Z control via PIC24 not supported
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC2_SELB                    (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Hi-Z control via PIC2 SELB supported
#define BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC20                        (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Hi-Z control via PIC20 not supported
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_PIC24                    (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CAN/GTM IO share via PIC24 not supported
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_PIC2_SELB                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CAN/GTM IO share via PIC2 SELB supported
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL_PIC24                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI trigger via PIC24 not supported
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL_PIC2_SELB                   (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< MSPI trigger via PIC2 SELB supported
#define BSP_FEATURE_PIC_TAUD_INPUT_SEL_PIC13                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD input selection via PIC13 not supported
#define BSP_FEATURE_PIC_TAUD_INPUT_SEL_PIC1_SELB                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< TAUD input selection via PIC1 SELB supported
#define BSP_FEATURE_PIC_ADCK_TRIG_SEL_PIC21                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCK trigger via PIC21 supported
#define BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST0                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Single SST0 on PIC1 supported
#define BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN0                          (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< HIZCEN0 supported
#define BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_CAN_NUMBER               (14U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of CAN/GTM shared IO signals
#define BSP_FEATURE_PIC_MSPI_TRIG_SEL_SIGNAL_NUMBER               (3U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of MSPI trigger signals supported
#define BSP_FEATURE_PIC_FEATURE_U2CX                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< PIC U2C2 feature set supported

/** DTSC */
#define BSP_FEATURE_DTSC_HW_TRANSFER_REQUEST                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< DTSC HW transfer request supported

/** PWM DIAG */
#define BSP_FEATURE_PWM_DIAG_NUM_OF_CHANNEL                       (96U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of PWM diagnostic channels

/** GTM */
#define BSP_FEATURE_GTM_ARU_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM ARU instances
#define BSP_FEATURE_GTM_BRC_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM BRC instances
#define BSP_FEATURE_GTM_PSM_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM PSM instances
#define BSP_FEATURE_GTM_CMU_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM CMU instances
#define BSP_FEATURE_GTM_CCM_NUMBER_INSTANCE                       (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM CCM instances
#define BSP_FEATURE_GTM_TBU_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM TBU instances
#define BSP_FEATURE_GTM_TIM_NUMBER_INSTANCE                       (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM TIM instances
#define BSP_FEATURE_GTM_TOM_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM TOM instances
#define BSP_FEATURE_GTM_ATOM_NUMBER_INSTANCE                      (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM ATOM instances
#define BSP_FEATURE_GTM_DTM_NUMBER_INSTANCE                       (6U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM DTM instances
#define BSP_FEATURE_GTM_MCS_NUMBER_INSTANCE                       (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCS instances
#define BSP_FEATURE_GTM_MCFG_NUMBER_INSTANCE                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCFG instances
#define BSP_FEATURE_GTM_MAP_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MAP instances
#define BSP_FEATURE_GTM_DPLL_NUMBER_INSTANCE                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM DPLL instances
#define BSP_FEATURE_GTM_SPE_NUMBER_INSTANCE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM SPE instances
#define BSP_FEATURE_GTM_ICM_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM ICM instances
#define BSP_FEATURE_GTM_CMP_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM CMP instances
#define BSP_FEATURE_GTM_MON_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MON instances
#define BSP_FEATURE_GTM_TIO_NUMBER_INSTANCE                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM TIO instances
#define BSP_FEATURE_GTM_INTSEL_SUPPORT                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Interconnections of Interrupt Factors Support

/** TAUD */
#define BSP_FEATURE_TAUD_MAX_UNIT                                 (3U)                                                                          ///< Maximum TAUD units

/** CLMA */
#define BSP_FEATURE_CLMA_MAX_CHANNEL                              (9U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum CLMA channels

/** VMON */
#define BSP_FEATURE_VMON_VDD2_AVAILABLE                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< VDD2 monitoring group
#define BSP_FEATURE_VMON_AWOVDD_AVAILABLE                         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< AWOVDD monitoring group
#define BSP_FEATURE_VMON_FC_ENABLE                                (0xFFU) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Bit positions for enabling VMON functionality
#define BSP_FEATURE_VMON_DMASK_ENABLE                             (0xFFU) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Enable/disable VMONDMASK functionality
#define BSP_FEATURE_VMON_OUT_DMASK_ENABLE                         (0xF0U) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Enable/disable VMON_OUT_DMASK functionality
#define BSP_FEATURE_VMON_RESET_DMASK_ENABLE                       (0x0FU) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< Enable/disable VMON_RESET_DMASK functionality

/** DMAC */
#define BSP_FEATURE_DMAC_SWITCHABLE_REDUNDANT_OPERATION           (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< DMAC redundant operation supported

/** DTSC */
#define BSP_FEATURE_DTSC_TRANSFER_RQ_OVERFLOW_FLAG                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< DTSC overflow flag supported

/** RLIN3 */
#if (BSP_PACKAGE_PINS_QFP100 == BSP_PACKAGE_PINS)
 #define BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK                      (0x80007FUL)                                                                  ///< LIN channel bitmask (8 channels)
 #define BSP_FEATURE_RLIN3_LIN_NUM_OF_CHANNELS                    (8U)                                                                          ///< Number of LIN channels (QFP100)
#else
 #define BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK                      (0x8007FFUL) /* PRQA S 1534 # Advisory R-2.5 */                               ///< LIN channel bitmask (12 channels)
 #define BSP_FEATURE_RLIN3_LIN_NUM_OF_CHANNELS                    (12U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of LIN channels (QFP144)
#endif

/** IOPORT */
#define BSP_FEATURE_IOPORT_NF_RLIN30                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RLIN30 signal
#define BSP_FEATURE_IOPORT_NF_RLIN31                              (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RLIN31 signal
#define BSP_FEATURE_IOPORT_NF_RSCAN                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RSCAN signal
#define BSP_FEATURE_IOPORT_NF_FLXA0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter FLXA0 signal
#define BSP_FEATURE_IOPORT_NF_FLXA1                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter FLXA1 signal
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
#define BSP_FEATURE_IOPORT_NF_PSI51                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PSI51 signal
#define BSP_FEATURE_IOPORT_NF_PSI52                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PSI52 signal
#define BSP_FEATURE_IOPORT_NF_PSI53                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PSI53 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA0                           (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA0 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA1                           (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA1 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA2                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA2 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA3                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA3 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA4                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA4 signal
#define BSP_FEATURE_IOPORT_NF_PIC_TAPA5                           (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TAPA5 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ0                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCJ0 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ1                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCJ1 signal
#define BSP_FEATURE_IOPORT_NF_ADCJ2                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCJ2 signal
#define BSP_FEATURE_IOPORT_NF_ADCK0                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCK0 signal
#define BSP_FEATURE_IOPORT_NF_ADCK1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCK1 signal
#define BSP_FEATURE_IOPORT_NF_ADCK2                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCK2 signal
#define BSP_FEATURE_IOPORT_NF_ADCK3                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCK3 signal
#define BSP_FEATURE_IOPORT_NF_ADCKA                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ADCKA signal
#define BSP_FEATURE_IOPORT_NF_RHSB0                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RHSB0 signal
#define BSP_FEATURE_IOPORT_NF_RHSB1                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RHSB1 signal
#define BSP_FEATURE_IOPORT_NF_RHSB2                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RHSB2 signal
#define BSP_FEATURE_IOPORT_NF_RHSB3                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter RHSB3 signal
#define BSP_FEATURE_IOPORT_NF_ENCA0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ENCA0 signal
#define BSP_FEATURE_IOPORT_NF_ENCA1                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter ENCA1 signal
#define BSP_FEATURE_IOPORT_NF_TAUD0                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUD0 signal
#define BSP_FEATURE_IOPORT_NF_TAUD1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUD1 signal
#define BSP_FEATURE_IOPORT_NF_TAUD2                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUD2 signal
#define BSP_FEATURE_IOPORT_NF_TAUD3                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TAUD3 signal
#define BSP_FEATURE_IOPORT_NF_TSG30                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TSG30 signal
#define BSP_FEATURE_IOPORT_NF_TSG31                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TSG31 signal
#define BSP_FEATURE_IOPORT_NF_TSG32                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter TSG32 signal
#define BSP_FEATURE_IOPORT_NF_IRQ0                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ0 signal
#define BSP_FEATURE_IOPORT_NF_IRQ1                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ1 signal
#define BSP_FEATURE_IOPORT_NF_IRQ2                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ2 signal
#define BSP_FEATURE_IOPORT_NF_IRQ3                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ3 signal
#define BSP_FEATURE_IOPORT_NF_IRQ4                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ4 signal
#define BSP_FEATURE_IOPORT_NF_IRQ5                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ5 signal
#define BSP_FEATURE_IOPORT_NF_IRQ6                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ6 signal
#define BSP_FEATURE_IOPORT_NF_IRQ7                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ7 signal
#define BSP_FEATURE_IOPORT_NF_IRQ8                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ8 signal
#define BSP_FEATURE_IOPORT_NF_IRQ9                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ9 signal
#define BSP_FEATURE_IOPORT_NF_IRQ10                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ10 signal
#define BSP_FEATURE_IOPORT_NF_IRQ11                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ11 signal
#define BSP_FEATURE_IOPORT_NF_IRQ12                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ12 signal
#define BSP_FEATURE_IOPORT_NF_IRQ13                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ13 signal
#define BSP_FEATURE_IOPORT_NF_IRQ14                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ14 signal
#define BSP_FEATURE_IOPORT_NF_IRQ15                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ15 signal
#define BSP_FEATURE_IOPORT_NF_IRQ16                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ16 signal
#define BSP_FEATURE_IOPORT_NF_IRQ17                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ17 signal
#define BSP_FEATURE_IOPORT_NF_IRQ18                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ18 signal
#define BSP_FEATURE_IOPORT_NF_IRQ19                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ19 signal
#define BSP_FEATURE_IOPORT_NF_IRQ20                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ20 signal
#define BSP_FEATURE_IOPORT_NF_IRQ21                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ21 signal
#define BSP_FEATURE_IOPORT_NF_IRQ22                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ22 signal
#define BSP_FEATURE_IOPORT_NF_IRQ23                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ23 signal
#define BSP_FEATURE_IOPORT_NF_IRQ24                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ24 signal
#define BSP_FEATURE_IOPORT_NF_IRQ25                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ25 signal
#define BSP_FEATURE_IOPORT_NF_IRQ26                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ26 signal
#define BSP_FEATURE_IOPORT_NF_IRQ27                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ27 signal
#define BSP_FEATURE_IOPORT_NF_IRQ28                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ28 signal
#define BSP_FEATURE_IOPORT_NF_IRQ29                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ29 signal
#define BSP_FEATURE_IOPORT_NF_IRQ30                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ30 signal
#define BSP_FEATURE_IOPORT_NF_IRQ31                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ31 signal
#define BSP_FEATURE_IOPORT_NF_IRQ32                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ32 signal
#define BSP_FEATURE_IOPORT_NF_IRQ33                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ33 signal
#define BSP_FEATURE_IOPORT_NF_IRQ34                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ34 signal
#define BSP_FEATURE_IOPORT_NF_IRQ35                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ35 signal
#define BSP_FEATURE_IOPORT_NF_IRQ36                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ36 signal
#define BSP_FEATURE_IOPORT_NF_IRQ37                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ37 signal
#define BSP_FEATURE_IOPORT_NF_IRQ38                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ38 signal
#define BSP_FEATURE_IOPORT_NF_IRQ39                               (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ39 signal
#define BSP_FEATURE_IOPORT_NF_IRQ_NMI                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ_NMI signal
#define BSP_FEATURE_IOPORT_NF_IRQ40                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ40 signal
#define BSP_FEATURE_IOPORT_NF_IRQ41                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ41 signal
#define BSP_FEATURE_IOPORT_NF_IRQ42                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ42 signal
#define BSP_FEATURE_IOPORT_NF_IRQ43                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ43 signal
#define BSP_FEATURE_IOPORT_NF_IRQ44                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ44 signal
#define BSP_FEATURE_IOPORT_NF_IRQ45                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ45 signal
#define BSP_FEATURE_IOPORT_NF_IRQ_ESR                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter IRQ_ESR signal
#define BSP_FEATURE_IOPORT_NF_INTP_NMI                            (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP_NMI signal
#define BSP_FEATURE_IOPORT_NF_INTP0                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP0 signal
#define BSP_FEATURE_IOPORT_NF_INTP1                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP1 signal
#define BSP_FEATURE_IOPORT_NF_INTP2                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP2 signal
#define BSP_FEATURE_IOPORT_NF_INTP3                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP3 signal
#define BSP_FEATURE_IOPORT_NF_INTP4                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP4 signal
#define BSP_FEATURE_IOPORT_NF_INTP5                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP5 signal
#define BSP_FEATURE_IOPORT_NF_INTP6                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP6 signal
#define BSP_FEATURE_IOPORT_NF_INTP7                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP7 signal
#define BSP_FEATURE_IOPORT_NF_INTP8                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP8 signal
#define BSP_FEATURE_IOPORT_NF_INTP9                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP9 signal
#define BSP_FEATURE_IOPORT_NF_INTP10                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP10 signal
#define BSP_FEATURE_IOPORT_NF_INTP11                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP11 signal
#define BSP_FEATURE_IOPORT_NF_INTP12                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP12 signal
#define BSP_FEATURE_IOPORT_NF_INTP13                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP13 signal
#define BSP_FEATURE_IOPORT_NF_INTP14                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP14 signal
#define BSP_FEATURE_IOPORT_NF_INTP15                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP15 signal
#define BSP_FEATURE_IOPORT_NF_INTP16                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP16 signal
#define BSP_FEATURE_IOPORT_NF_INTP17                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP17 signal
#define BSP_FEATURE_IOPORT_NF_INTP18                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP18 signal
#define BSP_FEATURE_IOPORT_NF_INTP19                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP19 signal
#define BSP_FEATURE_IOPORT_NF_INTP20                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP20 signal
#define BSP_FEATURE_IOPORT_NF_INTP21                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP21 signal
#define BSP_FEATURE_IOPORT_NF_INTP22                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP22 signal
#define BSP_FEATURE_IOPORT_NF_INTP23                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP23 signal
#define BSP_FEATURE_IOPORT_NF_INTP24                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP24 signal
#define BSP_FEATURE_IOPORT_NF_INTP25                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP25 signal
#define BSP_FEATURE_IOPORT_NF_INTP26                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP26 signal
#define BSP_FEATURE_IOPORT_NF_INTP27                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP27 signal
#define BSP_FEATURE_IOPORT_NF_INTP28                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP28 signal
#define BSP_FEATURE_IOPORT_NF_INTP29                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP29 signal
#define BSP_FEATURE_IOPORT_NF_INTP30                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP30 signal
#define BSP_FEATURE_IOPORT_NF_INTP31                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP31 signal
#define BSP_FEATURE_IOPORT_NF_INTP32                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP32 signal
#define BSP_FEATURE_IOPORT_NF_INTP33                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP33 signal
#define BSP_FEATURE_IOPORT_NF_INTP34                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP34 signal
#define BSP_FEATURE_IOPORT_NF_INTP35                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP35 signal
#define BSP_FEATURE_IOPORT_NF_INTP36                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP36 signal
#define BSP_FEATURE_IOPORT_NF_INTP37                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP37 signal
#define BSP_FEATURE_IOPORT_NF_INTP38                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP38 signal
#define BSP_FEATURE_IOPORT_NF_INTP39                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter INTP39 signal
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
#define BSP_FEATURE_IOPORT_NF_PIC_TSG                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter PIC_TSG signal
#define BSP_FEATURE_IOPORT_NF_EMU0                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter EMU0 signal
#define BSP_FEATURE_IOPORT_NF_EMU1                                (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter EMU1 signal
#define BSP_FEATURE_IOPORT_NF_SSIF                                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter SSIF signal
#define BSP_FEATURE_IOPORT_NF_OSPI0                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT Noise filter OSPI0 signal

#define BSP_FEATURE_IOPORT_LVDSA                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSB                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSC                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSD                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSE                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_LVDSF                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable LVDS feature (0U): Disable/ (1U): Enable

#define BSP_FEATURE_IOPORT_PST                                    (3U)                                                                          ///< Number of ECM error trigger
#define BSP_FEATURE_IOPORT_PEIODC                                 (1U)                                                                          ///< IOPORT enable/disable ERRORIN Open-drain feature (0U): Disable/ (1U): Enable
#define BSP_FEATURE_IOPORT_PULVSEL5                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< IOPORT enable/disable PULVSEL5 feature (0U): Disable/ (1U): Enable

/** OSTM */
#define BSP_FEATURE_OSTM_VALID_CHANNEL_MASK                       (0x1U) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Valid OSTM channel mask

/** LTSC */
#define BSP_FEATURE_LTSC_CLOCK_SOURCE                             (FSP_PRIV_CLOCK_CLKC_HSB) /* PRQA S 1534 # Advisory R-2.5 */                  ///< LTSC uses CLKC_HSB clock as the clock source.

/** ECM */
#define BSP_FEATURE_ECM_ERROR_SOURCE_CHECK_VALID                  (255U) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Maximum ECM error source index supported

/** PSI5 */
#define BSP_FEATURE_PSI5_MAX_CHANNEL                              (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum PSI5 unit supported
#define BSP_FEATURE_PSI5_NUM_SLOTS_EACH_UNIT                      (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum PSI5 slot per unit supported
#define BSP_FEATURE_PSI5_WP_IS_AVAILABLE                          (1U)                                                                          /* Indicates the wrapper function is available in PSI5 */ /* PRQA S 1534 # Advisory R-2.5 */

/** ICU */
#define BSP_FEATURE_FCLA5_IS_AVAILABLE                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< FCLA5 register availability (1 = available)
#define BSP_FEATURE_ICU_SIGNAL_INTP                               (0U)                                                                          /* PRQA S 1534 # Advisory R-2.5 */

/** CXPI */
#define BSP_FEATURE_CXPI_IS_AVAILABLE                             (0x01U) /* PRQA S 1534 # Advisory R-2.5 */                                    ///< CXPI peripheral availability (1 = available)
#define BSP_FEATURE_CXPI_NUM_CHANNELS                             (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum CXPI channel supported

/** CLMA */
#define BSP_FEATURE_CLMA_BACKUP_CLOCK_IS_AVAILABLE                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Backup Clock support
#define BSP_FEATURE_CLMA_SSCG1_UNDIVIDED_REQUIRED                 (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< CLMA CKS_SSCG1C Requirement

/** SSI */
#define BSP_FEATURE_SSI_FIFO_NUM_STAGES                           (32U)                                                                         /* Number of I2S FIFO stages on this MCU */ /* PRQA S 1534 # Advisory R-2.5 */

/** ECM */
#define BSP_FEATURE_ECM_INSTANCE                                  (7U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ECM feature instance

/** CANXL */
#define BSP_FEATURE_CANXL_NUM_CHANNELS                            (1UL)                                                                         /* Number of CANXL channel on this MCU */ /* PRQA S 1534 # Advisory R-2.5 */

/** CGC */
#define BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK                       (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of CPU system clock domains supported
#define BSP_FEATURE_CGC_HAS_CLK_SYS                               (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< System clock CLK_SYS is not available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN                         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< System clock CLK_SYS_CLEAN is available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG                          (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< System clock CLK_SYS_SSCG is available
#define BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1                         (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< System clock CLK_SYS_SSCG1 is not available
#define BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS         (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Peripheral input clock source selectors
#define BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Sub oscillator is available
#define BSP_FEATURE_CGC_HVIOSC_IS_AVAILABLE                       (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< High Voltage Internal Oscillator is not available
#define BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE         (5U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of repetition cases for System Clock gear up/down shifting

/** ADCK */
#define BSP_FEATURE_ADCK_NUM_OF_UNIT                              (2U)                                                                          /* ADCK support on this MCU */ /* PRQA S 1534 # Advisory R-2.5 */

/** ECM ERRORIN NF */
#define BSP_FEATURE_ECM_ERRORIN_NF_MAX                            IOPORT_FILTER_SIGNAL_ERRORIN3 /* PRQA S 1534 # Advisory R-2.5 */              ///< ECM ERRORIN NF Max
#define BSP_FEATURE_ECM_ERRORIN_NF_0                              IOPORT_FILTER_SIGNAL_ERRORIN0 /* PRQA S 1534 # Advisory R-2.5 */              ///< ECM ERRORIN NF0

/** DMAC */
#define BSP_FEATURE_DMAC_MAX_CHANNEL                              (16U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Maximum DMAC channels
#define BSP_FEATURE_DMAC_MAX_UNIT                                 (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Maximum DMAC units

/** LPS */
#define BSP_FEATURE_LPS_PIN_RH850U2Cx                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///<///< LPS support DPIN17 - DPIN22

/** GTM_ATOM */
#define BSP_FEATURE_GTM_ATOM_HRES_MODE_SUPPORT                    (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< High-Resolution PWM Support

/** FLASH */
#define BSP_FEATURE_FLASH_CF_WRITE_SIZE                           (512U) /* PRQA S 1534 # Advisory R-2.5 */                                     ///< Code flash minimum write size in bytes
#define BSP_FEATURE_FLASH_CF_REGION0_BLOCK_SIZE                   (0x4000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Small CF block size: 16 KB
#define BSP_FEATURE_FLASH_CF_REGION1_BLOCK_SIZE                   (0x10000U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< Large CF block size: 64 KB
#define BSP_FEATURE_FLASH_CF_REGION0_SIZE                         (0x00020000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Region 0 total size: 128 KB (8 x 16 KB)
#define BSP_FEATURE_FLASH_CF_BLANK_CHECK_AREA_START               (0x0C000000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Code Flash Blank Check Area base address

#define BSP_FEATURE_FLASH_CF_BANK_A_START                         (0x00000000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Bank A start address
#define BSP_FEATURE_FLASH_CF_BANK_A_SIZE                          (0x00100000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Bank A size: 1 MB (U2C2)
#define BSP_FEATURE_FLASH_CF_BANK_B_START                         (0x00400000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Bank B start address
#define BSP_FEATURE_FLASH_CF_BANK_B_SIZE                          (0x00100000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Bank B size: 1 MB (U2C2)

#define BSP_FEATURE_FLASH_CODE_FLASH_START                        (0x00000000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Code flash start address
#define BSP_FEATURE_FLASH_DF_BLOCK_SIZE                           (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Data flash block (erase unit): 4 KB
#define BSP_FEATURE_FLASH_DF_WRITE_SIZE                           (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Data flash minimum write size in bytes

#define BSP_FEATURE_FLASH_DATA_FLASH_AREA0_START                  (0xFF200000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Data Area 0 start (FPSYS0)
#define BSP_FEATURE_FLASH_DATA_FLASH_AREA0_SIZE                   (0x20000U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< Data Area 0 size: 128 KB
#define BSP_FEATURE_FLASH_DATA_FLASH_AREA2_START                  (0xFF280000U) /* PRQA S 1534 # Advisory R-2.5 */                              ///< Data Area 2 start - ICUMB unit only
#define BSP_FEATURE_FLASH_DATA_FLASH_AREA2_SIZE                   (0x10000U) /* PRQA S 1534 # Advisory R-2.5 */                                 ///< Data Area 2 size: 64 KB - ICUMB unit only
#define BSP_FEATURE_FLASH_DATA_FLASH_START                        (BSP_FEATURE_FLASH_DATA_FLASH_AREA0_START) /* PRQA S 1534 # Advisory R-2.5 */ ///< DF start address
#define BSP_DATA_FLASH_SIZE_BYTES                                 (BSP_FEATURE_FLASH_DATA_FLASH_AREA0_SIZE) /* PRQA S 1534 # Advisory R-2.5 */  ///< CPU-accessible DF: 128 KB (Area 0 only)

/** GTM_MCS */
#define BSP_FEATURE_GTM_MCS_NUMBER_CHANNEL                        (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCS channels per instance
#define BSP_FEATURE_GTM_MCS_NUMBER_TRIGGER                        (24U) /* PRQA S 1534 # Advisory R-2.5 */                                      ///< Number of GTM MCS trigger sources
#define BSP_FEATURE_GTM_MCS_NUMBER_SINT                           (8U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCS SINT sources
#define BSP_FEATURE_GTM_MCS_NUMBER_HBP                            (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of GTM MCS HBP units
#define BSP_FEATURE_GTM_MCS_SP_CNT_AVAILABLE                      (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Stack Pointer Counter support
#define BSP_FEATURE_GTM_MCS_EXT_IRQ_AVAILABLE                     (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< External Interrupt Selection support

/** SCI3 */
#define BSP_FEATURE_SCI3_CHANNEL_NUM                              (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of SCI3 channels available

#define BSP_FEATURE_RHSB_VALID_CHANNEL_MASK                       (0x00000000UL) /* PRQA S 1534 # Advisory R-2.5 */                             ///< RHSB implemented-unit mask: no RHSB unit on this device

#define BSP_FEATURE_R_ENCA0_TYPE_USED                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< R_ENCA0_Type is used

#define BSP_FEATURE_IOPORT_NF_TAUD2_USE_TAUD_REG                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Indicates whether TAUD2 noise filter uses the TAUD-specific control register.
#define BSP_FEATURE_IOPORT_NF_TAUJ_FCLA_AVAILABLE                 (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Indicates whether TAUJ noise filter has the TAUJ-specific FCLACTL edge detector register.

#define BSP_FEATURE_TPTM_COMPARISON_STATUS                        (1) /* PRQA S 1534 # Advisory R-2.5 */                                        ///< Comparison status is available

#define BSP_FEATURE_TPTM_INTERRUPT_UP_TIMER                       (1) /* PRQA S 1534 # Advisory R-2.5 */                                        ///< Interrupt up timer is  available

#define BSP_FEATURE_ADCJ_HWIP_SUPPORT                             (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCJ peripheral support
#define BSP_FEATURE_ADCK_HWIP_SUPPORT                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< ADCK peripheral support

#define BSP_FEATURE_MOTOR_SUPPORTED_VARIANT                       (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 hex present 1 MCU variant using alphabet order
#define BSP_FEATURE_MOTOR_IOPORT_PIN_NUMBER                       (BSP_PACKAGE_PINS) /* PRQA S 1534 # Advisory R-2.5 */                         ///< Pin profile number
#define BSP_FEATURE_MOTOR_TAUD_THREE_PHASE_SUPPORT                (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Taud three phase feature support

/** TAPA */
#define BSP_FEATURE_TAPA_SUPPORTED_VARIANT                        (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order

/** PWM_DIAG */
#define BSP_FEATURE_PWM_DIAG_SUPPORTED_VARIANT                    (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order

/** ADCK */
#define BSP_FEATURE_ADCK_SUPPORTED_VARIANT                        (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order

/** OSTM */
#define BSP_FEATURE_OSTM_SIMPLE_CHANNEL_LAYOUT_AVAILABLE          (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< OSTM simple channel layout is available
#define BSP_FEATURE_OSTM_EXTENDED_CHANNEL_LAYOUT_AVAILABLE        (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< OSTM extended channel layout is not available

/** PIC */
#define BSP_FEATURE_PIC_SUPPORTED_VARIANT                         (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order
#define BSP_FEATURE_PIC_AVAILABLE                                 (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order

/** LPS */
#define BSP_FEATURE_LPS_SUPPORTED_VARIANT                         (0x1000U) /* PRQA S 1534 # Advisory R-2.5 */                                  ///< Profile for MCU variant, 1 bit present 1 MCU variant using alphabet order
#define BSP_FEATURE_LPS_PIN_2_SUPPORT                             (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS support pin for U2Cx

/** LPS */
#define BSP_FEATURE_LPS_ANALOG_MODE_SUPPORT                       (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS analog mode support
#define BSP_FEATURE_LPS_DIGITAL_MODE_SUPPORT                      (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS digital mode support
#define BSP_FEATURE_LPS_DIGITAL_INPUT_SUPPORT                     (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< LPS digital input mode unsupport

/** GTM */
#define BSP_FEATURE_GTM_INTSEL_NUMBER                             (2U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of interrupt selection control registers
#define BSP_FEATURE_GTM_DMASEL_SUPPORT                            (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Interconnections of Interrupt Factors for DMA/DTS Support
#define BSP_FEATURE_GTM_DMASEL_NUMBER                             (4U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< Number of interrupt selection control for DMA/DTS registers
#define BSP_FEATURE_GTM_IP_GEN_3                                  (0U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM_IP generation 3 in used
#define BSP_FEATURE_GTM_IP_GEN_4                                  (1U) /* PRQA S 1534 # Advisory R-2.5 */                                       ///< GTM_IP generation 4 in used
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

/** @} (end addtogroup BSP_MCU_RH850U2C2) */

#endif                                 /* BSP_FEATURE_H */
