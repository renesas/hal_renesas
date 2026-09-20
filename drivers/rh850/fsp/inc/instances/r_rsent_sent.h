/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */
#ifndef R_RSENT_SENT_H
#define R_RSENT_SENT_H

/*******************************************************************************************************************//**
 * @addtogroup RSENT_SENT
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_sent_api.h"
#include "r_rsent_sent_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** RSENT communication clock configuration */
/* PRQA S 0750 ++ # Advisory R-19.2 */
typedef union u_rsent_com_clk_cfg      /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t all;

    /** RSENT communication clock configuration */
    struct
    {
        uint32_t scmv      : 5;        ///< Sample clock multiplication value.
        uint32_t reserved0 : 3;        ///< Reserved.
        uint32_t scdv      : 7;        ///< Sample clock division value.
        uint32_t reserved1 : 1;        ///< Reserved.
        uint32_t tti       : 7;        ///< Time tick integer.
        uint32_t reserved2 : 1;        ///< Reserved.
        uint32_t ttf       : 4;        ///< Time tick decimal fraction.
    } bit;
} rsent_com_clk_cfg_t;
/* PRQA S 0750 -- */
/** RSENT timestamp function configuration */
/* PRQA S 0750 ++ # Advisory R-19.2 */
typedef union u_rsent_timestamp_cfg    /* PRQA S 1536 # Advisory R-2.4 */
{
    /* PRQA S 0750 # Advisory R-19.2 */
    uint32_t all;

    /** RSENT timestamp function configuration */
    struct
    {
        uint32_t ttpv           : 7;   ///< Timestamp tick prescaler value.
        uint32_t reserved0      : 1;   ///< Reserved.
        uint32_t ttm            : 7;   ///< Timestamp tick multiplier value.
        uint32_t reserved1      : 1;   ///< Reserved.
        uint32_t timestamp_mode : 1;   ///< Configure timestamp mode (master/slave).
    } bit;
} rsent_timestamp_cfg_t;
/* PRQA S 0750 -- */
/** RSENT timestamp master */
typedef enum e_rsent_sent_timestamp_master                 /* PRQA S 1536 # Advisory R-2.4 */
{
    RSENT_SENT_TIMESTAMP_MASTER_NO_MASTER = 0U,            ///< No timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT0    = 1U,            ///< Select RSENT0 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT1    = 2U,            ///< Select RSENT1 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT2    = 3U,            ///< Select RSENT2 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT3    = 4U,            ///< Select RSENT3 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT4    = 5U,            ///< Select RSENT4 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT5    = 6U,            ///< Select RSENT5 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT6    = 7U,            ///< Select RSENT6 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT7    = 8U,            ///< Select RSENT7 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT8    = 9U,            ///< Select RSENT8 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT9    = 10U,           ///< Select RSENT9 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT10   = 11U,           ///< Select RSENT10 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT11   = 12U,           ///< Select RSENT11 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT12   = 13U,           ///< Select RSENT12 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT13   = 14U,           ///< Select RSENT13 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT14   = 15U,           ///< Select RSENT14 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT15   = 16U,           ///< Select RSENT15 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT16   = 17U,           ///< Select RSENT16 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT17   = 18U,           ///< Select RSENT17 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT18   = 19U,           ///< Select RSENT18 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT19   = 20U,           ///< Select RSENT19 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT20   = 21U,           ///< Select RSENT20 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT21   = 22U,           ///< Select RSENT21 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT22   = 23U,           ///< Select RSENT22 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT23   = 24U,           ///< Select RSENT23 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT24   = 25U,           ///< Select RSENT24 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT25   = 26U,           ///< Select RSENT25 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT26   = 27U,           ///< Select RSENT26 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT27   = 28U,           ///< Select RSENT27 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT28   = 29U,           ///< Select RSENT28 as timestamp master.
    RSENT_SENT_TIMESTAMP_MASTER_RSENT29   = 30U            ///< Select RSENT29 as timestamp master.
} rsent_sent_timestamp_master_t;

/** RSENT enable/disable synchronization on start */
typedef enum e_rsent_timestamp_synchronization_on_start    /* PRQA S 1536 # Advisory R-2.4 */
{
    RSENT_TIMESTAMP_SYNCHRONIZATION_ON_START_DISABLE = 0U, ///< Disable timestamp synchronization on start.
    RSENT_TIMESTAMP_SYNCHRONIZATION_ON_START_ENABLE  = 1U  ///< Enable timestamp synchronization on start.
} rsent_timestamp_synchronization_on_start_t;

/** RSENT enable/disable status interrupts configure */
/* PRQA S 0750 ++ # Advisory R-19.2 */
typedef union u_rsent_error_irq_cfg                        /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t all;

    /** RSENT enable/disable status interrupts configure */
    struct
    {
        uint32_t reserved0 : 1;        ///< Reserved.
        uint32_t fcie      : 1;        ///< 1: Enable/0: Disable Fast Channel CRC Error Interrupt.
        uint32_t fmie      : 1;        ///< 1: Enable/0: Disable Fast Channel Message Lost Interrupt.
        uint32_t feie      : 1;        ///< 1: Enable/0: Disable Fast Channel Nibble Encoding Error Interrupt.
        uint32_t fnie      : 1;        ///< 1: Enable/0: Disable Fast Channel Nibble Count Error Interrupt.
        uint32_t clie      : 1;        ///< 1: Enable/0: Disable Calibration Pulse Length Error Interrupt.
        uint32_t cvie      : 1;        ///< 1: Enable/0: Disable Calibration Pulse Length Variation Error Interrupt.
        uint32_t nrie      : 1;        ///< 1: Enable/0: Disable No Response Error Interrupt Enable.
        uint32_t scie      : 1;        ///< 1: Enable/0: Disable Slow Channel CRC Error Interrupt Enable.
        uint32_t smie      : 1;        ///< 1: Enable/0: Disable Slow Channel Message Lost Interrupt.
        uint32_t seie      : 1;        ///< 1: Enable/0: Disable Slow Channel Encoding Error Interrupt.
    } bit;
} rsent_error_irq_cfg_t;
/* PRQA S 0750 -- */
/** RSENT extend configuration structure */
typedef struct st_rsent_sent_extended_cfg                                          /* PRQA S 1536 # Advisory R-2.4 */
{
    rsent_com_clk_cfg_t                        com_clk_cfg;                        ///< Configure baurate, clock tick length.
    rsent_timestamp_cfg_t                      timestamp_cfg;                      ///< Configure timestamp function.
    rsent_sent_timestamp_master_t              timestamp_master;                   ///< Configure timestamp master.
    rsent_timestamp_synchronization_on_start_t timestamp_synchronization_on_start; ///< Configure timestamp synchronization on start.
    uint32_t              timestamp_init_value;                                    ///< Configure timestamp initial value.
    uint32_t              timestamp_start_value;                                   ///< Configure timestamp value written on start.
    rsent_error_irq_cfg_t error_irq_cfg;                                           ///< Enable/Disable each individual status interrupt.
#if defined(BSP_FEATURE_DEVICE_HAS_INTERRUPT_MERGE)
    bsp_imf_info_t rxi_imf_info;                                                   ///< Receive Interrupt Merge Information.
    bsp_imf_info_t eri_imf_info;                                                   ///< Error Interrupt Merge Information.
#endif
} rsent_sent_extended_cfg_t;

/** RSENT fast channel message */
typedef struct st_rsent_sent_fast_msg  /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t data;                     ///< Fast channel message data: 1->8 nibble(s).
    uint8_t  status_communication;     ///< Fast channel status and communication.
#if (RSENT_SENT_CFG_FIXED_MSG_LENGTH)
    uint32_t calibration_pulse_length; ///< Calibration pulse length in fixed message length mode.
    uint32_t msg_length;               ///< Message length in fixed message length mode.
#endif
#if (RSENT_SENT_CFG_FAST_MSG_TIMESTAMP)
    uint32_t timestamp;                ///< Fast channel message timestamp.
#endif
} rsent_sent_fast_msg_t;

/** RSENT slow channel message */
typedef struct st_rsent_sent_slow_msg  /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t data;                     ///< Slow channel message data.
    uint8_t  msg_id;                   ///< Slow channel message id.
    uint8_t  cfg_bit;                  ///< Slow channel message configuration bit (for enhanced serial message format).
#if (RSENT_SENT_CFG_SLOW_MSG_TIMESTAMP)
    uint32_t timestamp;                ///< Fast channel message timestamp.
#endif
} rsent_sent_slow_msg_t;

/** RSENT instance control block. */
typedef struct st_rsent_sent_instance_ctrl                                            /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t            open;                                                         ///< Store open status of unit.
    sent_cfg_t const  * p_cfg;                                                        ///< Pointer to the configuration block.
    R_RSENT0_Type     * p_reg;                                                        ///< Base register for this unit.
    R_RSENTTSSEL_Type * p_ts_spc_sel_reg;                                             ///< Base address for timestamp and SPC selection.
    uint32_t            fast_msg_rx_count;                                            ///< Number of fast channel messages received.
    uint32_t            fast_msg_rx_number;                                           ///< Number of fast channel messages to receive.
    void              * p_fast_msg_buffer;                                            ///< Pointer to buffer to store fast channel messages.
    uint32_t            slow_msg_rx_count;                                            ///< Number of slow channel messages received.
    uint32_t            slow_msg_rx_number;                                           ///< Number of slow channel messages to receive.
    void              * p_slow_msg_buffer;                                            ///< Pointer to buffer to store slow channel messages.
    void (* p_callback)(sent_callback_args_t *); /* PRQA S 1336 # DP_MSR_R-8.2_UC1 */ ///< Pointer to callback that is called when a sent_event_t occurs.
    sent_callback_args_t * p_callback_memory;                                         ///< Pointer to non-secure memory that can be used to pass arguments to a callback in non-secure memory.
    void const           * p_context;                                                 ///< Pointer to context to be passed into callback function.
} rsent_sent_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this instance. */
extern const sent_api_t g_sent_on_rsent;

/** @endcond */

fsp_err_t R_RSENT_SENT_Open(sent_ctrl_t * const p_ctrl, sent_cfg_t const * const p_cfg);
fsp_err_t R_RSENT_SENT_Close(sent_ctrl_t * const p_ctrl);
fsp_err_t R_RSENT_SENT_Start(sent_ctrl_t * const p_ctrl);
fsp_err_t R_RSENT_SENT_Stop(sent_ctrl_t * const p_ctrl);
fsp_err_t R_RSENT_SENT_FastMsgRead(sent_ctrl_t * const p_ctrl, void * p_dest, uint32_t const message_count);
fsp_err_t R_RSENT_SENT_SlowMsgRead(sent_ctrl_t * const p_ctrl, void * p_dest, uint32_t const message_count);
fsp_err_t R_RSENT_SENT_SPCWrite(sent_ctrl_t * const p_ctrl, uint8_t const pulse_length);
fsp_err_t R_RSENT_SENT_StatusGet(sent_ctrl_t * const p_ctrl, sent_status_t * p_status);
fsp_err_t R_RSENT_SENT_Abort(sent_ctrl_t * const p_ctrl, sent_communication_t const communication_to_abort);
fsp_err_t R_RSENT_SENT_CallbackSet(sent_ctrl_t * const          p_ctrl,
                                   void (                     * p_callback)(sent_callback_args_t *), /* PRQA S 1336 # DP_MSR_R-8.2_UC1 */
                                   void const * const           p_context,
                                   sent_callback_args_t * const p_callback_memory);
fsp_err_t R_RSENT_SENT_Select_SPCO(sent_ctrl_t * const p_ctrl, sent_spco_mode_t const mode);

/*******************************************************************************************************************//**
 * @} (end addtogroup RSENT_SENT)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
