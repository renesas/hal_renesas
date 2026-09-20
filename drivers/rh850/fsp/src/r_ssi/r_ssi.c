/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ssi.h"                     /* PRQA S 0380 # DP_MSR_D-1.1_UC1 */
#include "r_ssi_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define SSI_PRV_SSIFCR_AUCKE_BIT           (31U)
#define SSI_PRV_SSICR_MST_BIT              (14U)

#define SSI_PRV_SSIFCR_TIE_BIT             (3U)

#define SSI_PRV_SSICR_TX_RX_IRQ_MASK       (0x3C000000U)

#define SSI_PRV_SSICR_REN_TEN_MASK         (3U)
#define SSI_PRV_SSICR_TEN_BIT              (2U)
#define SSI_PRV_SSICR_REN_BIT              (1U)

#define SSI_PRV_SSICR_TUIEN_BIT            (29U)
#define SSI_PRV_SSICR_ROIEN_BIT            (26U)
#define SSI_PRV_SSICR_IIEN_BIT             (25U)
#define SSI_PRV_SSIFCR_RIE_BIT             (2U)

#define SSI_PRV_SSISR_TUIRQ_BIT            (29U)
#define SSI_PRV_SSISR_ROIRQ_BIT            (26U)

#define SSI_PRV_SSIFCR_TFRST_RFRST_MASK    (3U)

#define SSI_PRV_SSICR_SWL_BIT              (16U)
#define SSI_PRV_SSICR_DWL_BIT              (19U)
#define SSI_PRV_SSICR_CKDV_BIT             (4U)
#define SSI_PRV_SSICR_PDTA_BIT             (9U)

#define SSI_PRV_SSIOFR_LRCONT_BIT          (8U)
#define SSI_PRV_SSIFCR_SSIRST_BIT          (16U)
#define SSI_PRV_SSIDMA_EN_BIT              (28u)
#define SSI_PRV_SSICR_FRM_BIT              (22u)
#define SSI_PRV_SSICR_BCKP_BIT             (13u)
#define SSI_PRV_SSICR_LRCKP_BIT            (12u)
#define SSI_PRV_SSICR_DEL_BIT              (8u)

/* SSIFSR has two flags that can be cleared (TDE and RDF).  The other fields in SSIFSR are read only or reserved. To
 * clear one of these flags, set the other to 1 to avoid clearing it, and write all other bits to 0. */
#define SSI_PRV_SSIFSR_RDF_CLEAR           (1U << 16)
#define SSI_PRV_SSIFSR_TDE_CLEAR           (1U)

#define SSI_PRV_SSIFRDR_READ_16_BIT_POS    (16U)
#define SSI_PRV_SSIFRDR_READ_8_BIT_POS     (24U)

/* "SSI" in ASCII, used to determine if driver is open. */
#define SSI_PRV_OPEN                       (0x535349U)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if SSI_CFG_PARAM_CHECKING_ENABLE
static fsp_err_t r_ssi_frame_alignment_check(ssi_instance_ctrl_t const * const p_instance_ctrl, uint32_t const bytes);

#endif

/* ISR subroutines */
static void r_ssi_tx_fifo_write(ssi_instance_ctrl_t * p_instance_ctrl, uint32_t stages_to_write);
static void r_ssi_rx_fifo_read(ssi_instance_ctrl_t * p_instance_ctrl, uint32_t stages_to_read);
static void r_ssi_interrupts_configure(ssi_instance_ctrl_t * const p_instance_ctrl, i2s_cfg_t const * const p_cfg);

/* FIFO subroutines */
static void r_ssi_fifo_write(ssi_instance_ctrl_t * p_instance_ctrl);
static void r_ssi_fifo_read(ssi_instance_ctrl_t * p_instance_ctrl);

static fsp_err_t r_ssi_start_check(ssi_instance_ctrl_t * const p_instance_ctrl, ssi_dir_t dir);

/* Start subroutines */
static void r_ssi_start(ssi_instance_ctrl_t const * const p_instance_ctrl, ssi_dir_t dir);

/* Stop subroutines */
static fsp_err_t r_ssi_stop_sub(ssi_instance_ctrl_t * const p_instance_ctrl);

/* Read and write subroutines. */
fsp_err_t r_ssi_tx_load_fifo(ssi_instance_ctrl_t * const p_instance_ctrl, void const * const p_src,
                             uint32_t const bytes);
fsp_err_t r_ssi_rx_unload_fifo(ssi_instance_ctrl_t * const p_instance_ctrl, void * const p_dest, uint32_t const bytes);

static void r_ssi_call_callback(ssi_instance_ctrl_t * p_ctrl, i2s_event_t event);

#if SSI_CFG_TRANSFER_ENABLE
static fsp_err_t r_ssi_dependent_drivers_configure(i2s_cfg_t const * const     p_cfg,
                                                   ssi_instance_ctrl_t * const p_instance_ctrl);
void ssi_txi_transfer_callback(transfer_callback_args_t * p_transfer_args);
void ssi_rxi_transfer_callback(transfer_callback_args_t * p_transfer_args);

#endif

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

BSP_INTERRUPT_ATTRIBUTE void ssi_txi_isr(void); /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void ssi_rxi_isr(void); /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void ssi_int_isr(void); /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1 */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/** SSI Implementation of I2S interface.  */
const i2s_api_t g_i2s_on_ssi =         /* PRQA S 1502 # Advisory R-2.8 */
{
    .open        = R_SSI_Open,         /* PRQA S 3635 # Advisory R-17.12 */
    .stop        = R_SSI_Stop,         /* PRQA S 3635 # Advisory R-17.12 */
    .write       = R_SSI_Write,        /* PRQA S 3635 # Advisory R-17.12 */
    .read        = R_SSI_Read,         /* PRQA S 3635 # Advisory R-17.12 */
    .writeRead   = R_SSI_WriteRead,    /* PRQA S 3635 # Advisory R-17.12 */
    .mute        = R_SSI_Mute,         /* PRQA S 3635 # Advisory R-17.12 */
    .statusGet   = R_SSI_StatusGet,    /* PRQA S 3635 # Advisory R-17.12 */
    .close       = R_SSI_Close,        /* PRQA S 3635 # Advisory R-17.12 */
    .callbackSet = R_SSI_CallbackSet,  /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup SSI
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Opens the SSI. Implements @ref i2s_api_t::open.
 *
 * This function sets this clock divisor and the configurations specified in i2s_cfg_t.  It also opens the timer and
 * transfer instances if they are provided.
 *
 * @retval FSP_SUCCESS                     Ready for I2S communication.
 * @retval FSP_ERR_ASSERTION               The pointer to p_ctrl or p_cfg is null.
 * @retval FSP_ERR_ALREADY_OPEN            The control block has already been opened.
 * @return                                 See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                         possible return codes. This function calls:
 *                                             * @ref transfer_api_t::open
 **********************************************************************************************************************/

fsp_err_t R_SSI_Open (i2s_ctrl_t * const p_ctrl, i2s_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) p_ctrl;     /* PRQA S 0316 # Advisory R-11.5 */
#if SSI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_callback);
    FSP_ASSERT((p_cfg->int_irq >= 0) && (FSP_INVALID_VECTOR != p_cfg->int_irq));
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(SSI_PRV_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
#endif

    ssi_extended_cfg_t * p_extend = (ssi_extended_cfg_t *) p_cfg->p_extend;                                          /* PRQA S 0311, 0316, 0329, 3678 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-11.4, Advisory R-8.13 */

    /* Calculate base address for registers on this channel. */
    uint32_t       base_address = (uint32_t) R_SSIF0 - (p_cfg->channel * ((uint32_t) R_SSIF0 - (uint32_t) R_SSIF1)); /* PRQA S 0306, 0329 # Advisory R-11.4 */
    R_SSIF0_Type * p_reg        = (R_SSIF0_Type *) base_address;                                                     /* PRQA S 0306 # Advisory R-11.4 */

    /* Determine how to access the FIFO (1 byte, 2 byte, or 4 byte access). */
    transfer_size_t fifo_access_size = TRANSFER_SIZE_4_BYTE;
    if (I2S_PCM_WIDTH_8_BITS == p_cfg->pcm_width)
    {
        fifo_access_size = TRANSFER_SIZE_1_BYTE;
    }

    if (I2S_PCM_WIDTH_16_BITS == p_cfg->pcm_width)
    {
        fifo_access_size = TRANSFER_SIZE_2_BYTE;
    }

#if SSI_CFG_TRANSFER_ENABLE

    /* Configure transfer drivers. */
    fsp_err_t err = r_ssi_dependent_drivers_configure(p_cfg, p_instance_ctrl);
    FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
#endif

    /* Initialize the control structure. */
    p_instance_ctrl->p_reg            = p_reg;
    p_instance_ctrl->p_cfg            = p_cfg;
    p_instance_ctrl->fifo_access_size = fifo_access_size;

    /* Configure interrupts. */
    r_ssi_interrupts_configure(p_instance_ctrl, p_cfg);

    /* Calculate register settings. */

    /* Configure operating mode. */
    uint32_t ssifcr = (uint32_t) p_cfg->operating_mode << SSI_PRV_SSIFCR_AUCKE_BIT;
    uint32_t ssicr  = (uint32_t) p_cfg->operating_mode << SSI_PRV_SSICR_MST_BIT;

    /* Configure sample size, and word length. */
    ssicr |= (uint32_t) p_cfg->word_length << SSI_PRV_SSICR_SWL_BIT;
    ssicr |= (uint32_t) p_cfg->pcm_width << SSI_PRV_SSICR_DWL_BIT;
    if ((p_cfg->pcm_width > I2S_PCM_WIDTH_16_BITS) && (p_cfg->pcm_width < I2S_PCM_WIDTH_32_BITS))
    {
        /* Right justify data for 18-bit, 20-bit, 22-bit, or 24-bit samples. This setting is prohibited for 8-bit,
         * 16-bit, and 32-bit samples  */
        ssicr |= (1U << SSI_PRV_SSICR_PDTA_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
    }

    /* Configure audio clock, comumunication format and WS continue in master mode only. */
    uint32_t ssiofr = 0U;
    ssiofr |= p_extend->communication_format;  /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    ssicr  |= (1U << SSI_PRV_SSICR_LRCKP_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */

    /* Set Frame Word Number when communication format is TDM format. */
    if (SSI_COMMUNICATION_FORMAT_TDM == p_extend->communication_format)
    {
        ssicr |= (p_extend->frame_word_number << SSI_PRV_SSICR_FRM_BIT); /* PRQA S 1821, 4523, 4532 # DP_MSR_R-10.4_UC1 */
    }
    /* Enable bit BCKP when communication format is Monaural format. */
    else if (SSI_COMMUNICATION_FORMAT_MONAURAL == p_extend->communication_format)
    {
        ssicr |= (1U << SSI_PRV_SSICR_BCKP_BIT);                                /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */

        /* Set bit Selects serial data delay when communication format is Monaural format*/
        ssicr |= (p_extend->frametype_monauralformat << SSI_PRV_SSICR_DEL_BIT); /* PRQA S 1821, 4523, 4532 # DP_MSR_R-10.4_UC1 */
    }
    /*Disable bit LRCKP Bit when communication format is I2S format */
    else
    {
        ssicr &= ~(1U << SSI_PRV_SSICR_LRCKP_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
    }

    if (I2S_MODE_MASTER == p_cfg->operating_mode)
    {
        ssicr |= (uint32_t) p_extend->bit_clock_div << SSI_PRV_SSICR_CKDV_BIT;

        if (I2S_WS_CONTINUE_ON == p_cfg->ws_continue)
        {
            ssiofr |= (1U << SSI_PRV_SSIOFR_LRCONT_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
        }
    }

    /* Set SSIF receive FIFO watermark to exactly the DTC block size so the DTC triggers as soon as there is
     * a block of data in the FIFO. */
    uint32_t rx_fifo_setting = p_extend->receive_data_full;

    /* Set SSIF transmit FIFO watermark to trigger when 1/2 of the FIFO is empty so that read and write are less
     * likely to trigger at the same time. */
    uint32_t tx_fifo_setting = p_extend->transmit_data_empty;
    uint32_t ssiscr          = (tx_fifo_setting << 8) | rx_fifo_setting;

    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    /* Configure master/slave mode and associated clock settings for master mode. */
    p_instance_ctrl->p_reg->SSICR = ssicr;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Reset SSIF and set operating mode. Set SSIRST, then clear it and wait for it to clear. This operation resets
     * all interrupt flags in SSISR and empties the FIFOs. */
    p_instance_ctrl->p_reg->SSIFCR = ssifcr | (1U << SSI_PRV_SSIFCR_SSIRST_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */ /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg->SSIFCR = ssifcr;                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Wait for SSIRST to clear before continuing. */
    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->SSIFCR, ssifcr);          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_instance_ctrl->p_reg->SSISCR = ssiscr;                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg->SSIOFR = ssiofr;                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Initialization complete. */
    p_instance_ctrl->open = SSI_PRV_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stops SSI. Implements @ref i2s_api_t::stop.
 *
 * This function disables both transmission and reception, and disables any transfer instances used.
 *
 * The SSI will stop on the next frame boundary.  Do not restart SSI until it is idle.
 *
 * @retval FSP_SUCCESS           I2S communication stop request issued.
 * @retval FSP_ERR_ASSERTION     The pointer to p_ctrl was null.
 * @retval FSP_ERR_NOT_OPEN      The channel is not opened.
 * @return                       See @ref RENESAS_ERROR_CODES or lower level drivers for other possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_SSI_Stop (i2s_ctrl_t * const p_ctrl)                            /* PRQA S 1505 # Advisory R-8.7 */
{
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t             err             = FSP_SUCCESS;

#if SSI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SSI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Stop is complete after an I2S_EVENT_IDLE interrupt. */
    err = r_ssi_stop_sub(p_instance_ctrl);

    return err;
}

/*******************************************************************************************************************//**
 * Writes data buffer to SSI. Implements @ref i2s_api_t::write.
 *
 * This function resets the transfer if the transfer interface is used, or writes the length of data that fits in the
 * FIFO then stores the remaining write buffer in the control block to be written in the ISR.
 *
 * Write() cannot be called if another read() or writeRead() operation is in progress. Write can be called
 * when the SSI is idle, or after the I2S_EVENT_TX_EMPTY event.
 *
 * @retval FSP_SUCCESS                 Write initiated successfully.
 * @retval FSP_ERR_ASSERTION           The pointer to p_ctrl or p_src was null, or bytes requested was 0.
 * @retval FSP_ERR_IN_USE              Another transfer is in progress, data was not written.
 * @retval FSP_ERR_NOT_OPEN            The channel is not opened.
 * @retval FSP_ERR_UNDERFLOW           A transmit underflow error is pending. Wait for the SSI to go idle before
 *                                     resuming communication.
 * @return                             See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                     possible return codes. This function calls:
 *                                         * @ref transfer_api_t::reset
 **********************************************************************************************************************/

fsp_err_t R_SSI_Write (i2s_ctrl_t * const p_ctrl, void const * const p_src, uint32_t const bytes) /* PRQA S 1505 # Advisory R-8.7 */
{
    fsp_err_t             err;
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) p_ctrl;                       /* PRQA S 0316 # Advisory R-11.5 */

#if SSI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SSI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_src);
    err = r_ssi_frame_alignment_check(p_instance_ctrl, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    /* Validate that SSI is ready for a new transmit operation. */
    err = r_ssi_start_check(p_instance_ctrl, SSI_DIR_TX);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* If a transfer instance is provided for write, reset the transfer. Otherwise store data to transmit in the
     * transmit interrupt. */
    err = r_ssi_tx_load_fifo(p_instance_ctrl, p_src, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* If a transmit underflow is pending, return an error.  This could happen if the application is in an interrupt
     * context with a priority greater than or equal to the idle/error interrupt priority and does not reload the
     * transmit FIFO before it underflows. */
    FSP_ERROR_RETURN(0U == p_instance_ctrl->p_reg->SSISR_b.TUIRQ, FSP_ERR_UNDERFLOW); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Make sure transmission is enabled. */
    r_ssi_start(p_instance_ctrl, SSI_DIR_TX);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Reads data into provided buffer. Implements @ref i2s_api_t::read.
 *
 * This function resets the transfer if the transfer interface is used, or reads the length of data available in the
 * FIFO then stores the remaining read buffer in the control block to be filled in the ISR.
 *
 * Read() cannot be called if another write() or writeRead() operation is in progress. Read can be called
 * when the SSI is idle, or after the I2S_EVENT_RX_FULL event.
 *
 * @retval FSP_SUCCESS                 Read initiated successfully.
 * @retval FSP_ERR_IN_USE              Peripheral is in the wrong mode or not idle.
 * @retval FSP_ERR_ASSERTION           The pointer to p_ctrl or p_dest was null, or bytes requested was 0.
 * @retval FSP_ERR_NOT_OPEN            The channel is not opened.
 * @retval FSP_ERR_OVERFLOW            A receive overflow error is pending. Wait for the SSI to go idle before
 *                                     resuming communication.
 * @return                             See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                     possible return codes. This function calls:
 *                                         * @ref transfer_api_t::reset
 **********************************************************************************************************************/
fsp_err_t R_SSI_Read (i2s_ctrl_t * const p_ctrl, void * const p_dest, uint32_t const bytes) /* PRQA S 1505 # Advisory R-8.7 */
{
    fsp_err_t             err;
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) p_ctrl;                 /* PRQA S 0316 # Advisory R-11.5 */

#if SSI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SSI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_dest);
    err = r_ssi_frame_alignment_check(p_instance_ctrl, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    /* Validate that SSI is ready for a new receive operation. */
    err = r_ssi_start_check(p_instance_ctrl, SSI_DIR_RX);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* If a transfer instance is provided for read, reset the transfer. Otherwise store data to receive in the receive
     * interrupt. */
    err = r_ssi_rx_unload_fifo(p_instance_ctrl, p_dest, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* If a receive overflow is pending, return an error.  This could happen if the application is in an interrupt
     * context with a priority greater than or equal to the idle/error interrupt priority and does not read from the
     * receive FIFO before it overflows. */
    FSP_ERROR_RETURN(0U == p_instance_ctrl->p_reg->SSISR_b.ROIRQ, FSP_ERR_OVERFLOW); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Make sure reception is enabled. */
    r_ssi_start(p_instance_ctrl, SSI_DIR_RX);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Writes from source buffer and reads data into destination buffer. Implements @ref i2s_api_t::writeRead.
 *
 * This function calls R_SSI_Write and R_SSI_Read.
 *
 * writeRead() cannot be called if another write() or read() operation is in progress. writeRead() can be
 * called when the SSI is idle, or after the I2S_EVENT_RX_FULL event.
 *
 * @retval FSP_SUCCESS                 Write and read initiated successfully.
 * @retval FSP_ERR_IN_USE              Peripheral is in the wrong mode or not idle.
 * @retval FSP_ERR_ASSERTION           An input parameter was invalid.
 * @retval FSP_ERR_NOT_OPEN            The channel is not opened.
 * @retval FSP_ERR_UNDERFLOW           A transmit underflow error is pending. Wait for the SSI to go idle before
 *                                     resuming communication.
 * @retval FSP_ERR_OVERFLOW            A receive overflow error is pending. Wait for the SSI to go idle before
 *                                     resuming communication.
 * @return                             See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                     possible return codes. This function calls:
 *                                         * @ref transfer_api_t::reset
 **********************************************************************************************************************/
fsp_err_t R_SSI_WriteRead (i2s_ctrl_t * const p_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                           void const * const p_src,
                           void * const       p_dest,
                           uint32_t const     bytes)
{
    fsp_err_t             err;
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if SSI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);
    FSP_ERROR_RETURN(SSI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    err = r_ssi_frame_alignment_check(p_instance_ctrl, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    /* Validate that SSI is ready for a new transmit and receive operation. */
    err = r_ssi_start_check(p_instance_ctrl, SSI_DIR_TX_RX);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* If a transfer instance is provided for write, reset the transfer. Reset the transmit FIFO first since the
     * transmit FIFO will underflow before the receive FIFO overflows during full duplex communication. */
    err = r_ssi_tx_load_fifo(p_instance_ctrl, p_src, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* If a transfer instance is provided for read, reset the transfer. */
    err = r_ssi_rx_unload_fifo(p_instance_ctrl, p_dest, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* If a transmit underflow or receive overflow is pending, return an error.  This could happen if the application
     * is in an interrupt context with a priority greater than or equal to the idle/error interrupt priority and does
     * not reload the FIFOs before they underflow/overflow. */
    uint32_t ssisr = p_instance_ctrl->p_reg->SSISR;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_ERROR_RETURN(0U == (ssisr & (1U << SSI_PRV_SSISR_TUIRQ_BIT)), FSP_ERR_UNDERFLOW); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
    FSP_ERROR_RETURN(0U == (ssisr & (1U << SSI_PRV_SSISR_ROIRQ_BIT)), FSP_ERR_OVERFLOW);  /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */

    /* Make sure transmission and reception are enabled. */
    r_ssi_start(p_instance_ctrl, SSI_DIR_TX_RX);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Mutes SSI on the next frame boundary. Implements @ref i2s_api_t::mute.
 *
 * Data is still written while mute is enabled, but the transmit line outputs zeros.
 *
 * @retval FSP_SUCCESS           Transmission is muted.
 * @retval FSP_ERR_ASSERTION     The pointer to p_ctrl was null.
 * @retval FSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SSI_Mute (i2s_ctrl_t * const p_ctrl, i2s_mute_t const mute_enable) /* PRQA S 3673, 1505 # Advisory R-8.7 */
{
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) p_ctrl;    /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if SSI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SSI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->p_reg->SSICR_b.MUEN = mute_enable; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */ /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Gets SSI status and stores it in provided pointer p_status. Implements @ref i2s_api_t::statusGet.
 *
 * @retval FSP_SUCCESS           Information stored successfully.
 * @retval FSP_ERR_ASSERTION     The p_instance_ctrl or p_status parameter was null.
 * @retval FSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SSI_StatusGet (i2s_ctrl_t * const p_ctrl, i2s_status_t * const p_status) /* PRQA S 3673, 1505 # Advisory R-8.7 */
{
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) p_ctrl;          /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if SSI_CFG_PARAM_CHECKING_ENABLE

    /* Make sure parameters are valid. */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_status);
    FSP_ERROR_RETURN(SSI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_status->state = (i2s_state_t) p_instance_ctrl->p_reg->SSISR_b.IIRQ; /* PRQA S 4342 # Advisory R-10.5 */ /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Closes SSI. Implements @ref i2s_api_t::close.
 *
 * This function powers down the SSI and closes the lower level timer and transfer drivers if they are used.
 *
 * @retval FSP_SUCCESS           Device closed successfully.
 * @retval FSP_ERR_ASSERTION     The pointer to p_ctrl was null.
 * @retval FSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SSI_Close (i2s_ctrl_t * const p_ctrl)                           /* PRQA S 1505 # Advisory R-8.7 */
{
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t             err             = FSP_SUCCESS;
#if SSI_CFG_TRANSFER_ENABLE
    bool transfer_used = false;
#endif
#if SSI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SSI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->open = 0U;

    /* Stop SSIF. */
    p_instance_ctrl->p_reg->SSICR  = 0U;                                                           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg->SSIFCR = 0U;                                                           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Disable interrupts. */
    R_BSP_IrqDisable(p_instance_ctrl->p_cfg->int_irq);
    if ((p_instance_ctrl->p_cfg->rxi_irq >= 0) && (NULL == p_instance_ctrl->p_cfg->p_transfer_rx)) /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->rxi_irq);
    }

    if ((p_instance_ctrl->p_cfg->txi_irq >= 0) && (NULL == p_instance_ctrl->p_cfg->p_transfer_tx)) /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->txi_irq);
    }

#if (1U == SSI_CFG_TRANSFER_ENABLE)

/* If transfer is used, disable transfer when stop is requested. */
    if (NULL != p_instance_ctrl->p_cfg->p_transfer_rx)
    {
        err = p_instance_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_instance_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
        err = p_instance_ctrl->p_cfg->p_transfer_rx->p_api->close(p_instance_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
        transfer_used = true;
    }

    if (NULL != p_instance_ctrl->p_cfg->p_transfer_tx)
    {
        err = p_instance_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_instance_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
        err = p_instance_ctrl->p_cfg->p_transfer_tx->p_api->close(p_instance_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
        transfer_used = true;
    }

    if (true == transfer_used)
    {
        /* Bit DMA enable in SSIDMA is disable when DMA/DTS support */
        p_instance_ctrl->p_reg->SSIDMA &= ~(1U << SSI_PRV_SSIDMA_EN_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */ /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
#endif

    p_instance_ctrl->open = 0U;

    return err;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements i2s_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_SSI_CallbackSet (i2s_ctrl_t * const          p_api_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                             void (                    * p_callback)(i2s_callback_args_t * p_args),
                             void * const                p_context,
                             i2s_callback_args_t * const p_callback_memory)
{
    ssi_instance_ctrl_t * p_ctrl = (ssi_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (SSI_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_callback);                                            /* PRQA S 3344 # DP_MSR_R-14.4_UC1 */
    FSP_ERROR_RETURN(SSI_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup R_SSI)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Disables SSI transmission and reception.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 **********************************************************************************************************************/
static fsp_err_t r_ssi_stop_sub (ssi_instance_ctrl_t * const p_instance_ctrl) /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Stop communication following the procedure from Figure 30.58 "Procedure to halt communication (CPU operation
     * procedure)" in the R-CAR U5L manual . */
#if SSI_CFG_TRANSFER_ENABLE
    bool transfer_used = false;
#endif
    fsp_err_t err = FSP_SUCCESS;

    /* Disable transmission and reception and associated interrupts. Enable idle interrupt. */
    uint32_t ssicr = p_instance_ctrl->p_reg->SSICR;                                                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    ssicr &= ~(SSI_PRV_SSICR_REN_TEN_MASK | SSI_PRV_SSICR_TX_RX_IRQ_MASK);
    ssicr |= (1U << SSI_PRV_SSICR_IIEN_BIT);                                                                        /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
    p_instance_ctrl->p_reg->SSICR = ssicr;                                                                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Disable interrupt output. Clear RIE and TIE by clearing all bits except AUCKE, which is set or cleared depending
     * on the mode. All other bits can be set to 0. */
    p_instance_ctrl->p_reg->SSIFCR = (uint32_t) p_instance_ctrl->p_cfg->operating_mode << SSI_PRV_SSIFCR_AUCKE_BIT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

#if SSI_CFG_TRANSFER_ENABLE

    /* If transfer is used, disable transfer when stop is requested. */
    if (NULL != p_instance_ctrl->p_cfg->p_transfer_rx)
    {
        err = p_instance_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_instance_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
        transfer_used = true;
    }

    if (NULL != p_instance_ctrl->p_cfg->p_transfer_tx)
    {
        err = p_instance_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_instance_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
        transfer_used = true;
    }

    if (true == transfer_used)
    {
        /* Set bit DMA enable in SSIDMA is Disable when DMA/DTS support */
        p_instance_ctrl->p_reg->SSIDMA &= ~(1U << SSI_PRV_SSIDMA_EN_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */ /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
#endif

    /* Clear control structure data. */
    p_instance_ctrl->p_tx_src        = NULL;
    p_instance_ctrl->tx_src_samples  = 0U;
    p_instance_ctrl->p_rx_dest       = NULL;
    p_instance_ctrl->rx_dest_samples = 0U;

    return err;
}

/*******************************************************************************************************************//**
 * Configures the transmit FIFO to be loaded by DTC or stores the source data to be loaded into the FIFO in the transmit
 * interrupt.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 * @param[in] p_src                    Pointer to the source buffer.
 * @param[in] bytes                    Length of source buffer.
 *
 * @retval FSP_SUCCESS                 Transmit FIFO successfully loaded.
 * @return                             See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                     possible return codes. This function calls:
 *                                         * @ref transfer_api_t::reset
 **********************************************************************************************************************/
fsp_err_t r_ssi_tx_load_fifo (ssi_instance_ctrl_t * const p_instance_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                              void const * const          p_src,
                              uint32_t const              bytes)
{
    void   * p_data  = (void *) p_src;                                     /* PRQA S 0311, 3678 # DP_MSR_R-11.8_UC1,  Advisory R-8.13 */
    uint32_t samples = bytes >> p_instance_ctrl->fifo_access_size;         /* PRQA S 4524 */

#if SSI_CFG_TRANSFER_ENABLE

/* By default, bytes are written in the ISR. */
/* If a transfer instance is provided, reset the transfer. */
    transfer_instance_t const * const p_transfer = p_instance_ctrl->p_cfg->p_transfer_tx;
    if (NULL != p_transfer)
    {
        void * p_dest;

        /*Calculate the number of transfer */
        uint8_t transfer_blocks = BSP_FEATURE_SSI_FIFO_NUM_STAGES;
        if (samples < BSP_FEATURE_SSI_FIFO_NUM_STAGES)
        {
            /* set transfer block is samples when number data < number stages*/
            transfer_blocks = (uint8_t) samples;
        }

        p_dest = (void *) &(p_instance_ctrl->p_reg->SSIFTDR);                                   /* PRQA S 0311, 0312 # DP_MSR_R-11.8_UC1 */

        /* DTS Support */
        if (p_transfer->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_mode == /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            TRANSFER_DTSC_MODE_BLOCK_1)
        {
            p_transfer->p_cfg->p_info->number_transfer =
                (uint32_t) (transfer_blocks);
            p_transfer->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_data_size = /* PRQA S 4423 # Advisory R-10.3 */
                                                                                                    p_instance_ctrl->
                                                                                                    fifo_access_size;
        }
        /* DMA Support */
        else if (p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.transfer_mode == TRANSFER_MODE_NORMAL) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            p_transfer->p_cfg->p_info->number_transfer =
                (uint32_t) (transfer_blocks << (uint32_t) p_instance_ctrl->fifo_access_size);                        /* PRQA S 4391 # Advisory R-10.6 */
            p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size =                            /* PRQA S 4423 # Advisory R-10.3 */
                                                                                          p_instance_ctrl->
                                                                                          fifo_access_size;
            p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size =                            /* PRQA S 4423 # Advisory R-10.3 */
                                                                                          p_instance_ctrl->
                                                                                          fifo_access_size;
        }
        else
        {
            /*Do nothing*/
        }

        p_transfer->p_cfg->p_info->p_src  = p_src;
        p_transfer->p_cfg->p_info->p_dest = p_dest;

        fsp_err_t err = p_transfer->p_api->reconfigure(p_transfer->p_ctrl, p_transfer->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_transfer->p_api->enable(p_transfer->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Calculate the samples */
        samples -= transfer_blocks;

        /* Increase the source address */
        p_data = (void *) ((uint8_t *) (p_src) + ((0x01U << p_instance_ctrl->fifo_access_size) * transfer_blocks)); /* PRQA S 0311, 0316, 0488, 4524 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-18.4 */
    }
#endif

    p_instance_ctrl->p_tx_src       = p_data;
    p_instance_ctrl->tx_src_samples = samples;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configures the receive FIFO to be unloaded by DTC or stores the destination buffer for data to be unloaded in the
 * receive interrupt.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 * @param[in] p_dest                   Pointer to the destination buffer.
 * @param[in] bytes                    Length of destination buffer.
 *
 * @retval FSP_SUCCESS                 Receive FIFO successfully unloaded.
 * @return                             See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                     possible return codes. This function calls:
 *                                         * @ref transfer_api_t::reset
 **********************************************************************************************************************/
fsp_err_t r_ssi_rx_unload_fifo (ssi_instance_ctrl_t * const p_instance_ctrl, void * const p_dest, uint32_t const bytes) /* PRQA S 1505 # Advisory R-8.7 */
{
    void   * p_data  = p_dest;
    uint32_t samples = bytes >> p_instance_ctrl->fifo_access_size;                                                      /* PRQA S 4524 */

#if SSI_CFG_TRANSFER_ENABLE

/* By default, bytes are written in the ISR. */

/* If a transfer instance is provided for reception, reset the transfer. */
    transfer_instance_t const * const p_transfer = p_instance_ctrl->p_cfg->p_transfer_rx;
    ssi_extended_cfg_t              * p_extend   = (ssi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-8.13 */
    if (NULL != p_transfer)
    {
        void * p_src;                                                                                       /* PRQA S 3678 # Advisory R-8.13 */

/*Calculate the number of transfer*/
        uint8_t transfer_blocks = p_extend->receive_data_full + 1U;
        if (samples < transfer_blocks)
        {
            /* Set transfer block is samples when number data < number stages*/
            transfer_blocks = (uint8_t) samples;
        }

        p_src = (void *) &(p_instance_ctrl->p_reg->SSIFRDR); /* PRQA S 0311, 0312 # DP_MSR_R-11.8_UC1 */
        if (TRANSFER_SIZE_1_BYTE == p_instance_ctrl->fifo_access_size)
        {
            p_src = (void *) ((uint8_t *) p_src + 3U);       /* PRQA S 0316, 0488 # Advisory R-11.5, Advisory R-18.4 */
        }
        else if (TRANSFER_SIZE_2_BYTE == p_instance_ctrl->fifo_access_size)
        {
            p_src = (void *) ((uint8_t *) p_src + 2U);       /* PRQA S 0316, 0488 # Advisory R-11.5, Advisory R-18.4 */
        }
        else
        {
            /*Do nothing*/
        }

        /* DTS Support */
        if (p_transfer->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_mode == /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            TRANSFER_DTSC_MODE_BLOCK_1)
        {
            p_transfer->p_cfg->p_info->number_transfer =
                (uint32_t) (transfer_blocks);
            p_transfer->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_data_size = /* PRQA S 1881, 4423 # DP_MSR_R-10.4_UC1, Advisory R-10.3 */
                                                                                                    p_instance_ctrl->
                                                                                                    fifo_access_size;
        }
        /* DMA Support */
        else if (p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.transfer_mode == TRANSFER_MODE_NORMAL) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            p_transfer->p_cfg->p_info->number_transfer =
                (uint32_t) (transfer_blocks << p_instance_ctrl->fifo_access_size);                                   /* PRQA S 4391, 4524 # Advisory R-10.6 */
            p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size =                            /* PRQA S 4423 # Advisory R-10.3 */
                                                                                          p_instance_ctrl->
                                                                                          fifo_access_size;
            p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size =                            /* PRQA S 4423 # Advisory R-10.3 */
                                                                                          p_instance_ctrl->
                                                                                          fifo_access_size;
        }
        else
        {
            /*Do nothing*/
        }

        p_transfer->p_cfg->p_info->p_src  = p_src;
        p_transfer->p_cfg->p_info->p_dest = p_dest;
        fsp_err_t err = p_transfer->p_api->reconfigure(p_transfer->p_ctrl, p_transfer->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_transfer->p_api->enable(p_transfer->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

/* Calculate the samples */
        samples -= transfer_blocks;

/* Increase the dest address */
        p_data = (void *) ((uint8_t *) (p_dest) + ((0x01U << p_instance_ctrl->fifo_access_size) * transfer_blocks)); /* PRQA S 0316, 0488, 4524 # Advisory R-11.5, Advisory R-18.4 */
    }
#endif

    p_instance_ctrl->p_rx_dest       = p_data;
    p_instance_ctrl->rx_dest_samples = samples;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Checks whether SSI transmission and/or reception can be started.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 * @param[in] dir                      Start transmit, receive, or both.
 *
 * @retval FSP_SUCCESS                 Ready for I2S transmission.
 * @retval FSP_ERR_IN_USE              Peripheral is in the wrong mode or not idle.
 **********************************************************************************************************************/
static fsp_err_t r_ssi_start_check (ssi_instance_ctrl_t * const p_instance_ctrl, ssi_dir_t dir) /* PRQA S 3673 */
{
    uint32_t ssicr                 = p_instance_ctrl->p_reg->SSICR;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint32_t current_ssicr_ren_ten = ssicr & SSI_PRV_SSICR_REN_TEN_MASK;
    uint32_t desired_ssicr_ren_ten = (uint32_t) dir;

    /* If the peripheral is not already in the correct mode, attempt to start it. */
    if (desired_ssicr_ren_ten != (current_ssicr_ren_ten & desired_ssicr_ren_ten))
    {
        /* If the peripheral is in the wrong mode or not idle, return an error. The SSI must be idle before setting
         * REN or TEN. */
        FSP_ERROR_RETURN(0U == current_ssicr_ren_ten, FSP_ERR_IN_USE);
        FSP_ERROR_RETURN(1U == p_instance_ctrl->p_reg->SSISR_b.IIRQ, FSP_ERR_IN_USE); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enables SSI transmission and/or reception.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 * @param[in] dir                      Start transmit, receive, or both.
 **********************************************************************************************************************/
static void r_ssi_start (ssi_instance_ctrl_t const * const p_instance_ctrl, ssi_dir_t dir)
{
    uint32_t ssicr                 = p_instance_ctrl->p_reg->SSICR; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint32_t current_ssicr_ren_ten = ssicr & SSI_PRV_SSICR_REN_TEN_MASK;
    uint32_t desired_ssicr_ren_ten = (uint32_t) dir;

    /* If the peripheral is not already in the correct mode, attempt to start it. */
    if (desired_ssicr_ren_ten != (current_ssicr_ren_ten & desired_ssicr_ren_ten))
    {
        /* Reset SSIF FIFOs. Set TFRST and RFRST, then clear them and wait for them to clear. This operation empties
         * the FIFOs. */
        uint32_t ssifcr = p_instance_ctrl->p_reg->SSIFCR;                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg->SSIFCR = ssifcr | SSI_PRV_SSIFCR_TFRST_RFRST_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg->SSIFCR = ssifcr;                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Wait for TFRST and RFRST to clear before continuing. */
        FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->SSIFCR, ssifcr);        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* If starting transmission, enable transmit interrupts. */
        ssicr |= desired_ssicr_ren_ten;
        if ((desired_ssicr_ren_ten & SSI_PRV_SSICR_TEN_BIT) > 0U)
        {
            /* Setting to start SSI Tx */
            ssicr  |= (1U << SSI_PRV_SSICR_TUIEN_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
            ssifcr |= (1U << SSI_PRV_SSIFCR_TIE_BIT);  /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
        }

        /* If starting reception, enable receive interrupts. */
        if ((desired_ssicr_ren_ten & SSI_PRV_SSICR_REN_BIT) > 0U)
        {
            /* Setting to start SSI Rx */
            ssicr  |= (1U << SSI_PRV_SSICR_ROIEN_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
            ssifcr |= (1U << SSI_PRV_SSIFCR_RIE_BIT);  /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
        }

        if ((NULL != p_instance_ctrl->p_cfg->p_transfer_tx) ||
            (NULL != p_instance_ctrl->p_cfg->p_transfer_rx))
        {
            /* Set bit DMA enable in SSIDMA is Enable when DMA/DTS support */
            p_instance_ctrl->p_reg->SSIDMA |= (1U << SSI_PRV_SSIDMA_EN_BIT); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */ /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Enabling communication clears related error flags in SSISR. */
        p_instance_ctrl->p_reg->SSIFCR = ssifcr;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg->SSICR  = ssicr;                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
}

/*******************************************************************************************************************//**
 *  Writes data to FIFO.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 *
 * @return The number of stages written
 **********************************************************************************************************************/
static void r_ssi_fifo_write (ssi_instance_ctrl_t * p_instance_ctrl)
{
    /* Calculate the number of words to write, limited by the space available in the FIFO. */
    uint32_t stages_to_write = p_instance_ctrl->tx_src_samples;

    /* Calculate the number of free spaces in transmit FIFO. */
    uint32_t fifo_free_stages = BSP_FEATURE_SSI_FIFO_NUM_STAGES - (uint32_t) p_instance_ctrl->p_reg->SSIFSR_b.TDC; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    if (stages_to_write > fifo_free_stages)
    {
        stages_to_write = fifo_free_stages;
    }

    r_ssi_tx_fifo_write(p_instance_ctrl, stages_to_write);
    uint32_t bytes_written = stages_to_write << p_instance_ctrl->fifo_access_size;                  /* PRQA S 4524 */
    p_instance_ctrl->tx_src_samples -= stages_to_write;
    p_instance_ctrl->p_tx_src        = (uint8_t const *) p_instance_ctrl->p_tx_src + bytes_written; /* PRQA S 0316, 0488 # Advisory R-11.5, Advisory R-18.4 */

    /* Clear TDE only if something was written to the FIFO, otherwise the transmit interrupt will fire repeatedly. */
    if (stages_to_write > 0U)
    {
        /* The TDE bit must be read as 1 before it can be cleared. */
        p_instance_ctrl->p_reg->SSIFSR;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg->SSIFSR = SSI_PRV_SSIFSR_TDE_CLEAR; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* If transmission is complete, clear transmit buffer to NULL. */
    if (0U == p_instance_ctrl->tx_src_samples)
    {
        p_instance_ctrl->p_tx_src = NULL;
    }
}

/*******************************************************************************************************************//**
 *  Reads data from FIFO.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 **********************************************************************************************************************/
static void r_ssi_fifo_read (ssi_instance_ctrl_t * p_instance_ctrl)
{
    /* Calculate the number of available bytes of data in receive FIFO. */
    uint32_t fifo_filled_stages = p_instance_ctrl->p_reg->SSIFSR_b.RDC; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Calculate the number of FIFO stages requested to read. */
    uint32_t stages_to_read = 0U;
    stages_to_read = p_instance_ctrl->rx_dest_samples;
    if (stages_to_read > fifo_filled_stages)
    {
        stages_to_read = fifo_filled_stages;
    }

    r_ssi_rx_fifo_read(p_instance_ctrl, stages_to_read);
    uint32_t bytes_read = stages_to_read << p_instance_ctrl->fifo_access_size;               /* PRQA S 4524 */
    p_instance_ctrl->rx_dest_samples -= stages_to_read;
    p_instance_ctrl->p_rx_dest        = (uint8_t *) p_instance_ctrl->p_rx_dest + bytes_read; /* PRQA S 0316, 0488 # Advisory R-11.5, Advisory R-18.4 */

    /* Clear RDF only if something was written to the FIFO, otherwise the receive interrupt will fire repeatedly. */
    if (stages_to_read > 0U)
    {
        p_instance_ctrl->p_reg->SSIFSR;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg->SSIFSR = SSI_PRV_SSIFSR_RDF_CLEAR; /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */ /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* If reception is complete, clear receive buffer to NULL. */
    if (0U == p_instance_ctrl->rx_dest_samples)
    {
        p_instance_ctrl->p_rx_dest = NULL;
        if (NULL != p_instance_ctrl->p_cfg->p_transfer_rx)
        {
            r_ssi_call_callback(p_instance_ctrl, I2S_EVENT_RX_FULL);
        }
    }
}

/*******************************************************************************************************************//**
 * Writes data to the transmit FIFO based on the FIFO access size.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 * @param[in] stages_to_write          Number of times to write to the FIFO.
 **********************************************************************************************************************/
static void r_ssi_tx_fifo_write (ssi_instance_ctrl_t * p_instance_ctrl, uint32_t stages_to_write) /* PRQA S 3673 */
{
    if (TRANSFER_SIZE_4_BYTE == p_instance_ctrl->fifo_access_size)
    {
        uint32_t * p_src32 = (uint32_t *) p_instance_ctrl->p_tx_src;                              /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-8.13 */
        for (uint32_t i = 0; i < stages_to_write; i++)
        {
            p_instance_ctrl->p_reg->SSIFTDR = *p_src32;                                           /* PRQA S 1710 # Advisory D-4.5 */
            p_src32++;
        }
    }

    if (TRANSFER_SIZE_2_BYTE == p_instance_ctrl->fifo_access_size)
    {
        /* Half word access SSIFTDR register*/
        volatile uint16_t * p_ssiftdr16 = (volatile uint16_t *) &(p_instance_ctrl->p_reg->SSIFTDR); /* PRQA S 0310 # Advisory R-11.4 */
        uint16_t          * p_src16     = (uint16_t *) p_instance_ctrl->p_tx_src;                   /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-8.13 */
        for (uint32_t i = 0; i < stages_to_write; i++)
        {
            *p_ssiftdr16 = *p_src16;
            p_src16++;
        }
    }

    if (TRANSFER_SIZE_1_BYTE == p_instance_ctrl->fifo_access_size)
    {
        /* Byte access SSIFTDR register*/
        volatile uint8_t * p_ssiftdr8 = (volatile uint8_t *) &(p_instance_ctrl->p_reg->SSIFTDR);
        uint8_t          * p_src8     = (uint8_t *) p_instance_ctrl->p_tx_src; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-8.13 */
        for (uint32_t i = 0; i < stages_to_write; i++)
        {
            *p_ssiftdr8 = *p_src8;
            p_src8++;
        }
    }
}

/*******************************************************************************************************************//**
 * Reads data from the receive FIFO based on the FIFO access size.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 * @param[in] stages_to_read           Number of times to read from the FIFO.
 **********************************************************************************************************************/
static void r_ssi_rx_fifo_read (ssi_instance_ctrl_t * p_instance_ctrl, uint32_t stages_to_read) /* PRQA S 3673 */
{
    if (TRANSFER_SIZE_4_BYTE == p_instance_ctrl->fifo_access_size)
    {
        uint32_t * p_dest32 = (uint32_t *) p_instance_ctrl->p_rx_dest;                          /* PRQA S 0316 # Advisory R-11.5 */
        for (uint32_t i = 0; i < stages_to_read; i++)
        {
            *p_dest32 = p_instance_ctrl->p_reg->SSIFRDR;
            p_dest32++;
        }
    }

    if (TRANSFER_SIZE_2_BYTE == p_instance_ctrl->fifo_access_size)
    {
        uint16_t * p_dest16 = (uint16_t *) p_instance_ctrl->p_rx_dest; /* PRQA S 0316 # Advisory R-11.5 */

        for (uint32_t i = 0; i < stages_to_read; i++)
        {
            *p_dest16 = (uint16_t) ((p_instance_ctrl->p_reg->SSIFRDR) >> SSI_PRV_SSIFRDR_READ_16_BIT_POS);
            p_dest16++;
        }
    }

    if (TRANSFER_SIZE_1_BYTE == p_instance_ctrl->fifo_access_size)
    {
        uint8_t * p_dest8 = (uint8_t *) p_instance_ctrl->p_rx_dest; /* PRQA S 0316 # Advisory R-11.5 */
        for (uint32_t i = 0; i < stages_to_read; i++)
        {
            *p_dest8 = (uint8_t) ((p_instance_ctrl->p_reg->SSIFRDR) >> SSI_PRV_SSIFRDR_READ_8_BIT_POS);
            p_dest8++;
        }
    }
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to I2S instance control block
 * @param[in]     event      Event code
 **********************************************************************************************************************/
static void r_ssi_call_callback (ssi_instance_ctrl_t * p_ctrl, i2s_event_t event) /* PRQA S 3673 */
{
    i2s_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    i2s_callback_args_t * p_args = p_ctrl->p_callback_memory;
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

    /* If the project is not Trustzone Secure, then it will never need to change security state in order to call the callback. */
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

/*******************************************************************************************************************//**
 * For each interrupt, if it is enabled, sets the interrupt priority based on user configuration and stores the control
 * block so it can be accessed in the ISR.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 * @param[in] p_cfg                    Pointer to the configuration structure.
 **********************************************************************************************************************/
static void r_ssi_interrupts_configure (ssi_instance_ctrl_t * const p_instance_ctrl, i2s_cfg_t const * const p_cfg)
{
    /* Set interrupt priority based on user configuration of interrupt number for FSP_SIGNAL_SSI_INT signal */
    R_BSP_IrqCfgEnable(p_cfg->int_irq, p_cfg->idle_err_ipl, p_instance_ctrl);

    /* Set interrupt priority based on user configuration of interrupt number for FSP_SIGNAL_SSI_RXI
     * or FSP_SIGNAL_SSI_TXI_RXI signals */
    if ((p_cfg->rxi_irq >= 0) && (NULL == p_cfg->p_transfer_rx)) /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        R_BSP_IrqCfgEnable(p_cfg->rxi_irq, p_cfg->rxi_ipl, p_instance_ctrl);
    }

    /* Set interrupt priority based on user configuration of interrupt number for FSP_SIGNAL_SSI_TXI
     * or FSP_SIGNAL_SSI_TXI_RXI signals */
    if ((p_cfg->txi_irq >= 0) && (NULL == p_cfg->p_transfer_tx)) /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        R_BSP_IrqCfgEnable(p_cfg->txi_irq, p_cfg->txi_ipl, p_instance_ctrl);
    }
}

#if SSI_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Validates that the transfer length is aligned to the SSI frame size for the configured communication format.
 *
 * For I2S mode, the transfer length must be aligned to the sample size multiplied by the number of channels.
 * For TDM mode, the transfer length must be aligned to the sample size multiplied by the number of slots in a frame.
 *
 * @param[in] p_instance_ctrl          Pointer to the control block.
 * @param[in] bytes                    Requested transfer length in bytes.
 *
 * @retval FSP_SUCCESS                 Transfer length is valid.
 **********************************************************************************************************************/
static fsp_err_t r_ssi_frame_alignment_check (ssi_instance_ctrl_t const * const p_instance_ctrl, uint32_t const bytes)
{
    FSP_ASSERT(bytes > 0U);
    ssi_extended_cfg_t * p_extend = (ssi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    if (SSI_COMMUNICATION_FORMAT_I2S == p_extend->communication_format)
    {
        FSP_ASSERT(0U == (bytes % ((1U << p_instance_ctrl->fifo_access_size) * 2U)));
    }
    else if (SSI_COMMUNICATION_FORMAT_TDM == p_extend->communication_format)
    {
        FSP_ASSERT(0U == (bytes % ((1U << p_instance_ctrl->fifo_access_size) *
                                   ((p_instance_ctrl->p_reg->SSICR_b.FRM + 1U) * 2U))));
    }
    else
    {
        /* Do nothing */
    }

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * Transmit ISR. Calls callback when transmission is complete.  Fills FIFO if transfer interface is not used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void ssi_txi_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type             irq             = R_FSP_CurrentIrqGet();
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear(irq);

    if (NULL != p_instance_ctrl->p_tx_src)
    {
        /* If transfer is not used, write data. */
        r_ssi_fifo_write(p_instance_ctrl);
    }

    /* If there are more samples to write to the FIFO or the FIFO is above the watermark, don't call the callback. */
    if ((p_instance_ctrl->tx_src_samples == 0U) &&
        (p_instance_ctrl->p_reg->SSIFSR_b.TDC > (BSP_FEATURE_SSI_FIFO_NUM_STAGES / 2U))) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        r_ssi_call_callback(p_instance_ctrl, I2S_EVENT_TX_EMPTY);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Receive ISR.  Calls callback when reception is complete.  Empties FIFO if transfer interface is not used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void ssi_rxi_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type             irq             = R_FSP_CurrentIrqGet();
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear(irq);

    bool call_callback = true;

    if (NULL != p_instance_ctrl->p_rx_dest)
    {
        /* If transfer is not used, read data into the destination buffer. */
        r_ssi_fifo_read(p_instance_ctrl);

        /* If there is more space in the buffer, don't call the callback. */
        if (p_instance_ctrl->rx_dest_samples > 0U)
        {
            call_callback = false;
        }
    }

    if (call_callback)
    {
        r_ssi_call_callback(p_instance_ctrl, I2S_EVENT_RX_FULL);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Error and idle ISR.
 *
 * Handles transmit underflow errors and calls callback when idle interrupt occurs.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void ssi_int_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type             irq             = R_FSP_CurrentIrqGet();
    ssi_instance_ctrl_t * p_instance_ctrl = (ssi_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Events that are being serviced are being cleared. During this time if another error occurs then this
     * interrupt will fire again */

    /* Clear all flags in SSISR. These bits can only be cleared after reading them as 1. */
    uint32_t iirq_flag = p_instance_ctrl->p_reg->SSISR_b.IIRQ; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg->SSISR = 0U;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (1U == iirq_flag)
    {
        /* Disable idle interrupt. */
        p_instance_ctrl->p_reg->SSICR_b.IIEN = 0U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* If peripheral is idle, call idle callback. */
        r_ssi_call_callback(p_instance_ctrl, I2S_EVENT_IDLE);
    }
    else
    {
        if (NULL != p_instance_ctrl->p_rx_dest)
        {
            /* If there's more data to read, flush read data into the destination buffer. */
            r_ssi_fifo_read(p_instance_ctrl);
        }

        /* SSIE must go idle after a transmit or receive error.*/
        (void) r_ssi_stop_sub(p_instance_ctrl);
    }

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear(irq);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

#if SSI_CFG_TRANSFER_ENABLE

/*******************************************************************************************************************//**
 * Configures any dependent drivers selected by the user, including transfer and timer drivers.
 *
 * @param[in] p_reg                    Pointer to SSIF base register address for this channel.
 * @param[in] p_cfg                    Pointer to the configuration structure.
 *
 * @retval FSP_SUCCESS                 Dependent drivers configured successfully.
 * @return                             See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                     possible return codes. This function calls:
 *                                         * @ref transfer_api_t::open
 **********************************************************************************************************************/

static fsp_err_t r_ssi_dependent_drivers_configure (i2s_cfg_t const * const     p_cfg,
                                                    ssi_instance_ctrl_t * const p_instance_ctrl)
{
/* Prepare transfer configuration. */
    fsp_err_t err_transfer_tx = FSP_SUCCESS;
    fsp_err_t err_transfer_rx = FSP_SUCCESS;

/* If a transfer instance is provided for write, open the transfer instance. */
    if (NULL != p_cfg->p_transfer_tx)
    {
        err_transfer_tx = p_cfg->p_transfer_tx->p_api->open(p_cfg->p_transfer_tx->p_ctrl, p_cfg->p_transfer_tx->p_cfg);
        FSP_ERROR_RETURN((FSP_SUCCESS == err_transfer_tx), err_transfer_tx);
        err_transfer_tx =
            p_cfg->p_transfer_tx->p_api->callbackSet(p_cfg->p_transfer_tx->p_ctrl,
                                                     ssi_txi_transfer_callback, /* PRQA S 3635 */
                                                     p_instance_ctrl,
                                                     NULL);

        FSP_ERROR_RETURN((FSP_SUCCESS == err_transfer_tx), err_transfer_tx);
    }

/* If a transfer instance is provided for read, open the transfer instance. */
    if (NULL != p_cfg->p_transfer_rx)
    {
        err_transfer_rx = p_cfg->p_transfer_rx->p_api->open(p_cfg->p_transfer_rx->p_ctrl, p_cfg->p_transfer_rx->p_cfg);
        FSP_ERROR_RETURN((FSP_SUCCESS == err_transfer_rx), err_transfer_rx);
        err_transfer_rx =
            p_cfg->p_transfer_rx->p_api->callbackSet(p_cfg->p_transfer_rx->p_ctrl,
                                                     ssi_rxi_transfer_callback, /* PRQA S 3635 */
                                                     p_instance_ctrl,
                                                     NULL);

        FSP_ERROR_RETURN((FSP_SUCCESS == err_transfer_rx), err_transfer_rx);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * SSIF callback function from TransferTx
 **********************************************************************************************************************/

void ssi_txi_transfer_callback (transfer_callback_args_t * p_transfer_args)                         /* PRQA S 3673, 1505 # Advisory R-8.7 */
{
    ssi_instance_ctrl_t * p_ctrl   = (ssi_instance_ctrl_t *) p_transfer_args->p_context;            /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    ssi_extended_cfg_t  * p_extend = (ssi_extended_cfg_t *) p_ctrl->p_cfg->p_extend;                /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-8.13 */

/* Check transfer data event is complete */
    if (p_transfer_args->event & (TRANSFER_EVENT_TRANS_END_FLAG | TRANSFER_EVENT_TRANS_COMPLETION)) /* PRQA S 3344, 4522, 4532, 4542 # DP_MSR_R-14.4_UC1, DP_MSR_R-10.1_UC1 */
    {
        /* Check if tx sample data is not equal 0 to set up the next transfer data */
        if (0U < p_ctrl->tx_src_samples)
        {
            transfer_instance_t const * const p_transfer = p_ctrl->p_cfg->p_transfer_tx;
            uint16_t transfer_block = (uint16_t) p_extend->transmit_data_empty + 1U;
            if (transfer_block > p_ctrl->tx_src_samples)
            {
                transfer_block = (uint16_t) p_ctrl->tx_src_samples;
            }

            /* Update the setting as the source address and number of transfer to config transfer module */
            p_transfer->p_cfg->p_info->p_src = p_ctrl->p_tx_src;

            /* DTS Support */
            if (p_transfer->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_mode == /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
                TRANSFER_DTSC_MODE_BLOCK_1)
            {
                p_transfer->p_cfg->p_info->number_transfer =
                    (uint32_t) (transfer_block);
            }
            /* DMA Support */
            else if (p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.transfer_mode == TRANSFER_MODE_NORMAL) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            {
                p_transfer->p_cfg->p_info->number_transfer =
                    (uint32_t) (transfer_block << p_ctrl->fifo_access_size);                                             /* PRQA S 4391, 4524 # Advisory R-10.6 */
            }
            else
            {
                /*Do nothing*/
            }

            p_transfer->p_api->reconfigure(p_transfer->p_ctrl, p_transfer->p_cfg->p_info);
            p_transfer->p_api->enable(p_transfer->p_ctrl);
            p_ctrl->tx_src_samples -= transfer_block;

            /* Update the next source address for the next transfer */
            p_ctrl->p_tx_src = (void *) ((uint8_t *) (p_ctrl->p_tx_src) +                                    /* PRQA S 0311, 0316, 0488 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-18.4 */
                                         ((0x01U << (uint32_t) p_ctrl->fifo_access_size) * transfer_block)); /* PRQA S 1891 # Advisory R-10.6 */
        }
        else
        {
            p_ctrl->p_tx_src       = NULL;
            p_ctrl->tx_src_samples = 0U;
            r_ssi_call_callback(p_ctrl, I2S_EVENT_TX_EMPTY);
        }
    }
}

/*******************************************************************************************************************//**
 * SSIF callback function from TransferRx
 **********************************************************************************************************************/

void ssi_rxi_transfer_callback (transfer_callback_args_t * p_transfer_args)                         /* PRQA S 3673, 1505 # Advisory R-8.7 */
{
    ssi_instance_ctrl_t      * p_ctrl   = (ssi_instance_ctrl_t *) p_transfer_args->p_context;       /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    ssi_extended_cfg_t const * p_extend = (ssi_extended_cfg_t const *) p_ctrl->p_cfg->p_extend;     /* PRQA S 0316 # Advisory R-11.5 */

/* Check transfer data event is complete */
    if (p_transfer_args->event & (TRANSFER_EVENT_TRANS_END_FLAG | TRANSFER_EVENT_TRANS_COMPLETION)) /* PRQA S 3344, 4522, 4532, 4542 # DP_MSR_R-14.4_UC1, DP_MSR_R-10.1_UC1 */
    {
        /* Check if rx sample data is not equal 0 to set up the next transfer data.
         * Remaining samples are smaller than the DMA block size.
         * The tail data will be drained by ssi_int_isr(). */
        if (0U < p_ctrl->rx_dest_samples)
        {
            transfer_instance_t const * const p_transfer = p_ctrl->p_cfg->p_transfer_rx;
            uint16_t transfer_block = (uint16_t) p_extend->receive_data_full + 1U;
            if (transfer_block <= p_ctrl->rx_dest_samples)
            {
                /* Update the setting as the dest address and number of transfer to config transfer module */
                p_transfer->p_cfg->p_info->p_dest = p_ctrl->p_rx_dest;

                /* DTS Support */
                if (p_transfer->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_mode == /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
                    TRANSFER_DTSC_MODE_BLOCK_1)
                {
                    p_transfer->p_cfg->p_info->number_transfer =
                        (uint32_t) (transfer_block);
                }
                /* DMA Support */
                else if (p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.transfer_mode == /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
                         TRANSFER_MODE_NORMAL)
                {
                    p_transfer->p_cfg->p_info->number_transfer =
                        (uint32_t) (transfer_block << p_ctrl->fifo_access_size);                       /* PRQA S 4391, 4524 # Advisory R-10.6 */
                }
                else
                {
                    /*Do nothing*/
                }

                p_transfer->p_api->reconfigure(p_transfer->p_ctrl, p_transfer->p_cfg->p_info);
                p_transfer->p_api->enable(p_transfer->p_ctrl);
                p_ctrl->rx_dest_samples -= transfer_block;

                /* Update the next destination address for the next transfer */
                p_ctrl->p_rx_dest = (void *) ((uint8_t *) (p_ctrl->p_rx_dest) +                        /* PRQA S 0311, 0316, 0488 # DP_MSR_R-11.8_UC1 , Advisory R-11.5, Advisory R-18.4 */
                                              ((0x01U << p_ctrl->fifo_access_size) * transfer_block)); /* PRQA S 1891, 4524 # Advisory R-10.6 */
            }
            else
            {
                /*Do nothing*/
            }
        }
        else
        {
            p_ctrl->p_rx_dest       = NULL;
            p_ctrl->rx_dest_samples = 0U;
            r_ssi_call_callback(p_ctrl, I2S_EVENT_RX_FULL);
        }
    }
}

#endif

/* End of file R_SSI. */
