/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_TAPA_API_H
#define R_TAPA_API_H

/*******************************************************************************************************************//**
 * @defgroup TAPA_API TAPA Interface
 * @ingroup RENESAS_TIMERS_INTERFACES
 * @brief Interface for timer functions.
 *
 * @section TAPA_API_SUMMARY Summary
 * Provides a common interface for TAPA module.
 *
 * Implemented by:
 * - @ref TAPA
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Includes board and MCU related header files. */
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** TAPA asynchronous Hi-Z control operation status. */
typedef enum e_tapa_hiz_operation_status
{
    TAPA_HIZ_OPERATION_STATUS_STOPPED = 0U, ///< Asynchronous Hi-Z control is stopped.
    TAPA_HIZ_OPERATION_STATUS_ENABLED = 1U  ///< Asynchronous Hi-Z control is enabled.
} tapa_hiz_operation_status_t;

/** TAPA Hi-Z control monitor output level. */
typedef enum e_tapa_output_level
{
    TAPA_OUTPUT_LEVEL_HIGH = 0U,       ///< The monitored output is high.
    TAPA_OUTPUT_LEVEL_LOW  = 1U        ///< The monitored output is low.
} tapa_output_level_t;

/** TAPA output monitor availability. */
typedef enum e_tapa_output_monitor
{
    TAPA_OUTPUT_MONITOR_NOT_AVAILABLE = 0U, ///< Output monitor is not available for this TAPA unit.
    TAPA_OUTPUT_MONITOR_AVAILABLE     = 1U  ///< Output monitor is available for this TAPA unit.
} tapa_output_monitor_t;

/** TAPA asynchronous Hi-Z control status. */
typedef struct st_tapa_status
{
    tapa_hiz_operation_status_t hiz_operation_status; ///< Asynchronous Hi-Z control operation status.

    tapa_output_level_t u_phase_output_level;         ///< TAPAnTHZOUT0 monitor output level.
    tapa_output_level_t v_phase_output_level;         ///< TAPAnTHZOUT1 valid only when v_w_phase_monitor is AVAILABLE.
    tapa_output_level_t w_phase_output_level;         ///< TAPAnTHZOUT2 valid only when v_w_phase_monitor is AVAILABLE.

    tapa_output_monitor_t v_w_phase_monitor;          ///< Availability of HOF1 and HOF2.
} tapa_status_t;

/** TAPA control block.  Allocate an instance specific control block to pass into the TAPA API calls.
 */
typedef void tapa_ctrl_t;

/** User configuration structure, used in the open function. */
typedef struct st_tapa_cfg
{
    void const * p_extend;                               ///< Extension parameter for hardware specific settings.
    void (* p_callback)(timer_callback_args_t * p_args); ///< Callback called when a TAPA interrupt occurs.
    void    * p_context;                                 ///< User defined context passed into callback function.
    IRQn_Type valley_irq;                                ///< Valley interrupt vector
    uint8_t   valley_ipl;                                ///< Valley interrupt priority
    IRQn_Type peak_irq;                                  ///< Valley interrupt vector
    uint8_t   peak_ipl;                                  ///< Valley interrupt priority
} tapa_cfg_t;

/** TAPA API structure. TAPA functions implemented at the HAL layer will follow this API. */
typedef struct st_tapa_api
{
    /** Initial configuration.
     *
     * @param[in]   p_ctrl      Pointer to control block. Must be declared by user. Elements set here.
     * @param[in]   p_cfg       Pointer to configuration structure. All elements of this structure must be set by user.
     */
    fsp_err_t (* open)(tapa_ctrl_t * const p_ctrl, tapa_cfg_t const * const p_cfg);

    /** Enables asynchronous Hi-Z control.
     *
     * @param[in]   p_ctrl      Control block set in @ref tapa_api_t::open call.
     */
    fsp_err_t (* start)(tapa_ctrl_t * const p_ctrl);

    /** Disable asynchronous Hi-Z control.
     *
     * @param[in]   p_ctrl      Control block set in @ref tapa_api_t::open call.
     */
    fsp_err_t (* stop)(tapa_ctrl_t * const p_ctrl);

    /** Disables TAPA interrupt.
     *
     * @param[in]   p_ctrl      Control block set in @ref tapa_api_t::open call.
     */
    fsp_err_t (* close)(tapa_ctrl_t * const p_ctrl);

    /** Software trigger to start Hi-Z control.
     *
     * @param[in]   p_ctrl      Control block set in @ref tapa_api_t::open call.
     */
    fsp_err_t (* startTrigger)(tapa_ctrl_t * const p_ctrl);

    /** Software trigger to stop Hi-Z control.
     *
     * @param[in]   p_ctrl      Control block set in @ref tapa_api_t::open call.
     */
    fsp_err_t (* stopTrigger)(tapa_ctrl_t * const p_ctrl);

    /** Gets the current asynchronous Hi-Z control status.
     *
     * @param[in]  p_ctrl                 Control block set in @ref tapa_api_t::open call.
     * @param[out] p_status               Pointer to the TAPA status structure.
     */
    fsp_err_t (* statusGet)(tapa_ctrl_t * const p_ctrl, tapa_status_t * const p_status);

    /** Specify callback function and optional context pointer and working memory pointer.
     *
     * @param[in]   p_ctrl                   Control block set in @ref tapa_api_t::open call.
     * @param[in]   p_callback               Callback function
     * @param[in]   p_context                Pointer to send to callback function
     * @param[in]   p_callback_memory        Pointer to volatile memory where callback structure can be allocated.
     *                                       Callback arguments allocated here are only valid during the callback.
     */
    fsp_err_t (* callbackSet)(tapa_ctrl_t * const p_ctrl, void (* p_callback)(timer_callback_args_t * p_args),
                              void * const p_context, timer_callback_args_t * const p_callback_memory);
} tapa_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_tapa_instance
{
    tapa_ctrl_t      * p_ctrl;         ///< Pointer to the control structure for this instance
    tapa_cfg_t const * p_cfg;          ///< Pointer to the configuration structure for this instance
    tapa_api_t const * p_api;          ///< Pointer to the API structure for this instance
} tapa_instance_t;

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

/*******************************************************************************************************************//**
 * @} (end defgroup TAPA_API)
 **********************************************************************************************************************/

#endif                                 /* R_TAPA_API_H */
