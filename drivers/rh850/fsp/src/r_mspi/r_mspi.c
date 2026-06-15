/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "r_mspi.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define MSPI_OPEN                    (0x4D535049ULL) // 'MSPI' in ASCII
#define MSPI_NTFC_CLEAR              (0x1)           // Clear interrupt flags
#define MSPI_CTL1_CLEAR              (0x00)          // Clear CTL1 configuration
#define MSPI_CTL2_CLEAR              (0x00)          // Clear CTL2 configuration
#define MSPI_SSEL_CLEAR              (0x00)          // Clear CSR configuration
#define MSPI_CFG0_CLEAR              (0x00700000)    // Clear CFG0 configuration
#define MSPI_CFG1_CLEAR              (0x01000000)    // Clear CFG1 configuration
#define MSPI_CFG2_CLEAR              (0x0020)        // Clear CFG2 configuration
#define MSPI_TGCTL_CLEAR             (0x0000U)       // Clear DMAC/DTS activation source
#define MSPI_TGDMAALT_CLEAR          (0x0000U)       // Clear DMA alternative setting
#define MSPI_TGDTSALT_CLEAR          (0x0000U)       // Clear DTS alternative setting
#define MSPI_BR_CLEAR                (0x01)          // Clear Baud rate setting
#define MSPI_CFSET_CLEAR             (0x0001)        // Clear CFSET setting
#define MSPI_SEUP_CLEAR              (0x0001)        // Clear SEUP setting
#define MSPI_HOLD_CLEAR              (0x0001)        // Clear HOLD setting
#define MSPI_IDLE_CLEAR              (0x0001)        // Clear IDLE setting
#define MSPI_INDA_CLEAR              (0x00)          // Clear INDA setting
#define MSPI_RASTAD_CLEAR            (0x00)          // Clear RASTAD setting
#define MSPI_INIT_RAM_ADDR           (0x1000)        // Initial RAM address
#define MSPI_STAGE_SIZE_8            (0x8)           // FIFO stage size is 8 bits
#define MSPI_STAGE_SIZE_16           (0x10)          // FIFO stage size is 16 bits
#define MSPI_STAGE_SIZE_32           (0x20)          // FIFO stage size is 32 bits
#define MSPI_SSEL_SLAVE              (0x0000)        // Clear SSEL setting
#define ALIGNMENT_BOUNDARY_BYTES     (4U)            // Alignment boundary in bytes
#define ALIGNMENT_MASK               (0x3U)          // Alignment mask for 4 bytes
#define MSPI_PAIR_SIZE               (2U)            // Number of MSPI units per pair
#define MSPI0_UNIT                   (0U)            // MSPI unit 0
#define MSPI1_UNIT                   (1U)            // MSPI unit 1
#define MSPI2_UNIT                   (2U)            // MSPI unit 2
#define MSPI3_UNIT                   (3U)            // MSPI unit 3
#define MSPI_LENGTH_PER_COUNT_1      (1U)            // Length per count for 1 byte
#define MSPI_LENGTH_PER_COUNT_2      (2U)            // Length per count for 2 bytes
#define MSPI_LENGTH_PER_COUNT_4      (4U)            // Length per count for 4 bytes
#define MSPI_LENGTH_PER_COUNT_8      (8U)            // Length per count for 8 byte
#define MSPI_TRANSFER_ENABLE         (1U)            // MSPI enable transfer support
#define MSPI_CGTL_CHANNEL_GAP        (4U)            // MSPI channel gap in register MSPITGCTL
#define MSPI_CGTL_UNIT_GAP           (10U)           // MSPI unit gap in register MSPITGCTL
#define MSPI_TRANSFER_INV_CHANNEL    (31U)           // MSPI channel invalid for Trigger Alternative
#define MSPI_MINIMUM_CYLCE_INIT      (4U)            // Minimum cycles need to wait for MSPI initialization

/* Value of base address  Unit 0 and Unit 1 */
#define MSPI_BASE_ADDRESS_START(unit)    ((MSPI0_UNIT == unit) ? R_MSPI0_BASE : R_MSPI1_BASE)

/* Offset between MSPI register groups (even unit4-unit2, odd unit3-unit1) */
#define MSPI_REG_SIZE(unit)              (((R_MSPI4_BASE - R_MSPI2_BASE) * \
                                           (!(unit % MSPI_PAIR_SIZE)))     \
                                          +                                \
                                          ((R_MSPI5_BASE - R_MSPI3_BASE) * \
                                           ((unit % MSPI_PAIR_SIZE))))

/* Gap between two MSPI unit in the same MSPI group */
#define MSPI_REG_GROUP_GAP    (R_MSPI3_BASE - R_MSPI2_BASE)

/* Gap between two MSPI_INTF unit in the same MSPI_INTF group */
#define MSPI_INTF_REG_SIZE(unit)    (((R_MSPI2_INTF_BASE - R_MSPI0_INTF_BASE) * \
                                      (!(unit % MSPI_PAIR_SIZE)))               \
                                     +                                          \
                                     ((R_MSPI3_INTF_BASE - R_MSPI1_INTF_BASE) * \
                                      ((unit % MSPI_PAIR_SIZE))))

/* Gap between two MSPI_INTF unit in the same MSPI_INTF group */
#define MSPI_INTF_REG_GROUP_GAP    (R_MSPI1_INTF_BASE - R_MSPI0_INTF_BASE)

#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)

/* Macro use to disable IRQ when Ored function is provided */
 #define MSPI_DISABLE_IRQ(ored_field, irq_field)   \
    do {                                           \
        if (BSP_ORED_MSPI_DISABLE == (ored_field)) \
        {                                          \
            R_BSP_IrqDisable(irq_field);           \
        }                                          \
        else                                       \
        {                                          \
            R_BSP_ORed_IrqCfgDisable(ored_field);  \
        }                                          \
    } while (0)

/* Macro use to enable IRQ when Ored function is provided */
 #define MSPI_ENABLE_IRQ(ored_field, irq_field, irq_ipl, p_instance_ctrl)  \
    do {                                                                   \
        if (BSP_ORED_MSPI_DISABLE == (ored_field))                         \
        {                                                                  \
            R_BSP_IrqCfgEnable(irq_field, irq_ipl, p_instance_ctrl);       \
        }                                                                  \
        else                                                               \
        {                                                                  \
            R_BSP_ORed_IrqCfgEnable(ored_field, irq_ipl, p_instance_ctrl); \
        }                                                                  \
    } while (0)
#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static void r_mspi_hw_config(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_interrupt_config(mspi_instance_ctrl_t * p_instance_ctrl);

static void r_mspi_transmit(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_receive(mspi_instance_ctrl_t * p_instance_ctrl);

static fsp_err_t r_mspi_start_read_write(spi_ctrl_t * const    p_ctrl,
                                         void const          * p_src,
                                         void                * p_dest,
                                         uint32_t const        length,
                                         spi_bit_width_t const bit_width);
static fsp_err_t r_mspi_transfer_config(mspi_instance_ctrl_t * p_instance_ctrl, spi_cfg_t const * const p_cfg);

static void r_mspi_buffer_write(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_buffer_read(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_fifo_write(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_fifo_read(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_direct_write(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_direct_read(mspi_instance_ctrl_t * p_instance_ctrl);

static uint8_t r_mspi_align_ram(uint32_t length, uint32_t ram_init, uint32_t bitwidth);
static uint8_t r_mspi_count_byte(uint32_t bitwidth);
static uint8_t r_mspi_fifo_refill_stage(mspi_instance_ctrl_t * p_instance_ctrl);

static void r_mspi_read_register(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_write_register(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_call_callback(mspi_instance_ctrl_t * p_instance_ctrl, spi_event_t event);

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
void mspi_tx_transfer_callback(transfer_callback_args_t * p_transfer_args);
void mspi_rx_transfer_callback(transfer_callback_args_t * p_transfer_args);
void mspi_fe_transfer_callback(transfer_callback_args_t * p_transfer_args);

static transfer_size_t r_mspi_cal_transfer_size(uint32_t bitwidth);

static uint8_t r_mspi_dma_alternative(mspi_instance_ctrl_t * p_instance_ctrl);
static uint8_t r_mspi_dts_alternative(mspi_instance_ctrl_t * p_instance_ctrl);

#endif

BSP_INTERRUPT_ATTRIBUTE void mspi_txi_isr(void);
BSP_INTERRUPT_ATTRIBUTE void mspi_rxi_isr(void);
BSP_INTERRUPT_ATTRIBUTE void mspi_err_isr(void);
BSP_INTERRUPT_ATTRIBUTE void mspi_fe_isr(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
const spi_api_t g_spi_on_mspi =
{
    .open        = R_MSPI_Open,
    .read        = R_MSPI_Read,
    .write       = R_MSPI_Write,
    .writeRead   = R_MSPI_WriteRead,
    .close       = R_MSPI_Close,
    .callbackSet = R_MSPI_CallbackSet
};

/*******************************************************************************************************************//**
 * @addtogroup SPI
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * This functions initializes a unit for MSPI communication mode. Implements @ref spi_api_t::open.
 *
 * This function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Configures the peripheral registers according to the configuration.
 * - Initialize the control structure for use in other @ref SPI_API functions.
 *
 * @retval  FSP_SUCCESS                    Unit opened successfully.
 * @retval  FSP_ERR_ASSERTION              Pointer to MSPI control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN           Control block has already been opened or unit is being used by another
 *                                         instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_IP_UNIT_NOT_PRESENT    Requested unit does not exist on this device.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_Open (spi_ctrl_t * p_ctrl, spi_cfg_t const * const p_cfg)
{
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) p_ctrl;
    mspi_extended_cfg_t  * p_extend        = (mspi_extended_cfg_t *) p_cfg->p_extend;
#if MSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MSPI_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(BSP_FEATURE_MSPI_MAX_UNIT > p_extend->unit, FSP_ERR_IP_UNIT_NOT_PRESENT);
#endif

    /* Get the register address of the unit. */
    p_instance_ctrl->p_cfg             = p_cfg;
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    if (MSPI2_UNIT > p_extend->unit)
    {
        /* Get the register address of the unit 0-1-2 */
        p_instance_ctrl->p_regs = (R_MSPI0_Type *) MSPI_BASE_ADDRESS_START(p_extend->unit);
    }
    else
    {
        /* For the remaining unit, address is calculate from unit 0 base address */
        p_instance_ctrl->p_regs =
            (R_MSPI0_Type *) (R_MSPI2_BASE + (MSPI_REG_GROUP_GAP * (p_extend->unit % MSPI_PAIR_SIZE)) +
                              (MSPI_REG_SIZE(p_extend->unit) * ((p_extend->unit - MSPI2_UNIT) / MSPI_PAIR_SIZE)));
    }

    /* Calculate address of MSPIINTF register */
    p_instance_ctrl->p_regs_intf =
        (R_MSPI0_INTF_Type *) (R_MSPI0_INTF_BASE + (MSPI_INTF_REG_GROUP_GAP * (p_extend->unit % MSPI_PAIR_SIZE)) +
                               (MSPI_INTF_REG_SIZE(p_extend->unit) * ((p_extend->unit) / MSPI_PAIR_SIZE)));

    /* Calculate address of MSPITG register */
    p_instance_ctrl->p_regs_tg = (R_MSPITG_Type *) R_MSPITG_BASE;

    /* Configure hardware registers according to the r_spi_api configuration structure. */
    r_mspi_hw_config(p_instance_ctrl);

    /* Configure for transfer support */
    r_mspi_transfer_config(p_instance_ctrl, p_cfg);

    /* Enable interrupt*/
    r_mspi_interrupt_config(p_instance_ctrl);

    /* Mark driver as open */
    p_instance_ctrl->open = MSPI_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Aborts any in progress transfers. Disables interrupts, receiver, and transmitter.  Closes lower level transfer
 * drivers if used. Removes power.
 *
 * @retval  FSP_SUCCESS              Unit successfully closed.
 * @retval  FSP_ERR_ASSERTION        Pointer to MSPI control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_Close (spi_ctrl_t * const p_ctrl)
{
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) p_ctrl;
#if MSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MSPI_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    mspi_extended_cfg_t              * p_extend         = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    R_MSPI0_MSPI0_CH_Type            * p_mspi_regs      = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;

    uint8_t channel = p_instance_ctrl->p_cfg->channel;
    uint8_t unit    = p_extend->unit;

    /* Stop communicate in MSPI channel */
    p_mspi_regs[channel].CSTC_b.CHENC = R_MSPI0_MSPI0_CH_CSTC_CHENC_Msk;

    /* Clear all register status of MSPI. */
    p_mspi_regs[channel].CESTC |= R_MSPI0_MSPI0_CH_CESTC_PEC_Msk | R_MSPI0_MSPI0_CH_CESTC_CEC_Msk |
                                  R_MSPI0_MSPI0_CH_CESTC_DCEC_Msk | R_MSPI0_MSPI0_CH_CESTC_OVRUEC_Msk |
                                  R_MSPI0_MSPI0_CH_CESTC_OVWREC_Msk | R_MSPI0_MSPI0_CH_CESTC_OVREEC_Msk;
    p_mspi_regs[channel].SSEL   = MSPI_SSEL_CLEAR;
    p_mspi_regs[channel].CFG0   = MSPI_CFG0_CLEAR;
    p_mspi_regs[channel].CFG1   = MSPI_CFG1_CLEAR;
    p_mspi_regs[channel].CFG2   = MSPI_CFG2_CLEAR;
    p_mspi_regs[channel].CFG3   = MSPI_BR_CLEAR;
    p_mspi_regs[channel].CFSET  = MSPI_CFSET_CLEAR;
    p_mspi_regs[channel].SEUP   = MSPI_SEUP_CLEAR;
    p_mspi_regs[channel].HOLD   = MSPI_HOLD_CLEAR;
    p_mspi_regs[channel].IDLE   = MSPI_IDLE_CLEAR;
    p_mspi_regs[channel].INDA   = MSPI_INDA_CLEAR;
    p_mspi_regs[channel].RASTAD = MSPI_RASTAD_CLEAR;

    /* Clear all interrupt setting*/
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    MSPI_DISABLE_IRQ(p_extend->com_ored_int, p_instance_ctrl->p_cfg->txi_irq);
    MSPI_DISABLE_IRQ(p_extend->rec_ored_int, p_instance_ctrl->p_cfg->rxi_irq);
    R_BSP_ORed_IrqCfgDisable(p_extend->error_ored_int);
    R_BSP_ORed_IrqCfgDisable(p_extend->frame_ored_int);
#else
    R_BSP_IrqDisable(p_instance_ctrl->p_cfg->txi_irq);
    R_BSP_IrqDisable(p_instance_ctrl->p_cfg->rxi_irq);
    R_BSP_IrqDisable(p_instance_ctrl->p_cfg->tei_irq);
    R_BSP_IrqDisable(p_instance_ctrl->p_cfg->eri_irq);
#endif

    /* Clear all interrupt flag */
    p_mspi_intf_regs[0].INTFC |= MSPI_NTFC_CLEAR << channel;
    p_mspi_intf_regs[1].INTFC |= MSPI_NTFC_CLEAR << channel;
    p_mspi_intf_regs[2].INTFC |= MSPI_NTFC_CLEAR << channel;
    p_mspi_intf_regs[3].INTFC |= MSPI_NTFC_CLEAR << channel;

    /* Disable MSPI function. */
    p_instance_ctrl->p_regs->CTL1 &= MSPI_CTL1_CLEAR;
    p_instance_ctrl->p_regs->CTL2 &= MSPI_CTL2_CLEAR;
    p_instance_ctrl->p_regs->CTL0 &= (uint8_t) ~R_MSPI0_CTL0_EN_Msk;

    /* Disable MSPI transfer register */
#if (BSP_MCU_GROUP_RH850U2Bx)
    if (MSPI_CS_CHANNEL_10 <= channel)
    {
        p_instance_ctrl->p_regs_tg->MSPITGCTL1[unit] &= MSPI_TGCTL_CLEAR;
    }
    else
    {
        p_instance_ctrl->p_regs_tg->MSPITGCTL[unit] &= MSPI_TGCTL_CLEAR;
    }

#else
    p_instance_ctrl->p_regs_tg->MSPITGCTL[unit] &= MSPI_TGCTL_CLEAR;
#endif
    p_instance_ctrl->p_regs_tg->MSPITGDMAALT &= MSPI_TGDMAALT_CLEAR;
    p_instance_ctrl->p_regs_tg->MSPITGDTSALT &= MSPI_TGDTSALT_CLEAR;

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)

    /* Close transfer driver */
    if (NULL != p_instance_ctrl->p_cfg->p_transfer_tx)
    {
        p_instance_ctrl->p_cfg->p_transfer_tx->p_api->close(p_instance_ctrl->p_cfg->p_transfer_tx->p_ctrl);
    }

    if (NULL != p_instance_ctrl->p_cfg->p_transfer_rx)
    {
        p_instance_ctrl->p_cfg->p_transfer_rx->p_api->close(p_instance_ctrl->p_cfg->p_transfer_rx->p_ctrl);
    }
#endif

    /* Mark driver as close */
    p_instance_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function receives data from a MSPI device. Implements @ref spi_api_t::read.
 *
 * The function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Sets up the instance to complete a MSPI read operation.
 *
 * @retval  FSP_SUCCESS                  Data reception successfully ends.
 * @retval  FSP_ERR_ASSERTION            Pointer to MSPI control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The unit has not been opened. Open unit first.
 * @retval  FSP_ERR_INVALID_SIZE         Remaining RAM size is not within the hardware supported range for the
 *                                       configured FIFO stage size.
 * @retval  FSP_ERR_IN_USE               The channel is currently in use.
 * @retval  FSP_ERR_INVALID_ARGUMENT     Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_Read (spi_ctrl_t * const p_ctrl, void * p_dest, uint32_t const length, spi_bit_width_t const bit_width)
{

    /* Call the common helper function to perform MSPI Read operation.*/
    return r_mspi_start_read_write(p_ctrl, NULL, p_dest, length, bit_width);
}

/*******************************************************************************************************************//**
 * This function transmits data to a MSPI device.
 * Implements @ref spi_api_t::write.
 *
 * The function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Sets up the instance to complete a MSPI write operation.
 *
 * @retval  FSP_SUCCESS                  Write operation successfully completed.
 * @retval  FSP_ERR_ASSERTION            NULL pointer to control or source parameters or transfer length is zero.
 * @retval  FSP_ERR_NOT_OPEN             The unit has not been opened. Open the unit first.
 * @retval  FSP_ERR_INVALID_SIZE         Remaining RAM size is not within the hardware supported range for the
 *                                       configured FIFO stage size.
 * @retval  FSP_ERR_IN_USE               The channel is currently in use.
 * @retval  FSP_ERR_INVALID_ARGUMENT     Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_Write (spi_ctrl_t * const    p_ctrl,
                        void const          * p_src,
                        uint32_t const        length,
                        spi_bit_width_t const bit_width)
{

    /* Call the common helper function to perform MSPI Write operation. */
    return r_mspi_start_read_write(p_ctrl, p_src, NULL, length, bit_width);
}

/*******************************************************************************************************************//**
 * This function simultaneously transmits and receive data. Implements @ref spi_api_t::writeRead.
 *
 * The function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Sets up the instance to complete a MSPI writeRead operation.
 *
 * @retval  FSP_SUCCESS                  Write operation successfully completed.
 * @retval  FSP_ERR_ASSERTION            NULL pointer to control or source parameters or transfer length is zero.
 * @retval  FSP_ERR_NOT_OPEN             The unit has not been opened. Open the unit first.
 * @retval  FSP_ERR_INVALID_SIZE         Remaining RAM size is not within the hardware supported range for the
 *                                       configured FIFO stage size.
 * @retval  FSP_ERR_IN_USE               The channel is currently in use.
 * @retval  FSP_ERR_INVALID_ARGUMENT     Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_WriteRead (spi_ctrl_t * const    p_ctrl,
                            void const          * p_src,
                            void                * p_dest,
                            uint32_t const        length,
                            spi_bit_width_t const bit_width)
{
#if MSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);
#endif

    /* Call the common helper function to perform MSPI Read/Write operation. */
    return r_mspi_start_read_write(p_ctrl, p_src, p_dest, length, bit_width);
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements @ref spi_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_CallbackSet (spi_ctrl_t * const          p_ctrl,
                              void (                    * p_callback)(spi_callback_args_t *),
                              void * const                p_context,
                              spi_callback_args_t * const p_callback_memory)
{
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) p_ctrl;

#if (MSPI_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(MSPI_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup SPI)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Hardware configuration for settings given by the configuration structure.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_hw_config (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;

    mspi_extended_cfg_t * p_extend = ((mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    uint8_t               channel  = p_instance_ctrl->p_cfg->channel;
    uint8_t               ctl0     = 0;
    uint32_t              ctl1     = 0;
    uint8_t               ctl2     = 0;

    /* Ensure MSPI unit is disabled before configuration*/
    p_instance_ctrl->p_regs->CTL0 = (uint8_t) ctl0;

    /* Four or more PCLK clock cycles plus four or more MSPInCLK clock cycles are */
    /* necessary if MSPInEN has been cleared and then if MSPInEN has set again.   */
    R_BSP_SoftwareDelay(4 / BSP_CFG_CLOCK_CLK_MSPI_HZ, BSP_DELAY_UNITS_SECONDS);

    /* Enable MSPI function via CTL0 register*/
    ctl0 = R_MSPI0_CTL0_EN_Msk;

    /* Initialize value for CTL1 register */
    ctl1 = (uint32_t) (p_extend->mspi_ctl1.control_register_1);

    /* This register sets the transfer clock frequency for each chip select signal. */
    if (SPI_MODE_MASTER == p_instance_ctrl->p_cfg->operating_mode)
    {
        /* Set channel to master mode */
        ctl1 &= ~R_MSPI0_CTL1_MSSEL_Msk;

        /* Clear MSPInCSIE  */
        ctl1 &= ~(1U << R_MSPI0_CTL1_CSIE_Pos);

        /* Set MSPI configuration setting */
        /* Register sets the transfer clock frequency for each chip select signal. */
        if (MSPI_CHIP_SELECT_STATE_ACTIVE == p_extend->mspi_chip_selection)
        {
            p_mspi_regs[channel].CFG3  = (p_extend->mspi_baudrate_channel);
            p_mspi_regs[channel].SSEL |= (1 << channel);
        }

        /* Register sets the Job mode for each chip select signal.*/
        if (MSPI_JOB_ENABLE == p_extend->mspi_job_enable)
        {
            /*Enable Job mode*/
            p_mspi_regs[channel].SSEL |= (1 << R_MSPI0_MSPI0_CH_SSEL_JOBEN_Pos);
        }
        else
        {
            /*Disable Job mode*/
            p_mspi_regs[channel].SSEL &= ~(1 << R_MSPI0_MSPI0_CH_SSEL_JOBEN_Pos);
        }
    }
    else
    {
        /* Set channel to slave mode */
        ctl1 |= R_MSPI0_CTL1_MSSEL_Msk;

        /* Feature is not supported in slave mode */
        ctl1 &= ~(1U << R_MSPI0_CTL1_SAMP_Pos);

        ctl1 &= ~(3U << R_MSPI0_CTL1_SOLS_Pos);

        ctl1 &= ~((uint32_t) R_MSPI0_CTL1_CSP_Msk);

        /* Set MSPI configuration setting for slave */
        /* Only one chip select (channel 0) is allowed in slave mode */
        /* Disable Job mode in slave mode */
        /* Set MSPInSSELm to 0000H in the slave mode */
        p_mspi_regs[0].SSEL &= MSPI_SSEL_SLAVE;

        /* Register sets the transfer clock frequency for each chip select signal. */
        p_mspi_regs[0].CFG3 = MSPI_CLOCK_MODE_CLK_2;
    }

    /* Checking loop-back mode*/
    if ((MSPI_LOOPBACK_MODE_ENABLE == p_extend->mspi_loopback) &&
        (SPI_MODE_MASTER == p_instance_ctrl->p_cfg->operating_mode))
    {
        /* Enable loop-back mode */
        ctl2 |= R_MSPI0_CTL2_LBM_Msk;
    }
    else
    {
        /* Disable loop-back mode */
        ctl2 &= ~R_MSPI0_CTL2_LBM_Msk;
    }

    /* Checking data consistency*/
    if (MSPI_CONSISTENCY_CHECK_ENABLE == p_extend->mspi_consistency_check)
    {
        /* Enable Data Consistency Check */
        ctl2 |= R_MSPI0_CTL2_DCS_Msk;
    }
    else
    {
        /* Disable Data Consistency Check */
        ctl2 &= ~R_MSPI0_CTL2_DCS_Msk;
    }

    /* Write registers */
    p_instance_ctrl->p_regs->CTL1 = (uint32_t) ctl1;
    p_instance_ctrl->p_regs->CTL2 = (uint8_t) ctl2;
    p_instance_ctrl->p_regs->CTL0 = (uint8_t) ctl0;
}

/*******************************************************************************************************************//**
 * Enable Communication Complete, Receive Complete, Error Communication and Job Complete Interrupt.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_interrupt_config (mspi_instance_ctrl_t * p_instance_ctrl)
{
    mspi_extended_cfg_t       * p_extend      = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    const transfer_instance_t * p_transfer_tx = p_instance_ctrl->p_cfg->p_transfer_tx;
    const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;
    bool enable_tx = false;
    bool enable_rx = false;

    /* Set TX/RX enable flags based on MSPI transfer mode. */
    switch (p_extend->mspi_select)
    {
        /* Select mode transmit/receive */
        case MSPI_SELECTABLE_MODE_TRANSMIT_RECEIVE:
        {
            if (!p_transfer_tx && !p_transfer_rx)
            {
                enable_tx = true;
                enable_rx = true;
            }

            break;
        }

        /* Select mode transmit-only */
        case MSPI_SELECTABLE_MODE_TRANSMIT_ONLY:
        {
            if (!p_transfer_tx)
            {
                enable_tx = true;
            }

            break;
        }

        /* Default case handles receive-only mode. */
        default:
        {
            if (!p_transfer_rx)
            {
                enable_rx = true;
            }

            break;
        }
    }

    /* Enable or disable TX interrupt based on enable_tx flag. */
    if (enable_tx)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        MSPI_ENABLE_IRQ(p_extend->com_ored_int,
                        p_instance_ctrl->p_cfg->txi_irq,
                        p_instance_ctrl->p_cfg->txi_ipl,
                        p_instance_ctrl);
#else
        R_BSP_IrqCfgEnable(p_instance_ctrl->p_cfg->txi_irq, p_instance_ctrl->p_cfg->txi_ipl, p_instance_ctrl);
#endif
    }
    else
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        MSPI_DISABLE_IRQ(p_extend->com_ored_int, p_instance_ctrl->p_cfg->txi_irq);
#else
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->txi_irq);
#endif
    }

    /* Enable or disable RX interrupt based on enable_rx flag. */
    if (enable_rx)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        MSPI_ENABLE_IRQ(p_extend->rec_ored_int,
                        p_instance_ctrl->p_cfg->rxi_irq,
                        p_instance_ctrl->p_cfg->rxi_ipl,
                        p_instance_ctrl);
#else
        R_BSP_IrqCfgEnable(p_instance_ctrl->p_cfg->rxi_irq, p_instance_ctrl->p_cfg->rxi_ipl, p_instance_ctrl);
#endif
    }
    else
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        MSPI_DISABLE_IRQ(p_extend->rec_ored_int, p_instance_ctrl->p_cfg->rxi_irq);
#else
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->rxi_irq);
#endif
    }

    /* Enable error interrupts. */
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
    R_BSP_ORed_IrqCfgEnable(p_extend->error_ored_int, p_instance_ctrl->p_cfg->eri_ipl, p_instance_ctrl);
#else
    R_BSP_IrqCfgEnable(p_instance_ctrl->p_cfg->eri_irq, p_instance_ctrl->p_cfg->eri_ipl, p_instance_ctrl);
#endif

    /* Enable frame end interrupts. */
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
    R_BSP_ORed_IrqCfgEnable(p_extend->frame_ored_int, p_instance_ctrl->p_cfg->tei_ipl, p_instance_ctrl);
#else
    R_BSP_IrqCfgEnable(p_instance_ctrl->p_cfg->tei_irq, p_instance_ctrl->p_cfg->tei_ipl, p_instance_ctrl);
#endif
}

/*******************************************************************************************************************//**
 * This function sets up the DMAC transfer by assigning source/destination addresses,
 * opening the transfer instance, and setting the hardware request trigger.
 * @retval  FSP_SUCCESS                   Successful transfer initiation.
 **********************************************************************************************************************/
static fsp_err_t r_mspi_transfer_config (mspi_instance_ctrl_t * p_instance_ctrl, spi_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;
#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
    mspi_extended_cfg_t       * p_extend      = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    const transfer_instance_t * p_transfer_tx = p_instance_ctrl->p_cfg->p_transfer_tx;
    const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;
    uint8_t channel              = p_instance_ctrl->p_cfg->channel;
    uint8_t unit                 = p_extend->unit;
    uint8_t triggerSource        = p_extend->mspi_transfer_trigger;
    uint8_t activationSource     = p_extend->mspi_trigger_source;
    uint8_t dmaAlternativeResult = r_mspi_dma_alternative(p_instance_ctrl);
    uint8_t dtsAlternativeResult = r_mspi_dts_alternative(p_instance_ctrl);

    /* Configure the register fields separately for each unit and channel within separate devices */
 #if (BSP_MCU_GROUP_RH850U2Bx)

    /* For channel from 0 to 7 using register MSPITGDMAALT */
    if (MSPI_CS_CHANNEL_7 >= channel)
    {
        p_instance_ctrl->p_regs_tg->MSPITGCTL[unit] |=
            ((triggerSource << (channel * MSPI_CGTL_CHANNEL_GAP)) |
             (activationSource << ((channel * MSPI_CGTL_CHANNEL_GAP) + 1U)));
    }
    /* For others channel using register MSPITGDMAALT1 */
    else
    {
        p_instance_ctrl->p_regs_tg->MSPITGCTL1[unit] |=
            ((triggerSource << ((channel - MSPI_CGTL_UNIT_GAP) * MSPI_CGTL_CHANNEL_GAP)) |
             (activationSource << ((channel - MSPI_CGTL_UNIT_GAP) * MSPI_CGTL_CHANNEL_GAP) + 1U));
    }

 #else

    /* Set trigger source and type of trigger source */
    p_instance_ctrl->p_regs_tg->MSPITGCTL[unit] |=
        ((triggerSource << (channel * MSPI_CGTL_CHANNEL_GAP)) |
         (activationSource << ((channel * MSPI_CGTL_CHANNEL_GAP) + 1U)));
 #endif

    /* Set transfer trigger alternative */
    p_instance_ctrl->p_regs_tg->MSPITGDMAALT |= (p_extend->mspi_dma_alternative << dmaAlternativeResult);
    p_instance_ctrl->p_regs_tg->MSPITGDTSALT |= (p_extend->mspi_dts_alternative << dtsAlternativeResult);
    if (NULL != p_transfer_tx)
    {
        /* Set up data for transmission transfer */
        p_transfer_tx->p_cfg->p_info->p_dest = (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].TXDA0);

        err = p_transfer_tx->p_api->open(p_transfer_tx->p_ctrl, p_transfer_tx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        if (MSPI_TRIGGER_SOURCE_FE1 == p_extend->mspi_trigger_source)
        {
            err = p_transfer_tx->p_api->callbackSet(p_transfer_tx->p_ctrl,
                                                    mspi_fe_transfer_callback,
                                                    p_instance_ctrl,
                                                    NULL);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        }
        else
        {
            err =
                p_transfer_tx->p_api->callbackSet(p_transfer_tx->p_ctrl,
                                                  mspi_tx_transfer_callback,
                                                  p_instance_ctrl,
                                                  NULL);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        }
    }

    if (NULL != p_transfer_rx)
    {
        /* Set up data for reception transfer */
        p_transfer_rx->p_cfg->p_info->p_src = (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].RXDA0);

        err = p_transfer_rx->p_api->open(p_transfer_rx->p_ctrl, p_transfer_rx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        if (MSPI_TRIGGER_SOURCE_FE2 == p_extend->mspi_trigger_source)
        {
            err = p_transfer_rx->p_api->callbackSet(p_transfer_rx->p_ctrl,
                                                    mspi_fe_transfer_callback,
                                                    p_instance_ctrl,
                                                    NULL);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        }
        else
        {
            err =
                p_transfer_rx->p_api->callbackSet(p_transfer_rx->p_ctrl,
                                                  mspi_rx_transfer_callback,
                                                  p_instance_ctrl,
                                                  NULL);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        }
    }

#else

    /* Mark parameters as unused when transfer support is disabled */
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
    FSP_PARAMETER_NOT_USED(p_cfg);
#endif

    return err;
}

/*******************************************************************************************************************//**
 *  Configures the driver state and initiates a MSPI transfer for all modes of operation.
 *
 * @param[in]  p_ctrl               Pointer to control structure.
 * @param      p_src                Buffer to transmit data from.
 * @param      p_dest               Buffer to store received data in.
 * @param[in]  length               Number of data frame.
 * @param[in]  bit_width            Bitwidth of data.
 *
 * @retval  FSP_ERR_ASSERTION            An argument is invalid.
 * @retval  FSP_ERR_NOT_OPEN             The instance has not been initialized.
 * @retval  FSP_SUCCESS                  Transfer was started successfully.
 * @retval  FSP_ERR_INVALID_SIZE         Remaining RAM size is not within the hardware supported range for the
 *                                       configured FIFO stage size.
 * @retval  FSP_ERR_IN_USE               The channel is currently in use.
 * @retval  FSP_ERR_INVALID_ARGUMENT     Invalid input parameter.
 ***********************************************************************************************************************/
static fsp_err_t r_mspi_start_read_write (spi_ctrl_t * const    p_ctrl,
                                          void const          * p_src,
                                          void                * p_dest,
                                          uint32_t const        length,
                                          spi_bit_width_t const bit_width)
{
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) p_ctrl;
#if MSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MSPI_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT((0U < length) && (UINT32_MAX > length));
    FSP_ASSERT((SPI_BIT_WIDTH_2_BITS <= bit_width) && (SPI_BIT_WIDTH_128_BITS >= bit_width));
    FSP_ASSERT((NULL != p_src) || (NULL != p_dest));
#endif
    mspi_extended_cfg_t              * p_extend         = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    R_MSPI0_MSPI0_CH_Type            * p_mspi_regs      = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;

    uint8_t  channel       = p_instance_ctrl->p_cfg->channel;
    uint32_t transfer_mode = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;
    uint32_t refill_data   = 0;

    /* Define maximum hardware RAM capacity */
    uint32_t max_ram_size     = 0;
    uint32_t usable_ram_words = 0;
    uint32_t total_hw_ram     = BSP_FEATURE_MSPI_RAM_WORDS_MAX;
#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
    const transfer_instance_t * p_transfer_tx = p_instance_ctrl->p_cfg->p_transfer_tx;
    const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;
    uint8_t         transfer_count            = 0U;
    transfer_size_t transfer_size             = TRANSFER_SIZE_1_BYTE;
    transfer_size_t transfer_src_size         = TRANSFER_SIZE_1_BYTE;
    transfer_size_t transfer_des_size         = TRANSFER_SIZE_1_BYTE;

    uint32_t rastad   = p_mspi_regs[channel].RASTAD;
    uint32_t ram_init = (uint32_t) (p_instance_ctrl->p_regs) + MSPI_INIT_RAM_ADDR;

    volatile uint8_t  * p_addr_8  = (volatile uint8_t *) (ram_init + rastad);
    volatile uint16_t * p_addr_16 = (volatile uint16_t *) (ram_init + rastad);
    volatile uint32_t * p_addr_32 = (volatile uint32_t *) (ram_init + rastad);
    volatile uint64_t * p_addr_64 = (volatile uint64_t *) (ram_init + rastad);

    uint32_t        align         = 0U;
    uint32_t        num_count     = (bit_width + SPI_BIT_WIDTH_31_BITS) / SPI_BIT_WIDTH_32_BITS;
    static uint32_t dummy_tx_data = 0U;
    static uint32_t dummy_rx_data;
#endif

    /* Initialize control parameters */
    p_instance_ctrl->p_tx_data  = p_src;
    p_instance_ctrl->p_rx_data  = p_dest;
    p_instance_ctrl->tx_count   = 0U;
    p_instance_ctrl->rx_count   = 0U;
    p_instance_ctrl->length     = length;
    p_instance_ctrl->bitwidth   = bit_width;
    p_instance_ctrl->stage_size = 0U;

    /* Check the channel operating status.*/
    if (p_mspi_regs[channel].CSTR_b.ACTF != 0)
    {
        return FSP_ERR_IN_USE;
    }

    /* Check if the Safe Enable is set to 'Enable'*/
    if (MSPI_SAFE_ENABLE == p_extend->p_mspi_config_cfg1->mspi_chip_select_cfg1.mspi_safe_enable)
    {
        FSP_ASSERT(SPI_BIT_WIDTH_32_BITS == bit_width);
    }

    /* Check if the Frame Count End operation (FCCEm) is set to 'Not Clear'.
     * In this mode, the hardware keeps the channel enabled continuously after a transfer.
     */
    if ((p_mspi_regs[channel].CFG0 & (1U << R_MSPI0_MSPI0_CH_CFG0_FCCE_Pos)) != 0U)
    {
        /* Clear the Channel Enable flag and stop the channel */
        p_mspi_regs[channel].CSTC_b.CHENC = 1U;

        /* Poll the status register and wait until the hardware confirms
         * that the channel is fully disabled before proceeding.
         */
        FSP_HARDWARE_REGISTER_WAIT(p_mspi_regs[channel].CSTR_b.CHEN, 0U);
    }

    /* Initialize CFG registers */
    p_mspi_regs[channel].CFG2 = p_instance_ctrl->bitwidth;
    p_mspi_regs[channel].CFG0 = (uint32_t) (p_extend->p_mspi_config_cfg0->chip_select_config_0_u32);
    p_mspi_regs[channel].CFG1 = (uint32_t) (p_extend->p_mspi_config_cfg1->chip_select_config_1_u32);
    p_mspi_regs[channel].CFG4 = (uint32_t) (p_extend->p_mspi_config_cfg4->chip_select_config_4_u32);
    p_mspi_regs[channel].SEUP = (uint16_t) (p_extend->mspi_setup_time);
    p_mspi_regs[channel].HOLD = (uint16_t) (p_extend->mspi_hold_time);
    p_mspi_regs[channel].IDLE = (uint16_t) (p_extend->mspi_idle_time);
    p_mspi_regs[channel].INDA = (uint16_t) (p_extend->mspi_inter_data);

    /* Initialize Interrupt masked output */
    p_mspi_intf_regs[0].INTMSK = (p_mspi_intf_regs[0].INTMSK & ~(1U << channel)) |
                                 (((uint32_t) p_extend->mspi_tx_isr_mask & 1U) << channel);
    p_mspi_intf_regs[1].INTMSK = (p_mspi_intf_regs[1].INTMSK & ~(1U << channel)) |
                                 (((uint32_t) p_extend->mspi_rx_isr_mask & 1U) << channel);
    p_mspi_intf_regs[2].INTMSK = (p_mspi_intf_regs[2].INTMSK & ~(1U << channel)) |
                                 (((uint32_t) p_extend->mspi_fe_isr_mask & 1U) << channel);
    p_mspi_intf_regs[3].INTMSK = (p_mspi_intf_regs[3].INTMSK & ~(1U << channel)) |
                                 (((uint32_t) p_extend->mspi_err_isr_mask & 1U) << channel);

    if (MSPI_TRANSFER_MODE_DIRECT_ACCESS == transfer_mode)
    {
        /* Set Direct Access Mode and clear RAM address */
        p_mspi_regs[channel].CFG0  &= ~R_MSPI0_MSPI0_CH_CFG0_MD_Msk;
        p_mspi_regs[channel].RASTAD = MSPI_RASTAD_CLEAR;

        /* Disable Frame End interrupt if configured to keep state */
        if (MSPI_CHANNEL_FRAME_END_KEEP == p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_frame_end)
        {
            p_mspi_regs[channel].CFG0_b.IFEE = 0;
        }

        /* Set total frame count */
        p_mspi_regs[channel].CFSET = p_instance_ctrl->length;
    }
    else if (MSPI_TRANSFER_MODE_BUFFER_MEMORY == transfer_mode)
    {
        /* Set Buffer Memory Mode, configure RAM start, and disable Job Mode */
        p_mspi_regs[channel].CFG0 = (p_mspi_regs[channel].CFG0 & ~R_MSPI0_MSPI0_CH_CFG0_MD_Msk) |
                                    (1U << R_MSPI0_MSPI0_CH_CFG0_MD_Pos);
        p_mspi_regs[channel].RASTAD_b.RASTAD = (p_extend->mspi_ram_start);
        p_mspi_regs[channel].SSEL           &= ~(1U << R_MSPI0_MSPI0_CH_SSEL_JOBEN_Pos);

        usable_ram_words = total_hw_ram - (p_extend->mspi_ram_start / 4U);

        if (MSPI_SELECTABLE_MODE_TRANSMIT_RECEIVE == p_extend->mspi_select)
        {
            max_ram_size = usable_ram_words / 2U;
        }
        else
        {
            max_ram_size = usable_ram_words;
        }

        /* Adjust maximum RAM capacity for bit widths > 32 bits */
        if (p_instance_ctrl->bitwidth > SPI_BIT_WIDTH_32_BITS)
        {
            max_ram_size = max_ram_size / 2U;
        }

        /* Set frame count for the first chunk */
        if (p_instance_ctrl->length > max_ram_size)
        {
            p_mspi_regs[channel].CFSET = max_ram_size;
        }
        else
        {
            p_mspi_regs[channel].CFSET = p_instance_ctrl->length;
        }
    }
    else                               /* FIFO Memory Mode */
    {
        uint32_t total_hw_ram     = BSP_FEATURE_MSPI_RAM_WORDS_MAX;
        uint32_t usable_ram_words = total_hw_ram - p_extend->mspi_ram_start;
        uint32_t req_stage        = p_extend->p_mspi_config_cfg4->mspi_chip_select_cfg4.mspi_fifo_stage_size;

        if (MSPI_SELECTABLE_MODE_TRANSMIT_RECEIVE == p_extend->mspi_select)
        {
            usable_ram_words = usable_ram_words / 2U;
        }

        if (((usable_ram_words < 32U) && (MSPI_FIFO_STAGE_SIZE_32 == req_stage)) ||
            ((usable_ram_words < 16U) && (MSPI_FIFO_STAGE_SIZE_16 == req_stage)))
        {
            return FSP_ERR_INVALID_SIZE;
        }

        uint32_t max_allowed_length = 65535U;

        if (MSPI_FIFO_STAGE_SIZE_32 == req_stage)
        {
            max_allowed_length = 65520U;
        }
        else if (MSPI_FIFO_STAGE_SIZE_16 == req_stage)
        {
            max_allowed_length = 65528U;
        }
        else if (MSPI_FIFO_STAGE_SIZE_8 == req_stage)
        {
            max_allowed_length = 65532U;
        }

        if (p_instance_ctrl->length > max_allowed_length)
        {
            return FSP_ERR_INVALID_ARGUMENT;
        }

        /* Set FIFO Memory Mode, configure RAM start, and disable Job Mode */
        p_mspi_regs[channel].CFG0 = (p_mspi_regs[channel].CFG0 & ~R_MSPI0_MSPI0_CH_CFG0_MD_Msk) |
                                    (2U << R_MSPI0_MSPI0_CH_CFG0_MD_Pos);
        p_mspi_regs[channel].RASTAD = (uint16_t) (p_extend->mspi_ram_start);
        p_mspi_regs[channel].SSEL  &= ~(1U << R_MSPI0_MSPI0_CH_SSEL_JOBEN_Pos);

        /* Configure FIFO stage size  */
        if (MSPI_FIFO_STAGE_SIZE_8 == req_stage)
        {
            p_mspi_regs[channel].CFG4  &= ~R_MSPI0_MSPI0_CH_CFG4_SIZE_Msk;
            p_instance_ctrl->stage_size = MSPI_STAGE_SIZE_8;
        }
        else if (MSPI_FIFO_STAGE_SIZE_16 == req_stage)
        {
            p_mspi_regs[channel].CFG4 = (p_mspi_regs[channel].CFG4 & ~R_MSPI0_MSPI0_CH_CFG4_SIZE_Msk) |
                                        (1U << R_MSPI0_MSPI0_CH_CFG4_SIZE_Pos);
            p_instance_ctrl->stage_size = MSPI_STAGE_SIZE_16;
        }
        else
        {
            p_mspi_regs[channel].CFG4 = (p_mspi_regs[channel].CFG4 & ~R_MSPI0_MSPI0_CH_CFG4_SIZE_Msk) |
                                        (2U << R_MSPI0_MSPI0_CH_CFG4_SIZE_Pos);
            p_instance_ctrl->stage_size = MSPI_STAGE_SIZE_32;
        }

        refill_data                = r_mspi_fifo_refill_stage(p_instance_ctrl);
        p_mspi_regs[channel].CFSET = (p_instance_ctrl->length + refill_data);
    }

    if (MSPI_SELECTABLE_MODE_TRANSMIT_ONLY == p_extend->mspi_select)
    {
        /* Enable TX, disable RX */
        p_mspi_regs[channel].CFG0 = (p_mspi_regs[channel].CFG0 & ~R_MSPI0_MSPI0_CH_CFG0_RXE_Msk) |
                                    R_MSPI0_MSPI0_CH_CFG0_TXE_Msk;
    }
    else if (MSPI_SELECTABLE_MODE_TRANSMIT_RECEIVE == p_extend->mspi_select)
    {
        /* Enable both TX and RX (Full-Duplex) */
        p_mspi_regs[channel].CFG0 |= (R_MSPI0_MSPI0_CH_CFG0_TXE_Msk | R_MSPI0_MSPI0_CH_CFG0_RXE_Msk);
    }
    else                               /* Receive Only */
    {
        /* Enable RX, disable TX */
        p_mspi_regs[channel].CFG0 = (p_mspi_regs[channel].CFG0 & ~R_MSPI0_MSPI0_CH_CFG0_TXE_Msk) |
                                    R_MSPI0_MSPI0_CH_CFG0_RXE_Msk;
    }

    /* Setup initial Buffer Memory Data */
    if (MSPI_TRANSFER_MODE_BUFFER_MEMORY == transfer_mode)
    {
        p_mspi_regs[channel].CFG0_b.ITXE = MSPI_INTERRUPT_OUT_DISABLE;
        p_mspi_regs[channel].CFG0_b.IRXE = MSPI_INTERRUPT_OUT_DISABLE;

        if (MSPI_SELECTABLE_MODE_RECEIVE_ONLY != p_extend->mspi_select)
        {
            r_mspi_buffer_write(p_instance_ctrl);
        }
    }

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)

    /* Adjust the read pointer to skip alignment and previously written data */
    if ((MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE) &&
        (MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE))
    {
        align = (r_mspi_align_ram(length, ram_init, bit_width) + length);
    }
    else
    {
        align = 0U;
    }

    if (NULL != p_transfer_tx)
    {
        /* Count data unit base on bit_width */
        transfer_count = r_mspi_count_byte(bit_width);

        /* Calculate transfer size base on bit_width */
        transfer_size     = r_mspi_cal_transfer_size(bit_width);
        transfer_src_size = transfer_size;
        if ((MSPI_TRANSFER_MODE_FIFO_MEMORY ==
             p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode) ||
            (MSPI_TRANSFER_TRIGGER_DMA != p_extend->mspi_transfer_trigger))
        {
            if (bit_width <= SPI_BIT_WIDTH_32_BITS)
            {
                transfer_des_size = transfer_size;
            }
            else
            {
                transfer_des_size = TRANSFER_SIZE_4_BYTE;
            }

            p_transfer_tx->p_cfg->p_info->transfer_count_per_hw_request = ((p_instance_ctrl->stage_size) / 2) *
                                                                          num_count;
        }
        else
        {
            transfer_des_size = transfer_size;
        }

        /* Enable interrupt output for transmission */
        p_mspi_regs[channel].CFG0_b.ITXE = MSPI_INTERRUPT_OUT_ENABLE;

        /* Update data for transmission transfer*/
        if (NULL == p_src)
        {
            p_transfer_tx->p_cfg->p_info->p_src = (void const *) &dummy_tx_data;
            p_transfer_tx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_addr_mode =
                TRANSFER_ADDR_MODE_FIXED;
            p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.src_addr_direction =
                TRANSFER_DTSC_ADDR_MODE_FIXED;
        }
        else
        {
            p_transfer_tx->p_cfg->p_info->p_src = (void const *) p_src;
            p_transfer_tx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_addr_mode =
                TRANSFER_ADDR_MODE_INCREMENTED;
            p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.src_addr_direction =
                TRANSFER_DTSC_ADDR_MODE_INCREMENTED;
        }

        p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_data_size = transfer_des_size;
        p_transfer_tx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size           = transfer_src_size;
        p_transfer_tx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size           = transfer_des_size;

        /* Set up data size for DMA or DTS */
        if (MSPI_TRANSFER_TRIGGER_DMA == p_extend->mspi_transfer_trigger)
        {
            p_transfer_tx->p_cfg->p_info->number_transfer = (length + refill_data) * transfer_count;
        }
        else
        {
            /* Set up data size for DTS */
            p_transfer_tx->p_cfg->p_info->number_transfer = (length + refill_data) *
                                                            ((bit_width + SPI_BIT_WIDTH_31_BITS) /
                                                             SPI_BIT_WIDTH_32_BITS);
            p_transfer_tx->p_cfg->p_info->p_reload_des_addr =
                (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].TXDA0);
            p_transfer_tx->p_cfg->p_info->addr_reload_count = (p_instance_ctrl->stage_size / 2) *
                                                              ((bit_width + SPI_BIT_WIDTH_31_BITS) /
                                                               SPI_BIT_WIDTH_32_BITS);
            p_transfer_tx->p_cfg->p_info->reload_addr_reload_count = (p_instance_ctrl->stage_size / 2) *
                                                                     ((bit_width + SPI_BIT_WIDTH_31_BITS) /
                                                                      SPI_BIT_WIDTH_32_BITS);
            p_transfer_tx->p_cfg->p_info->reload_number_transfer = (p_instance_ctrl->stage_size) *
                                                                   ((bit_width + SPI_BIT_WIDTH_31_BITS) /
                                                                    SPI_BIT_WIDTH_32_BITS);
        }

        /* When using transfer api in Buffer Mode, no data transfer by DMA/DTS */
        if (MSPI_TRANSFER_MODE_BUFFER_MEMORY ==
            p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
        {
            /* In Buffer Memory mode, data are transferred directly through RAM */
            p_transfer_tx->p_cfg->p_info->number_transfer = 0U;
        }

        /* Re-config for data update */
        fsp_err_t err = p_transfer_tx->p_api->reconfigure(p_transfer_tx->p_ctrl, p_transfer_tx->p_cfg->p_info);
        FSP_ERROR_RETURN(err == FSP_SUCCESS, err);

        /* Enable transfer*/
        err = p_transfer_tx->p_api->enable(p_transfer_tx->p_ctrl);
        FSP_ERROR_RETURN(err == FSP_SUCCESS, err);
    }

    if (NULL != p_transfer_rx)
    {
        /* Count data unit base on bit_width */
        transfer_count = r_mspi_count_byte(bit_width);

        /* Calculate transfer size base on bit_width */
        transfer_size     = r_mspi_cal_transfer_size(bit_width);
        transfer_des_size = transfer_size;
        if ((MSPI_TRANSFER_MODE_FIFO_MEMORY ==
             p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode) ||
            (MSPI_TRANSFER_TRIGGER_DMA != p_extend->mspi_transfer_trigger))
        {
            if (bit_width <= SPI_BIT_WIDTH_32_BITS)
            {
                transfer_src_size = transfer_size;
            }
            else
            {
                transfer_src_size = TRANSFER_SIZE_4_BYTE;
            }

            p_transfer_rx->p_cfg->p_info->transfer_count_per_hw_request = ((p_instance_ctrl->stage_size) / 2) *
                                                                          num_count;
        }
        else
        {
            transfer_src_size = transfer_size;
        }

        /* Enable interrupt output for reception */
        p_mspi_regs[channel].CFG0_b.IRXE = MSPI_INTERRUPT_OUT_ENABLE;

        if (MSPI_TRANSFER_MODE_BUFFER_MEMORY ==
            p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
        {
            if (SPI_BIT_WIDTH_8_BITS >= bit_width)
            {
                p_addr_8 += align;
                p_transfer_rx->p_cfg->p_info->p_src = (void *) p_addr_8;
            }
            else if (SPI_BIT_WIDTH_16_BITS >= bit_width)
            {
                p_addr_16 += align;
                p_transfer_rx->p_cfg->p_info->p_src = (void *) p_addr_16;
            }
            else if (SPI_BIT_WIDTH_32_BITS >= bit_width)
            {
                p_addr_32 += align;
                p_transfer_rx->p_cfg->p_info->p_src = (void *) p_addr_32;
            }
            else
            {
                p_addr_64 += align;
                p_transfer_rx->p_cfg->p_info->p_src = (void *) p_addr_64;
            }
        }

        /* Update data for reception transfer */
        if (NULL == p_dest)
        {
            p_transfer_rx->p_cfg->p_info->p_dest = (void *) &dummy_rx_data;
            p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode =
                TRANSFER_ADDR_MODE_FIXED;
            p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.des_addr_direction =
                TRANSFER_DTSC_ADDR_MODE_FIXED;
        }
        else
        {
            p_transfer_rx->p_cfg->p_info->p_dest = (void *) p_dest;
            p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode =
                TRANSFER_ADDR_MODE_INCREMENTED;
            p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.des_addr_direction =
                TRANSFER_DTSC_ADDR_MODE_INCREMENTED;
        }

        p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_data_size = transfer_src_size;
        p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size           = transfer_src_size;
        p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size           = transfer_des_size;

        /* Set up data size for DMA or DTS */
        if (MSPI_TRANSFER_TRIGGER_DMA == p_extend->mspi_transfer_trigger)
        {
            p_transfer_rx->p_cfg->p_info->number_transfer = (length + refill_data) * transfer_count;
        }
        else
        {
            /* Set up data size for DTS */
            p_transfer_rx->p_cfg->p_info->number_transfer = (length + refill_data) *
                                                            ((bit_width + SPI_BIT_WIDTH_31_BITS) /
                                                             SPI_BIT_WIDTH_32_BITS);
            p_transfer_rx->p_cfg->p_info->p_reload_src_addr =
                (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].RXDA0);
            p_transfer_rx->p_cfg->p_info->addr_reload_count = (p_instance_ctrl->stage_size / 2) *
                                                              ((bit_width + SPI_BIT_WIDTH_31_BITS) /
                                                               SPI_BIT_WIDTH_32_BITS);
            p_transfer_rx->p_cfg->p_info->reload_addr_reload_count = (p_instance_ctrl->stage_size / 2) *
                                                                     ((bit_width + SPI_BIT_WIDTH_31_BITS) /
                                                                      SPI_BIT_WIDTH_32_BITS);
            p_transfer_rx->p_cfg->p_info->reload_number_transfer = (p_instance_ctrl->stage_size) *
                                                                   ((bit_width + SPI_BIT_WIDTH_31_BITS) /
                                                                    SPI_BIT_WIDTH_32_BITS);
        }

        /* Re-config for data update */
        fsp_err_t err = p_transfer_rx->p_api->reconfigure(p_transfer_rx->p_ctrl, p_transfer_rx->p_cfg->p_info);
        FSP_ERROR_RETURN(err == FSP_SUCCESS, err);

        /* Enable transfer*/
        err = p_transfer_rx->p_api->enable(p_transfer_rx->p_ctrl);
        FSP_ERROR_RETURN(err == FSP_SUCCESS, err);
    }
#endif
    if (0U == p_mspi_regs[channel].CFG4_b.HWTS)
    {
        /* Enable MSPI channel and start transfer */
        p_mspi_regs[channel].CSTS = (R_MSPI0_MSPI0_CH_CSTS_CHENS_Msk | R_MSPI0_MSPI0_CH_CSTS_ACTFS_Msk);
    }
    else
    {
        /* Hardware Trigger (DMA/DTS) */
        p_mspi_regs[channel].CSTS = R_MSPI0_MSPI0_CH_CSTS_CHENS_Msk;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Copy data into the MSPI data register for transmitting.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_transmit (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;
    mspi_extended_cfg_t   * p_extend    = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    uint32_t chip_select = p_instance_ctrl->p_cfg->channel;

    /* Read CFG0 register value to check configurations */
    uint32_t cfg0_val = p_mspi_regs[chip_select].CFG0;

    /* Clear Notification interrupt flag for the selected chip_select */
    p_mspi_intf_regs[0].INTFC = (1U << chip_select);

    /* Check whether FIFO mode is enabled and perform write operation */
    if (MSPI_TRANSFER_MODE_FIFO_MEMORY == p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
    {
        r_mspi_fifo_write(p_instance_ctrl);
    }
    else
    {
        r_mspi_direct_write(p_instance_ctrl);
    }
}

/*******************************************************************************************************************//**
 * Copy configured bit width from the MSPI data register to the current rx data location.
 * If the receive buffer is NULL, just read the MSPI data register.
 * If the total transfer length has already been received than do nothing.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_receive (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;
    mspi_extended_cfg_t   * p_extend    = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    uint32_t chip_select = p_instance_ctrl->p_cfg->channel;
    uint8_t  num_count   = ((p_instance_ctrl->bitwidth) + 31) / 32;

    /* Read CFG0 register value to check configurations */
    uint32_t cfg0_val = p_mspi_regs[chip_select].CFG0;

    /* Clear Notification interrupt flag for the selected chip_select.*/
    p_mspi_intf_regs[1].INTFC = (1U << chip_select);

    /* Check whether FIFO mode is enabled and perform read operation */
    if (MSPI_TRANSFER_MODE_FIFO_MEMORY == p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
    {
        /* FIFO access mode */
        r_mspi_fifo_read(p_instance_ctrl);
    }
    else
    {
        /* Direct access mode (non-FIFO) */
        r_mspi_direct_read(p_instance_ctrl);
    }

    /* Process callback logic only if FCCE mode is enabled */
    if ((cfg0_val & (1U << R_MSPI0_MSPI0_CH_CFG0_FCCE_Pos)) != 0U)
    {
        /* Check if all data has been completely received */
        if (p_instance_ctrl->rx_count >= (p_instance_ctrl->length * num_count))
        {
            /*Disable the RX interrupt to prevent unnecessary ISR triggers */
            p_mspi_regs[chip_select].CFG0_b.IRXE = 0U;

            /* Handle callback triggering based on the TX/RX configuration.*/
            if ((cfg0_val & R_MSPI0_MSPI0_CH_CFG0_TXE_Msk) != 0U)
            {
                /* Full-Duplex (TX/RX), report as Transfer Complete */
                r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_TRANSFER_COMPLETE);
            }
            else
            {
                /* Receive-Only (RX), report as Receive Complete */
                r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_RECEIVE_COMPLETE);
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Copy data into the MSPI RAM for transmission in fixed buffer mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_buffer_write (mspi_instance_ctrl_t * p_instance_ctrl)
{
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;

    uint32_t chunk_size = p_mspi_regs[channel].CFSET;
    uint32_t tx_offset  = p_instance_ctrl->tx_count;

    uint32_t            rastad   = p_mspi_regs[channel].RASTAD;
    uint32_t            ram_base = (uint32_t) p_instance_ctrl->p_regs + MSPI_INIT_RAM_ADDR;
    uint32_t            ram_addr = ram_base + rastad;
    uint32_t            i        = 0U;
    uint64_t            data_64;
    volatile uint8_t  * p_dest_8;
    volatile uint16_t * p_dest_16;
    volatile uint32_t * p_dest_32;

    uint8_t  * p_src_8;
    uint16_t * p_src_16;
    uint32_t * p_src_32;
    uint64_t * p_src_64;

    /* Check for NULL pointer (e.g., when the user only wants to Read) */
    if (NULL == p_instance_ctrl->p_tx_data)
    {
        return;
    }

    /* Write data from the user buffer to MSPI RAM based on the configured bit width */
    if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_8_BITS)
    {
        p_dest_8 = (volatile uint8_t *) ram_addr;
        p_src_8  = (uint8_t *) p_instance_ctrl->p_tx_data;

        for (i = 0U; i < chunk_size; i++)
        {
            p_dest_8[i] = p_src_8[tx_offset + i];
        }
    }
    else if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_16_BITS)
    {
        p_dest_16 = (volatile uint16_t *) ram_addr;
        p_src_16  = (uint16_t *) p_instance_ctrl->p_tx_data;

        for (i = 0U; i < chunk_size; i++)
        {
            p_dest_16[i] = p_src_16[tx_offset + i];
        }
    }
    else if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_32_BITS)
    {
        p_dest_32 = (volatile uint32_t *) ram_addr;
        p_src_32  = (uint32_t *) p_instance_ctrl->p_tx_data;

        for (i = 0U; i < chunk_size; i++)
        {
            p_dest_32[i] = p_src_32[tx_offset + i];
        }
    }
    else
    {
        /* Greater than 32-bit width (up to 64-bit) */
        p_dest_32 = (volatile uint32_t *) ram_addr;
        p_src_64  = (uint64_t *) p_instance_ctrl->p_tx_data;

        for (i = 0U; i < chunk_size; i++)
        {
            data_64 = p_src_64[tx_offset + i];

            p_dest_32[i * 2] = (uint32_t) (data_64 & 0xFFFFFFFFULL);

            p_dest_32[(i * 2) + 1] = (uint32_t) ((data_64 >> 32) & 0xFFFFFFFFULL);
        }
    }
}

/*******************************************************************************************************************//**
 * Copy data from the MSPI RAM for reception in fixed buffer mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_buffer_read (mspi_instance_ctrl_t * p_instance_ctrl)
{
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;

    uint32_t chunk_size = p_mspi_regs[channel].CFSET;
    uint32_t rx_offset  = p_instance_ctrl->rx_count;

    uint32_t rastad   = p_mspi_regs[channel].RASTAD;
    uint32_t ram_base = (uint32_t) p_instance_ctrl->p_regs + MSPI_INIT_RAM_ADDR;
    uint32_t ram_addr = ram_base + rastad;
    uint32_t low_word;
    uint32_t high_word;
    uint32_t align_elements = 0U;
    uint32_t index          = 0U;

    volatile uint8_t  * p_src_8;
    volatile uint16_t * p_src_16;
    volatile uint32_t * p_src_32;

    uint8_t  * p_dest_8;
    uint16_t * p_dest_16;
    uint32_t * p_dest_32;
    uint64_t * p_dest_64;

    /* Check for NULL pointer */
    if (NULL == p_instance_ctrl->p_rx_data)
    {
        return;
    }

    /* Calculate the alignment offset in RAM for Full-Duplex (TX/RX) Mode */
    if ((1U == p_mspi_regs[channel].CFG0_b.RXE) && (1U == p_mspi_regs[channel].CFG0_b.TXE))
    {
        align_elements = r_mspi_align_ram(chunk_size, ram_base, p_instance_ctrl->bitwidth) + chunk_size;
    }

    /* Read data from MSPI RAM to the user buffer based on the configured bit width */
    if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_8_BITS)
    {
        p_src_8  = (volatile uint8_t *) ram_addr;
        p_dest_8 = (uint8_t *) p_instance_ctrl->p_rx_data;

        for (index = 0U; index < chunk_size; index++)
        {
            p_dest_8[rx_offset + index] = p_src_8[align_elements + index];
        }
    }
    else if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_16_BITS)
    {
        p_src_16  = (volatile uint16_t *) ram_addr;
        p_dest_16 = (uint16_t *) p_instance_ctrl->p_rx_data;

        for (index = 0U; index < chunk_size; index++)
        {
            p_dest_16[rx_offset + index] = p_src_16[align_elements + index];
        }
    }
    else if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_32_BITS)
    {
        p_src_32  = (volatile uint32_t *) ram_addr;
        p_dest_32 = (uint32_t *) p_instance_ctrl->p_rx_data;

        for (index = 0U; index < chunk_size; index++)
        {
            p_dest_32[rx_offset + index] = p_src_32[align_elements + index];
        }
    }
    else
    {
        /* Greater than 32-bit width (up to 64-bit) */
        p_src_32  = (volatile uint32_t *) ram_addr;
        p_dest_64 = (uint64_t *) p_instance_ctrl->p_rx_data;

        uint32_t ram_offset_32 = align_elements * 2U;

        for (index = 0U; index < chunk_size; index++)
        {
            low_word  = p_src_32[ram_offset_32 + (index * 2)];
            high_word = p_src_32[ram_offset_32 + (index * 2) + 1];

            p_dest_64[rx_offset + index] = ((uint64_t) high_word << 32) | (uint64_t) low_word;
        }
    }
}

/*******************************************************************************************************************//**
 * Align ram address to 4.
 *
 * @param[in]  length         number of data.
 * @param[in]  ram_init       ram address intial
 * @param[in]  bitwidth       length of data (ex: 8 bits, 16 bits,...)
 **********************************************************************************************************************/
static uint8_t r_mspi_align_ram (uint32_t length, uint32_t ram_init, uint32_t bitwidth)
{
    uint32_t align      = 0U;
    uint32_t count      = 0U;
    uint32_t write_end  = 0U;
    uint32_t read_start = 0U;

    count = r_mspi_count_byte(bitwidth);

    write_end  = count * length + ram_init;
    read_start = ((write_end + ALIGNMENT_BOUNDARY_BYTES - 1U) & ~ALIGNMENT_MASK);

    align = ((read_start - write_end) / count);

    return align;
}

/*******************************************************************************************************************//**
 * Count number of data unit base on length of data.
 *
 * @param[in]  bitwidth       length of data (ex: 8 bits, 16 bits,...)
 **********************************************************************************************************************/
static uint8_t r_mspi_count_byte (uint32_t bitwidth)
{
    uint8_t count = 0U;

    if (bitwidth <= SPI_BIT_WIDTH_8_BITS)
    {
        count = MSPI_LENGTH_PER_COUNT_1;
    }
    else if (bitwidth <= SPI_BIT_WIDTH_16_BITS)
    {
        count = MSPI_LENGTH_PER_COUNT_2;
    }
    else if (bitwidth <= SPI_BIT_WIDTH_32_BITS)
    {
        count = MSPI_LENGTH_PER_COUNT_4;
    }
    else
    {
        count = MSPI_LENGTH_PER_COUNT_8;
    }

    return count;
}

/*******************************************************************************************************************//**
 * Copy data into the MSPI RAM for transmission in FIFO mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_fifo_write (mspi_instance_ctrl_t * p_instance_ctrl)
{
    uint32_t tx_count     = p_instance_ctrl->tx_count;
    uint8_t  stage        = p_instance_ctrl->stage_size;
    uint8_t  num_count    = (((p_instance_ctrl->bitwidth) + SPI_BIT_WIDTH_31_BITS) / SPI_BIT_WIDTH_32_BITS);
    uint8_t  refill_stage = r_mspi_fifo_refill_stage(p_instance_ctrl);
    uint32_t length       = (p_instance_ctrl->length + refill_stage);

    /* Transmit data based on bit width in FIFO mode */
    if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_32_BITS)
    {
        /* Transmit data from transmission buffer until half of the stage */
        for (tx_count = p_instance_ctrl->tx_count; tx_count < length; tx_count++)
        {
            /* Write data to register*/
            r_mspi_write_register(p_instance_ctrl);

            /* Increase transmission counter*/
            p_instance_ctrl->tx_count = (tx_count + 1U);

            /* Return when half of the stage is filled */
            if (0U == (tx_count + 1U) % (stage / 2U))
            {
                NOP();

                return;
            }
        }
    }
    else
    {
        /* For bitwidth > 32 bits, transmission data as multi-count */
        for (tx_count = p_instance_ctrl->tx_count; tx_count < (length * num_count); tx_count++)
        {
            /* Write data to register*/
            r_mspi_write_register(p_instance_ctrl);

            /* Increase transmission counter*/
            p_instance_ctrl->tx_count = (tx_count + 1U);

            /* Return when half of the stage is filled */
            if (0U == (tx_count + 1) % (stage * num_count / 2U))
            {
                SYNCP();

                return;
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Copy data from the MSPI RAM for reception in FIFO mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_fifo_read (mspi_instance_ctrl_t * p_instance_ctrl)
{
    uint8_t  stage     = p_instance_ctrl->stage_size;
    uint8_t  num_count = ((p_instance_ctrl->bitwidth) + SPI_BIT_WIDTH_31_BITS) / SPI_BIT_WIDTH_32_BITS;
    uint32_t rx_count  = p_instance_ctrl->rx_count;
    uint32_t length    = p_instance_ctrl->length;

    /* Transmit 32-bit data from reception buffer until half of the stage */
    if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_32_BITS)
    {
        for (rx_count = p_instance_ctrl->rx_count; rx_count < length; rx_count++)
        {
            /* Read data from register*/
            r_mspi_read_register(p_instance_ctrl);

            /* Increase reception counter*/
            p_instance_ctrl->rx_count = rx_count + 1U;

            /* Return when half of the stage is filled */
            if ((rx_count + 1U) % (stage / 2U) == 0U)
            {
                SYNCP();

                return;
            }
        }
    }
    else
    {
        /* For bitwidth > 32 bits, reception data as multi-count */
        for (rx_count = p_instance_ctrl->rx_count; rx_count < (length * num_count); rx_count++)
        {
            /* Read data from register*/
            r_mspi_read_register(p_instance_ctrl);

            /* Increase reception counter*/
            p_instance_ctrl->rx_count = rx_count + 1U;

            /* Return when half of the stage is filled */
            if ((rx_count + 1U) % (stage * num_count / 2U) == 0)
            {
                SYNCP();

                return;
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Copy data from the MSPI RAM for transmission in direct mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_direct_write (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;
    uint8_t                 num_count   = ((p_instance_ctrl->bitwidth) + SPI_BIT_WIDTH_31_BITS) / SPI_BIT_WIDTH_32_BITS;
    uint32_t                tx_count    = p_instance_ctrl->tx_count;
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;

    /* If all data has been transmitted, return */
    if (p_instance_ctrl->tx_count == (p_instance_ctrl->length) * num_count)
    {
        /* Clear Job mode function */
        p_mspi_regs[channel].SSEL_b.JOBEN = MSPI_SSEL_CLEAR;

        return;
    }

    /* Check if transmit request flag is set and transmission buffer is valid */
    if (((p_mspi_regs[channel].CSTR & R_MSPI0_MSPI0_CH_CSTR_TXRQF_Msk) != 0U))
    {
        /* Copy data from buffer to the register */
        r_mspi_write_register(p_instance_ctrl);
    }

    /* Update transmitted count */
    p_instance_ctrl->tx_count = tx_count + 1U;
}

/*******************************************************************************************************************//**
 * Copy data from the MSPI RAM for reception in direct mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_direct_read (mspi_instance_ctrl_t * p_instance_ctrl)
{
    uint8_t  num_count = ((p_instance_ctrl->bitwidth) + SPI_BIT_WIDTH_31_BITS) / SPI_BIT_WIDTH_32_BITS;
    uint32_t rx_count  = p_instance_ctrl->rx_count;

    /* If all data has been receive, return */
    if (p_instance_ctrl->rx_count == ((p_instance_ctrl->length) * num_count))
    {
        return;
    }

    /* Copy data from the register to buffer */
    r_mspi_read_register(p_instance_ctrl);

    /* Update received count */
    p_instance_ctrl->rx_count = rx_count + 1U;
}

/*******************************************************************************************************************//**
 * Copy data from the register to buffer.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_read_register (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs =
        (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;

    uint32_t const rx_count    = p_instance_ctrl->rx_count;
    uint32_t const chip_select = p_instance_ctrl->p_cfg->channel;
    uint32_t const rx_data     = p_mspi_regs[chip_select].RXDA0;

    if (NULL == p_instance_ctrl->p_rx_data)
    {
        return;
    }

    /* Store RX data into the user buffer according to the configured bit width. */
    if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_8_BITS)
    {
        /* Store 8-bit data */
        ((uint8_t *) p_instance_ctrl->p_rx_data)[rx_count] = (uint8_t) rx_data;
    }
    else if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_16_BITS)
    {
        /* Store 16-bit data */
        ((uint16_t *) p_instance_ctrl->p_rx_data)[rx_count] = (uint16_t) rx_data;
    }
    else
    {
        /* Store over 16-bit data */
        ((uint32_t *) p_instance_ctrl->p_rx_data)[rx_count] = rx_data;
    }
}

/*******************************************************************************************************************//**
 * Copy data from the register to buffer.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_write_register (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs =
        (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;

    uint32_t const chip_select = p_instance_ctrl->p_cfg->channel;
    uint32_t const tx_count    = p_instance_ctrl->tx_count;
    uint32_t       tx_data     = 0U;

    /* Store RX data into the user buffer according to the configured bit width. */
    if (NULL != p_instance_ctrl->p_tx_data)
    {
        if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_8_BITS)
        {
            /* Write 8-bit data  */
            tx_data = ((uint8_t const *) p_instance_ctrl->p_tx_data)[tx_count];
        }
        else if (p_instance_ctrl->bitwidth <= SPI_BIT_WIDTH_16_BITS)
        {
            /* Write 16-bit data */
            tx_data = ((uint16_t const *) p_instance_ctrl->p_tx_data)[tx_count];
        }
        else
        {
            /* Write 32-bit data */
            tx_data = ((uint32_t const *) p_instance_ctrl->p_tx_data)[tx_count];
        }
    }

    /* Write the prepared data to the selected channel TX register. */
    p_mspi_regs[chip_select].TXDA0 = tx_data;
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_instance_ctrl     Pointer to SPI instance control block
 * @param[in]     event      Event code
 **********************************************************************************************************************/
static void r_mspi_call_callback (mspi_instance_ctrl_t * p_instance_ctrl, spi_event_t event)
{
    spi_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    spi_callback_args_t * p_args = p_instance_ctrl->p_callback_memory;
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

    p_args->channel   = p_instance_ctrl->p_cfg->channel;
    p_args->event     = event;
    p_args->p_context = p_instance_ctrl->p_context;

    if (NULL != p_instance_ctrl->p_callback)
    {
        p_instance_ctrl->p_callback(p_args);
    }

    if (NULL != p_instance_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_instance_ctrl->p_callback_memory = args;
    }
}

/*******************************************************************************************************************//**
 * Calculate size of fifo transfer base on length of data.
 *
 **********************************************************************************************************************/
static uint8_t r_mspi_fifo_refill_stage (mspi_instance_ctrl_t * p_instance_ctrl)
{
    mspi_extended_cfg_t * p_extend     = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    uint8_t               refill_stage = 0U;
    uint32_t              length       = p_instance_ctrl->length;
    uint8_t               stage_size   = p_instance_ctrl->stage_size;
    if (MSPI_TRANSFER_MODE_FIFO_MEMORY ==
        p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
    {
        refill_stage = (((length + (stage_size / 2U) - 1U) / (stage_size / 2U)) * (stage_size / 2U) - length);
    }
    else
    {
        /* Refill data equal to 0 in direct mode and buffer memory mode */
        refill_stage = 0U;
    }

    /* Return transfer count */
    return refill_stage;
}

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)

/*******************************************************************************************************************//**
 * Select bit position for register MSPITGDMAALT
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static uint8_t r_mspi_dma_alternative (mspi_instance_ctrl_t * p_instance_ctrl)
{
    mspi_extended_cfg_t * p_extend             = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    uint8_t               unit                 = p_extend->unit;
    uint8_t               channel              = p_instance_ctrl->p_cfg->channel;
    uint8_t               dmaAlternative       = p_extend->mspi_dma_alternative;
    uint8_t               dmaAlternativeResult = MSPI_TRANSFER_INV_CHANNEL;

    if (BSP_FEATURE_MSPI_ALT_DMA_MAX_UNIT > unit)
    {
        if ((MSPI_TRANSFER_ALTERNATIVE_ENABLE == dmaAlternative) &&
            (MSPI_TRANSFER_ALTERNATIVE_DISABLE != (BSP_FEATURE_MSPI_ALT_DMA_UNIT & (1U << channel))))
        {
            dmaAlternativeResult = (channel % BSP_FEATURE_MSPI_ALT_GAP_CHANNEL) - BSP_FEATURE_MSPI_ALT_PAIR_CHANNEL +
                                   (BSP_FEATURE_MSPI_ALT_PAIR_CHANNEL * (channel / BSP_FEATURE_MSPI_ALT_GAP_CHANNEL)) +
                                   (unit * BSP_FEATURE_MSPI_ALT_GAP_UNIT);
        }

        if (BSP_FEATURE_MSPI_ALT_DMA_MAX_BIT < dmaAlternativeResult)
        {
            dmaAlternativeResult = MSPI_TRANSFER_INV_CHANNEL;
        }
    }

 #if (BSP_MCU_GROUP_RH850U2Cx)
    if ((MSPI2_UNIT == unit) && (MSPI_CS_CHANNEL_3 == channel))
    {
        if (MSPI_TRANSFER_ALTERNATIVE_ENABLE == dmaAlternative)
        {
            dmaAlternativeResult = BSP_FEATURE_DMAAS23;
        }
    }
    else if ((MSPI2_UNIT == unit) && (MSPI_CS_CHANNEL_1 == channel))
    {
        if (MSPI_TRANSFER_ALTERNATIVE_ENABLE == dmaAlternative)
        {
            dmaAlternativeResult = BSP_FEATURE_DMAAS21;
        }
    }
    else
    {
        /* Do nothing */
    }
 #endif

    return dmaAlternativeResult;
}

/*******************************************************************************************************************//**
 * Select bit position for register MSPITGDTSALT
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static uint8_t r_mspi_dts_alternative (mspi_instance_ctrl_t * p_instance_ctrl)
{
    mspi_extended_cfg_t * p_extend             = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    uint8_t               unit                 = p_extend->unit;
    uint8_t               channel              = p_instance_ctrl->p_cfg->channel;
    uint8_t               dtsAlternative       = p_extend->mspi_dma_alternative;
    uint8_t               dtsAlternativeResult = MSPI_TRANSFER_INV_CHANNEL;
    if (BSP_FEATURE_MSPI_ALT_DTS_MAX_UNIT > unit)
    {
        if ((MSPI_TRANSFER_ALTERNATIVE_ENABLE == dtsAlternative) &&
            (MSPI_TRANSFER_ALTERNATIVE_DISABLE != (BSP_FEATURE_MSPI_ALT_DTS_UNIT & (1U << channel))))
        {
            dtsAlternativeResult = BSP_FEATURE_MSPI_ALT_DTS_MAX_BIT -
                                   ((BSP_FEATURE_MSPI_ALT_DTS_MAX_UNIT - unit - 1U) * BSP_FEATURE_MSPI_ALT_GAP_CHANNEL +
                                    (BSP_FEATURE_MSPI_ALT_MAX_CHANNEL - channel));
        }

        if (BSP_FEATURE_MSPI_ALT_DTS_MAX_BIT < dtsAlternativeResult)
        {
            dtsAlternativeResult = MSPI_TRANSFER_INV_CHANNEL;
        }
    }

 #if (BSP_MCU_GROUP_RH850U2Bx)
    if ((MSPI0_UNIT == unit) && (MSPI_CS_CHANNEL_6 == channel))
    {
        dtsAlternativeResult = BSP_FEATURE_DTSAS06;
    }
    else if ((MSPI0_UNIT == unit) && (MSPI_CS_CHANNEL_7 == channel))
    {
        dtsAlternativeResult = BSP_FEATURE_DTSAS07;
    }
    else
    {
        /* Do nothing */
    }
 #endif

    return dtsAlternativeResult;
}

/*******************************************************************************************************************//**
 * Calculate size of data transfer base on length of data.
 *
 * @param[in]  bitwidth       length of data (ex: 8 bits, 16 bits,...)
 **********************************************************************************************************************/
static transfer_size_t r_mspi_cal_transfer_size (uint32_t bitwidth)
{
    if (bitwidth <= SPI_BIT_WIDTH_8_BITS)
    {
        return TRANSFER_SIZE_1_BYTE;
    }

    if (bitwidth <= SPI_BIT_WIDTH_16_BITS)
    {
        return TRANSFER_SIZE_2_BYTE;
    }

    return TRANSFER_SIZE_4_BYTE;
}

/*******************************************************************************************************************//**
 * Transfer callback for transmit.
 **********************************************************************************************************************/
void mspi_tx_transfer_callback (transfer_callback_args_t * p_transfer_args)
{
    mspi_instance_ctrl_t             * p_instance_ctrl  = (mspi_instance_ctrl_t *) p_transfer_args->p_context;
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;
    uint8_t channel = p_instance_ctrl->p_cfg->channel;

    /*Clear DMA trigger TX */
    p_mspi_intf_regs[0].INTFC = (MSPI_NTFC_CLEAR << channel);
}

/*******************************************************************************************************************//**
 * Transfer callback for receive.
 **********************************************************************************************************************/
void mspi_rx_transfer_callback (transfer_callback_args_t * p_transfer_args)
{
    mspi_instance_ctrl_t             * p_instance_ctrl  = (mspi_instance_ctrl_t *) p_transfer_args->p_context;
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;
    uint8_t channel = p_instance_ctrl->p_cfg->channel;

    /*Clear DMA trigger RX */
    p_mspi_intf_regs[1].INTFC = (MSPI_NTFC_CLEAR << channel);
}

/*******************************************************************************************************************//**
 * Transfer callback for frame end.
 **********************************************************************************************************************/
void mspi_fe_transfer_callback (transfer_callback_args_t * p_transfer_args)
{
    mspi_instance_ctrl_t             * p_instance_ctrl  = (mspi_instance_ctrl_t *) p_transfer_args->p_context;
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;
    uint8_t channel = p_instance_ctrl->p_cfg->channel;

    /*Clear DMA trigger FC */
    p_mspi_intf_regs[2].INTFC = (MSPI_NTFC_CLEAR << channel);
}

#endif

/*******************************************************************************************************************//**
 * ISR called when data is loaded into MSPI data register from the shift register.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mspi_txi_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    /* Get current IRQ*/
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
#endif

    /* Clear the TXI interrupt flag */
    R_BSP_IrqStatusClear(irq);

    /* Get control and configuration */
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /*Transmit data*/
    r_mspi_transmit(p_instance_ctrl);

    /* Restore RTOS context */
    FSP_CONTEXT_RESTORE;
}

/*******************************************************************************************************************//**
 * ISR called when MSPI finished to receive data and can be read data from register.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mspi_rxi_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    /* Get current IRQ*/
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
#endif

    /* Clear the RXI interrupt flag */
    R_BSP_IrqStatusClear(irq);

    /* Get control and configuration */
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /*Receive data*/
    r_mspi_receive(p_instance_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}

/*******************************************************************************************************************//**
 * ISR called error transmit/receive occur.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mspi_err_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    /* Get current IRQ*/
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
#endif

    /* Clear the ERR interrupt flag */
    R_BSP_IrqStatusClear(irq);

    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    mspi_extended_cfg_t  * p_extend        =
        ((mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    R_MSPI0_MSPI0_CH_Type            * p_mspi_regs      = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;
    uint8_t  channel     = p_instance_ctrl->p_cfg->channel;
    uint32_t rx_count    = 0U;
    uint8_t  remain_data = 0U;

    p_mspi_intf_regs[3].INTFC |= 1U << channel;

    /* Check error type and call the user callback with corresponding event code */
    if (1U == p_mspi_regs[channel].CEST_b.OVRUE)
    {
        /* Over-run error */
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_ERR_OVERRUN);

        remain_data = p_mspi_regs[channel].CSTR_b.FIRXN;
        p_mspi_regs[channel].CESTC_b.OVRUEC = 1U;
        p_instance_ctrl->p_regs->CTL0       = (uint8_t) ~R_MSPI0_CTL0_EN_Msk;

        /* Check Memory mode*/
        if (MSPI_TRANSFER_MODE_DIRECT_ACCESS ==
            p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
        {
            /* Read MSPInRXDAm0 until MSPInRXRQFm = 0*/
            while (0U != p_mspi_regs[channel].CSTR_b.RXRQF)
            {
                r_mspi_read_register(p_instance_ctrl);
            }
        }
        else if (MSPI_TRANSFER_MODE_FIFO_MEMORY ==
                 p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
        {
            /* Read the unread reception data stored in FIRXN */
            if (0U != p_mspi_regs[channel].CSTR_b.FIRXN)
            {
                for (rx_count = 0U; rx_count <= remain_data; rx_count++)
                {
                    r_mspi_read_register(p_instance_ctrl);
                }
            }
        }
        else
        {
            /* Do nothing */
        }
    }

    if (1U == p_mspi_regs[channel].CEST_b.OVREE)
    {
        /* Over-read error */
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_ERR_READ_OVERFLOW);
        p_mspi_regs[channel].CESTC_b.OVREEC = 1U;
    }

    if (1U == p_mspi_regs[channel].CEST_b.OVWRE)
    {
        /* Over-write error */
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_FIFO_OVERFLOW);
        p_mspi_regs[channel].CESTC_b.OVWREC = 1U;
    }

    if (1U == p_instance_ctrl->p_regs->MSPI0_CH[p_instance_ctrl->p_cfg->channel].CEST_b.DCE)
    {
        /* Data consistency error */
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_DATA_CONSISTENCY);
        p_instance_ctrl->p_regs->MSPI0_CH[p_instance_ctrl->p_cfg->channel].CESTC_b.DCEC = 1U;
    }

    if (1U == p_instance_ctrl->p_regs->MSPI0_CH[p_instance_ctrl->p_cfg->channel].CEST_b.CE)
    {
        /* Parity error (only in transmit-and-receive mode and transmit-only mode) */
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_CRC_ERROR);
        p_instance_ctrl->p_regs->MSPI0_CH[p_instance_ctrl->p_cfg->channel].CESTC_b.CEC = 1U;
    }

    if (1U == p_instance_ctrl->p_regs->MSPI0_CH[p_instance_ctrl->p_cfg->channel].CEST_b.PE)
    {
        /* Parity error (only in transmit-and-receive mode and transmit-only mode) */
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_ERR_PARITY);
        p_instance_ctrl->p_regs->MSPI0_CH[p_instance_ctrl->p_cfg->channel].CESTC_b.PEC = 1U;
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}

/*******************************************************************************************************************//**
 * ISR called when frame count is returned to zero.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mspi_fe_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    /* Variable declarations */
    mspi_instance_ctrl_t             * p_instance_ctrl;
    mspi_extended_cfg_t              * p_extend;
    R_MSPI0_MSPI0_CH_Type            * p_mspi_regs;
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs;
    uint8_t  channel;
    uint32_t transfer_mode;
    uint32_t current_chunk_size;
    uint32_t remain_data;
    uint32_t next_chunk;
    uint32_t max_ram_size;
    uint32_t total_hw_ram = BSP_FEATURE_MSPI_RAM_WORDS_MAX;
    uint32_t usable_ram_words;

    /* Get current IRQ */
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
#endif

    /* Clear the ERR interrupt flag */
    R_BSP_IrqStatusClear(irq);

    /* Initialize control parameters */
    p_instance_ctrl  = (mspi_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    p_extend         = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    channel          = p_instance_ctrl->p_cfg->channel;
    p_mspi_regs      = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;
    p_mspi_intf_regs = (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;
    transfer_mode    = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;

    /* Clear frame end status register */
    p_mspi_intf_regs[2].INTFC = (1U << channel);

    /* Disable the TXI IRQ to prevent unnecessary interrupts */
    R_BSP_IrqDisable(p_instance_ctrl->p_cfg->txi_irq);

    /* Chunking logic for Buffer Memory Mode */
    if (MSPI_TRANSFER_MODE_BUFFER_MEMORY == transfer_mode)
    {
        current_chunk_size = p_mspi_regs[channel].CFSET;

        /* Process received data */
        if (MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE)
        {
            if (NULL != p_instance_ctrl->p_rx_data)
            {
                r_mspi_buffer_read(p_instance_ctrl);
            }

            p_instance_ctrl->rx_count += current_chunk_size;
        }

        /* Update transmit counter */
        if (MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE)
        {
            p_instance_ctrl->tx_count += current_chunk_size;
        }
        else
        {
            /* Synchronize counters in Receive-Only mode */
            p_instance_ctrl->tx_count = p_instance_ctrl->rx_count;
        }

        /* Check for remaining data to transfer */
        if (p_instance_ctrl->tx_count < p_instance_ctrl->length)
        {
            usable_ram_words = total_hw_ram - (p_extend->mspi_ram_start / 4U);
            if (MSPI_SELECTABLE_MODE_TRANSMIT_RECEIVE == p_extend->mspi_select)
            {
                max_ram_size = usable_ram_words / 2U;
            }
            else
            {
                max_ram_size = usable_ram_words;
            }

            if (p_instance_ctrl->bitwidth > SPI_BIT_WIDTH_32_BITS)
            {
                max_ram_size = max_ram_size / 2U;
            }

            /* Calculate next chunk size */
            remain_data = p_instance_ctrl->length - p_instance_ctrl->tx_count;

            if (remain_data > max_ram_size)
            {
                next_chunk = max_ram_size;
            }
            else
            {
                next_chunk = remain_data;
            }

            /* Configure hardware frame count for next chunk */
            p_mspi_regs[channel].CFSET = next_chunk;

            /* Write next chunk to MSPI RAM  */
            if (MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE)
            {
                if (NULL != p_instance_ctrl->p_tx_data)
                {
                    r_mspi_buffer_write(p_instance_ctrl);
                }
            }

            /* Trigger hardware for the next chunk */
            p_mspi_regs[channel].CSTS = (R_MSPI0_MSPI0_CH_CSTS_CHENS_Msk | R_MSPI0_MSPI0_CH_CSTS_ACTFS_Msk);

            /* Exit ISR early, await next Frame End interrupt */
            FSP_CONTEXT_RESTORE;

            return;
        }
    }

    /* Notify application via callback */
    if (MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE)
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_TRANSFER_COMPLETE);
    }
    else if (MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE)
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_RECEIVE_COMPLETE);
    }

    /* Re-enable the TXI IRQ and clear the pending IRQ. */
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    MSPI_ENABLE_IRQ(p_extend->com_ored_int,
                    p_instance_ctrl->p_cfg->txi_irq,
                    p_instance_ctrl->p_cfg->txi_ipl,
                    p_instance_ctrl);
#else
    R_BSP_IrqEnable(p_instance_ctrl->p_cfg->txi_irq);
#endif

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}

/* End of file R_MSPI. */
