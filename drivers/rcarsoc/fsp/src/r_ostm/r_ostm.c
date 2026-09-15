/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ostm.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** "OSTM" in ASCII, used to determine if channel is open. */
#define OSTM_OPEN              (0x4F53544DU)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void r_ostm_period_register_set(ostm_instance_ctrl_t * p_instance_ctrl, uint32_t period_counts);

static uint32_t r_ostm_clock_frequency_get(R_OSTMn_Type * p_ostm_regs);

#if OSTM_CFG_PARAM_CHECKING_ENABLE
static fsp_err_t r_ostm_open_param_checking(ostm_instance_ctrl_t * p_instance_ctrl, timer_cfg_t const * const p_cfg);

#endif

/* ISRs. */
BSP_INTERRUPT_ATTRIBUTE void ostm_int_isr(ostm_instance_ctrl_t * p_instance_ctrl);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/** OSTM Implementation of General Timer Driver  */
const timer_api_t g_timer_on_ostm =
{
    .open         = R_OSTM_Open,
    .start        = R_OSTM_Start,
    .stop         = R_OSTM_Stop,
    .reset        = R_OSTM_Reset,
    .enable       = R_OSTM_Enable,
    .disable      = R_OSTM_Disable,
    .periodSet    = R_OSTM_PeriodSet,
    .dutyCycleSet = R_OSTM_DutyCycleSet,
    .infoGet      = R_OSTM_InfoGet,
    .statusGet    = R_OSTM_StatusGet,
    .callbackSet  = R_OSTM_CallbackSet,
    .close        = R_OSTM_Close
};

/*******************************************************************************************************************//**
 * @addtogroup OSTM
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*********************************************************************************************************************
 * Initializes the OSTM module instance. Implements @ref timer_api_t::open.
 *
 * The OSTM hardware does not support one-shot functionality natively.  The one-shot feature is therefore implemented in
 * the OSTM HAL layer. For a timer configured as a one-shot timer, the timer is stopped upon the first timer expiration.
 *
 * The OSTM implementation of the general timer requires an ostm_extended_cfg_t extension parameter.  The extension
 * selects the counter mode (interval or free-running compare), whether an interrupt is generated when the counter
 * starts, and whether the counter is reloaded when it starts.  The timer is not started; call R_OSTM_Start.
 *
 * @retval FSP_SUCCESS                 Initialization was successful.
 * @retval FSP_ERR_ASSERTION           A required input pointer is NULL or the period is 0.
 * @retval FSP_ERR_ALREADY_OPEN        R_OSTM_Open has already been called for this p_ctrl.
 * @retval FSP_ERR_IRQ_BSP_DISABLED    A required interrupt has not been enabled in the vector table.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT  Requested channel number is not available on OSTM.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_Open (timer_ctrl_t * const p_ctrl, timer_cfg_t const * const p_cfg)
{
    ostm_instance_ctrl_t * p_instance_ctrl = (ostm_instance_ctrl_t *) p_ctrl;

    /* Set counter mode (OSTMnMD) */
    uint32_t              ostmnmd = 0;
    ostm_extended_cfg_t * p_ext;

#if OSTM_CFG_PARAM_CHECKING_ENABLE
    fsp_err_t err = r_ostm_open_param_checking(p_instance_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    p_ext = (ostm_extended_cfg_t *) p_cfg->p_extend;

    /*
     * Base address of the requested channel. X5H places OSTM0..OSTM5 at a
     * constant 4 KB stride from R_OSTM0_BASE (0x1C060000..0x1C065000).
     */
    p_instance_ctrl->p_reg = (R_OSTMn_Type *) (R_OSTM0_BASE + (R_OSTM_CHANNEL_STRIDE * p_cfg->channel));

    p_instance_ctrl->p_cfg = p_cfg;

    /* Forcibly stop timer. */
    p_instance_ctrl->p_reg->OSTMnTT = 1;

    /* Set period register */
    r_ostm_period_register_set(p_instance_ctrl, p_cfg->period_counts);

    if (OSTM_GIWS_TYPE_ENABLED == p_ext->generate_interrupt_when_starts)
    {
        ostmnmd |= R_OSTMn_OSTMnCTL_OSTMnMD0_Msk;
    }

    if (OSTM_TIMER_MODE_FREERUN == p_ext->ostm_mode)
    {
        ostmnmd |= R_OSTMn_OSTMnCTL_OSTMnMD1_Msk;
    }

    if (OSTM_COUNTER_START_RELOAD_DISABLE == p_ext->counter_load_at_start)
    {
        ostmnmd |= R_OSTMn_OSTMnCTL_OSTMnMD2_Msk;
    }

    /* Enable OSTM Interrupt */
    ostmnmd |= R_OSTMn_OSTMnCTL_OSTMnIE_Msk;

    p_instance_ctrl->p_reg->OSTMnCTL = (uint8_t) ostmnmd;

    /* Set callback and context pointers */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    p_instance_ctrl->open = OSTM_OPEN;

    /* All done.  */
    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Starts timer. Implements @ref timer_api_t::start.
 *
 * @retval FSP_SUCCESS                 Timer started.
 * @retval FSP_ERR_ASSERTION           p_ctrl is null.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_Start (timer_ctrl_t * const p_ctrl)
{
    ostm_instance_ctrl_t * p_instance_ctrl = (ostm_instance_ctrl_t *) p_ctrl;
#if OSTM_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(OSTM_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Start timer */
    p_instance_ctrl->p_reg->OSTMnTS = 1;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stops the timer.  Implements @ref timer_api_t::stop.
 *
 * @retval FSP_SUCCESS                 Timer stopped.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_Stop (timer_ctrl_t * const p_ctrl)
{
    ostm_instance_ctrl_t * p_instance_ctrl = (ostm_instance_ctrl_t *) p_ctrl;
#if OSTM_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(OSTM_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Stop timer */
    p_instance_ctrl->p_reg->OSTMnTT = 1;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates period. Implements @ref timer_api_t::periodSet.
 *
 * If the timer is running it is stopped, the new period is written to OSTMnCMP, the counter is reloaded (with the
 * period in interval mode, with 0 in free-running compare mode), and the timer is restarted.
 *
 * @retval FSP_SUCCESS                 Period value updated.
 * @retval FSP_ERR_ASSERTION           A required pointer was NULL, or the period was 0.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_PeriodSet (timer_ctrl_t * const p_ctrl, uint32_t const period_counts)
{
    ostm_instance_ctrl_t * p_instance_ctrl = (ostm_instance_ctrl_t *) p_ctrl;
#if OSTM_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ASSERT(0U != period_counts);
    FSP_ERROR_RETURN(OSTM_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Set period. */
    r_ostm_period_register_set(p_instance_ctrl, period_counts);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Gets timer information and store it in provided pointer p_info. Implements @ref timer_api_t::infoGet.
 *
 * @retval FSP_SUCCESS                 Period, count direction, and frequency stored in p_info.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_InfoGet (timer_ctrl_t * const p_ctrl, timer_info_t * const p_info)
{
    ostm_instance_ctrl_t * p_instance_ctrl = (ostm_instance_ctrl_t *) p_ctrl;
#if OSTM_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_info);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(OSTM_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Get and store period */
    p_info->period_counts = p_instance_ctrl->period;

    /* Get and store clock frequency */
    p_info->clock_frequency = r_ostm_clock_frequency_get(p_instance_ctrl->p_reg);

    /* OSTM counting direction */
    p_info->count_direction =
        (p_instance_ctrl->p_reg->OSTMnCTL & R_OSTMn_OSTMnCTL_OSTMnMD1_Msk) ?
        TIMER_DIRECTION_UP : TIMER_DIRECTION_DOWN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Retrieves the current state and counter value stores them in p_status. Implements @ref timer_api_t::statusGet.
 *
 * @retval FSP_SUCCESS                 Current status and counter value provided in p_status.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_StatusGet (timer_ctrl_t * const p_ctrl, timer_status_t * const p_status)
{
    ostm_instance_ctrl_t * p_instance_ctrl = (ostm_instance_ctrl_t *) p_ctrl;

#if OSTM_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_status);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(OSTM_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Read the state. */
    p_status->state = (p_instance_ctrl->p_reg->OSTMnTE_b.OSTMnTE) ?
                      TIMER_STATE_COUNTING : TIMER_STATE_STOPPED;

    /* Read counter value */
    p_status->counter = p_instance_ctrl->p_reg->OSTMnCNT;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 * Implements @ref timer_api_t::callbackSet.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_CallbackSet (timer_ctrl_t * const          p_api_ctrl,
                              void (                      * p_callback)(timer_callback_args_t *),
                              void const * const            p_context,
                              timer_callback_args_t * const p_callback_memory)
{
    ostm_instance_ctrl_t * p_ctrl = (ostm_instance_ctrl_t *) p_api_ctrl;

#if OSTM_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_callback);
    FSP_ERROR_RETURN(OSTM_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */

    p_ctrl->p_callback = p_callback;

    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stops counter, disables interrupts, and clears internal driver data.  Implements @ref timer_api_t::close.
 *
 * @retval FSP_SUCCESS                 Timer closed.
 * @retval FSP_ERR_ASSERTION           p_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_Close (timer_ctrl_t * const p_ctrl)
{
    ostm_instance_ctrl_t * p_instance_ctrl = (ostm_instance_ctrl_t *) p_ctrl;
#if OSTM_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(OSTM_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Stop timer and disable interrupt. */
    p_instance_ctrl->p_reg->OSTMnTT            = 1U;
    p_instance_ctrl->p_reg->OSTMnCTL_b.OSTMnIE = 0U;

    p_instance_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @ref timer_api_t::dutyCycleSet is not supported by OSTM.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implementation.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_DutyCycleSet (timer_ctrl_t * const p_ctrl, uint32_t const duty_cycle_counts, uint32_t const pin)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(duty_cycle_counts);
    FSP_PARAMETER_NOT_USED(pin);

    /* Return the unsupported error. */
    FSP_RETURN(FSP_ERR_UNSUPPORTED);
}

/*******************************************************************************************************************//**
 * Reset OSTM Counter Register. Implements @ref timer_api_t::reset.
 *
 * @retval FSP_SUCCESS                 Timer reset.
 * @retval FSP_ERR_ASSERTION           p_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance control structure is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_Reset (timer_ctrl_t * const p_ctrl)
{
    ostm_instance_ctrl_t * p_instance_ctrl = (ostm_instance_ctrl_t *) p_ctrl;
#if OSTM_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(OSTM_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Reset timer counter with current period */
    r_ostm_period_register_set(p_instance_ctrl, p_instance_ctrl->period);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @ref timer_api_t::enable is not supported by OSTM.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implementation.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_Enable (timer_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    /* Return the unsupported error. */
    FSP_RETURN(FSP_ERR_UNSUPPORTED);
}

/*******************************************************************************************************************//**
 * @ref timer_api_t::disable is not supported by OSTM.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implementation.
 **********************************************************************************************************************/
fsp_err_t R_OSTM_Disable (timer_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    /* Return the unsupported error. */
    FSP_RETURN(FSP_ERR_UNSUPPORTED);
}

/** @} (end addtogroup OSTM) */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

#if OSTM_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Parameter checking for R_OSTM_Open.
 *
 * @param[in]  p_instance_ctrl    Pointer to instance control structure.
 * @param[in]  p_cfg              Configuration structure for this instance
 *
 * @retval FSP_SUCCESS                 All parameters are valid.
 * @retval FSP_ERR_ASSERTION           A required input pointer is NULL or the period is 0.
 * @retval FSP_ERR_ALREADY_OPEN        R_OSTM_Open has already been called for this p_ctrl.
 * @retval FSP_ERR_IRQ_BSP_DISABLED    A required interrupt has not been enabled in the vector table.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT  Requested channel number is not available on OSTM.
 **********************************************************************************************************************/
static fsp_err_t r_ostm_open_param_checking (ostm_instance_ctrl_t * p_instance_ctrl, timer_cfg_t const * const p_cfg)
{
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(OSTM_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);

    /* Enable IRQ if user supplied a callback function,
     *  or if the timer is a one-shot timer (so the driver is able to
     *  turn off the timer after one period. */
    if ((NULL != p_cfg->p_callback) || (TIMER_MODE_ONE_SHOT == p_cfg->mode))
    {
        /* Return error if IRQ is required and not in the vector table. */
        FSP_ERROR_RETURN(p_cfg->cycle_end_irq >= 0, FSP_ERR_IRQ_BSP_DISABLED);
    }

    FSP_ASSERT(0U != p_cfg->period_counts);

    /* Validate channel number. */
    FSP_ERROR_RETURN(((1U << p_cfg->channel) & BSP_FEATURE_OSTM_VALID_CHANNEL_MASK), FSP_ERR_IP_CHANNEL_NOT_PRESENT);

    return FSP_SUCCESS;
}

#endif                                 /* OSTM_CFG_PARAM_CHECKING_ENABLE */

/*******************************************************************************************************************//**
 * Sets period register and updates compare match registers in one-shot and periodic mode.
 *
 * @param[in]  p_instance_ctrl    Control block for this instance
 * @param[in]  period_counts      OSTM period in counts
 **********************************************************************************************************************/
static void r_ostm_period_register_set (ostm_instance_ctrl_t * p_instance_ctrl, uint32_t period_counts)
{
    ostm_extended_cfg_t * p_ext         = (ostm_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    uint32_t              timer_running = 0U;
    if (1U == p_instance_ctrl->p_reg->OSTMnTE)
    {
        timer_running = 1U;
        p_instance_ctrl->p_reg->OSTMnTT = 1U;
        FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->OSTMnTE, 0U);
    }

    /* Store the period value so it can be retrieved later. */
    p_instance_ctrl->period = period_counts;

    /* Set counter to period. */
    p_instance_ctrl->p_reg->OSTMnCMP = period_counts;

    if (OSTM_TIMER_MODE_FREERUN == p_ext->ostm_mode)
    {
        p_instance_ctrl->p_reg->OSTMnCNT = 0U;
    }
    else
    {
        p_instance_ctrl->p_reg->OSTMnCNT = period_counts;
    }

    if (timer_running)
    {
        p_instance_ctrl->p_reg->OSTMnTS = 1U;
    }
}

/*******************************************************************************************************************//**
 * Obtains the clock frequency of OSTM for all clock sources except OSTM0 underflow, with divisor applied.
 *
 * @param[in]  p_ostm_regs         Registers of OSTM channel used
 *
 * @return Source clock frequency of OSTM in Hz, divider applied.
 **********************************************************************************************************************/
static uint32_t r_ostm_clock_frequency_get (R_OSTMn_Type * p_ostm_regs)
{
    uint32_t clock_freq_hz = 0U;

    FSP_PARAMETER_NOT_USED(p_ostm_regs);

    /*
     * Every OSTM unit runs off the same fixed clock on this SoC, so the channel
     * registers carry nothing to read back -- see OSTM_CFG_CLOCK in r_ostm_cfg.h.
     */
    clock_freq_hz = OSTM_CFG_CLOCK;

    return clock_freq_hz;
}

/***********************************************************************************************************************
 * Interrupt service routines
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void ostm_int_isr (ostm_instance_ctrl_t * p_instance_ctrl)
{
    /* If the channel is configured to be one-shot mode, stop the timer. */
    if (TIMER_MODE_ONE_SHOT == p_instance_ctrl->p_cfg->mode)
    {
        /* Stop timer */
        p_instance_ctrl->p_reg->OSTMnTT = 1;
    }

    /* Invoke the callback function if it is set. */
    if (NULL != p_instance_ctrl->p_callback)
    {
        /* Setup parameters for the user-supplied callback function. */
        timer_callback_args_t callback_args;

        /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
         * stored in non-secure memory so they can be accessed by a non-secure callback function. */
        timer_callback_args_t * p_args = p_instance_ctrl->p_callback_memory;
        if (NULL == p_args)
        {
            /* Store on stack */
            p_args = &callback_args;
        }
        else
        {
            /* Save current arguments on the stack in case this is a nested interrupt. */
            callback_args = *p_args;
        }

        p_args->event     = TIMER_EVENT_CYCLE_END;
        p_args->p_context = p_instance_ctrl->p_context;

        /* If the project is not Trustzone Secure, no need to change security state in order to call the callback. */
        p_instance_ctrl->p_callback(p_args);

        if (NULL != p_instance_ctrl->p_callback_memory)
        {
            /* Restore callback memory in case this is a nested interrupt. */
            *p_instance_ctrl->p_callback_memory = callback_args;
        }
    }
}
