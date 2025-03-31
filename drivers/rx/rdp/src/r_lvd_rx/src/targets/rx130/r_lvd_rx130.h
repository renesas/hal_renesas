/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_lvd_rx130.h
* Description  : When a target product is selected, as for this header, it is done include.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.06.2016 1.00     First Release
*              : 01.10.2016 1.10     Deleted Tool-Chain version.
*              : 19.12.2016 1.20     Deleted unnecessary header information.
*              : 12.12.2022 1.30     Fixed condition of the voltage detection level for RX130 channel 2.
***********************************************************************************************************************/
#ifndef LVD_RX130_HEADER_FILE
#define LVD_RX130_HEADER_FILE

/***********************************************************************************************************************
Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Used for configuring the LVD module */
#include "r_lvd_rx_config.h"

/* Private header file for this package. */
#include "r_lvd_rx_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* Group: LVD setup group */
#define LVD_GROUP_SETUP                         (LVD_GROUP_SETUP_LVDAb_1)

/* Group: ICU group */
#define LVD_GROUP_INT                           (LVD_GROUP_INT_ICUb)

/* Support: Maskable interrupt */
#define LVD_SUPPORT_MI_CH1                      (LVD_ENABLE)
#define LVD_SUPPORT_MI_CH2                      (LVD_ENABLE)

/* Support: None-maskable interrupt */
#define LVD_SUPPORT_NMI_CH1                     (LVD_ENABLE)
#define LVD_SUPPORT_NMI_CH2                     (LVD_ENABLE)

/* Support: CMPA pin */
#define LVD_SUPPORT_CMPA_CH1                    (LVD_DISABLE)
#define LVD_SUPPORT_CMPA_CH2                    (LVD_ENABLE)

/* Support: Digital filter */
#define LVD_SUPPORT_DFILTER_CH1                 (LVD_DISABLE)
#define LVD_SUPPORT_DFILTER_CH2                 (LVD_DISABLE)

/* Value: Max priority of maskable interrupt */
#define LVD_MAX_MI_PRIO                         (15)

/* Default value: Default register value of voltage level */
#define LVD_VOLTAGE_LEVEL_DEFAULT_VALUE_CH1     (7)
#define LVD_VOLTAGE_LEVEL_DEFAULT_VALUE_CH2     (0)

/* Convert: Voltage level: Setting value -> register value */
#if (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 429)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (0)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 414)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (1)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 402)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (2)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 384)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (3)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 310)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (4)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 300)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (5)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 290)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (6)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 279)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (7)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 268)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (8)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 258)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (9)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 248)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (10)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 220)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (11)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 196)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (12)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == 186)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (13)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1 == LVD_DEFAULT_VALUE)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (LVD_VOLTAGE_LEVEL_DEFAULT_VALUE_CH1)
#else
    #define LVD_VOLTAGE_LEVEL_VALUE_CH1         (LVD_INVALID_VALUE)
#endif

#if (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_2 == 429)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH2         (0)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_2 == 414)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH2         (1)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_2 == 402)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH2         (2)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_2 == 384)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH2         (3)
#elif (LVD_CFG_VOLTAGE_LEVEL_CHANNEL_2 == LVD_DEFAULT_VALUE)
    #define LVD_VOLTAGE_LEVEL_VALUE_CH2         (LVD_VOLTAGE_LEVEL_DEFAULT_VALUE_CH2)
#else
    #define LVD_VOLTAGE_LEVEL_VALUE_CH2         (LVD_INVALID_VALUE)
#endif


/* Default value: Default setting value of the sampling clock */
#define LVD_DFILTER_DIV_VALUE_CH1               (LVD_INVALID_VALUE)
#define LVD_DFILTER_DIV_VALUE_CH2               (LVD_INVALID_VALUE)

/* Value: Wait for enable digital filter. (LOCO cycle) */
#define LVD_DELAY_DFILTER_ENABLE_LOCO_CH1       (0)
#define LVD_DELAY_DFILTER_ENABLE_LOCO_CH2       (0)


/* Value: Wait for enable LVD circuit. (micro sec) */
#define LVD_DELAY_CIRCUIT_ENABLE_USEC           (300)

/* Value: Wait for clear LVD status. (micro sec) */
#define LVD_DELAY_CLEAR_STATUS_USEC             (2)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/


#endif/* LVD_RX130_HEADER_FILE */

/* End of File */

