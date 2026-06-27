/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_rtca.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RTC_OPEN                                     (0x52544341ULL)

#define RTC_MASK_MSB                                 (0x0F)
#define RTC_MASK_LSB                                 (0xF0)

#define RTC_MAX_ERROR_ADJUSTMENT_VALUE               (0x3FU)

#define RTC_RTCA0ALM_DISABLE_VALUE                   (0x7FU)
#define RTC_RTCA0ALH_DISABLE_VALUE                   (0x3FU)
#define RTC_RTCA0ALW_DISABLE_VALUE                   (0x00U)

#define RTC_RTCA0CTRL1_RTCA0CT_ADJUSTMENT_VALUE_0    (11)
#define RTC_RTCA0CTRL1_RTCA0CT_ADJUSTMENT_VALUE_1    (12)

#define R_RTCA0_ALW_SUNDAY                           (0UL)
#define R_RTCA0_ALW_MONDAY                           (1UL)
#define R_RTCA0_ALW_TUESDAY                          (2UL)
#define R_RTCA0_ALW_WEDNESDAY                        (3UL)
#define R_RTCA0_ALW_THURSDAY                         (4UL)
#define R_RTCA0_ALW_FRIDAY                           (5UL)
#define R_RTCA0_ALW_SATURDAY                         (6UL)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static uint8_t r_rtc_dec_to_bcd(uint8_t to_convert);
static uint8_t r_rtc_bcd_to_dec(uint8_t to_convert);
static void    r_rtc_config(rtc_cfg_t const * const p_config);

static void r_rtc_error_adjustment_set(rtc_instance_ctrl_t * const              p_ctrl,
                                       rtc_error_adjustment_cfg_t const * const p_err_adj_cfg);

#if RTC_CFG_PARAM_CHECKING_ENABLE

static fsp_err_t r_rtc_err_adjustment_paramter_check(rtc_error_adjustment_cfg_t const * const p_err_adj_cfg);

static fsp_err_t r_rtc_time_and_date_validate(rtc_time_t * const p_time);

static fsp_err_t r_rtc_time_validate(rtc_time_t * const p_time);

static fsp_err_t r_rtc_date_validate(rtc_time_t * const p_time);

static fsp_err_t r_rtc_alarm_time_and_date_validate(rtc_alarm_time_t * const p_time);

static fsp_err_t r_rtc_alarm_time_validate(rtc_alarm_time_t * const p_time);

static fsp_err_t r_rtc_periodic_irq_rate_validate(rtc_periodic_irq_select_t const rate);

#endif

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
void rtc_alarm_isr(void);
void rtc_periodic_isr(void);
void rtc_one_second_isr(void);

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/** RTC Implementation of Real Time Clock  */
const rtc_api_t g_rtc_on_rtca =
{
    .open               = R_RTC_Open,
    .close              = R_RTC_Close,
    .clockSourceSet     = R_RTC_ClockSourceSet,
    .calendarTimeGet    = R_RTC_CalendarTimeGet,
    .calendarTimeSet    = R_RTC_CalendarTimeSet,
    .calendarAlarmGet   = R_RTC_CalendarAlarmGet,
    .calendarAlarmSet   = R_RTC_CalendarAlarmSet,
    .periodicIrqRateSet = R_RTC_PeriodicIrqRateSet,
    .infoGet            = R_RTC_InfoGet,
    .errorAdjustmentSet = R_RTC_ErrorAdjustmentSet,
    .callbackSet        = R_RTC_CallbackSet,
    .timeCaptureSet     = R_RTC_TimeCaptureSet,
    .timeCaptureGet     = R_RTC_TimeCaptureGet,
};

/*******************************************************************************************************************//**
 * @addtogroup RTCA
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
#if RTC_CFG_PARAM_CHECKING_ENABLE

/* Number of days in each months start from January to December */
static const uint8_t g_days_in_months[12] = {31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};
#endif

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Opens and configures the RTCA driver module. Implements @ref rtc_api_t::open.
 * Configuration includes clock source, and interrupt callback function.
 *
 * @retval FSP_SUCCESS              Initialization was successful and RTCA has started.
 * @retval FSP_ERR_ASSERTION        Invalid p_ctrl or p_cfg pointer.
 * @retval FSP_ERR_ALREADY_OPEN     Module is already open.
 * @retval FSP_ERR_INVALID_ARGUMENT Invalid parameter.
 **********************************************************************************************************************/
fsp_err_t R_RTC_Open (rtc_ctrl_t * const p_ctrl, rtc_cfg_t const * const p_cfg)
{
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;

    /* Parameter checking */
#if RTC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(RTC_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
#endif

    /* Save the configuration  */
    p_instance_ctrl->p_cfg = p_cfg;

    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

#if RTC_CFG_PARAM_CHECKING_ENABLE

    /* Verify the frequency comparison value for RTCA0SCMP */
    FSP_ERROR_RETURN(R_RTCA0_SCMP_SCMP_Msk >= p_cfg->freq_compare_value, FSP_ERR_INVALID_ARGUMENT);
#endif

    /*Enable interrupt RTC*/
    if (p_cfg->periodic_irq >= 0)
    {
        R_BSP_IrqCfgEnable(p_cfg->periodic_irq, p_cfg->periodic_ipl, p_ctrl);
    }

    if (p_cfg->alarm_irq >= 0)
    {
        R_BSP_IrqCfgEnable(p_cfg->alarm_irq, p_cfg->alarm_ipl, p_ctrl);
    }

    if (p_cfg->one_second_irq >= 0)
    {
        R_BSP_IrqCfgEnable(p_cfg->one_second_irq, p_cfg->one_second_ipl, p_ctrl);
    }

    /* Set the clock source for RTCA. */
    r_rtc_config(p_cfg);

    /** Mark driver as open by initializing it to "RTCA" in its ASCII equivalent. */
    p_instance_ctrl->open = RTC_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Close the RTCA driver.
 * Implements @ref rtc_api_t::close
 *
 * @retval FSP_SUCCESS          De-Initialization was successful and RTCA driver closed.
 * @retval FSP_ERR_ASSERTION    Invalid p_ctrl.
 * @retval FSP_ERR_NOT_OPEN     Driver not open already for close.
 **********************************************************************************************************************/
fsp_err_t R_RTC_Close (rtc_ctrl_t * const p_ctrl)
{
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;

    /* Parameter checking */
#if RTC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    if (p_instance_ctrl->p_cfg->periodic_irq >= 0)
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->periodic_irq);
        R_FSP_IsrContextSet(p_instance_ctrl->p_cfg->periodic_irq, NULL);
    }

    if (p_instance_ctrl->p_cfg->alarm_irq >= 0)
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->alarm_irq);
        R_FSP_IsrContextSet(p_instance_ctrl->p_cfg->alarm_irq, NULL);
    }

    if (p_instance_ctrl->p_cfg->one_second_irq >= 0)
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->one_second_irq);
        R_FSP_IsrContextSet(p_instance_ctrl->p_cfg->one_second_irq, NULL);
    }

    /* Clear the interrupt enable bit and pulse output bit */
    R_RTCA0->CTL1 = 0U;

    /* Clock counter operation stopped */
    R_RTCA0->CTL0_b.CE = 0U;

    /* Check that the clock counter has stopped */
    FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL0_b.CEST, 0U);

    p_instance_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @ref rtc_api_t::clockSourceSet is not supported on the RTC.
 *
 * @retval FSP_ERR_UNSUPPORTED         Function not supported in this implementation.
 **********************************************************************************************************************/
fsp_err_t R_RTC_ClockSourceSet (rtc_ctrl_t * const p_ctrl)
{
    /*
     * RTCA doesn't support change clock source internally */
    FSP_PARAMETER_NOT_USED(p_ctrl);

    /* Return the unsupported error. */
    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Set the calendar time.
 *
 * Implements @ref rtc_api_t::calendarTimeSet.
 *
 * @retval FSP_SUCCESS              Calendar time set operation was successful.
 * @retval FSP_ERR_ASSERTION        Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN         Driver not open already for operation.
 * @retval FSP_ERR_INVALID_ARGUMENT Invalid time parameter field.
 **********************************************************************************************************************/
fsp_err_t R_RTC_CalendarTimeSet (rtc_ctrl_t * const p_ctrl, rtc_time_t * const p_time)
{
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;

#if RTC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(p_time);
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Verify the seconds, minutes, hours, year ,day of the week, day of the month, month and year are valid values */
    FSP_ERROR_RETURN(FSP_SUCCESS == r_rtc_time_and_date_validate(p_time), FSP_ERR_INVALID_ARGUMENT);
    rtc_extended_cfg_t * p_extend = (rtc_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
    rtc_extended_cfg_t * p_extend = (rtc_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif

    /* See " Section Updating Clock Counters" and "Section Initial Setting" of
     *  the RH850 U2x manual */

    if (1U == R_RTCA0->CTL0_b.CE)
    {
        /* Check that previous write operations to RTCA0SEC to RTCA0YEAR have been completed */
        FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL2_b.WST, 0U);

        /* Stops all clock counters temporarily. */
        R_RTCA0->CTL2_b.WAIT = 1U;

        /* Wait until the corresponding status flag is set and all clock counters have stopped */
        FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL2_b.WST, 1U);
    }

    /* Set the year, month, day of the week, ... */
    R_RTCA0->SEC  = r_rtc_dec_to_bcd((uint8_t) p_time->tm_sec);
    R_RTCA0->MIN  = r_rtc_dec_to_bcd((uint8_t) p_time->tm_min);
    R_RTCA0->HOUR = r_rtc_dec_to_bcd((uint8_t) p_time->tm_hour);
    R_RTCA0->WEEK = r_rtc_dec_to_bcd((uint8_t) p_time->tm_wday);
    R_RTCA0->DAY  = r_rtc_dec_to_bcd((uint8_t) p_time->tm_mday);

    /* Add one to match with HW register */
    R_RTCA0->MONTH = r_rtc_dec_to_bcd((uint8_t) (p_time->tm_mon + 1));

    /* Subtract 100 to match with HW register */
    R_RTCA0->YEAR = r_rtc_dec_to_bcd((uint8_t) (p_time->tm_year - RTC_C_TIME_OFFSET));

    if (1U == R_RTCA0->CTL0_b.CE)
    {
        /* SEC to YEAR counter operation enable */
        R_RTCA0->CTL2_b.WAIT = 0U;
    }
    else
    {
        /* Clock counter operation enable */
        R_RTCA0->CTL0_b.CE = 1U;
    }

    if (RTC_OPERATION_MODE_FREQUENCY == p_extend->operation_mode)
    {
        /* Check that sub-counter is operating */
        FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL0_b.CEST, 1U);

        /*Wait until the previous write to RTCAnSCMP has been completed*/
        FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL2_b.WSST, 0U);

        R_RTCA0->SCMP_b.SCMP = (p_instance_ctrl->p_cfg->freq_compare_value) & R_RTCA0_SCMP_SCMP_Msk;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get the calendar time.
 *
 * Implements @ref rtc_api_t::calendarTimeGet
 *
 * @retval FSP_SUCCESS          Calendar time get operation was successful.
 * @retval FSP_ERR_ASSERTION    Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN     Driver not open already for operation.
 **********************************************************************************************************************/
fsp_err_t R_RTC_CalendarTimeGet (rtc_ctrl_t * const p_ctrl, rtc_time_t * const p_time)
{
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;
#if RTC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(p_time);
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#endif

    /* See section "Procedure for Reading Count Buffer Registers" of the RH850 U2x microcontroller manual */

    /* Check that previous write operations to RTCAnSEC to RTCAnYEAR have been completed */
    FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL2_b.WST, 0U);

    /* Stop all clock counters RTCAnSECC to RTCAnYEARC counter operation */
    R_RTCA0->CTL2_b.WAIT = 1U;

    /* Wait until the corresponding status flag is set and all clock counters have stopped */
    FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL2_b.WST, 1U);

    p_time->tm_sec  = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->SEC);
    p_time->tm_min  = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->MIN);
    p_time->tm_hour = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->HOUR);
    p_time->tm_mday = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->DAY);
    p_time->tm_wday = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->WEEK);

    /* Subtract one to match with C time.h standards */
    p_time->tm_mon = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->MONTH) - 1;

    /* Add 100 to match with C time.h standards */
    p_time->tm_year = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->YEAR) + RTC_C_TIME_OFFSET;

    /* Start all clock counters SECC to YEARC */
    R_RTCA0->CTL2_b.WAIT = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the calendar alarm time.
 *
 * Implements @ref rtc_api_t::calendarAlarmSet.
 *
 * @pre The calendar counter must be running before the alarm can be set.
 *
 * @retval FSP_SUCCESS              Calendar alarm time set operation was successful.
 * @retval FSP_ERR_INVALID_ARGUMENT Invalid time parameter field.
 * @retval FSP_ERR_ASSERTION        Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN         Driver not open already for operation.
 * @retval FSP_ERR_IRQ_BSP_DISABLED User IRQ parameter not valid
 **********************************************************************************************************************/
fsp_err_t R_RTC_CalendarAlarmSet (rtc_ctrl_t * const p_ctrl, rtc_alarm_time_t * const p_alarm)
{
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;
    volatile uint8_t      field;

#if RTC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(p_alarm);
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(FSP_INVALID_VECTOR != p_instance_ctrl->p_cfg->alarm_irq, FSP_ERR_IRQ_BSP_DISABLED);

    /* Verify the seconds, minutes, hours, year ,day of the week, day of the month and month are valid values */
    FSP_ERROR_RETURN(FSP_SUCCESS == r_rtc_alarm_time_and_date_validate(p_alarm), FSP_ERR_INVALID_ARGUMENT);
#endif

    if (p_instance_ctrl->p_cfg->alarm_irq >= 0)
    {
        /* Disable the ICU alarm interrupt request */
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->alarm_irq);
    }

    /* Set alarm time */

    if (p_alarm->min_match)
    {
        field = r_rtc_dec_to_bcd((uint8_t) p_alarm->time.tm_min);
    }
    else
    {
        field = RTC_RTCA0ALM_DISABLE_VALUE;
    }

    R_RTCA0->ALM = field;

    if (p_alarm->hour_match)
    {
        field = r_rtc_dec_to_bcd((uint8_t) p_alarm->time.tm_hour);
    }
    else
    {
        field = RTC_RTCA0ALH_DISABLE_VALUE;
    }

    R_RTCA0->ALH = field;

    R_RTCA0->ALW = (uint8_t) ((p_alarm->sunday_match << R_RTCA0_ALW_SUNDAY) |
                              (p_alarm->monday_match << R_RTCA0_ALW_MONDAY) |
                              (p_alarm->tuesday_match << R_RTCA0_ALW_TUESDAY) |
                              (p_alarm->wednesday_match << R_RTCA0_ALW_WEDNESDAY) |
                              (p_alarm->thursday_match << R_RTCA0_ALW_THURSDAY) |
                              (p_alarm->friday_match << R_RTCA0_ALW_FRIDAY) |
                              (p_alarm->saturday_match << R_RTCA0_ALW_SATURDAY));

    /* Enable the alarm interrupt */
    R_RTCA0->CTL1_b.ENALM = 1U;

    if (p_instance_ctrl->p_cfg->alarm_ipl != BSP_IRQ_DISABLED)
    {
        R_BSP_IrqEnable(p_instance_ctrl->p_cfg->alarm_irq);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get the calendar alarm time.
 *
 * Implements @ref rtc_api_t::calendarAlarmGet
 *
 * @retval FSP_SUCCESS           Calendar alarm time get operation was successful.
 * @retval FSP_ERR_ASSERTION     Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN      Driver not open already for operation.
 **********************************************************************************************************************/
fsp_err_t R_RTC_CalendarAlarmGet (rtc_ctrl_t * const p_ctrl, rtc_alarm_time_t * const p_alarm)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_alarm);
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
#endif

    /* Get alarm time */
    p_alarm->time.tm_min  = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->ALM);
    p_alarm->time.tm_hour = r_rtc_bcd_to_dec((uint8_t) R_RTCA0->ALH);

    p_alarm->sunday_match    = (bool) ((R_RTCA0->ALW_b.ALW >> R_RTCA0_ALW_SUNDAY) & 1UL);
    p_alarm->monday_match    = (bool) ((R_RTCA0->ALW_b.ALW >> R_RTCA0_ALW_MONDAY) & 1UL);
    p_alarm->tuesday_match   = (bool) ((R_RTCA0->ALW_b.ALW >> R_RTCA0_ALW_TUESDAY) & 1UL);
    p_alarm->wednesday_match = (bool) ((R_RTCA0->ALW_b.ALW >> R_RTCA0_ALW_WEDNESDAY) & 1UL);
    p_alarm->thursday_match  = (bool) ((R_RTCA0->ALW_b.ALW >> R_RTCA0_ALW_THURSDAY) & 1UL);
    p_alarm->friday_match    = (bool) ((R_RTCA0->ALW_b.ALW >> R_RTCA0_ALW_FRIDAY) & 1UL);
    p_alarm->saturday_match  = (bool) ((R_RTCA0->ALW_b.ALW >> R_RTCA0_ALW_SATURDAY) & 1UL);

    p_alarm->min_match       = (RTC_RTCA0ALM_DISABLE_VALUE != R_RTCA0->ALM);
    p_alarm->hour_match      = (RTC_RTCA0ALH_DISABLE_VALUE != R_RTCA0->ALH);
    p_alarm->dayofweek_match = (RTC_RTCA0ALW_DISABLE_VALUE != R_RTCA0->ALW);

    /* Set unsupported items to false */
    p_alarm->sec_match  = false;
    p_alarm->mday_match = false;
    p_alarm->mon_match  = false;
    p_alarm->year_match = false;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the periodic interrupt rate and enable periodic interrupt.
 *
 * Implements @ref rtc_api_t::periodicIrqRateSet
 *
 * @note To start the RTC @ref R_RTC_CalendarTimeSet must be called at least once.
 *
 * @retval FSP_SUCCESS               The periodic interrupt rate was successfully set.
 * @retval FSP_ERR_ASSERTION         Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN          Driver not open already for operation.
 * @retval FSP_ERR_IRQ_BSP_DISABLED  User IRQ parameter not valid
 * @retval FSP_ERR_INVALID_ARGUMENT  Invalid periodic IRQ rate value
 **********************************************************************************************************************/
fsp_err_t R_RTC_PeriodicIrqRateSet (rtc_ctrl_t * const p_ctrl, rtc_periodic_irq_select_t const rate)
{
    rtc_instance_ctrl_t * p_instance_ctrl     = (rtc_instance_ctrl_t *) p_ctrl;
    uint8_t               rtca_fixed_interval = 0;

#if RTC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg);
    FSP_ERROR_RETURN(FSP_INVALID_VECTOR != p_instance_ctrl->p_cfg->periodic_irq, FSP_ERR_IRQ_BSP_DISABLED);

    /* Verify the rate is valid value */
    FSP_ERROR_RETURN(FSP_SUCCESS == r_rtc_periodic_irq_rate_validate(rate), FSP_ERR_INVALID_ARGUMENT);
#endif

    if (p_instance_ctrl->p_cfg->periodic_irq >= 0)
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->periodic_irq);
    }

    if (RTC_PERIODIC_IRQ_SELECT_2_SECOND > rate)
    {
        rtca_fixed_interval = (uint8_t) (rate - RTC_RTCA0CTRL1_RTCA0CT_ADJUSTMENT_VALUE_0) & R_RTCA0_CTL1_CT_Msk;
    }
    else
    {
        rtca_fixed_interval = (uint8_t) (rate - RTC_RTCA0CTRL1_RTCA0CT_ADJUSTMENT_VALUE_1) & R_RTCA0_CTL1_CT_Msk;
    }

    R_RTCA0->CTL1_b.CT = (uint8_t) (rtca_fixed_interval & R_RTCA0_CTL1_CT_Msk);

    /*Enable interrupt RTC*/
    if (p_instance_ctrl->p_cfg->periodic_ipl != BSP_IRQ_DISABLED)
    {
        R_BSP_IrqEnable(p_instance_ctrl->p_cfg->periodic_irq);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get RTCA running status information and store it in provided pointer p_rtc_info.
 * Before or immediately after the first call to @ref R_RTC_CalendarTimeSet, RTC_STATUS_STOPPED will be returned.
 * Two PCLKRTC after the first call to @ref R_RTC_CalendarTimeSet, RTC_STATUS_RUNNING will be returned.
 *
 * Implements @ref rtc_api_t::infoGet
 *
 * @retval FSP_SUCCESS          Get information Successful.
 * @retval FSP_ERR_ASSERTION    Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN     Driver not open already for operation.
 **********************************************************************************************************************/
fsp_err_t R_RTC_InfoGet (rtc_ctrl_t * const p_ctrl, rtc_info_t * const p_rtc_info)
{
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;
#if RTC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_rtc_info);
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#endif

    p_rtc_info->status = (rtc_status_t) R_RTCA0->CTL0_b.CEST;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function sets time error adjustment
 *
 * Implements @ref rtc_api_t::errorAdjustmentSet
 *
 * @retval FSP_SUCCESS                 Time error adjustment successful.
 * @retval FSP_ERR_ASSERTION           Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN            Driver not open for operation.
 * @retval FSP_ERR_INVALID_MODE        Incorrect mode
 * @retval FSP_ERR_INVALID_ARGUMENT    Invalid error adjustment value.
 **********************************************************************************************************************/
fsp_err_t R_RTC_ErrorAdjustmentSet (rtc_ctrl_t * const p_ctrl, rtc_error_adjustment_cfg_t const * const p_err_adj_cfg)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(NULL != p_err_adj_cfg);
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg);
    rtc_extended_cfg_t * p_extend_cfg = (rtc_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Verify 32.768 kHz mode or not */
    FSP_ERROR_RETURN(RTC_OPERATION_MODE_32768 == p_extend_cfg->operation_mode, FSP_ERR_INVALID_MODE);

    /* Verify the frequecy comparison value for 32.768 kHz mode */
    FSP_ERROR_RETURN(FSP_SUCCESS == r_rtc_err_adjustment_paramter_check(p_err_adj_cfg), FSP_ERR_INVALID_ARGUMENT);
#endif

    /* Set Error Adjustment values */
    r_rtc_error_adjustment_set(p_ctrl, p_err_adj_cfg);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements rtc_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened
 **********************************************************************************************************************/
fsp_err_t R_RTC_CallbackSet (rtc_ctrl_t * const          p_ctrl,
                             void (                    * p_callback)(rtc_callback_args_t *),
                             void * const                p_context,
                             rtc_callback_args_t * const p_callback_memory)
{
    rtc_instance_ctrl_t * p_instance_ctrl = (rtc_instance_ctrl_t *) p_ctrl;

#if (RTC_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(RTC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */

    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Capture is not supported on the RTCA.
 *
 * @retval FSP_ERR_UNSUPPORTED         Function not supported in this implementation.
 **********************************************************************************************************************/
fsp_err_t R_RTC_TimeCaptureSet (rtc_ctrl_t * const p_ctrl, rtc_time_capture_t * const p_time_capture)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_time_capture);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Capture is not supported on the RTCA.
 *
 * @retval FSP_ERR_UNSUPPORTED         Function not supported in this implementation.
 **********************************************************************************************************************/
fsp_err_t R_RTC_TimeCaptureGet (rtc_ctrl_t * const p_ctrl, rtc_time_capture_t * const p_time_capture)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_time_capture);

    /* Return the unsupported error. */
    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup RTCA)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Set the RTCA clock source
 *
 * @param[in]  p_config                   Pointer to RTCA extended configuration.
 **********************************************************************************************************************/
static void r_rtc_config (rtc_cfg_t const * const p_config)
{
    rtc_extended_cfg_t * p_extend = (rtc_extended_cfg_t *) p_config->p_extend;

    /* Setting format hours. Set AMPM bit in the CTL0 register */
    R_RTCA0->CTL0_b.AMPM = p_extend->format_hours;

    /* The setting selects the operation mode */
    R_RTCA0->CTL0_b.SLSB = p_extend->operation_mode;

    /* The setting  1-second interrupt request generation*/
    R_RTCA0->CTL1_b.EN1S = p_extend->one_second_interrupt;

    /* The setting Enables/stops 1-Hz pulse output (RTCAT1HZ) */
    R_RTCA0->CTL1_b.EN1HZ = p_extend->one_hz_pulse_output;
}

/*******************************************************************************************************************//**
 * This function sets time error adjustment mode, period, type and value.
 *
 *@param[in] p_ctrl           Pointer to RTCA instance control block
 *@param[in] p_err_adj_cfg    Pointer to the Error Adjustment Configuration
 **********************************************************************************************************************/
static void r_rtc_error_adjustment_set (rtc_instance_ctrl_t * const              p_ctrl,
                                        rtc_error_adjustment_cfg_t const * const p_err_adj_cfg)
{
    rtc_extended_cfg_t          * p_extend_cfg     = (rtc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    rtc_operation_mode_t          mode             = p_extend_cfg->operation_mode;
    uint8_t                       error_adjustment = 0;
    rtc_error_adjustment_period_t period           = p_err_adj_cfg->adjustment_period;
    rtc_error_adjustment_t        type             = p_err_adj_cfg->adjustment_type;
    uint32_t                      value            = p_err_adj_cfg->adjustment_value;

    /* Set the period if mode is 32.768 kHz mode */
    if (RTC_OPERATION_MODE_32768 == mode)
    {
        /* Check that sub-counter is operating */
        FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL0_b.CEST, 1U);

        /*Wait until the previous write to RTCAnSUBU has been completed*/
        FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->CTL2_b.WUST, 0U);

        /*Setting error adjustment value*/
        error_adjustment =
            (uint8_t) ((uint8_t) (((uint8_t) type) << R_RTCA0_SUBU_F6_Pos) | (RTC_MAX_ERROR_ADJUSTMENT_VALUE & value) |
                       ((uint8_t) period << R_RTCA0_SUBU_DEV_Pos));

        /* Write value to SUBU register */
        R_RTCA0->SUBU = error_adjustment;

        /* When SUBU is modified, check that all the bits are updated before continuing with more processing. */
        FSP_HARDWARE_REGISTER_WAIT(R_RTCA0->SUBU, error_adjustment);
    }
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to RTCA instance control block
 * @param[in]     event      Event code
 **********************************************************************************************************************/
static void r_rtc_call_callback (rtc_instance_ctrl_t * p_ctrl, rtc_event_t event)
{
    rtc_callback_args_t args;

    /* Store callback arguments in memory provided by user if available. */
    rtc_callback_args_t * p_args = p_ctrl->p_callback_memory;
    if (NULL == p_args)
    {
        /* Store on stack */
        p_args = &args;
    }
    else
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        args = *p_args;
    }

    p_args->event     = event;
    p_args->p_context = p_ctrl->p_context;

    p_ctrl->p_callback(p_args);

    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }
}

#if RTC_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Validate Error Adjustment configuration when using SubClock
 *
 * @param[in]  p_err_adj_cfg           Pointer to error adjustment config
 *
 * @retval FSP_SUCCESS                 Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT    Invalid error configuration
 **********************************************************************************************************************/
static fsp_err_t r_rtc_err_adjustment_paramter_check (rtc_error_adjustment_cfg_t const * const p_err_adj_cfg)
{
    rtc_error_adjustment_period_t period = p_err_adj_cfg->adjustment_period;
    rtc_error_adjustment_t        mode   = p_err_adj_cfg->adjustment_type;
    uint32_t value = p_err_adj_cfg->adjustment_value;

    FSP_ERROR_RETURN((RTC_ERROR_ADJUSTMENT_PERIOD_20_SECOND == period) ||
                     (RTC_ERROR_ADJUSTMENT_PERIOD_1_MINUTE == period),
                     FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN((RTC_ERROR_ADJUSTMENT_ADD_PRESCALER == mode) || (RTC_ERROR_ADJUSTMENT_SUBTRACT_PRESCALER == mode),
                     FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN((RTC_MAX_ERROR_ADJUSTMENT_VALUE > value), FSP_ERR_INVALID_ARGUMENT);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Validate time and date fields of time parameter fields
 * Checking for seconds, minutes, hours are valid values by calling sub-function time validate.
 * Checking for year, month, day of the week and day of a month are valid values by calling sub-function
 * date validate.
 *
 * @param[in]  p_time                     Pointer to rtc_time_t
 *
 * @retval FSP_SUCCESS                    Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT       Invalid field in rtc_time_t structure
 **********************************************************************************************************************/
static fsp_err_t r_rtc_time_and_date_validate (rtc_time_t * const p_time)
{
    fsp_err_t err = FSP_SUCCESS;
    err = r_rtc_time_validate(p_time);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    err = r_rtc_date_validate(p_time);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Validate time fields of time type parameter
 * Checking for the seconds, minutes, hours values for valid specified range.
 * Seconds 0 to 59.
 * Minutes 0 to 59.
 * Hours   0 to 23.
 *
 * @param[in]  p_time                     Pointer to rtc_time_t
 *
 * @retval FSP_SUCCESS                    Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT       Invalid field in rtc_time_t structure
 **********************************************************************************************************************/
static fsp_err_t r_rtc_time_validate (rtc_time_t * p_time)
{
    fsp_err_t err;
    err = FSP_SUCCESS;
    if ((p_time->tm_sec < 0) || (p_time->tm_sec > RTC_SECONDS_IN_A_MINUTE) ||
        (p_time->tm_min < 0) || (p_time->tm_min > RTC_MINUTES_IN_A_HOUR) ||
        (p_time->tm_hour < 0) || (p_time->tm_hour > RTC_HOURS_IN_A_DAY))
    {
        err = FSP_ERR_INVALID_ARGUMENT;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Validate date fields of time type parameter
 * validating r_rtc date fields and setting day of a Week using Zeller's congruence.
 * Checking for year, month, day of the week and day of a month are valid values.
 * Leap year validation and Week of the day is calculated and updated in RTC time.
 * Day of week between 0 to 6
 * Day between 1 to 31
 * Month between 0 to 11 as per standard time.h, There's a mismatch between hardware configuration,
 * UM indicates that "a decimal value of 01 to 12 must be set in BCD code" for Month Count Buffer register in the RTC.
 * This difference will be taken care in the Set and Get functions.
 *
 * As per HW manual, value of Year is between 0 to 99, the RTC has a 100 year calendar from 2000 to 2099.
 * (see section "Overview" of the RH850 U2x microcontroller manual)
 * But as per C standards, tm_year is years since 1900.
 * A sample year set in an application would be like time.tm_year = 2019-1900; (to set year 2019)
 * Since RTC API follows the Date and time structure defined in C standard library <time.h>, the valid value of year is
 * between 100 and 199, which will be internally converted to HW required value.
 *
 * @param[in]  p_time                     Pointer to rtc_time_t
 *
 * @retval FSP_SUCCESS                    Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT       Invalid field in rtc_time_t structure
 **********************************************************************************************************************/
static fsp_err_t r_rtc_date_validate (rtc_time_t * const p_time)
{
    uint32_t num_days_month;
    uint32_t day_of_a_month;
    uint32_t temp_month;
    uint32_t temp_year;

    fsp_err_t err = FSP_SUCCESS;

    day_of_a_month = (uint32_t) p_time->tm_mday;
    temp_month     = (uint32_t) (p_time->tm_mon + RTC_TIME_H_MONTH_OFFSET);

    /* The valid value of year is between 100 to 199, The RTC has a 100 year calendar from 2000 to 2099
     * to match the starting year 2000, a sample year offset(1900) is added like 117 + 1900 = 2017*/
    temp_year = (uint32_t) (p_time->tm_year + RTC_TIME_H_YEAR_OFFSET);

    /* Checking the error condition for year and months values, here valid value of year is between 100 to 199
     * and for month 0 to 11*/
    if ((p_time->tm_year < RTC_YEAR_VALUE_MIN) || (p_time->tm_year > RTC_YEAR_VALUE_MAX) ||
        (p_time->tm_mon < 0) || (p_time->tm_mon > RTC_MONTHS_IN_A_YEAR))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /*For particular valid month, number of days in a month is updated */
    num_days_month = g_days_in_months[p_time->tm_mon];

    /* Checking for February month and Conditions for Leap year : Every fourth year is a leap year,
     * The RTC has a 100 year calendar from 2000 to 2099  */
    if ((RTC_FEBRUARY_MONTH == temp_month) && ((temp_year % 4U) == 0))
    {
        num_days_month = RTC_LAST_DAY_OF_LEAP_FEB_MONTH;
    }

    /* Checking for day of a month values for valid range */
    if ((p_time->tm_mday < RTC_FIRST_DAY_OF_A_MONTH) || (day_of_a_month > num_days_month))
    {
        err = FSP_ERR_INVALID_ARGUMENT;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Validate alarm time and date of Alarm time type parameter.
 * Checking that alarm enable bit are valid values.
 * Checking that alarm time with minutes and hours are valid values.
 *
 * @param[in]  p_time                     Pointer to rtc_time_t
 *
 * @retval FSP_SUCCESS                    Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT       Invalid field in rtc_time_t structure
 **********************************************************************************************************************/
static fsp_err_t r_rtc_alarm_time_and_date_validate (rtc_alarm_time_t * const p_time)
{
    fsp_err_t err = FSP_SUCCESS;
    err = r_rtc_alarm_time_validate(p_time);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    err = r_rtc_date_validate(&p_time->time);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Validate alarm time fields of Alarm time type parameter
 * Checking for alarm enable bit with the minutes, hours value for valid specified range.
 * Minutes 0 to 59.
 * Hours   0 to 23.
 *
 * @param[in]  p_time                     Pointer to rtc_time_t
 *
 * @retval FSP_SUCCESS                    Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT       Invalid field in rtc_time_t structure
 **********************************************************************************************************************/
static fsp_err_t r_rtc_alarm_time_validate (rtc_alarm_time_t * const p_time)
{
    fsp_err_t err;
    err = FSP_SUCCESS;

    if (((p_time->min_match) && ((p_time->time.tm_min < 0) ||
                                 (p_time->time.tm_min > RTC_MINUTES_IN_A_HOUR))) ||
        ((p_time->hour_match) && ((p_time->time.tm_hour < 0) || (p_time->time.tm_hour > RTC_HOURS_IN_A_DAY))))
    {
        err = FSP_ERR_INVALID_ARGUMENT;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Validate periodic IRQ rate
 *
 * @param[in] rate                        Periodic IRQ rate
 *
 * @retval FSP_SUCCESS                    Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT       Invalid periodic IRQ rate value
 **********************************************************************************************************************/
static fsp_err_t r_rtc_periodic_irq_rate_validate (rtc_periodic_irq_select_t const rate)
{
    fsp_err_t err = FSP_SUCCESS;

    if ((RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_4_SECOND == rate) ||
        (RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_2_SECOND == rate) ||
        (RTC_PERIODIC_IRQ_SELECT_1_SECOND == rate) ||
        (RTC_PERIODIC_IRQ_SELECT_1_MINUTE == rate) ||
        (RTC_PERIODIC_IRQ_SELECT_1_HOUR == rate) ||
        (RTC_PERIODIC_IRQ_SELECT_1_DAY == rate) ||
        (RTC_PERIODIC_IRQ_SELECT_1_MONTH == rate))
    {
        /* Do nothing */
    }
    else
    {
        err = FSP_ERR_INVALID_ARGUMENT;
    }

    return err;
}

#endif

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Convert decimal to BCD
 *
 * @param[in] to_convert   Decimal Value to be converted
 **********************************************************************************************************************/
static uint8_t r_rtc_dec_to_bcd (uint8_t to_convert)
{
    return (uint8_t) ((((to_convert / (uint8_t) 10) << 4) & (uint8_t) RTC_MASK_LSB) | (to_convert % (uint8_t) 10));
}

/*******************************************************************************************************************//**
 * Convert BCD to decimal
 *
 * @param[in] to_convert   BCD Value to be converted
 **********************************************************************************************************************/
static uint8_t r_rtc_bcd_to_dec (uint8_t to_convert)
{
    return (uint8_t) ((((to_convert & (uint8_t) RTC_MASK_LSB) >> 4) * (uint8_t) 10) +
                      (to_convert & (uint8_t) RTC_MASK_MSB));
}

/*******************************************************************************************************************//**
 * RTC Callback ISR for alarm interrupt.
 *
 * Saves context if RTOS is used, calls callback if one was provided in the open function, and restores context
 * if RTOS is used.
 **********************************************************************************************************************/
void rtc_alarm_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    IRQn_Type             irq    = R_FSP_CurrentIrqGet();
    rtc_instance_ctrl_t * p_ctrl = (rtc_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Call the callback routine if one is available */
    if (NULL != p_ctrl->p_callback)
    {
        /* Set data to identify callback to user, then call user callback. */
        rtc_event_t event;
        event = RTC_EVENT_ALARM_IRQ;

        /* Call callback */
        r_rtc_call_callback(p_ctrl, event);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}

/*******************************************************************************************************************//**
 * RTC Callback ISR for periodic interrupt.
 *
 * Saves context if RTOS is used, calls callback if one was provided in the open function, and restores context
 * if RTOS is used.
 **********************************************************************************************************************/
void rtc_periodic_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    IRQn_Type             irq    = R_FSP_CurrentIrqGet();
    rtc_instance_ctrl_t * p_ctrl = (rtc_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Call the callback routine if one is available */
    if (NULL != p_ctrl->p_callback)
    {
        /* Set data to identify callback to user, then call user callback. */
        rtc_event_t event;
        event = RTC_EVENT_PERIODIC_IRQ;

        /* Call callback */
        r_rtc_call_callback(p_ctrl, event);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}

/*******************************************************************************************************************//**
 * RTC Callback ISR for one second interrupt.
 *
 * Saves context if RTOS is used, calls callback if one was provided in the open function, and restores context
 * if RTOS is used.
 **********************************************************************************************************************/
void rtc_one_second_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    IRQn_Type             irq    = R_FSP_CurrentIrqGet();
    rtc_instance_ctrl_t * p_ctrl = (rtc_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Call the callback routine if one is available */
    if (NULL != p_ctrl->p_callback)
    {
        /* Set data to identify callback to user, then call user callback. */
        rtc_event_t event;
        event = RTC_EVENT_ONE_SECOND_IRQ;

        /* Call callback */
        r_rtc_call_callback(p_ctrl, event);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}
