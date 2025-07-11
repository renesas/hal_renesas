/***********************************************************************************************************************
* Copyright (c) 2013 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/******************************************************************************
* File Name    : r_s12ad_rx_private.h
* Description  : Definitions and structures used internally by the driver.
*******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
*           22.07.2013 1.00    Initial Release.
*           21.04.2014 1.20    Updated for RX210 advanced features; RX110/63x support.
*           06.11.2014 1.40    Added RX113 support.
*           20.05.2015 1.50    Added RX231 support.
*           01.03.2016 2.11    Added RX130 and RX230 support.
*           01.10.2016 2.20    Added RX65x support.
*           03.04.2017 2.30    Added RX65N-2MB and RX130-512KB support.
*           03.09.2018 3.00    Added RX66T support.
*           28.12.2018 3.10    Added RX72T support.
*                              Added RX65N 64pins support.
*           05.04.2019 4.00    Deleted the bellow macro definition of RX210, RX631, and RX63N.
*                              - ADC_PRV_INVALID_CH_MASK
*                              Added support for GNUC and ICCRX.
*           28.06.2019 4.10    Added RX23W support.
*           31.07.2019 4.20    Added RX72M support.
*           30.08.2019 4.30    Added RX13T support.
*           22.11.2019 4.40    Added RX66N and RX72N support.
*           28.02.2020 4.50    Added RX23E-A support.
*           10.06.2020 4.60    Added RX23T and RX24T and RX24U support.
*           01.03.2021 4.70    Added RX72M 144pins and 100pins support.
*                              Added RX23W 83pins support.
*           31.05.2021 4.80    Added RX671 support.
*           30.07.2021 4.90    Added RX140 support.
*           30.11.2021 4.93    Added RX66T 48pins support.
*           29.12.2021 5.00    Added RX660 support.
*           01.08.2022 5.10    Added RX26T support.
*           14.10.2022 5.20    Added RX23E-B support.
*           03.04.2023 5.30    Added RX26T 48k support.
*           13.02.2024 5.40    Added RX260 and RX261 support.
*           20.03.2025 5.41    Changed the disclaimer in program sources.
*******************************************************************************/

#ifndef S12AD_PRV_PRIVATE_H
#define S12AD_PRV_PRIVATE_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"
#include "r_s12ad_rx_if.h"

/******************************************************************************
Macro definitions
*******************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Macro for accessing RX64M/RX65x/RX66T/RX71M/RX72T/RX24T/RX24U/RX671 data register pointers for 
 a given unit (0, 1 or 2) */
#if (defined(BSP_MCU_RX66T)   || defined(BSP_MCU_RX72T) || defined(BSP_MCU_RX24T) \
    || defined(BSP_MCU_RX24U) || defined(BSP_MCU_RX26T))
#define ADC_PRV_GET_DATA_ARR(x)     (((x)==0) ? gp_dreg0_ptrs : \
                                    ((x)==1) ? gp_dreg1_ptrs : \
                                                gp_dreg2_ptrs)
#else
#define ADC_PRV_GET_DATA_ARR(x)         (((x)==0) ? gp_dreg0_ptrs : gp_dreg1_ptrs)
#endif /* #if (defined(BSP_MCU_RX66T) || defined(BSP_MCU_RX72T) || defined(BSP_MCU_RX671)) */

#if (defined(BSP_MCU_RX110) || defined(BSP_MCU_RX111))

#if BSP_PACKAGE_PINS == 64

/* valid: 1111 1111 0101 1111; invalid 0000 0000 1010 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFF00A0)

#elif BSP_PACKAGE_PINS == 48

/* valid: 1001 1111 0100 0111; invalid 0110 0000 1011 1000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFF60B8)

#elif BSP_PACKAGE_PINS == 40

/* valid: 0001 1111 0100 0110; invalid 1110 0000 1011 1001 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFFE0B9)

#elif BSP_PACKAGE_PINS == 36

/* valid: 0001 1111 0000 0110; invalid 1110 0000 1111 1001 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFFE0F9)

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /*  */

#endif


#ifdef BSP_MCU_RX113

#if BSP_PACKAGE_PINS == 100

/* valid: 0010 0000 1111 1111 1111 1111; invalid 1101 1111 0000 0000 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFDF0000)

#elif BSP_PACKAGE_PINS == 64

/* valid: 1111 1111 0000 0111; invalid 0000 0000 1111 1000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFF00F8)

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* #if BSP_PACKAGE_PINS == 100 */

#endif


#ifdef BSP_MCU_RX130

/* Temp sensor and internal v ref are in bit locations 8 and 9 in each mask for rx130 */
#if BSP_PACKAGE_PINS == 100

/* valid: 1111 1111 1111 1111 0000 0011 1111 1111; invalid 0000 0000 0000 0000 1111 1100 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0x0000FC00)

#elif BSP_PACKAGE_PINS == 80

/* valid: 0000 0111 0011 1111 0000 0011 1111 1111; invalid 1111 1000 1100 0000 1111 1100 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xF8C0FC00)

#elif BSP_PACKAGE_PINS == 64

/* valid: 0000 0000 0011 1111 0000 0011 1111 1111; invalid 1111 1111 1100 0000 1111 1100 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFC0FC00)

#elif BSP_PACKAGE_PINS == 48

/* valid: 0000 0000 0001 1110 0000 0011 1110 0111; invalid 1111 1111 1110 0001 1111 1100 0001 1000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFE1FC18)
#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* #if BSP_PACKAGE_PINS == 100 */

#endif


#ifdef BSP_MCU_RX13T

/* Internal v ref are in bit location 8 in each mask for rx130 */
#if BSP_PACKAGE_PINS == 48

/* valid: 0000 0000 0000 0000 0000 0001 1111 1111; invalid 1111 1111 1111 1111 1111 1110 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFFFE00)

#elif BSP_PACKAGE_PINS == 32

/* valid: 0000 0000 0000 0000 0000 0001 0001 1111; invalid 1111 1111 1111 1111 1111 1110 1110 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFFFEE0)

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 48 */

#endif


#ifdef BSP_MCU_RX140

/* Temp sensor and internal v ref are in bit locations 9 and 10 in each mask for rx140 */
#if BSP_PACKAGE_PINS == 80

/* valid: 0000 0111 0011 1111 0000 0111 1111 1111; invalid 1111 1000 1100 0000 1111 1000 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xF8C0F800)

#elif BSP_PACKAGE_PINS == 64

/* valid: 0000 0000 0011 1111 0000 0111 1111 1111; invalid 1111 1111 1100 0000 1111 1000 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFC0F800)

#elif BSP_PACKAGE_PINS == 48

/* valid: 0000 0000 0001 1110 0000 0111 1110 0111; invalid 1111 1111 1110 0001 1111 1000 0001 1000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFE1F818)

#elif BSP_PACKAGE_PINS == 32

/* valid: 0000 0000 0001 1110 0000 0111 1110 0111; invalid 1111 1111 1110 0001 1111 1000 1111 1000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFE1F8F8)

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* #if BSP_PACKAGE_PINS == 80 */

#endif


#ifdef BSP_MCU_RX230

/* Temp sensor and internal v ref are in bit locations 8 and 9 in each mask for rx230 */
#if BSP_PACKAGE_PINS == 100

/* valid: 1111 1111 1111 1111 0000 0011 1111 1111; invalid 0000 0000 0000 0000 1111 1100 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0x0000FC00)

#elif BSP_PACKAGE_PINS == 64

/* valid: 0000 0000 0011 1111 0000 0011 0101 1111; invalid 1111 1111 1100 0000 1111 1100 1010 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFC0FCA0)

#elif BSP_PACKAGE_PINS == 48

/* valid: 0000 0000 0001 1110 0000 0011 0100 0111; invalid 1111 1111 1110 0001 1111 1100 1011 1000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFE1FCB8)
#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 100 */

#endif


#ifdef BSP_MCU_RX231

/* Temp sensor and internal v ref are in bit locations 8 and 9 in each mask for rx231 */
#if BSP_PACKAGE_PINS == 100

/* valid: 1111 1111 1111 1111 0000 0011 1111 1111; invalid 0000 0000 0000 0000 1111 1100 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0x0000FC00)

#elif BSP_PACKAGE_PINS == 64

/* valid: 0000 0000 0011 1111 0000 0011 0101 1111; invalid 1111 1111 1100 0000 1111 1100 1010 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFC0FCA0)

#elif BSP_PACKAGE_PINS == 48

/* valid: 0000 0000 0001 1110 0000 0011 0100 0111; invalid 1111 1111 1110 0001 1111 1100 1011 1000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFE1FCB8)
#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* BSP_PACKAGE_PINS == 100 */

#endif


#ifdef BSP_MCU_RX23T

/* Internal v ref are in bit location 8 in each mask for rx23t */
#if BSP_PACKAGE_PINS == 64

/* valid: 0000 0000 0000 0011 0000 0001 1111 1111; invalid 1111 1111 1111 1100 1111 1110 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFCFE00)

#elif BSP_PACKAGE_PINS == 52

/* valid: 0000 0000 0000 0011 0000 0001 1111 1111; invalid 1111 1111 1111 1100 1111 1110 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFCFE00)

#elif BSP_PACKAGE_PINS == 48

/* valid: 0000 0000 0000 0011 0000 0001 1111 1111; invalid 1111 1111 1111 1100 1111 1110 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFCFE00)
#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* BSP_PACKAGE_PINS == 64 */

#endif


#ifdef BSP_MCU_RX23E_A

#if BSP_PACKAGE_PINS == 48
/* valid: 0000 0000 0000 0000 0000 0011 1111 1111; invalid 1111 1111 1111 1111 1111 1111 1100 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFFFFC0)

#elif BSP_PACKAGE_PINS == 40

/* valid: 0000 1000 0001 1100 0000 0011 1110 0010; invalid 1111 1111 1110 1111 1111 1111 1111 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xFFFFFFF0)

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 48 */

#endif

#ifdef BSP_MCU_RX23W

/* Temp sensor and internal v ref are in bit locations 8 and 9 in each mask for rx23w */
#if BSP_PACKAGE_PINS == 85
/* valid: 0000 1000 0001 1111 0000 0011 1111 1111; invalid 1111 0111 1110 0000 1111 1100 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xF7E0FC00)

#elif BSP_PACKAGE_PINS == 83
/* valid: 0000 1000 0001 1111 0000 0011 1111 1111; invalid 1111 0111 1110 0000 1111 1100 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK     (0xF7E0FC00)

#elif BSP_PACKAGE_PINS == 56

/* valid: 0000 1000 0001 1100 0000 0011 1110 0010; invalid 1111 0111 1110 0011 1111 1100 0001 1101 */
#define ADC_PRV_INVALID_CH_MASK     (0xF7E3FC1D)

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 85 */

#endif

#ifdef BSP_MCU_RX24T

/* Internal v ref are in bit location 12 in mask2 for rx24t */
#if BSP_PACKAGE_PINS == 100

#define ADC_PRV_INVALID_CH_MASK0    (0xFFFEFFF0)    /* all channels valid (0-3,16) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFEFFF0)    /* all channels valid (0-3,16) */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFFFE000)    /* all channels valid (0-11, sensors) */

#elif BSP_PACKAGE_PINS == 80

#define ADC_PRV_INVALID_CH_MASK0    (0xFFFEFFF0)    /* all channels valid (0-3,16) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFEFFF0)    /* all channels valid (0-3,16) */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFFFE0C4)    /* all channels valid (2,6-11, sensors) */

#elif BSP_PACKAGE_PINS == 64

#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFF8)    /* all channels valid (0-2) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFEFFF8)    /* all channels valid (0-2,16) */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFFFE83F)    /* all channels valid (6-10, sensors) */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 100 */

#endif

#ifdef BSP_MCU_RX24U

/* Internal v ref are in bit location 12 in mask2 for rx24u */
#if BSP_PACKAGE_PINS == 144

#define ADC_PRV_INVALID_CH_MASK0    (0xFFFEFFF0)    /* all channels valid (0-3,16) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFEFFF0)    /* all channels valid (0-3,16) */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFFFE000)    /* all channels valid (0-11, sensors) */

#elif BSP_PACKAGE_PINS == 100

#define ADC_PRV_INVALID_CH_MASK0    (0xFFFEFFF0)    /* all channels valid (0-3,16) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFEFFF0)    /* all channels valid (0-3,16) */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFFFE0C0)    /* all channels valid (0-5,8-11, sensors) */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 144 */

#endif

#if (defined(BSP_MCU_RX64M) || defined(BSP_MCU_RX71M))

#if BSP_PACKAGE_PINS == 177
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 176
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 145
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 100
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */

/* valid: 0110 0000 0011 1111 1111 1111; invalid 1001 1111 1100 0000 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF9FC000)    /* chans 0-13, sensors valid */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 177 */

#endif

#if (defined(BSP_MCU_RX65_ALL))

#if BSP_PACKAGE_PINS == 177
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 176
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 145
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 100
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */

/* valid: 0110 0000 0011 1111 1111 1111; invalid 1001 1111 1100 0000 0000 0000 */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF9FC000)    /* chans 0-13, sensors valid */

#elif BSP_PACKAGE_PINS == 64
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFF0)    /* channels 0-3 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF9FC33F)    /* chans 6, 7 10-13, sensors valid */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* BSP_PACKAGE_PINS == 177 */

#endif /* definedBSP_MCU_RX65_ALL */

#if (defined(BSP_MCU_RX660))

#if BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFC000000)    /* all channels valid (0-23 sensors) */

#elif BSP_PACKAGE_PINS == 100
#define ADC_PRV_INVALID_CH_MASK0    (0xFC000000)    /* all channels valid (0-23 sensors) */

#elif BSP_PACKAGE_PINS == 80
#define ADC_PRV_INVALID_CH_MASK0    (0xFCF8C000)    /* channels 0-13 16-18 sensors valid */

#elif BSP_PACKAGE_PINS == 64
#define ADC_PRV_INVALID_CH_MASK0    (0xFCFFC000)    /* channels 0-13 sensors valid */

#elif BSP_PACKAGE_PINS == 48
#define ADC_PRV_INVALID_CH_MASK0    (0xFCFFE118)    /* channels 0-2 5-7 9-12 sensors valid */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* BSP_PACKAGE_PINS == 177 */

#endif /* definedBSP_MCU_RX65_ALL */

#if (defined(BSP_MCU_RX66T))

#if BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F000)    /* all channels valid (0-11, 16, 17 sensors) */

#elif BSP_PACKAGE_PINS == 112
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF70)    /* channels 0-3, 7 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFF70)    /* channels 0-3, 7 valid */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F030)    /* channels 0-3, 6-11, 16, 17 sensors valid */

#elif BSP_PACKAGE_PINS == 100

#if ((BSP_CFG_MCU_PART_FUNCTION == 0xA) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xC) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xE) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0x10))
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF70)    /* channels 0-3, 7 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFF70)    /* channels 0-3, 7 valid */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F030)    /* channels 0-3, 6-11, 16, 17 sensors valid */
#else
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFF0)    /* channels 0-3 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFFF0)    /* channels 0-3 valid */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F000)    /* all channels valid (0-11, 16, 17 sensors) */
#endif /* #if ((BSP_CFG_MCU_PART_FUNCTION == 0xA) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xC) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xE) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0x10)) */

#elif BSP_PACKAGE_PINS == 80
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF70)    /* channels 0-3, 7 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFF70)    /* channels 0-3, 7 valid */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F2F0)    /* channels 0-3, 8, 10, 11, 16, 17, sensors valid */

#elif BSP_PACKAGE_PINS == 64
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF78)    /* channels 0-2, 7 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFF78)    /* channels 0-2, 7 valid */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F3F8)    /* channels 0-2, 10, 11, 16, 17, sensors valid */

#elif BSP_PACKAGE_PINS == 48
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFF0)    /* channels 0-3 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFFFE)    /* channels 0 valid */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF3F2FF)    /* channels 8, 10, 11, sensors valid */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* BSP_PACKAGE_PINS == 144 */

#endif /* definedBSP_MCU_RX66T */

#if (defined(BSP_MCU_RX671))

#if BSP_PACKAGE_PINS == 145
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFC000)    /* all channels valid (0-11, sensors) */

#elif BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFC000)    /* all channels valid (0-11, sensors) */

#elif BSP_PACKAGE_PINS == 100
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFCF00)    /* channels 0-7, sensors valid */

#elif BSP_PACKAGE_PINS == 64
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFF0)    /* channels 0-3 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFCFC0)    /* channels 0-5, sensors valid */

#elif BSP_PACKAGE_PINS == 48
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFF0)    /* channels 0-3 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFCFC3)    /* channels 2-5, sensors valid */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* BSP_PACKAGE_PINS == 145 */

#endif /* definedBSP_MCU_RX671 */

#if (defined(BSP_MCU_RX72T))

#if BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F000)    /* all channels valid (0-11, 16, 17 sensors) */

#elif BSP_PACKAGE_PINS == 100

#if ((BSP_CFG_MCU_PART_FUNCTION == 0xA) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xC) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xE) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0x10))
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF70)    /* channels 0-3, 7 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFF70)    /* channels 0-3, 7 valid */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F030)    /* channels 0-3, 6-11, 16, 17 sensors valid */
#else
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFF0)    /* channels 0-3 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFFFFFFF0)    /* channels 0-3 valid */
#define ADC_PRV_INVALID_CH_MASK2    (0xFFF0F000)    /* all channels valid (0-11, 16, 17 sensors) */
#endif /* #if ((BSP_CFG_MCU_PART_FUNCTION == 0xA) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xC) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xE) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0x10)) */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 144 */
#endif

#if (defined(BSP_MCU_RX72M))

#if BSP_PACKAGE_PINS == 224
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 176
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFE0)    /* channels 0-4 valid */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 100
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFFF8)    /* channels 0-2  */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF983931)    /* channels 1-3, 6, 7, 9, 10, 14-18, sensors valid */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif /* BSP_PACKAGE_PINS == 224 */
#endif

#if (defined(BSP_MCU_RX66N))

#if BSP_PACKAGE_PINS == 224
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 176
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 145
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 100
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF9FC000)    /* chans 0-13, sensors valid */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /* BSP_PACKAGE_PINS == 224 */
#endif

#if (defined(BSP_MCU_RX72N))

#if BSP_PACKAGE_PINS == 224
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 176
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 145
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 144
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF800000)    /* all channels valid (0-20, sensors) */

#elif BSP_PACKAGE_PINS == 100
#define ADC_PRV_INVALID_CH_MASK0    (0xFFFFFF00)    /* all channels valid (0-7) */
#define ADC_PRV_INVALID_CH_MASK1    (0xFF9FC000)    /* chans 0-13, sensors valid */

#else
    #error "ERROR - BSP_CFG_MCU_PART_PACKAGE - Unknown package chosen in r_bsp_config.h"
#endif  /*  */
#endif


/******************************************************************************
Typedef definitions
*******************************************************************************/
typedef R_BSP_VOLATILE_EVENACCESS uint16_t * const  dregs_t;

typedef struct st_adc_ctrl          // ADC Control Block
{
    adc_mode_t      mode;           // operational mode
    bool            opened;
    void          (*p_callback)(void *p_args);

#if (defined(BSP_MCU_RX64M) || defined(BSP_MCU_RX65_ALL) || defined(BSP_MCU_RX66T) \
    || defined(BSP_MCU_RX71M) || defined(BSP_MCU_RX72T) || defined(BSP_MCU_RX72M) \
    || defined(BSP_MCU_RX66N) || defined(BSP_MCU_RX72N) || defined(BSP_MCU_RX671) \
    || defined(BSP_MCU_RX660) || defined(BSP_MCU_RX26T))
    uint32_t        cmpi_mask;      // for GRPBL1
#endif    
#if (defined(BSP_MCU_RX65_ALL) || defined(BSP_MCU_RX66T) || defined(BSP_MCU_RX72T) \
    || defined(BSP_MCU_RX72M)  || defined(BSP_MCU_RX66N) || defined(BSP_MCU_RX72N) \
    || defined(BSP_MCU_RX671)  || defined(BSP_MCU_RX660) || defined(BSP_MCU_RX26T))
    uint32_t        cmpi_maskb;     // for GRPBL1 WINDOWB
#endif
} adc_ctrl_t;

/* ADCSR register ADCS field (non-63x) */
typedef enum e_adc_adcs
{
    ADC_ADCS_SINGLE_SCAN =0,
    ADC_ADCS_GROUP_SCAN  =1,
    ADC_ADCS_CONT_SCAN   =2,
    ADC_ADCS_MAX
} e_adc_adcs_t;

/******************************************************************************
Exported global variables
*******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/
extern adc_err_t adc_open(uint8_t const          unit,
                        adc_mode_t const       mode,
                        adc_cfg_t * const      p_cfg,
                        void         (* const  p_callback)(void *p_args));
extern adc_err_t adc_close(uint8_t const  unit);
extern adc_err_t adc_read_all(adc_data_t * const  p_all_data);
extern adc_err_t adc_control(uint8_t const       unit,
                            adc_cmd_t const     cmd,
                            void * const        p_args);

#if (!defined(BSP_MCU_RX64M) && !defined(BSP_MCU_RX65_ALL) && !defined(BSP_MCU_RX66T) \
    && !defined(BSP_MCU_RX71M) && !defined(BSP_MCU_RX72T) && !defined(BSP_MCU_RX72M) \
    && !defined(BSP_MCU_RX13T) && !defined(BSP_MCU_RX66N) && !defined(BSP_MCU_RX72N) \
    && !defined(BSP_MCU_RX23T) && !defined(BSP_MCU_RX24T) && !defined(BSP_MCU_RX24U) \
    && !defined(BSP_MCU_RX671) && !defined(BSP_MCU_RX26T))
/******************************************************************************
* Function Name: adc_enable_s12adi0
* Description  : This function clears the S12ADI0 interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void adc_enable_s12adi0(void);
#endif /* #if (!defined(BSP_MCU_RX64M) && !defined(BSP_MCU_RX65_ALL) && !defined(BSP_MCU_RX66T) \
    && !defined(BSP_MCU_RX71M) && !defined(BSP_MCU_RX72T) && !defined(BSP_MCU_RX72M) \
    && !defined(BSP_MCU_RX13T) && !defined(BSP_MCU_RX66N) && !defined(BSP_MCU_RX72N) \
    && !defined(BSP_MCU_RX23T) && !defined(BSP_MCU_RX24T) && !defined(BSP_MCU_RX24U) \
    && !defined(BSP_MCU_RX671) && !defined(BSP_MCU_RX26T)) */


#endif /* S12AD_PRV_PRIVATE_H */

