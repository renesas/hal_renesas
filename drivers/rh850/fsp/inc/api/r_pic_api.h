/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_PIC_API_H
#define R_PIC_API_H

/*****************************************************************************************************************//**
 * @ingroup RENESAS_CONNECTIVITY_INTERFACES
 * @defgroup PIC_API PIC Interface
 * @brief Interface for peripheral interconnect .
 *
 * @section PIC_API_SUMMARY Summary
 * Provides a common interface for PIC protocol.
 *
 * Implemented by:
 * - @ref PIC
 *
 * @{
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Includes
 ********************************************************************************************************************/

/* Includes board and MCU related header files. */
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*********************************************************************************************************************
 * Macro definitions
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Typedef definitions
 ********************************************************************************************************************/

#ifndef BSP_OVERRIDE_PIC10_FUNC_T

/** Selects PIC1 function mode. */
typedef enum e_pic10_func
{
    PIC10_FUNC_NOT_SELECTED                    = 0x0, ///< Not selected PIC1 function
    PIC10_FUNC_TIMER_INPUT_SEL                 = 0x1, ///< Timer Input Select Function
    PIC10_FUNC_PWM_OUTPUT                      = 0x2, ///< PWM output function with dead time
    PIC10_FUNC_TRIGGER_PULSE_WIDTH_MEASUREMENT = 0x3, ///< Trigger Pulse Width Measurement Function
    PIC10_FUNC_DELAY_PULSE_OUTPUT              = 0x4, ///< Delay Pulse Output Function with Dead Time
    PIC10_FUNC_HIGH_ACCURACY_TRIANGLE_WAVE_PWM = 0x5, ///< High Accuracy Triangle Wave PWM Output Function with Dead Time
} pic10_func_t;
#endif

#ifndef BSP_OVERRIDE_PIC_MODE_T

/** PIC mode. */
typedef enum e_pic_mode
{
    PIC_MODE_DISABLE = 0x0,            ///< PIC mode disable.
    PIC_MODE_ENABLE  = 0x1             ///< PIC mode enable.
} pic_mode_t;
#endif

/** PIC interface configuration. */
typedef struct st_pic_cfg
{
    pic10_func_t pic10_function_mode;       ///< Select PIC1 function mode: Only one mode can be active at a time.
    pic_mode_t   pic10_encoder_trigger;     ///< Enable/Disable Trigger Pulse Width Measurement Function.
    pic_mode_t   pic10_trigger_pulse_width; ///< Enable/Disable Trigger Pulse Width Measurement Function.
    pic_mode_t   pic_simultaneous;          ///< Generates a start trigger for the timer of PIC10 for which simultaneous start is enabled.
    pic_mode_t   pic_timer_input_sel;       ///< Enable/disable timer input select function for TAUD.
    pic_mode_t   pic_enca_trigger;          ///< Enable/Disable ENCA Trigger Selection Function.
    pic_mode_t   pic_taud_input_select;     ///< Enable/Disable TAUD Input Selection Function.
    pic_mode_t   pic_taud_trigger_output;   ///< Enable/Disable TAUD Trigger Output Function.
    void const * p_extend;                  ///< Extended PIC hardware dependent configuration.
} pic_cfg_t;

/** PIC control block.  Allocate an instance specific control block to pass into the PIC API calls.
 */
typedef void pic_ctrl_t;

/** Shared Interface definition for PIC */
typedef struct st_pic_api
{
    /** Initialize PIC protocol.
     *
     * @param[in, out] p_api_ctrl Pointer to user-provided storage for the control block.
     * @param[in]      p_cfg      Pointer to PIC configuration structure.
     */
    fsp_err_t (* open)(pic_ctrl_t * const p_api_ctrl, pic_cfg_t const * const p_cfg);

    /** Reset value of PIC register.
     *
     * @param[in]  p_api_ctrl     Pointer to the control block for PIC module.
     */
    fsp_err_t (* close)(pic_ctrl_t * const p_api_ctrl);
} pic_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_pic_instance
{
    pic_ctrl_t      * p_ctrl;          ///< Pointer to the control structure for this instance.
    pic_cfg_t const * p_cfg;           ///< Pointer to the configuration structure for this instance.
    pic_api_t const * p_api;           ///< Pointer to the API structure for this instance.
} pic_instance_t;

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

/*****************************************************************************************************************//**
 * @} (end defgroup PIC_API)
 ********************************************************************************************************************/

#endif
