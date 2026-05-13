/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************/ /**
 * @addtogroup DMAC
 * @{
 **********************************************************************************************************************/

#ifndef R_DMAC_H
#define R_DMAC_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_transfer_api.h"
#include "r_dmac_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Max configurable number of transfers in TRANSFER_MODE_NORMAL. */
#define DMAC_MAX_NORMAL_TRANSFER_LENGTH    (0xFFFF)

/* Max number of transfers per repeat for TRANSFER_MODE_REPEAT. */
#define DMAC_MAX_BLOCK_TRANSFER_LENGTH     (0xFFFF)

/* Transfer Control Register offset */
#define DMAC_PRV_TMR_SLM_OFFSET            (20U)
#define DMAC_PRV_TMR_PR_OFFSET             (16U)
#define DMAC_PRV_TMR_TRS_OFFSET            (12U)
#define DMAC_PRV_TMR_DM_OFFSET             (10U)
#define DMAC_PRV_TMR_SM_OFFSET             (8U)
#define DMAC_PRV_TMR_DTS_OFFSET            (4U)

/* The resource select RS[7:0] bit in the DMAjRS_n register */
#define DMAC_PRV_RS_OFFSET                 (0U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Events that can trigger a callback function. */
typedef enum e_dmac_event
{
    DMAC_EVENT_SW_TRANS_RQ_FLAG  = 0x01 << 0,  ///< Software DMA transfer request.
    DMAC_EVENT_HW_TRANS_RQ_FLAG  = 0x01 << 1,  ///< Hardware DMA transfer request.
    DMAC_EVENT_TRANS_COMPLETION  = 0x01 << 4,  ///< Completion of the last transfer and shows whether the DMA transfer is complete.
    DMAC_EVENT_TRANS_COUNT_MATCH = 0x01 << 5,  ///< Remaining transfer count is the same as the value set in transfer compare register.
    DMAC_EVENT_ERR_TRANS         = 0x01 << 7,  ///< DMA transfer error occurs.
    DMAC_EVENT_DMA_ON_GOING      = 0x01 << 8,  ///< DMA cycle is ongoing in this channel.
    DMAC_EVENT_ERR_TRANS_CYCLE   = 0x01 << 11, ///< DMA transfer error has occurred at read cycle or at write cycle.
} dmac_event_t;

/** DMAC channel scheduling. */
typedef enum e_dmac_channel_scheduling
{
    DMAC_CHANNEL_SCHEDULING_FIXED       = 0, ///< Fixed priority mode.
    DMAC_CHANNEL_SCHEDULING_ROUND_ROBIN = 3, ///< Round-robin mode.
} dmac_channel_scheduling_t;

/** Gather function enable. */
typedef enum e_dmac_gather_mode
{
    DMAC_GATHER_MODE_DISABLE = 0,      ///< Gathering is disabled.
    DMAC_GATHER_MODE_ENABLE  = 1,      ///< Gathering is enabled.
} dmac_gather_mode_t;

/** Scatter function enable. */
typedef enum e_dmac_scatter_mode
{
    DMAC_SCATTER_MODE_DISABLE = 0,     ///< Scattering is disabled.
    DMAC_SCATTER_MODE_ENABLE  = 1,     ///< Scattering is enabled.
} dmac_scatter_mode_t;

/** Zero fill enable. */
typedef enum e_dmac_zero_fill_mode
{
    DMAC_ZERO_FILL_MODE_DISABLE = 0,   ///< Zero fill is disabled.
    DMAC_ZERO_FILL_MODE_ENABLE  = 1,   ///< Zero fill is enabled.
} dmac_zero_fill_mode_t;

/** Descriptor Support Operation. */
typedef enum e_dmac_support_operation
{
    DMAC_SUPPORT_OPERATION_NORMAL             = 0, ///< Normal Descriptor Operations
    DMAC_SUPPORT_OPERATION_SPECIFIC           = 1, ///< Specific Order Operations
    DMAC_SUPPORT_OPERATION_STEP_END_INTERRUPT = 2, ///< Descriptor Step End Interrupt
    DMAC_SUPPORT_OPERATION_DOUBLE_BUFFER      = 3, ///< Double Buffer Operation
    DMAC_SUPPORT_OPERATION_REPEAT             = 4, ///< Repeat Operation
    DMAC_SUPPORT_OPERATION_HARDWARE_REQUEST   = 5, ///< Hardware Request Operation
} dmac_support_operation_t;

/** Descriptor Steps Operation. */
typedef enum e_dmac_steps_operation
{
    DMAC_STEPS_OPERATION_2 = 2,        ///< Descriptor 2 steps Operation
    DMAC_STEPS_OPERATION_3 = 3,        ///< Descriptor 3 steps Operation
    DMAC_STEPS_OPERATION_4 = 4,        ///< Descriptor 4 steps Operation
} dmac_steps_operation_t;

/** Callback function parameter data. */
typedef struct st_dmac_callback_args
{
    dmac_event_t event;                ///< Event code
    void const * p_context;            ///< Placeholder for user data.  Set in r_transfer_t::open function in ::transfer_cfg_t.
} dmac_callback_args_t;

/** Gather function parameter data. */
typedef struct st_dmac_gather_setting
{
    dmac_gather_mode_t gather_mode;               ///< Gather enable on destination side
    uint32_t           gather_repeat_count_inner; ///< Repeat count of the inner DMA loop if gathering is enabled
    uint32_t           gather_inner_address;      ///< The source address increment for the inner loop of a gather transfer
    uint32_t           gather_outer_address;      ///< The source address increment for the outer loop of a gather transfer
} dmac_gather_setting_t;

/** Scatter function parameter data. */
typedef struct st_dmac_scatter_setting
{
    dmac_scatter_mode_t   scatter_mode;               ///< Scatter enable on source side
    dmac_zero_fill_mode_t zero_fill_mode;             ///< Zero fill
    uint32_t              scatter_repeat_count_inner; ///< Repeat count of the inner DMA loop if scattering is enabled
    uint32_t              scatter_inner_address;      ///< the destination address increment for the inner loop of a scatter transfer
    uint32_t              scatter_outer_address;      ///< The destination address increment for the outer loop of a scatter transfer
} dmac_scatter_setting_t;

/** Setting structure for descriptor transfer */
typedef struct st_dmac_descriptor_cfg
{
    void const * volatile p_src;           ///< Source pointer
    void * volatile       p_dest;          ///< Destination Address
    uint32_t              number_transfer; ///< Transfer Size
    union
    {
        uint32_t descriptor_pointer_u32;
        struct
        {
            uint32_t continuation_flag       : 1;  ///< [0..0] Continuation flag of descriptor
            uint32_t descriptor_interrupt_en : 1;  ///< [1..1] Descriptor Interrupt enable
            uint32_t address_pointer         : 10; ///< [2..11] Address pointer of Descriptor
            uint32_t                         : 20;
        } descriptor_pointer_u32_b;
    } descriptor_pointer_cfg;
} dmac_descriptor_cfg_t;

/** Control block used by driver. DO NOT INITIALIZE - this structure will be initialized in @ref transfer_api_t::open. */
typedef struct st_dmac_instance_ctrl
{
    uint32_t open;                                   ///< Driver ID

    /* Pointer to base register. */
    R_DMAC0_Type     * p_reg;                        ///< Pointer to DMAC unit register base address
    R_DMATRGSEL_Type * p_defsel;                     ///< Pointer to DMAC selection source register base address

    transfer_cfg_t const * p_cfg;
    uint8_t                count_last_descriptor;    ///< Counter last descriptor

    void (* p_callback)(transfer_callback_args_t *); ///< Pointer to callback
    transfer_callback_args_t * p_callback_memory;    ///< Pointer to optional callback argument memory
    void const               * p_context;            ///< Pointer to context to be passed into callback function
} dmac_instance_ctrl_t;

/** DMAC transfer configuration extension. This extension is required. */
typedef struct st_dmac_extended_cfg
{
    uint8_t   unit;                                                   ///< Unit number, does not apply to all HAL drivers.
    uint8_t   channel;                                                ///< Channel number, does not apply to all HAL drivers.
    IRQn_Type irq;                                                    ///< DMAC interrupt number
    uint8_t   irq_priority_level;                                     ///< DMAC interrupt priority level

    IRQn_Type irq_error;                                              ///< DMAC interrupt error number
    uint8_t   irq_error_priority_level;                               ///< DMAC interrupt error priority level

    IRQn_Type irq_descriptor;                                         ///< DMAC interrupt descriptor number
    uint8_t   irq_descriptor_pri_level;                               ///< DMAC interrupt descriptor priority level

    /** Select which event will trigger the transfer. */
    dmac_trigger_event_t activation_source;                           ///< DMAC hardware request activation source

    /* Channel priority request */
    dmac_channel_scheduling_t channel_scheduling;                     ///< DMA channel scheduling
    bool     ch_req_pri_en;                                           ///< Channel Request Priority Setting enable
    uint8_t  ch_req_pri_num;                                          ///< Channel Request Priority Setting number
    uint16_t ch_req_pri_cnt_val;                                      ///< Channel Request Priority Count value

    /* Descriptor Operations config */
    dmac_descriptor_cfg_t  * p_descriptor;                            ///< Descriptor Operations
    dmac_support_operation_t support_operation;                       ///< Descriptor Support Operations
    bool descriptor_enable;                                           ///< Descriptor Operations enable

    /* Gather config */
    dmac_gather_setting_t gather_setting;                             ///< Gather function transfer

    /* Scatter config */
    dmac_scatter_setting_t scatter_setting;                           ///< Scatter function transfer

    /** Callback for transfer end interrupt. */
    void (* p_callback)(transfer_callback_args_t * cb_data);          ///< Pointer to callback that is called when an isr happen
    transfer_callback_args_t * p_callback_memory;                     ///< Pointer to non-secure memory that can be used to pass arguments to a callback in non-secure memory

    /** Placeholder for user data.  Passed to the user p_callback in ::transfer_callback_args_t. */
    void const * p_context;                                           ///< User defined context passed into callback function

    transfer_switchable_redundant_cfg_t * p_switchable_redundant_cfg; ///< Structure for switchable redundant operation
} dmac_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const transfer_api_t g_transfer_on_dmac;

/** @endcond */

/***********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Open(transfer_ctrl_t * const p_api_ctrl, transfer_cfg_t const * const p_cfg);
fsp_err_t R_DMAC_Reconfigure(transfer_ctrl_t * const p_api_ctrl, transfer_info_t * p_info);
fsp_err_t R_DMAC_Reset(transfer_ctrl_t * const p_api_ctrl,
                       void const * volatile   p_src,
                       void * volatile         p_dest,
                       uint16_t const          num_transfers);
fsp_err_t R_DMAC_SoftwareStart(transfer_ctrl_t * const p_api_ctrl, transfer_start_mode_t mode);
fsp_err_t R_DMAC_SoftwareStop(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DMAC_Enable(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DMAC_Disable(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DMAC_InfoGet(transfer_ctrl_t * const p_api_ctrl, transfer_properties_t * const p_info);
fsp_err_t R_DMAC_CallbackSet(transfer_ctrl_t * const          p_api_ctrl,
                             void (                         * p_callback)(transfer_callback_args_t *),
                             void * const                     p_context,
                             transfer_callback_args_t * const p_callback_memory);
fsp_err_t R_DMAC_Close(transfer_ctrl_t * const p_api_ctrl);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************/ /**
 * @} (end defgroup DMAC)
 **********************************************************************************************************************/
