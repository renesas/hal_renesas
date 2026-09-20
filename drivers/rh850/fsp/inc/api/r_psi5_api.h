/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_CONNECTIVITY_INTERFACES
 * @defgroup PSI5_API PSI5 Interfaces
 * @brief Interface for PSI5 communication
 *
 * @section PSI5_API_SUMMARY Summary
 * @brief The PSI5 interface provides APIs and definitions for PSI5 sensor communication.
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_PSI5_API_H
#define R_PSI5_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Common error codes and definitions. */
#include "bsp_api.h"
#include "r_transfer_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *
 **********************************************************************************************************************/

#ifndef BSP_OVERRIDE_PSI5_EVENT_T

/** PSI5 event */
typedef enum e_psi5_event
{
    PSI5_EVENT_NONE = 0,                        ///< Non event
    PSI5_EVENT_SYNC_END,                        ///< Synchronization end event
    PSI5_EVENT_SYNC_START,                      ///< Synchronization start event
    PSI5_EVENT_TRANSMIT_DATA_EMPTY,             ///< Transmit data empty event
    PSI5_EVENT_SYNC_PULSE_AND_RX_DATA_CONFLICT, ///< Synchronization pulse and receive data conflict error event
    PSI5_EVENT_RX_DATA_ERR,                     ///< Receive data error event
    PSI5_EVENT_RX_DATA_FIFO_OVERFLOW,           ///< Receive data FIFO overflow event
    PSI5_EVENT_RX_DATA_EXIST,                   ///< Receive data exist event
    PSI5_EVENT_RX_MESSAGE_ERR,                  ///< Receive message FIFO overflow event
    PSI5_EVENT_RX_MESSAGE_FIFO_OVERFLOW,        ///< Receive message error event
    PSI5_EVENT_RX_MESSAGE_EXIST,                ///< Receive message exist event
    PSI5_EVENT_WP_ALL_CHANNEL                   ///< All channel enable event
} psi5_event_t;
#endif

/** PSI5's communication mode */
typedef enum e_psi5_comm_mode
{
    PSI5_COMM_MODE_ASYNCHRONOUS = 0U,                  ///< A - Asynchronous Mode
    PSI5_COMM_MODE_SYNCHRONOUS_PARALLEL_BUS,           ///< P - Synchronous Parallel Bus Mode
    PSI5_COMM_MODE_SYNCHRONOUS_UNIVERSAL_BUS,          ///< U - Synchronous Universal Bus Mode
    PSI5_COMM_MODE_SYNCHRONOUS_DAISY_CHAIN_BUS,        ///< D - Synchronous Daisy Chain Bus Mode
    PSI5_COMM_MODE_VARIABLE_TIME_TRIGGERED_SYNCHRONOUS ///< V - Variable Time Triggered Synchronous Operation Mode
} psi5_comm_mode_t;

/** ECU to Sensor Communication - Physical Layer */
typedef enum e_psi5_tx_mode
{
    PSI5_TX_MODE_TOOTH_GAP = 0U,       ///< Tooth Gap Mode
    PSI5_TX_MODE_PUSLE_WIDTH           ///< Pulse Width Mode
} psi5_tx_mode_t;

/** ECU to Sensor Communication - Data Frames*/
typedef enum e_psi5_tx_frame_format
{
    PSI5_TX_FRAME_FORMAT_SHORT  = 1,   ///< Frame1 Short
    PSI5_TX_FRAME_FORMAT_LONG   = 2,   ///< Frame2 Long (4-bit or 8-bit data nibbles)
    PSI5_TX_FRAME_FORMAT_XLONG  = 3,   ///< Frame3 XLong
    PSI5_TX_FRAME_FORMAT_XXLONG = 4    ///< Frame4 XXLong
} psi5_tx_frame_format_t;

/** Setting Slot Enable */
typedef enum e_psi5_rx_slot
{
    PSI5_RX_SLOT_DISABLE = 0,          ///< Receive slot is disable
    PSI5_RX_SLOT_ENABLE                ///< Receive slot is enable
} psi5_rx_slot_t;

/** Setting for Reception Serial Message */
typedef enum e_psi5_rx_serial_message_channel
{
    PSI5_RX_SERIAL_MESSAGE_CHANNEL_DISABLE = 0, ///< Serial Message Reception is disable
    PSI5_RX_SERIAL_MESSAGE_CHANNEL_ENABLE       ///< Serial Message Reception is enable
} psi5_rx_serial_message_channel_t;

/** Setting for reception error detection mode */
typedef enum e_psi5_rx_err_detect
{
    PSI5_RX_ERR_DETECT_PARITY = 0,     ///< 1-bit parity check
    PSI5_RX_ERR_DETECT_CRC             ///< 3-bit CRC check
} psi5_rx_err_detect_t;

/** List of PSI5's operation mode*/
typedef enum e_psi5_operation_mode
{
    PSI5_OPERATION_MODE_NORMAL = 0U,   ///< Normal Operation Mode
    PSI5_OPERATION_MODE_PAS            ///< PAS Operation Mode
} psi5_operation_mode_t;

/** Timestamp operation */
typedef enum e_psi5_ip_timer
{
    PSI5_IP_TIMER_DISABLE = 0,         ///< IP timer is disable
    PSI5_IP_TIMER_ENABLE               ///< IP timer is enable
} psi5_ip_timer_t;

/** Timestamp mode */
typedef enum e_psi5_timestamp_mode
{
    PSI5_TIMESTAMP_MODE_SLAVE = 0U,    ///< Timer slave mode
    PSI5_TIMESTAMP_MODE_MASTER         ///< Timer master mode
} psi5_timestamp_mode_t;

/** Timestamp setting */
typedef struct st_psi5_timestamp_setting
{
    psi5_ip_timer_t       ip_timer_enable; ///< Enable timestamp timer
    psi5_timestamp_mode_t timestamp_mode;  ///< Timestamp mode
    uint8_t               master_select;   ///< Select current PSI5 channel's master
} psi5_timestamp_setting_t;

/** Reception and Tramission status */
typedef struct st_psi5_status
{
    uint32_t rx_message_status;        ///< Indicates status of receive-message channel
    uint32_t rx_module_status;         ///< Indicates status of receive module
    uint32_t tx_status;                ///< Indicates the transmission status
} psi5_status_t;

/** Receive data frame  */
typedef struct st_psi5_data_frame
{
    uint32_t rx_data;                  ///< Received data (not include start bit and parity/CRC bits)
    uint32_t rx_data_status;           ///< Indicates status of received data
    uint32_t rx_timestamp;             ///< Receive a timestamp when the last data frame is received
} psi5_data_frame_t;

/** Receive serial data frame  */
typedef struct st_psi5_serial_message_frame
{
    uint16_t data_field;               ///< Serial message data */
    uint8_t  serial_id;                ///< Serial message id */
    uint8_t  rx_crc;                   ///< Serial message crc */
    uint8_t  configuration;            ///< Configuration Bit */
    uint32_t rx_timestamp;             ///< Receive a timestamp when the last serial message frame is received
} psi5_serial_message_frame_t;

/** Callback function parameter data. */
typedef struct st_psi5_callback_args
{
    psi5_event_t event;                ///< Event code
    void const * p_context;            ///< Placeholder for user data.  Set in psi5_api_t::open function in ::psi5_cfg_t.
} psi5_callback_args_t;

/** Configuration for PSI5 channel */
typedef struct st_psi5_cfg
{
    uint8_t channel;                                    ///< PSI5 channel

    uint16_t bitrate;                                   ///< Baudrate - 1 bit cycle waveform

    uint32_t sync_period_counter;                       ///< Counter for the SYNC signal's period

    psi5_comm_mode_t communication_mode;                ///< Communication mode

    IRQn_Type tx_empty_irq;                             ///< PSI5 transmit data empty interrupt
    uint8_t   tx_empty_ipl;                             ///< PSI5 transmit data empty interrupt priority level

    IRQn_Type rx_data_exist_irq;                        ///< PSI5 Receive data exist interrupt
    uint8_t   rx_data_exist_ipl;                        ///< PSI5 Receive data exist interrupt priority level

    IRQn_Type status_irq;                               ///< PSI5 status interrupt
    uint8_t   status_ipl;                               ///< PSI5 status interrupt priority level

    transfer_instance_t const * p_transfer_rx;          ///< Transfer instance for PSI5 receive. Set to NULL if unused.

    void const * p_context;                             ///< User defined context passed into callback function.

    void (* p_callback)(psi5_callback_args_t * p_args); ///< Pointer to callback function.

    void const * p_extend;                              ///< Any configuration data needed by the hardware
} psi5_cfg_t;

/** PSI5 control block.  Allocate an instance specific control block to pass into the PSI5 API calls.
 */
typedef void psi5_ctrl_t;

/** PSI5 functions implemented at the HAL layer API. */
typedef struct st_psi5_api
{
    /** Open the PSI5 driver.
     *
     * @param[in]     p_ctrl    Pointer to PSI5 instance control block.
     * @param[in]     p_cfg     Pointer to PSI5 instance configuration structure.
     */
    fsp_err_t (* open)(psi5_ctrl_t * const p_ctrl, psi5_cfg_t const * const p_cfg);

    /** Close PSI5 driver.
     *
     * @param[in]     p_ctrl    Pointer to PSI5 instance control block.
     */
    fsp_err_t (* close)(psi5_ctrl_t * const p_ctrl);

    /** Start PSI5 device. Begin to capture incomming message.
     *
     * @param[in]   p_ctrl     Pointer to PSI5 instance control block.
     */
    fsp_err_t (* start)(psi5_ctrl_t * const p_ctrl);

    /** Stop PSI5 device. Stop capture message.
     *
     * @param[in]   p_ctrl     Pointer to PSI5 instance control block.
     */
    fsp_err_t (* stop)(psi5_ctrl_t * const p_ctrl);

    /** Collect the most present data from PSI5 channel
     *
     * @param[in]   p_ctrl          Pointer to PSI5 instance control block.
     * @param[out]  p_dataFrame     Pointer to the PSI5 message frame structure for the channel
     * @param[in]   slot_num        Select slot number to read data frame.
     */
    fsp_err_t (* dataFrameRead)(psi5_ctrl_t * const p_ctrl, psi5_data_frame_t * p_dataFrame, uint8_t slot_num);

    /** Collect the most present serial data (message) from PSI5 channel
     *
     * @param[in]   p_ctrl          Pointer to PSI5 instance control block.
     * @param[out]  p_messageframe  Pointer to the PSI5 message frame structure for the channel.
     * @param[in]   slot_num        Select slot number to read serial message frame.
     */
    fsp_err_t (* messageFrameRead)(psi5_ctrl_t * const p_ctrl, psi5_serial_message_frame_t * p_messageframe,
                                   uint8_t slot_num);

    /** Collect the status and error of PSI5 channel
     *
     * @param[in]   p_ctrl      Pointer to PSI5 instance control block.
     * @param[out]  p_status    Pointer to the PSI5 message frame structure for the channel.
     */
    fsp_err_t (* statusGet)(psi5_ctrl_t * const p_ctrl, psi5_status_t * p_status);

    /** Specify callback function and optional context pointer and working memory pointer.
     *
     * @param[in]   p_ctrl                   Pointer to PSI5 instance control block.
     * @param[in]   p_callback               Callback function
     * @param[in]   p_context                Pointer to send to callback function
     * @param[in]   p_callback_memory        Pointer to volatile memory where callback structure can be allocated.
     *                                       Callback arguments allocated here are only valid during the callback.
     */
    fsp_err_t (* callbackSet)(psi5_ctrl_t * const p_ctrl, void (* p_callback)(psi5_callback_args_t *),
                              void const * const p_context, psi5_callback_args_t * const p_callback_memory);

    /** Write Channel Data
     *
     * @param[in]     p_ctrl    Pointer to PSI5 instance control block.
     * @param[in]     p_src     Pointer to PSI5 transmit buffer
     */
    fsp_err_t (* write)(psi5_ctrl_t * const p_ctrl, uint32_t const * const p_src);

    /** Set Timestamp
     *
     * @param[in]     p_ctrl              Pointer to PSI5 instance control block.
     * @param[in]     p_timestamp_cfg     Pointer to PSI5 timestamp configuration
     */
    fsp_err_t (* timestampSet)(psi5_ctrl_t * const p_ctrl, psi5_timestamp_setting_t const * const p_timestamp_cfg);
} psi5_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_psi5_instance
{
    psi5_ctrl_t      * p_ctrl;         ///< Pointer to the control structure for this instance
    psi5_cfg_t const * p_cfg;          ///< Pointer to the configuration structure for this instance
    psi5_api_t const * p_api;          ///< Pointer to the API structure for this instance
} psi5_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup PSI5_API)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
