/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_iic_slave.h"

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "I2CS" in ASCII, used to determine if channel is open. */
#define IIC_SLAVE_OPEN                                     (0x49324353UL)

/* The timeout interrupt enable bit */
#define IIC_TMO_EN_BIT                                     (0x01U)

/* The arbitration loss detection interrupt enable bit */
#define IIC_ALD_EN_BIT                                     (0x02U)

/* The NAK reception interrupt enable bit */
#define IIC_NAK_EN_BIT                                     (0x10U)

/* Bit position for the START condition detection flag in ICSR2 */
#define ICSR2_START_BIT                                    (0x04U)

/* Bit position for the STOP condition detection flag in ICSR2 */
#define ICSR2_STOP_BIT                                     (0x08U)

/* Bit position for the START condition and STOP condition detection flag in ICSR2 */
#define ICSR2_START_STOP_BIT                               (ICSR2_START_BIT | ICSR2_STOP_BIT)

/* The stop condition detection interrupt enable bit */
#define IIC_STP_EN_BIT                                     (0x08U)

/* The start condition detection interrupt enable bit */
#define IIC_STR_EN_BIT                                     (0x04U)

/* I2C Bus Control Register 1 Masks */
#define IIC_SLAVE_PRV_SCL_SDA_NOT_DRIVEN                   (0x1FU)
#define IIC_SLAVE_ICCR1_ICE_BIT_MASK                       (0x80)
#define IIC_SLAVE_ICCR1_IICRST_BIT_MASK                    (0x40)

/* I2C Bus Mode Register 2 Masks */
#define IIC_SLAVE_BUS_MODE_REGISTER_2_MASK                 (0x02U)
#define IIC_SLAVE_INTERNAL_REF_CLOCK_SELECT_MAX            (0x07U)

/* I2C Bus Function Enable Register Mask */
#define IIC_SLAVE_FUNCTION_ENABLE_INIT_SETTINGS            (0x5DU)

/* I2C Bus Bit Rate Low-Level Register Mask */
#define IIC_SLAVE_BUS_RATE_REG_RESERVED_BITS               (0xE0U)

/* I2C Bus Status Enable Register Mask */
#define IIC_SLAVE_ICSER_SLAVE_ADDRESS_ENABLE_REGISTER_0    (0x01U)

/* I2C Bus Mode Register 1 Mask */
#define IIC_SLAVE_BUS_MODE_REGISTER_1_MASK                 (0x08U)

/* The receive data full interrupt enable bit */
#define IIC_RXI_EN_BIT                                     (0x20U)

/* The transmit end interrupt enable bit */
#define IIC_TEI_EN_BIT                                     (0x40U)

/* The transmit data empty interrupt enable bit */
#define IIC_TXI_EN_BIT                                     (0x80U)

/* Bit position for No Acknowledgment (NACKF) flag in ICSR2 */
#define ICSR2_NACKF_BIT                                    (0x10U)

/*I2C Slave Address Register Mask*/
#define IIC_SLAVE_SAR_FS_10BIT                             (0x8000U)

/* I2C Bus Status Register 2 Mask */
#define IIC_SLAVE_STATUS_REGISTER_2_ERR_MASK               (0x1FU)

/* Bit position for Arbitration loss (AL) and Timeout function (TMOF) detection flag in ICSR2 */
#define ICSR2_AL_TMOF_BIT                                  (0x03U)

/* Bit Fast-Mode Plus Enable (FMPE) in IICFER */
#define IIC_SLAVE_ICCFER_FMPE_BIT_MASK                     (0x80U)

/* Reset Source and Dest Number */
#define IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM              (0x01U)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private function prototypes
 *********************************************************************************************************************/
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
void             iic_slave_txi_transfer_callback(transfer_callback_args_t * p_args);
void             iic_slave_rxi_transfer_callback(transfer_callback_args_t * p_args);
static fsp_err_t iic_slave_transfer_open(iic_slave_instance_ctrl_t * const p_ctrl, i2c_slave_cfg_t const * const p_cfg);

#endif
void iic_slave_rxi_isr(void);
void iic_slave_txi_isr(void);
void iic_slave_tei_isr(void);
void iic_slave_eri_isr(void);

/* Internal helper functions */
static void      iic_slave_notify(iic_slave_instance_ctrl_t * const p_ctrl, i2c_slave_event_t const slave_event);
static void      iic_slave_callback_request(iic_slave_instance_ctrl_t * const p_ctrl, i2c_slave_event_t slave_event);
static void      iic_slave_initiate_transaction(iic_slave_instance_ctrl_t * p_ctrl, i2c_slave_event_t slave_event);
static fsp_err_t iic_slave_read_write(i2c_slave_ctrl_t * const p_api_ctrl,
                                      uint8_t * const          p_buffer,
                                      uint32_t const           bytes,
                                      iic_slave_transfer_dir_t direction);
static void r_iic_slave_call_callback(iic_slave_instance_ctrl_t * p_ctrl,
                                      i2c_slave_event_t           event,
                                      uint32_t                    transaction_count);

/* Interrupt handlers */
static void iic_rxi_slave(iic_slave_instance_ctrl_t * p_ctrl);
static void iic_txi_slave(iic_slave_instance_ctrl_t * p_ctrl);
static void iic_tei_slave(iic_slave_instance_ctrl_t * p_ctrl);
static void iic_naki_slave(iic_slave_instance_ctrl_t * p_ctrl);
static void iic_sti_spi_slave(iic_slave_instance_ctrl_t * p_ctrl);
static void iic_ali_tmoi_slave(iic_slave_instance_ctrl_t * p_ctrl);

/* Functions that manipulate hardware */
static void iic_open_hw_slave(iic_slave_instance_ctrl_t * const p_ctrl);
void        iic_interrupt_handling(IRQn_Type const irq);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* IIC Implementation of I2C device slave interface */
i2c_slave_api_t const g_i2c_slave_on_iic =
{
    .open        = R_IIC_SLAVE_Open,
    .write       = R_IIC_SLAVE_Write,
    .read        = R_IIC_SLAVE_Read,
    .close       = R_IIC_SLAVE_Close,
    .callbackSet = R_IIC_SLAVE_CallbackSet
};

/*******************************************************************************************************************//**
 * @addtogroup IIC_SLAVE
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/******************************************************************************************************************//**
 * Opens the I2C slave device.
 *
 * @retval  FSP_SUCCESS                       I2C slave device opened successfully.
 * @retval  FSP_ERR_ALREADY_OPEN              Module is already open.
 * @retval  FSP_ERR_IP_CHANNEL_NOT_PRESENT    Channel is not available on this MPU.
 * @retval  FSP_ERR_INVALID_ARGUMENT          NACK, Start, Stop, Timeout, Arbitration lost interrupt priority is lower than
 *                                            Transmit, Receive and Transmit End interrupt priority
 * @retval  FSP_ERR_ASSERTION                 Parameter check failure due to one or more reasons below:
 *                                            1. p_api_ctrl or p_cfg is NULL.
 *                                            2. extended parameter is NULL.
 *                                            3. Callback parameter is NULL.
 *                                            4. Set the rate to fast mode plus on a channel which does not support it.
 *                                            5. Invalid IRQ number assigned
 *********************************************************************************************************************/
fsp_err_t R_IIC_SLAVE_Open (i2c_slave_ctrl_t * const p_api_ctrl, i2c_slave_cfg_t const * const p_cfg)
{
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) p_api_ctrl;

#if IIC_SLAVE_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ASSERT(p_cfg != NULL);
    FSP_ASSERT(p_cfg->p_extend != NULL);
    FSP_ASSERT((p_cfg->rxi_irq >= (IRQn_Type) 0) && (FSP_INVALID_VECTOR != p_cfg->rxi_irq));
    FSP_ASSERT((p_cfg->txi_irq >= (IRQn_Type) 0) && (FSP_INVALID_VECTOR != p_cfg->txi_irq));
    FSP_ASSERT((p_cfg->tei_irq >= (IRQn_Type) 0) && (FSP_INVALID_VECTOR != p_cfg->tei_irq));
    FSP_ASSERT((p_cfg->eri_irq >= (IRQn_Type) 0) && (FSP_INVALID_VECTOR != p_cfg->eri_irq));
    FSP_ERROR_RETURN(BSP_FEATURE_IIC_VALID_CHANNEL_MASK & (1 << p_cfg->channel), FSP_ERR_IP_CHANNEL_NOT_PRESENT);
    FSP_ERROR_RETURN(IIC_SLAVE_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ERROR_RETURN(p_cfg->eri_ipl <= p_cfg->ipl, FSP_ERR_INVALID_ARGUMENT);
#endif
    p_ctrl->p_reg =
        (R_RIIC0_Type *) ((uintptr_t) R_RIIC0 + (p_cfg->channel * ((uintptr_t) R_RIIC1 - (uintptr_t) R_RIIC0)));

    /* Record the configuration on the device for use later */
    p_ctrl->p_cfg             = p_cfg;
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    /* Indicate that restart and stop condition detection yet to be enabled */
    p_ctrl->start_interrupt_enabled = false;

    /* Open the hardware in slave mode.
     *  Performs IIC initialization as described in hardware manual (see Section
     * 'Initial Settings' of the user's manual).
     **/
    iic_open_hw_slave(p_ctrl);
#if IIC_SLAVE_CFG_TRANSFER_ENABLE

    /* Open the IIC transfer interface if available */
    iic_slave_transfer_open(p_ctrl, p_cfg);
#endif
    R_BSP_IrqCfgEnable(p_cfg->eri_irq, p_ctrl->p_cfg->eri_ipl, p_ctrl);
    R_BSP_IrqCfgEnable(p_cfg->txi_irq, p_ctrl->p_cfg->ipl, p_ctrl);
    R_BSP_IrqCfgEnable(p_cfg->tei_irq, p_ctrl->p_cfg->ipl, p_ctrl);
    R_BSP_IrqCfgEnable(p_cfg->rxi_irq, p_ctrl->p_cfg->ipl, p_ctrl);

    /* Finally, can consider the device opened */
    p_ctrl->p_buff                = NULL;
    p_ctrl->total                 = 0U;
    p_ctrl->remain                = 0U;
    p_ctrl->loaded                = 0U;
    p_ctrl->transaction_completed = false;
    p_ctrl->open              = IIC_SLAVE_OPEN;
    p_ctrl->direction         = IIC_SLAVE_TRANSFER_DIR_NOT_ESTABLISHED;
    p_ctrl->do_dummy_read     = false;
    p_ctrl->notify_request    = false;
    p_ctrl->transaction_count = 0U;
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
    p_ctrl->activation_on_rxi = false;
    p_ctrl->activation_on_txi = false;
#endif

    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * Performs a write to the I2C Master device.
 *
 * This function will fail if there is already an in-progress I2C transfer on the associated channel. Otherwise, the
 * I2C slave write operation will begin. The caller will be notified when the operation has finished by an
 * I2C_SLAVE_EVENT_TX_COMPLETE in the callback.
 * In case the master continues to read more data, an I2C_SLAVE_EVENT_TX_MORE_REQUEST will be issued via callback.
 * In case of errors, an I2C_SLAVE_EVENT_ABORTED will be issued via callback.
 *
 * @retval  FSP_SUCCESS        Function executed without issue.
 * @retval  FSP_ERR_ASSERTION  p_api_ctrl or p_src is NULL.
 * @retval  FSP_ERR_IN_USE     Another transfer was in progress.
 * @retval  FSP_ERR_NOT_OPEN   Device is not open.
 *********************************************************************************************************************/
fsp_err_t R_IIC_SLAVE_Write (i2c_slave_ctrl_t * const p_api_ctrl, uint8_t * const p_src, uint32_t const bytes)
{
    fsp_err_t err = FSP_SUCCESS;
#if (IIC_SLAVE_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_api_ctrl);
#endif
    err = iic_slave_read_write(p_api_ctrl, p_src, bytes, IIC_SLAVE_TRANSFER_DIR_MASTER_READ_SLAVE_WRITE);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) p_api_ctrl;
    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        p_ctrl->activation_on_txi = true;
    }
#endif

    return err;
}

/******************************************************************************************************************//**
 * Performs a read from the I2C Master device.
 *
 * This function will fail if there is already an in-progress I2C transfer on the associated channel. Otherwise, the
 * I2C slave read operation will begin. The caller will be notified when the operation has finished by an
 * I2C_SLAVE_EVENT_RX_COMPLETE in the callback.
 * In case the master continues to write more data, an I2C_SLAVE_EVENT_RX_MORE_REQUEST will be issued via callback.
 * In case of errors, an I2C_SLAVE_EVENT_ABORTED will be issued via callback.
 *
 * @retval  FSP_SUCCESS        Function executed without issue
 * @retval  FSP_ERR_ASSERTION  p_api_ctrl, bytes or p_dest is NULL.
 * @retval  FSP_ERR_IN_USE     Another transfer was in progress.
 * @retval  FSP_ERR_NOT_OPEN   Device is not open.
 *********************************************************************************************************************/
fsp_err_t R_IIC_SLAVE_Read (i2c_slave_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes)
{
    fsp_err_t err = FSP_SUCCESS;
#if (IIC_SLAVE_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_api_ctrl);
#endif

    err = iic_slave_read_write(p_api_ctrl, p_dest, bytes, IIC_SLAVE_TRANSFER_DIR_MASTER_WRITE_SLAVE_READ);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) p_api_ctrl;
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        p_ctrl->activation_on_rxi = true;
    }
#endif

    return err;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements i2c_slave_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_IIC_SLAVE_CallbackSet (i2c_slave_ctrl_t * const          p_api_ctrl,
                                   void (                          * p_callback)(i2c_slave_callback_args_t *),
                                   void * const                      p_context,
                                   i2c_slave_callback_args_t * const p_callback_memory)
{
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) p_api_ctrl;

#if (IIC_SLAVE_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(IIC_SLAVE_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_ctrl->p_callback = p_callback;

    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * Closes the I2C device.
 *
 * @retval  FSP_SUCCESS         Device closed successfully.
 * @retval  FSP_ERR_NOT_OPEN    Device not opened.
 * @retval  FSP_ERR_ASSERTION   p_api_ctrl is NULL.
 *********************************************************************************************************************/
fsp_err_t R_IIC_SLAVE_Close (i2c_slave_ctrl_t * const p_api_ctrl)
{
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) p_api_ctrl;
    fsp_err_t err = FSP_SUCCESS;
#if IIC_SLAVE_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);

    /* Check if the device is even open, return an error if not */
    FSP_ERROR_RETURN(IIC_SLAVE_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* The device is considered closed */
    p_ctrl->open = 0U;

    /* Clear all interrupt bits */
    p_ctrl->p_reg->IER = 0U;
#if IIC_SLAVE_CFG_TRANSFER_ENABLE

    /* Close the handles for the transfer interfaces */
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->close(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->close(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    /* Disable all interrupts in NVIC */
    R_BSP_IrqDisable(p_ctrl->p_cfg->eri_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->rxi_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);
    R_BSP_IrqDisable(p_ctrl->p_cfg->tei_irq);

    // R_BSP_MODULE_STOP(FSP_IP_IIC, p_ctrl->p_cfg->channel);

    return err;
}

/******************************************************************************************************************//**
 * @} (end addtogroup IIC_SLAVE)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private Functions
 *********************************************************************************************************************/

/******************************************************************************************************************//**
 * Performs the hardware initialization sequence when operating as a slave (see Section
 * 'Initial Settings' of the user's manual).
 *
 * @param[in]       p_ctrl     Pointer to the control structure.
 *********************************************************************************************************************/
static void iic_open_hw_slave (iic_slave_instance_ctrl_t * const p_ctrl)
{
    iic_slave_extended_cfg_t * p_extend = (iic_slave_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

    uint8_t digital_filter_stages =
        (((iic_slave_extended_cfg_t *) p_ctrl->p_cfg->p_extend)->clock_settings.digital_filter_stages);

    /* Perform IIC reset */
    p_ctrl->p_reg->CR1 = (uint32_t) IIC_SLAVE_PRV_SCL_SDA_NOT_DRIVEN;

    /* Reset */
    p_ctrl->p_reg->CR1 = (uint32_t) (IIC_SLAVE_ICCR1_IICRST_BIT_MASK | IIC_SLAVE_PRV_SCL_SDA_NOT_DRIVEN);

    /* Come out of IIC reset to internal reset */
    p_ctrl->p_reg->CR1 =
        (uint8_t) (IIC_SLAVE_ICCR1_ICE_BIT_MASK | IIC_SLAVE_ICCR1_IICRST_BIT_MASK |
                   IIC_SLAVE_PRV_SCL_SDA_NOT_DRIVEN);

    /* Set Slave address in SARx and Set ICSER */
    /*7 bit mode selected, clear SAR. */
    if (I2C_SLAVE_ADDR_MODE_7BIT == p_ctrl->p_cfg->addr_mode)
    {
        p_ctrl->p_reg->SAR[0].SAR_b.SVA = (uint8_t) (p_ctrl->p_cfg->slave << R_RIIC0_SAR_SAR_SVA0_Pos);
    }
    /* 10 bit mode selected, set SARx. */
    else
    {
        p_ctrl->p_reg->SAR[0].SAR =
            (uint32_t) ((p_ctrl->p_cfg->slave << R_RIIC0_SAR_SAR_SVA0_Pos) | IIC_SLAVE_SAR_FS_10BIT);
    }

    /* Enable the slave address */
    p_ctrl->p_reg->SER = (uint32_t) ((uint8_t) IIC_SLAVE_ICSER_SLAVE_ADDRESS_ENABLE_REGISTER_0 |
                                     ((uint8_t) p_ctrl->p_cfg->general_call_enable << R_RIIC0_SER_GCE_Pos));

    /* Allow timeouts to be generated on the low value of SCL using long count mode */
    p_ctrl->p_reg->MR2 = IIC_SLAVE_BUS_MODE_REGISTER_2_MASK;

    /* ICFER Register Settings:
     * 1. Enable timeout function.
     * 2. Disable master arbitration loss detection.
     * 3. Enable NACK arbitration loss detection.
     * 4. Enable Slave arbitration loss detection.
     * 5. Enable NACK reception transfer suspension.
     * 6. Do not use the digital noise filter circuit.
     * 7. Use the SCL synchronous circuit.
     * (see Section 'I2C Bus Function Enable Register' of the user's manual for more details)
     */
    p_ctrl->p_reg->FER =
        (uint32_t) (((uint8_t) (0 < digital_filter_stages) << 5U) |
                    (uint8_t) IIC_SLAVE_FUNCTION_ENABLE_INIT_SETTINGS);
#if BSP_FEATURE_IIC_HAS_FMPE

    /* ICFER Register Settings:
     * 8. Enable FM+ slope circuit if fast mode plus is enabled.
     */
    if (I2C_SLAVE_RATE_FASTPLUS == p_ctrl->p_cfg->rate)
    {
        p_ctrl->p_reg->FER |= (uint8_t) IIC_SLAVE_ICCFER_FMPE_BIT_MASK;
    }
#endif

    /* Set the clock and slope circuits to match the data setup time specified by the I2C standard */
    /* This is set in the configuration structure by the tooling. */

    /* Set the number of counts that the clock remains low, bit 7 to 5 should be written as 1 */
    p_ctrl->p_reg->BRL =
        (uint32_t) (IIC_SLAVE_BUS_RATE_REG_RESERVED_BITS |
                    p_extend->clock_settings.brl_value);

    /* ICBRH need not be updated. */
    /* Set the internal reference clock source for generating IIC clock */
    p_ctrl->p_reg->MR1 = (uint32_t) (IIC_SLAVE_BUS_MODE_REGISTER_1_MASK |
                                     (uint8_t) ((p_extend->
                                                 clock_settings.cks_value &
                                                 IIC_SLAVE_INTERNAL_REF_CLOCK_SELECT_MAX) << 4U));

    /* 1. Set the digital filter.
     * 2. ACKBT should be set to 0 after reset to send out an ACK upon slave address match.
     * 3. Set WAIT bit based on user config.
     * Refer Section ICMR3 : I2C Bus Mode Register 3 :
     * 'WAIT bit (WAIT)' and 'RDRFS bit (RDRF Flag Set Timing Select)'
     * of the user's manual.
     * Since RDRFS = 0:
     * - SCLn line will *not* be held low at the falling edge of the 8th clock cycle.
     * - RDRF flag will be set at the rising edge of the 9th clock cycle. (Cause of iic_rxi_slave)
     * This means that iic_rxi_slave will be invoked during the 9th clock cycle (High) if there is no preemption.
     * Set the WAIT = 1 so that the SCLn line is held low from the falling edge of the 9th clock cycle.
     * This is done to support clock stretching during the 'iic_slave_initiate_transaction'/user
     * event callback (more often)
     * and if iic_rxi_slave can get preempted.
     *
     * Note 1: If the preemption happens after the Read API is called in the user event callback,
     *         WAIT = 1 will be able to handle that scenario,
     *         however if the preemption happens before, the slave will timeout if the Read API
     *         is not called within 'Long Timeout Mode' duration.
     *
     * Note 2: When WAIT = 1, this driver no longer supports data reception utilizing the double buffer HW setup.
     *
     * Note 3: WAIT bit is dont-care during transmission.
     * */
    p_ctrl->p_reg->MR3 = (uint32_t) ((uint8_t) (digital_filter_stages > 0U ? (digital_filter_stages - 1U) : 0U) |
                                     (uint8_t) (p_ctrl->p_cfg->clock_stretching_enable << R_RIIC0_MR3_WAIT_Pos));

    p_ctrl->p_reg->IER = (uint32_t) ((uint8_t) IIC_RXI_EN_BIT | (uint8_t) IIC_TXI_EN_BIT);

    /* Release IIC from internal reset */
    p_ctrl->p_reg->CR1 = (uint32_t) (IIC_SLAVE_ICCR1_ICE_BIT_MASK | IIC_SLAVE_PRV_SCL_SDA_NOT_DRIVEN);
}

/*******************************************************************************************************************//**
 * Helper function for handling I2C Slave Read or Write.
 *
 * @param      p_api_ctrl      Pointer to the control structure.
 * @param      p_buffer        Pointer to the buffer to store read/write data.
 * @param[in]  bytes           Number of bytes to be read/written.
 * @param[in]  direction       Slave Read or Slave Write
 *
 * @retval  FSP_SUCCESS           Function executed successfully.
 * @retval  FSP_ERR_ASSERTION     p_api_ctrl or p_buffer is NULL.
 *
 * @retval  FSP_ERR_IN_USE        Another transfer was in progress.
 * @retval  FSP_ERR_NOT_OPEN      Handle is not initialized. Call R_IIC_SLAVE_Open to initialize the control block.
 **********************************************************************************************************************/
static fsp_err_t iic_slave_read_write (i2c_slave_ctrl_t * const p_api_ctrl,
                                       uint8_t * const          p_buffer,
                                       uint32_t const           bytes,
                                       iic_slave_transfer_dir_t direction)
{
    fsp_err_t err = FSP_SUCCESS;
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) p_api_ctrl;

#if IIC_SLAVE_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ASSERT(p_buffer != NULL);

    /* Check if the device is even open, return an error if not */
    FSP_ERROR_RETURN(IIC_SLAVE_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Fail if there is already a transfer in progress */
    FSP_ERROR_RETURN(IIC_SLAVE_TRANSFER_DIR_NOT_ESTABLISHED == p_ctrl->direction, FSP_ERR_IN_USE);

    FSP_ASSERT(((iic_slave_instance_ctrl_t *) p_api_ctrl)->p_callback != NULL);
#endif

    /* Record the new information about this transfer */
    p_ctrl->p_buff    = p_buffer;
    p_ctrl->total     = bytes;
    p_ctrl->remain    = bytes;
    p_ctrl->direction = direction;

    /* Initialize fields used during transfer */
    p_ctrl->loaded = 0U;

    /* Indicate that restart and stop condition detection yet to be enabled */
    p_ctrl->start_interrupt_enabled = false;

    /* Set the response as ACK */
    p_ctrl->p_reg->MR3_b.ACKWP = 1;    /* Write Enable */
    p_ctrl->p_reg->MR3_b.ACKBT = 0;    /* Write */

    /* Timeouts are enabled by the driver code at the end of an IIC Slave callback.
     * Do not enable them here to prevent time restricting the application code.
     */

    p_ctrl->transaction_completed = false;
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    return err;
}

/******************************************************************************************************************//**
 * Handles the transmit data empty interrupt when operating as a slave.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 *********************************************************************************************************************/
static void iic_txi_slave (iic_slave_instance_ctrl_t * p_ctrl)
{
    /* Check if the read request event has been notified through callback, if not provide the callback */
    if (!p_ctrl->notify_request)
    {
        iic_slave_initiate_transaction(p_ctrl, I2C_SLAVE_EVENT_TX_REQUEST);
    }

    /* If MasterReadSlaveWrite API is invoked, proceed writing data */
    if (IIC_SLAVE_TRANSFER_DIR_MASTER_READ_SLAVE_WRITE != p_ctrl->direction)
    {
        /* MasterReadSlaveWrite API was not called in the callback.
         * Master will read 0xFF for all the byte(s) for this transaction.
         */
    }
    else
    {
        /* If slave has exhausted the buffer length from application, wait for slave to transmit last byte on the bus
         * and check for master is sending ACK or NACK. If master ACKs this last byte, it is expecting more data
         * from slave. Slave has to notify this event to application via callback */
        if (p_ctrl->total == p_ctrl->loaded)
        {
            /* Clear the pending interrupts for TEI */
            R_BSP_IrqStatusClear(p_ctrl->p_cfg->tei_irq);

            /* Enable the TEI interrupt source */
            p_ctrl->p_reg->IER_b.TEIE = 1U;
        }
        else
        {
            /* Write the data byte, this will also release SCL */
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
            uint32_t volatile const * p_iic_slave_tx_buffer = &(p_ctrl->p_reg->DRT);
            if ((NULL != p_ctrl->p_cfg->p_transfer_tx) && (p_ctrl->activation_on_txi == true))
            {
                /* Only supports transfer if the total data length is greater than one.*/
                if (p_ctrl->total > 1)
                {
                    if (p_ctrl->loaded == 0)
                    {
                        /* The transfer data full interrupt should be disabled when performing transfer. */
                        R_BSP_IrqDisable(p_ctrl->p_cfg->txi_irq);
                        p_ctrl->p_cfg->p_transfer_tx->p_api->reset(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                                   (void *) (p_ctrl->p_buff + 1),
                                                                   (uint8_t *) (p_iic_slave_tx_buffer),
                                                                   (uint16_t) (p_ctrl->total - 1));
                    }
                    else
                    {
                        /* Reload the remaining data.*/
                        p_ctrl->loaded            += p_ctrl->total - 1;
                        p_ctrl->transaction_count += p_ctrl->total - 1;
                        p_ctrl->remain             = 0;
                    }

                    if (p_ctrl->total == p_ctrl->loaded)
                    {
                        /* When all data has been read, transfer is disabled*/
                        if (p_ctrl->p_cfg->p_transfer_tx != NULL)
                        {
                            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer =
                                IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
                            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src  = (uint32_t *) NULL;
                            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest = (uint32_t *) NULL;
                            p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
                            p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                                             p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
                            iic_interrupt_handling(p_ctrl->p_cfg->txi_irq);
                            p_ctrl->activation_on_txi = false;
                        }
                    }
                }

                if (p_ctrl->loaded == 0)
                {
                    p_ctrl->p_reg->DRT_b.DRT = p_ctrl->p_buff[p_ctrl->loaded];
                    p_ctrl->loaded++;
                    p_ctrl->remain--;

                    /* Keep track of the the actual number of transactions */
                    p_ctrl->transaction_count++;
                }
            }
            else
#endif
            {
                /* Update data to buffer for transmit */
                p_ctrl->p_reg->DRT_b.DRT = p_ctrl->p_buff[p_ctrl->loaded];
                p_ctrl->loaded++;

                /* Keep track of the the actual number of transactions */
                p_ctrl->transaction_count++;
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Setup the Slave Read/Write transaction by issuing RX Request or TX request to the application via callback.
 *
 * @param      p_ctrl          Pointer to the control structure.
 * @param[in]  slave_event     Slave event to be reported via callback.
 **********************************************************************************************************************/
static void iic_slave_initiate_transaction (iic_slave_instance_ctrl_t * p_ctrl, i2c_slave_event_t slave_event)
{
    /* Set the status flag to ensure this conditional clause execution only once */
    p_ctrl->notify_request = true;

    /* Enable the Error ISR for servicing timeout,arbitration loss and a NACK detection conditions. */
    p_ctrl->p_reg->IER = (uint8_t) ((uint8_t) IIC_TMO_EN_BIT |
                                    (uint8_t) IIC_ALD_EN_BIT |
                                    (uint8_t) IIC_NAK_EN_BIT |
                                    (uint8_t) IIC_RXI_EN_BIT |
                                    (uint8_t) IIC_TXI_EN_BIT);

    /* Invoke callback for the user to call a valid API. */
    iic_slave_callback_request(p_ctrl, slave_event);

    /* Check if correct API is called here  Check direction (API called) against slave event requested (ISR invoked) */
    if (!(((IIC_SLAVE_TRANSFER_DIR_MASTER_WRITE_SLAVE_READ ==
            p_ctrl->direction) &&
           ((I2C_SLAVE_EVENT_RX_REQUEST == slave_event) || (I2C_SLAVE_EVENT_GENERAL_CALL == slave_event))) ||
          ((IIC_SLAVE_TRANSFER_DIR_MASTER_READ_SLAVE_WRITE ==
            p_ctrl->direction) && (I2C_SLAVE_EVENT_TX_REQUEST == slave_event))))
    {
        /* In case MasterWriteSlaveRead API is NOT called to service Master write operation a NACK is
         * issued from the RXI ISR (which is fired once) and the bus is released.
         *
         * In case MasterWriteSlaveRead API is NOT called to service Master General Call operation a NACK is
         * issued from the RXI ISR (which is fired once) and the bus is released.
         *
         * In case MasterReadSlaveWrite API is NOT called to service Master read operation the TXI will fire once,
         * no data will be written to ICDRT and the master will read oxFF for every byte it tries to read.
         *
         * For both the cases above the slave callback is invoked with I2C_SLAVE_EVENT_ABORTED
         * event to notify the user application.
         *
         */
    }
    else
    {
        /* Enable start interrupt to detect restart condition t the end of the current transaction. */
        if (!p_ctrl->start_interrupt_enabled)
        {
            /* Enable the Start condition detection to trigger STI ISR */

            /* Since address match is detected, enable STOP and RESTART detection for Master Read Slave Write.
             * This must be done conditionally only for Master Read Slave Write to prevent clearing the start bit
             * in case a restart occurred (and got captured) while in the user callback.
             * This capturing is made possible in the 'iic_rxi_slave' after the dummy read.
             */
            if (IIC_SLAVE_TRANSFER_DIR_MASTER_READ_SLAVE_WRITE == p_ctrl->direction)
            {
                /* Clear the Start and Stop condition flag for Slave Read/Write operation */
                p_ctrl->p_reg->SR2 &= ((uint8_t) ~((uint8_t) ICSR2_STOP_BIT | (uint8_t) ICSR2_START_BIT));
            }

            /* Enable the Start and Stop condition detection interrupt */
            p_ctrl->p_reg->IER = (uint8_t) ((uint8_t) IIC_STP_EN_BIT |
                                            (uint8_t) IIC_STR_EN_BIT |
                                            (uint8_t) IIC_TMO_EN_BIT |
                                            (uint8_t) IIC_ALD_EN_BIT |
                                            (uint8_t) IIC_NAK_EN_BIT |
                                            (uint8_t) IIC_RXI_EN_BIT |
                                            (uint8_t) IIC_TXI_EN_BIT);

            p_ctrl->start_interrupt_enabled = true;
        }
    }
}

/******************************************************************************************************************//**
 * Single point for managing the logic around notifying a transfer has finished.
 *
 * @param[in]       p_ctrl            Pointer to the control structure.
 * @param[in]       slave_event       The slave event code to pass to the callback.
 *********************************************************************************************************************/
static void iic_slave_notify (iic_slave_instance_ctrl_t * const p_ctrl, i2c_slave_event_t const slave_event)
{
    /* Clear all interrupt bits, Only enable TXI and RXI interrupts for the next transaction. */
    p_ctrl->p_reg->IER = (uint8_t) ((uint8_t) IIC_RXI_EN_BIT | (uint8_t) IIC_TXI_EN_BIT);

    /* Reset the status flags */
    p_ctrl->notify_request = false;
    p_ctrl->do_dummy_read  = false;

    /* Disable timeout function */
    p_ctrl->p_reg->FER_b.TMOE = 0;

    /* Check if the transaction ended with a stop (or restart) */
    if (p_ctrl->transaction_completed)
    {
        /* Clear all status flags */
        p_ctrl->p_reg->SR2 = 0x00;
    }
    /*
     * Since the transaction resulted in an error or is not completed at the master (this is a bus hang situation)
     * the slave must do an internal reset to release the bus.
     * Refer the note in Section 'I2C Bus Control Register 1 (ICCR1)' under
     * 'IICRST bit (IIC-Bus Interface Internal Reset)' description of the user's manual.
     */
    else
    {
        /* Internal reset */
        p_ctrl->p_reg->CR1 =
            (uint8_t) (IIC_SLAVE_ICCR1_ICE_BIT_MASK | IIC_SLAVE_ICCR1_IICRST_BIT_MASK |
                       IIC_SLAVE_PRV_SCL_SDA_NOT_DRIVEN);

        /* Release IIC from internal reset */
        p_ctrl->p_reg->CR1 = (uint8_t) (IIC_SLAVE_ICCR1_ICE_BIT_MASK | IIC_SLAVE_PRV_SCL_SDA_NOT_DRIVEN);
    }

    /* Save transaction count */
    uint32_t transaction_count = p_ctrl->transaction_count;

    /* Reset the transaction count here */
    p_ctrl->transaction_count = 0U;

    p_ctrl->direction = IIC_SLAVE_TRANSFER_DIR_NOT_ESTABLISHED;

    /* Invoke the callback */
    r_iic_slave_call_callback(p_ctrl, slave_event, transaction_count);
}

/*******************************************************************************************************************//**
 * Deliver callback. Timeouts are disabled causing clock to stretch.
 *
 * @param      p_ctrl          Pointer to the control structure.
 * @param[in]  slave_event     Slave event to be reported via callback.
 **********************************************************************************************************************/
static void iic_slave_callback_request (iic_slave_instance_ctrl_t * const p_ctrl, i2c_slave_event_t slave_event)
{
    p_ctrl->direction = IIC_SLAVE_TRANSFER_DIR_NOT_ESTABLISHED;

    /* Disable timeout function */
    p_ctrl->p_reg->FER_b.TMOE = 0;

    /* Invoke the callback to notify the read request.
     * The application must call MasterWriteSlaveRead API in the callback.*/
    r_iic_slave_call_callback(p_ctrl, slave_event, p_ctrl->transaction_count);

    /* Allow timeouts to be generated on the low value of SCL using long count mode */
    p_ctrl->p_reg->MR2 = IIC_SLAVE_BUS_MODE_REGISTER_2_MASK;

    /* Enable timeout function */
    p_ctrl->p_reg->FER_b.TMOE = 1;
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to iic slave instance control block
 * @param[in]     event      Event code
 * @param[in]     transaction_count      Transaction count for iic slave
 **********************************************************************************************************************/
static void r_iic_slave_call_callback (iic_slave_instance_ctrl_t * p_ctrl,
                                       i2c_slave_event_t           event,
                                       uint32_t                    transaction_count)
{
    i2c_slave_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    i2c_slave_callback_args_t * p_args = p_ctrl->p_callback_memory;
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

    p_args->bytes     = transaction_count;
    p_args->event     = event;
    p_args->p_context = p_ctrl->p_context;

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
}

#if IIC_SLAVE_CFG_TRANSFER_ENABLE

/*******************************************************************************************************************//**
 * Transmit data empty interrupt routine called when the transfer is completed.
 *
 * @param[in]     p_args     Pointer to callback.
 **********************************************************************************************************************/
void iic_slave_txi_transfer_callback (transfer_callback_args_t * p_args)
{
    IRQn_Type irq = *(IRQn_Type *) (p_args->p_context);

    /* Recover ISR context saved in open. */
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    if (NULL != p_args)
    {
        iic_txi_slave(p_ctrl);
    }
}

#endif
#if IIC_SLAVE_CFG_TRANSFER_ENABLE

/*******************************************************************************************************************//**
 * Receive data interrupt routine called when the transfer is completed.
 *
 * @param[in]     p_args     Pointer to callback.
 **********************************************************************************************************************/
void iic_slave_rxi_transfer_callback (transfer_callback_args_t * p_args)
{
    IRQn_Type irq = *(IRQn_Type *) (p_args->p_context);
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    if (NULL != p_args)
    {
        /* Now that the transfer using transfer is finished, enable the corresponding IRQ. */
        iic_rxi_slave(p_ctrl);
    }
}

#endif

/******************************************************************************************************************//**
 * Handles the transmission end interrupt when operating as a slave.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 *********************************************************************************************************************/
static void iic_tei_slave (iic_slave_instance_ctrl_t * p_ctrl)
{
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
    if (NULL != p_ctrl->p_cfg->p_transfer_rx)
    {
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);

        p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                         p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);

        iic_interrupt_handling(p_ctrl->p_cfg->rxi_irq);
        p_ctrl->activation_on_rxi = false;
    }

    if (NULL != p_ctrl->p_cfg->p_transfer_tx)
    {
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);

        p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                         p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);

        iic_interrupt_handling(p_ctrl->p_cfg->txi_irq);
        p_ctrl->activation_on_txi = false;
    }
#endif

    /* Check if ACK has been detected from master expecting further data */
    if (0UL == p_ctrl->p_reg->MR3_b.ACKBR)
    {
        iic_slave_callback_request(p_ctrl, I2C_SLAVE_EVENT_TX_MORE_REQUEST);

        if (IIC_SLAVE_TRANSFER_DIR_MASTER_READ_SLAVE_WRITE != p_ctrl->direction)
        {
            /* MasterReadSlaveWrite API was not called in the callback.
             * Master will read 0xFF for the remaining byte(s) for this transaction.
             */
        }
        else
        {
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_tx)
            {
                p_ctrl->activation_on_txi = false;
            }
#endif
            p_ctrl->p_reg->DRT_b.DRT = p_ctrl->p_buff[p_ctrl->loaded];
            p_ctrl->loaded++;
            p_ctrl->transaction_count++;
            p_ctrl->remain--;
        }
    }

    /* Disable the interrupt as finish the transfer */
    p_ctrl->p_reg->IER_b.TEIE = 0U;

    /* Wait for the value to reflect at the peripheral. */
    while (0UL != p_ctrl->p_reg->IER_b.TEIE)
    {
        /* Do nothing */
    }
}

/******************************************************************************************************************//**
 * Handles the NACK reception interrupt when operating as a slave.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 *********************************************************************************************************************/
static void iic_naki_slave (iic_slave_instance_ctrl_t * p_ctrl)
{
    uint8_t error_events = IIC_SLAVE_STATUS_REGISTER_2_ERR_MASK & p_ctrl->p_reg->SR2;

    /* NACK detected */
    if (error_events & ICSR2_NACKF_BIT)
    {
        /* NACK interrupt will be triggered on MasterReadSlaveWrite operation.
         * Do dummy read to release SCL
         * Refer Section "Slave Transmit Operation" of the user's manual.
         */
        volatile uint32_t dummy_read = p_ctrl->p_reg->DRR_b.DRR;
        FSP_PARAMETER_NOT_USED(dummy_read);

        /* Disable NACK interrupt, this is required since clearing NACK flag only on detection of STOP bit or
         * when a timeout occurs. Not clearing the flag will cause error interrupt to get triggered again.
         */
        p_ctrl->p_reg->IER &= ~(uint32_t) IIC_NAK_EN_BIT;
    }
}

/******************************************************************************************************************//**
 * Handles Start and Stop condition detection interrupt when operating as a slave.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 *********************************************************************************************************************/
static void iic_sti_spi_slave (iic_slave_instance_ctrl_t * p_ctrl)
{
    uint8_t error_events = IIC_SLAVE_STATUS_REGISTER_2_ERR_MASK & p_ctrl->p_reg->SR2;
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
    transfer_properties_t get_info;
#endif

    /* Restart condition or stop detected, a valid end of transaction */
    if (error_events & ICSR2_START_STOP_BIT)
    {
        i2c_slave_event_t i2c_event = I2C_SLAVE_EVENT_ABORTED;

        /* In case of restart or stop, set the transaction_completed flag */
        p_ctrl->transaction_completed = true;

        /* Set the I2C event */
        if (IIC_SLAVE_TRANSFER_DIR_MASTER_WRITE_SLAVE_READ == p_ctrl->direction)
        {
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
            if (NULL != p_ctrl->p_cfg->p_transfer_rx)
            {
                /* Check condition when start interrupt enable */
                if ((p_ctrl->total > p_ctrl->loaded) && (true == p_ctrl->activation_on_rxi))
                {
                    p_ctrl->p_cfg->p_transfer_rx->p_api->infoGet(p_ctrl->p_cfg->p_transfer_rx->p_ctrl, &get_info);
                    p_ctrl->transaction_count += p_ctrl->total - 1 - get_info.transfer_length_remaining;
                }
                else
                {
                    /* Do nothing */
                }
            }
#endif
            i2c_event = I2C_SLAVE_EVENT_RX_COMPLETE;
        }
        else
        {
            i2c_event = I2C_SLAVE_EVENT_TX_COMPLETE;

            /* Decrement the transaction count when slave configured to write more data than master requested.
             * Addresses the exception raised from double buffer hardware implementation */
            if (p_ctrl->total > p_ctrl->loaded)
            {
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
                if ((NULL != p_ctrl->p_cfg->p_transfer_tx) && (p_ctrl->activation_on_txi == true))
                {
                    p_ctrl->p_cfg->p_transfer_tx->p_api->infoGet(p_ctrl->p_cfg->p_transfer_tx->p_ctrl, &get_info);
                    p_ctrl->transaction_count += get_info.transfer_length_remaining + 1;
                }
#endif
                p_ctrl->transaction_count -= 1U;
            }
        }

#if IIC_SLAVE_CFG_TRANSFER_ENABLE
        if (NULL != p_ctrl->p_cfg->p_transfer_rx)
        {
            p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
            p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
            p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
            p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
            p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                             p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
            iic_interrupt_handling(p_ctrl->p_cfg->rxi_irq);
            p_ctrl->activation_on_rxi = false;
        }

        if (NULL != p_ctrl->p_cfg->p_transfer_tx)
        {
            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
            p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
            p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
            p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                             p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
            iic_interrupt_handling(p_ctrl->p_cfg->txi_irq);
            p_ctrl->activation_on_txi = false;
        }
#endif

        /* Notify the user */
        iic_slave_notify(p_ctrl, i2c_event);
    }
}

/******************************************************************************************************************//**
 * Handles Arbitration-Lost interrupt when operating as a slave.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 *********************************************************************************************************************/
static void iic_ali_tmoi_slave (iic_slave_instance_ctrl_t * p_ctrl)
{
    uint8_t error_events = IIC_SLAVE_STATUS_REGISTER_2_ERR_MASK & p_ctrl->p_reg->SR2;

    /* Arbitration loss or timeout detected */
    if (error_events & ICSR2_AL_TMOF_BIT)
    {
        /* Clear the stop flag. This indicates an error. */
        p_ctrl->transaction_completed = false;

        iic_slave_notify(p_ctrl, I2C_SLAVE_EVENT_ABORTED);
    }
}

/******************************************************************************************************************//**
 * Handles the receive data full interrupt when operating as a slave.
 *
 * @param[in]       p_ctrl     The target IIC block's control block.
 *********************************************************************************************************************/
static void iic_rxi_slave (iic_slave_instance_ctrl_t * p_ctrl)
{
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
    transfer_instance_t const * p_transfer = p_ctrl->p_cfg->p_transfer_rx;
#endif

    /* Perform dummy read after an address match detection. */
    if (!p_ctrl->do_dummy_read)
    {
        p_ctrl->do_dummy_read = true;

        volatile uint8_t address_and_intent = p_ctrl->p_reg->DRR_b.DRR;

        /* The below code enables/services 0 byte writes from Master.*/

        /* Since address match detected, enable STOP detection in case of Master Read Slave Write Operation;
         * and enable STOP and START (RESTART) detection in case of Master Write Slave Read Operation.
         * This is done so that RESTART handling is not missed if the user callback takes long and the Master
         * issues the restart on the bus.
         */
        if (address_and_intent & 1U)
        {
            p_ctrl->p_reg->SR2 &= (uint8_t) ~(ICSR2_STOP_BIT);
        }
        else
        {
            p_ctrl->p_reg->SR2 &= ((uint8_t) ~((uint8_t) ICSR2_STOP_BIT | (uint8_t) ICSR2_START_BIT));
        }

        p_ctrl->p_reg->IER = (uint8_t) ((uint8_t) IIC_RXI_EN_BIT |
                                        (uint8_t) IIC_TXI_EN_BIT |
                                        (uint8_t) IIC_STR_EN_BIT |
                                        (uint8_t) IIC_STP_EN_BIT);
    }
    else
    {
        /* Check if the read request event has been notified through callback, if not provide the callback */
        if (!p_ctrl->notify_request)
        {
            /* Check if this is a General Call by Master */
            i2c_slave_event_t receive_callback_event =
                (R_RIIC0_SR1_GCA_Msk ==
                 (p_ctrl->p_reg->SR1 &
                  R_RIIC0_SR1_GCA_Msk)) ? (I2C_SLAVE_EVENT_GENERAL_CALL) : (I2C_SLAVE_EVENT_RX_REQUEST);
            iic_slave_initiate_transaction(p_ctrl, receive_callback_event);
        }

#if IIC_SLAVE_CFG_PARAM_CHECKING_ENABLE

        /* Proceed reading data */
        if (IIC_SLAVE_TRANSFER_DIR_MASTER_WRITE_SLAVE_READ != p_ctrl->direction)
        {
            /* If the user application incorrectly handles Master Write, send a NACK to exit the transaction. */
            /* Do not dummy read here to allow slave to timeout */
            p_ctrl->p_reg->MR3_b.ACKWP = 1;
            p_ctrl->p_reg->MR3_b.ACKBT = 1;
            p_ctrl->p_reg->MR3_b.ACKWP = 0;
        }
        else
#endif
        {
            if (0U == p_ctrl->total)   /* Send NACK */
            {
                /* Slave is sending a NACK. */
                /* Do dummy read to release SCL */
                volatile uint8_t dummy_read = p_ctrl->p_reg->DRR_b.DRR;
                FSP_PARAMETER_NOT_USED(dummy_read);

                /* Set the response as NACK, since slave is not setup for reading any data from master at this time.
                 * This is an intentional way to let master know that the slave receiver cannot
                 * accept any data and hence should eventually result in I2C_SLAVE_EVENT_RX_COMPLETE.
                 */
                p_ctrl->p_reg->MR3_b.ACKBT = 1;
                p_ctrl->p_reg->MR3_b.ACKWP = 0;
            }
            /* If master is requesting still more data than configured to be read, notify
             * with a read more event in callback */
            else if (p_ctrl->total == p_ctrl->loaded)
            {
                iic_slave_callback_request(p_ctrl, I2C_SLAVE_EVENT_RX_MORE_REQUEST);
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
                if (NULL != p_ctrl->p_cfg->p_transfer_rx)
                {
                    p_ctrl->activation_on_rxi = false;
                }
#endif
                if (IIC_SLAVE_TRANSFER_DIR_MASTER_WRITE_SLAVE_READ != p_ctrl->direction)
                {
                    /* If the user application incorrectly handles Master Write, send a NACK to exit the transaction. */
                    /* Do not dummy read here to allow slave to timeout */
                    p_ctrl->p_reg->MR3_b.ACKWP = 1;
                    p_ctrl->p_reg->MR3_b.ACKBT = 1;
                    p_ctrl->p_reg->MR3_b.ACKWP = 0;
                }
                else
                {
                    if (0U == p_ctrl->total) /* Send NACK */
                    {
                        /* Do dummy read to release SCL */
                        volatile uint8_t dummy_read = p_ctrl->p_reg->DRR_b.DRR;
                        FSP_PARAMETER_NOT_USED(dummy_read);

                        /* Set the response as NACK, since slave is not setup for reading more data from master
                         * at this time.
                         * This is an intentional way to let master know that the slave receiver cannot
                         * accept any more data and hence should eventually result in I2C_SLAVE_EVENT_RX_COMPLETE.
                         **/
                        p_ctrl->p_reg->MR3_b.ACKWP = 1;
                        p_ctrl->p_reg->MR3_b.ACKBT = 1;
                        p_ctrl->p_reg->MR3_b.ACKWP = 0;
                    }
                    else
                    {
                        /* Read data */
                        p_ctrl->p_buff[p_ctrl->loaded++] = p_ctrl->p_reg->DRR_b.DRR;

                        /* Keep track of the the actual number of transactions */
                        p_ctrl->transaction_count++;
                    }
                }
            }
            else
            {
#if IIC_SLAVE_CFG_TRANSFER_ENABLE
                uint32_t volatile const * p_iic_slave_rx_buffer = &(p_ctrl->p_reg->DRR);

                if ((NULL != p_transfer) && (p_ctrl->activation_on_rxi == true))
                {
                    /* Only supports transfer if the total data length is greater than one.*/
                    if (p_ctrl->total > 1)
                    {
                        /* With the first data received, transfer is enabled and the RX interrupt is disabled.*/
                        if (p_ctrl->loaded == 0)
                        {
                            /* The receive data full interrupt should be disabled when performing transfer. */
                            R_BSP_IrqDisable(p_ctrl->p_cfg->rxi_irq);
                            p_ctrl->p_cfg->p_transfer_rx->p_api->enable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
                            p_ctrl->p_cfg->p_transfer_rx->p_api->reset(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                                       (uint8_t *) (p_iic_slave_rx_buffer),
                                                                       (void *) (&p_ctrl->p_buff[p_ctrl->loaded + 1]),
                                                                       (uint16_t) (p_ctrl->total - 1U));
                        }
                        else
                        {
                            /* Reload the remaining data.*/
                            p_ctrl->loaded            += p_ctrl->total - 1;
                            p_ctrl->transaction_count += p_ctrl->total - 1;
                            p_ctrl->remain            -= p_ctrl->total - 1;
                        }

                        if (p_ctrl->total == p_ctrl->loaded)
                        {
                            p_ctrl->activation_on_rxi = false;

                            /* When all data has been read, transfer is disabled*/
                            if (NULL != p_transfer)
                            {
                                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer =
                                    IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
                                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src  = (uint32_t *) NULL;
                                p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest = (uint32_t *) NULL;
                                p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
                                p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                                                 p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
                                iic_interrupt_handling(p_ctrl->p_cfg->rxi_irq);
                            }
                        }
                    }

                    /* The first data is processed without using transfer.*/
                    if (p_ctrl->loaded == 0)
                    {
                        /* Read data */
                        p_ctrl->p_buff[p_ctrl->loaded++] = (uint8_t) p_ctrl->p_reg->DRR_b.DRR;
                        p_ctrl->transaction_count++;
                        p_ctrl->remain--;
                    }
                }
                else
#endif
                {
                    /* Read data */
                    p_ctrl->p_buff[p_ctrl->loaded++] = (uint8_t) p_ctrl->p_reg->DRR_b.DRR;

                    /* Keep track of the the actual number of transactions */
                    p_ctrl->transaction_count++;
                }
            }
        }
    }
}

#if IIC_SLAVE_CFG_TRANSFER_ENABLE

/*******************************************************************************************************************//**
 * Configures IIC related transfer drivers (if enabled).
 *
 * @param[in]   p_ctrl    Pointer to IIC specific control structure
 * @param[in]   p_cfg     Pointer to IIC specific configuration structure
 *
 * @retval      FSP_SUCCESS                Transfer interface initialized successfully.
 * @retval      FSP_ERR_ASSERTION          Pointer to transfer instance for I2C receive in p_cfg is NULL.
 **********************************************************************************************************************/
static fsp_err_t iic_slave_transfer_open (iic_slave_instance_ctrl_t * const p_ctrl, i2c_slave_cfg_t const * const p_cfg)
{
 #if (IIC_SLAVE_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_cfg);
 #endif
    fsp_err_t err = FSP_SUCCESS;

    /* Initialize transfer for RX.*/
    if (NULL != p_cfg->p_transfer_rx)
    {
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->open(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                        p_cfg->p_transfer_rx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->callbackSet(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                               iic_slave_rxi_transfer_callback,
                                                               (void *) &p_ctrl->p_cfg->rxi_irq,
                                                               NULL);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        err = p_ctrl->p_cfg->p_transfer_rx->p_api->disable(p_ctrl->p_cfg->p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err)

        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        err = p_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err)
    }

    /* Initialize transfer for TX.*/
    if (NULL != p_cfg->p_transfer_tx)
    {
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->open(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                        p_cfg->p_transfer_tx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->callbackSet(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                               iic_slave_txi_transfer_callback,
                                                               (void *) &p_ctrl->p_cfg->txi_irq,
                                                               NULL);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        err = p_ctrl->p_cfg->p_transfer_tx->p_api->disable(p_ctrl->p_cfg->p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err)

        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->number_transfer = IIC_SLAVE_TRANSFER_RESET_SRC_DEST_NUM;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_src           = (uint32_t *) NULL;
        p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->p_dest          = (uint32_t *) NULL;
        err = p_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                               p_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err)
    }

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * Clear interrupt status and enable.
 *
 * This function implements Tx/Rx Interrupt Enable.
 *
 **********************************************************************************************************************/
void iic_interrupt_handling (IRQn_Type const irq)
{
    R_BSP_IrqStatusClear(irq);
    R_BSP_IrqEnable(irq);
}

/***********************************************************************************************************************
 * Interrupt service routines
 **********************************************************************************************************************/

/******************************************************************************************************************//**
 * Transmit data empty interrupt routine.
 *
 * This function implements the Transmit buffer empty ISR routine.
 *
 *********************************************************************************************************************/
void iic_slave_txi_isr (void)
{
    /* Save context if RTOS is used */

    FSP_CONTEXT_SAVE

    /* Clear the IR flag */
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    iic_txi_slave(p_ctrl);

    /* Restore context if RTOS is used */

    FSP_CONTEXT_RESTORE
}

/******************************************************************************************************************//**
 * Receive data full interrupt routine.
 *
 * This function implements the IIC Receive buffer full ISR routine.
 *
 *********************************************************************************************************************/
void iic_slave_rxi_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    /* Clear the IR flag */
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    iic_rxi_slave(p_ctrl);

    /* Restore context if RTOS is used */

    FSP_CONTEXT_RESTORE
}

/******************************************************************************************************************//**
 * Event and error slave routine
 *
 * This function implements the IIC Receive buffer full ISR routine.
 *
 *********************************************************************************************************************/

void iic_slave_eri_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* NACK detected */
    if (((1U == p_ctrl->p_reg->SR2_b.NACKF)) && (1U == p_ctrl->p_reg->IER_b.NAKIE))
    {
        iic_naki_slave(p_ctrl);
    }
    /* Restart condition detected, a valid end of transaction */
    else if ((1U == p_ctrl->p_reg->SR2_b.START) && (1U == p_ctrl->p_reg->IER_b.STIE))
    {
        iic_sti_spi_slave(p_ctrl);
    }
    /* Stop condition detected, a valid end of transaction */
    else if ((1U == p_ctrl->p_reg->SR2_b.STOP) && (1U == p_ctrl->p_reg->IER_b.SPIE))
    {
        iic_sti_spi_slave(p_ctrl);
    }
    /* Arbitration loss detected */
    else if ((1U == p_ctrl->p_reg->SR2_b.AL) && (1U == p_ctrl->p_reg->IER_b.ALIE))
    {
        iic_ali_tmoi_slave(p_ctrl);
    }
    /* Timeout loss detected */
    else if ((1U == p_ctrl->p_reg->SR2_b.TMOF) && (1U == p_ctrl->p_reg->IER_b.TMOIE))
    {
        iic_ali_tmoi_slave(p_ctrl);
    }
    else
    {
        /* Do Nothing */
    }

    /* Save context if RTOS is used */

    FSP_CONTEXT_SAVE
}

/****************************************************************************************************************//**
 * Transmit end interrupt routine.
 *
 * This function implements the IIC Transmission End ISR routine.
 *
 ********************************************************************************************************************/
void iic_slave_tei_isr (void)
{
    /* Save context if RTOS is used */

    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    iic_slave_instance_ctrl_t * p_ctrl = (iic_slave_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    iic_tei_slave(p_ctrl);

    /* Clear the IR flag */
    R_BSP_IrqStatusClear(irq);

    /* Restore context if RTOS is used */

    FSP_CONTEXT_RESTORE
}
