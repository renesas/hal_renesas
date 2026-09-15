/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_OSTM_H
#define R_OSTM_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER
#include "r_ostm_cfg.h"
#include "r_timer_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup OSTM
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Optional OSTM interrupt setting */
typedef enum e_ostm_giws_type
{
    OSTM_GIWS_TYPE_DISABLED = 0,       ///< Do not generate interrupt when timer started
    OSTM_GIWS_TYPE_ENABLED  = 1,       ///< Generates interrupt when timer started
} ostm_giws_type_t;

/** Optional OSTM timer mode setting */
typedef enum e_ostm_timer_mode
{
    OSTM_TIMER_MODE_INTERVAL = 0,      ///< Use interval timer mode
    OSTM_TIMER_MODE_FREERUN  = 1,      ///< Use free-running comparison mode
} ostm_timer_mode_t;

/** Reload Counter at counter operation start*/
typedef enum e_ostm_counter_start
{
    OSTM_COUNTER_START_RELOAD_ENABLE  = 0, ///< OSTMnCNT is loaded into at counter operation start
    OSTM_COUNTER_START_RELOAD_DISABLE = 1, ///< OSTMnCNT is not loaded into at counter operation start
} ostm_counter_start_t;

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when @ref timer_api_t::open is called. */
typedef struct st_ostm_instance_ctrl
{
    uint32_t                open;                 // Whether or not channel is open
    const timer_cfg_t     * p_cfg;                // Pointer to initial configurations
    R_OSTMn_Type          * p_reg;                // Base register for this channel
    uint32_t                period;               // Current timer period (counts)
    void (* p_callback)(timer_callback_args_t *); // Pointer to callback
    timer_callback_args_t * p_callback_memory;    // Pointer to pre-allocated callback argument
    void const            * p_context;            // Pointer to context to be passed into callback function
} ostm_instance_ctrl_t;

/** OSTM extension data structure. Required by R_OSTM_Open. */
typedef struct st_ostm_extended_cfg
{
    ostm_giws_type_t     generate_interrupt_when_starts; ///< Controls enabling/disabling of interrupt requests when start
    ostm_timer_mode_t    ostm_mode;                      ///< Select OSTM timer mode
    ostm_counter_start_t counter_load_at_start;          ///< Controls enabling/disabling reload counter when start
} ostm_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const timer_api_t g_timer_on_ostm;

/** @endcond */

fsp_err_t R_OSTM_Close(timer_ctrl_t * const p_ctrl);
fsp_err_t R_OSTM_PeriodSet(timer_ctrl_t * const p_ctrl, uint32_t const period_counts);
fsp_err_t R_OSTM_DutyCycleSet(timer_ctrl_t * const p_ctrl, uint32_t const duty_cycle_counts, uint32_t const pin);
fsp_err_t R_OSTM_Reset(timer_ctrl_t * const p_ctrl);
fsp_err_t R_OSTM_Start(timer_ctrl_t * const p_ctrl);
fsp_err_t R_OSTM_Enable(timer_ctrl_t * const p_ctrl);
fsp_err_t R_OSTM_Disable(timer_ctrl_t * const p_ctrl);
fsp_err_t R_OSTM_InfoGet(timer_ctrl_t * const p_ctrl, timer_info_t * const p_info);
fsp_err_t R_OSTM_StatusGet(timer_ctrl_t * const p_ctrl, timer_status_t * const p_status);
fsp_err_t R_OSTM_Stop(timer_ctrl_t * const p_ctrl);
fsp_err_t R_OSTM_Open(timer_ctrl_t * const p_ctrl, timer_cfg_t const * const p_cfg);
fsp_err_t R_OSTM_CallbackSet(timer_ctrl_t * const          p_api_ctrl,
                             void (                      * p_callback)(timer_callback_args_t *),
                             void const * const            p_context,
                             timer_callback_args_t * const p_callback_memory);

/*******************************************************************************************************************//**
 * @} (end defgroup OSTM)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */

FSP_FOOTER

#endif                                 /* R_OSTM_H */
