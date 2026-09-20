/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <string.h>
#include "r_mmca.h"                    /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#if MMCA_CFG_DMAC_ENABLE
 #include "r_dmac.h"
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "MMCA" in ASCII, used to determine if channel is open */
#define MMCA_PRV_OPEN                                   (0x4D4D4341U)

/* Peripheral Clocks */
#define MMCA_PRV_CLK_CTRL_EN_MASK                       (0x01000000U)
#define MMCA_PRV_CLK_POWER_UP_EMMC                      (0x01082000U)

#define MMCA_PRV_CLEAR_REG                              (0x00000000U)
#define MMCA_PRV_INT_EN_CMD_MASK                        (0x0001CF1FU)
#define MMCA_PRV_INT_EN_BSY_MASK                        (0x0002CF1FU)
#define MMCA_PRV_INT_EN_READ_MASK                       (0x0050CF1FU)
#define MMCA_PRV_INT_EN_WRITE_MASK                      (0x00A0CF1FU)
#define MMCA_PRV_BUSY_TIMEOUT                           (2000000U)
#define MMCA_PRV_POWER_UP_REPEAT                        (10U)
#define MMCA_PRV_RCA_STUFF_BITS                         (0x0003FFFFU)
#define MMCA_PRV_OCR_ARGUMENT                           (0x40FF8080UL)

#define MMCA_BLOCK_WRITE_PREPARE_DELAY_MS               (100U)                                           /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_HIGH_SPEED_SET                             (0x03B90100U)

/* Commands name                                                         Commands idx    Response */
#define MMCA_PRV_CMD_GO_IDLE_STATE                      (0x00000000U)                                    // CMD0            Response None
#define MMCA_PRV_CMD_SEND_OP_COND                       (0x01405000U)                                    // CMD1            Response R3
#define MMCA_PRV_CMD_ALL_SEND_CID                       (0x02806000U)                                    // CMD2            Response R2
#define MMCA_PRV_CMD_SEND_RELATIVE_ADDR                 (0x03400000U)                                    // CMD3            Response R1
#define MMCA_PRV_CMD_SWITCH_BKG_OP                      (0x06400000U)                                    // CMD6            Response R1
#define MMCA_PRV_CMD_SWITCH_RES_BSY                     (0x06600000U)                                    // CMD6            Response R1b
#define MMCA_PRV_CMD_SEL_DES_CARD                       (0x07400000U)                                    // CMD7            Response R1
#define MMCA_PRV_CMD_SEL_DES_CARD_RES_BSY               (0x07600000U) /* PRQA S 1534 # Advisory R-2.5 */ // CMD7            Response R1b
#define MMCA_PRV_CMD_SEND_EXT_CSD                       (0x08480000U)                                    // CMD8            Response R1
#define MMCA_PRV_CMD_SEND_CSD                           (0x09806000U)                                    // CMD9            Response R2
#define MMCA_PRV_CMD_STOP_TRANSMISSION                  (0x0C400000U) /* PRQA S 1534 # Advisory R-2.5 */ // CMD12           Response R1
#define MMCA_PRV_CMD_SEND_STATUS                        (0x0D400000U)                                    // CMD13           Response R1
#define MMCA_PRV_CMD_SET_BLOCKLEN                       (0x10400000U)                                    // CMD16           Response R1
#define MMCA_PRV_CMD_READ_SINGLE_BLOCK                  (0x11480000U)                                    // CMD17           Response R1
#define MMCA_PRV_CMD_READ_MULTIPLE_BLOCK_PRE_DEF        (0x124A0000U)                                    // CMD18           Response R1
#define MMCA_PRV_CMD_SET_BLOCK_COUNT                    (0x17400000U)                                    // CMD23
#define MMCA_PRV_CMD_WRITE_SINGLE_BLOCK                 (0x184C0000U)                                    // CMD24           Response R1
#define MMCA_PRV_CMD_WRITE_MULTIPLE_BLOCK_PRE_DEF       (0x194E0000U)                                    // CMD25           Response R1
#define MMCA_PRV_CMD_TAG_ERASE_GROUP_START              (0x23400000U)                                    // CMD35           Response R1
#define MMCA_PRV_CMD_TAG_ERASE_GROUP_END                (0x24400000U)                                    // CMD36           Response R1
#define MMCA_PRV_CMD_ERASE                              (0x26600000U)                                    // CMD38           Response R1b

/* Offsets */
#define MMCA_PRV_EMMC_EXT_CSD_HS_TIMING_OFFSET          (185U)                                           /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_EMMC_EXT_CSD_DEVICE_TYPE_OFFSET        (196U)                                           /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_EMMC_EXT_CSD_HPI_MGMT_OFFSET           (161U)                                           /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_EMMC_EXT_CSD_HPI_FEATURES_OFFSET       (503U)                                           /* PRQA S 1534 # Advisory R-2.5 */

/* Bit shifting */
#define MMCA_BYTE_SHIFT_24                              (24U)
#define MMCA_BYTE_SHIFT_16                              (16U)
#define MMCA_BYTE_SHIFT_8                               (8U)
#define MMCA_BYTE_SHIFT_6                               (6U)
#define MMCA_BYTE_SHIFT_3                               (3U)
#define MMCA_BYTE_SHIFT_2                               (2U) /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_BYTE_SHIFT_1                               (1U) /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_CMD23_RELIABLE_WRITE_FLAG                  (0x80000000UL)

/* Bit mask constants for field extraction */
#define MMCA_MASK_1BIT                                  (0x1U)
#define MMCA_MASK_2BITS                                 (0x3U)
#define MMCA_MASK_3BITS                                 (0x7U)
#define MMCA_MASK_4BITS                                 (0xFU)
#define MMCA_MASK_5BITS                                 (0x1FU)
#define MMCA_MASK_6BITS                                 (0x3FU)       /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_MASK_7BITS                                 (0x7FU)       /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_MASK_8BITS                                 (0xFFU)
#define MMCA_MASK_10BITS                                (0x3FFU)
#define MMCA_MASK_12BITS                                (0xFFFU)
#define MMCA_MASK_16BITS                                (0xFFFFU)     /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_MASK_32BITS                                (0xFFFFFFFFU) /* PRQA S 1534 # Advisory R-2.5 */

/* HPI feature bits */
#define MMCA_PRV_HPI_SUPPORT_BIT                        (1U << 0)     /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_HPI_IMPLEMENTATION_BIT                 (1U << 1)     /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_HPI_EN_BIT                             (1U << 0)     /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_HPI_TIMEOUT                            (10000U)      /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_R1_CURRENT_STATE_MASK                  (0x00001E00U)
#define MMCA_PRV_R1_CURRENT_STATE_SHIFT                 (9U)
#define MMCA_PRV_CARD_STATE_PRG                         (7U)          /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_CARD_STATE_TRAN                        (4U)
#define MMCA_PRV_CARD_STATE_INVALID                     (99U)
#define MMCA_PRV_EMMC_EXT_CSD_ARGUMENT_SET_MODE         (3U << 0)     /* PRQA S 1534 # Advisory R-2.5 */

/* Background Operation feature bit */
#define MMCA_PRV_BKOPS_SUPPORT_BIT                      (1U << 0)
#define MMCA_PRV_BKOPS_STATUS_BIT                       (3U << 0)
#define MMCA_PRV_EMMC_EXT_CSD_BKOPS_EN_OFFSET           (163U)
#define MMCA_PRV_EMMC_EXT_CSD_BKOPS_STATUS_OFFSET       (246U)
#define MMCA_PRV_EMMC_EXT_CSD_BKOPS_FEATURES_OFFSET     (502U)
#define MMCA_PRV_BKOPS_TIMEOUT                          (2U)
#define MMCA_PRV_BKOPS_TIMEUP                           (0U)

/* Set BKOPS manual mode, Access mode = 0x03000000, A4 = BKOPS_START Offset extended CSD, 1 = Value non-zero */
#define MMCA_PRV_BKOPS_MANUAL_START_ARGUMENT            (0x03A40100U)

/* Set BKOPS enable mode, Access mode = 0x03000000, A3 = BKOPS_EN Offset extended CSD, 1 = Enable */
#define MMCA_PRV_BKOPS_AUTO_ENABLE_ARGUMENT             (0x03A30100U)

/* Set BKOPS enable mode, Access mode = 0x03000000, A3 = BKOPS_EN Offset extended CSD, 0 = Disable */
#define MMCA_PRV_BKOPS_AUTO_DISABLE_ARGUMENT            (0x03A30000U)

/* Boot-related EXT_CSD offsets */
#define MMCA_PRV_EMMC_EXT_CSD_BOOT_BUS_WIDTH_OFFSET     (177U)
#define MMCA_PRV_EMMC_EXT_CSD_BOOT_CONFIG_OFFSET        (179U)
#define MMCA_PRV_EMMC_EXT_CSD_BOOT_SIZE_MULTI_OFFSET    (226U)
#define MMCA_PRV_BOOT_ACK_BIT                           (0x40U)
#define MMCA_PRV_BOOT_PARTITION_ENABLE                  (0x38U)
#define MMCA_PRV_BOOT_PARTITION_ACCESS                  (0x07U)
#define MMCA_PRV_BOOT_128KB_SIZE                        (128U * 1024U)

/* Configure CLK */
#define MMCA_PRV_EMMC_SWITCH_ACCESS_WRITE_BYTE          (3U)

#define MMCA_PRV_EMMC_ERASE_ARGUMENT_TRIM               (1U)
#define MMCA_PRV_EMMC_SWITCH_ARG_WRITE_BYTE(idx, val) /* PRQA S 3472 # Advisory D-4.9 */ \
    ((((uint32_t) MMCA_PRV_EMMC_SWITCH_ACCESS_WRITE_BYTE) << 24U) |                      \
     (((uint32_t) (idx)) << 16U) | (((uint32_t) (val)) << 8U))

/* Set data bus width/data line in MMCA communication */
#define MMCA_PRV_BUS_WIDTH_ARG_SET_1_BIT                (0x03B70000UL) /* PRQA S 1534 # Advisory R-2.5 */
#define MMCA_PRV_BUS_WIDTH_ARG_SET_4_BIT                (0x03B70100UL)
#define MMCA_PRV_BUS_WIDTH_ARG_SET_8_BIT                (0x03B70200UL)

/* Device sector count */
#define MMCA_EXT_CSD_SEC_COUNT_0                        (212U)
#define MMCA_EXT_CSD_SEC_COUNT_1                        (213U)
#define MMCA_EXT_CSD_SEC_COUNT_2                        (214U)
#define MMCA_EXT_CSD_SEC_COUNT_3                        (215U)

#define MMCA_PRV_BUF_ACC_DISABLE                        (0x00000000U) /* PRQA S 1534 # Advisory R-2.5 */

/* Set multi-block settings */
#define MMCA_PRV_MULTI_BLOCK_SETTING                    (2U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void      r_mmca_command_send_no_wait(mmca_instance_ctrl_t const * p_ctrl, uint32_t command, uint32_t argument);
static fsp_err_t r_mmca_command_send(mmca_instance_ctrl_t * p_ctrl, uint32_t command, uint32_t argument);
static fsp_err_t r_mmca_wait_for_event(mmca_instance_ctrl_t * const p_ctrl, mmca_event_t event);

static fsp_err_t r_mmca_transfer_write(mmca_instance_ctrl_t * const p_ctrl,
                                       uint32_t                     start_sector,
                                       uint32_t                     block_num,
                                       uint8_t const * const        p_source);

static fsp_err_t r_mmca_transfer_read(mmca_instance_ctrl_t * const p_ctrl,
                                      uint32_t                     start_sector,
                                      uint32_t                     block_num,
                                      uint8_t * const              p_dest);

static fsp_err_t r_mmca_single_block_write(mmca_instance_ctrl_t * const p_ctrl,
                                           uint32_t                     start_sector,
                                           uint8_t const * const        p_source);

static fsp_err_t r_mmca_multi_block_write(mmca_instance_ctrl_t * const p_ctrl,
                                          uint32_t                     start_sector,
                                          uint32_t                     block_num,
                                          uint8_t const * const        p_source);

static fsp_err_t r_mmca_single_block_read(mmca_instance_ctrl_t * const p_ctrl,
                                          uint32_t                     start_sector,
                                          uint8_t * const              p_dest);

static fsp_err_t r_mmca_multi_block_read(mmca_instance_ctrl_t * const p_ctrl,
                                         uint32_t                     start_sector,
                                         uint32_t                     block_num,
                                         uint8_t * const              p_dest);

static fsp_err_t r_mmca_high_speed_set(mmca_instance_ctrl_t * p_ctrl);
static fsp_err_t r_mmca_check_bkops_support(mmca_instance_ctrl_t * const p_ctrl);
static uint8_t   r_mmca_get_card_state(mmca_instance_ctrl_t * const p_ctrl);
static fsp_err_t r_mmca_csd_extended_get(mmca_instance_ctrl_t * const p_ctrl, uint8_t * extcsd_out);

#if MMCA_CFG_NON_BLOCK_ENABLE
static void      r_mmca_write_one_block(mmca_instance_ctrl_t * const p_ctrl);
static fsp_err_t r_mmca_enable_hpi(mmca_instance_ctrl_t * const p_ctrl);
static fsp_err_t r_mmca_check_hpi_support(mmca_instance_ctrl_t * const p_ctrl);

#endif

/* Bus initialize functions */
static fsp_err_t r_mmca_csd_get(mmca_instance_ctrl_t * const p_ctrl, mmc_response_t * p_device_type);
static fsp_err_t r_mmca_hw_cfg(mmca_instance_ctrl_t const * const p_ctrl);
static fsp_err_t r_mmca_card_power_up(mmca_instance_ctrl_t * const p_ctrl);
static fsp_err_t r_mmca_bus_cfg(mmca_instance_ctrl_t * const p_ctrl);
static fsp_err_t r_mmca_bus_width_set(mmca_instance_ctrl_t * const p_ctrl);
static fsp_err_t r_mmca_common_error_check(mmca_instance_ctrl_t const * const p_ctrl);

static fsp_err_t r_mmca_set_block_length(uint16_t block_length, mmca_instance_ctrl_t * const p_ctrl);
static void      r_mmca_call_callback(mmca_instance_ctrl_t const * p_ctrl, mmca_event_t event);

static fsp_err_t r_mmca_read_boot_info(mmca_ctrl_t * const p_api_ctrl, mmca_boot_info_t * p_boot_info);
static fsp_err_t r_mmca_extcsd_write_byte(mmca_instance_ctrl_t * const p_ctrl, uint8_t offset, uint8_t value);

#if MMCA_CFG_DMAC_ENABLE
static fsp_err_t r_mmca_dependent_drivers_configure(mmca_cfg_t const * const     p_cfg,
                                                    mmca_instance_ctrl_t * const p_instance_ctrl);
void mmca_txi_transfer_callback(transfer_callback_args_t * p_transfer_args);
void mmca_rxi_transfer_callback(transfer_callback_args_t * p_transfer_args);

#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* MMCA function pointers   */
const mmca_api_t g_mmca_on_mmca =                                 /* PRQA S 1502 # Advisory R-2.8 */
{
    .open                     = R_MMCA_Open,                      /* PRQA S 3635 # Advisory R-17.12 */
    .mediaInit                = R_MMCA_MediaInit,                 /* PRQA S 3635 # Advisory R-17.12 */
    .read                     = R_MMCA_Read,                      /* PRQA S 3635 # Advisory R-17.12 */
    .write                    = R_MMCA_Write,                     /* PRQA S 3635 # Advisory R-17.12 */
    .close                    = R_MMCA_Close,                     /* PRQA S 3635 # Advisory R-17.12 */
    .erase                    = R_MMCA_Erase,                     /* PRQA S 3635 # Advisory R-17.12 */
    .statusGet                = R_MMCA_Status,                    /* PRQA S 3635 # Advisory R-17.12 */
    .callbackSet              = R_MMCA_CallbackSet,               /* PRQA S 3635 # Advisory R-17.12 */
    .hpiTrigger               = R_MMCA_HPI_Trigger,               /* PRQA S 3635 # Advisory R-17.12 */
    .writeAsync               = R_MMCA_WriteAsync,                /* PRQA S 3635 # Advisory R-17.12 */
    .eraseAsync               = R_MMCA_EraseAsync,                /* PRQA S 3635 # Advisory R-17.12 */
    .bkopsManual              = R_MMCA_BackgroundManual,          /* PRQA S 3635 # Advisory R-17.12 */
    .bkopsAutoSet             = R_MMCA_BackgroundAutoSet,         /* PRQA S 3635 # Advisory R-17.12 */
    .bkopsStatusGet           = R_MMCA_BackgroundStatusGet,       /* PRQA S 3635 # Advisory R-17.12 */
    .bootInfo                 = R_MMCA_Boot_Info,                 /* PRQA S 3635 # Advisory R-17.12 */
    .bootConfigure            = R_MMCA_Boot_Configure,            /* PRQA S 3635 # Advisory R-17.12 */
    .bootPartitionAccessBegin = R_MMCA_Boot_PartitionAccessBegin, /* PRQA S 3635 # Advisory R-17.12 */
    .bootPartitionAccessEnd   = R_MMCA_Boot_PartitionAccessEnd,   /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup MMCA
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Opens the driver.  Resets MMCA.
 * @ref R_MMCA_MediaInit must be called after this function before any other functions can be used.
 *
 * Implements @ref mmca_api_t::open().
 *
 * @retval  FSP_SUCCESS                    Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION              A required pointer was NULL.
 * @retval  FSP_ERR_INVALID_CHANNEL        The requested channel does not exist on this MCU
 * @retval  FSP_ERR_ALREADY_OPEN           Control block has already been opened or channel is being used by another
 *                                         instance. Call close() then open() to reconfigure.
 *
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Open (mmca_ctrl_t * const p_api_ctrl, mmca_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Initialization */
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;               /* PRQA S 0316 # Advisory R-11.5 */

    fsp_err_t err = FSP_SUCCESS;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_cfg);
    FSP_ASSERT(p_cfg->p_callback != NULL);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ERROR_RETURN(p_cfg->channel < 1U, FSP_ERR_INVALID_CHANNEL);
#endif

    p_ctrl->p_reg      = R_MMCA0;      /* PRQA S 0306 # Advisory R-11.4 */
    p_ctrl->p_cfg      = p_cfg;
    p_ctrl->mmca_event = MMCA_EVENT_INITIALIZATION;

    /* Initialize instance variables */
    (void) memset(&p_ctrl->csd_infor, 0, sizeof(p_ctrl->csd_infor));
    (void) memset(&p_ctrl->response, 0, sizeof(p_ctrl->response));
    p_ctrl->p_mmca_tx_src           = NULL;
    p_ctrl->mmca_tx_bytes_remaining = 0U;
    p_ctrl->mmca_blocks_remaining   = 0U;
    p_ctrl->mmca_block_size         = MMCA_MAX_BLOCK_SIZE;
    p_ctrl->writing_nonblock_flag   = false;
    p_ctrl->erasing_nonblock_flag   = false;
    p_ctrl->erase_start_sector      = 0U;
    p_ctrl->erase_sector_count      = 0U;
    p_ctrl->dma_write_in_progress   = false;
    p_ctrl->dma_read_in_progress    = false;

    /* Reset stale interrupt flags */
    p_ctrl->p_reg->CE_INT = 0U;               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Software Reset MMCA. */
    p_ctrl->p_reg->CE_SWRESA_b.SWRST = 0x01U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_SWRESA_b.SWRST = 0x00U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Set callback and context pointers, if configured */
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    /* Device eMMC is not initialized */
    p_ctrl->initialized_mmc = false;

    if ((FSP_INVALID_VECTOR != p_ctrl->p_cfg->mmc_response_irq) &&
        (BSP_IRQ_DISABLED != p_ctrl->p_cfg->mmc_response_ipl))
    {
        R_BSP_IrqCfgEnable(p_ctrl->p_cfg->mmc_response_irq, p_ctrl->p_cfg->mmc_response_ipl, p_ctrl);
    }

#if MMCA_CFG_DMAC_ENABLE

    /* Configure dependent timer and transfer drivers. */
    err = r_mmca_dependent_drivers_configure(p_cfg, p_ctrl);
    FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
#endif

    /* Set Open status of MMCA interface module */
    p_ctrl->open = MMCA_PRV_OPEN;

    return err;
}

/*******************************************************************************************************************//**
 * Closes an open MMC device.  Implements @ref mmca_api_t::close().
 *
 * @retval     FSP_SUCCESS          Successful close.
 * @retval     FSP_ERR_ASSERTION    A required pointer was NULL.
 * @retval     FSP_ERR_DEVICE_BUSY  Device is holding DAT0 low (device is busy) or another operation is ongoing.
 * @retval     FSP_ERR_NOT_OPEN     Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Close (mmca_ctrl_t * const p_api_ctrl)                  /* PRQA S 1505 # Advisory R-8.7 */
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    uint32_t command = MMCA_PRV_CMD_GO_IDLE_STATE;

    if (p_ctrl->initialized_mmc)
    {
        /* Device eMMC is not initialized */
        p_ctrl->initialized_mmc = false;

        /* Put the card in idle state - CMD0 */
        err = r_mmca_command_send(p_ctrl, command, 0);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Do not set the module stop bit since the CMD0 may not be complete yet. Do not wait for CMD0 to complete because
         * the card could be unplugged and waiting for the response timeout in this function is not desirable. */

        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Disable MMC transfer interrupts. */
    p_ctrl->p_reg->CE_CLK_CTRL = 0U;                                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_INT_EN   = 0U;                                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    R_BSP_IrqDisable(p_ctrl->p_cfg->mmc_response_irq);
    R_FSP_IsrContextSet(p_ctrl->p_cfg->mmc_response_irq, NULL);

#if MMCA_CFG_DMAC_ENABLE

    /* If transfer is used, disable transfer when stop is requested. */
    if (NULL != p_ctrl->p_cfg->p_transfer)
    {
        err = p_ctrl->p_cfg->p_transfer->p_api->close(p_ctrl->p_cfg->p_transfer->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Disable buffer access (DMA) in MMCA0CE_BUF_ACC */
        p_ctrl->p_reg->CE_BUF_ACC = MMCA_PRV_BUF_ACC_DISABLE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
#endif

    p_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Initializes the MMCA hardware and completes identification and configuration for the SD or eMMC device. This
 * procedure requires several sequential commands.  This function blocks until all identification and configuration
 * commands are complete.
 *
 * Follow the procedure in A.8.1 "Bus Initialization - Power-up" in JEDEC Standard No. 84-A441.
 *
 *
 * @retval     FSP_SUCCESS               Module is now ready for read/write access.
 * @retval     FSP_ERR_NOT_OPEN          Driver has not been initialized.
 * @retval     FSP_ERR_ASSERTION         A required pointer was NULL.
 * @retval     FSP_ERR_TIMEOUT           Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY       Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_MediaInit (mmca_ctrl_t * const p_api_ctrl, mmca_device_t * const p_device) /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Initialization */
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                    /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_device);
#endif

    /* Configure MMCA peripheral. */
    err = r_mmca_hw_cfg(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Perform the power-up procedure for eMMC device. */
    err = r_mmca_card_power_up(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Configure mmc device's bus clock, block size, and bus width. */
    err = r_mmca_bus_cfg(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Save sector size bytes of device */
    p_ctrl->device.sector_size_bytes = p_ctrl->p_cfg->block_size;

    /* eMMC device is ready */
    p_ctrl->initialized_mmc = true;

    /* Copy device information back to caller */
    *p_device = p_ctrl->device;

#if MMCA_CFG_NON_BLOCK_ENABLE

    /* Check and enable HPI if supported */
    err = r_mmca_check_hpi_support(p_ctrl);
    if ((FSP_SUCCESS == err) && p_ctrl->device.hpi_supported)
    {
        err = r_mmca_enable_hpi(p_ctrl);
    }
#endif
    p_ctrl->p_reg->CE_CLK_CTRL_b.CLKEN = 0U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return err;
}

/*******************************************************************************************************************//**
 * Reads data from eMMC device.  Up to 0xFFFF blocks can be read at a time. Implements @ref mmca_api_t::read().
 * In the system design, multi block read applies CMD 18 open-ended mode (with Automatic CMD12 Issuance )
 *
 * @note  Behavior depends on MMCA_CFG_DMAC_ENABLE:
 *        - MMCA_CFG_DMAC_ENABLE = 0: Blocking. Data fully transferred (CPU copy) on return.
 *        - MMCA_CFG_DMAC_ENABLE = 1: MULTI-block transfers are NON-BLOCKING. The function starts
 *          a DMA transfer and returns immediately (CPU is free). Completion is signaled via the
 *          user callback: MMCA_EVENT_BUFFER_READ_COMPLETE. The caller MUST NOT reuse/free the
 *          data buffer nor start a new transfer until completion is notified (or poll
 *          R_MMCA_Status()->transfer_in_progress). SINGLE-block transfers remain blocking
 *          regardless of this setting.
 *
 * @retval     FSP_SUCCESS                   Data read successfully.
 * @retval     FSP_ERR_ASSERTION             A required pointer was NULL.
 * @retval     FSP_ERR_NOT_OPEN              Driver has not been initialized.
 * @retval     FSP_ERR_CARD_NOT_INITIALIZED  Card was unplugged.
 * @retval     FSP_ERR_TIMEOUT               Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY           Driver is busy with a previous operation.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Read (mmca_ctrl_t * const p_api_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                       uint8_t * const     p_dest,
                       uint32_t const      start_sector,
                       uint32_t const      sector_count)
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_dest);
    FSP_ASSERT(sector_count <= (uint32_t) UINT16_MAX);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check if common error happens */
    err = r_mmca_common_error_check(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Configure the transfer interface for reading. */
    err = r_mmca_transfer_read(p_ctrl, start_sector, sector_count, p_dest);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

#if !MMCA_CFG_DMAC_ENABLE

    /* Wait until bus is free before returning */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_CLK_CTRL_b.CLKEN = 0U;                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Writes data to an eMMC device. Up to 0xFFFF blocks can be written at a time. Implements @ref mmca_api_t::write().
 *
 * @note  Behavior depends on MMCA_CFG_DMAC_ENABLE:
 *        - MMCA_CFG_DMAC_ENABLE = 0: Blocking. Data fully transferred (CPU copy) on return.
 *        - MMCA_CFG_DMAC_ENABLE = 1: MULTI-block transfers are NON-BLOCKING. The function starts
 *          a DMA transfer and returns immediately (CPU is free). Completion is signaled via the
 *          user callback: MMCA_EVENT_DATA_TRANSMISSION_COMPLETE. The caller MUST NOT reuse/free
 *          the data buffer nor start a new transfer until completion is notified (or poll
 *          R_MMCA_Status()->transfer_in_progress). SINGLE-block transfers remain blocking
 *          regardless of this setting.
 *
 * @retval     FSP_SUCCESS                   Card write finished successfully.
 * @retval     FSP_ERR_ASSERTION             Handle or Source address is NULL.
 * @retval     FSP_ERR_NOT_OPEN              Driver has not been initialized.
 * @retval     FSP_ERR_CARD_NOT_INITIALIZED  Card was unplugged.
 * @retval     FSP_ERR_TIMEOUT               Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY           Driver is busy with a previous operation.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Write (mmca_ctrl_t * const   p_api_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                        uint8_t const * const p_source,
                        uint32_t const        start_sector,
                        uint32_t const        sector_count)
{
    /* Call MMCA protocol write function */
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_source);
    FSP_ASSERT(sector_count <= (uint32_t) UINT16_MAX);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check if common error happens */
    err = r_mmca_common_error_check(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Configure the transfer interface for writing. */
    err = r_mmca_transfer_write(p_ctrl, start_sector, sector_count, p_source);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

#if !MMCA_CFG_DMAC_ENABLE

    /* Wait until bus is free before returning */
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_CLK_CTRL_b.CLKEN = 0U;                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Erases sectors of an eMMC device. Implements @ref mmca_api_t::erase().
 *
 * This function blocks until the erase command is sent. Poll the status to determine when erase is complete.
 *
 * @retval     FSP_SUCCESS                  Erase operation requested.
 * @retval     FSP_ERR_ASSERTION            A required pointer is NULL or an argument is invalid.
 * @retval     FSP_ERR_NOT_OPEN             Driver has not been initialized.
 * @retval     FSP_ERR_CARD_NOT_INITIALIZED Card was unplugged.
 * @retval     FSP_ERR_DEVICE_BUSY          Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Erase (mmca_ctrl_t * const p_api_ctrl, uint32_t const start_sector, uint32_t const sector_count) /* PRQA S 1505 # Advisory R-8.7 */
{
    /* Initialization */
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                                          /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
    uint32_t               start_address;
    uint32_t               end_address;
    uint32_t               argument = MMCA_PRV_EMMC_ERASE_ARGUMENT_TRIM;

#if MMCA_CFG_PARAM_CHECKING_ENABLE

    /* Check pointers for NULL values */
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(0U != sector_count);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check if module is not in commence sequence */
    err = r_mmca_common_error_check(p_ctrl);

    /* Check if common error happens */
    if (FSP_SUCCESS == err)
    {
        /* Check if access mode is sector mode or byte mode*/
        if (true == p_ctrl->sector_addressing)
        {
            start_address = start_sector;
            end_address   = ((start_address + sector_count) - 1U);
        }
        else
        {
            start_address = (start_sector * p_ctrl->p_cfg->block_size);
            end_address   = (start_address + (sector_count * p_ctrl->p_cfg->block_size)) - 1U;
        }

        /* start command for CMD35*/
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_TAG_ERASE_GROUP_START, start_address);

        /* Check if common error happens */
        if (FSP_SUCCESS == err)
        {
            /* end address command for CMD36 */
            err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_TAG_ERASE_GROUP_END, end_address);

            /* Check if common error happens */
            if (FSP_SUCCESS == err)
            {
                /* erase command for CMD38*/
                r_mmca_command_send_no_wait(p_ctrl, MMCA_PRV_CMD_ERASE, argument);
                (void) r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_COMMAND_RESPONSE_COMPLETE);
            }
        }
    }

    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_CLK_CTRL_b.CLKEN = 0U;                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return err;
}

/*******************************************************************************************************************//**
 * Provides driver status. Implements mmca_api_t::statusGet().
 *
 * @retval     FSP_SUCCESS  Status stored in p_status.
 * @retval     FSP_ERR_ASSERTION    A required pointer was NULL.
 * @retval     FSP_ERR_NOT_OPEN Driver has not been initialized.
 *
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Status (mmca_ctrl_t * const p_api_ctrl, mmca_status_t * const p_status) /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    mmca_instance_ctrl_t const * p_ctrl = (mmca_instance_ctrl_t const *) p_api_ctrl;     /* PRQA S 0316 # Advisory R-11.5 */

#if MMCA_CFG_PARAM_CHECKING_ENABLE

    /* Check pointers for NULL values */
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_status);

    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Whether or not the media is initialized. */
    p_status->initialized_mmc = p_ctrl->initialized_mmc;

    /* Check if the card is busy. */
    p_status->transfer_in_progress = (p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY != 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 * Implements @ref mmca_api_t::callbackSet.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_NO_CALLBACK_MEMORY   p_callback is non-secure and p_callback_memory is either secure or NULL.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_CallbackSet (mmca_ctrl_t * const          p_api_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                              void (                     * p_callback)(mmca_callback_args_t * p_args),
                              void const * const           p_context,
                              mmca_callback_args_t * const p_callback_memory)
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_callback != NULL);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Trigger High Priority Interrupt (HPI) to interrupt current eMMC operation.
 *
 * This function executes the HPI sequence to safely interrupt ongoing eMMC operations such as data
 * transfer or programming. HPI is useful for time-critical operations that need to preempt current
 * activities. The function handles both CMD13-based and CMD12-based HPI implementations, manages
 * write operation recovery, and updates progress tracking for interrupted operations.
 *
 * @param[in]  p_api_ctrl           Pointer to the control structure.
 *
 * @retval     FSP_SUCCESS          HPI successfully executed and card returned to TRANSFER state.
 * @retval     FSP_ERR_ASSERTION    Pointer to the control structure is NULL.
 * @retval     FSP_ERR_NOT_OPEN     Driver is not opened.
 * @retval     FSP_ERR_UNSUPPORTED  HPI feature is not enabled on this device.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond or failed to reach expected state.
 * @retval     FSP_ERR_RESPONSE     Device responded with an error during command sequence.
 *
 * @note       For write operations, function retrieves correctly programmed sectors count.
 * @note       Function cleans up global operation state and restores interrupt settings.
 * @note       HPI implementation type (CMD13 vs CMD12) is determined from device capabilities.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_HPI_Trigger (mmca_ctrl_t * const p_api_ctrl)            /* PRQA S 1505, 3673 # Advisory R-8.7 & Advisory R-8.13 */
{
#if MMCA_CFG_NON_BLOCK_ENABLE
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
    uint32_t               card_status;
    uint32_t               timeout;
    bool is_write_operation;

 #if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(p_ctrl->device.hpi_enabled, FSP_ERR_UNSUPPORTED);
 #endif

    /* Check if there's an active async operation */
    is_write_operation = p_ctrl->writing_nonblock_flag;

    p_ctrl->device.operation_interrupted = true;

    /* Disable interrupts to prevent ISR conflicts during HPI */
    uint32_t saved_int_en = p_ctrl->p_reg->CE_INT_EN;                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Terminate command sequence */
    p_ctrl->p_reg->CE_CMD_CTRL_b.BREAK = 1U;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_CMD_CTRL_b.BREAK = 0U;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Wait for command sequence to stop */
    timeout = MMCA_PRV_HPI_TIMEOUT;
    while ((p_ctrl->p_reg->CE_HOST_STS1_b.CMDSEQ != 0U) && (timeout > 0U)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
        timeout--;
    }

    if (0U == timeout)
    {
        /* Restore interrupts */
        p_ctrl->p_reg->CE_INT_EN = saved_int_en; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        return FSP_ERR_TIMEOUT;
    }

    /* Handle write-specific HPI procedure */
    if (is_write_operation)
    {
        /* Issue CMD12 to stop transmission */
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_STOP_TRANSMISSION, 0);
        if (FSP_SUCCESS != err)
        {
            /* Restore interrupts */
            p_ctrl->p_reg->CE_INT_EN = saved_int_en; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            return err;
        }

        /* Wait for card to reach PRG or TRAN state */
        timeout     = MMCA_PRV_HPI_TIMEOUT;
        card_status = r_mmca_get_card_state(p_ctrl);

        if (MMCA_PRV_CARD_STATE_PRG == card_status)
        {
            /* Wait for PRG -> TRAN transition */
            do
            {
                R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
                card_status = r_mmca_get_card_state(p_ctrl);
                timeout--;
            } while ((MMCA_PRV_CARD_STATE_TRAN != card_status) && (timeout > 0U));
        }

        if (0U == timeout)
        {
            /* Restore interrupts */
            p_ctrl->p_reg->CE_INT_EN = saved_int_en; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            return FSP_ERR_TIMEOUT;
        }
    }

    /* Execute actual HPI command */
    if (!p_ctrl->device.hpi_implementation)
    {
        /* CMD13-based HPI */
        uint32_t hpi_arg = MMCA_PRV_RCA_STUFF_BITS | (1U << 0);
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SEND_STATUS, hpi_arg);
    }
    else
    {
        /* CMD12-based HPI */
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_STOP_TRANSMISSION, MMCA_PRV_RCA_STUFF_BITS);
    }

    if (FSP_SUCCESS != err)
    {
        /* Restore interrupts */
        p_ctrl->p_reg->CE_INT_EN = saved_int_en; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        return err;
    }

    /* Wait for TRAN state */
    timeout = MMCA_PRV_HPI_TIMEOUT;
    do
    {
        card_status = r_mmca_get_card_state(p_ctrl);
        if (MMCA_PRV_CARD_STATE_TRAN == card_status)
        {
            break;
        }

        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
        timeout--;
    } while (0U < timeout);

    if (0U == timeout)
    {
        /* Restore interrupts */
        p_ctrl->p_reg->CE_INT_EN = saved_int_en; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        return FSP_ERR_TIMEOUT;
    }

    /* Verify DAT0 line is released */
    if (0U == (p_ctrl->p_reg->CE_HOST_STS1_b.DATSIG & 0x01U)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /* Restore interrupts */
        p_ctrl->p_reg->CE_INT_EN = saved_int_en;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        return FSP_ERR_TIMEOUT;
    }

    /* Clean up operation flags */
    if (is_write_operation)
    {
        p_ctrl->writing_nonblock_flag   = false;
        p_ctrl->p_mmca_tx_src           = NULL;
        p_ctrl->mmca_tx_bytes_remaining = 0U;
        p_ctrl->mmca_blocks_remaining   = 0U;
    }

    /* Restore interrupts */
    p_ctrl->p_reg->CE_INT_EN = saved_int_en; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * @brief Start async erase operation with interrupt handling
 *
 * Erases sectors of an eMMC device in non-blocking mode. This function initiates the erase operation
 * and returns immediately.
 *
 * @param[in] p_api_ctrl    Pointer to control structure.
 * @param[in] start_sector  Starting sector number for erase operation.
 * @param[in] sector_count  Number of sectors to erase (must be multiple of erase_sector_count).
 *
 * @retval FSP_SUCCESS                  Erase operation started successfully.
 * @retval FSP_ERR_ASSERTION            A required pointer is NULL or an argument is invalid.
 * @retval FSP_ERR_NOT_OPEN             Driver has not been initialized.
 * @retval FSP_ERR_DEVICE_BUSY          Device is holding DAT0 low or another operation is ongoing.
 * @retval FSP_ERR_INVALID_ARGUMENT     Invalid sector count or start sector.
 * @retval FSP_ERR_UNSUPPORTED          Non-blocking operation is not enabled.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_EraseAsync (mmca_ctrl_t * const p_api_ctrl, uint32_t const start_sector, uint32_t const sector_count) /* PRQA S 1505, 3673 # Advisory R-8.7 & Advisory R-8.13 */
{
#if MMCA_CFG_NON_BLOCK_ENABLE
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                                               /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
    uint32_t               start_address;
    uint32_t               end_address;
    uint32_t               argument = MMCA_PRV_EMMC_ERASE_ARGUMENT_TRIM;

 #if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(0U != sector_count);
 #endif

    /* Check if write operation is in progress */
    if (p_ctrl->writing_nonblock_flag)
    {
        return FSP_ERR_DEVICE_BUSY;
    }

    /* Check if module is not in commence sequence */
    err = r_mmca_common_error_check(p_ctrl);
    if (FSP_SUCCESS != err)
    {
        return err;
    }

    /* Setup async erase context */
    p_ctrl->erasing_nonblock_flag = true;
    p_ctrl->erase_start_sector    = start_sector;
    p_ctrl->erase_sector_count    = sector_count;

    /* Store operation details for HPI support */
    p_ctrl->current_operation_start_sector = start_sector;
    p_ctrl->current_operation_sector_count = sector_count;
    p_ctrl->current_operation_buffer       = NULL; /* No buffer for erase */
    p_ctrl->device.operation_interrupted   = false;

    /* Check if access mode is sector mode or byte mode */
    if (true == p_ctrl->sector_addressing)
    {
        start_address = start_sector;
        end_address   = ((start_address + sector_count) - 1U);
    }
    else
    {
        start_address = (start_sector * p_ctrl->p_cfg->block_size);
        end_address   = (start_address + (sector_count * p_ctrl->p_cfg->block_size)) - 1U;
    }

    do                                 /* PRQA S 0771 # Advisory R-15.4 */
    {
        /* CMD35: Set erase group start address */
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_TAG_ERASE_GROUP_START, start_address);
        if (FSP_SUCCESS != err)
        {
            break;
        }

        /* CMD36: Set erase group end address */
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_TAG_ERASE_GROUP_END, end_address);
        if (FSP_SUCCESS != err)
        {
            break;
        }

        /* CMD38: Start erase operation (non-blocking) */
        r_mmca_command_send_no_wait(p_ctrl, MMCA_PRV_CMD_ERASE, argument);
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Operation started successfully - ISR will handle completion */
        return FSP_SUCCESS;
    } while (0);

    if (FSP_SUCCESS != err)
    {
        p_ctrl->erasing_nonblock_flag = false;
        p_ctrl->erase_start_sector    = 0;
        p_ctrl->erase_sector_count    = 0;
    }

    return err;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(start_sector);
    FSP_PARAMETER_NOT_USED(sector_count);

    return FSP_ERR_UNSUPPORTED;
#endif                                                         /* MMCA_CFG_NON_BLOCK_ENABLE */
}

/*******************************************************************************************************************//**
 * Start asynchronous write operation to eMMC device with HPI support.
 *
 * This function initiates a non-blocking write operation that can be interrupted by HPI (High Priority
 * Interrupt). The write is performed using interrupt-driven data transfer with CPU handling. For single
 * sector writes, CMD24 is used; for multi-sector writes, CMD23+CMD25 sequence with reliable write is used.
 * The operation continues in the background via ISR, allowing the calling thread to perform other tasks.
 *
 * @param[in]  p_api_ctrl           Pointer to the control structure.
 * @param[in]  p_source             Pointer to the source data buffer to write.
 * @param[in]  start_sector         Starting logical sector address.
 * @param[in]  sector_count         Number of sectors to write (1 to max supported).
 *
 * @retval     FSP_SUCCESS          Async write operation successfully initiated.
 * @retval     FSP_ERR_ASSERTION    Control structure or source buffer pointer is NULL.
 * @retval     FSP_ERR_NOT_OPEN     Driver is not opened.
 * @retval     FSP_ERR_DEVICE_BUSY  Another write operation is already in progress.
 * @retval     FSP_ERR_INVALID_ARGUMENT Invalid source buffer or zero sector count.
 * @retval     FSP_ERR_RESPONSE     Device responded with an error to write commands.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond to write commands.
 * @retval     FSP_ERR_UNSUPPORTED  Non-blocking operation is not enabled.
 *
 * @note       Function returns immediately after command initiation; data transfer occurs via ISR.
 * @note       Multi-block writes use reliable write mode (CMD23 bit 31 = 1) for better error recovery.
 * @note       Operation can be interrupted using R_MMCA_HPI_Trigger() for time-critical tasks.
 * @note       Global state variables track operation progress for HPI support.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_WriteAsync (mmca_ctrl_t * const   p_api_ctrl, /* PRQA S 1505, 3673 # Advisory R-8.7 & Advisory R-8.13 */
                             uint8_t const * const p_source,
                             uint32_t const        start_sector,
                             uint32_t const        sector_count)
{
#if MMCA_CFG_NON_BLOCK_ENABLE
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;

 #if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_source);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    /* Step 1: Validation */

    /* Check if write operation is already in progress */
    if (NULL != p_ctrl->p_mmca_tx_src)
    {
        return FSP_ERR_DEVICE_BUSY;
    }

    /* Validate parameters */
    if ((NULL == p_source) || (0U == sector_count))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* Step 2: Setup Context */

    /* Setup async write context */
    p_ctrl->p_mmca_tx_src           = p_source;
    p_ctrl->mmca_block_size         = MMCA_MAX_BLOCK_SIZE; /* Standard block size for eMMC */
    p_ctrl->mmca_tx_bytes_remaining = sector_count * p_ctrl->mmca_block_size;
    p_ctrl->mmca_blocks_remaining   = sector_count;

    /* Set write flag */
    p_ctrl->writing_nonblock_flag = true;

    /* Store operation details for HPI */
    p_ctrl->current_operation_start_sector = start_sector;
    p_ctrl->current_operation_sector_count = sector_count;
    p_ctrl->current_operation_buffer       = p_source;
    p_ctrl->device.operation_interrupted   = false;

    /* Step 3: Hardware Configuration */
    /* Configure block settings */
    p_ctrl->p_reg->CE_BLOCK_SET_b.BLKCNT = (uint16_t) sector_count; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Set block size based on operation type */
    if (1U == sector_count)
    {
        /* Single block: flexible size (1-512 bytes) */
        p_ctrl->p_reg->CE_BLOCK_SET_b.BLKSIZ = (uint16_t) p_ctrl->mmca_block_size; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        /* Multi block: fixed 512 bytes */
        p_ctrl->p_reg->CE_BLOCK_SET_b.BLKSIZ = MMCA_MAX_BLOCK_SIZE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Disable DMA transfer for CPU-based handling */
    p_ctrl->p_reg->CE_BUF_ACC_b.DMAWEN = 0U;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 0U;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Step 4: Send Commands */

    if (1U == sector_count)
    {
        /* Single block write - CMD24: WRITE_SINGLE_BLOCK */
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_WRITE_SINGLE_BLOCK, start_sector);
    }
    else
    {
        /* Multi block write - CMD23: SET_BLOCK_COUNT + CMD25: WRITE_MULTIPLE_BLOCK */

        /* First send CMD23 to set reliable write (bit 31 = 1) */
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SET_BLOCK_COUNT, sector_count | MMCA_CMD23_RELIABLE_WRITE_FLAG);

        if (FSP_SUCCESS == err)
        {
            /* Then send CMD25 to start multi-block write */
            R_BSP_SoftwareDelay(MMCA_BLOCK_WRITE_PREPARE_DELAY_MS, BSP_DELAY_UNITS_MILLISECONDS);
            err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_WRITE_MULTIPLE_BLOCK_PRE_DEF, start_sector);
        }
    }

    /* Step 5: Enable Interrupts */

    if (FSP_SUCCESS == err)
    {
        /* Enable write-related interrupts - ISR will handle the data transfer */
        p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_WRITE_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* At this point, hardware should trigger BUFWEN interrupt soon */
    }
    else
    {
        /* Error cleanup */
        p_ctrl->p_mmca_tx_src           = NULL;
        p_ctrl->mmca_tx_bytes_remaining = 0;
        p_ctrl->mmca_blocks_remaining   = 0;
        p_ctrl->writing_nonblock_flag   = false;
    }

    return err;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(p_source);
    FSP_PARAMETER_NOT_USED(start_sector);
    FSP_PARAMETER_NOT_USED(sector_count);

    return FSP_ERR_UNSUPPORTED;
#endif                                                                   /* MMCA_CFG_NON_BLOCK_ENABLE */
}

/*******************************************************************************************************************//**
 * Manual start Background Operation. Implements mmca_api_t::bkopsmanual().
 *
 * This function initiates Background Operation manual.
 *
 * @param[in]  p_api_ctrl           Pointer to the control structure.
 *
 * @retval     FSP_SUCCESS          Status stored in p_status.
 * @retval     FSP_ERR_ASSERTION    A required pointer was NULL.
 * @retval     FSP_ERR_UNSUPPORTED  Selected mode not supported
 * @retval     FSP_ERR_NOT_OPEN     Driver has not been initialized.
 *
 * @note       Function returns immediately after command initiation.
 * @note       Function is used When Background Operation status is bigger than 1.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_BackgroundManual (mmca_ctrl_t * const p_api_ctrl)       /* PRQA S 1505 # Advisory R-8.7 */
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check Background Operation if supported */
    err = r_mmca_check_bkops_support(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    if (p_ctrl->device.bkops_supported)
    {
        /* Request manual BKOPS: write non-zero to BKOPS_START (164) */
        r_mmca_command_send_no_wait(p_ctrl, MMCA_PRV_CMD_SWITCH_BKG_OP, MMCA_PRV_BKOPS_MANUAL_START_ARGUMENT);
    }
    else
    {
        err = FSP_ERR_UNSUPPORTED;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Enables or disables automatic background operations. Implements mmca_api_t::bkopsauto().
 *
 * @param[in]  p_api_ctrl                Pointer to the control structure.
 * @param[in]  enable                    Enable or disable Auto.
 *
 * @retval     FSP_SUCCESS               Operation completed successfully.
 * @retval     FSP_ERR_ASSERTION         A required pointer was NULL.
 * @retval     FSP_ERR_UNSUPPORTED       Selected mode not supported
 * @retval     FSP_ERR_NOT_OPEN          Driver has not been initialized.
 * @retval     FSP_ERR_TIMEOUT           Device did not respond.
 *
 * @note       Function used to enable auto one time only.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_BackgroundAutoSet (mmca_ctrl_t * const p_api_ctrl, mmca_bkops_auto_enabled_t enable) /* PRQA S 1505 # Advisory R-8.7 */
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                              /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
    uint32_t               arg;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check Background Operation if supported */
    err = r_mmca_check_bkops_support(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    if (true == p_ctrl->device.bkops_supported)
    {
        /* Check if value is true */
        if (MMCA_BKOPS_AUTO_ENABLED == enable)
        {
            arg = MMCA_PRV_BKOPS_AUTO_ENABLE_ARGUMENT;
        }
        else
        {
            arg = MMCA_PRV_BKOPS_AUTO_DISABLE_ARGUMENT;
        }

        /* Request manual BKOPS: write enable or disable to BKOPS_EN (163) */
        err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SWITCH_BKG_OP, arg);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        err = FSP_ERR_UNSUPPORTED;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Get Background Operation Status. Implements mmca_api_t::bkopsstatusget().
 *
 * @param[in]  p_api_ctrl                Pointer to the control structure.
 * @param[out] p_status                  Status of Background Operation.
 *
 * @retval     FSP_SUCCESS               Operation completed successfully.
 * @retval     FSP_ERR_ASSERTION         A required pointer was NULL.
 * @retval     FSP_ERR_UNSUPPORTED       Selected mode not supported
 * @retval     FSP_ERR_NOT_OPEN          Driver has not been initialized.
 * @retval     FSP_ERR_TIMEOUT           Device did not respond mode TRAN.
 *
 * @note       Function get status of Background Operation from Extended CSD register.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_BackgroundStatusGet (mmca_ctrl_t * const p_api_ctrl, uint8_t * const p_status) /* PRQA S 1505 # Advisory R-8.7 */
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                        /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
    uint32_t               timeout;
    uint8_t                card_state;
    uint8_t                extcsd_buf[MMCA_MAX_BLOCK_SIZE];
#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_status);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Wait for device to transition to TRAN state with timeout */
    timeout = MMCA_PRV_BKOPS_TIMEOUT;
    do
    {
        card_state = r_mmca_get_card_state(p_ctrl);
        if ((uint8_t) MMCA_R1_STATE_TRAN == card_state)
        {
            break;
        }

        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
        timeout--;
    } while (MMCA_PRV_BKOPS_TIMEUP < timeout);

    /* Check if timeout and return error */
    if (MMCA_PRV_BKOPS_TIMEUP == timeout)
    {
        err = FSP_ERR_TIMEOUT;
    }
    else
    {
        /* Get Extended CSD Register */
        err = r_mmca_csd_extended_get(p_ctrl, extcsd_buf);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        *p_status = (uint8_t) (extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_BKOPS_STATUS_OFFSET] & MMCA_PRV_BKOPS_STATUS_BIT);
    }

    return err;
}

/*******************************************************************************************************************//**
 * Reads boot-related information from EXT_CSD. Implements @ref mmca_api_t::bootInfo().
 *
 * @param[in]     p_api_ctrl    Pointer to MMC instance control block.
 * @param[out]    p_boot_info   Pointer to boot information structure.
 *
 * @retval     FSP_SUCCESS            Boot information read successfully.
 * @retval     FSP_ERR_ASSERTION      A required pointer was NULL.
 * @retval     FSP_ERR_DEVICE_BUSY    Device is holding DAT0 low (device is busy) or another operation is ongoing.
 * @retval     FSP_ERR_NOT_OPEN       Driver has not been initialized.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Boot_Info (mmca_ctrl_t * const p_api_ctrl, mmca_boot_info_t * const p_boot_info) /* PRQA S 1505 # Advisory R-8.7 */
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                          /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_boot_info);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check if common error happens */
    err = r_mmca_common_error_check(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    err = r_mmca_read_boot_info(p_ctrl, p_boot_info);

    return err;
}

/*******************************************************************************************************************//**
 * Configures boot-related settings in EXT_CSD. Implements @ref mmca_api_t::bootConfigure().
 *
 * This function:
 *  - Writes BOOT_CONFIG (EXT_CSD[179]) and BOOT_BUS_WIDTH (EXT_CSD[177]) according to the configuration in
 *    p_boot_cfg.
 *  - Reads back the values to verify they were set correctly.
 *
 * The card must be idle (not busy) before calling this function.
 *
 * @param[in]     p_api_ctrl   Pointer to MMC instance control block.
 * @param[out]    p_boot_cfg   Pointer to boot configuration structure.
 *
 * @retval     FSP_SUCCESS                  Boot configuration successful.
 * @retval     FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval     FSP_ERR_INVALID_STATE        Read-back values do not match the requested configuration.
 * @retval     FSP_ERR_NOT_OPEN             Driver has not been initialized.
 * @retval     FSP_ERR_DEVICE_BUSY          Device is busy (DAT0 held low) or another operation is ongoing.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Boot_Configure (mmca_ctrl_t * const p_api_ctrl, mmca_boot_cfg_t const * const p_boot_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                                   /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_boot_cfg);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check if common error happens */
    err = r_mmca_common_error_check(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Compose values */
    uint8_t boot_cfg_val = (uint8_t) ((((p_boot_cfg->boot_ack != 0U) ? 1U : 0U) << MMCA_BYTE_SHIFT_6) |
                                      ((p_boot_cfg->boot_partition_enable & MMCA_MASK_3BITS) << MMCA_BYTE_SHIFT_3) |
                                      (p_boot_cfg->boot_partition_access & MMCA_MASK_3BITS));

    uint8_t busw_val = (uint8_t) (((p_boot_cfg->reset_boot_bus_width & MMCA_MASK_3BITS) << MMCA_BYTE_SHIFT_3) |
                                  (p_boot_cfg->boot_bus_width & MMCA_MASK_3BITS));

    /* Write BOOT_CONFIG */
    err = r_mmca_extcsd_write_byte(p_ctrl, MMCA_PRV_EMMC_EXT_CSD_BOOT_CONFIG_OFFSET, boot_cfg_val);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Write BOOT_BUS_WIDTH */
    err = r_mmca_extcsd_write_byte(p_ctrl, MMCA_PRV_EMMC_EXT_CSD_BOOT_BUS_WIDTH_OFFSET, busw_val);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Re-read and compare */
    mmca_boot_info_t read_info = {0};
    err = r_mmca_read_boot_info(p_ctrl, &read_info);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Compare boot fields setting. Note: some cards may mask unsupported bits. */
    bool read_back_value =
        (((read_info.boot_ack & MMCA_MASK_1BIT) == (uint8_t) ((p_boot_cfg->boot_ack != 0U) ? 1U : 0U)) &&
         ((read_info.boot_enab & MMCA_MASK_3BITS) == (p_boot_cfg->boot_partition_enable & MMCA_MASK_3BITS)) &&
         ((read_info.boot_acc & MMCA_MASK_3BITS) == (p_boot_cfg->boot_partition_access & MMCA_MASK_3BITS)) &&
         ((read_info.boot_width & MMCA_MASK_3BITS) == (p_boot_cfg->boot_bus_width & MMCA_MASK_3BITS)) &&
         ((read_info.reset_boot_width & MMCA_MASK_3BITS) == (p_boot_cfg->reset_boot_bus_width & MMCA_MASK_3BITS)));

    /* Check if read back value config is not completed */
    if (!read_back_value)
    {
        return FSP_ERR_INVALID_STATE;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Begin temporary access to a specific partition in Normal mode by updating PARTITION_ACCESS (EXT_CSD[179] bits[2:0]).
 *
 * This function:
 *  - Reads current BOOT_CONFIG (EXT_CSD[179]) to preserve the existing value.
 *  - Updates only PARTITION_ACCESS bits [2:0] with the requested partition (0..7).
 *  - Stores the original BOOT_CONFIG into p_guard for later restoration by R_MMCA_Boot_PartitionAccessEnd().
 *
 * The card must be idle (not busy) before calling this function.
 *
 * @param[in]  p_api_ctrl               Pointer to MMCA control block.
 * @param[in]  partition_access         Target PARTITION_ACCESS value (0..7). For example:
 *                                      0: User Area, 1: Boot Partition 1, 2: Boot Partition 2.
 * @param[out] p_guard                  Pointer to guard structure to store the original BOOT_CONFIG.
 *
 * @retval FSP_SUCCESS                  PARTITION_ACCESS updated successfully (or already set).
 * @retval FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval FSP_ERR_INVALID_ARGUMENT     partition_access > 7.
 * @retval FSP_ERR_NOT_OPEN             Driver has not been initialized.
 * @retval FSP_ERR_DEVICE_BUSY          Device is busy (DAT0 held low) or another operation is ongoing.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Boot_PartitionAccessBegin (mmca_ctrl_t * const              p_api_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                                            uint8_t                          partition_access,
                                            mmca_boot_access_guard_t * const p_guard)
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                 /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
    uint8_t                extcsd_buf[MMCA_MAX_BLOCK_SIZE];

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_guard);
    FSP_ERROR_RETURN((partition_access <= 0x7U), FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check if common error happens */
    err = r_mmca_common_error_check(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Read current EXT_CSD to obtain BOOT_CONFIG. */
    err = r_mmca_csd_extended_get(p_ctrl, extcsd_buf);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    uint8_t old_cfg = extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_BOOT_CONFIG_OFFSET];
    uint8_t new_cfg = (uint8_t) (old_cfg & ~MMCA_PRV_BOOT_PARTITION_ACCESS) |
                      (partition_access & MMCA_PRV_BOOT_PARTITION_ACCESS);

    /* Save original config for later restoration. */
    p_guard->saved_boot_cfg = old_cfg;

    /* If PARTITION_ACCESS is already set as requested, no need to write. */
    if (new_cfg == old_cfg)
    {
        p_guard->valid = 1U;

        return FSP_SUCCESS;
    }

    /* Write updated BOOT_CONFIG (only PARTITION_ACCESS bits are changed). */
    err = r_mmca_extcsd_write_byte(p_ctrl, MMCA_PRV_EMMC_EXT_CSD_BOOT_CONFIG_OFFSET, new_cfg);
    if (FSP_SUCCESS != err)
    {
        p_guard->valid = 0U;           // Invalid guard on failure
        return err;
    }

    p_guard->valid = 1U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * End temporary access by restoring BOOT_CONFIG (EXT_CSD[179]) to the value saved by
 * R_MMCA_Boot_PartitionAccessBegin().
 *
 * This function writes back the original BOOT_CONFIG value and clears the guard state.
 *
 * @param[in]  p_api_ctrl   Pointer to MMCA control block.
 * @param[in]  p_guard      Pointer to guard containing the saved BOOT_CONFIG.
 *
 * @retval FSP_SUCCESS                  BOOT_CONFIG restored successfully.
 * @retval FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN             Driver has not been initialized.
 * @retval FSP_ERR_INVALID_STATE        Guard is not valid (Begin was not called or already restored).
 * @retval FSP_ERR_DEVICE_BUSY          Device is busy (DAT0 held low) or another operation is ongoing.
 **********************************************************************************************************************/
fsp_err_t R_MMCA_Boot_PartitionAccessEnd (mmca_ctrl_t * const p_api_ctrl, mmca_boot_access_guard_t * const p_guard) /* PRQA S 1505 # Advisory R-8.7 */
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl;                                            /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_guard);
    FSP_ERROR_RETURN(MMCA_PRV_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Verify guard is valid (Begin was called successfully) */
    FSP_ERROR_RETURN((p_guard->valid == 1U), FSP_ERR_INVALID_STATE);

    /* Check if common error happens */
    err = r_mmca_common_error_check(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Restore BOOT_CONFIG saved in the guard. */
    err = r_mmca_extcsd_write_byte(p_ctrl, MMCA_PRV_EMMC_EXT_CSD_BOOT_CONFIG_OFFSET, p_guard->saved_boot_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Invalidate guard after successful restore. */
    p_guard->valid = 0U;

    return FSP_SUCCESS;
}

/** @} (end addtogroup MMCA) */

/*******************************************************************************************************************//**
 * Initializes MMC card.
 *
 * Follow the procedure in A.8.1 "Bus Initialization - Power-up" in JEDEC Standard No. 84-A441.
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 * @retval     FSP_SUCCESS               Operation completed successfully.
 * @retval     FSP_ERR_TIMEOUT           Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY       Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_card_power_up (mmca_instance_ctrl_t * const p_ctrl)
{
    uint32_t  ocr = (uint32_t) MMCA_PRV_OCR_ARGUMENT;
    fsp_err_t err = FSP_SUCCESS;

    /* eMMC device goes to Idle State - CMD0 */
    r_mmca_command_send_no_wait(p_ctrl, MMCA_PRV_CMD_GO_IDLE_STATE, 0);

    /* Verify the device is not busy. */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_COMMAND_RESPONSE_COMPLETE);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    for (uint32_t power_up_retry_attempt = 0U;
         power_up_retry_attempt < MMCA_PRV_POWER_UP_REPEAT;
         power_up_retry_attempt++)
    {
        r_mmca_command_send_no_wait(p_ctrl, MMCA_PRV_CMD_SEND_OP_COND, ocr);

        /* Verify the device is not busy. */
        err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_COMMAND_RESPONSE_COMPLETE);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Power Up Time depend on application parameters */
        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);

        /* Check MMCA is power-up */
        err = FSP_ERR_DEVICE_BUSY;

        /* Get response of CMD1 - R3 type*/
        p_ctrl->response.ocr.r3 = p_ctrl->p_reg->CE_RESP0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Initialization complete */
        if (p_ctrl->response.ocr.r3_b.power_up_status != 0U)
        {
            p_ctrl->sector_addressing = ((p_ctrl->response.ocr.r3_b.card_capacity_status) != 0U);

            err = FSP_SUCCESS;

            break;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Initializes host interface hardware.
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 * @retval     FSP_SUCCESS               Operation completed successfully.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_hw_cfg (mmca_instance_ctrl_t const * const p_ctrl)
{
    /* Reset MMCA. */
    p_ctrl->p_reg->CE_SWRESA_b.SWRST = 0x01U;                                                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_SWRESA_b.SWRST = 0x00U;                                                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Execute software reset or check SD_INFO2.SD_CLK_CTRLEN prior to calling this function. */
    p_ctrl->p_reg->CE_INT      = MMCA_PRV_CLEAR_REG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */       // Clear interrupt flag
    p_ctrl->p_reg->CE_INT_EN   = MMCA_PRV_INT_EN_CMD_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */ // Enable errors/timeouts interrupt
    p_ctrl->p_reg->CE_BUF_ACC  = MMCA_PRV_CLEAR_REG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */       // Disable DMA transfers
    p_ctrl->p_reg->CE_CLK_CTRL = MMCA_PRV_CLEAR_REG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */       // Disable the clock output control

    /* Set the clock frequency to less than 400 kHz for device power-up process */
    p_ctrl->p_reg->CE_CLK_CTRL = MMCA_PRV_CLK_POWER_UP_EMMC;                                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

#if MMCA_CFG_BOOT_ENABLE

    /* Set Boot Frequency */
    p_ctrl->p_reg->CE_CLK_CTRL        = p_ctrl->p_cfg->clock_control.clk_ctrl.clk_ctrl_u32;                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_BOOT_b.BTCLKDIV = p_ctrl->p_cfg->clock_control.boot_clk_ctrl.boot_clk_ctrl_b.boot_clk_div; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

    /* The host shall supply at least 74 SD clocks to the SD card while keeping CMD line high.  Reference section
     * 6.4.1.1 "Power Up Time of Card" in the SD Physical Layer Specification Version 6.00.
     * In this context, MMCA supplies 400kHz to SD card, we can use this formula to calculate wait time:
     * time = (number of clock) / Frequency = 74 clocks / 400kHz = 0.185 ms = 185 us (microseconds)*//*Then wait for 74 clock cycles*/
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Initializes bus clock, block length, and bus width.
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 * @retval     FSP_SUCCESS               Operation completed successfully.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY       Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_bus_cfg (mmca_instance_ctrl_t * const p_ctrl)
{
    /* According to JESD84,choose RCA with value greater than 1 */
    fsp_err_t err          = FSP_SUCCESS;
    uint16_t  block_length = p_ctrl->p_cfg->block_size;
    p_ctrl->p_reg->CE_CLK_CTRL = p_ctrl->p_cfg->clock_control.clk_ctrl.clk_ctrl_u32; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint8_t extcsd_buf[MMCA_MAX_BLOCK_SIZE];

    /* Enter identification state - CMD2 */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_ALL_SEND_CID, 0);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Assign relative address of module channel to the eMMC device - CMD3 */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SEND_RELATIVE_ADDR, MMCA_PRV_RCA_STUFF_BITS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Read eMMC's CSD - CMD9 */
    err = r_mmca_csd_get(p_ctrl, &p_ctrl->csd_infor);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Select eMMC device by its own relative address - CMD7 */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SEL_DES_CARD, MMCA_PRV_RCA_STUFF_BITS);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (MMCA_HIGH_SPEED_MODE_DISABLED != (p_ctrl->p_cfg->high_speed_mode))
    {
        /* Set high speed mode for eMMC maximum clock according to the csd's clock information*/
        err = r_mmca_high_speed_set(p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* CMD16 - Set block length */
    /* Check if common error happens */
    err = r_mmca_set_block_length(block_length, p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Set data bus width of communication */
    err = r_mmca_bus_width_set(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Read eMMC's Extended CSD */
    err = r_mmca_csd_extended_get(p_ctrl, extcsd_buf);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Save sector count bytes of device */
    p_ctrl->device.sector_count = ((uint32_t) extcsd_buf[MMCA_EXT_CSD_SEC_COUNT_0]) |
                                  ((uint32_t) extcsd_buf[MMCA_EXT_CSD_SEC_COUNT_1] << MMCA_BYTE_SHIFT_8) |
                                  ((uint32_t) extcsd_buf[MMCA_EXT_CSD_SEC_COUNT_2] << MMCA_BYTE_SHIFT_16) |
                                  ((uint32_t) extcsd_buf[MMCA_EXT_CSD_SEC_COUNT_3] << MMCA_BYTE_SHIFT_24);

    /* Save erase sector size of device */
    p_ctrl->device.erase_sector_count = (((uint32_t) p_ctrl->csd_infor.card_response.csd_v1_b.erase_grp_size) + 1U) *
                                        (((uint32_t) p_ctrl->csd_infor.card_response.csd_v1_b.erase_grp_mult) + 1U);

    return err;
}

/*******************************************************************************************************************//**
 * Set bus width.
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 *
 * @retval     FSP_SUCCESS          Bus width updated on device.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY  Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_bus_width_set (mmca_instance_ctrl_t * const p_ctrl)
{
    mmca_bus_width_t bus_width = p_ctrl->p_cfg->bus_width;
    fsp_err_t        err       = FSP_SUCCESS;

    /* Set the bus width to 8-bit data bus width  */
    if (MMCA_BUS_WIDTH_8_BIT == bus_width)
    {
        /* Use CMD6 to configure bus width, Access mode = 0x03, Data_Width_set = B7, Value set 8-bit data
         * bus = 0x200*/
        err = r_mmca_command_send(p_ctrl,
                                  (uint32_t) MMCA_PRV_CMD_SWITCH_RES_BSY,
                                  (uint32_t) MMCA_PRV_BUS_WIDTH_ARG_SET_8_BIT);
    }
    /* Set the bus width to 4-bit data bus width  */
    else if (MMCA_BUS_WIDTH_4_BIT == bus_width)
    {
        /* Use CMD6 to configure bus width, Access mode = 0x03, Data_Width_set = B7, Value set 4-bit data
         * bus = 0x100*/
        err = r_mmca_command_send(p_ctrl,
                                  (uint32_t) MMCA_PRV_CMD_SWITCH_RES_BSY,
                                  (uint32_t) MMCA_PRV_BUS_WIDTH_ARG_SET_4_BIT);
    }
    /* Bus width default setting is 1-bit data bus width  */
    else
    {
        /* Do nothing */
    }

    return err;
}

/*******************************************************************************************************************//**
 * Set mmca clock to fastest allowable speed for card.
 *
 * @param[in]  p_ctrl                    Pointer to the instance control block.
 *
 * @retval     FSP_SUCCESS               Clock rate adjusted to the maximum speed allowed by both device and MCU.
 * @retval     FSP_ERR_DEVICE_BUSY       Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_high_speed_set (mmca_instance_ctrl_t * p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Use CMD6 to config eMMC, Set HS mode Access mode = 0x03000000, B9 = High-speed Offset CSD, Value = 01 (HS), 02(HS100)*/
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SWITCH_RES_BSY, MMCA_HIGH_SPEED_SET);

    return err;
}

/*******************************************************************************************************************//**
 * Get and store relevant card specific data.
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 * @param[out] p_device_type        Pointer to store device type, which is used to determine if high speed is supported.
 *
 * @retval     FSP_SUCCESS          Device type obtained from eMMC CSD.  Sector count is also calculated here
 *                                  if it was not determined previously.
 * @retval     FSP_ERR_DEVICE_BUSY  Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_csd_get (mmca_instance_ctrl_t * const p_ctrl, mmc_response_t * p_device_type)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Clear MMCA buffer */
    p_ctrl->p_reg->CE_BUF_ACC = 0U;    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Get CSD data - CMD9 */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SEND_CSD, MMCA_PRV_RCA_STUFF_BITS);

    /* CSD register value are stored in the registers MMCAnCE_RESP3 to 0 */
    p_device_type->card_response.reg.response0 = p_ctrl->p_reg->CE_RESP0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_device_type->card_response.reg.response1 = p_ctrl->p_reg->CE_RESP1; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_device_type->card_response.reg.response2 = p_ctrl->p_reg->CE_RESP2; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_device_type->card_response.reg.response3 = p_ctrl->p_reg->CE_RESP3; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Collect information from eMMC */
    p_device_type->card_response.csd_v1_b.file_format =
        ((p_device_type->card_response.reg.response0 >> 10) & MMCA_MASK_2BITS);
    p_device_type->card_response.csd_v1_b.tmp_write_protect =
        ((p_device_type->card_response.reg.response0 >> 12) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.perm_write_protect =
        ((p_device_type->card_response.reg.response0 >> 13) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.copy =
        ((p_device_type->card_response.reg.response0 >> 14) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.file_format_grp =
        ((p_device_type->card_response.reg.response0 >> 15) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.content_prot_app =
        ((p_device_type->card_response.reg.response0 >> 16) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.write_bl_partial =
        ((p_device_type->card_response.reg.response0 >> 21) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.write_bl_len =
        ((p_device_type->card_response.reg.response0 >> 22) & MMCA_MASK_4BITS);
    p_device_type->card_response.csd_v1_b.r2w_factor =
        ((p_device_type->card_response.reg.response0 >> 26) & MMCA_MASK_3BITS);
    p_device_type->card_response.csd_v1_b.default_ecc =
        ((p_device_type->card_response.reg.response0 >> 29) & MMCA_MASK_2BITS);
    p_device_type->card_response.csd_v1_b.wp_grp_enable =
        ((p_device_type->card_response.reg.response0 >> 31) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.wp_grp_size =
        ((p_device_type->card_response.reg.response1) & MMCA_MASK_5BITS);
    p_device_type->card_response.csd_v1_b.erase_grp_mult =
        ((p_device_type->card_response.reg.response1 >> 5) & MMCA_MASK_5BITS);
    p_device_type->card_response.csd_v1_b.erase_grp_size =
        ((p_device_type->card_response.reg.response1 >> 10) & MMCA_MASK_5BITS);
    p_device_type->card_response.csd_v1_b.c_size_mult =
        ((p_device_type->card_response.reg.response1 >> 15) & MMCA_MASK_3BITS);
    p_device_type->card_response.csd_v1_b.vdd_w_curr_max =
        ((p_device_type->card_response.reg.response1 >> 18) & MMCA_MASK_3BITS);
    p_device_type->card_response.csd_v1_b.vdd_w_curr_min =
        ((p_device_type->card_response.reg.response1 >> 21) & MMCA_MASK_3BITS);
    p_device_type->card_response.csd_v1_b.vdd_r_curr_max =
        ((p_device_type->card_response.reg.response1 >> 24) & MMCA_MASK_3BITS);
    p_device_type->card_response.csd_v1_b.vdd_r_curr_min =
        ((p_device_type->card_response.reg.response1 >> 27) & MMCA_MASK_3BITS);

    /* Read volatile registers separately to ensure defined access order */
    uint32_t response1 = p_device_type->card_response.reg.response1;
    uint32_t response2 = p_device_type->card_response.reg.response2;
    p_device_type->card_response.csd_v1_b.c_size =
        (((response1 >> 30) & MMCA_MASK_2BITS) | ((response2 & MMCA_MASK_10BITS)));

    p_device_type->card_response.csd_v1_b.dsr_imp =
        ((p_device_type->card_response.reg.response2 >> 12) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.read_blk_misalign =
        ((p_device_type->card_response.reg.response2 >> 13) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.write_blk_misalign =
        ((p_device_type->card_response.reg.response2 >> 14) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.read_bl_partial =
        ((p_device_type->card_response.reg.response2 >> 15) & MMCA_MASK_1BIT);
    p_device_type->card_response.csd_v1_b.read_bl_len =
        ((p_device_type->card_response.reg.response2 >> 16) & MMCA_MASK_4BITS);
    p_device_type->card_response.csd_v1_b.ccc =
        ((p_device_type->card_response.reg.response2 >> 20) & MMCA_MASK_12BITS);
    p_device_type->card_response.csd_v1_b.tran_speed = ((p_device_type->card_response.reg.response3) & MMCA_MASK_8BITS);
    p_device_type->card_response.csd_v1_b.nsac       =
        ((p_device_type->card_response.reg.response3 >> 8) & MMCA_MASK_8BITS);
    p_device_type->card_response.csd_v1_b.taac =
        ((p_device_type->card_response.reg.response3 >> 16) & MMCA_MASK_8BITS);
    p_device_type->card_response.csd_v1_b.spec_vers =
        ((p_device_type->card_response.reg.response3 >> 26) & MMCA_MASK_4BITS);
    p_device_type->card_response.csd_v1_b.csd_structure =
        ((p_device_type->card_response.reg.response3 >> 30) & MMCA_MASK_2BITS);

    return err;
}

/*******************************************************************************************************************//**
 * Send a command to MMC and wait for response
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 * @param[in]  command              Command to send.
 * @param[in]  argument             Argument to send with the command.
 *
 * @retval     FSP_SUCCESS          Command sent and response received, no errors in response.
 * @retval     FSP_ERR_DEVICE_BUSY  Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_command_send (mmca_instance_ctrl_t * p_ctrl, uint32_t command, uint32_t argument)
{
    /* Send the command. */
    fsp_err_t err = FSP_SUCCESS;
    r_mmca_command_send_no_wait(p_ctrl, command, argument);

    /* Wait for end of response, error or timeout */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_COMMAND_RESPONSE_COMPLETE);

    /* If command response busy */
    if (1U == p_ctrl->p_reg->CE_CMD_SET_b.RBSY)                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        p_ctrl->response.card_status.status = p_ctrl->p_reg->CE_RESP0;  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->CE_INT_EN            = MMCA_PRV_INT_EN_BSY_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return err;
}

/*******************************************************************************************************************//**
 * Begin write transaction
 *
 * @retval     FSP_SUCCESS                   Erase operation requested.
 * @retval     FSP_ERR_TIMEOUT               Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY           Device is holding DAT0 low (device is busy) or another operation is
 *                                           ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_transfer_write (mmca_instance_ctrl_t * const p_ctrl,
                                        uint32_t                     start_sector,
                                        uint32_t                     block_num,
                                        uint8_t const * const        p_source)
{
    fsp_err_t err = FSP_SUCCESS;

    p_ctrl->p_reg->CE_BLOCK_SET_b.BLKCNT = (uint16_t) block_num; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Transfer without DMA/DTS */
    /* Disable DMA transfer request for buffer writing */
    p_ctrl->p_reg->CE_BUF_ACC_b.DMAWEN = 0U;                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 0U;                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Check if it is a single block write */
    if (MMCA_PRV_MULTI_BLOCK_SETTING > block_num)
    {
        err = r_mmca_single_block_write(p_ctrl, start_sector, p_source);
    }
    else
    {
        err = r_mmca_multi_block_write(p_ctrl, start_sector, block_num, p_source);
    }

    return err;
}

/*******************************************************************************************************************//**
 * Write a single block of data to the eMMC device.
 *
 * @param[in]  p_ctrl                 Pointer to the instance control block
 * @param[in]  start_sector           Sector address to write to
 * @param[in]  p_source               Pointer to the source data buffer
 *
 * @retval     FSP_SUCCESS            Write operation completed successfully
 * @retval     FSP_ERR_TIMEOUT        Timeout occurred during write operation
 * @retval     FSP_ERR_DEVICE_BUSY    Device is busy and cannot accept write command
 * @retval     FSP_ERR_WRITE_FAILED   Write operation failed due to device error
 **********************************************************************************************************************/
static fsp_err_t r_mmca_single_block_write (mmca_instance_ctrl_t * const p_ctrl,
                                            uint32_t                     start_sector,
                                            uint8_t const * const        p_source)
{
    fsp_err_t err          = FSP_SUCCESS;
    uint16_t  block_length = p_ctrl->p_cfg->block_size;
    uint32_t  word_index;
    uint32_t  last_word       = 0U;
    uint32_t  num_words       = ((uint32_t) block_length) / 4U; /* Calculate number of complete 32-bit words to write */
    uint32_t  remaining_bytes = ((uint32_t) block_length) % 4U; /* Calculate any remaining bytes (0-3) */

    /* Enable data transmit interrupts for buffer write operations */
    p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_WRITE_MASK;      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Send CMD24 (WRITE_BLOCK) to initiate single block write to specified sector */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_WRITE_SINGLE_BLOCK, start_sector);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Re-enable data transmit interrupts to handle outgoing data */
    p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_WRITE_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Wait until buffer is ready to be written (buffer ready to accept data) */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_WRITE_READY);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Store response from card for status checking */
    p_ctrl->response.card_status.status = p_ctrl->p_reg->CE_RESP0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Write data in 32-bit word chunks using byte-safe reads to support unaligned buffers */
    for (word_index = 0; word_index < num_words; word_index++)
    {
        uint32_t byte_offset = word_index * 4U;
        p_ctrl->p_reg->CE_DATA = (uint32_t) p_source[byte_offset] | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                 ((uint32_t) p_source[byte_offset + 1U] << MMCA_BYTE_SHIFT_8) |
                                 ((uint32_t) p_source[byte_offset + 2U] << MMCA_BYTE_SHIFT_16) |
                                 ((uint32_t) p_source[byte_offset + 3U] << MMCA_BYTE_SHIFT_24);
    }

    /* Handle any remaining bytes (1-3) that don't form a complete 32-bit word */
    if (remaining_bytes > 0U)
    {
        /* Prepare the final word containing the remaining bytes */
        last_word = 0U;                                              /* Initialize with zeros
                                                                      * to avoid writing garbage data */

        /* Set up byte pointers for precise byte-level copying */
        uint8_t       * p_last_bytes  = (uint8_t *) &last_word;      /* Destination: bytes for last write word */
        const uint8_t * p_source_last = p_source + (num_words * 4U); /* Source: end of buffer */                   /* PRQA S 0488 # Advisory R-18.4 */

        /* Copy only the required remaining bytes (1-3) to maintain data integrity */
        for (word_index = 0; word_index < remaining_bytes; word_index++)
        {
            p_last_bytes[word_index] = p_source_last[word_index];
        }

        /* Write the final word with the remaining bytes */
        p_ctrl->p_reg->CE_DATA = last_word; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Wait for write operation to complete fully before returning */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_DATA_TRANSMISSION_COMPLETE);

    return err;
}

/*******************************************************************************************************************//**
 * Begin write multi blocks
 *
 * @retval     FSP_SUCCESS                   Erase operation requested.
 * @retval     FSP_ERR_TIMEOUT               Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY           Device is holding DAT0 low (device is busy) or another operation is
 *                                           ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_multi_block_write (mmca_instance_ctrl_t * const p_ctrl,
                                           uint32_t                     start_sector,
                                           uint32_t                     block_num,
                                           uint8_t const * const        p_source)
{
    fsp_err_t err = FSP_SUCCESS;
#if MMCA_CFG_DMAC_ENABLE
    uint32_t blocknumber = 0;
#else
    uint32_t byte_index;
    uint32_t block_index;
#endif

    /* CMD23 - Set number of blocks transfer to eMMC */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SET_BLOCK_COUNT, block_num | MMCA_CMD23_RELIABLE_WRITE_FLAG);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_WRITE_MULTIPLE_BLOCK_PRE_DEF, start_sector);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Enable data transfer interrupt */
    p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_WRITE_MASK;         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Read response from eMMC */
    p_ctrl->response.card_status.status = p_ctrl->p_reg->CE_RESP0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

#if MMCA_CFG_DMAC_ENABLE
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_WRITE_READY);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    transfer_instance_t const * const p_transfer = p_ctrl->p_cfg->p_transfer;
    if (NULL != p_transfer)
    {
        void * p_dest;
        p_dest      = (void *) &(p_ctrl->p_reg->CE_DATA);                                                            /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        blocknumber = (uint32_t) ((block_num * MMCA_MAX_BLOCK_SIZE) >> MMCA_BYTE_SHIFT_2);

        /* Configure transfer direction for write: src incremented (RAM), dest fixed (CE_DATA) */
        p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED; /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
        p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode = TRANSFER_ADDR_MODE_FIXED;       /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */

        /* DMA TSR register counts total transfer size in bytes */
        p_transfer->p_cfg->p_info->number_transfer = blocknumber;
        p_transfer->p_cfg->p_info->p_src           = p_source;
        p_transfer->p_cfg->p_info->p_dest          = p_dest;

        (void) p_transfer->p_api->callbackSet(p_transfer->p_ctrl, mmca_txi_transfer_callback, p_ctrl, NULL); /* PRQA S 3635 # Advisory R-17.12 */
        err = p_transfer->p_api->reconfigure(p_transfer->p_ctrl, p_transfer->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_transfer->p_api->enable(p_transfer->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Mark DMA write in progress (independent of HPI writing_nonblock_flag) */
        p_ctrl->dma_write_in_progress = true;

        /* Enable DTRANE interrupt so mmca_int_isr() finalizes and notifies caller */
        p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_WRITE_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Enable DMA transfer request for buffer writing */
        p_ctrl->p_reg->CE_BUF_ACC_b.DMAWEN = 1U;               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 1U;               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

#else

    /* Write data to buffer MMCA's DATA */
    for (block_index = 0; block_index < block_num; block_index++)
    {
        err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_WRITE_READY);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Block length is fixed to 512 bytes in multi block transaction */
        for (byte_index = (MMCA_MAX_BLOCK_SIZE * block_index);
             byte_index < (MMCA_MAX_BLOCK_SIZE * (block_index + 1U));
             byte_index += 4U)
        {
            p_ctrl->p_reg->CE_DATA = ((uint32_t) p_source[byte_index]) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                     ((uint32_t) p_source[byte_index + 1U] << MMCA_BYTE_SHIFT_8) |
                                     ((uint32_t) p_source[byte_index + 2U] << MMCA_BYTE_SHIFT_16) |
                                     ((uint32_t) p_source[byte_index + 3U] << MMCA_BYTE_SHIFT_24);
        }
    }
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_DATA_TRANSMISSION_COMPLETE);
#endif

    return err;
}

/*******************************************************************************************************************//**
 * Read a single block
 *
 * @retval     FSP_SUCCESS                   Erase operation requested.
 * @retval     FSP_ERR_TIMEOUT               Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY           Device is holding DAT0 low (device is busy) or another operation is
 *                                           ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_transfer_read (mmca_instance_ctrl_t * const p_ctrl,
                                       uint32_t                     start_sector,
                                       uint32_t                     block_num,
                                       uint8_t * const              p_dest)
{
    fsp_err_t err = FSP_SUCCESS;

    p_ctrl->p_reg->CE_BLOCK_SET_b.BLKCNT = (uint16_t) block_num; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Transfer without DMA/DTS */
    /* Disable DMA transfer request for buffer reading */
    p_ctrl->p_reg->CE_BUF_ACC_b.DMAREN = 0U;                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 0U;                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Check if it is a single block write */
    if (MMCA_PRV_MULTI_BLOCK_SETTING > block_num)
    {
        err = r_mmca_single_block_read(p_ctrl, start_sector, p_dest);
    }
    else
    {
        err = r_mmca_multi_block_read(p_ctrl, start_sector, block_num, p_dest);
    }

    return err;
}

/*******************************************************************************************************************//**
 * Read a single block of data from the eMMC device.
 *
 * @param[in]  p_ctrl                 Pointer to the instance control block
 * @param[in]  start_sector           Sector address to read from
 * @param[in]  p_dest                 Pointer to the destination data buffer
 *
 * @retval     FSP_SUCCESS            Read operation completed successfully
 * @retval     FSP_ERR_TIMEOUT        Timeout occurred during read operation
 * @retval     FSP_ERR_DEVICE_BUSY    Device is busy and cannot accept read command
 * @retval     FSP_ERR_READ_FAILED    Read operation failed due to device error
 **********************************************************************************************************************/
static fsp_err_t r_mmca_single_block_read (mmca_instance_ctrl_t * const p_ctrl,
                                           uint32_t                     start_sector,
                                           uint8_t * const              p_dest)
{
    fsp_err_t err          = FSP_SUCCESS;
    uint16_t  block_length = p_ctrl->p_cfg->block_size;

    uint32_t word_index;
    uint32_t data_reg;

    uint32_t last_word       = 0U;
    uint32_t num_words       = ((uint32_t) block_length) / 4U; /* Calculate number of complete 32-bit words to read */
    uint32_t remaining_bytes = ((uint32_t) block_length) % 4U; /* Calculate any remaining bytes (0-3) */

    /* Enable data receive interrupts for buffer read operations */
    p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_READ_MASK;      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Send CMD17 (READ_SINGLE_BLOCK) to initiate single block read from specified sector */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_READ_SINGLE_BLOCK, start_sector);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Re-enable data receive interrupts to handle incoming data */
    p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_READ_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Wait until buffer is ready to be read (data available in buffer) */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_READ_READY);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Store response from card for status checking */
    p_ctrl->response.card_status.status = p_ctrl->p_reg->CE_RESP0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Read data in 32-bit word chunks using byte-safe writes to support unaligned buffers */
    for (word_index = 0; word_index < num_words; word_index++)
    {
        uint32_t byte_offset = word_index * 4U;
        data_reg                 = p_ctrl->p_reg->CE_DATA; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_dest[byte_offset]      = (uint8_t) (data_reg);
        p_dest[byte_offset + 1U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_8);
        p_dest[byte_offset + 2U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_16);
        p_dest[byte_offset + 3U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_24);
    }

    /* Handle any remaining bytes (1-3) that don't form a complete 32-bit word */
    if (remaining_bytes > 0U)
    {
        /* Read the final word containing the remaining bytes */
        last_word = p_ctrl->p_reg->CE_DATA;                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Set up byte pointers for precise byte-level copying */
        uint8_t const * p_last_bytes = (uint8_t const *) &last_word;
        uint8_t       * p_dest_last  = p_dest + (num_words * 4U); /* PRQA S 0488 # Advisory R-18.4 */

        /* Copy only the required remaining bytes (1-3) to maintain data integrity */
        for (word_index = 0; word_index < remaining_bytes; word_index++)
        {
            p_dest_last[word_index] = p_last_bytes[word_index];
        }
    }

    /* Wait until the write buffer is ready */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_READ_COMPLETE);

    return err;
}

/*******************************************************************************************************************//**
 * Begin read multi blocks
 *
 * @retval     FSP_SUCCESS                   Erase operation requested.
 * @retval     FSP_ERR_TIMEOUT               Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY           Device is holding DAT0 low (device is busy) or another operation is
 *                                           ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_multi_block_read (mmca_instance_ctrl_t * const p_ctrl,
                                          uint32_t                     start_sector,
                                          uint32_t                     block_num,
                                          uint8_t * const              p_dest)
{
    fsp_err_t err = FSP_SUCCESS;

#if MMCA_CFG_DMAC_ENABLE
    uint32_t blocknumber = 0U;
#else
    uint32_t byte_index;
    uint32_t block_index = 0U;
    uint32_t data_reg;
#endif

    /* Send CMD23-Set Block Count to eMMC device */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SET_BLOCK_COUNT, block_num);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Send CMD18-Multi Block Read */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_READ_MULTIPLE_BLOCK_PRE_DEF, start_sector);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Read response from eMMC */
    p_ctrl->response.card_status.status = p_ctrl->p_reg->CE_RESP0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Enable data receive interrupt */
    p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_READ_MASK;          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

#if MMCA_CFG_DMAC_ENABLE

    /* Wait until the read buffer is ready */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_READ_READY);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    transfer_instance_t const * const p_transfer = p_ctrl->p_cfg->p_transfer;
    if (NULL != p_transfer)
    {
        void const * p_src;
        p_src       = (void const *) &(p_ctrl->p_reg->CE_DATA);                                                      /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        blocknumber = (uint32_t) ((block_num * MMCA_MAX_BLOCK_SIZE) >> MMCA_BYTE_SHIFT_2);

        /* Configure transfer direction for read: src fixed (CE_DATA), dest incremented (RAM) */
        p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED;       /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */
        p_transfer->p_cfg->p_info->transfer_mode_cfg.transfer_mode_b.des_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED; /* PRQA S 4423 # DP_MSR_R-10.3_UC1 */

        /* DMA TSR register counts total transfer size in bytes */
        p_transfer->p_cfg->p_info->number_transfer = blocknumber;
        p_transfer->p_cfg->p_info->p_src           = p_src;
        p_transfer->p_cfg->p_info->p_dest          = p_dest;

        (void) p_transfer->p_api->callbackSet(p_transfer->p_ctrl, mmca_rxi_transfer_callback, p_ctrl, NULL); /* PRQA S 3635 # Advisory R-17.12 */
        err = p_transfer->p_api->reconfigure(p_transfer->p_ctrl, p_transfer->p_cfg->p_info);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = p_transfer->p_api->enable(p_transfer->p_ctrl);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Mark DMA read in progress (independent of HPI flags) */
        p_ctrl->dma_read_in_progress = true;

        /* Keep read interrupts enabled so ISR finalizes on BUFRE */
        p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_READ_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Enable DMA transfer request for buffer reading */
        p_ctrl->p_reg->CE_BUF_ACC_b.DMAREN = 1U;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 1U;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

#else
    for (block_index = 0; block_index < block_num; block_index++)
    {
        /* Wait until the write buffer is ready */
        err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_READ_READY);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Block length is fixed to 512 bytes in multi block transaction*/
        for (byte_index = (MMCA_MAX_BLOCK_SIZE * block_index);
             byte_index < (MMCA_MAX_BLOCK_SIZE * (block_index + 1U));
             byte_index += 4U)
        {
            data_reg                = p_ctrl->p_reg->CE_DATA; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_dest[byte_index]      = (uint8_t) (data_reg);   // data handling, store to RAM
            p_dest[byte_index + 1U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_8);
            p_dest[byte_index + 2U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_16);
            p_dest[byte_index + 3U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_24);
        }
    }
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_READ_COMPLETE);
#endif

    return err;
}

/*******************************************************************************************************************//**
 * Set block length transaction to eMMC device
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 * @param[in]  block_length          Transaction block length (bytes) from load configuration.
 *
 * @retval     FSP_SUCCESS          Command sent and response received, no errors in response.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY  Device is holding DAT0 low (device is busy) or another operation is ongoing.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_set_block_length (uint16_t block_length, mmca_instance_ctrl_t * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Reset Buffer Access register*/
    p_ctrl->p_reg->CE_BUF_ACC            = 0U;                                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_CLK_CTRL           = p_ctrl->p_cfg->clock_control.clk_ctrl.clk_ctrl_u32; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_BLOCK_SET_b.BLKSIZ = block_length;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SET_BLOCKLEN, block_length);

    p_ctrl->response.card_status.status = p_ctrl->p_reg->CE_RESP0;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return err;
}

/*******************************************************************************************************************//**
 * Wait for the device.
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 * @param[in]  event                Event is pending.
 *
 * @retval     FSP_SUCCESS          Previous operation is complete, and mmca is ready for the next operation.
 * @retval     FSP_ERR_DEVICE_BUSY  Device is holding DAT0 low (device is busy) or another operation is ongoing.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_wait_for_event (mmca_instance_ctrl_t * const p_ctrl, mmca_event_t event)
{
    fsp_err_t err = FSP_ERR_TIMEOUT;

    /* Wait for device out of commence sequence in 5s */
    for (uint32_t wait_count = 0; wait_count < MMCA_PRV_BUSY_TIMEOUT; wait_count++)
    {
        /* Check if the expected event has occurred */
        if (0U != (((uint32_t) p_ctrl->mmca_event) & ((uint32_t) event)))
        {
            /* Clear the event flag to acknowledge it has been processed */
            uint32_t mmca_event_raw = ((uint32_t) p_ctrl->mmca_event) & (~((uint32_t) event));
            p_ctrl->mmca_event = (mmca_event_t) mmca_event_raw; /* PRQA S 4342 # Advisory R-10.5 */
            err                = FSP_SUCCESS;
            break;
        }

        /* Event not ready yet, wait 1 microsecond before next check */
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    }

    return err;
}

/*******************************************************************************************************************//**
 * Send a command to MMC
 *
 * @param[in]  p_ctrl          Pointer to the instance control block.
 * @param[in]  command         Command to send.
 * @param[in]  argument        Argument to send with the command.
 **********************************************************************************************************************/
static void r_mmca_command_send_no_wait (mmca_instance_ctrl_t const * p_ctrl, uint32_t command, uint32_t argument)
{
    /* Clear interrupt flags */
    p_ctrl->p_reg->CE_INT = 0U;                                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Enable Clock */
    p_ctrl->p_reg->CE_CLK_CTRL |= MMCA_PRV_CLK_CTRL_EN_MASK;                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Enable error interrupts*/
    p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_CMD_MASK;                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Setting of command transmission*/
    /* Write argument, then command to the MMCA peripheral. */
    p_ctrl->p_reg->CE_ARG     = argument;                                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_CMD_SET = (command | (uint32_t) p_ctrl->p_cfg->bus_width); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
}

#if MMCA_CFG_DMAC_ENABLE

/*******************************************************************************************************************//**
 * Configures any dependent drivers selected by the user, including transfer and timer drivers.
 *
 * @param[in] p_reg                    Pointer to MMCA base register address for this channel.
 * @param[in] p_cfg                    Pointer to the configuration structure.
 *
 * @retval FSP_SUCCESS                 Dependent drivers configured successfully.
 * @return                             See @ref RENESAS_ERROR_CODES or functions called by this function for other
 *                                     possible return codes. This function calls:
 *                                         * @ref transfer_api_t::open
 **********************************************************************************************************************/
static fsp_err_t r_mmca_dependent_drivers_configure (mmca_cfg_t const * const     p_cfg,
                                                     mmca_instance_ctrl_t * const p_instance_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    /* Prepare transfer configuration. */
    fsp_err_t err = FSP_SUCCESS;
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);

    /* Open transfer if provided */
    if (NULL != p_cfg->p_transfer)
    {
        err = p_cfg->p_transfer->p_api->open(p_cfg->p_transfer->p_ctrl, p_cfg->p_transfer->p_cfg);
        FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
    }

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * Parameter checking for runtime APIs.
 *
 * @param[in]  p_ctrl                        Pointer to the instance control block.
 *
 * @retval     FSP_SUCCESS                   Device is ready to be accessed.
 * @retval     FSP_ERR_ASSERTION             A required pointer is NULL.
 * @retval     FSP_ERR_NOT_OPEN              Driver has not been initialized.
 * @retval     FSP_ERR_CARD_NOT_INITIALIZED  Card was unplugged.
 * @retval     FSP_ERR_DEVICE_BUSY           Driver is busy with a previous operation.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_common_error_check (mmca_instance_ctrl_t const * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;

#if MMCA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
#endif

#if MMCA_CFG_DMAC_ENABLE
    if (p_ctrl->dma_write_in_progress || p_ctrl->dma_read_in_progress)
    {
        return FSP_ERR_DEVICE_BUSY;
    }
#endif

    /* To verify no command sequence is in progress in MMCA, verify MMCA0CE_HOST_STS1.CMDSEQ or
     * MMCA0CE_CLK_CTRL.MMCBUSSY (MMCBUSSY CMDSEQ set bit when command sequence happens at the same time but
     * MMCBUSSY clear bit 0 after CMDSEQ 10 clock cycle when command sequence ends)*/
    if (1U == p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        err = FSP_ERR_DEVICE_BUSY;
    }
    else if ((p_ctrl->initialized_mmc) == false)
    {
        err = FSP_ERR_CARD_NOT_INITIALIZED;
    }
    else
    {
        /* Do nothing */
    }

    return err;
}

/*******************************************************************************************************************//**
 * Calls user callback
 *
 * @param[in]     p_ctrl    Pointer to the instance control block.
 * @param[in]     event     MMCA event
 **********************************************************************************************************************/
static void r_mmca_call_callback (mmca_instance_ctrl_t const * p_ctrl, mmca_event_t event)
{
    if (NULL == p_ctrl->p_callback)
    {

        /* Return if user callback is not available */
        return;
    }

    mmca_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    mmca_callback_args_t * p_args = p_ctrl->p_callback_memory;

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

    p_args->p_context = p_ctrl->p_context;
    p_args->event     = event;

    p_ctrl->p_callback(p_args);

    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }
}

/*******************************************************************************************************************//**
 * MMCA ISR
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void mmca_int_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Clear IRQ flag to prevent re-triggering */
    R_BSP_IrqStatusClear((IRQn_Type) irq);

    /* Get the MMCA instance control structure */
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

#if MMCA_CFG_NON_BLOCK_ENABLE

    /* Read interrupt status register */
    uint32_t mmca_event = p_ctrl->p_reg->CE_INT; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

 #if MMCA_CFG_DMAC_ENABLE

    /* Finalize DMA-based Write (non-HPI). Independent of writing_nonblock_flag. */
    if (p_ctrl->dma_write_in_progress)
    {
        if (((uint32_t) mmca_event & (uint32_t) MMCA_PRV_ACCESS_ERROR_MASK) != 0U)                                 /* PRQA S 4522, 4542 # DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC1 */
        {
            p_ctrl->p_reg->CE_INT              = ~((uint32_t) mmca_event & (uint32_t) MMCA_PRV_ACCESS_ERROR_MASK); /* PRQA S 0662, 4522, 4542 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC1 */
            p_ctrl->p_reg->CE_INT_EN           = 0U;                                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_BUF_ACC_b.DMAWEN = 0U;                                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 0U;                                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->dma_write_in_progress      = false;
            p_ctrl->mmca_event                 =
                (mmca_event_t) ((uint32_t) mmca_event & (uint32_t) MMCA_PRV_ACCESS_ERROR_MASK);                    /* PRQA S 4342, 4394, 4522, 4542 # Advisory R-10.5, DP_MSR_R-10.8_UC1, DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC1 */
            r_mmca_call_callback(p_ctrl, p_ctrl->mmca_event);
            FSP_CONTEXT_RESTORE

            return;
        }

        if (((uint32_t) mmca_event & (uint32_t) MMCA_EVENT_DATA_TRANSMISSION_COMPLETE) != 0U)
        {
            p_ctrl->p_reg->CE_INT              = ~(uint32_t) MMCA_EVENT_DATA_TRANSMISSION_COMPLETE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_INT_EN           = 0U;                                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_BUF_ACC_b.DMAWEN = 0U;                                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 0U;                                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->dma_write_in_progress      = false;
            p_ctrl->mmca_event                 = MMCA_EVENT_DATA_TRANSMISSION_COMPLETE;
            r_mmca_call_callback(p_ctrl, MMCA_EVENT_DATA_TRANSMISSION_COMPLETE);
            FSP_CONTEXT_RESTORE

            return;
        }
    }

    /* Finalize DMA-based Read (non-HPI). */
    if (p_ctrl->dma_read_in_progress)
    {
        if (((uint32_t) mmca_event & (uint32_t) MMCA_PRV_ACCESS_ERROR_MASK) != 0U)                                 /* PRQA S 4522, 4542 # DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC1 */
        {
            p_ctrl->p_reg->CE_INT              = ~((uint32_t) mmca_event & (uint32_t) MMCA_PRV_ACCESS_ERROR_MASK); /* PRQA S 0662, 4522, 4542 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC1 */
            p_ctrl->p_reg->CE_INT_EN           = 0U;                                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_BUF_ACC_b.DMAREN = 0U;                                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 0U;                                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->dma_read_in_progress       = false;
            p_ctrl->mmca_event                 =
                (mmca_event_t) ((uint32_t) mmca_event & (uint32_t) MMCA_PRV_ACCESS_ERROR_MASK);                    /* PRQA S 4342, 4394, 4522, 4542 # Advisory R-10.5, DP_MSR_R-10.8_UC1, DP_MSR_R-10.1_UC1, DP_MSR_R-10.1_UC1 */
            r_mmca_call_callback(p_ctrl, p_ctrl->mmca_event);
            FSP_CONTEXT_RESTORE

            return;
        }

        if (((uint32_t) mmca_event & (uint32_t) MMCA_EVENT_BUFFER_READ_COMPLETE) != 0U)
        {
            p_ctrl->p_reg->CE_INT              = ~(uint32_t) MMCA_EVENT_BUFFER_READ_COMPLETE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_INT_EN           = 0U;                                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_BUF_ACC_b.DMAREN = 0U;                                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->p_reg->CE_BUF_ACC_b.DMATYP = 0U;                                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_ctrl->dma_read_in_progress       = false;
            p_ctrl->mmca_event                 = MMCA_EVENT_BUFFER_READ_COMPLETE;
            r_mmca_call_callback(p_ctrl, MMCA_EVENT_BUFFER_READ_COMPLETE);
            FSP_CONTEXT_RESTORE

            return;
        }
    }
 #endif                                /* MMCA_CFG_DMAC_ENABLE */

    /* Handle active write operation */

    if (p_ctrl->writing_nonblock_flag)
    {
        /* Buffer write ready event */
        if ((((uint32_t) mmca_event) & ((uint32_t) MMCA_EVENT_BUFFER_WRITE_READY)) != 0U) // BUFWEN bit
        {
            /* Only write if we have data source */
            if ((NULL != p_ctrl->p_mmca_tx_src) && (p_ctrl->mmca_blocks_remaining > 0U))
            {
                r_mmca_write_one_block(p_ctrl);
            }

            /* Clear ONLY the BUFWEN flag by writing 0 to bit 21 */
            p_ctrl->p_reg->CE_INT = ~(uint32_t) MMCA_EVENT_BUFFER_WRITE_READY;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Update the event mask to reflect cleared flag */
            mmca_event = (mmca_event_t) ((uint32_t) mmca_event & ~(uint32_t) MMCA_EVENT_BUFFER_WRITE_READY); /* PRQA S 4342, 4394, 4424 # Advisory R-10.5, DP_MSR_R-10.8_UC1, DP_MSR_R-10.3_UC1 */
        }

        /* Data transmission complete event */
        if ((((uint32_t) mmca_event) & ((uint32_t) MMCA_EVENT_DATA_TRANSMISSION_COMPLETE)) != 0U)
        {
            p_ctrl->p_reg->CE_INT = ~(uint32_t) MMCA_EVENT_DATA_TRANSMISSION_COMPLETE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Disable write interrupts to avoid re-entry */
            p_ctrl->p_reg->CE_INT_EN = 0U;                                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Clear operation state */
            p_ctrl->writing_nonblock_flag   = false;
            p_ctrl->p_mmca_tx_src           = NULL;
            p_ctrl->mmca_tx_bytes_remaining = 0;
            p_ctrl->mmca_blocks_remaining   = 0;

            /* Update event for callback */
            p_ctrl->mmca_event = MMCA_EVENT_DATA_TRANSMISSION_COMPLETE;

            /* Call user callback with success */
            if (NULL != p_ctrl->p_callback)
            {
                r_mmca_call_callback(p_ctrl, MMCA_EVENT_DATA_TRANSMISSION_COMPLETE);
            }

            /* Restore context and return - operation complete */
            FSP_CONTEXT_RESTORE

            return;
        }
    }

    /* Handle active erase operation */

    if (p_ctrl->erasing_nonblock_flag)
    {
        /* Command response complete event (for erase) */
        if ((((uint32_t) mmca_event) & ((uint32_t) MMCA_EVENT_COMMAND_RESPONSE_COMPLETE)) != 0U)
        {
            /* Clear the response complete flag */
            p_ctrl->p_reg->CE_INT = ~(uint32_t) MMCA_EVENT_COMMAND_RESPONSE_COMPLETE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Disable erase interrupts */
            p_ctrl->p_reg->CE_INT_EN = 0U;                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Clear erase operation state */
            p_ctrl->erasing_nonblock_flag = false;
            p_ctrl->erase_start_sector    = 0U;
            p_ctrl->erase_sector_count    = 0U;

            /* Update event for callback */
            p_ctrl->mmca_event = MMCA_EVENT_COMMAND_RESPONSE_COMPLETE;

            /* Call user callback */
            if (NULL != p_ctrl->p_callback)
            {
                r_mmca_call_callback(p_ctrl, p_ctrl->mmca_event);
            }

            /* Restore context and return - operation complete */
            FSP_CONTEXT_RESTORE

            return;
        }
    }

    /* Handle other events */

    /* Handle any remaining unprocessed events to prevent timeout */
    mmca_event = p_ctrl->p_reg->CE_INT;                 /* re-read to get remaining/pending bits */            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    if (mmca_event != 0U)
    {
        /* Clear all remaining flags */
        p_ctrl->p_reg->CE_INT = ~mmca_event;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Update event for callback */
        p_ctrl->mmca_event = (mmca_event_t) mmca_event; /* PRQA S 4342 # Advisory R-10.5 */

        /* Call callback for remaining events */
        if (NULL != p_ctrl->p_callback)
        {
            r_mmca_call_callback(p_ctrl, (mmca_event_t) mmca_event); /* PRQA S 4342 # Advisory R-10.5 */
        }
    }

#else

    /* Collect event from interrupt flag */
    p_ctrl->mmca_event = (mmca_event_t) p_ctrl->p_reg->CE_INT; /* PRQA S 4342, 0662 # Advisory R-10.5, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Clear interrupt flag */
    p_ctrl->p_reg->CE_INT = 0U;                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Call user callback */
    r_mmca_call_callback(p_ctrl, p_ctrl->mmca_event);
#endif

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

#if MMCA_CFG_DMAC_ENABLE

/*******************************************************************************************************************//**
 * DMAC Transmit ISR
 **********************************************************************************************************************/
void mmca_txi_transfer_callback (transfer_callback_args_t * p_transfer_args) /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    if ((TRANSFER_EVENT_TRANS_END_FLAG == p_transfer_args->event) ||
        (TRANSFER_EVENT_TRANS_COMPLETION == p_transfer_args->event))
    {
        mmca_instance_ctrl_t const * p_ctrl = (mmca_instance_ctrl_t const *) p_transfer_args->p_context; /* PRQA S 0316 # Advisory R-11.5 */
        r_mmca_call_callback(p_ctrl, MMCA_EVENT_DMA_TX_COMPLETED);
    }
}

/*******************************************************************************************************************//**
 * DMAC Receive ISR
 **********************************************************************************************************************/
void mmca_rxi_transfer_callback (transfer_callback_args_t * p_transfer_args) /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    if ((TRANSFER_EVENT_TRANS_END_FLAG == p_transfer_args->event) ||
        (TRANSFER_EVENT_TRANS_COMPLETION == p_transfer_args->event))
    {
        mmca_instance_ctrl_t const * p_ctrl = (mmca_instance_ctrl_t const *) p_transfer_args->p_context; /* PRQA S 0316 # Advisory R-11.5 */
        r_mmca_call_callback(p_ctrl, MMCA_EVENT_DMA_RX_COMPLETED);
    }
}

#endif

#if MMCA_CFG_NON_BLOCK_ENABLE

/*******************************************************************************************************************//**
 * Check High Priority Interrupt (HPI) support from eMMC device.
 *
 * This function reads the Extended CSD register to determine if the eMMC device supports HPI feature.
 * HPI allows interrupting ongoing data transfer operations for time-critical commands. The function
 * initializes HPI-related fields in the device structure and parses EXT_CSD[503] to extract HPI
 * capability information including support status and implementation type.
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 *
 * @retval     FSP_SUCCESS          HPI support status successfully determined from EXT_CSD.
 * @retval     FSP_ERR_RESPONSE     Device responded with an error during EXT_CSD read.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond to EXT_CSD command.
 * @retval     FSP_ERR_INVALID_STATE Card is not in valid state for EXT_CSD read.
 *
 * @note       Function updates device structure fields: hpi_supported, hpi_enabled, hpi_implementation.
 * @note       Must be called during media initialization after card is in TRANSFER state.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_check_hpi_support (mmca_instance_ctrl_t * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    uint8_t   extcsd_buf[MMCA_MAX_BLOCK_SIZE];

    /* Initialize HPI fields */
    p_ctrl->device.hpi_supported         = false;
    p_ctrl->device.hpi_enabled           = false;
    p_ctrl->device.hpi_implementation    = (bool) 0U;
    p_ctrl->device.operation_interrupted = false;

    /* Get EXT_CSD data */
    err = r_mmca_csd_extended_get(p_ctrl, extcsd_buf);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Check HPI support from EXT_CSD[503] */
    uint8_t hpi_features = extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_HPI_FEATURES_OFFSET];

    /* Parse HPI features */
    p_ctrl->device.hpi_supported      = ((hpi_features & ((uint8_t) MMCA_PRV_HPI_SUPPORT_BIT)) != 0U);
    p_ctrl->device.hpi_implementation =
        (((hpi_features & ((uint8_t) MMCA_PRV_HPI_IMPLEMENTATION_BIT)) >> MMCA_MASK_1BIT) != 0U);

    return err;
}

#endif

/*******************************************************************************************************************//**
 * Check Background Operation support support from eMMC device.
 *
 * @param[in]  p_ctrl                        Pointer to the instance control block.
 * @retval     FSP_SUCCESS                   Device is ready to be accessed.
 * @retval     FSP_ERR_DEVICE_BUSY           Device is holding DAT0 low (device is busy) or another operation is
 *                                           ongoing.
 * @retval     FSP_ERR_TIMEOUT               Device did not respond.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_check_bkops_support (mmca_instance_ctrl_t * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    uint8_t   extcsd_buf[MMCA_MAX_BLOCK_SIZE];

    /* Initialize Background Operation fields */
    p_ctrl->device.bkops_supported = false;
    p_ctrl->device.bkops_enabled   = false;
    p_ctrl->device.bkops_status    = 0U;

    /* Get Extended CSD Register */
    err = r_mmca_csd_extended_get(p_ctrl, extcsd_buf);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Check Background Operation support from EXT_CSD[502] */
    uint8_t bkops_features = extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_BKOPS_FEATURES_OFFSET];

    /* Check Background Operation status from EXT_CSD[246] */
    uint8_t bkops_status = extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_BKOPS_STATUS_OFFSET];

    /* Check Background Operation enable from EXT_CSD[163] */
    uint8_t bkops_enabled = extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_BKOPS_EN_OFFSET];

    /* Parse Background Operation features */
    p_ctrl->device.bkops_supported = ((bkops_features & MMCA_PRV_BKOPS_SUPPORT_BIT) != 0U);
    p_ctrl->device.bkops_status    = (uint8_t) (bkops_status & MMCA_MASK_2BITS);
    p_ctrl->device.bkops_enabled   = ((bkops_enabled & MMCA_MASK_1BIT) != 0U);

    return err;
}

#if MMCA_CFG_NON_BLOCK_ENABLE

/*******************************************************************************************************************//**
 * Enable High Priority Interrupt (HPI) feature on eMMC device.
 *
 * This function sends CMD6 (SWITCH) command to enable HPI functionality by writing to EXT_CSD[161].
 * HPI must be supported by the device (checked via r_mmca_check_hpi_support) before enabling.
 * Once enabled, HPI allows interrupting ongoing data transfer operations for urgent commands.
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 *
 * @retval     FSP_SUCCESS          HPI feature successfully enabled on device.
 * @retval     FSP_ERR_UNSUPPORTED  Device does not support HPI feature.
 * @retval     FSP_ERR_RESPONSE     Device responded with an error to CMD6 command.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond to CMD6 command.
 *
 * @note       Device must support HPI (hpi_supported = true) before calling this function.
 * @note       Function sets hpi_enabled flag to true upon successful completion.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_enable_hpi (mmca_instance_ctrl_t * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    if (false == p_ctrl->device.hpi_supported)
    {
        return FSP_ERR_UNSUPPORTED;
    }

    /* Send CMD6 to enable HPI */
    uint32_t arg = ((uint32_t) MMCA_PRV_EMMC_EXT_CSD_ARGUMENT_SET_MODE << MMCA_BYTE_SHIFT_24) |
                   ((uint32_t) MMCA_PRV_EMMC_EXT_CSD_HPI_MGMT_OFFSET << MMCA_BYTE_SHIFT_16) |
                   ((uint32_t) MMCA_PRV_HPI_EN_BIT << MMCA_BYTE_SHIFT_8);

    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SWITCH_RES_BSY, arg);

    if (FSP_SUCCESS == err)
    {
        /* Wait for eMMC bus to become not busy after switch operation */
        FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Mark HPI as enabled in device state */
        p_ctrl->device.hpi_enabled = true;
    }

    return err;
}

#endif

/*******************************************************************************************************************//**
 * Get current eMMC card state from device status register.
 *
 * This function sends CMD13 (SEND_STATUS) to query the eMMC device status and extracts the current
 * state field from the R1 response. The card state indicates the current operation mode such as
 * Standby, Transfer, Data, Receive, Program, or Disconnect states according to eMMC specification.
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 *
 * @note       Card states: 0=Idle, 1=Ready, 2=Ident, 3=Standby, 4=Transfer, 5=Data, 6=Receive, 7=Program, 8=Disconnect.
 * @note       Function returns MMCA_PRV_CARD_STATE_INVALID if CMD13 fails or card is in error condition.
 **********************************************************************************************************************/
static uint8_t r_mmca_get_card_state (mmca_instance_ctrl_t * const p_ctrl)
{
    uint8_t   card_status;
    fsp_err_t err;

    /* Send CMD13 (SEND_STATUS) to get current card status */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SEND_STATUS, MMCA_PRV_RCA_STUFF_BITS);
    if (FSP_SUCCESS != err)
    {
        /* Return invalid state if command failed */
        card_status = MMCA_PRV_CARD_STATE_INVALID;
    }
    else
    {
        /* Extract current state from R1 response bits [12:9] */
        card_status =
            (uint8_t) ((p_ctrl->p_reg->CE_RESP0 & MMCA_PRV_R1_CURRENT_STATE_MASK) >> MMCA_PRV_R1_CURRENT_STATE_SHIFT); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Return card state */
    return card_status;
}

/*******************************************************************************************************************//**
 * Get and store relevant extended card specific data.
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 * @param[out] extcsd_out           Pointer to buffer where Extended CSD data (512 bytes) will be stored.
 * @retval     FSP_SUCCESS          Device type obtained from eMMC extended CSD.  Sector count is also calculated here
 *                                  if it was not determined previously.
 * @retval     FSP_ERR_RESPONSE     Device responded with an error.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_csd_extended_get (mmca_instance_ctrl_t * const p_ctrl, uint8_t * extcsd_out)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t  data_reg;
    uint32_t  card_status;

    /* Parameter validation */
    if ((NULL == p_ctrl) || (NULL == extcsd_out))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    /* Enable swap-bit */
    p_ctrl->p_reg->CE_BUF_ACC_b.ATYP = 1U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Check card state before CMD8 - must be in TRANSFER state (4) */
    card_status = r_mmca_get_card_state(p_ctrl);
    if (MMCA_PRV_CARD_STATE_INVALID == card_status)
    {
        return FSP_ERR_INVALID_STATE;
    }

    if (MMCA_PRV_CARD_STATE_TRAN != card_status)
    {
        return FSP_ERR_INVALID_MODE;                            // Card not in TRANSFER state
    }

    /* Set block count = 1 */
    p_ctrl->p_reg->CE_BLOCK_SET_b.BLKCNT = 1;                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg->CE_BLOCK_SET_b.BLKSIZ = MMCA_MAX_BLOCK_SIZE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /*Wait card ready for transfer*/
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);

    /* Send CMD8 (SEND_EXT_CSD) - Get Extended CSD data */
    err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SEND_EXT_CSD, 0);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Enable data receive interrupts */
    p_ctrl->p_reg->CE_INT_EN = MMCA_PRV_INT_EN_READ_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Wait until buffer is ready */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_READ_READY);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Read 512 bytes of EXT_CSD data */
    /* Each read from CE_DATA gives 32-bit (4 bytes) */
    for (uint32_t sector_index = 0U; sector_index < MMCA_MAX_BLOCK_SIZE; sector_index += 4U)
    {
        /* Read 32-bit data from CE_DATA register */
        data_reg = p_ctrl->p_reg->CE_DATA; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Convert 32-bit data to 4 bytes in little-endian order */
        extcsd_out[sector_index]      = (uint8_t) (data_reg & MMCA_MASK_8BITS);
        extcsd_out[sector_index + 1U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_8);
        extcsd_out[sector_index + 2U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_16);
        extcsd_out[sector_index + 3U] = (uint8_t) (data_reg >> MMCA_BYTE_SHIFT_24);
    }

    /* Verify the device is not busy. */
    err = r_mmca_wait_for_event(p_ctrl, MMCA_EVENT_BUFFER_READ_COMPLETE);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Disable data receive interrupts */
    p_ctrl->p_reg->CE_INT_EN = 0U;         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Disable swap-bit */
    p_ctrl->p_reg->CE_BUF_ACC_b.ATYP = 0U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return err;
}

#if MMCA_CFG_NON_BLOCK_ENABLE

/*******************************************************************************************************************//**
 * Write one block of data to eMMC device via CE_DATA register.
 *
 * This function is called from ISR context to write a single block of data to the eMMC device.
 * It handles both single-block and multi-block write scenarios by writing data in 4-byte chunks
 * for optimal performance. For multi-block operations, it writes exactly 512 bytes per block.
 * For the last block, it writes only the remaining bytes to avoid buffer overflow.
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 *
 * @note       This function uses global variables for data source pointer and remaining counters.
 *             - gp_mmca_tx_src: Source data pointer (updated after each write)
 *             - g_mmca_blocks_remaining: Number of blocks left to write
 *             - g_mmca_tx_bytes_remaining: Total bytes remaining to write
 * @note       Function returns early if source pointer is NULL or no blocks remaining.
 * @note       Data is packed into little-endian 32-bit words before writing to CE_DATA register.
 **********************************************************************************************************************/
static void r_mmca_write_one_block (mmca_instance_ctrl_t * const p_ctrl)
{
    uint32_t bytes_to_write;
    uint32_t words_to_write;
    uint32_t remaining_bytes;
    uint32_t byte_index;

    /* Check if transmit source buffer is valid */
    if (NULL == p_ctrl->p_mmca_tx_src)
    {

        /* No data to write, exit early */
        return;
    }

    /* Check if there are blocks remaining to write */
    if (0U == p_ctrl->mmca_blocks_remaining)
    {

        /* All blocks completed, exit early */
        return;
    }

    /* Determine how many bytes to write for current block */
    if (1U < p_ctrl->mmca_blocks_remaining)
    {
        /* Multi-block: always write exactly 512 bytes per block */
        bytes_to_write = MMCA_MAX_BLOCK_SIZE;
    }
    else
    {
        /* Last block (single or final block of multi): write remaining bytes */
        bytes_to_write = p_ctrl->mmca_tx_bytes_remaining;
    }

    /* Write data in 4-byte (32-bit) chunks for efficiency */
    words_to_write = bytes_to_write / 4U;

    for (byte_index = 0; byte_index < words_to_write; byte_index++)
    {
        /* Pack 4 bytes into little-endian 32-bit word */
        uint32_t word = ((uint32_t) p_ctrl->p_mmca_tx_src[0]) |
                        ((uint32_t) p_ctrl->p_mmca_tx_src[1] << MMCA_BYTE_SHIFT_8) |
                        ((uint32_t) p_ctrl->p_mmca_tx_src[2] << MMCA_BYTE_SHIFT_16) |
                        ((uint32_t) p_ctrl->p_mmca_tx_src[3] << MMCA_BYTE_SHIFT_24);

        /* Write to CE_DATA - hardware routes to available internal buffer */
        p_ctrl->p_reg->CE_DATA = word;        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Update pointers and counters */
        p_ctrl->p_mmca_tx_src           += 4; /* PRQA S 0488 # Advisory R-18.4 */
        p_ctrl->mmca_tx_bytes_remaining -= 4U;
    }

    /* Handle remaining bytes (1-3 bytes) for non-aligned block sizes */
    remaining_bytes = bytes_to_write % 4U;

    if (0U < remaining_bytes)
    {
        uint32_t last_word = 0;

        /* Pack remaining bytes into 32-bit word */
        for (uint32_t block_index = 0; block_index < remaining_bytes; block_index++)
        {
            last_word |= ((uint32_t) p_ctrl->p_mmca_tx_src[block_index]) << (block_index * 8U);
        }

        /* Write final word */
        p_ctrl->p_reg->CE_DATA = last_word;                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Update pointers and counters */
        p_ctrl->p_mmca_tx_src           += remaining_bytes; /* PRQA S 0488 # Advisory R-18.4 */
        p_ctrl->mmca_tx_bytes_remaining -= remaining_bytes;
    }

    /* One block completed */
    p_ctrl->mmca_blocks_remaining--;

    /* Clear transmit buffer pointer when all blocks are written */
    if (0U == p_ctrl->mmca_blocks_remaining)
    {
        p_ctrl->p_mmca_tx_src = NULL;
    }
}

#endif

/*******************************************************************************************************************//**
 * Reads boot-related information from EXT_CSD.
 *
 * @param[in]  p_api_ctrl               Pointer to MMCA control block.
 * @param[out] p_boot_info              Pointer to structure to receive the boot information.
 *
 * @retval     FSP_SUCCESS               Boot information read successfully.
 * @retval     FSP_ERR_ASSERTION         A required pointer is NULL.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_read_boot_info (mmca_ctrl_t * const p_api_ctrl, mmca_boot_info_t * p_boot_info)
{
    mmca_instance_ctrl_t * p_ctrl = (mmca_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t              err    = FSP_SUCCESS;
    uint8_t                extcsd_buf[MMCA_MAX_BLOCK_SIZE];

    /* Get Extended CSD Register */
    err = r_mmca_csd_extended_get(p_ctrl, extcsd_buf);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* BOOT_SIZE_MULTI: size = value x 128 KB */
    p_boot_info->boot_size = (uint32_t) extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_BOOT_SIZE_MULTI_OFFSET] *
                             MMCA_PRV_BOOT_128KB_SIZE;

    /* BOOT_CONFIG: [6]=ACK, [5:3]=BOOT_PARTITION_ENABLE, [2:0]=BOOT_PARTITION_ACCESS */
    uint8_t boot_cfg = extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_BOOT_CONFIG_OFFSET];
    p_boot_info->boot_ack  = ((boot_cfg & MMCA_PRV_BOOT_ACK_BIT) >> MMCA_BYTE_SHIFT_6);
    p_boot_info->boot_enab = ((boot_cfg & MMCA_PRV_BOOT_PARTITION_ENABLE) >> MMCA_BYTE_SHIFT_3);
    p_boot_info->boot_acc  = (boot_cfg & MMCA_PRV_BOOT_PARTITION_ACCESS);

    /* BOOT_BUS_WIDTH: [5:3]=reset width, [2:0]=current width */
    uint8_t bus_width = extcsd_buf[MMCA_PRV_EMMC_EXT_CSD_BOOT_BUS_WIDTH_OFFSET];
    p_boot_info->reset_boot_width = ((bus_width & MMCA_PRV_BOOT_PARTITION_ENABLE) >> MMCA_BYTE_SHIFT_3);
    p_boot_info->boot_width       = (bus_width & MMCA_PRV_BOOT_PARTITION_ACCESS);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Write a single byte to the Extended CSD register using CMD6 (SWITCH).
 *
 * @param[in]  p_ctrl               Pointer to the instance control block.
 * @param[in]  offset               Offset within the Extended CSD register (0..511).
 * @param[in]  value                Value to write at the specified offset.
 *
 * @retval     FSP_SUCCESS          Extended CSD byte written successfully.
 * @retval     FSP_ERR_TIMEOUT      Device did not respond.
 * @retval     FSP_ERR_DEVICE_BUSY  Device is holding DAT0 low (device is busy) or another operation is ongoing.
 * @retval     FSP_ERR_INVALID_ARG  Invalid argument(s) provided.
 **********************************************************************************************************************/
static fsp_err_t r_mmca_extcsd_write_byte (mmca_instance_ctrl_t * const p_ctrl, uint8_t offset, uint8_t value)
{
    uint32_t arg = MMCA_PRV_EMMC_SWITCH_ARG_WRITE_BYTE(offset, value);       /* PRQA S 3469 # Advisory D-4.9 */

    fsp_err_t err = r_mmca_command_send(p_ctrl, MMCA_PRV_CMD_SWITCH_RES_BSY, arg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_HARDWARE_REGISTER_WAIT(p_ctrl->p_reg->CE_CLK_CTRL_b.MMCABUSBSY, 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return FSP_SUCCESS;
}
