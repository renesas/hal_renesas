/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_taud_three_phase.h"        /* PRQA S 0380 # DP_MSR_D-1.1_C99 & DP_MSR_D-1.1_C11 */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "TD3P" in ASCII, used to determine if channel is open */
#define TAUD_THREE_PHASE_OPEN             (0x54443350U)
#define TAUD_THREE_PHASE_CLOSE            (0x0U)
#define TAUD_DUTY_CYCLE_PERCENT_BASE      (100U)
#define TAUD_THREE_PHASE_SLAVE_U_INDEX    (0U)
#define TAUD_THREE_PHASE_SLAVE_V_INDEX    (2U)
#define TAUD_THREE_PHASE_SLAVE_W_INDEX    (4U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* Implementation of TAUD_THREE_PHASE interface  */
const three_phase_api_t g_taud_three_phase_on_taud_three_phase = /* PRQA S 1502 # Advisory R-2.8 */
{
    .open         = R_TAUD_THREE_PHASE_Open,                     /* PRQA S 3635 ++ # Advisory R-17.12 */
    .start        = R_TAUD_THREE_PHASE_Start,
    .stop         = R_TAUD_THREE_PHASE_Stop,
    .reset        = R_TAUD_THREE_PHASE_Reset,
    .dutyCycleSet = R_TAUD_THREE_PHASE_DutyCycleSet,
    .callbackSet  = R_TAUD_THREE_PHASE_CallbackSet,
    .close        = R_TAUD_THREE_PHASE_Close, /* PRQA S 3635 -- */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1 */

/*******************************************************************************************************************//**
 * @addtogroup TAUD_THREE_PHASE
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes the 3-phase timer module (and associated timers) and applies configurations.
 * Implements @ref three_phase_api_t::open.
 *
 * @retval FSP_SUCCESS                    Initialization was successful.
 * @retval FSP_ERR_ASSERTION              A required input pointer is NULL.
 * @retval FSP_ERR_ALREADY_OPEN           Module is already open.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_THREE_PHASE_Open (three_phase_ctrl_t * const p_ctrl, three_phase_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    taud_three_phase_instance_ctrl_t * p_instance_ctrl = (taud_three_phase_instance_ctrl_t *) p_ctrl;        /* PRQA S 0316 # Advisory R-11.5 */

#if (TAUD_THREE_PHASE_CFG_PARAM_CHECKING_ENABLE == 1U)
    FSP_ASSERT((NULL != p_cfg));                                                                             /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_cfg->p_extend));                                                                   /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl));                                                                   /* PRQA S 3432 */
    FSP_ERROR_RETURN((TAUD_THREE_PHASE_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);                /* PRQA S 3432 */
#endif

    uint8_t   channel = 0U;
    uint8_t   master_oneshot_opened_count = 0U;
    fsp_err_t err = FSP_SUCCESS;

    /* Save pointer to extended configuration structure */
    taud_three_phase_extended_cfg_t * p_extend = (taud_three_phase_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    taud_instance_ctrl_t * p_taud_master_pwm_instance_ctrl =
        (taud_instance_ctrl_t *) p_extend->p_taud_master_pwm->p_ctrl;                                 /* PRQA S 0316 # Advisory R-11.5 */

    taud_instance_ctrl_t * p_taud_slave_pwm_instance_ctrl[TAUD_THREE_PHASE_SLAVE_CHANNEL_NUMBER];

    taud_instance_ctrl_t * p_taud_master_oneshot_instance_ctrl[TAUD_MASTER_CHANNEL_ONE_SHOT_NUMBER];

    taud_instance_ctrl_t * p_taud_slave_oneshot_instance_ctrl[TAUD_SLAVE_CHANNEL_ONE_SHOT_NUMBER];

    for (channel = 0U; channel < TAUD_THREE_PHASE_SLAVE_CHANNEL_NUMBER; channel++)     /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_taud_slave_pwm_instance_ctrl[channel] =
            (taud_instance_ctrl_t *) p_extend->p_taud_slave_pwm[channel]->p_ctrl;      /* PRQA S 0316 # Advisory R-11.5 */
    }

    for (channel = 0U; channel < TAUD_MASTER_CHANNEL_ONE_SHOT_NUMBER; channel++)       /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_taud_master_oneshot_instance_ctrl[channel] =
            (taud_instance_ctrl_t *) p_extend->p_taud_master_oneshot[channel]->p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    }

    for (channel = 0U; channel < TAUD_SLAVE_CHANNEL_ONE_SHOT_NUMBER; channel++)        /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_taud_slave_oneshot_instance_ctrl[channel] =
            (taud_instance_ctrl_t *) p_extend->p_taud_slave_oneshot[channel]->p_ctrl;  /* PRQA S 0316 # Advisory R-11.5 */
    }

    /* Initialize control structure */
    p_instance_ctrl->p_cfg = p_cfg;

    /* Initialize control structure for Master Triangle PWM with Deadtime */
    p_taud_master_pwm_instance_ctrl->p_cfg = p_extend->p_taud_master_pwm->p_cfg;

    /* Initialize control structure for Slave Triangle PWM with Deadtime */
    for (channel = 0U; channel < TAUD_THREE_PHASE_SLAVE_CHANNEL_NUMBER; channel++) /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_taud_slave_pwm_instance_ctrl[channel]->p_cfg = p_extend->p_taud_slave_pwm[channel]->p_cfg;
    }

    /* Initialize control structure for Master One-shot Pulse */
    for (channel = 0U; channel < TAUD_MASTER_CHANNEL_ONE_SHOT_NUMBER; channel++) /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_taud_master_oneshot_instance_ctrl[channel]->p_cfg = p_extend->p_taud_master_oneshot[channel]->p_cfg;
    }

    /* Initialize control structure for Slave One-shot Pulse */
    for (channel = 0U; channel < TAUD_SLAVE_CHANNEL_ONE_SHOT_NUMBER; channel++) /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_taud_slave_oneshot_instance_ctrl[channel]->p_cfg = p_extend->p_taud_slave_oneshot[channel]->p_cfg;
    }

    /* Set TAUD registers base address */
    if ((uint8_t) TAUD_UNIT_1 == p_extend->taud_unit)          /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_instance_ctrl->p_taud_reg = (R_TAUD_Type *) R_TAUD1; /* PRQA S 0306 # Advisory R-11.4 */
    }

#if (BSP_FEATURE_TAUD_THREE_PHASE_NUMBER_OF_UNIT >= 3U)
    else if ((uint8_t) TAUD_UNIT_2 == p_extend->taud_unit)     /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_instance_ctrl->p_taud_reg = (R_TAUD_Type *) R_TAUD2; /* PRQA S 0306 # Advisory R-11.4 */
    }
#endif

#if (BSP_FEATURE_TAUD_THREE_PHASE_NUMBER_OF_UNIT >= 4U)
    else if ((uint8_t) TAUD_UNIT_3 == p_extend->taud_unit)     /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_instance_ctrl->p_taud_reg = (R_TAUD_Type *) R_TAUD3; /* PRQA S 0316 # Advisory R-11.5 */
    }
#endif
    else
    {
        p_instance_ctrl->p_taud_reg = (R_TAUD_Type *) R_TAUD0; /* PRQA S 0306 # Advisory R-11.4 */
    }

    /* Open TAUD Master channel for Triangle PWM with Dead Time */
    err = p_extend->p_taud_master_pwm->p_api->open(p_extend->p_taud_master_pwm->p_ctrl,
                                                   p_extend->p_taud_master_pwm->p_cfg);

    if (FSP_SUCCESS != err)
    {
        (void) p_extend->p_taud_master_pwm->p_api->close(p_extend->p_taud_master_pwm->p_ctrl);
    }

    /* Update channels mask in instance control block. */
    p_instance_ctrl->channels_mask |= (uint16_t) (p_taud_master_pwm_instance_ctrl->channels_mask);

    /* Update output channel mask in instance control block. */
    p_instance_ctrl->output_mask |= (uint16_t) (p_taud_master_pwm_instance_ctrl->output_mask);

    /* Update masks of TAUD Slave channels for Triangle PWM with Dead Time */
    for (channel = 0U; channel < TAUD_THREE_PHASE_SLAVE_CHANNEL_NUMBER; channel++) /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        /* Update channels mask in instance control block. */
        p_instance_ctrl->channels_mask |= (uint16_t) (p_taud_slave_pwm_instance_ctrl[channel]->channels_mask);

        /* Update output channel mask in instance control block. */
        p_instance_ctrl->output_mask |= (uint16_t) (p_taud_slave_pwm_instance_ctrl[channel]->output_mask);
    }

    /* Open TAUD Master channels for One-shot Pulse */
    for (channel = 0U; channel < TAUD_MASTER_CHANNEL_ONE_SHOT_NUMBER; channel++) /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        err = p_extend->p_taud_master_oneshot[channel]->p_api->open(p_extend->p_taud_master_oneshot[channel]->p_ctrl,
                                                                    p_extend->p_taud_master_oneshot[channel]->p_cfg);

        if (FSP_SUCCESS != err)
        {
            while (master_oneshot_opened_count > 0U)
            {
                master_oneshot_opened_count--;
                (void) p_extend->p_taud_master_oneshot[master_oneshot_opened_count]->p_api->close(
                    p_extend->p_taud_master_oneshot[master_oneshot_opened_count]->p_ctrl);
            }

            /* Clear the channel and output masks. */
            p_instance_ctrl->channels_mask = 0U;
            p_instance_ctrl->output_mask   = 0U;

            return err;
        }

        /* Update channels mask in instance control block. */
        p_instance_ctrl->channels_mask |= (uint16_t) (p_taud_master_oneshot_instance_ctrl[channel]->channels_mask);

        /* Update output channel mask in instance control block. */
        p_instance_ctrl->output_mask |= (uint16_t) (p_taud_master_oneshot_instance_ctrl[channel]->output_mask);

        master_oneshot_opened_count++;
    }

    /* Update masks for TAUD Slave channels for One-shot Pulse */
    for (channel = 0U; channel < TAUD_SLAVE_CHANNEL_ONE_SHOT_NUMBER; channel++) /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        /* Update channels mask in instance control block. */
        p_instance_ctrl->channels_mask |= (uint16_t) (p_taud_slave_oneshot_instance_ctrl[channel]->channels_mask);

        /* Update output channel mask in instance control block. */
        p_instance_ctrl->output_mask |= (uint16_t) (p_taud_slave_oneshot_instance_ctrl[channel]->output_mask);
    }

    /* Open TAPA module */
    err = p_extend->p_tapa_taud->p_api->open(p_extend->p_tapa_taud->p_ctrl, p_extend->p_tapa_taud->p_cfg);

    if (FSP_SUCCESS != err)
    {
        (void) p_extend->p_tapa_taud->p_api->close(p_extend->p_tapa_taud->p_ctrl);
    }

    /* TAUD Three-phase module is initialized */
    p_instance_ctrl->open = TAUD_THREE_PHASE_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Starts all timers synchronously. Implements @ref three_phase_api_t::start.
 *
 * @retval FSP_SUCCESS                 Timers successfully started.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_THREE_PHASE_Start (three_phase_ctrl_t * const p_ctrl)                                /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    taud_three_phase_instance_ctrl_t * p_instance_ctrl = (taud_three_phase_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if (TAUD_THREE_PHASE_CFG_PARAM_CHECKING_ENABLE == 1U)
    FSP_ASSERT((NULL != p_instance_ctrl));                                                            /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg));                                                     /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg->p_extend));                                           /* PRQA S 3432 */
    FSP_ERROR_RETURN((TAUD_THREE_PHASE_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);             /* PRQA S 3432 */
#endif

    /* Enable channel output. */
    p_instance_ctrl->p_taud_reg->TAUDnTOE |= p_instance_ctrl->output_mask;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Set channel output to Low level. */
    p_instance_ctrl->p_taud_reg->TAUDnTO &= (uint16_t) ~(p_instance_ctrl->output_mask);               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Start current channel's counter operation. */
    p_instance_ctrl->p_taud_reg->TAUDnTS = p_instance_ctrl->channels_mask;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stops all timers synchronously. Implements @ref three_phase_api_t::stop.
 *
 * @retval FSP_SUCCESS                 Timers successfully stopped.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_THREE_PHASE_Stop (three_phase_ctrl_t * const p_ctrl)                                 /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    taud_three_phase_instance_ctrl_t * p_instance_ctrl = (taud_three_phase_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if (TAUD_THREE_PHASE_CFG_PARAM_CHECKING_ENABLE == 1U)
    FSP_ASSERT((NULL != p_instance_ctrl));                                                            /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg));                                                     /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg->p_extend));                                           /* PRQA S 3432 */
    FSP_ERROR_RETURN((TAUD_THREE_PHASE_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);             /* PRQA S 3432 */
#endif

    /* Stop current channel's counter operation. */
    p_instance_ctrl->p_taud_reg->TAUDnTT = p_instance_ctrl->channels_mask;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Disable channel output. */
    p_instance_ctrl->p_taud_reg->TAUDnTOE &= (uint16_t) ~(p_instance_ctrl->output_mask);              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Resets the counter values to 0. Implements @ref three_phase_api_t::reset.
 *
 * @note This function can not reset the counter when counter is stopped.
 *
 * @retval FSP_SUCCESS                 Counters were reset successfully.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_THREE_PHASE_Reset (three_phase_ctrl_t * const p_ctrl)                                /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    taud_three_phase_instance_ctrl_t * p_instance_ctrl = (taud_three_phase_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if (TAUD_THREE_PHASE_CFG_PARAM_CHECKING_ENABLE == 1U)
    FSP_ASSERT((NULL != p_instance_ctrl));                                                            /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg));                                                     /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg->p_extend));                                           /* PRQA S 3432 */
    FSP_ERROR_RETURN((TAUD_THREE_PHASE_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);             /* PRQA S 3432 */
#endif

    /* Get the channel status */
    bool channel_in_use = false;

    channel_in_use =
        (bool) (p_instance_ctrl->p_taud_reg->TAUDnTE & p_instance_ctrl->channels_mask); /* PRQA S 0662, 4340, 4394 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-10.5, DP_MSR_R-10.8_UC1 */

    if (true == channel_in_use)
    {
        /* Stop timer. */
        p_instance_ctrl->p_taud_reg->TAUDnTT = p_instance_ctrl->channels_mask; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Restart timer. */
        p_instance_ctrl->p_taud_reg->TAUDnTS = p_instance_ctrl->channels_mask; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Sets duty cycle for all three timers. Implements @ref three_phase_api_t::dutyCycleSet.
 *
 * @note It is recommended to call this function in a high-priority callback to ensure that it is not interrupted and
 * that no TAUD events occur during setting that would result in a duty cycle buffer load operation.
 *
 * @retval FSP_SUCCESS                 Duty cycle updated successfully.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 * @retval FSP_ERR_INVALID_ARGUMENT    One or more duty cycle count values was outside the range 0..(period - 1).
 **********************************************************************************************************************/
fsp_err_t R_TAUD_THREE_PHASE_DutyCycleSet (three_phase_ctrl_t * const       p_ctrl,       /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
                                           three_phase_duty_cycle_t * const p_duty_cycle) /* PRQA S 3673 # Advisory R-8.13 */
{
    uint8_t channel = 0U;

    three_phase_channel_t              channel_index;
    taud_three_phase_instance_ctrl_t * p_instance_ctrl = (taud_three_phase_instance_ctrl_t *) p_ctrl;                    /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if (TAUD_THREE_PHASE_CFG_PARAM_CHECKING_ENABLE == 1U)
    FSP_ASSERT((NULL != p_duty_cycle));                                                                                  /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl));                                                                               /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg));                                                                        /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg->p_extend));                                                              /* PRQA S 3432 */
    FSP_ERROR_RETURN((TAUD_THREE_PHASE_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);                                /* PRQA S 3432 */

    /* Check that duty cycle values are in-range */
    for (channel_index = THREE_PHASE_CHANNEL_U; channel_index < TAUD_DUTY_CHANNEL_NUMBER; channel_index++)               /* PRQA S 1880, 4527 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.1_UC1 */
    {
        FSP_ERROR_RETURN((p_duty_cycle->duty[channel_index] <= TAUD_DUTY_CYCLE_PERCENT_BASE), FSP_ERR_INVALID_ARGUMENT); /* PRQA S 1842, 3432 # DP_MSR_R-10.4_UC1, DP_MSR_R-20.7_UC1 */
    }
#endif

    /* Save pointer to extended configuration structure */
    taud_three_phase_extended_cfg_t * p_extend = (taud_three_phase_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1, Advisory R-11.5 */

    taud_instance_ctrl_t * p_taud_master_pwm_instance_ctrl =
        (taud_instance_ctrl_t *) p_extend->p_taud_master_pwm->p_ctrl;                                                  /* PRQA S 0316 # Advisory R-11.5 */

    taud_instance_ctrl_t * p_taud_slave_pwm_instance_ctrl[TAUD_THREE_PHASE_SLAVE_CHANNEL_NUMBER];

    for (channel = 0U; channel < TAUD_THREE_PHASE_SLAVE_CHANNEL_NUMBER; channel++)                                     /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_taud_slave_pwm_instance_ctrl[channel] =
            (taud_instance_ctrl_t *) p_extend->p_taud_slave_pwm[channel]->p_ctrl;                                      /* PRQA S 0316 # Advisory R-11.5 */
    }

    /* Convert duty cycle percentage to timer counts */
    uint32_t duty_cycle_counts[TAUD_DUTY_CHANNEL_NUMBER];
    uint32_t period_counts = p_extend->p_taud_master_pwm->p_cfg->period_counts;

    for (channel_index = THREE_PHASE_CHANNEL_U; channel_index < TAUD_DUTY_CHANNEL_NUMBER; channel_index++) /* PRQA S 1880, 4527 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.1_UC1 */
    {
        duty_cycle_counts[channel_index] = (period_counts * p_duty_cycle->duty[channel_index]) /
                                           TAUD_DUTY_CYCLE_PERCENT_BASE;

        /* Update new duty cycle value for channels u, v, w to extend configuration */
        p_extend->duty_cycle[channel_index] = p_duty_cycle->duty[channel_index];
    }

    /* Set Duty cycle for slave channel 4, 6, 8 corresponding to U, V, W phases */
    p_extend->p_taud_master_pwm->p_api->dutyCycleSet(p_taud_master_pwm_instance_ctrl,
                                                     duty_cycle_counts[THREE_PHASE_CHANNEL_U],
                                                     p_taud_slave_pwm_instance_ctrl[TAUD_THREE_PHASE_SLAVE_U_INDEX]->p_cfg->channel);
    p_extend->p_taud_master_pwm->p_api->dutyCycleSet(p_taud_master_pwm_instance_ctrl,
                                                     duty_cycle_counts[THREE_PHASE_CHANNEL_V],
                                                     p_taud_slave_pwm_instance_ctrl[TAUD_THREE_PHASE_SLAVE_V_INDEX]->p_cfg->channel);
    p_extend->p_taud_master_pwm->p_api->dutyCycleSet(p_taud_master_pwm_instance_ctrl,
                                                     duty_cycle_counts[THREE_PHASE_CHANNEL_W],
                                                     p_taud_slave_pwm_instance_ctrl[TAUD_THREE_PHASE_SLAVE_W_INDEX]->p_cfg->channel);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback for the TAUD U-channel with the option to provide memory for the callback argument
 * structure.
 * Implements @ref three_phase_api_t::callbackSet.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_THREE_PHASE_CallbackSet (three_phase_ctrl_t * const    p_ctrl,                                        /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
                                          void (                      * p_callback)(timer_callback_args_t *),          /* PRQA S 1336 */
                                          void * const                  p_context,
                                          timer_callback_args_t * const p_callback_memory)
{
    taud_three_phase_instance_ctrl_t * p_instance_ctrl = (taud_three_phase_instance_ctrl_t *) p_ctrl;                  /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if (TAUD_THREE_PHASE_CFG_PARAM_CHECKING_ENABLE == 1U)
    FSP_ASSERT((NULL != p_callback));                                                                                  /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl));                                                                             /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg));                                                                      /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg->p_extend));                                                            /* PRQA S 3432 */
    FSP_ERROR_RETURN((TAUD_THREE_PHASE_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);                              /* PRQA S 3432 */
#endif

    /* Save pointer to extended configuration structure */
    taud_three_phase_extended_cfg_t * p_extend = (taud_three_phase_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    /* Set Callback for TAUD master channel */
    p_extend->p_taud_master_pwm->p_api->callbackSet(p_extend->p_taud_master_pwm->p_ctrl,
                                                    p_callback,
                                                    p_context,
                                                    p_callback_memory);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stops counters, disables output pins, and clears internal driver data. Implements @ref three_phase_api_t::close.
 *
 * @retval FSP_SUCCESS                 Successful close.
 * @retval FSP_ERR_ASSERTION           p_ctrl was NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_THREE_PHASE_Close (three_phase_ctrl_t * const p_ctrl)                                                 /* PRQA S 1505 # Advisory R-8.7 */
{
    uint8_t channel = 0U;
    taud_three_phase_instance_ctrl_t * p_instance_ctrl = (taud_three_phase_instance_ctrl_t *) p_ctrl;                  /* PRQA S 0316 # Advisory R-11.5 */

#if (TAUD_THREE_PHASE_CFG_PARAM_CHECKING_ENABLE == 1U)
    FSP_ASSERT((NULL != p_instance_ctrl));                                                                             /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg));                                                                      /* PRQA S 3432 */
    FSP_ASSERT((NULL != p_instance_ctrl->p_cfg->p_extend));                                                            /* PRQA S 3432 */
    FSP_ERROR_RETURN((TAUD_THREE_PHASE_OPEN == p_instance_ctrl->open), FSP_ERR_NOT_OPEN);                              /* PRQA S 3432 */
#endif

    /* Save pointer to extended configuration structure */
    taud_three_phase_extended_cfg_t * p_extend = (taud_three_phase_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    /* Close TAUD Master and Slave channels for Triangle PWM with Dead Time */
    p_extend->p_taud_master_pwm->p_api->close(p_extend->p_taud_master_pwm->p_ctrl);

    /* Close TAUD Master and Slave channels for One-shot Pulse */
    for (channel = 0U; channel < TAUD_MASTER_CHANNEL_ONE_SHOT_NUMBER; channel++) /* PRQA S 1862 # DP_MSR_R-10.4_UC1 */
    {
        p_extend->p_taud_master_oneshot[channel]->p_api->close(p_extend->p_taud_master_oneshot[channel]->p_ctrl);
    }

    /* Close TAPA module */
    p_extend->p_tapa_taud->p_api->close(p_extend->p_tapa_taud->p_ctrl);

    /* Clear channels mask in instance control block. */
    p_instance_ctrl->channels_mask = 0U;

    /* Clear output channel mask in instance control block. */
    p_instance_ctrl->output_mask = 0U;

    /* Close TAUD Three-phase module */
    p_instance_ctrl->open = TAUD_THREE_PHASE_CLOSE;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup TAUD_THREE_PHASE)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
