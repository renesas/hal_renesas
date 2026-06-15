/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_TAUD_H
#define R_TAUD_H

/*******************************************************************************************************************//**
 * @addtogroup TAUD
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_taud_cfg.h"
#include "r_timer_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
extern const timer_api_t g_timer_on_taud;

/* Note for the Synchronous Operation:
 *   + Even though there are 16 channels in each TAUD Unit,
 *     there always has to be one Slave Channel.
 *
 *   + Only even channels (channel 0, channel 2, channel 4, ...) can be set as Master Channels.
 *     Any channel apart from channel 0 can be set as a Slave Channel.
 *
 *   + Only channels lower than the Master Channel can be set as Slave Channels,
 *     and multiple Slave Channels can be set for one Master Channel.
 *     Example: If channel 2 is a Master Channel, channel 3 and the lower channels (channel 4, channel 5, ...)
 *              can be set as Slave Channels.
 *
 *   + If multiple Master Channels are used, Slave Channels cannot cross the Master Channels.
 *     Example: If channel 0 and channel 4 are Master Channels,
 *              channel 1 to channel 3 can be set as Slave Channels for channel 0, but channel 5 to channel 15 cannot.
 */
#define TAUD_MAX_NUM_SLAVE_CHANNELS    (15U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Operation Clock. */
typedef enum e_taud_clock
{
    TAUD_CLOCK_CK0 = 0,                ///< CKO clock.
    TAUD_CLOCK_CK1 = 1,                ///< CK1 clock.
    TAUD_CLOCK_CK2 = 2,                ///< CK2 clock.
    TAUD_CLOCK_CK3 = 3,                ///< CK3 clock.
} taud_clock_t;

/** Count Clock. */
typedef enum e_taud_count_clock
{
    TAUD_COUNT_CLOCK_OPERATION_CLOCK  = 0, ////< Operation clock specified by TAUDnCMORm.TAUDnCKS[1:0].
    TAUD_COUNT_CLOCK_VALID_EDGE       = 1, ////< Valid edge of TAUDTTINm input signal.
    TAUD_COUNT_CLOCK_MASTER_INTERRUPT = 3, ////< INTTAUDnIm signal of master channel.
} taud_count_clock_t;

/** Clock source divisors. */
typedef enum e_taud_div
{
    TAUD_PCLK_DIVBY_2_POWOF_0  = 0,    ///< PCLK divided by 2^0.
    TAUD_PCLK_DIVBY_2_POWOF_1  = 1,    ///< PCLK divided by 2^1.
    TAUD_PCLK_DIVBY_2_POWOF_2  = 2,    ///< PCLK divided by 2^2.
    TAUD_PCLK_DIVBY_2_POWOF_3  = 3,    ///< PCLK divided by 2^3.
    TAUD_PCLK_DIVBY_2_POWOF_4  = 4,    ///< PCLK divided by 2^4.
    TAUD_PCLK_DIVBY_2_POWOF_5  = 5,    ///< PCLK divided by 2^5.
    TAUD_PCLK_DIVBY_2_POWOF_6  = 6,    ///< PCLK divided by 2^6.
    TAUD_PCLK_DIVBY_2_POWOF_7  = 7,    ///< PCLK divided by 2^7.
    TAUD_PCLK_DIVBY_2_POWOF_8  = 8,    ///< PCLK divided by 2^8.
    TAUD_PCLK_DIVBY_2_POWOF_9  = 9,    ///< PCLK divided by 2^9.
    TAUD_PCLK_DIVBY_2_POWOF_10 = 10,   ///< PCLK divided by 2^10.
    TAUD_PCLK_DIVBY_2_POWOF_11 = 11,   ///< PCLK divided by 2^11.
    TAUD_PCLK_DIVBY_2_POWOF_12 = 12,   ///< PCLK divided by 2^12.
    TAUD_PCLK_DIVBY_2_POWOF_13 = 13,   ///< PCLK divided by 2^13.
    TAUD_PCLK_DIVBY_2_POWOF_14 = 14,   ///< PCLK divided by 2^14.
    TAUD_PCLK_DIVBY_2_POWOF_15 = 15,   ///< PCLK divided by 2^15.
} taud_div_t;

/** Specifies the output logic. */
typedef enum e_taud_output_logic
{
    TAUD_OUTPUT_LOGIC_POSITIVE = 0,    ///< Positive logic.
    TAUD_OUTPUT_LOGIC_NEGATIVE = 1,    ///< Negative logic.
    TAUD_OUTPUT_LOGIC_DISABLED = 2,    ///< Disabled in toggle mode.
} taud_output_logic_t;

/** Specifies the dead time operation. */
typedef enum e_taud_deadtime_operation
{
    TAUD_DEADTIME_OPERATION_DISABLED = 0, ///< Disables dead time operation.
    TAUD_DEADTIME_OPERATION_ENABLED  = 1, ///< Enables dead time operation.
} taud_deadtime_operation_t;

/** Specifies the phase in which dead time is added. */
typedef enum e_taud_add_deadtime
{
    TAUD_ADD_DEADTIME_UNUSED        = 0, ///< Unused.
    TAUD_ADD_DEADTIME_NORMAL_PHASE  = 0, ///< Adds dead time to normal phase.
    TAUD_ADD_DEADTIME_REVERSE_PHASE = 1, ///< Adds dead time to reverse phase.
} taud_add_deadtime_t;

/** Channel type. */
typedef enum e_taud_channel_type
{
    TAUD_CHANNEL_TYPE_SLAVE       = 0, ///< Slave channel.
    TAUD_CHANNEL_TYPE_MASTER      = 1, ///< Master channel.
    TAUD_CHANNEL_TYPE_INDEPENDENT = 2, ///< Independent channel.
    TAUD_CHANNEL_TYPE_SLAVE_EVEN  = 3, ///< Slave even channel.
    TAUD_CHANNEL_TYPE_SLAVE_ODD   = 4, ///< Slave odd channel.
} taud_channel_type_t;

/** External start trigger type. */
typedef enum e_taud_trigger
{
    TAUD_TRIGGER_SOFTWARE              = 0, ///< Software trigger.
    TAUD_TRIGGER_VALID_EDGE            = 1, ///< Valid edge of TAUDnTTINm input signal.
    TAUD_TRIGGER_START_STOP_VALID_EDGE = 2, ///< Valid edge used as a start/stop trigger.
    TAUD_TRIGGER_SIMULTANEOUS_REWRITE  = 3, ///< Trigger simultaneous rewrite.
    TAUD_TRIGGER_MASTER_INT            = 4, ///< INTTAUDnIm of master channel.
    TAUD_TRIGGER_UPPER_INT             = 5, ///< INTTAUDnIm of upper channel (m - 1).
    TAUD_TRIGGER_DEAD_TIME_OUTPUT      = 6, ///< Dead time output signal.
    TAUD_TRIGGER_UP_DOWN_OUTPUT        = 7, ///< Up/down output trigger signal.
} taud_trigger_t;

/** Specifies the timing for updating capture register and overflow flag. */
typedef enum e_taud_overflow
{
    TAUD_OVERFLOW_UNUSED                  = 0, ///< Not used.
    TAUD_OVERFLOW_AUTO_CLEAR              = 0, ///< TAUDnOVF is set by TAUDnTTINm valid edge.
    TAUD_OVERFLOW_MANUAL_CLEAR            = 1, ///< TAUDnOVF is set by TAUDnCLOV.
    TAUD_OVERFLOW_COUNT_STOP              = 2, ///< Counter update, TAUDnOVF is not set.
    TAUD_OVERFLOW_COUNT_STOP_MANUAL_CLEAR = 3, ///< Counter update, TAUDnOVF is set by TAUDnCLOV.
} taud_overflow_t;

/** Specifies an operating mode. */
typedef enum e_taud_mode
{
    TAUD_MODE_INTERVAL_TIMER_MODE         = (0x0000U), ///< Interval timer mode.
    TAUD_MODE_JUDGE_MODE                  = (0x0002U), ///< Judge mode.
    TAUD_MODE_CAPTURE_MODE                = (0x0004U), ///< Capture mode.
    TAUD_MODE_EVENT_COUNT_MODE            = (0x0006U), ///< Event count mode.
    TAUD_MODE_ONE_COUNT_MODE              = (0x0008U), ///< One-count mode.
    TAUD_MODE_CAPTURE_ONE_COUNT_MODE      = (0x000CU), ///< Capture and one-count mode.
    TAUD_MODE_JUDGE_ONE_COUNT_MODE        = (0x000EU), ///< Judge and one-count mode.
    TAUD_MODE_COUNT_UP_DOWN_MODE          = (0x0012U), ///< Count-up/-down mode.
    TAUD_MODE_PULSE_ONECOUNT_MODE         = (0x0014U), ///< Pulse one-count mode.
    TAUD_MODE_COUNT_CAPTURE_MODE          = (0x0016U), ///< Count capture mode.
    TAUD_MODE_GATE_COUNT_MODE             = (0x0018U), ///< Gate count mode.
    TAUD_MODE_CAPTURE_AND_GATE_COUNT_MODE = (0x001AU)  ///< Capture and gate count mode.
} taud_mode_t;

/** Specifies TAUD function. */
typedef enum e_taud_function
{
    TAUD_FUNCTION_INTERVAL_TIMER                                      = (0U),  ///< Interval Timer Function.
    TAUD_FUNCTION_INPUT_INTERVAL_TIMER                                = (1U),  ///< Input Interval Timer Function.
    TAUD_FUNCTION_CLOCK_DIVIDE                                        = (2U),  ///< Clock Divide Function.
    TAUD_FUNCTION_ONE_PULSE_OUTPUT                                    = (3U),  ///< One-Pulse Output Function.
    TAUD_FUNCTION_ONE_SHOT_PULSE_OUTPUT                               = (4U),  ///< One-Shot Pulse Output Function.
    TAUD_FUNCTION_PWM_OUTPUT                                          = (5U),  ///< PWM Output Function.
    TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT_DEADTIME                        = (6U),  ///< Triangle PWM Output Function with Dead Time.
    TAUD_FUNCTION_DELAY_PULSE_OUTPUT                                  = (7U),  ///< Delay Pulse Output Function.
    TAUD_FUNCTION_EXTERNAL_EVENT_COUNT                                = (8U),  ///< External Event Count Function.
    TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_MEASUREMENT                      = (9U),  ///< Input Signal Width Measurement Function.
    TAUD_FUNCTION_INPUT_PULSE_INTERVAL_MEASUREMENT                    = (10U), ///< Input Pulse Interval Measurement Function.
    TAUD_FUNCTION_INPUT_PERIOD_COUNT_DETECTION                        = (11U), ///< Input Period Count Detection Function.
    TAUD_FUNCTION_DELAY_COUNT                                         = (12U), ///< Delay Count Function.
    TAUD_FUNCTION_TRIANGLE_PWM_OUTPUT                                 = (13U), ///< Triangle PWM Output Function.
    TAUD_FUNCTION_INTERRUPT_REQUEST_SIGNAL_CULLING                    = (14U), ///< Interrupt Request Signals Culling Function.
    TAUD_FUNCTION_TRIGGER_START_PWM_OUTPUT                            = (15U), ///< Trigger Start PWM Output Function.
    TAUD_FUNCTION_ONE_PHASE_PWM_OUTPUT                                = (16U), ///< One-Phase PWM Output Function.
    TAUD_FUNCTION_OFFSET_TRIGGER_OUTPUT                               = (17U), ///< Offset Trigger Output Function.
    TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_1                             = (18U), ///< Real-Time Output Function Type 1.
    TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_1      = (19U), ///< Simultaneous Rewrite Trigger Generation Function Type 1.
    TAUD_FUNCTION_SIMULTANEOUS_REWRITE_TRIGGER_GENERATION_TYPE_2      = (20U), ///< Simultaneous Rewrite Trigger Generation Function Type 2.
    TAUD_FUNCTION_OVERFLOW_INTERRUPT_OUTPUT_TYPE_1                    = (21U), ///< Overflow Interrupt Output Function (during TAUDnTTINm Width Measurement).
    TAUD_FUNCTION_OVERFLOW_INTERRUPT_OUTPUT_TYPE_2                    = (22U), ///< Overflow Interrupt Output Function (during TAUDnTTINm Input Period Count Detection).
    TAUD_FUNCTION_INPUT_POSITION_DETECTION                            = (23U), ///< Input Position Detection Function.
    TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_2 = (24U), ///< Non-Complementary Modulation Output Function Type 2.
    TAUD_FUNCTION_REAL_TIME_OUTPUT_TYPE_2                             = (25U), ///< Real-Time Output Function Type 2.
    TAUD_FUNCTION_INPUT_PULSE_INTERVAL_JUDGMENT                       = (26U), ///< Input Pulse Interval Judgment Function.
    TAUD_FUNCTION_INPUT_SIGNAL_WIDTH_JUDGMENT                         = (27U), ///< Input Signal Width Judgment Function.
    TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_1                        = (28U), ///< A/D Conversion Trigger Output Function Type 1.
    TAUD_FUNCTION_AD_CONVERSION_TRIGGER_TYPE_2                        = (29U), ///< A/D Conversion Trigger Output Function Type 2.
    TAUD_FUNCTION_NON_COMPLEMENTARY_MODULATION_OUTPUT_FUNCTION_TYPE_1 = (30U), ///< Non-Complementary Modulation Output Function Type 1.
    TAUD_FUNCTION_COMPLEMENTARY_MODULATION_OUTPUT                     = (31U), ///< Complementary Modulation Output Function.
} taud_function_t;

/** Specifies Synchronous channel setting. */

/** Specifies the configuration of an operating mode. */
typedef enum e_taud_mode_specific
{
    TAUD_MODE_SPECIFIC_START_INT_NOT_GENERATED    = (0x0000U), ///< INTTAUDnIm is not generated.
    TAUD_MODE_SPECIFIC_START_TRIGGER_DISABLE      = (0x0000U), ///< Disables detection.
    TAUD_MODE_SPECIFIC_OUTPUT_IN_SIGNAL_WIDTH     = (0x0000U), ///< INTTAUDnIm output in signal width.
    TAUD_MODE_SPECIFIC_OUTPUT_IN_PULSE_INTERVAL   = (0x0000U), ///< INTTAUDnIm output in pulse interval (INTTAUDnIm is generated when TAUDnCNTm <= TAUDnCDRm).
    TAUD_MODE_SPECIFIC_START_INT_GENERATED        = (0x0001U), ///< INTTAUDnIm is generated.
    TAUD_MODE_SPECIFIC_START_TRIGGER_ENABLE       = (0x0001U), ///< Enables detection.
    TAUD_MODE_SPECIFIC_OUTPUT_OVER_SIGNAL_WIDTH   = (0x0001U), ///< INTTAUDnIm output over signal width.
    TAUD_MODE_SPECIFIC_OUTPUT_OVER_PULSE_INTERVAL = (0x0001U), ///< INTTAUDnIm output over pulse interval (INTTAUDnIm is generated when TAUDnCNTm > TAUDnCDRm).
} taud_mode_specific_t;

/** Enables/disables Simultaneous Rewrite of the data register. */
typedef enum e_taud_simultaneous_rewrite
{
    TAUD_SIMULTANEOUS_REWRITE_DISABLE = 0, ///< Disable simultaneous rewrite of the data register.
    TAUD_SIMULTANEOUS_REWRITE_ENABLE  = 1, ///< Enable simultaneous rewrite of the data register.
} taud_simultaneous_rewrite_t;

/** Specifies the channel that controls Simultaneous Rewrite. */
typedef enum e_taud_simultaneous_rewrite_channel
{
    TAUD_SIMULTANEOUS_REWRITE_CHANNEL_UNUSED  = 0, ///< Not used the simultaneous rewrite mode.
    TAUD_SIMULTANEOUS_REWRITE_CHANNEL_MASTER  = 0, ///< Master channel for simultaneous rewrite triggers.
    TAUD_SIMULTANEOUS_REWRITE_CHANNEL_ANOTHER = 1, ///< Another upper channel outside the channel group for simultaneous rewrite triggers.
} taud_simultaneous_rewrite_channel_t;

/** Specifies the timing for generating a Simultaneous Rewrite control signal. */
typedef enum e_taud_simultaneous_rewrite_trigger
{
    TAUD_SIMULTANEOUS_REWRITE_TRIGGER_UNUSED       = 0, ///< Not used the simultaneous rewrite mode.
    TAUD_SIMULTANEOUS_REWRITE_TRIGGER_MASTER_START = 0, ///< Generates a simultaneous rewrite trigger signal when the master channel starts to count.
    TAUD_SIMULTANEOUS_REWRITE_TRIGGER_SLAVE_PEAK   = 1, ///< Simultaneous rewrite trigger signal is generated when master channel counter is started and the corresponding slave channel is at the peak of triangular wave.
} taud_simultaneous_rewrite_trigger_t;

/** Type of valid edge detection */
typedef enum e_taud_input_edge
{
    TAUD_INPUT_EDGE_UNUSED            = (0x00U), ///< Unused edge detection.
    TAUD_INPUT_EDGE_FALLING           = (0x00U), ///< Falling edge.
    TAUD_INPUT_EDGE_RISING            = (0x01U), ///< Rising edge.
    TAUD_INPUT_EDGE_BOTH_MEASURE_LOW  = (0x02U), ///< Both edges (selects low width measurement).
    TAUD_INPUT_EDGE_BOTH_MEASURE_HIGH = (0x03U), ///< Both edges (selects high width measurement).
} taud_input_edge_t;

/** Timer output setting. */
typedef enum e_taud_output
{
    TAUD_OUTPUT_DISABLE = 0,           ///< Disables the independent timer output function.
    TAUD_OUTPUT_ENABLE  = 1,           ///< Enables the independent timer output function.
} taud_output_t;

/** Type of output mode. */
typedef enum e_taud_output_mode
{
    TAUD_OUTPUT_MODE_INDEPENDENT = 0,  ///< Independent channel operation.
    TAUD_OUTPUT_MODE_SYNCHRONOUS = 1,  ///< Synchronous channel operation.
} taud_output_mode_t;

/** Type of output mode of each channel in combination with TAUDnTOMm. */
typedef enum e_taud_output_operating_mode
{
    TAUD_OUTPUT_OPERATING_MODE_1 = 0,  ///< Operating mode 1.
    TAUD_OUTPUT_OPERATING_MODE_2 = 1,  ///< Operating mode 2.
} taud_output_operating_mode_t;

/** Enable/Disable real-time output. */
typedef enum e_taud_real_time_output
{
    TAUD_REAL_TIME_OUTPUT_DISABLE = 0, ///< Disable real-time output.
    TAUD_REAL_TIME_OUTPUT_ENABLE  = 1, ///< Enable real-time output.
} taud_real_time_output_t;

/** Specifies the real-time output trigger of each channel. */
typedef enum e_taud_real_time_output_trigger
{
    TAUD_REAL_TIME_OUTPUT_TRIGGER_UNUSED        = 0, ///< Unused.
    TAUD_REAL_TIME_OUTPUT_TRIGGER_NOT_GENERATED = 0, ///< Next upper channel with this bit set to 1 generates a real-time output trigger for channel m.
    TAUD_REAL_TIME_OUTPUT_TRIGGER_GENERATED     = 1, ///< Channel m generates an unique real-time output trigger.
} taud_real_time_output_trigger_t;

/** Specifies the real-time output level. */
typedef enum e_taud_real_time_output_level
{
    TAUD_REAL_TIME_OUTPUT_LEVEL_UNUSED = 0, ///< Unused.
    TAUD_REAL_TIME_OUTPUT_LEVEL_LOW    = 0, ///< Real-time output is low.
    TAUD_REAL_TIME_OUTPUT_LEVEL_HIGH   = 1, ///< Real-time output is high.
} taud_real_time_output_level_t;

/** Enable/Disable modulation output for timer output and real-time output. */
typedef enum e_taud_modulation
{
    TAUD_MODULATION_DISABLE = 0,       ///< Disable modulation.
    TAUD_MODULATION_ENABLE  = 1,       ///< Enable modulation.
} taud_modulation_t;

/** Input/Output pins, used to select which duty cycle to update in R_TAUD_DutyCycleSet(). */
typedef enum e_taud_io_pin
{
    TAUD_IO_PIN_TAUDTTOUT = 0,         ///< TAUDTTOUT.
} taud_io_pin_t;

/** Channel control block. DO NOT INITIALIZE. Initialization occurs when @ref timer_api_t::open is called. */
typedef struct st_taud_instance_ctrl
{
    uint32_t            open;                     ///< Whether or not channel is open.
    const timer_cfg_t * p_cfg;                    ///< Pointer to initial configurations.
    uint16_t            channels_mask;            ///< Masked value of all Master and Slave channels used.
    uint16_t            master_channel_mask;      ///< Masked value of Master channel used.
    uint16_t            slave_channels_mask;      ///< Masked value of Slave channels used.
    uint16_t            output_mask;              ///< Masked value of all output channels used in Synchronous Operation.
    R_TAUD_Type       * p_reg;                    ///< Base register for this channel.
    void (* p_callback)(timer_callback_args_t *); ///< Pointer to callback.
    timer_callback_args_t * p_callback_memory;    ///< Pointer to pre-allocated callback argument.
    void * p_context;                             ///< Pointer to context to be passed into callback function.
} taud_instance_ctrl_t;

/** Channel source. */
typedef enum e_taud_unit_source
{
    TAUD_UNIT0 = 0,                    ///< TAUD Unit 0.
    TAUD_UNIT1 = 1,                    ///< TAUD Unit 1.
#if (BSP_FEATURE_TAUD_MAX_UNIT > (2U))
    TAUD_UNIT2 = 2,                    ///< TAUD Unit 2.
#endif
#if (BSP_FEATURE_TAUD_MAX_UNIT > (3U))
    TAUD_UNIT3 = 3,                    ///< TAUD Unit 3.
#endif
} taud_unit_source_t;

/** Optional TAUD extension data structure. */
typedef struct st_taud_extended_cfg
{
    taud_function_t                     taud_function;                               ///< Selection of functions.
    taud_clock_t                        operation_clk;                               ///< The PCLK prescaler.
    taud_count_clock_t                  count_clk;                                   ///< Selection of Count Clock.
    taud_div_t                          clk_div;                                     ///< Clock division.
    uint16_t                            baudrate;                                    ///< Baud rate setting.
    uint32_t                            delay_counts;                                ///< Delay from trigger input to pulse output in raw timer counts.
    uint32_t                            width_counts;                                ///< Pulse width in raw timer counts.
    uint32_t                            divider_value;                               ///< Divider value for Clock Divide fucntion.
    uint32_t                            deadtime_value;                              ///< Dead time in raw timer counts.
    taud_channel_type_t                 channel_type;                                ///< Specifies the channel type, such as Master, Slave, Independent, Slave odd, or Slave even.
    uint8_t                             slave_ordinal_number;                        ///< Ordinal number of Slave channel, applied for Delay Pulse Output Function, Non-Complementary Modulation Output Function Type 1, Non-Complementary Modulation Output Function Type 2, Complementary Modulation Output Function.
    taud_unit_source_t                  taud_unit;                                   ///< Unit of TAUD module.
    taud_trigger_t                      trigger_type;                                ///< Selection of external start trigger.
    taud_overflow_t                     overflow_timing;                             ///< Specifies the timing for updating capture register and overflow flag.
    taud_mode_t                         operating_mode;                              ///< Specifies an operating mode.
    taud_mode_specific_t                mode_config;                                 ///< Specifies the configuration of an operating mode.
    taud_input_edge_t                   edge_type;                                   ///< Selection of input edge detection.
    taud_output_t                       output_enable;                               ///< Enables/disables the independent channel output mode.
    taud_output_mode_t                  output_mode;                                 ///< Specifies an output mode.
    taud_output_operating_mode_t        output_operating;                            ///< Specifies the output mode of each channel in combination with TAUDnTOMm.
    taud_output_logic_t                 output_logic;                                ///< Specifies the output logic.
    taud_deadtime_operation_t           deadtime_operation;                          ///< Specifies the dead time operation.
    taud_add_deadtime_t                 add_deadtime_phase;                          ///< Specifies the phase in which dead time is added.
    taud_simultaneous_rewrite_t         simultaneous_rewrite;                        ///< Enable/Disable simultaneous rewrite data register.
    taud_simultaneous_rewrite_channel_t simultaneous_rewrite_channel;                ///< Specifies the channel that controls Simultaneous Rewrite.
    taud_simultaneous_rewrite_trigger_t simultaneous_rewrite_trigger;                ///< Specifies the timing for generating a Simultaneous Rewrite control signal.
    taud_real_time_output_t             real_time_output;                            ///< Enable/Disable real-time output.
    taud_real_time_output_trigger_t     real_time_output_trigger;                    ///< Specifies the real-time output trigger of each channel.
    taud_real_time_output_level_t       real_time_output_level;                      ///< Specifies the real-time output level.
    taud_modulation_t                   modulation;                                  ///< Enable/Disable modulation output for timer output and real-time output.

#if (BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ)
    bsp_intsel_t intsel_irq_cfg;                                                     ///< Interrupt source for TAUD unit 3.
#endif

    timer_instance_t const * p_slave_channel_instances[TAUD_MAX_NUM_SLAVE_CHANNELS]; ///< Configuration for each Slave Channel, at least 1 Slave Channel is required.
} taud_extended_cfg_t;

/** Specifies the limited values for setting TAUD_CDR. */
typedef enum e_taud_cdr
{
    TAUD_CDR_MIN = 0U,                 ///< Minimum value that can be set for the TAUD_CDR register via the R_TAUD_PulseOutputDelay API.
    TAUD_CDR_MAX = 65536U,             ///< Maximum value that can be set for the TAUD_CDR register via the R_TAUD_PeriodSet API.
} taud_cdr_t;

/** Specifies the period count maximum values. */
typedef enum e_taud_period_count_max
{
    TAUD_PERIOD_COUNT_MAX_CDR_X2 = 0x20000, ///< Maximum period count with range from 0 to 131072.
    TAUD_PERIOD_COUNT_MAX_CDR_X1 = 0x10000, ///< Maximum period count with range from 0 to 65536.
} taud_period_count_max_t;

/** Specifies the deadtime count maximum values. */
typedef enum e_taud_deadtime_count_max_function
{
    TAUD_DEADTIME_COUNT_MAX_FUNCTION_ONE_PHASE_PWM_OUTPUT = 0x10000, ///< Maximum deadtime count for One-Phase PWM Output Function.
} taud_deadtime_count_max_function_t;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_TAUD_Open(timer_ctrl_t * const p_ctrl, timer_cfg_t const * const p_cfg);
fsp_err_t R_TAUD_Stop(timer_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_Start(timer_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_Enable(timer_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_Disable(timer_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_InfoGet(timer_ctrl_t * const p_ctrl, timer_info_t * const p_info);
fsp_err_t R_TAUD_StatusGet(timer_ctrl_t * const p_ctrl, timer_status_t * const p_status);
fsp_err_t R_TAUD_Reset(timer_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_PeriodSet(timer_ctrl_t * const p_ctrl, uint32_t const period);
fsp_err_t R_TAUD_CallbackSet(timer_ctrl_t * const          p_api_ctrl,
                             void (                      * p_callback)(timer_callback_args_t *),
                             void * const                  p_context,
                             timer_callback_args_t * const p_callback_memory);
fsp_err_t R_TAUD_DutyCycleSet(timer_ctrl_t * const p_ctrl, uint32_t const duty_cycle_counts, uint32_t const pin);
fsp_err_t R_TAUD_PulseOutputDelay(timer_ctrl_t * const p_ctrl, uint32_t const delay_counts);
fsp_err_t R_TAUD_PulseWidthSet(timer_ctrl_t * const p_ctrl, uint32_t const width_counts);
fsp_err_t R_TAUD_Close(timer_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_Clear(timer_ctrl_t * const p_ctrl);
fsp_err_t R_TAUD_InputEdgeSet(timer_ctrl_t * const p_ctrl, taud_input_edge_t const input_edge);
fsp_err_t R_TAUD_OutputLevelSet(timer_ctrl_t * const                p_ctrl,
                                taud_real_time_output_level_t const real_time_output_level);
fsp_err_t R_TAUD_ModulationSet(timer_ctrl_t * const p_ctrl, taud_modulation_t const modulation);
fsp_err_t R_TAUD_DeadTimeOutputLevelSet(timer_ctrl_t * const p_ctrl, taud_add_deadtime_t const add_deadtime_phase);

/*******************************************************************************************************************//**
 * @} (end defgroup TAUD)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_TAUD_H */
