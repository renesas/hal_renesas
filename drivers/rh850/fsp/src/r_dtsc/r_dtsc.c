/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_dtsc.h"                    /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "r_dtsc_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define DTSC_TRANSFER_MAX_LENGTH              (0xFFFFU)

/* "DTSC" in ASCII, used to determine if channel is open */
#define DTSC_OPEN                             (0x44545343U)

/* Maximum DTSC channels per unit */
#define BSP_FEATURE_DTSC_MAX_CHANNEL          (128U)

/* Common macro to disable bit */
#define DTSC_PRV_DISABLE_BIT                  (0U)

/* Common macro to enable bit */
#define DTSC_PRV_ENABLE_BIT                   (1U)

/* Macro to clear status register flag */
#define DTSC_PRV_CLEAR_BIT                    (1U)

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

#define DTSC_PRV_DTSSTS_ACTIVE_CH_MASK        (R_DTS0_DTSSTS_DTSACH_Msk | R_DTS0_DTSSTS_DTSACT_Msk)

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
static void r_dtsc_enable_isr_still_open(dtsc_instance_ctrl_t * p_ctrl);

#if DTSC_CFG_PARAM_CHECKING_ENABLE
static fsp_err_t r_dtsc_open_parameter_checking(dtsc_instance_ctrl_t * const p_ctrl,
                                                transfer_cfg_t const * const p_cfg);
static fsp_err_t r_dtsc_enable_parameter_checking(dtsc_instance_ctrl_t * const p_ctrl);

#endif

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void dtsc_transfer_isr(void);       /* PRQA S 3449, 3451 # Required R-8.5, Required R-8.5 */
BSP_INTERRUPT_ATTRIBUTE void dtsc_transfer_error_isr(void); /* PRQA S 3449, 3451 # Required R-8.5, Required R-8.5 */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* This table is used to store the context in the ISR. */
static void * gp_dtsc_isr_context[BSP_FEATURE_DTSC_MAX_CHANNEL];

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/** DTSC implementation of transfer API. */
const transfer_api_t g_transfer_on_dtsc =  /* PRQA S 1502 # Advisory R-2.8 */
{
    .open          = R_DTSC_Open,          /* PRQA S 3635 # Rule-17.12 */
    .reconfigure   = R_DTSC_Reconfigure,   /* PRQA S 3635 # Rule-17.12 */
    .reset         = R_DTSC_Reset,         /* PRQA S 3635 # Rule-17.12 */
    .infoGet       = R_DTSC_InfoGet,       /* PRQA S 3635 # Rule-17.12 */
    .softwareStart = R_DTSC_SoftwareStart, /* PRQA S 3635 # Rule-17.12 */
    .softwareStop  = R_DTSC_SoftwareStop,  /* PRQA S 3635 # Rule-17.12 */
    .enable        = R_DTSC_Enable,        /* PRQA S 3635 # Rule-17.12 */
    .disable       = R_DTSC_Disable,       /* PRQA S 3635 # Rule-17.12 */
    .reload        = R_DTSC_Reload,        /* PRQA S 3635 # Rule-17.12 */
    .close         = R_DTSC_Close,         /* PRQA S 3635 # Rule-17.12 */
    .callbackSet   = R_DTSC_CallbackSet,   /* PRQA S 3635 # Rule-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup DTSC
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

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
 * @retval FSP_ERR_INVALID_SIZE           The number of transfers is larger than the supported maximum size.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Open (transfer_ctrl_t * const p_api_ctrl, transfer_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    fsp_err_t err = FSP_SUCCESS;

    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    err = r_dtsc_open_parameter_checking(p_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_ASSERT(NULL != p_cfg->p_info);
    FSP_ERROR_RETURN(DTSC_TRANSFER_MAX_LENGTH >= p_cfg->p_info->number_transfer, FSP_ERR_INVALID_SIZE);
#endif

    /* Load bus configuration from application */
    p_ctrl->p_cfg = p_cfg;
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               channel  = p_extend->channel;

    /* Set callback and context pointers, if configured */
    p_ctrl->p_callback        = p_extend->p_callback;
    p_ctrl->p_context         = p_extend->p_context;
    p_ctrl->p_callback_memory = NULL;
    p_ctrl->p_reg             = (R_DTS0_Type *) R_DTS0; /* PRQA S 0306 # Advisory R-11.4 */

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

    /* Store the p_ctrl of channel to context */
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
 * @retval FSP_SUCCESS                         Transfer is configured and will start when trigger occurs.
 * @retval FSP_ERR_ASSERTION                   An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN                    Handle is not initialized.  Call R_DTSC_Open to initialize the control block.
 * @retval FSP_ERR_INVALID_HW_CONDITION        The channel being reconfigured is currently executing a transfer.
 * @retval FSP_ERR_INVALID_SIZE                The number of transfers is larger than the supported maximum size.
 **********************************************************************************************************************/
fsp_err_t R_DTSC_Reconfigure (transfer_ctrl_t * const p_api_ctrl, transfer_info_t * p_info) /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Initialize error code */
    fsp_err_t err = FSP_SUCCESS;

    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_info);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(DTSC_TRANSFER_MAX_LENGTH >= p_info->number_transfer, FSP_ERR_INVALID_SIZE); /* PRQA S 1842 # DP_MSR_R-10.4_UC1 */
#endif

    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;            /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    uint8_t  channel            = p_extend->channel;
    uint8_t  set_channel_enable = DTSC_PRV_CHANNEL_DISABLE;
    uint32_t active_target      = R_DTS0_DTSSTS_DTSACT_Msk |
                                  ((uint32_t) channel << R_DTS0_DTSSTS_DTSACH_Pos);

    /* Reject reconfiguration if the target channel is active in an ongoing DTSC transfer cycle */
    FSP_ERROR_RETURN((p_ctrl->p_reg->DTSSTS & DTSC_PRV_DTSSTS_ACTIVE_CH_MASK) != active_target, /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
                     FSP_ERR_INVALID_HW_CONDITION);

    /* Check whether the channel DTSC has disabled */
    if (DTSC_PRV_DISABLE_BIT != p_ctrl->p_reg->CH[channel].DTFSL_b.REQEN)                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
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
fsp_err_t R_DTSC_Reset (transfer_ctrl_t * const p_api_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                        void const * volatile   p_src,
                        void * volatile         p_dest,     /* PRQA S 3673 # Advisory R-8.13 */
                        uint16_t const          num_transfers)
{
    /* Initialize error code */
    fsp_err_t err = FSP_SUCCESS;

    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    uint8_t channel = p_extend->channel;

    /* Disable transfers if they are currently enabled. */
    r_dtsc_prv_disable(p_ctrl);

    if (NULL != p_src)
    {
        /* Configure the DTSC source pointer if it is provided. */
        p_ctrl->p_reg->CH[channel].DTSA = (uint32_t) p_src; /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */
    }

    if (NULL != p_dest)
    {
        /* Configure the DTSC destination pointer if it is provided. */
        p_ctrl->p_reg->CH[channel].DTDA = (uint32_t) p_dest;                                          /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */
    }

    if ((TRANSFER_DTSC_MODE_SINGLE != (transfer_dtsc_mode_t) p_ctrl->p_reg->CH[channel].DTTCT_b.TRM)) /* PRQA S 0662, 4342 # DP_MSR_D-1.1_UC1, Advisory R-10.5 */
    {
        /* Reset address reload count and transfer count. */
        p_ctrl->p_reg->CH[channel].DTTC_b.ARC = p_ctrl->p_cfg->p_info->addr_reload_count;             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
    }

    /* Reset the transfer count if it is single transfer mode. */
    p_ctrl->p_reg->CH[channel].DTTC_b.TRC = num_transfers;                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

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
fsp_err_t R_DTSC_InfoGet (transfer_ctrl_t * const p_api_ctrl, transfer_properties_t * const p_info) /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl;                            /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_info);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    uint8_t channel = p_extend->channel;

    p_info->transfer_length_max       = DTSC_TRANSFER_MAX_LENGTH;
    p_info->transfer_length_remaining = p_ctrl->p_reg->CH[channel].DTTC_b.TRC; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
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
fsp_err_t R_DTSC_Reload (transfer_ctrl_t * const p_api_ctrl, /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
                         void const            * p_src,
                         void                  * p_dest,     /* PRQA S 3673 # Advisory R-8.13 */
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
fsp_err_t R_DTSC_SoftwareStart (transfer_ctrl_t * const p_api_ctrl, transfer_start_mode_t mode) /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl;                        /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
    FSP_PARAMETER_NOT_USED(mode);
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    /* Return error if the activation source is not software source */
    FSP_ERROR_RETURN(DTSC_TRIGGER_EVENT_SOFTWARE == p_extend->activation_source, FSP_ERR_UNSUPPORTED);
    uint8_t channel = p_extend->channel;

    /* Generate a transfer request. */
    p_ctrl->p_reg->CH[channel].DTFSS = (uint32_t) DTSC_PRV_SOFTWARE_REQUEST_GENERATE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

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
fsp_err_t R_DTSC_SoftwareStop (transfer_ctrl_t * const p_api_ctrl)       /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;     /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               channel  = p_extend->channel;

    /* Clear transfer request pending bit. */
    p_ctrl->p_reg->CH[channel].DTFSC_b.DRQC = (uint32_t) DTSC_PRV_SOFTWARE_REQUEST_CLEAR; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

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
fsp_err_t R_DTSC_Enable (transfer_ctrl_t * const p_api_ctrl) /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Initialize error code */
    fsp_err_t err = FSP_SUCCESS;

    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

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
fsp_err_t R_DTSC_Disable (transfer_ctrl_t * const p_api_ctrl)            /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

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
fsp_err_t R_DTSC_Close (transfer_ctrl_t * const p_api_ctrl)              /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if DTSC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DTSC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    bool isr_flag_enable = false;

    uint8_t channel = p_extend->channel;

    r_dtsc_prv_disable(p_ctrl);

    /* Clear all DTSC transfer status */
    r_dtsc_clear_all_transfer_status(p_ctrl);

    /* Clear source address */
    p_ctrl->p_reg->CH[channel].DTSA = (uint32_t) NULL;                                     /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* Clear destination address */
    p_ctrl->p_reg->CH[channel].DTDA = (uint32_t) NULL;                                     /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* Clear DTSC number transfer and address reload count */
    p_ctrl->p_reg->CH[channel].DTTC = DTSC_PRV_DISABLE_BIT;                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Clear DTSC number reload transfer and reload address reload count */
    p_ctrl->p_reg->CH[channel].DTRTC = DTSC_PRV_DISABLE_BIT;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Clear DTSC source reload address */
    p_ctrl->p_reg->CH[channel].DTRSA = (uint32_t) NULL;                                    /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* Clear DTSC destination reload address */
    p_ctrl->p_reg->CH[channel].DTRDA = (uint32_t) NULL;                                    /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* Clear old priority bits for this channel */
    p_ctrl->p_reg->DTSPR0 &= ~((uint32_t) (0x3U) << (channel * DTSC_PRV_DTTCT_DS_OFFSET)); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Clear DTSC channel master setting register */
    p_ctrl->p_reg->DTSCM_CH[channel] = DTSC_PRV_DISABLE_BIT;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Clear DTSC transfer count compare register */
    p_ctrl->p_reg->CH[channel].DTTCC = DTSC_PRV_DISABLE_BIT;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    if (FSP_INVALID_VECTOR != p_extend->error_irq)
    {
        /* Disable DTSC error in transfer interrupt */
        R_BSP_IrqDisable(p_extend->error_irq);
        R_FSP_IsrContextSet(p_extend->error_irq, NULL);
        isr_flag_enable = true;
    }

    if (FSP_INVALID_VECTOR != p_extend->complete_irq)
    {
        /* Disable DTSC transfer complete channel interrupt */
        R_BSP_IrqDisable(p_extend->complete_irq);
        R_FSP_IsrContextSet(p_extend->complete_irq, NULL);
        isr_flag_enable = true;
    }

    if (FSP_INVALID_VECTOR != p_extend->count_match_irq)
    {
        /* Disable DTSC transfer count match interrupt */
        R_BSP_IrqDisable(p_extend->count_match_irq);
        R_FSP_IsrContextSet(p_extend->count_match_irq, NULL);
        isr_flag_enable = true;
    }

    /* Restore the p_ctrl of channel to context */
    gp_dtsc_isr_context[channel] = NULL;

    /* Check if the channel has an ISR if still open. */
    if (true == isr_flag_enable)
    {
        r_dtsc_enable_isr_still_open(p_ctrl);
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
fsp_err_t R_DTSC_CallbackSet (transfer_ctrl_t * const          p_api_ctrl,                              /* PRQA S 1505 # Advisory R-8.7 */
                              void (                         * p_callback)(transfer_callback_args_t *), /* PRQA S 1336 # DP_MSR_R-8.2_UC1 */
                              void * const                     p_context,                               /* PRQA S 3673 # Advisory R-8.13 */
                              transfer_callback_args_t * const p_callback_memory)
{
    /* Load module control from application */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) p_api_ctrl;                                /* PRQA S 0316 # Advisory R-11.5 */
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
static void r_dtsc_prv_enable (dtsc_instance_ctrl_t * p_ctrl)                                     /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Initialization */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;             /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               channel  = p_extend->channel;

    /* Enable DTSC channel operation */
    p_ctrl->p_reg->CH[channel].DTFSL_b.REQEN = DTSC_PRV_ENABLE_BIT;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
}

/*******************************************************************************************************************//**
 * Disable DTSC channel.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 **********************************************************************************************************************/
static void r_dtsc_prv_disable (dtsc_instance_ctrl_t * p_ctrl)                                    /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Initialization */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;             /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               channel  = p_extend->channel;

    /* Disable DTSC channel operation */
    p_ctrl->p_reg->CH[channel].DTFSL_b.REQEN = DTSC_PRV_DISABLE_BIT;                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
}

/*******************************************************************************************************************//**
 * Write the transfer info to the hardware registers.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 * @param[in]   p_info         Pointer to properties of transfer .
 *
 **********************************************************************************************************************/
static void r_dtsc_config_transfer_info (dtsc_instance_ctrl_t * p_ctrl, transfer_info_t * p_info) /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Get config from control block */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;             /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    /* Get config channel */
    uint8_t  channel          = p_extend->channel;
    uint32_t transfer_control =
        p_info->transfer_control_cfg.transfer_control_b.transfer_mode |                                           /* PRQA S 4532 # DP_MSR_R-10.1_UC1 */
        (p_info->transfer_control_cfg.transfer_control_b.transfer_data_size << DTSC_PRV_DTTCT_DS_OFFSET) |        /* PRQA S 4532, 4533 # DP_MSR_R-10.1_UC1, Rule-10.1 */
        (p_info->transfer_control_cfg.transfer_control_b.src_addr_direction << DTSC_PRV_DTTCT_SACM_OFFSET) |      /* PRQA S 4532, 4533 # DP_MSR_R-10.1_UC1, Rule-10.1 */
        (p_info->transfer_control_cfg.transfer_control_b.des_addr_direction << DTSC_PRV_DTTCT_DACM_OFFSET) |      /* PRQA S 4532, 4533 # DP_MSR_R-10.1_UC1, Rule-10.1 */
        (p_info->transfer_control_cfg.transfer_control_b.reload_func_1 << DTSC_PRV_DTTCT_RLD1M_OFFSET) |          /* PRQA S 0499, 4532, 4533 # DP_MSR_R-12.2_UC1, DP_MSR_R-10.1_UC1, Rule-10.1 */
        (p_info->transfer_control_cfg.transfer_control_b.reload_func_2 << DTSC_PRV_DTTCT_RLD2M_OFFSET) |          /* PRQA S 0499, 1861, 4532, 4533 # DP_MSR_R-12.2_UC1, Rule-10.4, DP_MSR_R-10.1_UC1, Rule-10.1 */
        (p_info->transfer_control_cfg.transfer_control_b.transfer_completion_irq << DTSC_PRV_DTTCT_TCE_OFFSET) |  /* PRQA S 0499, 1851, 4532 # DP_MSR_R-12.2_UC1, DP_MSR_R-10.4_UC1, DP_MSR_R-10.1_UC1 */
        (p_info->transfer_control_cfg.transfer_control_b.transfer_count_match_irq << DTSC_PRV_DTTCT_CCE_OFFSET) | /* PRQA S 0499, 1890, 4532 # DP_MSR_R-12.2_UC1, DP_MSR_R-10.7_UC1, DP_MSR_R-10.1_UC1 */
        (p_info->transfer_control_cfg.transfer_control_b.chain_function << DTSC_PRV_DTTCT_CHNE_OFFSET) |          /* PRQA S 0499, 1851, 4532, 4533 # DP_MSR_R-12.2_UC1, DP_MSR_R-10.4_UC1, DP_MSR_R-10.1_UC1, Rule-10.1 */
        (p_info->transfer_control_cfg.transfer_control_b.next_channel_to_chain << DTSC_PRV_DTTCT_CHNSEL_OFFSET) | /* PRQA S 0499, 1851, 4434, 4532 # DP_MSR_R-12.2_UC1, DP_MSR_R-10.4_UC1, DP_MSR_R-10.3_UC1, DP_MSR_R-10.1_UC1 */
        (p_info->transfer_control_cfg.transfer_control_b.transfer_disable_in_error << DTSC_PRV_DTTCT_ESE_OFFSET); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */

    /* Configure source address */
    p_ctrl->p_reg->CH[channel].DTSA = (uint32_t) p_info->p_src;                                                   /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* Configure destination address */
    p_ctrl->p_reg->CH[channel].DTDA = (uint32_t) p_info->p_dest;                                                  /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* Configure DTSC number transfer and address reload count */
    p_ctrl->p_reg->CH[channel].DTTC = (uint32_t) p_info->number_transfer |                                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
                                      ((uint32_t) p_info->addr_reload_count << DTSC_PRV_DTTC_ARC_OFFSET);

    /* Configure DTSC number reload transfer and reload address reload count */
    p_ctrl->p_reg->CH[channel].DTRTC = (uint32_t) p_info->reload_number_transfer |                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
                                       ((uint32_t) p_info->reload_addr_reload_count << DTSC_PRV_DTRTC_RARC_OFFSET);

    /* Configure DTSC source reload address */
    p_ctrl->p_reg->CH[channel].DTRSA = (uint32_t) p_info->p_reload_src_addr;                                      /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* Configure DTSC destination reload address */
    p_ctrl->p_reg->CH[channel].DTRDA = (uint32_t) p_info->p_reload_des_addr;                                      /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* Configure DTSC transfer control register */
    p_ctrl->p_reg->CH[channel].DTTCT = (uint32_t) transfer_control;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Configure DTSC channel master setting register */
    p_ctrl->p_reg->DTSCM_CH[channel] = (uint16_t) p_info->number_transfer_count_match;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Configure DTSC transfer count compare register */
    p_ctrl->p_reg->CH[channel].DTTCC = (uint16_t) p_info->number_transfer_count_match;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
}

/*******************************************************************************************************************//**
 * Clear all transfer status.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dtsc_clear_all_transfer_status (dtsc_instance_ctrl_t * p_ctrl)                                      /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Get config from control block */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;                             /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    /* Get config channel */
    uint8_t channel = p_extend->channel;

    /* Clear transfer error */
    p_ctrl->p_reg->CH[channel].DTFSC_b.ERC = DTSC_PRV_CLEAR_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Clear transfer count match flag */
    p_ctrl->p_reg->CH[channel].DTFSC_b.CCC = DTSC_PRV_CLEAR_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Clear transfer completion flag */
    p_ctrl->p_reg->CH[channel].DTFSC_b.TCC = DTSC_PRV_CLEAR_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

#if (1U == BSP_FEATURE_DTSC_TRANSFER_RQ_OVERFLOW_FLAG)

    /* Clear transfer request overflow flag */
    p_ctrl->p_reg->CH[channel].DTFSC_b.OVFC = DTSC_PRV_CLEAR_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
#endif

    /* Clear transfer request */
    p_ctrl->p_reg->CH[channel].DTFSC_b.DRQC = DTSC_PRV_CLEAR_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
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
static void r_dtsc_callback (dtsc_instance_ctrl_t * p_ctrl, transfer_event_t event) /* PRQA S 3673 # Advisory R-8.13 */
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

    if (NULL != p_ctrl->p_callback)
    {
        p_ctrl->p_callback(p_args);
    }

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
static fsp_err_t r_dtsc_open_parameter_checking (dtsc_instance_ctrl_t * const p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                                 transfer_cfg_t const * const p_cfg)
{
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(p_ctrl->open != DTSC_OPEN, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    FSP_ERROR_RETURN(p_extend->channel < BSP_FEATURE_DTSC_MAX_CHANNEL, FSP_ERR_IP_CHANNEL_NOT_PRESENT);

    if ((NULL != p_extend->p_callback))
    {
        /* When a callback is provided, at least one interrupt source must be fully configured.
         * An interrupt source is considered valid when:
         * - the corresponding DTSC interrupt is enabled,
         * - the interrupt priority is not BSP_IRQ_DISABLED,
         * - the interrupt vector is valid.
         */
        FSP_ERROR_RETURN((((DTSC_PRV_DISABLE_BIT !=
                            p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_completion_irq) &&
                           (BSP_IRQ_DISABLED != p_extend->transfer_complete_ipl) &&
                           (FSP_INVALID_VECTOR != p_extend->complete_irq)) ||
                          ((DTSC_PRV_DISABLE_BIT !=
                            p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_disable_in_error) &&
                           (BSP_IRQ_DISABLED != p_extend->transfer_error_ipl) &&
                           (FSP_INVALID_VECTOR != p_extend->error_irq)) ||
                          ((DTSC_PRV_DISABLE_BIT !=
                            p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_count_match_irq) &&
                           (BSP_IRQ_DISABLED != p_extend->transfer_count_match_ipl) &&
                           (FSP_INVALID_VECTOR != p_extend->count_match_irq))),
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
static fsp_err_t r_dtsc_enable_parameter_checking (dtsc_instance_ctrl_t * const p_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend;   /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               channel  = p_extend->channel;
    void const          * p_src    = (void const *) p_ctrl->p_reg->CH[channel].DTSA;    /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */
    void const          * p_dest   = (void const *) p_ctrl->p_reg->CH[channel].DTDA;    /* PRQA S 0326, 0662 # DP_MSR_R-11.6_UC1, DP_MSR_D-1.1_UC1 */

    /* The source and destination pointers cannot be NULL. */
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);

    /* A transfer count of 0 is not supported for any transfer mode. */
    FSP_ASSERT(0 != p_ctrl->p_reg->CH[channel].DTTC_b.TRC); /* PRQA S 0662, 1863 # DP_MSR_D-1.1_UC1, DP_MSR_R-10.4_UC1 */

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * Callback when transfer count match or transfer complete
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void dtsc_transfer_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type              irq = R_FSP_CurrentIrqGet();
    dtsc_instance_ctrl_t * p_ctrl;
    transfer_event_t       event;
    R_INTIF0_Type        * p_dtsc_irq = (R_INTIF0_Type *) R_INTIF0;                          /* PRQA S 0306 # Advisory R-11.4 */

    /* Recover ISR context saved in open to get the Peripheral Interrupt Status Register. */
    dtsc_instance_ctrl_t * p_ctrl_isr = (dtsc_instance_ctrl_t *) R_FSP_IsrContextGet(irq);   /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
    dtsc_extended_cfg_t  * p_extend   = (dtsc_extended_cfg_t *) p_ctrl_isr->p_cfg->p_extend; /* PRQA S 0311, 0316, 0662, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, DP_MSR_D-1.1_UC1, Advisory R-8.13 */

    /* Variable to store detected channel number. */
    uint8_t channel = 0U;

    /* Calculate register group index (each PINT register group handles 32 channels). */
    uint8_t registers = p_extend->channel / 32U;

    /* Read interrupt status registers. */
    uint32_t transfer_complete_irq    = p_dtsc_irq->PINT[registers];         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
    uint32_t transfer_count_match_irq = p_dtsc_irq->PINT4[registers];        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    if (0U != transfer_complete_irq)
    {
        channel = ((SCH1R(transfer_complete_irq) - 1U) + (registers * 32U)); /* PRQA S 1820, 1891, 3469, 4461 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.7_UC1, Advisory D-4.9, DP_MSR_R-10.3_UC1 */

        /* Get p_ctrl for the detected channel. */
        p_ctrl = (dtsc_instance_ctrl_t *) gp_dtsc_isr_context[channel];      /* PRQA S 0316 # Advisory R-11.5 */
        event  = TRANSFER_EVENT_TRANS_COMPLETION;

        /* Call user callback */
        r_dtsc_callback(p_ctrl, event);

        /* Clear Peripheral Interrupt Status */
        p_dtsc_irq->PINTCLR[registers] = transfer_complete_irq;      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

        /* Dummy read PINT0 register */
        transfer_complete_irq = p_dtsc_irq->PINT[registers];         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

        /* Clear transfer completion flag. */
        p_ctrl->p_reg->CH[channel].DTFSC_b.TCC = DTSC_PRV_CLEAR_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
    }
    /* Check if transfer count mach interrupt present in this channel  */
    else if (0U != transfer_count_match_irq)
    {
        channel = ((SCH1R(transfer_count_match_irq) - 1U) + (registers * 32U)); /* PRQA S 1820, 1891, 3469, 4461 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.7_UC1, Advisory D-4.9, DP_MSR_R-10.3_UC1 */

        /* Get p_ctrl for the detected channel. */
        p_ctrl = (dtsc_instance_ctrl_t *) gp_dtsc_isr_context[channel];         /* PRQA S 0316 # Advisory R-11.5 */
        event  = TRANSFER_EVENT_TRANS_COUNT_MATCH;

        /* Call user callback */
        r_dtsc_callback(p_ctrl, event);

        /* Clear Peripheral Interrupt Status */
        p_dtsc_irq->PINTCLR4[registers] = transfer_count_match_irq;  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

        /* Dummy read PINT4 register */
        transfer_count_match_irq = p_dtsc_irq->PINT4[registers];     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

        /* Clear transfer count match flag */
        p_ctrl->p_reg->CH[channel].DTFSC_b.CCC = DTSC_PRV_CLEAR_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
    }
    else
    {
        /* Do Nothing */
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * DTSC Error ISR
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void dtsc_transfer_error_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Recover ISR context saved in open. */
    dtsc_instance_ctrl_t * p_ctrl = (dtsc_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Assign channel */
    uint8_t          channel = p_ctrl->p_reg->DTSER_b.DTSERCH;                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */
    transfer_event_t event   = TRANSFER_EVENT_ERR_TRANS_READ_CYCLE;

    /* Get p_ctrl for the detected channel. */
    p_ctrl = (dtsc_instance_ctrl_t *) gp_dtsc_isr_context[channel];                    /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t dtfst = p_ctrl->p_reg->CH[channel].DTFST;                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

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

    /* Clear transfer error flag */
    p_ctrl->p_reg->CH[channel].DTFSC_b.ERC = DTSC_PRV_CLEAR_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Write reliability function info to hardware register.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dtsc_config_reliability_func (dtsc_instance_ctrl_t * p_ctrl)            /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Initialization */
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 0662, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, DP_MSR_D-1.1_UC1, Advisory R-8.13 */
    uint8_t               channel  = p_extend->channel;

    /* Setting for reliability function */
    /* PRQA S 0662 ++ # DP_MSR_D-1.1_UC1 */
    p_ctrl->p_reg->DTSCM_CH_b[channel].CHAIN_RESTRICT = DTSC_PRV_ENABLE_BIT;
    p_ctrl->p_reg->DTSCM_CH_b[channel].SPID           = (uint8_t) (p_extend->p_chain_restrict_cfg->master_spid & \
                                                                   DTSC_SPID_MASK);
    p_ctrl->p_reg->DTSCM_CH_b[channel].CHAIN_SPID = (uint8_t) (p_extend->p_chain_restrict_cfg->chain_spid & \
                                                               DTSC_CHAIN_SPID_MASK);
    p_ctrl->p_reg->DTSCM_CH_b[channel].UM = (uint8_t) (p_extend->p_chain_restrict_cfg->master_um & \
                                                       DTSC_UM_MASK);
    p_ctrl->p_reg->DTSCM_CH_b[channel].CHAIN_UM = (uint8_t) (p_extend->p_chain_restrict_cfg->chain_um & \
                                                             DTSC_CHAIN_UM_MASK);

    /* PRQA S 0662 -- */
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
    dtsc_extended_cfg_t * p_extend = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               sel_register;
    uint8_t               sel_register_bit;
    uint16_t              source        = p_extend->activation_source;                /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    uint8_t               request_group = source / DTSC_MAX_ACTIVATION_SOURCE;        /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
    volatile uint32_t   * sel_registers;

    p_ctrl->p_hwsel = DTSC_PRV_REG_DEFSEL;                                            /* PRQA S 0306, 4461 # Advisory R-11.4, DP_MSR_R-10.3_UC1 */

    /* Select correct DTSSEL0 register array based on unit0 */
    sel_registers = (volatile uint32_t *) (&p_ctrl->p_hwsel->DTSSEL0);                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 */

    /* Calculate register index and bit offset */
    /* Select DTS transfer request group 0 */
    if (DTSC_TRANSFER_REQUEST_GROUP0 == request_group)
    {
        sel_register     = source / 8U;                    /* PRQA S 4461, 4521 # DP_MSR_R-10.3_UC1, DP_MSR_R-10.1_UC1 */
        sel_register_bit = (uint8_t) ((source % 8U) * 4U); /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */

        /* Clear the corresponding bit */
        sel_registers[sel_register] |= ((uint32_t) request_group << sel_register_bit);
    }
    /* Select DTS transfer request other group */
    else
    {
        source           = p_extend->activation_source - (request_group * DTSC_MAX_ACTIVATION_SOURCE); /* PRQA S 4461, 4521 # DP_MSR_R-10.3_UC1, DP_MSR_R-10.1_UC1 */
        sel_register     = source / 8U;                                                                /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
        sel_register_bit = (uint8_t) ((source % 8U) * 4U);

        /* Clear the corresponding bit */
        sel_registers[sel_register] |= ((uint32_t) request_group << sel_register_bit);
    }
}

/*******************************************************************************************************************//**
 * Check if the channel has an ISR still open.
 *
 **********************************************************************************************************************/
static void r_dtsc_enable_isr_still_open (dtsc_instance_ctrl_t * p_ctrl)                   /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Initialization */
    dtsc_extended_cfg_t * p_extend      = (dtsc_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               group_channel = (uint8_t) (p_extend->channel / 32U);
    uint8_t               channel_start = 0U;
    uint8_t               channel_stop  = 0U;

    channel_start = (uint8_t) (32U * group_channel);
    channel_stop  = (uint8_t) ((32U * group_channel) + 32U);

    for ( ; channel_start < channel_stop; channel_start++)
    {
        bool isr_enable = false;
        if (gp_dtsc_isr_context[channel_start] != NULL)
        {
            dtsc_instance_ctrl_t * p_ctrl_restore = (dtsc_instance_ctrl_t *) gp_dtsc_isr_context[channel_start]; /* PRQA S 0316 # Advisory R-11.5 */

            dtsc_extended_cfg_t * p_extend_restore = (dtsc_extended_cfg_t *) p_ctrl_restore->p_cfg->p_extend;    /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

            if ((FSP_INVALID_VECTOR != p_extend_restore->error_irq) &&
                (BSP_IRQ_DISABLED != p_extend_restore->transfer_error_ipl))
            {
                /* Enable DTSC error in transfer interrupt */
                R_BSP_IrqCfgEnable(p_extend_restore->error_irq, p_extend_restore->transfer_error_ipl, p_ctrl_restore);
                isr_enable = true;
            }

            if ((FSP_INVALID_VECTOR != p_extend_restore->complete_irq) &&
                (BSP_IRQ_DISABLED != p_extend_restore->transfer_complete_ipl))
            {
                /* Enable DTSC transfer complete channel interrupt */
                R_BSP_IrqCfgEnable(p_extend_restore->complete_irq,
                                   p_extend_restore->transfer_complete_ipl,
                                   p_ctrl_restore);
                isr_enable = true;
            }

            if ((FSP_INVALID_VECTOR != p_extend_restore->count_match_irq) &&
                (BSP_IRQ_DISABLED != p_extend_restore->transfer_count_match_ipl))
            {
                /* Enable DTSC transfer count match interrupt */
                R_BSP_IrqCfgEnable(p_extend_restore->count_match_irq,
                                   p_extend_restore->transfer_count_match_ipl,
                                   p_ctrl_restore);
                isr_enable = true;
            }
        }

        if (true == isr_enable)
        {
            break;
        }
    }
}
