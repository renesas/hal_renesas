/***********************************************************************************************************************
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_lpc_rx_if.h
 * Description  : The LPC module configures the MCU for the different operating and low power modes.
 *                This file is the LPC module's interface header file and should be included by the application that
 *                intends to use the API.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.10.2016 1.00    First Release
 *         : 01.04.2019 1.41    Changed Minor version to 1.41.
 *         : 01.07.2019 1.42    Changed Minor version to 1.42.
 *         : 14.11.2019 2.00    Changed version to 2.00.
 *         : 10.06.2020 2.01    Changed version to 2.01.
 *         : 05.03.2021 2.02    Changed version to 2.02.
 *         : 31.07.2021 2.03    Changed version to 2.03.
 *         : 31.12.2021 2.04    Changed version to 2.04.
 *         : 28.06.2022 2.10    Added new demo projects.
 *         : 15.08.2022 2.20    Changed version to 2.20.
 *                              Fixed to comply with GSCE Coding Standards Rev.6.5.0.
 *         : 29.05.2023 2.30    Added support for RX23E-B.
 *                              Fixed to comply with GSCE Coding Standards Rev.6.5.0.
 *         : 28.06.2024 2.40    Added support for RX260, RX261.
 *                              Fixed to comply with GSCE Coding Standards Rev.6.5.0.
 *                              Removed duplicate macros (LPC_CLOCK_ACTIVE and LPC_CLOCK_INACTIVE) for all devices.
 *                              Modified comment of API R_LPC_OperatingModeSet (), R_LPC_ReturnClockSwitch (),
 *                              and R_LPC_LowPowerModeActivate () functions.
 *         : 01.11.2024 2.50    Moved the restrictions of PLL, PLL2 and main clock oscillator in middle-speed operation 
 *                              mode and middle-speed operation mode 2 to lpc_freq_range_check() and 
 *                              lpc_lowpower_activate_check() functions for RX260, RX261.
 *         : 15.03.2025 2.51    Updated disclaimer.
 ***********************************************************************************************************************/

#ifndef R_LPC_RX_IF_H
    #define R_LPC_RX_IF_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
    #include <stdint.h>

/* Includes board and MCU related header files. */
    #include "platform.h"
    #include "r_lpc_rx_config.h"
    #include "r_lpc_rx_platform.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
    #define LPC_RX_VERSION_MAJOR           (2)
    #define LPC_RX_VERSION_MINOR           (51)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
/*  LPC API Error codes */
typedef enum lpc_err
{
    LPC_SUCCESS = 0,
    LPC_ERR_OSC_STOP_ENABLED,     // Software Standby Mode cannot be entered if Oscillation Stop is enabled
    LPC_ERR_CLOCK_EXCEEDED,       // If clock speed exceeds the limit of current operating mode
    LPC_ERR_ILL_MAIN_CLK_FREQ,    // If Main Oscillator is more than limit and is chosen as the Sleep return clock
    LPC_ERR_ILL_CLOCK_SOURCE,     // Return Clock Switch is only supported if clock on entering Sleep is allowed clock
    LPC_ERR_P_E_MODE,             // The OPCCR register cannot be rewritten while the flash memory is being programmed or erased (P/E).
    LPC_ERR_DEEP_SLEEP_STATUS,    // The condition to go to a deep sleep mode isn't met.
    LPC_ERR_ILL_PARAM,            // Illegal argument not specified in enum
    LPC_ERR_ILLEGAL               // Illegal setting
} lpc_err_t;

/**********************************************************************************************************************
 * Function Name: lpc_callback_set_t
 * Description  : .
 * Argument     : pdata
 * Return Value : .
 *********************************************************************************************************************/
typedef void (*lpc_callback_set_t)(void *pdata); /* Callback function type */

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * Function Name: R_LPC_GetVersion
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
uint32_t R_LPC_GetVersion (void);
#ifndef LPC_INVALID_OPERATING_MODE
/**********************************************************************************************************************
 * Function Name: R_LPC_OperatingModeSet
 * Description  : .
 * Argument     : e_mode
 * Return Value : .
 *********************************************************************************************************************/
lpc_err_t R_LPC_OperatingModeSet (lpc_operating_mode_t e_mode);
#endif
/**********************************************************************************************************************
 * Function Name: R_LPC_LowPowerModeConfigure
 * Description  : .
 * Argument     : e_mode
 * Return Value : .
 *********************************************************************************************************************/
lpc_err_t R_LPC_LowPowerModeConfigure (lpc_low_power_mode_t e_mode);

/**********************************************************************************************************************
 * Function Name: R_LPC_LowPowerModeActivate
 * Description  : .
 * Argument     : pdata
 * Return Value : .
 *********************************************************************************************************************/
lpc_err_t R_LPC_LowPowerModeActivate (void (*pcallback)(void* pdata));

/**********************************************************************************************************************
 * Function Name: R_LPC_ReturnClockSwitch
 * Description  : .
 * Arguments    : e_clock_source
 *              : enable
 * Return Value : .
 *********************************************************************************************************************/
lpc_err_t R_LPC_ReturnClockSwitch (lpc_clock_switch_t e_clock_source, bool enable);
#ifdef LPC_VALID_SNOOZE_MODE
/**********************************************************************************************************************
 * Function Name: R_LPC_SnoozeModeConfigure
 * Description  : .
 * Argument     : snooze_mode
 * Return Value : .
 *********************************************************************************************************************/
lpc_err_t R_LPC_SnoozeModeConfigure (lpc_snooze_mode_t * snooze_mode);
#endif

#endif /* R_LPC_RX_IF_H */

