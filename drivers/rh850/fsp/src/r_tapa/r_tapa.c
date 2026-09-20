/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_tapa.h"                    /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "r_tapa_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "TAPA" in ASCII, used to determine if the module is open */
#define TAPA_OPEN                             (0x54415041U)
#define TAPA_CLOSED                           (0x00000000U)

#if (0x0001U == BSP_FEATURE_TAPA_SUPPORTED_VARIANT)
 #define TAPA_AD_TRIGGER_0_FIELD_MASK         ((uint32_t) (R_TAPA0_TAPAnCTL1_TAPAnATS1_Msk | \
                                                           R_TAPA0_TAPAnCTL1_TAPAnATS0_Msk))
 #define TAPA_AD_TRIGGER_1_FIELD_MASK         ((uint32_t) (R_TAPA0_TAPAnCTL1_TAPAnATS3_Msk | \
                                                           R_TAPA0_TAPAnCTL1_TAPAnATS2_Msk))
 #define TAPA_AD_TRIGGER_0_FIELD_POS          ((uint32_t) R_TAPA0_TAPAnCTL1_TAPAnATS0_Pos)
 #define TAPA_AD_TRIGGER_1_FIELD_POS          ((uint32_t) R_TAPA0_TAPAnCTL1_TAPAnATS2_Pos)

 #define TAPA_FLAG_ASYNC_HIZ_ENABLE_MASK      ((uint16_t) R_TAPA0_TAPAnFLG_TAPAnACE_Msk)
 #define TAPA_FLAG_U_PHASE_OUTPUT_MASK        ((uint16_t) R_TAPA0_TAPAnFLG_TAPAnHOF0_Msk)
 #define TAPA_FLAG_V_PHASE_OUTPUT_MASK        ((uint16_t) R_TAPA0_TAPAnFLG_TAPAnHOF1_Msk)
 #define TAPA_FLAG_W_PHASE_OUTPUT_MASK        ((uint16_t) R_TAPA0_TAPAnFLG_TAPAnHOF2_Msk)

#else
 #define TAPA_AD_TRIGGER_0_FIELD_MASK         ((uint32_t) (R_TAPA1_TAPAnCTL1_TAPAnATS1_Msk | \
                                                           R_TAPA1_TAPAnCTL1_TAPAnATS0_Msk))
 #define TAPA_AD_TRIGGER_1_FIELD_MASK         ((uint32_t) (R_TAPA1_TAPAnCTL1_TAPAnATS3_Msk | \
                                                           R_TAPA1_TAPAnCTL1_TAPAnATS2_Msk))
 #define TAPA_AD_TRIGGER_0_FIELD_POS          ((uint32_t) R_TAPA1_TAPAnCTL1_TAPAnATS0_Pos)
 #define TAPA_AD_TRIGGER_1_FIELD_POS          ((uint32_t) R_TAPA1_TAPAnCTL1_TAPAnATS2_Pos)

 #define TAPA_FLAG_ASYNC_HIZ_ENABLE_MASK      ((uint16_t) R_TAPA1_TAPAnFLG_TAPAnACE_Msk)
 #define TAPA_FLAG_U_PHASE_OUTPUT_MASK        ((uint16_t) R_TAPA1_TAPAnFLG_TAPAnHOF0_Msk)
 #define TAPA_FLAG_V_PHASE_OUTPUT_MASK        ((uint16_t) R_TAPA1_TAPAnFLG_TAPAnHOF1_Msk)
 #define TAPA_FLAG_W_PHASE_OUTPUT_MASK        ((uint16_t) R_TAPA1_TAPAnFLG_TAPAnHOF2_Msk)

#endif

#if (0x0010U == BSP_FEATURE_TAPA_SUPPORTED_VARIANT) || (0x0100U == BSP_FEATURE_TAPA_SUPPORTED_VARIANT)
 #define TAPA_UNIT_COUNT                      (6U)
 #define TAPA_HOF1_HOF2_MAX_SUPPORTED_UNIT    ((uint32_t) TAPA_UNIT_2)

/* PRQA S 2053 ++ # Advisory D-4.4 */
#else                                  /* (0x0001U == BSP_FEATURE_TAPA_SUPPORTED_VARIANT) || (0x1000U == BSP_FEATURE_TAPA_SUPPORTED_VARIANT) */
/* PRQA S 2053 -- */
 #define TAPA_UNIT_COUNT                      (4U)
 #define TAPA_HOF1_HOF2_MAX_SUPPORTED_UNIT    ((uint32_t) TAPA_UNIT_1)

#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void tapa_common_open(tapa_instance_ctrl_t * const p_instance_ctrl, tapa_cfg_t const * const p_cfg);
static void tapa_hardware_initialize(tapa_instance_ctrl_t * const p_instance_ctrl);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void tapa_int_valley_isr(void); /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1, DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void tapa_int_peak_isr(void);   /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1, DP_MSR_R-8.5_UC1 */

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* TAPA Implementation of TAPA Driver  */
const tapa_api_t g_tapa_on_tapa =      /* PRQA S 1502 # Advisory R-2.8 */
{
    .open         = &R_TAPA_Open,
    .start        = &R_TAPA_Start,
    .stop         = &R_TAPA_Stop,
    .startTrigger = &R_TAPA_StartTrigger,
    .stopTrigger  = &R_TAPA_StopTrigger,
    .statusGet    = &R_TAPA_StatusGet,
    .close        = &R_TAPA_Close,
    .callbackSet  = &R_TAPA_CallbackSet,
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * Initializes the TAPA module and applies configurations. Implements @ref tapa_api_t::open.
 *
 * @retval FSP_SUCCESS                    Initialization was successful.
 * @retval FSP_ERR_ASSERTION              Required pointer was NULL.
 * @retval FSP_ERR_ALREADY_OPEN           Module is already open.
 **********************************************************************************************************************/

/* PRQA S 0292 -- */
fsp_err_t R_TAPA_Open (tapa_ctrl_t * const p_ctrl, tapa_cfg_t const * const p_cfg) /* PRQA S 1505 EOF # Advisory R-8.7 */
{
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) p_ctrl;      /* PRQA S 0316 EOF # Advisory R-11.5 */

#if TAPA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT((NULL != p_instance_ctrl));
    FSP_ASSERT((NULL != p_cfg));
    FSP_ERROR_RETURN((TAPA_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);
#endif

    /* Initializes control structure */
    tapa_common_open(p_instance_ctrl, p_cfg);

    /* Performs hardware initialization of the TAPA */
    tapa_hardware_initialize(p_instance_ctrl);

    p_instance_ctrl->open = TAPA_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enable asynchronous Hi-Z control. Implements @ref tapa_api_t::start.
 *
 * @retval FSP_SUCCESS                 Asynchronous Hi-Z control successfully enabled.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 * @retval FSP_ERR_INVALID_MODE        This API only supports Asynchronous Hi-Z Control function.
 * @retval FSP_ERR_ABORTED             ACWE bit was not set successfully. Start operation aborted.
 **********************************************************************************************************************/
fsp_err_t R_TAPA_Start (tapa_ctrl_t * const p_ctrl)                           /* PRQA S 3673 EOF # Advisory R-8.13 */
{
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) p_ctrl; /* PRQA S 3678 EOF # Advisory R-8.13 */

#if TAPA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT((NULL != p_instance_ctrl));
    FSP_ERROR_RETURN((TAPA_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);
    tapa_extended_cfg_t const * p_extend = (tapa_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;
    FSP_ERROR_RETURN((TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_ENABLE == p_extend->async_hiz_control_func),
                     FSP_ERR_INVALID_MODE);
#endif

    /* Enable writing for asynchronous Hi-Z control. */
    p_instance_ctrl->p_reg->TAPAnACWE_b.TAPAnACWE = (uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_WRITE_ENABLE; /* PRQA S 0662 EOF # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm ACWE was actually set to 1 before writing ACTS,
     * since the setting of TAPAnACTS is only valid when TAPAnACWE = 1. */
    if ((uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_WRITE_ENABLE == p_instance_ctrl->p_reg->TAPAnACWE_b.TAPAnACWE)
    {
        /* Enable the start trigger for asynchronous Hi-Z control. */
        p_instance_ctrl->p_reg->TAPAnACTS_b.TAPAnACTS = (uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_START_TRIGGER_ENABLE;
    }
    else
    {

        /* ACWE was not set successfully, abort the operation. */
        return FSP_ERR_ABORTED;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Disable asynchronous Hi-Z control. Implements @ref tapa_api_t::stop.
 *
 * @retval FSP_SUCCESS                 Asynchronous Hi-Z control successfully disabled.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 * @retval FSP_ERR_INVALID_MODE        This API only supports Asynchronous Hi-Z Control function.
 * @retval FSP_ERR_ABORTED             ACWE bit was not set successfully. Stop operation aborted.
 **********************************************************************************************************************/
fsp_err_t R_TAPA_Stop (tapa_ctrl_t * const p_ctrl)
{
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) p_ctrl;

#if TAPA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT((NULL != p_instance_ctrl));
    FSP_ERROR_RETURN((TAPA_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);
    tapa_extended_cfg_t const * p_extend = (tapa_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;
    FSP_ERROR_RETURN((TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_ENABLE == p_extend->async_hiz_control_func),
                     FSP_ERR_INVALID_MODE);
#endif

    /* Enable writing for asynchronous Hi-Z control. */
    p_instance_ctrl->p_reg->TAPAnACWE_b.TAPAnACWE = (uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_WRITE_ENABLE;

    /* Confirm ACWE was actually set to 1 before writing ACTT,
     * since the setting of TAPAnACTT is only valid when TAPAnACWE = 1. */
    if ((uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_WRITE_ENABLE == p_instance_ctrl->p_reg->TAPAnACWE_b.TAPAnACWE)
    {
        /* Enables the stop trigger for asynchronous Hi-Z control. */
        p_instance_ctrl->p_reg->TAPAnACTT_b.TAPAnACTT = (uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_STOP_TRIGGER_ENABLE;
    }
    else
    {

        /* ACWE was not set successfully, abort the operation. */
        return FSP_ERR_ABORTED;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Software trigger to start Hi-Z control. Implements @ref tapa_api_t::startTrigger.
 *
 * @retval FSP_SUCCESS                 Software trigger to start Hi-Z control successfully.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 * @retval FSP_ERR_INVALID_MODE        This API only supports Asynchronous Hi-Z Control function.
 **********************************************************************************************************************/
fsp_err_t R_TAPA_StartTrigger (tapa_ctrl_t * const p_ctrl)
{
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) p_ctrl;
#if TAPA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT((NULL != p_instance_ctrl));
    FSP_ERROR_RETURN((TAPA_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);
    tapa_extended_cfg_t const * p_extend = (tapa_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;
    FSP_ERROR_RETURN((TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_ENABLE == p_extend->async_hiz_control_func),
                     FSP_ERR_INVALID_MODE);
#endif

    p_instance_ctrl->p_reg->TAPAnOPHS_b.TAPAnOPHS0 = (uint8_t) TAPA_HIZ_CONTROL_TRIGGER_ENABLE;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Software trigger to stop Hi-Z control. Implements @ref tapa_api_t::stopTrigger.
 *
 * @retval FSP_SUCCESS                 Software trigger to stop Hi-Z control successfully.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 * @retval FSP_ERR_INVALID_MODE        This API only supports Asynchronous Hi-Z Control function.
 **********************************************************************************************************************/
fsp_err_t R_TAPA_StopTrigger (tapa_ctrl_t * const p_ctrl)
{
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) p_ctrl;
#if TAPA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT((NULL != p_instance_ctrl));
    FSP_ERROR_RETURN((TAPA_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);
    tapa_extended_cfg_t const * p_extend = (tapa_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;
    FSP_ERROR_RETURN((TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_ENABLE == p_extend->async_hiz_control_func),
                     FSP_ERR_INVALID_MODE);
#endif

    p_instance_ctrl->p_reg->TAPAnOPHT_b.TAPAnOPHT0 = (uint8_t) TAPA_HIZ_CONTROL_TRIGGER_ENABLE;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Gets the current asynchronous Hi-Z control status. Implements @ref tapa_api_t::statusGet.
 *
 * TAPAnFLG is read once as a 16-bit value. TAPAnHOF1 and TAPAnHOF2 availability depends on the selected TAPA unit:
 *
 * - U2Bx/U2BxE: HOF1 and HOF2 are available for units 0 to 2.
 * - U2Ax/U2Cx:  HOF1 and HOF2 are available for units 0 to 1.
 *
 * @param[in]  p_ctrl                 Pointer to the TAPA control block.
 * @param[out] p_status               Pointer to the TAPA status structure.
 *
 * @retval FSP_SUCCESS                Status was read successfully.
 * @retval FSP_ERR_ASSERTION          p_ctrl or p_status was NULL.
 * @retval FSP_ERR_NOT_OPEN           The instance is not opened.
 * @retval FSP_ERR_INVALID_MODE       Asynchronous Hi-Z control is not enabled.
 **********************************************************************************************************************/
fsp_err_t R_TAPA_StatusGet (tapa_ctrl_t * const p_ctrl, tapa_status_t * const p_status)
{
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) p_ctrl;

#if TAPA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_status);

    FSP_ERROR_RETURN((TAPA_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);
#endif

    tapa_extended_cfg_t const * p_extend = (tapa_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;

#if TAPA_CFG_PARAM_CHECKING_ENABLE
    FSP_ERROR_RETURN((TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_ENABLE == p_extend->async_hiz_control_func),
                     FSP_ERR_INVALID_MODE);
#endif

    uint16_t const flag_register = p_instance_ctrl->p_reg->TAPAnFLG;
    uint32_t const flag_value    = (uint32_t) flag_register;
    uint32_t const tapa_unit     = (uint32_t) p_extend->tapa_unit;

    /* Check the operation status of Asynchronous Hi-Z control */
    if (0U != (flag_value & TAPA_FLAG_ASYNC_HIZ_ENABLE_MASK))
    {
        p_status->hiz_operation_status = TAPA_HIZ_OPERATION_STATUS_ENABLED;
    }
    else
    {
        p_status->hiz_operation_status = TAPA_HIZ_OPERATION_STATUS_STOPPED;
    }

    /* TAPAnHOF0: U phase Hi-Z Control Monitor */
    if (0U != (flag_value & TAPA_FLAG_U_PHASE_OUTPUT_MASK))
    {
        p_status->u_phase_output_level = TAPA_OUTPUT_LEVEL_LOW;
    }
    else
    {
        p_status->u_phase_output_level = TAPA_OUTPUT_LEVEL_HIGH;
    }

    /* HOF1 and HOF2 values shall only be used when v_w_phase_monitor is AVAILABLE. */
    p_status->v_phase_output_level = TAPA_OUTPUT_LEVEL_HIGH;
    p_status->w_phase_output_level = TAPA_OUTPUT_LEVEL_HIGH;
    p_status->v_w_phase_monitor    = TAPA_OUTPUT_MONITOR_NOT_AVAILABLE;

    if (tapa_unit <= TAPA_HOF1_HOF2_MAX_SUPPORTED_UNIT)
    {
        p_status->v_w_phase_monitor = TAPA_OUTPUT_MONITOR_AVAILABLE;

        /* TAPAnHOF1: V phase Hi-Z Control Monitor */
        if (0U != (flag_value & TAPA_FLAG_V_PHASE_OUTPUT_MASK))
        {
            p_status->v_phase_output_level = TAPA_OUTPUT_LEVEL_LOW;
        }
        else
        {
            p_status->v_phase_output_level = TAPA_OUTPUT_LEVEL_HIGH;
        }

        /* TAPAnHOF2: W phase Hi-Z Control Monitor */
        if (0U != (flag_value & TAPA_FLAG_W_PHASE_OUTPUT_MASK))
        {
            p_status->w_phase_output_level = TAPA_OUTPUT_LEVEL_LOW;
        }
        else
        {
            p_status->w_phase_output_level = TAPA_OUTPUT_LEVEL_HIGH;
        }
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop operations and clear initialization for TAPA module.
 * Implements @ref tapa_api_t::close.
 *
 * @retval FSP_SUCCESS                 Stop operations and clear initialization successfully.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 * @retval FSP_ERR_ABORTED             ACWE bit was not set successfully. Stop operation aborted.
 **********************************************************************************************************************/
fsp_err_t R_TAPA_Close (tapa_ctrl_t * const p_ctrl)
{
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) p_ctrl;

#if TAPA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT((NULL != p_instance_ctrl));
    FSP_ERROR_RETURN((TAPA_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);
#endif

    tapa_extended_cfg_t const * p_extend = (tapa_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;

    if (TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_ENABLE == p_extend->async_hiz_control_func)
    {
        /* Enable writing for asynchronous Hi-Z control. */
        p_instance_ctrl->p_reg->TAPAnACWE_b.TAPAnACWE = (uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_WRITE_ENABLE;

        /* Confirm ACWE was actually set to 1 before writing ACTT,
         * since the setting of TAPAnACTT is only valid when TAPAnACWE = 1. */
        if ((uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_WRITE_ENABLE == p_instance_ctrl->p_reg->TAPAnACWE_b.TAPAnACWE)
        {
            /* Enables the stop trigger for asynchronous Hi-Z control. */
            p_instance_ctrl->p_reg->TAPAnACTT_b.TAPAnACTT = (uint8_t) TAPA_ASYNCHRONOUS_HIZ_CONTROL_STOP_TRIGGER_ENABLE;
        }
        else
        {

            /* ACWE was not set successfully, abort the operation. */
            return FSP_ERR_ABORTED;
        }

        /* Clean up the TAPAnCTL0 register. */
        p_instance_ctrl->p_reg->TAPAnCTL0 = (0U);
    }

    /* Clean up the setting for A/D Conversion Trigger Selection */
    if (TAPA_FUNC_AD_CONVERSION_TRIGGER_ENABLE == p_extend->ad_conversion_trigger_func)
    {
        p_instance_ctrl->p_reg->TAPAnCTL1 = (0U);
    }

    /* Disable interrupts. */
    if (FSP_INVALID_VECTOR != p_instance_ctrl->p_cfg->valley_irq)
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->valley_irq);
    }

    if (FSP_INVALID_VECTOR != p_instance_ctrl->p_cfg->peak_irq)
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->peak_irq);
    }

    /* Clear open flag. */
    p_instance_ctrl->open = TAPA_CLOSED;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements @ref tapa_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_TAPA_CallbackSet (tapa_ctrl_t * const           p_ctrl,
                              void (                      * p_callback)(timer_callback_args_t * p_args),
                              void * const                  p_context,
                              timer_callback_args_t * const p_callback_memory)
{
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) p_ctrl;

#if (TAPA_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT((NULL != p_instance_ctrl));
    FSP_ASSERT((NULL != p_callback));
    FSP_ERROR_RETURN((TAPA_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Initializes control structure based on configuration.
 *
 * @param[in]  p_instance_ctrl         Instance control block.
 * @param[in]  p_cfg                   Pointer to TAPA configuration.
 **********************************************************************************************************************/
static void tapa_common_open (tapa_instance_ctrl_t * const p_instance_ctrl, tapa_cfg_t const * const p_cfg)
{
    /* Initialize control structure.  */
    p_instance_ctrl->p_cfg = p_cfg;

    /* Save pointer to extended configuration structure. */
    tapa_extended_cfg_t const * p_extend = (tapa_extended_cfg_t const *) p_cfg->p_extend;

    /* Set base address of TAPA module */
    static R_TAPA_Type * const g_tapa_base[TAPA_UNIT_COUNT] =
    {
        R_TAPA0,                       /* PRQA S 0306 # Advisory R-11.4 */
        R_TAPA1,                       /* PRQA S 0306 # Advisory R-11.4 */
        R_TAPA2,                       /* PRQA S 0306 # Advisory R-11.4 */
        R_TAPA3,                       /* PRQA S 0306 # Advisory R-11.4 */
#if (0x0010U == BSP_FEATURE_TAPA_SUPPORTED_VARIANT) || (0x0100U == BSP_FEATURE_TAPA_SUPPORTED_VARIANT)
        R_TAPA4,                       /* PRQA S 0306 # Advisory R-11.4 */
        R_TAPA5                        /* PRQA S 0306 # Advisory R-11.4 */
#endif
    };

    /* Set base address */
    uint32_t const tapa_unit = (uint32_t) p_extend->tapa_unit;
    p_instance_ctrl->p_reg = g_tapa_base[tapa_unit];

    /* Set callback and context pointers, if configured */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;
}

/*******************************************************************************************************************//**
 * Performs hardware initialization of the TAPA.
 *
 * @param[in]  p_instance_ctrl        Instance control block.
 **********************************************************************************************************************/
static void tapa_hardware_initialize (tapa_instance_ctrl_t * const p_instance_ctrl)
{
    tapa_extended_cfg_t const * p_extend = (tapa_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;

    if (TAPA_FUNC_ASYNCHRONOUS_HIZ_CONTROL_ENABLE == p_extend->async_hiz_control_func)
    {
        uint32_t const clear_condition = (uint32_t) p_extend->clear_condition;
        uint32_t const input_edge      = (uint32_t) p_extend->thasin_input_edge;
        uint16_t const ctl0_value      = (uint16_t) ((clear_condition << 4U) | input_edge);

        p_instance_ctrl->p_reg->TAPAnCTL0 = ctl0_value;
    }

    if (TAPA_FUNC_AD_CONVERSION_TRIGGER_ENABLE == p_extend->ad_conversion_trigger_func)
    {
        uint32_t const trigger_0 = (((uint32_t) p_extend->ad_conversion_trigger_0) << TAPA_AD_TRIGGER_0_FIELD_POS) &
                                   TAPA_AD_TRIGGER_0_FIELD_MASK;

        uint32_t const trigger_1 = (((uint32_t) p_extend->ad_conversion_trigger_1) << TAPA_AD_TRIGGER_1_FIELD_POS) &
                                   TAPA_AD_TRIGGER_1_FIELD_MASK;

        /* Setting specifies the A/D conversion trigger */
        uint8_t const ctl1_value = (uint8_t) (trigger_1 | trigger_0);

        p_instance_ctrl->p_reg->TAPAnCTL1 = ctl1_value;
    }

    /* Enable valley interrupts */
    if (FSP_INVALID_VECTOR != p_instance_ctrl->p_cfg->valley_irq)
    {
        R_BSP_IrqCfgEnable(p_instance_ctrl->p_cfg->valley_irq, p_instance_ctrl->p_cfg->valley_ipl, p_instance_ctrl);
    }

    /* Enable peak interrupts */
    if (FSP_INVALID_VECTOR != p_instance_ctrl->p_cfg->peak_irq)
    {
        R_BSP_IrqCfgEnable(p_instance_ctrl->p_cfg->peak_irq, p_instance_ctrl->p_cfg->peak_ipl, p_instance_ctrl);
    }
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to TAPA instance control block
 * @param[in]     event      Event code
 **********************************************************************************************************************/
static void r_tapa_call_callback (tapa_instance_ctrl_t * p_ctrl, timer_event_t event)
{
    timer_callback_args_t args;

    args.event     = event;
    args.p_context = p_ctrl->p_context;

    p_ctrl->p_callback(&args);
}

/***********************************************************************************************************************
 * Interrupt Valley service routines
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void tapa_int_valley_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    /* Recover ISR context saved in open. */
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    timer_event_t event = TIMER_EVENT_VALLEY_INTERRUPT;

    /* Invoke the callback function if it is set. */
    if (NULL != p_instance_ctrl->p_callback)
    {
        r_tapa_call_callback(p_instance_ctrl, event);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}

/***********************************************************************************************************************
 * Interrupt Peak service routines
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void tapa_int_peak_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    /* Recover ISR context saved in open. */
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);
    tapa_instance_ctrl_t * p_instance_ctrl = (tapa_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    timer_event_t event = TIMER_EVENT_PEAK_INTERRUPT;

    /* Invoke the callback function if it is set. */
    if (NULL != p_instance_ctrl->p_callback)
    {
        r_tapa_call_callback(p_instance_ctrl, event);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}
