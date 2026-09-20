/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_CONNECTIVITY_INTERFACES
 * @defgroup SENT_API SENT Interface
 * @brief Interface for SENT communications.
 *
 * @section SENT_INTERFACE_SUMMARY Summary
 * The SENT interface provides common APIs for SENT HAL drivers. The SENT interface supports the following features:
 * - Interrupt driven receive processing
 * - Callback function with returned event code
 *
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_SENT_API_H
#define R_SENT_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Includes board and MCU related header files. */
#include "bsp_api.h"
#include "r_timer_api.h"
#include "r_transfer_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** SENT number of data nibbles */
typedef enum e_sent_num_of_data_nibbles
{
    SENT_NUM_OF_DATA_NIBBLES_1,        ///< Number of data nibble: 1 nibble.
    SENT_NUM_OF_DATA_NIBBLES_2,        ///< Number of data nibble: 2 nibbles.
    SENT_NUM_OF_DATA_NIBBLES_3,        ///< Number of data nibble: 3 nibbles.
    SENT_NUM_OF_DATA_NIBBLES_4,        ///< Number of data nibble: 4 nibbles.
    SENT_NUM_OF_DATA_NIBBLES_5,        ///< Number of data nibble: 5 nibbles.
    SENT_NUM_OF_DATA_NIBBLES_6,        ///< Number of data nibble: 6 nibbles.
    SENT_NUM_OF_DATA_NIBBLES_7,        ///< Number of data nibble: 7 nibbles.
    SENT_NUM_OF_DATA_NIBBLES_8         ///< Number of data nibble: 8 nibbles.
} sent_num_of_data_nibbles_t;

/** SENT serial message format */
typedef enum e_sent_serial_message_format
{
    SENT_SERIAL_MESSAGE_FORMAT_NO_SERIAL_MESSAGE,      ///< No extraction of serial message.
    SENT_SERIAL_MESSAGE_FORMAT_SHORT_SERIAL_MESSAGE,   ///< Short serial message format.
    SENT_SERIAL_MESSAGE_FORMAT_ENHANCED_SERIAL_MESSAGE ///< Enhanced serial message format.
} sent_serial_message_format_t;

/** SENT frame check method for calibration pulse */
typedef enum e_sent_frame_check_method
{
    SENT_FRAME_CHECK_METHOD_NEXT_CALIBRATION_PULSE,    ///< Check against the next calibration pulse.
    SENT_FRAME_CHECK_METHOD_PREVIOUS_CALIBRATION_PULSE ///< Check against previous calibration pulse.
} sent_frame_check_method_t;

/** SENT fast channel CRC check */
typedef enum e_sent_fast_channel_crc_check
{
    SENT_FAST_CHANNEL_CRC_CHECK_ENABLE  = 0U, ///< Enable CRC check for fast channel.
    SENT_FAST_CHANNEL_CRC_CHECK_DISABLE = 1U  ///< Disable CRC check for fast channel.
} sent_fast_channel_crc_check_t;

/** SENT slow channel CRC check */
typedef enum e_sent_slow_channel_crc_check
{
    SENT_SLOW_CHANNEL_CRC_CHECK_ENABLE  = 0U, ///< Enable CRC check for slow channel.
    SENT_SLOW_CHANNEL_CRC_CHECK_DISABLE = 1U  ///< Disable CRC check for slow channel.
} sent_slow_channel_crc_check_t;

/** SENT data nibble CRC format */
typedef enum e_sent_data_nibble_crc_format
{
    SENT_DATA_NIBBLE_CRC_FORMAT_SAE_J2716,    ///< Use SAE J2716 2010/2016 format.
    SENT_DATA_NIBBLE_CRC_FORMAT_PRE_SAE_J2716 ///< Use pre SAE J2716 2010 format.
} sent_data_nibble_crc_format_t;

/** SENT pause pulse type */
typedef enum e_sent_pause_pulse_cfg
{
    SENT_PAUSE_PULSE_CFG_ABSENT = 0U,                  ///< Pause pulse is absent.
    SENT_PAUSE_PULSE_CFG_VARIABLE_MESSAGE_LENGTH = 1U, ///< Pause pulse with variable message length.
    SENT_PAUSE_PULSE_CFG_FIXED_MESSAGE_LENGTH    = 3U  ///< Pause pulse with fixed message length.
} sent_pause_pulse_cfg_t;

/** SENT SPC output polarity */
typedef enum e_sent_spc_output_polarity
{
    SENT_SPC_OUTPUT_POLARITY_ACTIVE_HIGH = 0U, ///< SENT SPC output active high.
    SENT_SPC_OUTPUT_POLARITY_ACTIVE_LOW  = 1U  ///< SENT SPC output active low.
} sent_spc_output_polarity_t;

/** SENT SPC configuration */
typedef enum e_sent_spc_cfg
{
    SENT_SPC_CFG_DISABLE = 0U,         ///< SENT SPC function disable.
    SENT_SPC_CFG_ENABLE  = 1U          ///< SENT SPC function enable.
} sent_spc_cfg_t;

/** SENT communication type */
typedef enum e_sent_communication
{
    SENT_COMMUNICATION_FAST_MESSAGE_READ, ///< Fast channel message read communication.
    SENT_COMMUNICATION_SLOW_MESSAGE_READ  ///< Slow channel message read communication.
} sent_communication_t;

/** SENT Event codes */
typedef enum e_sent_event
{
    SENT_EVENT_FAST_CHANNEL_RX_COMPLETE                 = (1UL << 0), ///< Fast channel receive complete event.
    SENT_EVENT_FAST_CHANNEL_CRC_ERROR                   = (1UL << 1), ///< Fast channel CRC error.
    SENT_EVENT_FAST_CHANNEL_MESSAGE_LOST                = (1UL << 2), ///< Fast channel message lost error.
    SENT_EVENT_FAST_CHANNEL_NIBBLE_ENCODING_ERROR       = (1UL << 3), ///< Fast channel nibble length is not in range.
    SENT_EVENT_FAST_CHANNEL_NIBBLE_COUNT_ERROR          = (1UL << 4), ///< Fast channel number of nibbles is not correct.
    SENT_EVENT_CALIBRATION_PULSE_LENGTH_ERROR           = (1UL << 5), ///< Calibration pulse length is not in acceptable range.
    SENT_EVENT_CALIBRATION_PULSE_LENGTH_VARIATION_ERROR = (1UL << 6), ///< Calibration pulse length variation is not in acceptable range.
    SENT_EVENT_NO_RESPONSE_ERROR           = (1UL << 7),              ///< No response between two SPC pulses.
    SENT_EVENT_SLOW_CHANNEL_CRC_ERROR      = (1UL << 8),              ///< Slow channel CRC error.
    SENT_EVENT_SLOW_CHANNEL_MESSAGE_LOST   = (1UL << 9),              ///< Slow channel message lost error.
    SENT_EVENT_SLOW_CHANNEL_ENCODING_ERROR = (1UL << 10),             ///< Slow channel encoding error (the received bits sequence is wrong).
    SENT_EVENT_SLOW_CHANNEL_RX_COMPLETE    = (1UL << 11),             ///< Slow channel receive complete event.
    SENT_EVENT_SPC_TIMEOUT                 = (1UL << 12)              ///< SPC pulse is sent after a timeout period but no message receive.
} sent_event_t;

/** SPCO source selection modes */
typedef enum e_sent_spco_mode
{
    SENT_SPCO_MODE_RSENT_SPC = 0U,     ///< Select RSENT_SPC as the source
    SENT_SPCO_MODE_TAUD      = 2U      ///< Select TAUDxx as the source
} sent_spco_mode_t;

/** Operation modes of RSENT */
typedef enum e_sent_status
{
    SENT_STATUS_RESET            = 0U, ///< SENT is in RESET mode.
    SENT_STATUS_CONFIGURATION    = 1U, ///< SENT is in CONFIGURATION mode.
    SENT_STATUS_OPERATION_IDLE   = 3U, ///< SENT is in OPERATION IDLE mode.
    SENT_STATUS_OPERATION_ACTIVE = 5U  ///< SENT is in OPERATION ACTIVE mode.
} sent_status_t;

/** SENT Callback parameter definition */
typedef struct st_sent_callback_arg
{
    sent_event_t event;                ///< Event code.
    void const * p_context;            ///< Context provided to user during callback.
} sent_callback_args_t;

/** SENT Configuration */
typedef struct st_sent_cfg
{
    /* SENT generic configuration */
    uint8_t channel;                                      ///< Select a channel corresponding to the channel number of the hardware.
    sent_num_of_data_nibbles_t    num_of_data_nibbles;    ///< Number of data nibble(s) in one fast message.
    sent_serial_message_format_t  serial_message_format;  ///< Select slow channel serial message format.
    sent_frame_check_method_t     frame_check_method;     ///< Select frame check method for calibration pulse.
    sent_fast_channel_crc_check_t fast_channel_crc_check; ///< Enable/Disable fast channel CRC check.
    sent_slow_channel_crc_check_t slow_channel_crc_check; ///< Enable/Disable slow channel CRC check.
    sent_data_nibble_crc_format_t data_nibble_crc_format; ///< Select CRC format.
    sent_pause_pulse_cfg_t        pause_pulse_cfg;        ///< Select pause pulse type.
    sent_spc_output_polarity_t    spc_output_polarity;    ///< Select SPC output polarity.
    sent_spc_cfg_t                spc_cfg;                ///< Enable/Disable SPC function.
    IRQn_Type rxi_irq;                                    ///< Receive interrupt IRQ number.
    uint8_t   rxi_ipl;                                    ///< Receive interrupt priority.
    IRQn_Type eri_irq;                                    ///< Error interrupt IRQ number.
    uint8_t   eri_ipl;                                    ///< Error interrupt priority.
    transfer_instance_t const * p_transfer_rx;            ///< To use SENT DTSC/DMAC read transfer, link a transfer instance here.  Set to NULL if unused.
    timer_instance_t const    * p_spc_timer;              ///< Timer instance for SPC function timeout. NULL when not use.

    /* Configuration for SENT event processing */
    void (* p_callback)(sent_callback_args_t * p_args);   ///< Pointer to callback function.
    void const * p_context;                               ///< User defined context passed into callback function.

    /* Pointer to SENT peripheral specific configuration */
    void const * p_extend;                                ///< SENT hardware dependent configurations.
} sent_cfg_t;

/** SENT control block. Allocate an instance specific control block to pass into the SENT API calls. */
typedef void sent_ctrl_t;

/** Shared Interface definition for SENT */
typedef struct st_sent_api
{
    /** Open SENT device.
     *
     * @param[in,out]  p_ctrl     Pointer to the SENT control block. Must be declared by user. Value set here.
     * @param[in]      sent_cfg_t Pointer to SENT configuration structure. All elements of this structure
     *                            must be set by user.
     */
    fsp_err_t (* open)(sent_ctrl_t * const p_ctrl, sent_cfg_t const * const p_cfg);

    /** Close SENT device.
     *
     * @param[in]   p_ctrl     Pointer to the SENT control block.
     */
    fsp_err_t (* close)(sent_ctrl_t * const p_ctrl);

    /** Start SENT device. Begin to capture incomming message.
     *
     * @param[in]   p_ctrl     Pointer to the SENT control block for the channel.
     */
    fsp_err_t (* start)(sent_ctrl_t * const p_ctrl);

    /** Stop SENT device. Stop capture sent message.
     *
     * @param[in]   p_ctrl     Pointer to the SENT control block for the channel.
     */
    fsp_err_t (* stop)(sent_ctrl_t * const p_ctrl);

    /** Read a number of fast channel SENT messages and save them to the provided buffer.
     *  If user callback is specified, it is called when the read is finished.
     *
     * @param[in]   p_ctrl          Pointer to the SENT control block for the channel.
     * @param[in]   p_dest          Pointer to the buffer to save fast channel SENT message
     * @param[in]   message_count   Number of messages to read.
     */
    fsp_err_t (* fastRead)(sent_ctrl_t * const p_ctrl, void * p_dest, uint32_t const message_count);

    /** Read a number of slow channel SENT messages and save them to the provided buffer.
     * If user callback is specified, it is called when the read is finished.
     *
     * @param[in]   p_ctrl          Pointer to the SENT control block for the channel.
     * @param[in]   p_dest          Pointer to the buffer to save slow channel SENT message.
     * @param[in]   message_count   Number of messages to read.
     */
    fsp_err_t (* slowRead)(sent_ctrl_t * const p_ctrl, void * p_dest, uint32_t const message_count);

    /** Send a SPC pulse with duration is pulse_length in ticks.
     *
     * @param[in]   p_ctrl          Pointer to the SENT control block for the channel.
     * @param[in]   pulse_length    SPC pulse length to generate in ticks (1->128 tick).
     */
    fsp_err_t (* spcWrite)(sent_ctrl_t * const p_ctrl, uint8_t const pulse_length);

    /** Select RSENT control for SPCO output signal.
     *
     * @param[in] p_ctrl                         Pointer to control block.
     * @param[in] mode                           Input source mode.
     */
    fsp_err_t (* spcSelect)(sent_ctrl_t * const p_ctrl, sent_spco_mode_t const mode);

    /** Abort an on-going communication.
     *
     * @param[in]   p_ctrl                  Pointer to the SENT control block for the channel.
     * @param[in]   communication_to_abort  The communication is going to abort.
     */
    fsp_err_t (* abort)(sent_ctrl_t * const p_ctrl, sent_communication_t const communication_to_abort);

    /** Specify callback function, optional context pointer and working memory pointer.
     *
     * @param[in]   p_ctrl                   Pointer to the SENT control block.
     * @param[in]   p_callback               Callback function.
     * @param[in]   p_context                Pointer to send to callback function.
     * @param[in]   p_working_memory         Pointer to volatile memory where callback structure can be allocated.
     *                                       Callback arguments allocated here are only valid during the callback.
     */
    fsp_err_t (* callbackSet)(sent_ctrl_t * const p_ctrl, void (* p_callback)(sent_callback_args_t *),
                              void const * const p_context, sent_callback_args_t * const p_callback_memory);

    /** Get SENT mode status and communication status.
     *
     * @param[in]   p_ctrl     Pointer to the SENT control block.
     * @param[out]  p_status   Pointer to the status structure to be filled in.
     *
     */
    fsp_err_t (* statusGet)(sent_ctrl_t * const p_ctrl, sent_status_t * const p_status);
} sent_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sent_instance
{
    sent_ctrl_t      * p_ctrl;         ///< Pointer to the control structure for this instance.
    sent_cfg_t const * p_cfg;          ///< Pointer to the configuration structure for this instance.
    sent_api_t const * p_api;          ///< Pointer to the API structure for this instance.
} sent_instance_t;

/** @} (end defgroup SENT_API) */

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
