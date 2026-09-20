/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_i3c.h"                     /* PRQA S 0380 # Required R-1.1 */
#include "r_i3c_cfg.h"

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_i3c_internal_state      /* PRQA S 3213, 1536 # Advisory R-2.4 */
{
    I3C_INTERNAL_STATE_DISABLED = 0U,
    I3C_INTERNAL_STATE_MASTER_IDLE,
    I3C_INTERNAL_STATE_MASTER_ENTDAA        = I3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE,
    I3C_INTERNAL_STATE_MASTER_WRITE         = I3C_EVENT_WRITE_COMPLETE,
    I3C_INTERNAL_STATE_MASTER_READ          = I3C_EVENT_READ_COMPLETE,
    I3C_INTERNAL_STATE_MASTER_COMMAND_WRITE = I3C_EVENT_COMMAND_COMPLETE,
    I3C_INTERNAL_STATE_MASTER_COMMAND_READ  = ((uint32_t)I3C_EVENT_COMMAND_COMPLETE | (0x80U)),
    I3C_INTERNAL_STATE_SLAVE_IDLE,
    I3C_INTERNAL_STATE_SLAVE_IBI = I3C_EVENT_IBI_WRITE_COMPLETE,
} i3c_internal_state_t;                      /* PRQA S 3205, 1535 # Advisory R-2.3 */

typedef enum e_i3c_slave_error_recovery_type /* PRQA S 3213, 1536 # Advisory R-2.4 */
{
    I3C_SLAVE_ERROR_RECOVERY_TYPE_WRITE = 0,
    I3C_SLAVE_ERROR_RECOVERY_TYPE_READ  = 1,
    I3C_SLAVE_ERROR_RECOVERY_TYPE_IBI   = 2,
    I3C_SLAVE_ERROR_RECOVERY_TYPE_OTHER = 3,
} i3c_slave_error_recovery_type_t;

typedef enum e_i3c_bsp_irq             /* PRQA S 3213, 1536 # Advisory R-2.4 */
{
    I3C_BSP_IRQ_DISABLE = 0,
    I3C_BSP_IRQ_ENABLE,
    I3C_BSP_IRQ_CFG_ENABLE,
    I3C_BSP_IRQ_CFG_ERR_ENABLE,
} i3c_bsp_irq_t;

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define I3C_OPEN                          (((uint32_t) 'I' << 16U) | ((uint32_t) '3' << 8U) | ((uint32_t) 'C' << 0U))

/* Address that is written by the slave during a Hot Join request. */
#define I3C_HOT_JOIN_ADDRESS              (2U)

/* Mask for flushing the Read, Write, and Command FIFO. */
#define I3C_RSTCTRL_FIFO_FLUSH_Msk        (R_RI3C0_RSTCTL_CMDQRST_Msk | R_RI3C0_RSTCTL_TDBRST_Msk | \
                                           R_RI3C0_RSTCTL_RDBRST_Msk)

/* Mask for converting the internal state into an I3C event. */
#define I3C_INTERNAL_EVENT_MASK           (0x7FU)

/* Number of bytes transferred in a single transfer */
#define I3C_NUM_BYTES_PER_TRANSFER        (I3C_TRANSFER_SIZE_PER_COUNT) /* PRQA S 1534 # Advisory R-2.5 */

/* The flag indicates that the data buffer is valid */
/* Number of bytes transferred per one transfer count (1 word = 4 byte)*/
#define I3C_TRANSFER_SIZE_PER_COUNT       (4U)                          /* PRQA S 1534 # Advisory R-2.5 */

/* -----------------------------------------------------------------------------------------------------------------
 * I3C_TDBFLV_POLL_MAX_COUNT: Maximum retry count when polling HDBSTLV.TDBFLV inside
 * r_i3c_load_number_count(), in order to avoid an infinite ISR hang in case TDBFLV has not been
 * updated yet due to clock domain crossing (TCLK vs PCLK) or because the bus is currently
 * running at a low baudrate.
 *
 * Formula: (CPU_CORE_CLOCK_MAX_HZ * BITS_PER_BYTE) / PCLK_MIN_HZ
 *        = (400,000,000 * 8) / 25,000,000
 *        = 128
 *
 * Where:
 *   - CPU_CORE_CLOCK_MAX_HZ = 400 MHz (worst-case CPU clock of the current project)
 *   - PCLK_MIN_HZ           = 25  MHz (worst-case PCLK, see CLKC_HBUS_I3C in the BSP clock
 *                                      configuration)
 *   - BITS_PER_BYTE         = 8   (minimum time required to wait for 1 byte to be transmitted
 *                                  onto the bus, expressed in PCLK cycle units)
 * --------------------------------------------------------------------------------------------------------------- */
#define I3C_TDBFLV_POLL_MAX_COUNT         (128U)                        /* PRQA S 1534 # Advisory R-2.5 */

/* Maximum TX DMA/DTS transfer count per TX empty callback.
 *
 * RI3C driver currently configures Normal/High Priority TX/RX data buffer threshold to 0,
 * and the driver comment states that the only supported configuration is two entries [1].
 *
 * HWUM: TXDBTH = 0 means the TX empty interrupt/status is triggered when there are
 * 2 TX buffer empty entries in DWORD units [2].
 */
#define I3C_TX_DMAC_MAX_TRANSFER_COUNT    (2U)                          /* PRQA S 1534 # Advisory R-2.5 */

/* Flag helper used when checking TX Data Buffer Empty status. */
#define I3C_TX_EMPTY_FLAG_NOT_SET         (0U)                          /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_TX_EMPTY_FLAG_SET             (1U)                          /* PRQA S 1534 # Advisory R-2.5 */

/***********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/

/* ISR function prototypes. */
BSP_INTERRUPT_ATTRIBUTE void i3c_resp_isr(void); /* PRQA S 3449, 3451 # Published API */
BSP_INTERRUPT_ATTRIBUTE void i3c_rx_isr(void);   /* PRQA S 3449, 3451 # Published API */
BSP_INTERRUPT_ATTRIBUTE void i3c_tx_isr(void);   /* PRQA S 3449, 3451 # Published API */
BSP_INTERRUPT_ATTRIBUTE void i3c_rcv_isr(void);  /* PRQA S 3449, 3451 # Published API */
BSP_INTERRUPT_ATTRIBUTE void i3c_ibi_isr(void);  /* PRQA S 3449, 3451 # Published API */
BSP_INTERRUPT_ATTRIBUTE void i3c_eei_isr(void);  /* PRQA S 3449, 3451 # Published API */

static uint8_t i3c_address_parity(uint8_t address);

#if I3C_CFG_MASTER_SUPPORT
static uint32_t i3c_xfer_command_calculate(uint32_t dev_index, bool rnw, uint32_t bitrate_setting, bool restart);
static void     i3c_internal_command_config(i3c_instance_ctrl_t * p_ctrl, i3c_mipi_cmd_t mipi_cmd);

#endif
static void i3c_fifo_read(i3c_instance_ctrl_t * p_ctrl, uint32_t bytes);
static void i3c_fifo_write(i3c_instance_ctrl_t * p_ctrl);
static void i3c_read_buffer_store(i3c_instance_ctrl_t * const    p_ctrl,
                                  i3c_read_buffer_descriptor_t * p_buffer_descriptor,
                                  uint32_t                       data_word,
                                  uint32_t                       num_bytes,
                                  i3c_event_t                    buffer_full_event);
static uint32_t     i3c_read_bytes_remaining_calculate(i3c_instance_ctrl_t * p_ctrl, uint32_t data_length);
static uint32_t     i3c_next_data_word_calculate(i3c_write_buffer_descriptor_t * p_buffer_descriptor);
static i3c_ctrl_t * i3c_isr_context_get(void);

#if I3C_CFG_MASTER_SUPPORT
static void i3c_master_error_recovery(i3c_instance_ctrl_t * p_ctrl, bool error_recovery_case_2);

#endif
#if I3C_CFG_SLAVE_SUPPORT
static void i3c_slave_error_recovery(i3c_instance_ctrl_t * p_ctrl, i3c_slave_error_recovery_type_t recovery_type);

#endif
static void i3c_internal_reset(i3c_instance_ctrl_t * p_ctrl);

#if I3C_CFG_TRANSFER_ENABLE
static fsp_err_t r_i3c_transfer_open(i3c_instance_ctrl_t * p_ctrl);
void             r_i3c_txi_transfer_callback(transfer_callback_args_t * p_args);
void             r_i3c_rxi_transfer_callback(transfer_callback_args_t * p_args);
static void      r_i3c_rxi_transfer(i3c_instance_ctrl_t * p_ctrl);
static uint16_t  r_i3c_load_number_count(i3c_instance_ctrl_t * p_ctrl);

#endif
static void r_i3c_bsp_irq(i3c_instance_ctrl_t * p_ctrl, IRQn_Type const irq, i3c_bsp_irq_t irq_type);

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* I3C implementation of I3C API. */
const i3c_api_t g_i3c_on_i3c =                                            /* PRQA S 1502 # Published object */
{
    .open                          = R_I3C_Open,                          /* PRQA S 3635 # Advisory R-13.4 */
    .enable                        = R_I3C_Enable,                        /* PRQA S 3635 # Advisory R-13.4 */
    .deviceCfgSet                  = R_I3C_DeviceCfgSet,                  /* PRQA S 3635 # Advisory R-13.4 */
    .masterDeviceTableSet          = R_I3C_MasterDeviceTableSet,          /* PRQA S 3635 # Advisory R-13.4 */
    .slaveStatusSet                = R_I3C_SlaveStatusSet,                /* PRQA S 3635 # Advisory R-13.4 */
    .deviceSelect                  = R_I3C_DeviceSelect,                  /* PRQA S 3635 # Advisory R-13.4 */
    .dynamicAddressAssignmentStart = R_I3C_DynamicAddressAssignmentStart, /* PRQA S 3635 # Advisory R-13.4 */
    .commandSend                   = R_I3C_CommandSend,                   /* PRQA S 3635 # Advisory R-13.4 */
    .write                         = R_I3C_Write,                         /* PRQA S 3635 # Advisory R-13.4 */
    .read                          = R_I3C_Read,                          /* PRQA S 3635 # Advisory R-13.4 */
    .ibiWrite                      = R_I3C_IbiWrite,                      /* PRQA S 3635 # Advisory R-13.4 */
    .ibiRead                       = R_I3C_IbiRead,                       /* PRQA S 3635 # Advisory R-13.4 */
    .close                         = R_I3C_Close,                         /* PRQA S 3635 # Advisory R-13.4 */
};

/* PRQA S 0292 ++ # Advisory D-1.1 */

/*******************************************************************************************************************//**
 * @addtogroup I3C
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configure an I3C instance. Implements @ref i3c_api_t::open.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was invalid.
 * @retval FSP_ERR_ALREADY_OPEN           Open has already been called for this instance.
 * @retval FSP_ERR_UNSUPPORTED            A selected feature is not supported with the current configuration.
 **********************************************************************************************************************/
fsp_err_t R_I3C_Open (i3c_ctrl_t * const p_api_ctrl, i3c_cfg_t const * const p_cfg)     /* PRQA S 1505 # Advisory R-8.7 */
{
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                  /* PRQA S 0316 # Advisory R-11.5 */

#if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                     /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_cfg);                                                          /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_cfg->p_callback);                                              /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_cfg->p_extend);                                                /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);                   /* PRQA S 3432 # Advisory R-20.7 */
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    FSP_ASSERT(BSP_IRQ_DISABLED != (uint32_t) p_extend->resp_irq);                      /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(BSP_IRQ_DISABLED != (uint32_t) p_extend->rcv_irq);                       /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(BSP_IRQ_DISABLED != (uint32_t) p_extend->tx_irq);                        /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(BSP_IRQ_DISABLED != (uint32_t) p_extend->rx_irq);                        /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(BSP_IRQ_DISABLED != (uint32_t) p_extend->htx_irq);                       /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(BSP_IRQ_DISABLED != (uint32_t) p_extend->hrx_irq);                       /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(BSP_IRQ_DISABLED != (uint32_t) p_extend->ibi_irq);                       /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(BSP_IRQ_DISABLED != (uint32_t) p_extend->eei_irq);                       /* PRQA S 3432 # Advisory R-20.7 */
 #if 0U == I3C_CFG_MASTER_SUPPORT
    FSP_ERROR_RETURN(I3C_DEVICE_TYPE_MAIN_MASTER != p_cfg->device_type, FSP_ERR_UNSUPPORTED);
 #endif
 #if 0U == I3C_CFG_SLAVE_SUPPORT
    FSP_ERROR_RETURN(I3C_DEVICE_TYPE_SLAVE != p_cfg->device_type, FSP_ERR_UNSUPPORTED);
 #endif
#endif

    /* Initialize the internal state of the driver. */
    (void) memset(p_ctrl, 0, sizeof(i3c_instance_ctrl_t));

    p_ctrl->open  = I3C_OPEN;
    p_ctrl->p_cfg = p_cfg;

    /* Get a pointer to the I3C registers for this channel. */
#if (BSP_FEATURE_I3C_NUM_CHANNELS > 1U)
    p_ctrl->p_reg =
        (R_RI3C0_Type *) ((uint32_t) R_RI3C0 + (p_cfg->channel * ((uint32_t) R_RI3C1 - (uint32_t) R_RI3C0))); /* PRQA S 0306, 0329 # Advisory R-11.4 */
#else
    p_ctrl->p_reg = R_RI3C0;
#endif

    /*
     * Reset the I3C Peripheral so that it is in a known state during initialization.
     */
    p_ctrl->p_reg->BCTL_b.BUSE = 0;                        /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->RSTCTL      = 1U;                       /* PRQA S 0662 # Required R-1.1 */

    /* The field will be cleared automatically upon reset completion. */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->RSTCTL, 0U); /* PRQA S 0662 # Required R-1.1 */

    /* Set internal reset to one before setting I3C mode. */
    p_ctrl->p_reg->RSTCTL = R_RI3C0_RSTCTL_INTLRST_Msk;    /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */

    /* Set I3C mode. */
    p_ctrl->p_reg->PRTS = 0U;                              /* PRQA S 0662 # Required R-1.1 */

    /* Clear internal reset. */
    p_ctrl->p_reg->RSTCTL = 0;                             /* PRQA S 0662 # Required R-1.1 */

#if I3C_CFG_TRANSFER_ENABLE

    /* Open the I3C transfer interface if available */
    fsp_err_t err = r_i3c_transfer_open(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */
#endif

    p_ctrl->activation_on_txi = false;
    p_ctrl->activation_on_rxi = false;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enable the I3C device.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           This instance is already enabled.
 **********************************************************************************************************************/
fsp_err_t R_I3C_Enable (i3c_ctrl_t * const p_api_ctrl)                                                        /* PRQA S 1505 # Advisory R-8.7 */
{
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                                        /* PRQA S 0316 # Advisory R-11.5 */

#if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                           /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                             /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_DISABLED == p_ctrl->internal_state, FSP_ERR_INVALID_MODE); /* PRQA S 3432 # Advisory R-20.7 */
#endif

    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend;               /* PRQA S 0316 # Advisory R-11.5 */

    /* PRQA S 1710 ++ # Advisory D-4.5 */

    /*
     * Write all remaining configuration settings.
     *
     * Configure the Normal IBI Data Segment Size used for receiving IBI data.
     */
    uint32_t nqthctl = (uint32_t) 6U << R_RI3C0_NQTHCTL_IBIDSSZ_Pos;

    /*
     * Enable Transfer Interrupts.
     * - Read Buffer Full Interrupt.
     * - Receive Status Queue Full Interrupt
     * - Response Queue Full Interrupt
     */
    uint32_t ntie = R_RI3C0_NTIE_RSQFIE_Msk; /* PRQA S 1257 # Advisory R-10.6, Advisory R-1.3 */

    /*
     * Enable High Transfer Interrupts.
     * - High Read Buffer Full Interrupt.
     * - High Response Queue Full Interrupt
     */
    uint32_t htie = R_RI3C0_HTIE_RDBFIE_Msk |
                    R_RI3C0_HTIE_RSPQFIE_Msk;

    /* PRQA S 1710 -- */

    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        ntie |= R_RI3C0_NTIE_RDBFIE0_Msk |
                R_RI3C0_NTIE_RSPQFIE_Msk;
    }

#if I3C_CFG_MASTER_SUPPORT
    if (I3C_DEVICE_TYPE_MAIN_MASTER == p_ctrl->p_cfg->device_type)
    {
        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE;

        /* Enable the Queue Empty/Full Interrupt. */
        ntie |= R_RI3C0_NTIE_IBIQEFIE_Msk;

        /* Write the standard and extended bitrate settings. */
        p_ctrl->p_reg->STDBR = p_extend->bitrate_settings.stdbr; /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->EXTBR = p_extend->bitrate_settings.extbr; /* PRQA S 0662 # Required R-1.1 */
    }
    else
#endif
    {
#if I3C_CFG_SLAVE_SUPPORT
        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE;

        /*
         * Configure IBI queue threshold so that an IBI IRQ is generated when there is one empty entry in the
         * IBI transmit FIFO.
         */
        nqthctl |= (uint32_t) 1U << R_RI3C0_NQTHCTL_IBIQTH_Pos;

        if (p_extend->secondary_master_enable == false)
        {
            /* Baudrate registers are not used in slave mode. */
            p_ctrl->p_reg->STDBR = 0;  /* PRQA S 0662 # Required R-1.1 */
            p_ctrl->p_reg->EXTBR = 0;  /* PRQA S 0662 # Required R-1.1 */
        }
        else
        {
            /* Baudrate registers are used in secondary master mode. */
            p_ctrl->p_reg->STDBR = p_extend->bitrate_settings.stdbr; /* PRQA S 0662 # Required R-1.1 */
            p_ctrl->p_reg->EXTBR = p_extend->bitrate_settings.extbr; /* PRQA S 0662 # Required R-1.1 */
        }
#endif
    }

    /*
     * Write settings to the Normal Queue Threshold Control Register.
     * - Normal Response Queue Threshold interrupt after one response is received.
     * - Normal IBI Queue Threshold interrupt after one IBI status is received.
     * - Normal IBI Data Segment Size set to 6 words.
     */
    p_ctrl->p_reg->NQTHCTL = nqthctl;  /* PRQA S 0662 # Required R-1.1 */

    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        /* Set the Normal Transfer Data Buffer threshold for transmit and receive (The only supported configuration
         * is two entries). */
        p_ctrl->p_reg->NTBTHCTL0 = 0;  /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        /*
         * Write settings to the Normal Queue Threshold Control Register.
         * - Normal Response Queue Threshold interrupt after one response is received.
         */
        p_ctrl->p_reg->HQTHCTL = 0;                   /* PRQA S 0662 # Required R-1.1 */

        /* Set the High Transfer Data Buffer threshold for transmit and receive (The only supported configuration
         * is two entries). */
        p_ctrl->p_reg->HTBTHCTL = 0;                  /* PRQA S 0662 # Required R-1.1 */
    }

    /* Set the Receive Status Queue Threshold to zero (Interrupt when there is one entry in the queue). */
    p_ctrl->p_reg->NRQTHCTL = 0;                      /* PRQA S 0662 # Required R-1.1 */

    /* Enable the Internal Error Status Flag. */
    p_ctrl->p_reg->INSTE = R_RI3C0_INSTE_INEE_Msk;    /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */

    /* Enable all Bus Status Flags. */
    p_ctrl->p_reg->BSTE = (R_RI3C0_BSTE_STCNDDE_Msk | /* PRQA S 0662 # Required R-1.1 */
                           R_RI3C0_BSTE_SPCNDDE_Msk |
#if BSP_FEATURE_I3C_HAS_HDR
                           R_RI3C0_BSTE_HDREXDE_Msk |
#endif
                           R_RI3C0_BSTE_NACKDE_Msk |
                           R_RI3C0_BSTE_TENDE_Msk |
                           R_RI3C0_BSTE_ALE_Msk |
                           R_RI3C0_BSTE_TODE_Msk);

    /* Enable all Transfer Status Flags. */
    p_ctrl->p_reg->NTSTE = (R_RI3C0_NTSTE_IBIQEFE_Msk | /* PRQA S 0662 # Required R-1.1 */
                            R_RI3C0_NTSTE_RSQFE_Msk);

    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        /* Enable all Normal Transfer Status Flags. */
        p_ctrl->p_reg->NTSTE |= (R_RI3C0_NTSTE_TDBEE0_Msk | /* PRQA S 0662 # Required R-1.1 */
                                 R_RI3C0_NTSTE_RDBFE0_Msk |
                                 R_RI3C0_NTSTE_CMDQEE_Msk |
                                 R_RI3C0_NTSTE_RSPQFE_Msk |
                                 R_RI3C0_NTSTE_TABTE_Msk |
                                 R_RI3C0_NTSTE_TEE_Msk);
    }
    else
    {
        /* Enable all High Transfer Status Flags. */
        p_ctrl->p_reg->HTSTE = (R_RI3C0_HTSTE_TDBEE_Msk | /* PRQA S 0662 # Required R-1.1 */
                                R_RI3C0_HTSTE_RDBFE_Msk |
                                R_RI3C0_HTSTE_CMDQEE_Msk |
                                R_RI3C0_HTSTE_RSPQFE_Msk |
                                R_RI3C0_HTSTE_TABTE_Msk |
                                R_RI3C0_HTSTE_TEE_Msk);
    }

    /* Enable the Internal Error Interrupt Requests. */
    p_ctrl->p_reg->INIE = R_RI3C0_INIE_INEIE_Msk; /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */

    /* Enable HDR exit pattern Interrupt Requests. */
    p_ctrl->p_reg->BIE =                          /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */
#if BSP_FEATURE_I3C_HAS_HDR
                         R_RI3C0_BIE_HDREXDIE_Msk |
#endif
                         R_RI3C0_BIE_TODIE_Msk;

    /* Write settings to the Normal Transfer Interrupt Enable Register. */
    p_ctrl->p_reg->NTIE = ntie;        /* PRQA S 0662 # Required R-1.1 */

    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_HIGH_PRIORITY)
    {
        /* Write settings to the High Transfer Interrupt Enable Register. */
        p_ctrl->p_reg->HTIE = htie;                                                                /* PRQA S 0662 # Required R-1.1 */
    }

    /* Write the Hot-Join Acknowledge settings. */
    p_ctrl->p_reg->BCTL_b.HJACKCTL = (0U == p_extend->ibi_control.hot_join_acknowledge) ? 1U : 0U; /* PRQA S 0662 # Required R-1.1 */

    /* Enables the transmission of the broadcast address on the I3C bus */
    if ((I3C_DEVICE_TYPE_SLAVE != p_ctrl->p_cfg->device_type) || (p_extend->secondary_master_enable == true))
    {
        p_ctrl->p_reg->BCTL_b.INCBA = (p_extend->broadcast_address) ? 1U : 0U; /* PRQA S 0662 # Required R-1.1 */
    }

    /* Calculate the value of the IBI Notify Control Register. */
    uint32_t ibinctl =
        (uint32_t) p_extend->ibi_control.notify_rejected_hot_join_requests << R_RI3C0_IBINCTL_NRHJCTL_Pos;
    ibinctl |= (uint32_t) p_extend->ibi_control.notify_rejected_mastership_requests << R_RI3C0_IBINCTL_NRMRCTL_Pos;
    ibinctl |= (uint32_t) p_extend->ibi_control.notify_rejected_interrupt_requests << R_RI3C0_IBINCTL_NRSIRCTL_Pos;

    /* Write settings to the IBI Notify Control Register. */
    p_ctrl->p_reg->IBINCTL = ibinctl;  /* PRQA S 0662 # Required R-1.1 */

    /* Calculate the value of the SCL Clock Stalling Control Register. */
    uint32_t scstlctl = (uint32_t) p_extend->bitrate_settings.clock_stalling.assigned_address_phase_enable <<
                        R_RI3C0_SCSTLCTL_AAPE_Pos;
#if defined(BSP_FEATURE_I3C_HAS_SCSTLCTL_TRAPE) && BSP_FEATURE_I3C_HAS_SCSTLCTL_TRAPE
    scstlctl |= (uint32_t) p_extend->bitrate_settings.clock_stalling.transition_phase_enable <<
                R_RI3C0_SCSTLCTL_TRAPE_Pos;
#endif
    scstlctl |=
        (uint32_t) p_extend->bitrate_settings.clock_stalling.parity_phase_enable << R_RI3C0_SCSTLCTL_PARPE_Pos;
    scstlctl |= (uint32_t) p_extend->bitrate_settings.clock_stalling.ack_phase_enable << R_RI3C0_SCSTLCTL_ACKPE_Pos;
    scstlctl |=
        (uint32_t) p_extend->bitrate_settings.clock_stalling.clock_stalling_time << R_RI3C0_SCSTLCTL_STLCYC_Pos; /* PRQA S 4391 # Advisory R-10.6 */

    /* Write clock stalling settings to the SCL CLock Stalling Control Register. */
    p_ctrl->p_reg->SCSTLCTL = scstlctl;                                                                          /* PRQA S 0662 # Required R-1.1 */

    /* Write Bus Condition Detection Registers. */
    p_ctrl->p_reg->BFRECDT = p_extend->bus_free_detection_time;                                                  /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->BAVLCDT = p_extend->bus_available_detection_time;                                             /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->BIDLCDT = p_extend->bus_idle_detection_time;                                                  /* PRQA S 0662 # Required R-1.1 */

    if (!p_extend->timeout_detection_enable)
    {
        /* Disable Timeout Detection. */
        p_ctrl->p_reg->TMOCTL = 0U;    /* PRQA S 0662 # Required R-1.1 */
    }

#if I3C_CFG_SLAVE_SUPPORT

    /* Calculate the value of the CCC Slave Events Command Register. */
    uint32_t csecmd =
        ((p_extend->slave_command_response_info.inband_interrupt_enable) ? 1U : 0U) << R_RI3C0_CSECMD_SVIRQE_Pos; /* PRQA S 4491 # Required R-10.6 */
    csecmd |=                                                                                                     /* PRQA S 1891 # Required R-10.7 */
              ((p_extend->slave_command_response_info.hotjoin_request_enable) ? 1U : 0U) << R_RI3C0_CSECMD_HJEVE_Pos;

    /* If secondary master mode enable, set MSRQE bit */
    if (p_extend->secondary_master_enable == true)
    {
        csecmd |= (uint32_t) (1U << R_RI3C0_CSECMD_MSRQE_Pos);
    }

    /* Calculate the value of the CCC Max Data Speed Read Register. */
    uint32_t cmdspr = (uint32_t) p_extend->slave_command_response_info.read_data_rate << R_RI3C0_CMDSPR_MSRDR_Pos;
    cmdspr |= (uint32_t) p_extend->slave_command_response_info.clock_data_turnaround << R_RI3C0_CMDSPR_CDTTIM_Pos;

    /* Calculate the value of the CCC Max Data Speed Turnaround Register. */
    uint32_t cmdspt = p_extend->slave_command_response_info.read_turnaround_time << R_RI3C0_CMDSPT_MRTTIM_Pos;
    cmdspt |=                                                                                    /* PRQA S 1891 # Required R-10.7 */
              ((p_extend->slave_command_response_info.read_turnaround_time_enable) ? 1U : 0U) << /* PRQA S 0499 # Required R-12.2 */
              R_RI3C0_CMDSPT_MRTE_Pos;

    /* Calculate the value of the CCC Exchange Timing Support Information Register. */
    uint32_t cetsm = ((uint32_t) p_extend->slave_command_response_info.oscillator_frequency << R_RI3C0_CETSM_FREQ_Pos) &
                     R_RI3C0_CETSM_FREQ_Msk;
    cetsm |= ((uint32_t) p_extend->slave_command_response_info.oscillator_inaccuracy << R_RI3C0_CETSM_INAC_Pos) &
             R_RI3C0_CETSM_INAC_Msk;

    uint32_t cmrlg = (uint32_t) p_extend->slave_command_response_info.read_length;
    cmrlg |= (uint32_t) p_extend->slave_command_response_info.ibi_payload_length << R_RI3C0_CMRLG_IBIPSZ_Pos;

 #if BSP_FEATURE_I3C_HAS_HDR
    uint32_t cghdrcap = (uint32_t) p_extend->slave_command_response_info.hdr_ddr_support;
    cghdrcap |= (uint32_t) p_extend->slave_command_response_info.hdr_tsp_support << R_I3C0_CGHDRCAP_TSPEN_Pos;
    cghdrcap |= (uint32_t) p_extend->slave_command_response_info.hdr_tsl_support << R_I3C0_CGHDRCAP_TSLEN_Pos;
 #endif

    /* Write Slave Command Response Info. */
    p_ctrl->p_reg->CSECMD  = csecmd;                                                                  /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->CEACTST = (1U << (uint32_t) p_extend->slave_command_response_info.activity_state); /* PRQA S 4491 # Required R-10.6 */ /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->CMWLG   = (uint32_t) p_extend->slave_command_response_info.write_length;           /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->CMRLG   = cmrlg;                                                                   /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->CGDVST  =                                                                          /* PRQA S 0662 # Required R-1.1 */
                             ((uint32_t) p_extend->slave_command_response_info.activity_state <<
                             R_RI3C0_CGDVST_ACTMD_Pos);
    p_ctrl->p_reg->CMDSPW = (uint32_t) p_extend->slave_command_response_info.write_data_rate;         /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->CMDSPR = cmdspr;                                                                   /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->CMDSPT = cmdspt;                                                                   /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->CETSM  = cetsm;                                                                    /* PRQA S 0662 # Required R-1.1 */
 #if BSP_FEATURE_I3C_HAS_HDR
    p_ctrl->p_reg->CGHDRCAP = cghdrcap;
 #endif
#endif
    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        /* Enable the command response status buffer full IRQ. */
        r_i3c_bsp_irq(p_ctrl, p_extend->resp_irq, I3C_BSP_IRQ_CFG_ENABLE);

        /* Enable the receive data buffer full IRQ. */
        r_i3c_bsp_irq(p_ctrl, p_extend->rx_irq, I3C_BSP_IRQ_CFG_ENABLE);

        /* Enable the transmit data buffer empty IRQ. */
        r_i3c_bsp_irq(p_ctrl, p_extend->tx_irq, I3C_BSP_IRQ_CFG_ENABLE);
#if I3C_CFG_TRANSFER_ENABLE
        if (NULL != p_ctrl->p_cfg->p_transfer_tx)
        {
            /* Disable the transmit data buffer empty IRQ. */
            r_i3c_bsp_irq(p_ctrl, p_extend->tx_irq, I3C_BSP_IRQ_DISABLE);
        }
#endif
    }
    else
    {
        /* Enable the command response status buffer full IRQ. */
        r_i3c_bsp_irq(p_ctrl, p_extend->hresp_irq, I3C_BSP_IRQ_CFG_ENABLE);

        /* Enable the receive data buffer full IRQ. */
        r_i3c_bsp_irq(p_ctrl, p_extend->hrx_irq, I3C_BSP_IRQ_CFG_ENABLE);

        /* Enable the transmit data buffer empty IRQ. */
        r_i3c_bsp_irq(p_ctrl, p_extend->htx_irq, I3C_BSP_IRQ_CFG_ENABLE);
#if I3C_CFG_TRANSFER_ENABLE
        if (NULL != p_ctrl->p_cfg->p_transfer_tx)
        {
            /* Disable the transmit data buffer empty IRQ. */
            r_i3c_bsp_irq(p_ctrl, p_extend->htx_irq, I3C_BSP_IRQ_DISABLE);
        }
#endif
    }

#if I3C_CFG_SLAVE_SUPPORT

    /* Enable the receive status buffer full IRQ. */
    r_i3c_bsp_irq(p_ctrl, p_extend->rcv_irq, I3C_BSP_IRQ_CFG_ENABLE);
#endif

    /* Enable the IBI IRQ. */
    r_i3c_bsp_irq(p_ctrl, p_extend->ibi_irq, I3C_BSP_IRQ_CFG_ENABLE);

    /* Enable the Error and Event IRQ. */
    r_i3c_bsp_irq(p_ctrl, p_extend->eei_irq, I3C_BSP_IRQ_CFG_ERR_ENABLE);

    /* Enable the I3C Bus. */
    p_ctrl->p_reg->BCTL |= R_RI3C0_BCTL_BUSE_Msk; /* PRQA S 0662 # Required R-1.1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the configuration for this device.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_UNSUPPORTED            The device cannot be a secondary master if master support is disabled.
 **********************************************************************************************************************/
fsp_err_t R_I3C_DeviceCfgSet (i3c_ctrl_t * const p_api_ctrl, i3c_device_cfg_t const * const p_device_cfg) /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                                    /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

#if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                       /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                         /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_device_cfg);                                                                     /* PRQA S 3432 # Advisory R-20.7 */
#endif

    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend;           /* PRQA S 0316 # Advisory R-11.5 */

#if I3C_CFG_MASTER_SUPPORT
    if (I3C_DEVICE_TYPE_MAIN_MASTER == p_ctrl->p_cfg->device_type)
    {
        /*
         * Configure the master dynamic address and set it to valid
         */
        uint32_t msdvad = (uint32_t) p_device_cfg->dynamic_address << R_RI3C0_MSDVAD_MDYAD_Pos;
        msdvad               |= R_RI3C0_MSDVAD_MDYADV_Msk;
        p_ctrl->p_reg->MSDVAD = msdvad; /* PRQA S 0662 # Required R-1.1 */
    }
    else
#endif
    {
#if I3C_CFG_SLAVE_SUPPORT

        /* To perform mastership request, both TBCR1 and TBCR76 must be enabled. */
        if (p_extend->secondary_master_enable == true)
        {
            FSP_ERROR_RETURN((p_device_cfg->slave_info.bcr_cfg.bcr_b.device_role == 1U), FSP_ERR_UNSUPPORTED);         /* PRQA S 3432 # Advisory R-20.7 */
            FSP_ERROR_RETURN((p_device_cfg->slave_info.bcr_cfg.bcr_b.ibi_request_capable == 1U), FSP_ERR_UNSUPPORTED); /* PRQA S 3432 # Advisory R-20.7 */
        }
        else
        {
            /* The master device role is only supported in Secondary Master mode.*/
            FSP_ERROR_RETURN((p_device_cfg->slave_info.bcr_cfg.bcr_b.device_role != 1U), FSP_ERR_UNSUPPORTED); /* PRQA S 3432 # Advisory R-20.7 */
        }

        /*
         * Configure slave device address table, Device Characteristics, Bus Characteristics, and
         * Provisional ID.
         *
         * Configure the device static address. */
        uint32_t sdatbas0 = (uint32_t) p_device_cfg->static_address & R_RI3C0_SDATBAS0_SDSTAD_Msk;

        /* Configure the device dynamic address. */
        sdatbas0 |= ((uint32_t) i3c_address_parity(p_device_cfg->dynamic_address) << R_RI3C0_SDATBAS0_SDDYAD_Pos) &
                    R_RI3C0_SDATBAS0_SDDYAD_Msk;

        /* Configure the IBI payload setting based on the BCR register. */
        sdatbas0 |= ((uint32_t) p_device_cfg->slave_info.bcr_cfg.bcr_b.ibi_payload << R_RI3C0_SDATBAS0_SDIBIPL_Pos) &
                    R_RI3C0_SDATBAS0_SDIBIPL_Msk;

        /* Set the slave address to valid. */
        p_ctrl->p_reg->SVCTL_b.SVAE0 = 1;   /* PRQA S 0662 # Required R-1.1 */

        /* Write settings to the Slave Device Address Table Register. */
        p_ctrl->p_reg->SDATBAS0 = sdatbas0; /* PRQA S 0662 # Required R-1.1 */

        /* Write the BCR and DCR settings to the Slave Device Characteristic Table Register. */
        uint32_t svdct = (uint32_t) p_device_cfg->slave_info.dcr;
        svdct |= (uint32_t) p_device_cfg->slave_info.bcr_cfg.bcr << R_RI3C0_SVDCT_TBCR_Pos;

        p_ctrl->p_reg->SVDCT = svdct;                                            /* PRQA S 0662 # Required R-1.1 */

        /* Write the PID setting to the Slave Device Characteristic Table PID Register. */
        uint8_t * pid = (uint8_t *) p_device_cfg->slave_info.pid;                /* PRQA S 0311, 3678 # Advisory R-11.8, Advisory R-8.13 */
        p_ctrl->p_reg->SDCTPIDL = ((uint32_t) pid[4] << 8U) | (uint32_t) pid[5]; /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->SDCTPIDH = ((uint32_t) pid[0] << 24U) |                   /* PRQA S 0662 # Required R-1.1 */
                                  ((uint32_t) pid[1] << 16U) |
                                  ((uint32_t) pid[2] << 8U) |
                                  ((uint32_t) pid[3] << 0U);
#endif
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configure an entry in the master device table. This function is called in master mode in order to configure
 * the devices on the I3C bus. It may also be called in slave mode when the slave receives the DEFSVLS command.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_UNSUPPORTED            Mastership requests must be rejected is slave support is disabled.
 **********************************************************************************************************************/
fsp_err_t R_I3C_MasterDeviceTableSet (i3c_ctrl_t * const                   p_api_ctrl, /* PRQA S 1505 # Published API */
                                      uint32_t                             device_index,
                                      i3c_device_table_cfg_t const * const p_device_table_cfg)
{
#if I3C_CFG_MASTER_SUPPORT
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
 #if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                    /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);      /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_device_table_cfg);                            /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_ctrl->p_cfg);                                 /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_ctrl->p_cfg->p_extend);                       /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(BSP_FEATURE_I3C_MAX_DEV_COUNT > device_index ||         /* PRQA S 3397, 3432 # Advisory R-10.6, Advisory R-20.7 */
               I3C_DEVICE_INDEX_EXTENDED_DEVICE == device_index);
 #endif
 #if BSP_FEATURE_I3C_HAS_BCTL_BMDS
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend;
 #endif
    uint32_t volatile * p_datbasn_reg = NULL;
    uint32_t            datbasn       = 0;

    /* The internal reset should be released when switching the device_index. */
    i3c_internal_reset(p_ctrl);

    if (I3C_DEVICE_INDEX_EXTENDED_DEVICE == device_index)
    {
        p_datbasn_reg = &p_ctrl->p_reg->EXDATBAS; /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        /* Compute the address of the DATBASn register. */
        uint32_t address_offset = (uint32_t) (&p_ctrl->p_reg->DATBAS1 - &p_ctrl->p_reg->DATBAS0);            /* PRQA S 0488 # Advisory R-18.4 */ /* PRQA S 0662 # Required R-1.1 */
        p_datbasn_reg = (uint32_t *) (&p_ctrl->p_reg->DATBAS0 + (device_index * address_offset));            /* PRQA S 0312, 0488 # Advisory R-11.4, Advisory R-18.4 */ /* PRQA S 0662 # Required R-1.1 */

        /* Configure the IBI settings for this device. */
        datbasn |= (((p_device_table_cfg->ibi_payload) ? 1U : 0U) << R_RI3C0_DATBAS0_DVIBIPL_Pos) &          /* PRQA S 0499, 1891 # Required R-12.2, Required R-10.7 */
                   R_RI3C0_DATBAS0_DVIBIPL_Msk;
        datbasn |= (((p_device_table_cfg->ibi_accept) ? 0U : 1U) << R_RI3C0_DATBAS0_DVSIRRJ_Pos) &           /* PRQA S 0499, 1891 # Required R-12.2, Required R-10.7 */
                   R_RI3C0_DATBAS0_DVSIRRJ_Msk;
        datbasn |= (((p_device_table_cfg->master_request_accept) ? 0U : 1U) << R_RI3C0_DATBAS0_DVMRRJ_Pos) & /* PRQA S 0499, 1891 # Required R-12.2, Required R-10.7 */
                   R_RI3C0_DATBAS0_DVMRRJ_Msk;
    }

    /* Configure the device static address. */
    datbasn |= ((uint32_t) p_device_table_cfg->static_address << R_RI3C0_DATBAS0_DVSTAD_Pos) &
               R_RI3C0_DATBAS0_DVSTAD_Msk;

    /* Configure the device dynamic address. */
    datbasn |= ((uint32_t) i3c_address_parity(p_device_table_cfg->dynamic_address) << R_RI3C0_DATBAS0_DVDYAD_Pos) &
               R_RI3C0_DATBAS0_DVDYAD_Msk;

    if (I3C_DEVICE_PROTOCOL_I2C == p_device_table_cfg->device_protocol)
    {
        /* Set the device type so that transfers with this device use the legacy I2C protocol. */
        datbasn |= R_RI3C0_DATBAS0_DVTYP_Msk;
 #if BSP_FEATURE_I3C_HAS_BCTL_BMDS

        /* Setting Bus mode selection for Legacy inclusive. */
        p_ctrl->p_reg->BCTL_b.BMDS = p_extend->legacy_inclusive_bus_mode;
 #endif
    }

    /* Write to the DATBASn register. */
    *p_datbasn_reg = datbasn;

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(device_index);
    FSP_PARAMETER_NOT_USED(p_device_table_cfg);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Set the status returned to the master in response to a GETSTATUS command.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           The instance is not in slave mode.
 * @retval FSP_ERR_UNSUPPORTED            Slave support is disabled.
 **********************************************************************************************************************/
fsp_err_t R_I3C_SlaveStatusSet (i3c_ctrl_t * const p_api_ctrl, i3c_device_status_t status) /* PRQA S 3673, 1505 # Advisory R-8.13, Advisory R-8.7 */
{
#if I3C_CFG_SLAVE_SUPPORT
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                     /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

 #if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                        /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                          /* PRQA S 3432 # Advisory R-20.7 */

    /* This function is only called in slave mode. */
    FSP_ERROR_RETURN(                                                                      /* PRQA S 3397 # Advisory R-10.6 */
        (uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE == p_ctrl->internal_state || (uint32_t) I3C_INTERNAL_STATE_SLAVE_IBI == p_ctrl->internal_state,
        FSP_ERR_INVALID_MODE);                                                             /* PRQA S 3432 # Advisory R-20.7 */
 #endif

    /* Clear the Pending Interrupt and Vendor Reserved fields. */
    uint32_t cgdvst = p_ctrl->p_reg->CGDVST;                                               /* PRQA S 0662 # Required R-1.1 */
    cgdvst &= ~(R_RI3C0_CGDVST_PNDINT_Msk | R_RI3C0_CGDVST_VDRSV_Msk);

    /* Write the new Pending Interrupt and Vendor Reserved fields. */
    cgdvst |= (uint32_t) status.pending_interrupt & R_RI3C0_CGDVST_PNDINT_Msk;
    cgdvst |= ((uint32_t) status.vendor_status << R_RI3C0_CGDVST_VDRSV_Pos) & R_RI3C0_CGDVST_VDRSV_Msk;

    p_ctrl->p_reg->CGDVST = cgdvst;    /* PRQA S 0662 # Required R-1.1 */

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(status);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * In master mode, select the device for the next transfer.
 * This function is not used in slave mode.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           This operation is prohibited in slave mode.
 * @retval FSP_ERR_UNSUPPORTED            Master support is disabled.
 **********************************************************************************************************************/
fsp_err_t R_I3C_DeviceSelect (i3c_ctrl_t * const p_api_ctrl, uint32_t device_index, uint32_t bitrate_mode) /* PRQA S 1505 # Advisory R-8.7 */
{
#if I3C_CFG_MASTER_SUPPORT
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                                     /* PRQA S 0316 # Advisory R-11.5 */

 #if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                        /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                          /* PRQA S 3432 # Advisory R-20.7 */
  #if BSP_FEATURE_I3C_HAS_HDR
    FSP_ASSERT(I3C_BITRATE_MODE_I3C_HDR_DDR_STDBR >= bitrate_mode);
  #else
    FSP_ASSERT((uint32_t) I3C_BITRATE_MODE_I3C_SDR4_EXTBR_X4 >= bitrate_mode);                             /* PRQA S 3432 # Advisory R-20.7 */
  #endif
    FSP_ASSERT(BSP_FEATURE_I3C_MAX_DEV_COUNT > device_index ||                                             /* PRQA S 3397, 3432 # Advisory R-10.6, Advisory R-20.7 */
               I3C_DEVICE_INDEX_EXTENDED_DEVICE == device_index);
 #endif
    FSP_ERROR_RETURN(                                                                                      /* PRQA S 3397 # Advisory R-10.6 */
        (uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE != p_ctrl->internal_state && (uint32_t) I3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state,
        FSP_ERR_INVALID_MODE);                                                                             /* PRQA S 3432 # Advisory R-20.7 */

    p_ctrl->device_index        = device_index;
    p_ctrl->device_bitrate_mode = (i3c_bitrate_mode_t) bitrate_mode;                                       /* PRQA S 4342 # Advisory R-10.5 */

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(device_index);
    FSP_PARAMETER_NOT_USED(bitrate_mode);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Start the Dynamic Address Assignment Process. Implements @ref i3c_api_t::dynamicAddressAssignmentStart.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL or invalid.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           This operation is prohibited in slave mode.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_UNSUPPORTED            Master support is disabled.
 **********************************************************************************************************************/
fsp_err_t R_I3C_DynamicAddressAssignmentStart (i3c_ctrl_t * const            p_api_ctrl, /* PRQA S 1505 # Published API */
                                               i3c_address_assignment_mode_t address_assignment_mode,
                                               uint32_t                      starting_device_index,
                                               uint32_t                      device_count)
{
#if I3C_CFG_MASTER_SUPPORT
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;  /* PRQA S 0316 # Advisory R-11.5 */

 #if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                     /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);       /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(                                                   /* PRQA S 3397 # Advisory R-10.6 */
        (uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE != p_ctrl->internal_state && (uint32_t) I3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state,
        FSP_ERR_INVALID_MODE);                                          /* PRQA S 3432 # Advisory R-20.7 */

    FSP_ASSERT(BSP_FEATURE_I3C_MAX_DEV_COUNT > starting_device_index || /* PRQA S 3397, 3432 # Advisory R-10.6, Advisory R-20.7 */
               I3C_DEVICE_INDEX_EXTENDED_DEVICE == starting_device_index);

    if (I3C_ADDRESS_ASSIGNMENT_MODE_ENTDAA == address_assignment_mode)
    {
        if (I3C_DEVICE_INDEX_EXTENDED_DEVICE == starting_device_index)
        {
            /* When the extended device index is selected, the device count must be 1. */
            FSP_ASSERT(1U == device_count); /* PRQA S 3432 # Advisory R-20.7 */
        }
        else
        {
            /* The device count must be less than the remaining devices in the device table. */
            FSP_ASSERT(BSP_FEATURE_I3C_MAX_DEV_COUNT > device_count + starting_device_index); /* PRQA S 3397, 3432 # Advisory R-10.6, Advisory R-20.7 */
        }
    }
    else
    {
        /* If the dynamic address is configured with SETDASA then the device count is not used and should be set to 0. */
        FSP_ASSERT(0U == device_count); /* PRQA S 3432 # Advisory R-20.7 */
    }
 #endif

    /* Ensure that driver is in the idle state. */
    FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE == p_ctrl->internal_state, FSP_ERR_IN_USE); /* PRQA S 3432 # Advisory R-20.7 */

    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend;            /* PRQA S 0316 # Advisory R-11.5 */
    p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_MASTER_ENTDAA;

    /* Set the transfer_data pointer to read the PID, DCR, and BCR registers for each device. */
    p_ctrl->read_buffer_descriptor.p_buffer    = (uint8_t *) &p_ctrl->current_slave_info;
    p_ctrl->read_buffer_descriptor.count       = 0;
    p_ctrl->read_buffer_descriptor.buffer_size = sizeof(i3c_slave_info_t);

    /* Setup the command descriptor to start the ENTDAA command starting at the selected device index. */
    uint32_t command_descriptor = 0;
    command_descriptor |= (I3C_CMD_DESC_CMD_ATTR_ADDR_ASSGN_CMD << I3C_CMD_DESC_CMD_ATTR_Pos);
    command_descriptor |= ((uint32_t) I3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE << I3C_CMD_DESC_TID_Pos);
    command_descriptor |= ((uint32_t) address_assignment_mode << I3C_CMD_DESC_CMD_Pos);
    command_descriptor |= (starting_device_index << I3C_CMD_DESC_DEV_INDEX_Pos);
    command_descriptor |= (device_count << I3C_CMD_DESC_ADDR_ASSGN_DEV_COUNT_Pos);
    command_descriptor |= I3C_CMD_DESC_ROC_Msk;
    command_descriptor |= I3C_CMD_DESC_TOC_Msk;

    /*
     * Write to the descriptor to the command queue.
     * Note that the command descriptor is two words. The least significant word must be written first followed by
     * the most significant word.
     */
    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        p_ctrl->p_reg->NCMDQP = command_descriptor; /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->NCMDQP = 0U;                 /* PRQA S 0662 # Required R-1.1 */

        /* Clear the command queue empty flag. */
        p_ctrl->p_reg->NTST_b.CMDQEF = 0;           /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        p_ctrl->p_reg->HCMDQP = command_descriptor; /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->HCMDQP = 0U;                 /* PRQA S 0662 # Required R-1.1 */

        /* Clear the command queue empty flag. */
        p_ctrl->p_reg->HTST_b.CMDQEF = 0;           /* PRQA S 0662 # Required R-1.1 */
    }

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(address_assignment_mode);
    FSP_PARAMETER_NOT_USED(starting_device_index);
    FSP_PARAMETER_NOT_USED(device_count);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Send a broadcast or direct command to slave devices on the bus. Implements @ref i3c_api_t::commandSend.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_INVALID_MODE           This driver is not in master mode.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes. If it is a read operation, the length
 *                                        also be a multiple of 4 bytes.
 * @retval FSP_ERR_UNSUPPORTED            Master support must be enabled to call this function. Slave support must be
 *                                        enabled when sending the GETACCMST command.
 **********************************************************************************************************************/
fsp_err_t R_I3C_CommandSend (i3c_ctrl_t * const p_api_ctrl, i3c_command_descriptor_t const * const p_command_descriptor) /* PRQA S 1505 # Advisory R-8.7 */
{
#if I3C_CFG_MASTER_SUPPORT
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                                                   /* PRQA S 0316 # Advisory R-11.5 */

 #if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                                      /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                                        /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_command_descriptor);                                                                            /* PRQA S 3432 # Advisory R-20.7 */
    if (p_command_descriptor->length > 0U)
    {
        FSP_ASSERT(NULL != p_command_descriptor->p_buffer);                                                              /* PRQA S 3432 # Advisory R-20.7 */
    }

    FSP_ERROR_RETURN(((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE != p_ctrl->internal_state) &&
                     ((uint32_t) I3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state),
                     FSP_ERR_INVALID_MODE); /* PRQA S 3432 # Advisory R-20.7 */

  #if !I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* Verify that the buffer is aligned to 4 bytes. */
    FSP_ERROR_RETURN(0U == ((uint32_t) p_command_descriptor->p_buffer & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
  #endif

  #if BSP_FEATURE_I3C_HAS_HDR
    if (I3C_BITRATE_MODE_I3C_HDR_DDR_STDBR == p_ctrl->device_bitrate_mode)
    {
        /* Verify that length is a multiple of 2 in HDR modes. */
        FSP_ERROR_RETURN(p_command_descriptor->length % 2 == 0, FSP_ERR_INVALID_ALIGNMENT);
    }
  #endif
 #endif

    /* Ensure that driver is in the idle state. */
    FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE == p_ctrl->internal_state, FSP_ERR_IN_USE); /* PRQA S 3432 # Advisory R-20.7 */

    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend;            /* PRQA S 0316 # Advisory R-11.5 */

    /* Save the current command code. */
    p_ctrl->current_command_code = p_command_descriptor->command_code;

    i3c_read_buffer_descriptor_t * p_transfer_descriptor;

 #if I3C_CFG_SLAVE_SUPPORT
    if ((uint8_t) I3C_CCC_DIRECT_GETACCMST == p_command_descriptor->command_code)
    {
        /* Disable the IBI Write Buffer Empty/Full IRQ in case the driver transitions to slave mode. */
        p_ctrl->p_reg->NTIE_b.IBIQEFIE = 0; /* PRQA S 0662 # Required R-1.1 */
    }
 #endif

    if (p_command_descriptor->rnw)
    {
        /* If this is a read transfer, update the read buffer descriptor. */
        p_transfer_descriptor = &p_ctrl->read_buffer_descriptor;
        p_transfer_descriptor->read_request_issued = false;

        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_MASTER_COMMAND_READ;
    }
    else
    {
        /* If this is a write transfer, update the write buffer descriptor. */
        p_transfer_descriptor = (i3c_read_buffer_descriptor_t *) &p_ctrl->write_buffer_descriptor; /* PRQA S 0310 # Advisory R-11.4 */

        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_MASTER_COMMAND_WRITE;
    }

    /* Update the buffer descriptor with the buffer provided in the command descriptor. */
    p_transfer_descriptor->p_buffer    = p_command_descriptor->p_buffer;
    p_transfer_descriptor->count       = 0;
    p_transfer_descriptor->buffer_size = p_command_descriptor->length;

    /* Calculate the command descriptor. */
    uint32_t cmd1 = 0;
    cmd1 |= ((uint32_t) p_ctrl->device_index << I3C_CMD_DESC_DEV_INDEX_Pos) & I3C_CMD_DESC_DEV_INDEX_Msk;
    cmd1 |= ((uint32_t) p_ctrl->device_bitrate_mode << I3C_CMD_DESC_XFER_MODE_Pos) & I3C_CMD_DESC_XFER_MODE_Msk;
    cmd1 |= ((p_command_descriptor->rnw) ? 1U : 0U) << I3C_CMD_DESC_XFER_RNW_Pos;                         /* PRQA S 0499, 1891 # Required R-12.2, Required R-10.7 */
    cmd1 |= I3C_CMD_DESC_ROC_Msk;
    cmd1 |= (((p_command_descriptor->restart) ? 0U : 1U) << I3C_CMD_DESC_TOC_Pos) & I3C_CMD_DESC_TOC_Msk; /* PRQA S 0499, 1891 # Required R-12.2, Required R-10.7 */
    cmd1 |= (uint32_t) p_command_descriptor->command_code << I3C_CMD_DESC_CMD_Pos;
    cmd1 |= I3C_CMD_DESC_XFER_CP_Msk;
    cmd1 |= ((uint32_t) I3C_EVENT_COMMAND_COMPLETE << I3C_CMD_DESC_TID_Pos);

    uint32_t cmd2;
    if ((4U >= p_command_descriptor->length) && !p_command_descriptor->rnw)
    {
        /* If the transfer length is less than or equal to 4 bytes, then use "Immediate Data Transfer".
         */
        cmd1 |= I3C_CMD_DESC_CMD_ATTR_IMMED_DATA_XFER;
        cmd1 |= (p_command_descriptor->length << I3C_CMD_DESC_IMMED_DATA_XFER_BYTE_CNT_Pos);
        cmd2  = i3c_next_data_word_calculate(&p_ctrl->write_buffer_descriptor);
        p_ctrl->write_buffer_descriptor.count = p_command_descriptor->length;
    }
    else
    {
        cmd2 = (p_command_descriptor->length << I3C_CMD_DESC_XFER_LENGTH_Pos) &
               I3C_CMD_DESC_XFER_LENGTH_Msk;
    }

    /*
     * Write the descriptor to the command queue.
     * Note that the command descriptor is two words. The least significant word must be written first followed by
     * the most significant word.
     */
    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        p_ctrl->p_reg->NCMDQP = cmd1;     /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->NCMDQP = cmd2;     /* PRQA S 0662 # Required R-1.1 */

        /* Clear the command queue empty flag. */
        p_ctrl->p_reg->NTST_b.CMDQEF = 0; /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        p_ctrl->p_reg->HCMDQP = cmd1;     /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->HCMDQP = cmd2;     /* PRQA S 0662 # Required R-1.1 */

        /* Clear the command queue empty flag. */
        p_ctrl->p_reg->HTST_b.CMDQEF = 0; /* PRQA S 0662 # Required R-1.1 */
    }

    if (!p_command_descriptor->rnw && (4U < p_command_descriptor->length))
    {
        /* Calculate the next data word that will be written to the FIFO. */
        p_ctrl->next_word = i3c_next_data_word_calculate(&p_ctrl->write_buffer_descriptor);

        /* Write data to the FIFO. */
        i3c_fifo_write(p_ctrl);

        /* If there is still data remaining in the transfer then it will be written in the Write Buffer Empty IRQ. */
        if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
        {
            /* Enable the Write Buffer Empty IRQ. */
            p_ctrl->p_reg->NTIE_b.TDBEIE0 = 1; /* PRQA S 0662 # Required R-1.1 */
        }
        else
        {
            /* Enable the Write Buffer Empty IRQ. */
            p_ctrl->p_reg->HTIE_b.TDBEIE = 1; /* PRQA S 0662 # Required R-1.1 */
        }
    }

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(p_command_descriptor);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Set the write buffer for the transfer. In master mode, start the transfer. When the transfer is completed send a
 * stop condition or a repeated-start. Implements @ref i3c_api_t::write.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_INVALID_MODE           This driver is disabled, or an invalid bitrate mode is selected.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes.
 **********************************************************************************************************************/
fsp_err_t R_I3C_Write (i3c_ctrl_t * const p_api_ctrl, uint8_t const * const p_data, uint32_t length, bool restart) /* PRQA S 1505 # Advisory R-8.7 */
{
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                                             /* PRQA S 0316 # Advisory R-11.5 */

#if 0U == I3C_CFG_MASTER_SUPPORT
    FSP_PARAMETER_NOT_USED(restart);
#endif

#if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                           /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                             /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_data);                                                                               /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state, FSP_ERR_INVALID_MODE); /* PRQA S 3432 # Advisory R-20.7 */
 #if !I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* Verify that the buffer is aligned to 4 bytes. */
    FSP_ERROR_RETURN(0U == ((uint32_t) p_data & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
 #endif
 #if BSP_FEATURE_I3C_HAS_HDR
    if ((uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE == p_ctrl->internal_state)
    {
        /* In master mode, this function can only be called to start SDR and I2C transfers. */
        FSP_ERROR_RETURN(p_ctrl->device_bitrate_mode < I3C_BITRATE_MODE_I3C_HDR_DDR_STDBR, FSP_ERR_INVALID_MODE);
    }
 #endif
#endif
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

#if I3C_CFG_SLAVE_SUPPORT
    if ((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE == p_ctrl->internal_state)
    {
        /* In slave mode, ensure that a write transfer is not currently in progress. */
        FSP_ERROR_RETURN(0U == (p_ctrl->p_reg->PRSST & R_RI3C0_PRSST_TRMD_Msk), FSP_ERR_IN_USE); /* PRQA S 3432 # Advisory R-20.7 */ /* PRQA S 0662 # Required R-1.1 */
    }
    else
#endif
    {
#if I3C_CFG_MASTER_SUPPORT

        /* In master mode, ensure that driver is in the idle state. */
        FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE == p_ctrl->internal_state, FSP_ERR_IN_USE); /* PRQA S 3432 # Advisory R-20.7 */
#endif
    }

    /* Set the internal transfer state for a write transfer. */
    p_ctrl->write_buffer_descriptor.count       = 0;
    p_ctrl->write_buffer_descriptor.buffer_size = length;
    p_ctrl->write_buffer_descriptor.p_buffer    = (uint8_t *) p_data; /* PRQA S 0311 # Advisory R-11.8 */

    /* Calculate the next data word that will be written to the FIFO. */
    p_ctrl->next_word = i3c_next_data_word_calculate(&p_ctrl->write_buffer_descriptor);

#if I3C_CFG_SLAVE_SUPPORT
    if ((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE == p_ctrl->internal_state)
    {
        if (NULL != p_ctrl->write_buffer_descriptor.p_buffer)
        {
            /*
             * If a write buffer has already been configured, then the FIFO needs to be reset before the new data
             * can be written.
             */
            p_ctrl->p_reg->RSTCTL = R_RI3C0_RSTCTL_TDBRST_Msk;     /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */

            /* The field will be cleared automatically upon reset completion. */
            FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->RSTCTL, 0U); /* PRQA S 0662 # Required R-1.1 */
        }

        /* Configure the number of bytes that the slave will write before ending the transfer via the T bit. */
        p_ctrl->p_reg->SVTDLG0 = (length << 16U);                  /* PRQA S 0662 # Required R-1.1 */
    }
#endif

    if ((length > 4U) || ((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE == p_ctrl->internal_state))
    {
        /* Write data to the FIFO. Note that in master mode, if the length is less than or equal to 4 bytes,
         * the "Immediate Data Transfer" command must be used instead of the "Regular Transfer" command. */
#if I3C_CFG_TRANSFER_ENABLE
        if (NULL != p_ctrl->p_cfg->p_transfer_tx)
        {
            p_ctrl->activation_on_txi = true;
        }
#endif
        i3c_fifo_write(p_ctrl);

        /* If there is still data remaining in the transfer then it will be written in the Write Buffer Empty IRQ. */
        if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
        {
            /* Enable the Write Buffer Empty IRQ. */
            p_ctrl->p_reg->NTIE_b.TDBEIE0 = 1; /* PRQA S 0662 # Required R-1.1 */
        }
        else
        {
            /* Enable the Write Buffer Empty IRQ. */
            p_ctrl->p_reg->HTIE_b.TDBEIE = 1; /* PRQA S 0662 # Required R-1.1 */
        }
    }

#if I3C_CFG_MASTER_SUPPORT
    if ((uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE == p_ctrl->internal_state)
    {
        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_MASTER_WRITE;

        /*
         * Write the descriptor to the command queue.
         * Note that the command descriptor is two words. The least significant word must be written first followed by
         * the most significant word.
         */
        uint32_t cmd1 =
            i3c_xfer_command_calculate(p_ctrl->device_index, false, (uint32_t) p_ctrl->device_bitrate_mode, restart);

        uint32_t cmd2;
        if (length <= 4U)
        {
            /* If the transfer length is less than or equal to 4 bytes, then use "Immediate Data Transfer".
             */
            cmd1 |= I3C_CMD_DESC_CMD_ATTR_IMMED_DATA_XFER;
            cmd1 |= (length << I3C_CMD_DESC_IMMED_DATA_XFER_BYTE_CNT_Pos);
            cmd2  = p_ctrl->next_word;
            p_ctrl->write_buffer_descriptor.count = length;
        }
        else
        {
            cmd2 = ((uint32_t) length << I3C_CMD_DESC_XFER_LENGTH_Pos) & I3C_CMD_DESC_XFER_LENGTH_Msk;
        }

        /*  Config CCC to ON OFF Broadcast*/
        if (p_extend->mipi_cmd == I3C_MIPI_CMD_INCLUDE_IBA)
        {
            i3c_internal_command_config(p_ctrl, I3C_MIPI_CMD_INCLUDE_IBA);
        }
        else
        {
            i3c_internal_command_config(p_ctrl, I3C_MIPI_CMD_IGNORE_IBA);
        }

        if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
        {
            p_ctrl->p_reg->NCMDQP = cmd1;     /* PRQA S 0662 # Required R-1.1 */
            p_ctrl->p_reg->NCMDQP = cmd2;     /* PRQA S 0662 # Required R-1.1 */

            /* Clear the command queue empty flag. */
            p_ctrl->p_reg->NTST_b.CMDQEF = 0; /* PRQA S 0662 # Required R-1.1 */
        }
        else
        {
            p_ctrl->p_reg->HCMDQP = cmd1;     /* PRQA S 0662 # Required R-1.1 */
            p_ctrl->p_reg->HCMDQP = cmd2;     /* PRQA S 0662 # Required R-1.1 */

            /* Clear the command queue empty flag. */
            p_ctrl->p_reg->HTST_b.CMDQEF = 0; /* PRQA S 0662 # Required R-1.1 */
        }
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the read buffer for the transfer. In master mode, start the transfer. When the transfer is completed send a
 * stop condition or a repeated-start. Implements @ref i3c_api_t::read.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_INVALID_MODE           This driver is disabled, or an invalid bitrate mode is selected.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes and the length must be a multiple of
 *                                        4 bytes.
 **********************************************************************************************************************/
fsp_err_t R_I3C_Read (i3c_ctrl_t * const p_api_ctrl, uint8_t * const p_data, uint32_t length, bool restart) /* PRQA S 1505 # Advisory R-8.7 */
{
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                                      /* PRQA S 0316 # Advisory R-11.5 */

#if 0U == I3C_CFG_MASTER_SUPPORT
    FSP_PARAMETER_NOT_USED(restart);
#endif

#if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                           /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                             /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_data);                                                                               /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state, FSP_ERR_INVALID_MODE); /* PRQA S 3432 # Advisory R-20.7 */
 #if !I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* The buffer must be 4 byte aligned. */
    FSP_ERROR_RETURN(0U == ((uint32_t) p_data & 0x03U), FSP_ERR_INVALID_ALIGNMENT);

    /* The buffer size must be a multiple of 4 bytes for both master and slave reads,
     * since data is written to the buffer one word (4 bytes) at a time when unaligned
     * buffer support is disabled. Failing to enforce this can cause up to 3 bytes to be
     * written past the end of the user-provided buffer. */
    FSP_ERROR_RETURN(0U == ((uint32_t) length & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
 #endif

 #if BSP_FEATURE_I3C_HAS_HDR
    if ((uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE == p_ctrl->internal_state)
    {
        /* In master mode, this function can only be called to start SDR and I2C transfers. */
        FSP_ERROR_RETURN(p_ctrl->device_bitrate_mode < I3C_BITRATE_MODE_I3C_HDR_DDR_STDBR, FSP_ERR_INVALID_MODE);
    }
 #endif
#endif

    /* In Master mode, ensure that driver is in the idle state. */
    FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE == p_ctrl->internal_state ||      /* PRQA S 3397 # Advisory R-10.6 */
                     (uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE == p_ctrl->internal_state,
                     FSP_ERR_IN_USE);                                                           /* PRQA S 3432 # Advisory R-20.7 */

    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* Disable the Receive Buffer Full IRQ in order to ensure that updating the read buffer state is not interrupted. */
    r_i3c_bsp_irq(p_ctrl, p_extend->rx_irq, I3C_BSP_IRQ_DISABLE);
    r_i3c_bsp_irq(p_ctrl, p_extend->hrx_irq, I3C_BSP_IRQ_DISABLE);

    /* Set the internal transfer state for a read transfer. */
    p_ctrl->read_buffer_descriptor.count               = 0;
    p_ctrl->read_buffer_descriptor.buffer_size         = length;
    p_ctrl->read_buffer_descriptor.p_buffer            = p_data;
    p_ctrl->read_buffer_descriptor.read_request_issued = false;

    r_i3c_bsp_irq(p_ctrl, p_extend->rx_irq, I3C_BSP_IRQ_ENABLE);
    r_i3c_bsp_irq(p_ctrl, p_extend->hrx_irq, I3C_BSP_IRQ_ENABLE);

#if I3C_CFG_MASTER_SUPPORT
    if ((uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE == p_ctrl->internal_state)
    {
        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_MASTER_READ;
        if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
        {
            /*
             * Write the descriptor to the command queue.
             * Note that the command descriptor is two words. The least significant word must be written first followed by
             * the most significant word.
             */
            p_ctrl->p_reg->NCMDQP = i3c_xfer_command_calculate(p_ctrl->device_index,                                    /* PRQA S 0662 # Required R-1.1 */
                                                               true,
                                                               (uint32_t) p_ctrl->device_bitrate_mode,
                                                               restart);
            p_ctrl->p_reg->NCMDQP = ((uint32_t) length << I3C_CMD_DESC_XFER_LENGTH_Pos) & I3C_CMD_DESC_XFER_LENGTH_Msk; /* PRQA S 0662 # Required R-1.1 */

            /* Clear the command queue empty flag. */
            p_ctrl->p_reg->NTST_b.CMDQEF = 0;                                                                           /* PRQA S 0662 # Required R-1.1 */
        }
        else
        {
            /*
             * Write the descriptor to the command queue.
             * Note that the command descriptor is two words. The least significant word must be written first followed by
             * the most significant word.
             */
            p_ctrl->p_reg->HCMDQP = i3c_xfer_command_calculate(p_ctrl->device_index,                                    /* PRQA S 0662 # Required R-1.1 */
                                                               true,
                                                               (uint32_t) p_ctrl->device_bitrate_mode,
                                                               restart);
            p_ctrl->p_reg->HCMDQP = ((uint32_t) length << I3C_CMD_DESC_XFER_LENGTH_Pos) & I3C_CMD_DESC_XFER_LENGTH_Msk; /* PRQA S 0662 # Required R-1.1 */

            /* Clear the command queue empty flag. */
            p_ctrl->p_reg->HTST_b.CMDQEF = 0;                                                                           /* PRQA S 0662 # Required R-1.1 */
        }
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Initiate an IBI write operation (This function is only used in slave mode).
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_INVALID_MODE           This function is only called in slave mode.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes.
 * @retval FSP_ERR_UNSUPPORTED            Slave support is disabled.
 **********************************************************************************************************************/
fsp_err_t R_I3C_IbiWrite (i3c_ctrl_t * const    p_api_ctrl, /* PRQA S 1505 # Published API */
                          i3c_ibi_type_t        ibi_type,
                          uint8_t const * const p_data,
                          uint32_t              length)
{
#if I3C_CFG_SLAVE_SUPPORT
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                                            /* PRQA S 0316 # Advisory R-11.5 */
 #if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                                               /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                                 /* PRQA S 3432 # Advisory R-20.7 */

    /* If the device has an IBI payload, then a buffer is required. */
    if ((I3C_IBI_TYPE_INTERRUPT == ibi_type) && (0U != (R_RI3C0_SDATBAS0_SDIBIPL_Msk & p_ctrl->p_reg->SDATBAS0))) /* PRQA S 0662 # Required R-1.1 */
    {
        FSP_ASSERT(NULL != p_data);                                                                               /* PRQA S 3432 # Advisory R-20.7 */
        FSP_ASSERT(0U != length);                                                                                 /* PRQA S 3432 # Advisory R-20.7 */
    }

    /* This function is only called in slave mode. */
    FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE == p_ctrl->internal_state, FSP_ERR_INVALID_MODE);   /* PRQA S 3432 # Advisory R-20.7 */

  #if !I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* The buffer must be 4 byte aligned. */
    FSP_ERROR_RETURN(0U == ((uint32_t) p_data & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
  #endif
 #endif

    /* An IBI can't be started if an IBI is already in progress. */
    FSP_ERROR_RETURN((uint32_t) I3C_INTERNAL_STATE_SLAVE_IBI != p_ctrl->internal_state, FSP_ERR_IN_USE); /* PRQA S 3432 # Advisory R-20.7 */

    p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_SLAVE_IBI;

    /* Setup the buffer for writing the data portion of the IBI. */
    p_ctrl->ibi_buffer_descriptor.p_buffer    = (uint8_t *) p_data; /* PRQA S 0311 # Advisory R-11.8 */
    p_ctrl->ibi_buffer_descriptor.count       = 0;
    p_ctrl->ibi_buffer_descriptor.buffer_size = length;

    /* Calculate the command descriptor for starting an IBI. */
    uint32_t command_descriptor = 0;
    command_descriptor |= I3C_CMD_DESC_ROC_Msk;
    command_descriptor |= ((uint32_t) I3C_EVENT_IBI_WRITE_COMPLETE << I3C_CMD_DESC_TID_Pos);

    if (I3C_IBI_TYPE_HOT_JOIN == ibi_type)
    {
        command_descriptor |= I3C_CMD_DESC_XFER_HJ_Msk;

        /* Refer to the 'Hot-Join flowchart' in the Hardware User Manual, after the I3C bus has already been configured. */
        /* Set Internal reset. */
        p_ctrl->p_reg->RSTCTL_b.INTLRST = 1;                             /* PRQA S 0662 # Required R-1.1 */

        /* Wait until the field is actually set to 1.*/
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->RSTCTL_b.INTLRST, 1U); /* PRQA S 0662 # Required R-1.1 */

        /* Wait until the field is actually set to 1.*/
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->BCST_b.BIDLF, 1U);     /* PRQA S 0662 # Required R-1.1 */

        /* Set Internal reset*/
        p_ctrl->p_reg->RSTCTL_b.INTLRST = 0;                             /* PRQA S 0662 # Required R-1.1 */

        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE;

        /* Hot-Join Event enable setting*/
        p_ctrl->p_reg->CSECMD_b.HJEVE = 1; /* PRQA S 0662 # Required R-1.1 */
    }
    else if (I3C_IBI_TYPE_INTERRUPT == ibi_type)
    {
        command_descriptor |= I3C_CMD_DESC_XFER_RNW_Msk;
    }
    else
    {
        /* Mastership request. */
    }

    if (0U < length)
    {
        /* Write the data to the IBI queue. */
        p_ctrl->p_reg->NIBIQP = i3c_next_data_word_calculate(                  /* PRQA S 0662 # Required R-1.1 */
            (i3c_write_buffer_descriptor_t *) &p_ctrl->ibi_buffer_descriptor); /* PRQA S 0310 # Advisory R-11.4 */

        if (sizeof(uint32_t) < length)
        {
            /* If the transfer length is greater than 4 bytes, then prepare the next word to be written to the FIFO. */
            p_ctrl->ibi_next_word = i3c_next_data_word_calculate(
                (i3c_write_buffer_descriptor_t *) &p_ctrl->ibi_buffer_descriptor);                              /* PRQA S 0310 # Advisory R-11.4 */

            /* Enable the IBI Write Buffer Empty/Full IRQ. */
            p_ctrl->p_reg->NTIE_b.IBIQEFIE = 1;                                                                 /* PRQA S 0662 # Required R-1.1 */
        }

        /* Clear the IBI Queue Empty/Full Flag (This will generate an IBI IRQ). */
        p_ctrl->p_reg->NTST_b.IBIQEFF = 0;                                                                      /* PRQA S 0662 # Required R-1.1 */
    }

    /*
     * Write the descriptor to the command queue.
     * Note that the command descriptor is two words. The least significant word must be written first followed by
     * the most significant word.
     */
    p_ctrl->p_reg->NCMDQP = command_descriptor;                                                                 /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->NCMDQP = ((uint32_t) length << I3C_CMD_DESC_XFER_LENGTH_Pos) & I3C_CMD_DESC_XFER_LENGTH_Msk; /* PRQA S 0662 # Required R-1.1 */

    /* Clear the command queue empty flag. */
    p_ctrl->p_reg->NTST_b.CMDQEF = 0;                                                                           /* PRQA S 0662 # Required R-1.1 */

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(ibi_type);
    FSP_PARAMETER_NOT_USED(p_data);
    FSP_PARAMETER_NOT_USED(length);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Set the read buffer for storing received IBI data (This function is only used in master mode).
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           This function is only called in master mode.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes and the length must be a multiple of
 *                                        4 bytes.
 * @retval FSP_ERR_UNSUPPORTED            Master support is disabled.
 **********************************************************************************************************************/
fsp_err_t R_I3C_IbiRead (i3c_ctrl_t * const p_api_ctrl, uint8_t * const p_data, uint32_t length) /* PRQA S 1505 # Advisory R-8.7 */
{
#if I3C_CFG_MASTER_SUPPORT
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl;                           /* PRQA S 0316 # Advisory R-11.5 */

 #if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                                              /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(NULL != p_data);                                                                  /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ASSERT(0U != length);                                                                    /* PRQA S 3432 # Advisory R-20.7 */

    /* This function is not used in slave mode. */
    FSP_ERROR_RETURN(((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE != p_ctrl->internal_state) &&
                     ((uint32_t) I3C_INTERNAL_STATE_SLAVE_IBI != p_ctrl->internal_state) &&
                     ((uint32_t) I3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state),
                     FSP_ERR_INVALID_MODE); /* PRQA S 3432 # Advisory R-20.7 */

  #if !I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* The buffer must be 4 byte aligned. */
    FSP_ERROR_RETURN(0U == ((uint32_t) p_data & 0x03U), FSP_ERR_INVALID_ALIGNMENT);

    /* The buffer size must be a multiple of 4 bytes. */
    FSP_ERROR_RETURN(0U == ((uint32_t) length & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
  #endif
 #endif

    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* Disable the IBI Buffer Full IRQ in order to ensure that updating the IBI buffer state is not interrupted. */
    r_i3c_bsp_irq(p_ctrl, p_extend->ibi_irq, I3C_BSP_IRQ_DISABLE);

    p_ctrl->ibi_buffer_descriptor.p_buffer            = p_data;
    p_ctrl->ibi_buffer_descriptor.count               = 0;
    p_ctrl->ibi_buffer_descriptor.buffer_size         = length;
    p_ctrl->ibi_buffer_descriptor.read_request_issued = false;

    r_i3c_bsp_irq(p_ctrl, p_extend->ibi_irq, I3C_BSP_IRQ_ENABLE);

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(p_data);
    FSP_PARAMETER_NOT_USED(length);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Close the I3C instance. Implements @ref i3c_api_t::close.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 **********************************************************************************************************************/
fsp_err_t R_I3C_Close (i3c_ctrl_t * const p_api_ctrl)                  /* PRQA S 1505 # Advisory R-8.7 */
{
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if I3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                                    /* PRQA S 3432 # Advisory R-20.7 */
    FSP_ERROR_RETURN(I3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);      /* PRQA S 3432 # Advisory R-20.7 */
#endif
#if I3C_CFG_TRANSFER_ENABLE
    fsp_err_t err = FSP_SUCCESS;
#endif
    i3c_extended_cfg_t * p_extend = (i3c_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Advisory R-11.8, Advisory R-11.5, Advisory R-8.13 */

    /* Disable IRQs. */
    r_i3c_bsp_irq(p_ctrl, p_extend->resp_irq, I3C_BSP_IRQ_DISABLE);
    r_i3c_bsp_irq(p_ctrl, p_extend->rx_irq, I3C_BSP_IRQ_DISABLE);
    r_i3c_bsp_irq(p_ctrl, p_extend->tx_irq, I3C_BSP_IRQ_DISABLE);
    r_i3c_bsp_irq(p_ctrl, p_extend->hrx_irq, I3C_BSP_IRQ_DISABLE);
    r_i3c_bsp_irq(p_ctrl, p_extend->htx_irq, I3C_BSP_IRQ_DISABLE);
    r_i3c_bsp_irq(p_ctrl, p_extend->rcv_irq, I3C_BSP_IRQ_DISABLE);
    r_i3c_bsp_irq(p_ctrl, p_extend->ibi_irq, I3C_BSP_IRQ_DISABLE);
#if I3C_CFG_TRANSFER_ENABLE

    /* Close the handles for the transfer interfaces */
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */

        err = p_ctrl->p_cfg->p_transfer_rx->p_api->close(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */
    }

    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->close(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */
    }
#endif

    /* Reset the I3C Peripheral so that it is in a known state. */
    p_ctrl->p_reg->BCTL_b.BUSE = 0;                        /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->RSTCTL      = 1U;                       /* PRQA S 0662 # Required R-1.1 */

    /* The field will be cleared automatically upon reset completion. */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->RSTCTL, 0U); /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->activation_on_txi = false;
    p_ctrl->activation_on_rxi = false;
    p_ctrl->open              = 0;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup I3C)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Compute the odd parity of an I3C address and set the parity bit.
 * @param[in]  address           7-bit address value to update with odd parity.
 *
 * @return                       The address with the parity bit set.
 **********************************************************************************************************************/
static uint8_t i3c_address_parity (uint8_t address)
{
    /* Compute the parity of the dynamic address. */
    uint32_t parity = address;

    /* XOR the upper 4 bits with the lower 4 bits. */
    parity ^= (parity >> 4);

    /* XOR the upper 2 bits of the result with the lower 2 bits. */
    parity ^= (parity >> 2);

    /* XOR the upper bit of the result with the lower bit. */
    parity ^= (parity >> 1);

    /* The result of XOR of all bits is stored in the LSB. */
    parity &= 1U;

    /* The ODD parity will be the inverse of the even parity. */
    parity = (0U == parity) ? 1U : 0U;

    return (uint8_t) (address | (parity << 7U));
}

#if I3C_CFG_MASTER_SUPPORT

/*******************************************************************************************************************//**
 * Setup the command descriptor for a read or write transfer to the selected device.
 *
 * @param[in]  dev_index         The selected index for the transfer.
 * @param[in]  rnw               Read not write.
 * @param[in]  bitrate_setting   The bitrate setting (See @ref i3c_bitrate_mode_t).
 * @param[in]  restart           Terminate the transfer with a repeated start condition.
 *
 * @return                       The calculated value for the command descriptor.
 **********************************************************************************************************************/
static uint32_t i3c_xfer_command_calculate (uint32_t dev_index, bool rnw, uint32_t bitrate_setting, bool restart)
{
    uint32_t command_descriptor = 0;
    if (rnw)
    {
        command_descriptor |= ((uint32_t) I3C_EVENT_READ_COMPLETE << I3C_CMD_DESC_TID_Pos);
    }
    else
    {
        command_descriptor |= ((uint32_t) I3C_EVENT_WRITE_COMPLETE << I3C_CMD_DESC_TID_Pos);
    }

    command_descriptor |= (dev_index << I3C_CMD_DESC_DEV_INDEX_Pos) & I3C_CMD_DESC_DEV_INDEX_Msk;
    command_descriptor |= (bitrate_setting << I3C_CMD_DESC_XFER_MODE_Pos) & I3C_CMD_DESC_XFER_MODE_Msk;
    command_descriptor |= ((rnw) ? 1U : 0U) << I3C_CMD_DESC_XFER_RNW_Pos;                         /* PRQA S 0499, 1891 # Required R-12.2, Required R-10.7 */
    command_descriptor |= I3C_CMD_DESC_ROC_Msk;
    command_descriptor |= (((restart) ? 0U : 1U) << I3C_CMD_DESC_TOC_Pos) & I3C_CMD_DESC_TOC_Msk; /* PRQA S 0499, 1891 # Required R-12.2, Required R-10.7 */

    return command_descriptor;
}

#endif

#if I3C_CFG_MASTER_SUPPORT

/*******************************************************************************************************************//**
 * config the CCC descriptor by internal control command and handle internal reset.
 *
 * @param[in]  dev_index         The selected index for the transfer.
 * @param[in]  mipi_cmd          include or ignore on off iba.
 *
 * @return                       The calculated value for the command descriptor.
 **********************************************************************************************************************/
static void i3c_internal_command_config (i3c_instance_ctrl_t * p_ctrl, i3c_mipi_cmd_t mipi_cmd) /* PRQA S 3673 # Advisory R-8.13 */
{
    uint32_t command_descriptor         = 0;
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    command_descriptor |= ((uint32_t) I3C_EVENT_COMMAND_COMPLETE << I3C_CMD_DESC_TID_Pos);
    command_descriptor |= I3C_CMD_DESC_CMD_ATTR_INTERNAL_CONTROL;
    command_descriptor |= (uint32_t) mipi_cmd << I3C_CMD_DESC_MIPI_CMD_Pos;
    command_descriptor |= (uint32_t) 1U << I3C_CMD_DESC_ON_OFF_Pos;

    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        p_ctrl->p_reg->NCMDQP = command_descriptor; /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->NCMDQP = 0U;                 /* PRQA S 0662 # Required R-1.1 */

        /* Clear the command queue empty flag. */
        p_ctrl->p_reg->NTST_b.CMDQEF = 0;           /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        p_ctrl->p_reg->HCMDQP = command_descriptor; /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->HCMDQP = 0U;                 /* PRQA S 0662 # Required R-1.1 */

        /* Clear the command queue empty flag. */
        p_ctrl->p_reg->HTST_b.CMDQEF = 0;           /* PRQA S 0662 # Required R-1.1 */
    }
}

#endif

/*******************************************************************************************************************//**
 * Read data from the Normal Transfer Data Buffer and write it into the user provided data buffer.
 * @param[in]  p_ctrl                Pointer to an instance's control structure.
 * @param[in]  bytes                 The number of bytes to read from the FIFO.
 **********************************************************************************************************************/
static void i3c_fifo_read (i3c_instance_ctrl_t * p_ctrl, uint32_t bytes)
{
    int32_t  bytes_remaining = (int32_t) bytes;
    uint32_t rx_data;
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    while (bytes_remaining > 0)
    {
        if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
        {
            /* Each entry in the FIFO is 4 bytes. */
            rx_data = p_ctrl->p_reg->NTDTBP0; /* PRQA S 0662 # Required R-1.1 */

            /* Clear the Read Buffer Full status flag. */
            p_ctrl->p_reg->NTST_b.RDBFF0 = 0; /* PRQA S 0662 # Required R-1.1 */
        }
        else
        {
            /* Each entry in the FIFO is 4 bytes. */
            rx_data = p_ctrl->p_reg->HTDTBP; /* PRQA S 0662 # Required R-1.1 */

            /* Clear the Read Buffer Full status flag. */
            p_ctrl->p_reg->HTST_b.RDBFF = 0; /* PRQA S 0662 # Required R-1.1 */
        }

        /*
         * Store the next word of data into the read buffer. If there is not enough space, a I3C_EVENT_READ_BUFFER_FULL
         * event will be called to notify the application that a new read buffer is required.
         */
        i3c_read_buffer_store(p_ctrl,
                              &p_ctrl->read_buffer_descriptor,
                              rx_data,
                              (uint32_t) bytes_remaining,
                              I3C_EVENT_READ_BUFFER_FULL);

        bytes_remaining -= 4;

        p_ctrl->read_transfer_count_final += 4U;
    }
}

/*******************************************************************************************************************//**
 * Write a word of data read from a FIFO into the read buffer.
 *
 * @param[in]  p_ctrl                Pointer to an instance's control structure.
 * @param[in]  p_buffer_descriptor   Pointer to a read buffer descriptor for storing the word of data.
 * @param[in]  data_word             Word of data read from a FIFO.
 * @param[in]  num_bytes             The number of valid data bytes in the data word.
 * @param[in]  buffer_full_event     The event to notify that application when their is no more space available in the
 *                                   buffer descriptor.
 **********************************************************************************************************************/
static void i3c_read_buffer_store (i3c_instance_ctrl_t * const    p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                   i3c_read_buffer_descriptor_t * p_buffer_descriptor,
                                   uint32_t                       data_word,
                                   uint32_t                       num_bytes,
                                   i3c_event_t                    buffer_full_event)
{
#if !I3C_CFG_UNALIGNED_BUFFER_SUPPORT
    FSP_PARAMETER_NOT_USED(num_bytes);
#endif

#if I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* If unaligned buffers are supported, the word must be written one byte at a time. */
    for (uint32_t i = 0; (i < sizeof(uint32_t)) && (i < num_bytes); i++) /* PRQA S 1710 # Advisory R-1.3 */
    {
#endif

    /* Check if the buffer descriptor has space available. */
    if (p_buffer_descriptor->count >= p_buffer_descriptor->buffer_size)
    {
        /* If there is no more space available, and the driver already notified the application,
         * then discard the data. */
        if (p_buffer_descriptor->read_request_issued)
        {
            return;
        }

        i3c_callback_args_t callback_args =
        {
            .event     = buffer_full_event,
            .p_context = p_ctrl->p_cfg->p_context,
        };

        /* Update the buffer descriptor state so that it will not notify the application again until a new buffer
         * is provided. */
        p_buffer_descriptor->read_request_issued = true;

        /* Notify the application that the read buffer is full. */
        p_ctrl->p_cfg->p_callback(&callback_args);

        /* If the application didn't provide a new read buffer, then discard the data. */
        if (p_buffer_descriptor->read_request_issued)
        {
            return;
        }
    }

#if I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* Write the data to the read buffer one byte at a time. */
    p_buffer_descriptor->p_buffer[p_buffer_descriptor->count++] = /* PRQA S 3387, 3440 # Advisory R-13.3, Advisory R-10.6 */
                                                                  (uint8_t) ((data_word >> (i * 8U)) &
                                                                             (uint32_t) UINT8_MAX);
}
#endif

#if !I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* Write the data to the read buffer one word at a time. */
    *((uint32_t *) &p_buffer_descriptor->p_buffer[p_buffer_descriptor->count]) = data_word;
    p_buffer_descriptor->count += 4U;
#endif
}

/*******************************************************************************************************************//**
 * Calculate the number of bytes left in the FIFO at the end of a transfer. If the buffer is aligned, then it is safe
 * to assume that the bytes remaining is a multiple of 4 bytes.
 *
 * @param[in]  p_ctrl                Pointer to an instance's control structure.
 * @param[in]  data_length           The total transfer length read from a status descriptor.
 **********************************************************************************************************************/
static uint32_t i3c_read_bytes_remaining_calculate (i3c_instance_ctrl_t * p_ctrl, uint32_t data_length) /* PRQA S 3673 # Advisory R-8.13 */
{
    uint32_t bytes_remaining = 0;
    if ((0U < p_ctrl->p_reg->NDBSTLV0_b.RDBLV) || (0U < p_ctrl->p_reg->HDBSTLV_b.RDBLV))                /* PRQA S 0662 # Required R-1.1 */
    {
        bytes_remaining = data_length - p_ctrl->read_transfer_count_final;
    }

    return bytes_remaining;
}

/*******************************************************************************************************************//**
 * Fill the transmit FIFO with data from a write buffer descriptor. If there is no more data in the write buffer,
 * then disable the transmit IRQ, and return.
 *
 * @param[in]  p_ctrl                Pointer to an instance's control structure.
 *
 **********************************************************************************************************************/
static void i3c_fifo_write (i3c_instance_ctrl_t * p_ctrl)
{
#if I3C_CFG_TRANSFER_ENABLE
    uint8_t remain_data_size = I3C_NUM_BYTES_PER_TRANSFER;
    volatile const uint32_t * p_ri3c_tx_buffer = &(p_ctrl->p_reg->NTDTBP0);                     /* PRQA S 0662 # Required R-1.1 */
#endif
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t value = 0;
    do
    {
        if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
        {
            /* Write data to the transmit FIFO. */
            p_ctrl->p_reg->NTDTBP0 = p_ctrl->next_word; /* PRQA S 0662 # Required R-1.1 */
            value = p_ctrl->p_reg->NDBSTLV0;            /* PRQA S 0662 # Required R-1.1 */
        }
        else
        {
            /* Write data to the transmit FIFO. */
            p_ctrl->p_reg->HTDTBP = p_ctrl->next_word;      /* PRQA S 0662 # Required R-1.1 */
            value                 = p_ctrl->p_reg->HDBSTLV; /* PRQA S 0662 # Required R-1.1 */
        }

        /* If there is no more data to write, disable the transmit IRQ and return. */
        if (p_ctrl->write_buffer_descriptor.count >= p_ctrl->write_buffer_descriptor.buffer_size)
        {
            if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
            {
                p_ctrl->p_reg->NTIE_b.TDBEIE0 = 0; /* PRQA S 0662 # Required R-1.1 */
                R_BSP_IrqStatusClear(p_extend->tx_irq);
            }
            else
            {
                p_ctrl->p_reg->HTIE_b.TDBEIE = 0; /* PRQA S 0662 # Required R-1.1 */
                R_BSP_IrqStatusClear(p_extend->htx_irq);
            }

            break;
        }

#if I3C_CFG_TRANSFER_ENABLE
        if (p_ctrl->write_buffer_descriptor.count > (p_ctrl->write_buffer_descriptor.buffer_size - 4U))
        {
            remain_data_size =
                (uint8_t) (p_ctrl->write_buffer_descriptor.buffer_size - p_ctrl->write_buffer_descriptor.count);
        }
        else
        {
            remain_data_size = I3C_NUM_BYTES_PER_TRANSFER;
        }
#endif

        /* Calculate the next word of data to write to the FIFO. */
        p_ctrl->next_word = i3c_next_data_word_calculate(&p_ctrl->write_buffer_descriptor);

        /* Continue writing data until the transmit FIFO is full. */
    } while (0U != ((value) & (uint32_t) UINT8_MAX));

    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        /* Clear the Transmit Buffer Empty status flag. */
        p_ctrl->p_reg->NTST_b.TDBEF0 = 0; /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        /* Clear the Transmit Buffer Empty status flag. */
        p_ctrl->p_reg->HTST_b.TDBEF = 0; /* PRQA S 0662 # Required R-1.1 */
    }

#if I3C_CFG_TRANSFER_ENABLE
    if (true == p_ctrl->activation_on_txi)
    {
        if ((NULL != p_ctrl->p_cfg->p_transfer_tx) &&
            ((p_ctrl->write_buffer_descriptor.count - 4U) < p_ctrl->write_buffer_descriptor.buffer_size))
        {
            /* Change the register setting if the transfer mode is high priority */
            if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_HIGH_PRIORITY)
            {
                p_ri3c_tx_buffer = &(p_ctrl->p_reg->HTDTBP); /* PRQA S 0662 # Required R-1.1 */
            }

            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = 1U;
            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           =
                (uint32_t *) (&p_ctrl->write_buffer_descriptor.p_buffer[p_ctrl->write_buffer_descriptor.count - /* PRQA S 0310, 3305 # Advisory R-11.4 */
                                                                        remain_data_size]);
            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest = (uint32_t *) p_ri3c_tx_buffer;                /* PRQA S 0311, 0312 # Advisory R-11.8, Advisory R-11.4 */
            (void) p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                                    p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);

            /* Update the current data transfer count */
            p_ctrl->write_buffer_descriptor.count += I3C_TRANSFER_SIZE_PER_COUNT;
            (void) p_ctrl->p_cfg->p_transfer_tx->p_api->enable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        }
    }
#endif
}

/*******************************************************************************************************************//**
 * Compute the value of the next data word in a write transfer.
 *
 * @param[in]  p_buffer_descriptor   Pointer to a write buffer descriptor.
 *
 **********************************************************************************************************************/
static uint32_t i3c_next_data_word_calculate (i3c_write_buffer_descriptor_t * p_buffer_descriptor)
{
    uint32_t data_word = 0;

    /* Read the current transfer count. */
    uint32_t        count           = p_buffer_descriptor->count;
    uint8_t const * p_transfer_data = p_buffer_descriptor->p_buffer;

#if I3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* Compute the value of the next data word that will be written. */
    for (uint32_t i = 0; (i < sizeof(uint32_t)) && (count < p_buffer_descriptor->buffer_size); i++)
    {
        /* If there is no more data to write, then exit. */
        data_word |= ((uint32_t) p_transfer_data[count++] << (i * 8U)); /* PRQA S 3387, 3440 # Advisory R-13.3, Advisory R-10.6 */
    }

#else

    /* If the buffer is aligned, then read the data from the buffer using a word access. */
    data_word = *((uint32_t *) (&p_transfer_data[count]));
    count    += 4U;
#endif

    /* Update the transfer count. */
    p_buffer_descriptor->count = count;

    return data_word;
}

/*******************************************************************************************************************//**
 * Get the control instance associated with the active IRQ.
 **********************************************************************************************************************/
static i3c_ctrl_t * i3c_isr_context_get (void)
{
    IRQn_Type             irq    = R_FSP_CurrentIrqGet();
    i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */
    R_BSP_IrqStatusClear(irq);

    return p_ctrl;
}

/*******************************************************************************************************************//**
 * Sets the interrupt priority and context, clears pending interrupts, then enables the interrupt.
 **********************************************************************************************************************/

static void r_i3c_bsp_irq (i3c_instance_ctrl_t * p_ctrl, IRQn_Type const irq, i3c_bsp_irq_t irq_type)
{
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* Clear the interrupt status flag for a given interrupt*/
    R_BSP_IrqStatusClear(irq);
    switch (irq_type)
    {
        case I3C_BSP_IRQ_DISABLE:
        {
            /* Disable IRQ. */
            R_BSP_IrqDisable(irq);
            break;
        }

        case I3C_BSP_IRQ_ENABLE:
        {
            /* Enable IRQ. */
            R_BSP_IrqEnable(irq);
            break;
        }

        case I3C_BSP_IRQ_CFG_ENABLE:
        {
            /* Enable IRQ. */
            R_BSP_IrqCfgEnable(irq, p_extend->ipl, p_ctrl);
            break;
        }

        case I3C_BSP_IRQ_CFG_ERR_ENABLE:
        {
            /* Enable IRQ. */
            R_BSP_IrqCfgEnable(irq, p_extend->eei_ipl, p_ctrl);
            break;
        }

        default:
        {
            /* No action required for any other IRQ configuration state. */
            break;
        }
    }
}

#if I3C_CFG_SLAVE_SUPPORT

/*******************************************************************************************************************//**
 * Perform slave error recovery
 **********************************************************************************************************************/
static void i3c_slave_error_recovery (i3c_instance_ctrl_t * p_ctrl, i3c_slave_error_recovery_type_t recovery_type) /* PRQA S 3673 # Advisory R-8.13 */
{
    switch (recovery_type)
    {
        case I3C_SLAVE_ERROR_RECOVERY_TYPE_WRITE:
        {
            /* Flush the Command and Tx Data FIFO. */
            p_ctrl->p_reg->RSTCTL = R_RI3C0_RSTCTL_TDBRST_Msk; /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */
            break;
        }

        case I3C_SLAVE_ERROR_RECOVERY_TYPE_READ:
        {
            /* Flush the Command and Rx Data FIFO. */
            p_ctrl->p_reg->RSTCTL = R_RI3C0_RSTCTL_RDBRST_Msk; /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */
            break;
        }

        case I3C_SLAVE_ERROR_RECOVERY_TYPE_IBI:
        {
            /* Flush the Command and Rx Data FIFO. */
            p_ctrl->p_reg->RSTCTL = R_RI3C0_RSTCTL_CMDQRST_Msk; /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */
            break;
        }

        default:
        {
            /* No action required for any other recovery type. */
            break;
        }
    }

    /* The field will be cleared automatically upon reset completion. */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->RSTCTL, 0U); /* PRQA S 0662 # Required R-1.1 */

    /* Resume I3C operation. */
    p_ctrl->p_reg->BCTL_b.RSM = 1;                         /* PRQA S 0662 # Required R-1.1 */
}

#endif

/*******************************************************************************************************************//**
 * Perform internal reset
 **********************************************************************************************************************/
static void i3c_internal_reset (i3c_instance_ctrl_t * p_ctrl)        /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Set Internal reset*/
    p_ctrl->p_reg->RSTCTL_b.INTLRST = 1;                             /* PRQA S 0662 # Required R-1.1 */

    /* Wait until the field is actually set to 1.*/
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->RSTCTL_b.INTLRST, 1U); /* PRQA S 0662 # Required R-1.1 */

    /* Set Internal reset*/
    p_ctrl->p_reg->RSTCTL_b.INTLRST = 0;                             /* PRQA S 0662 # Required R-1.1 */
}

#if I3C_CFG_TRANSFER_ENABLE

/*******************************************************************************************************************//**
 * Configures I3C related transfer drivers (if enabled).
 **********************************************************************************************************************/
static fsp_err_t r_i3c_transfer_open (i3c_instance_ctrl_t * p_ctrl)
{
    /* Declare local variable*/
    fsp_err_t err = FSP_SUCCESS;

    /* Hard-fixed configuration for initializing transfer*/
    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        /* Select activation source with suitable transfer mode */

        /* Initializing transfer */
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->open(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                        p_ctrl->p_cfg->p_transfer_tx->p_cfg);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);                                        /* PRQA S 3432 # Advisory R-20.7 */

        /* Set callback function for transmission */
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->callbackSet(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                               r_i3c_txi_transfer_callback, /* PRQA S 3635 # Advisory R-13.4 */
                                                               p_ctrl,
                                                               NULL);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);                                          /* PRQA S 3432 # Advisory R-20.7 */

        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src  = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest = (uint32_t *) NULL;

        /* Disable transfer*/
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */
    }

    /* Hard-fixed configuration for initializing transfer reception*/
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        /* Select activation source with suitable transfer mode */

        /* Initializing transfer */
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->open(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                        p_ctrl->p_cfg->p_transfer_rx->p_cfg);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);                                        /* PRQA S 3432 # Advisory R-20.7 */

        /* Set callback function for transmission */
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->callbackSet(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                               r_i3c_rxi_transfer_callback, /* PRQA S 3635 # Advisory R-13.4 */
                                                               p_ctrl,
                                                               NULL);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);                                          /* PRQA S 3432 # Advisory R-20.7 */

        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src  = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest = (uint32_t *) NULL;

        /* Disable transfer*/
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # Advisory R-20.7 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Receive called when the transfer is completed.
 *
 * @param[in]     p_args     Pointer to callback.
 **********************************************************************************************************************/
static void r_i3c_rxi_transfer (i3c_instance_ctrl_t * p_ctrl)
{
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /*
     * If the transfer is complete, the remaining data in the Rx Data FIFO must be handled
     * in the Receive Status Queue Full ISR or Response Status Queue Full ISR.
     *
     * The final valid byte count is known only from the DATA_LENGTH field in the
     * Receive Status Descriptor or Response Descriptor.
     */
    uint16_t value = 0U;
    uint32_t retry = I3C_TDBFLV_POLL_MAX_COUNT;

    /* Default setting: Normal FIFO buffer transfer. */
    volatile const uint32_t * p_ri3c_rx_buffer       = &(p_ctrl->p_reg->NTDTBP0); /* PRQA S 0662 # Required R-1.1 */
    volatile uint32_t       * p_ri3c_transfer_status = &(p_ctrl->p_reg->NTST);    /* PRQA S 0662 # Required R-1.1 */

    /*
     * In Normal mode, do not start another DMA/DTS transfer when either:
     * - Receive Status Queue Full is pending, or
     * - Response Queue Full is pending.
     *
     * The remaining FIFO data must be read after DATA_LENGTH is known.
     */
    uint32_t status_check_mask = R_RI3C0_NTST_RSQFF_Msk | R_RI3C0_NTST_RSPQFF_Msk;

    /* Normal Rx Data Buffer Full flag clear mask. */
    uint32_t rdbff_clear_mask = R_RI3C0_NTST_RDBFF0_Msk;                                 /* PRQA S 1257 # Advisory R-10.6 */

    /*
     * RXDBTH encoding:
     * 0: 2 DWORDs
     * 1: 4 DWORDs
     * 2: 8 DWORDs
     * ...
     * 7: 256 DWORDs
     */
    uint16_t rx_threshold = (uint16_t) (1U << (p_ctrl->p_reg->NTBTHCTL0_b.RXDBTH + 1U)); /* PRQA S 4391 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */

    uint32_t remain_bytes = 0U;

    /* Change the register setting if the transfer mode is high priority */
    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_HIGH_PRIORITY)
    {
        p_ri3c_rx_buffer       = &(p_ctrl->p_reg->HTDTBP);                         /* PRQA S 0662 # Required R-1.1 */
        p_ri3c_transfer_status = &(p_ctrl->p_reg->HTST);                           /* PRQA S 0662 # Required R-1.1 */

        /* Get High Priority Rx Data Buffer threshold in DWORDs. */
        rx_threshold = (uint16_t) (1U << (p_ctrl->p_reg->HTBTHCTL_b.RXDBTH + 1U)); /* PRQA S 4391 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */

        /* High Priority has Response Queue Full, but no Receive Status Queue Full.*/
        status_check_mask = R_RI3C0_HTST_RSPQFF_Msk;                               /* PRQA S 1257 # Advisory R-10.6 */

        /* High Priority Rx Data Buffer Full flag clear mask. */
        rdbff_clear_mask = R_RI3C0_HTST_RDBFF_Msk;                                 /* PRQA S 1257 # Advisory R-10.6 */
    }

    R_BSP_IrqStatusClear(p_extend->hrx_irq);
    R_BSP_IrqStatusClear(p_extend->rx_irq);

    /*
     * If Response Queue Full or Receive Status Queue Full is pending, do not read more data here.
     * The remaining bytes must be handled by Response/Receive Status ISR because the final
     * valid byte count is known from DATA_LENGTH.
     */
    if (0U == ((*p_ri3c_transfer_status) & status_check_mask))
    {
        /*
         * Check if the user read buffer still has free space.
         * If the buffer is already full, do not start another DMA/DTS transfer.
         */
        if (p_ctrl->read_buffer_descriptor.count < p_ctrl->read_buffer_descriptor.buffer_size)
        {
            /*
             * Read current Rx Data Buffer level in DWORDs.
             *
             * Normal mode      : NDBSTLV0.RDBLV
             * High Priority    : HDBSTLV.RDBLV
             */
            do
            {
                if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_HIGH_PRIORITY)
                {
                    value = p_ctrl->p_reg->HDBSTLV_b.RDBLV;  /* PRQA S 0662 # Required R-1.1 */
                }
                else
                {
                    value = p_ctrl->p_reg->NDBSTLV0_b.RDBLV; /* PRQA S 0662 # Required R-1.1 */
                }

                retry--;
            } while ((value == 0U) && (retry > 0U));

            /*
             * Calculate remaining byte space in the user buffer.
             * DMA/DTS transfer count is in DWORD units, so only full DWORDs are transferred here.
             */
            remain_bytes = p_ctrl->read_buffer_descriptor.buffer_size - p_ctrl->read_buffer_descriptor.count;
            if (((uint32_t) value * I3C_TRANSFER_SIZE_PER_COUNT) > remain_bytes)
            {
                value = (uint16_t) (remain_bytes / I3C_TRANSFER_SIZE_PER_COUNT);
            }

            /*
             * Limit transfer count by configured RX threshold.
             *
             * RXDBTH encoding:
             *   0: 2 DWORDs
             *   1: 4 DWORDs
             *   2: 8 DWORDs
             *   ...
             *   7: 256 DWORDs
             */
            if (value > rx_threshold)
            {
                value = rx_threshold;
            }

            /*
             * Start the next RX DMA/DTS block only when:
             *   - RX transfer instance exists, and
             *   - there is at least one DWORD available to transfer.
             */
            if ((NULL != p_ctrl->p_cfg->p_transfer_rx) && (value > 0U))
            {
                (void) p_ctrl->p_cfg->p_transfer_rx->p_api->reset(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                                  (uint32_t *) (p_ri3c_rx_buffer), /* PRQA S 0311, 0312 # Advisory R-11.8, Advisory R-11.4 */
                                                                  (void *) (&p_ctrl->read_buffer_descriptor.p_buffer[
                                                                                p_ctrl
                                                                                ->
                                                                                read_buffer_descriptor
                                                                                .
                                                                                count
                                                                            ]),
                                                                  (uint16_t) (value));

                /*
                 * Update software counters immediately after programming DMA/DTS.
                 * Each transfer count corresponds to one DWORD, 4 bytes.
                 */
                p_ctrl->read_buffer_descriptor.count += value * I3C_TRANSFER_SIZE_PER_COUNT; /* PRQA S 1891 # Advisory R-10.6 */
                p_ctrl->read_transfer_count_final    += value * I3C_TRANSFER_SIZE_PER_COUNT; /* PRQA S 1891 # Advisory R-10.6 */
            }

            /*
             * Clear the Rx Data Buffer Full status flag.
             *
             * In I3C mode, RDBFF/RDBFF0 is cleared by writing 0 after the flag has been read.
             */
            *p_ri3c_transfer_status = (*p_ri3c_transfer_status) & (~rdbff_clear_mask);
        }
    }
}

/*******************************************************************************************************************//**
 * Load number transfer count.
 *
 * @param[in]     p_args     Pointer to callback.
 **********************************************************************************************************************/
static uint16_t r_i3c_load_number_count (i3c_instance_ctrl_t * p_ctrl)                          /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Declare local variable*/
    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    uint16_t count        = p_ctrl->p_reg->NDBSTLV0_b.TDBFLV;                                   /* PRQA S 0662 # Required R-1.1 */
    uint16_t remain_count = 0;
    uint32_t retry        = I3C_TDBFLV_POLL_MAX_COUNT;

    /* Get Tx Data Buffer Free Level.
     * NDBSTLV0.TDBFLV/HDBSTLV.TDBFLV indicate number of free Tx data buffer entries in DWORDs.
     */
    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_HIGH_PRIORITY)
    {
        /*
         * In High Priority mode, HDBSTLV.TDBFLV may not be updated immediately
         * due to clock domain crossing or low baudrate. Poll with timeout to avoid ISR hang.
         */
        do
        {
            count = p_ctrl->p_reg->HDBSTLV_b.TDBFLV; /* PRQA S 0662 # Required R-1.1 */
            retry--;
        } while ((count == 0U) && (retry > 0U));
    }
    else
    {
        /*
         * In Normal mode, NDBSTLV0.TDBFLV may not be updated immediately
         * due to clock domain crossing or low baudrate. Poll with timeout to avoid ISR hang.
         */
        do
        {
            count = p_ctrl->p_reg->NDBSTLV0_b.TDBFLV; /* PRQA S 0662 # Required R-1.1 */
            retry--;
        } while ((count == 0U) && (retry > 0U));
    }

    /* If remaining data less than data buffer free level, this function returns length of remain data.
     * If remaining data more than data buffer free level, this function returns value of the data buffer free level*/
    if ((p_ctrl->write_buffer_descriptor.count >= I3C_TRANSFER_SIZE_PER_COUNT) &&
        (p_ctrl->write_buffer_descriptor.count <= p_ctrl->write_buffer_descriptor.buffer_size))
    {
        remain_count =
            (uint16_t) ((p_ctrl->write_buffer_descriptor.buffer_size -
                         (p_ctrl->write_buffer_descriptor.count - I3C_TRANSFER_SIZE_PER_COUNT)) /
                        I3C_TRANSFER_SIZE_PER_COUNT);
        if (remain_count < count)
        {
            count = remain_count;
        }
    }

    return count;
}

/*******************************************************************************************************************//**
 * Transmit data empty interrupt routine called when the transfer is completed.
 *
 * @param[in]     p_args     Pointer to callback.
 **********************************************************************************************************************/
void r_i3c_txi_transfer_callback (transfer_callback_args_t * p_args)                                /* PRQA S 1505, 3673 # Published API, Advisory R-8.13 */
{
    if ((NULL != p_args) && (NULL != p_args->p_context))
    {
        i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) p_args->p_context;                   /* PRQA S 0311, 0316 # Advisory R-11.8, Advisory R-11.5 */

        /* Declare local variable */
        i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
        uint8_t  checkdata = I3C_TX_EMPTY_FLAG_NOT_SET;
        uint16_t value     = 0;
        volatile const uint32_t * p_ri3c_tx_buffer       = &(p_ctrl->p_reg->NTDTBP0);               /* PRQA S 0662 # Required R-1.1 */
        volatile uint32_t       * p_ri3c_transfer_status = &(p_ctrl->p_reg->NTST);                  /* PRQA S 0662 # Required R-1.1 */
        uint32_t tx_empty_mask = R_RI3C0_NTST_TDBEF0_Msk;                                           /* PRQA S 1257 # Advisory R-10.6 */

        R_BSP_IrqStatusClear(p_extend->htx_irq);
        R_BSP_IrqStatusClear(p_extend->tx_irq);

        /* Change the register setting if the transfer mode is high priority */
        if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_HIGH_PRIORITY)
        {
            p_ri3c_tx_buffer       = &(p_ctrl->p_reg->HTDTBP); /* PRQA S 0662 # Required R-1.1 */
            p_ri3c_transfer_status = &(p_ctrl->p_reg->HTST);   /* PRQA S 0662 # Required R-1.1 */
            tx_empty_mask          = R_RI3C0_HTST_TDBEF_Msk;   /* PRQA S 1257 # Advisory R-10.6 */
        }

        /* if remain data is valid, execute transfer */
        if ((p_ctrl->write_buffer_descriptor.count - 4U) < p_ctrl->write_buffer_descriptor.buffer_size)
        {
            checkdata = (uint8_t) (((*p_ri3c_transfer_status) & R_RI3C0_NTST_TDBEF0_Msk) >> R_RI3C0_NTST_TDBEF0_Pos);
            if (checkdata == I3C_TX_EMPTY_FLAG_SET)
            {
                value = r_i3c_load_number_count(p_ctrl);

                if (value > I3C_TX_DMAC_MAX_TRANSFER_COUNT)
                {
                    value = I3C_TX_DMAC_MAX_TRANSFER_COUNT;
                }
            }

            if ((NULL != p_ctrl->p_cfg->p_transfer_tx) && (value > 0U))
            {
                (void) p_ctrl->p_cfg->p_transfer_tx->p_api->reset(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                                  (void *) (&p_ctrl->write_buffer_descriptor.p_buffer[
                                                                                p_ctrl->
                                                                                write_buffer_descriptor
                                                                                .
                                                                                count -
                                                                                4U]),
                                                                  (uint32_t *) (p_ri3c_tx_buffer), /* PRQA S 0311, 0312 # Advisory R-11.8, Advisory R-11.4 */
                                                                  (uint16_t) (value));

                /* Clear the Write Buffer Full status flag. */
                *p_ri3c_transfer_status = (*p_ri3c_transfer_status) & (~tx_empty_mask);

                /* Update the current data transfer count */
                p_ctrl->write_buffer_descriptor.count += (value) * I3C_TRANSFER_SIZE_PER_COUNT; /* PRQA S 1891 # Advisory R-10.6 */
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Receive data interrupt routine called when the transfer is completed.
 *
 * @param[in]     p_args     Pointer to callback.
 **********************************************************************************************************************/
void r_i3c_rxi_transfer_callback (transfer_callback_args_t * p_args)                /* PRQA S 1505, 3673 # Published API, Advisory R-8.13 */
{
    if ((NULL != p_args) && (NULL != p_args->p_context))
    {
        i3c_instance_ctrl_t * p_ctrl = (i3c_instance_ctrl_t *) (p_args->p_context); /* PRQA S 0311, 0316 # Advisory R-11.8, Advisory R-11.5 */
        r_i3c_rxi_transfer(p_ctrl);
    }
}

#endif

/*******************************************************************************************************************//**
 * ISR for handling the Command Response IRQ. The Command Response IRQ occurs on completion of a command and
 * provides status information.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void i3c_resp_isr (void) /* PRQA S 1503 # Published API */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    i3c_instance_ctrl_t * p_ctrl = i3c_isr_context_get();                                       /* PRQA S 0317 # Advisory R-11.5 */

    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

#if I3C_CFG_TRANSFER_ENABLE
    transfer_properties_t get_info;
#endif

    /* Get the response status from the Response Status Queue. */
    uint32_t response_status_descriptor;
    uint32_t bytes_remaining = 0;
    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        response_status_descriptor = p_ctrl->p_reg->NRSPQP; /* PRQA S 0662 # Required R-1.1 */

        /* Clear the Response Queue Full status flag. */
        p_ctrl->p_reg->NTST_b.RSPQFF = 0;                   /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        response_status_descriptor = p_ctrl->p_reg->HRSPQP; /* PRQA S 0662 # Required R-1.1 */

        /* Clear the Response Queue Full status flag. */
        p_ctrl->p_reg->HTST_b.RSPQFF = 0;                   /* PRQA S 0662 # Required R-1.1 */
    }

    /* Read the DATA_LENGTH field in the response descriptor. */
    uint32_t data_length = (response_status_descriptor & I3C_RESP_STATUS_DESC_DATA_LENGTH_Msk) >>
                           I3C_RESP_STATUS_DESC_DATA_LENGTH_Pos;

    /* Read the ERR_STATUS field in the response descriptor. */
    uint32_t err_status = (response_status_descriptor & I3C_RESP_STATUS_DESC_ERR_STATUS_Msk) >>
                          I3C_RESP_STATUS_DESC_ERR_STATUS_Pos;

    bool error_recovery_case_2 = false;

    uint32_t internal_state = p_ctrl->internal_state;

    i3c_callback_args_t callback_args;
    callback_args.event        = (i3c_event_t) (internal_state & I3C_INTERNAL_EVENT_MASK);                  /* PRQA S 4342, 4394 # Advisory R-10.5, Advisory R-10.6 */
    callback_args.event_status = err_status;
    callback_args.p_context    = p_ctrl->p_cfg->p_context;

    if ((I3C_DEVICE_TYPE_MAIN_MASTER == p_ctrl->p_cfg->device_type) || (0U != p_ctrl->p_reg->PRSST_b.CRMS)) /* PRQA S 0662 # Required R-1.1 */
    {
        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE;
    }
    else
    {
        p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE;
    }

    switch (internal_state)
    {
#if I3C_CFG_MASTER_SUPPORT
        case I3C_INTERNAL_STATE_MASTER_ENTDAA: /* PRQA S 0570 # Advisory R-10.6 */
        {
            /* Reset the read buffer descriptor that was used to read the slave info. */
            p_ctrl->read_buffer_descriptor = (i3c_read_buffer_descriptor_t) {
                0
            };

            break;
        }

        case I3C_INTERNAL_STATE_MASTER_WRITE:         /* PRQA S 0570 # Advisory R-10.6 */
        case I3C_INTERNAL_STATE_MASTER_COMMAND_WRITE: /* PRQA S 0570 # Advisory R-10.6 */
        {
            /* Calculate the total transfer length. Note that for a write transfer, the DATA_LENGTH field in the response
             * descriptor provides the number of bytes remaining. */
            callback_args.transfer_size = p_ctrl->write_buffer_descriptor.buffer_size - data_length;

            /* Reset the write buffer descriptor that was used for the transfer. */
            p_ctrl->write_buffer_descriptor = (i3c_write_buffer_descriptor_t) {
                0
            };
            if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
            {
                /* Disable the transmit IRQ if it hasn't been disabled already. */
                p_ctrl->p_reg->NTIE_b.TDBEIE0 = 0; /* PRQA S 0662 # Required R-1.1 */
                R_BSP_IrqStatusClear(p_extend->tx_irq);
            }
            else
            {
                /* Disable the transmit IRQ if it hasn't been disabled already. */
                p_ctrl->p_reg->HTIE_b.TDBEIE = 0; /* PRQA S 0662 # Required R-1.1 */
                R_BSP_IrqStatusClear(p_extend->htx_irq);
            }

 #if I3C_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_tx)
            {
                p_ctrl->activation_on_txi = false;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = 0U;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
                (void) p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
                (void) p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                                        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
            }
 #endif
            break;
        }

        case I3C_INTERNAL_STATE_MASTER_READ:         /* PRQA S 0570 # Advisory R-10.6 */
        case I3C_INTERNAL_STATE_MASTER_COMMAND_READ: /* PRQA S 0570 # Advisory R-10.6 */
        {
 #if I3C_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_rx)
            {
                p_ctrl->activation_on_rxi = false;
                if ((uint32_t) I3C_INTERNAL_STATE_MASTER_READ == internal_state)
                {
                    (void) p_ctrl->p_cfg->p_transfer_rx->p_api->infoGet(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                                        &get_info);
                    p_ctrl->read_buffer_descriptor.count -= get_info.transfer_length_remaining;
                    p_ctrl->read_transfer_count_final     = p_ctrl->read_buffer_descriptor.count;
                }

                r_i3c_bsp_irq(p_ctrl, p_extend->hrx_irq, I3C_BSP_IRQ_ENABLE);
                r_i3c_bsp_irq(p_ctrl, p_extend->rx_irq, I3C_BSP_IRQ_ENABLE);
                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = 0U;
                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
                (void) p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
                (void) p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                                        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
            }
 #endif
            bytes_remaining = i3c_read_bytes_remaining_calculate(p_ctrl, data_length);

            /* Read the remaining byte stored in the FIFO. */
            i3c_fifo_read(p_ctrl, bytes_remaining);

            /* If the transfer length is less than expected, the driver must perform error recovery defined */
            if (data_length != p_ctrl->read_buffer_descriptor.buffer_size)
            {
                error_recovery_case_2 = true;
            }

            /*
             * For a read transfer, the DATA_LENGTH field in the response descriptor provides the total number of bytes
             * read.
             */
            callback_args.transfer_size = data_length;

            /* Reset the read buffer descriptor that was used for the read transfer. */
            p_ctrl->read_buffer_descriptor = (i3c_read_buffer_descriptor_t) {
                0
            };
 #if I3C_CFG_SLAVE_SUPPORT
            if ((uint8_t) I3C_CCC_DIRECT_GETACCMST == p_ctrl->current_command_code)
            {
                if (0U == (p_ctrl->p_reg->PRSST & R_RI3C0_PRSST_CRMS_Msk)) /* PRQA S 0662 # Required R-1.1 */
                {
                    /* If the GETACCMST command was successful, transition the driver to slave mode. */
                    p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE;

                    /*
                     * Configure IBI queue threshold so that an IBI IRQ is generated when there is one empty entry in the
                     * IBI transmit FIFO.
                     */
                    p_ctrl->p_reg->NQTHCTL_b.IBIQTH = 1;           /* PRQA S 0662 # Required R-1.1 */

                    /* Enable IBI transfer when the device is acting as a slave. */
                    p_ctrl->p_reg->SVDCT_b.TBCR |= (uint8_t) (2U); /* PRQA S 0662 # Required R-1.1 */

                    i3c_internal_reset(p_ctrl);
                }
                else
                {
                    /* If the command was not successful, re-enable the IBI Write Buffer Empty/Full IRQ. */
                    p_ctrl->p_reg->NTIE_b.IBIQEFIE = 1; /* PRQA S 0662 # Required R-1.1 */
                }
            }
 #endif

            p_ctrl->current_command_code = 0;
            break;
        }
#endif

#if I3C_CFG_SLAVE_SUPPORT
        case I3C_INTERNAL_STATE_SLAVE_IBI: /* PRQA S 0570 # Advisory R-10.6 */
        {
            /* Calculate the total transfer length. Note that for an IBI transfer, the DATA_LENGTH field in the response
             * descriptor provides the number of bytes remaining. */
            callback_args.transfer_size = p_ctrl->ibi_buffer_descriptor.buffer_size - data_length;

            /* Reset the buffer descriptor that was used for the IBI transfer. */
            p_ctrl->ibi_buffer_descriptor = (i3c_read_buffer_descriptor_t) {
                0
            };

            p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE;
            break;
        }
#endif
        default:
        {
            /* No action required for any other internal state. */
            break;
        }
    }

    /* Reset the total bytes read. */
    p_ctrl->read_transfer_count_final = 0;

    uint32_t ntst;
    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        ntst = p_ctrl->p_reg->NTST;    /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        ntst = p_ctrl->p_reg->HTST;    /* PRQA S 0662 # Required R-1.1 */
    }

    /*
     * If the transfer was aborted, then the abort flag must be cleared before notifying the application
     * that a transfer has completed.
     */
    if (0U != (ntst & R_RI3C0_NTST_TABTF_Msk))
    {
        p_ctrl->p_reg->BCTL_b.ABT = 0; /* PRQA S 0662 # Required R-1.1 */
    }

    /* If a transfer error occurs, follow the error recovery operation. */
    if ((0U != (ntst & R_RI3C0_NTST_TEF_Msk)) || error_recovery_case_2)
    {
#if I3C_CFG_SLAVE_SUPPORT
        if ((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE == p_ctrl->internal_state)
        {
            i3c_slave_error_recovery(p_ctrl, I3C_SLAVE_ERROR_RECOVERY_TYPE_IBI);
        }
        else
#endif
        {
#if I3C_CFG_MASTER_SUPPORT
            i3c_master_error_recovery(p_ctrl, error_recovery_case_2);
#endif
        }
    }

    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        /* Clear error status flags. */
        p_ctrl->p_reg->NTST = p_ctrl->p_reg->NTST & (~(R_RI3C0_NTST_TEF_Msk | R_RI3C0_NTST_TABTF_Msk)); /* PRQA S 0662 # Required R-1.1 */
    }
    else
    {
        /* Clear error status flags. */
        p_ctrl->p_reg->HTST = p_ctrl->p_reg->HTST & (~(R_RI3C0_NTST_TEF_Msk | R_RI3C0_NTST_TABTF_Msk)); /* PRQA S 0662 # Required R-1.1 */
    }

    /* Notify the application of the event. */
    p_ctrl->p_cfg->p_callback(&callback_args);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR for handling the Receive Buffer Full IRQ. The Receive Buffer Full IRQ is generated when the number of entries
 * in the Receive Data Buffer is greater than or equal to the configured threshold.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void i3c_rx_isr (void)                                                  /* PRQA S 1503 # Published API */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE
    i3c_instance_ctrl_t * p_ctrl = i3c_isr_context_get();                                       /* PRQA S 0317 # Advisory R-11.5 */

    i3c_extended_cfg_t const * p_extend = (i3c_extended_cfg_t const *) p_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
#if I3C_CFG_TRANSFER_ENABLE
    volatile const uint32_t * p_ri3c_rx_buffer       = &(p_ctrl->p_reg->NTDTBP0);               /* PRQA S 0662 # Required R-1.1 */
    volatile uint32_t       * p_ri3c_transfer_status = &(p_ctrl->p_reg->NTST);                  /* PRQA S 0662 # Required R-1.1 */
    volatile const uint32_t * p_ri3c_data_buffer     = &(p_ctrl->p_reg->NDBSTLV0);              /* PRQA S 0662 # Required R-1.1 */
    uint16_t r_i3c_value_rx = 0;
#endif

    /*
     * If the transfer is complete, the remainder of the Read Data FIFO must be read in the Receive Status Queue Full,
     * or Response Status Queue Full ISRs.
     *
     * This is because in order to read the remaining data, the driver must know exactly how many total bytes were read
     * during the transfer.
     */
    if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_NORMAL)
    {
        if (0U == (p_ctrl->p_reg->NTST & (R_RI3C0_NTST_RSQFF_Msk | R_RI3C0_NTST_RSPQFF_Msk))) /* PRQA S 0662 # Required R-1.1 */
        {
            i3c_fifo_read(p_ctrl, p_ctrl->p_reg->NDBSTLV0_b.RDBLV * sizeof(uint32_t));        /* PRQA S 0662 # Required R-1.1 */
#if I3C_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_rx)
            {
                p_ctrl->activation_on_rxi = true;
            }
#endif
        }
    }
    else
    {
        if (0U == (p_ctrl->p_reg->HTST & (R_RI3C0_NTST_RSQFF_Msk | R_RI3C0_NTST_RSPQFF_Msk))) /* PRQA S 0662 # Required R-1.1 */
        {
            i3c_fifo_read(p_ctrl, p_ctrl->p_reg->HDBSTLV_b.RDBLV * sizeof(uint32_t));         /* PRQA S 0662 # Required R-1.1 */
#if I3C_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_rx)
            {
                p_ctrl->activation_on_rxi = true;
            }
#endif
        }
    }

#if I3C_CFG_MASTER_SUPPORT
    if ((uint32_t) I3C_INTERNAL_STATE_MASTER_ENTDAA == p_ctrl->internal_state)
    {
        /* The address phase during ENTDAA occurs after 8 bytes (PID, BCR, and DCR) are read. */
        if (8U == p_ctrl->read_buffer_descriptor.count)
        {
            /* Reset the data pointer after reading each device PID, BCR, and DCR. */
            p_ctrl->read_buffer_descriptor.count = 0;

            /* Notify the application that the ENTDAA Address Phase has started. */
            i3c_callback_args_t callback_args;
            callback_args.event        = I3C_EVENT_ENTDAA_ADDRESS_PHASE;
            callback_args.p_slave_info = &p_ctrl->current_slave_info;
            callback_args.p_context    = p_ctrl->p_cfg->p_context;

            p_ctrl->p_cfg->p_callback(&callback_args);
        }
    }
#endif
#if I3C_CFG_TRANSFER_ENABLE
    if ((NULL != p_ctrl->p_cfg->p_transfer_rx) &&
        (p_ctrl->read_buffer_descriptor.count < p_ctrl->read_buffer_descriptor.buffer_size))
    {
        if (p_ctrl->activation_on_rxi == true)
        {
            /* Change the register setting if the transfer mode is high priority */
            if (p_extend->transfer_mode == I3C_FIFO_BUFFER_TRANSFER_HIGH_PRIORITY)
            {
                p_ri3c_rx_buffer       = &(p_ctrl->p_reg->HTDTBP);  /* PRQA S 0662 # Required R-1.1 */
                p_ri3c_transfer_status = &(p_ctrl->p_reg->HTST);    /* PRQA S 0662 # Required R-1.1 */
                p_ri3c_data_buffer     = &(p_ctrl->p_reg->HDBSTLV); /* PRQA S 0662 # Required R-1.1 */
            }

            r_i3c_value_rx =
                (uint16_t) (((*p_ri3c_data_buffer) & R_RI3C0_NDBSTLV0_RDBLV_Msk) >> R_RI3C0_NDBSTLV0_RDBLV_Pos);
            if (r_i3c_value_rx > 0U)
            {
                (void) p_ctrl->p_cfg->p_transfer_rx->p_api->reset(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                                  (void *) (p_ri3c_rx_buffer),                           /* PRQA S 0311, 0312 # Advisory R-11.8, Advisory R-11.4 */
                                                                  (uint32_t *) (&p_ctrl->read_buffer_descriptor.p_buffer /* PRQA S 0310, 3305 # Advisory R-11.4 */
                                                                                [p_ctrl
                                                                                 ->
                                                                                 read_buffer_descriptor
                                                                                 .
                                                                                 count
                                                                                ]),
                                                                  (uint16_t) (r_i3c_value_rx));

                /* Update the current data transfer count */
                p_ctrl->read_buffer_descriptor.count += I3C_TRANSFER_SIZE_PER_COUNT * r_i3c_value_rx; /* PRQA S 1891 # Advisory R-10.6 */
                p_ctrl->read_transfer_count_final    += I3C_TRANSFER_SIZE_PER_COUNT * r_i3c_value_rx; /* PRQA S 1891 # Advisory R-10.6 */

                r_i3c_bsp_irq(p_ctrl, p_extend->hrx_irq, I3C_BSP_IRQ_DISABLE);
                r_i3c_bsp_irq(p_ctrl, p_extend->rx_irq, I3C_BSP_IRQ_DISABLE);

                /* Clear the Read Buffer Full status flag. */
                *p_ri3c_transfer_status = (*p_ri3c_transfer_status) & (~R_RI3C0_NTST_RDBFF0_Msk);
            }
        }
    }
#endif

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR for handling the Transmit Buffer Empty IRQ. The Transmit Buffer Empty IRQ is generated when the number of
 * empties in the Transmit Data Buffer is greater than or equal to the threshold.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void i3c_tx_isr (void) /* PRQA S 1503 # Published API */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    i3c_instance_ctrl_t * p_ctrl = i3c_isr_context_get(); /* PRQA S 0317 # Advisory R-11.5 */

    /* Write data to the FIFO. */
    i3c_fifo_write(p_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR for handling the Receive Status Buffer Full IRQ. The Receive Status Buffer Full IRQ is generated when the
 * number of Receive Status Queue entries is greater than the threshold.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void i3c_rcv_isr (void) /* PRQA S 1503 # Published API */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

#if I3C_CFG_SLAVE_SUPPORT
    i3c_instance_ctrl_t * p_ctrl = i3c_isr_context_get();                           /* PRQA S 0317 # Advisory R-11.5 */
    i3c_extended_cfg_t * p_extend = (i3c_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Advisory R-11.8, Advisory R-11.5, Advisory R-8.13 */
 #if I3C_CFG_TRANSFER_ENABLE
    transfer_properties_t get_info;
 #endif

    /* Get the receive status descriptor for the Receive Status Queue. */
    uint32_t receive_status_descriptor = p_ctrl->p_reg->NRSQP; /* PRQA S 0662 # Required R-1.1 */

    /* Clear the Receive Status Queue Full Flag. */
    p_ctrl->p_reg->NTST_b.RSQFF = 0;                           /* PRQA S 0662 # Required R-1.1 */

    /* Read the DATA_LENGTH field in the response descriptor. */
    uint32_t data_length = receive_status_descriptor & I3C_RECV_STATUS_DESC_DATA_LENGTH_Msk;

    /* Read the ERR_STATUS field in the response descriptor. */
    uint32_t err_status = (receive_status_descriptor & I3C_RECV_STATUS_DESC_ERR_STATUS_Msk) >>
                          I3C_RECV_STATUS_DESC_ERR_STATUS_Pos;

    i3c_callback_args_t callback_args;
    callback_args.p_context    = p_ctrl->p_cfg->p_context;
    callback_args.event_status = err_status;

    i3c_slave_error_recovery_type_t error_recovery_type = I3C_SLAVE_ERROR_RECOVERY_TYPE_OTHER;

    /* The transfer type is a normal SDR transfer. */
    if (0U == (receive_status_descriptor & I3C_RECV_STATUS_DESC_TRANSFER_TYPE_Msk))
    {
        /* If the transfer is a read transfer. */
        if (0U == (receive_status_descriptor & I3C_RECV_STATUS_DESC_SDR_R_W_TYPE_Msk))
        {
            /* Calculate the remaining number of bytes stored in the read FIFO. */
            uint32_t bytes_remaining = i3c_read_bytes_remaining_calculate(p_ctrl, data_length);
 #if I3C_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_rx)
            {
                (void) p_ctrl->p_cfg->p_transfer_rx->p_api->infoGet(p_ctrl->p_cfg->p_transfer_rx->p_ctrl, &get_info);
                p_ctrl->read_buffer_descriptor.count -= get_info.transfer_length_remaining;
            }
 #endif

            /* Read the remaining bytes stored in the FIFO. */
            i3c_fifo_read(p_ctrl, bytes_remaining);

            /*
             * If the last transfer was a read, then the transfer length was stored in
             * the receive status descriptor.
             */
            callback_args.transfer_size = data_length;
            callback_args.event         = I3C_EVENT_READ_COMPLETE;

 #if I3C_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_rx)
            {
                p_ctrl->activation_on_rxi = false;
                r_i3c_bsp_irq(p_ctrl, p_extend->hrx_irq, I3C_BSP_IRQ_ENABLE);
                r_i3c_bsp_irq(p_ctrl, p_extend->rx_irq, I3C_BSP_IRQ_ENABLE);
                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = 0U;
                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
                (void) p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
                (void) p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                                        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
            }
 #endif

            /* Reset the state for read transfers. */
            p_ctrl->read_buffer_descriptor = (i3c_read_buffer_descriptor_t) {
                0
            };

            /* Set the error recovery procedure to use if there was a transfer error. */
            error_recovery_type = I3C_SLAVE_ERROR_RECOVERY_TYPE_READ;
        }
        else
        {
            /* Calculate the total transfer length. Note that for a write transfer, the DATA_LENGTH field in the receive
             * descriptor provides the number of bytes remaining. */
            callback_args.transfer_size = data_length;
            callback_args.event         = I3C_EVENT_WRITE_COMPLETE;

            /* Reset the state for write transfers. */
            p_ctrl->write_buffer_descriptor = (i3c_write_buffer_descriptor_t) {
                0
            };

            /* Set the error recovery procedure to use if there was a transfer error. */
            error_recovery_type = I3C_SLAVE_ERROR_RECOVERY_TYPE_WRITE;

            /* Disable the transmit IRQ if it hasn't been disabled already. */
            p_ctrl->p_reg->NTIE_b.TDBEIE0 = 0; /* PRQA S 0662 # Required R-1.1 */
            R_BSP_IrqStatusClear(p_extend->tx_irq);
 #if I3C_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_tx)
            {
                p_ctrl->activation_on_txi = false;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = 0U;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
                (void) p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
                (void) p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                                        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
            }
 #endif
        }
    }
    else
    {
        /*
         * The transfer type is a command.
         *
         * Get the command code from the response status descriptor.
         */
        uint8_t command_code =
            (uint8_t) ((receive_status_descriptor & I3C_RECV_STATUS_DESC_CMD_Msk) >> I3C_RECV_STATUS_DESC_CMD_Pos);

        if (((uint8_t) I3C_CCC_BROADCAST_ENTDAA == command_code) ||
            ((uint8_t) I3C_CCC_DIRECT_SETDASA == command_code))
        {
            uint32_t data_length_w = (data_length + sizeof(uint32_t) - 1U) / sizeof(uint32_t);
            for (uint32_t i = 0; i < data_length_w; i++)
            {
                /* Perform a dummy read to flush one FIFO word (volatile read; result intentionally discarded). */
                (void) p_ctrl->p_reg->NTDTBP0; /* PRQA S 0662 # Required R-1.1 */
            }

            /* Reset the state for read transfers. */
            p_ctrl->read_buffer_descriptor = (i3c_read_buffer_descriptor_t) {
                0
            };

            /* Notify the application that the ENTDAA command has completed. */
            callback_args.event = I3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE;

            /* Verify that the slave address is valid. */
            if (0U != p_ctrl->p_reg->SVDVAD0_b.SDYADV)                                            /* PRQA S 0662 # Required R-1.1 */
            {
                /* Provide the assigned dynamic address in the callback. */
                uint32_t dynamic_address_w = (p_ctrl->p_reg->SVDVAD0 & R_RI3C0_SVDVAD0_SVAD_Msk); /* PRQA S 0662 # Required R-1.1 */
                callback_args.dynamic_address =
                    (uint8_t) ((dynamic_address_w >> R_RI3C0_SVDVAD0_SVAD_Pos) & (uint32_t) UINT8_MAX);
            }
        }
        else
        {
            /* Calculate the remaining number of bytes stored in the read FIFO. */
            uint32_t bytes_remaining = i3c_read_bytes_remaining_calculate(p_ctrl, data_length);

            /* Read the remaining byte stored in the FIFO. */
            i3c_fifo_read(p_ctrl, bytes_remaining);

            if ((uint8_t) I3C_CCC_DIRECT_GETACCMST == command_code)
            {
                if (0U != (p_ctrl->p_reg->PRSST & R_RI3C0_PRSST_CRMS_Msk)) /* PRQA S 0662 # Required R-1.1 */
                {
                    /* On success of the GETACCMST command, the driver will transition to master mode. */
                    p_ctrl->internal_state = (uint32_t) I3C_INTERNAL_STATE_MASTER_IDLE;

                    /*
                     * Configure IBI queue threshold so that an IBI IRQ is generated when there is 1 IBI status descriptor in the FIFO.
                     */
                    p_ctrl->p_reg->NQTHCTL_b.IBIQTH = 0;            /* PRQA S 0662 # Required R-1.1 */

                    /*
                     * In slave mode the IBI Queue Empty/Full flag indicates that the queue is empty. After transitioning to
                     * master mode, this bit indicates if there is status information in the queue. If the IBI queue was empty
                     * before transitioning to master mode, then it will be set and the status should be cleared.
                     */
                    p_ctrl->p_reg->NTST_b.IBIQEFF = 0;              /* PRQA S 0662 # Required R-1.1 */

                    /* Disable IBI transfer when the device is acting as the current master. */
                    p_ctrl->p_reg->SVDCT_b.TBCR &= (uint8_t) (~2U); /* PRQA S 0662 # Required R-1.1 */

                    i3c_internal_reset(p_ctrl);
                }

                /* Re-enable the IBI Status Buffer Full IRQ. */
                p_ctrl->p_reg->NTIE_b.IBIQEFIE = 1; /* PRQA S 0662 # Required R-1.1 */
            }

            if (p_ctrl->read_buffer_descriptor.count > 0U)
            {
                /* Reset the state for read transfers if data was read. */
                p_ctrl->read_buffer_descriptor = (i3c_read_buffer_descriptor_t) {
                    0
                };
            }

            callback_args.event         = I3C_EVENT_COMMAND_COMPLETE;
            callback_args.command_code  = command_code;
            callback_args.transfer_size = data_length;
        }
    }

    /* Reset the total bytes read. */
    p_ctrl->read_transfer_count_final = 0;

    uint32_t ntst = p_ctrl->p_reg->NTST;                                                                          /* PRQA S 0662 # Required R-1.1 */

    /* If an error occurred during the transfer, perform the error recovery operation */
    if (((0U != (ntst & (R_RI3C0_NTST_TEF_Msk | R_RI3C0_NTST_TABTF_Msk))) &&
         (0U == p_ctrl->p_reg->NRSQSTLV_b.RSQLV)) || (error_recovery_type == I3C_SLAVE_ERROR_RECOVERY_TYPE_READ)) /* PRQA S 0662 # Required R-1.1 */
    {
        if ((uint32_t) I3C_INTERNAL_STATE_SLAVE_IDLE == p_ctrl->internal_state)
        {
            i3c_slave_error_recovery(p_ctrl, error_recovery_type);
        }
    }

    /* Clear error status flags. */
    p_ctrl->p_reg->NTST &= ~(R_RI3C0_NTST_TEF_Msk | R_RI3C0_NTST_TABTF_Msk); /* PRQA S 0662 # Required R-1.1 */

    /* Notify the application of the event. */
    p_ctrl->p_cfg->p_callback(&callback_args);
#endif

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR for writing to the IBI Data Queue in slave mode, and reading from the IBI Status Queue in master mode.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void i3c_ibi_isr (void) /* PRQA S 1503 # Published API */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    i3c_instance_ctrl_t * p_ctrl = i3c_isr_context_get(); /* PRQA S 0317 # Advisory R-11.5 */
#if I3C_CFG_SLAVE_SUPPORT
    if ((uint32_t) I3C_INTERNAL_STATE_SLAVE_IBI == p_ctrl->internal_state)
    {
        /* Write the data to the IBI queue. */
        p_ctrl->p_reg->NIBIQP = p_ctrl->ibi_next_word; /* PRQA S 0662 # Required R-1.1 */

        /* Clear the IBI Queue Empty/Full Flag. */
        p_ctrl->p_reg->NTST_b.IBIQEFF = 0;             /* PRQA S 0662 # Required R-1.1 */

        if (p_ctrl->ibi_buffer_descriptor.count >= p_ctrl->ibi_buffer_descriptor.buffer_size)
        {
            /* If there is no more data to write, then disable the IBI IRQ. */
            p_ctrl->p_reg->NTIE_b.IBIQEFIE = 0;                                             /* PRQA S 0662 # Required R-1.1 */
            i3c_extended_cfg_t * p_extend = (i3c_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Advisory R-11.8, Advisory R-11.5, Advisory R-8.13 */
            R_BSP_IrqStatusClear(p_extend->ibi_irq);
        }
        else
        {
            /* Calculate the next word of data that will be written to the IBI FIFO. */
            p_ctrl->ibi_next_word = i3c_next_data_word_calculate(
                (i3c_write_buffer_descriptor_t *) &p_ctrl->ibi_buffer_descriptor); /* PRQA S 0310 # Advisory R-11.4 */
        }
    }
    else
#endif
    {
#if I3C_CFG_MASTER_SUPPORT

        /* Read the IBI status descriptor. */
        uint32_t ibi_status = p_ctrl->p_reg->NIBIQP; /* PRQA S 0662 # Required R-1.1 */

        /* Get the number of bytes following the status descriptor. */
        uint32_t ibi_data_length = (ibi_status & I3C_IBI_STATUS_DESC_LENGTH_Msk) >> I3C_IBI_STATUS_DESC_LENGTH_Pos;

        /* Update the total number of bytes read during the IBI transfer. */
        p_ctrl->ibi_transfer_count_final += ibi_data_length;

        /* Calculate the number of words that need to be read from the IBI Queue. */
        uint32_t ibi_data_length_w = (ibi_data_length + sizeof(uint32_t) - 1U) / sizeof(uint32_t);

        for (uint32_t i = 0; i < ibi_data_length_w; i++)
        {
            /* Get the next word of data from the IBI Queue. */
            uint32_t read_data = p_ctrl->p_reg->NIBIQP; /* PRQA S 0662 # Required R-1.1 */

            /*
             * Store the next word of data into the read buffer. If there is not enough space, a I3C_EVENT_IBI_READ_BUFFER_FULL
             * event will be called to notify the application that a new read buffer is required.
             */
            i3c_read_buffer_store(p_ctrl,
                                  &p_ctrl->ibi_buffer_descriptor,
                                  read_data,
                                  ibi_data_length,
                                  I3C_EVENT_IBI_READ_BUFFER_FULL);
            ibi_data_length -= 4U;
        }

        /* Clear the IBI Queue Empty/Full Flag. */
        p_ctrl->p_reg->NTST_b.IBIQEFF = 0; /* PRQA S 0662 # Required R-1.1 */

        /* If this is the last IBI status descriptor, then the IBI transfer is completed. */
        if (0U != (I3C_IBI_STATUS_DESC_LAST_STATUS_Msk & ibi_status))
        {
            /* Get the address of the IBI transfer. */
            uint8_t ibi_address = (uint8_t) ((ibi_status & I3C_IBI_STATUS_DESC_IBI_ID_Msk) >>
                                             (I3C_IBI_STATUS_DESC_IBI_ID_Pos + 1U));

            /* Get the RNW bit of the IBI transfer. */
            uint8_t ibi_rnw = (uint8_t) ((ibi_status >> I3C_IBI_STATUS_DESC_IBI_ID_Pos) & 1U);

            uint32_t err_status =
                (ibi_status & (I3C_IBI_STATUS_DESC_IBI_ST_Msk | I3C_IBI_STATUS_DESC_ERR_STATUS_Msk)) >>
                I3C_IBI_STATUS_DESC_ERR_STATUS_Pos;

            /* Read the volatile transfer-count field once into a local variable so it is not accessed again
             * (with a potential side effect) from within the designated initializer below. */
            uint32_t            ibi_transfer_size = p_ctrl->ibi_transfer_count_final;
            i3c_callback_args_t callback_args     =
            {
                .event         = I3C_EVENT_IBI_READ_COMPLETE,
                .event_status  = err_status,
                .ibi_type      = I3C_IBI_TYPE_INTERRUPT,
                .ibi_address   = ibi_address,
                .transfer_size = ibi_transfer_size,
                .p_context     = p_ctrl->p_cfg->p_context
            };

            if (I3C_HOT_JOIN_ADDRESS == ibi_address)
            {
                callback_args.ibi_type = I3C_IBI_TYPE_HOT_JOIN;

                /* Refer to the 'Hot-Join flowchart' in the Hardware User Manual, after the I3C bus has already been configured.*/
                /* Wait for the STOP condition before completing the Hot-Join.*/
                FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->BST_b.SPCNDDF, 1U); /* PRQA S 0662 # Required R-1.1 */
            }
            else if (0U == ibi_rnw)
            {
                callback_args.ibi_type = I3C_IBI_TYPE_MASTERSHIP_REQUEST;
            }
            else
            {
                /* IBI type is I3C_IBI_TYPE_INTERRUPT. */
            }

            p_ctrl->ibi_buffer_descriptor = (i3c_read_buffer_descriptor_t) {
                0
            };
            p_ctrl->ibi_transfer_count_final = 0;

            /* Notify the application of the event. */
            p_ctrl->p_cfg->p_callback(&callback_args);
        }
#endif
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR for providing the following events to the application:
 * - I3C_EVENT_INTERNAL_ERROR: An internal error can occur if too many transfers occur sequentially and overflow
 *                             the Receive Status Queue.
 * - I3C_EVENT_TIMEOUT_DETECTED
 * - I3C_EVENT_HDR_EXIT_PATTERN_DETECTED
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void i3c_eei_isr (void) /* PRQA S 1503 # Published API */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    i3c_instance_ctrl_t * p_ctrl = i3c_isr_context_get(); /* PRQA S 0317, 3678 # Advisory R-11.5, Advisory R-8.13 */

    i3c_callback_args_t callback_args;
    callback_args.p_context = p_ctrl->p_cfg->p_context;

    /* Read the Internal Status Register. */
    uint32_t inst            = p_ctrl->p_reg->INST; /* PRQA S 0662 # Required R-1.1 */
    uint32_t inst_clear_mask = 0;

    /* Read the Bus Status Register. */
    uint32_t bst            = p_ctrl->p_reg->BST;   /* PRQA S 0662 # Required R-1.1 */
    uint32_t bst_clear_mask = 0;

    if (0U != (inst & R_RI3C0_INST_INEF_Msk))
    {
        /* Notify the application of the internal error. */
        callback_args.event = I3C_EVENT_INTERNAL_ERROR;
        p_ctrl->p_cfg->p_callback(&callback_args);

        inst_clear_mask |= R_RI3C0_INST_INEF_Msk;

        /* Resume I3C operation. */
        p_ctrl->p_reg->BCTL_b.RSM = 1; /* PRQA S 0662 # Required R-1.1 */
    }

    /* Check if a timeout occurred. */
    if (0U != (bst & R_RI3C0_BST_TODF_Msk))
    {
        /* Notify that application that the timeout was detected. */
        callback_args.event = I3C_EVENT_TIMEOUT_DETECTED;
        p_ctrl->p_cfg->p_callback(&callback_args);

        bst_clear_mask |= R_RI3C0_BST_TODF_Msk;
    }

#if BSP_FEATURE_I3C_HAS_HDR

    /* Check if the HDR Exit Pattern was detected. */
    if (0U != (bst & R_RI3C0_BST_HDREXDF_Msk))
    {
        /* Notify that application that the HDR Exit pattern was detected. */
        callback_args.event = I3C_EVENT_HDR_EXIT_PATTERN_DETECTED;
        p_ctrl->p_cfg->p_callback(&callback_args);

        bst_clear_mask |= R_RI3C0_BST_HDREXDF_Msk;
    }
#endif

    /* Clear the status flags that have been handled. */
    p_ctrl->p_reg->INST = p_ctrl->p_reg->INST & (~inst_clear_mask); /* PRQA S 0662 # Required R-1.1 */
    p_ctrl->p_reg->BST  = p_ctrl->p_reg->BST & (~bst_clear_mask);   /* PRQA S 0662 # Required R-1.1 */

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

#if I3C_CFG_MASTER_SUPPORT

/*******************************************************************************************************************//**
 * Perform master error recovery
 **********************************************************************************************************************/
static void i3c_master_error_recovery (i3c_instance_ctrl_t * p_ctrl, bool error_recovery_case_2) /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Local copy of the "error_recovery_case_2" argument so the formal parameter itself is not modified. */
    bool is_error_recovery_case_2 = error_recovery_case_2;

    /* Flush the Command, Rx and Tx Buffers. */
    p_ctrl->p_reg->RSTCTL = I3C_RSTCTRL_FIFO_FLUSH_Msk;                                   /* PRQA S 0662 # Required R-1.1 */

    /* The field will be cleared automatically upon reset completion. */
    FSP_HARDWARE_REGISTER_WAIT((p_ctrl->p_reg->RSTCTL & I3C_RSTCTRL_FIFO_FLUSH_Msk), 0U); /* PRQA S 0662 # Required R-1.1 */

    /* Wait for the bus available condition. */
    while (1U != p_ctrl->p_reg->BCST_b.BAVLF)                                             /* PRQA S 0662 # Required R-1.1 */
    {
        /* If SDA is pulled low, then a slave device started an IBI during error recovery. */
        if (0U == p_ctrl->p_reg->PRSTDBG_b.SDILV)                                         /* PRQA S 0662 # Required R-1.1 */
        {
            is_error_recovery_case_2 = true;
            break;
        }
    }

    if (is_error_recovery_case_2)
    {
        /* Disable the IBI Status Buffer Full IRQ. */
        p_ctrl->p_reg->NTIE_b.IBIQEFIE = 0;                                                     /* PRQA S 0662 # Required R-1.1 */

        /* When recovering from a read operation where the transfer length is less than expected, perform internal
         * software reset. */
        p_ctrl->p_reg->RSTCTL = R_RI3C0_RSTCTL_INTLRST_Msk;                                     /* PRQA S 1257 # Advisory R-10.6 */ /* PRQA S 0662 # Required R-1.1 */
        p_ctrl->p_reg->RSTCTL = 0;                                                              /* PRQA S 0662 # Required R-1.1 */

        /* Restore the current master setting. */
        p_ctrl->p_reg->PRSST = (uint32_t) (R_RI3C0_PRSST_CRMS_Msk | R_RI3C0_PRSST_PRSSTWP_Msk); /* PRQA S 0662 # Required R-1.1 */

        /* After an internal reset, the CRMS bit is cleared which causes the IBI Queue Empty/Full Flag to be set indicating the queue is empty.
         * Since the driver is in master mode, this status should be discarded and the flag should be cleared. */
        p_ctrl->p_reg->NTST_b.IBIQEFF = 0;                                                      /* PRQA S 0662 # Required R-1.1 */

        /* Enable the IBI Status Buffer Full IRQ. */
        p_ctrl->p_reg->NTIE_b.IBIQEFIE = 1;                                                     /* PRQA S 0662 # Required R-1.1 */
    }

    /* Resume I3C operation. */
    p_ctrl->p_reg->BCTL_b.RSM = 1;                                                              /* PRQA S 0662 # Required R-1.1 */
}

#endif
