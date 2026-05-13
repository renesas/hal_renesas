/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @addtogroup DTSC
 * @{
 **********************************************************************************************************************/
#ifndef R_DTSC_H_
#define R_DTSC_H_

#include "bsp_api.h"
#include "r_transfer_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Events that can trigger a callback function. */
typedef enum e_dtsc_event
{
    DTSC_EVENT_TRANS_RQ_FLAG     = 0x01 << 0,  ///< DTSC transfer request in this channel is being held.
    DTSC_EVENT_TRANS_OVERFLOW    = 0x01 << 1,  ///< DTSC Transfer request overflow.
    DTSC_EVENT_TRANS_COMPLETION  = 0x01 << 4,  ///< The last transfer will generate this event for transfer completion
    DTSC_EVENT_TRANS_COUNT_MATCH = 0x01 << 5,  ///< The number of transfer count match the value set in transfer compare register.
    DTSC_EVENT_ERR_TRANS         = 0x01 << 7,  ///< DTSC transfer error occurs.
    DTSC_EVENT_ERR_TRANS_CYCLE   = 0x01 << 11, ///< DTSC transfer error has occurred at read cycle or at write cycle.
} dtsc_event_t;

/** Callback function parameter data. */
typedef struct st_dtsc_callback_args
{
    dtsc_event_t event;                ///< Event code
    void const * p_context;            ///< Placeholder for user data.  Set in @ref transfer_api_t::open function in ::transfer_cfg_t.
} dtsc_callback_args_t;

/** Control block used by driver. DO NOT INITIALIZE - this structure will be initialized in @ref transfer_api_t::open.*/
typedef struct st_dtsc_instance_ctrl
{
    uint32_t open;                                   ///< Driver ID

    /* Pointer to base register. */
    R_DTS0_Type      * p_reg;                        ///< Pointer to DTSC unit register base address
    R_DTSTRGSEL_Type * p_hwsel;                      ///< Pointer to DTSC selection hardware source

    transfer_cfg_t const * p_cfg;                    ///< Pointer to the configuration structure
    void (* p_callback)(transfer_callback_args_t *); ///< Pointer to callback
    transfer_callback_args_t * p_callback_memory;    ///< Pointer to optional callback argument memory
    void const               * p_context;            ///< Pointer to context to be passed into callback function
} dtsc_instance_ctrl_t;

/** DTSC transfer configuration extension. This extension is required. */
typedef struct st_dtsc_extended_cfg
{
    uint8_t   channel;                                       ///< Channel number, does not apply to all HAL drivers.
    uint8_t   transfer_complete_ipl;                         ///< DTSC transfer completion priority level
    uint8_t   transfer_count_match_ipl;                      ///< DTSC transfer count match priority level
    uint8_t   transfer_error_ipl;                            ///< DTSC transfer error priority level
    IRQn_Type complete_irq;                                  ///< DTSC transfer completion interrupt
    IRQn_Type count_match_irq;                               ///< DTSC transfer count match interrupt
    IRQn_Type error_irq;                                     ///< DTSC transfer error interrupt

    /** Callback for transfer end interrupt. */
    void (* p_callback)(transfer_callback_args_t * cb_data); ///< Pointer to callback that is called when an isr happen

    transfer_callback_args_t * p_callback_memory;            ///< Pointer to optional callback argument memory

    /** Select which event will trigger the transfer. */
    dtsc_trigger_event_t activation_source;                  ///< DTSC interrupt priority level

    /** Chain Restriction Config*/
    transfer_chain_restrict_cfg_t * p_chain_restrict_cfg;    ///< Confi for the chain restriction of each channel

    /** Placeholder for user data.  Passed to the user p_callback in ::transfer_callback_args_t. */
    void const * p_context;                                  ///< User defined context passed into callback function
} dtsc_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const transfer_api_t g_transfer_on_dtsc;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Open(transfer_ctrl_t * const p_api_ctrl, transfer_cfg_t const * const p_cfg);
fsp_err_t R_DTSC_SoftwareStart(transfer_ctrl_t * const p_api_ctrl, transfer_start_mode_t mode);
fsp_err_t R_DTSC_InfoGet(transfer_ctrl_t * const p_api_ctrl, transfer_properties_t * const p_info);
fsp_err_t R_DTSC_SoftwareStop(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DTSC_Enable(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DTSC_Disable(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DTSC_Close(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DTSC_Reconfigure(transfer_ctrl_t * const p_api_ctrl, transfer_info_t * p_info);
fsp_err_t R_DTSC_Reload(transfer_ctrl_t * const p_api_ctrl,
                        void const            * p_src,
                        void                  * p_dest,
                        uint32_t const          num_transfers);
fsp_err_t R_DTSC_Reset(transfer_ctrl_t * const p_api_ctrl,
                       void const * volatile   p_src,
                       void * volatile         p_dest,
                       uint16_t const          num_transfers);

fsp_err_t R_DTSC_CallbackSet(transfer_ctrl_t * const          p_api_ctrl,
                             void (                         * p_callback)(transfer_callback_args_t *),
                             void * const                     p_context,
                             transfer_callback_args_t * const p_callback_memory);

/*******************************************************************************************************************//**
 * @} (end defgroup DTSC)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
