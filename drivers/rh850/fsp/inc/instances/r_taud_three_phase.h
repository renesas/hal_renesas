/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_TAUD_THREE_PHASE_H
#define R_TAUD_THREE_PHASE_H

/* PRQA S 0292 ++ # DP_MSR_D-1.1 */

/*******************************************************************************************************************//**
 * @addtogroup TAUD_THREE_PHASE
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_taud.h"
#include "r_three_phase_api.h"
#include "r_taud_three_phase_cfg.h"
#include "r_tapa.h"
#include "r_tapa_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define TAUD_THREE_PHASE_SLAVE_CHANNEL_NUMBER    (6U)
#define TAUD_MASTER_CHANNEL_ONE_SHOT_NUMBER      (3U)
#define TAUD_SLAVE_CHANNEL_ONE_SHOT_NUMBER       (3U)
#define TAUD_DUTY_CHANNEL_NUMBER                 (3U)

#define TAUD_UNIT_1                              (1U)

#if (BSP_FEATURE_TAUD_THREE_PHASE_NUMBER_OF_UNIT == 3U)
 #define TAUD_UNIT_2                             (2U)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Channel control block. DO NOT INITIALIZE. Initialization occurs when @ref three_phase_api_t::open is called. */
typedef struct st_taud_three_phase_instance_ctrl             /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t open;                                           ///< Whether or not channel is open
    three_phase_cfg_t const * p_cfg;                         ///< Pointer to initial configurations
    uint16_t      channels_mask;                             // Masked value of all channels used
    uint16_t      output_mask;                               // Masked value of all output channels used
    R_TAUD_Type * p_taud_reg;                                ///< Base register for TAUD

    void (* p_callback)(timer_callback_args_t * p_arg);      ///< Pointer to callback
    timer_callback_args_t * p_callback_memory;               ///< Pointer to pre-allocated callback argument
    void * p_context;                                        ///< Pointer to context to be passed into callback function
} taud_three_phase_instance_ctrl_t;

/** Optional TAUD extension data structure */
typedef struct st_taud_three_phase_extended_cfg              /* PRQA S 1536 # Advisory R-2.4 */
{
    uint8_t  taud_unit;
    uint32_t duty_cycle[3];                                  ///< Duty cycle of TAUD channels U, V, W

    tapa_instance_t const * const  p_tapa_taud;              ///< TAPA - TAUD input selector
    timer_instance_t const * const p_taud_master_pwm;        ///< Master channel for Triangle PWM with Dead Time
    timer_instance_t const * const p_taud_slave_pwm[6];      ///< Slave channels for Triangle PWM with Dead Time
    timer_instance_t const * const p_taud_master_oneshot[3]; ///< Master channels for One-shot Pulse
    timer_instance_t const * const p_taud_slave_oneshot[3];  ///< Slave channels for One-shot Pulse
} taud_three_phase_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const three_phase_api_t g_taud_three_phase_on_taud_three_phase;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_TAUD_THREE_PHASE_Open(three_phase_ctrl_t * const p_ctrl, three_phase_cfg_t const * const p_cfg);
fsp_err_t R_TAUD_THREE_PHASE_Start(three_phase_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_THREE_PHASE_Stop(three_phase_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_THREE_PHASE_Reset(three_phase_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_THREE_PHASE_DutyCycleSet(three_phase_ctrl_t * const       p_ctrl,
                                          three_phase_duty_cycle_t * const p_duty_cycle);
fsp_err_t R_TAUD_THREE_PHASE_CallbackSet(three_phase_ctrl_t * const    p_ctrl,
                                         void (                      * p_callback)(timer_callback_args_t *), /* PRQA S 1336 # Rule-8.2 */
                                         void * const                  p_context,
                                         timer_callback_args_t * const p_callback_memory);
fsp_err_t R_TAUD_THREE_PHASE_Close(three_phase_ctrl_t * const p_ctrl);

/*******************************************************************************************************************//**
 * @} (end defgroup TAUD_THREE_PHASE)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_TAUD_THREE_PHASE_H */
