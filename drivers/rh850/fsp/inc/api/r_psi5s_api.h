/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_CONNECTIVITY_INTERFACES
 * @defgroup PSI5S_API PSI5S Interfaces
 * @brief Interface for PSI5S communication
 *
 * @section PSI5S_API_SUMMARY Summary
 * @brief The PSI5S interface provides APIs and definitions for PSI5S sensor communication.
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_PSI5S_API_H
#define R_PSI5S_API_H

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

#ifndef BSP_OVERRIDE_PSI5S_CHANNEL_T

/** PSI5S channel */
typedef enum e_psi5s_channel
{
    PSI5S_CHANNEL_0 = 0U,              ///< PSI5S channel 0
    PSI5S_CHANNEL_1,                   ///< PSI5S channel 1
    PSI5S_CHANNEL_2,                   ///< PSI5S channel 2
    PSI5S_CHANNEL_3,                   ///< PSI5S channel 3
    PSI5S_CHANNEL_4,                   ///< PSI5S channel 4
    PSI5S_CHANNEL_5,                   ///< PSI5S channel 5
    PSI5S_CHANNEL_6,                   ///< PSI5S channel 6
    PSI5S_CHANNEL_7                    ///< PSI5S channel 7
} psi5s_channel_t;
#endif

#ifndef BSP_OVERRIDE_PSI5S_PACKET_FRAME_T

/** PSI5S error packet frame */
typedef enum e_psi5s_packet_frame
{
    PSI5S_PACKET_FRAME_NONE = 0U,      ///< No RX error
    PSI5S_PACKET_FRAME_1,              ///< RX error at packet frame 1
    PSI5S_PACKET_FRAME_2,              ///< RX error at packet frame 2
    PSI5S_PACKET_FRAME_3,              ///< RX error at packet frame 3
    PSI5S_PACKET_FRAME_4,              ///< RX error at packet frame 4
    PSI5S_PACKET_FRAME_5,              ///< RX error at packet frame 5
    PSI5S_PACKET_FRAME_6               ///< RX error at packet frame 6
} psi5s_packet_frame_t;
#endif

/** PSI5S event */
typedef enum e_psi5s_event
{
    PSI5S_EVENT_NONE = 0U,                 ///< Non event
    PSI5S_EVENT_RX_PACKET_FRAME_XCRC_ERR,  ///< Interrupt status of packet frame XCRC error
    PSI5S_EVENT_RX_PAYLOAD_CRC_ERR,        ///< Interrupt status of pay load data CRC error
    PSI5S_EVENT_RX_PAYLOAD_PARITY_ERR,     ///< Interrupt status of pay load data parity error
    PSI5S_EVENT_RX_TRANSCEIVER_STATUS_ERR, ///< Interrupt status of RX transceiver status error
    PSI5S_EVENT_RX_UART_PARITY_ERR,        ///< Interrupt status of RX UART parity error
    PSI5S_EVENT_RX_UART_FRAME_ERR,         ///< Interrupt status of RX UART framing error
    PSI5S_EVENT_RX_WDT_ERR,                ///< Interrupt status of RX WDT error
    PSI5S_EVENT_RX_OVERRUN_ERR,            ///< Interrupt status of RX overrun error
    PSI5S_EVENT_RX_FRAME_LACK_ERR,         ///< Interrupt status of RX frame lack error
    PSI5S_EVENT_RX_FRAME_EXCESS_ERR,       ///< Interrupt status RX frame excess error
    PSI5S_EVENT_RX_FINISH,                 ///< Interrupt status of RX finish
    PSI5S_EVENT_DDSR_OVERWRITE_ERR,        ///< CPU interrupt status of DDSR overwrite error
    PSI5S_EVENT_DDSR_FINISH,               ///< CPU interrupt status of DDSR finish
    PSI5S_EVENT_TX_CMD_FINISH              ///< Interrupt status of command TX finish
} psi5s_event_t;

/** PSI5S operation mode */
typedef enum e_psi5s_operation_mode
{
    PSI5S_OPERATION_MODE_UART = 0U,    ///< UART operation mode
    PSI5S_OPERATION_MODE_PSI5S,        ///< PSI5S operation mode
    PSI5S_OPERATION_MODE_CONFIG        ///< Configuration mode
} psi5s_operation_mode_t;

/** PSI5S synchronization mode selection */
typedef enum e_psi5s_sync_mode_select
{
    PSI5S_SYNC_MODE_SELECT_SYNC = 0U,  ///< Select synchronous mode using synchronization pulse
    PSI5S_SYNC_MODE_SELECT_ASYNC       ///< Select asynchronous mode without synchronization pulse
} psi5s_sync_mode_select_t;

/** PSI5S LoopBack mode */
typedef enum e_psi5s_loopback_mode
{
    PSI5S_LOOPBACK_MODE_DISABLE = 0U,  ///< LoopBack test disable
    PSI5S_LOOPBACK_MODE_ENABLE         ///< LoopBack test enable
} psi5s_loopback_mode_t;

/** PSI5S ECU to Sensor DDSR transmit data frames type */
typedef enum e_psi5s_ddsr_trans_type
{
    PSI5S_DDSR_TRANS_TYPE_SHORT = 0U,  ///< Short DDSR frame
    PSI5S_DDSR_TRANS_TYPE_LONG,        ///< Long DDSR frame
    PSI5S_DDSR_TRANS_TYPE_XLONG,       ///< Extra-long DDSR frame
    PSI5S_DDSR_TRANS_TYPE_XXLONG,      ///< Extra-extra-long DDSR frame
} psi5s_ddsr_trans_type_t;

/** PSI5S setting channel filter Enable */
typedef enum e_psi5s_noise_filter
{
    PSI5S_NOISE_FILTER_DISABLE = 0U,   ///< Noise filter disabled
    PSI5S_NOISE_FILTER_ENABLE          ///< Noise filter enabled
} psi5s_noise_filter_t;

/** PSI5S Setting UART communication clock output Enable */
typedef enum e_psi5s_clock_status
{
    PSI5S_CLOCK_STATUS_CLOSE = 0U,     ///< UART communication clock output disabled
    PSI5S_CLOCK_STATUS_OPEN            ///< UART communication clock output enabled
} psi5s_clock_status_t;

/** PSI5S TX sync command setting */
typedef struct st_psi5s_sync_cmd_setting
{
    uint8_t sync_cmd_alt_trans_cmd;    ///< Alternate transport command
    uint8_t sync_cmd_alt_trans_chid;   ///< Alternate transport ChID
    uint8_t sync_cmd_trans_cmd;        ///< Transport command
    uint8_t sync_cmd_trans_chid;       ///< Transport ChID
} psi5s_sync_cmd_setting_t;

/** PSI5S Receive MailBox data */
typedef struct st_psi5s_mailbox
{
    uint32_t rx_data_status;           ///< Data status MailBox
    uint32_t rx_data;                  ///< Data MailBox
    uint32_t rx_timestamp;             ///< Timestamp MailBox
    bool     rx_unread;                ///< Unread RX data in mailbox
} psi5s_mailbox_t;

/** PSI5S transmit information */
typedef struct st_psi5s_tx_info
{
    uint8_t         command_id;              ///< Command identifier
    uint8_t         chid;                    ///< Channel ID
    uint8_t         address;                 ///< Address
    uint8_t         data_len;                ///< Data length in bytes
    uint8_t const * data;                    ///< User data buffer

    psi5s_ddsr_trans_type_t ddsr_trans_type; ///< DDSR transmit data frames type
    uint32_t                tx_data;         ///< DDSR transmit data
} psi5s_tx_info_t;

/** Callback function parameter data. */
typedef struct st_psi5s_callback_args
{
    psi5s_packet_frame_t packet_frame; ///< Error packet frame
    psi5s_event_t        event;        ///< Event code
    psi5s_channel_t      channel;      ///< PSI5S channel
    void const         * p_context;    ///< Placeholder for user data.  Set in psi5s_api_t::open function in ::psi5s_cfg_t.
} psi5s_callback_args_t;

/** Configuration for PSI5S unit */
typedef struct st_psi5s_cfg
{
    /* Generic configuration */
    uint8_t               unit;                              ///< PSI5S unit
    psi5s_clock_status_t  clk_bus_en;                        ///< PSI5S clock output status
    psi5s_noise_filter_t  noise_filter_setting;              ///< PSI5S noise filter setting
    psi5s_loopback_mode_t loopback_mode;                     ///< PSI5S loopback test mode

    /* Interrupt configuration */
    IRQn_Type const * communication_irq;                     ///< PSI5S Communication interrupt for each channel
    uint16_t          communication_ipl;                     ///< PSI5S Communication interrupt priority level for all channel
    uint32_t const  * psi5s_cpu_int_cfg;                     ///< PSI5S CPU interrupt config

    /* PSI5S mode configuration */
    psi5s_sync_cmd_setting_t const * psi5s_sync_cmd_setting; ///< PSI5S sync command config for each channel
    uint8_t frame_gap;                                       ///< PSI5S frame gap config for all channel

    /* Parameters to control software behavior */
    void (* p_callback)(psi5s_callback_args_t * p_args);     ///< Pointer to callback function.
    void const * p_context;                                  ///< User defined context passed into callback function.

    /* Implementation-specific configuration */
    void const * p_extend;                                   ///< Any configuration data needed by the hardware
} psi5s_cfg_t;

/** PSI5S control block.  Allocate an instance specific control block to pass into the PSI5S API calls.
 */
typedef void psi5s_ctrl_t;

/** PSI5S functions implemented at the HAL layer API. */
typedef struct st_psi5s_api
{
    /** Open the PSI5S driver.
     *
     * @param[in]     p_ctrl    Pointer to PSI5S instance control block.
     * @param[in]     p_cfg     Pointer to PSI5S instance configuration structure.
     */
    fsp_err_t (* open)(psi5s_ctrl_t * const p_ctrl, psi5s_cfg_t const * const p_cfg);

    /** Close PSI5S driver.
     *
     * @param[in]     p_ctrl    Pointer to PSI5S instance control block.
     */
    fsp_err_t (* close)(psi5s_ctrl_t * const p_ctrl);

    /** start PSI5S driver.
     *
     * @param[in]     p_ctrl    Pointer to PSI5S instance control block.
     */
    fsp_err_t (* start)(psi5s_ctrl_t * const p_ctrl);

    /** stop PSI5S driver.
     *
     * @param[in]     p_ctrl    Pointer to PSI5S instance control block.
     */
    fsp_err_t (* stop)(psi5s_ctrl_t * const p_ctrl);

    /** Transmitting channel data.
     *
     * @param[in]   p_api_ctrl                  Pointer to control block
     * @param[in]   channel                     Channel send data
     * @param[in]   p_tx_info                   Pointer to command block (command_id, chid, address, data_len, data[])
     */
    fsp_err_t (* write)(psi5s_ctrl_t * const p_api_ctrl, psi5s_channel_t channel, psi5s_tx_info_t const * p_tx_info);

    /** Get mailbox data.
     *
     * @param[in]   p_api_ctrl                  Pointer to control block
     * @param[in]   channel                     Channel get data
     * @param[out]  p_mailbox_data              Pointer to channel mailbox block
     */
    fsp_err_t (* read)(psi5s_ctrl_t * const p_api_ctrl, psi5s_mailbox_t * p_mailbox_data, psi5s_channel_t channel);

    /** Specify callback function and optional context pointer and working memory pointer.
     *
     * @param[in]   p_ctrl                   Pointer to PSI5S instance control block.
     * @param[in]   p_callback               Callback function
     * @param[in]   p_context                Pointer to send to callback function
     * @param[in]   p_callback_memory        Pointer to volatile memory where callback structure can be allocated.
     *                                       Callback arguments allocated here are only valid during the callback.
     */
    fsp_err_t (* callbackSet)(psi5s_ctrl_t * const p_ctrl, void (* p_callback)(psi5s_callback_args_t *),
                              void const * const p_context, psi5s_callback_args_t * const p_callback_memory);
} psi5s_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_psi5s_instance
{
    psi5s_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    psi5s_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    psi5s_api_t const * p_api;         ///< Pointer to the API structure for this instance
} psi5s_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup psi5s_API)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
