/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_taud.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** "TAUD" in ASCII, used to determine if channel is open. */
#define TAUD_OPEN                                     (0x54415544ULL)
#define TAUD_OVERFLOW_INTERRUPT_FUNCTION_CDR_VALUE    (65535U) /*!< 0xFFFF */
#define TAUD_CMOR_COS_Pos                             (6UL)    /*!< COS (Bit 6)  */
#define TAUD_CMOR_STS_Pos                             (8UL)    /*!< STS (Bit 8)  */
#define TAUD_CMOR_MAS_Pos                             (11UL)   /*!< MAS (Bit 11) */
#define TAUD_CMOR_CCS_Pos                             (12UL)   /*!< CCS (Bit 12) */
#define TAUD_CMOR_CKS_Pos                             (14UL)   /*!< CKS (Bit 14) */
#define TAUD_OVERFLOW_VALUE                           (0x10000)

/* Memory address of DNFA control and enable registers for TAUD units . */
#if defined(BSP_MCU_GROUP_RH850U2Ax)
 #define DNFACTL_TAUD0                                *((volatile uint8_t *) 0xFFED0A00)
 #define DNFACTL_TAUD1                                *((volatile uint8_t *) 0xFFED0B00)
 #define DNFACTL_TAUD2                                *((volatile uint8_t *) 0xFFBF6900)
 #define DNFAEN_TAUD0                                 *((volatile uint16_t *) 0xFFED0A04)
 #define DNFAEN_TAUD1                                 *((volatile uint16_t *) 0xFFED0B04)
 #define DNFAEN_TAUD2                                 *((volatile uint16_t *) 0xFFBF6904)
#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void     taud_common_open(taud_instance_ctrl_t * const p_instance_ctrl, timer_cfg_t const * const p_cfg);
static void     taud_hardware_initialize(taud_instance_ctrl_t * const p_instance_ctrl, timer_cfg_t const * const p_cfg);
static uint32_t taud_clock_frequency_get(taud_instance_ctrl_t * const p_instance_ctrl);
static void     taud_update_cdr_master(taud_instance_ctrl_t * const p_instance_ctrl);
static void     taud_update_cdr_independent(taud_instance_ctrl_t * const p_instance_ctrl);
static void     taud_update_cdr_slave(taud_instance_ctrl_t * const p_instance_ctrl);
static void     taud_update_cdr_slave_even(taud_instance_ctrl_t * const p_instance_ctrl);
static void     taud_update_cdr_slave_odd(taud_instance_ctrl_t * const p_instance_ctrl);

#if defined(BSP_MCU_GROUP_RH850U2Ax)
static void taud_init_dnf(taud_instance_ctrl_t * const p_instance_ctrl);

#endif

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void taud_int_isr(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* TAUD implementation of timer interface. */
const timer_api_t g_timer_on_taud =
{
    .open         = R_TAUD_Open,
    .start        = R_TAUD_Start,
    .stop         = R_TAUD_Stop,
    .enable       = R_TAUD_Enable,
    .disable      = R_TAUD_Disable,
    .reset        = R_TAUD_Reset,
    .dutyCycleSet = R_TAUD_DutyCycleSet,
    .periodSet    = R_TAUD_PeriodSet,
    .infoGet      = R_TAUD_InfoGet,
    .statusGet    = R_TAUD_StatusGet,
    .close        = R_TAUD_Close,
    .callbackSet  = R_TAUD_CallbackSet
};

/*******************************************************************************************************************//**
 * @addtogroup TAUD
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes the timer module and applies configurations.
 *
 * The TAUD implementation of the general timer can accept a taud_extended_cfg_t extension parameter.
 *
 * Implements @ref timer_api_t::open.
 *
 * @retval FSP_SUCCESS                   Initialization was successful and timer has started.
 * @retval FSP_ERR_ASSERTION             A required input pointer is NULL.
 * @retval FSP_ERR_ALREADY_OPEN          Module is already open.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Open (timer_ctrl_t * const p_ctrl, timer_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;

    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
#endif

    /* Initializes control structure. */
    taud_common_open(p_instance_ctrl, p_cfg);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

#if defined(BSP_MCU_GROUP_RH850U2Ax)

    /* Initialize the digital noise filters for U2A device. */
    taud_init_dnf(p_instance_ctrl);
#endif

    /* Performs hardware initialization of the TAUD. */
    taud_hardware_initialize(p_instance_ctrl, p_cfg);

    /* Enable interrupts. */
    if (p_cfg->cycle_end_irq >= 0)
    {
        R_BSP_IrqCfgEnable(p_cfg->cycle_end_irq, p_cfg->cycle_end_ipl, p_instance_ctrl);
    }

#if ((BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ) && (BSP_FEATURE_TAUD_MAX_UNIT > (3U)))
    taud_unit_source_t unit_used = p_extend->taud_unit;

    /* Interrupt select only available for TAUD Unit 3 */
    if (TAUD_UNIT3 == unit_used)
    {
        if (BSP_INTSEL_DISABLED != p_extend->intsel_irq_cfg)
        {
            R_BSP_IntSel_CfgEnable(p_extend->intsel_irq_cfg);
        }
    }
#endif

    p_instance_ctrl->open = TAUD_OPEN;

    if (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)
    {
#if (0 == TAUD_CFG_MULTI_SLAVE_ENABLE)
        const uint16_t index = 0;
#else
        for (uint16_t index = 0; index < TAUD_MAX_NUM_SLAVE_CHANNELS; index++)
#endif
        {
            if (NULL != p_extend->p_slave_channel_instances[index])
            {
                /* Initialize control structure */
                taud_instance_ctrl_t * slave_channel_instance_ctrl[TAUD_MAX_NUM_SLAVE_CHANNELS];
                slave_channel_instance_ctrl[index] =
                    (taud_instance_ctrl_t *) p_extend->p_slave_channel_instances[index]->p_ctrl;

                slave_channel_instance_ctrl[index]->p_cfg = p_extend->p_slave_channel_instances[index]->p_cfg;

                R_TAUD_Open(slave_channel_instance_ctrl[index], slave_channel_instance_ctrl[index]->p_cfg);
            }
        }
    }
    else
    {
        /* Do nothing. */
    }

    return err;
}

/*******************************************************************************************************************//**
 * Stops timer.
 *
 * Implements @ref timer_api_t::stop.
 *
 * @retval FSP_SUCCESS                   Timer successfully stopped.
 * @retval FSP_ERR_ASSERTION             p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Stop (timer_ctrl_t * const p_ctrl)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    if (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)
    {
        /* Stop current channel's counter operation. */
        p_instance_ctrl->p_reg->TAUDnTT = p_instance_ctrl->channels_mask;

        /* Disable channel output. */
        p_instance_ctrl->p_reg->TAUDnTOE &= (uint16_t) ~(p_instance_ctrl->output_mask);
    }
    else if ((TAUD_CHANNEL_TYPE_INDEPENDENT == p_extend->channel_type) ||
             (TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT == p_extend->taud_function))
    {
        /* Stop current channel's counter operation. */
        p_instance_ctrl->p_reg->TAUDnTT = (1U << p_instance_ctrl->p_cfg->channel);

        if (TAUD_OUTPUT_ENABLE == p_extend->output_enable)
        {
            /* Disable channel output. */
            p_instance_ctrl->p_reg->TAUDnTOE &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        }
    }
    else
    {

        /* Stop with slave channel is unsupported. */
        return FSP_ERR_UNSUPPORTED;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Starts timer.
 *
 * Implements @ref timer_api_t::start.
 *
 * @retval FSP_SUCCESS                 Timer successfully started.
 * @retval FSP_ERR_ASSERTION           p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 * @retval FSP_ERR_UNSUPPORTED         The setting is not supported.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Start (timer_ctrl_t * const p_ctrl)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    FSP_ERROR_RETURN(((TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_1 != p_extend->taud_function) &&
                      (TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_2 != p_extend->taud_function)) ||
                     (((TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_1 == p_extend->taud_function) ||
                       (TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_2 == p_extend->taud_function)) &&
                      (TAUD_REAL_TIME_OUTPUT_TRIGGER_GENERATED == p_extend->real_time_output_trigger)),
                     FSP_ERR_UNSUPPORTED);
#else
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    if (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)
    {
        /* Enable channel output. */
        p_instance_ctrl->p_reg->TAUDnTOE |= p_instance_ctrl->output_mask;

        /* Set channel output to Low level. */
        p_instance_ctrl->p_reg->TAUDnTO &= (uint16_t) ~(p_instance_ctrl->output_mask);

        /* Start current channel's counter operation. */
        p_instance_ctrl->p_reg->TAUDnTS = p_instance_ctrl->channels_mask;
    }
    else if ((TAUD_CHANNEL_TYPE_INDEPENDENT == p_extend->channel_type) ||
             (TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT == p_extend->taud_function))
    {
        if (TAUD_OUTPUT_ENABLE == p_extend->output_enable)
        {
            /* Enable channel output. */
            p_instance_ctrl->p_reg->TAUDnTOE |= (1U << p_instance_ctrl->p_cfg->channel);

            /* Set channel output to Low level. */
            p_instance_ctrl->p_reg->TAUDnTO &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        }

        /* Start current channel's counter operation. */
        p_instance_ctrl->p_reg->TAUDnTS = (1U << (p_instance_ctrl->p_cfg->channel));
    }
    else
    {

        /* Start with slave channel is unsupported. */
        return FSP_ERR_UNSUPPORTED;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Resets the counter value to the current period, duty cycle.
 *
 * @note This function can not reset the counter when counter is stopped.
 *
 * Implements @ref timer_api_t::reset.
 *
 * @retval FSP_SUCCESS                 Counter value reset successfully.
 * @retval FSP_ERR_ASSERTION           p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 * @retval FSP_ERR_UNSUPPORTED         The setting is not supported.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Reset (timer_ctrl_t * const p_ctrl)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* This function cannot be forcibly restarted. */
    FSP_ERROR_RETURN(((TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_MEASUREMENT != p_extend->taud_function) &&
                      (TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_1 != p_extend->taud_function) &&
                      (TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_2 != p_extend->taud_function) &&
                      (TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_JUDGMENT != p_extend->taud_function) &&
                      (TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_1 != p_extend->taud_function) &&
                      (TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_2 != p_extend->taud_function)) ||
                     (((TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_1 == p_extend->taud_function) ||
                       (TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_2 == p_extend->taud_function)) &&
                      (TAUD_REAL_TIME_OUTPUT_TRIGGER_GENERATED == p_extend->real_time_output_trigger)),
                     FSP_ERR_UNSUPPORTED);
#else
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    /* Get the channel status */
    bool channel_in_use = false;

    if (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)
    {
        channel_in_use =
            (bool) (p_instance_ctrl->p_reg->TAUDnTE & p_instance_ctrl->channels_mask);

        if (true == channel_in_use)
        {
            /* Stop timer. */
            p_instance_ctrl->p_reg->TAUDnTT = p_instance_ctrl->channels_mask;

            /* Restart timer. */
            p_instance_ctrl->p_reg->TAUDnTS = p_instance_ctrl->channels_mask;
        }
    }
    else if ((TAUD_CHANNEL_TYPE_INDEPENDENT == p_extend->channel_type) ||
             (TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT == p_extend->taud_function))
    {
        channel_in_use =
            (bool) (p_instance_ctrl->p_reg->TAUDnTE & (1U << p_instance_ctrl->p_cfg->channel) >>
                    p_instance_ctrl->p_cfg->channel);

        if (true == channel_in_use)
        {
            /* Stop timer. */
            p_instance_ctrl->p_reg->TAUDnTT = (1U << p_instance_ctrl->p_cfg->channel);

            /* Restart timer. */
            p_instance_ctrl->p_reg->TAUDnTS = (1U << p_instance_ctrl->p_cfg->channel);
        }
    }
    else
    {

        /* Start with slave channel is unsupported. */
        return FSP_ERR_UNSUPPORTED;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @ref timer_api_t::R_TAUD_Enable is not supported on the RH850 MCUs.
 *
 * @retval FSP_ERR_UNSUPPORTED           Function not supported in this implementation.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Enable (timer_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    /* Return the unsupported error. */
    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @ref timer_api_t::R_TAUD_Disable is not supported on the RH850 MCUs.
 *
 * @retval FSP_ERR_UNSUPPORTED           Function not supported in this implementation.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Disable (timer_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    /* Return the unsupported error. */
    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 *
 * Implements @ref timer_api_t::callbackSet.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_CallbackSet (timer_ctrl_t * const          p_api_ctrl,
                              void (                      * p_callback)(timer_callback_args_t *),
                              void * const                  p_context,
                              timer_callback_args_t * const p_callback_memory)
{
    taud_instance_ctrl_t * p_ctrl = (taud_instance_ctrl_t *) p_api_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(TAUD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(p_callback);
#endif

    /* Store callback and context */
    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Sets period value provided. If the timer is running, the period will be updated after the next counter overflow.
 * If the timer is stopped, this function resets the counter and updates the period.
 *
 * R_TAUD_PeriodSet support features: PWM Output (Master channel), Interval Timer, Input Interval Timer,
 *                                    Delay Pulse Output (Master channel), External Event Count, Delay Count,
 *                                    Interrupt Request Signals Culling, Trigger Start PWM Output (Master channel),
 *                                    One-Phase PWM Output (Lower channel - Slave Odd),
 *                                    Real-Time Output Type 1 (Channel that generates real-time output trigger),
 *                                    Input Pulse Interval Judgment,
 *                                    Complementary Modulation Output (Slave channel 1).
 *
 * For External Event Count feature: This function is used to set the number of event count.
 *
 * For Non-Complementary Modulation Output Type 2 feature: Applied only for slave 1
 *
 * For Input Signal Width Judgment feature: This function set value of TAUDnCDRm register.
 *
 * For Complementary Modulation Output feature: This function is used to set the CDR value for Slave channel 1.
 *
 * Implements @ref timer_api_t::periodSet.
 *
 * @retval FSP_SUCCESS                   Period value written successfully.
 * @retval FSP_ERR_ASSERTION             p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_INVALID_ARGUMENT      The period counts is out of range.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_UNSUPPORTED           The setting is not supported.
 * @retval FSP_ERR_INVALID_HW_CONDITION  The simultaneous rewrite status (RSF flag) had not yet completed.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_PeriodSet (timer_ctrl_t * const p_ctrl, uint32_t const period)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

    /* Current status of timer. */
    timer_status_t current_status =
    {
        .counter       = 0U,
        .state         = TIMER_STATE_STOPPED,
        .is_rsf_active = false
    };

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    FSP_ERROR_RETURN(((TAUD_FUNCTION_PWM_OUTPUT == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_PWM_OUTPUT)) ||
                     ((TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1 == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1)) ||
                     ((TAUD_FUNCTION_INTERVAL_TIMER == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_INTERVAL_TIMER)) ||
                     ((TAUD_FUNCTION_INPUT_INTERVAL_TIMER == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_INPUT_INTERVAL_TIMER)) ||
                     ((TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)) ||
                     ((TAUD_FUNCTION_EXTERNAL_EVENT_COUNT == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_EXTERNAL_EVENT_COUNT)) ||
                     ((TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT == p_extend->taud_function) &&
                      (period <= TAUD_DEADTIME_COUNT_MAX_FUNCTION_ONE_PHASE_PWM_OUTPUT)) ||
                     ((TAUD_FUNCTION_INTERRUPT_REQUEST_SIGNAL_CULLING == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_INTERRUPT_REQUEST_SIGNAL_CULLING)) ||
                     ((TAUD_FUNCTION_DELAY_COUNT == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_DELAY_COUNT)) ||
                     ((TAUD_FUNCTION_TRIGGER_START_PWM_OUTPUT == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_TRIGGER_START_PWM_OUTPUT)) ||
                     ((TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_1 == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_REAL_TIME_OUTPUT_TYPE_1)) ||
                     ((TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_1 == p_extend->taud_function) &&
                      (period <= TAUD_CDR_MAX)) ||
                     ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_TYPE_2)) ||
                     ((TAUD_FUNCTION_INPUT_PULSE_INTERVAL_JUDGMENT == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_INPUT_PULSE_INTERVAL)) ||
                     ((TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_JUDGMENT == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_INPUT_SIGNAL_WIDTH)) ||
                     ((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) &&
                      (period <= TAUD_PERIOD_COUNT_MAX_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT)),
                     FSP_ERR_INVALID_ARGUMENT);
#else
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    /* TAUDnCDRm value */
    uint32_t taud_cdr = (0U);

    if ((TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type) &&
        ((TAUD_FUNCTION_PWM_OUTPUT == p_extend->taud_function) ||
         (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1 == p_extend->taud_function) ||
         (TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function) ||
         (TAUD_FUNCTION_TRIGGER_START_PWM_OUTPUT == p_extend->taud_function)))
    {
        /* Set the period value for the Master channel. */
        taud_cdr = period - 1U;
    }
    else if (((TAUD_FUNCTION_INTERVAL_TIMER == p_extend->taud_function) ||
              (TAUD_FUNCTION_INPUT_INTERVAL_TIMER == p_extend->taud_function)) && (period >= 2U))
    {
        taud_cdr = (period >> 1U) - 1U;
    }
    else if ((TAUD_FUNCTION_EXTERNAL_EVENT_COUNT == p_extend->taud_function) ||
             (TAUD_FUNCTION_INTERRUPT_REQUEST_SIGNAL_CULLING == p_extend->taud_function) ||
             (TAUD_FUNCTION_DELAY_COUNT == p_extend->taud_function) ||
             (TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_JUDGMENT == p_extend->taud_function) ||
             ((TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT == p_extend->taud_function) &&
              (TAUD_CHANNEL_TYPE_SLAVE_ODD == p_extend->channel_type)) ||
             ((TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_1 == p_extend->taud_function) &&
              (TAUD_REAL_TIME_OUTPUT_TRIGGER_GENERATED == p_extend->real_time_output_trigger)) ||
             (TAUD_FUNCTION_INPUT_PULSE_INTERVAL_JUDGMENT == p_extend->taud_function))
    {
        taud_cdr = period - 1U;
    }
    else if (TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_1 == p_extend->taud_function)
    {
        taud_cdr = period - 1U;
    }
    else if (((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) ||
              (TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 == p_extend->taud_function)) &&
             (1U == p_extend->slave_ordinal_number))
    {
        taud_cdr = period;
    }
    else
    {
        return FSP_ERR_UNSUPPORTED;
    }

    if (TAUD_SIMULTANEOUS_REWRITE_ENABLE == p_extend->simultaneous_rewrite)
    {
        /* Update current status of simultaneous rewrite. */
        R_TAUD_StatusGet(p_instance_ctrl, &current_status);

        /* Check if simultaneous rewrite enabling flag is not set. */
        if (false == current_status.is_rsf_active)
        {
            /* Write to Data Register. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) taud_cdr;

            /* Trigger simultaneous rewrite enabling state. */
            p_instance_ctrl->p_reg->TAUDnRDT = (uint16_t) (1U << p_instance_ctrl->p_cfg->channel);
        }
        else
        {
            return FSP_ERR_INVALID_HW_CONDITION;
        }
    }
    else
    {
        /* Write to Data Register. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) taud_cdr;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Sets duty cycle for the timer.
 *
 * R_TAUD_DutyCycleSet support features: PWM Output (Slave channel), Triangle PWM Output (Slave channel),
 *                                       Triangle PWM Output With Dead Time (Slave even channel),
 *                                       Delay Pulse Output (Slave channel 1 and Slave channel 3),
 *                                       Trigger Start PWM Output (Slave channel),
 *                                       Non-Complementary modulatiton output function type 2(Slave 2 to slave 7),
 *                                       Clock Divide, Complementary Modulation Output (Slave channel 2, 4, 6).
 *
 * @note pin (Param in): Specifies which Slave channel will be updated the duty cycle.
 *
 * Implements @ref timer_api_t::dutyCycleSet.
 *
 * @retval FSP_SUCCESS                   Duty cycle updated successfully.
 * @retval FSP_ERR_ASSERTION             p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_INVALID_ARGUMENT      Duty cycle count values was outside the range 0...65536.
 * @retval FSP_ERR_UNSUPPORTED           The setting is not supported.
 * @retval FSP_ERR_INVALID_HW_CONDITION  The simultaneous rewrite status (RSF flag) had not yet completed.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_DutyCycleSet (timer_ctrl_t * const p_ctrl, uint32_t const duty_cycle_counts, uint32_t const pin)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

    /* Current status of timer. */
    timer_status_t current_status =
    {
        .counter       = 0U,
        .state         = TIMER_STATE_STOPPED,
        .is_rsf_active = false
    };

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    FSP_ERROR_RETURN((((TAUD_FUNCTION_PWM_OUTPUT == p_extend->taud_function) ||
                       (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1 == p_extend->taud_function) ||
                       (TAUD_FUNCTION_TRIGGER_START_PWM_OUTPUT == p_extend->taud_function)) &&
                      (TAUD_CHANNEL_TYPE_SLAVE == p_extend->channel_type)) ||
                     (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2 == p_extend->taud_function) ||
                     ((TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_SLAVE == p_extend->channel_type) &&
                      ((1U == p_extend->slave_ordinal_number) || (3U == p_extend->slave_ordinal_number))) ||
                     (TAUD_FUNCTION_CLOCK_DIVIDE == p_extend->taud_function) ||
                     (((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_1 ==
                        p_extend->taud_function)) &&
                      (TAUD_CHANNEL_TYPE_SLAVE == p_extend->channel_type) &&
                      ((2U <= p_extend->slave_ordinal_number) && (7U >= p_extend->slave_ordinal_number))) ||
                     ((TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)) ||
                     ((TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT_DEADTIME == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)) ||
                     ((TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2 == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)) ||
                     ((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)) ||
                     ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)),
                     FSP_ERR_UNSUPPORTED);

    FSP_ERROR_RETURN(duty_cycle_counts <= TAUD_CDR_MAX, FSP_ERR_INVALID_ARGUMENT);

    /* Do not select the pin of master channel. */
    FSP_ERROR_RETURN(pin != p_instance_ctrl->p_cfg->channel, FSP_ERR_UNSUPPORTED);

 #if (0 == TAUD_CFG_MULTI_SLAVE_ENABLE)
    const uint16_t index = 0;
 #else
    for (uint16_t index = 0; index < TAUD_MAX_NUM_SLAVE_CHANNELS; index++)
 #endif
    {
        if (NULL != p_extend->p_slave_channel_instances[index])
        {
            uint16_t slave_channel = p_extend->p_slave_channel_instances[index]->p_cfg->channel;
            if (slave_channel == pin)
            {
                taud_extended_cfg_t * slave_channel_extend =
                    (taud_extended_cfg_t *) p_extend->p_slave_channel_instances[index]->p_cfg->p_extend;

                FSP_ERROR_RETURN(p_extend->taud_function == slave_channel_extend->taud_function, FSP_ERR_UNSUPPORTED);

                FSP_ERROR_RETURN((((TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT == slave_channel_extend->taud_function)) &&
                                  (TAUD_CHANNEL_TYPE_SLAVE == slave_channel_extend->channel_type)) ||
                                 ((TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT_DEADTIME == slave_channel_extend->taud_function) &&
                                  (TAUD_CHANNEL_TYPE_SLAVE_EVEN == slave_channel_extend->channel_type)) ||
                                 ((TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2 == slave_channel_extend->taud_function) &&
                                  (TAUD_CHANNEL_TYPE_SLAVE == slave_channel_extend->channel_type)) ||
                                 (((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 ==
                                    slave_channel_extend->taud_function)) &&
                                  (TAUD_CHANNEL_TYPE_SLAVE == slave_channel_extend->channel_type) &&
                                  ((2U <= slave_channel_extend->slave_ordinal_number) &&
                                   (7U >= slave_channel_extend->slave_ordinal_number))) ||
                                 ((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT ==
                                   slave_channel_extend->taud_function) &&
                                  ((2U == slave_channel_extend->slave_ordinal_number) ||
                                   (4U == slave_channel_extend->slave_ordinal_number) ||
                                   (6U == slave_channel_extend->slave_ordinal_number))),
                                 FSP_ERR_UNSUPPORTED);
            }
        }
    }

#else
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    if ((TAUD_FUNCTION_PWM_OUTPUT == p_extend->taud_function) ||
        ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_1 == p_extend->taud_function) &&
         (TAUD_CHANNEL_TYPE_SLAVE == p_extend->channel_type) && (2U <= p_extend->slave_ordinal_number)) ||
        (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1 == p_extend->taud_function))
    {
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) duty_cycle_counts;
    }
    else if (TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function)
    {
        /* Set pulse width counts for reference signal or delay signal. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) duty_cycle_counts;
    }
    else if (TAUD_FUNCTION_TRIGGER_START_PWM_OUTPUT == p_extend->taud_function)
    {
        /* Set the duty cycle value for the Slave channel of Trigger Start PWM Output function. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) duty_cycle_counts;
    }
    else if (TAUD_FUNCTION_CLOCK_DIVIDE == p_extend->taud_function)
    {
        /* TAUDmCDR value */
        uint32_t cdr_value = (0U);

        if ((TAUD_INPUT_EDGE_RISING == p_extend->edge_type) || (TAUD_INPUT_EDGE_FALLING == p_extend->edge_type))
        {
            cdr_value = (duty_cycle_counts >> 1U) - 1U;
        }
        else if (TAUD_INPUT_EDGE_BOTH_MEASURE_LOW == p_extend->edge_type)
        {
            cdr_value = duty_cycle_counts - 1U;
        }

        /* Write TAUDmCDR value to TAUDnCDR register */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) cdr_value;
    }
    else
    {
        /* Do nothing. */
    }

#if (0 == TAUD_CFG_MULTI_SLAVE_ENABLE)
    const uint16_t index = 0;
#else
    for (uint16_t index = 0; index < TAUD_MAX_NUM_SLAVE_CHANNELS; index++)
#endif
    {
        if (NULL != p_extend->p_slave_channel_instances[index])
        {
            uint16_t slave_channel = p_extend->p_slave_channel_instances[index]->p_cfg->channel;

            if (slave_channel == pin)
            {
                taud_instance_ctrl_t * slave_channel_instance_ctrl[TAUD_MAX_NUM_SLAVE_CHANNELS];
                slave_channel_instance_ctrl[index] =
                    (taud_instance_ctrl_t *) p_extend->p_slave_channel_instances[index]->p_ctrl;
                slave_channel_instance_ctrl[index]->p_cfg = p_extend->p_slave_channel_instances[index]->p_cfg;
                taud_extended_cfg_t * slave_channel_extend =
                    (taud_extended_cfg_t *) p_extend->p_slave_channel_instances[index]->p_cfg->p_extend;

                uint16_t master_cdr = p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR;

                if ((TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT_DEADTIME == p_extend->taud_function) ||
                    ((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) &&
                     ((2U == slave_channel_extend->slave_ordinal_number) ||
                      (4U == slave_channel_extend->slave_ordinal_number) ||
                      (6U == slave_channel_extend->slave_ordinal_number))))
                {
                    uint16_t slave_deadtime_channel = pin + 1;
                    uint16_t slave_deadtime_cdr     = p_instance_ctrl->p_reg->TAUDnCDR[slave_deadtime_channel].TAUDnCDR;
                    uint32_t period_counts          = (master_cdr + 1) << 1;
                    uint16_t deadtime_value         = slave_deadtime_cdr + 1;

                    /* TAUDmCDR value. */
                    uint32_t cdr_value = (0U);

                    /* Update TAUDmCDR value. */
                    if (0U == duty_cycle_counts)
                    {
                        cdr_value = period_counts >> 1U;
                    }
                    else if (duty_cycle_counts >= period_counts)
                    {
                        cdr_value = (0U);
                    }
                    else
                    {
                        cdr_value = (period_counts - duty_cycle_counts - deadtime_value) >> 1U;
                    }

                    /* Write TAUDmCDR value to TAUDnCDR register.TAUDn */
                    p_instance_ctrl->p_reg->TAUDnCDR[slave_channel_instance_ctrl[index]->p_cfg->channel].TAUDnCDR =
                        (uint16_t) cdr_value;
                }
                else if ((TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT == p_extend->taud_function) ||
                         (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2 == p_extend->taud_function))
                {
                    uint32_t period_counts = (master_cdr + 1) << 1;

                    /* TAUDmCDR value */
                    uint32_t cdr_value = (0U);

                    /* Set CDRm value based on duty cycle counts */
                    if ((0U) == duty_cycle_counts)
                    {
                        cdr_value = period_counts >> 1U; // Duty cycle = 0%
                    }
                    else if (duty_cycle_counts >= period_counts)
                    {
                        cdr_value = 0U;                  // Duty cycle = 100%
                    }
                    else
                    {
                        cdr_value = (period_counts - duty_cycle_counts) >> 1U;
                    }

                    /* Write TAUDmCDR value to TAUDnCDR register */
                    p_instance_ctrl->p_reg->TAUDnCDR[slave_channel_instance_ctrl[index]->p_cfg->channel].TAUDnCDR =
                        (uint16_t) cdr_value;
                }
                else if ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 ==
                          p_extend->taud_function) &&
                         (((2U) == slave_channel_extend->slave_ordinal_number) ||
                          ((3U) == slave_channel_extend->slave_ordinal_number) ||
                          ((4U) == slave_channel_extend->slave_ordinal_number) ||
                          ((5U) == slave_channel_extend->slave_ordinal_number) ||
                          ((6U) == slave_channel_extend->slave_ordinal_number) ||
                          ((7U) == slave_channel_extend->slave_ordinal_number)))
                {
                    uint32_t period_counts = (master_cdr + 1) << 1;

                    /* TAUDmCDR value */
                    uint32_t cdr_value = (0U);

                    /* Set CDRm value based on duty cycle counts */
                    if ((0U) == duty_cycle_counts)
                    {
                        cdr_value = period_counts >> 1U; // Duty cycle = 0%
                    }
                    else if (duty_cycle_counts >= period_counts)
                    {
                        cdr_value = 0U;                  // Duty cycle = 100%
                    }
                    else
                    {
                        cdr_value = (period_counts - duty_cycle_counts) >> 1U;
                    }

                    /* Write TAUDmCDR value to TAUDnCDR register */
                    p_instance_ctrl->p_reg->TAUDnCDR[slave_channel_instance_ctrl[index]->p_cfg->channel].TAUDnCDR =
                        (uint16_t) cdr_value;
                }
            }
        }
    }

    /* Triggers a rewrite simultaneously. */
    if ((TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT_DEADTIME == p_extend->taud_function) ||
        (TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) ||
        (TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT == p_extend->taud_function) ||
        (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2 == p_extend->taud_function) ||
        (TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 == p_extend->taud_function))
    {
#if (0 == TAUD_CFG_MULTI_SLAVE_ENABLE)
        const uint16_t index = 0;
#else
        for (uint16_t index = 0; index < TAUD_MAX_NUM_SLAVE_CHANNELS; index++)
#endif
        {
            if (NULL != p_extend->p_slave_channel_instances[index])
            {
                uint16_t slave_channel = p_extend->p_slave_channel_instances[index]->p_cfg->channel;

                if (slave_channel == pin)
                {
                    /* Initialize control structure */
                    taud_instance_ctrl_t * slave_channel_instance_ctrl[TAUD_MAX_NUM_SLAVE_CHANNELS];
                    slave_channel_instance_ctrl[index] =
                        (taud_instance_ctrl_t *) p_extend->p_slave_channel_instances[index]->p_ctrl;

                    slave_channel_instance_ctrl[index]->p_cfg = p_extend->p_slave_channel_instances[index]->p_cfg;

                    if (slave_channel_instance_ctrl[index]->p_reg->TAUDnTE &
                        (1U << slave_channel_instance_ctrl[index]->p_cfg->channel))
                    {
                        /* Update current status of simultaneous rewrite. */
                        R_TAUD_StatusGet(slave_channel_instance_ctrl[index], &current_status);

                        /* Check if simultaneous rewrite enabling flag is not set. */
                        if (false == current_status.is_rsf_active)
                        {
                            /* Trigger simultaneous rewrite enabling state. */
                            slave_channel_instance_ctrl[index]->p_reg->TAUDnRDT =
                                (uint16_t) (1U << slave_channel_instance_ctrl[index]->p_cfg->channel);
                        }
                        else
                        {
                            return FSP_ERR_INVALID_HW_CONDITION;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (p_instance_ctrl->p_reg->TAUDnTE & (1U << p_instance_ctrl->p_cfg->channel))
        {
            /* Update current status of simultaneous rewrite. */
            R_TAUD_StatusGet(p_instance_ctrl, &current_status);

            /* Check if simultaneous rewrite enabling flag is not set. */
            if (false == current_status.is_rsf_active)
            {
                /* Trigger simultaneous rewrite enabling state. */
                p_instance_ctrl->p_reg->TAUDnRDT = (uint16_t) (1U << p_instance_ctrl->p_cfg->channel);
            }
            else
            {
                return FSP_ERR_INVALID_HW_CONDITION;
            }
        }
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get timer information and store it in provided pointer p_info.
 *
 * R_TAUD_InfoGet support features: Interval Timer, Input Interval Timer, Clock Divide,
 *                                  PWM Output (Master channel), Triangle PWM Output (Master channel),
 *                                  External Event Count, Delay Count, Delay Pulse Output,
 *                                  Interrupt Request Signals Culling, Trigger Start PWM Output (Master channel),
 *                                  Offset Trigger Output, Simultaneous Rewrite Trigger Generation Type 1,
 *                                  Non-Complementary modulatiton output function type 2 (Master channel and
 *                                  Slave channel 1), Input Pulse Interval Judgment,
 *                                  One-Phase PWM Output (Lower channel - Slave Odd),
 *                                  Complementary Modulation Output (Master channel and Slave channel 1).
 *
 * Implements @ref timer_api_t::infoGet.
 *
 * @retval FSP_SUCCESS                   Period, count direction, frequency written to caller's structure successfully.
 * @retval FSP_ERR_ASSERTION             p_ctrl or p_info or p_extend is NULL.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_UNSUPPORTED           The setting is not supported.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_InfoGet (timer_ctrl_t * const p_ctrl, timer_info_t * const p_info)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_info);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    uint32_t          period_counts   = (0U);
    timer_direction_t count_direction = TIMER_DIRECTION_DOWN;

    /* Get Channel Data Register value. */
    uint16_t taud_cdr = p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR;

    if (((TAUD_FUNCTION_PWM_OUTPUT == p_extend->taud_function) ||
         (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1 == p_extend->taud_function) ||
         (TAUD_FUNCTION_TRIGGER_START_PWM_OUTPUT == p_extend->taud_function)) &&
        (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type))
    {
        /* Take the period info from Master channel. */
        period_counts   = taud_cdr + 1U;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if ((TAUD_FUNCTION_INTERVAL_TIMER == p_extend->taud_function) ||
             (TAUD_FUNCTION_INPUT_INTERVAL_TIMER == p_extend->taud_function))
    {
        period_counts   = (taud_cdr + 1U) << 1U;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if (TAUD_FUNCTION_CLOCK_DIVIDE == p_extend->taud_function)
    {
        if ((TAUD_INPUT_EDGE_RISING == p_extend->edge_type) || (TAUD_INPUT_EDGE_FALLING == p_extend->edge_type))
        {
            period_counts = (taud_cdr + 1U) << 1U;
        }
        else if (TAUD_INPUT_EDGE_BOTH_MEASURE_LOW == p_extend->edge_type)
        {
            period_counts = taud_cdr + 1U;
        }

        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if ((TAUD_FUNCTION_EXTERNAL_EVENT_COUNT == p_extend->taud_function) ||
             (TAUD_FUNCTION_INTERRUPT_REQUEST_SIGNAL_CULLING == p_extend->taud_function) ||
             ((TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT == p_extend->taud_function) &&
              (TAUD_CHANNEL_TYPE_SLAVE_ODD == p_extend->channel_type)) ||
             (TAUD_FUNCTION_DELAY_COUNT == p_extend->taud_function) ||
             ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 == p_extend->taud_function) &&
              ((1U) == p_extend->slave_ordinal_number)) ||
             (TAUD_FUNCTION_INPUT_PULSE_INTERVAL_JUDGMENT == p_extend->taud_function) ||
             (TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_JUDGMENT == p_extend->taud_function))
    {
        period_counts   = taud_cdr + 1U;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if ((TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function) &&
             ((TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type) ||
              ((TAUD_CHANNEL_TYPE_SLAVE == p_extend->channel_type) && (2U == p_extend->slave_ordinal_number))))
    {
        period_counts   = taud_cdr + 1U;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if ((TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function) &&
             (TAUD_CHANNEL_TYPE_SLAVE == p_extend->channel_type) && (2U != p_extend->slave_ordinal_number))
    {
        period_counts   = taud_cdr;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if (((TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT == p_extend->taud_function) ||
              (TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) ||
              (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2 == p_extend->taud_function) ||
              (TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 == p_extend->taud_function)) &&
             (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type))
    {
        period_counts   = (taud_cdr + 1U) << 1U;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if ((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) &&
             ((1U) == p_extend->slave_ordinal_number))
    {
        period_counts   = taud_cdr;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_1 == p_extend->taud_function) &&
             (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type))
    {
        period_counts   = taud_cdr + 1U;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else if ((TAUD_FUNCTION_OFFSET_TRIGGER_OUTPUT == p_extend->taud_function) &&
             (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type))
    {
        period_counts   = taud_cdr + 1U;
        count_direction = TIMER_DIRECTION_UP;
    }
    else if (TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_1 == p_extend->taud_function)
    {
        period_counts   = taud_cdr + 1U;
        count_direction = TIMER_DIRECTION_DOWN;
    }
    else
    {
        return FSP_ERR_UNSUPPORTED;
    }

    /* Store period counts. */
    p_info->period_counts = period_counts;

    /* Store count direction. */
    p_info->count_direction = count_direction;

    /* Store clock frequency. */
    p_info->clock_frequency = taud_clock_frequency_get(p_instance_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get current timer status and store it in provided pointer p_status.
 *
 * Implements @ref timer_api_t::statusGet.
 *
 * @retval FSP_SUCCESS                   Current timer state and counter value set successfully.
 * @retval FSP_ERR_ASSERTION             p_ctrl or p_status is NULL.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_StatusGet (timer_ctrl_t * const p_ctrl, timer_status_t * const p_status)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_status);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Get counter state. */
    p_status->state =
        ((p_instance_ctrl->p_reg->TAUDnTE & (1U << p_instance_ctrl->p_cfg->channel)) >
         0U ? TIMER_STATE_COUNTING : TIMER_STATE_STOPPED);

    /* Get counter value. */
    p_status->counter = (uint32_t) p_instance_ctrl->p_reg->TAUDnCNT[p_instance_ctrl->p_cfg->channel].TAUDnCNT;

    /* Get simultaneous rewrite state of channel (RSF value). */
    p_status->is_rsf_active = (bool) ((p_instance_ctrl->p_reg->TAUDnRSF & (1U << p_instance_ctrl->p_cfg->channel)) >>
                                      p_instance_ctrl->p_cfg->channel);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stops counter, disables output pins, and clears internal driver data.
 *
 * Implements @ref timer_api_t::close.
 *
 * @retval FSP_SUCCESS                 Successful close.
 * @retval FSP_ERR_ASSERTION           p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_NOT_OPEN            The instance is not opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Close (timer_ctrl_t * const p_ctrl)
{
    fsp_err_t              err             = FSP_SUCCESS;
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Disable channel counter operation. */
    p_instance_ctrl->p_reg->TAUDnTT = (1U << p_instance_ctrl->p_cfg->channel);

    /* Clear TAUDn Channel Mode OS Register.TAUDn */
    p_instance_ctrl->p_reg->TAUDnCMOR[p_instance_ctrl->p_cfg->channel].TAUDnCMOR = 0U;

    /* Clear TAUDn Channel Mode User Register.TAUDn */
    p_instance_ctrl->p_reg->TAUDnCMUR[p_instance_ctrl->p_cfg->channel].TAUDnCMUR = 0U;

    /* Clear TAUDn Channel Data Register.TAUDn */
    p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = 0U;

    /* Clear Channel Output Mode setting. */
    if (TAUD_OUTPUT_ENABLE == p_extend->output_enable)
    {
        p_instance_ctrl->p_reg->TAUDnTOE &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTO  &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTOM &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTOC &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTOL &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTDE &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTDM &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTDL &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTRE &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTRO &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnTME &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
    }

    /* Clear TAUDn Channel Real-time Output Control Register */
    p_instance_ctrl->p_reg->TAUDnTRC &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);

    /* Clear Simultaneous Rewrite registers. */
    if (TAUD_SIMULTANEOUS_REWRITE_ENABLE == p_extend->simultaneous_rewrite)
    {
        p_instance_ctrl->p_reg->TAUDnRDE &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnRDS &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnRDM &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnRDC &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
    }

    /* Disable interrupts. */
    if (p_instance_ctrl->p_cfg->cycle_end_irq >= 0)
    {
#if ((BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ) && (BSP_FEATURE_TAUD_MAX_UNIT > (3U)))
        taud_extended_cfg_t * p_extend  = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
        taud_unit_source_t    unit_used = p_extend->taud_unit;

        /* Interrupt select only available for TAUD Unit 3 */
        if (TAUD_UNIT3 == unit_used)
        {
            if (BSP_INTSEL_DISABLED != p_extend->intsel_irq_cfg)
            {
                R_BSP_IntSel_CfgDisable(p_extend->intsel_irq_cfg);
            }
        }
#endif
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->cycle_end_irq);
    }

    /* Clear open flag. */
    p_instance_ctrl->open = 0U;

    if (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)
    {
#if (0 == TAUD_CFG_MULTI_SLAVE_ENABLE)
        const uint16_t index = 0;
#else
        for (uint16_t index = 0; index < TAUD_MAX_NUM_SLAVE_CHANNELS; index++)
#endif
        {
            if (NULL != p_extend->p_slave_channel_instances[index])
            {
                taud_instance_ctrl_t * slave_channel_instance_ctrl[TAUD_MAX_NUM_SLAVE_CHANNELS];
                slave_channel_instance_ctrl[index] =
                    (taud_instance_ctrl_t *) p_extend->p_slave_channel_instances[index]->p_ctrl;
                R_TAUD_Close(slave_channel_instance_ctrl[index]);
            }
        }
    }
    else
    {
        /* Do nothing. */
    }

    return err;
}

/***********************************************************************************************************************
 * Set the pulse delay for the output pin signal.
 *
 * R_TAUD_PulseOutputDelay support features: One-Shot Pulse Output, Delay Pulse Output (Slave channel 2).
 *
 * @retval FSP_SUCCESS                   Set the pulse delay successfully.
 * @retval FSP_ERR_ASSERTION             p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_INVALID_ARGUMENT      The pulse delay counts is out of range.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_UNSUPPORTED           The functions is not supported.
 * @retval FSP_ERR_INVALID_HW_CONDITION  The simultaneous rewrite status (RSF flag) had not yet completed.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_PulseOutputDelay (timer_ctrl_t * const p_ctrl, uint32_t const delay_counts)
{
    fsp_err_t              err             = FSP_SUCCESS;
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

    /* Current status of timer. */
    timer_status_t current_status =
    {
        .counter       = 0U,
        .state         = TIMER_STATE_STOPPED,
        .is_rsf_active = false
    };

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    FSP_ERROR_RETURN((TAUD_FUNCTION_ONE_SHOT_PULSE_OUTPUT == p_extend->taud_function) ||
                     ((TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_SLAVE == p_extend->channel_type) &&
                      (2U == p_extend->slave_ordinal_number)),
                     FSP_ERR_UNSUPPORTED);
    FSP_ERROR_RETURN(((delay_counts > TAUD_CDR_MIN) && (delay_counts < TAUD_CDR_MAX)) ||
                     ((TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function) &&
                      (TAUD_CDR_MIN < delay_counts) && (delay_counts < p_instance_ctrl->p_cfg->period_counts)),
                     FSP_ERR_INVALID_ARGUMENT);
#else
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    /* Use case used for One-Shot Pulse Output Function, Delay Pulse Output Function. */
    if (TAUD_SIMULTANEOUS_REWRITE_ENABLE == p_extend->simultaneous_rewrite)
    {
        /* Update current status of simultaneous rewrite. */
        R_TAUD_StatusGet(p_instance_ctrl, &current_status);

        /* Check if simultaneous rewrite enabling flag is not set. */
        if (false == current_status.is_rsf_active)
        {
            /* Set pulse delay counts into Channel Data register. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) (delay_counts - 1U);

            /* Trigger simultaneous rewrite enabling state. */
            p_instance_ctrl->p_reg->TAUDnRDT = (uint16_t) (1U << p_instance_ctrl->p_cfg->channel);

            /* Update pulse delay counts for instance. */
            p_extend->delay_counts = delay_counts;
        }
        else
        {
            err = FSP_ERR_INVALID_HW_CONDITION;
        }
    }
    /* Use case used for One-Pulse Output Function. */
    else
    {
        /* Set pulse delay counts into Channel Data register. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) (delay_counts - 1U);

        /* Update pulse delay counts for instance. */
        p_extend->delay_counts = delay_counts;
    }

    return err;
}

/***********************************************************************************************************************
 * Set the pulse width for the output signal.
 *
 * R_TAUD_PulseWidthSet support features: One-Shot Pulse Output, One-Pulse Output.
 *
 * @retval FSP_SUCCESS                   Set the pulse width successfully.
 * @retval FSP_ERR_ASSERTION             p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_INVALID_ARGUMENT      The width count is out of range.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_UNSUPPORTED           The functions is not supported.
 * @retval FSP_ERR_INVALID_HW_CONDITION  The simultaneous rewrite status (RSF flag) had not yet completed.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_PulseWidthSet (timer_ctrl_t * const p_ctrl, uint32_t const width_counts)
{
    fsp_err_t              err             = FSP_SUCCESS;
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

    /* Current status of timer. */
    timer_status_t current_status =
    {
        .counter       = 0U,
        .state         = TIMER_STATE_STOPPED,
        .is_rsf_active = false
    };

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    FSP_ERROR_RETURN((TAUD_FUNCTION_ONE_SHOT_PULSE_OUTPUT == p_extend->taud_function) ||
                     (TAUD_FUNCTION_ONE_PULSE_OUTPUT == p_extend->taud_function) ||
                     ((TAUD_FUNCTION_OFFSET_TRIGGER_OUTPUT == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_SLAVE == p_extend->channel_type)),
                     FSP_ERR_UNSUPPORTED);
    FSP_ERROR_RETURN(width_counts < TAUD_CDR_MAX, FSP_ERR_INVALID_ARGUMENT);
#else
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    /* Use case used for One-Shot Pulse Output Function. */
    if (TAUD_SIMULTANEOUS_REWRITE_ENABLE == p_extend->simultaneous_rewrite)
    {
        /* Update current status of simultaneous rewrite. */
        R_TAUD_StatusGet(p_instance_ctrl, &current_status);

        /* Check if simultaneous rewrite enabling flag is not set. */
        if (false == current_status.is_rsf_active)
        {
            /* Set pulse width counts into Channel Data register. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) width_counts;

            /* Trigger simultaneous rewrite enabling state. */
            p_instance_ctrl->p_reg->TAUDnRDT = (uint16_t) (1U << p_instance_ctrl->p_cfg->channel);

            /* Update pulse width counts for instance. */
            p_extend->width_counts = width_counts;
        }
        else
        {
            err = FSP_ERR_INVALID_HW_CONDITION;
        }
    }
    /* Use case used for One-Pulse Output Function. */
    else
    {
        /* Set pulse width counts into Channel Data register. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) width_counts;

        /* Update pulse width counts for instance. */
        p_extend->width_counts = width_counts;
    }

    return err;
}

/*******************************************************************************************************************//**
 * This function is used to clear counter overflow occurrence flag.
 *
 * @retval FSP_SUCCESS                   Counter overflow occurrence flag is updated successfully.
 * @retval FSP_ERR_UNSUPPORTED           Function not supported in this implementation.
 * @retval FSP_ERR_ASSERTION             p_ctrl or p_extend is NULL.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Clear (timer_ctrl_t * const p_ctrl)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    FSP_ERROR_RETURN((TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_MEASUREMENT == p_extend->taud_function) ||
                     (TAUD_FUNCTION_INPUT_PULSE_INTERVAL_MEASUREMENT == p_extend->taud_function) ||
                     ((TAUD_FUNCTION_OFFSET_TRIGGER_OUTPUT == p_extend->taud_function) &&
                      (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)),
                     FSP_ERR_UNSUPPORTED);
#else
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    uint8_t overflow_occurs = (0U);

    /* Get overflow status. */
    overflow_occurs = p_instance_ctrl->p_reg->TAUDnCSR[p_instance_ctrl->p_cfg->channel].TAUDnCSR_b.TAUDnOVF;

    /* Update when overflow occurs. */
    if (1U == overflow_occurs)
    {
        /* Clear the overflow flag. */
        if ((TAUD_OVERFLOW_MANUAL_CLEAR == p_extend->overflow_timing) ||
            (TAUD_OVERFLOW_COUNT_STOP_MANUAL_CLEAR == p_extend->overflow_timing))
        {
            p_instance_ctrl->p_reg->TAUDnCSC[p_instance_ctrl->p_cfg->channel].TAUDnCSC_b.TAUDnCLOV = (1U);
        }
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set input edge of TAUDnTTINm input signal
 *
 * R_TAUD_InputEdgeSet support features: Input Pulse Interval Measurement,
 *                                       Real-Time Output Function Type 2 (Upper Channel).
 *
 * @retval FSP_SUCCESS                   Input edge is set successfully.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_ASSERTION             p_instance_ctrl or p_extend is NULL.
 * @retval FSP_ERR_UNSUPPORTED           The function is not supported for feature.
 * @retval FSP_ERR_INVALID_ARGUMENT      The input_edge is out of range.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_InputEdgeSet (timer_ctrl_t * const p_ctrl, taud_input_edge_t const input_edge)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    FSP_ERROR_RETURN((TAUD_FUNCTION_INPUT_POSITION_DETECTION == p_extend->taud_function) ||
                     (TAUD_FUNCTION_INPUT_INTERVAL_TIMER == p_extend->taud_function) ||
                     (TAUD_FUNCTION_INPUT_PULSE_INTERVAL_MEASUREMENT == p_extend->taud_function) ||
                     ((TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_2 == p_extend->taud_function) &&
                      (TAUD_REAL_TIME_OUTPUT_TRIGGER_GENERATED == p_extend->real_time_output_trigger)),
                     FSP_ERR_UNSUPPORTED);

    FSP_ERROR_RETURN(TAUD_INPUT_EDGE_BOTH_MEASURE_HIGH != input_edge, FSP_ERR_INVALID_ARGUMENT);
#else
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    if (input_edge != p_extend->edge_type)
    {
        /* Set type of valid edge detection for input signal. */
        p_instance_ctrl->p_reg->TAUDnCMUR[p_instance_ctrl->p_cfg->channel].TAUDnCMUR_b.TAUDnTIS = input_edge;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the real-time output level to TAUDnTTOUTm (TRO register setting).
 *
 * R_TAUD_OutputLevelSet support features: Real-Time Output Type 1,
 *                                         Complementary Modulation Output (Slave channel 2 to Slave channel 7).
 *
 * @retval FSP_SUCCESS                   Real-time output level is set successfully.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_ASSERTION             p_instance_ctrl or p_extend is NULL.
 * @retval FSP_ERR_UNSUPPORTED           The function is not supported for feature.
 * @retval FSP_ERR_INVALID_ARGUMENT      The real-time output level is not valid.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_OutputLevelSet (timer_ctrl_t * const                p_ctrl,
                                 taud_real_time_output_level_t const real_time_output_level)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* This TME register setting is valid when TRE register is set to 1. */
    FSP_ERROR_RETURN(TAUD_REAL_TIME_OUTPUT_ENABLE == p_extend->real_time_output, FSP_ERR_UNSUPPORTED);
    FSP_ERROR_RETURN(TAUD_REAL_TIME_OUTPUT_LEVEL_HIGH >= real_time_output_level, FSP_ERR_INVALID_ARGUMENT);
#endif

    /* Sets a value which is output to TAUDnTTOUTm.TAUDn */
    p_instance_ctrl->p_reg->TAUDnTRO =
        (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTRO & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                    (real_time_output_level << p_instance_ctrl->p_cfg->channel));

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enables/disables modulation output for timer output and real-time output of channel m (TME register setting).
 *
 * R_TAUD_ModulationSet support features: Complementary Modulation Output (Slave channel 2 to Slave channel 7).
 *
 * @retval FSP_SUCCESS                   Modulation output is set successfully.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_ASSERTION             p_instance_ctrl or p_extend is NULL.
 * @retval FSP_ERR_UNSUPPORTED           The function is not supported for feature.
 * @retval FSP_ERR_INVALID_ARGUMENT      The modulation output setting is not valid.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_ModulationSet (timer_ctrl_t * const p_ctrl, taud_modulation_t const modulation)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* This TME register setting is valid when TOE, TRE registers are set to 1. */
    FSP_ERROR_RETURN((TAUD_OUTPUT_ENABLE == p_extend->output_enable) &&
                     (TAUD_REAL_TIME_OUTPUT_ENABLE == p_extend->real_time_output),
                     FSP_ERR_UNSUPPORTED);
    FSP_ERROR_RETURN(TAUD_MODULATION_ENABLE >= modulation, FSP_ERR_INVALID_ARGUMENT);
#endif

    /* Enables/disables the modulation output. */
    p_instance_ctrl->p_reg->TAUDnTME =
        (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTME & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                    (modulation << p_instance_ctrl->p_cfg->channel));

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the dead time output level (TDL register setting).
 *
 * R_TAUD_DeadTimeOutputLevelSet support features: Complementary Modulation Output
 *                                                 (Slave channel 2 to Slave channel 7).
 *
 * @retval FSP_SUCCESS                   Dead time output level is set successfully.
 * @retval FSP_ERR_NOT_OPEN              The instance is not opened.
 * @retval FSP_ERR_ASSERTION             p_instance_ctrl or p_extend is NULL.
 * @retval FSP_ERR_UNSUPPORTED           The function is not supported for feature.
 * @retval FSP_ERR_INVALID_ARGUMENT      The dead time output level is not valid.
 **********************************************************************************************************************/
fsp_err_t R_TAUD_DeadTimeOutputLevelSet (timer_ctrl_t * const p_ctrl, taud_add_deadtime_t const add_deadtime_phase)
{
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) p_ctrl;

#if TAUD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(TAUD_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* This TDL register setting is valid when TOE, TOM, TOC, TDE registers are set to 1. */
    FSP_ERROR_RETURN((TAUD_OUTPUT_ENABLE == p_extend->output_enable) &&
                     (TAUD_OUTPUT_MODE_SYNCHRONOUS == p_extend->output_mode) &&
                     (TAUD_OUTPUT_OPERATING_MODE_2 == p_extend->output_operating) &&
                     (TAUD_DEADTIME_OPERATION_ENABLED == p_extend->deadtime_operation),
                     FSP_ERR_UNSUPPORTED);
    FSP_ERROR_RETURN(TAUD_ADD_DEADTIME_REVERSE_PHASE >= add_deadtime_phase, FSP_ERR_INVALID_ARGUMENT);
#endif

    /* Select the phase in which dead time is added (normal/reverse phase). */
    p_instance_ctrl->p_reg->TAUDnTDL =
        (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTDL & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                    (add_deadtime_phase << p_instance_ctrl->p_cfg->channel));

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup TAUD)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes control structure based on configuration.
 *
 * @param[in]  p_instance_ctrl         Instance control block.
 * @param[in]  p_cfg                   Pointer to timer configuration.
 **********************************************************************************************************************/
static void taud_common_open (taud_instance_ctrl_t * const p_instance_ctrl, timer_cfg_t const * const p_cfg)
{
    /* Initialize control structure. */
    p_instance_ctrl->p_cfg = p_cfg;

    /* Save pointer to extended configuration structure. */
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_cfg->p_extend;

    /* Set base address of TAUD module */
    uint32_t base_address;

    switch (p_extend->taud_unit)
    {
#if (BSP_FEATURE_TAUD_MAX_UNIT > (3U))
        case TAUD_UNIT3:
        {
            base_address = (uint32_t) R_TAUD3;
            break;
        }
#endif
#if (BSP_FEATURE_TAUD_MAX_UNIT > (2U))
        case TAUD_UNIT2:
        {
            base_address = (uint32_t) R_TAUD2;
            break;
        }
#endif
        case TAUD_UNIT1:
        {
            base_address = (uint32_t) R_TAUD1;
            break;
        }

        default:
        {
            base_address = (uint32_t) R_TAUD0;
            break;
        }
    }

    /* Set Base address of TAUD module. */
    p_instance_ctrl->p_reg = (R_TAUD_Type *) base_address;

    /* Set callback and context pointers, if configured. */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;
}

/*******************************************************************************************************************//**
 * Performs hardware initialization of the TAUD.
 *
 * @param[in]  p_instance_ctrl        Instance control block.
 * @param[in]  p_cfg                  Pointer to timer configuration.
 **********************************************************************************************************************/
static void taud_hardware_initialize (taud_instance_ctrl_t * const p_instance_ctrl, timer_cfg_t const * const p_cfg)
{
    /* Save pointer to extended configuration structure. */
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_cfg->p_extend;

    /* TAUDnCMORm value.TAUDn */
    uint16_t cmor_value = (0U);

    /* Set Prescaler Clock. */
    switch (p_extend->operation_clk)
    {
        case TAUD_CLOCK_CK1:
        {
            p_instance_ctrl->p_reg->TAUDnTPS_b.TAUDnPRS1 = p_extend->clk_div;
            break;
        }

        case TAUD_CLOCK_CK2:
        {
            p_instance_ctrl->p_reg->TAUDnTPS_b.TAUDnPRS2 = p_extend->clk_div;
            break;
        }

        case TAUD_CLOCK_CK3:
        {
            p_instance_ctrl->p_reg->TAUDnTPS_b.TAUDnPRS3 = p_extend->clk_div;
            p_instance_ctrl->p_reg->TAUDnBRS             = (uint8_t) (p_extend->baudrate - 1U);
            break;
        }

        /* Case of TAUD_CLOCK_CK0. */
        default:
        {
            p_instance_ctrl->p_reg->TAUDnTPS_b.TAUDnPRS0 = p_extend->clk_div;
            break;
        }
    }

    /* Set Operation Clock. */
    cmor_value |= (p_extend->operation_clk << TAUD_CMOR_CKS_Pos);

    /* Set Count Clock. */
    cmor_value |= (p_extend->count_clk << TAUD_CMOR_CCS_Pos);

    /* Set channel for synchronous operation. */
    if (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)
    {
        cmor_value |= (1U << TAUD_CMOR_MAS_Pos);
    }

    /* Set external start trigger type. */
    cmor_value |= (p_extend->trigger_type << TAUD_CMOR_STS_Pos);

    /* Specifies the timing for updating capture register and overflow flag. */
    cmor_value |= (p_extend->overflow_timing << TAUD_CMOR_COS_Pos);

    /* Set operating mode. */
    cmor_value |= (p_extend->operating_mode | p_extend->mode_config);

    /* Write setting data to TAUDnCMORm register.TAUDn */
    p_instance_ctrl->p_reg->TAUDnCMOR[p_cfg->channel].TAUDnCMOR = cmor_value;

    /* Set Channel Data Register value. */
    switch (p_extend->channel_type)
    {
        /* Set Channel Data Register value. */
        case TAUD_CHANNEL_TYPE_MASTER:
        {
            taud_update_cdr_master(p_instance_ctrl);
            break;
        }

        case TAUD_CHANNEL_TYPE_SLAVE:
        {
            taud_update_cdr_slave(p_instance_ctrl);
            break;
        }

        case TAUD_CHANNEL_TYPE_SLAVE_EVEN:
        {
            taud_update_cdr_slave_even(p_instance_ctrl);
            break;
        }

        case TAUD_CHANNEL_TYPE_SLAVE_ODD:
        {
            taud_update_cdr_slave_odd(p_instance_ctrl);
            break;
        }

        /* Case of TAUD_CHANNEL_TYPE_INDEPENDENT. */
        default:
        {
            taud_update_cdr_independent(p_instance_ctrl);
            break;
        }
    }

    /* Set type of valid edge detection for input signal. */
    p_instance_ctrl->p_reg->TAUDnCMUR[p_cfg->channel].TAUDnCMUR_b.TAUDnTIS = p_extend->edge_type;

    /* Set Channel Output Mode register. */
    if (TAUD_OUTPUT_ENABLE == p_extend->output_enable)
    {
        if (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)
        {
            /*Set output masks channel for synchronous operation. */
            p_instance_ctrl->output_mask |= (uint16_t) (1U << (p_cfg->channel));
        }

        /*Set enables/disables independent channel output mode. */
        p_instance_ctrl->p_reg->TAUDnTOE |= (1U << p_instance_ctrl->p_cfg->channel);

        /* Specifies Channel Output Mode. */
        if (TAUD_OUTPUT_MODE_INDEPENDENT == p_extend->output_mode)
        {
            p_instance_ctrl->p_reg->TAUDnTOM &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        }
        else
        {
            p_instance_ctrl->p_reg->TAUDnTOM |= (1U << p_instance_ctrl->p_cfg->channel);
        }

        /* Specifies Channel Output Configuration. */
        p_instance_ctrl->p_reg->TAUDnTOC |= (p_extend->output_operating << p_instance_ctrl->p_cfg->channel);

        /* Specifies an output logic. */
        if (TAUD_OUTPUT_LOGIC_DISABLED != p_extend->output_logic)
        {
            p_instance_ctrl->p_reg->TAUDnTOL |= (p_extend->output_logic << p_instance_ctrl->p_cfg->channel);
        }

        /* Clear target TDE bit then write target value. */
        p_instance_ctrl->p_reg->TAUDnTDE =
            (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTDE & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                        (p_extend->deadtime_operation << p_instance_ctrl->p_cfg->channel));

        /* Specifies the timing to add dead time during dead time output. */
        if (p_extend->taud_function == TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT)
        {
            p_instance_ctrl->p_reg->TAUDnTDM =
                (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTDM & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                            (1U << p_instance_ctrl->p_cfg->channel));
        }
        else
        {
            p_instance_ctrl->p_reg->TAUDnTDM &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        }

        /* Clear target TDL bit then write target value. */
        p_instance_ctrl->p_reg->TAUDnTDL =
            (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTDL & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                        (p_extend->add_deadtime_phase << p_instance_ctrl->p_cfg->channel));

        /* Specifies a channel on which the real-time output trigger for channel m is generated. */
        p_instance_ctrl->p_reg->TAUDnTRE =
            (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTRE & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                        (p_extend->real_time_output << p_instance_ctrl->p_cfg->channel));

        /* Sets a value which is output to TAUDTTOUTm. */
        p_instance_ctrl->p_reg->TAUDnTRO =
            (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTRO & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                        (p_extend->real_time_output_level << p_instance_ctrl->p_cfg->channel));

        /* Enables/disables modulation output for timer output and real-time output of channel m. */
        p_instance_ctrl->p_reg->TAUDnTME =
            (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTME & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                        (p_extend->modulation << p_instance_ctrl->p_cfg->channel));
    }
    else
    {
        /* Set enables/disables independent channel output mode. */
        p_instance_ctrl->p_reg->TAUDnTOE &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
    }

    /* Specifies a channel on which the real-time output trigger for channel m is generated. */
    p_instance_ctrl->p_reg->TAUDnTRC =
        (uint16_t) ((p_instance_ctrl->p_reg->TAUDnTRC & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                    (p_extend->real_time_output_trigger << p_instance_ctrl->p_cfg->channel));

    /* Set Simultaneous Rewrite registers. */
    if (TAUD_SIMULTANEOUS_REWRITE_ENABLE == p_extend->simultaneous_rewrite)
    {
        /* Enables simultaneous rewrite. */
        p_instance_ctrl->p_reg->TAUDnRDE |= (1U << p_instance_ctrl->p_cfg->channel);

        /* Set a channel that controls a simultaneous rewrite trigger. */
        p_instance_ctrl->p_reg->TAUDnRDS &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        p_instance_ctrl->p_reg->TAUDnRDS |=
            (uint16_t) (p_extend->simultaneous_rewrite_channel << p_instance_ctrl->p_cfg->channel);

        /* Set the timing for generating a simultaneous rewrite control signal. */
        p_instance_ctrl->p_reg->TAUDnRDM =
            (uint16_t) ((p_instance_ctrl->p_reg->TAUDnRDM & ~(1U << p_instance_ctrl->p_cfg->channel)) |
                        (p_extend->simultaneous_rewrite_trigger << p_instance_ctrl->p_cfg->channel));

        /* Specifies whether the channel generates a simultaneous rewrite trigger signal or not. */
        if ((TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_1 == p_extend->taud_function) ||
            (TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_2 == p_extend->taud_function))
        {
            p_instance_ctrl->p_reg->TAUDnRDC |= (uint16_t) (1U << p_instance_ctrl->p_cfg->channel);
        }
        else
        {
            p_instance_ctrl->p_reg->TAUDnRDC &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
        }
    }
    else
    {
        /* Disables simultaneous rewrite. */
        p_instance_ctrl->p_reg->TAUDnRDE &= (uint16_t) ~(1U << p_instance_ctrl->p_cfg->channel);
    }

    if (TAUD_CHANNEL_TYPE_MASTER == p_extend->channel_type)
    {
        /* Update master channel mask in instance control block. */
        p_instance_ctrl->master_channel_mask = (uint16_t) (1U << (p_cfg->channel));

#if (0 == TAUD_CFG_MULTI_SLAVE_ENABLE)
        const uint16_t index = 0;
#else
        for (uint16_t index = 0; index < TAUD_MAX_NUM_SLAVE_CHANNELS; index++)
#endif
        {
            if (NULL != p_extend->p_slave_channel_instances[index])
            {
                uint16_t slave_channel      = p_extend->p_slave_channel_instances[index]->p_cfg->channel;
                uint16_t slave_channel_mask = (uint16_t) (1U << slave_channel);
                p_instance_ctrl->slave_channels_mask |= slave_channel_mask;
                taud_extended_cfg_t * slave_channel_extend =
                    (taud_extended_cfg_t *) p_extend->p_slave_channel_instances[index]->p_cfg->p_extend;

                /* Set Channel Output Mode register. */
                if (TAUD_OUTPUT_ENABLE == slave_channel_extend->output_enable)
                {
                    /*Set output masks channel for synchronous operation. */
                    p_instance_ctrl->output_mask |= (uint16_t) (1U << (slave_channel));
                }
            }
        }

        /* Update channels mask in instance control block. */
        p_instance_ctrl->channels_mask = p_instance_ctrl->master_channel_mask | p_instance_ctrl->slave_channels_mask;
    }
    else if ((TAUD_CHANNEL_TYPE_INDEPENDENT != p_extend->channel_type) &&
             (TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT != p_extend->taud_function))
    {
        /* Update slave channels mask in instance control block. */
        p_instance_ctrl->slave_channels_mask |= (uint16_t) (1U << (p_cfg->channel));
    }
    else
    {
        /* Do nothing. */
    }
}

/*******************************************************************************************************************//**
 * Calculates clock frequency of TAUD counter.
 *
 * @param[in]  p_instance_ctrl           Instance control block
 *
 * @return     Clock frequency of the TAUD counter.
 **********************************************************************************************************************/
uint32_t taud_clock_frequency_get (taud_instance_ctrl_t * const p_instance_ctrl)
{
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    uint32_t clock_frequency = (0U);

    /* Get TAUD clock divider. */
    timer_source_div_t pclk_divisor = TIMER_SOURCE_DIV_1;

    switch (p_extend->operation_clk)
    {
        case TAUD_CLOCK_CK1:
        {
            pclk_divisor = (timer_source_div_t) p_instance_ctrl->p_reg->TAUDnTPS_b.TAUDnPRS1;
            break;
        }

        case TAUD_CLOCK_CK2:
        {
            pclk_divisor = (timer_source_div_t) p_instance_ctrl->p_reg->TAUDnTPS_b.TAUDnPRS2;
            break;
        }

        case TAUD_CLOCK_CK3:
        {
            pclk_divisor = (timer_source_div_t) p_instance_ctrl->p_reg->TAUDnTPS_b.TAUDnPRS3;
            break;
        }

        /* Case of TAUD_CLOCK_CK0. */
        default:
        {
            pclk_divisor = (timer_source_div_t) p_instance_ctrl->p_reg->TAUDnTPS_b.TAUDnPRS0;
            break;
        }
    }

    uint32_t pclk_freq_hz = TAUD_CFG_CLOCK;

    if (TAUD_CLOCK_CK3 == p_extend->operation_clk)
    {
        clock_frequency = (uint32_t) ((pclk_freq_hz >> pclk_divisor) / (p_instance_ctrl->p_reg->TAUDnBRS + 1U));
    }
    else
    {
        clock_frequency = (uint32_t) (pclk_freq_hz >> pclk_divisor);
    }

    return clock_frequency;
}

/*******************************************************************************************************************//**
 * Update Channel Data register for master channel.
 *
 * @param[in]  p_instance_ctrl           Instance control block
 **********************************************************************************************************************/
static void taud_update_cdr_master (taud_instance_ctrl_t * const p_instance_ctrl)
{
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    switch (p_extend->taud_function)
    {
        case TAUD_FUNCTION_ONE_SHOT_PULSE_OUTPUT:
        {
            /* Set pulse delay count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) (p_extend->delay_counts - 1U);
            break;
        }

        case TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT_DEADTIME:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts >> 1U) - 1U);
            break;
        }

        case TAUD_FUNCTION_DELAY_PULSE_OUTPUT:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) (p_instance_ctrl->p_cfg->period_counts - 1U);
            break;
        }

        case TAUD_FUNCTION_TRIGGER_START_PWM_OUTPUT:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) (p_instance_ctrl->p_cfg->period_counts - 1U);
            break;
        }

        case TAUD_FUNCTION_INTERRUPT_REQUEST_SIGNAL_CULLING:
        {
            /* Set period count */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) (p_instance_ctrl->p_cfg->period_counts - 1U);
            break;
        }

        case TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_1:
        {
            /* Set period count */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) (p_instance_ctrl->p_cfg->period_counts - 1U);
            break;
        }

        case TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts >> 1U) - 1U);
            break;
        }

        case TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts >> 1U) - 1U);
            break;
        }

        case TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1:
        {
            /* Set period count */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) (p_instance_ctrl->p_cfg->period_counts - 1U);
            break;
        }

        case TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts >> 1) - 1U);
            break;
        }

        case TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts >> 1) - 1U);
            break;
        }

        /* Case of TAUD_FUNCTION_PWM_OUTPUT. */
        default:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) (p_instance_ctrl->p_cfg->period_counts - 1U);
            break;
        }
    }
}

/*******************************************************************************************************************//**
 * Update Channel Data register for independent channel operations.
 *
 * @param[in]  p_instance_ctrl           Instance control block
 **********************************************************************************************************************/
static void taud_update_cdr_independent (taud_instance_ctrl_t * const p_instance_ctrl)
{
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    switch (p_extend->taud_function)
    {
        case TAUD_FUNCTION_INPUT_INTERVAL_TIMER:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts >> 1U) - 1U);
            break;
        }

        case TAUD_FUNCTION_ONE_PULSE_OUTPUT:
        {
            /* Set width counts. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) p_extend->width_counts;
            break;
        }

        case TAUD_FUNCTION_CLOCK_DIVIDE:
        {
            /* Set divider value. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_extend->divider_value) - 1U);
            break;
        }

        case TAUD_FUNCTION_EXTERNAL_EVENT_COUNT:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts) - 1U);
            break;
        }

        case TAUD_FUNCTION_DELAY_COUNT:
        {
            /* Set period count */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts) - 1U);
            break;
        }

        case TAUD_FUNCTION_OVERFLOW_INTERRUPT_OUTPUT_TYPE_1:
        {
            /* Set TAUDnCDRm value to 0xFFFF.TAUDn */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) TAUD_OVERFLOW_INTERRUPT_FUNCTION_CDR_VALUE;
            break;
        }

        case TAUD_FUNCTION_OVERFLOW_INTERRUPT_OUTPUT_TYPE_2:
        {
            /* Set TAUDnCDRm value to 0xFFFF.TAUDn */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) TAUD_OVERFLOW_INTERRUPT_FUNCTION_CDR_VALUE;
            break;
        }

        case TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_1:
        {
            /* Set period count */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts) - 1U);
            break;
        }

        case TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_1:
        {
            /* Set period count */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts) - 1U);
            break;
        }

        case TAUD_FUNCTION_INPUT_PULSE_INTERVAL_JUDGMENT:
        {
            /* Set period count */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts) - 1U);
            break;
        }

        case TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_JUDGMENT:
        {
            /* Set period count */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts) - 1U);
            break;
        }

        /* Case of TAUD_FUNCTION_INTERVAL_TIMER. */
        default:
        {
            /* Set period count. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) ((p_instance_ctrl->p_cfg->period_counts >> 1U) - 1U);
            break;
        }
    }
}

/*******************************************************************************************************************//**
 * Update Channel Data register for slave channel.
 *
 * @param[in]  p_instance_ctrl           Instance control block
 **********************************************************************************************************************/
static void taud_update_cdr_slave (taud_instance_ctrl_t * const p_instance_ctrl)
{
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Calculate the CDRm value. */
    uint32_t cdr_value = (0U);

    if (TAUD_FUNCTION_PWM_OUTPUT == p_extend->taud_function)
    {
        /* Set duty cycle. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) p_instance_ctrl->p_cfg->duty_cycle_counts;
    }
    else if (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1 == p_extend->taud_function)
    {
        /* Set duty cycle */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) p_instance_ctrl->p_cfg->duty_cycle_counts;
    }
    else if (TAUD_FUNCTION_ONE_SHOT_PULSE_OUTPUT == p_extend->taud_function)
    {
        /* Set pulse width counts. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) p_extend->width_counts;
    }
    else if (TAUD_FUNCTION_DELAY_PULSE_OUTPUT == p_extend->taud_function)
    {
        if ((1U == p_extend->slave_ordinal_number) || (3U == p_extend->slave_ordinal_number))
        {
            /* Set pulse width counts for reference signal and delay signal. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) p_instance_ctrl->p_cfg->duty_cycle_counts;
        }
        else if (2U == p_extend->slave_ordinal_number)
        {
            /* Set delay counts for delay signal. */
            p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
                (uint16_t) (p_extend->delay_counts - 1U);
        }
        else
        {
            /* Do nothing. */
        }
    }
    else if (TAUD_FUNCTION_INTERRUPT_REQUEST_SIGNAL_CULLING == p_extend->taud_function)
    {
        /* Set period count */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) ((p_instance_ctrl->p_cfg->period_counts) - 1U);
    }
    else if (TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT == p_extend->taud_function)
    {
        /* Set CDRm value based on duty cycle counts. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) ((p_instance_ctrl->p_cfg->period_counts - p_instance_ctrl->p_cfg->duty_cycle_counts) >> 1U);
    }
    else if (TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2 == p_extend->taud_function)
    {
        /* Set CDRm value based on duty cycle counts. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) ((p_instance_ctrl->p_cfg->period_counts - p_instance_ctrl->p_cfg->duty_cycle_counts) >> 1U);
    }
    else if (TAUD_FUNCTION_TRIGGER_START_PWM_OUTPUT == p_extend->taud_function)
    {
        /* Set duty cycle. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) p_instance_ctrl->p_cfg->duty_cycle_counts;
    }
    else if (TAUD_FUNCTION_OFFSET_TRIGGER_OUTPUT == p_extend->taud_function)
    {
        /* Set pulse width counts */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) p_extend->width_counts;
    }
    else if ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 == p_extend->taud_function) &&
             ((1U) == p_extend->slave_ordinal_number))
    {
        /* Set the number of interrupts of the Master Channel to be ignored in each interrupt of Slave Channel 1 is generated. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) ((p_instance_ctrl->p_cfg->period_counts) - 1U);
    }
    else if ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 == p_extend->taud_function) &&
             (((2U) == p_extend->slave_ordinal_number) || ((3U) == p_extend->slave_ordinal_number) ||
              ((4U) == p_extend->slave_ordinal_number) || ((5U) == p_extend->slave_ordinal_number) ||
              ((6U) == p_extend->slave_ordinal_number) || ((7U) == p_extend->slave_ordinal_number)))
    {
        if (0U == p_instance_ctrl->p_cfg->duty_cycle_counts)
        {
            cdr_value = p_instance_ctrl->p_cfg->period_counts >> 1U;
        }
        else if (p_instance_ctrl->p_cfg->duty_cycle_counts >= p_instance_ctrl->p_cfg->period_counts)
        {
            cdr_value = (0U);
        }
        else
        {
            cdr_value =
                (p_instance_ctrl->p_cfg->period_counts - p_instance_ctrl->p_cfg->duty_cycle_counts) >> 1U;
        }

        /* Set the CDRm value based on duty cycle counts. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) cdr_value;
    }
    else if ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_1 == p_extend->taud_function) &&
             ((1U) == p_extend->slave_ordinal_number))
    {
        /* Set the CDRm value 0x0000 for Slave Channel 1. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) (0U);
    }
    else if ((TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_1 == p_extend->taud_function) &&
             (((2U) <= p_extend->slave_ordinal_number) && ((7U) >= p_extend->slave_ordinal_number)))
    {
        /* Set the CDRm value based on duty cycle counts for Slave Channel 2 to Slave Channel 7. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) p_instance_ctrl->p_cfg->duty_cycle_counts;
    }
    else if ((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) &&
             ((1U) == p_extend->slave_ordinal_number))
    {
        /* Set the number of interrupts of the Master Channel to be ignored
         * in each interrupt of Slave Channel 1 is generated. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) p_instance_ctrl->p_cfg->period_counts;
    }
    else if ((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) &&
             (((2U) == p_extend->slave_ordinal_number) || ((4U) == p_extend->slave_ordinal_number) ||
              ((6U) == p_extend->slave_ordinal_number)))
    {
        if (0U == p_instance_ctrl->p_cfg->duty_cycle_counts)
        {
            cdr_value = p_instance_ctrl->p_cfg->period_counts >> 1U;
        }
        else if (p_instance_ctrl->p_cfg->duty_cycle_counts >= p_instance_ctrl->p_cfg->period_counts)
        {
            cdr_value = (0U);
        }
        else
        {
            cdr_value =
                (p_instance_ctrl->p_cfg->period_counts - p_instance_ctrl->p_cfg->duty_cycle_counts -
                 p_extend->deadtime_value) >> 1U;
        }

        /* Set the CDRm value based on duty cycle counts. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) cdr_value;
    }
    else if ((TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT == p_extend->taud_function) &&
             (((3U) == p_extend->slave_ordinal_number) || ((5U) == p_extend->slave_ordinal_number) ||
              ((7U) == p_extend->slave_ordinal_number)))
    {
        if (1U > p_extend->deadtime_value)
        {
            cdr_value = (1U);
        }
        else
        {
            cdr_value = (p_extend->deadtime_value - 1U);
        }

        /* Set the dead time. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) cdr_value;
    }
    else
    {
        /* Do nothing. */
    }
}

/*******************************************************************************************************************//**
 * Update Channel Data register for slave even channel.
 *
 * @param[in]  p_instance_ctrl           Instance control block
 **********************************************************************************************************************/
static void taud_update_cdr_slave_even (taud_instance_ctrl_t * const p_instance_ctrl)
{
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Calculate the CDRm value based on duty cycle counts. */
    uint32_t cdr_value = (0U);

    if (0U == p_instance_ctrl->p_cfg->duty_cycle_counts)
    {
        cdr_value = p_instance_ctrl->p_cfg->period_counts >> 1U;
    }
    else if (p_instance_ctrl->p_cfg->duty_cycle_counts >= p_instance_ctrl->p_cfg->period_counts)
    {
        cdr_value = (0U);
    }
    else
    {
        cdr_value =
            (p_instance_ctrl->p_cfg->period_counts - p_instance_ctrl->p_cfg->duty_cycle_counts -
             p_extend->deadtime_value) >> 1U;
    }

    /* Set the CDRm value based on duty cycle counts. */
    p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) cdr_value;
}

/*******************************************************************************************************************//**
 * Update Channel Data register for slave odd channel.
 *
 * @param[in]  p_instance_ctrl           Instance control block
 **********************************************************************************************************************/
static void taud_update_cdr_slave_odd (taud_instance_ctrl_t * const p_instance_ctrl)
{
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Calculate the CDRm value based on dead time value counts. */
    uint32_t cdr_value = (0U);

    if (1U > p_extend->deadtime_value)
    {
        cdr_value = (1U);
    }
    else
    {
        cdr_value = (p_extend->deadtime_value - 1U);
    }

    if (p_extend->taud_function == TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT)
    {
        /* Set the CDRm value based on deadtime count. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR =
            (uint16_t) ((p_extend->deadtime_value) - 1U);
    }
    else
    {
        /* Set the dead time. */
        p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR = (uint16_t) cdr_value;
    }
}

/***********************************************************************************************************************
 * Initialize the digital noise filters for U2A device.
 **********************************************************************************************************************/
#if defined(BSP_MCU_GROUP_RH850U2Ax)
static void taud_init_dnf (taud_instance_ctrl_t * const p_instance_ctrl)
{
    /* Save pointer to extended configuration structure. */
    taud_extended_cfg_t * p_extend = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Get the configured unit. */
    taud_unit_source_t unit_used = p_extend->taud_unit;

    /* Enable the digital noise elimination based on channel used and
     * set the stub value for digital noise elimination control register. */
    if (TAUD_UNIT0 == unit_used)
    {
        DNFAEN_TAUD0 |= (uint16_t) (1U << p_instance_ctrl->p_cfg->channel);
        DNFACTL_TAUD0 = 0;
    }
    else if (TAUD_UNIT1 == unit_used)
    {
        DNFAEN_TAUD1 |= (uint16_t) (1U << p_instance_ctrl->p_cfg->channel);
        DNFACTL_TAUD1 = 0;
    }
    else                               /* (TAUD_UNIT2 == unit_used) */
    {
        DNFAEN_TAUD2 |= (uint16_t) (1U << p_instance_ctrl->p_cfg->channel);
        DNFACTL_TAUD2 = 0;
    }
}

#endif

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to TAUD instance control block
 * @param[in]     event      Event code
 * @param[in]     capture    Event capture counts (if applicable)
 **********************************************************************************************************************/
static void r_taud_call_callback (taud_instance_ctrl_t * p_ctrl, timer_event_t event, uint32_t capture)
{
    timer_callback_args_t args;

    args.event     = event;
    args.capture   = capture;
    args.p_context = p_ctrl->p_context;

    p_ctrl->p_callback(&args);
}

/***********************************************************************************************************************
 * Interrupt service routines
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void taud_int_isr (void)
{
    /* Save context if RTOS is used. */
    FSP_CONTEXT_SAVE;

    /* Recover ISR context saved in open. */
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);
    taud_instance_ctrl_t * p_instance_ctrl = (taud_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    taud_extended_cfg_t  * p_extend        = (taud_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    uint32_t capture         = (0U);
    uint8_t  overflow_occurs = (0U);

    timer_event_t event = TIMER_EVENT_CYCLE_END;

    if ((TAUD_FUNCTION_INPUT_PERIOD_COUNT_DETECTION == p_extend->taud_function) ||
        (TAUD_FUNCTION_INPUT_POSITION_DETECTION == p_extend->taud_function) ||
        (TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_2 == p_extend->taud_function))
    {
        event = TIMER_EVENT_INPUT_CAPTURE;

        /* Get captured value. */
        capture = (uint32_t) (p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR + 1U);
    }

    if ((TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_MEASUREMENT == p_extend->taud_function) ||
        (TAUD_FUNCTION_INPUT_PULSE_INTERVAL_MEASUREMENT == p_extend->taud_function) ||
        (TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_2 == p_extend->taud_function))
    {
        event = TIMER_EVENT_INPUT_CAPTURE;

        /* Get captured value. */
        capture = (uint32_t) (p_instance_ctrl->p_reg->TAUDnCDR[p_instance_ctrl->p_cfg->channel].TAUDnCDR + 1U);

        /* Get overflow status. */
        overflow_occurs = p_instance_ctrl->p_reg->TAUDnCSR[p_instance_ctrl->p_cfg->channel].TAUDnCSR_b.TAUDnOVF;

        /* Update when overflow occurs. */
        if (1U == overflow_occurs)
        {
            /* Update captured value. */
            capture = capture + TAUD_OVERFLOW_VALUE;
        }
    }

    /* Invoke the callback function if it is set. */
    if (NULL != p_instance_ctrl->p_callback)
    {
        r_taud_call_callback(p_instance_ctrl, event, capture);
    }

    /* Restore context if RTOS is used. */
    FSP_CONTEXT_RESTORE;
}
