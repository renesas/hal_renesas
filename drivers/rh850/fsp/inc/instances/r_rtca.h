/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_RTCA_H
#define R_RTCA_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_rtc_api.h"
#include "bsp_api.h"
#include "r_rtca_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup RTCA
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef enum e_rtc_hour_format
{
    RTC_HOUR_FORMAT_12 = 0,            // Select format hour is 12
    RTC_HOUR_FORMAT_24 = 1             // Select format hour is 24
} rtc_hour_format_t;

typedef enum e_rtc_one_second_irq
{
    RTC_ONE_SECOND_IRQ_DISABLES = 0,   // Disables one second interrupt request
    RTC_ONE_SECOND_IRQ_ENABLE   = 1,   // Enable one second interrupt request
} rtc_one_second_irq_t;

typedef enum e_rtc_one_hz_pulse_out
{
    RTC_ONE_HZ_PULSE_OUT_DISABLES = 0, // Disables one second interrupt request
    RTC_ONE_HZ_PULSE_OUT_ENABLE   = 1, // Enable one second interrupt request
} rtc_one_hz_pulse_out_t;

/** Selects the operation mode */
typedef enum e_rtc_operation_mode
{
    RTC_OPERATION_MODE_32768     = 0,  ///< 32.768 kHz mode
    RTC_OPERATION_MODE_FREQUENCY = 1,  ///< Frequency selection mode
} rtc_operation_mode_t;

typedef enum e_rtc_time_constants
{
    RTC_FIRST_DAY_OF_A_MONTH       = (1),   ///< First valid day of any month
    RTC_FEBRUARY_MONTH             = (2),   ///< February month index (used for leap year checks)
    RTC_MONTHS_IN_A_YEAR           = (11),  ///< Max tm_mon value (0–11 for Jan–Dec)
    RTC_HOURS_IN_A_DAY             = (23),  ///< Max hour value (0–23)
    RTC_LAST_DAY_OF_LEAP_FEB_MONTH = (29),  ///< Last day of February in a leap year
    RTC_SECONDS_IN_A_MINUTE        = (59),  ///< Max second value (0–59)
    RTC_MINUTES_IN_A_HOUR          = (59),  ///< Max minute value (0–59)
    RTC_YEAR_VALUE_MIN             = (100), ///< Minimum tm_year value (for year 2000)
    RTC_YEAR_VALUE_MAX             = (199), ///< Maximum tm_year value (for year 2099)
} e_rtc_time_constants_t;

typedef enum e_rtc_time_offset
{
    RTC_TIME_H_MONTH_OFFSET = (1),     ///< Offset to convert tm_mon (0–11) to RTC month format
    RTC_C_TIME_OFFSET       = (100),   ///< Offset to convert RTC year (0–99) to C standard tm_year
    RTC_TIME_H_YEAR_OFFSET  = (1900),  ///< Offset to convert tm_year back to actual calendar year
} rtc_time_offset_t;

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when @ref rtc_api_t::open is called */

typedef struct st_rtc_ctrl
{
    uint32_t          open;                     ///< Whether or not driver is open
    const rtc_cfg_t * p_cfg;                    ///< Pointer to initial configurations

    void (* p_callback)(rtc_callback_args_t *); // Pointer to callback that is called when a rtc_event_t occurs.
    rtc_callback_args_t * p_callback_memory;    // Pointer to non-secure memory that can be used to pass arguments to a callback in non-secure memory.

    void * p_context;                           // Pointer to context to be passed into callback function
} rtc_instance_ctrl_t;

/** RTC extended configuration */
typedef struct st_rtc_extended_cfg
{
    rtc_hour_format_t      format_hours;         ///< Select format hour for RTCA.
    rtc_one_second_irq_t   one_second_interrupt; ///< Enables/disables 1-second interrupt request generation
    rtc_one_hz_pulse_out_t one_hz_pulse_output;  ///< Enables/stops 1-Hz pulse output
    rtc_operation_mode_t   operation_mode;       ///< Selects the operation mode
} rtc_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const rtc_api_t g_rtc_on_rtca;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_RTC_Open(rtc_ctrl_t * const p_ctrl, rtc_cfg_t const * const p_cfg);
fsp_err_t R_RTC_Close(rtc_ctrl_t * const p_ctrl);
fsp_err_t R_RTC_ClockSourceSet(rtc_ctrl_t * const p_ctrl);
fsp_err_t R_RTC_CalendarTimeSet(rtc_ctrl_t * const p_ctrl, rtc_time_t * const p_time);
fsp_err_t R_RTC_CalendarTimeGet(rtc_ctrl_t * const p_ctrl, rtc_time_t * const p_time);
fsp_err_t R_RTC_CalendarAlarmSet(rtc_ctrl_t * const p_ctrl, rtc_alarm_time_t * const p_alarm);
fsp_err_t R_RTC_CalendarAlarmGet(rtc_ctrl_t * const p_ctrl, rtc_alarm_time_t * const p_alarm);
fsp_err_t R_RTC_PeriodicIrqRateSet(rtc_ctrl_t * const p_ctrl, rtc_periodic_irq_select_t const rate);
fsp_err_t R_RTC_InfoGet(rtc_ctrl_t * const p_ctrl, rtc_info_t * const p_rtc_info);
fsp_err_t R_RTC_CallbackSet(rtc_ctrl_t * const          p_ctrl,
                            void (                    * p_callback)(rtc_callback_args_t *),
                            void * const                p_context,
                            rtc_callback_args_t * const p_callback_memory);
fsp_err_t R_RTC_ErrorAdjustmentSet(rtc_ctrl_t * const p_ctrl, rtc_error_adjustment_cfg_t const * const p_err_adj_cfg);
fsp_err_t R_RTC_TimeCaptureSet(rtc_ctrl_t * const p_ctrl, rtc_time_capture_t * const p_time_capture);
fsp_err_t R_RTC_TimeCaptureGet(rtc_ctrl_t * const p_ctrl, rtc_time_capture_t * const p_time_capture);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_RTCA_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RTCA)
 **********************************************************************************************************************/
