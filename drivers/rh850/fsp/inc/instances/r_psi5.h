/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup PSI5
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

#ifndef R_PSI5_H
#define R_PSI5_H

#include "r_psi5_api.h"
#include "r_psi5_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* PRQA S 1536 ++ # Advisory R-2.4 */
/* PRQA S 1336 ++ # Required R-8.2 */
/** Select the timestamp master */
typedef enum e_psi5_timestamp_master_sel
{
    PSI5_NO_MASTER = 0U,                  ///< No Timestamp master
    PSI5_TIMESTAMP_MODE_MASTER_SEL_PSI50, ///< PSI50 is the timestamp master
    PSI5_TIMESTAMP_MODE_MASTER_SEL_PSI51, ///< PSI51 is the timestamp master
    PSI5_TIMESTAMP_MODE_MASTER_SEL_PSI52, ///< PSI52 is the timestamp master
    PSI5_TIMESTAMP_MODE_MASTER_SEL_PSI53  ///< PSI53 is the timestamp master
} psi5_timestamp_master_sel_t;            /* PRQA S 1535 # Advisory R-2.3 */

/** List of PSI5's start operation mode*/
typedef enum e_psi5_start_operation_mode
{
    PSI5_START_MODE_NORMAL = 0U,       ///< Start channel with normal operation
    PSI5_START_MODE_ASYNC_PULSE        ///< Start channel with asynchronous sync pulse
} psi5_start_operation_mode_t;

/** Sampling timing setting */
typedef struct st_psi5_rx_sampling_setting
{
    uint8_t  sampling_interval;            ///< Sampling interval (sampling period) counter for received data
    uint32_t non_data_tgl_detec_start_pos; ///< Specifies the non-data toggle detection start position
    uint32_t non_data_tgl_detect_end_pos;  ///< Specifies the non-data toggle detection end position
    uint16_t tgl_detect_start_pos;         ///< Specifies the toggle detection start position
    uint8_t  tgl_detect_end_pos;           ///< Specifies the toggle detection end position
} psi5_rx_sampling_setting_t;

/** Received slot setting */
typedef struct st_psi5_rx_slot_setting
{
    psi5_rx_slot_t slot_enable;                             ///< Enable corresponding slot to rcv data
    psi5_rx_serial_message_channel_t serial_message_enable; ///< Enable corresponding slot to rcv serial message
    psi5_operation_mode_t            operation_mode;        ///< Specifies the operation mode
    psi5_rx_err_detect_t             crc_parity_sel;        ///< CRC or parity selection for slots 1 to 8
    uint8_t  data_length;                                   ///< Data length
    uint32_t offset_counter;                                ///< Offset counter
} psi5_rx_slot_setting_t;

/** Reception setting - Sensor to ECU communication*/
typedef struct st_psi5_rx_setting
{
    psi5_rx_sampling_setting_t rx_sampl_timing; ///< Specifies sampling timing. This IP uses for 16 times sampling
    psi5_rx_slot_setting_t   * p_rx_slot;       ///< Specifies the receive slot setting
} psi5_rx_setting_t;

/** Transmission setting - ECU to Sensor communication */
typedef struct st_psi5_tx_setting
{
    uint8_t                sync_pulse_value; ///< Initial value of synchronization pulse
    uint16_t               long_count;       ///< Upper limit of "long" synchronization pulse width
    uint16_t               short_count;      ///< Upper limit of "short" synchronization pulse width
    psi5_tx_frame_format_t frame_format;     ///< Transmit frame format
    psi5_tx_mode_t         physical_layer;   ///< Synchronous pulse physic layer mode
} psi5_tx_setting_t;

/* Start operation setting */
typedef struct st_psi5_start_op_setting
{
    psi5_start_operation_mode_t start_operation_mode; ///< Indicate start operation mode
    uint16_t delay_count;                             ///< Indicate delay count (us)
} psi5_start_op_setting_t;

/** Control block used by driver. DO NOT INITIALIZE - this structure will be initialized in @ref psi5_api_t::open. */
typedef struct st_psi5_instance_ctrl
{
    uint32_t           open;                     ///< Driver ID
    R_PSI50_Type     * p_reg;                    ///< Pointer to PSI5 unit register base address
    R_SELB_PSI5_Type * p_defsel;                 ///< Pointer to PSI5 selection source register base address
#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    R_WP_PSI5_Type * p_wp_reg;                   ///< Pointer to PSI5 wrapper register base address
#endif
    uint32_t const * p_tx_source;                ///< Pointer to transmission buffer
    psi5_status_t    p_sts_buff;                 ///< PSI5 status buffer
    void (* p_callback)(psi5_callback_args_t *); ///< Pointer to callback
    psi5_callback_args_t * p_callback_memory;    ///< Pointer to optional callback argument memory
    void const           * p_context;            ///< Pointer to context to be passed into callback function
    psi5_cfg_t const     * p_cfg;                ///< Pointer to configuration block
} psi5_instance_ctrl_t;

/** PSI5 Extended Configuration */
typedef struct st_psi5_extended_cfg
{
    psi5_tx_setting_t        tx_setting;             ///< Transmit setting
    psi5_rx_setting_t        rx_setting;             ///< Receive setting
    psi5_timestamp_setting_t timestamp_setting;      ///< Timestamp setting
#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    psi5_start_op_setting_t start_operation_setting; ///< Start operation mode
    IRQn_Type               channel_en_irq;          ///< PSI5 reports in the end of all channel enable interrupt
    uint8_t                 channel_en_ipl;          ///< Channel enable interrupt priority level
#endif
} psi5_extended_cfg_t;

/* PRQA S 1536 -- */

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const psi5_api_t g_psi5_on_psi5;

/** @endcond */

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/

/* PRQA S 1710 ++ # Advisory R-4.5 */
fsp_err_t R_PSI5_Open(psi5_ctrl_t * const p_api_ctrl, psi5_cfg_t const * const p_cfg);
fsp_err_t R_PSI5_Close(psi5_ctrl_t * const p_api_ctrl);
fsp_err_t R_PSI5_Start(psi5_ctrl_t * const p_api_ctrl);
fsp_err_t R_PSI5_Stop(psi5_ctrl_t * const p_api_ctrl);
fsp_err_t R_PSI5_ReadDataFrame(psi5_ctrl_t * const p_api_ctrl, psi5_data_frame_t * p_dataFrame, uint8_t slot_num);
fsp_err_t R_PSI5_ReadMessageFrame(psi5_ctrl_t * const           p_api_ctrl,
                                  psi5_serial_message_frame_t * p_msgFrame,
                                  uint8_t                       slot_num);
fsp_err_t R_PSI5_StatusGet(psi5_ctrl_t * const p_api_ctrl, psi5_status_t * p_status);
fsp_err_t R_PSI5_CallbackSet(psi5_ctrl_t * const          p_api_ctrl,
                             void (                     * p_callback)(psi5_callback_args_t *),
                             void const * const           p_context,
                             psi5_callback_args_t * const p_callback_memory);
fsp_err_t R_PSI5_TimestampSet(psi5_ctrl_t * const p_api_ctrl, psi5_timestamp_setting_t const * const p_timestamp_cfg);
fsp_err_t R_PSI5_SendChannelData(psi5_ctrl_t * const p_api_ctrl, uint32_t const * const p_src);

/* PRQA S 1710 -- */
/* PRQA S 1336 -- */

/*******************************************************************************************************************//**
 * @} (end addtogroup PSI5)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_PSI5_H
