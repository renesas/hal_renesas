/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include "bsp_api.h"                   /* PRQA S 0380 # Required R-1.1-C99 */
#include "r_psi5.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "PSI5" in ASCII, used to determine if channel is open */
#define PSI5_PRV_OPEN                                (0x50534935U)
#define PSI5_SELB_PSI5_REG                           ((R_SELB_PSI5_Type *) R_SELB_PSI5_BASE)
#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
 #define PSI5_WP_PSI5_REG                            ((R_WP_PSI5_Type *) R_WP_PSI5_BASE)
#endif
#define PSI5_PRV_MIN_SLOT                            (1U)
#define PSI5_PRV_MAX_SLOT                            (8U)
#define PSI5_PRV_NUMBER_TRANSFER                     (3U) /* PRQA S 1534 # Advisory R-2.5 */
#define PSI5_PRV_CLEARED                             (0U)
#define PSI5_PRV_SET                                 (1U)
#define PSI5_PRV_CHANNEL_DISABLED                    (0x00U)
#define PSI5_PRV_CHANNEL_ENABLED                     (0x01U)
#define PSI5_PRV_TX_FRAME_FORMAT_SHORT_MASK_BITS     (0x0000003FU)
#define PSI5_PRV_TX_FRAME_FORMAT_LONG_MASK_BITS      (0x0000FFFFU)
#define PSI5_PRV_TX_FRAME_FORMAT_XLONG_MASK_BITS     (0x003FFFFFU)
#define PSI5_PRV_TX_FRAME_FORMAT_XXLONG_MASK_BITS    (0x00FFFFFFU)
#define PSI5_DEST_RESET                              (0U) /* PRQA S 1534 # Advisory R-2.5 */
#define PSI5_SRC_RESET                               (0U) /* PRQA S 1534 # Advisory R-2.5 */
#define PSI5_NUM_TRANSFER_RESET                      (0U) /* PRQA S 1534 # Advisory R-2.5 */
#define PSI5_TIMESTAMP_UNIT_0_MASTER_SET             (0x01U)
#define PSI5_TIMESTAMP_UNIT_1_MASTER_SET             (0x02U)
#define PSI5_TIMESTAMP_UNIT_2_MASTER_SET             (0x03U)
#define PSI5_TIMESTAMP_UNIT_3_MASTER_SET             (0x04U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void r_psi5_init_setting(psi5_instance_ctrl_t                 * p_ctrl,
                                psi5_timestamp_setting_t const * const p_timestamp_cfg);
static void r_psi5_op_setting(psi5_instance_ctrl_t * p_ctrl);
static void r_psi5_rx_setting(psi5_instance_ctrl_t * p_ctrl);
static void r_psi5_tx_setting(psi5_instance_ctrl_t * p_ctrl);
static void r_psi5_timestamp_setting(psi5_instance_ctrl_t                 * p_ctrl,
                                     psi5_timestamp_setting_t const * const p_timestamp_cfg);
static void r_psi5_channel_set_master(psi5_instance_ctrl_t * p_ctrl);
static void r_psi5_channel_set_slave(psi5_instance_ctrl_t                 * p_ctrl,
                                     psi5_timestamp_setting_t const * const p_timestamp_cfg);
static void                  r_psi5_status_handler(psi5_instance_ctrl_t * p_ctrl);
static void                  r_psi5_read_data_frame(psi5_instance_ctrl_t * p_ctrl);
static void                  r_psi5_read_msg_frame(psi5_instance_ctrl_t * p_ctrl);
static void                  r_psi5_transmit_data(psi5_instance_ctrl_t * p_ctrl);
BSP_INTERRUPT_ATTRIBUTE void r_psi5_rx_isr(void);     /* PRQA S 3449, 3451 # Required R-8.5, Required R-8.5 */
BSP_INTERRUPT_ATTRIBUTE void r_psi5_tx_isr(void);     /* PRQA S 3449, 3451 # Required R-8.5, Required R-8.5 */
BSP_INTERRUPT_ATTRIBUTE void r_psi5_status_isr(void); /* PRQA S 3449, 3451 # Required R-8.5, Required R-8.5 */
static void                  r_psi5_call_callback(psi5_instance_ctrl_t * p_ctrl, psi5_event_t event);

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
static void                  r_psi5_init_start_operation(psi5_instance_ctrl_t * p_ctrl);
BSP_INTERRUPT_ATTRIBUTE void r_psi5_wp_isr(void); /* PRQA S 3449, 3451 # Required R-8.5, Required R-8.5 */

#endif

#if PSI5_CFG_TRANSFER_ENABLE                      /* PRQA S 3332 # Required R-20.9 */
static fsp_err_t r_psi5_dependent_drivers_configure(psi5_cfg_t const * const     p_cfg,
                                                    psi5_instance_ctrl_t * const p_instance_ctrl);
void r_psi5_rxi_transfer_callback(transfer_callback_args_t * p_transfer_args);

#endif

#if PSI5_CFG_PARAM_CHECKING_ENABLE
static fsp_err_t r_psi5_open_parameter_checking(psi5_ctrl_t * const p_api_ctrl, psi5_cfg_t const * const p_cfg);

#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

#if PSI5_CFG_TRANSFER_ENABLE           /* PRQA S 3332 # Required R-20.9 */
static psi5_data_frame_t psi5_rx_buffer;
#endif

/* PSI5 Unit Base Addresses */
const uint32_t g_psi5_unit_base_addr[] = /* PRQA S 3408, 1514 # Required R-8.4, Advisory R-8.7 */
{
#if defined(R_PSI50)
    (uint32_t) R_PSI50,                  /* PRQA S 0306, 0329 # Advisory R-11.4, Advisory R-11.4 */
#endif
#if defined(R_PSI51)
    (uint32_t) R_PSI51,                  /* PRQA S 0306, 0329 # Advisory R-11.4, Advisory R-11.4 */
#endif
#if defined(R_PSI52)
    (uint32_t) R_PSI52,                  /* PRQA S 0306, 0329 # Advisory R-11.4, Advisory R-11.4 */
#endif
#if defined(R_PSI53)
    (uint32_t) R_PSI53                   /* PRQA S 0306, 0329 # Advisory R-11.4, Advisory R-11.4 */
#endif
};

/* software buffer stores received data corresponding slot receive */
static psi5_data_frame_t psi5_rx_data[BSP_FEATURE_PSI5_MAX_CHANNEL][BSP_FEATURE_PSI5_NUM_SLOTS_EACH_UNIT];

/* software buffer stores received serial data (message) corresponding to slot receive */
static psi5_serial_message_frame_t psi5_rx_msg[BSP_FEATURE_PSI5_MAX_CHANNEL][BSP_FEATURE_PSI5_NUM_SLOTS_EACH_UNIT];

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* PSI5 function pointers   */
const psi5_api_t g_psi5_on_psi5 =                /* PRQA S 1502 # Advisory R-2.8 */
{
    .open             = R_PSI5_Open,             /* PRQA S 3635 # Advisory R-17.12 */
    .close            = R_PSI5_Close,            /* PRQA S 3635 # Advisory R-17.12 */
    .start            = R_PSI5_Start,            /* PRQA S 3635 # Advisory R-17.12 */
    .stop             = R_PSI5_Stop,             /* PRQA S 3635 # Advisory R-17.12 */
    .dataFrameRead    = R_PSI5_ReadDataFrame,    /* PRQA S 3635 # Advisory R-17.12 */
    .messageFrameRead = R_PSI5_ReadMessageFrame, /* PRQA S 3635 # Advisory R-17.12 */
    .statusGet        = R_PSI5_StatusGet,        /* PRQA S 3635 # Advisory R-17.12 */
    .callbackSet      = R_PSI5_CallbackSet,      /* PRQA S 3635 # Advisory R-17.12 */
    .write            = R_PSI5_SendChannelData,  /* PRQA S 3635 # Advisory R-17.12 */
    .timestampSet     = R_PSI5_TimestampSet      /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup PSI5
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Opens the driver.
 *
 * Implements @ref psi5_api_t::open().
 * @param[in]   p_api_ctrl                      Pointer to control block
 * @param[in]   p_cfg                           Pointer to configuration block
 * @retval FSP_SUCCESS                          Input Parameters are Valid.
 * @retval FSP_ERR_ASSERTION                    An input parameter is invalid.
 * @retval FSP_ERR_IRQ_BSP_DISABLED             Callback is NULL and the PSI5 IRQ is not enabled.
 * @retval FSP_ERR_ALREADY_OPEN                 The control structure is already opened.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_Open (psi5_ctrl_t * const p_api_ctrl, psi5_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    fsp_err_t              err      = FSP_SUCCESS;
    psi5_instance_ctrl_t * p_ctrl   = (psi5_instance_ctrl_t *) p_api_ctrl;             /* PRQA S 0316 # Advisory R-11.5 */
    psi5_extended_cfg_t  * p_extend = NULL;                                            /* PRQA S 3678 # Advisory R-8.13 */
    uint8_t                channel  = 0U;
    uint8_t                slot     = 0U;

#if PSI5_CFG_PARAM_CHECKING_ENABLE

    /* Check parameters. */
    err = r_psi5_open_parameter_checking(p_api_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif
    p_extend = (psi5_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # Required R-11.8, Advisory R-11.5 */

    /* Load configuration from application*/
    p_ctrl->p_cfg = p_cfg;

    channel = p_ctrl->p_cfg->channel;

    /* Set register call to PSI5 channel*/
    p_ctrl->p_reg    = (R_PSI50_Type *) g_psi5_unit_base_addr[channel]; /* PRQA S 0306 # Advisory R-11.4 */
    p_ctrl->p_defsel = PSI5_SELB_PSI5_REG;                              /* PRQA S 0306 # Advisory R-11.4 */
#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    p_ctrl->p_wp_reg = PSI5_WP_PSI5_REG;                                /* PRQA S 0306 # Advisory R-11.4 */
#endif

    /* Set callback and context pointers, if configured */
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    /* Initialize the buffer data */
    for (channel = 0U; channel < BSP_FEATURE_PSI5_MAX_CHANNEL; channel++)
    {
        for (slot = 0U; slot < BSP_FEATURE_PSI5_NUM_SLOTS_EACH_UNIT; slot++)
        {
            psi5_rx_data[channel][slot].rx_data        = 0U;
            psi5_rx_data[channel][slot].rx_data_status = 0U;
            psi5_rx_data[channel][slot].rx_timestamp   = 0U;
            psi5_rx_msg[channel][slot].data_field      = 0U;
            psi5_rx_msg[channel][slot].serial_id       = 0U;
            psi5_rx_msg[channel][slot].rx_crc          = 0U;
            psi5_rx_msg[channel][slot].configuration   = 0U;
            psi5_rx_msg[channel][slot].rx_timestamp    = 0U;
        }
    }

#if PSI5_CFG_TRANSFER_ENABLE           /* PRQA S 3332 # Required R-20.9 */
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        /* Configure dependent timer and transfer drivers. */
        err = r_psi5_dependent_drivers_configure(p_cfg, p_ctrl);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
    }
#endif

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE

    /* Initialize start operation with asynchronous pulse, if configured */
    if (PSI5_START_MODE_ASYNC_PULSE == p_extend->start_operation_setting.start_operation_mode)
    {
        r_psi5_init_start_operation(p_ctrl);
    }
#endif

    /* Initialize the PSI5 with the application configuration*/
    r_psi5_init_setting(p_ctrl, &p_extend->timestamp_setting);

    /* Enable channel interrupt */
    if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->tx_empty_irq)
    {
        R_BSP_IrqCfgEnable(p_ctrl->p_cfg->tx_empty_irq, p_ctrl->p_cfg->tx_empty_ipl, p_ctrl);
    }

    if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->rx_data_exist_irq)
    {
        R_BSP_IrqCfgEnable(p_ctrl->p_cfg->rx_data_exist_irq, p_ctrl->p_cfg->rx_data_exist_ipl, p_ctrl);
    }

    if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->status_irq)
    {
        R_BSP_IrqCfgEnable(p_ctrl->p_cfg->status_irq, p_ctrl->p_cfg->status_ipl, p_ctrl);
    }

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    if (FSP_INVALID_VECTOR != p_extend->channel_en_irq)
    {
        R_BSP_IrqCfgEnable(p_extend->channel_en_irq, p_extend->channel_en_ipl, p_ctrl);
    }
#endif

    /* Marked notify module PSI5 channel is opened*/
    p_ctrl->open = PSI5_PRV_OPEN;

    return err;
}

/*******************************************************************************************************************//**
 * Closes the driver.  Implements @ref psi5_api_t::close().
 *
 * @retval     FSP_SUCCESS        Successful close.
 * @retval     FSP_ERR_ASSERTION  The parameter p_ctrl is NULL.
 * @retval     FSP_ERR_NOT_OPEN   Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_Close (psi5_ctrl_t * const p_api_ctrl)                  /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Load module control from application*/
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    psi5_extended_cfg_t * p_extend = NULL;                               /* PRQA S 3678 # Advisory R-8.13 */
#endif

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    p_extend = (psi5_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # Required R-11.8, Advisory R-11.5 */
#endif

    /* Reset IP timer */
    p_ctrl->p_reg->PSI5nIPTIMERCTRL = PSI5_PRV_CLEARED;         /* PRQA S 0662 # Required R-1.1 */

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE

    /* Clear shifting channel enable status of all channels  */
    p_ctrl->p_wp_reg->PSI5WPASSC_b.SCPC = PSI5_PRV_SET;                                                                /* PRQA S 0662 # Required R-1.1 */
#endif

    /* Clear PSI5 transmission status */
    p_ctrl->p_reg->PSI5nTXSTCLR = (R_PSI50_PSI5nTXSTCLR_SYNCEDCLR_Msk | R_PSI50_PSI5nTXSTCLR_SYNCSTCLR_Msk);           /* PRQA S 0662 # Required R-1.1 */

    /* Clear transmission interrupt */
    p_ctrl->p_reg->PSI5nTXSTINTEN = PSI5_PRV_CLEARED;                                                                  /* PRQA S 0662 # Required R-1.1 */

    /* Clear receive module status */
    p_ctrl->p_reg->PSI5nRXMODSTCLR = (R_PSI50_PSI5nRXMODSTCLR_RXDERRCLR_Msk | R_PSI50_PSI5nRXMODSTCLR_RXDFOVFCLR_Msk | /* PRQA S 0662 # Required R-1.1 */
                                      R_PSI50_PSI5nRXMODSTCLR_RXDSCNFERRCLR_Msk);

    /* Clear receive data interrupt */
    p_ctrl->p_reg->PSI5nRXMODSTINTEN = PSI5_PRV_CLEARED;                                                               /* PRQA S 0662 # Required R-1.1 */

    /* Clear receive message status */
    p_ctrl->p_reg->PSI5nRXMMSTCLR = (R_PSI50_PSI5nRXMMSTCLR_RXMERRCLR_Msk | R_PSI50_PSI5nRXMMSTCLR_RXMFOVFCLR_Msk);    /* PRQA S 0662 # Required R-1.1 */

    /* Clear receive message interrupt */
    p_ctrl->p_reg->PSI5nRXMMSTINTEN = PSI5_PRV_CLEARED;                                                                /* PRQA S 0662 # Required R-1.1 */

    /* Disable PSI5 channel*/
    p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN = PSI5_PRV_CHANNEL_DISABLED;                                                     /* PRQA S 0662 # Required R-1.1 */

    /* Reset timestamp value */
    p_ctrl->p_reg->PSI5nIPTIMER = PSI5_PRV_CLEARED;                                                                    /* PRQA S 0662 # Required R-1.1 */

    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN, 0U);                                                 /* PRQA S 0662 # Required R-1.1 */

    /* Clear the open mark of PSI5 channel*/
    p_ctrl->open = PSI5_PRV_CLEARED;

    /* Disable channel interrupt */
    if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->tx_empty_irq)
    {
        R_BSP_IrqDisable(p_ctrl->p_cfg->tx_empty_irq);
    }

    if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->rx_data_exist_irq)
    {
        R_BSP_IrqDisable(p_ctrl->p_cfg->rx_data_exist_irq);
    }

    if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->status_irq)
    {
        R_BSP_IrqDisable(p_ctrl->p_cfg->status_irq);
    }

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    if (FSP_INVALID_VECTOR != p_extend->channel_en_irq)
    {
        R_BSP_IrqDisable(p_extend->channel_en_irq);
    }
#endif

#if PSI5_CFG_TRANSFER_ENABLE           /* PRQA S 3332 # Required R-20.9 */
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = PSI5_NUM_TRANSFER_RESET;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;

        p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);

        p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                         p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);

        p_ctrl->p_cfg->p_transfer_rx->p_api->close(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start the driver.  Implements @ref psi5_api_t::start().
 * @param[in]  p_api_ctrl         Pointer to control block
 * @retval     FSP_SUCCESS        Successful start.
 * @retval     FSP_ERR_ASSERTION  The parameter p_ctrl is NULL.
 * @retval     FSP_ERR_NOT_OPEN   Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_Start (psi5_ctrl_t * const p_api_ctrl)                  /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    /* Load module control from application*/
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    psi5_extended_cfg_t * p_extend = NULL;                               /* PRQA S 3678 # Advisory R-8.13 */
#endif

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
    p_extend = (psi5_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # Required R-11.8, Advisory R-11.5 */
#endif

    if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->rx_data_exist_irq)
    {
        /* Enable receive data exist interrupt */
        p_ctrl->p_reg->PSI5nRXMODSTINTEN_b.RXDEXISTINTEN = PSI5_PRV_SET; /* PRQA S 0662 # Required R-1.1 */

        /* Enable receive message exist interrupt */
        p_ctrl->p_reg->PSI5nRXMMSTINTEN_b.RXMEXISTINTEN = PSI5_PRV_SET;  /* PRQA S 0662 # Required R-1.1 */
    }

    if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->status_irq)
    {
        /* Enable receive data error interrupt */
        p_ctrl->p_reg->PSI5nRXMODSTINTEN_b.RXDERRINTEN = PSI5_PRV_SET;     /* PRQA S 0662 # Required R-1.1 */

        /* Enable receive data overflow interrupt */
        p_ctrl->p_reg->PSI5nRXMODSTINTEN_b.RXDFOVFINTEN = PSI5_PRV_SET;    /* PRQA S 0662 # Required R-1.1 */

        /* Enable synchronization pulse and receive data conflict error interrupt */
        p_ctrl->p_reg->PSI5nRXMODSTINTEN_b.RXDSCNFERRINTEN = PSI5_PRV_SET; /* PRQA S 0662 # Required R-1.1 */
    }

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE

    /* Enable PSI5 channel */
    if (PSI5_START_MODE_ASYNC_PULSE == p_extend->start_operation_setting.start_operation_mode)
    {
        /* Clear shifting channel enable status of all channels  */
        p_ctrl->p_wp_reg->PSI5WPASSC_b.SCPC = PSI5_PRV_SET;                            /* PRQA S 0662 # Required R-1.1 */

        /* Enable controls shifting channel enable interrupt */
        p_ctrl->p_wp_reg->PSI5WPASTI_b.SCPI = PSI5_PRV_SET;                            /* PRQA S 0662 # Required R-1.1 */

        /* Enable all PSI5n channel */
        p_ctrl->p_wp_reg->PSI5WPASTC_b.STST = PSI5_PRV_SET;                            /* PRQA S 0662 # Required R-1.1 */

        /* Wait for channel enable */
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_wp_reg->PSI5WPASTS_b.SCPL, PSI5_PRV_SET); /* PRQA S 0662 # Required R-1.1 */
    }
    else
#endif
    {
        p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN = PSI5_PRV_CHANNEL_ENABLED;                /* PRQA S 0662 # Required R-1.1 */

        /* Wait for channel enable */
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN, PSI5_PRV_SET); /* PRQA S 0662 # Required R-1.1 */
    }

#if PSI5_CFG_TRANSFER_ENABLE                                                         /* PRQA S 3332 # Required R-20.9 */
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        /* Disable Rx interrupt */
        R_BSP_IrqDisable(p_ctrl->p_cfg->rx_data_exist_irq);

        p_ctrl->p_cfg->p_transfer_rx->p_api->reset(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                   (void *) &(p_ctrl->p_reg->PSI5nRXDFIFO), /* PRQA S 0312, 0311, 0662 # Required R-11.8, Required R-11.8, Required R-1.1 */
                                                   (void *) &psi5_rx_buffer,
                                                   PSI5_PRV_NUMBER_TRANSFER);
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop the driver.  Implements @ref psi5_api_t::stop().
 * @param[in]  p_api_ctrl         Pointer to control block
 * @retval     FSP_SUCCESS        Successful stop.
 * @retval     FSP_ERR_ASSERTION  The parameter p_ctrl is NULL.
 * @retval     FSP_ERR_NOT_OPEN   Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_Stop (psi5_ctrl_t * const p_api_ctrl)                   /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    /* Load module control from application*/
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Disable PSI5 channel*/
    p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN = PSI5_PRV_CHANNEL_DISABLED;                   /* PRQA S 0662 # Required R-1.1 */

    /* Wait for channel disable */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN, PSI5_PRV_CLEARED); /* PRQA S 0662 # Required R-1.1 */
#if PSI5_CFG_TRANSFER_ENABLE                                                         /* PRQA S 3332 # Required R-20.9 */
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read the latest data frame.  Implements @ref psi5_api_t::dataFrameRead().
 * @param[in]   p_api_ctrl        Pointer to control block
 * @param[out]  p_dataframe       Pointer to the receive buffer
 * @param[in]   slot_num          Slot to receive
 * @retval     FSP_SUCCESS        Data frame read successfully.
 * @retval     FSP_ERR_ASSERTION  The parameter p_ctrl is NULL.
 * @retval     FSP_ERR_NOT_OPEN   Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_ReadDataFrame (psi5_ctrl_t * const p_api_ctrl, psi5_data_frame_t * p_dataframe, uint8_t slot_num) /* PRQA S 1330, 3673, 1505 # Required R-8.3, Advisory R-8.13, Advisory R-8.7 */
{
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl;                                           /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(NULL != p_dataframe);
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(slot_num > 0 && slot_num <= PSI5_PRV_MAX_SLOT, FSP_ERR_INVALID_ARGUMENT); /* PRQA S 1862, 3397 # Required R-10.4, Advisory R-12.1 */
#endif

    uint8_t channel = p_ctrl->p_cfg->channel;

    p_dataframe->rx_data        = psi5_rx_data[channel][slot_num - 1U].rx_data;
    p_dataframe->rx_data_status = psi5_rx_data[channel][slot_num - 1U].rx_data_status;
    p_dataframe->rx_timestamp   = psi5_rx_data[channel][slot_num - 1U].rx_timestamp;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read the latest message frame.  Implements @ref psi5_api_t::messageFrameRead().
 * @param[in]   p_api_ctrl        Pointer to control block
 * @param[out]  p_msgframe        Pointer to the receive serial message buffer
 * @param[in]   slot_num          Slot to receive
 * @retval     FSP_SUCCESS        Message frame read successfully.
 * @retval     FSP_ERR_ASSERTION  The parameter p_ctrl is NULL.
 * @retval     FSP_ERR_NOT_OPEN   Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_ReadMessageFrame (psi5_ctrl_t * const           p_api_ctrl, /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
                                   psi5_serial_message_frame_t * p_msgframe, /* PRQA S 1330 # Required R-8.3 */
                                   uint8_t                       slot_num)
{
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl;     /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_msgframe);
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(slot_num > 0 && slot_num <= PSI5_PRV_MAX_SLOT, FSP_ERR_INVALID_ARGUMENT); /* PRQA S 1862, 3397 # Required R-10.4, Advisory R-12.1 */
#endif
    uint8_t channel = p_ctrl->p_cfg->channel;

    p_msgframe->serial_id     = psi5_rx_msg[channel][slot_num - 1U].serial_id;
    p_msgframe->configuration = psi5_rx_msg[channel][slot_num - 1U].configuration;
    p_msgframe->data_field    = psi5_rx_msg[channel][slot_num - 1U].data_field;
    p_msgframe->rx_crc        = psi5_rx_msg[channel][slot_num - 1U].rx_crc;
    p_msgframe->rx_timestamp  = psi5_rx_msg[channel][slot_num - 1U].rx_timestamp;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get the driver status from registers.  Implements @ref psi5_api_t::statusGet().
 * @param[in]   p_api_ctrl        Pointer to control block
 * @param[out]  p_status          Pointer to the status buffer
 * @retval     FSP_SUCCESS        Status read successfully.
 * @retval     FSP_ERR_ASSERTION  The parameter p_ctrl is NULL.
 * @retval     FSP_ERR_NOT_OPEN   Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_StatusGet (psi5_ctrl_t * const p_api_ctrl, psi5_status_t * p_status) /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    /* Load module control from application*/
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl;              /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(p_status);
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Get status of receive-message channel module*/
    p_status->rx_message_status = p_ctrl->p_sts_buff.rx_message_status;

    /* Get receive module status */
    p_status->rx_module_status = p_ctrl->p_sts_buff.rx_module_status;

    /* Get transmit status */
    p_status->tx_status = p_ctrl->p_sts_buff.tx_status;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 * Implements @ref psi5_api_t::callbackSet.
 * @param[in]   p_api_ctrl               Pointer to control block
 * @param[in]   p_callback               Pointer to configuration block
 * @param[in]   p_context                Pointer to configuration block
 * @param[in]   p_callback_memory        Pointer to configuration block
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_CallbackSet (psi5_ctrl_t * const          p_api_ctrl,                          /* PRQA S 1505 # Advisory R-8.7 */
                              void (                     * p_callback)(psi5_callback_args_t *), /* PRQA S 1336 # Required R-8.2 */
                              void const * const           p_context,
                              psi5_callback_args_t * const p_callback_memory)
{
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl;                        /* PRQA S 0316 # Advisory R-11.5 */

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(p_callback);                                                                     /* PRQA S 3344 # Required R-14.4 */
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Load application configuration of timestamp to PSI5 peripheral
 * Implements @ref psi5_api_t::timestampSet.
 * @param[in]   p_api_ctrl               Pointer to control block
 * @param[in]   p_timestamp_cfg          Pointer to configuration timestamp
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_TimestampSet (psi5_ctrl_t * const p_api_ctrl, psi5_timestamp_setting_t const * const p_timestamp_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl;                                               /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t               chen_state;

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_timestamp_cfg);
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Save current channel enable state to restore it after reconfiguration */
    chen_state = p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN;                /* PRQA S 0662 # Required R-1.1 */

    /*Disable PSI5 channel*/
    p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN = PSI5_PRV_CHANNEL_DISABLED; /* PRQA S 0662 # Required R-1.1 */

    /* Load timestamp configuration */
    r_psi5_timestamp_setting(p_ctrl, p_timestamp_cfg);

    /* Restore the previous channel enable state */
    p_ctrl->p_reg->PSI5nCHCTRL_b.CHEN = chen_state; /* PRQA S 0662 # Required R-1.1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Transfer data from ECU to sensor
 * Implements @ref psi5_api_t::write.
 * @param[in]   p_api_ctrl               Pointer to control block
 * @param[in]   p_src                    Pointer to source buffer
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_PSI5_SendChannelData (psi5_ctrl_t * const p_api_ctrl, uint32_t const * const p_src) /* PRQA S 1505 # Advisory R-8.7 */
{
    psi5_instance_ctrl_t * p_ctrl   = (psi5_instance_ctrl_t *) p_api_ctrl;                      /* PRQA S 0316 # Advisory R-11.5 */
    psi5_extended_cfg_t  * p_extend = NULL;                                                     /* PRQA S 3678 # Advisory R-8.13 */

#if PSI5_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_src);
    FSP_ERROR_RETURN(PSI5_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    p_extend = (psi5_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # Required R-11.8, Advisory R-11.5 */

    /* Assign data for transmit */
    p_ctrl->p_tx_source = (p_src);

    /* Set data transmission format */
    p_ctrl->p_reg->PSI5nTXDCTRL_b.FRMFORMAT = p_extend->tx_setting.frame_format; /* PRQA S 0662, 4424 # Required R-1.1, Required R-10.3 */

    /* Set enable Transmit data empty interrupt*/
    p_ctrl->p_reg->PSI5nTXSTINTEN_b.TXDEMPTYINTEN = PSI5_PRV_SET;                /* PRQA S 0662 # Required R-1.1 */

    return FSP_SUCCESS;
}

/** @} (end addtogroup PSI5) */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Set the initial settings for channels
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 * @param[in]  p_timestamp_cfg           Pointer to the psi5 timestamp configuration
 *
 **********************************************************************************************************************/
static void r_psi5_init_setting (psi5_instance_ctrl_t * p_ctrl, psi5_timestamp_setting_t const * const p_timestamp_cfg)
{
    /** Initialization value corresponding to specification of the PSI5 sensor */
    r_psi5_op_setting(p_ctrl);

    /** Reception setting */
    r_psi5_rx_setting(p_ctrl);

    /** Transmission setting */
    r_psi5_tx_setting(p_ctrl);

    /** Timestamp setting */
    r_psi5_timestamp_setting(p_ctrl, p_timestamp_cfg);
}

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE

/*******************************************************************************************************************//**
 * Set start operation mode for PSI5 channel
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
static void r_psi5_init_start_operation (psi5_instance_ctrl_t * p_ctrl)                          /* PRQA S 3673 # Advisory R-8.13 */
{
    psi5_extended_cfg_t * p_extend = (psi5_extended_cfg_t *) p_ctrl->p_cfg->p_extend;            /* PRQA S 0311, 0316, 3678 # Required R-11.8, Advisory R-11.5, Advisory R-8.13 */

    uint8_t channel = p_ctrl->p_cfg->channel;

    p_ctrl->p_wp_reg->PSI5WP[channel].PSI5WPDST = p_extend->start_operation_setting.delay_count; /* PRQA S 0662 # Required R-1.1 */

    p_ctrl->p_wp_reg->PSI5WP[channel].PSI5WPSCT = PSI5_PRV_SET;                                  /* PRQA S 0662 # Required R-1.1 */
}

#endif

/*******************************************************************************************************************//**
 * Set operation mode for PSI5 channel
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
static void r_psi5_op_setting (psi5_instance_ctrl_t * p_ctrl)                                /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Set the communication mode for PSI5 channel  */
    p_ctrl->p_reg->PSI5nOPMCOMM = p_ctrl->p_cfg->communication_mode;                         /* PRQA S 0662, 4424 # Required R-1.1, Required R-10.3 */

    /* Set the bit rate for PSI5 channel
     * Note: This is 1-bit clock baudrate, and it is dependence on clock tolerance and communication clock
     * The baudrate here followed recommendation setting in section 3.9 Baudrate */
    p_ctrl->p_reg->PSI5nOPMBITRATE = p_ctrl->p_cfg->bitrate;                                 /* PRQA S 0662 # Required R-1.1 */
}

/*******************************************************************************************************************//**
 * Load configuration for data reception
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
static void r_psi5_rx_setting (psi5_instance_ctrl_t * p_ctrl)                                /* PRQA S 3673 # Advisory R-8.13 */
{
    psi5_extended_cfg_t * p_extend        = (psi5_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Required R-11.8, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               psi5_rx_slot    = 0U;
    uint8_t               index           = 0U;
    uint32_t              rx_slot_setting = 0U;

    /* Set the sampling timing for received data*/
    p_ctrl->p_reg->PSI5nRXSPLSET_b.SMPLPROD = p_extend->rx_setting.rx_sampl_timing.sampling_interval; /* PRQA S 0662 # Required R-1.1 */

    /* Set the receive window setting of sampling timing */
    psi5_rx_sampling_setting_t * p_rx_timing = &p_extend->rx_setting.rx_sampl_timing;                 /* PRQA S 3678 # Advisory R-8.13 */
    p_ctrl->p_reg->PSI5nRXWINSET =                                                                    /* PRQA S 0662 # Required R-1.1 */
                                   (((uint32_t) p_rx_timing->non_data_tgl_detec_start_pos <<
                                     R_PSI50_PSI5nRXWINSET_NDWINEDPOS_Pos) |
                                    ((uint32_t) p_rx_timing->non_data_tgl_detect_end_pos <<
                                     R_PSI50_PSI5nRXWINSET_NDWINSTPOS_Pos) |
                                    ((uint32_t) p_rx_timing->tgl_detect_start_pos <<
                                     R_PSI50_PSI5nRXWINSET_DWINEDPOS_Pos) |
                                    (uint32_t) p_rx_timing->tgl_detect_end_pos);

    /* Load number of enable slots from loaded configurations*/

    if (PSI5_COMM_MODE_ASYNCHRONOUS != p_ctrl->p_reg->PSI5nOPMCOMM_b.COMMODE) /* PRQA S 0662, 1881 # Required R-1.1, Required R-10.4 */
    {
        psi5_rx_slot = PSI5_PRV_MAX_SLOT;
    }
    /* In asynchronous mode (PSI5nOPMCOMM.COMMODE = 000B), only PSI5nRXS1SET is available.*/
    else
    {
        psi5_rx_slot = PSI5_PRV_MIN_SLOT;
    }

    /* Reset RXMSET before setting for receive */
    p_ctrl->p_reg->PSI5nRXMSET = 0U;   /* PRQA S 0662 # Required R-1.1 */

    /* Setting each receive slot */
    for (index = 0U; index < psi5_rx_slot; index++)
    {
        rx_slot_setting = 0U;

        /* Enable Message channel for received slot  */
        p_ctrl->p_reg->PSI5nRXMSET |= (uint32_t) p_extend->rx_setting.p_rx_slot[index].serial_message_enable << index; /* PRQA S 0662 # Required R-1.1 */

        /* Enable received slot, Load configurations operation mode, error detection scheme, data length, and offset
         * timer to slot control registers */

        rx_slot_setting |=
            (((uint32_t) p_extend->rx_setting.p_rx_slot[index].slot_enable << R_PSI50_RXSSET_RXSSET_SLTEN_Pos) &
             R_PSI50_RXSSET_RXSSET_SLTEN_Msk);

        rx_slot_setting |=
            (((uint32_t) p_extend->rx_setting.p_rx_slot[index].operation_mode << R_PSI50_RXSSET_RXSSET_PASCMP_Pos) &
             R_PSI50_RXSSET_RXSSET_PASCMP_Msk);

        rx_slot_setting |=
            (((uint32_t) p_extend->rx_setting.p_rx_slot[index].data_length << R_PSI50_RXSSET_RXSSET_LENGTH_Pos) &
             R_PSI50_RXSSET_RXSSET_LENGTH_Msk);

        rx_slot_setting |=
            (((uint32_t) p_extend->rx_setting.p_rx_slot[index].crc_parity_sel << R_PSI50_RXSSET_RXSSET_ERRDET_Pos) &
             R_PSI50_RXSSET_RXSSET_ERRDET_Msk);

        /* Load offset timer for synchronous mode */
        if (PSI5_COMM_MODE_ASYNCHRONOUS != p_ctrl->p_reg->PSI5nOPMCOMM_b.COMMODE) /* PRQA S 0662, 1881 # Required R-1.1, Required R-10.4 */
        {
            rx_slot_setting |=
                (p_extend->rx_setting.p_rx_slot[index].offset_counter & R_PSI50_RXSSET_RXSSET_OFFSETCNT_Msk);
        }

        p_ctrl->p_reg->RXSSET[index].RXSSET = rx_slot_setting; /* PRQA S 0662 # Required R-1.1 */
    }
}

/*******************************************************************************************************************//**
 * Load configuration for data transmission
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
static void r_psi5_tx_setting (psi5_instance_ctrl_t * p_ctrl)                         /* PRQA S 3673 # Advisory R-8.13 */
{
    psi5_extended_cfg_t * p_extend = (psi5_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Required R-11.8, Advisory R-11.5, Advisory R-8.13 */
    uint32_t              tx_setting;

    /* Transmission setting in Synchronous Mode */
    if (PSI5_COMM_MODE_ASYNCHRONOUS != p_ctrl->p_reg->PSI5nOPMCOMM_b.COMMODE)         /* PRQA S 0662, 1881 # Required R-1.1, Required R-10.4 */
    {
        tx_setting = (((uint32_t) p_extend->tx_setting.long_count << R_PSI50_PSI5nTXSETTING_LONGCNT_Pos) &
                      R_PSI50_PSI5nTXSETTING_LONGCNT_Msk);

        tx_setting |= (uint32_t) p_extend->tx_setting.short_count & R_PSI50_PSI5nTXSETTING_SHORTCNT_Msk;
        tx_setting |= (((uint32_t) p_extend->tx_setting.physical_layer << R_PSI50_PSI5nTXSETTING_PHYMODE_Pos) &
                       R_PSI50_PSI5nTXSETTING_PHYMODE_Msk);
        tx_setting |= ((uint32_t) p_extend->tx_setting.sync_pulse_value << R_PSI50_PSI5nTXSETTING_DEFTXVAL_Pos) &
                      R_PSI50_PSI5nTXSETTING_DEFTXVAL_Msk;

        p_ctrl->p_reg->PSI5nTXSETTING = tx_setting;                       /* PRQA S 0662 # Required R-1.1 */

        /* Set the Operating-Mode cycle time (count value) */
        p_ctrl->p_reg->PSI5nOPMCYCT = p_ctrl->p_cfg->sync_period_counter; /* PRQA S 0662 # Required R-1.1 */
    }
    /* Transmission setting in Asynchronous Mode */
    else
    {
        p_ctrl->p_reg->PSI5nTXSETTING = (p_extend->tx_setting.sync_pulse_value << R_PSI50_PSI5nTXSETTING_DEFTXVAL_Pos) & /* PRQA S 0499, 0662, 1891 # Required R-12.2, Required R-1.1, Required R-10.7 */
                                        R_PSI50_PSI5nTXSETTING_DEFTXVAL_Msk;
    }
}

/*******************************************************************************************************************//**
 * Set the timestamp for PSI5 channel
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 * @param[in]  p_timestamp_cfg           Pointer to the psi5 timestamp configuration
 *
 **********************************************************************************************************************/
static void r_psi5_timestamp_setting (psi5_instance_ctrl_t                 * p_ctrl,
                                      psi5_timestamp_setting_t const * const p_timestamp_cfg)
{
    if (PSI5_IP_TIMER_ENABLE == p_timestamp_cfg->ip_timer_enable)
    {
        switch (p_timestamp_cfg->timestamp_mode)
        {
            case PSI5_TIMESTAMP_MODE_MASTER:
            {
                r_psi5_channel_set_master(p_ctrl);

                break;
            }

            case PSI5_TIMESTAMP_MODE_SLAVE:
            {
                r_psi5_channel_set_slave(p_ctrl, p_timestamp_cfg);

                break;
            }

            default:                   /* PRQA S 2016 # Required R-16.4 */
            {
                break;
            }
        }

        p_ctrl->p_reg->PSI5nIPTIMERCTRL_b.IPTIMEN = PSI5_IP_TIMER_ENABLE; /* PRQA S 0662, 4424 # Required R-1.1, Required R-10.3 */
    }
    else
    {
        /* Disable the IP timer when timestamp is not used */
        p_ctrl->p_reg->PSI5nIPTIMERCTRL_b.IPTIMEN = PSI5_IP_TIMER_DISABLE; /* PRQA S 0662, 4424 # Required R-1.1, Required R-10.3 */
    }
}

/*******************************************************************************************************************//**
 * Set the PSI5 channel to timer master mode.
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
static void r_psi5_channel_set_master (psi5_instance_ctrl_t * p_ctrl)      /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Set the master mode for current PSI5 channel*/
    p_ctrl->p_reg->PSI5nIPTIMERCTRL_b.MSTSLV = PSI5_TIMESTAMP_MODE_MASTER; /* PRQA S 0662, 4424 # Required R-1.1, Required R-10.3 */

    uint8_t channel = p_ctrl->p_cfg->channel;

    switch (channel)
    {
        case 0:
        {
            p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL0 = 0x00U; /* PRQA S 0662 # Required R-1.1 */
            break;
        }

        case 1:
        {
            p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL1 = 0x00U; /* PRQA S 0662 # Required R-1.1 */
            break;
        }

#if (BSP_FEATURE_PSI5_MAX_CHANNEL > (2U))
        case 2:
        {
            p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL2 = 0x00U; /* PRQA S 0662 # Required R-1.1 */
            break;
        }

        case 3:
        {
            p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL3 = 0x00U; /* PRQA S 0662 # Required R-1.1 */
            break;
        }
#endif
        default:                                              /* PRQA S 2016 # Required R-16.4 */
        {
            break;
        }
    }
}

/*******************************************************************************************************************//**
 * Set the PSI5 channel to timer slave mode and specifies its master channel. Notes: It is possible that all PSI5
 * channels enable timestamp mode but none of them work as a slave timer.
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 * @param[in]  p_timestamp_cfg           Pointer to the psi5 timestamp configuration
 *
 **********************************************************************************************************************/
static void r_psi5_channel_set_slave (psi5_instance_ctrl_t                 * p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                      psi5_timestamp_setting_t const * const p_timestamp_cfg)
{
    uint8_t channel = p_ctrl->p_cfg->channel;

    /* Set the slave mode for current PSI5 channel*/
    p_ctrl->p_reg->PSI5nIPTIMERCTRL_b.MSTSLV = PSI5_TIMESTAMP_MODE_SLAVE;       /* PRQA S 0662, 4424 # Required R-1.1, Required R-10.3 */

    if (PSI5_TIMESTAMP_MODE_MASTER_SEL_PSI50 == p_timestamp_cfg->master_select) /* PRQA S 1881 # Required R-10.4 */
    {
        switch (channel)
        {
            case 0:
            {
                /* Do nothing */
                break;
            }

            case 1:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL1 = PSI5_TIMESTAMP_UNIT_0_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

#if (BSP_FEATURE_PSI5_MAX_CHANNEL > (2U))
            case 2:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL2 = PSI5_TIMESTAMP_UNIT_0_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

            case 3:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL3 = PSI5_TIMESTAMP_UNIT_0_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }
#endif
            default:                                                                         /* PRQA S 2016 # Required R-16.4 */
            {
                break;
            }
        }
    }
    else if (PSI5_TIMESTAMP_MODE_MASTER_SEL_PSI51 == p_timestamp_cfg->master_select) /* PRQA S 1881 # Required R-10.4 */
    {
        switch (channel)
        {
            case 0:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL0 = PSI5_TIMESTAMP_UNIT_1_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

            case 1:
            {
                /* Do nothing */
                break;
            }

#if (BSP_FEATURE_PSI5_MAX_CHANNEL > (2U))
            case 2:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL2 = PSI5_TIMESTAMP_UNIT_1_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

            case 3:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL3 = PSI5_TIMESTAMP_UNIT_1_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }
#endif
            default:                                                                         /* PRQA S 2016 # Required R-16.4 */
            {
                break;
            }
        }
    }
    else if (PSI5_TIMESTAMP_MODE_MASTER_SEL_PSI52 == p_timestamp_cfg->master_select) /* PRQA S 1881 # Required R-10.4 */
    {
        switch (channel)
        {
            case 0:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL0 = PSI5_TIMESTAMP_UNIT_2_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

            case 1:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL1 = PSI5_TIMESTAMP_UNIT_2_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

#if (BSP_FEATURE_PSI5_MAX_CHANNEL > (2U))
            case 2:
            {
                /* Do nothing */
                break;
            }

            case 3:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL3 = PSI5_TIMESTAMP_UNIT_2_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }
#endif
            default:                                                                         /* PRQA S 2016 # Required R-16.4 */
            {
                break;
            }
        }
    }

#if (BSP_FEATURE_PSI5_MAX_CHANNEL > (2U))
    else if (PSI5_TIMESTAMP_MODE_MASTER_SEL_PSI53 == p_timestamp_cfg->master_select) /* PRQA S 1881 # Required R-10.4 */
    {
        switch (channel)
        {
            case 0:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL0 = PSI5_TIMESTAMP_UNIT_3_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

            case 1:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL1 = PSI5_TIMESTAMP_UNIT_3_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

 #if (BSP_FEATURE_PSI5_MAX_CHANNEL > (2U))
            case 2:
            {
                /* Set its channel master */
                p_ctrl->p_defsel->PSI5TSSEL_b.PSI5MSSEL2 = PSI5_TIMESTAMP_UNIT_3_MASTER_SET; /* PRQA S 0662 # Required R-1.1 */
                break;
            }

            case 3:
            {
                /* Do nothing */
                break;
            }
 #endif
            default:                   /* PRQA S 2016 # Required R-16.4 */
            {
                break;
            }
        }
    }
#endif
    else
    {
        /* Do nothing */
    }
}

/*******************************************************************************************************************//**
 * Read data frame from driver register
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
static void r_psi5_read_data_frame (psi5_instance_ctrl_t * p_ctrl)
{
    uint8_t  slot_num = 0U;
    uint8_t  channel  = p_ctrl->p_cfg->channel;
    uint32_t rx_data;
    uint32_t rx_status;
    uint32_t rx_timestamp;
#if (PSI5_CFG_TRANSFER_ENABLE)                     /* PRQA S 3332 # Required R-20.9 */
    if (NULL == p_ctrl->p_cfg->p_transfer_rx)
#endif
    {
        rx_data      = p_ctrl->p_reg->PSI5nRXDATA; /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */
        rx_status    = p_ctrl->p_reg->PSI5nRXDST;  /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */
        rx_timestamp = p_ctrl->p_reg->PSI5nRXDTIM; /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */
    }

#if PSI5_CFG_TRANSFER_ENABLE                       /* PRQA S 3332 # Required R-20.9 */
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        transfer_instance_t const * const p_transfer       = p_ctrl->p_cfg->p_transfer_rx;
        transfer_info_t                 * p_info           = p_transfer->p_cfg->p_info;
        uint32_t volatile const         * p_psi5_rx_buffer = &(p_ctrl->p_reg->PSI5nRXDFIFO);

        /* Get slot num from message data status */
        uint8_t raw_slot =
            (uint8_t) ((psi5_rx_buffer.rx_data_status & R_PSI50_PSI5nEMRXDST_RXSLOTNUM_Msk) >>
                       R_PSI50_PSI5nEMRXDST_RXSLOTNUM_Pos);
        if ((raw_slot >= PSI5_PRV_MIN_SLOT) && (raw_slot <= PSI5_PRV_MAX_SLOT))
        {
            slot_num = raw_slot - 1U;
            psi5_rx_data[channel][slot_num] = psi5_rx_buffer; /* PRQA S 0488 # Array bounds verified by range check */
        }

        /* Reconfiguration new buffer data */
        p_info->number_transfer = PSI5_PRV_NUMBER_TRANSFER;
        p_info->p_src           = (const uint32_t *) (p_psi5_rx_buffer), /* PRQA S 0312 # Required R-11.8 */
        p_info->p_dest          = (void *) &psi5_rx_buffer;              /* PRQA S 0306 # Framework requires void pointer for buffer reference */
        p_transfer->p_api->reset(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                 (const uint32_t *) (p_psi5_rx_buffer),
                                 (void *) &psi5_rx_buffer,
                                 PSI5_PRV_NUMBER_TRANSFER);
    }
    else
#endif
    {
        uint8_t raw_value =
            (uint8_t) ((rx_status & R_PSI50_PSI5nEMRXDST_RXSLOTNUM_Msk) >> R_PSI50_PSI5nEMRXDST_RXSLOTNUM_Pos);

        /* Get slot num from message data status */
        if ((raw_value >= PSI5_PRV_MIN_SLOT) && (raw_value <= PSI5_PRV_MAX_SLOT))
        {
            slot_num = (uint8_t) (raw_value - 1U);
        }

        psi5_rx_data[channel][slot_num].rx_data        = rx_data; /* PRQA S 0488 # Array bounds verified by channel and slot initialization */
        psi5_rx_data[channel][slot_num].rx_data_status = rx_status;
        psi5_rx_data[channel][slot_num].rx_timestamp   = rx_timestamp;
    }

    /* Call user callback */
    r_psi5_call_callback(p_ctrl, PSI5_EVENT_RX_DATA_EXIST);
}

/*******************************************************************************************************************//**
 * Read serial data frame from driver register
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
static void r_psi5_read_msg_frame (psi5_instance_ctrl_t * p_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    uint32_t rx_message        = p_ctrl->p_reg->PSI5nRXMRXMSG;    /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */
    uint32_t rx_message_status = p_ctrl->p_reg->PSI5nRXMRXST;     /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */
    uint32_t rx_timestamp      = p_ctrl->p_reg->PSI5nRXMRXTIM;    /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */
    uint8_t  channel           = p_ctrl->p_cfg->channel;

    uint8_t raw_slot_num = (uint8_t) ((rx_message_status & R_PSI50_PSI5nEMRXMRXST_SLOTNUM_Msk) >>
                                      R_PSI50_PSI5nEMRXMRXST_SLOTNUM_Pos);
    uint8_t slot_num = (uint8_t) ((raw_slot_num > 0U) ? (raw_slot_num - 1U) : 0U);

    uint32_t raw_crc = (uint32_t) (((rx_message_status & R_PSI50_PSI5nEMRXMRXST_RXCRC_Msk) >>
                                    R_PSI50_PSI5nEMRXMRXST_RXCRC_Pos));

    /* Read CRC check sum*/
    psi5_rx_msg[channel][slot_num].rx_crc = (uint8_t) raw_crc; /* PRQA S 0488 # Array bounds verified by message status parsing */

    /* Read the timestamp*/
    psi5_rx_msg[channel][slot_num].rx_timestamp = rx_timestamp;

    /* Read the received message*/
    psi5_rx_msg[channel][slot_num].serial_id = (uint8_t) ((rx_message & R_PSI50_PSI5nEMRXMRXMSG_SERIALID_Msk) >>
                                                          R_PSI50_PSI5nEMRXMRXMSG_SERIALID_Pos); /* PRQA S 0488 # Array bounds verified by message status parsing */
    psi5_rx_msg[channel][slot_num].configuration = (uint8_t) ((rx_message & R_PSI50_PSI5nEMRXMRXMSG_CONFIGBIT_Msk) >>
                                                              R_PSI50_PSI5nEMRXMRXMSG_CONFIGBIT_Pos);
    psi5_rx_msg[channel][slot_num].data_field = (uint16_t) ((rx_message & R_PSI50_PSI5nEMRXMRXMSG_DATAFIELD_Msk) >>
                                                            R_PSI50_PSI5nEMRXMRXMSG_DATAFIELD_Pos);
}

/*******************************************************************************************************************//**
 * Handle status errors and message reception receive exist interrupts.
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
static void r_psi5_status_handler (psi5_instance_ctrl_t * p_ctrl)
{
    uint32_t rx_msg_err  = p_ctrl->p_reg->PSI5nRXMMST;  /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */
    uint32_t tx_status   = p_ctrl->p_reg->PSI5nTXST;    /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */
    uint32_t rx_data_err = p_ctrl->p_reg->PSI5nRXMODST; /* PRQA S 0404, 0662 # ISR requires simultaneous volatile register reads, Required R-1.1 */

    p_ctrl->p_sts_buff.rx_module_status  = rx_data_err;
    p_ctrl->p_sts_buff.rx_message_status = rx_msg_err;
    p_ctrl->p_sts_buff.tx_status         = tx_status;

    if (R_PSI50_PSI5nRXMMST_RXMEXIST_Msk == (rx_msg_err & R_PSI50_PSI5nRXMMST_RXMEXIST_Msk))
    {
        r_psi5_read_msg_frame(p_ctrl);

        /* Call user callback */
        r_psi5_call_callback(p_ctrl, PSI5_EVENT_RX_MESSAGE_EXIST);
    }

    if (R_PSI50_PSI5nRXMMST_RXMFOVF_Msk == (rx_msg_err & R_PSI50_PSI5nRXMMST_RXMFOVF_Msk))
    {
        p_ctrl->p_reg->PSI5nRXMMSTCLR_b.RXMFOVFCLR = 1U; /* PRQA S 0662 # Required R-1.1 */

        /* Call user callback */
        r_psi5_call_callback(p_ctrl, PSI5_EVENT_RX_MESSAGE_FIFO_OVERFLOW);
    }

    if (R_PSI50_PSI5nRXMMST_RXMERR_Msk == (rx_msg_err & R_PSI50_PSI5nRXMMST_RXMERR_Msk))
    {
        p_ctrl->p_reg->PSI5nRXMMSTCLR_b.RXMERRCLR = 1U; /* PRQA S 0662 # Required R-1.1 */

        /* Call user callback */
        r_psi5_call_callback(p_ctrl, PSI5_EVENT_RX_MESSAGE_ERR);
    }

    if (R_PSI50_PSI5nTXST_SYNCED_Msk == (tx_status & R_PSI50_PSI5nTXST_SYNCED_Msk))
    {
        p_ctrl->p_reg->PSI5nTXSTCLR_b.SYNCEDCLR = 1U; /* PRQA S 0662 # Required R-1.1 */

        /* Call user callback */
        r_psi5_call_callback(p_ctrl, PSI5_EVENT_SYNC_END);
    }

    if (R_PSI50_PSI5nTXST_SYNCST_Msk == (tx_status & R_PSI50_PSI5nTXST_SYNCST_Msk))
    {
        p_ctrl->p_reg->PSI5nTXSTCLR_b.SYNCSTCLR = 1U; /* PRQA S 0662 # Required R-1.1 */

        /* Call user callback */
        r_psi5_call_callback(p_ctrl, PSI5_EVENT_SYNC_START);
    }

    if (R_PSI50_PSI5nRXMODST_RXDERR_Msk == (rx_data_err & R_PSI50_PSI5nRXMODST_RXDERR_Msk))
    {
        p_ctrl->p_reg->PSI5nRXMODSTCLR_b.RXDERRCLR = 1U; /* PRQA S 0662 # Required R-1.1 */

        /* Call user callback */
        r_psi5_call_callback(p_ctrl, PSI5_EVENT_RX_DATA_ERR);
    }

    if (R_PSI50_PSI5nRXMODST_RXDFOVF_Msk == (rx_data_err & R_PSI50_PSI5nRXMODST_RXDFOVF_Msk))
    {
        p_ctrl->p_reg->PSI5nRXMODSTCLR_b.RXDFOVFCLR = 1U; /* PRQA S 0662 # Required R-1.1 */

        /* Call user callback */
        r_psi5_call_callback(p_ctrl, PSI5_EVENT_RX_DATA_FIFO_OVERFLOW);
    }

    if (R_PSI50_PSI5nRXMODST_RXDSCNFERR_Msk == (rx_data_err & R_PSI50_PSI5nRXMODST_RXDSCNFERR_Msk))
    {
        p_ctrl->p_reg->PSI5nRXMODSTCLR_b.RXDSCNFERRCLR = 1U; /* PRQA S 0662 # Required R-1.1 */

        /* Call user callback */
        r_psi5_call_callback(p_ctrl, PSI5_EVENT_SYNC_PULSE_AND_RX_DATA_CONFLICT);
    }
}

/*******************************************************************************************************************//**
 * Handle action after transmit data empty interrupt happens
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 **********************************************************************************************************************/
void r_psi5_transmit_data (psi5_instance_ctrl_t * p_ctrl)                             /* PRQA S 3224, 3673 # Required R-8.8, Advisory R-8.13 */
{
    uint32_t              tx_data  = (*p_ctrl->p_tx_source);
    psi5_extended_cfg_t * p_extend = (psi5_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Required R-11.8, Advisory R-11.5, Advisory R-8.13 */

    if (PSI5_TX_FRAME_FORMAT_SHORT == p_extend->tx_setting.frame_format)
    {
        tx_data = (tx_data & PSI5_PRV_TX_FRAME_FORMAT_SHORT_MASK_BITS);
    }
    else if (PSI5_TX_FRAME_FORMAT_LONG == p_extend->tx_setting.frame_format)
    {
        tx_data = (tx_data & PSI5_PRV_TX_FRAME_FORMAT_LONG_MASK_BITS);
    }
    else if (PSI5_TX_FRAME_FORMAT_XLONG == p_extend->tx_setting.frame_format)
    {
        tx_data = (tx_data & PSI5_PRV_TX_FRAME_FORMAT_XLONG_MASK_BITS);
    }
    else if (PSI5_TX_FRAME_FORMAT_XXLONG == p_extend->tx_setting.frame_format)
    {
        tx_data = (tx_data & PSI5_PRV_TX_FRAME_FORMAT_XXLONG_MASK_BITS);
    }
    else
    {
        /* Do nothing */
    }

    /* Write transmit data */
    p_ctrl->p_reg->PSI5nTXDATA = tx_data;                                     /* PRQA S 0662 # Required R-1.1 */

    /* Disable the transmit data empty interrupt */
    p_ctrl->p_reg->PSI5nTXSTINTEN_b.TXDEMPTYINTEN = 0U;                       /* PRQA S 0662 # Required R-1.1 */
}

/*******************************************************************************************************************//**
 * Calls user callback
 *
 * @param[in]     p_ctrl    Pointer to the instance control block.
 * @param[in]     event     PSI5 event
 **********************************************************************************************************************/
void r_psi5_call_callback (psi5_instance_ctrl_t * p_ctrl, psi5_event_t event) /* PRQA S 3224, 3673 # Required R-8.8, Advisory R-8.13 */
{
    /* Call user callback if provided, if an event was determined, and if the driver is initialized. */
    if (NULL != p_ctrl->p_callback)
    {
        psi5_callback_args_t args;

        /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
         * stored in non-secure memory so they can be accessed by a non-secure callback function. */
        psi5_callback_args_t * p_args = p_ctrl->p_callback_memory;

        if (NULL == p_args)
        {
            /* Use provided args struct on stack */
            p_args = &args;
        }
        else
        {
            /* Save current arguments on the stack in case this is a nested interrupt. */
            args = *p_args;
        }

        p_args->p_context = p_ctrl->p_context;
        p_args->event     = event;

        p_ctrl->p_callback(p_args);

        if (NULL != p_ctrl->p_callback_memory)
        {
            /* Restore callback memory in case this is a nested interrupt. */
            *p_ctrl->p_callback_memory = args;
        }
    }
    else
    {

        /* Return if user callback is not available */
        return;
    }
}

#if PSI5_CFG_TRANSFER_ENABLE           /* PRQA S 3332 # Required R-20.9 */

/*******************************************************************************************************************//**
 * Configures any dependent drivers selected by the user, including transfer and timer drivers.
 *
 * @param[in] p_cfg                    Pointer to the configuration structure.
 * @param[in] p_instance_ctrl          Pointer to the instance control block.
 *
 * @retval FSP_SUCCESS                 Dependent drivers configured successfully.
 * @return                             See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                     possible return codes. This function calls:
 *                                         * @ref transfer_api_t::open
 **********************************************************************************************************************/
static fsp_err_t r_psi5_dependent_drivers_configure (psi5_cfg_t const * const     p_cfg,
                                                     psi5_instance_ctrl_t * const p_instance_ctrl)
{
    /* Prepare transfer configuration. */
    fsp_err_t err_transfer_rx = FSP_SUCCESS;

    /* If a transfer instance is provided for read, open the transfer instance. */
    if (NULL != p_cfg->p_transfer_rx)
    {
        err_transfer_rx = p_cfg->p_transfer_rx->p_api->open(p_cfg->p_transfer_rx->p_ctrl, p_cfg->p_transfer_rx->p_cfg);
        FSP_ERROR_RETURN((FSP_SUCCESS == err_transfer_rx), err_transfer_rx);

        err_transfer_rx =
            p_cfg->p_transfer_rx->p_api->callbackSet(p_cfg->p_transfer_rx->p_ctrl,
                                                     r_psi5_rxi_transfer_callback, /* PRQA S 3635 # Advisory R-17.12 */
                                                     p_instance_ctrl,
                                                     NULL);
        FSP_ERROR_RETURN((FSP_SUCCESS == err_transfer_rx), err_transfer_rx);
    }

    return FSP_SUCCESS;
}

#endif
#if PSI5_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Parameter checking of R_PSI5_Open.
 *
 * @param[in]   p_api_ctrl                Pointer to control structure from application.
 * @param[in]   p_cfg                     Pointer to configuration structure. All elements of the structure must be
 *                                        set by user.
 *
 * @retval FSP_SUCCESS                    Input Parameters are Valid.
 * @retval FSP_ERR_ASSERTION              An input parameter is invalid.
 * @retval FSP_ERR_IRQ_BSP_DISABLED       Callback is NULL and the PSI5 IRQ is not enabled.
 * @retval FSP_ERR_ALREADY_OPEN           The control structure is already opened.
 **********************************************************************************************************************/
static fsp_err_t r_psi5_open_parameter_checking (psi5_ctrl_t * const p_api_ctrl, psi5_cfg_t const * const p_cfg) /* PRQA S 3673 # Advisory R-8.13 */
{
    FSP_ASSERT(NULL != p_api_ctrl);

    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_api_ctrl;                                         /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

    FSP_ERROR_RETURN(PSI5_PRV_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);

    if ((NULL != p_cfg->p_callback))
    {
        if (BSP_IRQ_DISABLED != p_cfg->rx_data_exist_ipl)
        {
            FSP_ERROR_RETURN(p_cfg->rx_data_exist_irq >= 0, FSP_ERR_IRQ_BSP_DISABLED); /* PRQA S 1880 # Required R-10.4 */
        }

        if (BSP_IRQ_DISABLED != p_cfg->tx_empty_ipl)
        {
            FSP_ERROR_RETURN(p_cfg->tx_empty_irq >= 0, FSP_ERR_IRQ_BSP_DISABLED); /* PRQA S 1880 # Required R-10.4 */
        }

        if (BSP_IRQ_DISABLED != p_cfg->status_ipl)
        {
            FSP_ERROR_RETURN(p_cfg->status_irq >= 0, FSP_ERR_IRQ_BSP_DISABLED); /* PRQA S 1880 # Required R-10.4 */
        }
    }

    return FSP_SUCCESS;
}

#endif

#if PSI5_CFG_TRANSFER_ENABLE                                                             /* PRQA S 3332 # Required R-20.9 */

/*******************************************************************************************************************//**
 * DMAC Receive ISR callback
 **********************************************************************************************************************/
void r_psi5_rxi_transfer_callback (transfer_callback_args_t * p_transfer_args)           /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) p_transfer_args->p_context; /* PRQA S 0306, 0316, 0311 # Framework requires void pointer context casting, Advisory R-11.5, Required R-11.8 */

    if ((NULL != p_ctrl) && (NULL != p_transfer_args))
    {
        r_psi5_read_data_frame(p_ctrl);
    }
}

#endif

/*******************************************************************************************************************//**
 * PSI5 ISR
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void r_psi5_tx_isr (void) /* PRQA S 1503 # Required R-2.1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /*  Clear the level interrupt flag of the peripheral module*/
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Write transmit data to empty transmit buffer*/
    r_psi5_transmit_data(p_ctrl);

    /* Call user callback */
    r_psi5_call_callback(p_ctrl, PSI5_EVENT_TRANSMIT_DATA_EMPTY);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

#if BSP_FEATURE_PSI5_WP_IS_AVAILABLE
BSP_INTERRUPT_ATTRIBUTE void r_psi5_wp_isr (void) /* PRQA S 1503 # Required R-2.1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /*  Clear the level interrupt flag of the peripheral module*/
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Disable controls shifting channel enable interrupt */
    p_ctrl->p_wp_reg->PSI5WPASTI_b.SCPI = 0U;                                          /* PRQA S 0662 # Required R-1.1 */

    /* Call user callback */
    r_psi5_call_callback(p_ctrl, PSI5_EVENT_WP_ALL_CHANNEL);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

#endif

BSP_INTERRUPT_ATTRIBUTE void r_psi5_status_isr (void) /* PRQA S 1503 # Required R-2.1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Clear the level interrupt flag of the peripheral module*/
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    r_psi5_status_handler(p_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

BSP_INTERRUPT_ATTRIBUTE void r_psi5_rx_isr (void) /* PRQA S 1503 # Required R-2.1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /*  Clear the level interrupt flag of the peripheral module*/
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    psi5_instance_ctrl_t * p_ctrl = (psi5_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Read data frame */
    r_psi5_read_data_frame(p_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}
