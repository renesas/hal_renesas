/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2A16
 * @defgroup BSP_ECM_U2A16 BSP ECM
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
typedef enum e_ecm_error_source_u2a16
{
    ECM_ERROR_SOURCE_ERROR_OUTPUT_MONITOR = (0),                                  ///< Error output monitor
    ECM_ERROR_SOURCE_ECMMESET_MONITOR     = (2),                                  ///< ECMmESET monitor
    ECM_ERROR_SOURCE_ECMMESET0_MONITOR    = (3),                                  ///< ECMmESET0 monitor
    ECM_ERROR_SOURCE_ECMMESET1_MONITOR    = (4),                                  ///< ECMmESET1 monitor
    ECM_ERROR_SOURCE_ECMMESET2_MONITOR    = (5),                                  ///< ECMmESET2 monitor
    ECM_ERROR_SOURCE_ECMMESET3_MONITOR    = (6),                                  ///< ECMmESET3 monitor
    ECM_ERROR_SOURCE_DELAY_TIMER_OVERFLOW_FOR_ERROR_OUTPUT = (11),                ///< Delay timer overflow for Error output
    ECM_ERROR_SOURCE_ECM_COMPARE_ERROR = (20),                                    ///< ECM compare error
    ECM_ERROR_SOURCE_MODE_ERR_NORMAL_OPERATION_OR_USER_BOOT_1  = (24),            ///< Unintended activation of Production Test Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_USER_BOOT_MODE_1        = (25),            ///< Unintended activation of Normal Operating Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_NORMAL_OPERATION_MODE_1 = (26),            ///< Mode error during Normal Operation Mode
    ECM_ERROR_SOURCE_MODE_ERR_NORMAL_OPERATION_OR_USER_BOOT_2  = (27),            ///< Unintended activation of Serial Programming Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_NORMAL_OPERATION_MODE_2 = (28),            ///< Mode error during Normal Operation Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_USER_BOOT_MODE_2        = (29),            ///< Unintended deactivation of User Boot Mode
    ECM_ERROR_SOURCE_MODE_ERROR_DURING_ANY_MODE                = (30),            ///< Mode error during any mode
    ECM_ERROR_SOURCE_FLASH_ACCESS_ERROR                              = (32),      ///< Flash access error
    ECM_ERROR_SOURCE_FACI_RESET_TRANSFER_ERROR                       = (33),      ///< FACI reset transfer error
    ECM_ERROR_SOURCE_BIST_PARAMETER_TRANSFER_ERROR                   = (34),      ///< BIST parameter transfer error
    ECM_ERROR_SOURCE_DTS_COMPARE_ERROR                               = (36),      ///< DTS compare error
    ECM_ERROR_SOURCE_BUS_BRIDGE_COMPARE_ERROR_OF_SDMAC               = (37),      ///< BUS Bridge compare error of sDMAC
    ECM_ERROR_SOURCE_BUS_BRIDGE_COMPARE_ERROR                        = (38),      ///< BUS Bridge compare error
    ECM_ERROR_SOURCE_INTERPROCESSOR_ELEMENT_BUS_ROUTING_ERROR        = (40),      ///< Inter-processor element Bus routing error
    ECM_ERROR_SOURCE_INTERCLUSTER_BUS_IBUS_ROUTING_ERROR             = (41),      ///< Inter-cluster Bus (I-Bus) routing error
    ECM_ERROR_SOURCE_PERIPHERAL_BUS_PBUS_ROUTING_ERROR               = (42),      ///< Peripheral Bus (P-Bus) routing error
    ECM_ERROR_SOURCE_CRAM_BUS_ROUTING_ERROR                          = (43),      ///< CRAM Bus routing error
    ECM_ERROR_SOURCE_SYSTEM_BUS_ROUTING_ERROR                        = (44),      ///< System Bus routing error
    ECM_ERROR_SOURCE_GLOBAL_FLASH_BUS_ROUTING_ERROR                  = (45),      ///< Global Flash Bus routing error
    ECM_ERROR_SOURCE_LOCAL_FLASH_BUS_ROUTING_ERROR                   = (46),      ///< Local Flash Bus routing error
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_MOSC                = (48),      ///< Clock monitor error for CLK_MOSC
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_WDT                 = (49),      ///< Clock monitor error for CLK_WDT
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_LSIOSC              = (50),      ///< Clock monitor error for CLK_LSIOSC
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_LSB                 = (51),      ///< Clock monitor error for CLK_LSB
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_UHSB                = (52),      ///< Clock monitor error for CLK_UHSB
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_HBUS                = (53),      ///< Clock monitor error for CLK_HBUS
    ECM_ERROR_SOURCE_OS_TIMER_1_INTERRUPT                            = (56),      ///< OS timer 1 interrupt
    ECM_ERROR_SOURCE_OS_TIMER_2_INTERRUPT                            = (57),      ///< OS timer 2 interrupt
    ECM_ERROR_SOURCE_OS_TIMER_3_INTERRUPT                            = (58),      ///< OS timer 3 interrupt
    ECM_ERROR_SOURCE_OS_TIMER_4_INTERRUPT                            = (59),      ///< OS timer 4 interrupt
    ECM_ERROR_SOURCE_OS_TIMER_5_INTERRUPT                            = (60),      ///< OS timer 5 interrupt
    ECM_ERROR_SOURCE_OS_TIMER_6_INTERRUPT                            = (61),      ///< OS timer 6 interrupt
    ECM_ERROR_SOURCE_OS_TIMER_7_INTERRUPT                            = (62),      ///< OS timer 7 interrupt
    ECM_ERROR_SOURCE_OS_TIMER_8_INTERRUPT                            = (63),      ///< OS timer 8 interrupt
    ECM_ERROR_SOURCE_OS_TIMER_9_INTERRUPT                            = (64),      ///< OS timer 9 interrupt
    ECM_ERROR_SOURCE_AD_CONVERTER_PARITY_ERROR                       = (72),      ///< A/D converter parity error
    ECM_ERROR_SOURCE_TEMPERATURE_SENSOR_ERROR                        = (76),      ///< Temperature sensor error
    ECM_ERROR_SOURCE_CODE_FLASH_ADDRESS_PARITY_ERROR                 = (80),      ///< Code Flash - Address parity error
    ECM_ERROR_SOURCE_CODE_FLASH_DATA_ECC_2BIT_ERROR                  = (81),      ///< Code Flash - Data ECC 2-bit error
    ECM_ERROR_SOURCE_CODE_FLASH_DATA_ECC_1BIT_ERROR                  = (82),      ///< Code Flash - Data ECC 1-bit error
    ECM_ERROR_SOURCE_CODE_FLASH_ECC_OVERFLOW_ERROR                   = (83),      ///< Code Flash - ECC overflow error
    ECM_ERROR_SOURCE_DATA_FLASH_DATA_ECC_2BIT_ERROR                  = (88),      ///< Data Flash - Data ECC 2-bit error
    ECM_ERROR_SOURCE_DATA_FLASH_DATA_ECC_1BIT_ERROR                  = (89),      ///< Data Flash - Data ECC 1-bit error
    ECM_ERROR_SOURCE_DATA_FLASH_ECC_OVERFLOW_ERROR                   = (90),      ///< Data Flash - ECC overflow error
    ECM_ERROR_SOURCE_LOCAL_RAM_DATA_ECC_2BIT_ERROR                   = (96),      ///< Local RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_DATA_ECC_1BIT_ERROR                   = (97),      ///< Local RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_ECC_OVERFLOW_ERROR                    = (98),      ///< Local RAM - ECC overflow error
    ECM_ERROR_SOURCE_CLUSTER_RAM_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT = (104),     ///< Cluster RAM - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_CLUSTER_RAM_DATA_ECC_1BIT_ERROR                 = (105),     ///< Cluster RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_CLUSTER_RAM_ECC_OVERFLOW_ERROR                  = (106),     ///< Cluster RAM - ECC overflow error
    ECM_ERROR_SOURCE_DTSRAM_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT      = (112),     ///< DTSRAM - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_DTSRAM_DATA_ECC_1BIT_ERROR                      = (113),     ///< DTSRAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_DTSRAM_ECC_OVERFLOW_ERROR                       = (114),     ///< DTSRAM - ECC overflow error
    ECM_ERROR_SOURCE_SDMAC0_RAM_DATA_ECC_2BIT_ERROR                  = (120),     ///< sDMAC0 RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_SDMAC0_RAM_DATA_ECC_1BIT_ERROR                  = (121),     ///< sDMAC0 RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_SDMAC1_RAM_DATA_ECC_2BIT_ERROR                  = (122),     ///< sDMAC1 RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_SDMAC1_RAM_DATA_ECC_1BIT_ERROR                  = (123),     ///< sDMAC1 RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_FLEXRAY_RAM_DATA_ECC_2BIT_ERROR                 = (128),     ///< FlexRay RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_FLEXRAY_RAM_DATA_ECC_1BIT_ERROR                 = (129),     ///< FlexRay RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_RSCANFD_RAM_DATA_ECC_2BIT_ERROR                 = (130),     ///< RS-CANFD RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_RSCANFD_RAM_DATA_ECC_1BIT_ERROR                 = (131),     ///< RS-CANFD RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_MSPI_RAM_DATA_ECC_2BIT_ERROR                    = (132),     ///< MSPI RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_MSPI_RAM_DATA_ECC_1BIT_ERROR                    = (133),     ///< MSPI RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_GTM_RAM_DATA_ECC_2BIT_ERROR                     = (134),     ///< GTM RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_GTM_RAM_DATA_ECC_1BIT_ERROR                     = (135),     ///< GTM RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_FAST_ETHERNET_RAM_DATA_ECC_2BIT_ERROR           = (136),     ///< Fast Ethernet RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_FAST_ETHERNET_RAM_DATA_ECC_1BIT_ERROR           = (137),     ///< Fast Ethernet RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_GIGABIT_ETHERNET_RAM_DATA_ECC_2BIT_ERROR        = (138),     ///< Gigabit Ethernet RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_GIGABIT_ETHERNET_RAM_DATA_ECC_1BIT_ERROR        = (139),     ///< Gigabit Ethernet RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_MMCA_RAM_DATA_ECC_2BIT_ERROR                    = (140),     ///< MMCA RAM - Data ECC 2-bit error
    ECM_ERROR_SOURCE_MMCA_RAM_DATA_ECC_1BIT_ERROR                    = (141),     ///< MMCA RAM - Data ECC 1-bit error
    ECM_ERROR_SOURCE_PERIPHERAL_RAM_ECC_OVERFLOW_ERROR               = (159),     ///< Peripheral RAM - ECC overflow error
    ECM_ERROR_SOURCE_DATA_TRANSFER_PATH_ADDR_EDC_ERROR               = (160),     ///< Data transfer path - Address EDC error
    ECM_ERROR_SOURCE_DATA_TRANSFER_PATH_DATA_ECC_2BIT_ERROR          = (161),     ///< Data transfer path - Data ECC 2-bit error
    ECM_ERROR_SOURCE_DATA_TRANSFER_PATH_DATA_ECC_1BIT_ERROR          = (162),     ///< Data transfer path - Data ECC 1-bit error
    ECM_ERROR_SOURCE_CRAM_GUARD_ERROR                                    = (168), ///< CRAM Guard error
    ECM_ERROR_SOURCE_IBUS_GUARD_ERROR                                    = (169), ///< I-Bus Guard error
    ECM_ERROR_SOURCE_PBUS_GUARD_ERROR                                    = (170), ///< P-Bus Guard error
    ECM_ERROR_SOURCE_HBUS_GUARD_ERROR                                    = (171), ///< H-Bus Guard error
    ECM_ERROR_SOURCE_DTS_TRANSFER_ERR_DMA_ADDR_ERR                       = (182), ///< DTS transfer error/DMA address error
    ECM_ERROR_SOURCE_HBUS_MASTER_TRANSFER_ERR                            = (183), ///< H-Bus master Transfer Error
    ECM_ERROR_SOURCE_EXTERNAL_ERROR_INPUT_0                              = (184), ///< External Error Input 0
    ECM_ERROR_SOURCE_EXTERNAL_ERROR_INPUT_1                              = (185), ///< External Error Input 1
    ECM_ERROR_SOURCE_EXTERNAL_ERROR_INPUT_2                              = (186), ///< External Error Input 2
    ECM_ERROR_SOURCE_EXTERNAL_ERROR_INPUT_3                              = (187), ///< External Error Input 3
    ECM_ERROR_SOURCE_SOFTWARE_ALARM_0                                    = (192), ///< Software Alarm 0
    ECM_ERROR_SOURCE_SOFTWARE_ALARM_1                                    = (193), ///< Software Alarm 1
    ECM_ERROR_SOURCE_SOFTWARE_ALARM_2                                    = (194), ///< Software Alarm 2
    ECM_ERROR_SOURCE_SOFTWARE_ALARM_3                                    = (195), ///< Software Alarm 3
    ECM_ERROR_SOURCE_DCLS_COMPARE_ERROR_PE0                              = (224), ///< DCLS compare error (PE0)
    ECM_ERROR_SOURCE_UNINTENDED_DEBUG_ENABLE_DETECTION_PE0               = (227), ///< Unintended Debug Enable detection (PE0)
    ECM_ERROR_SOURCE_WATCHDOG_TIMER_ERROR_PE0                            = (228), ///< Watchdog timer error (PE0)
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_CPU_PE0                 = (229), ///< Clock monitor error for CLK_CPU (PE0)
    ECM_ERROR_SOURCE_LOCAL_RAM_PE0_ADDR_FEEDBACK_CMP_ERROR_DATA_ECC_2BIT = (232), ///< Local RAM (PE0) - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE0_DATA_ECC_1BIT_ERROR                   = (233), ///< Local RAM (PE0) - Data ECC 1-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE0_ECC_OVERFLOW_ERROR                    = (234), ///< Local RAM (PE0) - ECC overflow error
    ECM_ERROR_SOURCE_INS_CACHE_RAM_PE0_ADDR_FEEDBACK_CMP_ERR_DATA_EDC    = (236), ///< Instruction cache RAM (PE0) - Address feedback compare error - Data EDC error
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_PE0                                  = (240), ///< PE guard error (PE0)
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_BY_PE0_READ_ACCESS_PE0               = (241), ///< PE guard error by PE0 read access (PE0)
    ECM_ERROR_SOURCE_DATA_ACCESS_ERROR_PE0_7                             = (255), ///< Data Access Error (PE0) *7
    ECM_ERROR_SOURCE_DCLS_COMPARE_ERROR_PE1                              = (256), ///< DCLS compare error (PE1)
    ECM_ERROR_SOURCE_UNINTENDED_DEBUG_ENABLE_DETECTION_PE1               = (259), ///< Unintended Debug Enable detection (PE1)
    ECM_ERROR_SOURCE_WATCHDOG_TIMER_ERROR_PE1                            = (260), ///< Watchdog timer error (PE1)
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_CPU_PE1                 = (261), ///< Clock monitor error for CLK_CPU (PE1)
    ECM_ERROR_SOURCE_LOCAL_RAM_PE1_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT   = (264), ///< Local RAM (PE1) - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE1_DATA_ECC_1BIT_ERROR                   = (265), ///< Local RAM (PE1) - Data ECC 1-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE1_ECC_OVERFLOW_ERROR                    = (266), ///< Local RAM (PE1) - ECC overflow error
    ECM_ERROR_SOURCE_INS_CACHE_RAM_PE1_ADDR_FEEDBACK_CMP_ERR_DATA_EDC    = (268), ///< Instruction cache RAM (PE1) - Address feedback compare error - Data EDC error
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_PE1                                  = (272), ///< PE guard error (PE1)
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_BY_PE1_READ_ACCESS_PE1               = (273), ///< PE guard error by PE1 read access (PE1)
    ECM_ERROR_SOURCE_DATA_ACCESS_ERROR_PE1_7                             = (287), ///< Data Access Error (PE1) *7
    ECM_ERROR_SOURCE_DCLS_COMPARE_ERROR_PE2                              = (288), ///< DCLS compare error (PE2)
    ECM_ERROR_SOURCE_UNINTENDED_DEBUG_ENABLE_DETECTION_PE2               = (291), ///< Unintended Debug Enable detection (PE2)
    ECM_ERROR_SOURCE_WATCHDOG_TIMER_ERROR_PE2                            = (292), ///< Watchdog timer error (PE2)
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_CPU_PE2                 = (293), ///< Clock monitor error for CLK_CPU (PE2)
    ECM_ERROR_SOURCE_LOCAL_RAM_PE2_ADDR_FEEDBACK_CMP_ERR_DATA_ECC_2BIT   = (296), ///< Local RAM (PE2) - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE2_DATA_ECC_1BIT_ERROR                   = (297), ///< Local RAM (PE2) - Data ECC 1-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE2_ECC_OVERFLOW_ERROR                    = (298), ///< Local RAM (PE2) - ECC overflow error
    ECM_ERROR_SOURCE_INS_CACHE_RAM_PE2_ADDR_FEEDBACK_CMP_ERR_DATA_EDC    = (300), ///< Instruction cache RAM (PE2) - Address feedback compare error - Data EDC error
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_PE2                                  = (304), ///< PE guard error (PE2)
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_BY_PE2_READ_ACCESS_PE2               = (305), ///< PE guard error by PE2 read access (PE2)
    ECM_ERROR_SOURCE_DATA_ACCESS_ERROR_PE2_7                             = (319), ///< Data Access Error (PE2) *7
    ECM_ERROR_SOURCE_DCLS_COMPARE_ERROR_PE3                              = (320), ///< DCLS compare error (PE3)
    ECM_ERROR_SOURCE_UNINTENDED_DEBUG_ENABLE_DETECTION_PE3               = (323), ///< Unintended Debug Enable detection (PE3)
    ECM_ERROR_SOURCE_WATCHDOG_TIMER_ERROR_PE3                            = (324), ///< Watchdog timer error (PE3)
    ECM_ERROR_SOURCE_CLOCK_MONITOR_ERROR_FOR_CLK_CPU_PE3                 = (325), ///< Clock monitor error for CLK_CPU (PE3)
    ECM_ERROR_SOURCE_LOCAL_RAM_PE3_ADDR_FEEDBACK_CMP_ERROR_DATA_ECC_2BIT = (328), ///< Local RAM (PE3) - Address feedback compare error - Data ECC 2-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE3_DATA_ECC_1BIT_ERROR                   = (329), ///< Local RAM (PE3) - Data ECC 1-bit error
    ECM_ERROR_SOURCE_LOCAL_RAM_PE3_ECC_OVERFLOW_ERROR                    = (330), ///< Local RAM (PE3) - ECC overflow error
    ECM_ERROR_SOURCE_INS_CACHE_RAM_PE3_ADDR_FEEDBACK_CMP_ERR_DATA_EDC    = (332), ///< Instruction cache RAM (PE3) - Address feedback compare error - Data EDC error
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_PE3                                  = (336), ///< PE guard error (PE3)
    ECM_ERROR_SOURCE_PE_GUARD_ERROR_BY_PE3_READ_ACCESS_PE3               = (337), ///< PE guard error by PE3 read access (PE3)
    ECM_ERROR_SOURCE_DATA_ACCESS_ERRORPE37                               = (351), ///< Data Access Error(PE3)*7
} ecm_error_source_t;

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

#endif                                 /* BSP_ECM_H */

/** @} (end addtogroup BSP_MCU_RH850U2A16) */
