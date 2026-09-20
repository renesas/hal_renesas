/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "r_mspi.h"                                                              /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define MSPI_OPEN                        (0x4D535049ULL)                         // 'MSPI' in ASCII
#define MSPI_NTFC_CLEAR                  (0x1U)                                  // Clear interrupt flags
#define MSPI_CTL1_CLEAR                  (0x00U)                                 // Clear CTL1 configuration
#define MSPI_CTL2_CLEAR                  (0x00U)                                 // Clear CTL2 configuration
#define MSPI_SSEL_CLEAR                  (0x00U)                                 // Clear CSR configuration
#define MSPI_CFG0_CLEAR                  (0x00700000U)                           // Clear CFG0 configuration
#define MSPI_CFG1_CLEAR                  (0x01000000U)                           // Clear CFG1 configuration
#define MSPI_CFG2_CLEAR                  (0x0020U)                               // Clear CFG2 configuration
#define MSPI_TGCTL_CLEAR                 (0x0000U)                               // Clear DMAC/DTS activation source
#define MSPI_BR_CLEAR                    (0x01U)                                 // Clear Baud rate setting
#define MSPI_CFSET_CLEAR                 (0x0001U)                               // Clear CFSET setting
#define MSPI_SEUP_CLEAR                  (0x0001U)                               // Clear SEUP setting
#define MSPI_HOLD_CLEAR                  (0x0001U)                               // Clear HOLD setting
#define MSPI_IDLE_CLEAR                  (0x0001U)                               // Clear IDLE setting
#define MSPI_INDA_CLEAR                  (0x00U)                                 // Clear INDA setting
#define MSPI_RASTAD_CLEAR                (0x00U)                                 // Clear RASTAD setting
#define MSPI_INIT_RAM_ADDR               (0x1000U)                               // Initial RAM address
#define MSPI_STAGE_SIZE_8                (0x8U)                                  // FIFO stage size is 8 bits
#define MSPI_STAGE_SIZE_16               (0x10U)                                 // FIFO stage size is 16 bits
#define MSPI_STAGE_SIZE_32               (0x20U)                                 // FIFO stage size is 32 bits
#define MSPI_SSEL_SLAVE                  (0x0000U)                               // Clear SSEL setting
#define ALIGNMENT_BOUNDARY_BYTES         (4U)                                    // Alignment boundary in bytes
#define ALIGNMENT_MASK                   (0x3U)                                  // Alignment mask for 4 bytes
#define MSPI_PAIR_SIZE                   (2U)                                    // Number of MSPI units per pair
#define MSPI0_UNIT                       (0U)                                    // MSPI unit 0
#define MSPI1_UNIT                       (1U) /* PRQA S 1534 # Advisory R-2.5 */ // MSPI unit 1
#define MSPI2_UNIT                       (2U)                                    // MSPI unit 2
#define MSPI3_UNIT                       (3U) /* PRQA S 1534 # Advisory R-2.5 */ // MSPI unit 3
#define MSPI_LENGTH_PER_COUNT_1          (1U)                                    // Length per count for 1 byte
#define MSPI_LENGTH_PER_COUNT_2          (2U)                                    // Length per count for 2 bytes
#define MSPI_LENGTH_PER_COUNT_4          (4U)                                    // Length per count for 4 bytes
#define MSPI_LENGTH_PER_COUNT_8          (8U)                                    // Length per count for 8 byte
#define MSPI_LENGTH_MAX                  (0xFFFFFFFFU)                           // Maximum transfer length
#define MSPI_TRANSFER_ENABLE             (1U)                                    // MSPI enable transfer support
#define MSPI_CGTL_CHANNEL_GAP            (4U)                                    // MSPI channel gap in register MSPITGCTL
#define MSPI_TGCTL1_FIRST_CHANNEL        (8U)                                    // First channel controlled by register MSPITGCTL1
#if (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > MSPI_TGCTL1_FIRST_CHANNEL)

 #define MSPI_TGCTL1_UNIT_MAX            ((uint32_t) BSP_FEATURE_MSPI_ALT_DMA_MAX_UNIT)

 #define MSPI_TGCTL1_OFFSET              (0x30U)
 #define MSPI_TGCTL1(p_regs_tg, unit)                                                           /* PRQA S 3472 # Advisory D-4.9 */ \
    (*(volatile uint32_t *) ((uintptr_t) (p_regs_tg) + MSPI_TGCTL1_OFFSET                                                          \
                             + ((uint32_t) (unit) * sizeof(uint32_t))))
#endif
#define MSPI_TRANSFER_INV_CHANNEL        (31U)                                                  // MSPI channel invalid for Trigger Alternative
#define MSPI_MINIMUM_CYCLE_INIT          (4U)                                                   // Minimum cycles need to wait for MSPI initialization

#define MSPI_DIRECT_MAX_FRAME_COUNT      (65535U)                                               // Max frames per hardware pass (16-bit CFSET) in Direct mode

/* INTF register group indices (per-direction notification/mask register banks). */
#define MSPI_INTF_IDX_TX                 (0U)                                                   // TX notification/mask bank
#define MSPI_INTF_IDX_RX                 (1U)                                                   // RX notification/mask bank
#define MSPI_INTF_IDX_FE                 (2U)                                                   // Frame-end notification/mask bank
#define MSPI_INTF_IDX_ERR                (3U)                                                   // Error notification/mask bank

/* Single-bit mask applied to a per-channel ISR mask configuration bit. */
#define MSPI_ISR_MASK_BIT                (1U)                                                   // One configuration bit per channel

/* One open-status / notification bit per channel (used in INTF/status bitmasks). */
#define MSPI_CHANNEL_STATUS_BIT          (1U)                                                   // Single per-channel bit

/* Timing / clock-cycle helpers used when re-enabling the unit. */
#define MSPI_US_PER_SEC                  (1000000U)                                             // Microseconds per second

/* 64-bit frame handling in buffer mode (split into two 32-bit words). */
#define MSPI_WORDS_PER_64BIT             (2U)                                                   // 32-bit words per 64-bit frame
#define MSPI_LOWER_WORD_MASK             (0xFFFFFFFFULL)                                        // Lower 32-bit mask of a 64-bit value
#define MSPI_UPPER_WORD_SHIFT            (32U)                                                  // Shift to extract the upper 32-bit word

/* Full-duplex splits the RAM/stage evenly between TX and RX. */
#define MSPI_HALF_DIVISOR                (2U)                                                   // Divide-by-two for half-stage / half-RAM

/* FIFO minimum usable RAM (bytes) required for the 32-/16-stage sizes. */
#define MSPI_FIFO_MIN_BYTES_STAGE_32     (32U)                                                  // Minimum usable bytes for 32-stage FIFO
#define MSPI_FIFO_MIN_BYTES_STAGE_16     (16U)                                                  // Minimum usable bytes for 16-stage FIFO

/* CFG0 transfer-mode field encodings (MD bits). */
#define MSPI_CFG0_MD_BUFFER              (1U)                                                   // MD = Buffer Memory mode
#define MSPI_CFG0_MD_FIFO                (2U)                                                   // MD = FIFO Memory mode

/* CFG4 FIFO stage-size field encodings (SIZE bits). */
#define MSPI_CFG4_SIZE_16                (1U)                                                   // SIZE = 16 stage
#define MSPI_CFG4_SIZE_32                (2U)                                                   // SIZE = 32 stage

/* Positional/index helpers replacing magic numbers. */
#define MSPI_WORD_OFFSET_UPPER           (1U)                                                   // Upper 32-bit word index within a 64-bit frame
#define MSPI_TG_ACTIVATION_BIT_OFFSET    (1U)                                                   // Activation-source bit follows the trigger-source bit
#define MSPI_MIN_DELAY_US                (1U)                                                   // Minimum software delay in microseconds
#define MSPI_SLAVE_CHANNEL               (0U)                                                   // Only channel 0 is valid in slave mode

/* Value of base address Unit 0 and Unit 1 */
#define MSPI_BASE_ADDRESS_START(unit)    ((MSPI0_UNIT == (unit)) ? R_MSPI0_BASE : R_MSPI1_BASE) /* PRQA S 3472 # Advisory D-4.9 */

/* Offset between MSPI register groups (even unit4-unit2, odd unit3-unit1) */
/* PRQA S 3472 ++ # Advisory D-4.9 */
#define MSPI_REG_SIZE(unit)              ((0U == ((unit) % MSPI_PAIR_SIZE)) ? \
                                          (R_MSPI4_BASE - R_MSPI2_BASE) :     \
                                          (R_MSPI5_BASE - R_MSPI3_BASE))

/* PRQA S 3472 -- */

/* Gap between two MSPI unit in the same MSPI group */
#define MSPI_REG_GROUP_GAP    (R_MSPI3_BASE - R_MSPI2_BASE)

/* Gap between two MSPI_INTF unit in the same MSPI_INTF group */
/* PRQA S 3472 ++ # Advisory D-4.9 */
#define MSPI_INTF_REG_SIZE(unit)    ((0U == ((unit) % MSPI_PAIR_SIZE)) ?       \
                                     (R_MSPI2_INTF_BASE - R_MSPI0_INTF_BASE) : \
                                     (R_MSPI3_INTF_BASE - R_MSPI1_INTF_BASE))

/* PRQA S 3472 -- */

/* Gap between two MSPI_INTF unit in the same MSPI_INTF group */
#define MSPI_INTF_REG_GROUP_GAP        (R_MSPI1_INTF_BASE - R_MSPI0_INTF_BASE)

#define MSPI_COMPLETE_SRC_FE_ISR       (0U)                                                                     // Non-DMA: frame-end ISR drives
#define MSPI_COMPLETE_SRC_FE_XFER      (1U)                                                                     // DMA with FE as trigger: fe transfer cb
#define MSPI_COMPLETE_SRC_TXRX_XFER    (2U)                                                                     // DMA with TX/RX trigger: tx/rx transfer cb

#define MSPI_BITS_PER_WORD             (32U)                                                                    // Number of bits in one 32-bit transfer word

/* Ceil division: number of 32-bit words needed to hold a frame of 'bits' bits. */
#define MSPI_WORD_COUNT(bits)            (((uint32_t) (bits) + (MSPI_BITS_PER_WORD - 1U)) / MSPI_BITS_PER_WORD) /* PRQA S 3472 # Advisory D-4.9 */

/* First set bit from the LSB, 0-based (__SCH1R returns a 1-based position, so subtract 1). */
#define MSPI_FIRST_CHANNEL_BIT(value)    ((uint8_t) ((uint32_t) __SCH1R((value)) - 1U))                         /* PRQA S 3472 # Advisory D-4.9 */

#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)

/* Disable the IRQ, routing to the ORed variant when configured. */
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

/* Enable the IRQ, routing to the ORed variant when configured. */
 #define MSPI_ENABLE_IRQ(ored_field, irq_field, irq_ipl, p_context)  \
    do {                                                             \
        if (BSP_ORED_MSPI_DISABLE == (ored_field))                   \
        {                                                            \
            R_BSP_IrqCfgEnable(irq_field, irq_ipl, p_context);       \
        }                                                            \
        else                                                         \
        {                                                            \
            R_BSP_ORed_IrqCfgEnable(ored_field, irq_ipl, p_context); \
        }                                                            \
    } while (0)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Unit-level ISR context: everything the shared/dedicated interrupt handlers need for one MSPI unit. */
typedef struct st_mspi_unit_ctrl                                            /* PRQA S 1536, 3213 # Advisory R-2.4, Advisory R-2.4 */
{
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_regs_intf;                         // INTF register bank of this unit
    uint8_t                unit;                                            // Unit index
    uint32_t               channel_status;                                  // Bitmask of currently open channels (bit == channel index)
    uint32_t               ram_active_mask;                                 // Bitmask of channels currently owning a RAM window
    mspi_instance_ctrl_t * p_ctrl[BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT]; // Per-channel control block
} mspi_unit_ctrl_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static fsp_err_t r_mspi_hw_config(mspi_instance_ctrl_t * p_instance_ctrl);
static void      r_mspi_interrupt_config(mspi_instance_ctrl_t * p_instance_ctrl);

static void r_mspi_shared_line_need(mspi_instance_ctrl_t * p_instance_ctrl, bool * p_need_tx, bool * p_need_rx);
static bool r_mspi_irq_already_active(mspi_unit_ctrl_t * p_unit, mspi_instance_ctrl_t * p_self, IRQn_Type irq);
static void r_mspi_error_handle(mspi_instance_ctrl_t * p_instance_ctrl);

static void r_mspi_transmit(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_receive(mspi_instance_ctrl_t * p_instance_ctrl);

static fsp_err_t r_mspi_start_read_write(spi_ctrl_t * const    p_ctrl,
                                         void const          * p_src,
                                         void                * p_dest,
                                         uint32_t const        length,
                                         spi_bit_width_t const bit_width);
static fsp_err_t r_mspi_transfer_config(mspi_instance_ctrl_t * p_instance_ctrl, spi_cfg_t const * const p_cfg);

static void r_mspi_buffer_write(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_buffer_read(mspi_instance_ctrl_t const * p_instance_ctrl);
static void r_mspi_fifo_write(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_fifo_read(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_direct_write(mspi_instance_ctrl_t * p_instance_ctrl);
static void r_mspi_direct_read(mspi_instance_ctrl_t * p_instance_ctrl);

static uint8_t  r_mspi_align_ram(uint32_t length, uint32_t ram_init, uint32_t bitwidth);
static uint8_t  r_mspi_count_byte(uint32_t bitwidth);
static uint8_t  r_mspi_fifo_refill_stage(mspi_instance_ctrl_t const * p_instance_ctrl);
static void     r_mspi_notify_if_complete(mspi_instance_ctrl_t * p_instance_ctrl);
static uint32_t r_mspi_max_chunk_frames(mspi_instance_ctrl_t * p_instance_ctrl);
static void     r_mspi_continue_or_complete(mspi_instance_ctrl_t * p_instance_ctrl, bool ack_tx, bool ack_rx);
static uint32_t r_mspi_ram_max_frames(mspi_instance_ctrl_t * p_instance_ctrl);

/* Runtime RAM-region arbitration (buffer/FIFO modes). */
static uint32_t  r_mspi_ram_free_bytes(mspi_instance_ctrl_t * p_instance_ctrl);
static void      r_mspi_ram_window(mspi_instance_ctrl_t * p_instance_ctrl, uint32_t * p_start, uint32_t * p_end);
static fsp_err_t r_mspi_ram_region_acquire(mspi_instance_ctrl_t * p_instance_ctrl);
static void      r_mspi_ram_region_release(mspi_instance_ctrl_t * p_instance_ctrl);

static void r_mspi_read_register(mspi_instance_ctrl_t const * p_instance_ctrl);
static void r_mspi_write_register(mspi_instance_ctrl_t const * p_instance_ctrl);
static void r_mspi_call_callback(mspi_instance_ctrl_t const * p_instance_ctrl, spi_event_t event);

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
void mspi_tx_transfer_callback(transfer_callback_args_t * p_transfer_args);
void mspi_rx_transfer_callback(transfer_callback_args_t * p_transfer_args);
void mspi_fe_transfer_callback(transfer_callback_args_t * p_transfer_args);

static transfer_size_t r_mspi_cal_transfer_size(uint32_t bitwidth);

static void    r_mspi_buffer_dma_rx_config(mspi_instance_ctrl_t const * p_instance_ctrl);
static uint8_t r_mspi_dma_alternative(mspi_instance_ctrl_t const * p_instance_ctrl);
static uint8_t r_mspi_dts_alternative(mspi_instance_ctrl_t const * p_instance_ctrl);

#endif

/* PRQA S 3449, 3451 ++ # DP_MSR_R-8.5_UC1, DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void mspi_txi_isr(void);
BSP_INTERRUPT_ATTRIBUTE void mspi_rxi_isr(void);
BSP_INTERRUPT_ATTRIBUTE void mspi_err_isr(void);
BSP_INTERRUPT_ATTRIBUTE void mspi_fe_isr(void);

/* PRQA S 3449, 3451 -- */

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* One unit-level ISR context per MSPI unit (see mspi_unit_ctrl_t). */
static mspi_unit_ctrl_t g_mspi_unit_ctrl[BSP_FEATURE_MSPI_MAX_UNIT] = {0};

/* PRQA S 1502, 3635 ++ # Advisory R-2.8 & Advisory R-17.12 */
const spi_api_t g_spi_on_mspi =
{
    .open        = R_MSPI_Open,
    .read        = R_MSPI_Read,
    .write       = R_MSPI_Write,
    .writeRead   = R_MSPI_WriteRead,
    .close       = R_MSPI_Close,
    .callbackSet = R_MSPI_CallbackSet
};

/* PRQA S 1502, 3635 -- */

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup SPI
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/* PRQA S 0662 ++ # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/* PRQA S 1505 ++ # Advisory R-8.7 */

/*******************************************************************************************************************//**
 * This functions initializes a unit for MSPI communication mode. Implements @ref spi_api_t::open.
 *
 * @retval  FSP_SUCCESS                    Unit opened successfully.
 * @retval  FSP_ERR_ASSERTION              Pointer to MSPI control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN           Control block has already been opened or unit is being used by another
 *                                         instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_IP_UNIT_NOT_PRESENT    Requested unit does not exist on this device.
 * @retval  FSP_ERR_IP_CHANNEL_NOT_PRESENT Requested channel does not exist on the selected unit.
 * @retval  FSP_ERR_INVALID_CHANNEL        A nonzero channel was selected in slave mode (only channel 0 is allowed).
 * @retval  FSP_ERR_IN_USE                 Unit-level configuration conflict with a channel already running.
 * @retval  FSP_ERR_INVALID_SIZE           Configured FIFO stage size does not fit the usable RAM.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_Open (spi_ctrl_t * p_ctrl, spi_cfg_t const * const p_cfg)
{
    mspi_instance_ctrl_t      * p_instance_ctrl = (mspi_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    mspi_extended_cfg_t const * p_extend;

#if MSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MSPI_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);

    /* Check the unit exists. */
    FSP_ERROR_RETURN(BSP_FEATURE_MSPI_MAX_UNIT > ((mspi_extended_cfg_t *) p_cfg->p_extend)->unit,       /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1, Advisory R-11.5 */
                     FSP_ERR_IP_UNIT_NOT_PRESENT);

    /* Check the channel is valid for the unit before any array access. */
    FSP_ERROR_RETURN((((mspi_extended_cfg_t *) p_cfg->p_extend)->mspi_number_of_cs > p_cfg->channel) && /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1, Advisory R-11.5 */
                     (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > p_cfg->channel),
                     FSP_ERR_IP_CHANNEL_NOT_PRESENT);

    /* Only channel 0 is allowed in slave mode. */
    FSP_ERROR_RETURN((SPI_MODE_SLAVE != p_cfg->operating_mode) || (MSPI_SLAVE_CHANNEL == p_cfg->channel),
                     FSP_ERR_INVALID_CHANNEL);
#endif

    p_extend = (mspi_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1, Advisory R-11.5 */
    uint8_t channel = p_cfg->channel;
    uint8_t unit    = p_extend->unit;

    /* Get the register address of the unit. */
    p_instance_ctrl->p_cfg             = p_cfg;
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    if (MSPI2_UNIT > p_extend->unit)
    {
        /* Get the register address of the unit 0-1-2 */
        p_instance_ctrl->p_regs = (R_MSPI0_Type *) MSPI_BASE_ADDRESS_START(p_extend->unit); /* PRQA S 0306, 3469 # Advisory R-11.4, Advisory D-4.9 */
    }
    else
    {
        /* For the remaining unit, address is calculate from unit 0 base address */
        p_instance_ctrl->p_regs =
            (R_MSPI0_Type *) (R_MSPI2_BASE + (MSPI_REG_GROUP_GAP * ((uint32_t) p_extend->unit % MSPI_PAIR_SIZE)) + /* PRQA S 0306 # Advisory R-11.4 */
                              (MSPI_REG_SIZE(p_extend->unit) *                                                     /* PRQA S 3469 # Advisory D-4.9 */
                               (((uint32_t) p_extend->unit - MSPI2_UNIT) / MSPI_PAIR_SIZE)));
    }

    /* Calculate address of MSPIINTF register */
    p_instance_ctrl->p_regs_intf =
        (R_MSPI0_INTF_Type *) (R_MSPI0_INTF_BASE +                                                                     /* PRQA S 0306 # Advisory R-11.4 */
                               (MSPI_INTF_REG_GROUP_GAP * ((uint32_t) p_extend->unit % MSPI_PAIR_SIZE)) +
                               (MSPI_INTF_REG_SIZE(p_extend->unit) * (((uint32_t) p_extend->unit) / MSPI_PAIR_SIZE))); /* PRQA S 3469 # Advisory D-4.9 */

    /* Calculate address of MSPITG register */
    p_instance_ctrl->p_regs_tg = (R_MSPITG_Type *) R_MSPITG_BASE;                                                      /* PRQA S 0306 # Advisory R-11.4 */

    /* Configure hardware registers according to the r_spi_api configuration structure. */
    fsp_err_t err = r_mspi_hw_config(p_instance_ctrl);

    /* Configure for transfer support */
    if (FSP_SUCCESS == err)
    {
        err = r_mspi_transfer_config(p_instance_ctrl, p_cfg);
    }

    /* Enable interrupt */
    if (FSP_SUCCESS == err)
    {
        r_mspi_interrupt_config(p_instance_ctrl);

        /* Publish the channel only after every fallible step has succeeded. */
        g_mspi_unit_ctrl[unit].channel_status |= (uint32_t) (MSPI_CHANNEL_STATUS_BIT << channel); /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */

        /* Mark driver as open */
        p_instance_ctrl->open = MSPI_OPEN;                                                        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    }

    return err;
}

/*******************************************************************************************************************//**
 * Aborts any in progress transfers on this channel. Removes the channel from the unit, releases its RAM window,
 * disables only the interrupt lines this channel no longer shares with a sibling, and when it is the last open
 * channel on the unit, tears down the unit-level registers.
 *
 * @retval  FSP_SUCCESS              Unit successfully closed.
 * @retval  FSP_ERR_ASSERTION        Pointer to MSPI control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_Close (spi_ctrl_t * const p_ctrl)
{
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if MSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MSPI_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    mspi_extended_cfg_t const * p_extend =
        (mspi_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend;                                                /* PRQA S 0316 # Advisory R-11.5 */
    R_MSPI0_MSPI0_CH_Type            * p_mspi_regs      = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;                             /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */

    uint8_t            channel = p_instance_ctrl->p_cfg->channel;
    uint8_t            unit    = p_extend->unit;
    mspi_unit_ctrl_t * p_unit  = &g_mspi_unit_ctrl[unit];

    /* Stop communicate in MSPI channel */
    p_mspi_regs[channel].CSTC_b.CHENC = R_MSPI0_MSPI0_CH_CSTC_CHENC_Msk;                           /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */

    /* Clear all register status of MSPI. */
    p_mspi_regs[channel].CESTC = R_MSPI0_MSPI0_CH_CESTC_PEC_Msk | R_MSPI0_MSPI0_CH_CESTC_CEC_Msk | /* PRQA S 4464 # DP_MSR_R-10.3_UC1 */
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

    /* Mask this channel's interrupt outputs so a stale flag cannot drive a shared line after close. */
    p_mspi_intf_regs[MSPI_INTF_IDX_TX].INTMSK  |= (uint32_t) ((uint32_t) MSPI_CHANNEL_STATUS_BIT << channel);
    p_mspi_intf_regs[MSPI_INTF_IDX_RX].INTMSK  |= (uint32_t) ((uint32_t) MSPI_CHANNEL_STATUS_BIT << channel);
    p_mspi_intf_regs[MSPI_INTF_IDX_FE].INTMSK  |= (uint32_t) ((uint32_t) MSPI_CHANNEL_STATUS_BIT << channel);
    p_mspi_intf_regs[MSPI_INTF_IDX_ERR].INTMSK |= (uint32_t) ((uint32_t) MSPI_CHANNEL_STATUS_BIT << channel);

    /* Clear all interrupt flags for this channel (write-1-to-clear). */
    p_mspi_intf_regs[MSPI_INTF_IDX_TX].INTFC  = (uint32_t) ((uint32_t) MSPI_NTFC_CLEAR << channel);
    p_mspi_intf_regs[MSPI_INTF_IDX_RX].INTFC  = (uint32_t) ((uint32_t) MSPI_NTFC_CLEAR << channel);
    p_mspi_intf_regs[MSPI_INTF_IDX_FE].INTFC  = (uint32_t) ((uint32_t) MSPI_NTFC_CLEAR << channel);
    p_mspi_intf_regs[MSPI_INTF_IDX_ERR].INTFC = (uint32_t) ((uint32_t) MSPI_NTFC_CLEAR << channel);

    /* Unregister this channel BEFORE evaluating the guard so it is excluded from the sibling scan, and release any
     * RAM window it still owns so a sibling may reuse the region. */
    p_unit->p_ctrl[channel]  = NULL;
    p_unit->channel_status  &= ~((uint32_t) (MSPI_CHANNEL_STATUS_BIT << channel)); /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
    p_unit->ram_active_mask &= ~((uint32_t) (MSPI_CHANNEL_STATUS_BIT << channel)); /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */

    /* Disable each interrupt line only when no OTHER open channel is routed to the same interrupt NUMBER. This
     * applies to BOTH ORed and plain shared interrupts. */
    if (!r_mspi_irq_already_active(p_unit, p_instance_ctrl, p_instance_ctrl->p_cfg->txi_irq))
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        MSPI_DISABLE_IRQ(p_extend->com_ored_int, p_instance_ctrl->p_cfg->txi_irq);
#else
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->txi_irq);
#endif
    }

    if (!r_mspi_irq_already_active(p_unit, p_instance_ctrl, p_instance_ctrl->p_cfg->rxi_irq))
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        MSPI_DISABLE_IRQ(p_extend->rec_ored_int, p_instance_ctrl->p_cfg->rxi_irq);
#else
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->rxi_irq);
#endif
    }

    if (!r_mspi_irq_already_active(p_unit, p_instance_ctrl, p_instance_ctrl->p_cfg->eri_irq))
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        MSPI_DISABLE_IRQ(p_extend->error_ored_int, p_instance_ctrl->p_cfg->eri_irq);
#else
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->eri_irq);
#endif
    }

    if (!r_mspi_irq_already_active(p_unit, p_instance_ctrl, p_instance_ctrl->p_cfg->tei_irq))
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        MSPI_DISABLE_IRQ(p_extend->frame_ored_int, p_instance_ctrl->p_cfg->tei_irq);
#else
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->tei_irq);
#endif
    }

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)

    /* Clear alternative trigger bits for this channel */
    {
        uint8_t dma_alt_bit = r_mspi_dma_alternative(p_instance_ctrl);
        uint8_t dts_alt_bit = r_mspi_dts_alternative(p_instance_ctrl);

        /* Clear the alternative trigger bits for this channel if they are valid. */
        if (MSPI_TRANSFER_INV_CHANNEL != dma_alt_bit)
        {
            p_instance_ctrl->p_regs_tg->MSPITGDMAALT &= ~((uint32_t) ((uint32_t) 1U << dma_alt_bit));
        }

        /* Clear the alternative trigger bits for this channel if they are valid. */
        if (MSPI_TRANSFER_INV_CHANNEL != dts_alt_bit)
        {
            p_instance_ctrl->p_regs_tg->MSPITGDTSALT &= ~((uint32_t) ((uint32_t) 1U << dts_alt_bit));
        }
    }
#endif

    /* Only tear down the unit-level registers when this is the LAST open channel. */
    if (0U == p_unit->channel_status)
    {
        /* Disable MSPI function. */
        p_instance_ctrl->p_regs->CTL1 &= MSPI_CTL1_CLEAR;
        p_instance_ctrl->p_regs->CTL2 &= MSPI_CTL2_CLEAR;
        p_instance_ctrl->p_regs->CTL0 &= (uint8_t) ~R_MSPI0_CTL0_EN_Msk;

        /* Clear trigger control registers */
        p_instance_ctrl->p_regs_tg->MSPITGCTL[unit] &= MSPI_TGCTL_CLEAR;
#if (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > MSPI_TGCTL1_FIRST_CHANNEL)
        if (MSPI_TGCTL1_UNIT_MAX > unit)
        {
            MSPI_TGCTL1(p_instance_ctrl->p_regs_tg, unit) &= MSPI_TGCTL_CLEAR; /* PRQA S 0303, 3469 # Advisory R-11.4, Advisory D-4.9 */
        }
#endif
    }

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
    p_instance_ctrl->frames_done  = 0U;
    p_instance_ctrl->chunk_frames = 0U;
    p_instance_ctrl->open         = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function receives data from a MSPI device. Implements @ref spi_api_t::read.
 *
 * @retval  FSP_SUCCESS                  Data reception successfully ends.
 * @retval  FSP_ERR_ASSERTION            Pointer to MSPI control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The unit has not been opened. Open unit first.
 * @retval  FSP_ERR_INVALID_SIZE         Remaining RAM size is not within the hardware supported range for the
 *                                       configured FIFO stage size.
 * @retval  FSP_ERR_IN_USE               The channel is currently in use.
 **********************************************************************************************************************/
fsp_err_t R_MSPI_Read (spi_ctrl_t * const p_ctrl, void * p_dest, uint32_t const length, spi_bit_width_t const bit_width)
{

    /* Call the common helper function to perform MSPI Read operation. */
    return r_mspi_start_read_write(p_ctrl, NULL, p_dest, length, bit_width);
}

/*******************************************************************************************************************//**
 * This function transmits data to a MSPI device. Implements @ref spi_api_t::write.
 *
 * @retval  FSP_SUCCESS                  Write operation successfully completed.
 * @retval  FSP_ERR_ASSERTION            NULL pointer to control or source parameters or transfer length is zero.
 * @retval  FSP_ERR_NOT_OPEN             The unit has not been opened. Open the unit first.
 * @retval  FSP_ERR_INVALID_SIZE         Remaining RAM size is not within the hardware supported range for the
 *                                       configured FIFO stage size.
 * @retval  FSP_ERR_IN_USE               The channel is currently in use.
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
 * @retval  FSP_SUCCESS                  Write operation successfully completed.
 * @retval  FSP_ERR_ASSERTION            NULL pointer to control or source parameters or transfer length is zero.
 * @retval  FSP_ERR_NOT_OPEN             The unit has not been opened. Open the unit first.
 * @retval  FSP_ERR_INVALID_SIZE         Remaining RAM size is not within the hardware supported range for the
 *                                       configured FIFO stage size.
 * @retval  FSP_ERR_IN_USE               The channel is currently in use.
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
                              void (                    * p_callback)(spi_callback_args_t * p_args),
                              void * const                p_context,
                              spi_callback_args_t * const p_callback_memory)
{
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

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
 * Determines which interrupt lines (TX / RX) a channel requires, based on its selectable mode and whether a DMA/DTS
 * transfer instance drives that direction (in which case no CPU ISR is needed for it).
 *
 * @param[in]   p_instance_ctrl   channel control block.
 * @param[out]  p_need_tx         set to true when the channel needs the TX interrupt.
 * @param[out]  p_need_rx         set to true when the channel needs the RX interrupt.
 **********************************************************************************************************************/
static void r_mspi_shared_line_need (mspi_instance_ctrl_t * p_instance_ctrl, bool * p_need_tx, bool * p_need_rx) /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_extended_cfg_t       * p_extend      = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;        /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    const transfer_instance_t * p_transfer_tx = p_instance_ctrl->p_cfg->p_transfer_tx;
    const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;

    *p_need_tx = false;
    *p_need_rx = false;

    switch (p_extend->mspi_select)
    {
        /* Select mode transmit/receive */
        case MSPI_SELECTABLE_MODE_TRANSMIT_RECEIVE:
        {
            /* When a transfer instance is provided for a direction, the CPU ISR is not needed for that direction. */
            *p_need_tx = (NULL == p_transfer_tx);
            *p_need_rx = (NULL == p_transfer_rx);

            break;
        }

        /* Select mode transmit-only */
        case MSPI_SELECTABLE_MODE_TRANSMIT_ONLY:
        {
            if (NULL == p_transfer_tx)
            {
                *p_need_tx = true;
            }

            break;
        }

        /* Default case handles receive-only mode. */
        default:
        {
            if (NULL == p_transfer_rx)
            {
                *p_need_rx = true;
            }

            break;
        }
    }
}

/*******************************************************************************************************************//**
 * Returns whether an already-open sibling channel on the unit is driving the given interrupt number.
 *
 * Keyed off the interrupt NUMBER so it is device-independent (handles ORed and plain shared lines).
 *
 * @param[in]  p_unit    unit context holding the per-channel control blocks.
 * @param[in]  p_self    the channel being configured/closed (excluded from the scan).
 * @param[in]  irq       the interrupt number to test against the open siblings.
 *
 * @retval  true    a sibling channel is already routed to this interrupt number.
 * @retval  false   no sibling uses this interrupt number.
 **********************************************************************************************************************/
static bool r_mspi_irq_already_active (mspi_unit_ctrl_t * p_unit, mspi_instance_ctrl_t * p_self, IRQn_Type irq) /* PRQA S 3673 # Advisory R-8.13 */
{
    bool    active = false;
    uint8_t sibling;

    /* A negative IRQ number is the FSP convention for "no interrupt assigned"; nothing to match. */
    if (0 > irq)                       /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        return false;
    }

    for (sibling = 0U; sibling < BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT; sibling++)
    {
        mspi_instance_ctrl_t * p_sibling = p_unit->p_ctrl[sibling]; /* PRQA S 3678 # Advisory R-8.13 */

        if ((NULL != p_sibling) && (p_sibling != p_self))
        {
            spi_cfg_t const * p_sib_cfg = p_sibling->p_cfg;

            /* A sibling shares this line only when it is routed to the SAME interrupt number. */
            if ((irq == p_sib_cfg->txi_irq) || (irq == p_sib_cfg->rxi_irq) ||
                (irq == p_sib_cfg->eri_irq) || (irq == p_sib_cfg->tei_irq))
            {
                active = true;

                break;
            }
        }
    }

    return active;
}

/*******************************************************************************************************************//**
 * Hardware configuration for settings given by the configuration structure.
 *
 * Programs the unit-level registers (CTL0/CTL1/CTL2) and every channel register that stays constant for the whole
 * lifetime of the channel. The per-transfer registers are programmed later in r_mspi_start_read_write().
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 *
 * @retval  FSP_SUCCESS                  Channel configured successfully.
 * @retval  FSP_ERR_IN_USE               Unit-level configuration conflict with a channel already running.
 * @retval  FSP_ERR_INVALID_SIZE         Configured FIFO stage size does not fit the usable RAM.
 **********************************************************************************************************************/
static fsp_err_t r_mspi_hw_config (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type            * p_mspi_regs      = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;                             /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */

    mspi_extended_cfg_t * p_extend      = ((mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);                  /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               channel       = p_instance_ctrl->p_cfg->channel;
    uint8_t               ctl0          = 0U;
    uint32_t              ctl1          = 0U;
    uint8_t               ctl2          = 0U;
    uint32_t              transfer_mode = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;

    /* Initialize value for CTL1 register */
    ctl1 = (uint32_t) (p_extend->mspi_ctl1.control_register_1);

    if (SPI_MODE_MASTER == p_instance_ctrl->p_cfg->operating_mode)
    {
        /* Set channel to master mode */
        ctl1 &= ~R_MSPI0_CTL1_MSSEL_Msk;

        /* Clear MSPInCSIE */
        ctl1 &= ~(((uint32_t) 1U << R_MSPI0_CTL1_CSIE_Pos));
    }
    else
    {
        /* Set channel to slave mode */
        ctl1 |= R_MSPI0_CTL1_MSSEL_Msk;

        /* Feature is not supported in slave mode */
        ctl1 &= ~(((uint32_t) 1U << R_MSPI0_CTL1_SAMP_Pos));
        ctl1 &= ~((uint32_t) (3U << R_MSPI0_CTL1_SOLS_Pos)); /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
        ctl1 &= ~((uint32_t) R_MSPI0_CTL1_CSP_Msk);
    }

    /* Checking loop-back mode */
    if ((MSPI_LOOPBACK_MODE_ENABLE == p_extend->mspi_loopback) &&
        (SPI_MODE_MASTER == p_instance_ctrl->p_cfg->operating_mode))
    {
        ctl2 |= R_MSPI0_CTL2_LBM_Msk;  /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
    }
    else
    {
        ctl2 &= (uint8_t) ~R_MSPI0_CTL2_LBM_Msk;
    }

    /* Checking data consistency */
    if (MSPI_CONSISTENCY_CHECK_ENABLE == p_extend->mspi_consistency_check)
    {
        ctl2 |= R_MSPI0_CTL2_DCS_Msk;  /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
    }
    else
    {
        ctl2 &= (uint8_t) ~R_MSPI0_CTL2_DCS_Msk;
    }

    /* Intended CTL0 value once the unit is running. */
    ctl0 = R_MSPI0_CTL0_EN_Msk;        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */

    if (0U == p_instance_ctrl->p_regs->CTL0_b.EN)
    {
        /* Initialize unit-level registers before the first channel enables the unit. */
        p_instance_ctrl->p_regs->CTL0 = 0U;

        /* Four or more PCLK cycles plus four or more MSPInCLK cycles are required after
         * MSPInEN is cleared before it may be set again. Computed as the ceiling (in us)
         * of 4 PCLK cycles plus 4 MSPInCLK cycles. */
        uint32_t delay_us =
            (uint32_t) (((MSPI_MINIMUM_CYCLE_INIT * MSPI_US_PER_SEC) + ((uint32_t) BSP_CFG_CLOCK_CLK_HSB_HZ - 1U)) /  /* PRQA S 1840 # DP_MSR_R-10.4_UC1 */
                        BSP_CFG_CLOCK_CLK_HSB_HZ) +
            (uint32_t) (((MSPI_MINIMUM_CYCLE_INIT * MSPI_US_PER_SEC) + ((uint32_t) BSP_CFG_CLOCK_CLK_MSPI_HZ - 1U)) / /* PRQA S 1840 # DP_MSR_R-10.4_UC1 */
                        BSP_CFG_CLOCK_CLK_MSPI_HZ);

        if (0U == delay_us)
        {
            delay_us = MSPI_MIN_DELAY_US;
        }

        R_BSP_SoftwareDelay(delay_us, BSP_DELAY_UNITS_MICROSECONDS);

        p_instance_ctrl->p_regs->CTL1 = (uint32_t) ctl1;
        p_instance_ctrl->p_regs->CTL2 = (uint8_t) ctl2;
        p_instance_ctrl->p_regs->CTL0 = (uint8_t) ctl0;
    }
    else
    {
        /* Verify the requested unit-level configuration matches the active unit before sharing it. */
        const uint32_t current_ctl1 = p_instance_ctrl->p_regs->CTL1;
        const uint8_t  current_ctl2 = (uint8_t) p_instance_ctrl->p_regs->CTL2;

        if (((uint32_t) ctl1 != current_ctl1) || ((uint8_t) ctl2 != current_ctl2))
        {

            /* Unit-level configuration conflict with a channel already running on this unit. */
            return FSP_ERR_IN_USE;
        }
    }

    /* Configure channel-specific settings without affecting other active channels. */
    if (SPI_MODE_MASTER == p_instance_ctrl->p_cfg->operating_mode)
    {
        /* Set the chip select bit for this channel */
        uint32_t ssel_cs_bit = (uint32_t) ((uint32_t) 1U << channel);

        if (MSPI_CHIP_SELECT_STATE_ACTIVE == p_extend->mspi_chip_selection)
        {
            p_mspi_regs[channel].CFG3  = (p_extend->mspi_baudrate_channel); /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
            p_mspi_regs[channel].SSEL |= ssel_cs_bit;                       /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
        }
        else
        {
            p_mspi_regs[channel].SSEL &= ~ssel_cs_bit;                      /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
        }

        if (MSPI_JOB_ENABLE == p_extend->mspi_job_enable)
        {
            p_mspi_regs[channel].SSEL |= ((uint32_t) 1U << R_MSPI0_MSPI0_CH_SSEL_JOBEN_Pos);    /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
        }
        else
        {
            p_mspi_regs[channel].SSEL &= ~(((uint32_t) 1U << R_MSPI0_MSPI0_CH_SSEL_JOBEN_Pos)); /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
        }
    }
    else
    {
        /* Only one chip select (channel 0) is allowed in slave mode */
        p_mspi_regs[MSPI_SLAVE_CHANNEL].SSEL &= MSPI_SSEL_SLAVE;
        p_mspi_regs[MSPI_SLAVE_CHANNEL].CFG3  = MSPI_CLOCK_MODE_CLK_2; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    }

    /* Static channel configuration: registers that remain constant for every transfer on this channel. */

    /* Initialize the constant channel configuration registers */
    p_mspi_regs[channel].CFG0 = (uint32_t) (p_extend->p_mspi_config_cfg0->chip_select_config_0_u32);
    p_mspi_regs[channel].CFG1 = (uint32_t) (p_extend->p_mspi_config_cfg1->chip_select_config_1_u32);
    p_mspi_regs[channel].CFG4 = (uint32_t) (p_extend->p_mspi_config_cfg4->chip_select_config_4_u32);
    p_mspi_regs[channel].SEUP = (uint16_t) (p_extend->mspi_setup_time);
    p_mspi_regs[channel].HOLD = (uint16_t) (p_extend->mspi_hold_time);
    p_mspi_regs[channel].IDLE = (uint16_t) (p_extend->mspi_idle_time);
    p_mspi_regs[channel].INDA = (uint16_t) (p_extend->mspi_inter_data);

    /* Initialize Interrupt masked output */
    p_mspi_intf_regs[MSPI_INTF_IDX_TX].INTMSK =
        (p_mspi_intf_regs[MSPI_INTF_IDX_TX].INTMSK & ~((uint32_t) (1U << channel))) |  /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
        (uint32_t) ((((uint32_t) p_extend->mspi_tx_isr_mask & MSPI_ISR_MASK_BIT) << channel));
    p_mspi_intf_regs[MSPI_INTF_IDX_RX].INTMSK =
        (p_mspi_intf_regs[MSPI_INTF_IDX_RX].INTMSK & ~((uint32_t) (1U << channel))) |  /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
        (uint32_t) ((((uint32_t) p_extend->mspi_rx_isr_mask & MSPI_ISR_MASK_BIT) << channel));
    p_mspi_intf_regs[MSPI_INTF_IDX_FE].INTMSK =
        (p_mspi_intf_regs[MSPI_INTF_IDX_FE].INTMSK & ~((uint32_t) (1U << channel))) |  /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
        (uint32_t) ((((uint32_t) p_extend->mspi_fe_isr_mask & MSPI_ISR_MASK_BIT) << channel));
    p_mspi_intf_regs[MSPI_INTF_IDX_ERR].INTMSK =
        (p_mspi_intf_regs[MSPI_INTF_IDX_ERR].INTMSK & ~((uint32_t) (1U << channel))) | /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
        (uint32_t) ((((uint32_t) p_extend->mspi_err_isr_mask & MSPI_ISR_MASK_BIT) << channel));

    /* Transfer-mode dependent, but configuration-static, channel setup. */
    p_instance_ctrl->stage_size = 0U;

    if (MSPI_TRANSFER_MODE_DIRECT_ACCESS == transfer_mode) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        /* Set Direct Access Mode and clear RAM address */
        p_mspi_regs[channel].CFG0  &= ~R_MSPI0_MSPI0_CH_CFG0_MD_Msk;
        p_mspi_regs[channel].RASTAD = MSPI_RASTAD_CLEAR;

        /* Disable Frame End interrupt if configured to keep state */
        if (MSPI_CHANNEL_FRAME_END_KEEP == p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_frame_end) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            p_mspi_regs[channel].CFG0_b.IFEE = 0U;
        }
    }
    else if (MSPI_TRANSFER_MODE_BUFFER_MEMORY == transfer_mode) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        /* Set Buffer Memory Mode, configure RAM start, and disable Job Mode */
        p_mspi_regs[channel].CFG0 = (p_mspi_regs[channel].CFG0 & ~R_MSPI0_MSPI0_CH_CFG0_MD_Msk) |
                                    (uint32_t) ((uint32_t) MSPI_CFG0_MD_BUFFER << R_MSPI0_MSPI0_CH_CFG0_MD_Pos);

        /* Configure RAM start address */
        p_mspi_regs[channel].RASTAD = (uint16_t) (p_extend->mspi_ram_start);
        p_mspi_regs[channel].SSEL  &= ~((uint32_t) (1U << R_MSPI0_MSPI0_CH_SSEL_JOBEN_Pos)); /* PRQA S 0499, 4461 # DP_MSR_R-12.2_UC1, DP_MSR_R-10.3_UC1 */
    }
    else                                                                                     /* FIFO Memory Mode */
    {
        uint32_t req_stage = p_extend->p_mspi_config_cfg4->mspi_chip_select_cfg4.mspi_fifo_stage_size;

        /* Byte units (macro is bytes). Stage sizes 8/16/32 are compared in bytes. */
        uint32_t usable_bytes = (uint32_t) BSP_FEATURE_MSPI_RAM_WORDS_MAX - p_extend->mspi_ram_start;

        if ((1U == p_mspi_regs[channel].CFG0_b.TXE) && (1U == p_mspi_regs[channel].CFG0_b.RXE))
        {
            usable_bytes = usable_bytes / MSPI_HALF_DIVISOR;
        }

        if (((MSPI_FIFO_MIN_BYTES_STAGE_32 > usable_bytes) && (MSPI_FIFO_STAGE_SIZE_32 == req_stage)) || /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            ((MSPI_FIFO_MIN_BYTES_STAGE_16 > usable_bytes) && (MSPI_FIFO_STAGE_SIZE_16 == req_stage)))   /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            return FSP_ERR_INVALID_SIZE;
        }

        /* Set FIFO Memory Mode, configure RAM start, and disable Job Mode */
        p_mspi_regs[channel].CFG0 = (p_mspi_regs[channel].CFG0 & ~R_MSPI0_MSPI0_CH_CFG0_MD_Msk) |
                                    (uint32_t) ((uint32_t) MSPI_CFG0_MD_FIFO << R_MSPI0_MSPI0_CH_CFG0_MD_Pos);
        p_mspi_regs[channel].RASTAD = (uint16_t) (p_extend->mspi_ram_start);
        p_mspi_regs[channel].SSEL  &= ~((uint32_t) (1U << R_MSPI0_MSPI0_CH_SSEL_JOBEN_Pos)); /* PRQA S 0499, 4461 # DP_MSR_R-12.2_UC1, DP_MSR_R-10.3_UC1 */

        /* Program stage size so it is valid for the chunk/refill computation. */
        if (MSPI_FIFO_STAGE_SIZE_8 == req_stage)                                             /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            p_mspi_regs[channel].CFG4  &= ~R_MSPI0_MSPI0_CH_CFG4_SIZE_Msk;
            p_instance_ctrl->stage_size = MSPI_STAGE_SIZE_8;
        }
        else if (MSPI_FIFO_STAGE_SIZE_16 == req_stage) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            p_mspi_regs[channel].CFG4 = (p_mspi_regs[channel].CFG4 & ~R_MSPI0_MSPI0_CH_CFG4_SIZE_Msk) |
                                        (uint32_t) ((uint32_t) MSPI_CFG4_SIZE_16 << R_MSPI0_MSPI0_CH_CFG4_SIZE_Pos);
            p_instance_ctrl->stage_size = MSPI_STAGE_SIZE_16;
        }
        else
        {
            p_mspi_regs[channel].CFG4 = (p_mspi_regs[channel].CFG4 & ~R_MSPI0_MSPI0_CH_CFG4_SIZE_Msk) |
                                        (uint32_t) ((uint32_t) MSPI_CFG4_SIZE_32 << R_MSPI0_MSPI0_CH_CFG4_SIZE_Pos);
            p_instance_ctrl->stage_size = MSPI_STAGE_SIZE_32;
        }
    }

    /* Set transfer direction (TX/RX enable) - constant per configuration. */
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

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enable Communication, Receive, Error and Frame-end interrupts for this channel.
 *
 * A line is dedicated or shared purely by its interrupt NUMBER (see r_mspi_irq_already_active), so this is
 * device-independent. The per-unit context is passed as the interrupt context so the ISR can dispatch to the
 * correct channel.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_interrupt_config (mspi_instance_ctrl_t * p_instance_ctrl)
{
    mspi_extended_cfg_t * p_extend = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    spi_cfg_t const     * p_cfg    = p_instance_ctrl->p_cfg;
    uint8_t               unit     = p_extend->unit;
    uint8_t               channel  = p_instance_ctrl->p_cfg->channel;
    mspi_unit_ctrl_t    * p_unit   = &g_mspi_unit_ctrl[unit];

    /* Channels already open on this unit before the current one (used to init the unit context once). */
    uint32_t others_open = p_unit->channel_status & ~((uint32_t) (MSPI_CHANNEL_STATUS_BIT << channel)); /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */

    bool need_tx = false;
    bool need_rx = false;
    r_mspi_shared_line_need(p_instance_ctrl, &need_tx, &need_rx);

    /* Initialize the per-unit context on the first channel of the unit. */
    if (0U == others_open)
    {
        p_unit->unit        = unit;
        p_unit->p_regs_intf = (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    }

    /* Register this channel so the ISR can dispatch to the correct control block. */
    p_unit->p_ctrl[channel] = p_instance_ctrl;

    /* TX line: enable unless a sibling already drives this exact interrupt number (shared line). */
    if (need_tx && (!r_mspi_irq_already_active(p_unit, p_instance_ctrl, p_cfg->txi_irq)))
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        MSPI_ENABLE_IRQ(p_extend->com_ored_int, p_cfg->txi_irq, p_cfg->txi_ipl, p_unit);
#else
        R_BSP_IrqCfgEnable(p_cfg->txi_irq, p_cfg->txi_ipl, p_unit);
#endif
    }

    /* RX line: enable unless a sibling already drives this exact interrupt number (shared line). */
    if (need_rx && (!r_mspi_irq_already_active(p_unit, p_instance_ctrl, p_cfg->rxi_irq)))
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        MSPI_ENABLE_IRQ(p_extend->rec_ored_int, p_cfg->rxi_irq, p_cfg->rxi_ipl, p_unit);
#else
        R_BSP_IrqCfgEnable(p_cfg->rxi_irq, p_cfg->rxi_ipl, p_unit);
#endif
    }

    /* Error line (always required): enable unless a sibling already drives this exact interrupt number. */
    if (!r_mspi_irq_already_active(p_unit, p_instance_ctrl, p_cfg->eri_irq))
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        MSPI_ENABLE_IRQ(p_extend->error_ored_int, p_cfg->eri_irq, p_cfg->eri_ipl, p_unit);
#else
        R_BSP_IrqCfgEnable(p_cfg->eri_irq, p_cfg->eri_ipl, p_unit);
#endif
    }

    /* Frame-end line (always required): enable unless a sibling already drives this exact interrupt number. */
    if (!r_mspi_irq_already_active(p_unit, p_instance_ctrl, p_cfg->tei_irq))
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        MSPI_ENABLE_IRQ(p_extend->frame_ored_int, p_cfg->tei_irq, p_cfg->tei_ipl, p_unit);
#else
        R_BSP_IrqCfgEnable(p_cfg->tei_irq, p_cfg->tei_ipl, p_unit);
#endif
    }
}

/*******************************************************************************************************************//**
 * Computes the byte window [*p_start, *p_end) that a channel's current transfer occupies in the unit RAM, derived
 * from chunk_frames (= min(length, capacity)). Direct Access mode uses no RAM and returns an empty window.
 *
 * @param[in]   p_instance_ctrl   channel control block (bit_width / chunk_frames already set for the transfer).
 * @param[out]  p_start           RAM window start (byte offset from the unit RAM base).
 * @param[out]  p_end             RAM window end (exclusive).
 **********************************************************************************************************************/
static void r_mspi_ram_window (mspi_instance_ctrl_t * p_instance_ctrl, uint32_t * p_start, uint32_t * p_end) /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_extended_cfg_t   * p_extend = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;             /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    R_MSPI0_MSPI0_CH_Type * p_regs   = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;          /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
    uint8_t                 channel  = p_instance_ctrl->p_cfg->channel;
    uint32_t                mode     = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;
    uint32_t                start    = p_extend->mspi_ram_start;
    uint32_t                bytes    = (uint32_t) r_mspi_count_byte(p_instance_ctrl->bitwidth);              /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    uint32_t                chunk    = p_instance_ctrl->chunk_frames;
    uint32_t                span     = 0U;

    if (MSPI_TRANSFER_MODE_FIFO_MEMORY == mode)                                                              /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        /* FIFO stage RAM window (TX and RX halves both live inside one stage). */
        span = (uint32_t) p_instance_ctrl->stage_size;
    }
    else if (MSPI_TRANSFER_MODE_BUFFER_MEMORY == mode) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        if ((1U == p_regs[channel].CFG0_b.TXE) && (1U == p_regs[channel].CFG0_b.RXE))
        {
            /* Full-duplex layout: [TX chunk][4-byte align gap][RX chunk] (see r_mspi_buffer_read). */
            uintptr_t ram_addr = (uintptr_t) p_instance_ctrl->p_regs + MSPI_INIT_RAM_ADDR + start;
            uint32_t  align    = (uint32_t) r_mspi_align_ram(chunk, (uint32_t) ram_addr, p_instance_ctrl->bitwidth); /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
            span = ((align + (MSPI_HALF_DIVISOR * chunk)) * bytes);
        }
        else
        {
            /* Single direction: one chunk of frames. */
            span = (chunk * bytes);
        }
    }
    else
    {
        /* Direct Access mode: no RAM window. */
        span = 0U;
    }

    *p_start = start;
    *p_end   = start + span;
}

/*******************************************************************************************************************//**
 * Returns the number of contiguous free RAM bytes available to this channel, starting at its RASTAD, bounded by the
 * unit RAM end and by the nearest sibling channel that is CURRENTLY transferring. An idle sibling reserves nothing.
 *
 * @param[in]  p_instance_ctrl   channel about to start / continue a transfer.
 * @return  Free contiguous bytes from ram_start (0 when an active sibling already occupies the start).
 **********************************************************************************************************************/
static uint32_t r_mspi_ram_free_bytes (mspi_instance_ctrl_t * p_instance_ctrl)                   /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_extended_cfg_t * p_extend   = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    mspi_unit_ctrl_t    * p_unit     = &g_mspi_unit_ctrl[p_extend->unit];                        /* PRQA S 3678 # Advisory R-8.13 */
    uint8_t               channel    = p_instance_ctrl->p_cfg->channel;
    uint32_t              total_ram  = (uint32_t) BSP_FEATURE_MSPI_RAM_WORDS_MAX;
    uint32_t              self_start = p_extend->mspi_ram_start;
    uint32_t              limit      = total_ram;
    uint8_t               sibling;

    if (self_start >= total_ram)
    {
        return 0U;
    }

    for (sibling = 0U; sibling < BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT; sibling++)
    {
        if ((sibling != channel) &&
            (0U != (p_unit->ram_active_mask & (uint32_t) (MSPI_CHANNEL_STATUS_BIT << sibling)))) /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
        {
            mspi_instance_ctrl_t * p_sibling = p_unit->p_ctrl[sibling];

            if (NULL != p_sibling)
            {
                uint32_t sib_start;
                uint32_t sib_end;
                r_mspi_ram_window(p_sibling, &sib_start, &sib_end);

                /* Active sibling occupies our start -> no room at all. */
                if ((sib_start <= self_start) && (self_start < sib_end))
                {
                    return 0U;
                }

                /* Sibling ahead of us caps how far our window may grow. */
                if ((sib_start > self_start) && (sib_start < limit))
                {
                    limit = sib_start;
                }
            }
        }
    }

    return limit - self_start;
}

/*******************************************************************************************************************//**
 * Runtime RAM arbitration for the transfer that is about to start. An idle sibling reserves nothing, so its region
 * is free to reuse.
 *
 * @param[in]  p_instance_ctrl   channel about to start (bit_width / chunk_frames already set).
 *
 * @retval  FSP_SUCCESS            RAM window is valid and free (caller then claims it).
 * @retval  FSP_ERR_INVALID_SIZE   Window does not fit in the remaining unit RAM.
 * @retval  FSP_ERR_IN_USE         Window overlaps a sibling channel that is currently transferring.
 **********************************************************************************************************************/
static fsp_err_t r_mspi_ram_region_acquire (mspi_instance_ctrl_t * p_instance_ctrl)
{
    mspi_extended_cfg_t * p_extend = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint32_t              mode     = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;

    /* Direct Access mode does not use the RAM working area. */
    if (MSPI_TRANSFER_MODE_DIRECT_ACCESS == mode)                                              /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        return FSP_SUCCESS;
    }

    mspi_unit_ctrl_t * p_unit    = &g_mspi_unit_ctrl[p_extend->unit]; /* PRQA S 3678 # Advisory R-8.13 */
    uint8_t            channel   = p_instance_ctrl->p_cfg->channel;
    uint32_t           total_ram = (uint32_t) BSP_FEATURE_MSPI_RAM_WORDS_MAX;
    uint32_t           self_start;
    uint32_t           self_end;
    uint8_t            sibling;

    r_mspi_ram_window(p_instance_ctrl, &self_start, &self_end);

    /* An empty window needs no RAM and cannot conflict. */
    if (self_start == self_end)
    {
        return FSP_SUCCESS;
    }

    /* The window must fit inside the unit RAM (enough room from the start address). */
    if ((self_start >= total_ram) || (self_end > total_ram))
    {
        return FSP_ERR_INVALID_SIZE;
    }

    /* The window must not overlap a sibling that is currently transferring. */
    for (sibling = 0U; sibling < BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT; sibling++)
    {
        if ((sibling != channel) &&
            (0U != (p_unit->ram_active_mask & (uint32_t) (MSPI_CHANNEL_STATUS_BIT << sibling)))) /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
        {
            mspi_instance_ctrl_t * p_sibling = p_unit->p_ctrl[sibling];

            if (NULL != p_sibling)
            {
                uint32_t sib_start;
                uint32_t sib_end;
                r_mspi_ram_window(p_sibling, &sib_start, &sib_end);

                /* Half-open ranges [start,end) intersect (also catches an identical start). */
                if ((self_start < sib_end) && (sib_start < self_end))
                {
                    return FSP_ERR_IN_USE;
                }
            }
        }
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Releases the RAM window owned by a channel so a sibling may reuse the region.
 *
 * @param[in]  p_instance_ctrl   channel whose RAM window is being released.
 **********************************************************************************************************************/
static void r_mspi_ram_region_release (mspi_instance_ctrl_t * p_instance_ctrl)                              /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_extended_cfg_t * p_extend = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;              /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               channel  = p_instance_ctrl->p_cfg->channel;

    g_mspi_unit_ctrl[p_extend->unit].ram_active_mask &= ~((uint32_t) (MSPI_CHANNEL_STATUS_BIT << channel)); /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
}

/*******************************************************************************************************************//**
 * This function sets up the DMAC transfer by assigning source/destination addresses, opening the transfer instance,
 * and setting the hardware request trigger.
 * @retval  FSP_SUCCESS                   Successful transfer initiation.
 **********************************************************************************************************************/
static fsp_err_t r_mspi_transfer_config (mspi_instance_ctrl_t * p_instance_ctrl, spi_cfg_t const * const p_cfg)
{
    FSP_PARAMETER_NOT_USED(p_cfg);

    fsp_err_t err = FSP_SUCCESS;
#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
    mspi_extended_cfg_t const * p_extend      = (mspi_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    const transfer_instance_t * p_transfer_tx = p_instance_ctrl->p_cfg->p_transfer_tx;
    const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;
    uint8_t channel              = p_instance_ctrl->p_cfg->channel;
    uint8_t unit                 = p_extend->unit;
    uint8_t triggerSource        = p_extend->mspi_transfer_trigger; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    uint8_t activationSource     = p_extend->mspi_trigger_source;   /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    uint8_t dmaAlternativeResult = r_mspi_dma_alternative(p_instance_ctrl);
    uint8_t dtsAlternativeResult = r_mspi_dts_alternative(p_instance_ctrl);

    /* Configure the register fields separately for each unit and channel within separate devices */
 #if (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > MSPI_TGCTL1_FIRST_CHANNEL)

    /* For channel from 0 to 7 using register MSPITGDMAALT */
    if (MSPI_CS_CHANNEL_7 >= channel)
    {
        p_instance_ctrl->p_regs_tg->MSPITGCTL[unit] |=
            (((uint32_t) triggerSource << ((uint32_t) channel * MSPI_CGTL_CHANNEL_GAP)) |
             ((uint32_t) activationSource <<
              (((uint32_t) channel * MSPI_CGTL_CHANNEL_GAP) + MSPI_TG_ACTIVATION_BIT_OFFSET)));
    }
    /* For channels 8 to 11 using register MSPITGCTL1 */
    else if (MSPI_TGCTL1_UNIT_MAX > unit)
    {
        /* For channel 8 to 11 using register MSPITGCTL1 */
        uint32_t tgctl1_shift = ((uint32_t) channel - MSPI_TGCTL1_FIRST_CHANNEL) * MSPI_CGTL_CHANNEL_GAP;

        /* Set trigger source and type of trigger source */
        MSPI_TGCTL1(p_instance_ctrl->p_regs_tg, unit) |= /* PRQA S 0303, 3469 # Advisory R-11.4, Advisory D-4.9 */
                                                         (((uint32_t) triggerSource << tgctl1_shift) |
                                                          ((uint32_t) activationSource <<
                                                           (tgctl1_shift + MSPI_TG_ACTIVATION_BIT_OFFSET)));
    }
    else
    {
        /* This unit has no MSPITGCTL1 register, so no trigger source can be routed for this channel. */
    }

 #else

    /* Set trigger source and type of trigger source */
    p_instance_ctrl->p_regs_tg->MSPITGCTL[unit] |=
        (((uint32_t) triggerSource << ((uint32_t) channel * MSPI_CGTL_CHANNEL_GAP)) |
         ((uint32_t) activationSource <<
          (((uint32_t) channel * MSPI_CGTL_CHANNEL_GAP) + MSPI_TG_ACTIVATION_BIT_OFFSET)));
 #endif

    /*Set transfer trigger alternative */
    if (MSPI_TRANSFER_INV_CHANNEL != dmaAlternativeResult)
    {
        uint32_t dma_alt_mask = (uint32_t) ((uint32_t) MSPI_CHANNEL_STATUS_BIT << dmaAlternativeResult);

        p_instance_ctrl->p_regs_tg->MSPITGDMAALT =
            (p_instance_ctrl->p_regs_tg->MSPITGDMAALT & ~dma_alt_mask) |
            (uint32_t) (((uint32_t) p_extend->mspi_dma_alternative & MSPI_CHANNEL_STATUS_BIT) <<
                        dmaAlternativeResult);
    }

    if (MSPI_TRANSFER_INV_CHANNEL != dtsAlternativeResult)
    {
        uint32_t dts_alt_mask = (uint32_t) ((uint32_t) MSPI_CHANNEL_STATUS_BIT << dtsAlternativeResult);

        p_instance_ctrl->p_regs_tg->MSPITGDTSALT =
            (p_instance_ctrl->p_regs_tg->MSPITGDTSALT & ~dts_alt_mask) |
            (uint32_t) (((uint32_t) p_extend->mspi_dts_alternative & MSPI_CHANNEL_STATUS_BIT) <<
                        dtsAlternativeResult);
    }

    if (NULL != p_transfer_tx)
    {
        /* Set up data for transmission transfer */
        p_transfer_tx->p_cfg->p_info->p_dest = (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].TXDA0); /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */

        err = p_transfer_tx->p_api->open(p_transfer_tx->p_ctrl, p_transfer_tx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        if (MSPI_TRIGGER_SOURCE_FE1 == p_extend->mspi_trigger_source)
        {
            err = p_transfer_tx->p_api->callbackSet(p_transfer_tx->p_ctrl,
                                                    mspi_fe_transfer_callback, /* PRQA S 3635 # Advisory R-17.12 */
                                                    p_instance_ctrl,
                                                    NULL);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        }
        else
        {
            err =
                p_transfer_tx->p_api->callbackSet(p_transfer_tx->p_ctrl,
                                                  mspi_tx_transfer_callback, /* PRQA S 3635 # Advisory R-17.12 */
                                                  p_instance_ctrl,
                                                  NULL);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        }
    }

    if (NULL != p_transfer_rx)
    {
        /* Set up data for reception transfer */
        p_transfer_rx->p_cfg->p_info->p_src = (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].RXDA0); /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */

        err = p_transfer_rx->p_api->open(p_transfer_rx->p_ctrl, p_transfer_rx->p_cfg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        if (MSPI_TRIGGER_SOURCE_FE2 == p_extend->mspi_trigger_source)
        {
            err = p_transfer_rx->p_api->callbackSet(p_transfer_rx->p_ctrl,
                                                    mspi_fe_transfer_callback, /* PRQA S 3635 # Advisory R-17.12 */
                                                    p_instance_ctrl,
                                                    NULL);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        }
        else
        {
            err =
                p_transfer_rx->p_api->callbackSet(p_transfer_rx->p_ctrl,
                                                  mspi_rx_transfer_callback, /* PRQA S 3635 # Advisory R-17.12 */
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
 * Configures the driver state and initiates a MSPI transfer for all modes of operation. Only the per-transfer
 * registers are programmed here; the constant channel configuration is programmed once in r_mspi_hw_config().
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
 **********************************************************************************************************************/
static fsp_err_t r_mspi_start_read_write (spi_ctrl_t * const    p_ctrl,
                                          void const          * p_src,
                                          void                * p_dest,
                                          uint32_t const        length,
                                          spi_bit_width_t const bit_width)
{
    mspi_instance_ctrl_t * p_instance_ctrl = (mspi_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if MSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MSPI_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT((0U < length) && (MSPI_LENGTH_MAX > length));
    FSP_ASSERT((SPI_BIT_WIDTH_2_BITS <= bit_width) && (SPI_BIT_WIDTH_128_BITS >= bit_width));
    FSP_ASSERT((NULL != p_src) || (NULL != p_dest));
#endif
    mspi_extended_cfg_t   * p_extend    = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;    /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */

    uint8_t  channel       = p_instance_ctrl->p_cfg->channel;
    uint32_t transfer_mode = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;
    uint32_t refill_data   = 0U;

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
    const transfer_instance_t * p_transfer_tx     = p_instance_ctrl->p_cfg->p_transfer_tx;
    const transfer_instance_t * p_transfer_rx     = p_instance_ctrl->p_cfg->p_transfer_rx;
    transfer_size_t             transfer_size     = TRANSFER_SIZE_1_BYTE;
    transfer_size_t             transfer_src_size = TRANSFER_SIZE_1_BYTE;
    transfer_size_t             transfer_des_size = TRANSFER_SIZE_1_BYTE;

    uint32_t        num_count     = MSPI_WORD_COUNT(bit_width); /* PRQA S 3469 # Advisory D-4.9 */
    static uint32_t dummy_tx_data = 0U;
    static uint32_t dummy_rx_data;
#endif

    /* Read CFG0 register value to check configurations */
    uint32_t cfg0_val = p_mspi_regs[channel].CFG0;

    if ((1U == p_instance_ctrl->transfer_notified) &&
        ((cfg0_val & ((uint32_t) 1U << R_MSPI0_MSPI0_CH_CFG0_FCCE_Pos)) != 0U))
    {
        /* Stop communicate in MSPI channel */
        p_mspi_regs[channel].CSTC_b.CHENC = R_MSPI0_MSPI0_CH_CSTC_CHENC_Msk; /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */

        /* Wait until the hardware confirms the channel is fully disabled. */
        FSP_HARDWARE_REGISTER_WAIT(p_mspi_regs[channel].CSTR_b.CHEN, 0U);
    }

    /* Check the channel operating status. */
    if (0U != p_mspi_regs[channel].CSTR_b.ACTF)
    {
        return FSP_ERR_IN_USE;
    }

    /* Initialize control parameters */
    p_instance_ctrl->p_tx_data         = p_src;
    p_instance_ctrl->p_rx_data         = p_dest;
    p_instance_ctrl->tx_count          = 0U;
    p_instance_ctrl->rx_count          = 0U;
    p_instance_ctrl->length            = length;
    p_instance_ctrl->bitwidth          = bit_width;
    p_instance_ctrl->transfer_notified = 0U;
    p_instance_ctrl->frames_done       = 0U;
    p_instance_ctrl->chunk_frames      = 0U;
    p_instance_ctrl->chunk_read_done   = 0U;
    p_instance_ctrl->chunk_tx_pending  = 0U;
    p_instance_ctrl->chunk_rx_pending  = 0U;

    /* Pick the single completion source. */
#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
    bool tx_only = (MSPI_SELECTABLE_MODE_TRANSMIT_ONLY == p_extend->mspi_select);

    if ((uint32_t) MSPI_TRANSFER_MODE_BUFFER_MEMORY == transfer_mode)
    {
        /* Buffer mode completes via FE: FE-transfer-cb when RX DMA is used, else FE ISR. */
        p_instance_ctrl->completion_source = (NULL != p_instance_ctrl->p_cfg->p_transfer_rx) ?
                                             MSPI_COMPLETE_SRC_FE_XFER :
                                             MSPI_COMPLETE_SRC_FE_ISR;
    }
    else if ((NULL != p_instance_ctrl->p_cfg->p_transfer_tx) ||
             (NULL != p_instance_ctrl->p_cfg->p_transfer_rx))
    {
        if ((MSPI_TRIGGER_SOURCE_FE1 == p_extend->mspi_trigger_source) ||
            (MSPI_TRIGGER_SOURCE_FE2 == p_extend->mspi_trigger_source))
        {
            p_instance_ctrl->completion_source = MSPI_COMPLETE_SRC_FE_XFER;
        }
        else if (((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == transfer_mode) && tx_only)
        {
            /* Use the frame-end event to complete the transmit-only transfer. */
            p_instance_ctrl->completion_source = MSPI_COMPLETE_SRC_FE_ISR;
        }
        else
        {
            p_instance_ctrl->completion_source = MSPI_COMPLETE_SRC_TXRX_XFER;
        }
    }
    else
#endif
    {
        /* Default to frame-end ISR for other transfer modes */
        p_instance_ctrl->completion_source = MSPI_COMPLETE_SRC_FE_ISR;
    }

    /* Check if the Safe Enable is set to 'Enable' */
    if ((uint32_t) MSPI_SAFE_ENABLE == p_extend->p_mspi_config_cfg1->mspi_chip_select_cfg1.mspi_safe_enable)
    {
        FSP_ASSERT(SPI_BIT_WIDTH_32_BITS == bit_width);
    }

    /* Frame Count End set to 'Not Clear': stop the channel before reconfiguring. */
    if (((p_mspi_regs[channel].CFG0 & (uint32_t) (1U << R_MSPI0_MSPI0_CH_CFG0_FCCE_Pos)) != 0U)) /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
    {
        /* Clear the Channel Enable flag and stop the channel */
        p_mspi_regs[channel].CSTC_b.CHENC = 1U;

        /* Wait until the hardware confirms the channel is fully disabled. */
        FSP_HARDWARE_REGISTER_WAIT(p_mspi_regs[channel].CSTR_b.CHEN, 0U);
    }

    /* Program the per-transfer bit width. */
    p_mspi_regs[channel].CFG2 = p_instance_ctrl->bitwidth; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */

    if (MSPI_TRANSFER_MODE_DIRECT_ACCESS == transfer_mode) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        /* First chunk uses the shared limit. */
        uint32_t max_chunk = r_mspi_max_chunk_frames(p_instance_ctrl);
        p_instance_ctrl->chunk_frames = (p_instance_ctrl->length > max_chunk) ?
                                        max_chunk :
                                        p_instance_ctrl->length;
        p_mspi_regs[channel].CFSET = p_instance_ctrl->chunk_frames; /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
    }

    /* Program CFSET after TXE/RXE (set in r_mspi_hw_config). chunk_frames = min(length, capacity). */
    if (MSPI_TRANSFER_MODE_DIRECT_ACCESS != transfer_mode)          /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        uint32_t max_chunk = r_mspi_max_chunk_frames(p_instance_ctrl);
        p_instance_ctrl->chunk_frames = (p_instance_ctrl->length > max_chunk) ?
                                        max_chunk : p_instance_ctrl->length;

        refill_data = r_mspi_fifo_refill_stage(p_instance_ctrl); /* 0 for buffer */

        /* Length-based CFSET only for the DTS FIFO one-shot; other paths use chunk-based CFSET. */
        bool dts_fifo_cfset = false;
#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
        dts_fifo_cfset = ((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == transfer_mode) &&
                         (MSPI_TRANSFER_TRIGGER_DMA != p_extend->mspi_transfer_trigger) &&
                         ((NULL != p_transfer_tx) || (NULL != p_transfer_rx));
#endif

        if (dts_fifo_cfset)
        {
            uint32_t half = ((uint32_t) p_instance_ctrl->stage_size / MSPI_HALF_DIVISOR);

            /* Refill padding so the whole length is a whole number of half-stage bursts. */
            uint32_t fifo_refill = 0U;

            if (0U != half)
            {
                fifo_refill = ((((p_instance_ctrl->length + half) - 1U) / half) * half) - p_instance_ctrl->length;
            }

            refill_data                = fifo_refill;
            p_mspi_regs[channel].CFSET = (uint16_t) (p_instance_ctrl->length + fifo_refill);
        }
        else
        {
            p_mspi_regs[channel].CFSET = (p_instance_ctrl->chunk_frames + refill_data); /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
        }
    }

    /* Runtime RAM-region arbitration: reject if the window overflows the unit RAM or overlaps a channel currently
     * transferring, else claim the window for this transfer. */
    fsp_err_t ram_err = r_mspi_ram_region_acquire(p_instance_ctrl);
    if (FSP_SUCCESS != ram_err)
    {
        return ram_err;
    }

    g_mspi_unit_ctrl[p_extend->unit].ram_active_mask |= (uint32_t) (MSPI_CHANNEL_STATUS_BIT << channel); /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */

    /* Setup initial Buffer Memory Data */
    if (MSPI_TRANSFER_MODE_BUFFER_MEMORY == transfer_mode)                                               /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        p_mspi_regs[channel].CFG0_b.ITXE = MSPI_INTERRUPT_OUT_DISABLE;                                   /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
        p_mspi_regs[channel].CFG0_b.IRXE = MSPI_INTERRUPT_OUT_DISABLE;                                   /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */

        if (MSPI_SELECTABLE_MODE_RECEIVE_ONLY != p_extend->mspi_select)
        {
            r_mspi_buffer_write(p_instance_ctrl);
        }
    }

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)

    /* One-shot DTS block parameters (FIFO mode, DTS trigger). */
    bool dts_fifo = ((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == transfer_mode) &&
                    (MSPI_TRANSFER_TRIGGER_DMA != p_extend->mspi_transfer_trigger);
    uint32_t block_units = ((uint32_t) p_instance_ctrl->stage_size / MSPI_HALF_DIVISOR) * num_count;

    /* DTS count-match spans the whole length (+refill), matching CFSET. */
    uint32_t total_units = (p_instance_ctrl->length + refill_data) * num_count;

    if (NULL != p_transfer_tx)
    {
        /* Calculate transfer size base on bit_width */
        transfer_size     = r_mspi_cal_transfer_size(bit_width); /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
        transfer_src_size = transfer_size;
        if (((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY ==
             p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode) ||
            (MSPI_TRANSFER_TRIGGER_DMA != p_extend->mspi_transfer_trigger))
        {
            if (SPI_BIT_WIDTH_32_BITS >= bit_width)
            {
                transfer_des_size = transfer_size;
            }
            else
            {
                transfer_des_size = TRANSFER_SIZE_4_BYTE;
            }

            p_transfer_tx->p_cfg->p_info->transfer_count_per_hw_request =
                (uint16_t) ((((uint32_t) p_instance_ctrl->stage_size) / MSPI_HALF_DIVISOR) * num_count);
        }
        else
        {
            transfer_des_size = transfer_size;
        }

        /* Enable interrupt output for transmission */
        p_mspi_regs[channel].CFG0_b.ITXE = MSPI_INTERRUPT_OUT_ENABLE; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */

        /* Update data for transmission transfer */
        if (NULL == p_src)
        {
            p_transfer_tx->p_cfg->p_info->p_src = (void const *) &dummy_tx_data;
            p_transfer_tx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_addr_mode =            /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                            TRANSFER_ADDR_MODE_FIXED;
            p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.src_addr_direction = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                       TRANSFER_DTSC_ADDR_MODE_FIXED;
        }
        else
        {
            p_transfer_tx->p_cfg->p_info->p_src = (void const *) p_src;
            p_transfer_tx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_addr_mode =                           /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                            TRANSFER_ADDR_MODE_INCREMENTED;
            p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.src_addr_direction =                /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                       TRANSFER_DTSC_ADDR_MODE_INCREMENTED;
        }

        p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_data_size = transfer_des_size; /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
        p_transfer_tx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size           = transfer_src_size; /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
        p_transfer_tx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size           = transfer_des_size; /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */

        /* Set up data size for DMA or DTS. */
        if (MSPI_TRANSFER_TRIGGER_DMA == p_extend->mspi_transfer_trigger)
        {
            /* DMA scales by the transfer size internally, so pass the frame count only. */
            p_transfer_tx->p_cfg->p_info->number_transfer =
                (uint32_t) ((p_instance_ctrl->chunk_frames + refill_data) * num_count);
        }
        else if (dts_fifo)
        {
            /* DTS one-shot (FIFO): half-stage block per request, auto-reload, single count-match at whole length. */
            p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_mode = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                  TRANSFER_DTSC_MODE_BLOCK_1;
            p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.reload_func_1 = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                  TRANSFER_RELOAD_FUNCTION_1_DEST_ADDR_RELOAD;
            p_transfer_tx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.reload_func_2 = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                  TRANSFER_RELOAD_FUNCTION_2_DEST_ADDR_RELOAD;

            p_transfer_tx->p_cfg->p_info->number_transfer             = block_units;
            p_transfer_tx->p_cfg->p_info->reload_number_transfer      = (uint16_t) block_units;
            p_transfer_tx->p_cfg->p_info->number_transfer_count_match = (uint16_t) total_units;
            p_transfer_tx->p_cfg->p_info->addr_reload_count           = (uint16_t) block_units;
            p_transfer_tx->p_cfg->p_info->reload_addr_reload_count    = (uint16_t) block_units;
            p_transfer_tx->p_cfg->p_info->p_reload_des_addr           =
                (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].TXDA0); /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
        }
        else
        {
            /* DTS (non-FIFO) fallback: whole chunk in 32-bit words. */
            p_transfer_tx->p_cfg->p_info->number_transfer =
                (uint32_t) ((p_instance_ctrl->chunk_frames + refill_data) * num_count);
            p_transfer_tx->p_cfg->p_info->p_reload_des_addr =
                (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].TXDA0);               /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
            p_transfer_tx->p_cfg->p_info->addr_reload_count =
                (uint16_t) ((((uint32_t) p_instance_ctrl->stage_size) / MSPI_HALF_DIVISOR) * num_count);
            p_transfer_tx->p_cfg->p_info->reload_addr_reload_count =
                (uint16_t) ((p_instance_ctrl->stage_size / MSPI_HALF_DIVISOR) * num_count); /* PRQA S 1891 # DP_MSR_R-10.7_UC1 */
            p_transfer_tx->p_cfg->p_info->reload_number_transfer =
                (uint16_t) ((p_instance_ctrl->stage_size) * num_count);
        }

        /* When using transfer api in Buffer Mode, no data transfer by DMA/DTS */
        if ((uint32_t) MSPI_TRANSFER_MODE_BUFFER_MEMORY ==
            p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
        {
            /* In Buffer Memory mode, data are transferred directly through RAM */
            p_transfer_tx->p_cfg->p_info->number_transfer = 0U;
        }

        /* Re-config for data update */
        fsp_err_t err = p_transfer_tx->p_api->reconfigure(p_transfer_tx->p_ctrl, p_transfer_tx->p_cfg->p_info);
        if (FSP_SUCCESS != err)
        {
            r_mspi_ram_region_release(p_instance_ctrl);

            return err;
        }

        /* Enable transfer */
        err = p_transfer_tx->p_api->enable(p_transfer_tx->p_ctrl);
        if (FSP_SUCCESS != err)
        {
            r_mspi_ram_region_release(p_instance_ctrl);

            return err;
        }
    }

    if (NULL != p_transfer_rx)
    {
        if (MSPI_TRANSFER_MODE_BUFFER_MEMORY ==                           /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
        {
            p_mspi_regs[channel].CFG0_b.IRXE = MSPI_INTERRUPT_OUT_ENABLE; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
            r_mspi_buffer_dma_rx_config(p_instance_ctrl);
        }
        else
        {
            transfer_size     = r_mspi_cal_transfer_size(bit_width); /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
            transfer_des_size = transfer_size;
            if (((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY ==
                 p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode) ||
                (MSPI_TRANSFER_TRIGGER_DMA != p_extend->mspi_transfer_trigger))
            {
                if (SPI_BIT_WIDTH_32_BITS >= bit_width)
                {
                    transfer_src_size = transfer_size;
                }
                else
                {
                    transfer_src_size = TRANSFER_SIZE_4_BYTE;
                }

                p_transfer_rx->p_cfg->p_info->transfer_count_per_hw_request =
                    (uint16_t) ((((uint32_t) p_instance_ctrl->stage_size) / MSPI_HALF_DIVISOR) * num_count);
            }
            else
            {
                transfer_src_size = transfer_size;
            }

            /* Enable interrupt output for reception */
            p_mspi_regs[channel].CFG0_b.IRXE = MSPI_INTERRUPT_OUT_ENABLE; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */

            /* Update data for reception transfer */
            if (NULL == p_dest)
            {
                p_transfer_rx->p_cfg->p_info->p_dest = (void *) &dummy_rx_data;
                p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode =            /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                TRANSFER_ADDR_MODE_FIXED;
                p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.des_addr_direction = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                           TRANSFER_DTSC_ADDR_MODE_FIXED;
            }
            else
            {
                p_transfer_rx->p_cfg->p_info->p_dest = (void *) p_dest;
                p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode =                 /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                TRANSFER_ADDR_MODE_INCREMENTED;
                p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.des_addr_direction =      /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                           TRANSFER_DTSC_ADDR_MODE_INCREMENTED;
            }

            p_transfer_rx->p_cfg->p_info->p_src = (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].RXDA0); /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
            p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_addr_mode =                     /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                            TRANSFER_ADDR_MODE_FIXED;
            p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.src_addr_direction =          /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                       TRANSFER_DTSC_ADDR_MODE_FIXED;

            p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_data_size =          /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                       transfer_src_size;
            p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size =                    /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                             transfer_src_size;
            p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size =                    /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                             transfer_des_size;

            /* Set up data size for DMA or DTS. */
            if (MSPI_TRANSFER_TRIGGER_DMA == p_extend->mspi_transfer_trigger)
            {
                p_transfer_rx->p_cfg->p_info->number_transfer =
                    (uint32_t) ((p_instance_ctrl->chunk_frames + refill_data) * num_count);
            }
            else if (dts_fifo)
            {
                /* DTS one-shot (FIFO): half-stage block per request, auto-reload, single count-match at whole
                 * length. */
                p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_mode = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                      TRANSFER_DTSC_MODE_BLOCK_1;
                p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.reload_func_1 = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                      TRANSFER_RELOAD_FUNCTION_1_SRC_ADDR_RELOAD;
                p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.reload_func_2 = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                      TRANSFER_RELOAD_FUNCTION_2_SRC_ADDR_RELOAD;

                p_transfer_rx->p_cfg->p_info->number_transfer             = block_units;
                p_transfer_rx->p_cfg->p_info->reload_number_transfer      = (uint16_t) block_units;
                p_transfer_rx->p_cfg->p_info->number_transfer_count_match = (uint16_t) total_units;
                p_transfer_rx->p_cfg->p_info->addr_reload_count           = (uint16_t) block_units;
                p_transfer_rx->p_cfg->p_info->reload_addr_reload_count    = (uint16_t) block_units;
                p_transfer_rx->p_cfg->p_info->p_reload_src_addr           =
                    (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].RXDA0); /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
            }
            else
            {
                /* DTS (non-FIFO) fallback: whole chunk in 32-bit words. */
                p_transfer_rx->p_cfg->p_info->number_transfer =
                    (uint32_t) ((p_instance_ctrl->chunk_frames + refill_data) * num_count);
                p_transfer_rx->p_cfg->p_info->p_reload_src_addr =
                    (void *) &(p_instance_ctrl->p_regs->MSPI0_CH[channel].RXDA0);               /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
                p_transfer_rx->p_cfg->p_info->addr_reload_count =
                    (uint16_t) ((((uint32_t) p_instance_ctrl->stage_size) / MSPI_HALF_DIVISOR) * num_count);
                p_transfer_rx->p_cfg->p_info->reload_addr_reload_count =
                    (uint16_t) ((p_instance_ctrl->stage_size / MSPI_HALF_DIVISOR) * num_count); /* PRQA S 1891 # DP_MSR_R-10.7_UC1 */
                p_transfer_rx->p_cfg->p_info->reload_number_transfer =
                    (uint16_t) ((p_instance_ctrl->stage_size) * num_count);
            }

            /* Re-config for data update */
            fsp_err_t err = p_transfer_rx->p_api->reconfigure(p_transfer_rx->p_ctrl, p_transfer_rx->p_cfg->p_info);
            if (FSP_SUCCESS != err)
            {
                r_mspi_ram_region_release(p_instance_ctrl);

                return err;
            }

            /* Enable transfer */
            err = p_transfer_rx->p_api->enable(p_transfer_rx->p_ctrl);
            if (FSP_SUCCESS != err)
            {
                r_mspi_ram_region_release(p_instance_ctrl);

                return err;
            }
        }
    }
#endif
#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)

    /* Slave FIFO transmit-only completes on the frame-end ISR. Re-arm IFEE for this transfer. */
    if ((MSPI_COMPLETE_SRC_FE_ISR == p_instance_ctrl->completion_source) &&
        ((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == transfer_mode) &&
        ((NULL != p_instance_ctrl->p_cfg->p_transfer_tx) ||
         (NULL != p_instance_ctrl->p_cfg->p_transfer_rx)))
    {
        p_mspi_regs[channel].CFG0_b.IFEE = MSPI_INTERRUPT_OUT_ENABLE; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    }
#endif

    if (0U == p_mspi_regs[channel].CFG4_b.HWTS)
    {
        /* Enable MSPI channel and start transfer */
        p_mspi_regs[channel].CSTS = (R_MSPI0_MSPI0_CH_CSTS_CHENS_Msk | R_MSPI0_MSPI0_CH_CSTS_ACTFS_Msk); /* PRQA S 4464 # DP_MSR_R-10.3_UC1 */
    }
    else
    {
        /* Hardware trigger driven by DMA or DTS. */
        p_mspi_regs[channel].CSTS = R_MSPI0_MSPI0_CH_CSTS_CHENS_Msk; /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    }

    /* Arm the in-flight chunk pending flags. */
    if (MSPI_TRANSFER_MODE_BUFFER_MEMORY == transfer_mode)           /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        p_instance_ctrl->chunk_tx_pending = 0U;
        p_instance_ctrl->chunk_rx_pending =
            ((NULL != p_instance_ctrl->p_cfg->p_transfer_rx) &&
             ((uint32_t) MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE)) ? 1U : 0U;
    }
    else
    {
        p_instance_ctrl->chunk_tx_pending =
            ((uint32_t) MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE) ? 1U : 0U;
        p_instance_ctrl->chunk_rx_pending =
            ((uint32_t) MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE) ? 1U : 0U;
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
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;                    /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    R_MSPI0_MSPI0_CH_Type     * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;    /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
    mspi_extended_cfg_t const * p_extend    = (mspi_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    uint32_t chip_select = p_instance_ctrl->p_cfg->channel;
    uint8_t  num_count   = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);                              /* PRQA S 3469 # Advisory D-4.9 */

    /* Clear this channel's TX notification flag (write-1-to-clear). */
    p_mspi_intf_regs[MSPI_INTF_IDX_TX].INTFC = (uint32_t) (1U << chip_select);                                /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */

    /* Check whether FIFO mode is enabled and perform write operation */
    if ((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY ==
        p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
    {
        r_mspi_fifo_write(p_instance_ctrl);
    }
    else
    {
        r_mspi_direct_write(p_instance_ctrl);
    }

    /* Frame-end ISR is active only when BOTH the peripheral output (IFEE) and the NVIC
     * vector (valid tei_irq with an enabled priority) are enabled. */
    bool fe_isr_active = ((BSP_IRQ_DISABLED != p_instance_ctrl->p_cfg->tei_ipl) &&
                          (MSPI_INTERRUPT_OUT_ENABLE == p_mspi_regs[chip_select].CFG0_b.IFEE)); /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */

    /* No frame-end ISR + transmit-only: no receive ISR exists to complete the transfer,
     * so drive chunk continuation/completion from the transmit ISR once the in-flight
     * chunk's TX frames have all been written. RX-enabled transfers complete from the
     * receive ISR instead. */
    if ((MSPI_COMPLETE_SRC_FE_ISR == p_instance_ctrl->completion_source) &&
        (!fe_isr_active) &&
        (MSPI_RECEPTION_ENABLE != p_mspi_regs[chip_select].CFG0_b.RXE)) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        uint32_t chunk_tx_end = (p_instance_ctrl->frames_done + p_instance_ctrl->chunk_frames) * num_count;

        if (p_instance_ctrl->tx_count >= chunk_tx_end)
        {
            r_mspi_continue_or_complete(p_instance_ctrl, true, true);
        }
    }
}

/*******************************************************************************************************************//**
 * Copy configured bit width from the MSPI data register to the current rx data location.
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_receive (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;                    /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    R_MSPI0_MSPI0_CH_Type     * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;    /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
    mspi_extended_cfg_t const * p_extend    = (mspi_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    uint32_t chip_select = p_instance_ctrl->p_cfg->channel;
    uint8_t  num_count   = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);                              /* PRQA S 3469 # Advisory D-4.9 */

    /* Read CFG0 register value to check configurations */
    uint32_t cfg0_val = p_mspi_regs[chip_select].CFG0;

    /* Clear this channel's RX notification flag (write-1-to-clear). */
    p_mspi_intf_regs[MSPI_INTF_IDX_RX].INTFC = (uint32_t) (1U << chip_select);                                    /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */

    /* Check whether FIFO mode is enabled and perform read operation */
    if (MSPI_TRANSFER_MODE_FIFO_MEMORY == p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        /* FIFO access mode */
        r_mspi_fifo_read(p_instance_ctrl);
    }
    else
    {
        /* Direct access mode (non-FIFO) */
        r_mspi_direct_read(p_instance_ctrl);
    }

    /* Frame-end ISR is active only when BOTH the peripheral output (IFEE) and the NVIC
     * vector (valid tei_irq with an enabled priority) are enabled. If either is disabled
     * the frame-end ISR will never run, so the receive ISR must drive completion. */
    bool fe_isr_active = ((BSP_IRQ_DISABLED != p_instance_ctrl->p_cfg->tei_ipl) &&
                          (MSPI_INTERRUPT_OUT_ENABLE == p_mspi_regs[chip_select].CFG0_b.IFEE)); /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */

    /* Process callback logic only if FCCE mode is enabled */
    if ((cfg0_val & ((uint32_t) 1U << R_MSPI0_MSPI0_CH_CFG0_FCCE_Pos)) != 0U)
    {
        /* Check if all data has been completely received */
        if (p_instance_ctrl->rx_count >= (p_instance_ctrl->length * num_count))
        {
            r_mspi_notify_if_complete(p_instance_ctrl);
        }
    }
    /* No frame-end ISR available: drive chunk continuation/completion here once the
     * in-flight chunk's RX frames have all been read. notify_if_complete still waits on
     * ACTF, so the callback is only raised after the last frame has physically shifted. */
    else if ((MSPI_COMPLETE_SRC_FE_ISR == p_instance_ctrl->completion_source) && (!fe_isr_active))
    {
        uint32_t chunk_rx_end = (p_instance_ctrl->frames_done + p_instance_ctrl->chunk_frames) * num_count;

        if (p_instance_ctrl->rx_count >= chunk_rx_end)
        {
            r_mspi_continue_or_complete(p_instance_ctrl, true, true);
        }
    }
    else
    {
        /* Completion is driven by the frame-end ISR. */
    }
}

/*******************************************************************************************************************//**
 * Copy data into the MSPI RAM for transmission in fixed buffer mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_buffer_write (mspi_instance_ctrl_t * p_instance_ctrl)                               /* PRQA S 3673 # Advisory R-8.13 */
{
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */

    /* Use chunk_frames (source of truth); CFSET may not be programmed yet on the first call. */
    uint32_t chunk_size = p_instance_ctrl->chunk_frames;
    uint32_t tx_offset  = p_instance_ctrl->tx_count;

    uintptr_t           rastad   = p_mspi_regs[channel].RASTAD;
    uintptr_t           ram_base = (uintptr_t) p_instance_ctrl->p_regs + MSPI_INIT_RAM_ADDR;
    uintptr_t           ram_addr = ram_base + rastad;
    uint32_t            i        = 0U; /* PRQA S 1710 # Advisory D-4.5 */
    uint64_t            data_64;
    volatile uint8_t  * p_dest_8;      /* PRQA S 1710 # Advisory D-4.5 */
    volatile uint16_t * p_dest_16;     /* PRQA S 1710 # Advisory D-4.5 */
    volatile uint32_t * p_dest_32;     /* PRQA S 1710 # Advisory D-4.5 */

    uint8_t  * p_src_8;                /* PRQA S 1710, 3678 # Advisory D-4.5, Advisory R-8.13 */
    uint16_t * p_src_16;               /* PRQA S 1710, 3678 # Advisory D-4.5, Advisory R-8.13 */
    uint32_t * p_src_32;               /* PRQA S 3678 # Advisory R-8.13 */
    uint64_t * p_src_64;               /* PRQA S 3678 # Advisory R-8.13 */

    /* Check for NULL pointer (e.g., when the user only wants to Read) */
    if (NULL == p_instance_ctrl->p_tx_data)
    {
        return;
    }

    /* Confine the TX write to the TX portion of the RAM. */
    {
        uint32_t tx_capacity = r_mspi_ram_max_frames(p_instance_ctrl);

        if (chunk_size > tx_capacity)
        {
            chunk_size = tx_capacity;
        }
    }

    /* Write data from the user buffer to MSPI RAM based on the configured bit width */
    if (SPI_BIT_WIDTH_8_BITS >= p_instance_ctrl->bitwidth)
    {
        p_dest_8 = (volatile uint8_t *) ram_addr;
        p_src_8  = (uint8_t *) p_instance_ctrl->p_tx_data; /* PRQA S 0316, 0311 # Advisory R-11.5, DP_MSR_R-11.8_UC1 */

        for (i = 0U; i < chunk_size; i++)
        {
            p_dest_8[i] = p_src_8[tx_offset + i];
        }
    }
    else if (SPI_BIT_WIDTH_16_BITS >= p_instance_ctrl->bitwidth)
    {
        p_dest_16 = (volatile uint16_t *) ram_addr;
        p_src_16  = (uint16_t *) p_instance_ctrl->p_tx_data; /* PRQA S 0316, 0311 # Advisory R-11.5, DP_MSR_R-11.8_UC1 */

        for (i = 0U; i < chunk_size; i++)
        {
            p_dest_16[i] = p_src_16[tx_offset + i];
        }
    }
    else if (SPI_BIT_WIDTH_32_BITS >= p_instance_ctrl->bitwidth)
    {
        p_dest_32 = (volatile uint32_t *) ram_addr;
        p_src_32  = (uint32_t *) p_instance_ctrl->p_tx_data; /* PRQA S 0316, 0311 # Advisory R-11.5, DP_MSR_R-11.8_UC1 */

        for (i = 0U; i < chunk_size; i++)
        {
            p_dest_32[i] = p_src_32[tx_offset + i];
        }
    }
    else
    {
        /* Greater than 32-bit width (up to 64-bit) */
        p_dest_32 = (volatile uint32_t *) ram_addr;
        p_src_64  = (uint64_t *) p_instance_ctrl->p_tx_data; /* PRQA S 0316, 0311 # Advisory R-11.5, DP_MSR_R-11.8_UC1 */

        for (i = 0U; i < chunk_size; i++)
        {
            data_64 = p_src_64[tx_offset + i];

            p_dest_32[i * MSPI_WORDS_PER_64BIT] = (uint32_t) (data_64 & MSPI_LOWER_WORD_MASK);
            p_dest_32[(i * MSPI_WORDS_PER_64BIT) + MSPI_WORD_OFFSET_UPPER] =
                (uint32_t) ((data_64 >> MSPI_UPPER_WORD_SHIFT) & MSPI_LOWER_WORD_MASK);
        }
    }
}

/*******************************************************************************************************************//**
 * Copy data from the MSPI RAM for reception in fixed buffer mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_buffer_read (mspi_instance_ctrl_t const * p_instance_ctrl)
{
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */

    uint32_t chunk_size = p_mspi_regs[channel].CFSET;
    uint32_t rx_offset  = p_instance_ctrl->rx_count;

    uintptr_t rastad   = p_mspi_regs[channel].RASTAD;
    uintptr_t ram_base = (uintptr_t) p_instance_ctrl->p_regs + MSPI_INIT_RAM_ADDR;
    uintptr_t ram_addr = ram_base + rastad;
    uint32_t  low_word;
    uint32_t  high_word;
    uint32_t  align_elements = 0U;
    uint32_t  index          = 0U;

    volatile uint8_t  * p_src_8;       /* PRQA S 3678 # Advisory R-8.13 */
    volatile uint16_t * p_src_16;      /* PRQA S 3678 # Advisory R-8.13 */
    volatile uint32_t * p_src_32;      /* PRQA S 3678 # Advisory R-8.13 */

    uint8_t  * p_dest_8;
    uint16_t * p_dest_16;
    uint32_t * p_dest_32;
    uint64_t * p_dest_64;

    /* Check for NULL pointer */
    if (NULL == p_instance_ctrl->p_rx_data)
    {
        return;
    }

    /* In full-duplex the RX region follows the TX region (+4-byte alignment). */
    if ((1U == p_mspi_regs[channel].CFG0_b.RXE) && (1U == p_mspi_regs[channel].CFG0_b.TXE))
    {
        align_elements = (uint32_t) r_mspi_align_ram(chunk_size, (uint32_t) ram_addr, p_instance_ctrl->bitwidth) + /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
                         chunk_size;
    }

    /* Read data from MSPI RAM to the user buffer based on the configured bit width */
    if (SPI_BIT_WIDTH_8_BITS >= p_instance_ctrl->bitwidth)
    {
        p_src_8  = (volatile uint8_t *) ram_addr;
        p_dest_8 = (uint8_t *) p_instance_ctrl->p_rx_data; /* PRQA S 0316 # Advisory R-11.5 */

        for (index = 0U; index < chunk_size; index++)
        {
            p_dest_8[rx_offset + index] = p_src_8[align_elements + index];
        }
    }
    else if (SPI_BIT_WIDTH_16_BITS >= p_instance_ctrl->bitwidth)
    {
        p_src_16  = (volatile uint16_t *) ram_addr;
        p_dest_16 = (uint16_t *) p_instance_ctrl->p_rx_data; /* PRQA S 0316 # Advisory R-11.5 */

        for (index = 0U; index < chunk_size; index++)
        {
            p_dest_16[rx_offset + index] = p_src_16[align_elements + index];
        }
    }
    else if (SPI_BIT_WIDTH_32_BITS >= p_instance_ctrl->bitwidth)
    {
        p_src_32  = (volatile uint32_t *) ram_addr;
        p_dest_32 = (uint32_t *) p_instance_ctrl->p_rx_data; /* PRQA S 0316 # Advisory R-11.5 */

        for (index = 0U; index < chunk_size; index++)
        {
            p_dest_32[rx_offset + index] = p_src_32[align_elements + index];
        }
    }
    else
    {
        /* Greater than 32-bit width (up to 64-bit) */
        p_src_32  = (volatile uint32_t *) ram_addr;
        p_dest_64 = (uint64_t *) p_instance_ctrl->p_rx_data; /* PRQA S 0316 # Advisory R-11.5 */

        uint32_t ram_offset_32 = align_elements * MSPI_WORDS_PER_64BIT;

        for (index = 0U; index < chunk_size; index++)
        {
            low_word  = p_src_32[ram_offset_32 + (index * MSPI_WORDS_PER_64BIT)];
            high_word = p_src_32[ram_offset_32 + (index * MSPI_WORDS_PER_64BIT) + MSPI_WORD_OFFSET_UPPER];

            p_dest_64[rx_offset + index] = ((uint64_t) high_word << MSPI_UPPER_WORD_SHIFT) | (uint64_t) low_word;
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
    const uint32_t count      = (uint32_t) r_mspi_count_byte(bitwidth);
    const uint32_t write_end  = (count * length) + ram_init;
    const uint32_t read_start = ((write_end + ALIGNMENT_BOUNDARY_BYTES - 1U) & (uint32_t) (~ALIGNMENT_MASK));
    const uint32_t align      = ((read_start - write_end) / count);

    return (uint8_t) align;
}

/*******************************************************************************************************************//**
 * Count number of data unit base on length of data.
 *
 * @param[in]  bitwidth       length of data (ex: 8 bits, 16 bits,...)
 **********************************************************************************************************************/
static uint8_t r_mspi_count_byte (uint32_t bitwidth)
{
    uint8_t count = 0U;

    if ((uint32_t) SPI_BIT_WIDTH_8_BITS >= bitwidth)
    {
        count = MSPI_LENGTH_PER_COUNT_1;
    }
    else if ((uint32_t) SPI_BIT_WIDTH_16_BITS >= bitwidth)
    {
        count = MSPI_LENGTH_PER_COUNT_2;
    }
    else if ((uint32_t) SPI_BIT_WIDTH_32_BITS >= bitwidth)
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
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;
    uint8_t                 stage       = p_instance_ctrl->stage_size;
    uint8_t                 num_count   = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);        /* PRQA S 3469 # Advisory D-4.9 */
    uint8_t                 refill      = r_mspi_fifo_refill_stage(p_instance_ctrl);

    uint32_t half_writes = ((uint32_t) stage * (uint32_t) num_count) / MSPI_HALF_DIVISOR;
    uint32_t real_writes = p_instance_ctrl->chunk_frames * num_count;
    uint32_t total_write = (p_instance_ctrl->chunk_frames + refill) * num_count;
    uint32_t base_real   = p_instance_ctrl->frames_done * num_count;
    uint32_t written     = p_instance_ctrl->tx_count;

    uint32_t chunk_written = (written > base_real) ? (written - base_real) : 0U;

    /* Transmit data from transmission buffer until half of the stage */
    while (chunk_written < total_write)
    {
        if (chunk_written < real_writes)
        {
            p_instance_ctrl->tx_count = base_real + chunk_written;

            /* Write data to register */
            r_mspi_write_register(p_instance_ctrl);
        }
        else
        {
            p_mspi_regs[channel].TXDA0 = 0U;
        }

        chunk_written++;

        /* Increase transmission counter */
        p_instance_ctrl->tx_count = base_real + chunk_written;

        /* Return when half of the stage is filled */
        if ((0U != half_writes) && (0U == (chunk_written % half_writes)))
        {
            SYNCP();                   /* PRQA S 3469 # Advisory D-4.9 */

            return;
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
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;
    uint8_t                 stage       = p_instance_ctrl->stage_size;
    uint8_t                 num_count   = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);        /* PRQA S 3469 # Advisory D-4.9 */
    uint8_t                 refill      = r_mspi_fifo_refill_stage(p_instance_ctrl);

    uint32_t half_reads  = ((uint32_t) stage * (uint32_t) num_count) / MSPI_HALF_DIVISOR;
    uint32_t real_reads  = p_instance_ctrl->chunk_frames * num_count;
    uint32_t total_reads = (p_instance_ctrl->chunk_frames + refill) * num_count;
    uint32_t base_real   = p_instance_ctrl->frames_done * num_count;

    while (p_instance_ctrl->chunk_read_done < total_reads)
    {
        /* RXI normally means that a complete half-stage is available. Frame-end can occur
         * with fewer entries remaining, so do not read past the count reported by FIRXN. */
        if (0U == p_mspi_regs[channel].CSTR_b.FIRXN)
        {
            return;
        }

        if (p_instance_ctrl->chunk_read_done < real_reads)
        {
            p_instance_ctrl->rx_count = base_real + p_instance_ctrl->chunk_read_done;

            /* Read data from register */
            r_mspi_read_register(p_instance_ctrl);
        }
        else
        {
            volatile uint32_t discard = p_mspi_regs[channel].RXDA0;
            (void) discard;
        }

        p_instance_ctrl->chunk_read_done++;

        /* Return when half of the stage is filled */
        if ((0U != half_reads) && (0U == (p_instance_ctrl->chunk_read_done % half_reads)))
        {
            SYNCP();                   /* PRQA S 3469 # Advisory D-4.9 */

            return;
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
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    uint8_t                 num_count   = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);        /* PRQA S 3469 # Advisory D-4.9 */
    uint32_t                tx_count    = p_instance_ctrl->tx_count;
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;

    /* If all data has been transmitted, return */
    if (p_instance_ctrl->tx_count == ((p_instance_ctrl->length) * num_count))
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

        /* Update transmit count only after a successful TX write. */
        p_instance_ctrl->tx_count = tx_count + 1U;
    }
}

/*******************************************************************************************************************//**
 * Copy data from the MSPI RAM for reception in direct mode.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_direct_read (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
    uint8_t                 num_count   = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);        /* PRQA S 3469 # Advisory D-4.9 */
    uint32_t                channel     = p_instance_ctrl->p_cfg->channel;
    uint32_t                rx_count    = p_instance_ctrl->rx_count;

    /* If all data has been receive, return */
    if (p_instance_ctrl->rx_count >= ((p_instance_ctrl->length) * num_count))
    {
        return;
    }

    /* Only read when the hardware actually holds a received frame. This prevents an
     * over-read if a stray RXI runs after the frame-end ISR has already drained it. */
    if (0U == p_mspi_regs[channel].CSTR_b.RXRQF)
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
static void r_mspi_read_register (mspi_instance_ctrl_t const * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs =                                                              /* PRQA S 3678 # Advisory R-8.13 */
                                          (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */

    uint32_t const rx_count    = p_instance_ctrl->rx_count;
    uint32_t const chip_select = p_instance_ctrl->p_cfg->channel;
    uint32_t const rx_data     = p_mspi_regs[chip_select].RXDA0;

    if (NULL == p_instance_ctrl->p_rx_data)
    {
        return;
    }

    /* Store RX data into the user buffer according to the configured bit width. */
    if (SPI_BIT_WIDTH_8_BITS >= p_instance_ctrl->bitwidth)
    {
        /* Store 8-bit data */
        ((uint8_t *) p_instance_ctrl->p_rx_data)[rx_count] = (uint8_t) rx_data; /* PRQA S 0316 # Advisory R-11.5 */
    }
    else if (SPI_BIT_WIDTH_16_BITS >= p_instance_ctrl->bitwidth)
    {
        /* Store 16-bit data */
        ((uint16_t *) p_instance_ctrl->p_rx_data)[rx_count] = (uint16_t) rx_data; /* PRQA S 0316 # Advisory R-11.5 */
    }
    else
    {
        /* Store over 16-bit data */
        ((uint32_t *) p_instance_ctrl->p_rx_data)[rx_count] = rx_data; /* PRQA S 0316 # Advisory R-11.5 */
    }
}

/*******************************************************************************************************************//**
 * Copy data from the buffer to register.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_write_register (mspi_instance_ctrl_t const * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs =
        (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */

    uint32_t const chip_select = p_instance_ctrl->p_cfg->channel;
    uint32_t const tx_count    = p_instance_ctrl->tx_count;
    uint32_t       tx_data     = 0U;

    /* Store TX data from the user buffer according to the configured bit width. */
    if (NULL != p_instance_ctrl->p_tx_data)
    {
        if (SPI_BIT_WIDTH_8_BITS >= p_instance_ctrl->bitwidth)
        {
            /* Write 8-bit data */
            tx_data = ((uint8_t const *) p_instance_ctrl->p_tx_data)[tx_count]; /* PRQA S 0316 # Advisory R-11.5 */
        }
        else if (SPI_BIT_WIDTH_16_BITS >= p_instance_ctrl->bitwidth)
        {
            /* Write 16-bit data */
            tx_data = ((uint16_t const *) p_instance_ctrl->p_tx_data)[tx_count]; /* PRQA S 0316 # Advisory R-11.5 */
        }
        else
        {
            /* Write 32-bit data */
            tx_data = ((uint32_t const *) p_instance_ctrl->p_tx_data)[tx_count]; /* PRQA S 0316 # Advisory R-11.5 */
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
static void r_mspi_call_callback (mspi_instance_ctrl_t const * p_instance_ctrl, spi_event_t event)
{
    spi_callback_args_t args;

    /* Store callback arguments in memory provided by user if available. */
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
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static uint8_t r_mspi_fifo_refill_stage (mspi_instance_ctrl_t const * p_instance_ctrl)
{
    const mspi_extended_cfg_t * p_extend   = (const mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    const uint32_t              length     = p_instance_ctrl->chunk_frames;
    const uint32_t              half_stage = (uint32_t) p_instance_ctrl->stage_size / MSPI_HALF_DIVISOR;
    uint8_t refill = 0U;

    if ((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY ==
        p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode)
    {
        /* Pad the chunk up to a whole number of half-stage bursts. */
        if (0U != half_stage)
        {
            refill = (uint8_t) ((((length + half_stage - 1U) / half_stage) * half_stage) - length);
        }
    }
    else
    {
        /* No refill in direct mode and buffer memory mode. */
        refill = 0U;
    }

    /* Return transfer count */
    return refill;
}

/*******************************************************************************************************************//**
 * Notifies the application of transfer completion, once, after the required length has been transferred on every
 * enabled direction. Releases the channel's RAM window so a sibling may reuse the region.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_notify_if_complete (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
    uint8_t                 channel     = p_instance_ctrl->p_cfg->channel;
    uint8_t                 num_count   = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);        /* PRQA S 3469 # Advisory D-4.9 */
    uint32_t                expected    = p_instance_ctrl->length * num_count;

    /* Read CFG0 register value to check configurations */
    uint32_t cfg0_val = p_mspi_regs[channel].CFG0;

    bool tx_enabled = ((uint32_t) MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE);
    bool rx_enabled = ((uint32_t) MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE);

    /* Already reported for this transfer. */
    if (0U != p_instance_ctrl->transfer_notified)
    {
        return;
    }

    /* Every enabled direction must have reached the requested length. */
    if (tx_enabled && (p_instance_ctrl->tx_count < expected))
    {
        return;
    }

    if (rx_enabled && (p_instance_ctrl->rx_count < expected))
    {
        return;
    }

    /* Mark as notified BEFORE the callback to guarantee single-shot delivery. */
    p_instance_ctrl->transfer_notified = 1U;

    /* Wait for the frame-end event before completing the transfer. */
    if (MSPI_COMPLETE_SRC_TXRX_XFER == p_instance_ctrl->completion_source)
    {
        mspi_extended_cfg_t const * p_extend = (mspi_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
        uint32_t mode = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;

        bool dts_fifo = ((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == mode) &&
                        (MSPI_TRANSFER_TRIGGER_DMA != p_extend->mspi_transfer_trigger) &&
                        ((NULL != p_instance_ctrl->p_cfg->p_transfer_tx) ||
                         (NULL != p_instance_ctrl->p_cfg->p_transfer_rx));

        /* Wait for the channel to become inactive so ACTF clears. */
        if (dts_fifo)
        {
            FSP_HARDWARE_REGISTER_WAIT(p_mspi_regs[channel].CSTR_b.ACTF, 0U);
        }
    }

    /* Wait for the channel to become inactive so ACTF clears. */
    if (0U == (cfg0_val & ((uint32_t) 1U << R_MSPI0_MSPI0_CH_CFG0_FCCE_Pos)))
    {
        FSP_HARDWARE_REGISTER_WAIT(p_mspi_regs[channel].CSTR_b.ACTF, 0U);
    }

    /* Release this channel's RAM window so a sibling may reuse the region. */
    r_mspi_ram_region_release(p_instance_ctrl);

    if (tx_enabled)
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_TRANSFER_COMPLETE);
    }
    else if (rx_enabled)
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_RECEIVE_COMPLETE);
    }
    else
    {
        /* Do nothing */
    }
}

/*******************************************************************************************************************//**
 * Returns the maximum number of user frames per hardware pass for the configured memory mode.
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static uint32_t r_mspi_max_chunk_frames (mspi_instance_ctrl_t * p_instance_ctrl)                           /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_extended_cfg_t const * p_extend = (mspi_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t mode = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;
    uint32_t max_chunk;
    uint8_t  half_stage = (uint8_t) (p_instance_ctrl->stage_size / MSPI_HALF_DIVISOR);

    if ((uint32_t) MSPI_TRANSFER_MODE_DIRECT_ACCESS == mode)
    {
        max_chunk = MSPI_DIRECT_MAX_FRAME_COUNT;
    }
    else if ((MSPI_TRANSFER_MODE_FIFO_MEMORY == mode) &&                                                       /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
             (NULL == p_instance_ctrl->p_cfg->p_transfer_tx) &&
             (NULL == p_instance_ctrl->p_cfg->p_transfer_rx))
    {
        R_MSPI0_MSPI0_CH_Type * p_mspi_regs     = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
        uint8_t                 channel         = p_instance_ctrl->p_cfg->channel;
        uint32_t                direction_count = 0U;

        /* Count the number of active communication directions. */
        if (MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            direction_count++;
        }

        if (MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            direction_count++;
        }

        /* Calculate the available RAM bytes for the current configuration. */
        uint32_t total_ram_bytes = BSP_FEATURE_MSPI_RAM_WORDS_MAX;
        uint32_t usable_bytes    = (p_extend->mspi_ram_start < total_ram_bytes) ?
                                   (total_ram_bytes - p_extend->mspi_ram_start) :
                                   0U;
        uint32_t stage_bytes = (uint32_t) p_instance_ctrl->stage_size *
                               r_mspi_count_byte(p_instance_ctrl->bitwidth) * /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
                               direction_count;

        /* Choose the appropriate maximum chunk size based on available RAM. */
        max_chunk = (stage_bytes <= usable_bytes) ?
                    MSPI_DIRECT_MAX_FRAME_COUNT :
                    r_mspi_ram_max_frames(p_instance_ctrl);
    }
    else
    {
        /* Buffer and FIFO are bound by the free RAM (width-aware, half in full-duplex). */
        max_chunk = r_mspi_ram_max_frames(p_instance_ctrl);
    }

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)

    /* Cap to the 16-bit number_transfer field. DMA counts frames (count 1); DTS counts words. */
    if ((NULL != p_instance_ctrl->p_cfg->p_transfer_tx) ||
        (NULL != p_instance_ctrl->p_cfg->p_transfer_rx))
    {
        uint32_t count = (MSPI_TRANSFER_TRIGGER_DMA == p_extend->mspi_transfer_trigger) ?
                         1U :
                         MSPI_WORD_COUNT(p_instance_ctrl->bitwidth); /* PRQA S 3469 # Advisory D-4.9 */
        uint32_t dma_max = MSPI_DIRECT_MAX_FRAME_COUNT / count;

        /* Reserve headroom for the FIFO refill units added to the chunk. */
        if (((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == mode) && (0U != half_stage))
        {
            dma_max = (dma_max > half_stage) ? (dma_max - half_stage) : 1U;
        }

        if (max_chunk > dma_max)
        {
            max_chunk = dma_max;
        }
    }
#endif

    /* FIFO: align to whole half-stage bursts. */
    if (((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == mode) && (0U != half_stage))
    {
        max_chunk = (max_chunk / half_stage) * half_stage;
        if (0U == max_chunk)
        {
            max_chunk = half_stage;
        }
    }

    if (0U == max_chunk)
    {
        max_chunk = 1U;
    }

    return max_chunk;
}

/*******************************************************************************************************************//**
 * Shared continuation/completion handler.
 *
 * @param[in]  p_instance_ctrl  pointer to control structure.
 * @param[in]  ack_tx           this call acknowledges the TX direction of the in-flight chunk.
 * @param[in]  ack_rx           this call acknowledges the RX direction of the in-flight chunk.
 **********************************************************************************************************************/
static void r_mspi_continue_or_complete (mspi_instance_ctrl_t * p_instance_ctrl, bool ack_tx, bool ack_rx)
{
    mspi_extended_cfg_t const * p_extend    = (mspi_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    R_MSPI0_MSPI0_CH_Type     * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH;    /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    uint8_t  channel   = p_instance_ctrl->p_cfg->channel;
    uint32_t mode      = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;
    uint8_t  num_count = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);                                /* PRQA S 3469 # Advisory D-4.9 */

    /* Buffer mode RX handled by frame-end-triggered DMA (no CPU read, no per-frame ack). */
    bool buffer_dma_rx = ((uint32_t) MSPI_TRANSFER_MODE_BUFFER_MEMORY == mode) &&
                         (NULL != p_instance_ctrl->p_cfg->p_transfer_rx);

    bool dts_fifo = ((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == mode) &&
                    (MSPI_TRANSFER_TRIGGER_DMA != p_extend->mspi_transfer_trigger) &&
                    ((NULL != p_instance_ctrl->p_cfg->p_transfer_tx) ||
                     (NULL != p_instance_ctrl->p_cfg->p_transfer_rx));

    /* Acknowledge the direction(s) reported by this caller. */
    if (ack_tx)
    {
        p_instance_ctrl->chunk_tx_pending = 0U;
    }

    if (ack_rx)
    {
        p_instance_ctrl->chunk_rx_pending = 0U;
    }

    /* Full-duplex: the chunk is done only when both directions have acknowledged. */
    if ((0U != p_instance_ctrl->chunk_tx_pending) || (0U != p_instance_ctrl->chunk_rx_pending))
    {
        return;
    }

    /* Buffer mode RX copy by CPU only. With DMA-RX the data is already copied by the DMA. */
    if (((uint32_t) MSPI_TRANSFER_MODE_BUFFER_MEMORY == mode) && (!buffer_dma_rx))
    {
        if (((uint32_t) MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE) &&
            (NULL != p_instance_ctrl->p_rx_data))
        {
            r_mspi_buffer_read(p_instance_ctrl);
        }
    }

    /* Account this callback: DTS+FIFO one-shot completes the whole length; other paths one chunk. */
    if (dts_fifo)
    {
        p_instance_ctrl->frames_done = p_instance_ctrl->length;
    }
    else
    {
        p_instance_ctrl->frames_done += p_instance_ctrl->chunk_frames;
    }

    /* All requested frames done -> report completion. */
    if (p_instance_ctrl->frames_done >= p_instance_ctrl->length)
    {
        /* Sync counters so the length check in notify_if_complete passes. */
        p_instance_ctrl->tx_count = p_instance_ctrl->length * num_count;
        p_instance_ctrl->rx_count = p_instance_ctrl->length * num_count;

        r_mspi_notify_if_complete(p_instance_ctrl);

        return;
    }

    /* Arm the next chunk. */
    uint32_t remaining = p_instance_ctrl->length - p_instance_ctrl->frames_done;
    uint32_t max_chunk = r_mspi_max_chunk_frames(p_instance_ctrl);
    uint32_t next      = (remaining > max_chunk) ? max_chunk : remaining;

    p_instance_ctrl->chunk_frames    = next;
    p_instance_ctrl->chunk_read_done = 0U;

    uint32_t bytes_per_frame = r_mspi_count_byte(p_instance_ctrl->bitwidth);          /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */

    /* Buffer counters are frame-based (index into user buffer); Direct/FIFO are in register-write units. */
    uint8_t count_unit = (MSPI_TRANSFER_MODE_BUFFER_MEMORY == mode) ? 1U : num_count; /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    p_instance_ctrl->tx_count = p_instance_ctrl->frames_done * count_unit;
    p_instance_ctrl->rx_count = p_instance_ctrl->frames_done * count_unit;

    /* Stop the channel before loading the frame count of the next chunk.*/
    p_mspi_regs[channel].CSTC_b.CHENC = R_MSPI0_MSPI0_CH_CSTC_CHENC_Msk; /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */

    FSP_HARDWARE_REGISTER_WAIT(p_mspi_regs[channel].CSTR_b.CHEN, 0U);

    uint32_t refill = r_mspi_fifo_refill_stage(p_instance_ctrl);
    p_mspi_regs[channel].CFSET = (next + refill); /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */

    /* Re-arm the pending flags for the new chunk. */
    if ((uint32_t) MSPI_TRANSFER_MODE_BUFFER_MEMORY == mode)
    {
        /* Buffer TX is CPU-synchronous */
        p_instance_ctrl->chunk_tx_pending = 0U;
        p_instance_ctrl->chunk_rx_pending = (buffer_dma_rx) ? 1U : 0U;
    }
    else
    {
        p_instance_ctrl->chunk_tx_pending =
            (MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE) ? 1U : 0U; /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        p_instance_ctrl->chunk_rx_pending =
            (MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE) ? 1U : 0U;    /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    }

    /* Buffer mode TX: stage the next chunk into MSPI RAM (CPU). */
    if (MSPI_TRANSFER_MODE_BUFFER_MEMORY == mode)                                    /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        if (((uint32_t) MSPI_TRANSMISSION_ENABLE == p_mspi_regs[channel].CFG0_b.TXE) &&
            (NULL != p_instance_ctrl->p_tx_data))
        {
            r_mspi_buffer_write(p_instance_ctrl);
        }

        /* Re-arm the frame-end-triggered RX DMA for the next chunk (RAM->buffer). */
#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
        if (buffer_dma_rx)
        {
            r_mspi_buffer_dma_rx_config(p_instance_ctrl);
        }
#endif

        /* Keep the frame-end interrupt enabled to drive the next chunk. */
        p_mspi_regs[channel].CFG0_b.IFEE = MSPI_INTERRUPT_OUT_ENABLE; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    }

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)
    const transfer_instance_t * p_transfer_tx = p_instance_ctrl->p_cfg->p_transfer_tx;
    const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;
    uint32_t byte_offset = p_instance_ctrl->frames_done * bytes_per_frame;

    /* Reprogram DMA/DTS for the next chunk (Direct/FIFO only; Buffer RX DMA handled above). */
    if ((uint32_t) MSPI_TRANSFER_MODE_BUFFER_MEMORY != mode)
    {
        bool      is_dma = (MSPI_TRANSFER_TRIGGER_DMA == p_extend->mspi_transfer_trigger);
        fsp_err_t err    = FSP_SUCCESS;

        /* DTS: half-stage block per request; DMA: whole chunk (frame count). */
        uint32_t block_units = (uint32_t) (p_instance_ctrl->stage_size / MSPI_HALF_DIVISOR) * num_count; /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */
        uint32_t total_units = remaining * num_count;
        uint32_t dma_units   = (next + refill);

        if (NULL != p_transfer_tx)
        {
            if (NULL != p_instance_ctrl->p_tx_data)
            {
                p_transfer_tx->p_cfg->p_info->p_src =
                    (void const *) &((uint8_t const *) p_instance_ctrl->p_tx_data)[byte_offset]; /* PRQA S 0316 # Advisory R-11.5 */
            }

            if (is_dma)
            {
                p_transfer_tx->p_cfg->p_info->number_transfer = (uint32_t) (dma_units * num_count);
            }
            else
            {
                p_transfer_tx->p_cfg->p_info->number_transfer             = block_units;
                p_transfer_tx->p_cfg->p_info->reload_number_transfer      = (uint16_t) block_units;
                p_transfer_tx->p_cfg->p_info->number_transfer_count_match = (uint16_t) total_units;
                p_transfer_tx->p_cfg->p_info->addr_reload_count           = (uint16_t) block_units;
                p_transfer_tx->p_cfg->p_info->reload_addr_reload_count    = (uint16_t) block_units;
            }

            err = p_transfer_tx->p_api->reconfigure(p_transfer_tx->p_ctrl, p_transfer_tx->p_cfg->p_info);
            if (FSP_SUCCESS == err)
            {
                err = p_transfer_tx->p_api->enable(p_transfer_tx->p_ctrl);
            }
        }

        if ((FSP_SUCCESS == err) && (NULL != p_transfer_rx))
        {
            if (NULL != p_instance_ctrl->p_rx_data)
            {
                p_transfer_rx->p_cfg->p_info->p_dest =
                    (void *) &((uint8_t *) p_instance_ctrl->p_rx_data)[byte_offset]; /* PRQA S 0316 # Advisory R-11.5 */
            }

            if (is_dma)
            {
                p_transfer_rx->p_cfg->p_info->number_transfer = (uint32_t) (dma_units * num_count);
            }
            else
            {
                p_transfer_rx->p_cfg->p_info->number_transfer             = block_units;
                p_transfer_rx->p_cfg->p_info->reload_number_transfer      = (uint16_t) block_units;
                p_transfer_rx->p_cfg->p_info->number_transfer_count_match = (uint16_t) total_units;
                p_transfer_rx->p_cfg->p_info->addr_reload_count           = (uint16_t) block_units;
                p_transfer_rx->p_cfg->p_info->reload_addr_reload_count    = (uint16_t) block_units;
            }

            err = p_transfer_rx->p_api->reconfigure(p_transfer_rx->p_ctrl, p_transfer_rx->p_cfg->p_info);
            if (FSP_SUCCESS == err)
            {
                err = p_transfer_rx->p_api->enable(p_transfer_rx->p_ctrl);
            }
        }

        if (FSP_SUCCESS != err)
        {
            r_mspi_ram_region_release(p_instance_ctrl);
            r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_ERR_MODE_FAULT);

            return;
        }
    }
#endif

    /* Configure interrupt settings for the MSPI channel */
    {
        bool need_tx = false;
        bool need_rx = false;
        r_mspi_shared_line_need(p_instance_ctrl, &need_tx, &need_rx);

        if (need_tx &&
            (!r_mspi_irq_already_active(&g_mspi_unit_ctrl[p_extend->unit], p_instance_ctrl,
                                        p_instance_ctrl->p_cfg->txi_irq)))
        {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
            MSPI_ENABLE_IRQ(p_extend->com_ored_int,
                            p_instance_ctrl->p_cfg->txi_irq,
                            p_instance_ctrl->p_cfg->txi_ipl,
                            &g_mspi_unit_ctrl[p_extend->unit]);
#else
            R_BSP_IrqCfgEnable(p_instance_ctrl->p_cfg->txi_irq, p_instance_ctrl->p_cfg->txi_ipl,
                               &g_mspi_unit_ctrl[p_extend->unit]);
#endif
        }
    }

    /* Re-trigger the channel for the NEW chunk (start of a fresh CFSET transaction). */
    if (((uint32_t) MSPI_TRANSFER_MODE_DIRECT_ACCESS == mode) && (0U != p_mspi_regs[channel].CFG4_b.HWTS))
    {
        /* Direct mode under a hardware trigger: channel enable only. */
        p_mspi_regs[channel].CSTS = R_MSPI0_MSPI0_CH_CSTS_CHENS_Msk; /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    }
    else
    {
        /* Software start (Buffer/FIFO and non-HW-triggered Direct): enable + activate. */
        p_mspi_regs[channel].CSTS = (R_MSPI0_MSPI0_CH_CSTS_CHENS_Msk | R_MSPI0_MSPI0_CH_CSTS_ACTFS_Msk); /* PRQA S 4464 # DP_MSR_R-10.3_UC1 */
    }
}

/*******************************************************************************************************************//**
 * Returns the user frames that fit the free MSPI RAM for the current data width (contiguous free region from RASTAD,
 * bounded by the unit RAM end and by any actively-transferring sibling; halved in full-duplex).
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static uint32_t r_mspi_ram_max_frames (mspi_instance_ctrl_t * p_instance_ctrl)
{
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
    uint8_t                 channel     = p_instance_ctrl->p_cfg->channel;

    /* Free contiguous bytes from ram_start, bounded by the unit RAM end AND by any active sibling. */
    uint32_t usable_bytes    = r_mspi_ram_free_bytes(p_instance_ctrl);
    uint32_t bytes_per_frame = r_mspi_count_byte(p_instance_ctrl->bitwidth); /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    uint32_t max_frames      = usable_bytes / bytes_per_frame;

    /* Full-duplex: TX and RX share the RAM -> half per direction. */
    if ((1U == p_mspi_regs[channel].CFG0_b.TXE) && (1U == p_mspi_regs[channel].CFG0_b.RXE))
    {
        max_frames = max_frames / MSPI_HALF_DIVISOR;
    }

    if (0U == max_frames)
    {
        max_frames = 1U;
    }

    return max_frames;
}

#if (MSPI_TRANSFER_ENABLE == MSPI_TRANSFER_SUPPORT_ENABLE)

/*******************************************************************************************************************//**
 * Buffer mode + DMA RX: configure the RX DMA as a RAM-to-buffer memory copy driven by the frame-end trigger.
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_mspi_buffer_dma_rx_config (mspi_instance_ctrl_t const * p_instance_ctrl)
{
    const transfer_instance_t * p_transfer_rx = p_instance_ctrl->p_cfg->p_transfer_rx;
    R_MSPI0_MSPI0_CH_Type     * p_mspi_regs   = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312, 3678 # DP_MSR_R-11.8_UC1, Advisory R-8.13 */
    uint8_t channel = p_instance_ctrl->p_cfg->channel;

    /* Sink for discarded RX data when the caller provides no receive buffer. */
    static uint32_t dummy_rx_data;

    uintptr_t rastad    = p_mspi_regs[channel].RASTAD;
    uintptr_t ram_addr  = (uintptr_t) p_instance_ctrl->p_regs + MSPI_INIT_RAM_ADDR + rastad;
    uint32_t  bytes     = r_mspi_count_byte(p_instance_ctrl->bitwidth); /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    uint32_t  chunk     = p_instance_ctrl->chunk_frames;
    uint32_t  num_count = MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);   /* PRQA S 3469 # Advisory D-4.9 */

    /* RX region follows the TX region (+4-byte alignment) in full-duplex. */
    uint32_t rx_off = 0U;
    if ((1U == p_mspi_regs[channel].CFG0_b.RXE) && (1U == p_mspi_regs[channel].CFG0_b.TXE))
    {
        rx_off = (uint32_t) r_mspi_align_ram(chunk, (uint32_t) ram_addr, p_instance_ctrl->bitwidth) + chunk; /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    }

    uintptr_t       src   = ram_addr + (rx_off * bytes);
    transfer_size_t tsize = r_mspi_cal_transfer_size(p_instance_ctrl->bitwidth);                             /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */

    /* Source is the RX region of MSPI RAM (increments across the chunk). */
    p_transfer_rx->p_cfg->p_info->p_src = (void const *) src;

    p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_addr_mode  = TRANSFER_ADDR_MODE_INCREMENTED; /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
    p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_trans_size = tsize;                          /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
    p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_trans_size = tsize;                          /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */

    p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.src_addr_direction =                       /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                               TRANSFER_DTSC_ADDR_MODE_INCREMENTED;
    p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_mode =                            /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                          TRANSFER_DTSC_MODE_BLOCK_1;
    p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.transfer_data_size = tsize;                /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */

    if (NULL == p_instance_ctrl->p_rx_data)
    {
        p_transfer_rx->p_cfg->p_info->p_dest = (void *) &dummy_rx_data;
        p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode =            /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                        TRANSFER_ADDR_MODE_FIXED;
        p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.des_addr_direction = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                   TRANSFER_DTSC_ADDR_MODE_FIXED;
    }
    else
    {
        uintptr_t dst = (uintptr_t) p_instance_ctrl->p_rx_data + (p_instance_ctrl->frames_done * bytes);
        p_transfer_rx->p_cfg->p_info->p_dest = (void *) dst;
        p_transfer_rx->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode =            /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                        TRANSFER_ADDR_MODE_INCREMENTED;
        p_transfer_rx->p_cfg->p_info->transfer_control_cfg.transfer_control_b.des_addr_direction = /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
                                                                                                   TRANSFER_DTSC_ADDR_MODE_INCREMENTED;
    }

    /* Whole chunk moved on a single frame-end request. */
    p_transfer_rx->p_cfg->p_info->number_transfer               = (uint32_t) (chunk * num_count);
    p_transfer_rx->p_cfg->p_info->transfer_count_per_hw_request = chunk; /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
    p_transfer_rx->p_cfg->p_info->transfer_limit_per_hw_request = 0U;
    p_transfer_rx->p_cfg->p_info->transfer_pre_load             = 0U;

    (void) p_transfer_rx->p_api->reconfigure(p_transfer_rx->p_ctrl, p_transfer_rx->p_cfg->p_info);
    (void) p_transfer_rx->p_api->enable(p_transfer_rx->p_ctrl);
}

/*******************************************************************************************************************//**
 * Select bit position for register MSPITGDMAALT
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static uint8_t r_mspi_dma_alternative (mspi_instance_ctrl_t const * p_instance_ctrl)
{
    mspi_extended_cfg_t const * p_extend = (mspi_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    uint8_t unit                 = p_extend->unit;
    uint8_t channel              = p_instance_ctrl->p_cfg->channel;
    uint8_t dmaAlternative       = p_extend->mspi_dma_alternative;                                         /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    uint8_t dmaAlternativeResult = MSPI_TRANSFER_INV_CHANNEL;

    if (BSP_FEATURE_MSPI_ALT_DMA_MAX_UNIT > unit)
    {
        if (((uint32_t) MSPI_TRANSFER_ALTERNATIVE_ENABLE == dmaAlternative) &&
            (MSPI_TRANSFER_ALTERNATIVE_DISABLE != (BSP_FEATURE_MSPI_ALT_DMA_UNIT & ((uint32_t) 1U << channel)))) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            dmaAlternativeResult = (uint8_t) (((uint32_t) channel % BSP_FEATURE_MSPI_ALT_GAP_CHANNEL) -
                                              BSP_FEATURE_MSPI_ALT_PAIR_CHANNEL +
                                              (BSP_FEATURE_MSPI_ALT_PAIR_CHANNEL *
                                               ((uint32_t) channel / BSP_FEATURE_MSPI_ALT_GAP_CHANNEL)) +
                                              ((uint32_t) unit * BSP_FEATURE_MSPI_ALT_GAP_UNIT));
        }

        if (BSP_FEATURE_MSPI_ALT_DMA_MAX_BIT < dmaAlternativeResult)
        {
            dmaAlternativeResult = MSPI_TRANSFER_INV_CHANNEL;
        }
    }

 #if (BSP_FEATURE_DMAAS23 && BSP_FEATURE_DMAAS21) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
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
static uint8_t r_mspi_dts_alternative (mspi_instance_ctrl_t const * p_instance_ctrl)
{
    mspi_extended_cfg_t * p_extend             = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    uint8_t               unit                 = p_extend->unit;
    uint8_t               channel              = p_instance_ctrl->p_cfg->channel;
    uint8_t               dtsAlternative       = p_extend->mspi_dts_alternative;                           /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */
    uint8_t               dtsAlternativeResult = MSPI_TRANSFER_INV_CHANNEL;
    if (BSP_FEATURE_MSPI_ALT_DTS_MAX_UNIT > unit)
    {
        if (((uint32_t) MSPI_TRANSFER_ALTERNATIVE_ENABLE == dtsAlternative) &&
            (MSPI_TRANSFER_ALTERNATIVE_DISABLE != (BSP_FEATURE_MSPI_ALT_DTS_UNIT & ((uint32_t) 1U << channel)))) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            dtsAlternativeResult = (uint8_t) (BSP_FEATURE_MSPI_ALT_DTS_MAX_BIT -
                                              (((BSP_FEATURE_MSPI_ALT_DTS_MAX_UNIT - unit - 1U) *
                                                BSP_FEATURE_MSPI_ALT_GAP_CHANNEL) +
                                               (BSP_FEATURE_MSPI_ALT_MAX_CHANNEL - channel)));
        }

        if (BSP_FEATURE_MSPI_ALT_DTS_MAX_BIT < dtsAlternativeResult)
        {
            dtsAlternativeResult = MSPI_TRANSFER_INV_CHANNEL;
        }
    }

 #if BSP_FEATURE_DTSAS07
    if ((MSPI0_UNIT == unit) && (MSPI_CS_CHANNEL_6 == channel))
    {
        /* DTSAS06 is used for MSPI0 channel 6 */
        if (MSPI_TRANSFER_ALTERNATIVE_ENABLE == dtsAlternative) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            dtsAlternativeResult = BSP_FEATURE_DTSAS06;
        }
    }
    else if ((MSPI0_UNIT == unit) && (MSPI_CS_CHANNEL_7 == channel))
    {
        /* DTSAS07 is used for MSPI0 channel 7 */
        if (MSPI_TRANSFER_ALTERNATIVE_ENABLE == dtsAlternative) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            dtsAlternativeResult = BSP_FEATURE_DTSAS07;
        }
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
    if (SPI_BIT_WIDTH_8_BITS >= bitwidth) /* PRQA S 1880 # DP_MSR_R-10.4_UC1 */
    {
        return TRANSFER_SIZE_1_BYTE;
    }

    if ((uint32_t) SPI_BIT_WIDTH_16_BITS >= bitwidth)
    {
        return TRANSFER_SIZE_2_BYTE;
    }

    return TRANSFER_SIZE_4_BYTE;
}

/*******************************************************************************************************************//**
 * Transfer callback for transmit.
 **********************************************************************************************************************/
void mspi_tx_transfer_callback (transfer_callback_args_t * p_transfer_args)                                    /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_instance_ctrl_t             * p_instance_ctrl  = (mspi_instance_ctrl_t *) p_transfer_args->p_context; /* PRQA S 0316, 0311 # Advisory R-11.5, DP_MSR_R-11.8_UC1 */
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;                     /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    uint8_t channel = p_instance_ctrl->p_cfg->channel;

    p_mspi_intf_regs[MSPI_INTF_IDX_TX].INTFC = (uint32_t) ((uint32_t) MSPI_NTFC_CLEAR << channel);

    /* Advance by one completed chunk, then continue or complete via the shared path. */
    if (MSPI_COMPLETE_SRC_TXRX_XFER == p_instance_ctrl->completion_source)
    {
        r_mspi_continue_or_complete(p_instance_ctrl, true, false);
    }
}

/*******************************************************************************************************************//**
 * Transfer callback for receive.
 **********************************************************************************************************************/
void mspi_rx_transfer_callback (transfer_callback_args_t * p_transfer_args)                                    /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_instance_ctrl_t             * p_instance_ctrl  = (mspi_instance_ctrl_t *) p_transfer_args->p_context; /* PRQA S 0316, 0311 # Advisory R-11.5, DP_MSR_R-11.8_UC1 */
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;                     /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    uint8_t channel = p_instance_ctrl->p_cfg->channel;

    p_mspi_intf_regs[MSPI_INTF_IDX_RX].INTFC = (uint32_t) ((uint32_t) MSPI_NTFC_CLEAR << channel);

    /* Advance by one completed chunk, then continue or complete via the shared path. */
    if (MSPI_COMPLETE_SRC_TXRX_XFER == p_instance_ctrl->completion_source)
    {
        r_mspi_continue_or_complete(p_instance_ctrl, false, true);
    }
}

/*******************************************************************************************************************//**
 * Transfer callback for frame end.
 **********************************************************************************************************************/
void mspi_fe_transfer_callback (transfer_callback_args_t * p_transfer_args)                                    /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_instance_ctrl_t             * p_instance_ctrl  = (mspi_instance_ctrl_t *) p_transfer_args->p_context; /* PRQA S 0316, 0311 # Advisory R-11.5, DP_MSR_R-11.8_UC1 */
    R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs =
        (R_MSPI0_INTF_MSPI0_INTF_CMM_Type *) p_instance_ctrl->p_regs_intf->MSPI0_INTF_CMM;                     /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    uint8_t channel = p_instance_ctrl->p_cfg->channel;

    /* Clear the frame-end DMA/DTS trigger flag */
    p_mspi_intf_regs[MSPI_INTF_IDX_FE].INTFC = (uint32_t) ((uint32_t) MSPI_NTFC_CLEAR << channel);

    /* Frame end => both TX and RX of the chunk are done. */
    if (MSPI_COMPLETE_SRC_FE_XFER == p_instance_ctrl->completion_source)
    {
        /* Frame end => both TX and RX of the chunk are done. */
        r_mspi_continue_or_complete(p_instance_ctrl, true, true);
    }
}

/* PRQA S 1505 -- */

#endif

/* PRQA S 1503 ++ # DP_MSR_R-2.1_UC1 */

/*******************************************************************************************************************//**
 * Handles all latched error flags for ONE channel.
 *
 * The over-run recovery clears MSPInEN, a UNIT-LEVEL bit, which stops every channel on the unit; the channels that
 * were actually transferring are reported and their RAM windows released before the unit is re-enabled.
 *
 * @param[in]  p_instance_ctrl    control block of the channel whose error flags are set.
 **********************************************************************************************************************/
static void r_mspi_error_handle (mspi_instance_ctrl_t * p_instance_ctrl)                                 /* PRQA S 3673 # Advisory R-8.13 */
{
    mspi_extended_cfg_t   * p_extend      = (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;    /* PRQA S 0311, 0316, 3678 # DP_MSR_R-11.8_UC1, Advisory R-11.5, Advisory R-8.13 */
    R_MSPI0_MSPI0_CH_Type * p_mspi_regs   = (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->MSPI0_CH; /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
    uint8_t                 channel       = p_instance_ctrl->p_cfg->channel;
    uint8_t                 unit          = p_extend->unit;
    uint32_t                transfer_mode = p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;
    uint32_t                rx_count      = 0U;
    uint8_t                 remain_data   = 0U;

    /* [RULE-2] Read the error status register ONCE and accumulate all clears into a single mask. */
    uint32_t cest        = p_mspi_regs[channel].CEST;
    uint32_t cestc_clear = 0U;

    /* Over-run error: recover per the hardware manual (set MSPInEN = 0, read received data, re-enable). */
    if (0U != (cest & R_MSPI0_MSPI0_CH_CEST_OVRUE_Msk))
    {
        /* Snapshot the number of unread RX words before touching the hardware. */
        remain_data = p_mspi_regs[channel].CSTR_b.FIRXN;

        /* Hardware manual: set MSPInEN = 0 on overrun before reading the received data.
         * MSPInEN is UNIT-LEVEL, so this also stops every other channel on the unit. */
        p_instance_ctrl->p_regs->CTL0_b.EN = 0U;

        if ((uint32_t) MSPI_TRANSFER_MODE_DIRECT_ACCESS == transfer_mode)
        {
            /* Direct memory mode: read MSPInRXDAm0 until MSPInRXRQFm = 0. */
            while (0U != p_mspi_regs[channel].CSTR_b.RXRQF)
            {
                r_mspi_read_register(p_instance_ctrl);
            }
        }
        else if ((uint32_t) MSPI_TRANSFER_MODE_FIFO_MEMORY == transfer_mode)
        {
            /* Fixed FIFO memory mode: read exactly the unread reception data reported by FIRXN. */
            for (rx_count = 0U; rx_count < (uint32_t) remain_data; rx_count++)
            {
                r_mspi_read_register(p_instance_ctrl);
            }
        }
        else
        {
            /* Buffer memory mode: no FIFO drain required. */
        }

        /* The overrun aborts only the channels that were transferring (ram_active_mask); an open-idle channel is
         * not affected. Notify those channels and release their RAM windows. */
        {
            mspi_unit_ctrl_t * p_unit  = &g_mspi_unit_ctrl[unit]; /* PRQA S 3678 # Advisory R-8.13 */
            uint32_t           pending = p_unit->ram_active_mask;

            /* Visit only the actively-transferring channels (bit-search over the active mask). */
            while (0U != pending)
            {
                uint8_t notify_channel = MSPI_FIRST_CHANNEL_BIT(pending); /* PRQA S 3469 # Advisory D-4.9 */

                /* Clear the bit now so the loop always advances. */
                pending &= ~((uint32_t) 1U << notify_channel);

                if (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > notify_channel)
                {
                    mspi_instance_ctrl_t * p_notify_ctrl = p_unit->p_ctrl[notify_channel];

                    if (NULL != p_notify_ctrl)
                    {
                        /* The transfer is aborted: mark it reported, release its RAM window, then notify. */
                        p_notify_ctrl->transfer_notified = 1U;
                        r_mspi_ram_region_release(p_notify_ctrl);
                        r_mspi_call_callback(p_notify_ctrl, SPI_EVENT_ERR_OVERRUN);
                    }
                }
            }
        }

        /* Four or more PCLK cycles plus four or more MSPInCLK cycles are required after MSPInEN is cleared
         * before it may be set again. Computed as the ceiling (in us) of 4 PCLK + 4 MSPInCLK cycles. */
        {
            uint32_t delay_us =
                (uint32_t) (((MSPI_MINIMUM_CYCLE_INIT * MSPI_US_PER_SEC) + ((uint32_t) BSP_CFG_CLOCK_CLK_HSB_HZ - 1U)) / /* PRQA S 1840 # DP_MSR_R-10.4_UC1 */
                            BSP_CFG_CLOCK_CLK_HSB_HZ) +
                (uint32_t) (((MSPI_MINIMUM_CYCLE_INIT * MSPI_US_PER_SEC) +
                             ((uint32_t) BSP_CFG_CLOCK_CLK_MSPI_HZ - 1U)) /                                              /* PRQA S 1840 # DP_MSR_R-10.4_UC1 */
                            BSP_CFG_CLOCK_CLK_MSPI_HZ);

            if (0U == delay_us)
            {
                delay_us = MSPI_MIN_DELAY_US;
            }

            R_BSP_SoftwareDelay(delay_us, BSP_DELAY_UNITS_MICROSECONDS);
        }

        /* Re-enable the MSPI unit and the affected channel. */
        p_instance_ctrl->p_regs->CTL0_b.EN = 1U;
        p_mspi_regs[channel].CSTS          = R_MSPI0_MSPI0_CH_CSTS_CHENS_Msk; /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */

        /* Re-enable the data reception */
        p_mspi_regs[channel].CFG0_b.IRXE = MSPI_INTERRUPT_OUT_ENABLE;         /* PRQA S 4424 # DP_MSR_R-10.3_UC1 */

        /* Defer the clear: accumulate into the mask. */
        cestc_clear |= R_MSPI0_MSPI0_CH_CESTC_OVRUEC_Msk;
    }

    /* Over-read error */
    if (0U != (cest & R_MSPI0_MSPI0_CH_CEST_OVREE_Msk))
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_ERR_READ_OVERFLOW);
        cestc_clear |= R_MSPI0_MSPI0_CH_CESTC_OVREEC_Msk;
    }

    /* Over-write error */
    if (0U != (cest & R_MSPI0_MSPI0_CH_CEST_OVWRE_Msk))
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_FIFO_OVERFLOW);
        cestc_clear |= R_MSPI0_MSPI0_CH_CESTC_OVWREC_Msk;
    }

    /* Data consistency error */
    if (0U != (cest & R_MSPI0_MSPI0_CH_CEST_DCE_Msk))
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_DATA_CONSISTENCY);
        cestc_clear |= R_MSPI0_MSPI0_CH_CESTC_DCEC_Msk;
    }

    /* Communication (CRC) error (only in transmit-and-receive mode and transmit-only mode) */
    if (0U != (cest & R_MSPI0_MSPI0_CH_CEST_CE_Msk))
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_CRC_ERROR);
        cestc_clear |= R_MSPI0_MSPI0_CH_CESTC_CEC_Msk;
    }

    /* Parity error (only in transmit-and-receive mode and transmit-only mode) */
    if (0U != (cest & R_MSPI0_MSPI0_CH_CEST_PE_Msk))
    {
        r_mspi_call_callback(p_instance_ctrl, SPI_EVENT_ERR_PARITY);
        cestc_clear |= R_MSPI0_MSPI0_CH_CESTC_PEC_Msk;
    }

    /* All callbacks done: clear every latched error flag in a single write. */
    p_mspi_regs[channel].CESTC = cestc_clear; /* PRQA S 4461 # DP_MSR_R-10.3_UC1 */
}

/*******************************************************************************************************************//**
 * ISR called when data is loaded into MSPI data register from the shift register.
 *
 * Services each channel whose TX flag is set and not masked. The 'irq == txi_irq' guard isolates dedicated
 * per-channel IRQs from a shared line.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mspi_txi_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
    IRQn_Type actual_irq = R_FSP_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    IRQn_Type actual_irq = irq;
#endif

    R_BSP_IrqStatusClear(actual_irq);

    mspi_unit_ctrl_t * p_unit = (mspi_unit_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

    if ((NULL != p_unit) && (0U != p_unit->channel_status) && (NULL != p_unit->p_regs_intf))
    {
        uint32_t notified = p_unit->p_regs_intf[MSPI_INTF_IDX_TX].INTF;
        uint32_t masked   = p_unit->p_regs_intf[MSPI_INTF_IDX_TX].INTMSK;

        /* Effective pending = notified (INTF) AND unmasked (INTMSK bit cleared). */
        uint32_t pending = notified & ~masked;

        /* Visit only the set bits (bit-search). */
        while (0U != pending)
        {
            uint8_t channel = MSPI_FIRST_CHANNEL_BIT(pending); /* PRQA S 3469 # Advisory D-4.9 */

            /* Clear the bit now so every path below still advances the loop. */
            pending &= ~((uint32_t) 1U << channel);

            if (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > channel)
            {
                mspi_instance_ctrl_t * p_instance_ctrl = p_unit->p_ctrl[channel];

                /* Only service channels routed to the interrupt currently being handled (isolates dedicated IRQs). */
                if ((NULL != p_instance_ctrl) && (actual_irq == p_instance_ctrl->p_cfg->txi_irq))
                {
                    r_mspi_transmit(p_instance_ctrl);
                }
            }
        }
    }

    /* Restore RTOS context */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR called when MSPI finished to receive data and can be read data from register.
 * Dispatch identical to the TXI ISR, guarded by 'irq == rxi_irq'.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mspi_rxi_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
    IRQn_Type actual_irq = R_FSP_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    IRQn_Type actual_irq = irq;
#endif

    R_BSP_IrqStatusClear(actual_irq);

    mspi_unit_ctrl_t * p_unit = (mspi_unit_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

    if ((NULL != p_unit) && (0U != p_unit->channel_status) && (NULL != p_unit->p_regs_intf))
    {
        uint32_t notified = p_unit->p_regs_intf[MSPI_INTF_IDX_RX].INTF;
        uint32_t masked   = p_unit->p_regs_intf[MSPI_INTF_IDX_RX].INTMSK;

        /* Effective pending = notified (INTF) AND unmasked (INTMSK bit cleared). */
        uint32_t pending = notified & ~masked;

        /* Visit only the set bits (bit-search). */
        while (0U != pending)
        {
            uint8_t channel = MSPI_FIRST_CHANNEL_BIT(pending); /* PRQA S 3469 # Advisory D-4.9 */

            /* Clear the bit now so every path below still advances the loop. */
            pending &= ~((uint32_t) 1U << channel);

            if (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > channel)
            {
                mspi_instance_ctrl_t * p_instance_ctrl = p_unit->p_ctrl[channel];

                if ((NULL != p_instance_ctrl) && (actual_irq == p_instance_ctrl->p_cfg->rxi_irq))
                {
                    r_mspi_receive(p_instance_ctrl);
                }
            }
        }
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR called error transmit/receive occur.
 * Dispatches each pending channel (routed to this interrupt) to r_mspi_error_handle(), guarded by 'irq == eri_irq'.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mspi_err_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
    IRQn_Type actual_irq = R_FSP_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    IRQn_Type actual_irq = irq;
#endif

    R_BSP_IrqStatusClear(actual_irq);

    mspi_unit_ctrl_t * p_unit = (mspi_unit_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

    if ((NULL != p_unit) && (0U != p_unit->channel_status) && (NULL != p_unit->p_regs_intf))
    {
        R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs = p_unit->p_regs_intf;

        uint32_t notified = p_mspi_intf_regs[MSPI_INTF_IDX_ERR].INTF;
        uint32_t masked   = p_mspi_intf_regs[MSPI_INTF_IDX_ERR].INTMSK;

        /* Effective pending = notified (INTF) AND unmasked (INTMSK bit cleared). */
        uint32_t pending = notified & ~masked;

        /* Visit only the set bits (bit-search). */
        while (0U != pending)
        {
            uint8_t channel = MSPI_FIRST_CHANNEL_BIT(pending); /* PRQA S 3469 # Advisory D-4.9 */

            /* Clear the bit now so every path below still advances the loop. */
            pending &= ~((uint32_t) 1U << channel);

            if (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > channel)
            {
                mspi_instance_ctrl_t * p_instance_ctrl = p_unit->p_ctrl[channel];

                if ((NULL != p_instance_ctrl) && (actual_irq == p_instance_ctrl->p_cfg->eri_irq))
                {
                    /* Clear the error notification flag for this channel (write-1-to-clear). */
                    p_mspi_intf_regs[MSPI_INTF_IDX_ERR].INTFC = (uint32_t) (MSPI_CHANNEL_STATUS_BIT << channel); /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */

                    r_mspi_error_handle(p_instance_ctrl);
                }
            }
        }
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ISR called when frame count is returned to zero.
 * Dispatches each pending channel (routed to this interrupt). Does NOT disable any shared TX line; per-channel TX
 * gating is via ITXE.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mspi_fe_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
    IRQn_Type actual_irq = R_FSP_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    IRQn_Type actual_irq = irq;
#endif

    R_BSP_IrqStatusClear(actual_irq);

    mspi_unit_ctrl_t * p_unit = (mspi_unit_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */

    if ((NULL != p_unit) && (0U != p_unit->channel_status) && (NULL != p_unit->p_regs_intf))
    {
        R_MSPI0_INTF_MSPI0_INTF_CMM_Type * p_mspi_intf_regs = p_unit->p_regs_intf;

        uint32_t notified = p_mspi_intf_regs[MSPI_INTF_IDX_FE].INTF;
        uint32_t masked   = p_mspi_intf_regs[MSPI_INTF_IDX_FE].INTMSK;

        /* Effective pending = notified (INTF) AND unmasked (INTMSK bit cleared). */
        uint32_t pending = notified & ~masked;

        /* Visit only the set bits (bit-search). */
        while (0U != pending)
        {
            uint8_t channel = MSPI_FIRST_CHANNEL_BIT(pending); /* PRQA S 3469 # Advisory D-4.9 */

            /* Clear the bit now so every path below still advances the loop. */
            pending &= ~((uint32_t) 1U << channel);

            if (BSP_FEATURE_MSPI_MAX_NUMBER_CHIP_SELECT > channel)
            {
                mspi_instance_ctrl_t * p_instance_ctrl = p_unit->p_ctrl[channel];

                if ((NULL != p_instance_ctrl) && (actual_irq == p_instance_ctrl->p_cfg->tei_irq))
                {
                    /* Clear this channel's frame-end status. */
                    p_mspi_intf_regs[MSPI_INTF_IDX_FE].INTFC = (uint32_t) (MSPI_CHANNEL_STATUS_BIT << channel); /* PRQA S 4391 # DP_MSR_R-10.8_UC1 */

                    /* Advance only when the frame-end ISR is the completion source (Non-DMA / CPU read).
                     * When RX DMA is used, frame-end hardware-triggers the RX DMA and its transfer
                     * callback drives the continuation, so the FE ISR must not advance here. */
                    if (MSPI_COMPLETE_SRC_FE_ISR == p_instance_ctrl->completion_source)
                    {
                        /* Frame-end may precede the final RXI: read out any RX still held by the hardware
                         * so the tail data is not dropped (Direct via RXRQF, FIFO via FIRXN). */
                        {
                            mspi_extended_cfg_t * p_extend =                                                           /* PRQA S 3678 # Advisory R-8.13 */
                                                             (mspi_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1, Advisory R-11.5 */
                            R_MSPI0_MSPI0_CH_Type * p_mspi_regs =                                                      /* PRQA S 3678 # Advisory R-8.13 */
                                                                  (R_MSPI0_MSPI0_CH_Type *) p_instance_ctrl->p_regs->  /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
                                                                  MSPI0_CH;                                            /* PRQA S 0312 # DP_MSR_R-11.8_UC1 */
                            uint32_t mode =
                                p_extend->p_mspi_config_cfg0->mspi_chip_select_cfg0.mspi_transfer_mode;

                            if (MSPI_RECEPTION_ENABLE == p_mspi_regs[channel].CFG0_b.RXE)                              /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
                            {
                                uint8_t num_count = (uint8_t) MSPI_WORD_COUNT(p_instance_ctrl->bitwidth);              /* PRQA S 3469 # Advisory D-4.9 */

                                if (MSPI_TRANSFER_MODE_DIRECT_ACCESS == mode)                                          /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
                                {
                                    uint32_t expected = p_instance_ctrl->length * num_count;

                                    /* Read while the hardware still reports a pending frame. */
                                    while ((p_instance_ctrl->rx_count < expected) &&
                                           (0U != p_mspi_regs[channel].CSTR_b.RXRQF))
                                    {
                                        r_mspi_direct_read(p_instance_ctrl);
                                    }
                                }
                                else if (MSPI_TRANSFER_MODE_FIFO_MEMORY == mode) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
                                {
                                    uint8_t  refill      = r_mspi_fifo_refill_stage(p_instance_ctrl);
                                    uint32_t total_reads = (p_instance_ctrl->chunk_frames + refill) * num_count;

                                    /* r_mspi_fifo_read is FIRXN-guarded, so it stops as soon as the FIFO is empty. */
                                    while ((p_instance_ctrl->chunk_read_done < total_reads) &&
                                           (0U != p_mspi_regs[channel].CSTR_b.FIRXN))
                                    {
                                        r_mspi_fifo_read(p_instance_ctrl);
                                    }
                                }
                                else
                                {
                                    /* Buffer memory mode: RX copy handled in continue_or_complete. */
                                }
                            }
                        }

                        r_mspi_continue_or_complete(p_instance_ctrl, true, true);
                    }
                }
            }
        }
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/* End of file R_MSPI. */

/* PRQA S 0662 -- */
/* PRQA S 1503 -- */
