/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_iic_master.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "IIC" in ASCII, used to determine if channel is open. */
#define IIC_MASTER_OPEN                             (0x52494943U)

#define I2C_CODE_10BIT                              (0xF0U)
#define IIC_MASTER_SLAVE_10_BIT_ADDR_LEN_ADJUST     (2U)
#define I2C_CODE_READ                               (0x01U)
#define IIC_MASTER_BUS_MODE_REGISTER_2_MASK         (0x04U)
#define IIC_MASTER_INTERRUPT_ENABLE_INIT_MASK       (0xB3U)
#define IIC_MASTER_ICCR2_ST_BIT_MASK                (0x02)
#define IIC_MASTER_BUS_MODE_REGISTER_1_MASK         (0x08U)
#define IIC_MASTER_INTERNAL_REF_CLOCK_SELECT_MAX    (0x07U)
#define IIC_MASTER_ICCR2_RS_BIT_MASK                (0x04)
#define IIC_MASTER_ICCR2_SP_BIT_MASK                (0x08)
#define IIC_MASTER_ICCFER_FMPE_BIT_MASK             (0x80)
#define IIC_MASTER_TRANSFER_RESET_SRC_DEST_NUM      (1U)

/* Bit position for STOP condition flag in ICSR2 */
#define IIC_MASTER_ICSR2_STOP_BIT                   (0x08U)

#define IIC_MASTER_HARDWARE_REGISTER_WAIT(reg, required_value, timeout) \
    while ((timeout))                                                   \
    {                                                                   \
        if ((required_value) == (reg))                                  \
        {                                                               \
            break;                                                      \
        }                                                               \
        (timeout)--;                                                    \
    }

/* IIC read/write enumeration */

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_iic_master_transfer_dir_option
{
    IIC_MASTER_TRANSFER_DIR_WRITE = 0x0,
    IIC_MASTER_TRANSFER_DIR_READ  = 0x1
} iic_master_transfer_dir_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if IIC_MASTER_CFG_TRANSFER_ENABLE
void             iic_master_txi_transfer_callback(transfer_callback_args_t * p_args);
void             iic_master_rxi_transfer_callback(transfer_callback_args_t * p_args);
static fsp_err_t iic_master_transfer_open(iic_master_instance_ctrl_t * const p_ctrl,
                                          i2c_master_cfg_t const * const     p_cfg);

#endif
BSP_INTERRUPT_ATTRIBUTE void iic_master_rxi_isr(void);
BSP_INTERRUPT_ATTRIBUTE void iic_master_txi_isr(void);
BSP_INTERRUPT_ATTRIBUTE void iic_master_tei_isr(void);
BSP_INTERRUPT_ATTRIBUTE void iic_master_eri_isr(void);
void                         iic_interrupt_handling_master(IRQn_Type const irq);

/* Interrupt handlers */
static void iic_master_rxi_master(iic_master_instance_ctrl_t * p_ctrl);
static void iic_master_txi_master(iic_master_instance_ctrl_t * p_ctrl);
static void iic_master_tei_master(iic_master_instance_ctrl_t * p_ctrl);
static void iic_master_naki_master(iic_master_instance_ctrl_t * p_ctrl);
static void iic_master_spi_master(iic_master_instance_ctrl_t * p_ctrl);
static void iic_master_sti_master(iic_master_instance_ctrl_t * p_ctrl);
static void iic_master_ali_master(iic_master_instance_ctrl_t * p_ctrl);
static void iic_master_tmoi_master(iic_master_instance_ctrl_t * p_ctrl);

/* Internal helper functions */
static fsp_err_t iic_master_read_write(i2c_master_ctrl_t * const p_api_ctrl,
                                       uint8_t * const           p_buffer,
                                       uint32_t const            bytes,
                                       iic_master_transfer_dir_t direction);

static void iic_master_abort_seq_master(iic_master_instance_ctrl_t * const p_ctrl, bool iic_reset);
static void iic_master_notify(iic_master_instance_ctrl_t * const p_ctrl, i2c_master_event_t const event);

/* Functions that manipulate hardware */
static void iic_master_open_hw_master(iic_master_instance_ctrl_t * const p_ctrl,
                                      i2c_master_cfg_t const * const     p_cfg);
static fsp_err_t iic_master_run_hw_master(iic_master_instance_ctrl_t * const p_ctrl);
static void      iic_master_rxi_read_data(iic_master_instance_ctrl_t * const p_ctrl);
static void      iic_master_txi_send_address(iic_master_instance_ctrl_t * const p_ctrl);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* IIC Implementation of I2C device master interface */
i2c_master_api_t const g_i2c_master_on_iic =
{
    .open            = R_IIC_MASTER_Open,
    .read            = R_IIC_MASTER_Read,
    .close           = R_IIC_MASTER_Close,
    .abort           = R_IIC_MASTER_Abort,
    .slaveAddressSet = R_IIC_MASTER_SlaveAddressSet,
    .write           = R_IIC_MASTER_Write,
    .statusGet       = R_IIC_MASTER_StatusGet,
    .callbackSet     = R_IIC_MASTER_CallbackSet
};

/*******************************************************************************************************************//**
 * @addtogroup IIC_MASTER
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Opens the I2C device.
 *
 * @retval  FSP_SUCCESS                       Requested clock rate was set exactly.
 * @retval  FSP_ERR_ALREADY_OPEN              Module is already open.
 * @retval  FSP_ERR_IP_CHANNEL_NOT_PRESENT    Channel is not available on this MCU.
 * @retval  FSP_ERR_ASSERTION                 Parameter check failure due to one or more reasons below:
 *                                            1. p_api_ctrl or p_cfg is NULL.
 *                                            2. extended parameter is NULL.
 *                                            3. Callback parameter is NULL.
 *                                            4. Set the rate to fast mode plus on a channel which does not support it.
 *                                            5. Invalid IRQ number assigned
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_Open (i2c_master_ctrl_t * const p_api_ctrl, i2c_master_cfg_t const * const p_cfg)
{
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) p_api_ctrl;

#if IIC_MASTER_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_api_ctrl != NULL);
    FSP_ASSERT(p_cfg != NULL);
    FSP_ASSERT(p_cfg->p_extend != NULL);
    FSP_ASSERT((p_cfg->rxi_irq >= (IRQn_Type) 0) && (FSP_INVALID_VECTOR != p_cfg->rxi_irq));
    FSP_ASSERT((p_cfg->txi_irq >= (IRQn_Type) 0) && (FSP_INVALID_VECTOR != p_cfg->txi_irq));
    FSP_ASSERT((p_cfg->tei_irq >= (IRQn_Type) 0) && (FSP_INVALID_VECTOR != p_cfg->tei_irq));
    FSP_ASSERT((p_cfg->eri_irq >= (IRQn_Type) 0) && (FSP_INVALID_VECTOR != p_cfg->eri_irq));
    FSP_ERROR_RETURN(IIC_MASTER_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);

    FSP_ERROR_RETURN(BSP_FEATURE_IIC_VALID_CHANNEL_MASK & (1 << p_cfg->channel), FSP_ERR_IP_CHANNEL_NOT_PRESENT);
#endif

    /* Save register base address. */
    p_ctrl->p_reg =
        (R_RIIC0_Type *) ((uintptr_t) R_RIIC0 + (p_cfg->channel * ((uintptr_t) R_RIIC1 - (uintptr_t) R_RIIC0)));

    /* Record the pointer to the configuration structure for later use */
    p_ctrl->p_cfg             = p_cfg;
    p_ctrl->slave             = p_cfg->slave;
    p_ctrl->addr_mode         = p_cfg->addr_mode;
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    // R_BSP_MODULE_START(FSP_IP_IIC, p_cfg->channel);

    /* Open the hardware in master mode. */
    iic_master_open_hw_master(p_ctrl, p_cfg);
#if IIC_MASTER_CFG_TRANSFER_ENABLE

    /* Open the IIC transfer interface if available */
    fsp_err_t err = iic_master_transfer_open(p_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif
    p_ctrl->p_buff    = NULL;
    p_ctrl->total     = 0U;
    p_ctrl->remain    = 0U;
    p_ctrl->loaded    = 0U;
    p_ctrl->read      = false;
    p_ctrl->restart   = false;
    p_ctrl->err       = false;
    p_ctrl->restarted = false;
    p_ctrl->open      = IIC_MASTER_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Performs a write to the I2C device.
 * The caller will be notified when the operation has completed (successfully) by an
 * I2C_MASTER_EVENT_TX_COMPLETE in the callback.
 *
 * @retval  FSP_SUCCESS           Function executed without issue.
 * @retval  FSP_ERR_ASSERTION     p_api_ctrl or p_src is NULL.
 * @retval  FSP_ERR_IN_USE        Bus busy condition. Another transfer was in progress.
 * @retval  FSP_ERR_NOT_OPEN      Handle is not initialized.  Call R_IIC_MASTER_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_Write (i2c_master_ctrl_t * const p_api_ctrl,
                              uint8_t * const           p_src,
                              uint32_t const            bytes,
                              bool const                restart)
{
#if IIC_MASTER_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_api_ctrl != NULL);
#endif
    fsp_err_t err = FSP_SUCCESS;

    /* Record the restart information about this transfer.
     * This is done here to keep the parameter (argument) list of iic_master_read_write to 4. */
    ((iic_master_instance_ctrl_t *) p_api_ctrl)->restart = restart;

    /* Call the common helper function to perform I2C Write operation.*/
    err = iic_master_read_write(p_api_ctrl, p_src, bytes, IIC_MASTER_TRANSFER_DIR_WRITE);

    return err;
}

/*******************************************************************************************************************//**
 * Performs a read from the I2C device.
 * The caller will be notified when the operation has completed (successfully) by an
 * I2C_MASTER_EVENT_RX_COMPLETE in the callback.
 *
 * @retval  FSP_SUCCESS             Function executed without issue.
 * @retval  FSP_ERR_ASSERTION       p_api_ctrl, p_dest or bytes is NULL.
 * @retval  FSP_ERR_IN_USE          Bus busy condition. Another transfer was in progress.
 * @retval  FSP_ERR_NOT_OPEN        Handle is not initialized.  Call R_IIC_MASTER_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_Read (i2c_master_ctrl_t * const p_api_ctrl,
                             uint8_t * const           p_dest,
                             uint32_t const            bytes,
                             bool const                restart)
{
#if IIC_MASTER_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_api_ctrl != NULL);
    FSP_ASSERT(bytes != 0U);
#endif
    fsp_err_t err = FSP_SUCCESS;

    /* Record the restart information about this transfer.
     * This is done here to keep the parameter (argument) list of iic_master_read_write to 4. */
    ((iic_master_instance_ctrl_t *) p_api_ctrl)->restart = restart;

    /* Call the common helper function to perform I2C Read operation.*/
    err = iic_master_read_write(p_api_ctrl, p_dest, bytes, IIC_MASTER_TRANSFER_DIR_READ);

    return err;
}

/*******************************************************************************************************************//**
 * Safely aborts any in-progress transfer and forces the IIC peripheral into ready state.
 *
 *
 * @retval  FSP_SUCCESS             Channel was reset successfully.
 * @retval  FSP_ERR_ASSERTION       p_api_ctrl is NULL.
 * @retval  FSP_ERR_NOT_OPEN        Handle is not initialized.  Call R_IIC_MASTER_Open to initialize the control block.
 *
 * @note A callback will not be invoked in case an in-progress transfer gets aborted by calling this API.
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_Abort (i2c_master_ctrl_t * const p_api_ctrl)
{
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) p_api_ctrl;

#if IIC_MASTER_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ERROR_RETURN(IIC_MASTER_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Abort any transfer happening on the channel */
    iic_master_abort_seq_master(p_ctrl, true);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Sets address and addressing mode of the slave device.
 * This function is used to set the device address and addressing mode of the slave
 * without reconfiguring the entire bus.
 *
 * @retval  FSP_SUCCESS             Address of the slave is set correctly.
 * @retval  FSP_ERR_ASSERTION       Pointer to control structure is NULL.
 * @retval  FSP_ERR_IN_USE          Another transfer was in-progress.
 * @retval  FSP_ERR_NOT_OPEN        Handle is not initialized.  Call R_IIC_MASTER_Open to initialize the control block.
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_SlaveAddressSet (i2c_master_ctrl_t * const    p_api_ctrl,
                                        uint32_t const               slave,
                                        i2c_master_addr_mode_t const addr_mode)
{
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) p_api_ctrl;

    fsp_err_t err = FSP_SUCCESS;

#if IIC_MASTER_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ERROR_RETURN(IIC_MASTER_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Fail if there is already a transfer in progress */
    FSP_ERROR_RETURN(((0 == p_ctrl->remain) && (false == p_ctrl->restart)), FSP_ERR_IN_USE);
#endif

    /* Sets the address of the slave device */
    p_ctrl->slave = slave;

    /* Sets the mode of addressing */
    p_ctrl->addr_mode = addr_mode;

    return err;
}

/*******************************************************************************************************************//**
 * Provides driver status.
 *
 * @retval     FSP_SUCCESS                   Status stored in p_status.
 * @retval     FSP_ERR_ASSERTION             NULL pointer.
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_StatusGet (i2c_master_ctrl_t * const p_api_ctrl, i2c_master_status_t * p_status)
{
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) p_api_ctrl;

#if IIC_MASTER_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ASSERT(p_status != NULL);
#endif

    p_status->open = (IIC_MASTER_OPEN == p_ctrl->open);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements i2c_master_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_CallbackSet (i2c_master_ctrl_t * const          p_api_ctrl,
                                    void (                           * p_callback)(i2c_master_callback_args_t *),
                                    void * const                       p_context,
                                    i2c_master_callback_args_t * const p_callback_memory)
{
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) p_api_ctrl;

#if (IIC_MASTER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(IIC_MASTER_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Closes the I2C device. May power down IIC peripheral.
 * This function will safely terminate any in-progress I2C transfers.
 *
 * @retval  FSP_SUCCESS         Device closed without issue.
 * @retval  FSP_ERR_ASSERTION   p_api_ctrl is NULL.
 * @retval  FSP_ERR_NOT_OPEN    Handle is not initialized.  Call R_IIC_MASTER_Open to initialize the control block.
 *
 * @note A callback will not be invoked in case an in-progress transfer gets aborted by calling this API.
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_Close (i2c_master_ctrl_t * const p_api_ctrl)
{
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) p_api_ctrl;
    fsp_err_t err = FSP_SUCCESS;
#if IIC_MASTER_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ASSERT(p_ctrl->p_cfg != NULL);
    FSP_ERROR_RETURN(IIC_MASTER_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Abort an in-progress transfer with this device only */
    iic_master_abort_seq_master(p_ctrl, true);

    /* Disable I2C interrupts. Described in hardware manual (see section
     * 'I2C Bus Interrupt Enable Register (ICIER)' of the user's manual). */
    p_ctrl->p_reg->IER = (uint32_t) 0x00000000UL;

    /* The device is considered closed */
    p_ctrl->open = 0U;
#if IIC_MASTER_CFG_TRANSFER_ENABLE

    /* Close the handles for the transfer interfaces */
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        err = p_ctrl->p_cfg->p_transfer_rx->p_api->close(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->close(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    R_BSP_IrqDisable(p_ctrl->p_cfg->eri_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->rxi_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->tei_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);

    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup IIC_MASTER)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Performs the hardware initialization sequence when operating as a master.
 * (see section 'Initial Settings' of the user's manual).
 *
 * @param[in]  p_ctrl                Pointer to IIC specific control structure
 * @param[in]  p_cfg                 Pointer to IIC specific configuration structure.
 **********************************************************************************************************************/
static void iic_master_open_hw_master (iic_master_instance_ctrl_t * const p_ctrl, i2c_master_cfg_t const * const p_cfg)
{
    iic_master_extended_cfg_t * p_extend = (iic_master_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

    /* Perform IIC reset */
    p_ctrl->p_reg->CR1 = (uint32_t) IIC_MASTER_PRV_SCL_SDA_NOT_DRIVEN;

    /* Reset */
    p_ctrl->p_reg->CR1 =
        (uint32_t) (IIC_MASTER_ICCR1_IICRST_BIT_MASK | IIC_MASTER_PRV_SCL_SDA_NOT_DRIVEN);

    /* Come out of IIC reset to internal reset */
    p_ctrl->p_reg->CR1 =
        (uint32_t) (IIC_MASTER_ICCR1_ICE_BIT_MASK | IIC_MASTER_ICCR1_IICRST_BIT_MASK |
                    IIC_MASTER_PRV_SCL_SDA_NOT_DRIVEN);

    /* Configure the clock settings. This is set in the configuration structure by the tooling. */
    /* Set the number of counts that the clock remains low, bit 7 to 5 should be written as 1 */
    p_ctrl->p_reg->BRL =
        (uint32_t) (IIC_MASTER_BUS_RATE_REG_RESERVED_BITS |
                    p_extend->clock_settings.brl_value);

    /* Set the number of counts that the clock remains high, bit 7 to 5 should be written as 1 */
    p_ctrl->p_reg->BRH = (uint32_t) (IIC_MASTER_BUS_RATE_REG_RESERVED_BITS |
                                     p_extend->clock_settings.brh_value);

    /* Set the internal reference clock source for generating IIC clock */
    p_ctrl->p_reg->MR1 = (uint32_t) (IIC_MASTER_BUS_MODE_REGISTER_1_MASK |
                                     (uint8_t) ((p_extend->
                                                 clock_settings.cks_value &
                                                 IIC_MASTER_INTERNAL_REF_CLOCK_SELECT_MAX) << 4U));

    /* Allow timeouts to be generated on the low value of SCL using either long or short mode */

    /* TMOL 'Timeout L Count Control' and TMOH 'Timeout H Count Control' will be set at the time of I2C reset.
     * This will enable time out detection for both SCLn high and low.
     * Only Set/Clear TMOS here to select long or short mode.
     * (see section 'I2C Bus Mode Register 2 (ICMR2)' of the user's manual).
     */
    p_ctrl->p_reg->MR2 = (uint32_t) (IIC_MASTER_BUS_MODE_REGISTER_2_MASK |
                                     (uint8_t) (IIC_MASTER_TIMEOUT_MODE_SHORT == p_extend->timeout_mode)
                                     |
                                     (uint8_t) (((iic_master_extended_cfg_t *) p_ctrl->p_cfg->p_extend)->
                                                timeout_scl_low << 1)
                                     |
                                     (uint8_t) (((iic_master_extended_cfg_t *) p_ctrl->p_cfg->p_extend)->
                                                timeout_scl_high << 2));

    /* Set the digital filter.*/

    /* ICFER Register Settings:
     * 1. Enable timeout function.
     * 2. Enable master arbitration loss detection.
     * 3. Enable NACK arbitration loss detection.
     * 4. Disable Slave arbitration loss detection.
     * 5. Enable NACK reception transfer suspension.
     * 6. Use the digital noise filter circuit.
     * 7. Use the SCL synchronous circuit.
     * (see section 'I2C Bus Function Enable Register' of the user's manual).
     */
    if (p_extend->noise_filter_stage > 0)
    {
        p_ctrl->p_reg->MR3 |= p_extend->noise_filter_stage - 1;
        p_ctrl->p_reg->FER  = (uint32_t) (IIC_MASTER_FUNCTION_ENABLE_INIT_SETTINGS);
    }
    else
    {
        p_ctrl->p_reg->MR3_b.NF = 0;
        p_ctrl->p_reg->FER      = (uint32_t) (IIC_MASTER_FUNCTION_ENABLE_INIT_SETTINGS) &
                                  (~R_RIIC0_FER_NFE_Msk);
    }

#if BSP_FEATURE_IIC_HAS_FMPE

    /* ICFER Register Settings:
     * 8. Enable FM+ slope circuit if fast mode plus is enabled.
     */
    if (I2C_MASTER_RATE_FASTPLUS == p_ctrl->p_cfg->rate)
    {
        p_ctrl->p_reg->FER |= (uint8_t) IIC_MASTER_ICCFER_FMPE_BIT_MASK;
    }
#endif

    /* Ensure the HW is in master mode and does not behave as a slave to another master on the same channel. */
    p_ctrl->p_reg->SER = (uint32_t) 0x00000000UL;

    /* Enable Interrupts: TMOIE, ALIE, NAKIE, RIE, TIE.
     * Disable Interrupt: TEIE, STIE, SPIE
     * (see section 'I2C Bus Interrupt Enable Register (ICIER)' of the user's manual).
     */
    p_ctrl->p_reg->IER = (uint32_t) IIC_MASTER_INTERRUPT_ENABLE_INIT_MASK;

    /* Set valid interrupt contexts and user provided priority. Enable the interrupts at the GIC  */
    R_BSP_IrqCfgEnable(p_cfg->eri_irq, p_cfg->ipl, p_ctrl);
    R_BSP_IrqCfgEnable(p_cfg->txi_irq, p_cfg->ipl, p_ctrl);
    R_BSP_IrqCfgEnable(p_cfg->tei_irq, p_cfg->ipl, p_ctrl);
    R_BSP_IrqCfgEnable(p_cfg->rxi_irq, p_cfg->ipl, p_ctrl);

    /* Release IIC from internal reset */

    /* Reset */
    p_ctrl->p_reg->CR1 = (uint32_t) (IIC_MASTER_ICCR1_ICE_BIT_MASK | IIC_MASTER_PRV_SCL_SDA_NOT_DRIVEN);
}

/*******************************************************************************************************************//**
 * Single point for managing the logic around aborting a transfer when operating as a master.
 *
 * @param[in]       p_ctrl  Pointer to control structure of specific device.
 * @param[in]       iic_reset  Flag to enable IIC reset
 **********************************************************************************************************************/
static void iic_master_abort_seq_master (iic_master_instance_ctrl_t * const p_ctrl, bool iic_reset)
{
    /* Safely stop the hardware from operating. */
    /* Reset the peripheral */
    if (true == iic_reset)
    {
        /* Disable channel interrupts */
        p_ctrl->p_reg->IER = (uint32_t) 0x00000000UL;

        /* This helper function would do a full IIC reset
         * followed by re-initializing the required peripheral registers. */
        iic_master_open_hw_master(p_ctrl, p_ctrl->p_cfg);
    }

    /* Update the transfer descriptor to show no longer in-progress and an error */
    p_ctrl->remain = 0U;

    /* Update the transfer descriptor to make sure interrupts no longer process */
    p_ctrl->addr_loaded = p_ctrl->addr_total;
    p_ctrl->loaded      = p_ctrl->total;
    p_ctrl->restarted   = false;
    p_ctrl->restart     = false;

    /* Enable Interrupts: TMOIE, ALIE, NAKIE, RIE, TIE.
     * Disable Interrupt: TEIE, STIE, SPIE
     * (see section 'I2C Bus Interrupt Enable Register (ICIER)' of the user's manual).
     */
    p_ctrl->p_reg->IER = (uint32_t) IIC_MASTER_INTERRUPT_ENABLE_INIT_MASK;
}

/*******************************************************************************************************************//**
 * Helper function for handling I2C Read or Write.
 *
 * @param      p_api_ctrl      Pointer to control block
 * @param      p_buffer        Pointer to the buffer to store read/write data.
 * @param[in]  bytes           Number of bytes to be read/written.
 * @param[in]  direction       Read or Write
 *
 * @retval  FSP_SUCCESS           Function executed successfully.
 * @retval  FSP_ERR_ASSERTION     p_api_ctrl or p_buffer is NULL.
 * @retval  FSP_ERR_IN_USE        Another transfer was in progress.
 * @retval  FSP_ERR_NOT_OPEN      Handle is not initialized. Call R_IIC_MASTER_Open to initialize the control block.
 **********************************************************************************************************************/
static fsp_err_t iic_master_read_write (i2c_master_ctrl_t * const p_api_ctrl,
                                        uint8_t * const           p_buffer,
                                        uint32_t const            bytes,
                                        iic_master_transfer_dir_t direction)
{
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) p_api_ctrl;

#if IIC_MASTER_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_buffer != NULL);
    FSP_ERROR_RETURN((p_ctrl->open == IIC_MASTER_OPEN), FSP_ERR_NOT_OPEN);
#endif

    fsp_err_t err = FSP_SUCCESS;

    p_ctrl->p_buff           = p_buffer;
    p_ctrl->total            = bytes;
    p_ctrl->nack_before_stop = false;

    /* Handle the (different) addressing mode(s) */
    if (p_ctrl->addr_mode == I2C_MASTER_ADDR_MODE_7BIT)
    {
        /* Set the address bytes according to a 7-bit slave read command */
        p_ctrl->addr_high  = 0U;
        p_ctrl->addr_low   = (uint8_t) ((p_ctrl->slave << 1U) | (uint8_t) direction);
        p_ctrl->addr_total = 1U;
    }

#if IIC_MASTER_CFG_ADDR_MODE_10_BIT_ENABLE
    else
    {
        /* Set the address bytes according to a 10-bit slave read command */
        p_ctrl->addr_high = (uint8_t) (((p_ctrl->slave >> 7U) | I2C_CODE_10BIT) & (uint8_t) ~(I2C_CODE_READ));
        p_ctrl->addr_low  = (uint8_t) p_ctrl->slave;

        /* Addr total = 3 for Read and 2 for Write.
         * See section "Communication Data Format" of the user's manual.
         */
        p_ctrl->addr_total = (uint8_t) ((uint8_t) direction + IIC_MASTER_SLAVE_10_BIT_ADDR_LEN_ADJUST);
    }
#endif

    p_ctrl->read = (bool) direction;

    /* Kickoff the read operation as a master */
    err = iic_master_run_hw_master(p_ctrl);

    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Performs the data transfer described by the parameters when operating as a master.
 * See section "Master Transmit Operation" and "Master Receive Operation"
 * of the user's manual.
 * @param[in]       p_ctrl  Pointer to control structure of specific device.
 *
 * @retval  FSP_SUCCESS       Data transfer success.
 * @retval  FSP_ERR_IN_USE    If data transfer is in progress.
 **********************************************************************************************************************/
static fsp_err_t iic_master_run_hw_master (iic_master_instance_ctrl_t * const p_ctrl)
{
    /* IICn operates using PPLLCLK2. The ratio I2CLK (System Clock)/(PPLLCLK2/IIC) gives the CPU cycles for 1
     * ICBRL count.
     * IIC is division ratio of internal reference clock of IICn.
     * The internal reference clock (IIC_CLK) is calculated as PLLCLK2/IIC.
     * Since each time loop the timeout count will be decremented by 1 this would require at least 4 CPU clocks,
     * making the final timeout count as:
     * Timeout = ((I2CLK/(IIC_CLK))*ICBRL)/4.
     */
    uint32_t  ppll_clk2 = IIC_MASTER_CFG_CLOCK;
    uint32_t  i2_clk;
    fsp_err_t err = FSP_SUCCESS;

#if  (BSP_FEATURE_IIC_HAS_SYS_CLK == 1U)

    /* For U2Ax, IICn operates using CLK_SYS */
    i2_clk = BSP_CFG_CLOCK_CLK_SYS_HZ;
#else

    /* For U2Bx, IICn operates using CKS_CLEANC domain */
    if (1U == BSP_CFG_CLOCK_CKS_CLEANC_SRC)
    {
        i2_clk = BSP_CFG_CLOCK_CLK_IOSC_HZ;
    }
    else
    {
        i2_clk = BSP_CFG_CLOCK_CLK_PLL_HZ;
    }
#endif
    uint32_t iic_clk       = ppll_clk2 / (1U << p_ctrl->p_reg->MR1_b.CKS);
    uint32_t timeout_count = ((i2_clk / iic_clk) * p_ctrl->p_reg->BRL_b.BRL) >> 2U;

    /* Check if this is a new transaction or a continuation */
    if (!p_ctrl->restarted)
    {
        /* BBSY clearing conditions in section 'I2C Bus Control Register 2 (ICCR2)' of the user's manual,
         * the BBSY bit is 0 after the bus free time (ICBRL setting)
         * if a start condition is not detected after a stop condition detection.
         */
        IIC_MASTER_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CR2_b.BBSY, 0UL, timeout_count);

        /* If bus is busy, return error */
        FSP_ERROR_RETURN((0U != timeout_count), FSP_ERR_IN_USE);

        /* This is not a restarted transaction. Enable TXI for the next transfer.
         * This had been disabled at the end of TXI interrupt.
         * The intention is to only enable IIC_MASTER_TXI_EN_BIT.
         * Writing the whole mask - IIC_MASTER_INTERRUPT_ENABLE_INIT_MASK saves cycles.
         */
        p_ctrl->p_reg->IER = (uint32_t) IIC_MASTER_INTERRUPT_ENABLE_INIT_MASK;
    }

    /* Initialize fields used during transfer */
    p_ctrl->addr_loaded          = 0U;
    p_ctrl->loaded               = 0U;
    p_ctrl->remain               = p_ctrl->total;
    p_ctrl->addr_remain          = p_ctrl->addr_total;
    p_ctrl->err                  = false;
    p_ctrl->dummy_read_completed = false;
    p_ctrl->activation_on_rxi    = false;
    p_ctrl->activation_on_txi    = false;
    p_ctrl->address_restarted    = false;

    /* Allow timeouts to be generated on the low value of SCL using either short or long mode.
     * This gets disabled in case the previous transaction issues a restart.
     */

    /* TMOL 'Timeout L Count Control' and TMOH 'Timeout H Count Control' will be set at the time of I2C reset.
     * This will enable time out detection for both SCLn high and low.
     * Only Set/Clear TMOS here to select long or short mode.
     * (see section 'I2C Bus Mode Register 2 (ICMR2)' of the user's manual).
     */
    p_ctrl->p_reg->MR2 = (uint32_t) (IIC_MASTER_BUS_MODE_REGISTER_2_MASK |
                                     (uint8_t) (IIC_MASTER_TIMEOUT_MODE_SHORT == p_ctrl->timeout_mode)
                                     |
                                     (uint8_t) (((iic_master_extended_cfg_t *) p_ctrl->p_cfg->p_extend)->
                                                timeout_scl_low << 1)
                                     |
                                     (uint8_t) (((iic_master_extended_cfg_t *) p_ctrl->p_cfg->p_extend)->
                                                timeout_scl_high << 2));

    /* Enable timeout function */
    p_ctrl->p_reg->FER_b.TMOE = 1UL;

#if IIC_MASTER_CFG_TRANSFER_ENABLE
    if (p_ctrl->p_cfg->p_transfer_tx != NULL)
    {
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = IIC_MASTER_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);

        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (p_ctrl->p_cfg->p_transfer_rx != NULL)
    {
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = IIC_MASTER_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    /* Enable TXI. This is treated differently to support restart functionality.
     * In case the previous IIC master transaction enabled restart, the queued TXI will fire a this point.
     */

    iic_interrupt_handling_master(p_ctrl->p_cfg->txi_irq);

    /* Enable SPIE to detect unexpected STOP condition. This is disabled between communication events as it can lead
     * to undesired interrupts in multi-master setups. */
    p_ctrl->p_reg->IER = (uint32_t) (IIC_MASTER_INTERRUPT_ENABLE_INIT_MASK |
                                     R_RIIC0_IER_STIE_Msk | R_RIIC0_IER_SPIE_Msk);

    /* If previous transaction did not end with restart, send a start condition */
    if (!p_ctrl->restarted)
    {
        /* Request IIC to issue the start condition */
        p_ctrl->p_reg->CR2 = (uint32_t) IIC_MASTER_ICCR2_ST_BIT_MASK;
    }
    else
    {
        /* Address low byte, this could either be a 7 bit address or low byte of 10 bit address */
        uint8_t address_byte = p_ctrl->addr_low;
#if IIC_MASTER_CFG_ADDR_MODE_10_BIT_ENABLE

        /* 10 bit address, handle accordingly */
        if (p_ctrl->addr_total > 1U)
        {
            /* MSB transfer, send address high byte with with R/W set to 0 */
            if (0U == p_ctrl->addr_loaded)
            {
                address_byte = p_ctrl->addr_high;
            }
            /* MSB transfer after restart of 10 bit read, send high byte with R/W set to 1 */
            else if ((2U == p_ctrl->addr_loaded) && (3U == p_ctrl->addr_total))
            {
                address_byte = p_ctrl->addr_high | (uint8_t) I2C_CODE_READ;
            }
            /* Low byte transfer */
            else
            {
                address_byte = p_ctrl->addr_low;
            }
        }
#endif
        p_ctrl->restarted = false;

        /* Update the number of address bytes loaded for next pass */
        p_ctrl->addr_loaded++;
        p_ctrl->addr_remain--;

        /* Write the address byte */
        p_ctrl->p_reg->DRT = address_byte;
    }

    /*
     * The Flowchart "Master Transmit Operation" and section "Master Receive Operation"
     * of the user's manual is covered in the interrupts:
     *
     * 1. NACKF processing is handled in the NAKI interrupt.
     *    For receive, dummy reading ICDRR is not required because the NACK processing in
     *    this driver resets the IIC peripheral.
     * 2. Data is written to ICDRT in the transmit interrupt (TDRE is set when a transmit interrupt fires).
     * 3. For transmit, stop is issued in the transmit end interrupt (TEND is set when a transmit end interrupt fires).
     * 4. For transmit, ICSR2 is cleared in the transmit end interrupt.
     * 5. For receive, remaining processing including reading ICDRR happens in the receive interrupt (RDRF is
     * set when a receive interrupt fires).
     */
    return err;
}

/*******************************************************************************************************************//**
 * Handles the transmit data empty interrupt when operating as a master.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 **********************************************************************************************************************/
static void iic_master_txi_master (iic_master_instance_ctrl_t * p_ctrl)
{
    /* Check if there is issue in the slave address */
    if (0U < p_ctrl->addr_remain)
    {
        iic_master_txi_send_address(p_ctrl);
    }
    else if (!p_ctrl->read)
    {
        if (0U < p_ctrl->remain)
        {
            /* Update the number of bytes remaining for next pass */
            p_ctrl->remain--;

            /* Write the data to ICDRT register */
            p_ctrl->p_reg->DRT_b.DRT = p_ctrl->p_buff[p_ctrl->loaded];

            p_ctrl->loaded++;
        }

        /* Complete loading ICDRT, wait for TEND to send a stop/restart */
        if (0U == p_ctrl->remain)
        {
#if IIC_MASTER_CFG_TRANSFER_ENABLE
            if (p_ctrl->p_cfg->p_transfer_tx != NULL)
            {
                p_ctrl->activation_on_txi = false;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = IIC_MASTER_TRANSFER_RESET_SRC_DEST_NUM;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
                p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
                p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
                p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                                 p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
                iic_interrupt_handling_master(p_ctrl->p_cfg->txi_irq);
            }
#endif
            p_ctrl->p_reg->IER_b.TIE = 0UL;

            /* Wait for the value to reflect at the peripheral.
             * See 'Caution:1' under Table "Interrupt sources" of the user's manual. */
            while (0UL != p_ctrl->p_reg->IER_b.TIE)
            {
                /* Do nothing */
            }

            /* Enable the transmit end IRQ, to issue a STOP or RESTART */
            /* Clear any pending TEND interrupts */
            R_BSP_IrqStatusClear(p_ctrl->p_cfg->tei_irq);

            /* Enable the TXEND interrupt */
            p_ctrl->p_reg->IER_b.TEIE = 1UL;

            /* No need to wait to check TEIE has actually become 1U; because if that's not the case,
             * no other operation can occur at this point */
        }
    }
    else
    {
#if IIC_MASTER_CFG_TRANSFER_ENABLE
        if (p_ctrl->p_cfg->p_transfer_tx != NULL)
        {
            p_ctrl->activation_on_txi = false;
            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = IIC_MASTER_TRANSFER_RESET_SRC_DEST_NUM;
            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
            p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
            p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                             p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
            iic_interrupt_handling_master(p_ctrl->p_cfg->txi_irq);
        }
#endif
    }
}

/*******************************************************************************************************************//**
 * Write the address byte to the iic bus
 *
 * @param[in]       p_ctrl  Pointer to transfer control block
 **********************************************************************************************************************/
static void iic_master_txi_send_address (iic_master_instance_ctrl_t * const p_ctrl)
{
#if IIC_MASTER_CFG_TRANSFER_ENABLE
    uint32_t volatile const * p_iic_master_tx_buffer = &(p_ctrl->p_reg->DRT);
#endif

    /* This is a 10 bit read and transmit the low byte, next is restart */
    if ((3U == p_ctrl->addr_total) && (2U == p_ctrl->addr_loaded) && (false == p_ctrl->address_restarted))
    {
#if IIC_MASTER_CFG_ADDR_MODE_10_BIT_ENABLE

        /* For Read operation an extra address byte needs to be sent after issuing restart.
         * At this point sent the first 2 address bytes. Disable TXI.
         */

        p_ctrl->p_reg->IER_b.TIE = 0UL;

        /* Wait for the value to reflect at the peripheral.
         * See 'Note' under table "Interrupt sources" of the user's manual. */
        while (0UL != p_ctrl->p_reg->IER_b.TIE)
        {
            /* Do nothing */
        }

        /* Enable the transmit end IRQ and generate a RESTART condition */
        /* Clear any pending TEND interrupts */
        R_BSP_IrqStatusClear(p_ctrl->p_cfg->tei_irq);

        /* Enable the TXEND interrupt */
        p_ctrl->p_reg->IER_b.TEIE = 1UL;

        /* No need to wait to check TEIE has actually become 1U; because if that's not the case,
         * no other operation can occur at this point */
#endif
    }
    else
    {
        /* Address low byte, this could either be a 7 bit address or low byte of 10 bit address */
        uint8_t address_byte = p_ctrl->addr_low;
#if IIC_MASTER_CFG_ADDR_MODE_10_BIT_ENABLE

        /* 10 bit address, handle accordingly */
        if (p_ctrl->addr_total > 1U)
        {
            /* MSB transfer, send address high byte with with R/W set to 0 */
            if (0U == p_ctrl->addr_loaded)
            {
                address_byte = p_ctrl->addr_high;
            }
            /* MSB transfer after restart of 10 bit read, send high byte with R/W set to 1 */
            else if ((2U == p_ctrl->addr_loaded) && (3U == p_ctrl->addr_total))
            {
                address_byte = p_ctrl->addr_high | (uint8_t) I2C_CODE_READ;
            }
            /* Low byte transfer */
            else
            {
                address_byte = p_ctrl->addr_low;
            }
        }
#endif
#if IIC_MASTER_CFG_TRANSFER_ENABLE

        /* If this is the last address byte, enable transfer */
        if (1U == p_ctrl->addr_remain)
        {
            if ((NULL != p_ctrl->p_cfg->p_transfer_tx) && !(p_ctrl->read) && (p_ctrl->total > 0U))
            {
                /* Disable interrupt */
                R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);
                p_ctrl->p_cfg->p_transfer_tx->p_api->reset(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                           (void *) (p_ctrl->p_buff),
                                                           (uint8_t *) (p_iic_master_tx_buffer),
                                                           (uint16_t) (p_ctrl->remain));
                p_ctrl->remain            = 0U;
                p_ctrl->loaded            = p_ctrl->total;
                p_ctrl->activation_on_txi = true;
            }
        }
#endif

        /* Update the number of address bytes loaded for next pass */
        p_ctrl->addr_loaded++;
        p_ctrl->addr_remain--;

        /* Write the address byte */
        p_ctrl->p_reg->DRT_b.DRT = address_byte;
    }
}

/*******************************************************************************************************************//**
 * Handles the receive data full interrupt when operating as a master.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 **********************************************************************************************************************/
static void iic_master_rxi_master (iic_master_instance_ctrl_t * p_ctrl)
{
    volatile uint8_t dummy_read;

    /* First receive interrupt: Handle the special case of 1 or 2 byte read here */
    if (false == p_ctrl->dummy_read_completed)
    {
        /* Enable WAIT for 1 or 2 byte read */
        if (2U <= p_ctrl->total)
        {
            p_ctrl->p_reg->MR3_b.WAIT = 1UL;
        }

        /* Enable NACK for 1 byte read */
        if (1U == p_ctrl->remain)
        {
            /* Writes to be done separately.
             * See section 'I2C Bus Mode Register 3 (ICMR3)' of the user's manual.
             */
            p_ctrl->p_reg->MR3_b.ACKWP = 1UL; /* Write enable ACKBT */
            p_ctrl->p_reg->MR3_b.ACKBT = 1UL;
        }

        /* Do a dummy read to clock the data into the ICDRR. */
#if IIC_MASTER_CFG_TRANSFER_ENABLE
        uint32_t volatile const * p_iic_master_rx_buffer = &(p_ctrl->p_reg->DRR);

        /* Enable transfer support if possible */
        if ((NULL != p_ctrl->p_cfg->p_transfer_rx) && (p_ctrl->read) && (p_ctrl->total > 3U))
        {
            /* Disable interrupt */
            R_BSP_IrqDisable(p_ctrl->p_cfg->rxi_irq);

            p_ctrl->p_cfg->p_transfer_rx->p_api->reset(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                       (uint8_t *) (p_iic_master_rx_buffer),
                                                       (void *) (p_ctrl->p_buff),
                                                       (uint16_t) (p_ctrl->total - 3U));

            p_ctrl->remain            = 4U;
            p_ctrl->loaded            = p_ctrl->total - 3U;
            p_ctrl->activation_on_rxi = true;
        }
#endif
        dummy_read = p_ctrl->p_reg->DRR_b.DRR;

        FSP_PARAMETER_NOT_USED(dummy_read);

        /* Update the counter */
        p_ctrl->dummy_read_completed = true;
    }
    /* ICDRR contain valid received data */
    else if (0U < p_ctrl->remain)
    {
        iic_master_rxi_read_data(p_ctrl);
    }
    else
    {
        /* Do nothing */
    }
}

/*******************************************************************************************************************//**
 * Check valid receive data and set WAIT, NACK and STOP/RESTART bit in RXI handler.
 *
 * @param[in]       p_ctrl  Pointer to transfer control block
 **********************************************************************************************************************/
static void iic_master_rxi_read_data (iic_master_instance_ctrl_t * const p_ctrl)
{
    /* If next data = (final byte - 2), enable WAIT */
    if (3U == p_ctrl->remain)
    {
        p_ctrl->p_reg->MR3_b.WAIT = 1UL;
    }
    /* If next data = (final byte - 1), enable NACK  */
    else if (2U == p_ctrl->remain)
    {
        /* Writes to be done separately.
         * See Note 1 in section 'I2C Bus Mode Register 3 (ICMR3)' of the user's manual.
         */
        p_ctrl->p_reg->MR3_b.ACKWP = 1UL; /* Write enable ACKBT */
        p_ctrl->p_reg->MR3_b.ACKBT = 1UL;
    }
    /* If next data = final byte, send STOP or RESTART */
    else if (1U == p_ctrl->remain)
    {
        if (p_ctrl->restart)
        {
            /* NOTE:Only disable in GIC, disabling in I2C would cause the
             * restart condition to fail because of using the buffered
             * interrupt to start the next sequence */
            R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);
            p_ctrl->p_reg->IER_b.TIE = 1UL;

            p_ctrl->p_reg->MR3_b.ACKWP = 1UL; /* Write enable ACKBT */

            /* This bit clears to 0 automatically by issuing stop condition.
             * For restart condition, clear bit by software.
             */
            p_ctrl->p_reg->MR3_b.ACKBT = 0UL;

            /* Request IIC to issue the restart condition */
            p_ctrl->p_reg->CR2 = (uint32_t) IIC_MASTER_ICCR2_RS_BIT_MASK;

            /* Disable timeout function */
            p_ctrl->p_reg->FER_b.TMOE = 0UL;

            /* Remember the restart when doing the next transfer */
            p_ctrl->restarted = true;
        }
        else
        {
            /* Clear STOP flag and set SP.
             * It is ok to clear other status' as this transaction is over.
             */
            p_ctrl->p_reg->SR2 &= (uint32_t) ~(IIC_MASTER_ICSR2_STOP_BIT);;

            /* Request IIC to issue the stop condition */
            p_ctrl->p_reg->CR2 = (uint32_t) IIC_MASTER_ICCR2_SP_BIT_MASK; /* It is safe to write 0's to other bits. */

            /* STOP flag will not be set just yet.
             * STOP will be set only after reading the last byte from ICDRR and clearing the WAIT.
             * See Point #7 under section 'Master Receive Operation' of the user's manual.
             */
        }
    }
    else
    {
        /* Do nothing */
    }

#if IIC_MASTER_CFG_TRANSFER_ENABLE
    uint32_t volatile const * p_iic_master_rx_buffer = &(p_ctrl->p_reg->DRR);

    /* Enable transfer support if possible */
    if ((NULL != p_ctrl->p_cfg->p_transfer_rx) && (p_ctrl->read))
    {
        if ((p_ctrl->remain > 1) && (p_ctrl->total > 3U))
        {
            p_ctrl->p_cfg->p_transfer_rx->p_api->reset(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                       (uint8_t *) (p_iic_master_rx_buffer),
                                                       (void *) (&p_ctrl->p_buff[p_ctrl->loaded]), (uint16_t) (1U));

            p_ctrl->remain--;
            p_ctrl->loaded++;
            p_ctrl->activation_on_rxi = true;
        }
        else if (p_ctrl->total > 3U)
        {
            p_ctrl->activation_on_rxi = false;
            p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = 1U;
            p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
            p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
            p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
            p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                             p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
            iic_interrupt_handling_master(p_ctrl->p_cfg->rxi_irq);
            p_ctrl->remain--;
        }
        else
        {
            p_ctrl->p_buff[p_ctrl->loaded] = (uint8_t) p_ctrl->p_reg->DRR_b.DRR;;

            /* Update the counter values */
            p_ctrl->loaded++;
            p_ctrl->remain--;
        }
    }
    else
#endif
    {
        p_ctrl->p_buff[p_ctrl->loaded] = (uint8_t) p_ctrl->p_reg->DRR_b.DRR;;

        /* Update the counter values */
        p_ctrl->loaded++;
        p_ctrl->remain--;
    }

#if IIC_MASTER_CFG_TRANSFER_ENABLE

    /*  Complete the reception, clear the WAIT bit */
    if ((0U == p_ctrl->remain) && (false == p_ctrl->activation_on_rxi))
#else

    /*  Complete the reception, clear the WAIT bit */
    if (0U == p_ctrl->remain)
#endif
    {
        p_ctrl->p_reg->MR3_b.WAIT = 0UL;

        /* If this transaction does not have the restart flag set to true,
         * last byte has been read and WAIT has been cleared.
         * Callback will be issued by the SPI once the stop condition is detected
         * In case of restart flag set to true a callback will be issued by the STI once the start
         * (from restart) condition is detected
         */
    }
}

/*******************************************************************************************************************//**
 * Handles the transmit end interrupt when operating as a master.
 * @note This interrupt is configured to be generated at the end of last byte of the requested transfer.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 **********************************************************************************************************************/
static void iic_master_tei_master (iic_master_instance_ctrl_t * p_ctrl)
{
    /* This is a 10 bit address read, issue a restart prior to the last address byte transmission  */
    if ((p_ctrl->read) && (1U == p_ctrl->addr_remain) && (false == p_ctrl->address_restarted))
    {
#if IIC_MASTER_CFG_ADDR_MODE_10_BIT_ENABLE

        /* Enable TXI so that it fires after restart condition. */
        p_ctrl->p_reg->IER_b.TIE = 1UL;

        /* Request IIC to issue the restart condition */
        p_ctrl->p_reg->CR2        = (uint32_t) IIC_MASTER_ICCR2_RS_BIT_MASK;
        p_ctrl->address_restarted = true;
#endif
    }
    /*Complete the transfer, send STOP or RESTART */
    else if (0U == p_ctrl->remain)
    {
        /* Send RESTART */
        if (p_ctrl->restart)
        {
            /* NOTE:Only disable in GIC, disabling in I2C would cause the
             * restart condition to fail because of using the buffered
             * interrupt to start the next sequence */
            R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);
            p_ctrl->p_reg->IER_b.TIE = 1UL;

            /* Request IIC to issue the restart condition and queue a TXI at the GIC level. */
            p_ctrl->p_reg->CR2 = (uint32_t) IIC_MASTER_ICCR2_RS_BIT_MASK;

            /* Disable timeout function */
            p_ctrl->p_reg->FER_b.TMOE = 0UL;

            /* Remember if there is issue with a restart for the next transfer */
            p_ctrl->restarted = true;
        }
        /* Send STOP */
        else
        {
            /* Clear STOP flag and set SP.
             * It is ok to clear other status' as this transaction is over.
             */
            p_ctrl->p_reg->SR2 &= (uint32_t) ~(IIC_MASTER_ICSR2_STOP_BIT);

            /* Request IIC to issue the stop condition */
            p_ctrl->p_reg->CR2 = (uint32_t) IIC_MASTER_ICCR2_SP_BIT_MASK; /* It is safe to write 0's to other bits. */
        }
    }
    else
    {
        /* Do nothing */
    }

    /* Disable the interrupt as finish the transfer */
    p_ctrl->p_reg->IER_b.TEIE = 0UL;

    /* Wait for the value to reflect at the peripheral. */
    while (0UL != p_ctrl->p_reg->IER_b.TEIE)
    {
        /* Do nothing */
    }
}

/*******************************************************************************************************************//**
 * Handles the reception of NACK interrupt when operating as a master.
 *
 * @param[in]       p_ctrl     Pointer to transfer control block
 **********************************************************************************************************************/
static void iic_master_naki_master (iic_master_instance_ctrl_t * p_ctrl)
{
    iic_master_extended_cfg_t * p_extend = (iic_master_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

    if ((1UL != p_ctrl->p_reg->CR2_b.MST) && (true != p_ctrl->nack_before_stop))
    {
        p_ctrl->p_reg->SR2_b.NACKF = 0UL;

        /*This is a NACK error and this device is no longer the active master on the bus.
         *    The MST bit here can get cleared:
         *     1. In case of an arbitration loss error.also occurs.
         *     2. If the slave timeout is lesser than master timeout and the slave releases
         *        the bus by performing an internal reset.
         *       Refer section "I2C Bus Control Register 2 (ICCR2) - Clearing conditions for MST"
         *        of the user's manual.
         */
        p_ctrl->err = true;

        /* Abort an in-progress transfer with the current device */
        iic_master_abort_seq_master(p_ctrl, true); /* This will reset the IIC Master driver */
        /* Notify anyone waiting that the transfer is Aborted due to error. */
        iic_master_notify(p_ctrl, I2C_MASTER_EVENT_ABORTED);
    }
    else if (true == p_ctrl->nack_before_stop)
    {
        /* This is the process that branches Reception of NACK interrupt again,
         * when Stop Condition was requested once in Reception of NACK interrupt,
         * and then ICSR2_b.STOP is not 1 .
         */
        p_ctrl->nack_before_stop = false;

        p_ctrl->p_reg->SR2_b.NACKF = 0UL;
    }
    else
    {
        /* MST bit must be set to issue a stop condition.
         * Refer section "Issuing a Stop Condition" of the user's manual.
         */

        /* Set the error flag when an error event occurred
         * This will be checked after the stop condition is detected from the request below. */
        p_ctrl->err = true;

        /* The sequence below is to handle a NACK received from slave in the middle of a write.
         * See item '[4]' under figure 'Example master transmission flow' of the user's manual. */

        /* Request IIC to issue the stop condition */
        p_ctrl->p_reg->SR2 &= (uint32_t) ~(IIC_MASTER_ICSR2_STOP_BIT);
        p_ctrl->p_reg->CR2  = (uint32_t) IIC_MASTER_ICCR2_SP_BIT_MASK; /* It is safe to write 0's to other bits. */
        /* Allow timeouts to be generated on the low value of SCL using either long or short mode */
        p_ctrl->p_reg->MR2 = (uint32_t) ((uint8_t) 0x02UL |
                                         (uint8_t) (IIC_MASTER_TIMEOUT_MODE_SHORT ==
                                                    p_extend->timeout_mode));
        p_ctrl->p_reg->FER_b.TMOE = 1UL;

        /* NACK flag must be cleared after SR2_b.STOP=1. */
        if (1UL == p_ctrl->p_reg->SR2_b.STOP)
        {
            p_ctrl->p_reg->SR2_b.NACKF = 0UL;
        }
        else
        {
            /* If the state of SR2_b.STOP=1 is not reached by the above process,
             * the information that Stop Condition was requested in Reception of NACK interrupt is saved.
             */
            p_ctrl->nack_before_stop = true;
        }
    }
}

/*******************************************************************************************************************//**
 * Handles the Detection of a stop condition interrupt when operating as a master.
 *
 * @param[in]       p_ctrl     Pointer to transfer control block.
 **********************************************************************************************************************/
static void iic_master_spi_master (iic_master_instance_ctrl_t * p_ctrl)
{
    p_ctrl->p_reg->SR2_b.STOP = 0UL;
    i2c_master_event_t event = I2C_MASTER_EVENT_ABORTED;
    if (false == p_ctrl->err)          /* Successful transaction */
    {
        /* Get the correct event to notify the user */
        event = (p_ctrl->read) ? I2C_MASTER_EVENT_RX_COMPLETE : I2C_MASTER_EVENT_TX_COMPLETE;

        /* Disable SPIE to prevent Detection of a stop condition interrupts in multi-master scenarios */
        p_ctrl->p_reg->IER_b.SPIE = 0UL;
    }
    else
    {
        /* This is the STOP condition requested due to a NACK error earlier.
         * Since the stop condition is successfully issued there is no need to reset the driver.
         */
        iic_master_abort_seq_master(p_ctrl, false); /* Clear the transaction flags only */
    }

    /* Notify anyone waiting */
    iic_master_notify(p_ctrl, event);
}

/*******************************************************************************************************************//**
 * Handles the Detection of a start condition interrupt when operating as a master.
 *
 * @param[in]       p_ctrl     Pointer to transfer control block.
 **********************************************************************************************************************/
static void iic_master_sti_master (iic_master_instance_ctrl_t * p_ctrl)
{
    p_ctrl->p_reg->SR2_b.START = 0UL;
    if (p_ctrl->restarted)
    {
        i2c_master_event_t event = I2C_MASTER_EVENT_ABORTED;
        if (false == p_ctrl->err)      /* Successful transaction */
        {
            /* Get the correct event to notify the user */
            event = (p_ctrl->read) ? I2C_MASTER_EVENT_RX_COMPLETE : I2C_MASTER_EVENT_TX_COMPLETE;

            /* Disable STIE to prevent Detection of a start condition interrupts in multi-master scenarios */
            p_ctrl->p_reg->IER_b.STIE = 0UL;
        }

        /* Notify anyone waiting */
        iic_master_notify(p_ctrl, event);
    }
}

/*******************************************************************************************************************//**
 * Handles the Arbitration lost interrupt when operating as a master.
 *
 * @param[in]       p_ctrl     Pointer to transfer control block.
 **********************************************************************************************************************/
static void iic_master_ali_master (iic_master_instance_ctrl_t * p_ctrl)
{
    p_ctrl->p_reg->SR2_b.AL = 0UL;

    /* This is arbitration loss error during an ongoing transaction */
    /* Set the error flag when an error event occurred */
    p_ctrl->err = true;

    /* Abort an in-progress transfer with the current device */
    iic_master_abort_seq_master(p_ctrl, true); /* This will reset the IIC Master driver */
    /* Notify anyone waiting that the transfer is Aborted due to error. */
    iic_master_notify(p_ctrl, I2C_MASTER_EVENT_ABORTED);
}

/*******************************************************************************************************************//**
 * Handles the Timeout interrupt when operating as a master.
 *
 * @param[in]       p_ctrl     Pointer to transfer control block.
 **********************************************************************************************************************/
static void iic_master_tmoi_master (iic_master_instance_ctrl_t * p_ctrl)
{
    p_ctrl->p_reg->SR2_b.TMOF = 0UL;

    /* This is Timeout  error during an ongoing transaction */
    /* Set the error flag when an error event occurred */
    p_ctrl->err = true;

    /* Abort an in-progress transfer with the current device */
    iic_master_abort_seq_master(p_ctrl, true); /* This will reset the IIC Master driver */
    /* Notify anyone waiting that the transfer is Aborted due to error. */
    iic_master_notify(p_ctrl, I2C_MASTER_EVENT_ABORTED);
}

/*******************************************************************************************************************//**
 * Single point for managing the logic around notifying a transfer has finished.
 *
 * @param[in]       p_ctrl      Pointer to transfer that is ending.
 * @param[in]       event       The event code to pass to the callback.
 **********************************************************************************************************************/
static void iic_master_notify (iic_master_instance_ctrl_t * const p_ctrl, i2c_master_event_t const event)
{
    i2c_master_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    i2c_master_callback_args_t * p_args = p_ctrl->p_callback_memory;
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

    p_args->p_context = p_ctrl->p_context;
    p_args->event     = event;

    /* Do the callback here */
    if (NULL != p_ctrl->p_callback)
    {
        p_ctrl->p_callback(p_args);
    }

    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }

    /* Clear the err flags */
    p_ctrl->err = false;
}

#if IIC_MASTER_CFG_TRANSFER_ENABLE

/*******************************************************************************************************************//**
 * Configures IIC related transfer drivers (if enabled).
 *
 * @param[in]   p_ctrl    Pointer to IIC specific control structure
 * @param[in]   p_cfg     Pointer to IIC specific configuration structure
 *
 * @retval      FSP_SUCCESS                Transfer interface initialized successfully.
 * @retval      FSP_ERR_ASSERTION          Pointer to transfer instance for I2C receive in p_cfg is NULL.
 **********************************************************************************************************************/
static fsp_err_t iic_master_transfer_open (iic_master_instance_ctrl_t * const p_ctrl,
                                           i2c_master_cfg_t const * const     p_cfg)
{
 #if (IIC_MASTER_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_cfg);
 #endif
    fsp_err_t err = FSP_SUCCESS;
    if (NULL != p_cfg->p_transfer_rx)
    {
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->open(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                        p_cfg->p_transfer_rx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->callbackSet(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                               iic_master_rxi_transfer_callback,
                                                               (void *) &p_ctrl->p_cfg->rxi_irq,
                                                               NULL);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_cfg->p_transfer_tx)
    {
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->open(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                        p_cfg->p_transfer_tx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->callbackSet(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                               iic_master_txi_transfer_callback,
                                                               (void *) &p_ctrl->p_cfg->txi_irq,
                                                               NULL);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    return FSP_SUCCESS;
}

#endif

#if IIC_MASTER_CFG_TRANSFER_ENABLE

/*******************************************************************************************************************//**
 * Transmit data empty interrupt routine called when the transfer is completed.
 *
 * @param[in]     p_args     Pointer to callback.
 **********************************************************************************************************************/
void iic_master_txi_transfer_callback (transfer_callback_args_t * p_args)
{
    IRQn_Type irq = *(IRQn_Type *) (p_args->p_context);

    /* Recover ISR context saved in open. */
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    if (NULL != p_args)
    {
        p_ctrl->activation_on_txi = true;
        iic_master_txi_master(p_ctrl);
    }
}

#endif
#if IIC_MASTER_CFG_TRANSFER_ENABLE

/*******************************************************************************************************************//**
 * Receive data interrupt routine called when the transfer is completed.
 *
 * @param[in]     p_args     Pointer to callback.
 **********************************************************************************************************************/
void iic_master_rxi_transfer_callback (transfer_callback_args_t * p_args)
{
    IRQn_Type irq = *(IRQn_Type *) (p_args->p_context);
    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    if ((NULL != p_args) && (p_ctrl->read))
    {
        iic_master_rxi_master(p_ctrl);
    }
}

#endif

/*******************************************************************************************************************//**
 * Clear interrupt status and enable.
 *
 * This function implements Tx/Rx Interrupt Enable.
 *
 **********************************************************************************************************************/
void iic_interrupt_handling_master (IRQn_Type const irq)
{
    R_BSP_IrqStatusClear(irq);
    R_BSP_IrqEnable(irq);
}

/***********************************************************************************************************************
 * Interrupt service routines
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Transmit data empty interrupt routine.
 *
 * This function implements the Transmit buffer empty ISR routine.
 *
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void iic_master_txi_isr (void)
{
    /* Save context if RTOS is used */

    FSP_CONTEXT_SAVE

    /* Clear the IR flag */
    IRQn_Type irq = R_FSP_CurrentIrqGet();

    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    iic_master_txi_master(p_ctrl);

    /* Restore context if RTOS is used */

    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Receive data full interrupt routine.
 *
 * This function implements the IIC Receive buffer full ISR routine.
 *
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void iic_master_rxi_isr (void)
{
    /* Save context if RTOS is used */

    FSP_CONTEXT_SAVE

    /* Clear the IR flag */

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    iic_master_rxi_master(p_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Transmit end interrupt routine.
 *
 * This function implements the IIC Transmission End ISR routine.
 *
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void iic_master_tei_isr (void)
{
    /* Save context if RTOS is used */

    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    iic_master_tei_master(p_ctrl);

    /* Clear the IR flag */
    R_BSP_IrqStatusClear(irq);

    /* Restore context if RTOS is used */

    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Error and event interrupt routine.
 *
 * This function implements the IIC Event/Error.
 *
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void iic_master_eri_isr (void)
{
    /* Save context if RTOS is used */

    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    iic_master_instance_ctrl_t * p_ctrl = (iic_master_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
#if IIC_MASTER_CFG_TRANSFER_ENABLE
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = 1U;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                         p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
    }

    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = 1U;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                         p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
    }
#endif
    if ((1U == p_ctrl->p_reg->SR2_b.NACKF) && (1U == p_ctrl->p_reg->IER_b.NAKIE))
    {
        iic_master_naki_master(p_ctrl);
    }
    else if ((1U == p_ctrl->p_reg->SR2_b.START) && (1U == p_ctrl->p_reg->IER_b.STIE))
    {
        iic_master_sti_master(p_ctrl);
    }
    else if ((1U == p_ctrl->p_reg->SR2_b.STOP) && (1U == p_ctrl->p_reg->IER_b.SPIE))
    {
        iic_master_spi_master(p_ctrl);
    }
    else if ((1U == p_ctrl->p_reg->SR2_b.AL) && (p_ctrl->p_reg->IER_b.ALIE))
    {
        iic_master_ali_master(p_ctrl);
    }
    else if ((1U == p_ctrl->p_reg->SR2_b.TMOF) && (1U == p_ctrl->p_reg->IER_b.TMOIE))
    {
        iic_master_tmoi_master(p_ctrl);
    }
    else
    {
        /*Do Nothing*/
    }

    /* Restore context if RTOS is used */

    FSP_CONTEXT_RESTORE
}
