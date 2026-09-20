/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include "r_psi5s.h"                   /* PRQA S 0380 # Required R-1.1-C99 */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "PSIS" in ASCII, used to determine if channel is open */

#define PSI5S_PRV_OPEN                      (0x50534953U)
#define PSI5S_PRV_CLOSE                     (0U)

/* Worst case ratio of ((cpu_clk / PCLK) / cycles_per_nop) * Max_frame = 1.5*234 approximately. */
#define PSI5S_PERIPHERAL_REG_MAX_WAIT       (0x160U)

/* Wait at least 60 PCLK until set operation to enable ((cpu_clk / PCLK) / cycles_per_nop) * Max_frame = 3*60 approximately. */
#define PSI5S_RESTART_OPERATION_DELAY       (0xB4U)

/* DDSR user data input mask */
#define PSI5S_DDSR_DATA_FIELD_MASK          (0xFFFFFU)    /* 20 bits  -> [23:4] */
#define PSI5S_DDSR_ADDR_MASK                (0xFU)        /* 4 bits   -> [3:0]  */

/* loopback test key sequence */
#define PSI5S_LOOPBACK_KEY_STEP_1           (0x000000A7U) /* Step 1 */
#define PSI5S_LOOPBACK_KEY_STEP_2           (0x00000058U) /* Step 2 */
#define PSI5S_LOOPBACK_KEY_STEP_3           (0x00000001U) /* Step 3 */

/* Bit field definitions for TDT1 */
#define PSI5S_CM5_MASK                      (0x1FU)       /* 5-bit command */
#define PSI5S_CM5_SHIFT                     (3U)
#define PSI5S_CHID3_MASK                    (0x07U)       /* 3-bit channel ID */

/* Bit field definitions for TDT2 */
#define PSI5S_ADDR6_MASK                    (0x3FU)       /* 6-bit address */

/* PSI5S DDSR data length definition (number of valid data bits) */
#define PSI5S_DDSR_DATA_BITS_SHORT          (3U)          ///< Short frame: 3-bit data field
#define PSI5S_DDSR_DATA_BITS_LONG           (13U)         ///< Long frame: 13-bit data field
#define PSI5S_DDSR_DATA_BITS_XLONG          (19U)         ///< Extra-long frame: 19-bit data field
#define PSI5S_DDSR_DATA_BITS_XXLONG         (20U)         ///< Extra-extra-long frame: 20-bit data field

/* Number of PSI5S error packet frames */
#define PSI5S_RX_ERROR_PACKET_FRAME_NUM     (7U)

/* Size of error packet frame */
#define PSI5S_RX_ERROR_PACKET_FRAME_SIZE    (1U)

/* Number of PSI5S DDSR transmit data frame types */
#define PSI5S_DDSR_TRANS_TYPE_NUM           (4U)

/* Size of DDSR transmit type */
#define PSI5S_DDSR_TRANS_TYPE_SIZE          (1U)

/* PRQA S 3472 ++ # Advisory Dir-4.9 */
/* Function-like macro used to converts a logical channel (CH1..CH7) to a zero-based index  */
#define PSI5S_CH_TO_CFG_CH_INDEX(ch)    ((uint8_t) (((uint32_t) (ch)) - ((uint32_t) PSI5S_CHANNEL_1)))

/* redefined Function-like macro used to Busy-wait until a hardware-visible condition equals a required value or timeout expires. */
#define FSP_HARDWARE_REGISTER_WAIT_TIMEOUT(expr, req, to_var) \
    do                                                        \
    {                                                         \
        (void) (to_var);                                      \
        while (((expr) != (req)) && ((to_var) > 0U))          \
        {                                                     \
            (to_var)--;                                       \
            NOP();                                            \
        }                                                     \
    } while (0)

/* PRQA S 3472 -- */

/* TX frame data */
#define PSI5S_TDT_SIZE              (8U)
#define PSI5S_TDT_HEADER_BYTES      (2U)
#define PSI5S_TDT_PAYLOAD_OFFSET    (2U)
#define PSI5S_TDT_TXNUM_ADJUST      (1U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static fsp_err_t r_psi5s_software_reset(psi5s_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_psi5s_common_config(psi5s_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_psi5s_loopback_config(psi5s_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_psi5s_wdt_config(psi5s_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_psi5s_timestamp_config(psi5s_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_psi5s_interrupt_config(psi5s_instance_ctrl_t * p_ctrl);
static fsp_err_t r_psi5s_sync_trigger_config(psi5s_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_psi5s_sync_command_config(psi5s_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_psi5s_frame_reception_config(psi5s_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_psi5s_build_tdt(psi5s_tx_info_t const * p_tx_cmd, uint8_t p_tdt8[PSI5S_TDT_SIZE], uint8_t * p_txnum);
static fsp_err_t r_psi5s_trans_transceiver_cmd(psi5s_instance_ctrl_t * const p_ctrl, psi5s_tx_info_t const * p_tx_cmd);

static psi5s_operation_mode_t r_psi5s_current_mode(psi5s_instance_ctrl_t const * const p_ctrl);
static psi5s_packet_frame_t   r_psi5s_get_err_packet_frame(psi5s_instance_ctrl_t const * const p_ctrl,
                                                           psi5s_channel_t                     channel);

static void r_psi5s_set_noise_filter(psi5s_instance_ctrl_t const * const p_ctrl);
static void r_psi5s_set_parity(psi5s_instance_ctrl_t const * const p_ctrl);
static void r_psi5s_set_baud_rate(psi5s_instance_ctrl_t const * const p_ctrl,
                                  psi5s_baud_setting_t const * const  p_baud_setting);
static void r_psi5s_set_uart_clock_output(psi5s_instance_ctrl_t const * const p_ctrl,
                                          psi5s_clock_status_t const          clock_status);
static void r_psi5s_operation_mode_setting(psi5s_instance_ctrl_t const * const p_ctrl,
                                           psi5s_operation_mode_t              mode_select);
static void     r_psi5s_set_wdt(psi5s_instance_ctrl_t const * const p_ctrl);
static void     r_psi5s_set_timestamp(psi5s_instance_ctrl_t const * const p_ctrl);
static void     r_psi5s_set_cpu_int(psi5s_instance_ctrl_t const * const p_ctrl);
static void     r_psi5s_set_sync_trigger(psi5s_instance_ctrl_t const * const p_ctrl);
static void     r_psi5s_set_sync_command(psi5s_instance_ctrl_t const * const p_ctrl);
static void     r_psi5s_set_frame_reception(psi5s_instance_ctrl_t const * const p_ctrl);
static void     r_psi5s_set_tx_frame_data(psi5s_instance_ctrl_t const * const p_ctrl);
static uint32_t r_psi5s_ddsr_pack_data(uint32_t tx_data, psi5s_ddsr_trans_type_t type);
static void     r_psi5s_read_mailbox(psi5s_instance_ctrl_t * p_ctrl, psi5s_channel_t channel);
static void     r_psi5s_status_handler(psi5s_instance_ctrl_t * const p_ctrl, psi5s_channel_t channel);

static void r_psi5s_call_callback(psi5s_instance_ctrl_t const * const p_ctrl,
                                  psi5s_event_context_t               event_context,
                                  psi5s_event_t                       event);

static void r_psi5s_clear_interrupt_status(psi5s_instance_ctrl_t * const p_ctrl,
                                           psi5s_channel_t               channel,
                                           uint32_t                      status_mask);

static void r_psi5s_clear_receive_error_status(psi5s_instance_ctrl_t * const p_ctrl,
                                               psi5s_channel_t               channel,
                                               uint8_t                       error_mask);

BSP_INTERRUPT_ATTRIBUTE void psi5s_ch_isr(void); /* PRQA S 3449, 3451 # Required R-8.5, Required R-8.5 */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* PSI5S function pointers */
const psi5s_api_t g_psi5s_on_psi5s =    /* PRQA S 1502 # Published object */
{
    .open  = R_PSI5S_Open,              /* PRQA S 3635 # Advisory R-17.12 */
    .close = R_PSI5S_Close,             /* PRQA S 3635 # Advisory R-17.12 */

    .start       = R_PSI5S_Start,       /* PRQA S 3635 # Advisory R-17.12 */
    .stop        = R_PSI5S_Stop,        /* PRQA S 3635 # Advisory R-17.12 */
    .write       = R_PSI5S_Write,       /* PRQA S 3635 # Advisory R-17.12 */
    .read        = R_PSI5S_Read,        /* PRQA S 3635 # Advisory R-17.12 */
    .callbackSet = R_PSI5S_CallbackSet, /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup PSI5S
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Opens the driver.
 *
 * Implements @ref psi5s_api_t::open().
 * @param[in]   p_api_ctrl                  Pointer to control block
 * @param[in]   p_cfg                       Pointer to configuration block
 *
 * @retval      FSP_SUCCESS                 Input Parameters are valid.
 * @retval      FSP_ERR_ASSERTION           An input parameter is invalid.
 * @retval      FSP_ERR_IRQ_BSP_DISABLED    Callback is NULL and the PSI5S IRQ is not enabled.
 * @retval      FSP_ERR_ALREADY_OPEN        The control structure is already opened.
 **********************************************************************************************************************/
fsp_err_t R_PSI5S_Open (psi5s_ctrl_t * const p_api_ctrl, psi5s_cfg_t const * const p_cfg) /* PRQA S 1505, 3673 # Published API, Advisory R-8.13 */
{
    psi5s_instance_ctrl_t * p_ctrl = (psi5s_instance_ctrl_t *) p_api_ctrl;                /* PRQA S 0316 # Advisory R-11.5 */

#if PSI5S_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_api_ctrl != NULL);                                                       /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(p_cfg != NULL);                                                            /* PRQA S 3432 # Advisory R-20.7 */

    FSP_ERROR_RETURN(PSI5S_PRV_OPEN != p_ctrl->open, (FSP_ERR_ALREADY_OPEN));             /* PRQA S 3432 # Advisory R-20.7 */
#endif

    fsp_err_t err = FSP_SUCCESS;

    /* Load configuration from application*/
    p_ctrl->p_cfg = p_cfg;

    /* Set register call to PSI5S unit */
#if (BSP_FEATURE_PSI5S_NUM_UNIT > 1U)
    static R_PSI5S0_Type * const gp_psi5s_reg[BSP_FEATURE_PSI5S_NUM_UNIT] = {R_PSI5S0, R_PSI5S1}; /* PRQA S 0306 # Advisory R-11.4 */

    p_ctrl->p_reg = gp_psi5s_reg[p_ctrl->p_cfg->unit];
#else
    p_ctrl->p_reg = R_PSI5S0;
#endif

    p_ctrl->cur_loopback_mode = p_cfg->loopback_mode;

    for (uint8_t channel = 0; channel < PSI5S_NUM_CHANNELS; channel++)
    {
        for (uint8_t frame_index = 0; frame_index < PSI5S_NUM_PACKET_FRAMES; frame_index++)
        {
            p_ctrl->psi5s_mailbox[channel][frame_index].rx_data_status = PSI5S_REG_RESET_VALUE;
            p_ctrl->psi5s_mailbox[channel][frame_index].rx_data        = PSI5S_REG_RESET_VALUE;
            p_ctrl->psi5s_mailbox[channel][frame_index].rx_timestamp   = PSI5S_REG_RESET_VALUE;
        }
    }

    /* Perform PSI5S software reset */
    err = r_psi5s_software_reset(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Apply common PSI5S configuration */
    err = r_psi5s_common_config(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Set PSI5S frame reception config */
    err = r_psi5s_frame_reception_config(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Configure the WDT */
    err = r_psi5s_wdt_config(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Configure the timestamp */
    err = r_psi5s_timestamp_config(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Configure the sync trigger */
    err = r_psi5s_sync_trigger_config(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Configure the sync command */
    err = r_psi5s_sync_command_config(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Marked notify module PSI5S channel is opened*/
    p_ctrl->open             = PSI5S_PRV_OPEN;
    p_ctrl->cur_status       = PSI5S_OPERATION_STATUS_IDLE;
    p_ctrl->cur_start_status = PSI5S_START_STATUS_DISABLE;

    return err;
}

/*******************************************************************************************************************//**
 * Closes the driver.  Implements @ref psi5s_api_t::close().
 * @param[in]   p_api_ctrl                  Pointer to control block
 *
 * @retval      FSP_SUCCESS         Successful close.
 * @retval      FSP_ERR_ASSERTION   The parameter p_ctrl is NULL.
 * @retval      FSP_ERR_NOT_OPEN    Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_PSI5S_Close (psi5s_ctrl_t * const p_api_ctrl)                  /* PRQA S 1505, 3673 # Published API, Advisory R-8.13 */
{
    psi5s_instance_ctrl_t * p_ctrl = (psi5s_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if PSI5S_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_api_ctrl != NULL);                                        /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_PRV_OPEN == p_ctrl->open, (FSP_ERR_NOT_OPEN));  /* PRQA S 3432 # Advisory R-20.7 */
#endif

    fsp_err_t err = FSP_SUCCESS;

    /* Perform PSI5S software reset */
    err = r_psi5s_software_reset(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* PSI5S disable channel used IRQ */
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_0]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_1]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_2]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_3]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_4]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_5]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_6]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_7]);

    /* Verify UART communication RX interrupt disable */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->UCRIE, (FSP_ERR_IN_USE));                               /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Verify UART communication TX interrupt disable */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->UCTIE, (FSP_ERR_IN_USE));                               /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Verify PSI5S CPU interrupt disable */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->PCIE0, (FSP_ERR_IN_USE));                               /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_1)].PCIE, /* PRQA S 0662, 3469 # Advisory R-1.2/Dir-1.1, Advisory R-4.9 */
                     (FSP_ERR_IN_USE));                                                                              /* PRQA S 3469, 3432 # Advisory R-4.9, Advisory R-20.7 */

    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_2)].PCIE, /* PRQA S 0662, 3469 # Advisory R-1.2/Dir-1.1, Advisory R-4.9 */
                     (FSP_ERR_IN_USE));                                                                              /* PRQA S 3469, 3432 # Advisory R-4.9, Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_3)].PCIE, /* PRQA S 0662, 3469 # Advisory R-1.2/Dir-1.1, Advisory R-4.9 */
                     (FSP_ERR_IN_USE));                                                                              /* PRQA S 3469, 3432 # Advisory R-4.9, Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_4)].PCIE, /* PRQA S 0662, 3469 # Advisory R-1.2/Dir-1.1, Advisory R-4.9 */
                     (FSP_ERR_IN_USE));                                                                              /* PRQA S 3469, 3432 # Advisory R-4.9, Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_5)].PCIE, /* PRQA S 0662, 3469 # Advisory R-1.2/Dir-1.1, Advisory R-4.9 */
                     (FSP_ERR_IN_USE));                                                                              /* PRQA S 3469, 3432 # Advisory R-4.9, Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_6)].PCIE, /* PRQA S 0662, 3469 # Advisory R-1.2/Dir-1.1, Advisory R-4.9 */
                     (FSP_ERR_IN_USE));                                                                              /* PRQA S 3469, 3432 # Advisory R-4.9, Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_7)].PCIE, /* PRQA S 0662, 3469 # Advisory R-1.2/Dir-1.1, Advisory R-4.9 */
                     (FSP_ERR_IN_USE));                                                                              /* PRQA S 3469, 3432 # Advisory R-4.9, Advisory R-20.7 */

    /* Verify PSI5S module disable */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->PUOEB_b.OPEN, (FSP_ERR_IN_USE));                        /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Set user selected operation mode */
    p_ctrl->cur_start_status = PSI5S_START_STATUS_DISABLE;

    /* Marked notify module PSI5S channel is opened */
    p_ctrl->open = PSI5S_PRV_CLOSE;

    return err;
}

/*******************************************************************************************************************//**
 * PSI5S start operation with user selected mode.  Implements @ref psi5s_api_t::start().
 *
 * @retval      FSP_SUCCESS             Successful start operation
 * @retval      FSP_ERR_ASSERTION       p_api_ctrl or p_cfg is NULL.
 * @retval      FSP_ERR_NOT_OPEN        Driver has not been initialized.
 * @retval      FSP_ERR_IN_USE          Driver has been started.
 * @retval      FSP_ERR_INVALID_MODE    Operation mode is not in configuration mode.
 * @retval      FSP_ERR_TIMEOUT         PSI5S operation enter user selected operation mode did not complete within the specified timeout.
 **********************************************************************************************************************/
fsp_err_t R_PSI5S_Start (psi5s_ctrl_t * const p_api_ctrl)                                      /* PRQA S 1505, 3673 # Published API, Advisory R-8.13 */
{
    psi5s_instance_ctrl_t * p_ctrl = (psi5s_instance_ctrl_t *) p_api_ctrl;                     /* PRQA S 0316 # Advisory R-11.5 */

#if PSI5S_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_api_ctrl != NULL);                                                            /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_PRV_OPEN == p_ctrl->open, (FSP_ERR_NOT_OPEN));                      /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_START_STATUS_ENABLE != p_ctrl->cur_start_status, (FSP_ERR_IN_USE)); /* PRQA S 3432 # Advisory R-20.7 */
#endif
    fsp_err_t err = FSP_SUCCESS;

    uint16_t timeout = PSI5S_RESTART_OPERATION_DELAY;

    /* Check if operation in configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE)); /* PRQA S 3432 # Advisory R-20.7 */

    /* Configure loopback mode */
    err = r_psi5s_loopback_config(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Configure interrupt according to the user selected operation mode */
    err = r_psi5s_interrupt_config(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Set user selected operation mode */
    p_ctrl->cur_start_status = PSI5S_START_STATUS_ENABLE;
    r_psi5s_operation_mode_setting(p_ctrl, PSI5S_OPERATION_MODE_PSI5S);

    /* The minimum interval is 60 PCLK (80 MHz) cycles until set operation to enable */
    FSP_HARDWARE_REGISTER_WAIT_TIMEOUT(r_psi5s_current_mode(p_ctrl), PSI5S_OPERATION_MODE_PSI5S, timeout); /* PRQA S 3473 # Advisory Dir-4.9 */

    /* User selected operation mode could not be entered. */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_PSI5S == r_psi5s_current_mode(p_ctrl), (FSP_ERR_TIMEOUT));       /* PRQA S 3432 # Advisory R-20.7 */

    return err;
}

/*******************************************************************************************************************//**
 * Closes the driver.  Implements @ref psi5s_api_t::stop().
 *
 * @warning This terminates any in-progress transmission.
 *
 * @retval      FSP_SUCCESS         Successful close.
 * @retval      FSP_ERR_ASSERTION   The parameter p_ctrl is NULL.
 * @retval      FSP_ERR_NOT_OPEN    Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_PSI5S_Stop (psi5s_ctrl_t * const p_api_ctrl)                   /* PRQA S 1505, 3673 # Published API, Advisory R-8.13 */
{
    psi5s_instance_ctrl_t * p_ctrl = (psi5s_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if PSI5S_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_api_ctrl != NULL);                                        /* PRQA S 3432 */
    FSP_ERROR_RETURN(PSI5S_PRV_OPEN == p_ctrl->open, (FSP_ERR_NOT_OPEN));
#endif
    uint16_t timeout = PSI5S_PERIPHERAL_REG_MAX_WAIT;

    /* PSI5S disable channel used IRQ */
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_0]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_1]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_2]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_3]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_4]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_5]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_6]);
    R_BSP_IrqDisable(p_ctrl->p_cfg->communication_irq[PSI5S_CHANNEL_7]);

    /* Force enter configuration mode */
    p_ctrl->p_reg->PUOEB = PSI5S_REG_RESET_VALUE;                                                     /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Wait operation enter configuration mode */
    FSP_HARDWARE_REGISTER_WAIT_TIMEOUT(p_ctrl->p_reg->PUOS_b.ACSTS, 0U, timeout);                     /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */ /* PRQA S 3473 # Advisory Dir-4.9 */

    /* Check if operation in configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_TIMEOUT)); /* PRQA S 3432 # Advisory R-20.7 */

    /* Set user selected operation mode */
    p_ctrl->cur_start_status = PSI5S_START_STATUS_DISABLE;

    /* Writing reset value (0) clears the enable bit for UART communication RX interrupt */
    p_ctrl->p_reg->UCRIE = PSI5S_REG_RESET_VALUE;                                                  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Writing reset value (0) clears the enable bit for UART communication TX interrupt */
    p_ctrl->p_reg->UCTIE = PSI5S_REG_RESET_VALUE;                                                  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Writing reset value (0) clears the enable bit for PSI5S CPU interrupt */
    p_ctrl->p_reg->PCIE0 = PSI5S_REG_RESET_VALUE;                                                  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_1)].PCIE = PSI5S_REG_RESET_VALUE; /* PRQA S 3469, 0662 # Advisory R-4.9 #, Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_2)].PCIE = PSI5S_REG_RESET_VALUE; /* PRQA S 3469, 0662 # Advisory R-4.9 #, Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_3)].PCIE = PSI5S_REG_RESET_VALUE; /* PRQA S 3469, 0662 # Advisory R-4.9 #, Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_4)].PCIE = PSI5S_REG_RESET_VALUE; /* PRQA S 3469, 0662 # Advisory R-4.9 #, Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_5)].PCIE = PSI5S_REG_RESET_VALUE; /* PRQA S 3469, 0662 # Advisory R-4.9 #, Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_6)].PCIE = PSI5S_REG_RESET_VALUE; /* PRQA S 3469, 0662 # Advisory R-4.9 #, Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(PSI5S_CHANNEL_7)].PCIE = PSI5S_REG_RESET_VALUE; /* PRQA S 3469, 0662 # Advisory R-4.9 #, Advisory R-1.2/Dir-1.1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Transmitting channel data.
 *
 * Implements @ref psi5s_api_t::write().
 * @param[in]   p_api_ctrl                  Pointer to control block
 * @param[in]   channel                     Channel send data
 * @param[in]   p_tx_info                   Pointer to command block (command_id, chid, address, data_len, data[])
 *
 * @retval      FSP_SUCCESS                 Input parameters are valid.
 * @retval      FSP_ERR_INVALID_ARGUMENT    Parameter check failure due to one or more reasons below:
 *                                            1. p_api_ctrl or p_cfg is NULL.
 *                                            2. p_tx_info is NULL.
 *                                            3. p_tx_info.data_len over 6 bytes pay load.
 *                                            4. p_tx_info.ddsr_trans_type is invalid.
 * @retval      FSP_ERR_IN_USE              Transmission busy.
 * @retval      FSP_ERR_NOT_OPEN            Driver has not been initialized.
 * @retval      FSP_ERR_INVALID_MODE        Operation mode is not in PSI5S mode or configuration mode.
 **********************************************************************************************************************/
fsp_err_t R_PSI5S_Write (psi5s_ctrl_t * const p_api_ctrl, psi5s_channel_t channel, psi5s_tx_info_t const * p_tx_info) /* PRQA S 1505, 3673 # Published API, Advisory R-8.13 */
{
    psi5s_instance_ctrl_t const * p_ctrl = (psi5s_instance_ctrl_t const *) p_api_ctrl;                                /* PRQA S 0316 # Advisory R-11.5 */

#if PSI5S_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                                   /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_tx_info);                                                                                    /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(((uint8_t) channel) < BSP_FEATURE_PSI5_NUM_SLOTS_EACH_UNIT);                                           /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_PRV_OPEN == p_ctrl->open, (FSP_ERR_NOT_OPEN));                                             /* PRQA S 3432 # Advisory R-20.7 */
#endif

    fsp_err_t err = FSP_SUCCESS;

    uint16_t timeout = PSI5S_PERIPHERAL_REG_MAX_WAIT;

    /* Check if operation in PSI5S mode or configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_UART != r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE));

    if (PSI5S_CHANNEL_0 == channel)
    {
        /* Configure the TX frame data for transmitting */
        err = r_psi5s_trans_transceiver_cmd((psi5s_instance_ctrl_t *) p_api_ctrl, p_tx_info); /* PRQA S 0316 # Advisory R-11.5 */

        /* Handle error */
        FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

        /* Transmission start */
        p_ctrl->p_reg->PTFST_b.TXST = 1U; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

        return err;
    }
    else
    {
        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(channel);                                            /* PRQA S 3469 # Advisory Dir-4.9 */

        /* Check if CH n DDSR transmission is not in progress */
        FSP_HARDWARE_REGISTER_WAIT_TIMEOUT(p_ctrl->p_reg->CFG_CH[channel_index].PDDS_b.DDSRSTS, 0U, timeout); /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */ /* PRQA S 3473 # Advisory Dir-4.9 */

        /* Handle error */
        FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->CFG_CH[channel_index].PDDS_b.DDSRSTS,        /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                         (FSP_ERR_IN_USE));                                                                   /* PRQA S 3432 # Advisory R-20.7 */

        /* check if DDSR transmit data frames type valid */
        if ((uint8_t) PSI5S_DDSR_TRANS_TYPE_XXLONG >= (uint8_t) p_tx_info->ddsr_trans_type)
        {
            /* Specify the type of ECU-to-sensor data */
            p_ctrl->p_reg->CFG_CH[channel_index].PDDTP = (uint8_t) p_tx_info->ddsr_trans_type; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

            /* Prepare DDSR transmit data */
            uint32_t pdddn_reg = r_psi5s_ddsr_pack_data(p_tx_info->tx_data, p_tx_info->ddsr_trans_type);

            /* Set DDSR data to start transmit */
            p_ctrl->p_reg->CFG_CH[channel_index].PDDD = pdddn_reg; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
        }
        else
        {
            err = FSP_ERR_INVALID_ARGUMENT;
        }

        return err;
    }
}

/*******************************************************************************************************************//**
 * Get mailbox data.
 *
 * Implements @ref psi5s_api_t::read().
 * @param[in]   p_api_ctrl                  Pointer to control block
 * @param[out]  p_mailbox_data              Pointer to an array of psi5s_mailbox_t. The caller shall provide
 *                                          PSI5S_NUM_PACKET_FRAMES elements (PSI5S_CH0_NUM_PACKET_FRAMES elements
 *                                          if channel is PSI5S_CHANNEL_0), since one entry is written per packet
 *                                          frame.
 * @param[in]   channel                     Channel get data
 *
 * @retval      FSP_SUCCESS                 Input parameters are valid.
 * @retval      FSP_ERR_INVALID_ARGUMENT    Parameter check failure due to one or more reasons below:
 *                                            1. p_api_ctrl or p_cfg is NULL.
 *                                            2. p_mailbox_data is NULL.
 **********************************************************************************************************************/
fsp_err_t R_PSI5S_Read (psi5s_ctrl_t * const p_api_ctrl, psi5s_mailbox_t * p_mailbox_data, psi5s_channel_t channel) /* PRQA S 1505, 3673 # Published API, Advisory R-8.13 */
{
    psi5s_instance_ctrl_t * p_ctrl = (psi5s_instance_ctrl_t *) p_api_ctrl;                                          /* PRQA S 0316 # Advisory R-11.5 */

#if PSI5S_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                                 /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_mailbox_data);                                                                             /* PRQA S 3432 # Advisory R-20.7 */

    FSP_ASSERT(((uint8_t) channel) < BSP_FEATURE_PSI5_NUM_SLOTS_EACH_UNIT);                                         /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_PRV_OPEN == p_ctrl->open, (FSP_ERR_NOT_OPEN));                                           /* PRQA S 3432 # Advisory R-20.7 */
#endif

    uint8_t max_frame_packet = (PSI5S_CHANNEL_0 == channel) ? PSI5S_CH0_NUM_PACKET_FRAMES : PSI5S_NUM_PACKET_FRAMES;

    for (uint8_t frame_packet = 0U; frame_packet < max_frame_packet; frame_packet++)
    {
        p_mailbox_data[frame_packet].rx_data_status = p_ctrl->psi5s_mailbox[channel][frame_packet].rx_data_status;
        p_mailbox_data[frame_packet].rx_data        = p_ctrl->psi5s_mailbox[channel][frame_packet].rx_data;
        p_mailbox_data[frame_packet].rx_timestamp   = p_ctrl->psi5s_mailbox[channel][frame_packet].rx_timestamp;
        p_mailbox_data[frame_packet].rx_unread      = p_ctrl->psi5s_mailbox[channel][frame_packet].rx_unread;

        /* data have been read */
        p_ctrl->psi5s_mailbox[channel][frame_packet].rx_unread = false;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 *
 * Implements @ref psi5s_api_t::callbackSet().
 * @param[in]   p_api_ctrl               Pointer to control block
 * @param[in]   p_callback               Pointer to configuration block
 * @param[in]   p_context                Pointer to configuration block
 * @param[in]   p_callback_memory        Pointer to configuration block
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_PSI5S_CallbackSet (psi5s_ctrl_t * const          p_api_ctrl,                           /* PRQA S 1505 # Published API */
                               void (                      * p_callback)(psi5s_callback_args_t *), /* PRQA S 1336 # Required R-8.2 */
                               void const * const            p_context,
                               psi5s_callback_args_t * const p_callback_memory)
{
    psi5s_instance_ctrl_t * p_ctrl = (psi5s_instance_ctrl_t *) p_api_ctrl;                         /* PRQA S 0316 # Advisory R-11.5 */

#if PSI5S_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);                                                                            /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(PSI5S_PRV_OPEN == p_ctrl->open, (FSP_ERR_NOT_OPEN));                          /* PRQA S 3432 # Advisory R-20.7 */
#endif

    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/** @} (end addtogroup PSI5S) */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * PSI5S software reset config and enter Configuration mode.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS             Software reset PSI5S execution done and enter configuration mode.
 * @retval      FSP_ERR_TIMEOUT         PSI5S operation enter Configuration mode did not complete within the specified timeout.
 *
 * @warning This resets all register configurations to the default settings.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_software_reset (psi5s_instance_ctrl_t const * const p_ctrl)
{
    uint16_t timeout = PSI5S_PERIPHERAL_REG_MAX_WAIT;

    /* Software reset PSI5S */
    p_ctrl->p_reg->PUSWR = 0x00000001U;                                                               /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Wait until the SW reset execution done */
    FSP_HARDWARE_REGISTER_WAIT_TIMEOUT(p_ctrl->p_reg->PUOS_b.SWSTS, 0U, timeout);                     /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */ /* PRQA S 3473 # Advisory Dir-4.9 */

    /* check if operation transitions to the configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_TIMEOUT)); /* PRQA S 3432 # Advisory R-20.7 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Apply common PSI5S configuration to hardware registers:
 * - Noise filter level
 * - TX/RX parity modes
 * - UART baud rate
 * - UART clock output enable
 *
 * @param[in]  p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS             Configuration completed successfully.
 * @retval      FSP_ERR_INVALID_MODE    Operation mode is not in configuration mode.
 * @retval      FSP_ERR_IN_USE          UART clock output is currently enabled, so baud rate cannot be updated.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_common_config (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* Check if operation in configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE)); /* PRQA S 3432 # Advisory R-20.7 */

    psi5s_extended_cfg_t const * p_extend = (psi5s_extended_cfg_t const *) p_ctrl->p_cfg->p_extend;        /* PRQA S 0316 # Advisory R-11.5 */

    /* Set noise filter: user configuration */
    r_psi5s_set_noise_filter(p_ctrl);

    /* Set parity: user configuration */
    r_psi5s_set_parity(p_ctrl);

    /* Check UART clock (psis_tx_sclk) output disable */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->PUBCE_b.SCKEN, (FSP_ERR_IN_USE)); /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Set baud rate: user configuration */
    r_psi5s_set_baud_rate(p_ctrl, p_extend->uart_baud_setting);

    /* Set clock output: user configuration */
    r_psi5s_set_uart_clock_output(p_ctrl, p_ctrl->p_cfg->clk_bus_en);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure loopback Mode.
 *
 * @param[in]  p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS             Successfully set loopback mode.
 * @retval      FSP_ERR_TIMEOUT         LoopBack enable/disable sequence did not complete within the specified timeout.
 * @retval      FSP_ERR_INVALID_MODE    Operation mode is not in configuration mode.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_loopback_config (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* Check if operation in configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE)); /* PRQA S 3432 # Advisory R-20.7 */

    uint16_t timeout = PSI5S_PERIPHERAL_REG_MAX_WAIT;

    /* Disable loopback */
    if (PSI5S_LOOPBACK_MODE_DISABLE == p_ctrl->cur_loopback_mode)
    {
        p_ctrl->p_reg->PUCLB = PSI5S_REG_RESET_VALUE;                                              /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

        /* Wait LBEN = 0 */
        FSP_HARDWARE_REGISTER_WAIT_TIMEOUT(p_ctrl->p_reg->PUCLB_b.LBEN, 0U, timeout);              /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */ /* PRQA S 3473 # Advisory Dir-4.9 */

        FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->PUCLB_b.LBEN, (FSP_ERR_TIMEOUT)); /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
    else                                                                                           /* Enable loopback */
    {
        /* loopback test key sequence (3 step):
         *    Step-1: write 0xA7
         *    Step-2: write 0x58
         *    Step-3: write 0x01 (LBEN = 1)
         */
        p_ctrl->p_reg->PUCLB = PSI5S_LOOPBACK_KEY_STEP_1;                                          /* Step 1 */ /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
        p_ctrl->p_reg->PUCLB = PSI5S_LOOPBACK_KEY_STEP_2;                                          /* Step 2 */ /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
        p_ctrl->p_reg->PUCLB = PSI5S_LOOPBACK_KEY_STEP_3;                                          /* Step 3 */ /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

        /* Wait LBEN = 1 */
        timeout = PSI5S_PERIPHERAL_REG_MAX_WAIT;
        FSP_HARDWARE_REGISTER_WAIT_TIMEOUT(p_ctrl->p_reg->PUCLB_b.LBEN, 1U, timeout);              /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */ /* PRQA S 3473 # Advisory Dir-4.9 */

        FSP_ERROR_RETURN(1U == p_ctrl->p_reg->PUCLB_b.LBEN, (FSP_ERR_TIMEOUT));                    /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure the WDT for PSI5S mode.
 *
 * @param[in]  p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS             Successfully configured the WDT.
 * @retval      FSP_ERR_INVALID_MODE    Operation mode is not in configuration mode.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_wdt_config (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* Check if Operation in PSI5S mode or Configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE)); /* PRQA S 3432 # Advisory R-20.7 */

    /* Set WDT: user configuration */
    r_psi5s_set_wdt(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure the timestamp for PSI5S Mode.
 *
 * @param[in]  p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS             Successfully configured the timestamp.
 * @retval      FSP_ERR_INVALID_MODE    Operation mode is not in Configuration mode.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_timestamp_config (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* Check if operation in configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE)); /* PRQA S 3432 # Advisory R-20.7 */

    /* Set timestamp: user configuration */
    r_psi5s_set_timestamp(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure PSI5S interrupt system according to the selected operation mode:
 * - UART mode
 * - PSI5S mode
 *
 * @param[in]  p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS                 Configuration completed successfully.
 * @retval      FSP_ERR_INVALID_MODE        Operation mode is not in PSI5S mode or configuration mode.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_interrupt_config (psi5s_instance_ctrl_t * p_ctrl)
{
    /* Configure and enable PSI5S communication CH n IRQ in the NVIC using the user-provided priority */
    for (uint8_t ch = 0U; ch < PSI5S_NUM_CHANNELS; ch++)
    {
        if (FSP_INVALID_VECTOR != p_ctrl->p_cfg->communication_irq[ch])
        {
            /* PSI5S enable channel used IRQ */
            R_BSP_IrqCfgEnable(p_ctrl->p_cfg->communication_irq[ch], p_ctrl->p_cfg->communication_ipl, p_ctrl);
        }
    }

    /* Check if current operation in PSI5S mode or configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_UART != r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE));

    /* Set PSI5S CPU interrupt: user configuration */
    r_psi5s_set_cpu_int(p_ctrl);

    /* Writing reset value (0) clears the enable bit for UART communication RX interrupt */
    p_ctrl->p_reg->UCRIE = PSI5S_REG_RESET_VALUE; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Writing reset value (0) clears the enable bit for UART communication TX interrupt */
    p_ctrl->p_reg->UCTIE = PSI5S_REG_RESET_VALUE; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure the sync trigger for PSI5S mode.
 *
 * @param[in]  p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS             Successfully configured the sync trigger.
 * @retval      FSP_ERR_INVALID_MODE    Operation mode is not in configuration mode.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_sync_trigger_config (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* Check if operation in configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE)); /* PRQA S 3432 # Advisory R-20.7 */

    /* Set sync trigger: user configuration */
    r_psi5s_set_sync_trigger(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure the sync command for PSI5S Mode.
 *
 * @param[in]  p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS             Successfully configured the sync command.
 * @retval      FSP_ERR_INVALID_MODE    Operation mode is not in configuration mode.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_sync_command_config (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* Check if operation in configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE)); /* PRQA S 3432 # Advisory R-20.7 */

    /* Set sync command: user configuration */
    r_psi5s_set_sync_command(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure the frame reception for PSI5S Mode.
 *
 * @param[in]  p_ctrl               Pointer to instance control block
 *
 * @retval      FSP_SUCCESS             Successfully configured the frame reception.
 * @retval      FSP_ERR_INVALID_MODE    Operation mode is not in configuration mode.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_frame_reception_config (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* Check if operation in configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_CONFIG == r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE)); /* PRQA S 3432 # Advisory R-20.7 */

    /* Set frame reception: user configuration */
    r_psi5s_set_frame_reception(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Build TX frame data for transmitting transceiver commands.
 *
 * @param[in]     p_tx_cmd    Pointer to command block (command_id, chid, address, data_len, data[])
 * @param[out]    p_tdt8      Pointer to 8-byte PSI5S TX frame data buffer (TDT1..TDT8).
 *                            If payload length is less than 6, the remaining bytes are set to 0.
 * @param[out]    p_txnum     TX command data number PTFNM.TXNUM
 *
 * @retval        FSP_SUCCESS               TX frame data was built successfully.
 * @retval        FSP_ERR_INVALID_ARGUMENT  p_tx_cmd, p_tdt8, or p_txnum is NULL, or payload exceeds 6 bytes.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_build_tdt (psi5s_tx_info_t const * p_tx_cmd, uint8_t p_tdt8[PSI5S_TDT_SIZE], uint8_t * p_txnum)
{
    if (p_tx_cmd->data_len > PSI5S_CMD_NUM_PACKET)
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* TDT1, TDT2 (header) */
    p_tdt8[0] =
        (uint8_t) (((p_tx_cmd->command_id & PSI5S_CM5_MASK) << PSI5S_CM5_SHIFT) | (p_tx_cmd->chid & PSI5S_CHID3_MASK));
    p_tdt8[1] = (uint8_t) (p_tx_cmd->address & PSI5S_ADDR6_MASK);

    /* Copy payload into TDT3...TDT8 */
    for (uint8_t index = 0; index < PSI5S_CMD_NUM_PACKET; index++)
    {
        if (index < p_tx_cmd->data_len)
        {
            p_tdt8[PSI5S_TDT_PAYLOAD_OFFSET + index] = p_tx_cmd->data[index];
        }
        else
        {
            p_tdt8[PSI5S_TDT_PAYLOAD_OFFSET + index] = 0U;
        }
    }

    /* Total transmitted bytes = header bytes + payload bytes */
    {
        uint8_t total_bytes = (uint8_t) (PSI5S_TDT_HEADER_BYTES + p_tx_cmd->data_len);

        /* TXNUM = total_bytes - 1 (encoded range: 0...7 for 1...8 bytes) */
        *p_txnum = (uint8_t) (total_bytes - PSI5S_TDT_TXNUM_ADJUST);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure the TX frame data for transmitting transceiver commands for PSI5S mode
 *
 * @param[in]       p_tx_cmd                Pointer to command block (command_id, chid, address, data_len, data[])
 * @param[in]       p_ctrl                  Pointer to instance control block
 *
 * @retval      FSP_SUCCESS                 Successfully transmitted the transceiver command.
 * @retval      FSP_ERR_INVALID_ARGUMENT    p_tx_cmd, p_ctrl->cur_tdt input parameter is NULL or Input over 6 bytes pay load.
 * @retval      FSP_ERR_IN_USE              Transmission busy.
 * @retval      FSP_ERR_INVALID_MODE        Operation mode is not in PSI5S mode or configuration mode.
 **********************************************************************************************************************/
static fsp_err_t r_psi5s_trans_transceiver_cmd (psi5s_instance_ctrl_t * const p_ctrl, psi5s_tx_info_t const * p_tx_cmd)
{
    uint16_t timeout = PSI5S_PERIPHERAL_REG_MAX_WAIT;

    fsp_err_t err = FSP_SUCCESS;

    /* build TX Frame Data for Transmitting Transceiver Commands */
    err = r_psi5s_build_tdt(p_tx_cmd, p_ctrl->cur_tdt, &p_ctrl->cur_txnum);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, (err));

    /* Check if transmission not busy */
    FSP_HARDWARE_REGISTER_WAIT_TIMEOUT(p_ctrl->p_reg->PTFS_b.TXSTS, 0U, timeout);             /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */ /* PRQA S 3473 # Advisory Dir-4.9 */

    /* Handle error */
    FSP_ERROR_RETURN(PSI5S_REG_RESET_VALUE == p_ctrl->p_reg->PTFS_b.TXSTS, (FSP_ERR_IN_USE)); /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Check if operation in PSI5S mode or configuration mode */
    FSP_ERROR_RETURN(PSI5S_OPERATION_MODE_UART != r_psi5s_current_mode(p_ctrl), (FSP_ERR_INVALID_MODE));

    /* Set TX frame data */
    r_psi5s_set_tx_frame_data(p_ctrl);

    return err;
}

/*******************************************************************************************************************//**
 * Get the current PSI5S operation mode from hardware status bits:
 *
 * @param[in]  p_ctrl   Pointer to PSI5S instance control block.
 *
 * @retval PSI5S_OPERATION_MODE_CONFIG          Configuration mode (ACSTS == 0)
 * @retval PSI5S_OPERATION_MODE_UART            UART mode (ACSTS == 1 and MSTS == 0)
 * @retval PSI5S_OPERATION_MODE_PSI5S           PSI5S mode (ACSTS == 1 and MSTS == 1)
 **********************************************************************************************************************/
static psi5s_operation_mode_t r_psi5s_current_mode (psi5s_instance_ctrl_t const * const p_ctrl)
{
    uint32_t const puos_reg = p_ctrl->p_reg->PUOS; /* one volatile read */ /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    bool const acsts = (PSI5S_REG_RESET_VALUE != (puos_reg & R_PSI5S0_PUOS_ACSTS_Msk));
    bool const msts  = (PSI5S_REG_RESET_VALUE != (puos_reg & R_PSI5S0_PUOS_MSTS_Msk));

    /* Check operation mode */
    if (acsts && (!msts))              /* UART Mode: ACSTS = 1, MSTS = 0 */
    {
        return PSI5S_OPERATION_MODE_UART;
    }
    else if (acsts && msts)            /* PSI5S Mode: ACSTS = 1, MSTS = 1 */
    {
        return PSI5S_OPERATION_MODE_PSI5S;
    }
    else /* Configuration Mode: ACSTS = 0 */
    {
        return PSI5S_OPERATION_MODE_CONFIG;
    }
}

/*******************************************************************************************************************//**
 * Get the current error packet frame from hardware status bits:
 *
 * @param[in]  p_ctrl   Pointer to PSI5S instance control block.
 * @param[in]  channel  Current channel.
 *
 * @retval PSI5S_PACKET_FRAME_NONE          No RX error
 * @retval PSI5S_PACKET_FRAME_1             RX error at packet frame 1
 * @retval PSI5S_PACKET_FRAME_2             RX error at packet frame 2
 * @retval PSI5S_PACKET_FRAME_3             RX error at packet frame 3
 * @retval PSI5S_PACKET_FRAME_4             RX error at packet frame 4
 * @retval PSI5S_PACKET_FRAME_5             RX error at packet frame 5
 * @retval PSI5S_PACKET_FRAME_6             RX error at packet frame 6
 **********************************************************************************************************************/
static psi5s_packet_frame_t r_psi5s_get_err_packet_frame (psi5s_instance_ctrl_t const * const p_ctrl,
                                                          psi5s_channel_t                     channel)
{
    /* PSI5S receive error status CH n register: PSI5SjPRESn
     *  [5] RERRF6 :  1: RX error has occurred at packet frame 6
     *  [4] RERRF5 :  1: RX error has occurred at packet frame 5
     *  [3] RERRF4 :  1: RX error has occurred at packet frame 4
     *  [2] RERRF3 :  1: RX error has occurred at packet frame 3
     *  [1] RERRF2 :  1: RX error has occurred at packet frame 2
     *  [0] RERRF1 :  1: RX error has occurred at packet frame 1
     */
    uint32_t pres_reg;
    if (PSI5S_CHANNEL_0 == channel)
    {
        pres_reg = p_ctrl->p_reg->PRES0;                                          /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
    else
    {
        pres_reg = p_ctrl->p_reg->CFG_CH[PSI5S_CH_TO_CFG_CH_INDEX(channel)].PRES; /* PRQA S 3469, 0662 # Advisory R-4.9 #, Advisory R-1.2/Dir-1.1 */
    }

    /* RX error at packet frame 1 */
    if (R_PSI5S0_CFG_CH_PRES_RERRF1_Msk == (pres_reg & R_PSI5S0_CFG_CH_PRES_RERRF1_Msk))
    {
        return PSI5S_PACKET_FRAME_1;
    }

    /* RX error at packet frame 2 */
    if (R_PSI5S0_CFG_CH_PRES_RERRF2_Msk == (pres_reg & R_PSI5S0_CFG_CH_PRES_RERRF2_Msk))
    {
        return PSI5S_PACKET_FRAME_2;
    }

    /* RX error at packet frame 3 */
    if (R_PSI5S0_CFG_CH_PRES_RERRF3_Msk == (pres_reg & R_PSI5S0_CFG_CH_PRES_RERRF3_Msk))
    {
        return PSI5S_PACKET_FRAME_3;
    }

    /* RX error at packet frame 4 */
    if (R_PSI5S0_CFG_CH_PRES_RERRF4_Msk == (pres_reg & R_PSI5S0_CFG_CH_PRES_RERRF4_Msk))
    {
        return PSI5S_PACKET_FRAME_4;
    }

    /* RX error at packet frame 5 */
    if (R_PSI5S0_CFG_CH_PRES_RERRF5_Msk == (pres_reg & R_PSI5S0_CFG_CH_PRES_RERRF5_Msk))
    {
        return PSI5S_PACKET_FRAME_5;
    }

    /* RX error at packet frame 6 */
    if (R_PSI5S0_CFG_CH_PRES_RERRF6_Msk == (pres_reg & R_PSI5S0_CFG_CH_PRES_RERRF6_Msk))
    {
        return PSI5S_PACKET_FRAME_6;
    }

    /* No RX error, or an unrecognized/reserved PRES bit is set: treat as no error */
    return PSI5S_PACKET_FRAME_NONE;
}

/*******************************************************************************************************************//**
 * Sets the PSI5S noise filter according to user configuration.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in Configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_noise_filter (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* PSI5S/UART noise filter set register: PSI5SjPUNFST
     *  [0] NFSET :  0: noise filter disable
     */
    p_ctrl->p_reg->PUNFST = (uint8_t) p_ctrl->p_cfg->noise_filter_setting; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
}

/*******************************************************************************************************************//**
 * Sets the PSI5S parity according to user configuration.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in Configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_parity (psi5s_instance_ctrl_t const * const p_ctrl)
{
    psi5s_extended_cfg_t const * p_extend = (psi5s_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* PSI5S/UART RX/TX parity set register: PSI5SjPUPTS
     *  [9:8] UTPRTY :  2: Zero parity
     *  [1:0] URPRTY :  2: Parity do not care
     */
    uint32_t pupts_reg =
        (((uint32_t) (p_extend->tx_parity_setting) << R_PSI5S0_PUPTS_UTPRTY_Pos) & R_PSI5S0_PUPTS_UTPRTY_Msk) |
        (((uint32_t) (p_extend->rx_parity_setting) << R_PSI5S0_PUPTS_URPRTY_Pos) & R_PSI5S0_PUPTS_URPRTY_Msk);

    p_ctrl->p_reg->PUPTS = pupts_reg;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
}

/*******************************************************************************************************************//**
 * Changes baud rate based on predetermined register settings.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 * @param[in]   p_baud_setting       Pointer to other divisor related settings
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in configuration mode.
 *       - Psis_tx_sclk (UART TX clock output) is disabled.
 **********************************************************************************************************************/
static void r_psi5s_set_baud_rate (psi5s_instance_ctrl_t const * const p_ctrl,
                                   psi5s_baud_setting_t const * const  p_baud_setting)
{
    /* PSI5S/UART Baud rate parameter register: PSI5SjPUBPR
     *  [19:16] RXOSMP :     4: (4+1) RX over sample number
     *  [15:8] SCKDIV :      2: (2+1) clock divide value of UART SCLK (psis_tx_sclk)
     *  [6:0] SCKPRS :       0: (0+1) pre-scaler of UART SCLK (psis_tx_sclk)
     *
     * Baud rate = 80MHz * (1/1) * (1/3) * (1/5) = 5.33 Mbps
     */
    uint32_t pubpr_reg =
        ((((uint32_t) p_baud_setting->bit_sampling) << R_PSI5S0_PUBPR_RXOSMP_Pos) & R_PSI5S0_PUBPR_RXOSMP_Msk) |
        ((((uint32_t) p_baud_setting->divider) << R_PSI5S0_PUBPR_SCKDIV_Pos) & R_PSI5S0_PUBPR_SCKDIV_Msk) |
        ((((uint32_t) p_baud_setting->prescaler) << R_PSI5S0_PUBPR_SCKPRS_Pos) & R_PSI5S0_PUBPR_SCKPRS_Msk);

    p_ctrl->p_reg->PUBPR = pubpr_reg;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
}

/*******************************************************************************************************************//**
 * Sets the UART clock (Psis_tx_sclk) output according to user configuration.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 * @param[in]   clock_status         UART TX clock output status
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_uart_clock_output (psi5s_instance_ctrl_t const * const p_ctrl,
                                           psi5s_clock_status_t const          clock_status)
{
    /* PSI5S/UART baud rate clock enable Register: PSI5SjPUBCE
     *  [0] SCKEN :  1: UART clock (Psis_tx_sclk) output enable
     */
    p_ctrl->p_reg->PUBCE = (uint8_t) clock_status; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
}

/*******************************************************************************************************************//**
 * PSI5S operation mode (UART / PSI5S) setting
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 * @param[in]   mode_select          Operation mode:
 *                                      - PSI5S_OPERATION_MODE_UART
 *                                      - PSI5S_OPERATION_MODE_PSI5S
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in Configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_operation_mode_setting (psi5s_instance_ctrl_t const * const p_ctrl,
                                            psi5s_operation_mode_t              mode_select)
{
    if (PSI5S_OPERATION_MODE_CONFIG > mode_select)
    {
        /* Set operation mode */
        p_ctrl->p_reg->PUOMD = (uint8_t) mode_select;              /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

        /* Start the operation of PSI5S */
        p_ctrl->p_reg->PUOEB = (uint8_t) p_ctrl->cur_start_status; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
}

/*******************************************************************************************************************//**
 * Enable PSI5S WDT config for PSI5S mode according to user configuration.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in Configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_wdt (psi5s_instance_ctrl_t const * const p_ctrl)
{
    psi5s_extended_cfg_t const * p_extend = (psi5s_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* set WDT config for CH1 -> CH7 */
    for (uint8_t ch = 1U; ch < PSI5S_NUM_CHANNELS; ch++)
    {
        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(ch);                                          /* PRQA S 3469 # Advisory Dir-4.9 */

        /* PSI5S WDT pre-scaler CH n Register: PSI5SjPWDPn
         *  [11:0] WDTPRS :  1 to 4095: enabled at 1 clock/x clock (x:1 to 4095)
         */
        p_ctrl->p_reg->CFG_CH[channel_index].PWDP = p_extend->psi5s_wdt_setting[ch].wdt_prescaler;     /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

        /* PSI5S WDT expiration value CH n register: PSI5SjPWDEVn
         *  [23:0] WDTPRS :  Watchdog timer expiration value
         */
        p_ctrl->p_reg->CFG_CH[channel_index].PWDEV = p_extend->psi5s_wdt_setting[ch].wdt_expire_value; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

        /* PSI5S WDT enable CH n register: PSI5SjPWDEn
         *  [0] WDTEB :  1: Enable
         */
        p_ctrl->p_reg->CFG_CH[channel_index].PWDE = p_extend->psi5s_wdt_setting[ch].wdt_enable;        /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }

    /* PSI5S WDT pre-scaler CH0 */
    p_ctrl->p_reg->PWDP0 = p_extend->psi5s_wdt_setting[PSI5S_CHANNEL_0].wdt_prescaler;                 /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* PSI5S WDT expiration Value CH0 */
    p_ctrl->p_reg->PWDEV0 = p_extend->psi5s_wdt_setting[PSI5S_CHANNEL_0].wdt_expire_value;             /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* PSI5S WDT enable CH0 */
    p_ctrl->p_reg->PWDE0 = p_extend->psi5s_wdt_setting[PSI5S_CHANNEL_0].wdt_enable;                    /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
}

/*******************************************************************************************************************//**
 * Enable PSI5S timestamp config for PSI5S mode according to user configuration.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_timestamp (psi5s_instance_ctrl_t const * const p_ctrl)
{
    psi5s_extended_cfg_t const * p_extend = (psi5s_extended_cfg_t const *) p_ctrl->p_cfg->p_extend;                     /* PRQA S 0316 # Advisory R-11.5 */

    /* PSI5S timestamp pre-scaler register: PSI5SjPTPS
     *  [25:16] TSPRSU :   Timestamp pre-scaler (upper). These bits configure a max 1 ms enable pulse from a 1 us enable pulse.
     *  [6:0] TSPRSL :   Timestamp pre-scaler (lower). These bits configure a 1 us clock from PCLK.
     *
     * TSPRSL set: Target generate a 1 us standard pulse from PCLK (80Mhz <=> 12.5 ns)
     *      => 1 us / 12.5 ns = 80 cycle PCLK
     *      => TSPRSL = 80 - 1 = 79 (0x4F)
     *
     * TSPRSU set: Final timestamp resolution (Tick timestamp)
     *      => Timestamp tick = (TSPRSU + 1) * 1 us
     *      => TSPRSU = (1 ms / 1 us) - 1 = 999 (0x3E7)
     */
    p_ctrl->p_reg->PTPS = p_extend->timestamp_setting->timestamp_prescaler;                                             /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* PSI5S timestamp counter A select register: PSI5SjPTCAS
     *  [16] TSCACLS :  0: Timestamp counter A clear select signal generated by PSI5S
     *  [8] TSCAEBS :  0: Timestamp counter A enable select signal generated by PSI5S
     *  [0] TSCACKS :  0: Timestamp counter A clock select signal generated by PSI5S
     */
    p_ctrl->p_reg->PTCAS_b.TSCACLS = (uint8_t) p_extend->timestamp_setting->timestamp_counter_a->counter_clear_select;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->PTCAS_b.TSCAEBS = (uint8_t) p_extend->timestamp_setting->timestamp_counter_a->counter_enable_select; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->PTCAS_b.TSCACKS = (uint8_t) p_extend->timestamp_setting->timestamp_counter_a->counter_clock_select;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* PSI5S timestamp counter A enable register: PSI5SjPTCAE
     *  [0] TSCAEB :  1: Timestamp counter A enable
     */
    p_ctrl->p_reg->PTCAE = (uint8_t) p_extend->timestamp_setting->timestamp_counter_a->timestamp_counter_enable;        /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* PSI5S timestamp counter B select register: PSI5SjPTCAS */
    p_ctrl->p_reg->PTCBS_b.TSCBCLS = (uint8_t) p_extend->timestamp_setting->timestamp_counter_b->counter_clear_select;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->PTCBS_b.TSCBEBS = (uint8_t) p_extend->timestamp_setting->timestamp_counter_b->counter_enable_select; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->PTCBS_b.TSCBCKS = (uint8_t) p_extend->timestamp_setting->timestamp_counter_b->counter_clock_select;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* PSI5S timestamp counter B enable register: PSI5SjPTCAE */
    p_ctrl->p_reg->PTCBE = (uint8_t) p_extend->timestamp_setting->timestamp_counter_b->timestamp_counter_enable;        /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
}

/*******************************************************************************************************************//**
 * Enable PSI5S CPU interrupt according to user configuration.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in PSI5S mode or configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_cpu_int (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* Enable PSI5S CPU interrupt for CH1 -> CH7 */
    for (uint8_t ch = 1U; ch < PSI5S_NUM_CHANNELS; ch++)
    {
        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(ch);                             /* PRQA S 3469 # Advisory Dir-4.9 */

        /* PSI5S CPU interrupt enable CH n register: PSI5SjPCIEn
         *  [13] IEBDDSFN :  1: Interrupt enable of DDSR finish flag
         *  [12] IEBDDSOW :  1: Interrupt enable of DDSR overwrite
         *  [11] IEBRFN :  1: Interrupt enable of RX packet frame finish flag
         *  [10] IEBRFEX :  1: Interrupt enable of RX frame excess error
         *  [9] IEBRFLK :  1: Interrupt enable of RX frame lack error
         *  [8] IEBROV :  1: Interrupt enable of RX overrun error
         *  [7] IEBRWDT :  1: Interrupt enable RX WDT error
         *  [3] IEBTRST :  1: Interrupt enable of RX transceiver status error
         *  [2] IEBPT :  1: Interrupt enable of RX pay load data parity error
         *  [1] IEBCRC :  1: Interrupt enable of RX pay load data CRC error
         */
        p_ctrl->p_reg->CFG_CH[channel_index].PCIE = p_ctrl->p_cfg->psi5s_cpu_int_cfg[ch]; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }

    /* PSI5S CPU interrupt enable CH0 register: PSI5SjPCIE0
     *  [14] IEBCTFN :  1: Interrupt enable of command TX finish
     *  [11] IEBRFN :  1: Interrupt enable of RX packet frame finish flag
     *  [10] IEBRFEX :  1: Interrupt enable of RX frame excess error
     *  [9] IEBRFLK :  1: Interrupt enable of RX frame lack error
     *  [8] IEBROV :  1: Interrupt enable of RX overrun error
     *  [7] IEBRWDT :  1: Interrupt enable RX WDT error
     *  [5] IEBUTFR :  1: Interrupt enable of RX UART framing error
     *  [4] IEBUTPT :  1: Interrupt enable of RX UART parity error
     *  [3] IEBTRST :  1: Interrupt enable of RX transceiver status error
     *  [2] IEBPT :  1: Interrupt enable of RX pay load data parity error
     *  [1] IEBCRC :  1: Interrupt enable of RX pay load data CRC error
     *  [0] IEBXCRC :  1: Interrupt enable of packet frame XCRC error
     */
    p_ctrl->p_reg->PCIE0 = p_ctrl->p_cfg->psi5s_cpu_int_cfg[PSI5S_CHANNEL_0];             /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
}

/*******************************************************************************************************************//**
 * Enable PSI5S sync trigger config for PSI5S mode according to user configuration.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_sync_trigger (psi5s_instance_ctrl_t const * const p_ctrl)
{
    psi5s_extended_cfg_t const * p_extend = (psi5s_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    for (uint8_t ch = 1U; ch < PSI5S_NUM_CHANNELS; ch++)
    {
        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(ch);                                       /* PRQA S 3469 # Advisory Dir-4.9 */

        /* PSI5S sync trigger pre-scaler CH n register: PSI5SjPSTPn
         *  [11:0] STPRS :  1 to 4095: Enabled at 1 clock/x clock (x:1 to 4095)
         */
        p_ctrl->p_reg->CFG_CH[channel_index].PSTP =                                                 /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                                                    p_extend->psi5s_sync_pulse_setting[channel_index].
                                                    sync_pulse_prescaler;

        /* PSI5S sync trigger expiration value CH n register: PSI5SjPSTEVn
         *  [23:0] STEX :  Synchronous trigger generation counter expiration value
         */
        p_ctrl->p_reg->CFG_CH[channel_index].PSTEV = /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                                                     p_extend->psi5s_sync_pulse_setting[channel_index].
                                                     sync_pulse_expire_value;

        /* PSI5S sync trigger select CH n register: PSI5SjPSTSn
         *  [0] STSEL :  0: Signal generated by PSI5S is selected
         */
        p_ctrl->p_reg->CFG_CH[channel_index].PSTS = /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                                                    (uint8_t) p_extend->psi5s_sync_pulse_setting[channel_index].
                                                    sync_pulse_src_select;
    }
}

/*******************************************************************************************************************//**
 * Set PSI5S sync command config for PSI5S mode according to user configuration.
 *
 * When the value output by shifting is 0, the command data in SFR is TRSCMD[4:0] + CHID[2:0]
 * When the value output by shifting is 1, the command data in SFR is ATRSCMD[4:0] + ACHID[2:0]
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in Configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_sync_command (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* TX command data */
    for (uint8_t ch = 1U; ch < PSI5S_NUM_CHANNELS; ch++)
    {
        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(ch); /* PRQA S 3469 # Advisory Dir-4.9 */

        /* PSI5S TX command data CH n register: PSI5SjPTCDn
         *  [15:11] ATRSCMD :  Alternate transport command
         *  [10:8] ACHID :  Alternate transport ChID
         *  [7:3] TRSCMD :  Transport command
         *  [2:0] CHID :  Transport ChID
         */
        uint32_t ptcd_reg =
            ((((uint32_t) p_ctrl->p_cfg->psi5s_sync_cmd_setting[channel_index].sync_cmd_alt_trans_cmd) <<
                R_PSI5S0_CFG_CH_PTCD_ATRSCMD_Pos) & R_PSI5S0_CFG_CH_PTCD_ATRSCMD_Msk) |
            ((((uint32_t) p_ctrl->p_cfg->psi5s_sync_cmd_setting[channel_index].sync_cmd_alt_trans_chid) <<
                R_PSI5S0_CFG_CH_PTCD_ACHID_Pos) & R_PSI5S0_CFG_CH_PTCD_ACHID_Msk) |
            ((((uint32_t) p_ctrl->p_cfg->psi5s_sync_cmd_setting[channel_index].sync_cmd_trans_cmd) <<
                R_PSI5S0_CFG_CH_PTCD_TRSCMD_Pos) &
             R_PSI5S0_CFG_CH_PTCD_TRSCMD_Msk) |
            ((((uint32_t) p_ctrl->p_cfg->psi5s_sync_cmd_setting[channel_index].sync_cmd_trans_chid) <<
                R_PSI5S0_CFG_CH_PTCD_CHID_Pos) &
             R_PSI5S0_CFG_CH_PTCD_CHID_Msk);

        /* Set TX command data */
        p_ctrl->p_reg->CFG_CH[channel_index].PTCD = ptcd_reg; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
}

/*******************************************************************************************************************//**
 * Set PSI5S frame reception config for PSI5S mode according to user configuration.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in Configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_frame_reception (psi5s_instance_ctrl_t const * const p_ctrl)
{
    psi5s_extended_cfg_t const * p_extend = (psi5s_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* Specify the desired settings in the PSI5S frame reception configuration for CH0 */

    /* PSI5S receive config1 CH n register: PSI5SjPRCF1n
     *  [27:24] PFRMIDLE :  Minimum packet frame gap (only ch0)
     *  [23:21] F6PKT :  Frame 6 packet number
     *  [20:18] F5PKT :  Frame 5 packet number
     *  [17:15] F4PKT :  Frame 4 packet number
     *  [14:12] F3PKT :  Frame 3 packet number
     *  [11:9] F2PKT :  Frame 2 packet number
     *  [8:6] F1PKT :  Frame 1 packet number
     *  [5] SYSEL : 0: synchronous mode select
     *  [4] TSCTS : 0: Timestamp capture trigger transmission synchronous pulse timing selected
     *  [3] TSCS : 0: Timestamp counter timestamp counter B selected
     *  [2] TSEN : 0: Timestamp capture disable
     *  [1] RFCPS : 0: RX frame checksum parity selected
     *  [0] CHEN : 1: Channel enable
     */
    uint32_t prcf1_reg =
        ((((uint32_t) p_ctrl->p_cfg->frame_gap) << R_PSI5S0_PRCF10_PFRMIDLE_Pos) & R_PSI5S0_PRCF10_PFRMIDLE_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[0].packet_number) <<
            R_PSI5S0_PRCF10_F1PKT_Pos) & R_PSI5S0_PRCF10_F1PKT_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[1].packet_number) <<
            R_PSI5S0_PRCF10_F2PKT_Pos) & R_PSI5S0_PRCF10_F2PKT_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[2].packet_number) <<
            R_PSI5S0_PRCF10_F3PKT_Pos) & R_PSI5S0_PRCF10_F3PKT_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[3].packet_number) <<
            R_PSI5S0_PRCF10_F4PKT_Pos) & R_PSI5S0_PRCF10_F4PKT_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[4].packet_number) <<
            R_PSI5S0_PRCF10_F5PKT_Pos) & R_PSI5S0_PRCF10_F5PKT_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[5].packet_number) <<
            R_PSI5S0_PRCF10_F6PKT_Pos) & R_PSI5S0_PRCF10_F6PKT_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].ts_count_sel) <<
            R_PSI5S0_PRCF10_TSCS_Pos) & R_PSI5S0_PRCF10_TSCS_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].ts_capture_en) <<
            R_PSI5S0_PRCF10_TSEN_Pos) & R_PSI5S0_PRCF10_TSEN_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].checksum) <<
            R_PSI5S0_PRCF10_RFCPS_Pos) & R_PSI5S0_PRCF10_RFCPS_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].channel_en) <<
            R_PSI5S0_PRCF10_CHEN_Pos) & R_PSI5S0_PRCF10_CHEN_Msk);

    /* PSI5S receive config2 CH n register: PSI5SjPRCF2n
     *  [23:21] F6PAYLD :  The pay load bit length of packet frame 6
     *  [20:18] F5PAYLD :  The pay load bit length of packet frame 5
     *  [17:15] F4PAYLD :  The pay load bit length of packet frame 4
     *  [14:12] F3PAYLD :  The pay load bit length of packet frame 3
     *  [11:9] F2PAYLD :  The pay load bit length of packet frame 2
     *  [8:6] F1PAYLD :  The pay load bit length of packet frame 1
     */
    uint32_t prcf2_reg =
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[0].payload_bit_length) <<
            R_PSI5S0_PRCF20_F1PAYLD_Pos) & R_PSI5S0_PRCF20_F1PAYLD_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[1].payload_bit_length) <<
            R_PSI5S0_PRCF20_F2PAYLD_Pos) & R_PSI5S0_PRCF20_F2PAYLD_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[2].payload_bit_length) <<
            R_PSI5S0_PRCF20_F3PAYLD_Pos) & R_PSI5S0_PRCF20_F3PAYLD_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[3].payload_bit_length) <<
            R_PSI5S0_PRCF20_F4PAYLD_Pos) & R_PSI5S0_PRCF20_F4PAYLD_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[4].payload_bit_length) <<
            R_PSI5S0_PRCF20_F5PAYLD_Pos) & R_PSI5S0_PRCF20_F5PAYLD_Msk) |
        ((((uint32_t) p_extend->psi5s_frame_rx_setting[PSI5S_CHANNEL_0].frame_packet[5].payload_bit_length) <<
            R_PSI5S0_PRCF20_F6PAYLD_Pos) & R_PSI5S0_PRCF20_F6PAYLD_Msk);

    /* Set frame reception config for CH0 */
    p_ctrl->p_reg->PRCF10 = prcf1_reg; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->PRCF20 = prcf2_reg; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Specify the desired settings in the PSI5S frame reception configuration for CH1 -> CH7 */
    for (uint8_t ch = 1U; ch < PSI5S_NUM_CHANNELS; ch++)
    {
        /* PSI5S receive config1 CH n register */
        prcf1_reg =
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[0].packet_number) <<
                R_PSI5S0_CFG_CH_PRCF1_F1PKT_Pos) & R_PSI5S0_CFG_CH_PRCF1_F1PKT_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[1].packet_number) <<
                R_PSI5S0_CFG_CH_PRCF1_F2PKT_Pos) & R_PSI5S0_CFG_CH_PRCF1_F2PKT_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[2].packet_number) <<
                R_PSI5S0_CFG_CH_PRCF1_F3PKT_Pos) & R_PSI5S0_CFG_CH_PRCF1_F3PKT_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[3].packet_number) <<
                R_PSI5S0_CFG_CH_PRCF1_F4PKT_Pos) & R_PSI5S0_CFG_CH_PRCF1_F4PKT_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[4].packet_number) <<
                R_PSI5S0_CFG_CH_PRCF1_F5PKT_Pos) & R_PSI5S0_CFG_CH_PRCF1_F5PKT_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[5].packet_number) <<
                R_PSI5S0_CFG_CH_PRCF1_F6PKT_Pos) & R_PSI5S0_CFG_CH_PRCF1_F6PKT_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].sync_mode) << R_PSI5S0_CFG_CH_PRCF1_SYSEL_Pos) &
             R_PSI5S0_CFG_CH_PRCF1_SYSEL_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].ts_cap_trigger) << R_PSI5S0_CFG_CH_PRCF1_TSCTS_Pos) &
             R_PSI5S0_CFG_CH_PRCF1_TSCTS_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].ts_count_sel) << R_PSI5S0_CFG_CH_PRCF1_TSCS_Pos) &
             R_PSI5S0_CFG_CH_PRCF1_TSCS_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].ts_capture_en) << R_PSI5S0_CFG_CH_PRCF1_TSEN_Pos) &
             R_PSI5S0_CFG_CH_PRCF1_TSEN_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].checksum) << R_PSI5S0_CFG_CH_PRCF1_RFCPS_Pos) &
             R_PSI5S0_CFG_CH_PRCF1_RFCPS_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].channel_en) << R_PSI5S0_CFG_CH_PRCF1_CHEN_Pos) &
             R_PSI5S0_CFG_CH_PRCF1_CHEN_Msk);

        /* PSI5S receive config2 CH n register */
        prcf2_reg =
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[0].payload_bit_length) <<
                R_PSI5S0_CFG_CH_PRCF2_F1PAYLD_Pos) & R_PSI5S0_CFG_CH_PRCF2_F1PAYLD_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[1].payload_bit_length) <<
                R_PSI5S0_CFG_CH_PRCF2_F2PAYLD_Pos) & R_PSI5S0_CFG_CH_PRCF2_F2PAYLD_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[2].payload_bit_length) <<
                R_PSI5S0_CFG_CH_PRCF2_F3PAYLD_Pos) & R_PSI5S0_CFG_CH_PRCF2_F3PAYLD_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[3].payload_bit_length) <<
                R_PSI5S0_CFG_CH_PRCF2_F4PAYLD_Pos) & R_PSI5S0_CFG_CH_PRCF2_F4PAYLD_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[4].payload_bit_length) <<
                R_PSI5S0_CFG_CH_PRCF2_F5PAYLD_Pos) & R_PSI5S0_CFG_CH_PRCF2_F5PAYLD_Msk) |
            ((((uint32_t) p_extend->psi5s_frame_rx_setting[ch].frame_packet[5].payload_bit_length) <<
                R_PSI5S0_CFG_CH_PRCF2_F6PAYLD_Pos) & R_PSI5S0_CFG_CH_PRCF2_F6PAYLD_Msk);

        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(ch);   /* PRQA S 3469 # Advisory Dir-4.9 */

        /* Set frame reception config for CH n */
        p_ctrl->p_reg->CFG_CH[channel_index].PRCF1 = prcf1_reg; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
        p_ctrl->p_reg->CFG_CH[channel_index].PRCF2 = prcf2_reg; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
}

/*******************************************************************************************************************//**
 * Set TX frame data for PSI5S mode.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 *
 * @note Preconditions (ensured by caller):
 *       - PSI5S operation is in PSI5S mode or Configuration mode.
 **********************************************************************************************************************/
static void r_psi5s_set_tx_frame_data (psi5s_instance_ctrl_t const * const p_ctrl)
{
    /* PSI5S TX frame data1 register: PSI5SjPTFD1
     *  [31:24] TDT4 :  Define the transmission data of the 4th packet
     *  [23:16] TDT3 :  Define the transmission data of the 3rd packet
     *  [15:8] TDT2 :  Define the transmission data of the 2nd packet
     *  [7:0] TDT1 :  Define the transmission data of the 1st packet
     */
    uint32_t ptfd1_reg =
        ((((uint32_t) p_ctrl->cur_tdt[3]) << R_PSI5S0_PTFD1_TDT4_Pos) & R_PSI5S0_PTFD1_TDT4_Msk) |
        ((((uint32_t) p_ctrl->cur_tdt[2]) << R_PSI5S0_PTFD1_TDT3_Pos) & R_PSI5S0_PTFD1_TDT3_Msk) |
        ((((uint32_t) p_ctrl->cur_tdt[1]) << R_PSI5S0_PTFD1_TDT2_Pos) & R_PSI5S0_PTFD1_TDT2_Msk) |
        ((((uint32_t) p_ctrl->cur_tdt[0]) << R_PSI5S0_PTFD1_TDT1_Pos) & R_PSI5S0_PTFD1_TDT1_Msk);

    /* PSI5S TX frame data2 register: PSI5SjPTFD2
     *  [31:24] TDT8 :  Define the transmission data of the 8th packet
     *  [23:16] TDT7 :  Define the transmission data of the 7th packet
     *  [15:8] TDT6 :  Define the transmission data of the 6th packet
     *  [7:0] TDT5 :  Define the transmission data of the 5th packet
     */
    uint32_t ptfd2_reg =
        ((((uint32_t) p_ctrl->cur_tdt[7]) << R_PSI5S0_PTFD2_TDT8_Pos) & R_PSI5S0_PTFD2_TDT8_Msk) |
        ((((uint32_t) p_ctrl->cur_tdt[6]) << R_PSI5S0_PTFD2_TDT7_Pos) & R_PSI5S0_PTFD2_TDT7_Msk) |
        ((((uint32_t) p_ctrl->cur_tdt[5]) << R_PSI5S0_PTFD2_TDT6_Pos) & R_PSI5S0_PTFD2_TDT6_Msk) |
        ((((uint32_t) p_ctrl->cur_tdt[4]) << R_PSI5S0_PTFD2_TDT5_Pos) & R_PSI5S0_PTFD2_TDT5_Msk);

    /* PSI5S TX frame data2 register: PSI5SjPTFD2
     *  [2:0] TXNUM :  TX command data number (1->8)
     */
    uint32_t ptfnm_reg =
        ((((uint32_t) p_ctrl->cur_txnum) << R_PSI5S0_PTFNM_TXNUM_Pos) & R_PSI5S0_PTFNM_TXNUM_Msk);

    /* Set TX frame data */
    p_ctrl->p_reg->PTFD1 = ptfd1_reg;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    p_ctrl->p_reg->PTFD2 = ptfd2_reg;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

    /* Set TX frame number */
    p_ctrl->p_reg->PTFNM = ptfnm_reg;  /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
}

/*******************************************************************************************************************//**
 * Set DDSR pack data for PSI5S mode.
 *
 * @param[in]   tx_data               Transmit data (address and data)
 * @param[in]   type                  ECU to Sensor Frame Types
 *
 * @return DDSR pack Data.
 **********************************************************************************************************************/
static uint32_t r_psi5s_ddsr_pack_data (uint32_t tx_data, psi5s_ddsr_trans_type_t type)
{
    const uint32_t raw_addr = tx_data & PSI5S_DDSR_ADDR_MASK; /* Lower 4 bits */
    const uint32_t raw_data = tx_data >> 4;                   /* Upper bits are data */

    const uint8_t psi5s_ddsr_transmit_type[PSI5S_DDSR_TRANS_TYPE_NUM][PSI5S_DDSR_TRANS_TYPE_SIZE] =
    {
        [PSI5S_DDSR_TRANS_TYPE_SHORT]  = {PSI5S_DDSR_DATA_BITS_SHORT },
        [PSI5S_DDSR_TRANS_TYPE_LONG]   = {PSI5S_DDSR_DATA_BITS_LONG  },
        [PSI5S_DDSR_TRANS_TYPE_XLONG]  = {PSI5S_DDSR_DATA_BITS_XLONG },
        [PSI5S_DDSR_TRANS_TYPE_XXLONG] = {PSI5S_DDSR_DATA_BITS_XXLONG}
    };

    /* type is validated in R_PSI5S_Write() */
    uint32_t data_bits = psi5s_ddsr_transmit_type[type][0];

    /* Mask used data (in 20-bit field) */
    uint32_t data_mask = (((uint32_t) 1U) << data_bits) - ((uint32_t) 1U);

    uint32_t used_data   = raw_data & data_mask;
    uint32_t unused_mask = (uint32_t) (PSI5S_DDSR_DATA_FIELD_MASK & (~data_mask));

    /* Set '1' for unused field */
    uint32_t data_field = used_data | unused_mask;
    uint32_t pdddn_reg  = (data_field << 4) | (raw_addr & PSI5S_DDSR_ADDR_MASK);

    /* Bits [31:24] = 0 (reserved), [23:4] = data_field, [3:0] = raw_addr */
    return pdddn_reg;
}

/*******************************************************************************************************************//**
 * Get mailbox data and store at local buffer p_ctrl->psi5s_mailbox.
 *
 * @param[in]   p_ctrl               Pointer to instance control block
 * @param[in]   channel               Current channel.
 *
 **********************************************************************************************************************/
static void r_psi5s_read_mailbox (psi5s_instance_ctrl_t * p_ctrl, psi5s_channel_t channel)
{
    uint8_t cur_dci = PSI5S_REG_RESET_VALUE;
    uint8_t new_dci = PSI5S_REG_RESET_VALUE;

    /* CH0 has only 2 packet frames */
    if (PSI5S_CHANNEL_0 == channel)
    {
        /* Read data in order S -> D -> T */
        for (uint8_t frame = 0U; frame < PSI5S_CH0_NUM_PACKET_FRAMES; frame++)
        {
            cur_dci =
                (uint8_t) (((p_ctrl->psi5s_mailbox[channel][frame].rx_data_status) & R_PSI5S0_MB_CH_FRM_PMBT_DCI_Msk) >>
                           R_PSI5S0_MB_CH_FRM_PMBT_DCI_Pos);
            new_dci =
                (uint8_t) (((p_ctrl->p_reg->MB_CH0_FRM[frame].PMBS) & R_PSI5S0_MB_CH_FRM_PMBT_DCI_Msk) >> /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                           R_PSI5S0_MB_CH_FRM_PMBT_DCI_Pos);

            if ((cur_dci != new_dci) || (new_dci == PSI5S_REG_RESET_VALUE))
            {
                p_ctrl->psi5s_mailbox[channel][frame].rx_data_status = p_ctrl->p_reg->MB_CH0_FRM[frame].PMBS; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                p_ctrl->psi5s_mailbox[channel][frame].rx_data        = p_ctrl->p_reg->MB_CH0_FRM[frame].PMBD; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                p_ctrl->psi5s_mailbox[channel][frame].rx_timestamp   = p_ctrl->p_reg->MB_CH0_FRM[frame].PMBT; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

                /* new data that user haven't read*/
                p_ctrl->psi5s_mailbox[channel][frame].rx_unread = true;
            }
        }

        /* If the 2D array is sized according to max frame (6), zero non-existing slots (CH0) to clear the data */
        for (uint8_t frame = PSI5S_CH0_NUM_PACKET_FRAMES; frame < PSI5S_NUM_PACKET_FRAMES; ++frame)
        {
            p_ctrl->psi5s_mailbox[channel][frame].rx_data_status = PSI5S_REG_RESET_VALUE;
            p_ctrl->psi5s_mailbox[channel][frame].rx_data        = PSI5S_REG_RESET_VALUE;
            p_ctrl->psi5s_mailbox[channel][frame].rx_timestamp   = PSI5S_REG_RESET_VALUE;
        }
    }
    else
    {
        /* Read data in order S -> D -> T */
        for (uint8_t frame = 0U; frame < PSI5S_NUM_PACKET_FRAMES; frame++)
        {
            cur_dci =
                (uint8_t) (((p_ctrl->psi5s_mailbox[channel][frame].rx_data_status) & R_PSI5S0_MB_CH_FRM_PMBT_DCI_Msk) >>
                           R_PSI5S0_MB_CH_FRM_PMBT_DCI_Pos);
            new_dci =
                (uint8_t) (((p_ctrl->p_reg->MB_CH[channel].FRM[frame].PMBS) & R_PSI5S0_MB_CH_FRM_PMBT_DCI_Msk) >> /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                           R_PSI5S0_MB_CH_FRM_PMBT_DCI_Pos);

            if ((cur_dci != new_dci) || (new_dci == PSI5S_REG_RESET_VALUE))
            {
                p_ctrl->psi5s_mailbox[channel][frame].rx_data_status = p_ctrl->p_reg->MB_CH[channel].FRM[frame].PMBS; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                p_ctrl->psi5s_mailbox[channel][frame].rx_data        = p_ctrl->p_reg->MB_CH[channel].FRM[frame].PMBD; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
                p_ctrl->psi5s_mailbox[channel][frame].rx_timestamp   = p_ctrl->p_reg->MB_CH[channel].FRM[frame].PMBT; /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */

                /* new data that user haven't read*/
                p_ctrl->psi5s_mailbox[channel][frame].rx_unread = true;
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Calls user callback
 *
 * @param[in]     p_ctrl                Pointer to the instance control block.
 * @param[in]     event_context         PSI5S error/event context records.
 * @param[in]     event                 PSI5 event
 **********************************************************************************************************************/
static void r_psi5s_call_callback (psi5s_instance_ctrl_t const * const p_ctrl,
                                   psi5s_event_context_t               event_context,
                                   psi5s_event_t                       event)
{
    psi5s_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function.
     */
    psi5s_callback_args_t * p_args = p_ctrl->p_callback_memory;

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

    p_args->p_context    = p_ctrl->p_context;
    p_args->event        = event;
    p_args->packet_frame = event_context.event_packet_frame;
    p_args->channel      = event_context.channel;

    /* Now do the callback here */
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

static void r_psi5s_clear_interrupt_status (psi5s_instance_ctrl_t * const p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                            psi5s_channel_t               channel,
                                            uint32_t                      status_mask)
{
    if (PSI5S_CHANNEL_0 == channel)
    {
        /* Clear channel 0 CPU interrupt status (W1C). */
        p_ctrl->p_reg->PCISC0 = status_mask;                                          /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
    else
    {
        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(channel);                    /* PRQA S 3469 # Advisory Dir-4.9 */

        /* Clear channel n CPU interrupt status (W1C). */
        p_ctrl->p_reg->CFG_CH[channel_index].PCISC = status_mask;                     /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
}

static void r_psi5s_clear_receive_error_status (psi5s_instance_ctrl_t * const p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                                psi5s_channel_t               channel,
                                                uint8_t                       error_mask)
{
    if (PSI5S_CHANNEL_0 == channel)
    {
        /* Clear channel 0 receive error status (W1C). */
        p_ctrl->p_reg->PRESC0 = error_mask;                        /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
    else
    {
        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(channel); /* PRQA S 3469 # Advisory Dir-4.9 */

        /* Clear channel n receive error status (W1C). */
        p_ctrl->p_reg->CFG_CH[channel_index].PRESC = error_mask;   /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
}

/*******************************************************************************************************************//**
 * RX/TX status handler
 *
 * @param[in]   p_ctrl                Pointer to the instance control block.
 * @param[in]   channel               Current channel.
 **********************************************************************************************************************/
static void r_psi5s_status_handler (psi5s_instance_ctrl_t * const p_ctrl, psi5s_channel_t channel)
{
    /* PSI5S error packet frame table. */
    static const uint8_t
        psi5s_rx_error_packet_frame[PSI5S_RX_ERROR_PACKET_FRAME_NUM][PSI5S_RX_ERROR_PACKET_FRAME_SIZE] =
    {
        [PSI5S_PACKET_FRAME_NONE] = {(uint8_t) PSI5S_REG_RESET_VALUE          },
        [PSI5S_PACKET_FRAME_1]    = {(uint8_t) R_PSI5S0_CFG_CH_PRES_RERRF1_Msk},
        [PSI5S_PACKET_FRAME_2]    = {(uint8_t) R_PSI5S0_CFG_CH_PRES_RERRF2_Msk},
        [PSI5S_PACKET_FRAME_3]    = {(uint8_t) R_PSI5S0_CFG_CH_PRES_RERRF3_Msk},
        [PSI5S_PACKET_FRAME_4]    = {(uint8_t) R_PSI5S0_CFG_CH_PRES_RERRF4_Msk},
        [PSI5S_PACKET_FRAME_5]    = {(uint8_t) R_PSI5S0_CFG_CH_PRES_RERRF5_Msk},
        [PSI5S_PACKET_FRAME_6]    = {(uint8_t) R_PSI5S0_CFG_CH_PRES_RERRF6_Msk},
    };

    /* Get the packet frame type in which the error is detected. */
    psi5s_packet_frame_t err_packet_frame =
        r_psi5s_get_err_packet_frame(p_ctrl, channel);

    uint8_t const receive_error_mask =
        psi5s_rx_error_packet_frame[err_packet_frame][0];

    uint32_t int_status_reg = PSI5S_REG_RESET_VALUE;

    /* Get RX/TX status without clearing it here. */
    if (PSI5S_CHANNEL_0 == channel)
    {
        /* Get channel 0 CPU interrupt status. */
        int_status_reg = p_ctrl->p_reg->PCIS0;                     /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }
    else
    {
        uint8_t channel_index = PSI5S_CH_TO_CFG_CH_INDEX(channel); /* PRQA S 3469 # Advisory Dir-4.9 */

        /* Get channel n CPU interrupt status. */
        int_status_reg =
            p_ctrl->p_reg->CFG_CH[channel_index].PCIS;             /* PRQA S 0662 # Advisory R-1.2/Dir-1.1 */
    }

    /* Get PSI5S error and event context records. */
    psi5s_event_context_t event_context =
    {
        .event_packet_frame = err_packet_frame,
        .channel            = channel
    };

    /* Command TX finish: CH0 only. */
    if (R_PSI5S0_PCIS0_ISTCTFN_Msk ==
        (int_status_reg & R_PSI5S0_PCIS0_ISTCTFN_Msk))
    {
        r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_TX_CMD_FINISH);

        /* Clear command TX finish status after callback. */
        r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_PCIS0_ISTCTFN_Msk); /* PRQA S 1257 # Required R-10.3 */
    }

    /* DDSR TX finish: CH1 - CH7. */
    if (R_PSI5S0_CFG_CH_PCIS_ISTDDSFN_Msk ==
        (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTDDSFN_Msk))
    {
        r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_DDSR_FINISH);

        /* Clear DDSR TX finish status after callback. */
        r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTDDSFN_Msk); /* PRQA S 1257 # Required R-10.3 */
    }

    /* RX finish status handler. */
    if (R_PSI5S0_CFG_CH_PCIS_ISTRFN_Msk ==
        (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTRFN_Msk))
    {
        /* Read data from mailbox. */
        r_psi5s_read_mailbox(p_ctrl, channel);

        r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_FINISH);

        /* Clear RX finish status after mailbox processing and callback. */
        r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTRFN_Msk); /* PRQA S 1257 # Required R-10.3 */
    }
    else
    {
        /* DDSR overwrite error: CH1 - CH7. */
        if (R_PSI5S0_CFG_CH_PCIS_ISTDDSOW_Msk ==
            (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTDDSOW_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_DDSR_OVERWRITE_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTDDSOW_Msk); /* PRQA S 1257 # Required R-10.3 */

            /*
             * DDSR overwrite is not an RX packet-frame error,
             * so PRESC is not cleared here.
             */
        }

        /* RX frame excess error: CH0 - CH7. */
        if (R_PSI5S0_CFG_CH_PCIS_ISTRFEX_Msk ==
            (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTRFEX_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_FRAME_EXCESS_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTRFEX_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* RX frame lack error: CH0 - CH7. */
        if (R_PSI5S0_CFG_CH_PCIS_ISTRFLK_Msk ==
            (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTRFLK_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_FRAME_LACK_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTRFLK_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* RX overrun error: CH0 - CH7. */
        if (R_PSI5S0_CFG_CH_PCIS_ISTROV_Msk ==
            (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTROV_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_OVERRUN_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTROV_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* RX watchdog error: CH0 - CH7. */
        if (R_PSI5S0_CFG_CH_PCIS_ISTRWDT_Msk ==
            (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTRWDT_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_WDT_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTRWDT_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* RX UART framing error: CH0 only. */
        if (R_PSI5S0_PCIS0_ISTUTFR_Msk ==
            (int_status_reg & R_PSI5S0_PCIS0_ISTUTFR_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_UART_FRAME_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_PCIS0_ISTUTFR_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* RX UART parity error: CH0 only. */
        if (R_PSI5S0_PCIS0_ISTUTPT_Msk ==
            (int_status_reg & R_PSI5S0_PCIS0_ISTUTPT_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_UART_PARITY_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_PCIS0_ISTUTPT_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* RX transceiver status error: CH0 - CH7. */
        if (R_PSI5S0_CFG_CH_PCIS_ISTTRST_Msk ==
            (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTTRST_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_TRANSCEIVER_STATUS_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTTRST_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* Payload parity error: CH0 - CH7. */
        if (R_PSI5S0_CFG_CH_PCIS_ISTPT_Msk ==
            (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTPT_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_PAYLOAD_PARITY_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTPT_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* Payload CRC error: CH0 - CH7. */
        if (R_PSI5S0_CFG_CH_PCIS_ISTCRC_Msk ==
            (int_status_reg & R_PSI5S0_CFG_CH_PCIS_ISTCRC_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_PAYLOAD_CRC_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_CFG_CH_PCIS_ISTCRC_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }

        /* Packet-frame XCRC error: CH0 only. */
        if (R_PSI5S0_PCIS0_ISTXCRC_Msk ==
            (int_status_reg & R_PSI5S0_PCIS0_ISTXCRC_Msk))
        {
            r_psi5s_call_callback(p_ctrl, event_context, PSI5S_EVENT_RX_PACKET_FRAME_XCRC_ERR);

            r_psi5s_clear_interrupt_status(p_ctrl, channel, R_PSI5S0_PCIS0_ISTXCRC_Msk); /* PRQA S 1257 # Required R-10.3 */

            r_psi5s_clear_receive_error_status(p_ctrl, channel, receive_error_mask);
        }
    }
}

/*******************************************************************************************************************//**
 * PSI5S ISR
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void psi5s_ch_isr (void) /* PRQA S 1503 # Required R-2.1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                             /* PRQA S 3432, 4461 # Advisory R-20.7, Required R-10.3 */
    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Clear the level interrupt flag of the peripheral module*/
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    psi5s_instance_ctrl_t * p_ctrl = (psi5s_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    uint16_t base_irq    = (uint16_t) p_ctrl->p_cfg->communication_irq[0];
    uint16_t current_irq = (uint16_t) irq;

    if (current_irq >= base_irq)
    {
        uint16_t channel_index = (uint16_t) (current_irq - base_irq);

        if (channel_index < PSI5S_NUM_CHANNELS)
        {
            static const psi5s_channel_t psi5s_channel_lut[PSI5S_NUM_CHANNELS] =
            {
                PSI5S_CHANNEL_0, PSI5S_CHANNEL_1, PSI5S_CHANNEL_2, PSI5S_CHANNEL_3,
                PSI5S_CHANNEL_4, PSI5S_CHANNEL_5, PSI5S_CHANNEL_6, PSI5S_CHANNEL_7,
            };

            r_psi5s_status_handler(p_ctrl, psi5s_channel_lut[channel_index]);
        }
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE                /* PRQA S 3432 # Advisory R-20.7 */
}
