/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2C8
 * @defgroup BSP_ECM_U2C8 BSP ECM
 * @brief This module provides the macros, data types, and definition constants required for ECM.
 *
 * @{
 **********************************************************************************************************************/

#ifndef BSP_ECM_H
#define BSP_ECM_H

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** ECM Error Factor */
typedef enum e_ecm_error_source
{
    ECM_ERROR_SOURCE_ERROR_OUTPUT_MONITOR = (0),                                  ///< Error output monitor
    ECM_ERROR_SOURCE_ECMMESET_MONITOR     = (2),                                  ///< ECMmESET monitor
    ECM_ERROR_SOURCE_ECMMESET0_MONITOR    = (3),                                  ///< ECMmESET0 monitor
    ECM_ERROR_SOURCE_ECMMESET1_MONITOR    = (4),                                  ///< ECMmESET1 monitor
    ECM_ERROR_SOURCE_ECMMESET2_MONITOR    = (5),                                  ///< ECMmESET2 monitor
    ECM_ERROR_SOURCE_ECMMESET3_MONITOR    = (6),                                  ///< ECMmESET3 monitor
    ECM_ERROR_SOURCE_DELAY_TIMER_OVERFLOW_FOR_ERROR_OUTPUT = (11),                ///< Delay timer overflow for Error output
    ECM_ERROR_SOURCE_ECM_COMPARE_ERROR = (20),                                    ///< ECM compare error
    ECM_ERROR_SOURCE_MODE_ERR_NORMAL_OPERATION_OR_USER_BOOT_1       = (24),       ///< Unintended activation of Production Test Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_USER_BOOT_MODE_1             = (25),       ///< Unintended activation of Normal Operating Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_NORMAL_OPERATION_MODE_1      = (26),       ///< Mode error during Normal Operating Mode
    ECM_ERROR_SOURCE_MODE_ERR_DURING_NOR_OPER_MODE_USER_BOOT_MODE_2 = (27),       ///< Unintended activation of Serial Programming Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_NORMAL_OPERATING_MODE_2      = (28),       ///< Mode error during Normal Operating Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_USER_BOOT_MODE_2             = (29),       ///< Unintended deactivation of User Boot Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_ANY_MODE                      = (30),      ///< Mode error during any mode
    ECM_ERROR_SOURCE_FLASH_ACCESS_ERROR                              = (32),      ///< Flash access error
    ECM_ERROR_SOURCE_FACI_RESET_TRANSFER_ERROR                       = (33),      ///< FACI reset transfer error
    ECM_ERROR_SOURCE_BIST_PARAMETER_TRANSFER_ERROR                   = (34),      ///< BIST parameter transfer error
    ECM_ERROR_SOURCE_DTS_COMPARE_ERROR                               = (36),      ///< DTS compare error
    ECM_ERROR_SOURCE_SDMAC0_CHANNEL_0_COMPARE_ERROR                  = (42),      ///< sDMAC0 channel 0 compare error
    ECM_ERROR_SOURCE_SDMAC0_CHANNEL_1_COMPARE_ERROR                  = (43),      ///< sDMAC0 channel 1 compare error
    ECM_ERROR_SOURCE_SDMAC0_CHANNEL_2_COMPARE_ERROR                  = (44),      ///< sDMAC0 channel 2 compare error
    ECM_ERROR_SOURCE_SDMAC0_CHANNEL_3_COMPARE_ERROR                  = (45),      ///< sDMAC0 channel 3 compare error
    ECM_ERROR_SOURCE_SDMAC0_CHANNEL_4_COMPARE_ERROR                  = (46),      ///< sDMAC0 channel 4 compare error
    ECM_ERROR_SOURCE_SDMAC0_CHANNEL_5_COMPARE_ERROR                  = (47),      ///< sDMAC0 channel 5 compare error
    ECM_ERROR_SOURCE_SDMAC0_CHANNEL_6_COMPARE_ERROR                  = (48),      ///< sDMAC0 channel 6 compare error
    ECM_ERROR_SOURCE_SDMAC0_CHANNEL_7_COMPARE_ERROR                  = (49),      ///< sDMAC0 channel 7 compare error
    ECM_ERROR_SOURCE_SDMAC1_CHANNEL_0_COMPARE_ERROR                  = (50),      ///< sDMAC1 channel 0 compare error
    ECM_ERROR_SOURCE_SDMAC1_CHANNEL_1_COMPARE_ERROR                  = (51),      ///< sDMAC1 channel 1 compare error
    ECM_ERROR_SOURCE_SDMAC1_CHANNEL_2_COMPARE_ERROR                  = (52),      ///< sDMAC1 channel 2 compare error
    ECM_ERROR_SOURCE_SDMAC1_CHANNEL_3_COMPARE_ERROR                  = (53),      ///< sDMAC1 channel 3 compare error
    ECM_ERROR_SOURCE_SDMAC1_CHANNEL_4_COMPARE_ERROR                  = (54),      ///< sDMAC1 channel 4 compare error
    ECM_ERROR_SOURCE_SDMAC1_CHANNEL_5_COMPARE_ERROR                  = (55),      ///< sDMAC1 channel 5 compare error
    ECM_ERROR_SOURCE_SDMAC1_CHANNEL_6_COMPARE_ERROR                  = (56),      ///< sDMAC1 channel 6 compare error
    ECM_ERROR_SOURCE_SDMAC1_CHANNEL_7_COMPARE_ERROR                  = (57),      ///< sDMAC1 channel 7 compare error
    ECM_ERROR_SOURCE_BUS_BRIDGE_COMPARE_ERROR_OF_SDMAC0              = (58),      ///< BUS Bridge compare error of sDMAC0
    ECM_ERROR_SOURCE_BUS_BRIDGE_COMPARE_ERROR_OF_SDMAC1              = (59),      ///< BUS Bridge compare error of sDMAC1
    ECM_ERROR_SOURCE_BUS_BRIDGE_COMPARE_ERROR                        = (60),      ///< BUS Bridge compare error
    ECM_ERROR_SOURCE_GTM_COMPARE_ERROR                               = (61),      ///< GTM compare error
    ECM_ERROR_SOURCE_GTM_ERROR_INTERRUPT                             = (62),      ///< GTM error interrupt
    ECM_ERROR_SOURCE_INTERPROCESSOR_ELEMENT_BUS_ROUTING_ERROR        = (64),      ///< Inter-processor element Bus routing error
    ECM_ERROR_SOURCE_PERIPHERAL_BUS_PBUS_ROUTING_ERROR               = (66),      ///< Peripheral Bus (P-Bus) routing error
    ECM_ERROR_SOURCE_SYSTEM_BUS_ROUTING_ERROR                        = (68),      ///< System Bus routing error
    ECM_ERROR_SOURCE_GLOBAL_FLASH_BUS_ROUTING_ERROR                  = (69),      ///< Global Flash Bus routing error
    ECM_ERROR_SOURCE_LOCAL_FLASH_BUS_ROUTING_ERROR                   = (70),      ///< Local Flash Bus routing error
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_MOSC                = (71),      ///< Clock monitor error for CLK_MOSC
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_WDTBCLK_HSIOSC      = (72),      ///< Clock monitor error for CLK_WDTB/CLK_HSIOSC
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_LSIOSC              = (73),      ///< Clock monitor error for CLK_LSIOSC
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_LSB                 = (74),      ///< Clock monitor error for CLK_LSB
    ECM_ERROR_SOURCE_CLKC_SBUS_CLKC_UHSB_GTM_CLOCK                   = (75),      ///< Clock monitor error for CLKC_SBUS / CLKC_UHSB (GTM clock)
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLKC_LSB                = (77),      ///< Clock monitor error for CLKC_LSB
    ECM_ERROR_SOURCE_CLK_SBUS_ICUM_CLOCK                             = (79),      ///< Clock monitor error for CLK_SBUS (ICUM clock)
    ECM_ERROR_SOURCE_OS_TIMER_1_INTERRUPT                            = (80),      ///< OS timer 1 interrupt
    ECM_ERROR_SOURCE_CANXL0_SAFETY_RELEVANT_INTERRUPT                = (89),      ///< CANXL0 safety relevant interrupt
    ECM_ERROR_SOURCE_CANXL1_SAFETY_RELEVANT_INTERRUPT                = (90),      ///< CANXL1 safety relevant interrupt
    ECM_ERROR_SOURCE_TSG30_ERROR_INTERRUPT                           = (96),      ///< TSG30 error interrupt
    ECM_ERROR_SOURCE_TSG31_ERROR_INTERRUPT                           = (97),      ///< TSG31 error interrupt
    ECM_ERROR_SOURCE_SECONDARY_POWER_SUPPLY_VOLTAGE_MONITOR_ERROR    = (99),      ///< Secondary power supply voltage monitor error
    ECM_ERROR_SOURCE_AD_CONVERTER_PARITY_ERROR                       = (100),     ///< A/D converter parity error
    ECM_ERROR_SOURCE_TEMPERATURE_SENSOR_ERROR                        = (103),     ///< Temperature sensor error
    ECM_ERROR_SOURCE_CODE_FLASH_ADDRESS_PARITY_ERROR                 = (104),     ///< Code Flash - Address parity error
    ECM_ERROR_SOURCE_CODE_FLASH_DATA_ECC_2BIT_ERROR                  = (105),     ///< Code Flash - Data ECC 2-bit error
    ECM_ERROR_SOURCE_CODE_FLASH_DATA_ECC_1BIT_ERROR                  = (106),     ///< Code Flash - Data ECC 1-bit error
    ECM_ERROR_SOURCE_CODE_FLASH_ECC_OVERFLOW_ERROR                   = (107),     ///< Code Flash - ECC overflow error
    ECM_ERROR_SOURCE_DATA_FLASH_DATA_ECC_2BIT_ERROR                  = (108),     ///< Data Flash - Data ECC 2-bit error
    ECM_ERROR_SOURCE_DATA_FLASH_DATA_ECC_1BIT_ERROR                  = (109),     ///< Data Flash - Data ECC 1-bit error
    ECM_ERROR_SOURCE_DATA_FLASH_ECC_OVERFLOW_ERROR                   = (110),     ///< Data Flash - ECC overflow error
    ECM_ERROR_SOURCE_CLUSTER_RAM_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT = (114),     ///< Cluster RAM - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_CLUSTER_RAM_DATA_ECC_1BIT_ERROR                 = (115),     ///< Cluster RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_CLUSTER_RAM_ECC_OVERFLOW_ERROR                  = (116),     ///< Cluster RAM - ECC overflow error
    ECM_ERROR_SOURCE_DTSRAM_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT      = (117),     ///< DTSRAM - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_DTSRAM_DATA_ECC_1BIT_ERROR                      = (118),     ///< DTSRAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_DTSRAM_ECC_OVERFLOW_ERROR                       = (119),     ///< DTSRAM - ECC overflow error
    ECM_ERROR_SOURCE_SDMAC0_RAM_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT  = (120),     ///< sDMAC0 RAM - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_SDMAC0_RAM_DATA_ECC_1BIT_ERROR                  = (121),     ///< sDMAC0 RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_SDMAC1_RAM_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT  = (122),     ///< sDMAC1 RAM - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_SDMAC1_RAM_DATA_ECC_1BIT_ERROR                  = (123),     ///< sDMAC1 RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_FLEXRAY_RAM_DATA_ECC_2BIT_ERROR                 = (128),     ///< FlexRay RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_FLEXRAY_RAM_DATA_ECC_1BIT_ERROR                 = (129),     ///< FlexRay RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_RSCANFD_RAM_DATA_ECC_2BIT_ERROR                 = (130),     ///< RS-CANFD RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_RSCANFD_RAM_DATA_ECC_1BIT_ERROR                 = (131),     ///< RS-CANFD RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_MSPI_RAM_DATA_ECC_2BIT_ERROR                    = (132),     ///< MSPI RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_MSPI_RAM_DATA_ECC_1BIT_ERROR                    = (133),     ///< MSPI RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_GTM_RAM_DATA_ECC_2BIT_ERROR                     = (134),     ///< GTM RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_GTM_RAM_DATA_ECC_1BIT_ERROR                     = (135),     ///< GTM RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_TSNES0_DATA_ECC_2BIT_ERROR                      = (136),     ///< TSNES0 - Data ECC 2-bit error
    ECM_ERROR_SOURCE_TSNES0_DATA_ECC_1BIT_ERROR                      = (137),     ///< TSNES0 - Data ECC 1-bit error
    ECM_ERROR_SOURCE_TSNES1_DATA_ECC_2BIT_ERROR                      = (138),     ///< TSNES1 - Data ECC 2-bit error
    ECM_ERROR_SOURCE_TSNES1_DATA_ECC_1BIT_ERROR                      = (139),     ///< TSNES1 - Data ECC 1-bit error
    ECM_ERROR_SOURCE_MMCA_RAM_DATA_ECC_2BIT_ERROR                    = (140),     ///< MMCA RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_MMCA_RAM_DATA_ECC_1BIT_ERROR                    = (141),     ///< MMCA RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_CANXL_RAM_DATA_ECC_2BIT_ERROR                   = (142),     ///< CANXL RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_CANXL_RAM_DATA_ECC_1BIT_ERROR                   = (143),     ///< CANXL RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_RETENTION_RAM_DATA_ECC_2BIT_ERROR               = (144),     ///< Retention RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_RETENTION_RAM_DATA_ECC_1BIT_ERROR               = (145),     ///< Retention RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_RETENTION_RAM_DATA_ECC_OVERFLOW_ERROR           = (146),     ///< Retention RAM - Data ECC overflow error
    ECM_ERROR_SOURCE_ETNF_RAM_DATA_ECC_2BIT_ERROR                    = (147),     ///< ETNF RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_ETNF_RAM_DATA_ECC_1BIT_ERROR                    = (148),     ///< ETNF RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_PERIPHERAL_RAM_ECC_OVERFLOW_ERROR               = (159),     ///< Peripheral RAM - ECC overflow error
    ECM_ERROR_SOURCE_DATA_TRANSFER_PATH_ADDRESS_EDC_ERROR            = (160),     ///< Data transfer path - Address EDC error
    ECM_ERROR_SOURCE_DATA_TRANSFER_PATH_DATA_ECC_2BIT_ERROR          = (161),     ///< Data transfer path - Data ECC 2-bit error
    ECM_ERROR_SOURCE_DATA_TRANSFER_PATH_DATA_ECC_1BIT_ERROR          = (162),     ///< Data transfer path - Data ECC 1-bit error
    ECM_ERROR_SOURCE_CRAM_GUARD_ERROR = (168),                                    ///< CRAM Guard error
    ECM_ERROR_SOURCE_INTERPROCESS_ELEMENT_BUS_GUARD_ERROR = (169),                ///< Inter-process element Bus Guard error
    ECM_ERROR_SOURCE_PBUS_GUARD_ERROR = (170),                                    ///< P-Bus Guard error
    ECM_ERROR_SOURCE_HBUS_GUARD_ERROR = (171),                                    ///< H-Bus Guard error
    ECM_ERROR_SOURCE_DTS_TRANSFER_ERRORDMA_ADDRESS_ERROR_5 = (182),               ///< DTS transfer Error/DMA address error *5
    ECM_ERROR_SOURCE_HBUS_MASTER_TRANSFER_ERROR_5          = (183),               ///< H-Bus master Transfer Error *5
    ECM_ERROR_SOURCE_EXTERNAL_ERROR_INPUT_0                = (184),               ///< External Error Input 0
    ECM_ERROR_SOURCE_EXTERNAL_ERROR_INPUT_1                = (185),               ///< External Error Input 1
    ECM_ERROR_SOURCE_EXTERNAL_ERROR_INPUT_2                = (186),               ///< External Error Input 2
    ECM_ERROR_SOURCE_EXTERNAL_ERROR_INPUT_3                = (187),               ///< External Error Input 3
    ECM_ERROR_SOURCE_SOFTWARE_ALARM_0                                    = (192), ///< Software Alarm 0
    ECM_ERROR_SOURCE_SOFTWARE_ALARM_1                                    = (193), ///< Software Alarm 1
    ECM_ERROR_SOURCE_SOFTWARE_ALARM_2                                    = (194), ///< Software Alarm 2
    ECM_ERROR_SOURCE_SOFTWARE_ALARM_3                                    = (195), ///< Software Alarm 3
    ECM_ERROR_SOURCE_ICUMHB_COMPARE_ERROR                                = (200), ///< ICUMHB compare error
    ECM_ERROR_SOURCE_ACEU0_COMPARE_ERROR                                 = (201), ///< ACEU0 compare error
    ECM_ERROR_SOURCE_ICUMHB_INTERNAL_BUS_ROUTING_ERROR                   = (203), ///< ICUMHB Internal Bus routing error
    ECM_ERROR_SOURCE_DATA_TRANS_PATH_FOR_SEC_MASTER_DATA_ECC_2BIT        = (204), ///< Data transfer path for security master - Data ECC 2-bit error
    ECM_ERROR_SOURCE_DATA_TRANS_PATH_FOR_SEC_MASTER_DATA_ECC_1BIT        = (205), ///< Data transfer path for security master - Data ECC 1-bit error
    ECM_ERROR_SOURCE_SECURITY_GUARD_ERROR                                = (206), ///< Security Guard Error
    ECM_ERROR_SOURCE_SECURITY_MASTER_DIRECT_TRANSFER_ERROR               = (207), ///< Security Master Direct Transfer  Error
    ECM_ERROR_SOURCE_ICUM_DMAC_DATA_RAM_DATA_ECC_2BIT_ERROR              = (208), ///< ICUM_DMAC DATA RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_ICUM_DMAC_DATA_RAM_DATA_ECC_1BIT_ERROR              = (209), ///< ICUM_DMAC DATA RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_DCLS_COMPARE_ERROR_PE0                              = (224), ///< DCLS compare error (PE0)
    ECM_ERROR_SOURCE_DCLS_MODE_ERROR_PE0                                 = (225), ///< DCLS mode error (PE0)
    ECM_ERROR_SOURCE_UNINTENDED_DEBUG_ENABLE_DETECTION_PE0               = (227), ///< Unintended Debug Enable detection (PE0)
    ECM_ERROR_SOURCE_WATCHDOG_TIMER_ERROR_PE0                            = (228), ///< Watchdog timer error (PE0)
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_CPU_PE0                 = (229), ///< Clock monitor error for CLK_CPU (PE0)
    ECM_ERROR_SOURCE_LOCAL_RAM_PE0_ADDR_FEEDBACK_CMP_ERROR_DATA_ECC_2BIT = (232), ///< Local RAM (PE0) - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE0_DATA_ECC_1BIT_ERROR                   = (233), ///< Local RAM (PE0) - Data ECC 1-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE0_ECC_OVERFLOW_ERROR                    = (234), ///< Local RAM (PE0) - ECC overflow error
    ECM_ERROR_SOURCE_INS_CACHE_RAM_PE0_ADDR_FEEDBACK_CMP_ERR_DATA_EDC    = (236), ///< Instruction cache RAM (PE0) - Address feedback compare error - Data EDC error
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_PE0                                  = (240), ///< PE guard error (PE0)
    ECM_ERROR_SOURCE_DATA_ACCESS_ERROR_PE0_5                             = (255), ///< Data Access Error (PE0) *5
    ECM_ERROR_SOURCE_DCLS_COMPARE_ERROR_PE1                              = (256), ///< DCLS compare error (PE1)
    ECM_ERROR_SOURCE_DCLS_MODE_ERROR_PE1                                 = (257), ///< DCLS mode error (PE1)
    ECM_ERROR_SOURCE_UNINTENDED_PE1_DISABLEENABLE_DETECTION              = (258), ///< Unintended PE1 disable/enable detection
    ECM_ERROR_SOURCE_UNINTENDED_DEBUG_ENABLE_DETECTION_PE1               = (259), ///< Unintended Debug Enable detection (PE1)
    ECM_ERROR_SOURCE_WATCHDOG_TIMER_ERROR_PE1                            = (260), ///< Watchdog timer error (PE1)
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_CPU_PE1                 = (261), ///< Clock monitor error for CLK_CPU (PE1)
    ECM_ERROR_SOURCE_LOCAL_RAM_PE1_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT   = (264), ///< Local RAM (PE1) - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE1_DATA_ECC_1BIT_ERROR                   = (265), ///< Local RAM (PE1) - Data ECC 1-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE1_ECC_OVERFLOW_ERROR                    = (266), ///< Local RAM (PE1) - ECC overflow error
    ECM_ERROR_SOURCE_INS_CACHE_RAM_PE1_ADDR_FEEDBACK_CMP_ERR_DATA_EDC    = (268), ///< Instruction cache RAM (PE1) - Address feedback compare error - Data EDC error
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_PE1                                  = (272), ///< PE guard error (PE1)
    ECM_ERROR_SOURCE_DATA_ACCESS_ERROR_PE1_5                             = (287), ///< Data Access Error (PE1) *5
} ecm_error_source_t;

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

#endif                                 /* BSP_ECM_H */

/** @} (end addtogroup BSP_MCU_RH850U2C8) */
