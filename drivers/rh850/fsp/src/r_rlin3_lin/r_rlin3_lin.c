/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"                   /* PRQA S 0380 # DP_MSR_D-1.1_UC1 & DP_MSR_D-1.1_UC2 */
#include "r_rlin3_lin.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "RLIN" in ASCII.  Used to determine if the control block is open. */
#define RLIN3_LIN_OPEN     (0x524C494EU)

/* Channel 23 of RLIN3 */
#define RLIN323_CHANNEL    (23U)

/* PRQA S 3472 ++ # Advisory D-4.6 */
/* Offset between RLIN3 register groups (even 32-30, odd 33-31) */
#define RLIN3_REG_SIZE(channel)    (((R_RLN32_BASE - R_RLN30_BASE) * \
                                     (!((channel) % 2u)))            \
                                    +                                \
                                    ((R_RLN33_BASE - R_RLN31_BASE) * \
                                     (((channel) % 2u))))

/* PRQA S 3472 -- */
/* GAP of two register in peripheral group 4 and group 10 */
#define RLIN3_REG_GROUP_GAP                           (R_RLN31_BASE - R_RLN30_BASE)

/* LIN 6-bit ID Mask . */
#define RLIN3_LIN_FRAME_ID_MASK                       (0x3FU)

/* Number of possible frame identifiers */
#define LIN_ID_COUNT                                  (RLIN3_LIN_FRAME_ID_MASK + 1U) /* PRQA S 1534 # Advisory R-2.5 */

/* Number of clock divisors select */
#define RLIN3_LIN_MAX_CLKDIV_SEL                      (8U)

/* Macro to clear error flags*/
#define RLIN3_LIN_CLEAR_ERROR_FLAG                    (0U)

/* LEDE register masks*/
#define RLIN3_LIN_LEDE_MASTER_MASK                    (0x8FU)
#define RLIN3_LIN_LEDE_SLAVE_MASK                     (0xDDU)

/*Marco to switch to reset mode*/
#define RLIN3_LIN_RESET_MODE                          (0xFEU) /* PRQA S 1534 # Advisory R-2.5 */

/* Max frame data length */
#define RLIN3_LIN_MAX_FRAME_NUM_BYTES                 (8U)

/* Max value of baudrate prescaler */
#define RLIN3_LIN_MAX_MASTER_BAUDRATE_PRESCALER       (0xFFU)
#define RLIN3_LIN_MAX_SLAVE_BAUDRATE_PRESCALER        (0xFFFFU)

/* Register Enable LIN Operating Interrupt Mask  */
#define RLIN3_LIN_OPERATING_INTERRUPT_ENABLE          (0x0FU)

/*Max value of bit sampling for setting baudrate*/
#define RLIN3_LIN_MAX_BITSAMPLING                     (15U)

/*Initial value of bit sampling in slave mode*/
#define RLIN3_LIN_SLAVE_SAMPLING                      (3U) /* PRQA S 1534 # Advisory R-2.5 */

/*Self Test Mode - Write command 1*/
#define LIN_SELFTEST_COMMAND1                         (0xA7U)

/*Self Test Mode - Write command 2*/
#define LIN_SELFTEST_COMMAND2                         (0x58U)

/*Enable Self Test Mode*/
#define LIN_SELFTEST_ENABLE                           (1U)

/*Disable Self Test Mode*/
#define LIN_SELFTEST_DISABLE                          (0U)

/*Bit error detection event.*/
#define RLIN3_LIN_EVENT_ERR_BIT                       (0x1UL)  /* PRQA S 1534 # Advisory R-2.5 */

/*Physical bus error detection event.*/
#define RLIN3_LIN_EVENT_ERR_PHYSICAL_BUS              (0x2UL)  /* PRQA S 1534 # Advisory R-2.5 */

/*Timeout error event.*/
#define RLIN3_LIN_EVENT_ERR_TIMEOUT                   (0x4UL)  /* PRQA S 1534 # Advisory R-2.5 */

/*Framing error event.*/
#define RLIN3_LIN_EVENT_ERR_FRAMING                   (0x8UL)  /* PRQA S 1534 # Advisory R-2.5 */

/*Sync field error event.*/
#define RLIN3_LIN_EVENT_ERR_SYNC                      (0x10UL) /* PRQA S 1534 # Advisory R-2.5 */

/*Checksum was invalid.*/
#define RLIN3_LIN_EVENT_ERR_INVALID_CHECKSUM          (0x20UL) /* PRQA S 1534 # Advisory R-2.5 */

/*ID Parity error event.*/
#define RLIN3_LIN_EVENT_ERR_PARITY                    (0x40UL) /* PRQA S 1534 # Advisory R-2.5 */

/*Response Preparation Error event.*/
#define RLIN3_LIN_EVENT_ERR_RESPONSE_PREPARE          (0x80UL) /* PRQA S 1534 # Advisory R-2.5 */

#define RLIN3_LIN_MODE_MASTER                         ((uint8_t) 0U)

#define RLIN3_LIN_MAX_CHECKSUM                        (0xFFU)

#define RLIN3_LIN_MAX_PRESCALER1                      (0xFFU)

/* Constants for baud rate calculation */
#define RLIN3_LIN_100_PERCENT_X1000                   (100000)
#define RLIN3_LIN_MASTER_MAX_BAUD_RATE_ERROR_X1000    (500)
#define RLIN3_LIN_SLAVE_MAX_BAUD_RATE_ERROR_X1000     (1500)

/**********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/* LIN on RLIN3 HAL API mapping for LIN interface */
const lin_api_t g_lin_on_rlin3 =                          /* PRQA S 1502 # Advisory R-2.8 */
{
    .open               = R_RLIN3_LIN_Open,               /* PRQA S 3635 # Advisory R-17.12 */
    .startFrameWrite    = R_RLIN3_LIN_StartFrameWrite,    /* PRQA S 3635 # Advisory R-17.12 */
    .write              = R_RLIN3_LIN_Write,              /* PRQA S 3635 # Advisory R-17.12 */
    .read               = R_RLIN3_LIN_Read,               /* PRQA S 3635 # Advisory R-17.12 */
    .communicationAbort = R_RLIN3_LIN_CommunicationAbort, /* PRQA S 3635 # Advisory R-17.12 */
    .callbackSet        = R_RLIN3_LIN_CallbackSet,        /* PRQA S 3635 # Advisory R-17.12 */
    .wakeupSend         = R_RLIN3_LIN_WakeupSend,         /* PRQA S 3635 # Advisory R-17.12 */
    .sleepEnter         = R_RLIN3_LIN_SleepEnter,         /* PRQA S 3635 # Advisory R-17.12 */
    .sleepExit          = R_RLIN3_LIN_SleepExit,          /* PRQA S 3635 # Advisory R-17.12 */
    .close              = R_RLIN3_LIN_Close,              /* PRQA S 3635 # Advisory R-17.12 */
};

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static void    r_rlin3_lin_baud_set(R_RLN30_Type * p_rlin3_reg, rlin3_lin_baud_setting_t const * const p_baud_setting);
static void    r_rlin3_lin_hw_configure(rlin3_lin_instance_ctrl_t * const p_ctrl);
static void    r_rlin3_lin_transmission(rlin3_lin_instance_ctrl_t * const p_ctrl);
static void    r_rlin3_lin_reception(rlin3_lin_instance_ctrl_t * const p_ctrl);
static uint8_t r_rlin3_lin_pid_calculate(uint8_t frame_id);
static uint8_t r_rlin3_checksum_calculate(uint8_t             frame_id,
                                          lin_checksum_type_t checksum_type,
                                          uint8_t           * p_data,
                                          uint16_t            data_len);

static void r_rlin3_lin_call_callback(rlin3_lin_instance_ctrl_t * p_ctrl, lin_event_t event);
static void r_rlin3_lin_resp_space_dominant_handle(rlin3_lin_instance_ctrl_t * p_instance_ctrl);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

BSP_INTERRUPT_ATTRIBUTE void rlin3_lin_rxi_isr(void);                       /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1, DP_MSR_R-8.5_UC2 */
BSP_INTERRUPT_ATTRIBUTE void rlin3_lin_isr(void);                           /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1, DP_MSR_R-8.5_UC2 */
BSP_INTERRUPT_ATTRIBUTE void rlin3_lin_txi_isr(void);                       /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1, DP_MSR_R-8.5_UC2 */
BSP_INTERRUPT_ATTRIBUTE void rlin3_lin_eri_isr(void);                       /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1, DP_MSR_R-8.5_UC2 */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

static const uint16_t rlin3_lin_clock_prescaler[RLIN3_LIN_MAX_CLKDIV_SEL] = /* PRQA S 3218 # Advisory R-8.9 */
{
    1U,
    2U,
    4U,
    8U,
    16U,
    32U,
    64U,
    128U,
};
static const uint16_t rlin3_lin_system_clock[4] = /* PRQA S 3218 # Advisory R-8.9 */
{
    1U,
    2U,
    8U,
    2U,
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup RLIN3_LIN
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configures the LIN driver channel based on the input configuration.
 *
 * Implements @ref lin_api_t::open.
 *
 * Example:
 * @snippet r_rlin3_lin_example.c R_RLIN3_LIN_Open
 *
 * @retval  FSP_SUCCESS                    Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION              Pointer to LIN control block or configuration structure is NULL.
 * @retval  FSP_ERR_INVALID_CHANNEL        The requested channel does not exist on this MCU
 * @retval  FSP_ERR_ALREADY_OPEN           Control block has already been opened or channel is being used by another
 *                                         instance. Call close() then open() to reconfigure.
 *
 * @return                                 See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                          possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_Open (lin_ctrl_t * const p_api_ctrl, lin_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) p_api_ctrl;        /* PRQA S 0316 # Advisory R-11.5 */
#if RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE

    /* Check parameters. */
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_callback);
    FSP_ASSERT(p_cfg->p_extend);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ERROR_RETURN(((1UL << (uint32_t) p_cfg->channel) & BSP_FEATURE_RLIN3_LIN_CHANNELS_MASK) != 0UL,
                     FSP_ERR_INVALID_CHANNEL);
#endif
    rlin3_lin_extended_cfg_t const * const p_extend = (rlin3_lin_extended_cfg_t const *) p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* Initialize control block */
    if (RLIN323_CHANNEL == p_cfg->channel)
    {
        p_ctrl->p_reg = (R_RLN30_Type *) (R_RLN323_BASE); /* PRQA S 0306 # Advisory R-11.4 */
    }
    else
    {
        /* PRQA S 4501, 4558, 3469 ++ # DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC2, Advisory D-4.6 */
        p_ctrl->p_reg = ((R_RLN30_Type *) (R_RLN30_BASE + (RLIN3_REG_GROUP_GAP * ((uint32_t) p_cfg->channel % 2UL)) + /* PRQA S 0306 # Advisory R-11.4 */
                                           (RLIN3_REG_SIZE((uint32_t) p_cfg->channel) *
                                            ((uint32_t) p_cfg->channel / 2UL))));

        /* PRQA S 4501, 4558, 3469 -- */
    }

    p_ctrl->p_cfg             = p_cfg;
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;
    p_ctrl->event             = LIN_EVENT_NONE;
    p_ctrl->status            = RLIN3_LIN_MODE_OPERATION;
    p_ctrl->p_data            = NULL;
    p_ctrl->tx_src_bytes      = 0U;
    p_ctrl->rx_src_bytes      = 0U;
    p_ctrl->rx_bytes_received = 0U;

    p_ctrl->rx_checksum = 0U;

    /* Switch to LIN Operation Mode, Enable LIN reset mode */
    p_ctrl->p_reg->LCUC_b.OM1 = 1U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->LCUC_b.OM0 = 0U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM1, 1U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    r_rlin3_lin_hw_configure(p_ctrl);

    /*  LIN reset mode is canceled. */
    p_ctrl->p_reg->LCUC_b.OM0 = 1U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 1U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->p_reg->LEST_LIN = RLIN3_LIN_CLEAR_ERROR_FLAG;       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (0U == p_extend->interrupt_output)
    {
        R_BSP_IrqCfgEnable(p_extend->lin_irq, p_extend->lin_ipl, p_ctrl);
    }
    else
    {
        R_BSP_IrqCfgEnable(p_extend->txi_irq, p_extend->txi_ipl, p_ctrl);
        R_BSP_IrqCfgEnable(p_extend->rxi_irq, p_extend->rxi_ipl, p_ctrl);
        R_BSP_IrqCfgEnable(p_extend->eri_irq, p_extend->eri_ipl, p_ctrl);
    }

#if (BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ)                  /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    if (BSP_INTSEL_DISABLED != p_extend->intsel_irq_cfg) /* PRQA S 4342 # Advisory R-10.5 */
    {
        R_BSP_IntSel_CfgEnable(p_extend->intsel_irq_cfg);
    }
#endif

    /* Mark the control block as open */
    p_ctrl->open = RLIN3_LIN_OPEN;

#if !BSP_VLAB_HARDWARE_BASED_USED       /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    /* Start LIN Operating frame in slave mode */
    if (LIN_MODE_MASTER != p_ctrl->p_cfg->mode)
    {
        p_ctrl->p_reg->LTRC_b.FTS = 1U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Begins non-blocking transmission of a LIN start frame (break, sync and protected identifier).
 *
 * Implements @ref lin_api_t::startFrameWrite.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported by RLIN3_LIN.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_StartFrameWrite (lin_ctrl_t * const p_api_ctrl, uint8_t const frame_id) /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(frame_id);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Place the LIN node in bus sleep mode.
 *
 * Implements @ref lin_api_t::sleepEnter.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported by RLIN3_LIN.
 **********************************************************************************************************************/

fsp_err_t R_RLIN3_LIN_SleepEnter (lin_ctrl_t * const p_api_ctrl) /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    FSP_PARAMETER_NOT_USED(p_api_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Exit the bus sleep mode for LIN device.
 *
 * Implements @ref lin_api_t::sleepExit.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported by RLIN3_LIN.
 **********************************************************************************************************************/

fsp_err_t R_RLIN3_LIN_SleepExit (lin_ctrl_t * const p_api_ctrl) /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    FSP_PARAMETER_NOT_USED(p_api_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Begins non-blocking transmission of a LIN information frame.
 *
 * On successful information frame reception, the callback is called with
 * event @ref lin_event_t::LIN_EVENT_TX_DATA_COMPLETE.
 *
 * Implements @ref lin_api_t::write.
 *
 * Example:
 * @snippet r_rlin3_lin_example.c R_RLIN3_LIN_Write
 *
 * @retval  FSP_SUCCESS                  Data transmission started successfully.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_ASSERTION            Pointer to LIN control block, transfer parameters, or tx/rx buffer is NULL,
 *                                       or 0 bytes length provided
 * @retval  FSP_ERR_INVALID_ARGUMENT     ID out of range (unprotected ID must be less than 64)
 * @retval  FSP_ERR_IN_USE               A transmission or reception is currently in progress.
 *                                       Call R_RLIN3_LIN_CommunicationAbort to cancel it if desired, or wait for the
 *                                       current transfer operation to complete before starting a new one.
 * @return                               See @ref RENESAS_ERROR_CODES or functions called by this function for other possible
 *                                       return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_Write (lin_ctrl_t * const p_api_ctrl, const lin_transfer_params_t * const p_transfer_params) /* PRQA S 1505 # Advisory R-8.7 */
{
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) p_api_ctrl;                                 /* PRQA S 0316 # Advisory R-11.5 */

#if RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE

    /* Check parameters. */
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    FSP_ASSERT(NULL != p_transfer_params);
    FSP_ASSERT(NULL != p_transfer_params->p_buffer.p_data);
    FSP_ERROR_RETURN(p_transfer_params->id <= RLIN3_LIN_FRAME_ID_MASK, FSP_ERR_INVALID_ARGUMENT);

    /* If information buffer is still set, then a transmit/receive is already
     * in progress and we don't want to overwrite the buffer pointer. */
    FSP_ERROR_RETURN(NULL == p_ctrl->p_data, FSP_ERR_IN_USE);
#endif

    /* Prepare the information frame transmission state. */
    p_ctrl->p_data = p_transfer_params->p_buffer.p_data;

    /* Set the number of bytes to be transmitted. */
    if (p_transfer_params->num_bytes > RLIN3_LIN_MAX_FRAME_NUM_BYTES)
    {
        if ((LIN_MODE_SLAVE == p_ctrl->p_cfg->mode) ||
            ((uint32_t) RLIN3_LIN_TRANSFER_MODE_SEPARATE == p_ctrl->p_reg->LDFC_LIN_b.FSM)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* In slave mode or frame separate mode, the number of bytes to be transmitted may exceed 8 bytes. */
            p_ctrl->tx_src_bytes = p_transfer_params->num_bytes;
        }
        else
        {
            /* In master mode without frame separation, the number of bytes to be transmitted must not exceed 8 bytes. */
            p_ctrl->tx_src_bytes = RLIN3_LIN_MAX_FRAME_NUM_BYTES;
        }
    }
    else
    {
        p_ctrl->tx_src_bytes = p_transfer_params->num_bytes;
    }

    /* Check data to be transmitted is not the last data group when response data of 9 bytes or more */
    if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->tx_src_bytes)
    {
        p_ctrl->p_reg->LDFC_LIN_b.LSS  = 1U;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->LDFC_LIN_b.RFDL = RLIN3_LIN_MAX_FRAME_NUM_BYTES;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        p_ctrl->p_reg->LDFC_LIN_b.LSS  = 0U;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->LDFC_LIN_b.RFDL = (uint8_t) (p_ctrl->tx_src_bytes & 0x0FU); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Configure checksum for transmission */
    p_ctrl->p_reg->LDFC_LIN_b.CSM = (uint8_t) p_transfer_params->checksum_type;    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->LDFC_LIN_b.RFT = 1U;                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Store data into the registers*/
    r_rlin3_lin_transmission(p_ctrl);

    /* Start Response transmission in slave mode */
    if (LIN_MODE_MASTER != p_ctrl->p_cfg->mode)
    {
        if (0x1U == p_ctrl->p_reg->LRSS_b.RSDD) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* Response Space Dominant Detection */
            r_rlin3_lin_resp_space_dominant_handle(p_ctrl);
        }

#if BSP_VLAB_HARDWARE_BASED_USED        /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
        p_ctrl->p_reg->LTRC_b.FTS = 1U;
#else
        p_ctrl->p_reg->LTRC_b.RTS = 1U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif
    }
    else
    {
        /* Prepare the start frame transmission state */
        p_ctrl->last_pid = r_rlin3_lin_pid_calculate(p_transfer_params->id);

        /* Save the PID of the last attempted transmit */
        p_ctrl->p_reg->LIDB_LIN = p_ctrl->last_pid; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Start frame and response transmission*/
        p_ctrl->p_reg->LTRC_b.FTS = 1U;             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Begins non-blocking information frame reception to receive user specified number of bytes into destination buffer
 * pointer.
 *
 * On successful information frame reception, the callback is called with
 * event @ref lin_event_t::LIN_EVENT_RX_DATA_COMPLETE.
 *
 * Implements @ref lin_api_t::read.
 *
 * Example:
 * @snippet r_rlin3_lin_example.c R_RLIN3_LIN_Read
 *
 * @retval  FSP_SUCCESS                  Data reception started successfully.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_ASSERTION            Pointer to LIN control block, transfer parameters, or tx/rx buffer is NULL,
 *                                       or 0 bytes length provided
 * @retval  FSP_ERR_INVALID_ARGUMENT     ID out of range (unprotected ID must be less than 64)
 * @retval  FSP_ERR_IN_USE               A transmission or reception is currently in progress. Call
 *                                       R_RLIN3_LIN_CommunicationAbort to cancel it if desired, or wait for the current
 *                                       transfer operation to complete before starting a new one.
 * @return                               See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                       possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_Read (lin_ctrl_t * const p_api_ctrl, lin_transfer_params_t * const p_transfer_params) /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) p_api_ctrl;                          /* PRQA S 0316 # Advisory R-11.5 */

#if RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE

    /* Check parameters. */
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    FSP_ASSERT(NULL != p_transfer_params);
    FSP_ASSERT(NULL != p_transfer_params->p_buffer.p_data);
    FSP_ERROR_RETURN(p_transfer_params->id <= RLIN3_LIN_FRAME_ID_MASK, FSP_ERR_INVALID_ARGUMENT);

    /* If information buffer is still set, then a transmit/receive is already
     * in progress and we don't want to overwrite the buffer pointer. */
    FSP_ERROR_RETURN(NULL == p_ctrl->p_data, FSP_ERR_IN_USE);
#endif

    p_ctrl->p_data            = p_transfer_params->p_buffer.p_data;
    p_ctrl->rx_bytes_received = 0U;

    /* Set the number of bytes to be received. */
    if (p_transfer_params->num_bytes > RLIN3_LIN_MAX_FRAME_NUM_BYTES)
    {
        if ((LIN_MODE_SLAVE == p_ctrl->p_cfg->mode) ||
            ((uint32_t) RLIN3_LIN_TRANSFER_MODE_SEPARATE == p_ctrl->p_reg->LDFC_LIN_b.FSM)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* In slave mode or frame separate mode, the number of bytes to be received may exceed 8 bytes. */
            p_ctrl->rx_src_bytes = p_transfer_params->num_bytes;
        }
        else
        {
            /* In master mode without frame separation, the number of bytes to be received must not exceed 8 bytes. */
            p_ctrl->rx_src_bytes = RLIN3_LIN_MAX_FRAME_NUM_BYTES;
        }
    }
    else
    {
        p_ctrl->rx_src_bytes = p_transfer_params->num_bytes;
    }

    /* Check data to be received is not the last data group when response data of 9 bytes or more */
    if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->rx_src_bytes)
    {
        p_ctrl->p_reg->LDFC_LIN_b.LSS  = 1U;                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->LDFC_LIN_b.RFDL = RLIN3_LIN_MAX_FRAME_NUM_BYTES;         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        p_ctrl->p_reg->LDFC_LIN_b.LSS  = 0U;                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->LDFC_LIN_b.RFDL = p_ctrl->rx_src_bytes & 0xFU;           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Configure checksum for reception */
    p_ctrl->p_reg->LDFC_LIN_b.CSM = (uint8_t) p_transfer_params->checksum_type; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->LDFC_LIN_b.RFT = 0U;                                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Start Response reception in slave mode  */
    if (LIN_MODE_MASTER != p_ctrl->p_cfg->mode)
    {
#if  BSP_VLAB_HARDWARE_BASED_USED       /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
        /* Slave start read information frame in RX interrupt */
        p_ctrl->p_reg->LTRC_b.FTS = 1U;
#else
        p_ctrl->p_reg->LTRC_b.RTS = 1U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif
    }
    else
    {
        /* Prepare the start frame transmission state */
        p_ctrl->last_pid = r_rlin3_lin_pid_calculate(p_transfer_params->id);

        /* Save the PID of the last attempted transmit */
        p_ctrl->p_reg->LIDB_LIN = p_ctrl->last_pid; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Start frame and response transmission*/
        p_ctrl->p_reg->LTRC_b.FTS = 1U;             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * No LIN Response Request in case ID do not match
 *
 * @retval  FSP_SUCCESS                No Response Request successfully
 * @retval  FSP_ERR_ASSERTION          p_api_ctrl was NULL
 * @retval  FSP_ERR_NOT_OPEN           The control block has not been opened.
 * @retval  FSP_ERR_INVALID_MODE       Current Mode is not Slave Mode.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_NoResponseRequest (lin_ctrl_t * const p_api_ctrl)                        /* PRQA S 3673, 1503 # Advisory R-8.13, DP_MSR_R-2.1_UC1 */
{
    rlin3_lin_instance_ctrl_t const * p_ctrl = (rlin3_lin_instance_ctrl_t const *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE

    /* Check for null parameters */
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Function only supported for slave mode */
    FSP_ERROR_RETURN(LIN_MODE_MASTER != p_ctrl->p_cfg->mode, FSP_ERR_INVALID_MODE);
#endif

    /* Start No Response Request */
    p_ctrl->p_reg->LTRC_b.LNRR = 1U;   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Cancels in progress information frame read or write, or start frame read or write. Break field reception cannot be
 * cancelled. For slave nodes, reception of a new start frame reception is still enabled after a call to this function.
 *
 * Implements @ref lin_api_t::communicationAbort.
 *
 * @retval  FSP_SUCCESS              Data transfer aborted successfully or no transfer was in progress.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 * @retval  FSP_ERR_ASSERTION        Pointer to LIN control block is NULL.
 *
 * @return                           See @ref RENESAS_ERROR_CODES or functions called by this function for
 *                                   other possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_CommunicationAbort (lin_ctrl_t * const p_api_ctrl)           /* PRQA S 1505 # Advisory R-8.7 */
{
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Cancel information frame or start frame write */
    p_ctrl->tx_src_bytes = 0U;

    /* Cancel information frame read */
    p_ctrl->rx_src_bytes = 0U;

    /* Reset information buffer pointer */
    p_ctrl->p_data = NULL;

    p_ctrl->p_reg->LCUC_b.OM0 = 0U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->p_reg->LCUC_b.OM0 = 1U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 1U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Start a new frame in slave mode */
    if (LIN_MODE_MASTER != p_ctrl->p_cfg->mode)
    {
        p_ctrl->p_reg->LTRC_b.FTS = 1U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 *
 * Implements @ref lin_api_t::callbackSet.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            Pointer to LIN control block or callback is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 *
 * @return                               See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                       possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_CallbackSet (lin_ctrl_t * const          p_api_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                                   void (                    * p_callback)(lin_callback_args_t * p_args),
                                   void * const                p_context,
                                   lin_callback_args_t * const p_callback_memory)
{
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(NULL != p_callback);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Calculates baud rate and LIN timer (BFLW and TCSS) register settings. This function evaluates and determines the
 * most accurate settings for the baud rate and timer related registers.
 *
 * The LIN timer setting is used for break field transmission/detection. Because the timer setting is specified in terms
 * of bits, and the duration of a bit varies depending on baud rate, the baud rate register settings and timer register
 * settings are related. The smallest possible LIN timer divider which can achieve the desired break field bits setting
 * at the configured baud rate is selected to provide the highest measurement accuracy.
 *
 * The baud rate cannot be updated at runtime with this function. This function is provided to ease configuration of
 * the initial baud settings.
 *
 * @retval  FSP_SUCCESS                 Register settings updated in provided p_baud_setting
 * @retval  FSP_ERR_ASSERTION           p_baud_setting or p_ctrl was NULL
 * @retval  FSP_ERR_INVALID_ARGUMENT    Cannot configure baudrate with provided settings
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_BaudCalculate (lin_ctrl_t * const               p_api_ctrl,              /* PRQA S 3673, 1503 # Advisory R-8.13, DP_MSR_R-2.1_UC1 */
                                     uint32_t                         baudrate,
                                     rlin3_lin_baud_setting_t * const p_baud_setting)
{
    rlin3_lin_instance_ctrl_t const * p_ctrl = (rlin3_lin_instance_ctrl_t const *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_baud_setting);
    FSP_ERROR_RETURN((0U != baudrate), FSP_ERR_INVALID_ARGUMENT);
#endif

    uint32_t lbrp        = 0U;
    uint32_t divisor     = 0U;
    uint32_t err_divisor = 0U;
    int32_t  hit_bit_err = RLIN3_LIN_100_PERCENT_X1000;
    int32_t  bit_err     = RLIN3_LIN_100_PERCENT_X1000;
    uint8_t  lprs        = 0U;
    uint8_t  bit_samp    = 0U;
    uint32_t freq_hz     = 0U;

    /* Determine the RLIN3 clock frequency based on the channel */
    if (RLIN323_CHANNEL != p_ctrl->p_cfg->channel)
    {
        freq_hz = RLIN3_LIN_CFG_CLOCK;
    }
    else
    {
        freq_hz = RLIN3_LIN_CFG_CLOCK_CH23;
    }

    p_baud_setting->prescaler_setting.prescaler = RLIN3_LIN_MAX_MASTER_BAUDRATE_PRESCALER;
    p_baud_setting->prescaler_clock             = RLIN3_LIN_MAX_CLKDIV_SEL - 1U;
    p_baud_setting->bit_sampling                = RLIN3_LIN_MAX_BITSAMPLING;
    if (LIN_MODE_MASTER == p_ctrl->p_cfg->mode)
    {
        for (lprs = 0U; lprs < RLIN3_LIN_MAX_CLKDIV_SEL; lprs++)
        {
            bit_samp = RLIN3_LIN_MAX_BITSAMPLING;
            divisor  =
                (((uint32_t) bit_samp + 1U) * baudrate *
                 (uint32_t) rlin3_lin_system_clock[p_baud_setting->clock_select]) *
                (uint32_t) (rlin3_lin_clock_prescaler[lprs]);
            lbrp = freq_hz / divisor;
            if (lbrp > RLIN3_LIN_MAX_MASTER_BAUDRATE_PRESCALER)
            {
                continue;
            }

            if (((freq_hz % divisor) > (divisor / 2U)) || (0U == lbrp))
            {
                lbrp += 1U;
            }

            err_divisor = freq_hz /
                          ((uint32_t) (rlin3_lin_clock_prescaler[lprs]) * ((uint32_t) bit_samp + 1U) *
                           rlin3_lin_system_clock[p_baud_setting->clock_select] * lbrp);
            bit_err = (int32_t) (((((int64_t) baudrate) * (int64_t) RLIN3_LIN_100_PERCENT_X1000) /
                                  (int64_t) err_divisor) - (int64_t) RLIN3_LIN_100_PERCENT_X1000);
            if (bit_err < 0)
            {
                bit_err = -bit_err;
            }

            if (bit_err < hit_bit_err)
            {
                hit_bit_err = bit_err;
                p_baud_setting->bit_sampling                = bit_samp & 0xFU;
                p_baud_setting->prescaler_clock             = lprs & 0x7U;
                p_baud_setting->prescaler_setting.prescaler = (uint16_t) lbrp - 1U;
            }
        }
    }
    else
    {
        for (lprs = 0U; lprs < RLIN3_LIN_MAX_CLKDIV_SEL; lprs++)
        {
            bit_samp = RLIN3_LIN_MAX_BITSAMPLING;
            divisor  =
                (((uint32_t) bit_samp + 1U) * baudrate *
                 (uint32_t) rlin3_lin_system_clock[p_baud_setting->clock_select]) *
                (uint32_t) (rlin3_lin_clock_prescaler[lprs]);
            lbrp = freq_hz / divisor;
            if (lbrp > RLIN3_LIN_MAX_SLAVE_BAUDRATE_PRESCALER)
            {
                continue;
            }

            if (((freq_hz % divisor) > (divisor / 2U)) || (0U == lbrp))
            {
                lbrp += 1U;
            }

            err_divisor = freq_hz /
                          ((uint32_t) (rlin3_lin_clock_prescaler[lprs]) * ((uint32_t) bit_samp + 1U) *
                           rlin3_lin_system_clock[p_baud_setting->clock_select] * lbrp);
            bit_err = (int32_t) (((((int64_t) baudrate) * (int64_t) RLIN3_LIN_100_PERCENT_X1000) /
                                  (int64_t) err_divisor) - (int64_t) RLIN3_LIN_100_PERCENT_X1000);
            if (bit_err < 0)
            {
                bit_err = -bit_err;
            }

            if (bit_err < hit_bit_err)
            {
                hit_bit_err = bit_err;
                p_baud_setting->bit_sampling                = bit_samp & 0xFU;
                p_baud_setting->prescaler_clock             = lprs & 0x7U;
                p_baud_setting->prescaler_setting.prescaler = (uint16_t) lbrp - 1U;
            }
        }
    }

    if (LIN_MODE_MASTER == p_ctrl->p_cfg->mode)
    {
        FSP_ERROR_RETURN((hit_bit_err <= RLIN3_LIN_MASTER_MAX_BAUD_RATE_ERROR_X1000), FSP_ERR_INVALID_ARGUMENT);
    }
    else
    {
        FSP_ERROR_RETURN((hit_bit_err <= RLIN3_LIN_SLAVE_MAX_BAUD_RATE_ERROR_X1000), FSP_ERR_INVALID_ARGUMENT);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the the ID filter settings for filtering control field 1 (PID byte).
 *
 * @retval FSP_ERR_UNSUPPORTED          API not supported by RLIN3_LIN
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_IdFilterSet (lin_ctrl_t * const p_api_ctrl, uint8_t frame_id) /* PRQA S 3673, 1503 # Advisory R-8.13, DP_MSR_R-2.1_UC1 */
{
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(frame_id);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Returns the currently configured ID filter settings.
 *
 * @retval  FSP_SUCCESS              ID filter updated successfully.
 * @retval  FSP_ERR_ASSERTION        Pointer to LIN control block or p_config is NULL.
 * @retval  FSP_ERR_INVALID_MODE     Function called by master node (not supported for master nodes)
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 *
 * @return                           See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                   possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_IdFilterGet (lin_ctrl_t * const p_api_ctrl, uint8_t * const p_frame_id)  /* PRQA S 3673, 1503 # Advisory R-8.13, DP_MSR_R-2.1_UC1 */
{
    rlin3_lin_instance_ctrl_t const * p_ctrl = (rlin3_lin_instance_ctrl_t const *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE

    /* Check for null parameters */
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_ctrl->p_cfg);
    FSP_ASSERT(p_frame_id);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Function only supported for slave mode */
    FSP_ERROR_RETURN(LIN_MODE_MASTER != p_ctrl->p_cfg->mode, FSP_ERR_INVALID_MODE);
#endif
    *p_frame_id = p_ctrl->last_pid;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Switch Wakeup Mode or Operation Mode
 *
 * @retval  FSP_SUCCESS              ID filter updated successfully.
 * @retval  FSP_ERR_ASSERTION        Pointer to LIN control block or p_config is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 * @retval  FSP_ERR_IN_USE           Master communication is in progress
 *
 * @return                           See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                  possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_ModeSwitch (lin_ctrl_t * const p_api_ctrl, rlin3_lin_mode_t wk_status) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) p_api_ctrl;           /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN((LIN_MODE_MASTER != p_ctrl->p_cfg->mode) || (0U == p_ctrl->p_reg->LTRC_b.FTS), FSP_ERR_IN_USE); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

    /* Switch to LIN Reset */
    p_ctrl->p_reg->LCUC_b.OM0 = 0U;                                                                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 0U);                                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->status = wk_status;

    /* Switch to Wakeup/Operation mode */
    p_ctrl->p_reg->LCUC = (uint8_t) ((uint32_t) wk_status << R_RLN30_LCUC_OM1_Pos) | 0x1U;                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST, (uint8_t) (((uint32_t) wk_status << R_RLN30_LCUC_OM1_Pos) | 0x1U)); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (((LIN_MODE_MASTER == p_ctrl->p_cfg->mode) && (RLIN3_LIN_MODE_WAKEUP == wk_status)) ||
        (LIN_MODE_SLAVE == p_ctrl->p_cfg->mode))
    {
        p_ctrl->p_reg->LCUC_b.OM0 = 0U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        p_ctrl->p_reg->LDFC_LIN_b.RFT = 0U;                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        p_ctrl->p_reg->LCUC_b.OM0 = 1U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 1U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        p_ctrl->p_reg->LTRC_b.FTS = 1U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Switch transfer standard or separate Mode
 *
 * @retval  FSP_SUCCESS              ID filter updated successfully.
 * @retval  FSP_ERR_ASSERTION        Pointer to LIN control block or p_config is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 * @retval  FSP_ERR_INVALID_ARGUMENT LIN is in Slave(this function is only supported in master mode).
 *
 * @return                           See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                   possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_TransferModeSwitch (lin_ctrl_t * const p_api_ctrl, rlin3_lin_transfer_mode_t transfer_mode) /* PRQA S 3673, 1503 # Advisory R-8.13, DP_MSR_R-2.1_UC1 */
{
    rlin3_lin_instance_ctrl_t const * p_ctrl = (rlin3_lin_instance_ctrl_t const *) p_api_ctrl;                    /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN((LIN_MODE_MASTER == p_ctrl->p_cfg->mode), FSP_ERR_INVALID_ARGUMENT);
#endif

    /* Switch to standard/separate mode */
    p_ctrl->p_reg->LDFC_LIN_b.FSM = (uint8_t) transfer_mode;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    FSP_HARDWARE_REGISTER_WAIT((uint8_t) transfer_mode, p_ctrl->p_reg->LDFC_LIN_b.FSM); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start Wake up transmission/reception.
 *
 * @retval  FSP_SUCCESS              ID filter updated successfully.
 * @retval  FSP_ERR_ASSERTION        Pointer to LIN control block or p_config is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 * @retval  FSP_ERR_IN_USE           Communication is in progress
 *
 * @return                           See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                   possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_WakeupSend (lin_ctrl_t * const p_api_ctrl)                               /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    rlin3_lin_instance_ctrl_t const * p_ctrl = (rlin3_lin_instance_ctrl_t const *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(RLIN3_LIN_MODE_WAKEUP == p_ctrl->status, FSP_ERR_IN_USE);
#endif

    /* Start Wakeup Transmission/Reception */
    p_ctrl->p_reg->LCUC_b.OM0 = 0U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->p_reg->LDFC_LIN_b.RFT = 1U;                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->p_reg->LCUC_b.OM0 = 1U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 1U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->p_reg->LTRC_b.FTS = 1U;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Init Self-Test Mode
 *
 *  * Example:
 * @snippet r_rlin3_lin_example.c R_RLIN3_LIN_InitSelfTestMode
 *
 * @retval  FSP_SUCCESS                  ID filter updated successfully.
 * @retval  FSP_ERR_ASSERTION            Pointer to LIN control block or p_config is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_IN_USE               Master communication is in progress
 * @retval  FSP_ERR_INVALID_ARGUMENT     ID out of range (unprotected ID must be less than 64)
 * @return                               See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                       possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_InitSelfTestMode (lin_ctrl_t * const            p_api_ctrl,                                 /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
                                        lin_transfer_params_t * const p_transfer_params,                          /* PRQA S 3673 # Advisory R-8.13 */
                                        rlin3_lin_wk_dir_t            wk_dir)
{
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) p_api_ctrl;                                /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    rlin3_lin_extended_cfg_t const * const p_extend = (rlin3_lin_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN((LIN_MODE_SLAVE_AUTO != p_extend->slave_mode), FSP_ERR_IN_USE);
    FSP_ASSERT(NULL != p_transfer_params);
    FSP_ASSERT(NULL != p_transfer_params->p_buffer.p_data);
    FSP_ERROR_RETURN(p_transfer_params->id <= RLIN3_LIN_FRAME_ID_MASK, FSP_ERR_INVALID_ARGUMENT);
#endif

    /* Switch to LIN Reset Mode */
    p_ctrl->p_reg->LCUC_b.OM0 = 0U;                                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 0U);                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Enable Self Test Mode - Write command 1 */
    p_ctrl->p_reg->LSTC = LIN_SELFTEST_COMMAND1;                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Enable Self Test Mode - Write command 2 */
    p_ctrl->p_reg->LSTC = LIN_SELFTEST_COMMAND2;                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Enable Self Test Mode - Write command 3 */
    p_ctrl->p_reg->LSTC = LIN_SELFTEST_ENABLE;                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Switch to LIN Self-Test Mode */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LSTC_b.LSTM, LIN_SELFTEST_ENABLE); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Release from the LIN reset mode */
    p_ctrl->p_reg->LCUC_b.OM0 = 1U;                                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 1U);                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Configure checksum for transmission */
    p_ctrl->p_reg->LDFC_LIN_b.CSM = (uint8_t) p_transfer_params->checksum_type;  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (RLIN3_LIN_WK_DIR_TRANSMISSION == wk_dir)
    {
        /* Prepare the information frame transmission state. */
        p_ctrl->p_data       = p_transfer_params->p_buffer.p_data;
        p_ctrl->tx_src_bytes = p_transfer_params->num_bytes;

        /* Configure data field */
        p_ctrl->p_reg->LDFC_LIN_b.LSS  = 0U;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->LDFC_LIN_b.RFDL = (uint8_t) (p_ctrl->tx_src_bytes & 0x0FU);         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* handle multi interrupts using same interrupt processing(IRS) */
        if ((0U == p_ctrl->p_reg->LMD_b.LIOS) || (LIN_MODE_MASTER == p_ctrl->p_cfg->mode)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* Disable interrupt transmit header success*/
            p_ctrl->p_reg->LIE_b.SHIE = 0U;                                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Store data into the registers*/
        r_rlin3_lin_transmission(p_ctrl);
    }
    else
    {
        /* Prepare the information frame transmission state. */
        p_ctrl->p_data            = p_transfer_params->p_buffer.p_data;
        p_ctrl->rx_src_bytes      = p_transfer_params->num_bytes;
        p_ctrl->rx_bytes_received = 0U;

        /* Configure checksum for transmission */
        p_ctrl->p_reg->LDFC_LIN_b.CSM = (uint8_t) p_transfer_params->checksum_type;        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Configure data field */
        p_ctrl->p_reg->LDFC_LIN_b.LSS  = 0U;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->LDFC_LIN_b.RFDL = (uint8_t) (p_ctrl->rx_src_bytes & 0x0FU);         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* handle multi interrupts using same interrupt processing(IRS) */
        if ((0U == p_ctrl->p_reg->LMD_b.LIOS) || (LIN_MODE_MASTER != p_ctrl->p_cfg->mode)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* Disable interrupt receive header success*/
            p_ctrl->p_reg->LIE_b.SHIE = 0U;                                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Store data into the registers*/
        r_rlin3_lin_reception(p_ctrl);

        p_ctrl->rx_checksum = r_rlin3_checksum_calculate(p_transfer_params->id,
                                                         p_transfer_params->checksum_type,
                                                         p_transfer_params->p_buffer.p_data,
                                                         p_transfer_params->num_bytes);

        p_ctrl->p_reg->LCBR = p_ctrl->rx_checksum;    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Set up Direction */
    p_ctrl->p_reg->LDFC_LIN_b.RFT = (uint8_t) wk_dir; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Prepare the start frame transmission state */
    p_ctrl->last_pid = r_rlin3_lin_pid_calculate(p_transfer_params->id);

    /* Save the PID of the last attempted transmit */
    p_ctrl->p_reg->LIDB_LIN = p_ctrl->last_pid; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Start frame and response transmission or reception*/
    p_ctrl->p_reg->LTRC_b.FTS = 1U;             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * DeInit Self-Test Mode
 *
 * @retval  FSP_SUCCESS              ID filter updated successfully.
 * @retval  FSP_ERR_ASSERTION        Pointer to LIN control block or p_config is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 * @retval  FSP_ERR_IN_USE           Master communication is in progress
 *
 * @return                           See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                   possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_DeInitSelfTestMode (lin_ctrl_t * const p_api_ctrl)                                    /* PRQA S 3673, 1503 # Advisory R-8.13, DP_MSR_R-2.1_UC1 */
{
    rlin3_lin_instance_ctrl_t const * p_ctrl = (rlin3_lin_instance_ctrl_t const *) p_api_ctrl;              /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    rlin3_lin_extended_cfg_t const * p_extend = (rlin3_lin_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN((LIN_MODE_SLAVE_AUTO != p_extend->slave_mode), FSP_ERR_IN_USE);
#endif

    /* Switch to LIN Reset Mode */
    p_ctrl->p_reg->LCUC_b.OM0 = 0U;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 0U);                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Cancel to LIN Self-Test Mode */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LSTC_b.LSTM, LIN_SELFTEST_DISABLE); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read the currently data of self-test mode.
 *
 * @retval  FSP_SUCCESS              ID filter updated successfully.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 * @retval  FSP_ERR_ASSERTION        Pointer to LIN control block or p_config is NULL.
 * @retval  FSP_ERR_IN_USE           Master communication is in progress
 *
 * @return                           See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                   possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_RLIN3_LIN_SelftestDataRead (lin_ctrl_t * const p_api_ctrl, /* PRQA S 3673, 1503 # Advisory R-8.13, DP_MSR_R-2.1_UC1 */
                                        uint8_t          * p_buffer,
                                        uint8_t          * p_data,
                                        uint8_t          * p_selftest_checksum)
{
    rlin3_lin_instance_ctrl_t const * p_ctrl = (rlin3_lin_instance_ctrl_t const *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE

    /* Check for null parameters */
    FSP_ASSERT(p_ctrl);
    rlin3_lin_extended_cfg_t const * p_extend = (rlin3_lin_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Function only supported for slave mode */
    FSP_ERROR_RETURN((LIN_MODE_SLAVE_AUTO != p_extend->slave_mode), FSP_ERR_IN_USE);
#endif

    /*Read buffer after self-test mode*/
    *p_buffer = ~(p_ctrl->p_reg->LIDB_LIN); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    *p_buffer = *p_buffer & RLIN3_LIN_FRAME_ID_MASK;

    /*Read data after self-test mode*/
    for (uint8_t data_idx = 0U; data_idx < RLIN3_LIN_MAX_FRAME_NUM_BYTES; data_idx++)
    {
        *p_data = ~(p_ctrl->p_reg->LDBR[data_idx]); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_data++;                                   /* PRQA S 1338 # Advisory R-17.8 */
    }

    /*Read checksum after self-test mode*/
    *p_selftest_checksum = ~(p_ctrl->p_reg->LCBR);  /* PRQA S 1710,0662 # Advisory D-4.5, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Closes the LIN driver.
 *
 * Implements @ref lin_api_t::close.
 *
 * Example:
 * @snippet r_rlin3_lin_example.c R_RLIN3_LIN_Close
 *
 * @retval  FSP_SUCCESS              Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION        Pointer to LIN control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 *
 * @return                           See @ref RENESAS_ERROR_CODES or functions called by this function for other possible
 *                                   return codes.
 **********************************************************************************************************************/

fsp_err_t R_RLIN3_LIN_Close (lin_ctrl_t * const p_api_ctrl)                        /* PRQA S 1505 # Advisory R-8.7 */
{
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (RLIN3_LIN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_ctrl->p_cfg->p_extend);
    FSP_ERROR_RETURN(RLIN3_LIN_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    rlin3_lin_extended_cfg_t const * const p_extend = (rlin3_lin_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    p_ctrl->p_reg->LCUC_b.OM0 = 0U;                                                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->LMST_b.OMM0, 0U);                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->LIE = 0U;                                                                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (0U == p_extend->interrupt_output)
    {
        R_BSP_IrqDisable(p_extend->lin_irq);
    }
    else
    {
        R_BSP_IrqDisable(p_extend->txi_irq);
        R_BSP_IrqDisable(p_extend->rxi_irq);
        R_BSP_IrqDisable(p_extend->eri_irq);
    }

#if (BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ)                  /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    if (BSP_INTSEL_DISABLED != p_extend->intsel_irq_cfg) /* PRQA S 4342 # Advisory R-10.5 */
    {
        R_BSP_IntSel_CfgDisable(p_extend->intsel_irq_cfg);
    }
#endif

    p_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup RLIN3_LIN)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes the RLIN common
 *
 * @param[in]  p_ctrl                    Pointer to driver control block
 **********************************************************************************************************************/
static void r_rlin3_lin_hw_configure (rlin3_lin_instance_ctrl_t * const p_ctrl)                           /* PRQA S 3673 # Advisory R-8.13 */
{
    lin_cfg_t const * p_cfg = p_ctrl->p_cfg;
    rlin3_lin_extended_cfg_t const * const p_extend = (rlin3_lin_extended_cfg_t const *) p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    R_RLN30_Type * p_reg = p_ctrl->p_reg;

    /* Update RLIN3 Baud Rate Setting Register */
    r_rlin3_lin_baud_set(p_ctrl->p_reg, &(p_extend->baud_setting));

    /* Configure Interrupt Output, Noise Filter, Master/Slave Mode LIN Mode Register */
    if (LIN_MODE_MASTER == p_cfg->mode)
    {
        p_reg->LMD = (uint8_t) (((uint32_t) p_extend->noise_filter_disable << R_RLN30_LMD_LRDNFS_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                ((uint32_t) p_extend->interrupt_output << R_RLN30_LMD_LIOS_Pos) |
                                (((uint32_t) p_cfg->mode & (uint32_t) RLIN3_LIN_MODE_MASTER) << R_RLN30_LMD_LMD_Pos));
    }
    else
    {
        p_reg->LMD = (uint8_t) (((uint32_t) p_extend->noise_filter_disable << R_RLN30_LMD_LRDNFS_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                ((uint32_t) p_extend->interrupt_output << R_RLN30_LMD_LIOS_Pos) |
                                ((uint32_t) p_extend->slave_mode << R_RLN30_LMD_LMD_Pos));
    }

    /* Enable Header/Frame Transmission/Reception, Error Detection Interrupt */
    p_reg->LIE = RLIN3_LIN_OPERATING_INTERRUPT_ENABLE;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Config wake-up Transmission Low Level Width */
    p_reg->LWUP = p_extend->wakeup_setting.wk_width << R_RLN30_LWUP_WUTL_Pos; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Config inter-byte space and response space width */
    p_reg->LSC = (p_extend->inter_byte_space << R_RLN30_LSC_IBS_Pos) |        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                 (p_extend->response_space << R_RLN30_LSC_IBHS_Pos);

    if (LIN_MODE_MASTER == p_cfg->mode)
    {
        /* Enable/Disable Frame Separate Mode */
        p_reg->LDFC_LIN = p_extend->frame_separate_enable << R_RLN30_LDFC_LIN_FSM_Pos;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Config wake-up source clock, enable error interrupt, Transmission Break Delimiter in Master */
        p_reg->LWBR_b.LWBR0 = p_extend->wakeup_setting.wk_clock_src;                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_reg->LEDE_LIN     = p_extend->err_detect_cfg.err_detect_enb & RLIN3_LIN_LEDE_MASTER_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_reg->LBFC         = (p_extend->bfw_high << R_RLN30_LBFC_MASTER_BDT_Pos) |                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                              (p_extend->bfw_low << R_RLN30_LBFC_MASTER_BLT_Pos);

        /* Init Tranmission for Master Response Field Direction */
        p_reg->LDFC_LIN_b.RFT = 1U;                                                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        /* Config wake-up source clock, enable error interrupt, Reception Break Delimiter in Slave */
        p_reg->LEDE_LIN = p_extend->err_detect_cfg.err_detect_enb & RLIN3_LIN_LEDE_SLAVE_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_reg->LBFC     = p_extend->bfw_low << R_RLN30_LBFC_MASTER_BLT_Pos;                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Init Reception for Slave Response Field Direction  */
        p_reg->LDFC_LIN_b.RFT = 0U;                                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
}

/*******************************************************************************************************************//**
 * Calculates the protected identifier.
 *
 * Implements the algorithm according to LIN Protocol Specification Rev. 2.2A pg.53.
 *
 * The PID field is 8 bit field: [p1 p0 frame_id5 frame_id4 frame_id3 frame_id2 frame_id1 frame_id0].
 * P1 is bit 7 and is calculated as frame_id1 XOR frame_id3 XOR frame_id4 XOR frame_id5.
 * P0 is bit 6 and is calculated as frame_id0 XOR frame_id1 XOR frame_id2 XOR frame_id4.
 *
 * @param[in] frame_id LIN unprotected frame identifier
 *
 * @retval             LIN protected frame identifier result
 **********************************************************************************************************************/
static uint8_t r_rlin3_lin_pid_calculate (uint8_t frame_id)
{
    /* ID is 6 bits */
    uint8_t pid = frame_id & RLIN3_LIN_FRAME_ID_MASK;

    /* P0 = id0 ^ id1 ^ id2 ^ id4*/
    uint8_t tmp = (((frame_id >> 0U) ^ (frame_id >> 1U) ^ (frame_id >> 2U) ^ (frame_id >> 4U)) & 0x01U);
    pid |= (uint8_t) (tmp << 6U);

    /* P1 = inverse of id1 ^ id3 ^ id4 ^ id5 */
    tmp  = (uint8_t) ~(((frame_id >> 1U) ^ (frame_id >> 3U) ^ (frame_id >> 4U) ^ (frame_id >> 5U)) & 0x01U);
    pid |= (uint8_t) (tmp << 7U);

    return pid;
}

/*******************************************************************************************************************//**
 * Calculates the checksum
 * This is a different implementation of the algorithm than the SCI_B_LIN implementation.
 *
 * @returns the checksum
 ***********************************************************************************************************************/
static uint8_t r_rlin3_checksum_calculate (uint8_t             frame_id,
                                           lin_checksum_type_t checksum_type,
                                           uint8_t           * p_data, /* PRQA S 3673 # Advisory R-8.13 */
                                           uint16_t            data_len)
{
    uint32_t checksum = 0U;
    uint8_t  parity_0;
    uint8_t  parity_1;

    if (LIN_CHECKSUM_TYPE_ENHANCED == checksum_type)
    {
        /* calculate ID[5:0] bit parity */
        frame_id = frame_id & RLIN3_LIN_FRAME_ID_MASK; /* PRQA S 1338 # Advisory R-17.8 */

        /* Calculate parity */
        parity_0 =
            ((frame_id >> 0U) & 1U) ^ ((frame_id >> 1U) & 1U) ^ ((frame_id >> 2U) & 1U) ^ ((frame_id >> 4U) & 1U);
        parity_1 =
            ((frame_id >> 1U) & 1U) ^ ((frame_id >> 3U) & 1U) ^ ((frame_id >> 4U) & 1U) ^ ((frame_id >> 5U) & 1U);
        parity_1 = ~parity_1 & 1U;

        /* set initial value of checksum */
        checksum = ((uint32_t) parity_1 << 7U) | ((uint32_t) parity_0 << 6U) | (uint32_t) frame_id;
    }

    /* Sum all bytes (1's complement sum). Accumulate all carries, then add them at the end. */
    for (uint8_t data_idx = 0U; data_idx < data_len; data_idx++)
    {
        checksum += *p_data;
        p_data++;                      /* PRQA S 1338 # Advisory R-17.8 */
    }

    /* add 8-bit overflow */
    checksum = ((checksum & RLIN3_LIN_MAX_CHECKSUM) + ((checksum >> 8U) & RLIN3_LIN_MAX_CHECKSUM)) &
               RLIN3_LIN_MAX_CHECKSUM;

    /* complement of the accumulated value */
    checksum = ~checksum & RLIN3_LIN_MAX_CHECKSUM;

    return (uint8_t) checksum;
}

/*******************************************************************************************************************//**
 * Changes baud rate based on predetermined register settings.
 *
 * @param[in]  p_rlin3_reg       Base pointer for RLIN3 registers
 * @param[in]  p_baud_setting    Pointer to other divisor related settings
 *
 * @note                         Must to be in reset mode (RLN3nLMST.OM0 = 0) when calling this function.
 **********************************************************************************************************************/
static void r_rlin3_lin_baud_set (R_RLN30_Type * p_rlin3_reg, rlin3_lin_baud_setting_t const * const p_baud_setting)
{
    p_rlin3_reg->LWBR = (uint8_t) (((uint32_t) p_baud_setting->bit_sampling << R_RLN30_LWBR_NSPB_Pos) |          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                   ((uint32_t) p_baud_setting->prescaler_clock << R_RLN30_LWBR_LPRS_Pos));

    p_rlin3_reg->LMD_b.LCKS = (uint8_t) p_baud_setting->clock_select;                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (RLIN3_LIN_CLOCK_SOURCE_FD == (rlin3_lin_clock_source_t) p_baud_setting->clock_select)                    /* PRQA S 4332 # Advisory R-10.5 */
    {
        p_rlin3_reg->LBRP1 = (uint8_t) (p_baud_setting->prescaler_setting.prescaler & RLIN3_LIN_MAX_PRESCALER1); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        p_rlin3_reg->LBRP01 = p_baud_setting->prescaler_setting.prescaler;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
}

/*******************************************************************************************************************//**
 * Handle response space dominant detection
 * Slave node only
 *
 * @param[in]   p_instance_ctrl           Pointer to control block of LIN driver.
 **********************************************************************************************************************/
static void r_rlin3_lin_resp_space_dominant_handle (rlin3_lin_instance_ctrl_t * p_instance_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    /* User custom */
#if RLIN3_LIN_CFG_CUSTOM_RESP_SPACE_DOMI_DETECT_ENABLE                                           /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
/* PRQA S 0316 ++ # Advisory R-11.5 */
    rlin3_lin_extended_cfg_t const * const p_extend =
        (rlin3_lin_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;

/* PRQA S 0316 -- */

    if (NULL != p_extend)
    {
        if (NULL != p_extend->p_resp_space_handler)
        {
            p_extend->p_resp_space_handler(p_instance_ctrl);
        }
    }

#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#endif
}

/*******************************************************************************************************************//**
 * Move data to LIN buffer.
 *
 * @param[in]  p_ctrl       Pointer to driver control block
 **********************************************************************************************************************/
static void r_rlin3_lin_transmission (rlin3_lin_instance_ctrl_t * const p_ctrl)
{
    uint8_t data_length = RLIN3_LIN_MAX_FRAME_NUM_BYTES;

    if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->tx_src_bytes)
    {
        data_length = RLIN3_LIN_MAX_FRAME_NUM_BYTES;
    }
    else
    {
        data_length = (uint8_t) p_ctrl->tx_src_bytes;
    }

    for (uint8_t data_idx = 0U; data_idx < data_length; data_idx++)
    {
        p_ctrl->p_reg->LDBR[data_idx] = *(p_ctrl->p_data); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->tx_src_bytes         -= 1U;
        p_ctrl->p_data               += 1U;                /* PRQA S 0489 # Advisory R-18.4 */
    }
}

/*******************************************************************************************************************//**
 * Move data to LIN buffer for support selftest.
 *
 * @param[in]  p_ctrl       Pointer to driver control block
 **********************************************************************************************************************/
static void r_rlin3_lin_reception (rlin3_lin_instance_ctrl_t * const p_ctrl)
{
    uint8_t data_length = RLIN3_LIN_MAX_FRAME_NUM_BYTES;
    data_length = (uint8_t) p_ctrl->rx_src_bytes;

    for (uint8_t data_idx = 0U; data_idx < data_length; data_idx++)
    {
        p_ctrl->p_reg->LDBR[data_idx] = *(p_ctrl->p_data); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->rx_src_bytes         -= 1U;
        p_ctrl->p_data               += 1U;                /* PRQA S 0489 # Advisory R-18.4 */
    }

    for (uint8_t data_idx = 0U; data_idx < data_length; data_idx++)
    {
        p_ctrl->p_data -= 1U;          /* PRQA S 0489 # Advisory R-18.4 */
    }
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to LIN instance control block
 * @param[in]     event      Event code
 **********************************************************************************************************************/
static void r_rlin3_lin_call_callback (rlin3_lin_instance_ctrl_t * p_ctrl, lin_event_t event) /* PRQA S 3673 # Advisory R-8.13 */
{
    lin_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    lin_callback_args_t * p_args = p_ctrl->p_callback_memory;
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

    p_args->channel        = p_ctrl->p_cfg->channel;
    p_args->event          = event;
    p_args->pid            = p_ctrl->last_pid;
    p_args->p_context      = p_ctrl->p_context;
    p_args->bytes_received = p_ctrl->rx_bytes_received;
    p_args->checksum       = p_ctrl->rx_checksum;

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
 * Interrupt service routines
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * TXI interrupt processing for LIN mode. The TXI interrupt fires after LIN frame or header transmission succesfully,
 *  and the next data can be written.
 *
 * @retval    none
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Interrupt service routines
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void rlin3_lin_txi_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    lin_event_t       event = LIN_EVENT_NONE;
    lin_cfg_t const * p_cfg = p_ctrl->p_cfg;

    if ((p_ctrl->p_reg->LST & R_RLN30_LST_FTC_Msk) != 0U) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    /*Frame transmit successfully*/
    {
        /*Clear flags and set event*/
        p_ctrl->p_reg->LST_b.FTC = 0U;                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        if (0U < p_ctrl->tx_src_bytes)
        /*Continue transmission*/
        {
            if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->tx_src_bytes)
            {
                p_ctrl->p_reg->LDFC_LIN_b.LSS = 1U;                           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
            else
            {
                p_ctrl->p_reg->LDFC_LIN_b.LSS  = 0U;                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_ctrl->p_reg->LDFC_LIN_b.RFDL = p_ctrl->tx_src_bytes & 0xFU; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }

            r_rlin3_lin_transmission(p_ctrl);

            /* Start Response transmission */
            p_ctrl->p_reg->LTRC_b.RTS = 1U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else
        {
            p_ctrl->p_data = NULL;
        }

        if (LIN_MODE_SLAVE == p_cfg->mode)
        {
            if (0x1U == p_ctrl->p_reg->LRSS_b.RSDD) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                /* Response Space Dominant Detection */
                r_rlin3_lin_resp_space_dominant_handle(p_ctrl);
            }
        }

        if (RLIN3_LIN_MODE_OPERATION == p_ctrl->status)
        {
            event = LIN_EVENT_TX_DATA_COMPLETE;
        }
        else
        {
            event = LIN_EVENT_TX_WAKEUP_COMPLETE;
        }
    }
    else
    {
        if (LIN_MODE_MASTER == p_cfg->mode)
        /* Header transmission successfully */
        {
            if ((p_ctrl->p_reg->LST & R_RLN30_LST_HTRC_Msk) != 0U) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                /*Clear flags and set event*/
                p_ctrl->p_reg->LST_b.HTRC = 0U;                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /* If the master is in the separate mode, the RTS bit must be set to start response transmission/reception. */
                if (1U == p_ctrl->p_reg->LDFC_LIN_b.FSM)           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                {
                    p_ctrl->p_reg->LTRC_b.RTS = 1U;                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                }

                event = LIN_EVENT_TX_HEADER_COMPLETE;
            }
        }
    }

    /* If callback event occurred, call the callback */
    if (LIN_EVENT_NONE != event)
    {
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * RXI interrupt processing for LIN mode. RXI interrupt fires when LIN frame or header reception successfully.
 *
 * @retval    none
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void rlin3_lin_rxi_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Process the received data and determine whether there is a callback event present */
    lin_event_t       event = LIN_EVENT_NONE;
    lin_cfg_t const * p_cfg = p_ctrl->p_cfg;

    if ((p_ctrl->p_reg->LST & R_RLN30_LST_FRC_Msk) != 0U) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    /*Frame reception successfully*/
    {
        uint8_t data_length = RLIN3_LIN_MAX_FRAME_NUM_BYTES;

        if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->rx_src_bytes)
        {
            data_length = RLIN3_LIN_MAX_FRAME_NUM_BYTES;
        }
        else
        {
            data_length = (uint8_t) p_ctrl->rx_src_bytes;
        }

        /* Check pointer null*/
        if (p_ctrl->p_data != NULL)
        {
            /* Read data from the registers*/
            for (uint8_t data_idx = 0U; data_idx < data_length; data_idx++)
            {
                *(p_ctrl->p_data)          = p_ctrl->p_reg->LDBR[data_idx]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_ctrl->rx_src_bytes      -= 1U;
                p_ctrl->rx_bytes_received += 1U;
                p_ctrl->p_data            += 1U;                            /* PRQA S 0489 # Advisory R-18.4 */
            }
        }

        /*Clear flags and set event*/
        p_ctrl->p_reg->LST &= (uint8_t) ~(R_RLN30_LST_FRC_Msk | R_RLN30_LST_D1RC_Msk); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        if (RLIN3_LIN_MODE_OPERATION == p_ctrl->status)
        {
            event = LIN_EVENT_RX_DATA_COMPLETE;
        }
        else
        {
            event = LIN_EVENT_RX_WAKEUP_COMPLETE;
        }

        if (0U < p_ctrl->rx_src_bytes)
        /* Continue reception */
        {
            if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->rx_src_bytes)
            {
                p_ctrl->p_reg->LDFC_LIN_b.LSS = 1U;                           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
            else
            {
                p_ctrl->p_reg->LDFC_LIN_b.LSS  = 0U;                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_ctrl->p_reg->LDFC_LIN_b.RFDL = p_ctrl->rx_src_bytes & 0xFU; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }

            /* Start Response Reception */
            p_ctrl->p_reg->LTRC_b.RTS = 1U;                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else
        {
            p_ctrl->rx_checksum = p_ctrl->p_reg->LCBR;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_data      = NULL;
        }
    }
    else
    {
        if (LIN_MODE_MASTER != p_cfg->mode)
        {
            /*Check successful header reception flag*/
            if (((p_ctrl->p_reg->LST & R_RLN30_LST_HTRC_Msk) != 0U) &&   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                ((p_ctrl->p_reg->LBSS & R_RLN30_LBSS_SYCC_Msk) != 0U) && /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                ((p_ctrl->p_reg->LBSS & R_RLN30_LBSS_BRKC_Msk) != 0U))   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
#if  BSP_VLAB_HARDWARE_BASED_USED                                        /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
                p_ctrl->p_reg->LTRC_b.RTS = 1U;
#endif

                /* Read the received data */
                p_ctrl->last_pid = (uint8_t) (p_ctrl->p_reg->LIDB_LIN); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /*Clear flags*/
                p_ctrl->p_reg->LST_b.HTRC = 0U;                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_ctrl->p_reg->LBSS       = 0U;                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /* Set the event to notify the application (slave node only). */
                event = LIN_EVENT_RX_HEADER_COMPLETE;
            }
        }
    }

    /* If callback event occurred, call the callback */
    if (LIN_EVENT_NONE != event)
    {
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * LIN interrupt processing for LIN mode. LIN interrupt fires when occurs the following case
 * Successful header transmission or reception
 * Successful frame transmission or reception
 * An error that is detectable by the RLIN hardware
 *
 * @retval    none
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void rlin3_lin_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Process the received data and determine whether there is a callback event present */
    lin_event_t       event = LIN_EVENT_NONE;
    lin_cfg_t const * p_cfg = p_ctrl->p_cfg;

    if ((p_ctrl->p_reg->LST & R_RLN30_LST_FTC_Msk) != 0U) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    /*Frame transmit successfully*/
    {
        /*Clear flags and set event*/
        p_ctrl->p_reg->LST_b.FTC = 0U;                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        if (0U < p_ctrl->tx_src_bytes)
        /*Continue transmission*/
        {
            if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->tx_src_bytes)
            {
                p_ctrl->p_reg->LDFC_LIN_b.LSS = 1U;                           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
            else
            {
                p_ctrl->p_reg->LDFC_LIN_b.LSS  = 0U;                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_ctrl->p_reg->LDFC_LIN_b.RFDL = p_ctrl->tx_src_bytes & 0xFU; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }

            r_rlin3_lin_transmission(p_ctrl);

            /* Start Response transmission */
            p_ctrl->p_reg->LTRC_b.RTS = 1U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else
        {
            p_ctrl->p_data = NULL;
        }

        if (LIN_MODE_SLAVE == p_cfg->mode)
        {
            if (0x1U == p_ctrl->p_reg->LRSS_b.RSDD) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                /* Response Space Dominant Detection */
                r_rlin3_lin_resp_space_dominant_handle(p_ctrl);
            }
        }

        if (RLIN3_LIN_MODE_OPERATION == p_ctrl->status)
        {
            event = LIN_EVENT_TX_DATA_COMPLETE;
        }
        else
        {
            event = LIN_EVENT_TX_WAKEUP_COMPLETE;
        }
    }
    else if ((p_ctrl->p_reg->LST & R_RLN30_LST_FRC_Msk) != 0U) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    /*Frame reception successfully*/
    {
        uint8_t data_length = RLIN3_LIN_MAX_FRAME_NUM_BYTES;

        if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->rx_src_bytes)
        {
            data_length = RLIN3_LIN_MAX_FRAME_NUM_BYTES;
        }
        else
        {
            data_length = (uint8_t) p_ctrl->rx_src_bytes;
        }

        /* Check NULL pointer*/
        if (p_ctrl->p_data != NULL)
        {
            /* Read data from the registers*/
            for (uint8_t data_idx = 0U; data_idx < data_length; data_idx++)
            {
                *(p_ctrl->p_data)          = p_ctrl->p_reg->LDBR[data_idx]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_ctrl->rx_src_bytes      -= 1U;
                p_ctrl->rx_bytes_received += 1U;
                p_ctrl->p_data            += 1U;                            /* PRQA S 0489 # Advisory R-18.4 */
            }
        }

        /*Clear flags and set event*/
        p_ctrl->p_reg->LST &= (uint8_t) ~(R_RLN30_LST_FRC_Msk | R_RLN30_LST_D1RC_Msk); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        if (RLIN3_LIN_MODE_OPERATION == p_ctrl->status)
        {
            event = LIN_EVENT_RX_DATA_COMPLETE;
        }
        else
        {
            event = LIN_EVENT_RX_WAKEUP_COMPLETE;
        }

        if (0U < p_ctrl->rx_src_bytes)
        /* Continue reception */
        {
            if (RLIN3_LIN_MAX_FRAME_NUM_BYTES < p_ctrl->rx_src_bytes)
            {
                p_ctrl->p_reg->LDFC_LIN_b.LSS = 1U;                           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
            else
            {
                p_ctrl->p_reg->LDFC_LIN_b.LSS  = 0U;                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_ctrl->p_reg->LDFC_LIN_b.RFDL = p_ctrl->rx_src_bytes & 0xFU; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }

            /* Start Response Reception */
            p_ctrl->p_reg->LTRC_b.RTS = 1U;                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else
        {
            p_ctrl->rx_checksum = p_ctrl->p_reg->LCBR;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_data      = NULL;
        }
    }
    /* Transmission/Reception Header Interrupt*/
    else
    {
        /* Transmission Header Interrupt*/
        if (LIN_MODE_MASTER == p_cfg->mode)
        {
            if ((p_ctrl->p_reg->LST & R_RLN30_LST_HTRC_Msk) != 0U) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                /*Clear flags and set event*/
                p_ctrl->p_reg->LST_b.HTRC = 0U;                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /* If the master is in the separate mode, the RTS bit must be set to start response transmission/reception. */
                if (1U == p_ctrl->p_reg->LDFC_LIN_b.FSM)           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                {
                    p_ctrl->p_reg->LTRC_b.RTS = 1U;                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                }

                event = LIN_EVENT_TX_HEADER_COMPLETE;
            }
        }
        /* Reception Header Interrupt*/
        else
        {
            /*Check successful header reception flag*/
            if (((p_ctrl->p_reg->LST & R_RLN30_LST_HTRC_Msk) != 0U) &&   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                ((p_ctrl->p_reg->LBSS & R_RLN30_LBSS_SYCC_Msk) != 0U) && /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                ((p_ctrl->p_reg->LBSS & R_RLN30_LBSS_BRKC_Msk) != 0U))   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
#if  BSP_VLAB_HARDWARE_BASED_USED                                        /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
                p_ctrl->p_reg->LTRC_b.RTS = 1U;
#endif

                /* Read the received data */
                p_ctrl->last_pid = (uint8_t) (p_ctrl->p_reg->LIDB_LIN); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /*Clear flags and set event*/
                p_ctrl->p_reg->LST_b.HTRC = 0U;                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_ctrl->p_reg->LBSS       = 0U;                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /* Set the event to notify the application (slave node only). */
                event = LIN_EVENT_RX_HEADER_COMPLETE;
            }
        }
    }

    /* If callback event occurred, call the callback */
    if (LIN_EVENT_NONE != event)
    {
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ERI interrupt processing for LIN mode. The ERI interrupt fires when an error that is detectable by the RLIN
 * hardware occurs.
 *
 * @retval    none
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void rlin3_lin_eri_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    rlin3_lin_instance_ctrl_t * p_ctrl = (rlin3_lin_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Process the received data and determine whether there is a callback event present */
    lin_event_t event = LIN_EVENT_NONE;

    uint8_t reg_lest = p_ctrl->p_reg->LEST_LIN;           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->p_reg->LEST_LIN = RLIN3_LIN_CLEAR_ERROR_FLAG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (0U != (reg_lest & R_RLN30_LEST_LIN_BER_Msk))
    {
        event = LIN_EVENT_ERR_BUS_COLLISION_DETECTED;
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    if (0U != (reg_lest & R_RLN30_LEST_LIN_PBER_Msk))
    {
        event = LIN_EVENT_ERR_PHYSICAL_BUS;
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    if (0U != (reg_lest & R_RLN30_LEST_LIN_TER_Msk))
    {
        event = LIN_EVENT_ERR_TIMEOUT;
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    if (0U != (reg_lest & R_RLN30_LEST_LIN_FER_Msk))
    {
        event = LIN_EVENT_ERR_FRAMING;
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    if (0U != (reg_lest & R_RLN30_LEST_LIN_SFER_Msk))
    {
        event = LIN_EVENT_ERR_SYNC;
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    if (0U != (reg_lest & R_RLN30_LEST_LIN_CSER_Msk))
    {
        event = LIN_EVENT_ERR_INVALID_CHECKSUM;
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    if (0U != (reg_lest & R_RLN30_LEST_LIN_IPER_Msk))
    {
        event = LIN_EVENT_ERR_PARITY;
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    if (0U != (reg_lest & R_RLN30_LEST_LIN_RPER_Msk))
    {
        event = LIN_EVENT_ERR_RESPONSE_PREPARE;
        r_rlin3_lin_call_callback(p_ctrl, event);
    }

    /* Stop any in progress transmission/reception */
    p_ctrl->rx_bytes_received = 0U;
    p_ctrl->rx_src_bytes      = 0U;
    p_ctrl->tx_src_bytes      = 0U;
    p_ctrl->p_data            = NULL;

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}
