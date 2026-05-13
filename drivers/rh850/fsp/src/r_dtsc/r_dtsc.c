/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_dtsc.h"
#include "r_dtsc_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define DTSC_TRANSFER_MAX_LENGTH              (0xFFFF)

/* "DTSC" in ASCII, used to determine if channel is open */
#define DTSC_OPEN                             (0x44545343U)

/* Maximum DTSC channels per unit */
#define BSP_FEATURE_DTSC_MAX_CHANNEL          (128U)

/* Common macro to disable bit */
#define DTSC_PRV_DISABLE_BIT                  (0U)

/* Common macro to enable bit */
#define DTSC_PRV_ENABLE_BIT                   (1U)

/* Common macro to enable channel */
#define DTSC_PRV_CHANNEL_ENABLE               (1U)

/* Common macro to disable channel */
#define DTSC_PRV_CHANNEL_DISABLE              (0U)

/* Used to generate a software transfer request by writing 1 to bit DTFSS_b.DRQS */
#define DTSC_PRV_SOFTWARE_REQUEST_GENERATE    (1U)

/* Used to clear a software transfer request by writing 1 to bit DTFSC_b.DRQC */
#define DTSC_PRV_SOFTWARE_REQUEST_CLEAR       (1U)

/* Transfer disable on error offset bitfield position */
#define DTSC_PRV_DTTCT_ESE_OFFSET             (27U)

/* Next channel to chain select offset bitfield position */
#define DTSC_PRV_DTTCT_CHNSEL_OFFSET          (18U)

/* Chain function enable offset bitfield position */
#define DTSC_PRV_DTTCT_CHNE_OFFSET            (16U)

/* Transfer count match enable offset bitfield position */
#define DTSC_PRV_DTTCT_CCE_OFFSET             (15U)

/* Transfer completion enable offset bitfield position */
#define DTSC_PRV_DTTCT_TCE_OFFSET             (14U)

/* Reload function 2 offset bitfield position */
#define DTSC_PRV_DTTCT_RLD2M_OFFSET           (11U)

/* Reload function 1 offset bitfield position */
#define DTSC_PRV_DTTCT_RLD1M_OFFSET           (9U)

/* Destination address offset bitfield position */
#define DTSC_PRV_DTTCT_DACM_OFFSET            (7U)

/* Source address offset bitfield position */
#define DTSC_PRV_DTTCT_SACM_OFFSET            (5U)

/* Data size offset bitfield position */
#define DTSC_PRV_DTTCT_DS_OFFSET              (2U)

/* Address reload count offset bitfield position */
#define DTSC_PRV_DTTC_ARC_OFFSET              (16U)

/* Reload address reload count offset bitfield position */
#define DTSC_PRV_DTRTC_RARC_OFFSET            (16U)

#define DTSC_SPID_MASK                        (0x1FU)
#define DTSC_CHAIN_SPID_MASK                  (0x1FU)
#define DTSC_UM_MASK                          (0x01U)
#define DTSC_CHAIN_UM_MASK                    (0x01U)

#define DTSC_TRANSFER_REQUEST_GROUP0          (0U)
#define DTSC_MAX_ACTIVATION_SOURCE            (128U)

#define DTSC_DEFSEL                           (0xFF090600U)
#define DTSC_PRV_REG_DEFSEL                   ((R_DTSTRGSEL_Type *) (DTSC_DEFSEL))

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void r_dtsc_prv_enable(dtsc_instance_ctrl_t * p_ctrl);
static void r_dtsc_prv_disable(dtsc_instance_ctrl_t * p_ctrl);
static void r_dtsc_config_transfer_info(dtsc_instance_ctrl_t * p_ctrl, transfer_info_t * p_info);
static void r_dtsc_clear_all_transfer_status(dtsc_instance_ctrl_t * p_ctrl);
static void r_dtsc_config_reliability_func(dtsc_instance_ctrl_t * p_ctrl);
static void r_dtsc_set_hardware_request(dtsc_instance_ctrl_t * p_ctrl);
static void r_dtsc_prv_select_group_for_act_src(dtsc_instance_ctrl_t * p_ctrl);

#if DTSC_CFG_PARAM_CHECKING_ENABLE
static fsp_err_t r_dtsc_open_parameter_checking(dtsc_instance_ctrl_t * const p_ctrl,
                                                transfer_cfg_t const * const p_cfg);
static fsp_err_t r_dtsc_enable_parameter_checking(dtsc_instance_ctrl_t * const p_ctrl);

#endif

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void dtsc_transfer_isr(void);
BSP_INTERRUPT_ATTRIBUTE void dtsc_transfer_error_isr(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* This table is used to store the context in the ISR. */
void * gp_dtsc_isr_context[BSP_FEATURE_DTSC_MAX_CHANNEL];

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/** DTSC implementation of transfer API. */
const transfer_api_t g_transfer_on_dtsc =
{
    .open          = R_DTSC_Open,
    .reconfigure   = R_DTSC_Reconfigure,
    .reset         = R_DTSC_Reset,
    .infoGet       = R_DTSC_InfoGet,
    .softwareStart = R_DTSC_SoftwareStart,
    .softwareStop  = R_DTSC_SoftwareStop,
    .enable        = R_DTSC_Enable,
    .disable       = R_DTSC_Disable,
    .reload        = R_DTSC_Reload,
    .close         = R_DTSC_Close,
    .callbackSet   = R_DTSC_CallbackSet,
};

/*******************************************************************************************************************//**
 * @addtogroup DTSC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configure DTSC channel.
 *
 * Implement
 * @retval FSP_SUCCESS                    Successful open.
 * @retval FSP_ERR_ASSERTION              An input parameter is invalid.
 * @retval FSP_ERR_IRQ_BSP_DISABLED       The IRQ associated with the activation source is not enabled in the BSP.
 * @retval FSP_ERR_ALREADY_OPEN           The control structure is already opened.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT The configured channel is invalid.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Open (transfer_ctrl_t * const p_api_ctrl, transfer_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    err = r_dtsc_open_parameter_checking(p_api_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl;

    /* Load bus configuration from application */
    p_ctrl->p_cfg = p_cfg;
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    /* Set callback and context pointers, if configured */
    p_ctrl->p_callback        = p_extend->p_callback;
    p_ctrl->p_context         = p_extend->p_context;
    p_ctrl->p_callback_memory = NULL;
    p_ctrl->p_reg             = (R_DTS0_Type *) R_DTS0;

    if ((FSP_INVALID_VECTOR != p_extend->error_irq) && (BSP_IRQ_DISABLED != p_extend->transfer_error_ipl))
    {
        /* Enable DTSC error in transfer interrupt */
        R_BSP_IrqCfgEnable(p_extend->error_irq, p_extend->transfer_error_ipl, p_ctrl);
    }

    if ((FSP_INVALID_VECTOR != p_extend->complete_irq) && (BSP_IRQ_DISABLED != p_extend->transfer_complete_ipl))
    {
        /* Enable DTSC transfer complete channel interrupt */
        R_BSP_IrqCfgEnable(p_extend->complete_irq, p_extend->transfer_complete_ipl, p_ctrl);
    }

    if ((FSP_INVALID_VECTOR != p_extend->count_match_irq) && (BSP_IRQ_DISABLED != p_extend->transfer_count_match_ipl))
    {
        /* Enable DTSC transfer count match interrupt */
        R_BSP_IrqCfgEnable(p_extend->count_match_irq, p_extend->transfer_count_match_ipl, p_ctrl);
    }

    /* Store the p_ctrl of channel to context*/
    gp_dtsc_isr_context[channel] = p_ctrl;

    /* Clear all DTSC transfer status */
    r_dtsc_clear_all_transfer_status(p_ctrl);

    /* Configure the transfer settings. */
    r_dtsc_config_transfer_info(p_ctrl, p_cfg->p_info);

    /* Configure reliability function*/
    if (TRANSFER_CHAIN_RESTRICTION_ENABLE == p_extend->p_chain_restrict_cfg->chain_restriction_mode)
    {
        r_dtsc_config_reliability_func(p_ctrl);
    }

    /* Check if hardware transfer request. */
    if (DTSC_TRIGGER_EVENT_SOFTWARE != p_extend->activation_source)
    {
        r_dtsc_set_hardware_request(p_ctrl);
    }

    /* Mark driver as open by initializing "DTSC" in its ASCII equivalent. */
    p_ctrl->open = DTSC_OPEN;

    return err;
}

/*******************************************************************************************************************//**
 * Reconfigure the transfer with new transfer info.
 *
 * Implement
 * @retval FSP_SUCCESS              Transfer is configured and will start when trigger occurs.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 * @retval FSP_ERR_INVALID_HW_CONDITION        DTSC transfer is ongoing, not be able to reconfigure
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Reconfigure (transfer_ctrl_t * const p_api_ctrl, transfer_info_t * p_info)
{
    /* Initialize error code */
    fsp_err_t err = FSP_SUCCESS;

    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl   = (dtsc_instance_ctrl_t *) p_api_ctrl;
    dtsc_extended_cfg_t  * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_info);
    FSP_ASSERT(NULL != p_extend);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    uint8_t  channel                    = p_extend->channel;
    uint32_t mask_channel_reconfig      = DTSC_PRV_ENABLE_BIT << channel;
    uint32_t mask_channel_trans_ongoing = p_ctrl->p_reg->DTSSTS_b.DTSACH;
    uint8_t  set_channel_enable         = DTSC_PRV_CHANNEL_DISABLE;

    /* Check if DTSC cycle is in progress and reconfigure channel is also ongoing transfer */
    if ((1U == p_ctrl->p_reg->DTSSTS_b.DTSCYC) ||
        ((1U == p_ctrl->p_reg->DTSSTS_b.DTSACT) && (mask_channel_reconfig == mask_channel_trans_ongoing)))
    {
        return FSP_ERR_INVALID_HW_CONDITION;
    }

    /* Check whether the channel DTSC has disabled */
    if (DTSC_PRV_DISABLE_BIT != p_ctrl->p_reg->CH[channel].DTFSL_b.REQEN)
    {
        /* Disable DTSC Channel */
        r_dtsc_prv_disable(p_ctrl);
        set_channel_enable = DTSC_PRV_CHANNEL_ENABLE;
    }

    /* Reconfigure the transfer settings. */
    r_dtsc_config_transfer_info(p_ctrl, p_info);

    if (DTSC_PRV_CHANNEL_ENABLE == set_channel_enable)
    {
        /* Enable the transfer configuration. */
        r_dtsc_prv_enable(p_ctrl);
    }

    return err;
}

/*******************************************************************************************************************//**
 * Reset transfer source, destination, and number of transfers.
 *
 * Implement
 * @retval FSP_SUCCESS              Transfer reset successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Reset (transfer_ctrl_t * const p_api_ctrl,
                        void const * volatile   p_src,
                        void * volatile         p_dest,
                        uint16_t const          num_transfers)
{
    /* Initialize error code */
    fsp_err_t err = FSP_SUCCESS;

    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl   = (dtsc_instance_ctrl_t *) p_api_ctrl;
    dtsc_extended_cfg_t  * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_extend);
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    uint8_t channel = p_extend->channel;

    /* Disable transfers if they are currently enabled. */
    r_dtsc_prv_disable(p_ctrl);

    if (NULL != p_src)
    {
        /* Configure the DTSC source pointer if it is provided. */
        p_ctrl->p_reg->CH[channel].DTSA = (uint32_t) p_src;
    }

    if (NULL != p_dest)
    {
        /* Configure the DTSC destination pointer if it is provided. */
        p_ctrl->p_reg->CH[channel].DTDA = (uint32_t) p_dest;
    }

    if ((TRANSFER_DTSC_MODE_SINGLE != (transfer_dtsc_mode_t) p_ctrl->p_reg->CH[channel].DTTCT_b.TRM))
    {
        /* Reset address reload count and transfer count. */
        p_ctrl->p_reg->CH[channel].DTTC_b.ARC = p_ctrl->p_cfg->p_info->addr_reload_count;
    }

    /* Reset the transfer count if it is single transfer mode. */
    p_ctrl->p_reg->CH[channel].DTTC_b.TRC = num_transfers;

    /* Clear all DTSC transfer status */
    r_dtsc_clear_all_transfer_status(p_ctrl);

    /* Enable the transfer configuration. */
    r_dtsc_prv_enable(p_ctrl);

    return err;
}

/*********************************************************************************************************************//**
 * Get transfer count and maximum transfer length from provided control block.
 *
 * Implement
 * @retval FSP_SUCCESS              Information has been written to p_info.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_InfoGet (transfer_ctrl_t * const p_api_ctrl, transfer_properties_t * const p_info)
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl   = (dtsc_instance_ctrl_t *) p_api_ctrl;
    dtsc_extended_cfg_t  * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_info);
    FSP_ASSERT(NULL != p_extend);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    uint8_t channel = p_extend->channel;

    p_info->transfer_length_max       = DTSC_TRANSFER_MAX_LENGTH;
    p_info->transfer_length_remaining = p_ctrl->p_reg->CH[channel].DTTC_b.TRC;
    p_info->block_count_remaining     = 0U;
    p_info->block_count_max           = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * To update next transfer information without interruption during transfer. Not support on DTSC module.
 *
 * Implement
 * @retval FSP_ERR_UNSUPPORTED        This feature is not supported.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Reload (transfer_ctrl_t * const p_api_ctrl,
                         void const            * p_src,
                         void                  * p_dest,
                         uint32_t const          num_transfers)
{
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(p_src);
    FSP_PARAMETER_NOT_USED(p_dest);
    FSP_PARAMETER_NOT_USED(num_transfers);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Generate a transfer request by software. "mode" is not used.
 *
 * Implement
 * @retval FSP_SUCCESS              Transfer started written successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 * @retval FSP_ERR_UNSUPPORTED      Handle was not configured for software activation.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_SoftwareStart (transfer_ctrl_t * const p_api_ctrl, transfer_start_mode_t mode)
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl   = (dtsc_instance_ctrl_t *) p_api_ctrl;
    dtsc_extended_cfg_t  * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    FSP_PARAMETER_NOT_USED(mode);
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_extend);
#endif

    /* Return error if the activation source is not software source */
    FSP_ERROR_RETURN(DTSC_TRIGGER_EVENT_SOFTWARE == p_extend->activation_source, FSP_ERR_UNSUPPORTED);
    uint8_t channel = p_extend->channel;

    /* Generate a transfer request. */
    p_ctrl->p_reg->CH[channel].DTFSS = (uint32_t) DTSC_PRV_SOFTWARE_REQUEST_GENERATE;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop software transfers, clear transfer request.
 *
 * Implement
 * @retval FSP_SUCCESS              Transfer stopped written successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_SoftwareStop (transfer_ctrl_t * const p_api_ctrl)
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl   = (dtsc_instance_ctrl_t *) p_api_ctrl;
    dtsc_extended_cfg_t  * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_extend);
#endif
    uint8_t channel = p_extend->channel;

    /* Clear transfer request pending bit. */
    p_ctrl->p_reg->CH[channel].DTFSC_b.DRQC = (uint32_t) DTSC_PRV_SOFTWARE_REQUEST_CLEAR;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enable transfers for the configured activation source, allow to send request to DTSFSL.
 *
 * Implement
 * @retval FSP_SUCCESS              Enable channel successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Enable (transfer_ctrl_t * const p_api_ctrl)
{
    /* Initialize error code */
    fsp_err_t err = FSP_SUCCESS;

    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl;

#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    err = r_dtsc_enable_parameter_checking(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    r_dtsc_prv_enable(p_ctrl);

    return err;
}

/*******************************************************************************************************************//**
 * Disable transfers so that they are no longer triggered by the activation source, not able to send request to DTSFSL.
 *
 * Implement
 * @retval FSP_SUCCESS              Disable channel successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Disable (transfer_ctrl_t * const p_api_ctrl)
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl;

#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    r_dtsc_prv_disable(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Disable transfer and clean up internal data.
 *
 * Implement
 * @retval FSP_SUCCESS           Successful close.
 * @retval FSP_ERR_ASSERTION     An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN      Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Close (transfer_ctrl_t * const p_api_ctrl)
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl   = (dtsc_instance_ctrl_t *) p_api_ctrl;
    dtsc_extended_cfg_t  * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_extend);
#endif

    uint8_t channel = p_extend->channel;

    r_dtsc_prv_disable(p_ctrl);

    /* Clear all DTSC transfer status */
    r_dtsc_clear_all_transfer_status(p_ctrl);

    /* Clear source address */
    p_ctrl->p_reg->CH[channel].DTSA = (uint32_t) NULL;

    /* Clear destination address */
    p_ctrl->p_reg->CH[channel].DTDA = (uint32_t) NULL;

    /* Clear DTSC number transfer and address reload count */
    p_ctrl->p_reg->CH[channel].DTTC = DTSC_PRV_DISABLE_BIT;

    /* Clear DTSC number reload transfer and reload address reload count */
    p_ctrl->p_reg->CH[channel].DTRTC = DTSC_PRV_DISABLE_BIT;

    /* Clear DTSC source reload address */
    p_ctrl->p_reg->CH[channel].DTRSA = (uint32_t) NULL;

    /* Clear DTSC destination reload address */
    p_ctrl->p_reg->CH[channel].DTRDA = (uint32_t) NULL;

    /* Clear old priority bits for this channel */
    p_ctrl->p_reg->DTSPR0 &= ~((uint32_t) (0x3U) << (channel * DTSC_PRV_DTTCT_DS_OFFSET));

    /* Clear DTSC channel master setting register */
    p_ctrl->p_reg->DTSCM_CH[channel] = DTSC_PRV_DISABLE_BIT;

    /* Clear DTSC transfer count compare register */
    p_ctrl->p_reg->CH[channel].DTTCC = DTSC_PRV_DISABLE_BIT;

    if (FSP_INVALID_VECTOR != p_extend->error_irq)
    {
        /* Disable DTSC error in transfer interrupt */
        R_BSP_IrqDisable(p_extend->error_irq);
        R_FSP_IsrContextSet(p_extend->error_irq, NULL);
    }

    if (FSP_INVALID_VECTOR != p_extend->complete_irq)
    {
        /* Disable DTSC transfer complete channel interrupt */
        R_BSP_IrqDisable(p_extend->complete_irq);
        R_FSP_IsrContextSet(p_extend->complete_irq, NULL);
    }

    if (FSP_INVALID_VECTOR != p_extend->count_match_irq)
    {
        /* Disable DTSC transfer count match interrupt */
        R_BSP_IrqDisable(p_extend->count_match_irq);
        R_FSP_IsrContextSet(p_extend->count_match_irq, NULL);
    }

    /* Clear ID so control block can be reused. */
    p_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 *
 * Implement
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_CallbackSet (transfer_ctrl_t * const          p_api_ctrl,
                              void (                         * p_callback)(transfer_callback_args_t *),
                              void * const                     p_context,
                              transfer_callback_args_t * const p_callback_memory)
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl;
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_callback);
#endif

    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup DTSC)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Enable DTSC channel.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dtsc_prv_enable (dtsc_instance_ctrl_t * p_ctrl)
{
    /* Initialization */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    /* Enable DTSC channel operation */
    p_ctrl->p_reg->CH[channel].DTFSL_b.REQEN = DTSC_PRV_ENABLE_BIT;
}

/*******************************************************************************************************************//**
 * Disable DTSC channel.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 **********************************************************************************************************************/
static void r_dtsc_prv_disable (dtsc_instance_ctrl_t * p_ctrl)
{
    /* Initialization */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    /* Disable DTSC channel operation */
    p_ctrl->p_reg->CH[channel].DTFSL_b.REQEN = DTSC_PRV_DISABLE_BIT;
}

/*******************************************************************************************************************//**
 * Write the transfer info to the hardware registers.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 * @param[in]   p_info         Pointer to properties of transfer .
 *
 **********************************************************************************************************************/
static void r_dtsc_config_transfer_info (dtsc_instance_ctrl_t * p_ctrl, transfer_info_t * p_info)
{
    /* Get config from control block */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

    /* Get config channel */
    uint8_t channel = p_extend->channel;

    /* Configure source address */
    p_ctrl->p_reg->CH[channel].DTSA = (uint32_t) p_info->p_src;

    /* Configure destination address */
    p_ctrl->p_reg->CH[channel].DTDA = (uint32_t) p_info->p_dest;

    /* Configure DTSC number transfer and address reload count */
    p_ctrl->p_reg->CH[channel].DTTC = (uint32_t) p_info->number_transfer |
                                      ((uint32_t) p_info->addr_reload_count << DTSC_PRV_DTTC_ARC_OFFSET);

    /* Configure DTSC number reload transfer and reload address reload count */
    p_ctrl->p_reg->CH[channel].DTRTC = (uint32_t) p_info->reload_number_transfer |
                                       ((uint32_t) p_info->reload_addr_reload_count << DTSC_PRV_DTRTC_RARC_OFFSET);

    /* Configure DTSC source reload address */
    p_ctrl->p_reg->CH[channel].DTRSA = (uint32_t) p_info->p_reload_src_addr;

    /* Configure DTSC destination reload address */
    p_ctrl->p_reg->CH[channel].DTRDA = (uint32_t) p_info->p_reload_des_addr;

    /* Configure DTSC transfer control register */
    p_ctrl->p_reg->CH[channel].DTTCT = (uint32_t) (p_info->transfer_control_cfg.transfer_control);

    /* Configure DTSC channel master setting register */
    p_ctrl->p_reg->DTSCM_CH[channel] = (uint16_t) p_info->number_transfer_count_match;

    /* Configure DTSC transfer count compare register */
    p_ctrl->p_reg->CH[channel].DTTCC = (uint16_t) p_info->number_transfer_count_match;
}

/*******************************************************************************************************************//**
 * Clear all transfer status.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dtsc_clear_all_transfer_status (dtsc_instance_ctrl_t * p_ctrl)
{
    /* Get config from control block */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

    /* Get config channel */
    uint8_t channel = p_extend->channel;

    /* Clear transfer error */
    p_ctrl->p_reg->CH[channel].DTFSC_b.ERC = 1U;

    /* Clear transfer count match flag */
    p_ctrl->p_reg->CH[channel].DTFSC_b.CCC = 1U;

    /* Clear transfer completion flag */
    p_ctrl->p_reg->CH[channel].DTFSC_b.TCC = 1U;

#if (1U == BSP_FEATURE_DTSC_TRANSFER_RQ_OVERFLOW_FLAG)

    /* Clear transfer request overflow flag */
    p_ctrl->p_reg->CH[channel].DTFSC_b.OVFC = 1U;
#endif

    /* Clear transfer request */
    p_ctrl->p_reg->CH[channel].DTFSC_b.DRQC = 1U;
}

/*******************************************************************************************************************//**
 * DTSC Interrupt service routine
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to dtsc instance control block
 * @param[in]     event      Event code
 **********************************************************************************************************************/
static void r_dtsc_callback (dtsc_instance_ctrl_t * p_ctrl, transfer_event_t event)
{
    /* Initialization */
    transfer_callback_args_t args;

    transfer_callback_args_t * p_args = p_ctrl->p_callback_memory;
    if (NULL == p_args)
    {
        /* Store on stack */
        p_args = &args;
    }
    else
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        args = *p_args;
    }

    p_args->event     = event;
    p_args->p_context = p_ctrl->p_context;

    p_ctrl->p_callback(p_args);

    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }
}

#if DTSC_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Parameter checking of R_DTSC_Open.
 *
 * @param[in]   p_ctrl                    Pointer to control structure.
 * @param[in]   p_cfg                     Pointer to configuration structure. All elements of the structure must be
 *                                        set by user.
 *
 * @retval FSP_SUCCESS                    Input Parameters are Valid.
 * @retval FSP_ERR_ASSERTION              An input parameter is invalid.
 * @retval FSP_ERR_IRQ_BSP_DISABLED       Callback is NULL and the DTSC IRQ is not enabled.
 * @retval FSP_ERR_ALREADY_OPEN           The control structure is already opened.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT Channel is not available on this MCU.
 **********************************************************************************************************************/
static fsp_err_t r_dtsc_open_parameter_checking (dtsc_instance_ctrl_t * const p_ctrl,
                                                 transfer_cfg_t const * const p_cfg)
{
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(p_ctrl->open != DTSC_OPEN, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_cfg->p_extend;

    FSP_ERROR_RETURN(p_extend->channel < BSP_FEATURE_DTSC_MAX_CHANNEL, FSP_ERR_IP_CHANNEL_NOT_PRESENT);

    if ((NULL != p_extend->p_callback))
    {
        FSP_ERROR_RETURN(((0 < p_extend->complete_irq) && (FSP_INVALID_VECTOR >= p_extend->complete_irq)),
                         FSP_ERR_IRQ_BSP_DISABLED);
        FSP_ERROR_RETURN(((0 < p_extend->error_irq) && (FSP_INVALID_VECTOR >= p_extend->error_irq)),
                         FSP_ERR_IRQ_BSP_DISABLED);
        FSP_ERROR_RETURN(((0 < p_extend->count_match_irq) && (FSP_INVALID_VECTOR >= p_extend->count_match_irq)),
                         FSP_ERR_IRQ_BSP_DISABLED);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Parameter checking for R_DTSC_Enable.
 *
 * @param[in]   p_ctrl                           Pointer to control structure.
 *
 * @retval      FSP_SUCCESS                      Alignment on source and destination pointers is valid.
 * @retval      FSP_ERR_ASSERTION                An input parameter is invalid.
 **********************************************************************************************************************/
static fsp_err_t r_dtsc_enable_parameter_checking (dtsc_instance_ctrl_t * const p_ctrl)
{
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;
    void const          * p_src    = (void const *) p_ctrl->p_reg->CH[channel].DTSA;
    void const          * p_dest   = (void const *) p_ctrl->p_reg->CH[channel].DTDA;
    transfer_dtsc_mode_t  mode     = (transfer_dtsc_mode_t) p_ctrl->p_reg->CH[channel].DTTCT_b.TRM;

    /* The source and destination pointers cannot be NULL. */
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);

    if (TRANSFER_DTSC_MODE_SINGLE == mode)
    {
        /* Setting transfer count to 0 in normal mode means transfer forever. This feature is not supported. */
        FSP_ASSERT(0 != p_ctrl->p_reg->CH[channel].DTTC_b.TRC);
    }

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * Callback when transfer count match or transfer complete
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void dtsc_transfer_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type              irq = R_FSP_CurrentIrqGet();
    dtsc_instance_ctrl_t * p_ctrl;
    transfer_event_t       event;
    R_INTIF0_Type        * p_dtsc_irq = (R_INTIF0_Type *) R_INTIF0;

    /* Recover ISR context saved in open to get the Peripheral Interrupt Status Register. */
    dtsc_instance_ctrl_t * p_ctrl_isr = (dtsc_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    dtsc_extended_cfg_t  * p_extend   = (dtsc_extended_cfg_t *) p_ctrl_isr->p_cfg->p_extend;

    /* Variable to store detected channel number. */
    uint8_t channel = 0U;

    /* Calculate register group index (each PINT register group handles 32 channels). */
    uint8_t registers = p_extend->channel / 32U;

    /* Read interrupt status registers. */
    uint32_t transfer_complete_irq    = p_dtsc_irq->PINT[registers];
    uint32_t transfer_count_match_irq = p_dtsc_irq->PINT4[registers];

    if (0U != transfer_complete_irq)
    {
        channel = ((SCH1R(transfer_complete_irq) - 1U) + (registers * 32U));

        /* Clear Peripheral Interrupt Status */
        p_dtsc_irq->PINTCLR[registers] = transfer_complete_irq;

        /* Dummy read PINT0 register */
        transfer_complete_irq = p_dtsc_irq->PINT[registers];
    }
    /* Check if transfer count mach interrupt present in this channel  */
    else if (0U != transfer_count_match_irq)
    {
        channel = ((SCH1R(transfer_count_match_irq) - 1U) + (registers * 32U));

        /* Clear Peripheral Interrupt Status */
        p_dtsc_irq->PINTCLR4[registers] = transfer_count_match_irq;

        /* Dummy read PINT4 register */
        transfer_count_match_irq = p_dtsc_irq->PINT4[registers];
    }
    else
    {
        /* Do Nothing */
    }

    /* Get p_ctrl for the detected channel. */
    p_ctrl = (dtsc_instance_ctrl_t *) gp_dtsc_isr_context[channel];
    event  = (transfer_event_t) (p_ctrl->p_reg->CH[channel].DTFST);

    /* Clear transfer completion and count match flags. */
    p_ctrl->p_reg->CH[channel].DTFSC_b.TCC = 1U;

    /* Clear transfer count match flag */
    p_ctrl->p_reg->CH[channel].DTFSC_b.CCC = 1U;

    /* Call user callback */
    r_dtsc_callback(p_ctrl, event);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * DTSC Error ISR
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void dtsc_transfer_error_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Recover ISR context saved in open. */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Assign unit, channel */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;
    transfer_event_t      event    = TRANSFER_EVENT_ERR_TRANS_READ_CYCLE;
    uint32_t              dtfst    = p_ctrl->p_reg->CH[channel].DTFST;

    /* DTS transfer error occurred in the write cycle. Bit ERWR of DTFST resgister is 1 */
    if (DTSC_PRV_DISABLE_BIT != (dtfst & R_DTS0_CH_DTFST_ERWR_Msk))
    {
        event = TRANSFER_EVENT_ERR_TRANS_WRITE_CYCLE;
    }
    else
    {
        /* Nothing to do */
    }

    /* Call user callback */
    r_dtsc_callback(p_ctrl, event);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Write reliability function info to hardware register.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dtsc_config_reliability_func (dtsc_instance_ctrl_t * p_ctrl)
{
    /* Initialization */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    /* Setting for reliability function */
    p_ctrl->p_reg->DTSCM_CH_b[channel].CHAIN_RESTRICT = DTSC_PRV_ENABLE_BIT;
    p_ctrl->p_reg->DTSCM_CH_b[channel].SPID           = (uint8_t) (p_extend->p_chain_restrict_cfg->master_spid & \
                                                                   DTSC_SPID_MASK);
    p_ctrl->p_reg->DTSCM_CH_b[channel].CHAIN_SPID = (uint8_t) (p_extend->p_chain_restrict_cfg->chain_spid & \
                                                               DTSC_CHAIN_SPID_MASK);
    p_ctrl->p_reg->DTSCM_CH_b[channel].UM = (uint8_t) (p_extend->p_chain_restrict_cfg->master_um & \
                                                       DTSC_UM_MASK);
    p_ctrl->p_reg->DTSCM_CH_b[channel].CHAIN_UM = (uint8_t) (p_extend->p_chain_restrict_cfg->chain_um & \
                                                             DTSC_CHAIN_UM_MASK);
}

/*******************************************************************************************************************//**
 * Generate a hardware transfer request.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dtsc_set_hardware_request (dtsc_instance_ctrl_t * p_ctrl)
{
    /* Select group for trigger source */
    r_dtsc_prv_select_group_for_act_src(p_ctrl);
}

/*******************************************************************************************************************//**
 * Select group for trigger source.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dtsc_prv_select_group_for_act_src (dtsc_instance_ctrl_t * p_ctrl)
{
    /* Initialization */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               sel_register;
    uint8_t               sel_register_bit;
    uint16_t              source        = p_extend->activation_source;
    uint8_t               request_group = source / DTSC_MAX_ACTIVATION_SOURCE;
    volatile uint32_t   * sel_registers;

    p_ctrl->p_hwsel = DTSC_PRV_REG_DEFSEL;

    /* Select correct DTSSEL0 register array based on unit0 */
    sel_registers = (volatile uint32_t *) (&p_ctrl->p_hwsel->DTSSEL0);

    /* Calculate register index and bit offset */
    /* Select DTS transfer request group 0 */
    if (DTSC_TRANSFER_REQUEST_GROUP0 == request_group)
    {
        sel_register     = source / 8U;
        sel_register_bit = (uint8_t) ((source % 8U) * 4U);

        /* Clear the corresponding bit */
        sel_registers[sel_register] |= ((uint32_t) (request_group << sel_register_bit));
    }
    /* Select DTS transfer request other group */
    else
    {
        source           = p_extend->activation_source - (request_group * DTSC_MAX_ACTIVATION_SOURCE);
        sel_register     = source / 8U;
        sel_register_bit = (uint8_t) ((source % 8U) * 4U);

        /* Clear the corresponding bit */
        sel_registers[sel_register] |= ((uint32_t) (request_group << sel_register_bit));
    }
}
