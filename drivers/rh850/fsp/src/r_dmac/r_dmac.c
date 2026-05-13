/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_dmac.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**  Number of DMAC channel on this MCU */
#define DMAC_MAX_CHANNEL                      (16U)

/** "DMAC" in ASCII, used to determine if channel is open */
#define DMAC_OPEN                             (0x444d4143)

/** Length limited to 2*2^16 transfers */
#define DMAC_TRANSFER_MAX_LENGTH              (0xFFFFFFFF)

/** Transfer units */
#define DMAC_PRV_UNIT0_SELECTION              (0U)
#define DMAC_PRV_UNIT1_SELECTION              (1U)

/** Transfer DMA Timeout */

/* Define the timeout for waiting on the DMAjCHSTA register status as 20 us,
 * which is half of the maximum ISR duration (40 us) */
#define DMAC_TIMEOUT                          (0x46)

/** Select DMA transfer request groups 0, 1, 2 and 3. */
#define DMAC_GROUP0_SELECTION                 (0x3U)
#define DMAC_GROUP1_SELECTION                 (1U)
#define DMAC_GROUP2_SELECTION                 (2U)
#define DMAC_GROUP3_SELECTION                 (3U)

/** Set the updating the SAR, DAR, TSR register */
#define DMAC_DPCR_SAR_DAR_TSR                 (0x00000007UL)

/* Bit shifting */
#define DMAC_BIT_SHIFT_0                      (0U)
#define DMAC_BIT_SHIFT_3                      (3U)

/* Register shifting */
#define DMAC_REGISTER_SHIFT_1                 (1U)
#define DMAC_REGISTER_SHIFT_2                 (2U)
#define DMAC_REGISTER_SHIFT_3                 (3U)

/* Register length */
#define DMAC_REGISTER_LENGTH_32BIT            (4U)

/* The last descriptor number of Double Buffer Operation */
#define DMAC_LAST_DESCRIPTOR_NUMBER           (3U)

/** Set the clear flags for clearing DMA transfer status. */
#define DMAC_CHFCR_OVFC_DRQC_DPEC_CAEC_TEC    ((uint32_t) (1U << R_DMAC0_sDMAC_CH_CHFCR_OVFC_Pos | \
                                                           1U << R_DMAC0_sDMAC_CH_CHFCR_DRQC_Pos | \
                                                           1U << R_DMAC0_sDMAC_CH_CHFCR_CAEC_Pos | \
                                                           1U << R_DMAC0_sDMAC_CH_CHFCR_TEC_Pos))
#define DMAC_CHFCR_OVFC_DRQC_BIT              ((uint32_t) (1U << R_DMAC0_sDMAC_CH_CHFCR_OVFC_Pos | \
                                                           1U << R_DMAC0_sDMAC_CH_CHFCR_DRQC_Pos))

/** Configure for Scatter/Gather Transfer */
#define DMAC_SGCR_SEN_BIT                     (uint32_t) (1U << R_DMAC0_sDMAC_CH_SGCR_SEN_Pos | \
                                                          1U << R_DMAC0_sDMAC_CH_SGCR_GEN_Pos)

/* Common macro to disable bit */
#define DMAC_PRV_DISABLE_BIT                  (0U)

/* Common macro to enable bit */
#define DMAC_PRV_ENABLE_BIT                   (1U)

/* Common macro to disable channel */
#define DMAC_PRV_CHANNEL_DISABLE              (0U)

/* Common macro to enable channel */
#define DMAC_PRV_CHANNEL_ENABLE               (1U)

/* Master channel number */
#define DMAC_PRV_MASTER_CHANNEL_NUMBER        (7U)

/** Set up the configuration to initialize all DMA channels */
#define DMAC_CHRST_ALL_CHANNELS               (0xFFU)

/** The maximum interrupt trigger source for each sDMAC transfer group.*/
#define DMAC_PRV_MAX_RQ_SRC_PER_GROUP         (256U)

#define DMAC_PRV_CLEAR_ALL_TRANSFER_STATUS    (0x320FU)

#define DMAC_DEFSEL                           (0xFF090400UL)

#define DMAC_SPID_MASK                        (0x1FU)

/** Calculate the mask bits for byte alignment from the transfer_size_t. */
#define DMAC_PRV_MASK_ALIGN_N_BYTES(x)    ((1U << (x)) - 1U)

#define DMAC_PRV_REG(unit)                ((R_DMAC0_Type *) (((uint32_t) R_DMAC1 - (uint32_t) R_DMAC0) * \
                                                             (uint32_t) unit + (uint32_t) R_DMAC0))

#define DMAC_PRV_REG_DEFSEL    ((R_DMATRGSEL_Type *) (DMAC_DEFSEL))

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static fsp_err_t r_dmac_prv_enable(dmac_instance_ctrl_t * p_ctrl);
static void      r_dmac_prv_disable(dmac_instance_ctrl_t * p_ctrl);
static void      r_dmac_config_transfer_info(dmac_instance_ctrl_t * p_ctrl, transfer_info_t * p_info);
static void      r_dmac_call_callback(dmac_instance_ctrl_t * p_ctrl, transfer_event_t event);
static void      r_dmac_set_hardware_request(dmac_instance_ctrl_t * p_ctrl);
static void      r_dmac_config_descriptor_mem(dmac_instance_ctrl_t * p_ctrl);
static void      r_dmac_config_reliability_func(dmac_instance_ctrl_t * p_ctrl);
static void      r_dmac_gather_set(dmac_instance_ctrl_t * p_ctrl);
static void      r_dmac_scatter_set(dmac_instance_ctrl_t * p_ctrl);
static void      r_dmac_descriptor_set(dmac_instance_ctrl_t * p_ctrl);
static void      r_dmac_check_double_buffer_op(dmac_instance_ctrl_t * p_ctrl);
static uint32_t  r_dmac_number_transfer(transfer_info_t * p_info);

#if DMAC_CFG_PARAM_CHECKING_ENABLE
static fsp_err_t r_dmac_open_parameter_checking(dmac_instance_ctrl_t * const p_ctrl,
                                                transfer_cfg_t const * const p_cfg);
static fsp_err_t r_dmac_enable_parameter_checking(dmac_instance_ctrl_t * const p_ctrl);

#endif

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** DMAC implementation of transfer API. */
const transfer_api_t g_transfer_on_dmac =
{
    .open          = R_DMAC_Open,
    .reconfigure   = R_DMAC_Reconfigure,
    .reset         = R_DMAC_Reset,
    .infoGet       = R_DMAC_InfoGet,
    .softwareStart = R_DMAC_SoftwareStart,
    .softwareStop  = R_DMAC_SoftwareStop,
    .enable        = R_DMAC_Enable,
    .disable       = R_DMAC_Disable,
    .close         = R_DMAC_Close,
    .callbackSet   = R_DMAC_CallbackSet,
};

BSP_INTERRUPT_ATTRIBUTE void dmac_int_isr(void);
BSP_INTERRUPT_ATTRIBUTE void dmac_error_isr(void);

/*******************************************************************************************************************//**
 * @addtogroup DMAC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configure a DMAC channel.
 *
 * @retval FSP_SUCCESS                    Successful open.
 * @retval FSP_ERR_ASSERTION              An input parameter is invalid.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT The configured channel is invalid.
 * @retval FSP_ERR_IRQ_BSP_DISABLED       The IRQ associated with the activation source is not enabled in the BSP.
 * @retval FSP_ERR_ALREADY_OPEN           The control structure is already opened.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Open (transfer_ctrl_t * const p_api_ctrl, transfer_cfg_t const * const p_cfg)
{
#if DMAC_CFG_PARAM_CHECKING_ENABLE
    fsp_err_t err = FSP_SUCCESS;
    err = r_dmac_open_parameter_checking(p_api_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    /* Load module control from application */
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) p_api_ctrl;

    /* Load bus configuration from application*/
    p_ctrl->p_cfg = p_cfg;

    /* Set register call to correct DMAC module unit  */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               unit     = p_extend->unit;
    uint8_t               channel  = p_extend->channel;
    p_ctrl->p_reg = DMAC_PRV_REG(unit);

    /* Set callback and context pointers, if configured */
    p_ctrl->p_callback            = p_extend->p_callback;
    p_ctrl->p_context             = p_extend->p_context;
    p_ctrl->p_callback_memory     = NULL;
    p_ctrl->count_last_descriptor = 0U;
    p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.transfer_request_type = TRANSFER_REQUEST_AUTO;

    /* Check if hardware transfer request. */
    if (DMAC_TRIGGER_EVENT_SOFTWARE != p_extend->activation_source)
    {
        p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.transfer_request_type = TRANSFER_REQUEST_HARDWARE;
        r_dmac_set_hardware_request(p_ctrl);
    }

    if ((DMAC_PRV_DISABLE_BIT != p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.transfer_completion_irq) &&
        (BSP_IRQ_DISABLED != p_extend->irq_priority_level))
    {
        /* Transfer end interrupt enable */
        p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.IE = DMAC_PRV_ENABLE_BIT;

        /* Enable Transfer end interrupt */
        R_BSP_IrqCfgEnable(p_extend->irq, p_extend->irq_priority_level, p_ctrl);
    }

    if ((DMAC_PRV_DISABLE_BIT != p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.channel_address_error_irq) &&
        (BSP_IRQ_DISABLED != p_extend->irq_error_priority_level))
    {
        /* Enable Channel address error interrupt */
        R_BSP_IrqCfgEnable(p_extend->irq_error, p_extend->irq_error_priority_level, p_ctrl);

        /* Channel address error interrupt INTSDMACERR is enable */
        p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.CAEE = DMAC_PRV_ENABLE_BIT;

        /* ISR occurs: channel address error interrupt INTSDMACjCHn is enable */
        p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.CAIE = DMAC_PRV_ENABLE_BIT;
    }

    if ((DMAC_PRV_DISABLE_BIT != p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.transfer_descriptor_irq) &&
        (BSP_IRQ_DISABLED != p_extend->irq_descriptor_pri_level))
    {
        /* Transfer end interrupt enable */
        p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.IE = DMAC_PRV_ENABLE_BIT;

        /* Transfer descriptor step end interrupt enable */
        p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.DSIE = DMAC_PRV_ENABLE_BIT;

        /* Enable Channel descriptor step end interrupt */
        R_BSP_IrqCfgEnable(p_extend->irq_descriptor, p_extend->irq_descriptor_pri_level, p_ctrl);
    }

    /* Configure reliability function */
    if (TRANSFER_SWITCHABLE_REDUNDANT_OPERATION_ENABLE == p_extend->p_switchable_redundant_cfg->redundant_operation)
    {
        r_dmac_config_reliability_func(p_ctrl);
    }

    /* Setting up the transfer information. */
    r_dmac_config_transfer_info(p_ctrl, p_cfg->p_info);

    /* Setting the Gather transfer */
    if (DMAC_GATHER_MODE_ENABLE == p_extend->gather_setting.gather_mode)
    {
        r_dmac_gather_set(p_ctrl);
    }

    /* Setting the Scatter transfer */
    if (DMAC_SCATTER_MODE_ENABLE == p_extend->scatter_setting.scatter_mode)
    {
        r_dmac_scatter_set(p_ctrl);
    }

    /* Setting the Descriptor Operation transfer */
    if (true == p_extend->descriptor_enable)
    {
        r_dmac_descriptor_set(p_ctrl);
    }

    p_ctrl->open = DMAC_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Reconfigure the transfer with new transfer info.
 *
 * @retval FSP_SUCCESS              Transfer is configured and will start when trigger occurs.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Reconfigure (transfer_ctrl_t * const p_api_ctrl, transfer_info_t * p_info)
{
    /* Initialization */
    fsp_err_t err = FSP_SUCCESS;

    /* Load module control from application */
    dmac_instance_ctrl_t * p_ctrl             = (dmac_instance_ctrl_t *) p_api_ctrl;
    uint8_t                set_channel_enable = DMAC_PRV_CHANNEL_DISABLE;
    dmac_extended_cfg_t  * p_extend           = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t                channel            = p_extend->channel;
#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ASSERT(p_info != NULL);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check if DMA channel operation */
    if (DMAC_PRV_DISABLE_BIT != p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.DE)
    {
        /* Disable DMA Channel */
        r_dmac_prv_disable(p_ctrl);
        set_channel_enable = DMAC_PRV_CHANNEL_ENABLE;
    }

    /* Reconfigure the transfer settings. */
    r_dmac_config_transfer_info(p_ctrl, p_info);

    /* Reconfigure the Descriptor transfer setting */
    if (true == p_extend->descriptor_enable)
    {
        r_dmac_descriptor_set(p_ctrl);
    }

    if (DMAC_PRV_CHANNEL_ENABLE == set_channel_enable)
    {
        /* Enable the transfer configuration. */
        err = r_dmac_prv_enable(p_ctrl);
    }

    return err;
}

/*******************************************************************************************************************//**
 * Reset transfer source, destination, transaction size and number of transfers.
 *
 * @retval FSP_SUCCESS              Transfer reset successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Reset (transfer_ctrl_t * const p_api_ctrl,
                        void const * volatile   p_src,
                        void * volatile         p_dest,
                        uint16_t const          num_transfers)
{
    /* Initialization */
    fsp_err_t err;

    /* Load module control from application */
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) p_api_ctrl;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Set register call to correct DMAC module unit */
    dmac_extended_cfg_t * p_extend             = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               unit                 = p_extend->unit;
    uint8_t               channel              = p_extend->channel;
    uint32_t              number_transfer_byte = num_transfers;
    p_ctrl->p_cfg->p_info->number_transfer = num_transfers;
    p_ctrl->p_reg = DMAC_PRV_REG(unit);

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Disable transfers if they are currently enabled. */
    r_dmac_prv_disable(p_ctrl);

    if (NULL != p_src)
    {
        /* Configure the DMAC source pointer if it is provided. */
        p_ctrl->p_reg->sDMAC_CH[channel].SAR = (uint32_t) p_src;
    }

    if (NULL != p_dest)
    {
        /* Configure the DMAC destination pointer if it is provided. */
        p_ctrl->p_reg->sDMAC_CH[channel].DAR = (uint32_t) p_dest;
    }

    if (DMAC_TRIGGER_EVENT_SOFTWARE != p_extend->activation_source)
    {
        number_transfer_byte = r_dmac_number_transfer(p_ctrl->p_cfg->p_info);
    }

    /* Reset the transfer count if it is normal transfer mode. */
    p_ctrl->p_reg->sDMAC_CH[channel].TSR = (uint32_t) number_transfer_byte;

    /* Clear transfer end flag */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR = DMAC_PRV_CLEAR_ALL_TRANSFER_STATUS;

    /* Enable the transfer configuration. */
    err = r_dmac_prv_enable(p_ctrl);

    return err;
}

/*******************************************************************************************************************//**
 * Set a transfer request by Auto request (software request).
 *
 * @retval FSP_SUCCESS              Transfer started written successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 * @retval FSP_ERR_UNSUPPORTED      Handle was not configured for software activation.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_SoftwareStart (transfer_ctrl_t * const p_api_ctrl, transfer_start_mode_t mode)
{
    /* Initialization */
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) p_api_ctrl;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    FSP_ERROR_RETURN(DMAC_TRIGGER_EVENT_SOFTWARE == p_extend->activation_source, FSP_ERR_UNSUPPORTED);
#else
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
#endif
    FSP_PARAMETER_NOT_USED(mode);
    uint8_t channel = p_extend->channel;

    /* Disable Hardware request. */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR = DMAC_CHFCR_OVFC_DRQC_BIT;

    /* Enable the transfer configuration. */
    r_dmac_prv_enable(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * The transfer is stopped immediately, regardless of the request type (software or hardware).
 *
 * @retval FSP_SUCCESS              Transfer stopped written successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_SoftwareStop (transfer_ctrl_t * const p_api_ctrl)
{
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) p_api_ctrl;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    /* Clear transfer status */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR = DMAC_PRV_CLEAR_ALL_TRANSFER_STATUS;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enable transfers for the configured activation source.
 *
 * @retval FSP_SUCCESS              Counter value written successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Enable (transfer_ctrl_t * const p_api_ctrl)
{
    /* Initialization */
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) p_api_ctrl;

    /* Set register call to correct DMAC module unit */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Set the clear flags for clearing DMA transfer status. */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR = DMAC_CHFCR_OVFC_DRQC_DPEC_CAEC_TEC;

    r_dmac_prv_enable(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Disable transfers so that they are no longer triggered by the activation source.
 *
 * @retval FSP_SUCCESS              Counter value written successfully.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Disable (transfer_ctrl_t * const p_api_ctrl)
{
    /* Initialization */
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) p_api_ctrl;

    /* Set register call to correct DMAC module unit */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    r_dmac_prv_disable(p_ctrl);

    /* Clear transfer end flag */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR_b.TEC = DMAC_PRV_ENABLE_BIT;

    /* Clear Descriptor step end flag */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR_b.DSEC = DMAC_PRV_ENABLE_BIT;

    /* Disable DMA control bits */
    p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.CAEE = DMAC_PRV_ENABLE_BIT;

    /* Clear transfer status */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR_b.DRQC = DMAC_PRV_ENABLE_BIT;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set driver specific information in provided pointer.
 *
 * @retval FSP_SUCCESS              Information has been written to p_info.
 * @retval FSP_ERR_NOT_OPEN         Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 * @retval FSP_ERR_ASSERTION        An input parameter is invalid.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_InfoGet (transfer_ctrl_t * const p_api_ctrl, transfer_properties_t * const p_info)
{
    /* Initialization */
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) p_api_ctrl;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_info);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    p_info->transfer_length_max       = DMAC_TRANSFER_MAX_LENGTH;
    p_info->transfer_length_remaining = p_ctrl->p_reg->sDMAC_CH[channel].TSR;
    p_info->block_count_remaining     = 0U;
    p_info->block_count_max           = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_CallbackSet (transfer_ctrl_t * const          p_api_ctrl,
                              void (                         * p_callback)(transfer_callback_args_t *),
                              void * const                     p_context,
                              transfer_callback_args_t * const p_callback_memory)
{
    FSP_PARAMETER_NOT_USED(p_callback_memory);

    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) p_api_ctrl;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_callback);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_ctrl->p_callback = p_callback;
    p_ctrl->p_context  = p_context;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Disable transfer and clean up internal data. Implements @ref transfer_api_t::close.
 *
 * @retval FSP_SUCCESS           Successful close.
 * @retval FSP_ERR_ASSERTION     An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN      Handle is not initialized.  Call R_DMAC_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Close (transfer_ctrl_t * const p_api_ctrl)
{
    /* Initialization */
    dmac_instance_ctrl_t * p_ctrl   = (dmac_instance_ctrl_t *) p_api_ctrl;
    dmac_extended_cfg_t  * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t                channel  = p_extend->channel;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(DMAC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Reset the initialization state of all registers in every channel. */
    p_ctrl->p_reg->DMAjCHRST |= (uint32_t) (DMAC_PRV_ENABLE_BIT << channel);

    /* Disable DMA transfers on all channels */
    p_ctrl->p_reg->DMAjOR_b.DME = DMAC_PRV_DISABLE_BIT;

    /* Clear Source Address register */
    p_ctrl->p_reg->sDMAC_CH[channel].SAR = (uint32_t) NULL;

    /* Clear Destination Address register */
    p_ctrl->p_reg->sDMAC_CH[channel].DAR = (uint32_t) NULL;

    /* Clear Transfer size in transfer size  */
    p_ctrl->p_reg->sDMAC_CH[channel].TSR = DMAC_PRV_DISABLE_BIT;

    /* Clear transfer status */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR = DMAC_PRV_CLEAR_ALL_TRANSFER_STATUS;

    /* Disable Transfer End isr */
    R_BSP_IrqDisable(p_extend->irq);
    R_FSP_IsrContextSet(p_extend->irq, NULL);

    /* Disable Descriptor Step End isr */
    R_BSP_IrqDisable(p_extend->irq_descriptor);
    R_FSP_IsrContextSet(p_extend->irq_descriptor, NULL);

    /* Disable Channel address error isr */
    R_BSP_IrqDisable(p_extend->irq_error);
    R_FSP_IsrContextSet(p_extend->irq_error, NULL);

    /* Clear ID so control block can be reused. */
    p_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup DMAC)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Enable DMAC channel operation.
 *
 * @param[in]   p_ctrl              Pointer to control structure.
 *
 * @retval      FSP_SUCCESS         Input Parameters are Valid.
 * @retval      FSP_ERR_ASSERTION   An input parameter is invalid.
 **********************************************************************************************************************/
static fsp_err_t r_dmac_prv_enable (dmac_instance_ctrl_t * p_ctrl)
{
    /* Initialization */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;
    fsp_err_t             err      = FSP_SUCCESS;

#if DMAC_CFG_PARAM_CHECKING_ENABLE
    err = r_dmac_enable_parameter_checking(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    /* Enable DMA channel operation */
    if (DMAC_PRV_DISABLE_BIT == (p_ctrl->p_reg->DMAjOR_b.DME & R_DMAC0_DMAjOR_DME_Msk))
    {
        /* Enable DMA transfers on all channels */
        p_ctrl->p_reg->DMAjOR_b.DME = DMAC_PRV_ENABLE_BIT;
    }

    /* Enable DMA channel operation */
    p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.DE = DMAC_PRV_ENABLE_BIT;

    return err;
}

/*******************************************************************************************************************//**
 * Disable DMAC channel operation.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dmac_prv_disable (dmac_instance_ctrl_t * p_ctrl)
{
    /* Initialization */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;
    uint8_t               timeout  = DMAC_TIMEOUT;

    /* Wait for the previous DMA transfer to finish or be aborted. */
    while ((timeout > 0UL) && (0UL != p_ctrl->p_reg->sDMAC_CH[channel].CHSTA_b.BUSY))
    {
        timeout--;
    }

    /* Disable DMA transfer */
    p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.DE = DMAC_PRV_DISABLE_BIT;
}

/*******************************************************************************************************************//**
 * Write the transfer info to the hardware registers.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 * @param[in]   p_info         Pointer to properties of transfer.
 *
 **********************************************************************************************************************/
static void r_dmac_config_transfer_info (dmac_instance_ctrl_t * p_ctrl, transfer_info_t * p_info)
{
    /* Initialization */
    dmac_extended_cfg_t * p_extend             = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel              = p_extend->channel;
    uint32_t              number_transfer_byte = p_info->number_transfer;

    /* Source Address register */
    p_ctrl->p_reg->sDMAC_CH[channel].SAR = (uint32_t) p_info->p_src;

    /* Source Destination register */
    p_ctrl->p_reg->sDMAC_CH[channel].DAR = (uint32_t) p_info->p_dest;

    if (DMAC_TRIGGER_EVENT_SOFTWARE != p_extend->activation_source)
    {
        number_transfer_byte = r_dmac_number_transfer(p_info);
    }

    /* Set Transfer size in transfer size register */
    p_ctrl->p_reg->sDMAC_CH[channel].TSR = (uint32_t) number_transfer_byte;

    /* Set Transfer mode */
    p_ctrl->p_reg->sDMAC_CH[channel].TMR = (uint32_t) (p_info->transfer_mode_cfg.transfer_mode);

    /* Priority control select */
    p_ctrl->p_reg->DMAjOR_b.PR = p_extend->channel_scheduling;

    /* Channel request priorities */
    if (DMAC_PRV_ENABLE_BIT == p_extend->ch_req_pri_en)
    {
        p_ctrl->p_reg->sDMAC_CH[channel].TMR_b.PRI = (p_extend->ch_req_pri_en << DMAC_BIT_SHIFT_3) |
                                                     p_extend->ch_req_pri_num;
        p_ctrl->p_reg->DMAjCHPRI_b.PRICNT = p_extend->ch_req_pri_cnt_val;
    }

    /* Setting transfer count per hardware request to TC bit. */
    p_ctrl->p_reg->sDMAC_CH[channel].RS_b.TC = (uint32_t) (p_info->transfer_count_per_hw_request);

    /* Clear transfer status */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR = DMAC_PRV_CLEAR_ALL_TRANSFER_STATUS;
}

/*******************************************************************************************************************//**
 * Caculate the number transfer by DMA hardware transfer
 *
 * @param[in]   p_info      Pointer to properties of transfer.
 *
 **********************************************************************************************************************/
static uint32_t r_dmac_number_transfer (transfer_info_t * p_info)
{
    uint32_t number_transfer_byte = 0U;
    if ((1U == p_info->transfer_pre_load) && (1U == p_info->transfer_limit_per_hw_request) &&
        (p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size > TRANSFER_SIZE_1_BYTE))
    {
        /* Transaction size indicated by DMAjTMR_n.DTS * DMAjRS_n.TC */
        number_transfer_byte =
            (uint32_t) ((2 << (p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size - 1U)) *
                        p_info->number_transfer);
    }
    else if ((0U == p_info->transfer_pre_load) && (0U == p_info->transfer_limit_per_hw_request) &&
             (p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size > TRANSFER_SIZE_1_BYTE))
    {
        /* Transaction size indicated by DMAjTMR_n.STS * DMAjRS_n.TC */
        number_transfer_byte =
            (uint32_t) ((2 << (p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size - 1U)) *
                        p_info->number_transfer);
    }
    else
    {
        number_transfer_byte = p_info->number_transfer;
    }

    return number_transfer_byte;
}

#if DMAC_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Parameter checking of R_DMAC_Open.
 *
 * @param[in]   p_ctrl                    Pointer to control structure.
 * @param[in]   p_cfg                     Pointer to configuration structure. All elements of the structure must be
 *                                        set by user.
 *
 * @retval FSP_SUCCESS                    Input Parameters are Valid.
 * @retval FSP_ERR_ASSERTION              An input parameter is invalid.
 * @retval FSP_ERR_IRQ_BSP_DISABLED       Callback is NULL and the DMAC IRQ is not enabled.
 * @retval FSP_ERR_ALREADY_OPEN           The control structure is already opened.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT Channel is not available on this MCU.
 **********************************************************************************************************************/
static fsp_err_t r_dmac_open_parameter_checking (dmac_instance_ctrl_t * const p_ctrl,
                                                 transfer_cfg_t const * const p_cfg)
{
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(p_ctrl->open != DMAC_OPEN, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_cfg->p_extend;

    FSP_ERROR_RETURN(p_extend->channel < DMAC_MAX_CHANNEL, FSP_ERR_IP_CHANNEL_NOT_PRESENT);

    if ((NULL != p_extend->p_callback))
    {
        FSP_ERROR_RETURN(((0 < p_extend->irq) && (FSP_INVALID_VECTOR >= p_extend->irq)), FSP_ERR_IRQ_BSP_DISABLED);
        FSP_ERROR_RETURN(((0 < p_extend->irq_descriptor) && (FSP_INVALID_VECTOR >= p_extend->irq_descriptor)),
                         FSP_ERR_IRQ_BSP_DISABLED);
        FSP_ERROR_RETURN(((0 < p_extend->irq_error) && (FSP_INVALID_VECTOR >= p_extend->irq_error)),
                         FSP_ERR_IRQ_BSP_DISABLED);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Parameter checking for r_dmac_prv_enable.
 *
 * @param[in]   p_ctrl                           Pointer to control structure.
 *
 * @retval      FSP_SUCCESS                      Alignment on source and destination pointers is valid.
 * @retval      FSP_ERR_ASSERTION                The current configuration is invalid.
 **********************************************************************************************************************/
static fsp_err_t r_dmac_enable_parameter_checking (dmac_instance_ctrl_t * const p_ctrl)
{
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;
    void const          * p_src    = (void const *) p_ctrl->p_reg->sDMAC_CH[channel].SAR;
    void const          * p_dest   = (void const *) p_ctrl->p_reg->sDMAC_CH[channel].DAR;
    transfer_size_t       des_size = (transfer_size_t) p_ctrl->p_reg->sDMAC_CH[channel].TMR_b.DTS;
    transfer_size_t       src_size = (transfer_size_t) p_ctrl->p_reg->sDMAC_CH[channel].TMR_b.STS;
    transfer_mode_t       mode     = (transfer_mode_t) p_ctrl->p_reg->sDMAC_CH[channel].TMR_b.SLM;

    /* The source and destination pointers cannot be NULL. */
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);

    /* The source and destination pointers must be aligned to the transfer size. */
    FSP_ASSERT(0U == ((uint32_t) p_dest & DMAC_PRV_MASK_ALIGN_N_BYTES(des_size)));
    FSP_ASSERT(0U == ((uint32_t) p_src & DMAC_PRV_MASK_ALIGN_N_BYTES(src_size)));

    if (TRANSFER_MODE_NORMAL == mode)
    {
        /* Setting transfer count to 0 in normal mode means transfer forever. This feature is not supported. */
        FSP_ASSERT(0 != p_ctrl->p_reg->sDMAC_CH[channel].TSR);
    }

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * Set a activation events trigger DMAC transfer and enable DMA hardware transfer
 *
 * @param[in]   p_ctrl                           Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dmac_set_hardware_request (dmac_instance_ctrl_t * p_ctrl)
{
    transfer_info_t     * p_info   = p_ctrl->p_cfg->p_info;
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint16_t              unit     = p_extend->unit;
    uint16_t              channel  = p_extend->channel;
    uint16_t              sel_register;
    uint16_t              sel_register_bit;

    uint16_t            source = p_extend->activation_source;
    uint8_t             group  = source / DMAC_PRV_MAX_RQ_SRC_PER_GROUP;
    volatile uint32_t * sel_registers;

    p_ctrl->p_defsel = DMAC_PRV_REG_DEFSEL;

    /* Setting pre-load function to PLE bit. */
    p_ctrl->p_reg->sDMAC_CH[channel].RS_b.PLE = (uint32_t) (p_info->transfer_pre_load);

    /* Setting first pre-load trigger to FPT bit. */
    p_ctrl->p_reg->sDMAC_CH[channel].RS_b.FPT = (uint32_t) (p_info->transfer_first_pre_load);

    /* Setting transfer count per hardware request to TC bit. */
    p_ctrl->p_reg->sDMAC_CH[channel].RS_b.TC = (uint32_t) (p_info->transfer_count_per_hw_request);

    /* Setting transfer limit per hardware request to TL bit. */
    p_ctrl->p_reg->sDMAC_CH[channel].RS_b.TL = (uint32_t) (p_info->transfer_limit_per_hw_request);

    /* Select correct DMACSEL register array based on unit */
    if (unit == DMAC_PRV_UNIT0_SELECTION)
    {
        sel_registers = (volatile uint32_t *) &p_ctrl->p_defsel->DMACSEL0_0_b;
    }
    else                               /* unit == DMAC_PRV_UNIT1_SELECTION */
    {
        sel_registers = (volatile uint32_t *) &p_ctrl->p_defsel->DMACSEL1_0_b;
    }

    /* Calculate register index and bit offset */
    /* Select DMA transfer request group 0 */
    if (0U == group)
    {
        sel_register                 = source / 16U;
        sel_register_bit             = (uint8_t) ((source % 16U) * 2U);
        sel_registers[sel_register] &= ~((uint32_t) (DMAC_GROUP0_SELECTION << sel_register_bit));
    }
    /* Select DMA transfer request the other group */
    else
    {
        source                       = (uint8_t) (source - (group * DMAC_PRV_MAX_RQ_SRC_PER_GROUP));
        sel_register                 = source / 16U;
        sel_register_bit             = (uint8_t) ((source % 16U) * 2U);
        sel_registers[sel_register] |= (uint32_t) (group << sel_register_bit);
    }

    /* Select Hardware DMA request source */
    p_ctrl->p_reg->sDMAC_CH[channel].RS_b.RS = (source << DMAC_PRV_RS_OFFSET);
}

/*******************************************************************************************************************//**
 * Config descriptor memory.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dmac_config_descriptor_mem (dmac_instance_ctrl_t * p_ctrl)
{
    /* Set register call to correct DMAC module unit */
    dmac_extended_cfg_t    * p_extend           = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t                  channel            = p_extend->channel;
    uint8_t                  cnt                = 0U;
    dmac_support_operation_t descriptor_support = p_extend->support_operation;
    uint8_t                  descriptor_steps;

    /* Check steps operation */
    if (DMAC_SUPPORT_OPERATION_DOUBLE_BUFFER == descriptor_support)
    {
        descriptor_steps = DMAC_STEPS_OPERATION_2;
    }
    else if ((DMAC_SUPPORT_OPERATION_REPEAT == descriptor_support) ||
             (DMAC_SUPPORT_OPERATION_HARDWARE_REQUEST == descriptor_support))
    {
        descriptor_steps = DMAC_STEPS_OPERATION_3;
    }
    else
    {
        descriptor_steps = DMAC_STEPS_OPERATION_4;
    }

    /* Update source, destination and transfer size */
    p_ctrl->p_reg->sDMAC_CH[channel].DPCR_b.UPF = DMAC_DPCR_SAR_DAR_TSR;

    /* Init first descriptor pointer */
    p_ctrl->p_reg->sDMAC_CH[channel].DPPTR_b.PTR = 0x0U;

    /* Copy the data to descriptor memory */
    for ( ; cnt < descriptor_steps; cnt++)
    {
        p_ctrl->p_reg->DMAjDES_b[(cnt * DMAC_REGISTER_LENGTH_32BIT)].DES_RAM =
            (uint32_t) (p_extend->p_descriptor[cnt].p_src);
        p_ctrl->p_reg->DMAjDES_b[(cnt * DMAC_REGISTER_LENGTH_32BIT + DMAC_REGISTER_SHIFT_1)].DES_RAM =
            (uint32_t) (p_extend->p_descriptor[cnt].p_dest);
        p_ctrl->p_reg->DMAjDES_b[(cnt * DMAC_REGISTER_LENGTH_32BIT + DMAC_REGISTER_SHIFT_2)].DES_RAM =
            (uint32_t) (p_extend->p_descriptor[cnt].number_transfer);
        p_ctrl->p_reg->DMAjDES_b[(cnt * DMAC_REGISTER_LENGTH_32BIT + DMAC_REGISTER_SHIFT_3)].DES_RAM =
            (uint32_t) (p_extend->p_descriptor[cnt].descriptor_pointer_cfg.descriptor_pointer_u32);
    }
}

/*******************************************************************************************************************//**
 * Write reliability function info to hardware register.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dmac_config_reliability_func (dmac_instance_ctrl_t * p_ctrl)
{
    /* Initialization */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    /* Setting for reliability function */
#if (1U == BSP_FEATURE_DMAC_SWITCHABLE_REDUNDANT_OPERATION)
    if (DMAC_PRV_MASTER_CHANNEL_NUMBER >= channel)
    {
        p_ctrl->p_reg->DMAjRCHS_b.RCHS = DMAC_PRV_ENABLE_BIT << channel;
    }
#endif
    p_ctrl->p_reg->DMAjCM_b[channel].SPID = (uint8_t) p_extend->p_switchable_redundant_cfg->master_spid;
    p_ctrl->p_reg->DMAjCM_b[channel].UM   = (uint8_t) p_extend->p_switchable_redundant_cfg->master_um;
}

/*******************************************************************************************************************//**
 * Supports Gathering on the source side.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dmac_gather_set (dmac_instance_ctrl_t * p_ctrl)
{
    /* Set register call to correct DMAC module unit */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    p_ctrl->p_reg->sDMAC_CH[channel].SGCR_b.GEN   = DMAC_GATHER_MODE_ENABLE;
    p_ctrl->p_reg->sDMAC_CH[channel].SGCR_b.GIRPT = p_extend->gather_setting.gather_repeat_count_inner;
    p_ctrl->p_reg->sDMAC_CH[channel].GIAI_b.GIAI  = p_extend->gather_setting.gather_inner_address;
    p_ctrl->p_reg->sDMAC_CH[channel].GOAI_b.GOAI  = p_extend->gather_setting.gather_outer_address;
}

/*******************************************************************************************************************//**
 * Supports Scattering on the destination side.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dmac_scatter_set (dmac_instance_ctrl_t * p_ctrl)
{
    /* Set register call to correct DMAC module unit */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    p_ctrl->p_reg->sDMAC_CH[channel].SGCR_b.SEN   = DMAC_SCATTER_MODE_ENABLE;
    p_ctrl->p_reg->sDMAC_CH[channel].SGCR_b.SIRPT = p_extend->scatter_setting.scatter_repeat_count_inner;
    p_ctrl->p_reg->sDMAC_CH[channel].SGCR_b.ZF    = p_extend->scatter_setting.zero_fill_mode;
    p_ctrl->p_reg->sDMAC_CH[channel].SIAI_b.SIAI  = p_extend->scatter_setting.scatter_inner_address;
    p_ctrl->p_reg->sDMAC_CH[channel].SOAI_b.SOAI  = p_extend->scatter_setting.scatter_outer_address;
}

/*******************************************************************************************************************//**
 * Supports Descriptor transfer that describe a single transfer task on a channel.
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dmac_descriptor_set (dmac_instance_ctrl_t * p_ctrl)
{
    /* Set register call to correct DMAC module unit */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    /* Config descriptor memory */
    r_dmac_config_descriptor_mem(p_ctrl);

    /* Start DMA transfer after the channel configuaration is copied from the descriptor memory */
    p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.DPB = DMAC_PRV_ENABLE_BIT;

    /* Start the descriptor operation */
    p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.DPE = DMAC_PRV_ENABLE_BIT;
}

/*******************************************************************************************************************//**
 * Clear flag CF in the last descriptor with double buffer operation
 *
 * @param[in]   p_ctrl         Pointer to control structure.
 *
 **********************************************************************************************************************/
static void r_dmac_check_double_buffer_op (dmac_instance_ctrl_t * p_ctrl)
{
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

    if (DMAC_SUPPORT_OPERATION_DOUBLE_BUFFER == p_extend->support_operation)
    {
        /* Increase the counter last descriptor value */
        p_ctrl->count_last_descriptor++;
        if (DMAC_LAST_DESCRIPTOR_NUMBER == p_ctrl->count_last_descriptor)
        {
            /* Update counter last descriptor value */
            p_ctrl->count_last_descriptor = 0U;

            /* Clear CF flag in Descriptor 0 */
            p_ctrl->p_reg->DMAjDES_b[DMAC_REGISTER_SHIFT_3].DES_RAM &= ~(DMAC_PRV_ENABLE_BIT << DMAC_BIT_SHIFT_0);
        }
    }
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to dmac instance control block
 * @param[in]     event      Event code
 **********************************************************************************************************************/
static void r_dmac_call_callback (dmac_instance_ctrl_t * p_ctrl, transfer_event_t event)
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

/*******************************************************************************************************************//**
 * DMAC ISR - DMA Channel Interrupt or descriptor step end interrupt
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void dmac_int_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Clear IRQ to make sure it doesn't fire again after exiting */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Assign unit, channel */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    transfer_event_t event = (transfer_event_t) p_ctrl->p_reg->sDMAC_CH[channel].CHSTA;

    /* Check if event is descriptor step end flag */
    if (TRANSFER_EVENT_DESCRIPTOR_FLAG == (TRANSFER_EVENT_DESCRIPTOR_FLAG & event))
    {
        /* Clear descriptor step end flag */
        p_ctrl->p_reg->sDMAC_CH[channel].CHFCR_b.DSEC = DMAC_PRV_ENABLE_BIT;

        r_dmac_check_double_buffer_op(p_ctrl);
    }

    /* Check if event is transfer end flag */
    if (TRANSFER_EVENT_TRANS_END_FLAG == (TRANSFER_EVENT_TRANS_END_FLAG & event))
    {
        /* Disable DMA transfer */
        p_ctrl->p_reg->sDMAC_CH[channel].CHCR_b.DE = DMAC_PRV_DISABLE_BIT;

        /* Clear transfer end flag */
        p_ctrl->p_reg->sDMAC_CH[channel].CHFCR_b.TEC = DMAC_PRV_ENABLE_BIT;
    }

    /* Call user callback */
    r_dmac_call_callback(p_ctrl, event);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * DMAC Address Error ISR
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void dmac_error_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Clear IRQ to make sure it doesn't fire again after exiting */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    dmac_instance_ctrl_t * p_ctrl = (dmac_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Assign unit, channel */
    dmac_extended_cfg_t * p_extend = (dmac_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t               channel  = p_extend->channel;

    transfer_event_t event = (transfer_event_t) p_ctrl->p_reg->sDMAC_CH[channel].CHSTA;

    /* Clear Address error flag */
    p_ctrl->p_reg->sDMAC_CH[channel].CHFCR_b.CAEC = DMAC_PRV_ENABLE_BIT;

    /* Call user callback */
    r_dmac_call_callback(p_ctrl, event);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}
