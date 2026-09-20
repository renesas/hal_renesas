/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup TAPA
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

#ifndef R_TAPA_H
#define R_TAPA_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "r_tapa_api.h"
#include "r_tapa_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Specifies the trigger enable value used for both Hi-Z Control start and stop registers */
typedef enum e_tapa_hiz_control_trigger                              /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_HIZ_CONTROL_TRIGGER_ENABLE = 1U                             ///< Enable Hi-Z control trigger.
} tapa_hiz_control_trigger_t;                                        /* PRQA S 1535 # Advisory R-2.3 */

/** Specifies Asynchronous Hi-Z Control function */
typedef enum e_tapa_func_asynchronous_hiz_control                    /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_DISABLE = 0U,                 ///< Asynchronous Hi-Z Control function is enable
    TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_ENABLE  = 1U,                 ///< Asynchronous Hi-Z Control function is disable
} tapa_func_asynchronous_hiz_control_t;

/** Specifies A/D Converter Conversion Trigger function */
typedef enum e_tapa_func_ad_conversion_trigger                       /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_FUNC_AD_CONVERSION_TRIGGER_DISABLE = 0U,                    ///< Asynchronous Hi-Z Control function is enable
    TAPA_FUNC_AD_CONVERSION_TRIGGER_ENABLE  = 1U,                    ///< Asynchronous Hi-Z Control function is disable
} tapa_func_ad_conversion_trigger_t;

/** Specifies Asynchronous Hi-Z Control start trigger function */
typedef enum e_tapa_asynchronous_hiz_control_start_trigger           /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_ASYNCHRONOUS_HIZ_CONTROL_START_TRIGGER_DISABLE = 0U,        ///< Disable the start trigger for asynchronous Hi-Z control.
    TAPA_ASYNCHRONOUS_HIZ_CONTROL_START_TRIGGER_ENABLE  = 1U         ///< Enable the start trigger for asynchronous Hi-Z control.
} tapa_asynchronous_hiz_control_start_trigger_t;                     /* PRQA S 1535 # Advisory R-2.3 */

/** Specifies Asynchronous Hi-Z Control stop trigger function */
typedef enum e_tapa_asynchronous_hiz_control_stop_trigger            /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_ASYNCHRONOUS_HIZ_CONTROL_STOP_TRIGGER_DISABLE = 0U,         ///< Disable the stop trigger for asynchronous Hi-Z control.
    TAPA_ASYNCHRONOUS_HIZ_CONTROL_STOP_TRIGGER_ENABLE  = 1U          ///< Enable the stop trigger for asynchronous Hi-Z control.
} tapa_asynchronous_hiz_control_stop_trigger_t;                      /* PRQA S 1535 # Advisory R-2.3 */

/** Specifies Asynchronous Hi-Z Control write enable */
typedef enum e_tapa_asynchronous_hiz_control_write                   /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_ASYNCHRONOUS_HIZ_CONTROL_WRITE_DISABLE = 0U,                ///< Disable writing to TAPAnACTS and TAPAnACTT
    TAPA_ASYNCHRONOUS_HIZ_CONTROL_WRITE_ENABLE  = 1U                 ///< Enable writing to TAPAnACTS and TAPAnACTT
} tapa_asynchronous_hiz_control_write_t;                             /* PRQA S 1535 # Advisory R-2.3 */

/** This setting specifies the clear conditions for Hi-Z control output. */
typedef enum e_tapa_clear_condition                                  /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_CLEAR_CONDITION_REGARDLESS_THASHIN_SIGNAL_LEVEL = 0U,       ///< Enables manipulation of TAPAnOPHT0 regardless of the TAPAnTHASIN signal input level.
    TAPA_CLEAR_CONDITION_ONLY_THASIN_SIGNAL_INACTIVE     = 1U,       ///< Enables manipulation of TAPAnOPHT0 only if the TAPAnTHASIN signal input is inactive.
} tapa_clear_condition_t;

/** Hi-Z Input Edge Selection. */
typedef enum e_tapa_input_edge                                       /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_INPUT_EDGE_NOT_DETECT = 0U,                                 ///< Does not detect valid edges
    TAPA_INPUT_EDGE_RISING     = 4U,                                 ///< Detects a rising edge as the valid edge (active level = high)
    TAPA_INPUT_EDGE_FALLING    = 8U,                                 ///< Detects a falling edge as the valid edge (active level = low)
} tapa_input_edge_t;

/** A/D Conversion Trigger Selection */
typedef enum e_tapa_interrupt_trigger                                /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_INTERRUPT_TRIGGER_FALLING                      = 0U,        ///< INT signal while the triangle wave is falling (counting down)
    TAPA_INTERRUPT_TRIGGER_RISING                       = 1U,        ///< INT signal while the triangle wave is rising (counting up)
    TAPA_INTERRUPT_TRIGGER_RISING_OR_FALLING            = 2U,        ///< INT signal while the triangle wave is rising (counting up) or falling (counting down)
    TAPA_INTERRUPT_TRIGGER_RISING_OR_FALLING_AND_VALLEY = 3U,        ///< INT signal while the triangle wave is rising (counting up) or falling (counting down) and valley interrupt INTTAPAnIVLY0 signal
} tapa_interrupt_trigger_t;

/** Unit source */
typedef enum e_tapa_unit                                             /* PRQA S 1536 # Advisory R-2.4 */
{
    TAPA_UNIT_0 = 0,                                                 ///< TAPA unit 0
    TAPA_UNIT_1 = 1,                                                 ///< TAPA unit 1
    TAPA_UNIT_2 = 2,                                                 ///< TAPA unit 2
    TAPA_UNIT_3 = 3,                                                 ///< TAPA unit 3
    TAPA_UNIT_4 = 4,                                                 ///< TAPA unit 4
    TAPA_UNIT_5 = 5,                                                 ///< TAPA unit 5
} tapa_unit_t;

/** TAPA extension parameter for hardware specific settings. */
typedef struct st_tapa_extended_cfg                                  /* PRQA S 1536 # Advisory R-2.4 */
{
    tapa_func_asynchronous_hiz_control_t async_hiz_control_func;     ///< Specifies Asynchronous Hi-Z Control function
    tapa_func_ad_conversion_trigger_t    ad_conversion_trigger_func; ///< Specifies A/D Converter Conversion Trigger function
    tapa_clear_condition_t               clear_condition;            ///< Clear Condition Configuration
    tapa_input_edge_t        thasin_input_edge;                      ///< Hi-Z Input Edge Selection
    tapa_interrupt_trigger_t ad_conversion_trigger_0;                ///< A/D Conversion Trigger 0 Selection
    tapa_interrupt_trigger_t ad_conversion_trigger_1;                ///< A/D Conversion Trigger 1 Selection
    tapa_unit_t              tapa_unit;                              ///< Unit of TAPA module
} tapa_extended_cfg_t;

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when @ref tapa_api_t::open is called. */
typedef struct st_tapa_instance_ctrl                                 /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t           open;                                         ///< Whether or not channel is open
    const tapa_cfg_t * p_cfg;                                        ///< Pointer to initial configurations
    R_TAPA_Type      * p_reg;                                        ///< Base register for this unit
    void (* p_callback)(timer_callback_args_t * p_args);             ///< Pointer to callback
    timer_callback_args_t * p_callback_memory;                       ///< Pointer to optional callback argument memory
    void * p_context;                                                ///< Pointer to context to be passed into callback function
} tapa_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const tapa_api_t g_tapa_on_tapa;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_TAPA_Open(tapa_ctrl_t * const p_ctrl, tapa_cfg_t const * const p_cfg);
fsp_err_t R_TAPA_Start(tapa_ctrl_t * const p_ctrl);
fsp_err_t R_TAPA_Stop(tapa_ctrl_t * const p_ctrl);
fsp_err_t R_TAPA_Close(tapa_ctrl_t * const p_ctrl);
fsp_err_t R_TAPA_StartTrigger(tapa_ctrl_t * const p_ctrl);
fsp_err_t R_TAPA_StopTrigger(tapa_ctrl_t * const p_ctrl);
fsp_err_t R_TAPA_StatusGet(tapa_ctrl_t * const p_ctrl, tapa_status_t * const p_status);
fsp_err_t R_TAPA_CallbackSet(tapa_ctrl_t * const           p_ctrl,
                             void (                      * p_callback)(timer_callback_args_t * p_args),
                             void * const                  p_context,
                             timer_callback_args_t * const p_callback_memory);

/*******************************************************************************************************************//**
 * @} (end defgroup TAPA)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_TAPA_H */
