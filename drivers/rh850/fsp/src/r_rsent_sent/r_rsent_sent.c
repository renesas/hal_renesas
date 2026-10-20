/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_rsent_sent.h"              /* PRQA S 0380 # DP_MSR_D-1.1_C99 & DP_MSR_D-1.1_C11 */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Macros used to request RSENT change mode in MDC register */
#define RSENT_SENT_MDC_OMC_RESET                           (0x0U)
#define RSENT_SENT_MDC_OMC_CONFIGURATION                   (0x1U)
#define RSENT_SENT_MDC_OMC_OPERATION_IDLE                  (0x3U)
#define RSENT_SENT_MDC_OMC_OPERATION_ACTIVE                (0x5U)

/* Macros used to check RSENT current mode in MST register */
#define RSENT_SENT_MST_OMC_RESET                           (0x0U)
#define RSENT_SENT_MST_OMC_CONFIGURATION                   (0x1U)

/* Short serial message data and id format */
#define RSENT_SENT_SRXD_SHORT_SERIAL_MSG_DATA_MASK         (0x000FFU)
#define RSENT_SENT_SRXD_SHORT_SERIAL_MSG_ID_MASK           (0x00F00U)
#define RSENT_SENT_SRXD_SHORT_SERIAL_MSG_ID_POS            (8U)

/* Enhanced serial message data and id format with configuration bit is 0 */
#define RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_0_DATA_MASK    (0x00FFFU)
#define RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_0_ID_MASK      (0xFF000U)
#define RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_0_ID_POS       (12U)

/* Enhanced serial message data and id format with configuration bit is 1 */
#define RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_1_DATA_MASK    (0x0FFFFU)
#define RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_1_ID_MASK      (0xF0000U)
#define RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_1_ID_POS       (16U)

/* Write the value of this macro to CSC register to clear all communication status flag pending */
#define RSENT_SENT_CSC_ALL_ERROR_STATUS_CLEAR              (0x7FEU)

/* This mask is used to get the RSENT master selection bits (5 bits) */
#define RSENT_SENT_SELB_RSENT0_MASTER_SELECTION_MASK       (0x1FU)

/* Number of available RSENT channels */
#define RSENT_SENT_NUM_OF_UNIT                             (BSP_FEATURE_RSENT_SENT_NUM_OF_UNIT)

/** "RSEN" in ASCII, used to determine if channel is open. */
#define RSENT_SENT_OPEN                                    (0x5253454EU)

/** RSENT channel field: offset per channel and bit mask (2 bits per 4-bit field). */
#define RSENT_CHANNEL_OFFSET_BITS                          (4U)   /* PRQA S 1534 # Advisory R-2.5 */
#define RSENT_CHANNEL_BIT_MASK                             (0x3U) /* PRQA S 1534 # Advisory R-2.5 */

#define RSENT_PAIR_STRIDE                                  (R_RSENT2_BASE - R_RSENT0_BASE)
#define RSENT_GROUP_GAP                                    (R_RSENT1_BASE - R_RSENT0_BASE)

/* Set/Reset the RSENT master selection bit for this channel */
#define RSENT_SENT_SET_TIMESTAMP                           (0U)
#define RSENT_SENT_RESET_TIMESTAMP                         (1U)

/* RSENT Timestamp Select Register*/
#define RSENT_SENT_TSSEL1                                  (2U)
#define RSENT_SENT_TSSEL2                                  (3U)
#define RSENT_SENT_TSSEL3                                  (4U)
#define RSENT_SENT_TSSEL4                                  (5U)
#define RSENT_SENT_TSSEL5                                  (6U)
#define RSENT_SENT_TSSEL6                                  (7U)
#define RSENT_SENT_TSSEL7                                  (8U)

#define RSENT_SENT_SPC_PULSE_LENGTH_MAX                    (128U)

#if (1 == RSENT_SENT_TRANSFER_SUPPORT_ENABLE)

/* Reset value used to clear fast/slow channel message receive state after completion */
 #define RSENT_SENT_MSG_RX_STATE_RESET                     (0U)

/* Max number of messages supported by a transfer backed FastMsgRead or SlowMsgRead call */
 #define RSENT_SENT_TRANSFER_MSG_STAGING_MAX_COUNT         (64U)

/* Number of SENT frames carrying one slow channel message */
 #define RSENT_SENT_SHORT_SERIAL_MSG_FRAMES                (CONFIG_SENT_RENESAS_RH850_SERIAL_SHORT_FRAME)
 #define RSENT_SENT_ENHANCED_SERIAL_MSG_FRAMES             (CONFIG_SENT_RENESAS_RH850_SERIAL_ENHANCED_FRAME)
#endif

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void r_rsent_sent_call_callback(rsent_sent_instance_ctrl_t * p_instance_ctrl, sent_event_t event);

static void r_rsent_sent_msg_read(rsent_sent_instance_ctrl_t * p_instance_ctrl);

static void r_rsent_sent_error_handle(rsent_sent_instance_ctrl_t * p_instance_ctrl);

/* Functions for SPC timeout using timer interface */
static fsp_err_t r_rsent_sent_spc_timer_open(timer_instance_t const     * p_spc_timer,
                                             rsent_sent_instance_ctrl_t * p_instance_ctrl);
static fsp_err_t r_rsent_sent_spc_timer_close(timer_instance_t const * p_spc_timer);
static void      r_rsent_sent_spc_timer_callback(timer_callback_args_t * p_callback_args);
static fsp_err_t r_rsent_sent_transfer_config(rsent_sent_instance_ctrl_t * p_instance_ctrl,
                                              sent_cfg_t const * const     p_cfg);

#if (RSENT_SENT_TRANSFER_SUPPORT_ENABLE)
static void r_rsent_sent_transfer_callback(transfer_callback_args_t * p_args);

#endif

static fsp_err_t r_rsent_set_timestamp_master(sent_ctrl_t * const p_ctrl, sent_cfg_t const * const p_cfg,
                                              uint8_t status);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

/* PRQA S 3449, 3451 ++ # DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void rsent_sent_rxi_isr(void);
BSP_INTERRUPT_ATTRIBUTE void rsent_sent_eri_isr(void);

/* PRQA S 3449, 3451 -- */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

#if (1 == RSENT_SENT_TRANSFER_SUPPORT_ENABLE)

/* Flat staging buffers used by transfer backed fast and slow channel reads */
static uint32_t g_rsent_sent_fast_msg_transfer_staging[RSENT_SENT_TRANSFER_MSG_STAGING_MAX_COUNT];
static uint32_t g_rsent_sent_slow_msg_transfer_staging[RSENT_SENT_TRANSFER_MSG_STAGING_MAX_COUNT];

#endif

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
const sent_api_t g_sent_on_rsent =           /* PRQA S 1502 # Advisory R-2.8 */
{
    .open        = R_RSENT_SENT_Open,        /* PRQA S 3635 # Advisory R-17.12 */
    .close       = R_RSENT_SENT_Close,       /* PRQA S 3635 # Advisory R-17.12 */
    .start       = R_RSENT_SENT_Start,       /* PRQA S 3635 # Advisory R-17.12 */
    .stop        = R_RSENT_SENT_Stop,        /* PRQA S 3635 # Advisory R-17.12 */
    .fastRead    = R_RSENT_SENT_FastMsgRead, /* PRQA S 3635 # Advisory R-17.12 */
    .slowRead    = R_RSENT_SENT_SlowMsgRead, /* PRQA S 3635 # Advisory R-17.12 */
    .spcWrite    = R_RSENT_SENT_SPCWrite,    /* PRQA S 3635 # Advisory R-17.12 */
    .abort       = R_RSENT_SENT_Abort,       /* PRQA S 3635 # Advisory R-17.12 */
    .callbackSet = R_RSENT_SENT_CallbackSet, /* PRQA S 3635 # Advisory R-17.12 */
    .spcSelect   = R_RSENT_SENT_Select_SPCO, /* PRQA S 3635 # Advisory R-17.12 */
    .statusGet   = R_RSENT_SENT_StatusGet,   /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup RSENT_SENT
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configures the SENT driver based on the input configurations. Implements @ref sent_api_t::open
 *
 * @param[in] p_ctrl                         Pointer to control block.
 * @param[in] p_cfg                          Pointer to configuration structure.
 * @retval    FSP_SUCCESS                    Unit opened successfully.
 * @retval    FSP_ERR_ASSERTION              Pointer to SENT control block or configuration structure or
 *                                           extend configuration structure is NULL or channel is not exist or
 *                                           receive interrupt irq < 0 or error interrupt irq < 0.
 * @retval    FSP_ERR_ALREADY_OPEN           Control block has already been opened or channel is being used by another
 *                                           instance. Call close() then open() to reconfigure.
 *
 * @return                     See @ref RENESAS_ERROR_CODES or functions called by this function for other possible
 *                               return codes.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_Open (sent_ctrl_t * const p_ctrl, sent_cfg_t const * const p_cfg)  /* PRQA S 1505 # Advisory R-8.7 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(p_cfg);                                                                                       /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    FSP_ASSERT(p_cfg->p_extend);                                                                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Verify this channel has not already been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);                        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check channel exist */
    FSP_ASSERT(p_cfg->channel < RSENT_SENT_NUM_OF_UNIT);                                                     /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif

    p_instance_ctrl->p_cfg = p_cfg;
    const rsent_sent_extended_cfg_t * p_extend = (const rsent_sent_extended_cfg_t *) p_cfg->p_extend;        /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t channel = (uint32_t) p_cfg->channel;
    p_instance_ctrl->p_reg = (R_RSENT0_Type *) (R_RSENT0_BASE + (uint32_t) RSENT_GROUP_GAP * (channel % 2) + /* PRQA S 1840, 0306, 3395 # DP_MSR_R-10.4_UC1, Advisory R-11.4, Advisory R-12.1 */
                                                (uint32_t) RSENT_PAIR_STRIDE * (channel / 2));               /* PRQA S 1840 # DP_MSR_R-10.4_UC1 */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_callback_memory = NULL;
    p_instance_ctrl->p_context         = p_cfg->p_context;

    p_instance_ctrl->fast_msg_rx_count  = 0;
    p_instance_ctrl->fast_msg_rx_number = 0;
    p_instance_ctrl->slow_msg_rx_count  = 0;
    p_instance_ctrl->slow_msg_rx_number = 0;

    uint32_t tmp_reg_cc = 0;

    /* Change mode to CONFIGURATION */
    p_instance_ctrl->p_reg->MDC = RSENT_SENT_MDC_OMC_CONFIGURATION;                            /* PRQA S 0662 # Required R-1.1 */

    /* Wait for mode to change to CONFIGURATION */
    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->MST, RSENT_SENT_MST_OMC_CONFIGURATION); /* PRQA S 0662 # Required R-1.1 */

    /* Communication clock configuration */
    p_instance_ctrl->p_reg->BRP = p_extend->com_clk_cfg.all;                                   /* PRQA S 0662 # Required R-1.1 */

    /* Timestamp configuration */
    p_instance_ctrl->p_reg->TSPC = p_extend->timestamp_cfg.all;                                /* PRQA S 0662 # Required R-1.1 */
    p_instance_ctrl->p_reg->TSC  = p_extend->timestamp_init_value;                             /* PRQA S 0662 # Required R-1.1 */

    /* Configure timestamp master selection */
    fsp_err_t timestamp_err = r_rsent_set_timestamp_master(p_instance_ctrl,
                                                           p_instance_ctrl->p_cfg,
                                                           RSENT_SENT_SET_TIMESTAMP);
    FSP_ERROR_RETURN(FSP_SUCCESS == timestamp_err, timestamp_err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Communication configuration */
    /* Number of data nibbles */
    tmp_reg_cc |= ((uint32_t) p_cfg->num_of_data_nibbles << R_RSENT0_CC_NDN_Pos) & R_RSENT0_CC_NDN_Msk;

    /* Pause pulse configuration */
    tmp_reg_cc |= ((uint32_t) p_cfg->pause_pulse_cfg << R_RSENT0_CC_PPC_Pos) & (R_RSENT0_CC_PPC_Msk |
                                                                                R_RSENT0_CC_PPTC_Msk);

    /* Serial message format */
    tmp_reg_cc |= ((uint32_t) p_cfg->serial_message_format << R_RSENT0_CC_SMF_Pos) & R_RSENT0_CC_SMF_Msk;

    /* Data nibble CRC format */
    tmp_reg_cc |= ((uint32_t) p_cfg->data_nibble_crc_format << R_RSENT0_CC_DCF_Pos) & R_RSENT0_CC_DCF_Msk;

    /* Fast channel CRC check */
    tmp_reg_cc |= ((uint32_t) p_cfg->fast_channel_crc_check << R_RSENT0_CC_FCCD_Pos) & R_RSENT0_CC_FCCD_Msk;

    /* Slow channel CRC check */
    tmp_reg_cc |= ((uint32_t) p_cfg->slow_channel_crc_check << R_RSENT0_CC_SCCD_Pos) & R_RSENT0_CC_SCCD_Msk;

    /* Frame check method */
    tmp_reg_cc |= ((uint32_t) p_cfg->frame_check_method << R_RSENT0_CC_FCM_Pos) & R_RSENT0_CC_FCM_Msk;

    /* SPC output polarity */
    tmp_reg_cc |= ((uint32_t) p_cfg->spc_output_polarity << R_RSENT0_CC_SOPC_Pos) & R_RSENT0_CC_SOPC_Msk;

    /* SPC function configuration */
    tmp_reg_cc |= ((uint32_t) p_cfg->spc_cfg << R_RSENT0_CC_SPCE_Pos) & R_RSENT0_CC_SPCE_Msk;

    p_instance_ctrl->p_reg->CC = tmp_reg_cc; /* PRQA S 0662 # Required R-1.1 */

    /* Enable fast channel data receive interrupt */
    if (p_cfg->rxi_irq >= 0)                 /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        R_BSP_IrqCfgEnable(p_cfg->rxi_irq, p_cfg->rxi_ipl, p_instance_ctrl);
    }

    /* Enable receive error interrupts */
    if (p_cfg->eri_irq >= 0)           /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        R_BSP_IrqCfgEnable(p_cfg->eri_irq, p_cfg->eri_ipl, p_instance_ctrl);
    }

    /* Open SPC timeout timer if it is available */
    if (NULL != p_cfg->p_spc_timer)
    {
        fsp_err_t timer_err = r_rsent_sent_spc_timer_open(p_cfg->p_spc_timer, p_instance_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == timer_err, timer_err);   /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    }

    fsp_err_t transfer_err = r_rsent_sent_transfer_config(p_instance_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == transfer_err, transfer_err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    p_instance_ctrl->open = RSENT_SENT_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Reset RSENT module. Disables receive and communication status interrupts.
 * Implements @ref sent_api_t::close
 *
 * @param[in] p_ctrl                   Pointer to control block.
 * @retval    FSP_SUCCESS              Unit successfully closed.
 * @retval    FSP_ERR_ASSERTION        Pointer to control block is NULL.
 * @retval    FSP_ERR_NOT_OPEN         The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_Close (sent_ctrl_t * const p_ctrl)                                 /* PRQA S 1505 # Advisory R-8.7 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check if control block is NULL. */
    FSP_ASSERT(p_instance_ctrl);                                                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check this channel has not been opened */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif

    /* Close SPC timeout timer if it is available */
    if (NULL != p_instance_ctrl->p_cfg->p_spc_timer)
    {
        fsp_err_t timer_err = r_rsent_sent_spc_timer_close(p_instance_ctrl->p_cfg->p_spc_timer);
        FSP_ERROR_RETURN(FSP_SUCCESS == timer_err, timer_err);                         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    }

    /* Request to change operation mode to RESET mode */
    p_instance_ctrl->p_reg->MDC = RSENT_SENT_MDC_OMC_RESET;                            /* PRQA S 0662 # Required R-1.1 */

    /* Wait for mode to change to RESET */
    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->MST, RSENT_SENT_MST_OMC_RESET); /* PRQA S 0662 # Required R-1.1 */

    /* Disable fast channel data receive interrupt */
    if (p_instance_ctrl->p_cfg->rxi_irq >= 0)                                          /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->rxi_irq);
    }

    /* Disable receive error interrupts */
    if (p_instance_ctrl->p_cfg->eri_irq >= 0) /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->eri_irq);
    }

    /* Reset the RSENT master selection bit for this channel in RSENTTSSELn registers */
    fsp_err_t timestamp_err = r_rsent_set_timestamp_master(p_instance_ctrl,
                                                           p_instance_ctrl->p_cfg,
                                                           RSENT_SENT_RESET_TIMESTAMP);
    FSP_ERROR_RETURN(FSP_SUCCESS == timestamp_err, timestamp_err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

#if (RSENT_SENT_TRANSFER_SUPPORT_ENABLE)
    if (NULL != p_instance_ctrl->p_cfg->p_transfer_rx)
    {
        /* Reset is skipped because it enables the transfer channel again with zero transfers and raises an extra interrupt */
        p_instance_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_instance_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        p_instance_ctrl->p_cfg->p_transfer_rx->p_api->close(p_instance_ctrl->p_cfg->p_transfer_rx->p_ctrl);
    }
#endif

    /* Drop any pending request so it cannot block the next open */
    p_instance_ctrl->fast_msg_rx_number = 0U;
    p_instance_ctrl->fast_msg_rx_count  = 0U;
    p_instance_ctrl->p_fast_msg_buffer  = NULL;
    p_instance_ctrl->slow_msg_rx_number = 0U;
    p_instance_ctrl->slow_msg_rx_count  = 0U;
    p_instance_ctrl->p_slow_msg_buffer  = NULL;

    /* Mark the channel not open so other APIs cannot use it. */
    p_instance_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function enable fast channel message receive interrupt, communication errors interrupt and change RSENT
 * operation mode to OPERATION ACTIVE mode. User calls this function to start receiving incomming messages.
 * If receive buffer is specifed by R_RSENT_SENT_FastMsgRead or R_RSENT_SENT_SlowMsgRead, the received message is written
 * into the specifed receive buffer in receive interrupt service routine.
 * When an error happened during receive process, the user callback (if is is not NULL) is called in communication errors
 * interrupt service routine with event code corresponding to the error. The user application is then decide to continue
 * or abort (by calling to R_RSENT_SENT_Abort API) the receiving process.
 * In order stop capturing incomming messages, user need to call to R_RSENT_SENT_Stop API.
 * Implements @ref sent_api_t::start
 *
 * @param[in] p_ctrl                 Pointer to control block.
 * @retval    FSP_SUCCESS            Start request is successful.
 * @retval    FSP_ERR_ASSERTION      Pointer to control block is NULL.
 * @retval    FSP_ERR_NOT_OPEN       The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_Start (sent_ctrl_t * const p_ctrl)                                                         /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl;                         /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                                                  /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);                                 /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif
    rsent_sent_extended_cfg_t * p_extend = (rsent_sent_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;        /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    /* Clear receive error interrupt flags by set bits in CSC registers */
    p_instance_ctrl->p_reg->CSC = RSENT_SENT_CSC_ALL_ERROR_STATUS_CLEAR;                                          /* PRQA S 0662 # Required R-1.1 */

    /* Clear fast channel message receive interrupt if it set */
    volatile uint32_t dummy_read = 0;
    while (1U == p_instance_ctrl->p_reg->CS_b.FRS)                                                                /* PRQA S 0662 # Required R-1.1 */
    {
        /* Dummy read FRXD register to clear fast channel message receive interrupt */
        dummy_read = p_instance_ctrl->p_reg->FRXD;                                                                /* PRQA S 0662 # Required R-1.1 */
    }

    /* Read slow channel message data register to make sure it is cleared */
    dummy_read = p_instance_ctrl->p_reg->SRXD;                                                                    /* PRQA S 0662 # Required R-1.1 */
    FSP_PARAMETER_NOT_USED(dummy_read);

    /* Receive error interrupts */
    uint32_t tmp_reg_ide = ((rsent_sent_extended_cfg_t *) (p_instance_ctrl->p_cfg->p_extend))->error_irq_cfg.all; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1, Advisory R-11.5 */

    /* Fast message receive interrupt */
    tmp_reg_ide |= 1U << R_RSENT0_IDE_FRIE_Pos;

    /* Set bits in IDE register to enable interrupts */
    p_instance_ctrl->p_reg->IDE = tmp_reg_ide; /* PRQA S 0662 # Required R-1.1 */

    /* Update the timestamp register at start when timestamp synchronization on start is enabled. */
    if (RSENT_TIMESTAMP_SYNCHRONIZATION_ON_START_ENABLE == p_extend->timestamp_synchronization_on_start)
    {
        p_instance_ctrl->p_reg->TSC = p_extend->timestamp_start_value; /* PRQA S 0662 # Required R-1.1 */
    }

    /* Request to change operation mode to OPERATION ACTIVE mode */
    p_instance_ctrl->p_reg->MDC = RSENT_SENT_MDC_OMC_OPERATION_ACTIVE; /* PRQA S 0662 # Required R-1.1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function change RSENT operation mode to OPERATION IDLE mode (stop capturing new message), disable receive
 * interrupt and communication errors interrupt and clear the receive buffer.
 * Implements @ref sent_api_t::stop
 *
 * @param[in] p_ctrl                 Pointer to control block.
 * @retval    FSP_SUCCESS            Stop request is successful.
 * @retval    FSP_ERR_ASSERTION      Pointer to control block is NULL.
 * @retval    FSP_ERR_NOT_OPEN       The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_Stop (sent_ctrl_t * const p_ctrl)                                  /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif

    /* Request to change operation mode to OPERATION IDLE mode */
    p_instance_ctrl->p_reg->MDC = RSENT_SENT_MDC_OMC_OPERATION_IDLE;                      /* PRQA S 0662 # Required R-1.1 */

    /* Disable receive related interrupts */
    uint32_t tmp_reg = 0U;

    /* Fast message receive interrupt */
    tmp_reg |= 1U << R_RSENT0_IDE_FRIE_Pos;

    /* Receive error interrupts */
    tmp_reg |= ((rsent_sent_extended_cfg_t *) (p_instance_ctrl->p_cfg->p_extend))->error_irq_cfg.all; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1, Advisory R-11.5 */

    /* Clear bits in IDE register to disable interrupts */
    p_instance_ctrl->p_reg->IDE &= ~tmp_reg;                                                          /* PRQA S 0662 # Required R-1.1 */

    /* Clear receive error interrupt flags by set bits in CSC registers */
    p_instance_ctrl->p_reg->CSC = RSENT_SENT_CSC_ALL_ERROR_STATUS_CLEAR;                              /* PRQA S 0662 # Required R-1.1 */

    /* Clear fast channel message receive interrupt if it is set */
    volatile uint32_t dummy_read = 0;
    while (1U == p_instance_ctrl->p_reg->CS_b.FRS)                                                    /* PRQA S 0662 # Required R-1.1 */
    {
        /* Dummy read FRXD register to clear fast channel message receive interrupt */
        dummy_read = p_instance_ctrl->p_reg->FRXD;                                                    /* PRQA S 0662 # Required R-1.1 */
    }

    FSP_PARAMETER_NOT_USED(dummy_read);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function specifies the number of fast channel message to read and destination to store the read messages.
 * When RSENT is in OPERATION ACTIVE mode, fast channel new data receive interrupt is enabled (after Start API is called)
 * and a new fast channel message is received successfully, this message is written to the destination buffer in receive
 * interrupt service routine. After received the specified number of fast channel messages, the user specified callback
 * function is called (if it is not NULL) with SENT_EVENT_FAST_CHANNEL_RX_COMPLETE event code.
 * Implements @ref sent_api_t::fastRead.
 *
 * @param[in]   p_ctrl                    Pointer to control block
 * @param[in]   p_dest                    Pointer to the destination buffer
 * @param[in]   message_count             Number of fast channel message to read
 * @retval      FSP_SUCCESS               Read request is issued successfully.
 * @retval      FSP_ERR_ASSERTION         p_ctrl, p_dest is NULL or message_count is 0.
 * @retval      FSP_ERR_NOT_OPEN          The RSENT channel specified in p_ctrl is not opened.
 * @retval      FSP_ERR_IN_USE            The fast channel message receive is on-going, or (transfer instance is used)
 *                                        the slow channel message receive is on-going. Wait for it to complete
 *                                        or abort it before create a new read request.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_FastMsgRead (sent_ctrl_t * const p_ctrl, void * p_dest, uint32_t const message_count) /* PRQA S 1505 # Advisory R-8.7 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl;                    /* PRQA S 0316 # Advisory R-11.5 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(p_dest);                                                                                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(0U != message_count);                                                                         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);                            /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif
    if (0U == p_instance_ctrl->fast_msg_rx_number)
    {
#if (RSENT_SENT_TRANSFER_SUPPORT_ENABLE)

        /* Pointer to the receive transfer instance configuration */
        const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;

        if (NULL != p_transfer_rx)
        {
            /* One transfer channel serves one request at a time */
            FSP_ERROR_RETURN(0U == p_instance_ctrl->slow_msg_rx_number, FSP_ERR_IN_USE); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            /* Pointer to the transfer information structure */
            transfer_info_t * p_info = p_transfer_rx->p_cfg->p_info;

            /* Reject reads larger than the fast message staging buffer */
            FSP_ERROR_RETURN(RSENT_SENT_TRANSFER_MSG_STAGING_MAX_COUNT >= message_count, FSP_ERR_INVALID_SIZE); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            /* Set the destination address for the transfer to the flat staging buffer */
            p_transfer_rx->p_cfg->p_info->p_dest = (uint32_t *) &g_rsent_sent_fast_msg_transfer_staging[0];

            /* Force a 4 byte transfer unit to match the staging buffer element size */
            if ((uint32_t) TRANSFER_ADDR_MODE_INCREMENTED == p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode) /* PRQA S 1881, 1823 # DP_MSR_R-10.4_UC1 */
            {
                p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size = TRANSFER_SIZE_4_BYTE;                      /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size = TRANSFER_SIZE_4_BYTE;                      /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
            }

            if ((uint32_t) TRANSFER_DTSC_ADDR_MODE_FIXED ==                                                           /* PRQA S 1823 # DP_MSR_R-10.4_UC1 */
                p_info->transfer_control_cfg.transfer_control_b.src_addr_direction)                                   /* PRQA S 1881, 1823 # DP_MSR_R-10.4_UC1 */
            {
                p_info->transfer_control_cfg.transfer_control_b.transfer_data_size = TRANSFER_SIZE_4_BYTE;            /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
            }

            /* Set the number of messages to transfer */
            p_info->number_transfer = (uint16_t) message_count;

            /* Reconfigure the transfer with updated settings */
            fsp_err_t err = p_transfer_rx->p_api->reconfigure(p_transfer_rx->p_ctrl, p_info);

            /* Return error code if reconfiguration fails */
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            /* Enable transfer using the Enable API */
            err = p_transfer_rx->p_api->enable(p_transfer_rx->p_ctrl);

            /* Return error code if reconfiguration fails */
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        }
#endif

        /* Save fast channel message receive buffer information to control block */
        p_instance_ctrl->p_fast_msg_buffer  = p_dest;
        p_instance_ctrl->fast_msg_rx_count  = 0;
        p_instance_ctrl->fast_msg_rx_number = message_count;
    }
    else
    {

        /* Fast channel message receive is on-going, return error */
        return FSP_ERR_IN_USE;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function specifies the number of slow channel message to read and destination to store the read messages.
 * When RSENT is in OPERATION ACTIVE mode, fast channel new data receive interrupt is enabled (after Start API is called)
 * and a new slow channel message is received successfully, this message is written to the destination buffer in receive
 * interrupt service routine. After received the specified number of slow channel messages, the user specified callback
 * function is called (if it is not NULL) with SENT_EVENT_SLOW_CHANNEL_RX_COMPLETE event code.
 * Implements @ref sent_api_t::slowRead.
 *
 * @param[in]   p_ctrl                    Pointer to control block
 * @param[in]   p_dest                    Pointer to the destination buffer
 * @param[in]   message_count             Number of slow channel message to read
 * @retval      FSP_SUCCESS               Read request is issued successfully.
 * @retval      FSP_ERR_ASSERTION         p_ctrl, p_dest is NULL or message_count is 0.
 * @retval      FSP_ERR_NOT_OPEN          The RSENT channel specified in p_ctrl is not opened.
 * @retval      FSP_ERR_IN_USE            The slow channel message receive is on-going, or (transfer instance is used)
 *                                        the fast channel message receive is on-going. Wait for it to complete
 *                                        or abort it before create a new read request.
 * @retval      FSP_ERR_INVALID_SIZE      A transfer instance is used and message_count is not 1.
 * @retval      FSP_ERR_INVALID_MODE      A transfer instance is used and no serial message format is configured.
 * @return      See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 *              This function calls:
 *                  * @ref transfer_api_t::reconfigure
 *                  * @ref transfer_api_t::enable
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_SlowMsgRead (sent_ctrl_t * const p_ctrl, void * p_dest, uint32_t const message_count) /* PRQA S 1505 # Advisory R-8.7 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl;                    /* PRQA S 0316 # Advisory R-11.5 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(p_dest);                                                                                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(0U != message_count);                                                                         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);                            /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif
    if (0U == p_instance_ctrl->slow_msg_rx_number)
    {
#if (RSENT_SENT_TRANSFER_SUPPORT_ENABLE)

        /* Pointer to the receive transfer instance configuration */
        const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;

        if (NULL != p_transfer_rx)
        {
            /* DMA/DTSC only counts the frames of one slow message */
            FSP_ERROR_RETURN(1U == message_count, FSP_ERR_INVALID_SIZE);                 /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            /* One transfer channel serves one request at a time */
            FSP_ERROR_RETURN(0U == p_instance_ctrl->fast_msg_rx_number, FSP_ERR_IN_USE); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            /* Slow channel messages are not extracted when no serial message format is configured */
            FSP_ERROR_RETURN(
                (uint32_t) SENT_SERIAL_MESSAGE_FORMAT_NO_SERIAL_MESSAGE != p_instance_ctrl->p_reg->CC_b.SMF,
                FSP_ERR_INVALID_MODE); /* PRQA S 3432, 1881, 0662 # DP_MSR_R-20.7_UC1, DP_MSR_R-10.4_UC1, Required R-1.1 */

            /* Pointer to the transfer information structure */
            transfer_info_t * p_info = p_transfer_rx->p_cfg->p_info;

            /* The transfer only counts frames. The message is read in the transfer callback when the hardware reports
             * new slow channel data. */
            uint32_t frames_per_msg;
            if ((uint32_t) SENT_SERIAL_MESSAGE_FORMAT_SHORT_SERIAL_MESSAGE == p_instance_ctrl->p_reg->CC_b.SMF) /* PRQA S 1881, 0662 # DP_MSR_R-10.4_UC1, Required R-1.1 */
            {
                frames_per_msg = RSENT_SENT_SHORT_SERIAL_MSG_FRAMES;
            }
            else
            {
                frames_per_msg = RSENT_SENT_ENHANCED_SERIAL_MSG_FRAMES;
            }

            /* Set the destination address for the transfer to the flat staging buffer */
            p_transfer_rx->p_cfg->p_info->p_dest = (uint32_t *) &g_rsent_sent_slow_msg_transfer_staging[0];

            /* Force a 4 byte transfer unit to match the staging buffer element size */
            if ((uint32_t) TRANSFER_ADDR_MODE_INCREMENTED == p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode) /* PRQA S 1881, 1823 # DP_MSR_R-10.4_UC1 */
            {
                p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size = TRANSFER_SIZE_4_BYTE;                      /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size = TRANSFER_SIZE_4_BYTE;                      /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
            }

            if ((uint32_t) TRANSFER_DTSC_ADDR_MODE_FIXED ==                                                           /* PRQA S 1823 # DP_MSR_R-10.4_UC1 */
                p_info->transfer_control_cfg.transfer_control_b.src_addr_direction)                                   /* PRQA S 1881, 1823 # DP_MSR_R-10.4_UC1 */
            {
                p_info->transfer_control_cfg.transfer_control_b.transfer_data_size = TRANSFER_SIZE_4_BYTE;            /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
            }

            /* Set the number of transfers to one per frame */
            p_info->number_transfer = (uint16_t) frames_per_msg;

            /* Set the request state before enabling the transfer */
            p_instance_ctrl->p_slow_msg_buffer  = p_dest;
            p_instance_ctrl->slow_msg_rx_count  = 0U;
            p_instance_ctrl->slow_msg_rx_number = 1U;

            /* Reconfigure the transfer with updated settings */
            fsp_err_t err = p_transfer_rx->p_api->reconfigure(p_transfer_rx->p_ctrl, p_info);

            /* Return error code if reconfiguration fails */
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            /* Enable transfer using the Enable API */
            err = p_transfer_rx->p_api->enable(p_transfer_rx->p_ctrl);

            /* Return error code if reconfiguration fails */
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            return FSP_SUCCESS;
        }
#endif

        /* Save fast channel message receive buffer information to control block */
        p_instance_ctrl->p_slow_msg_buffer  = p_dest;
        p_instance_ctrl->slow_msg_rx_count  = 0;
        p_instance_ctrl->slow_msg_rx_number = message_count;
    }
    else
    {

        /* Slow channel message receive is on-going, return error */
        return FSP_ERR_IN_USE;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Sent a SPC pulse with length specified in pulse_length.
 * Implements @ref sent_api_t::spcWrite
 *
 * @param[in]    p_ctrl                         Pointer to RSENT control structure.
 * @param[in]    pulse_length                   SPC pulse length to generate in ticks (1->128 tick).
 * @retval       FSP_SUCCESS                    SPC pulse sent successfuly.
 * @retval       FSP_ERR_ASSERTION              Pointer to control block is NULL.
 * @retval       FSP_ERR_NOT_OPEN               The control block has not been opened.
 * @retval       FSP_ERR_SENT_SPC_PULSE_LENGTH  The pulse_length is not in between 1 and 128.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_SPCWrite (sent_ctrl_t * const p_ctrl, uint8_t const pulse_length)              /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl;             /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);                     /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check pulse_length is in range between 1 and 128 */
    FSP_ERROR_RETURN(1U <= pulse_length, FSP_ERR_SENT_SPC_PULSE_LENGTH);                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(RSENT_SENT_SPC_PULSE_LENGTH_MAX >= pulse_length, FSP_ERR_SENT_SPC_PULSE_LENGTH); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif

    /* Write to SPCT register to initialize a SPC pulse */
    p_instance_ctrl->p_reg->SPCT_b.TLL = (pulse_length - 1U) & R_RSENT0_SPCT_TLL_Msk;                 /* PRQA S 1891, 0662 # DP_MSR_R-10.7_UC1, Required R-1.1 */

    /* Check if no response status bit is set */
    if (1U == p_instance_ctrl->p_reg->CS_b.NRS)                                                       /* PRQA S 0662 # Required R-1.1 */
    {
        /* Because NRS is set => No SPC pulse is sent. We need to clear NRS and sent SPC pulse against */
        p_instance_ctrl->p_reg->CSC_b.NRC  = 1U;                                                      /* PRQA S 0662 # Required R-1.1 */
        p_instance_ctrl->p_reg->SPCT_b.TLL = (pulse_length - 1U) & R_RSENT0_SPCT_TLL_Msk;             /* PRQA S 1891, 0662 # DP_MSR_R-10.7_UC1, Required R-1.1 */
    }

    /* Check if SPC timeout timer is available. If it is available, we start timeout function */
    if (NULL != p_instance_ctrl->p_cfg->p_spc_timer)
    {
        p_instance_ctrl->p_cfg->p_spc_timer->p_api->start(p_instance_ctrl->p_cfg->p_spc_timer->p_ctrl);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Select RSENT control for SPCO output signal, enabling/disabling TAUD output control.
 * Implements @ref sent_api_t::spcSelect
 *
 * @param[in] p_ctrl                         Pointer to control block.
 * @param[in] mode                           Input source mode. Use one of the following value:
 *                                           SENT_SPCO_MODE_RSENT_SPC (0U): Select RSENT_SPC as the source.
 *                                           SENT_SPCO_MODE_TAUD  (2U): Select TAUDxx as the source.
 * @retval    FSP_SUCCESS                    Start request is successful.
 * @retval    FSP_ERR_ASSERTION              Pointer to control block is NULL.
 * @retval    FSP_ERR_NOT_OPEN               The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_Select_SPCO (sent_ctrl_t * const p_ctrl, sent_spco_mode_t const mode) /* PRQA S 1505, 3206 # Advisory R-8.7, Advisory R-2.7 */
{
#if (BSP_FEATURE_RSENT_SENT_SPCO_SUPPORT)                                                    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl;

 #if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(NULL != p_instance_ctrl);

    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    /* Select channel */
    uint8_t channel = p_instance_ctrl->p_cfg->channel;

    /* Calculate the bit position offset corresponding to the channel */
    uint8_t bit_pos = channel * RSENT_CHANNEL_OFFSET_BITS;

    /* Clear the existing 2 bits at the target position */
    p_instance_ctrl->p_ts_spc_sel_reg->RSENTSPCSEL &= ~(RSENT_CHANNEL_BIT_MASK << bit_pos);

    /* Write the new mode value into the cleared position */
    p_instance_ctrl->p_ts_spc_sel_reg->RSENTSPCSEL |= ((uint32_t) mode << bit_pos);

    return FSP_SUCCESS;
#else

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * This function is used to get RSENT mode status and communication status.
 *
 * @param[in]    p_ctrl                   Pointer to RSENT control structure.
 * @param[in]    p_status                 Pointer to memory to store the RSENT status.
 * @retval       FSP_SUCCESS              Status get successfully.
 * @retval       FSP_ERR_ASSERTION        Pointer to p_ctrl or p_status is NULL.
 * @retval       FSP_ERR_NOT_OPEN         The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_StatusGet (sent_ctrl_t * const p_ctrl, sent_status_t * p_status)   /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(p_status);                                                                 /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif

    /* Get RSENT current mode in MST register */
    *p_status = (sent_status_t) p_instance_ctrl->p_reg->MST_b.OMS;                        /* PRQA S 0662, 4342 # Required R-1.1, Advisory R-10.5 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function is used to abort the ongoing read.
 * Implements @ref sent_api_t::abort
 * @param[in]    p_ctrl                     Pointer to RSENT control structure.
 * @param[in]    communication_to_abort     The communication is going to abort.
 * @retval       FSP_ERR_ASSERTION          Pointer to p_ctrl is NULL.
 * @retval       FSP_ERR_NOT_OPEN           The control block has not been opened.
 * @retval       FSP_SUCCESS                The abortion is successfully.
 * @retval       FSP_ERR_SENT_ABORT_FAILED  The abortion is failed due to communication is not ongoing.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_Abort (sent_ctrl_t * const p_ctrl, sent_communication_t const communication_to_abort) /* PRQA S 1505 # Advisory R-8.7 */
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl;                    /* PRQA S 0316 # Advisory R-11.5 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);                            /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif

    if (SENT_COMMUNICATION_FAST_MESSAGE_READ == communication_to_abort)
    {
        /* Check if fast channel message read is ongoing */
        if (p_instance_ctrl->fast_msg_rx_count < p_instance_ctrl->fast_msg_rx_number)
        {
            /* Fast channel message read abort */
            p_instance_ctrl->fast_msg_rx_number = 0;
            p_instance_ctrl->fast_msg_rx_count  = 0;
            p_instance_ctrl->p_fast_msg_buffer  = NULL;
        }
        else
        {

            /* Fast channel message is not in receiving state */
            return FSP_ERR_SENT_ABORT_FAILED;
        }
    }
    else if (SENT_COMMUNICATION_SLOW_MESSAGE_READ == communication_to_abort) /* PRQA S 2004 # DP_MSR_R-15.7_UC1 */
    {
        /* Check if slow channel message read is ongoing */
        if (p_instance_ctrl->slow_msg_rx_count < p_instance_ctrl->slow_msg_rx_number)
        {
            /* Slow channel message read abort */
            p_instance_ctrl->slow_msg_rx_number = 0;
            p_instance_ctrl->slow_msg_rx_count  = 0;
            p_instance_ctrl->p_slow_msg_buffer  = NULL;
        }
        else
        {

            /* Slow channel message is not in receiving state */
            return FSP_ERR_SENT_ABORT_FAILED;
        }
    }
    else
    {
        /* Invalid communication type */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Specify callback function, optional context pointer and working memory pointer.
 * Implements @ref sent_api_t::callbackSet
 *
 * @param[in] p_ctrl              Pointer to RSENT control structure.
 * @param[in] p_callback          Callback function pointer input.
 * @param[in] p_context           Context pointer input.
 * @param[in] p_callback_memory   Callback memory pointer input.
 * @retval    FSP_SUCCESS         Callback function is set successfully.
 * @retval    FSP_ERR_ASSERTION   Pointer to control block or p_callback is NULL.
 * @retval    FSP_ERR_NOT_OPEN    The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_RSENT_SENT_CallbackSet (sent_ctrl_t * const          p_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                                    void (                     * p_callback)(sent_callback_args_t * p_args),
                                    void const * const           p_context,
                                    sent_callback_args_t * const p_callback_memory)
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                                                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(p_callback);                                                               /* PRQA S 3432, 3344 # DP_MSR_R-20.7_UC1, DP_MSR_R-14.4_UC1 */
    /* Verify this channel has not been initialized */
    FSP_ERROR_RETURN(RSENT_SENT_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif

    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup RSENT_SENT)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Local function used by open API to open timer for SPC timeout function
 *
 * @param[in] p_spc_timer        Pointer to timer instance
 * @param[in] p_instance_ctrl    Pointer to control block
 * @retval    FSP_SUCCESS        SPC timer is opened successfully
 * @return                       See @ref RENESAS_ERROR_CODES or functions called by this function for other possible
 *                               return codes. This function calls:
 *                                   * @ref timer_api_t::open
 *                                   * @ref timer_api_t::callbackSet
 **********************************************************************************************************************/
static fsp_err_t r_rsent_sent_spc_timer_open (timer_instance_t const     * p_spc_timer,
                                              rsent_sent_instance_ctrl_t * p_instance_ctrl)
{
    /* Open SPC timer */
    fsp_err_t err = p_spc_timer->p_api->open(p_spc_timer->p_ctrl, p_spc_timer->p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Set p_callback for SPC timer to r_rsent_sent_spc_timer_callback in this file */
    err = p_spc_timer->p_api->callbackSet(p_spc_timer->p_ctrl,
                                          r_rsent_sent_spc_timer_callback, /* PRQA S 3635 # Advisory R-17.12 */
                                          (void *) p_instance_ctrl,
                                          NULL);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configures the transfer settings for the RSENT module.
 * This function sets up the receive transfer instance if transfer support is enabled, including opening the transfer,
 * setting the callback function, and configuring hardware requests.
 *
 * @param[in] p_instance_ctrl Pointer to the RSENT instance control structure.
 * @param[in] p_cfg           Pointer to the RSENT configuration structure.
 **********************************************************************************************************************/
static fsp_err_t r_rsent_sent_transfer_config (rsent_sent_instance_ctrl_t * p_instance_ctrl,
                                               sent_cfg_t const * const     p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;

#if (RSENT_SENT_TRANSFER_SUPPORT_ENABLE == 1)

    /* Pointer to the receive transfer instance configuration */
    const transfer_instance_t * p_transfer_rx = p_cfg->p_transfer_rx;

    /* Pointer to the RSENT channel base address for the specified channel */
    R_RSENT0_Type * reg_base = p_instance_ctrl->p_reg; /* PRQA S 3678 # Advisory R-8.13 */

    /* Pointer to the EFRD1 register for receive data */
    void * p_spdr_rx = (void *) &(reg_base->EFRD1);    /* PRQA S 0311, 0312, 0662, 3678 # DP_MSR_R-11.8_UC1, Required R-1.1, Advisory R-8.13 */

    if (NULL != p_transfer_rx)
    {
        /* Disable isr of rsent to use transfer api */
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->rxi_irq);

        /* Set the source address for the transfer to the EFRD1 register */
        p_transfer_rx->p_cfg->p_info->p_src = p_spdr_rx;

        /* Open the transfer instance */
        err = p_transfer_rx->p_api->open(p_transfer_rx->p_ctrl, p_transfer_rx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

        /* Set the callback function for transfer completion */
        err = p_transfer_rx->p_api->callbackSet(p_transfer_rx->p_ctrl,
                                                r_rsent_sent_transfer_callback, /* PRQA S 3635 # Advisory R-17.12 */
                                                p_instance_ctrl,
                                                NULL);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    }

#else

    /* Mark parameters as unused when transfer support is disabled */
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
    FSP_PARAMETER_NOT_USED(p_cfg);
#endif

    return err;
}

/*******************************************************************************************************************//**
 * Local function used by close API to close timer for SPC timeout function
 *
 * @param[in] p_spc_timer        Pointer to timer instance
 * @retval    FSP_SUCCESS        SPC timer is closed successfully
 * @return                       See @ref RENESAS_ERROR_CODES or functions called by this function for other possible
 *                               return codes. This function calls:
 *                                   * @ref timer_api_t::close
 **********************************************************************************************************************/
static fsp_err_t r_rsent_sent_spc_timer_close (timer_instance_t const * p_spc_timer)
{
    /* Close SPC timer */
    fsp_err_t err = p_spc_timer->p_api->close(p_spc_timer->p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read fast and slow message (if available).
 *
 * @param[in]     p_instance_ctrl     Pointer to RSENT instance control block.
 **********************************************************************************************************************/
static void r_rsent_sent_msg_read (rsent_sent_instance_ctrl_t * p_instance_ctrl)
{
    uint8_t                 slow_channel_new_data = 0U;
    rsent_sent_fast_msg_t * p_fast_msg_buff       = NULL;
    rsent_sent_slow_msg_t * p_slow_msg_buff       = NULL;

    /* Check if fast channel message buffer is available */
    if (p_instance_ctrl->fast_msg_rx_count < p_instance_ctrl->fast_msg_rx_number)
    {
        /* Get current fast channel message buffer pointer */
        p_fast_msg_buff = (rsent_sent_fast_msg_t *) p_instance_ctrl->p_fast_msg_buffer +                 /* PRQA S 0316, 0488 # Advisory R-11.5, Advisory R-18.4 */
                          p_instance_ctrl->fast_msg_rx_count;

        /* Read status and communication in case no serial message extraction */
        if ((uint32_t) SENT_SERIAL_MESSAGE_FORMAT_NO_SERIAL_MESSAGE == p_instance_ctrl->p_reg->CC_b.SMF) /* PRQA S 1881, 0662 # DP_MSR_R-10.4_UC1, Required R-1.1 */
        {
            p_fast_msg_buff->status_communication = (uint8_t) (p_instance_ctrl->p_reg->SRXD);            /* PRQA S 0662 # Required R-1.1 */
        }

#if (RSENT_SENT_CFG_FIXED_MSG_LENGTH)

        /* Provide calibration pulse and message length to user to perform message diagnostic in fixed message length mode */
        p_fast_msg_buff->calibration_pulse_length = p_instance_ctrl->p_reg->CPL;
        p_fast_msg_buff->msg_length               = p_instance_ctrl->p_reg->ML; /* PRQA S 0662 # Required R-1.1 */
#endif                                                                          /* RSENT_SENT_CFG_FIXED_MSG_LENGTH */

#if (RSENT_SENT_CFG_FAST_MSG_TIMESTAMP)

        /* Read timestamp for fast channel message in FRTS register */
        p_fast_msg_buff->timestamp = p_instance_ctrl->p_reg->FRTS;                     /* PRQA S 0662 # Required R-1.1 */
#endif

        /* Read fast channel message data */
        if ((uint32_t) SENT_NUM_OF_DATA_NIBBLES_6 >= p_instance_ctrl->p_reg->CC_b.NDN) /* PRQA S 0662 # Required R-1.1 */
        {
            /* Number of data nibble: 1 -> 6 */
            uint32_t frxd = p_instance_ctrl->p_reg->FRXD;                              /* PRQA S 0662 # Required R-1.1 */

            /* Slow Channel New Data */
            slow_channel_new_data = (uint8_t) ((frxd & R_RSENT0_FRXD_SNDM_Msk) >> R_RSENT0_FRXD_SNDM_Pos);

            /* Fast Channel Communication Nibble Bits */
            p_fast_msg_buff->status_communication |=
                (uint8_t) ((frxd & R_RSENT0_FRXD_FCCN_Msk) >> R_RSENT0_FRXD_FCCN_Pos);

            /* Fast Channel Nibble Data */
            p_fast_msg_buff->data = (frxd & R_RSENT0_FRXD_ND_Msk) >> R_RSENT0_FRXD_ND_Pos;
        }
        else
        {
            /* Number of data nibble: 7, 8 */
            uint32_t efrxd0 = (uint8_t) (p_instance_ctrl->p_reg->EFRD0);                           /* PRQA S 0662 # Required R-1.1 */

            /* Slow Channel New Data */
            slow_channel_new_data = (efrxd0 & R_RSENT0_EFRD0_SNDM_Msk) >> R_RSENT0_EFRD0_SNDM_Pos; /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */

            /* Fast Channel Communication Nibble Bits */
            p_fast_msg_buff->status_communication |=
                (uint8_t) ((efrxd0 & R_RSENT0_EFRD0_FCCN_Msk) >> R_RSENT0_EFRD0_FCCN_Pos);

            /* Fast Channel Nibble Data */
            p_fast_msg_buff->data = p_instance_ctrl->p_reg->EFRD1; /* PRQA S 0662 # Required R-1.1 */
        }

        p_instance_ctrl->fast_msg_rx_count++;

        /* Check if callback function is available and fast channel message completed */
        if (p_instance_ctrl->fast_msg_rx_count == p_instance_ctrl->fast_msg_rx_number)
        {
            /* Receive finished so clear the slow channel message buffer */
            p_instance_ctrl->fast_msg_rx_number = 0U;
            p_instance_ctrl->fast_msg_rx_count  = 0U;
            p_instance_ctrl->p_fast_msg_buffer  = NULL;
            r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_FAST_CHANNEL_RX_COMPLETE);
        }
    }
    else
    {
        /* Read FRXD register to get Slow Channel New Data bit and clear fast channel message buffer */
        slow_channel_new_data = (uint8_t) p_instance_ctrl->p_reg->FRXD_b.SNDM;                           /* PRQA S 0662 # Required R-1.1 */
    }

    /* Check if slow channel message is available */
    if (slow_channel_new_data &&                                                                         /* PRQA S 4558, 4115 # DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC1 */
        ((uint32_t) SENT_SERIAL_MESSAGE_FORMAT_NO_SERIAL_MESSAGE != (p_instance_ctrl->p_reg->CC_b.SMF))) /* PRQA S 1881, 4115, 4558, 0662 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC1, Required R-1.1 */
    {
        /* Check if slow channel message buffer is available */
        if (p_instance_ctrl->slow_msg_rx_count < p_instance_ctrl->slow_msg_rx_number)
        {
            /* Get current slow channel message buffer pointer */
            p_slow_msg_buff = (rsent_sent_slow_msg_t *) p_instance_ctrl->p_slow_msg_buffer + /* PRQA S 0316, 0488 # Advisory R-11.5, Advisory R-18.4 */
                              p_instance_ctrl->slow_msg_rx_count;
#if (RSENT_SENT_CFG_SLOW_MSG_TIMESTAMP)

            /* Read timestamp for slow channel message in SRTS register */
            p_slow_msg_buff->timestamp = p_instance_ctrl->p_reg->SRTS;                                          /* PRQA S 0662 # Required R-1.1 */
#endif                                                                                                          /* RSENT_SENT_CFG_SLOW_MSG_TIMESTAMP */

            /* Read slow channel message */
            uint32_t srxd = p_instance_ctrl->p_reg->SRXD;                                                       /* PRQA S 0662 # Required R-1.1 */

            if ((uint32_t) SENT_SERIAL_MESSAGE_FORMAT_SHORT_SERIAL_MESSAGE == p_instance_ctrl->p_reg->CC_b.SMF) /* PRQA S 1881, 0662 # DP_MSR_R-10.4_UC1, Required R-1.1 */
            {
                /* Short serial message: 4 bits message id, 8 bits data */
                p_slow_msg_buff->data   = srxd & RSENT_SENT_SRXD_SHORT_SERIAL_MSG_DATA_MASK;
                p_slow_msg_buff->msg_id =                                                                       /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
                                          (srxd & RSENT_SENT_SRXD_SHORT_SERIAL_MSG_ID_MASK) >>
                                          RSENT_SENT_SRXD_SHORT_SERIAL_MSG_ID_POS;
            }
            else
            {
                /* Enhanced serial message */
                /* Read configuration bit */
                p_slow_msg_buff->cfg_bit = (srxd & R_RSENT0_SRXD_SMGC_Msk) >> R_RSENT0_SRXD_SMGC_Pos; /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
                if (0U == p_slow_msg_buff->cfg_bit)
                {
                    /* Enhanced serial message 0: 8 bits message id, 12 bits data */
                    p_slow_msg_buff->data   = srxd & RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_0_DATA_MASK;
                    p_slow_msg_buff->msg_id = /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
                                              (srxd & RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_0_ID_MASK) >>
                                              RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_0_ID_POS;
                }
                else
                {
                    /* Enhanced serial message 1: 4 bits message id, 16 bits data */
                    p_slow_msg_buff->data   = srxd & RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_1_DATA_MASK;
                    p_slow_msg_buff->msg_id = /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
                                              (srxd & RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_1_ID_MASK) >>
                                              RSENT_SENT_SRXD_ENHANCED_SERIAL_MSG_1_ID_POS;
                }
            }

            p_instance_ctrl->slow_msg_rx_count++;

            /* Check if callback function is available and fast channel message completed */
            if (p_instance_ctrl->slow_msg_rx_count == p_instance_ctrl->slow_msg_rx_number)
            {
                /* Receive finished so clear the slow channel message buffer */
                p_instance_ctrl->slow_msg_rx_number = 0U;
                p_instance_ctrl->slow_msg_rx_count  = 0U;
                p_instance_ctrl->p_slow_msg_buffer  = NULL;
                r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_SLOW_CHANNEL_RX_COMPLETE);
            }
        }
        else
        {
            /* Read SRXD to clear SRXD */
            (void) p_instance_ctrl->p_reg->SRXD; /* PRQA S 0662 # Required R-1.1 */
        }
    }
}

/*******************************************************************************************************************//**
 * Sets the timestamp master configuration for the SENT protocol.
 *
 * This function configures timestamp master selection for the SENT channels based on the given status and configuration.
 * It adjusts the selection registers according to the MCU group being used.
 *
 **********************************************************************************************************************/
static fsp_err_t r_rsent_set_timestamp_master (sent_ctrl_t * const      p_ctrl,
                                               sent_cfg_t const * const p_cfg,
                                               uint8_t                  status)
{
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) p_ctrl;         /* PRQA S 0316 # Advisory R-11.5 */
    rsent_sent_extended_cfg_t  * p_extend        = (rsent_sent_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */

    /* Pointer to register and composite value for register setting */
    volatile uint32_t * rsent_mssel_reg;
    uint32_t            rsent_mssel_shift;
    uint32_t            rsent_mssel_tmp;
    uint32_t            rsent_mssel_mask;
    uint32_t            rsent_mssel_value;
    uint32_t            rsent_mssel;

#if (RSENT_SENT_CFG_PARAM_CHECKING_ENABLE)

    /* Check parameters. */
    FSP_ASSERT(p_instance_ctrl);                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(p_cfg->p_extend);                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#endif

    /* Set timestamp selection register */
    p_instance_ctrl->p_ts_spc_sel_reg = R_RSENTTSSEL; /* PRQA S 0306 # Advisory R-11.4 */
    uint8_t channel = p_cfg->channel;

    /* Select the MSSEL value based on the status */
    if (status == RSENT_SENT_SET_TIMESTAMP)
    {
        rsent_mssel = p_extend->timestamp_master & RSENT_SENT_SELB_RSENT0_MASTER_SELECTION_MASK;            /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    }
    else
    {
        rsent_mssel = RSENT_SENT_TIMESTAMP_MASTER_NO_MASTER & RSENT_SENT_SELB_RSENT0_MASTER_SELECTION_MASK; /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    }

    /* Select the register and bit position based on the channel number */
    if (channel < BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD)
    {
        rsent_mssel_reg   = &p_instance_ctrl->p_ts_spc_sel_reg->RSENTTSSEL0;        /* PRQA S 0662 # Required R-1.1 */
        rsent_mssel_shift = channel * BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS; /* PRQA S 4491 # DP_MSR_R-10.6_UC1 */
    }

#if (4U != BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS)
    else if (channel < (RSENT_SENT_TSSEL1 * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD))
    {
        rsent_mssel_reg   = &p_instance_ctrl->p_ts_spc_sel_reg->RSENTTSSEL1;                              /* PRQA S 0662 # Required R-1.1 */
        rsent_mssel_shift = (channel - BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD) *                           /* PRQA S 4491 # DP_MSR_R-10.6_UC1 */
                            BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS;
    }
#endif
#if (1U == BSP_FEATURE_RSENT_SENT_HAS_EXTENDED_TSSEL)                                                     /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    else if (channel < (RSENT_SENT_TSSEL2 * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD))
    {
        rsent_mssel_reg   = &p_instance_ctrl->p_ts_spc_sel_reg->RSENTTSSEL2;                              /* PRQA S 0662 # Required R-1.1 */
        rsent_mssel_shift = (channel - (RSENT_SENT_TSSEL2 - 1) * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD) * /* PRQA S 1840, 4491, 3395 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.6_UC1, Advisory R-12.1 */
                            BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS;
    }
    else if (channel < (RSENT_SENT_TSSEL3 * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD))
    {
        rsent_mssel_reg   = &p_instance_ctrl->p_ts_spc_sel_reg->RSENTTSSEL3;                              /* PRQA S 0662 # Required R-1.1 */
        rsent_mssel_shift = (channel - (RSENT_SENT_TSSEL3 - 1) * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD) * /* PRQA S 1840, 4491, 3395 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.6_UC1, Advisory R-12.1 */
                            BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS;
    }
    else if (channel < (RSENT_SENT_TSSEL4 * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD))
    {
        rsent_mssel_reg   = &p_instance_ctrl->p_ts_spc_sel_reg->RSENTTSSEL4;                              /* PRQA S 0662 # Required R-1.1 */
        rsent_mssel_shift = (channel - (RSENT_SENT_TSSEL4 - 1) * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD) * /* PRQA S 1840, 4491, 3395 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.6_UC1, Advisory R-12.1 */
                            BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS;
    }
    else if (channel < (RSENT_SENT_TSSEL5 * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD))
    {
        rsent_mssel_reg   = &p_instance_ctrl->p_ts_spc_sel_reg->RSENTTSSEL5;                              /* PRQA S 0662 # Required R-1.1 */
        rsent_mssel_shift = (channel - (RSENT_SENT_TSSEL5 - 1) * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD) * /* PRQA S 1840, 4491, 3395 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.6_UC1, Advisory R-12.1 */
                            BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS;
    }
    else if (channel < (RSENT_SENT_TSSEL6 * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD))
    {
        rsent_mssel_reg   = &p_instance_ctrl->p_ts_spc_sel_reg->RSENTTSSEL6;                              /* PRQA S 0662 # Required R-1.1 */
        rsent_mssel_shift = (channel - (RSENT_SENT_TSSEL6 - 1) * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD) * /* PRQA S 1840, 4491, 3395 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.6_UC1, Advisory R-12.1 */
                            BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS;
    }
    else
    {
        rsent_mssel_reg   = &p_instance_ctrl->p_ts_spc_sel_reg->RSENTTSSEL7;                              /* PRQA S 0662 # Required R-1.1 */
        rsent_mssel_shift = (channel - (RSENT_SENT_TSSEL7 - 1) * BSP_FEATURE_RSENT_SENT_UNIT_THRESHOLD) * /* PRQA S 1840, 4491, 3395 # DP_MSR_R-10.4_UC1, DP_MSR_R-10.6_UC1, Advisory R-12.1 */
                            BSP_FEATURE_RSENT_SENT_RSENT_SENT_MSSEL_BITS;
    }
#endif

    /* Prepare a mask for the 5 bits corresponding to this channel */
    rsent_mssel_mask = RSENT_SENT_SELB_RSENT0_MASTER_SELECTION_MASK << rsent_mssel_shift; /* PRQA S 4491 # DP_MSR_R-10.6_UC1 */

    /* Read and clear the old 5-bit field for this channel */
    rsent_mssel_tmp = *rsent_mssel_reg & ~rsent_mssel_mask;

    /* Set the new value in the cleared bit field */
    rsent_mssel_value = (rsent_mssel << rsent_mssel_shift) & rsent_mssel_mask;
    rsent_mssel_tmp   = rsent_mssel_tmp | rsent_mssel_value;

    /* Write the updated value back to the register */
    *rsent_mssel_reg = rsent_mssel_tmp;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Local function to calls user callback.
 *
 * @param[in]     p_instance_ctrl     Pointer to RSENT instance control block.
 * @param[in]     event               Event code.
 **********************************************************************************************************************/
static void r_rsent_sent_call_callback (rsent_sent_instance_ctrl_t * p_instance_ctrl, sent_event_t event) /* PRQA S 3673 # Advisory R-8.13 */
{
    sent_callback_args_t callback_args;

    if (NULL == p_instance_ctrl->p_callback)
    {

        /* Return if user callback is not available */
        return;
    }

    /* Store callback arguments in memory provided by user if available */
    sent_callback_args_t * p_callback_args = p_instance_ctrl->p_callback_memory;

    if (NULL == p_callback_args)
    {
        /* Store on stack */
        p_callback_args = &callback_args;
    }
    else
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        callback_args = *p_callback_args;
    }

    p_callback_args->event     = event;
    p_callback_args->p_context = p_instance_ctrl->p_context;

    /* Call user callback function */
    p_instance_ctrl->p_callback(p_callback_args);

    if (NULL != p_instance_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_instance_ctrl->p_callback_memory = callback_args;
    }
}

/*******************************************************************************************************************//**
 * Local function to handle communication error and call user callback function if it is available
 *
 * @param[in]     p_instance_ctrl     Pointer to RSENT instance control block
 **********************************************************************************************************************/
static void r_rsent_sent_error_handle (rsent_sent_instance_ctrl_t * p_instance_ctrl)
{
    /* Determine the error */
    uint32_t     error_interrupt_enable_mask = p_instance_ctrl->p_reg->IDE & (~R_RSENT0_IDE_FRIE_Msk); /* PRQA S 0662 # Required R-1.1 */
    sent_event_t interrupt_event             =
        (sent_event_t) (p_instance_ctrl->p_reg->CS & error_interrupt_enable_mask);                     /* PRQA S 4394, 0662, 4342 # DP_MSR_R-10.8_UC1, Required R-1.1, Advisory R-10.5 */

    if (R_RSENT0_CS_FCS_Msk == (interrupt_event & R_RSENT0_CS_FCS_Msk))                                /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Fast Channel CRC Error */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_FAST_CHANNEL_CRC_ERROR);

        /* Clear Fast Channel CRC Error flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_FCS_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_FMS_Msk == (interrupt_event & R_RSENT0_CS_FMS_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Fast Channel Message Lost */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_FAST_CHANNEL_MESSAGE_LOST);

        /* Clear Fast Channel Message Lost flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_FMS_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_FES_Msk == (interrupt_event & R_RSENT0_CS_FES_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Fast Channel Nibble Encoding Error */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_FAST_CHANNEL_NIBBLE_ENCODING_ERROR);

        /* Clear Fast Channel Nibble Encoding Error flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_FES_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_FNS_Msk == (interrupt_event & R_RSENT0_CS_FNS_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Fast Channel Nibble Count Error */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_FAST_CHANNEL_NIBBLE_COUNT_ERROR);

        /* Clear Fast Channel Nibble Count Error flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_FNS_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_CLS_Msk == (interrupt_event & R_RSENT0_CS_CLS_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Calibration Pulse Length Error */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_CALIBRATION_PULSE_LENGTH_ERROR);

        /* Clear Calibration Pulse Length Error flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_CLS_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_CVS_Msk == (interrupt_event & R_RSENT0_CS_CVS_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Calibration Pulse Length Variation Error */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_CALIBRATION_PULSE_LENGTH_VARIATION_ERROR);

        /* Clear Calibration Pulse Length Variation Error flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_CVS_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_NRS_Msk == (interrupt_event & R_RSENT0_CS_NRS_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* No Response Error */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_NO_RESPONSE_ERROR);

        /* Clear No Response Error flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_NRS_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_SCS_Msk == (interrupt_event & R_RSENT0_CS_SCS_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Slow Channel CRC Error */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_SLOW_CHANNEL_CRC_ERROR);

        /* Slow Channel CRC Error flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_SCS_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_SMS_Msk == (interrupt_event & R_RSENT0_CS_SMS_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Slow Channel Message Lost */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_SLOW_CHANNEL_MESSAGE_LOST);

        /* Clear Slow Channel Message Lost flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_SMS_Msk;       /* PRQA S 0662 # Required R-1.1 */
    }
    else if (R_RSENT0_CS_SES_Msk == (interrupt_event & R_RSENT0_CS_SES_Msk)) /* PRQA S 4522 # DP_MSR_R-10.1_UC1 */
    {
        /* Slow channel Encoding Error */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_SLOW_CHANNEL_ENCODING_ERROR);

        /* Clear Slow channel Encoding Error flag */
        p_instance_ctrl->p_reg->CSC |= (uint32_t) R_RSENT0_CS_SES_Msk; /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        /* Should not run into this case */
    }
}

/*******************************************************************************************************************//**
 * Interrupt service routines
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * ERI interrupt handle. The ERI interrupt fires when a communication error happened.
 * The user callback function is called with the communication error event code if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void rsent_sent_eri_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Recover ISR context saved in open. */
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Handle error(s) */
    r_rsent_sent_error_handle(p_instance_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * RXI interrupt processing for receiving mode. The RXI interrupt fires when a new fast message is available.
 * The user callback function is called with the SENT_EVENT_FAST_CHANNEL_RX_COMPLETE event code if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void rsent_sent_rxi_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* RXI is edge interrupt so clear the flag at beginning of isr */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Check if SPC timeout timer is available. If it is available, force to stop the timer */
    if (NULL != p_instance_ctrl->p_cfg->p_spc_timer)
    {
        p_instance_ctrl->p_cfg->p_spc_timer->p_api->stop(p_instance_ctrl->p_cfg->p_spc_timer->p_ctrl);
    }

    /* Read fast message and slow message (if available) */
    r_rsent_sent_msg_read(p_instance_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * This is the callback function for SPC timer
 * This function is called by SPC timer with event code TIMER_EVENT_CYCLE_END when the timer is expired. When that
 * happened, rsent user callback function is called with event code SENT_EVENT_SPC_TIMEOUT if it is available.
 **********************************************************************************************************************/
static void r_rsent_sent_spc_timer_callback (timer_callback_args_t * p_callback_args) /* PRQA S 3673 # Advisory R-8.13 */
{
    if (TIMER_EVENT_CYCLE_END == p_callback_args->event)
    {
        /* Get the pointer to control block of RSENT saved in p_context */
        rsent_sent_instance_ctrl_t * p_instance_ctrl =
            (rsent_sent_instance_ctrl_t *) p_callback_args->p_context; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1, Advisory R-11.5 */

        /* Call user callback to notify SPC timeout occurred */
        r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_SPC_TIMEOUT);
    }
}

#if (RSENT_SENT_TRANSFER_SUPPORT_ENABLE)

/*******************************************************************************************************************//**
 * Callback function for RSENT transfer completion.
 * When a transfer completes, it invokes the user callback function
 * with the event code SENT_EVENT_FAST_CHANNEL_RX_COMPLETE or SENT_EVENT_SLOW_CHANNEL_RX_COMPLETE.
 **********************************************************************************************************************/
static void r_rsent_sent_transfer_callback (transfer_callback_args_t * p_args) /* PRQA S 3673 # Advisory R-8.13 */
{
    if ((TRANSFER_EVENT_TRANS_END_FLAG == p_args->event) || (TRANSFER_EVENT_TRANS_COMPLETION == p_args->event))
    {
        /* Retrieve the RSENT control block from the callback context */
        rsent_sent_instance_ctrl_t * p_instance_ctrl = (rsent_sent_instance_ctrl_t *) (uintptr_t) p_args->p_context; /* PRQA S 0306 # Advisory R-11.4 */

        if (0U != p_instance_ctrl->fast_msg_rx_number)
        {
            /* Copy each staged value into message data, other fields keep their reset value */
            rsent_sent_fast_msg_t * p_fast_msg_buff = (rsent_sent_fast_msg_t *) p_instance_ctrl->p_fast_msg_buffer; /* PRQA S 0316 # Advisory R-11.5 */

            for (uint32_t i = 0U; i < p_instance_ctrl->fast_msg_rx_number; i++)
            {
                p_fast_msg_buff[i].data = g_rsent_sent_fast_msg_transfer_staging[i];
            }

            /* Receive finished so clear the fast channel message buffer state */
            p_instance_ctrl->fast_msg_rx_number = RSENT_SENT_MSG_RX_STATE_RESET;
            p_instance_ctrl->fast_msg_rx_count  = RSENT_SENT_MSG_RX_STATE_RESET;
            p_instance_ctrl->p_fast_msg_buffer  = NULL;

            /* Call the user callback with the fast channel receive complete event */
            r_rsent_sent_call_callback(p_instance_ctrl, SENT_EVENT_FAST_CHANNEL_RX_COMPLETE);
        }
        else
        {
            /* Read the slow message the same way as interrupt mode after DMA counted all frames */
            r_rsent_sent_msg_read(p_instance_ctrl);

            /* The transfer is not re-armed, so release the request whether or not a message was read */
            p_instance_ctrl->slow_msg_rx_number = RSENT_SENT_MSG_RX_STATE_RESET;
            p_instance_ctrl->slow_msg_rx_count  = RSENT_SENT_MSG_RX_STATE_RESET;
            p_instance_ctrl->p_slow_msg_buffer  = NULL;
        }
    }
}

#endif
