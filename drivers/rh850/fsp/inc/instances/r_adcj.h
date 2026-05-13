/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_ADCJ_H
#define R_ADCJ_H

/*******************************************************************************************************************//**
 * @addtogroup ADCJ
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_adc_api.h"
#include "r_adcj_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ADCJ_UPPER_LOWER_LIMIT_TABLE_ENTRIES           (8U)
#define ADCJ_NUMBER_OF_AD_CONVERSION_TIMING_MONITOR    (5U)
#define ADCJ_WAIT_SETTING_TABLE_ENTRIES                (8U)
#define ADCJ_NUMBER_OF_TH_UNIT                         (4U)
#define ADCJ_NUMBER_OF_AD_TIMER                        (2U)
#define ADCJ_NUMBER_OF_VIRTUAL_CHANNEL                 (64U)
#define ADCJ_NUMBER_OF_TH_GROUP                        (2U)
#define ADCJ_NUMBER_OF_SG                              (BSP_OVERRIDE_ADC_NUMBER_OF_SG)
#define ADCJ_TRANSFER_COUNT(info, group)    ((uint16_t) ((info).length[(group)]))
#define ADCJ_SRC_ADDRESS(info, group)       ((void const * volatile) ((info).p_address[(group)]))

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** ADCJ suspend methods options */
typedef enum e_adcj_suspend_method
{
    ADCJ_SUSPEND_METHOD_SYNCHRONOUS              = 0U, ///< Synchronous suspend for all scan group
    ADCJ_SUSPEND_METHOD_SYNCHRONOUS_ASYNCHRONOUS = 1U, ///< Asynchronous suspend for SG0, synchronous otherwise
    ADCJ_SUSPEND_METHOD_ASYNCHRONOUS             = 2U  ///< Asynchronous suspend for all scan group
} adcj_suspend_method_t;

/** ADCJ addition count options */
typedef enum e_adcj_addition_count
{
    ADCJ_ADDITION_COUNT_TWICE = 0U,    ///< Addition twice
    ADCJ_ADDITION_COUNT_FOUR  = 1U     ///< Addition four
} adcj_addition_count_t;

/** ADCJ data format options */
typedef enum e_adcj_data_format
{
    ADCJ_DATA_FORMAT_12_BIT_SIGNED_FIXED_POINT               = 0U, ///< Resolution 12-bit signed fixed-point format
    ADCJ_DATA_FORMAT_12_BIT_SIGNED_INTEGER                   = 1U, ///< Resolution 12-bit signed integer format
    ADCJ_DATA_FORMAT_12_BIT_UNSIGNED_FIXED_POINT             = 2U, ///< Resolution 12-bit unsigned fixed-point format
    ADCJ_DATA_FORMAT_10_BIT_UNSIGNED_FIXED_POINT             = 3U, ///< Resolution 10-bit unsigned fixed-point format
    ADCJ_DATA_FORMAT_12_BIT_UNSIGNED_INTEGER_RIGHT_JUSTIFIED = 4U, ///< Resolution 12-bit unsigned integer format (right-justified)
    ADCJ_DATA_FORMAT_12_BIT_UNSIGNED_INTEGER_LEFT_JUSTIFIED  = 5U  ///< Resolution 12-bit unsigned integer format (left-justified)
} adcj_data_format_t;

/** ADCJ parity error interrupt enable or disable */
typedef enum e_adcj_parity_error_int
{
    ADCJ_PARITY_ERROR_INT_DISABLE = 0U, ///< Parity error not create error interrupt
    ADCJ_PARITY_ERROR_INT_ENABLE  = 1U  ///< Parity error create error interrupt
} adcj_parity_error_int_t;

/** ADCJ overwrite error interrupt enable or disable */
typedef enum e_adcj_overwrite_error_int
{
    ADCJ_OVERWRITE_ERROR_INT_DISABLE = 0U, ///< Overwrite error not create error interrupt
    ADCJ_OVERWRITE_ERROR_INT_ENABLE  = 1U  ///< Overwrite error create error interrupt
} adcj_overwrite_error_int_t;

/** ADCJ clear after read data register enable or disable */
typedef enum e_adcj_clear_after_read
{
    ADCJ_CLEAR_AFTER_READ_DISABLE = 0U, ///< Enable clear after read function
    ADCJ_CLEAR_AFTER_READ_ENABLE  = 1U  ///< Disable clear after read function
} adcj_clear_after_read_t;

/** ADC trigger overlap interrupt enable or disable */
typedef enum e_adcj_trigger_overlap_int
{
    ADCJ_TRIGGER_OVERLAP_INT_DISABLE = 0U, ///< Trigger overlap error not create error interrupt
    ADCJ_TRIGGER_OVERLAP_INT_ENABLE  = 1U  ///< Trigger overlap create error interrupt
} adcj_trigger_overlap_int_t;

/** ADCJ ID error interrupt enable or disable */
typedef enum e_adcj_id_error_int
{
    ADCJ_ID_ERROR_INT_DISABLE = 0U,    ///< ID error not create error interrupt
    ADCJ_ID_ERROR_INT_ENABLE  = 1U     ///< ID error create error interrupt
} adcj_id_error_int_t;

/** ADCJ PWM-Diag upper/lower limit interrupt */
typedef enum e_adcj_pwm_diag_ul_limit_int
{
    ADCJ_PWM_DIAG_UL_LIMIT_INT_DISABLE = 0U, ///< PWM-Diag upper/lower limit error not create error interrupt
    ADCJ_PWM_DIAG_UL_LIMIT_INT_ENABLE  = 1U  ///< PWM-Diag upper/lower limit error create error interrupt
} adcj_pwm_diag_ul_limit_int_t;

/** ADCJ sampling period options */
typedef enum e_adcj_sampling_period
{
    ADCJ_SAMPLING_PERIOD_18_STATES  = 0x00U, ///< 18 CLKAD cycles
    ADCJ_SAMPLING_PERIOD_102_STATES = 0x66U, ///< 102 CLKAD cycles
    ADCJ_SAMPLING_PERIOD_204_STATES = 0xCCU, ///< 204 CLKAD cycles
    ADCJ_SAMPLING_PERIOD_252_STATES = 0xFCU  ///< 252 CLKAD cycles
} adcj_sampling_period_t;

/** ADCJ sampling period select options */
typedef enum e_adcj_sampling_period_sel
{
    ADCJ_SAMPLING_PERIOD_SEL_DISABLE = 0U, ///< Use default sampling period (18 CLKAD cycles)
    ADCJ_SAMPLING_PERIOD_SEL_ENABLE  = 1U  ///< Use sampling period
} adcj_sampling_period_sel_t;

/** ADCJ buffer amp configuration options */
typedef enum e_adcj_buffer_amp
{
    ADCJ_BUFFER_AMP_ENABLE  = 0U,      ///< Enable input buffer amplifier
    ADCJ_BUFFER_AMP_DISABLE = 1U       ///< Disable input buffer amplifier
} adcj_buffer_amp_t;

/** ADCJ extended sampling period options */
typedef enum e_adcj_extended_sampling_period
{
    ADCJ_EXTENDED_SAMPLING_PERIOD_60_STATES  = 0x00U, ///< 60 CLKAD cycles
    ADCJ_EXTENDED_SAMPLING_PERIOD_102_STATES = 0x66U, ///< 102 CLKAD cycles
    ADCJ_EXTENDED_SAMPLING_PERIOD_138_STATES = 0x8AU, ///< 138 CLKAD cycles
    ADCJ_EXTENDED_SAMPLING_PERIOD_252_STATES = 0xFCU  ///< 252 CLKAD cycles
} adcj_extended_sampling_period_t;

/** ADCJ pin level self-diagnostic voltage level options */
typedef enum e_adcj_pin_lv_self_diag_vol
{
    ADCJ_PIN_LV_SELF_DIAG_VOL_EVEN_AVSS_ODD_AVCC      = 0x0U, ///< Even group pin to AVSS, odd group pin to AVCC
    ADCJ_PIN_LV_SELF_DIAG_VOL_EVEN_AVCC_ODD_AVSS      = 0x1U, ///< Even group pin to AVCC, odd group pin to AVSS
    ADCJ_PIN_LV_SELF_DIAG_VOL_EVEN_AVSS_ODD_HALF_AVCC = 0x2U, ///< Even group pin to AVSS, odd group pin to half AVSS
    ADCJ_PIN_LV_SELF_DIAG_VOL_EVEN_HALF_AVCC_ODD_VSS  = 0x3U  ///< Even group pin to half AVCC, odd group pin to AVSS
} adcj_pin_lv_self_diag_vol_t;

/** ADCJ T&H path self-diagnostic enable or disable */
typedef enum e_adcj_th_path_self_diag
{
    ADCJ_TH_PATH_SELF_DIAG_DISABLE = 0U, ///< Enable T&H path self-diagnostic
    ADCJ_TH_PATH_SELF_DIAG_ENABLE  = 1U, ///< Disable T&H path self-diagnostic
} adcj_th_path_self_diag_t;

/** ADCJ wring-break detection mode 1 pulse width options */
typedef enum e_adcj_wiring_break_detect_pulse_width
{
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_1_STATE   = 0x4U,  ///< 1 CLKAD cycle
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_2_STATES  = 0x5U,  ///< 2 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_3_STATES  = 0x6U,  ///< 3 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_4_STATES  = 0x7U,  ///< 4 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_5_STATES  = 0x8U,  ///< 5 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_6_STATES  = 0x9U,  ///< 6 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_7_STATES  = 0xAU,  ///< 7 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_8_STATES  = 0xBU,  ///< 8 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_9_STATES  = 0xCU,  ///< 9 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_10_STATES = 0xDU,  ///< 10 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_11_STATES = 0xEU,  ///< 11 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_12_STATES = 0xFU,  ///< 12 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_13_STATES = 0x10U, ///< 13 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_14_STATES = 0x11U, ///< 14 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_15_STATES = 0x12U, ///< 15 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_16_STATES = 0x13U, ///< 16 CLKAD cycles
    ADCJ_WIRING_BREAK_DETECT_PULSE_WIDTH_17_STATES = 0x14U, ///< 17 CLKAD cycles
} adcj_wiring_break_detect_pulse_width_t;

/** ADCJ wiring-break detection addition mode enable or disable */
typedef enum e_adcj_wiring_break_addition_mode
{
    ADCJ_WIRING_BREAK_ADDITION_MODE_DISABLE = 0U, ///< Disable wiring-break addition mode
    ADCJ_WIRING_BREAK_ADDITION_MODE_ENABLE  = 1U  ///< Enable wiring-break addition mode
} adcj_wiring_break_addition_mode_t;

/** ADCJ physical channel options for virtual channels general control */
typedef enum e_adcj_physical_channel
{
    ADCJ_PHYSICAL_CHANNEL_AN00 = (0x0U << 2U) | 0U, ///< Analog input AN00
    ADCJ_PHYSICAL_CHANNEL_AN01 = (0x0U << 2U) | 1U, ///< Analog input AN01
    ADCJ_PHYSICAL_CHANNEL_AN02 = (0x0U << 2U) | 2U, ///< Analog input AN02
    ADCJ_PHYSICAL_CHANNEL_AN03 = (0x0U << 2U) | 3U, ///< Analog input AN03
    ADCJ_PHYSICAL_CHANNEL_AN10 = (0x1U << 2U) | 0U, ///< Analog input AN10
    ADCJ_PHYSICAL_CHANNEL_AN11 = (0x1U << 2U) | 1U, ///< Analog input AN11
    ADCJ_PHYSICAL_CHANNEL_AN12 = (0x1U << 2U) | 2U, ///< Analog input AN12
    ADCJ_PHYSICAL_CHANNEL_AN13 = (0x1U << 2U) | 3U, ///< Analog input AN13
    ADCJ_PHYSICAL_CHANNEL_AN20 = (0x2U << 2U) | 0U, ///< Analog input AN20
    ADCJ_PHYSICAL_CHANNEL_AN21 = (0x2U << 2U) | 1U, ///< Analog input AN21
    ADCJ_PHYSICAL_CHANNEL_AN22 = (0x2U << 2U) | 2U, ///< Analog input AN22
    ADCJ_PHYSICAL_CHANNEL_AN23 = (0x2U << 2U) | 3U, ///< Analog input AN23
    ADCJ_PHYSICAL_CHANNEL_AN30 = (0x3U << 2U) | 0U, ///< Analog input AN30
    ADCJ_PHYSICAL_CHANNEL_AN31 = (0x3U << 2U) | 1U, ///< Analog input AN31
    ADCJ_PHYSICAL_CHANNEL_AN32 = (0x3U << 2U) | 2U, ///< Analog input AN32
    ADCJ_PHYSICAL_CHANNEL_AN33 = (0x3U << 2U) | 3U, ///< Analog input AN33
    ADCJ_PHYSICAL_CHANNEL_AN40 = (0x4U << 2U) | 0U, ///< Analog input AN40
    ADCJ_PHYSICAL_CHANNEL_AN41 = (0x4U << 2U) | 1U, ///< Analog input AN41
    ADCJ_PHYSICAL_CHANNEL_AN42 = (0x4U << 2U) | 2U, ///< Analog input AN42
    ADCJ_PHYSICAL_CHANNEL_AN43 = (0x4U << 2U) | 3U, ///< Analog input AN43
    ADCJ_PHYSICAL_CHANNEL_AN50 = (0x5U << 2U) | 0U, ///< Analog input AN50
    ADCJ_PHYSICAL_CHANNEL_AN51 = (0x5U << 2U) | 1U, ///< Analog input AN51
    ADCJ_PHYSICAL_CHANNEL_AN52 = (0x5U << 2U) | 2U, ///< Analog input AN52
    ADCJ_PHYSICAL_CHANNEL_AN53 = (0x5U << 2U) | 3U, ///< Analog input AN53
    ADCJ_PHYSICAL_CHANNEL_AN60 = (0x6U << 2U) | 0U, ///< Analog input AN60
    ADCJ_PHYSICAL_CHANNEL_AN61 = (0x6U << 2U) | 1U, ///< Analog input AN61
    ADCJ_PHYSICAL_CHANNEL_AN62 = (0x6U << 2U) | 2U, ///< Analog input AN62
    ADCJ_PHYSICAL_CHANNEL_AN63 = (0x6U << 2U) | 3U, ///< Analog input AN63
    ADCJ_PHYSICAL_CHANNEL_AN70 = (0x7U << 2U) | 0U, ///< Analog input AN70
    ADCJ_PHYSICAL_CHANNEL_AN71 = (0x7U << 2U) | 1U, ///< Analog input AN71
    ADCJ_PHYSICAL_CHANNEL_AN72 = (0x7U << 2U) | 2U, ///< Analog input AN72
    ADCJ_PHYSICAL_CHANNEL_AN73 = (0x7U << 2U) | 3U, ///< Analog input AN73
    ADCJ_PHYSICAL_CHANNEL_AN80 = (0x8U << 2U) | 0U, ///< Analog input AN80
    ADCJ_PHYSICAL_CHANNEL_AN81 = (0x8U << 2U) | 1U, ///< Analog input AN81
    ADCJ_PHYSICAL_CHANNEL_AN82 = (0x8U << 2U) | 2U, ///< Analog input AN82
    ADCJ_PHYSICAL_CHANNEL_AN83 = (0x8U << 2U) | 3U, ///< Analog input AN83
    ADCJ_PHYSICAL_CHANNEL_AN90 = (0x9U << 2U) | 0U, ///< Analog input AN90
    ADCJ_PHYSICAL_CHANNEL_AN91 = (0x9U << 2U) | 1U  ///< Analog input AN91
} adcj_physical_channel_t;

/** ADCJ T&H channel options for virtual channel general control */
typedef enum e_adcj_th_channel
{
    ADCJ_TH_CHANNEL_0 = 0x00U,         ///< T&H channel 0
    ADCJ_TH_CHANNEL_1 = 0x01U,         ///< T&H channel 1
    ADCJ_TH_CHANNEL_2 = 0x02U,         ///< T&H channel 2
    ADCJ_TH_CHANNEL_3 = 0x03U          ///< T&H channel 3
} adcj_th_channel_t;

/** ADCJ ADcore self-diagnostic voltage level options for virtual channel general control */
typedef enum e_adcj_ad_core_self_diag_voltage_lv
{
    ADCJ_AD_CORE_SELF_DIAG_VOLTAGE_LV_0_VREFH       = 0x00U, ///< ADcore self-diagnostic voltage 0 VREFH
    ADCJ_AD_CORE_SELF_DIAG_VOLTAGE_LV_1_DIV_4_VREFH = 0x04U, ///< ADcore self-diagnostic voltage 1/4 VREFH
    ADCJ_AD_CORE_SELF_DIAG_VOLTAGE_LV_1_DIV_2_VREFH = 0x08U, ///< ADcore self-diagnostic voltage 1/2 VREFH
    ADCJ_AD_CORE_SELF_DIAG_VOLTAGE_LV_3_DIV_4_VREFH = 0x0CU, ///< ADcore self-diagnostic voltage 3/4 VREFH
    ADCJ_AD_CORE_SELF_DIAG_VOLTAGE_LV_VREFH         = 0x10U  ///< ADcore self-diagnostic voltage VREFH
} adcj_ad_core_self_diag_voltage_lv_t;

/** ADCJ fixed A/D conversion data in A/D conversion data path diagnostic A/D conversion (ADVAL mode) for virtual channel general control */
typedef enum e_adcj_adval_data
{
    ADCJ_ADVAL_DATA_AAA = 0x00U,       ///< A/D conversion data is fixed to 0xAAA
    ADCJ_ADVAL_DATA_555 = 0x01U        ///< A/D conversion data is fixed to 0x555
} adcj_adval_data_t;

/** ADCJ virtual channel end interrupt enable or disable */
typedef enum e_adcj_vc_end_int
{
    ADCJ_VC_END_INT_DISABLE = 0U,      ///< Interrupt is not generated when virtual channel finished conversion
    ADCJ_VC_END_INT_ENABLE  = 1U       ///< Interrupt is generated when virtual channel finished conversion
} adcj_vc_end_int_t;

/** ADCJ virtual channel conversion type options */
typedef enum e_adcj_vc_conv_type
{
    ADCJ_VC_CONV_TYPE_NORMAL                                         = 0x0U, ///< Normal A/D conversion
    ADCJ_VC_CONV_TYPE_HOLD_VALUE                                     = 0x1U, ///< Hold value A/D conversion
    ADCJ_VC_CONV_TYPE_EXTENDED_SAMPLING_TIME                         = 0x2U, ///< Normal A/D conversion with extended sampling time
    ADCJ_VC_CONV_TYPE_AD_CORE_SELF_DIAG                              = 0x3U, ///< ADcore self-diagnostic conversion
    ADCJ_VC_CONV_TYPE_ADDITION_MODE                                  = 0x4U, ///< Addition mode A/D conversion
    ADCJ_VC_CONV_TYPE_MPX_NORMAL                                     = 0x5U, ///< MPX normal A/D conversion
    ADCJ_VC_CONV_TYPE_MPX_ADDITION_MODE                              = 0x6U, ///< MPX addition mode A/D conversion
    ADCJ_VC_CONV_TYPE_PIN_LEVEL_SELF_DIAGNOSTIC                      = 0x7U, ///< Pin level self-diagnostic A/D conversion
    ADCJ_VC_CONV_TYPE_WIRING_BREAK_DETECT_MODE_1                     = 0x8U, ///< Wiring-break detection mode 1 A/D conversion
    ADCJ_VC_CONV_TYPE_WIRING_BREAK_DETECT_MODE_2_PULL_DOWN           = 0x9U, ///< Wiring-break detection mode 2 pull down A/D conversion
    ADCJ_VC_CONV_TYPE_WIRING_BREAK_DETECT_MODE_2_PULL_UP             = 0xAU, ///< Wiring-break detection mode 2 pull up A/D conversion
    ADCJ_VC_CONV_TYPE_SELF_DIAG_WIRING_BREAK_DETECT_MODE_1           = 0xBU, ///< Self-diagnostic wiring-break detection mode 1 A/D conversion
    ADCJ_VC_CONV_TYPE_SELF_DIAG_WIRING_BREAK_DETECT_MODE_2_PULL_DOWN = 0xCU, ///< Self-diagnostic wiring-break detection mode 2 pull down A/D conversion
    ADCJ_VC_CONV_TYPE_SELF_DIAG_WIRING_BREAK_DETECT_MODE_2_PULL_UP   = 0xDU, ///< Self-diagnostic wiring-break detection mode 2 pull up A/D conversion
    ADCJ_VC_CONV_TYPE_AD_CONV_PATH_DIAG                              = 0xEU  ///< Data path diagnostic A/D conversion
} adcj_vc_conv_type_t;

/** ADCJ wait time table options */
typedef enum e_adcj_wait_time_table
{
    ADCJ_WAIT_TIME_TABLE_DISABLE = 0U, ///< Disable wait time
    ADCJ_WAIT_TIME_TABLE_0       = 1U, ///< Wait time table 0 is selected
    ADCJ_WAIT_TIME_TABLE_1       = 2U, ///< Wait time table 1 is selected
    ADCJ_WAIT_TIME_TABLE_2       = 3U, ///< Wait time table 2 is selected
    ADCJ_WAIT_TIME_TABLE_3       = 4U, ///< Wait time table 3 is selected
    ADCJ_WAIT_TIME_TABLE_4       = 5U, ///< Wait time table 4 is selected
    ADCJ_WAIT_TIME_TABLE_5       = 6U, ///< Wait time table 5 is selected
    ADCJ_WAIT_TIME_TABLE_6       = 7U, ///< Wait time table 6 is selected
    ADCJ_WAIT_TIME_TABLE_7       = 8U  ///< Wait time table 7 is selected
} adcj_wait_time_table_t;

/** ADCJ upper/lower limit table options */
typedef enum e_adcj_ul_limit_table
{
    ADCJ_UL_LIMIT_TABLE_DISABLE = 0U,  ///< Disable upper/lower limit check
    ADCJ_UL_LIMIT_TABLE_0       = 1U,  ///< Upper/Lower limit table 0 is selected
    ADCJ_UL_LIMIT_TABLE_1       = 2U,  ///< Upper/Lower limit table 1 is selected
    ADCJ_UL_LIMIT_TABLE_2       = 3U,  ///< Upper/Lower limit table 2 is selected
    ADCJ_UL_LIMIT_TABLE_3       = 4U,  ///< Upper/Lower limit table 3 is selected
    ADCJ_UL_LIMIT_TABLE_4       = 5U,  ///< Upper/Lower limit table 4 is selected
    ADCJ_UL_LIMIT_TABLE_5       = 6U,  ///< Upper/Lower limit table 5 is selected
    ADCJ_UL_LIMIT_TABLE_6       = 7U,  ///< Upper/Lower limit table 6 is selected
    ADCJ_UL_LIMIT_TABLE_7       = 8U   ///< Upper/Lower limit table 7 is selected
} adcj_ul_limit_table_t;

/** ADCJ MPX interrupt enable or disable */
typedef enum e_adcj_mpx_int
{
    ADCJ_MPX_INT_DISABLE = 0U,         ///< Enable MPX interrupt
    ADCJ_MPX_INT_ENABLE  = 1U          ///< Disable MPX interrupt
} adcj_mpx_int_t;

/** ADCJ enable or disable trigger overlap detection function */
typedef enum e_adcj_trigger_overlap_detect
{
    ADCJ_TRIGGER_OVERLAP_DETECT_DISABLE = 0U, ///< Disable trigger overlap detection function
    ADCJ_TRIGGER_OVERLAP_DETECT_ENABLE  = 1U  ///< Enable trigger overlap detection function
} adcj_trigger_overlap_detect_t;

/** ADCJ T&H group options */
typedef enum e_adcj_th_group
{
    ADCJ_TH_GROUP_A = 0U,              ///< T&H group A
    ADCJ_TH_GROUP_B = 1U               ///< T&H group B
} adcj_th_group_t;

/** ADCJ T&H unit enable or disable options */
typedef enum e_adcj_th_unit_cfg
{
    ADC_TH_UNIT_CFG_DISABLE = 0U,      ///< Disable T&H unit
    ADC_TH_UNIT_CFG_ENABLE  = 1U,      ///< Enable T&H unit
} adcj_th_unit_cfg_t;

/** ADCJ T&H group trigger method options */
typedef enum e_adcj_th_group_trigger_method
{
    ADCJ_TH_GROUP_TRIGGER_METHOD_SW = 0U, ///< T&H group is triggered by software input
    ADCJ_TH_GROUP_TRIGGER_METHOD_HW = 1U  ///< T&H group is triggered by hardware input
} adcj_th_group_trigger_method_t;

/** ADCJ AD timer trigger method options */
typedef enum st_adcj_ad_timer_trigger_method
{
    ADCJ_AD_TIMER_TRIGGER_METHOD_SW = 0U, ///< AD timer trigger by API call
    ADCJ_AD_TIMER_TRIGGER_METHOD_HW = 1U  ///< AD timer trigger by HW trigger
} adcj_ad_timer_trigger_method_t;

/** This enum contains triggers methods for scan groups (not all are available for all scan groups) */
typedef enum e_adcj_sg_trigger_method
{
    ADCJ_SG_TRIGGER_METHOD_SW       = 0U, ///< Scan group is trigger by software API call
    ADCJ_SG_TRIGGER_METHOD_HW       = 1U, ///< Scan group is trigger by hardware input
    ADCJ_SG_TRIGGER_METHOD_AD_TIMER = 2U, ///< Scan group is triggered by AD timer (apply with scan group 3, 4)
    ADCJ_SG_TRIGGER_METHOD_THA      = 3U, ///< Scan group is triggered by T&H group A hold trigger (apply with scan group 1 to 4)
    ADCJ_SG_TRIGGER_METHOD_THB      = 4U, ///< Scan group is triggered by T&H group B hold trigger (apply with scan group 1 to 4)
} adcj_sg_trigger_method_t;

/** Scan group end interrupt enable or disable options */
typedef enum e_adcj_sg_end_int
{
    ADCJ_SG_END_INT_DISABLE = 0U,      ///< Scan end interrupt is disable
    ADCJ_SG_END_INT_ENABLE  = 1U       ///< Scan end interrupt is enable
} adcj_sg_end_int_t;

/** This enum contains scan mode available for scan groups */
typedef enum e_adcj_sg_scan_mode
{
    ADCJ_SG_SCAN_MODE_MULTICYLE  = 0U, ///< Multi-cycle scan mode is chosen for scan group
    ADCJ_SG_SCAN_MODE_CONTINUOUS = 1U  ///< Continuous scan mode is chosen for scan group
} adcj_sg_scan_mode_t;

/** Enable or disable scan group 4 as PWM-Diag */
typedef enum e_adcj_pwm_diag_cfg
{
    ADCJ_PWM_DIAG_CFG_DISABLE = 0U,    ///< Not using SG4 as PWM-Diag
    ADCJ_PWM_DIAG_CFG_ENABLE  = 1U     ///< Using SG4 as PWM-Diag
} adcj_pwm_diag_cfg_t;

/** Trigger source select option for scan group of ADCJ0 */
typedef enum e_adcj_adcj0_trigger_src
{
    ADCJ_ADCJ0_TRIGGER_SRC_DISABLE = 0U,         ///< Disable hardware trigger for scan group x of ADCJ0
    ADCJ_ADCJ0_TRIGGER_SRC_LPS     = 0x1U << 5U, ///< Select LPS as hardware trigger source for scan group x of ADCJ0
    ADCJ_ADCJ0_TRIGGER_SRC_PIC2    = 0x1U << 6U, ///< Select PIC2 as hardware trigger source for scan group x of ADCJ0
} adcj_adcj0_trigger_src_t;

/** The status of scan group in ADCJ */
typedef enum e_adcj_sg_status
{
    ADCJ_SG_STATUS_IDLE   = 0U,        ///< Scan group is in idle state
    ADCJ_SG_STATUS_ACTIVE = 1U,        ///< Scan group is in active state
} adcj_sg_status_t;

/** Enable or disable Temperature Measurement */
typedef enum e_adcj_tsn0_measure_cfg
{
    ADCJ_TSN0_MEASURE_CFG_DISABLE   = 0U, ///< Not using temperature measurement
    ADCJ_TSN0_MEASURE_CFG_ENABLE    = 1U, ///< Using temperature measurement
    ADCJ_TSN0_MEASURE_CFG_SELF_DIAG = 2U, ///< Using temperature measurement self-diag
} adcj_tsn0_measure_cfg_t;

/** The states of T&H group A and T&H group B */
typedef enum e_adcj_th_state
{
    ADCJ_TH_STATE_SAMPLING = 0U,       ///< T&H group is in sampling or hold state
    ADCJ_TH_STATE_STOP     = 0U,       ///< T&H group is stopped
} adcj_th_state_t;

/** Normal scan group x in ADCJ */
typedef enum e_adcj_scan_group
{
    ADCJ_SCAN_GROUP_SG0 = 0U,          ///< Scan group 0
    ADCJ_SCAN_GROUP_SG1 = 1U,          ///< Scan group 1
    ADCJ_SCAN_GROUP_SG2 = 2U,          ///< Scan group 2
    ADCJ_SCAN_GROUP_SG3 = 3U,          ///< Scan group 3
    ADCJ_SCAN_GROUP_SG4 = 4U,          ///< Scan group 4
} adcj_scan_group_t;

/** ADCJ data format and addition count configuration */
typedef union u_adcj_dfmt_addnt
{
    uint8_t adcr2;                     ///< Value to be written to ADCJnADCR2 register

    /** ADCJ data format and addition count configuration */
    struct
    {
        adcj_addition_count_t addition_count : 1; ///< Addition count
        uint8_t                              : 3; ///< The value of this field should be 0
        adcj_data_format_t data_format       : 3; ///< Data format
    } adcr2_b;
} adcj_dfmt_addnt_t;

/** ADCJ safety control (error interrupts enable, read and clear feature) */
typedef union u_adcj_safety_cfg
{
    uint8_t sftcr;                     ///< Value to be written to ADCJnSFTCR register

    /** ADCJ safety configurations */
    struct
    {
        adcj_id_error_int_t        id              : 1; ///< ID error output configuration
        adcj_parity_error_int_t    parity          : 1; ///< Parity error output configuration
        adcj_overwrite_error_int_t overwrite       : 1; ///< Overwrite error output configuration
        uint8_t                                    : 1; ///< The value of this field should be 0
        adcj_clear_after_read_t read_clear         : 1; ///< Enable/Disable clear after read function
        uint8_t                                    : 1; ///< The value of this field should be 0
        adcj_trigger_overlap_int_t trigger_overlap : 1; ///< Trigger overlap error output configuration
    } sftcr_b;
} adcj_safety_cfg_t;

/** ADCJ upper/lower limit table format */
typedef union u_adcj_ul_limit_tb
{
    uint32_t vcullmtbr;                ///< Value to be written to ADCJnCULLMTBR register

    /** ADCJ upper/lower limit configurations */
    struct
    {
        uint16_t vcllmtb;              ///< Lower limit value
        uint16_t vculmtb;              ///< Upper limit value
    } vcullmtbr_b;
} adcj_ul_limit_tb_t;

/** ADCJ upper/lower limit control */
typedef struct st_adcj_ul_limit_cfg
{
    adcj_ul_limit_tb_t           ul_limit_tb[ADCJ_UPPER_LOWER_LIMIT_TABLE_ENTRIES]; ///< Upper lower limit table configuration data
    uint32_t                     vc_ul_limit_int_0;                                 ///< Enable/Disable upper/lower limit interrupt for virtual channel
    uint32_t                     vc_ul_limit_int_1;                                 ///< Enable/Disable upper/lower limit interrupt for virtual channel
    adcj_pwm_diag_ul_limit_int_t pwm_diag_ul_limit_int;                             ///< Enable/Disable upper/lower limit interrupt for PWM-Diag
} adcj_ul_limit_cfg_t;

/** ADCJ sampling configuration and enable/disable input buffer */
typedef union u_adcj_sampling_cfg
{
    uint32_t smpcr;                    ///< Value to be written to ADCJnSMPCR register

    /** ADCJ sampling and input buffer configurations */
    struct
    {
        adcj_sampling_period_t period                   : 8; ///< Sampling period setting
        uint32_t                                        : 4; ///< The value of this field should be 0
        adcj_sampling_period_sel_t period_sel           : 1; ///< Sampling period select setting
        uint32_t                                        : 2; ///< The value of this field should be 0
        adcj_buffer_amp_t               buffer_amp      : 1; ///< Enable/Disable input buffer amplifier setting
        adcj_extended_sampling_period_t extended_period : 8; ///< Extended sampling period setting
    } smpcr_b;
} adcj_sampling_cfg_t;

/** ADCJ pin level self-diagnostic control */
typedef union u_adcj_pin_lv_self_diag_cfg
{
    uint8_t tdcr;                      ///< Value to be written to ADCJnTDCR register

    /** ADCJ pin level self-diagnostic configurations */
    struct
    {
        adcj_pin_lv_self_diag_vol_t voltage_level  : 2; ///< Pin level self-diagnostic voltage level setting
        uint8_t                                    : 5; ///< The value of this field should be 0
        adcj_th_path_self_diag_t th_path_self_diag : 1; ///< T&H path self-diagnostic enable/disable setting
    } tdcr_b;
} adcj_pin_lv_self_diag_cfg_t;

/** ADCJ wiring-detection control */
typedef union u_adcj_wiring_break_detect_cfg
{
    uint16_t odcr;                     ///< Value to be written to ADCJn ODCR register

    /** ADCJ wiring-break detect configurations */
    struct
    {
        adcj_wiring_break_detect_pulse_width_t pulse_width : 5; ///< Wiring-break detection mode 1 pulse with setting
        uint8_t                                            : 3; ///< The value of this field should be 0
        adcj_wiring_break_addition_mode_t addition_mode    : 1; ///< Addition mode wiring-break detection enable/disable setting
    } odcr_b;
} adcj_wiring_break_detect_cfg_t;

/* ADCJ MPX settings */
typedef struct st_adcj_mpx_cfg_t
{
    uint8_t        mask_control_format;     ///< Mask control format setting for using MPX in I/O port mode
    uint8_t        spi_command_information; ///< SPI communication information for using MPX in SPI mode
    adcj_mpx_int_t mpx_int;                 ///< MPX interrupt enable/disable setting
} adcj_mpx_cfg_t;

/** ADCJ A/D conversion timing monitor target virtual channel configuration */
typedef struct st_adcj_ad_conv_timing_monitor
{
    uint8_t target_vc[ADCJ_NUMBER_OF_AD_CONVERSION_TIMING_MONITOR]; ///< Target virtual channel for A/D conversion timing monitor
} adcj_ad_conv_timing_monitor_t;

/** ADCJ T&H group select configurations */
typedef union u_adcj_th_group_cfg
{
    uint16_t thgsr;                    ///< Value to be written to ADCJnTHGSR register

    /** ADCJ group selection configurations for track and hold units */
    struct
    {
        adcj_th_group_t th0_group_cfg : 1; ///< T&H group selection setting for T&H unit 0
        uint16_t                      : 1; ///< The value of this field should be 0
        adcj_th_group_t th1_group_cfg : 1; ///< T&H group selection setting for T&H unit 1
        uint16_t                      : 1; ///< The value of this field should be 0
        adcj_th_group_t th2_group_cfg : 1; ///< T&H group selection setting for T&H unit 2
        uint16_t                      : 1; ///< The value of this field should be 0
        adcj_th_group_t th3_group_cfg : 1; ///< T&H group selection setting for T&H unit 3
    } thgsr_b;
} adcj_th_group_cfg_t;

/** ADCJ T&H enable configurations */
typedef union u_adcj_th_enable_cfg
{
    uint8_t ther;                      ///< Value to be written to ADCJnTHER register

    /** ADCJ track and hold units enable configurations */
    struct
    {
        adcj_th_unit_cfg_t th0_enable : 1; ///< T&H unit 0 enable/disable setting
        adcj_th_unit_cfg_t th1_enable : 1; ///< T&H unit 1 enable/disable setting
        adcj_th_unit_cfg_t th2_enable : 1; ///< T&H unit 2 enable/disable setting
        adcj_th_unit_cfg_t th3_enable : 1; ///< T&H unit 3 enable/disable setting
    } ther_b;
} adcj_th_enable_cfg_t;

/** ADCJ T&H settings */
typedef struct st_adcj_th_cfg
{
    adcj_th_group_cfg_t            group_select_cfg;                        ///< T&H group selection configurations
    adcj_th_enable_cfg_t           th_enable_cfg;                           ///< T&H units enable/disable settings
    adcj_th_group_trigger_method_t trigger_method[ADCJ_NUMBER_OF_TH_GROUP]; ///< T&H group trigger method
    uint32_t channel_select[ADCJ_NUMBER_OF_TH_UNIT];                        ///< T&H group channel mapping
} adcj_th_cfg_t;

/** ADCJ AD timers setting */
typedef struct st_adcj_ad_timer_cfg
{
    adcj_ad_timer_trigger_method_t trigger_method[ADCJ_NUMBER_OF_AD_TIMER]; ///< Trigger method for AD timer
    uint32_t initial_phase[ADCJ_NUMBER_OF_AD_TIMER];                        ///< AD timer initial phase setting
    uint32_t cycle[ADCJ_NUMBER_OF_AD_TIMER];                                ///< AD timer cycle time setting
} adcj_ad_timer_cfg_t;

/** ADCJ wait settings table */
typedef struct st_adcj_wait_setting
{
    uint16_t wait_setting[ADCJ_WAIT_SETTING_TABLE_ENTRIES]; ///< Wait time setting for wait time table
} adcj_wait_setting_t;

/** ADCJ interrupts configuration */
typedef struct st_adcj_int_cfg
{
    IRQn_Type error_irq;               ///< ADCJ error interrupt IRQ number
    uint8_t   error_ipl;               ///< ADCJ error interrupt priority level
    IRQn_Type scan_group_0_end_irq;    ///< ADCJ group 0 scan end interrupt IRQ number
    uint8_t   scan_group_0_end_ipl;    ///< ADCJ group 0 scan end interrupt priority number
    IRQn_Type scan_group_1_end_irq;    ///< ADCJ group 1 scan end interrupt IRQ number
    uint8_t   scan_group_1_end_ipl;    ///< ADCJ group 1 scan end interrupt priority number
    IRQn_Type scan_group_2_end_irq;    ///< ADCJ group 2 scan end interrupt IRQ number
    uint8_t   scan_group_2_end_ipl;    ///< ADCJ group 2 scan end interrupt priority number
    IRQn_Type scan_group_3_end_irq;    ///< ADCJ group 3 scan end interrupt IRQ number
    uint8_t   scan_group_3_end_ipl;    ///< ADCJ group 3 scan end interrupt priority number
    IRQn_Type scan_group_4_end_irq;    ///< ADCJ group 4 scan end interrupt IRQ number
    uint8_t   scan_group_4_end_ipl;    ///< ADCJ group 4 scan end interrupt priority number
} adcj_int_cfg_t;

/** ADCJ virtual channel configuration */
typedef union u_adc_vc_cfg
{
    uint32_t vcr;                      ///< Value to be written to ADCJnVCR register

    /** ADCJ virtual channel configuration */
    struct
    {
        uint32_t general_ctrl                : 6; ///< General control
        uint32_t                             : 1; ///< The value of this field should be 0
        adcj_vc_end_int_t   channel_end_int  : 1; ///< Enable/Disable channel end interrupt
        uint32_t            mpx_value        : 3; ///< MPX value used in conversions using MPX
        adcj_vc_conv_type_t conv_type        : 4; ///< Conversion type
        uint32_t                             : 9; ///< The value of this field should be 0
        adcj_wait_time_table_t wait_time_sel : 4; ///< Wait time table setting
        adcj_ul_limit_table_t  ul_limit_sel  : 4; ///< Upper/Lower limit check table
    } vcr_b;
} adcj_vc_cfg_t;

typedef union u_adcj_sg_ctrl
{
    uint8_t sgcr;                      ///< This value will be written to ADCJnSGCR

    /** ADCJ scan group scan end interrupt enable and scan mode configurations */
    struct
    {
        uint8_t                       : 4; ///< The value of this field should be 0
        adcj_sg_end_int_t   end_int   : 1; ///< Enable/Disable scan end interrupt
        adcj_sg_scan_mode_t scan_mode : 1; ///< Scan mode of scan group
    } sgcr_b;
} adcj_sg_ctrl_t;

/** This structure contains generic configurations of scan group 0 to 4 */
typedef struct st_adcj_sg_generic_cfg
{
    adcj_adcj0_trigger_src_t adcj_trigger_src; ///< ADCJ0 hardware trigger source select for scan group x
    adcj_sg_trigger_method_t trigger_method;   ///< Scan group trigger method select
    adcj_sg_ctrl_t           sg_ctrl;          ///< Store scan group control data
    uint8_t               scan_cycle;          ///< Number of scan cycle of scan group in multi-cycle scan mode
    uint8_t               start_vc_ptr;        ///< Start virtual channel pointer of scan group
    uint8_t               end_vc_ptr;          ///< End virtual channel pointer of scan group
    adcj_vc_cfg_t const * p_vc_cfg;            ///< Pointer to virtual channel configurations of scan group x
} adcj_sg_generic_cfg_t;

typedef struct st_adcj_sg_cfg
{
    adcj_sg_generic_cfg_t const * p_sg_cfg[ADCJ_NUMBER_OF_SG]; ///< Scan group 0 to 4 configurations
    adcj_pwm_diag_cfg_t           pwm_diag_cfg;                ///< PWM-Diag enable/disable setting
    adcj_tsn0_measure_cfg_t       tsn0_measure_cfg;            ///< TSN0-Measure enable/disable setting
} adcj_sg_cfg_t;

/** Extended configuration structure for ADCJ. This structure contains ADCJ specific configurations */
typedef struct st_adcj_extended_cfg
{
    adcj_suspend_method_t          suspend_method;             ///< Suspend method setting
    adcj_dfmt_addnt_t              data_format_addition_count; ///< Data format and addition count settings
    adcj_safety_cfg_t              safety_cfg;                 ///< Safety setting (error interrupts enable, clear after read function setting)
    adcj_ul_limit_cfg_t            ul_limit_cfg;               ///< Upper/lower limit error check configurations
    adcj_sampling_cfg_t            sampling_cfg;               ///< Sampling time settings
    adcj_pin_lv_self_diag_cfg_t    pin_lv_self_diag_cfg;       ///< Pin level self-diagnostic settings
    adcj_wiring_break_detect_cfg_t wiring_break_detect_cfg;    ///< Wiring-break detection settings
    adcj_mpx_cfg_t                 mpx_cfg;                    ///< MPX settings
    adcj_trigger_overlap_detect_t  trigger_overlap_detect;     ///< Trigger overlap detection function enable/disable settings
    adcj_ad_conv_timing_monitor_t  ad_conv_timing_monitor;     ///< AD conversion timing monitor target virtual channel settings
    adcj_th_cfg_t       th_cfg;                                ///< T&H settings
    adcj_ad_timer_cfg_t ad_timer_cfg;                          ///< AD timer settings for SG3 and SG4
    adcj_wait_setting_t wait_setting;                          ///< Wait time table settings
    adcj_int_cfg_t      int_cfg;                               ///< ADCJ interrupts enable/disable settings
} adcj_extended_cfg_t;

/** ADCJ instance control block. DO NOT INITIALIZE.  Initialized in @ref adc_api_t::open() */
typedef struct st_adcj_instance_ctrl
{
    adc_cfg_t const     * p_cfg;                             ///< Pointer to ADCJ generic configuration
    adcj_sg_cfg_t const * p_sg_cfg;                          ///< Pointer to ADCJ scan groups configuration
    R_ADCJ1_Type        * p_reg;                             ///< Pointer to ADCJ unit register base address
    R_ADCJ2_SELB_Type   * p_selb_reg;                        ///< Pointer to ADCJ channel select hardware source trigger registers
    void (* p_callback)(adc_callback_args_t *);              ///< Pointer to callback that is called when an adcj_event_t occurs
    adc_callback_args_t * p_callback_memory;                 ///< Pointer to non-secure memory that can be used to pass arguments to a callback in non-secure memory
    void                * p_context;                         ///< User defined context passed into callback function
    uint32_t              opened;                            ///< Open status of ADCJ
    uint32_t              initialized;                       ///< Initialized status of ADCJ
    adcj_th_state_t       th_state[ADCJ_NUMBER_OF_TH_GROUP]; ///< Store current state of T&H groups
} adcj_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this instance. */
extern const adc_api_t g_adc_on_adcj;

/** @endcond */

fsp_err_t R_ADCJ_Open(adc_ctrl_t * const p_ctrl, adc_cfg_t const * const p_cfg);
fsp_err_t R_ADCJ_Close(adc_ctrl_t * const p_ctrl);
fsp_err_t R_ADCJ_ScanCfg(adc_ctrl_t * const p_ctrl, void const * const p_scan_group_cfg);
fsp_err_t R_ADCJ_ScanStart(adc_ctrl_t * const p_ctrl);
fsp_err_t R_ADCJ_ScanStop(adc_ctrl_t * const p_ctrl);
fsp_err_t R_ADCJ_ScanGroupStart(adc_ctrl_t * p_ctrl, adc_group_mask_t group_mask);
fsp_err_t R_ADCJ_ScanGroupStop(adc_ctrl_t * p_ctrl, adc_group_mask_t group_mask);
fsp_err_t R_ADCJ_ScanStatusGet(adc_ctrl_t * const p_ctrl, adc_status_t * p_status);
fsp_err_t R_ADCJ_ScanGroupStatusGet(adc_ctrl_t * const       p_ctrl,
                                    adc_group_mask_t const   group_mask,
                                    adcj_sg_status_t * const p_status);
fsp_err_t R_ADCJ_Read(adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, uint16_t * const p_data);
fsp_err_t R_ADCJ_Read32(adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, uint32_t * const p_data);
fsp_err_t R_ADCJ_PWMDiagRead(adc_ctrl_t * const p_ctrl, uint16_t * const p_data);
fsp_err_t R_ADCJ_SGDiagRead(adc_ctrl_t * const p_ctrl, uint16_t * const p_data);
fsp_err_t R_ADCJ_CallbackSet(adc_ctrl_t * const          p_ctrl,
                             void (                    * p_callback)(adc_callback_args_t *),
                             void * const                p_context,
                             adc_callback_args_t * const p_callback_memory);
fsp_err_t R_ADCJ_InfoGet(adc_ctrl_t * const p_ctrl, adc_info_t * const p_adc_info);
fsp_err_t R_ADCJ_Calibrate(adc_ctrl_t * const p_ctrl, void const * p_extend);
fsp_err_t R_ADCJ_OffsetSet(adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, int32_t const offset);
fsp_err_t R_ADCJ_THEnable(adc_ctrl_t * const p_ctrl);
fsp_err_t R_ADCJ_PinLVSelfDiagVoltageChange(adc_ctrl_t * const p_ctrl, const adcj_pin_lv_self_diag_vol_t voltage_level);

/*******************************************************************************************************************//**
 * @} (end addtogroup ADCJ)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_ADCJ_H */
