/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_IO_DEVICE_H
#define BSP_IO_DEVICE_H

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2C8
 * @defgroup BSP_IO_U2C8 BSP I/O access
 * @brief This module provides basic read/write access to port pins.
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define R_BSP_JPORT                ((R_PORT_Type *) 0xFFDA0000)             ///< Base address of JPORT instance
#define R_BSP_APORT                ((R_PORT_Type *) (R_PORT_BASE + 0x0C80)) ///< Base address of APORT instance

#define BSP_PACKAGE_PINS_BGA292    (292U)                                   ///< Package pin count: BGA292

#define BSP_IO_PORT_P02_PWE        (2)                                      ///< PWE value for P02
#define BSP_IO_PORT_P03_PWE        (3)                                      ///< PWE value for P03
#define BSP_IO_PORT_P04_PWE        (4)                                      ///< PWE value for P04
#define BSP_IO_PORT_P06_PWE        (6)                                      ///< PWE value for P06
#define BSP_IO_PORT_P08_PWE        (7)                                      ///< PWE value for P08
#define BSP_IO_PORT_P10_PWE        (8)                                      ///< PWE value for P10
#define BSP_IO_PORT_P17_PWE        (10)                                     ///< PWE value for P17
#define BSP_IO_PORT_P20_PWE        (12)                                     ///< PWE value for P20
#define BSP_IO_PORT_P21_PWE        (13)                                     ///< PWE value for P21
#define BSP_IO_PORT_P22_PWE        (14)                                     ///< PWE value for P22
#define BSP_IO_PORT_P24_PWE        (15)                                     ///< PWE value for P24

#define BSP_IO_PORT_AP00_PWE       (20)                                     ///< PWE value for AP00
#define BSP_IO_PORT_AP01_PWE       (21)                                     ///< PWE value for AP01
#define BSP_IO_PORT_AP02_PWE       (22)                                     ///< PWE value for AP02
#define BSP_IO_PORT_AP03_PWE       (23)                                     ///< PWE value for AP03
#define BSP_IO_PORT_AP04_PWE       (24)                                     ///< PWE value for AP04

#define BSP_IO_PORT_JP00_PWE       (255)                                    ///< PWE value for JP00
#define BSP_IO_PORT_P27_PWE        (16)                                     ///< PWE value for P27
#define BSP_IO_PORT_IP00_PWE       (255)                                    ///< PWE value for IP00

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Superset list of all possible IO ports. */
/** BSP_IO_PORT_[TYPE][GROUP] = [PWE] << 24U | [TYPE] << 16U | [GROUP] << 8U | [PORT] */
typedef enum e_bsp_io_port
{
    BSP_IO_PORT_P02  = ((BSP_IO_PORT_P02_PWE << 24U) | (0 << 16U) | (2 << 8U) | 0),  ///< IO port P02
    BSP_IO_PORT_P03  = ((BSP_IO_PORT_P03_PWE << 24U) | (0 << 16U) | (3 << 8U) | 0),  ///< IO port P03
    BSP_IO_PORT_P04  = ((BSP_IO_PORT_P04_PWE << 24U) | (0 << 16U) | (4 << 8U) | 0),  ///< IO port P04
    BSP_IO_PORT_P06  = ((BSP_IO_PORT_P06_PWE << 24U) | (0 << 16U) | (6 << 8U) | 0),  ///< IO port P06
    BSP_IO_PORT_P08  = ((BSP_IO_PORT_P08_PWE << 24U) | (0 << 16U) | (8 << 8U) | 0),  ///< IO port P08
    BSP_IO_PORT_P10  = ((BSP_IO_PORT_P10_PWE << 24U) | (0 << 16U) | (10 << 8U) | 0), ///< IO port P10
    BSP_IO_PORT_P17  = ((BSP_IO_PORT_P17_PWE << 24U) | (0 << 16U) | (17 << 8U) | 0), ///< IO port P17
    BSP_IO_PORT_P20  = ((BSP_IO_PORT_P20_PWE << 24U) | (0 << 16U) | (20 << 8U) | 0), ///< IO port P20
    BSP_IO_PORT_P21  = ((BSP_IO_PORT_P21_PWE << 24U) | (0 << 16U) | (21 << 8U) | 0), ///< IO port P21
    BSP_IO_PORT_P22  = ((BSP_IO_PORT_P22_PWE << 24U) | (0 << 16U) | (22 << 8U) | 0), ///< IO port P22
    BSP_IO_PORT_P24  = ((BSP_IO_PORT_P24_PWE << 24U) | (0 << 16U) | (24 << 8U) | 0), ///< IO port P24
    BSP_IO_PORT_AP00 = ((BSP_IO_PORT_AP00_PWE << 24U) | (2 << 16U) | (0 << 8U) | 0), ///< IO port AP00
    BSP_IO_PORT_AP01 = ((BSP_IO_PORT_AP01_PWE << 24U) | (2 << 16U) | (1 << 8U) | 0), ///< IO port AP01
    BSP_IO_PORT_AP02 = ((BSP_IO_PORT_AP02_PWE << 24U) | (2 << 16U) | (2 << 8U) | 0), ///< IO port AP02
    BSP_IO_PORT_AP03 = ((BSP_IO_PORT_AP03_PWE << 24U) | (2 << 16U) | (3 << 8U) | 0), ///< IO port AP03
    BSP_IO_PORT_AP04 = ((BSP_IO_PORT_AP04_PWE << 24U) | (2 << 16U) | (4 << 8U) | 0), ///< IO port AP04
    BSP_IO_PORT_JP00 = ((BSP_IO_PORT_JP00_PWE << 24U) | (1 << 16U) | (0 << 8U) | 0), ///< IO port JP00
    BSP_IO_PORT_P27  = ((BSP_IO_PORT_P27_PWE << 24U) | (0 << 16U) | (27 << 8U) | 0), ///< IO port P27
    BSP_IO_PORT_IP00 = ((BSP_IO_PORT_IP00_PWE << 24U) | (3 << 16U) | (0 << 8U) | 0), ///< IO port IP00
} bsp_io_port_t;

/** Superset list of all possible IO port pins. */
typedef enum e_bsp_io_port_pin_t
{
    BSP_IO_PORT_P02_PIN_0  = (BSP_IO_PORT_P02 | 0x0),   ///< IO port P02 pin 0
    BSP_IO_PORT_P02_PIN_1  = (BSP_IO_PORT_P02 | 0x1),   ///< IO port P02 pin 1
    BSP_IO_PORT_P02_PIN_2  = (BSP_IO_PORT_P02 | 0x2),   ///< IO port P02 pin 2
    BSP_IO_PORT_P02_PIN_3  = (BSP_IO_PORT_P02 | 0x3),   ///< IO port P02 pin 3
    BSP_IO_PORT_P02_PIN_4  = (BSP_IO_PORT_P02 | 0x4),   ///< IO port P02 pin 4
    BSP_IO_PORT_P02_PIN_5  = (BSP_IO_PORT_P02 | 0x5),   ///< IO port P02 pin 5
    BSP_IO_PORT_P02_PIN_6  = (BSP_IO_PORT_P02 | 0x6),   ///< IO port P02 pin 6
    BSP_IO_PORT_P02_PIN_7  = (BSP_IO_PORT_P02 | 0x7),   ///< IO port P02 pin 7
    BSP_IO_PORT_P02_PIN_8  = (BSP_IO_PORT_P02 | 0x8),   ///< IO port P02 pin 8
    BSP_IO_PORT_P02_PIN_9  = (BSP_IO_PORT_P02 | 0x9),   ///< IO port P02 pin 9
    BSP_IO_PORT_P02_PIN_10 = (BSP_IO_PORT_P02 | 0xA),   ///< IO port P02 pin 10
    BSP_IO_PORT_P02_PIN_11 = (BSP_IO_PORT_P02 | 0xB),   ///< IO port P02 pin 11
    BSP_IO_PORT_P02_PIN_12 = (BSP_IO_PORT_P02 | 0xC),   ///< IO port P02 pin 12
    BSP_IO_PORT_P02_PIN_13 = (BSP_IO_PORT_P02 | 0xD),   ///< IO port P02 pin 13
    BSP_IO_PORT_P02_PIN_14 = (BSP_IO_PORT_P02 | 0xE),   ///< IO port P02 pin 14

    BSP_IO_PORT_P03_PIN_0  = (BSP_IO_PORT_P03 | 0x0),   ///< IO port P03 pin 0
    BSP_IO_PORT_P03_PIN_1  = (BSP_IO_PORT_P03 | 0x1),   ///< IO port P03 pin 1
    BSP_IO_PORT_P03_PIN_10 = (BSP_IO_PORT_P03 | 0xA),   ///< IO port P03 pin 10
    BSP_IO_PORT_P03_PIN_11 = (BSP_IO_PORT_P03 | 0xB),   ///< IO port P03 pin 11
    BSP_IO_PORT_P03_PIN_12 = (BSP_IO_PORT_P03 | 0xC),   ///< IO port P03 pin 12

    BSP_IO_PORT_P04_PIN_0  = (BSP_IO_PORT_P04 | 0x0),   ///< IO port P04 pin 0
    BSP_IO_PORT_P04_PIN_1  = (BSP_IO_PORT_P04 | 0x1),   ///< IO port P04 pin 1
    BSP_IO_PORT_P04_PIN_2  = (BSP_IO_PORT_P04 | 0x2),   ///< IO port P04 pin 2
    BSP_IO_PORT_P04_PIN_3  = (BSP_IO_PORT_P04 | 0x3),   ///< IO port P04 pin 3
    BSP_IO_PORT_P04_PIN_4  = (BSP_IO_PORT_P04 | 0x4),   ///< IO port P04 pin 4
    BSP_IO_PORT_P04_PIN_5  = (BSP_IO_PORT_P04 | 0x5),   ///< IO port P04 pin 5
    BSP_IO_PORT_P04_PIN_6  = (BSP_IO_PORT_P04 | 0x6),   ///< IO port P04 pin 6
    BSP_IO_PORT_P04_PIN_7  = (BSP_IO_PORT_P04 | 0x7),   ///< IO port P04 pin 7
    BSP_IO_PORT_P04_PIN_8  = (BSP_IO_PORT_P04 | 0x8),   ///< IO port P04 pin 8
    BSP_IO_PORT_P04_PIN_9  = (BSP_IO_PORT_P04 | 0x9),   ///< IO port P04 pin 9
    BSP_IO_PORT_P04_PIN_10 = (BSP_IO_PORT_P04 | 0xA),   ///< IO port P04 pin 10
    BSP_IO_PORT_P04_PIN_11 = (BSP_IO_PORT_P04 | 0xB),   ///< IO port P04 pin 11
    BSP_IO_PORT_P04_PIN_12 = (BSP_IO_PORT_P04 | 0xC),   ///< IO port P04 pin 12
    BSP_IO_PORT_P04_PIN_13 = (BSP_IO_PORT_P04 | 0xD),   ///< IO port P04 pin 13
    BSP_IO_PORT_P04_PIN_14 = (BSP_IO_PORT_P04 | 0xE),   ///< IO port P04 pin 14
    BSP_IO_PORT_P04_PIN_15 = (BSP_IO_PORT_P04 | 0xF),   ///< IO port P04 pin 15

    BSP_IO_PORT_P06_PIN_0  = (BSP_IO_PORT_P06 | 0x0),   ///< IO port P06 pin 0
    BSP_IO_PORT_P06_PIN_1  = (BSP_IO_PORT_P06 | 0x1),   ///< IO port P06 pin 1
    BSP_IO_PORT_P06_PIN_2  = (BSP_IO_PORT_P06 | 0x2),   ///< IO port P06 pin 2
    BSP_IO_PORT_P06_PIN_3  = (BSP_IO_PORT_P06 | 0x3),   ///< IO port P06 pin 3
    BSP_IO_PORT_P06_PIN_4  = (BSP_IO_PORT_P06 | 0x4),   ///< IO port P06 pin 4
    BSP_IO_PORT_P06_PIN_5  = (BSP_IO_PORT_P06 | 0x5),   ///< IO port P06 pin 5
    BSP_IO_PORT_P06_PIN_6  = (BSP_IO_PORT_P06 | 0x6),   ///< IO port P06 pin 6
    BSP_IO_PORT_P06_PIN_7  = (BSP_IO_PORT_P06 | 0x7),   ///< IO port P06 pin 7
    BSP_IO_PORT_P06_PIN_8  = (BSP_IO_PORT_P06 | 0x8),   ///< IO port P06 pin 8
    BSP_IO_PORT_P06_PIN_9  = (BSP_IO_PORT_P06 | 0x9),   ///< IO port P06 pin 9
    BSP_IO_PORT_P06_PIN_10 = (BSP_IO_PORT_P06 | 0xA),   ///< IO port P06 pin 10
    BSP_IO_PORT_P06_PIN_11 = (BSP_IO_PORT_P06 | 0xB),   ///< IO port P06 pin 11
    BSP_IO_PORT_P06_PIN_12 = (BSP_IO_PORT_P06 | 0xC),   ///< IO port P06 pin 12
    BSP_IO_PORT_P06_PIN_13 = (BSP_IO_PORT_P06 | 0xD),   ///< IO port P06 pin 13

    BSP_IO_PORT_P08_PIN_0 = (BSP_IO_PORT_P08 | 0x0),    ///< IO port P08 pin 0
    BSP_IO_PORT_P08_PIN_1 = (BSP_IO_PORT_P08 | 0x1),    ///< IO port P08 pin 1
    BSP_IO_PORT_P08_PIN_2 = (BSP_IO_PORT_P08 | 0x2),    ///< IO port P08 pin 2
    BSP_IO_PORT_P08_PIN_3 = (BSP_IO_PORT_P08 | 0x3),    ///< IO port P08 pin 3

    BSP_IO_PORT_P10_PIN_0  = (BSP_IO_PORT_P10 | 0x0),   ///< IO port P10 pin 0
    BSP_IO_PORT_P10_PIN_1  = (BSP_IO_PORT_P10 | 0x1),   ///< IO port P10 pin 1
    BSP_IO_PORT_P10_PIN_2  = (BSP_IO_PORT_P10 | 0x2),   ///< IO port P10 pin 2
    BSP_IO_PORT_P10_PIN_3  = (BSP_IO_PORT_P10 | 0x3),   ///< IO port P10 pin 3
    BSP_IO_PORT_P10_PIN_4  = (BSP_IO_PORT_P10 | 0x4),   ///< IO port P10 pin 4
    BSP_IO_PORT_P10_PIN_5  = (BSP_IO_PORT_P10 | 0x5),   ///< IO port P10 pin 5
    BSP_IO_PORT_P10_PIN_6  = (BSP_IO_PORT_P10 | 0x6),   ///< IO port P10 pin 6
    BSP_IO_PORT_P10_PIN_7  = (BSP_IO_PORT_P10 | 0x7),   ///< IO port P10 pin 7
    BSP_IO_PORT_P10_PIN_8  = (BSP_IO_PORT_P10 | 0x8),   ///< IO port P10 pin 8
    BSP_IO_PORT_P10_PIN_9  = (BSP_IO_PORT_P10 | 0x9),   ///< IO port P10 pin 9
    BSP_IO_PORT_P10_PIN_10 = (BSP_IO_PORT_P10 | 0xA),   ///< IO port P10 pin 10
    BSP_IO_PORT_P10_PIN_11 = (BSP_IO_PORT_P10 | 0xB),   ///< IO port P10 pin 11
    BSP_IO_PORT_P10_PIN_12 = (BSP_IO_PORT_P10 | 0xC),   ///< IO port P10 pin 12
    BSP_IO_PORT_P10_PIN_13 = (BSP_IO_PORT_P10 | 0xD),   ///< IO port P10 pin 13
    BSP_IO_PORT_P10_PIN_14 = (BSP_IO_PORT_P10 | 0xE),   ///< IO port P10 pin 14

    BSP_IO_PORT_P17_PIN_0  = (BSP_IO_PORT_P17 | 0x0),   ///< IO port P17 pin 0
    BSP_IO_PORT_P17_PIN_1  = (BSP_IO_PORT_P17 | 0x1),   ///< IO port P17 pin 1
    BSP_IO_PORT_P17_PIN_2  = (BSP_IO_PORT_P17 | 0x2),   ///< IO port P17 pin 2
    BSP_IO_PORT_P17_PIN_3  = (BSP_IO_PORT_P17 | 0x3),   ///< IO port P17 pin 3
    BSP_IO_PORT_P17_PIN_4  = (BSP_IO_PORT_P17 | 0x4),   ///< IO port P17 pin 4
    BSP_IO_PORT_P17_PIN_5  = (BSP_IO_PORT_P17 | 0x5),   ///< IO port P17 pin 5
    BSP_IO_PORT_P17_PIN_6  = (BSP_IO_PORT_P17 | 0x6),   ///< IO port P17 pin 6
    BSP_IO_PORT_P17_PIN_7  = (BSP_IO_PORT_P17 | 0x7),   ///< IO port P17 pin 7
    BSP_IO_PORT_P17_PIN_8  = (BSP_IO_PORT_P17 | 0x8),   ///< IO port P17 pin 8
    BSP_IO_PORT_P17_PIN_9  = (BSP_IO_PORT_P17 | 0x9),   ///< IO port P17 pin 9
    BSP_IO_PORT_P17_PIN_10 = (BSP_IO_PORT_P17 | 0xA),   ///< IO port P17 pin 10
    BSP_IO_PORT_P17_PIN_11 = (BSP_IO_PORT_P17 | 0xB),   ///< IO port P17 pin 11
    BSP_IO_PORT_P17_PIN_12 = (BSP_IO_PORT_P17 | 0xC),   ///< IO port P17 pin 12
    BSP_IO_PORT_P17_PIN_13 = (BSP_IO_PORT_P17 | 0xD),   ///< IO port P17 pin 13

    BSP_IO_PORT_P20_PIN_0  = (BSP_IO_PORT_P20 | 0x0),   ///< IO port P20 pin 0
    BSP_IO_PORT_P20_PIN_1  = (BSP_IO_PORT_P20 | 0x1),   ///< IO port P20 pin 1
    BSP_IO_PORT_P20_PIN_2  = (BSP_IO_PORT_P20 | 0x2),   ///< IO port P20 pin 2
    BSP_IO_PORT_P20_PIN_3  = (BSP_IO_PORT_P20 | 0x3),   ///< IO port P20 pin 3
    BSP_IO_PORT_P20_PIN_4  = (BSP_IO_PORT_P20 | 0x4),   ///< IO port P20 pin 4
    BSP_IO_PORT_P20_PIN_5  = (BSP_IO_PORT_P20 | 0x5),   ///< IO port P20 pin 5
    BSP_IO_PORT_P20_PIN_6  = (BSP_IO_PORT_P20 | 0x6),   ///< IO port P20 pin 6
    BSP_IO_PORT_P20_PIN_7  = (BSP_IO_PORT_P20 | 0x7),   ///< IO port P20 pin 7
    BSP_IO_PORT_P20_PIN_8  = (BSP_IO_PORT_P20 | 0x8),   ///< IO port P20 pin 8
    BSP_IO_PORT_P20_PIN_9  = (BSP_IO_PORT_P20 | 0x9),   ///< IO port P20 pin 9
    BSP_IO_PORT_P20_PIN_10 = (BSP_IO_PORT_P20 | 0xA),   ///< IO port P20 pin 10
    BSP_IO_PORT_P20_PIN_11 = (BSP_IO_PORT_P20 | 0xB),   ///< IO port P20 pin 11
    BSP_IO_PORT_P20_PIN_12 = (BSP_IO_PORT_P20 | 0xC),   ///< IO port P20 pin 12
    BSP_IO_PORT_P20_PIN_13 = (BSP_IO_PORT_P20 | 0xD),   ///< IO port P20 pin 13

    BSP_IO_PORT_P21_PIN_0  = (BSP_IO_PORT_P21 | 0x0),   ///< IO port P21 pin 0
    BSP_IO_PORT_P21_PIN_1  = (BSP_IO_PORT_P21 | 0x1),   ///< IO port P21 pin 1
    BSP_IO_PORT_P21_PIN_2  = (BSP_IO_PORT_P21 | 0x2),   ///< IO port P21 pin 2
    BSP_IO_PORT_P21_PIN_3  = (BSP_IO_PORT_P21 | 0x3),   ///< IO port P21 pin 3
    BSP_IO_PORT_P21_PIN_4  = (BSP_IO_PORT_P21 | 0x4),   ///< IO port P21 pin 4
    BSP_IO_PORT_P21_PIN_5  = (BSP_IO_PORT_P21 | 0x5),   ///< IO port P21 pin 5
    BSP_IO_PORT_P21_PIN_6  = (BSP_IO_PORT_P21 | 0x6),   ///< IO port P21 pin 6
    BSP_IO_PORT_P21_PIN_7  = (BSP_IO_PORT_P21 | 0x7),   ///< IO port P21 pin 7
    BSP_IO_PORT_P21_PIN_8  = (BSP_IO_PORT_P21 | 0x8),   ///< IO port P21 pin 8
    BSP_IO_PORT_P21_PIN_9  = (BSP_IO_PORT_P21 | 0x9),   ///< IO port P21 pin 9
    BSP_IO_PORT_P21_PIN_10 = (BSP_IO_PORT_P21 | 0xA),   ///< IO port P21 pin 10

    BSP_IO_PORT_P22_PIN_0 = (BSP_IO_PORT_P22 | 0x0),    ///< IO port P22 pin 0
    BSP_IO_PORT_P22_PIN_1 = (BSP_IO_PORT_P22 | 0x1),    ///< IO port P22 pin 1
    BSP_IO_PORT_P22_PIN_2 = (BSP_IO_PORT_P22 | 0x2),    ///< IO port P22 pin 2
    BSP_IO_PORT_P22_PIN_3 = (BSP_IO_PORT_P22 | 0x3),    ///< IO port P22 pin 3
    BSP_IO_PORT_P22_PIN_4 = (BSP_IO_PORT_P22 | 0x4),    ///< IO port P22 pin 4
    BSP_IO_PORT_P22_PIN_5 = (BSP_IO_PORT_P22 | 0x5),    ///< IO port P22 pin 5
    BSP_IO_PORT_P22_PIN_6 = (BSP_IO_PORT_P22 | 0x6),    ///< IO port P22 pin 6

    BSP_IO_PORT_P24_PIN_4  = (BSP_IO_PORT_P24 | 0x4),   ///< IO port P24 pin 4
    BSP_IO_PORT_P24_PIN_5  = (BSP_IO_PORT_P24 | 0x5),   ///< IO port P24 pin 5
    BSP_IO_PORT_P24_PIN_6  = (BSP_IO_PORT_P24 | 0x6),   ///< IO port P24 pin 6
    BSP_IO_PORT_P24_PIN_7  = (BSP_IO_PORT_P24 | 0x7),   ///< IO port P24 pin 7
    BSP_IO_PORT_P24_PIN_8  = (BSP_IO_PORT_P24 | 0x8),   ///< IO port P24 pin 8
    BSP_IO_PORT_P24_PIN_9  = (BSP_IO_PORT_P24 | 0x9),   ///< IO port P24 pin 9
    BSP_IO_PORT_P24_PIN_10 = (BSP_IO_PORT_P24 | 0xA),   ///< IO port P24 pin 10
    BSP_IO_PORT_P24_PIN_11 = (BSP_IO_PORT_P24 | 0xB),   ///< IO port P24 pin 11
    BSP_IO_PORT_P24_PIN_12 = (BSP_IO_PORT_P24 | 0xC),   ///< IO port P24 pin 12
    BSP_IO_PORT_P24_PIN_13 = (BSP_IO_PORT_P24 | 0xD),   ///< IO port P24 pin 13

    BSP_IO_PORT_AP00_PIN_0  = (BSP_IO_PORT_AP00 | 0x0), ///< IO port AP00 pin 0
    BSP_IO_PORT_AP00_PIN_1  = (BSP_IO_PORT_AP00 | 0x1), ///< IO port AP00 pin 1
    BSP_IO_PORT_AP00_PIN_2  = (BSP_IO_PORT_AP00 | 0x2), ///< IO port AP00 pin 2
    BSP_IO_PORT_AP00_PIN_3  = (BSP_IO_PORT_AP00 | 0x3), ///< IO port AP00 pin 3
    BSP_IO_PORT_AP00_PIN_4  = (BSP_IO_PORT_AP00 | 0x4), ///< IO port AP00 pin 4
    BSP_IO_PORT_AP00_PIN_5  = (BSP_IO_PORT_AP00 | 0x5), ///< IO port AP00 pin 5
    BSP_IO_PORT_AP00_PIN_6  = (BSP_IO_PORT_AP00 | 0x6), ///< IO port AP00 pin 6
    BSP_IO_PORT_AP00_PIN_7  = (BSP_IO_PORT_AP00 | 0x7), ///< IO port AP00 pin 7
    BSP_IO_PORT_AP00_PIN_8  = (BSP_IO_PORT_AP00 | 0x8), ///< IO port AP00 pin 8
    BSP_IO_PORT_AP00_PIN_9  = (BSP_IO_PORT_AP00 | 0x9), ///< IO port AP00 pin 9
    BSP_IO_PORT_AP00_PIN_10 = (BSP_IO_PORT_AP00 | 0xA), ///< IO port AP00 pin 10
    BSP_IO_PORT_AP00_PIN_11 = (BSP_IO_PORT_AP00 | 0xB), ///< IO port AP00 pin 11
    BSP_IO_PORT_AP00_PIN_12 = (BSP_IO_PORT_AP00 | 0xC), ///< IO port AP00 pin 12
    BSP_IO_PORT_AP00_PIN_13 = (BSP_IO_PORT_AP00 | 0xD), ///< IO port AP00 pin 13
    BSP_IO_PORT_AP00_PIN_14 = (BSP_IO_PORT_AP00 | 0xE), ///< IO port AP00 pin 14
    BSP_IO_PORT_AP00_PIN_15 = (BSP_IO_PORT_AP00 | 0xF), ///< IO port AP00 pin 15

    BSP_IO_PORT_AP01_PIN_0 = (BSP_IO_PORT_AP01 | 0x0),  ///< IO port AP01 pin 0
    BSP_IO_PORT_AP01_PIN_1 = (BSP_IO_PORT_AP01 | 0x1),  ///< IO port AP01 pin 1
    BSP_IO_PORT_AP01_PIN_2 = (BSP_IO_PORT_AP01 | 0x2),  ///< IO port AP01 pin 2
    BSP_IO_PORT_AP01_PIN_3 = (BSP_IO_PORT_AP01 | 0x3),  ///< IO port AP01 pin 3

    BSP_IO_PORT_AP02_PIN_0  = (BSP_IO_PORT_AP02 | 0x0), ///< IO port AP02 pin 0
    BSP_IO_PORT_AP02_PIN_1  = (BSP_IO_PORT_AP02 | 0x1), ///< IO port AP02 pin 1
    BSP_IO_PORT_AP02_PIN_2  = (BSP_IO_PORT_AP02 | 0x2), ///< IO port AP02 pin 2
    BSP_IO_PORT_AP02_PIN_3  = (BSP_IO_PORT_AP02 | 0x3), ///< IO port AP02 pin 3
    BSP_IO_PORT_AP02_PIN_4  = (BSP_IO_PORT_AP02 | 0x4), ///< IO port AP02 pin 4
    BSP_IO_PORT_AP02_PIN_5  = (BSP_IO_PORT_AP02 | 0x5), ///< IO port AP02 pin 5
    BSP_IO_PORT_AP02_PIN_6  = (BSP_IO_PORT_AP02 | 0x6), ///< IO port AP02 pin 6
    BSP_IO_PORT_AP02_PIN_7  = (BSP_IO_PORT_AP02 | 0x7), ///< IO port AP02 pin 7
    BSP_IO_PORT_AP02_PIN_8  = (BSP_IO_PORT_AP02 | 0x8), ///< IO port AP02 pin 8
    BSP_IO_PORT_AP02_PIN_9  = (BSP_IO_PORT_AP02 | 0x9), ///< IO port AP02 pin 9
    BSP_IO_PORT_AP02_PIN_10 = (BSP_IO_PORT_AP02 | 0xA), ///< IO port AP02 pin 10
    BSP_IO_PORT_AP02_PIN_11 = (BSP_IO_PORT_AP02 | 0xB), ///< IO port AP02 pin 11
    BSP_IO_PORT_AP02_PIN_12 = (BSP_IO_PORT_AP02 | 0xC), ///< IO port AP02 pin 12
    BSP_IO_PORT_AP02_PIN_13 = (BSP_IO_PORT_AP02 | 0xD), ///< IO port AP02 pin 13
    BSP_IO_PORT_AP02_PIN_14 = (BSP_IO_PORT_AP02 | 0xE), ///< IO port AP02 pin 14
    BSP_IO_PORT_AP02_PIN_15 = (BSP_IO_PORT_AP02 | 0xF), ///< IO port AP02 pin 15

    BSP_IO_PORT_AP03_PIN_0 = (BSP_IO_PORT_AP03 | 0x0),  ///< IO port AP03 pin 0
    BSP_IO_PORT_AP03_PIN_1 = (BSP_IO_PORT_AP03 | 0x1),  ///< IO port AP03 pin 1
    BSP_IO_PORT_AP03_PIN_2 = (BSP_IO_PORT_AP03 | 0x2),  ///< IO port AP03 pin 2
    BSP_IO_PORT_AP03_PIN_3 = (BSP_IO_PORT_AP03 | 0x3),  ///< IO port AP03 pin 3

    BSP_IO_PORT_AP04_PIN_0 = (BSP_IO_PORT_AP04 | 0x0),  ///< IO port AP04 pin 0
    BSP_IO_PORT_AP04_PIN_1 = (BSP_IO_PORT_AP04 | 0x1),  ///< IO port AP04 pin 1
    BSP_IO_PORT_AP04_PIN_2 = (BSP_IO_PORT_AP04 | 0x2),  ///< IO port AP04 pin 2
    BSP_IO_PORT_AP04_PIN_3 = (BSP_IO_PORT_AP04 | 0x3),  ///< IO port AP04 pin 3
    BSP_IO_PORT_AP04_PIN_4 = (BSP_IO_PORT_AP04 | 0x4),  ///< IO port AP04 pin 4
    BSP_IO_PORT_AP04_PIN_5 = (BSP_IO_PORT_AP04 | 0x5),  ///< IO port AP04 pin 5
    BSP_IO_PORT_AP04_PIN_6 = (BSP_IO_PORT_AP04 | 0x6),  ///< IO port AP04 pin 6
    BSP_IO_PORT_AP04_PIN_7 = (BSP_IO_PORT_AP04 | 0x7),  ///< IO port AP04 pin 7

    BSP_IO_PORT_JP00_PIN_0 = (BSP_IO_PORT_JP00 | 0x0),  ///< IO port JP00 pin 0
    BSP_IO_PORT_JP00_PIN_1 = (BSP_IO_PORT_JP00 | 0x1),  ///< IO port JP00 pin 1
    BSP_IO_PORT_JP00_PIN_2 = (BSP_IO_PORT_JP00 | 0x2),  ///< IO port JP00 pin 2
    BSP_IO_PORT_JP00_PIN_3 = (BSP_IO_PORT_JP00 | 0x3),  ///< IO port JP00 pin 3
    BSP_IO_PORT_JP00_PIN_5 = (BSP_IO_PORT_JP00 | 0x5),  ///< IO port JP00 pin 5

    BSP_IO_PORT_P27_PIN_0 = (BSP_IO_PORT_P27 | 0x0),    ///< IO port P27 pin 0

    BSP_IO_PORT_IP00_PIN_0 = (BSP_IO_PORT_IP00 | 0x0),  ///< IO port IP00 pin 0
    BSP_IO_PORT_IP00_PIN_1 = (BSP_IO_PORT_IP00 | 0x1),  ///< IO port IP00 pin 1
} bsp_io_port_pin_t;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_IO) */

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* BSP_IO_DEVICE_H */
