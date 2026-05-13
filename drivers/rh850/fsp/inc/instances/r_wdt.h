/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_WDT_H
#define R_WDT_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_wdt_cfg.h"
#include "bsp_api.h"
#include "r_wdt_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup WDT
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**Window Open function mode selection */
typedef enum e_wdt_window_open_function_mode
{
    WDT_WINDOW_OPEN_FUNCTION_MODE_SELECT_BY_WS = 0, ///< Outputs when the counter reaches 75% of the overflow setting defined
    WDT_WINDOW_OPEN_FUNCTION_MODE_SET_BY_WOST       ///< Outputs when WDTBnWIS/WDTBAWIS matches WDTB counter
} wdt_window_open_function_mode_t;

/** Enables/disables the 75% interrupt request */
typedef enum e_wdt_interrupt_request
{
    WDT_INTERRUPT_DISABLED = 0,        ///< Disable 75% interrupt
    WDT_INTERRUPT_ENABLED              ///< Enable 75% interrupt
} wdt_interrrupt_request_t;

/** Select the period over which the window is open */
typedef enum e_wdt_window_open
{
    WDT_PERIOD_25 = 0,                 ///< Window-Open Period 25%
    WDT_PERIOD_50,                     ///< Window-Open Period 50%
    WDT_PERIOD_75,                     ///< Window-Open Period 75%
    WDT_PERIOD_100                     ///< Window-Open Period 100%
} wdt_window_open_t;

/** Select the overflow interval time */
typedef enum e_wdt_overflow_time
{
    WDT_OVERFLOW_WDTBTCKI_2_9 = 0,     ///< Overflow Interval Time: 2^9/WDTBTCKI
    WDT_OVERFLOW_WDTBTCKI_2_10,        ///< Overflow Interval Time: 2^10/WDTBTCKI
    WDT_OVERFLOW_WDTBTCKI_2_11,        ///< Overflow Interval Time: 2^11/WDTBTCKI
    WDT_OVERFLOW_WDTBTCKI_2_12,        ///< Overflow Interval Time: 2^12/WDTBTCKI
    WDT_OVERFLOW_WDTBTCKI_2_13,        ///< Overflow Interval Time: 2^13/WDTBTCKI
    WDT_OVERFLOW_WDTBTCKI_2_14,        ///< Overflow Interval Time: 2^14/WDTBTCKI
    WDT_OVERFLOW_WDTBTCKI_2_15,        ///< Overflow Interval Time: 2^15/WDTBTCKI
    WDT_OVERFLOW_WDTBTCKI_2_16         ///< Overflow Interval Time: 2^16/WDTBTCKI
} wdt_overflow_time_t;

/** Select the activation code method. The WDTB trigger register, which generates a WDTB trigger, is specified by the start-up option OPWDVAC */
typedef enum e_wdt_activation_code
{
    WDT_ACTIVATION_CODE_FIX = 0,       ///< WDTB trigger with fixed activation code (0xAC)
    WDT_ACTIVATION_CODE_VARIABLE       ///< WDTB trigger with variable activation code
} wdt_activation_code_t;

typedef enum e_wdt_unit
{
    WDT_UNIT_A = 0,                    ///< WDTB Unit A
    WDT_UNIT_0,                        ///< WDTB Unit 0
    WDT_UNIT_1,                        ///< WDTB Unit 1
    WDT_UNIT_2,                        ///< WDTB Unit 2
    WDT_UNIT_3,                        ///< WDTB Unit 3
    WDT_UNIT_4,                        ///< WDTB Unit 4
    WDT_UNIT_5,                        ///< WDTB Unit 5
    WDT_UNIT_6                         ///< WDTB Unit 6
} wdt_unit_t;

/** WDT private control block. DO NOT MODIFY. Initialization occurs when R_WDT_Open() is called. */
typedef struct st_wdt_instance_ctrl
{
    wdt_cfg_t const * p_cfg;                           ///< Pointer to the configuration block

    uint32_t       wdt_open;                           ///< Indicates whether the open() API has been successfully called.
    R_WDTB1_Type * p_reg;                              ///< Base register for this channel
    void         * p_context;                          ///< Placeholder for user data.  Passed to the user callback in wdt_callback_args_t.
    void (* p_callback)(wdt_callback_args_t * p_args); ///< Callback provided when a WDT ISR occurs.
    wdt_callback_args_t * p_callback_memory;           ///< Pointer to non-secure memory that can be used to pass arguments to a callback in non-secure memory.
} wdt_instance_ctrl_t;

/** WDT extended configuration */
typedef struct st_wdt_extended_cfg
{
    wdt_window_open_function_mode_t window_open_mode_select;      ///< Window Open function mode selection.
    uint16_t                 window_open_start;                   ///< Setting for the start timing of the window open.
    uint16_t                 window_open_interrupt_output_timimg; ///< Window Open interrupt output timing setting
    wdt_unit_t               unit;                                ///< Select a unit corresponding to the unit number of the hardware.
    uint8_t                  channel;                             ///< Identifier recognizable by implementation.
    wdt_window_open_t        window_open;                         ///< Refresh permitted window open position.
    wdt_overflow_time_t      overflow_time;                       ///< Overflow Interval Time
    wdt_interrrupt_request_t isr_request;                         ///< Enable/disables the 75% interrupt
    wdt_activation_code_t    activation_code;                     ///< Trigger function

    uint8_t   ipl;                                                ///< Interrupt priority level.
    IRQn_Type wdt_irq;                                            ///< 75% interrupt number.
} wdt_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const wdt_api_t g_wdt_on_wdt;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_WDT_Open(wdt_ctrl_t * const p_ctrl, wdt_cfg_t const * const p_cfg);

fsp_err_t R_WDT_Refresh(wdt_ctrl_t * const p_ctrl);

fsp_err_t R_WDT_StatusClear(wdt_ctrl_t * const p_ctrl, const wdt_status_t status);

fsp_err_t R_WDT_StatusGet(wdt_ctrl_t * const p_ctrl, wdt_status_t * const p_status);

fsp_err_t R_WDT_CounterGet(wdt_ctrl_t * const p_ctrl, uint32_t * const p_count);

fsp_err_t R_WDT_TimeoutGet(wdt_ctrl_t * const p_ctrl, wdt_timeout_values_t * const p_timeout);

fsp_err_t R_WDT_CallbackSet(wdt_ctrl_t * const          p_ctrl,
                            void (                    * p_callback)(wdt_callback_args_t *),
                            void * const                p_context,
                            wdt_callback_args_t * const p_callback_memory);

/*******************************************************************************************************************//**
 * @} (end defgroup WDT)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_WDT_H */
