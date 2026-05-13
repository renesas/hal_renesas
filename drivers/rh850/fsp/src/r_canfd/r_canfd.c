/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_canfd.h"
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define CANFD_OPEN                             (0x52434644U) // "RCFD" in ASCII

#define CANFD_BAUD_RATE_PRESCALER_MIN          (1U)
#define CANFD_BAUD_RATE_PRESCALER_MAX          (1024U)

#define CANFD_PRV_CTR_MODE_MASK                (R_CANFD0_CFDGCTR_GSLPR_Msk | R_CANFD0_CFDGCTR_GMDC_Msk)
#define CANFD_PRV_CTR_RESET_BIT                (1U)
#define CANFD_PRV_OPERATION_MODE_MASK          (0x0FU)

#define CANFD_PRV_RX_FIFO_MAX                  (8U)
#define CANFD_PRV_RX_BUFFER_RAM_LIMIT_BYTES    (4864U)
#define CANFD_PRV_RXMB_MAX                     (96U)
#define CANFD_PRV_TXMB_CHANNEL_OFFSET          (64UL)
#define CANFD_PRV_TXMB_OFFSET_IN_CHANNEL       (32UL)
#define CANFD_PRV_STANDARD_ID_MAX              (0x7FFU)
#define CANFD_PRV_CFDFESTS_RFXEMP_MSK          (0xFFUL)
#define CANFD_PRV_CFDTM_CH_LENGTH              (2UL)

#define CANFD_PRV_TX_QUEUE_WINDOW              (31UL)

#define CANFD_PRV_OFFSET_TXQ_ML                (4UL)
#define CANFD_PRV_TXQ_MSK                      (0xfUL)
#define CANFD_PRV_BUFFER_NUM_MASK              (0x00ffUL)
#define CANFD_PRV_BUFFER_TYPE_MASK             (0xff00UL)

#define CANFD_PRV_UPDATE_POINTER_CMD           (0xffUL)

#define CANFD_PRV_ERROR_CH_FLAG_REG_MASK       (0xffUL)
#define CANFD_PRV_ERROR_GL_FLAG_REG_MASK       (0xffUL)
#define CANFD_PRV_VM_ERR_REG_MASK              (0xff0000UL)

#define CANFD_PRV_ERROR_FD_ENUM_POS            (16UL)
#define CANFD_PRV_ERROR_GLOBAL_ENUM_POS        (20UL)

#define CANFD_PRV_CFIFO_OFFSET                 (BSP_FEATURE_CANFD_NUM_RXFIFO)
#define CANFD_PRV_CAN_HUB_OFFSET               (16UL)
#if (BSP_FEATURE_CANFD_TXMB_MAX_NUM == CANFD_PRV_TXMB_CHANNEL_OFFSET)
 #define CANFD_PRV_TXMB_LOW_BUFFER_NUM         (32UL)
#else
 #define CANFD_PRV_TXMB_LOW_BUFFER_NUM         (16UL)
#endif
#define CANFD_PRV_TXMB_HIGH_BUFFER_NUM         (48UL)

/* Valid txmb is 0 - (LOW_BUFFER_NUM - 1) and 32 - (HIGH_BUFFER_NUM - 1) */
#define CANFD_PRV_VALID_TXMB_RANGE(txmb)    (BSP_FEATURE_CANFD_TXMB_SUPPORTED_SLOT & (1ULL << txmb))

#define CANFD_PRV_TXQ_INDEX_0                  (0UL)
#define CANFD_PRV_TXQ_INDEX_1                  (1UL)
#define CANFD_PRV_TXQ_INDEX_2                  (2UL)
#define CANFD_PRV_TXQ_INDEX_3                  (3UL)

#define CANFD_PRV_TXQ_CH_OFFSET                (4UL)
#define CANFD_FLX_BUFF_CH_OFFSET               (8UL)

#define CANFD_PRV_TXMB_OUT_RANGE               (0xFFFFFFFF)

/* Get amount of lend buffer of the channel */
#define CANFD_PRV_LEND_BUFF_GET(reg, channel)    (((uint8_t *) &(reg->CFDGFTBAC))[(channel) / 2] * 4)

/* Fill n MSB bit 1 */
#define CANFD_PRV_FILL_1(n)                      ((1U << (n)) - 1)

#define CANFD_PRV_TX_QUEUE_2_WINDOW       (32UL)
#define CANFD_PRV_TX_QUEUE_3_WINDOW       (63UL)
#define CANFD_THIRTYTWO                   (32)
#define CANFD_MAX_UINT_7                  (0x7F)

#define CANFD_PRV_SEC_TO_NS               (1000000000U)

/* Macro for value of CFDCnFDCTR.KEY */
#define CANFD_PRV_PNMDC_KEY               0xC4000000UL

/* CFDGFFIMC KEY */
#define CANFD_PRV_CFDGFFIMC_KEY           (0xC4UL << 8UL)
#define CANFD_PRV_CFDGFFIMC_FFIEN         (0x01UL)
#define CANFD_PRV_CFDGFFIMC_FFIEN_OFF     (0x00UL)
#define CANFD_PRV_CHANNELS_OFFSET         (8)

/* LOCKK KEY */
#define CANFD_GLOCKK_UNLOCK_KEY1          0x00007575UL
#define CANFD_GLOCKK_UNLOCK_KEY2          0x00008A8AUL

#define CANFD_RAM_TEST_SET_DATA           0xFFFFFFFFUL
#define CANFD_RAM_TEST_CLEAR_DATA         0x00000000UL

#define CANFD_RAM_PAGE_SIZE_ACCESS_REG    (64UL)
#define CANFD_RAM_TEST_DATA               (0xAAAAAAAAUL)

#define CANFD_RAM_TEST_CELL_DIVIDER       (0x05U)
#define CANFD_RAM_NUM_BYTES_PER_PAGE      (256)
#define CANFD_ONE                         (1U)
#define CANFD_PRV_DATA_LENGTH_CODE_0      (0U)
#define CANFD_PRV_DATA_LENGTH_CODE_8      (8U)
#define CANFD_PRV_DATA_LENGTH_CODE_9      (9U)
#define CANFD_PRV_DATA_LENGTH_CODE_15     (15U)

/***********************************************************************************************************************
 * Const data
 **********************************************************************************************************************/

/* LUT to convert DLC values to payload size in bytes */
static const uint8_t dlc_to_bytes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

#if CANFD_CFG_PARAM_CHECKING_ENABLE

/* LUT to determine the hierarchy of can_operation_mode_t modes. */
static const uint8_t g_mode_order[] = {0, 2, 1, 0, 0, 3};
#endif

/* LUT to convert Transmit Queue into TXMB access windows */
static const uint8_t g_queue_window[] = {0, 31, 32, 63};

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if CANFD_CFG_PARAM_CHECKING_ENABLE
static bool        r_canfd_bit_timing_nominal_parameter_check(can_bit_timing_cfg_t * p_bit_timing);
static bool        r_canfd_bit_timing_data_parameter_check(can_bit_timing_cfg_t * p_bit_timing);
static fsp_err_t   r_canfd_validate_tx_buffer(canfd_instance_ctrl_t * p_ctrl, can_buffer_t buffer);
static fsp_err_t   r_canfd_validate_rx_buffer(const can_cfg_t * p_cfg, can_buffer_t buffer);
static inline bool r_canfd_cfifo_link_queue_check(const can_cfg_t * p_cfg);
static inline bool r_canfd_cfifo_same_link_check(canfd_cfifo_cfg_t cfifo_cfg_0, canfd_cfifo_cfg_t cfifo_cfg_1);

#endif

static void r_canfd_mb_read(uint32_t             channel,
                            can_buffer_t         buffer,
                            can_frame_t * const  p_frame,
                            canfd_global_cfg_t * p_global_cfg);
static void    r_canfd_call_callback(canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args);
static void    r_canfd_mode_transition(canfd_instance_ctrl_t * p_ctrl, can_operation_mode_t operation_mode);
static void    r_canfd_mode_ctr_set(volatile uint32_t * p_ctr_reg, can_operation_mode_t operation_mode);
static uint8_t r_canfd_bytes_to_dlc(uint16_t bytes);
static bool    r_canfd_is_all_channel_close(uint8_t unit);
static void    r_canfd_handle_txhistory(canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args);

static inline void r_canfd_thl_handle_txqueue(canfd_instance_ctrl_t * const p_ctrl,
                                              can_callback_args_t * const   p_args,
                                              uint32_t                      buffer_num,
                                              uint32_t                      channel);
static inline void r_canfd_thl_handle_txfifo(canfd_instance_ctrl_t * const p_ctrl,
                                             can_callback_args_t * const   p_args,
                                             uint32_t                      buffer_num,
                                             uint32_t                      channel);
static inline void r_canfd_thl_handle_txmb(canfd_instance_ctrl_t * const p_ctrl,
                                           can_callback_args_t * const   p_args,
                                           uint32_t                      buffer_num,
                                           uint32_t                      channel);
static void r_canfd_handle_txmb(canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args, can_event_t event);
static void r_canfd_handle_txqueue(canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args);
static void r_canfd_handle_txcfifo(canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args);
static void r_canfd_handle_txhistory(canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args);

#ifdef BSP_FEATURE_CANFD_FFI_MODE
static void r_canfd_handle_txqueue_ffi_error(canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args);

#endif

static void r_canfd_handle_txqueue_error(canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args);

static void r_canfd_setting_filter_rule(canfd_instance_ctrl_t * p_ctrl);

#ifdef BSP_FEATURE_CANFD_PN_MODE
static void r_canfd_copy_pnf_rule(volatile R_CANFD0_CFDGPFL_Type * p_reg, const canfd_pnf_entry_t * p_entry);

#endif
static void      r_canfd_set_ram_data(canfd_global_cfg_t * p_global_cfg, canfd_ramtest_data_t data_ram);
static fsp_err_t r_canfd_ram_check_algorithm(canfd_global_cfg_t * p_global_cfg);
static fsp_err_t r_canfd_ram_walkpath_algorithm(canfd_global_cfg_t      * p_global_cfg,
                                                canfd_ramtest_walk_type_t walk_type);
static inline uint32_t r_canfd_caculate_no_channel(uint8_t unit);
static uint32_t        r_canfd_ram_caculate_max_page(uint8_t unit, canfd_ramtest_field_t ram_field);
static uint32_t        r_canfd_ram_caculate_limit_byte(canfd_global_cfg_t * p_global_cfg);
static uint32_t        r_canfd_ram_caculate_max_page(uint8_t unit, canfd_ramtest_field_t ram_field);

BSP_INTERRUPT_ATTRIBUTE void canfd_global_error_isr(void);
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_error_isr(void);
BSP_INTERRUPT_ATTRIBUTE void canfd_rx_fifo_isr(void);
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_tx_isr(void);
BSP_INTERRUPT_ATTRIBUTE void canfd_rx_common_fifo_isr(void);
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_tx_vm_isr(void);
BSP_INTERRUPT_ATTRIBUTE void canfd_vm_rx_isr(void);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* Channel control struct array */
static canfd_instance_ctrl_t * gp_ctrl[BSP_FEATURE_CANFD_NUM_CHANNELS] = {NULL};

/* CAN function pointers   */
const can_api_t g_can_on_canfd =
{
    .open           = R_CANFD_Open,
    .close          = R_CANFD_Close,
    .write          = R_CANFD_Write,
    .read           = R_CANFD_Read,
    .modeTransition = R_CANFD_ModeTransition,
    .infoGet        = R_CANFD_InfoGet,
    .callbackSet    = R_CANFD_CallbackSet,
};

/*******************************************************************************************************************//**
 * @addtogroup CANFD
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***************************************************************************************************************//**
 * Open and configure the CANFD channel for operation.
 *
 * @retval FSP_SUCCESS                            Channel opened successfully.
 * @retval FSP_ERR_ALREADY_OPEN                   Driver already open.
 * @retval FSP_ERR_IN_USE                         Channel is already in use.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT         Channel does not exist on this MCU.
 * @retval FSP_ERR_ASSERTION                      A required pointer was NULL.
 * @retval FSP_ERR_CAN_INIT_FAILED                The provided nominal or data bitrate is invalid.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Open (can_ctrl_t * const p_api_ctrl, can_cfg_t const * const p_cfg)
{
    canfd_instance_ctrl_t  * p_ctrl;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_api_ctrl);
    FSP_ASSERT(p_cfg);
    FSP_ASSERT(p_cfg->p_extend);
    FSP_ASSERT(p_cfg->p_callback);
    FSP_ASSERT(p_cfg->p_bit_timing);

    /* Get control block */
    p_ctrl   = (canfd_instance_ctrl_t *) p_api_ctrl;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    FSP_ASSERT(p_extend->p_global_cfg);
    p_global_cfg = p_extend->p_global_cfg;

    /*Take index for channel when device have more than one unit*/
    p_ctrl->channel_index = p_cfg->channel - CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit;
    channel_index         = p_ctrl->channel_index;

    /* Check that the module is not open, the channel is present and that it is not in use */
    FSP_ERROR_RETURN(CANFD_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ERROR_RETURN(p_cfg->channel < BSP_FEATURE_CANFD_NUM_CHANNELS, FSP_ERR_IP_CHANNEL_NOT_PRESENT);
    FSP_ERROR_RETURN(NULL == gp_ctrl[p_cfg->channel], FSP_ERR_IN_USE);

    /* Check that mandatory interrupts are enabled */
    FSP_ERROR_RETURN(p_global_cfg->rx_fifo_irq >= 0, FSP_ERR_CAN_INIT_FAILED);
    FSP_ERROR_RETURN(p_global_cfg->err_glb_irq >= 0, FSP_ERR_CAN_INIT_FAILED);

    /* Check nominal bit timing parameters for correctness */
    FSP_ERROR_RETURN(r_canfd_bit_timing_nominal_parameter_check(p_cfg->p_bit_timing), FSP_ERR_CAN_INIT_FAILED);

    /* Check that bit timing for FD bitrate switching is present and correct */
    can_bit_timing_cfg_t * p_data_timing = p_extend->p_data_timing;
    FSP_ASSERT(p_data_timing);
    FSP_ERROR_RETURN(r_canfd_bit_timing_data_parameter_check(p_data_timing), FSP_ERR_CAN_INIT_FAILED);

    can_bit_timing_cfg_t * p_bit_timing = p_cfg->p_bit_timing;

    /* Check that data rate > nominal rate */
    uint32_t data_rate_clocks = p_data_timing->baud_rate_prescaler *
                                (p_data_timing->time_segment_1 + p_data_timing->time_segment_2 + 1U);
    uint32_t nominal_rate_clocks = p_bit_timing->baud_rate_prescaler *
                                   (p_bit_timing->time_segment_1 + p_bit_timing->time_segment_2 + 1U);
    FSP_ERROR_RETURN(data_rate_clocks <= nominal_rate_clocks, FSP_ERR_CAN_INIT_FAILED);

    /* Set txmb valid range */
    uint8_t lend_txmb = p_global_cfg->flex_tx_buffer[channel_index / 2] * 4;
    if (0 == lend_txmb)
    {
        /* Flexible buffer assignment is disabled on this channel */
        p_ctrl->txmb_range_min    = 0;
        p_ctrl->txmb_range_max    = CANFD_PRV_TXMB_CHANNEL_OFFSET;
        p_ctrl->txqueue_range_min = 0;
        p_ctrl->txqueue_range_max = CANFD_PRV_TXQ_CH_OFFSET;
    }
    else if (0 == (p_cfg->channel % 2))
    {
        /* Even channel lend buffer from the consecutive odd channel */
        p_ctrl->txmb_range_min = 0;
        p_ctrl->txmb_range_max = CANFD_PRV_TXMB_CHANNEL_OFFSET + lend_txmb;

        /* If even channel lend 16 buffer, It will also lend 2 transmit queue.
         * Else It only lend 1 transmit queue */
        p_ctrl->txqueue_range_min = 0;
        p_ctrl->txqueue_range_max = CANFD_PRV_TXMB_LOW_BUFFER_NUM == lend_txmb ?
                                    CANFD_PRV_TXQ_CH_OFFSET + 2 : CANFD_PRV_TXQ_CH_OFFSET + 1;
    }
    else
    {
        /* Odd channel txmb is borrowed */
        p_ctrl->txmb_range_min = lend_txmb;
        p_ctrl->txmb_range_max = CANFD_PRV_TXMB_CHANNEL_OFFSET;

        /* If even channel lend 16 buffer, It will also lend 2 transmit queue.
         * Else It only lend 1 transmit queue */
        p_ctrl->txqueue_range_min = CANFD_PRV_TXMB_LOW_BUFFER_NUM == lend_txmb ? 2 : 1;
        p_ctrl->txqueue_range_max = CANFD_PRV_TXQ_CH_OFFSET;
    }

    /* Configure Transmit Queue */
    for (uint8_t tx_queue_index = 0; tx_queue_index < CANFD_TX_QUEUE_NUM; tx_queue_index++)
    {
        if (p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable)
        {
            /* Check Transmit Queue is handled by this channel or not */
            FSP_ERROR_RETURN((tx_queue_index >= p_ctrl->txqueue_range_min) &&
                             (tx_queue_index < p_ctrl->txqueue_range_max),
                             FSP_ERR_CAN_INIT_FAILED);

            /* Check Transmit Queue depth > 2 when enable transmit queue */
            FSP_ERROR_RETURN((p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.depth >= 2U),
                             FSP_ERR_CAN_INIT_FAILED);
        }
    }

    /* Check transmit buffers configuration is valid */
    FSP_ERROR_RETURN(r_canfd_cfifo_link_queue_check(p_cfg), FSP_ERR_CAN_INIT_FAILED);
    uint32_t cfifo_start_index = channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;
    FSP_ERROR_RETURN(r_canfd_cfifo_same_link_check(p_global_cfg->common_fifo_config[cfifo_start_index + 0],
                                                   p_global_cfg->common_fifo_config[cfifo_start_index + 1]),
                     FSP_ERR_CAN_INIT_FAILED);
    FSP_ERROR_RETURN(r_canfd_cfifo_same_link_check(p_global_cfg->common_fifo_config[cfifo_start_index + 0],
                                                   p_global_cfg->common_fifo_config[cfifo_start_index + 2]),
                     FSP_ERR_CAN_INIT_FAILED);
    FSP_ERROR_RETURN(r_canfd_cfifo_same_link_check(p_global_cfg->common_fifo_config[cfifo_start_index + 1],
                                                   p_global_cfg->common_fifo_config[cfifo_start_index + 2]),
                     FSP_ERR_CAN_INIT_FAILED);

    /* Maximum 4 flex mode windown per 8 channels */
    uint8_t max_channel_flex_tx = (BSP_FEATURE_CANFD_NUM_CHANNELS > 8) ? 8 : BSP_FEATURE_CANFD_NUM_CHANNELS;

    for (uint32_t flex_tx_assign = 0; flex_tx_assign < max_channel_flex_tx / 2; flex_tx_assign++)
    {
        /* Check Flexible transmission buffer assignment configured and Flexible CAN mode configured isn't used simultaneously.*/
 #ifdef BSP_FEATURE_CANFD_CAN_HUB_MODE
        if (p_global_cfg->can_hub_mode & (3U << (flex_tx_assign * 2 + CANFD_PRV_CAN_HUB_OFFSET)))
        {
            FSP_ERROR_RETURN(0 == p_global_cfg->flex_tx_buffer[flex_tx_assign], FSP_ERR_CAN_INIT_FAILED);
        }

 #else
        if (p_global_cfg->flexible_mode & (1U << flex_tx_assign))
        {
            FSP_ERROR_RETURN(0 == p_global_cfg->flex_tx_buffer[flex_tx_assign], FSP_ERR_CAN_INIT_FAILED);
        }
 #endif
    }

    /* Check Common FIFOs config */
#else
    p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /*Take index for channel when device have more than one unit*/
    p_ctrl->channel_index = p_cfg->channel - CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit;
    channel_index         = p_ctrl->channel_index;

    /* Set txmb and tx queue valid range */
    uint8_t lend_txmb = p_global_cfg->flex_tx_buffer[channel_index / 2] * 4;
    if (0 == lend_txmb)
    {
        /* Flexible buffer assignment is disabled on this channel */
        p_ctrl->txmb_range_min    = 0;
        p_ctrl->txmb_range_max    = CANFD_PRV_TXMB_CHANNEL_OFFSET;
        p_ctrl->txqueue_range_min = 0;
        p_ctrl->txqueue_range_max = CANFD_PRV_TXQ_CH_OFFSET;
    }
    else if (0 == (p_cfg->channel % 2))
    {
        /* Even channel lend buffer from the consecutive odd channel */
        p_ctrl->txmb_range_min = 0;
        p_ctrl->txmb_range_max = CANFD_PRV_TXMB_CHANNEL_OFFSET + lend_txmb;

        /* If even channel lend 16 buffer, It will also lend 2 transmit queue.
         * Else It only lend 1 transmit queue */
        p_ctrl->txqueue_range_min = 0;
        p_ctrl->txqueue_range_max = CANFD_PRV_TXMB_LOW_BUFFER_NUM == lend_txmb ?
                                    CANFD_PRV_TXQ_CH_OFFSET + 2 : CANFD_PRV_TXQ_CH_OFFSET + 1;
    }
    else
    {
        /* Odd channel txmb is borrowed */
        p_ctrl->txmb_range_min = lend_txmb;
        p_ctrl->txmb_range_max = CANFD_PRV_TXMB_CHANNEL_OFFSET;

        /* If even channel lend 16 buffer, It will also lend 2 transmit queue.
         * Else It only lend 1 transmit queue */
        p_ctrl->txqueue_range_min = CANFD_PRV_TXMB_LOW_BUFFER_NUM == lend_txmb ? 2 : 1;
        p_ctrl->txqueue_range_max = CANFD_PRV_TXQ_CH_OFFSET;
    }
#endif

    fsp_err_t err = FSP_SUCCESS;

    /* Initialize the control block */
    p_ctrl->p_cfg = p_cfg;

    /* Set callback and context pointers, if configured */
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /* Start module */

    /* Perform global config only if the module is in Global Sleep or Global Reset */
    if (p_canfd_reg->CFDGSTS & R_CANFD0_CFDGSTS_GRSTSTS_Msk)
    {
        /* Wait for RAM initialization
         *(see RH850 microcontroller User's Manual section "Timing of Global Mode Change") */
        FSP_HARDWARE_REGISTER_WAIT((p_canfd_reg->CFDGSTS & R_CANFD0_CFDGSTS_GRAMINIT_Msk), 0);

        /* Cancel Global Sleep and wait for transition to Global Reset */
        r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);

        /* Configure global TX priority, DLC check/replace functions, external/internal clock select and payload
         * overflow behavior */
        p_canfd_reg->CFDGCFG = p_global_cfg->global_config |
                               (p_global_cfg->timestamp_config.clock_div << R_CANFD0_CFDGCFG_TSP_Pos) |
                               (p_global_cfg->timestamp_config.clock_source << R_CANFD0_CFDGCFG_TSSS_Pos);

        /* Set CAN FD Protocol Exception response (ISO exception state or send error frame)
         * and time stamp capture config */
        p_canfd_reg->CFDGFDCFG =
            (p_global_cfg->timestamp_config.capture_cfg << R_CANFD0_CFDGFDCFG_TSCCFG_Pos) |
            (p_global_cfg->prt_exception << R_CANFD0_CFDGFDCFG_RPED_Pos);

        /* Configure AFL rule count for maximum 8 channels */
        p_canfd_reg->CFDGAFLCFG[0] = (p_global_cfg->afl_rule_num[1] << R_CANFD0_CFDGAFLCFG_RNC1_Pos) |
                                     (p_global_cfg->afl_rule_num[0] << R_CANFD0_CFDGAFLCFG_RNC0_Pos);
        p_canfd_reg->CFDGAFLCFG[1] = (p_global_cfg->afl_rule_num[3] << R_CANFD0_CFDGAFLCFG_RNC1_Pos) |
                                     (p_global_cfg->afl_rule_num[2] << R_CANFD0_CFDGAFLCFG_RNC0_Pos);
        p_canfd_reg->CFDGAFLCFG[2] = (p_global_cfg->afl_rule_num[5] << R_CANFD0_CFDGAFLCFG_RNC1_Pos) |
                                     (p_global_cfg->afl_rule_num[4] << R_CANFD0_CFDGAFLCFG_RNC0_Pos);
        p_canfd_reg->CFDGAFLCFG[3] = (p_global_cfg->afl_rule_num[7] << R_CANFD0_CFDGAFLCFG_RNC1_Pos) |
                                     (p_global_cfg->afl_rule_num[6] << R_CANFD0_CFDGAFLCFG_RNC0_Pos);
#ifdef BSP_FEATURE_CANFD_PN_MODE

        /* Configure PNF rule count for maximum 8 channels */
        p_canfd_reg->CFDGPFLCFG[0] = (p_global_cfg->pnf_rule_num[1] << R_CANFD0_CFDGPFLCFG_RNC1_Pos) |
                                     (p_global_cfg->pnf_rule_num[0] << R_CANFD0_CFDGPFLCFG_RNC0_Pos) |
                                     (p_global_cfg->pnf_rule_num[3] << R_CANFD0_CFDGPFLCFG_RNC3_Pos) |
                                     (p_global_cfg->pnf_rule_num[2] << R_CANFD0_CFDGPFLCFG_RNC2_Pos);
        p_canfd_reg->CFDGPFLCFG[1] = (p_global_cfg->pnf_rule_num[5] << R_CANFD0_CFDGPFLCFG_RNC1_Pos) |
                                     (p_global_cfg->pnf_rule_num[4] << R_CANFD0_CFDGPFLCFG_RNC0_Pos) |
                                     (p_global_cfg->pnf_rule_num[7] << R_CANFD0_CFDGPFLCFG_RNC3_Pos) |
                                     (p_global_cfg->pnf_rule_num[6] << R_CANFD0_CFDGPFLCFG_RNC2_Pos);
#endif

        /* Set number and size of RX message buffers */
        p_canfd_reg->CFDRMNB = p_global_cfg->rx_mb_config;

        /* Configure RX FIFOs and interrupt */
        for (uint32_t i = 0; i < CANFD_PRV_RX_FIFO_MAX; i++)
        {
            if (1 == (p_global_cfg->rx_fifo_config[i] & R_CANFD0_CFDRFCC_RFE_Msk))
            {
                p_canfd_reg->CFDRFCC[i] = p_global_cfg->rx_fifo_config[i];
            }
        }

        /* Configure global RXFIFO interrupt */
        R_BSP_IrqCfgEnable(p_global_cfg->rx_fifo_irq, p_global_cfg->rx_fifo_ipl, p_ctrl);

#ifdef BSP_FEATURE_CANFD_FFI_MODE

        /* Initialization of global virtual machine  */
        if (CANFD_FFI_MODE_OPTIONS_ENABLE == p_global_cfg->canfd_ffi_mode)
        {
            /* Enable FFI mode for VM ISR */
            p_canfd_reg->CFDGFFIMC = (uint32_t) (CANFD_PRV_CFDGFFIMC_FFIEN | CANFD_PRV_CFDGFFIMC_KEY);

            /* Config Global virtual machine receive FIFO Configuration */
            p_canfd_reg->CFDVMRFCFG = p_global_cfg->ffi_fifo_vm;

            /* Config Global virtual machine error interrupt */
            p_canfd_reg->CFDGVMEIS = p_global_cfg->ffi_err_vm.ffi_error_cfg_u32;

            /* Config Global virtual machine Transmit/Receive FIFO and TXQ Configuration */
            uint32_t cfdvmcfg = 0;

            for (uint32_t txq_cfifo_channel = 0; txq_cfifo_channel < 8; txq_cfifo_channel++)
            {
                for (uint8_t tx_queue_index = p_ctrl->txqueue_range_min;
                     tx_queue_index < p_ctrl->txqueue_range_max;
                     tx_queue_index++)
                {
                    cfdvmcfg |= (p_global_cfg->ffi_txq_cfifo_vm[txq_cfifo_channel].ffi_txq_cfifo_cfg_u32) &
                                (R_CANFD0_CFDVMCFG_TXQ0VMN_Msk << (tx_queue_index * 4));
                }

                for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
                {
                    uint32_t cfifo_index = cfifo + txq_cfifo_channel * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;

                    if (p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable)
                    {
                        cfdvmcfg |=
                            (p_global_cfg->ffi_txq_cfifo_vm[txq_cfifo_channel].ffi_txq_cfifo_cfg_u32) &
                            (R_CANFD0_CFDVMCFG_CF0VMN_Msk << ((cfifo * 4U)));
                    }
                }

                p_canfd_reg->CFDVMCFG[txq_cfifo_channel] |= cfdvmcfg;

                cfdvmcfg = 0;
            }
        }
#endif

        /* Set global error interrupts */
        p_canfd_reg->CFDGCTR = p_global_cfg->global_interrupts;

        /* Configure common fifo */
        for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT; cfifo++)
        {
            if (p_global_cfg->common_fifo_config[cfifo].cfifo_cfg_b.enable)
            {
                p_canfd_reg->CFDCFCC[cfifo]  = p_global_cfg->common_fifo_config[cfifo].cfifo_cfg_u32;
                p_canfd_reg->CFDCFCCE[cfifo] = p_global_cfg->common_fifo_config_enhancement[cfifo].cfifo_cfg_enh_u32;
            }
        }

#ifdef BSP_FEATURE_CANFD_CAN_HUB_MODE

        /* Configure CAN HUB mode */
        p_canfd_reg->CFDGFCMC = p_global_cfg->can_hub_mode;
#else

        /* Configure Flexible Mode */
        p_canfd_reg->CFDGFCMC = p_global_cfg->flexible_mode;
#endif

        /* Configure Transmission buffer assignment*/
        for (uint32_t flex_tx_assign = 0; flex_tx_assign < CANFD_NUM_CHANNEL_PER_UNIT / 2; flex_tx_assign++)
        {
            p_canfd_reg->CFDGFTBAC |= p_global_cfg->flex_tx_buffer[flex_tx_assign] <<
                                      (flex_tx_assign * CANFD_FLX_BUFF_CH_OFFSET);
        }
    }

    if (p_global_cfg->channel_error == p_cfg->channel)
    {
        /* Configure global error interrupt */
        R_BSP_IrqCfgEnable(p_global_cfg->err_glb_irq, p_global_cfg->global_err_ipl, p_ctrl);
    }

    /* Track ctrl struct */
    gp_ctrl[p_cfg->channel] = p_ctrl;

    /* Setting filter rule */
    r_canfd_setting_filter_rule(p_ctrl);

    /* Cancel Channel Sleep and wait for transition to Channel Reset */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_RESET);

    if (p_extend->tx_history_cfg.enable)
    {
        /* Setting Transmit History for TXMB, TXFIFO, TXQueue*/
        p_canfd_reg->CFDTHLCC[channel_index] = p_extend->tx_history_cfg.enable << R_CANFD0_CFDTHLCC_THLDTE_Pos;
    }

    /* Configure bitrate */
    p_canfd_reg->CFDC[channel_index].NCFG =
        (uint32_t) (((p_cfg->p_bit_timing->baud_rate_prescaler - 1) & R_CANFD0_CFDC_NCFG_NBRP_Msk) <<
                    R_CANFD0_CFDC_NCFG_NBRP_Pos) |
        ((p_cfg->p_bit_timing->time_segment_1 - 1U) << R_CANFD0_CFDC_NCFG_NTSEG1_Pos) |
        ((p_cfg->p_bit_timing->time_segment_2 - 1U) << R_CANFD0_CFDC_NCFG_NTSEG2_Pos) |
        ((p_cfg->p_bit_timing->synchronization_jump_width - 1U) << R_CANFD0_CFDC_NCFG_NSJW_Pos);

    /* Configure data bitrate for rate switching on FD frames */
    p_canfd_reg->CFDC2[channel_index].DCFG =
        (uint32_t) (((p_extend->p_data_timing->baud_rate_prescaler - 1) & R_CANFD0_CFDC2_DCFG_DBRP_Msk) <<
                    R_CANFD0_CFDC2_DCFG_DBRP_Pos) |
        ((p_extend->p_data_timing->time_segment_1 - 1U) << R_CANFD0_CFDC2_DCFG_DTSEG1_Pos) |
        ((p_extend->p_data_timing->time_segment_2 - 1U) << R_CANFD0_CFDC2_DCFG_DTSEG2_Pos) |
        ((p_extend->p_data_timing->synchronization_jump_width - 1U) << R_CANFD0_CFDC2_DCFG_DSJW_Pos);

    /* Ensure transceiver delay offset is not larger than 7 bits */
    uint32_t tdco = p_extend->p_data_timing->time_segment_1;
    if (tdco > CANFD_MAX_UINT_7)
    {
        tdco = CANFD_MAX_UINT_7;
    }

    /* Configure transceiver delay compensation; allow user to set ESI bit manually
     */
    p_canfd_reg->CFDC2[channel_index].FDCFG =
        (tdco << R_CANFD0_CFDC2_FDCFG_TDCO_Pos) |
        (uint32_t) (p_extend->delay_compensation << R_CANFD0_CFDC2_FDCFG_TDCE_Pos) |
        R_CANFD0_CFDC2_FDCFG_ESIC_Msk | 1U |
        p_extend->gateway_cfg.enable << R_CANFD0_CFDC2_FDCFG_GWEN_Pos |
            p_extend->gateway_cfg.frame << R_CANFD0_CFDC2_FDCFG_GWFDF_Pos |
            p_extend->gateway_cfg.brs << R_CANFD0_CFDC2_FDCFG_GWBRS_Pos;

    /* Configure protocol mode */
    p_canfd_reg->CFDC2[channel_index].FDCFG |= p_extend->prt_mode << R_CANFD0_CFDC2_FDCFG_FDOE_Pos;
#ifdef BSP_FEATURE_CANFD_PN_MODE

/* Configure PNF mode*/
    p_canfd_reg->CFDC2[channel_index].FDCFG |= p_extend->pnf_mode << R_CANFD0_CFDC2_FDCFG_RPNMD_Pos;
#endif

    /* Write TX message buffer interrupt enable bits. Only enable interrupt TXMB handled by channel */
    if (0 == (p_cfg->channel % 2))
    {
        /* Even channel lend TXMB from odd channel. Enable TXMB interrupt of odd channel */
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH]      = p_extend->txmb_txi_enable[0];
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH + 1]  = p_extend->txmb_txi_enable[1];
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH + 2] |= p_extend->txmb_txi_enable[2] &
                                                                                CANFD_PRV_FILL_1(lend_txmb);
    }
    else
    {
        /* Odd channel TXMB is lent. Do not enable lent buffer*/
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH] |= p_extend->txmb_txi_enable[0] &
                                                                            (~CANFD_PRV_FILL_1(lend_txmb));
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH + 1] = p_extend->txmb_txi_enable[1];
    }

    /* Configure channel error interrupts */
    p_canfd_reg->CFDC[channel_index].CTR = p_extend->error_interrupts | R_CANFD0_CFDC_CTR_CHMDC_Msk;

    /* Configure Transmit Queue */
    for (uint8_t tx_queue_index = p_ctrl->txqueue_range_min;
         tx_queue_index < p_ctrl->txqueue_range_max;
         tx_queue_index++)
    {
        if (p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable)
        {
            if (tx_queue_index < CANFD_PRV_TXQ_CH_OFFSET)
            {
                p_canfd_reg->CFDTXQ[tx_queue_index].CC[channel_index] =
                    p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_u32;
            }
            else
            {
                p_canfd_reg->CFDTXQ[tx_queue_index - CANFD_PRV_TXQ_CH_OFFSET].CC[channel_index + 1] =
                    p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_u32;
            }
        }
    }

    /* Enable channel interrupts */

    if (p_cfg->error_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->err_ored_int, p_cfg->ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->error_irq, p_cfg->ipl, p_ctrl);
#endif
    }

    if (p_cfg->tx_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->tx_ored_int, p_cfg->ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->tx_irq, p_cfg->ipl, p_ctrl);
#endif
    }

    if (p_cfg->rx_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->rx_ored_int, p_cfg->ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->rx_irq, p_cfg->ipl, p_ctrl);
#endif
    }

    if (p_cfg->tx_vm_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->vmtx_ored_int, p_cfg->vm_ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->tx_vm_irq, p_cfg->vm_ipl, p_ctrl);
#endif
    }

    if (p_cfg->rx_vm_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->vmrx_ored_int, p_cfg->vm_ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->rx_vm_irq, p_cfg->vm_ipl, p_ctrl);
#endif
    }

    if (p_cfg->err_vm_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->vmerr_ored_int, p_cfg->vm_ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->err_vm_irq, p_cfg->vm_ipl, p_ctrl);
#endif
    }

#if !BSP_VLAB_HARDWARE_BASED_USED

    /* Set global mode to Operation and wait for transition */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_OPERATION);

    /* Transition to Channel Operation */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_NORMAL);
#endif

#ifdef BSP_FEATURE_CANFD_PN_MODE

/* Setting PNF mode*/
    p_canfd_reg->CFDC2[channel_index].FDCTR |= (p_extend->pnf_mode << R_CANFD0_CFDC2_FDCTR_PNMDC_Pos) |
                                               CANFD_PRV_PNMDC_KEY;
#endif

    /* Setting Transmit History if channel is requested*/
    if (p_extend->tx_history_cfg.enable)
    {
        p_canfd_reg->CFDTHLCC[channel_index] |= R_CANFD0_CFDTHLCC_THLE_Msk | R_CANFD0_CFDTHLCC_THLIE_Msk |
                                                (p_extend->tx_history_cfg.isr_mode << R_CANFD0_CFDTHLCC_THLIM_Pos);

#ifdef BSP_FEATURE_CANFD_FFI_MODE
        p_canfd_reg->CFDTHLCC[channel_index] |=
            (p_extend->tx_history_cfg.thl_entry_lost << R_CANFD0_CFDTHLCC_THLEIE_Pos);
#endif
    }

    /* Enable Transmit Queue
     * TXQE can only be modified in channel operation mode or channel halt mode
     **/

    /* Configure Transmit Queue */
    for (uint8_t tx_queue_index = p_ctrl->txqueue_range_min;
         tx_queue_index < p_ctrl->txqueue_range_max;
         tx_queue_index++)
    {
        if (p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable)
        {
            if (tx_queue_index < CANFD_PRV_TXQ_CH_OFFSET)
            {
                p_canfd_reg->CFDTXQ[tx_queue_index].CC_b[channel_index].TXQE = 1;
            }
            else
            {
                p_canfd_reg->CFDTXQ[tx_queue_index - CANFD_PRV_TXQ_CH_OFFSET].CC_b[channel_index + 1].TXQE = 1;
            }
        }
    }

    /* Enable Common FIFOs can only be enable channel operation mode or channel halt mode */
    for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
    {
        uint32_t cfifo_index = channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL + cfifo;
        p_canfd_reg->CFDCFCC_b[cfifo_index].CFE = p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable;
    }

    /* Set current operation modes */
    p_ctrl->operation_mode = CAN_OPERATION_MODE_NORMAL;
    p_ctrl->test_mode      = CAN_TEST_MODE_DISABLED;

#if (BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ)
    if (BSP_INTSEL_DISABLED != p_extend->intsel_irq_cfg)
    {
        R_BSP_IntSel_CfgEnable(p_extend->intsel_irq_cfg);
    }
#endif

    /* Set driver to open */
    p_ctrl->open = CANFD_OPEN;

    return err;
}

/***************************************************************************************************************//**
 * Close the CANFD channel.
 *
 * @retval FSP_SUCCESS               Channel closed successfully.
 * @retval FSP_ERR_NOT_OPEN          Control block not open.
 * @retval FSP_ERR_ASSERTION         Null pointer presented.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Close (can_ctrl_t * const p_api_ctrl)
{
    canfd_instance_ctrl_t  * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
    can_cfg_t              * p_cfg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;
#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);

    /* Get control block */
    p_ctrl   = (canfd_instance_ctrl_t *) p_api_ctrl;
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    FSP_ASSERT(p_extend->p_global_cfg);
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);
#else

    /* Get control block */
    p_ctrl   = (canfd_instance_ctrl_t *) p_api_ctrl;
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;
#endif

    /* Disable channel interrupts */
    if (p_cfg->error_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgDisable(p_extend->err_ored_int);
        }
#else
        R_BSP_IrqDisable(p_cfg->error_irq);
#endif
    }

    if (p_cfg->tx_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgDisable(p_extend->tx_ored_int);
        }
#else
        R_BSP_IrqDisable(p_cfg->tx_irq);
#endif
    }

    if (p_cfg->rx_irq >= 0)
    {
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
        {
            R_BSP_ORed_IrqCfgDisable(p_extend->rx_ored_int);
        }
#else
        R_BSP_IrqDisable(p_cfg->rx_irq);
#endif
    }

    /* Disable Global Error interrupt if the handler channel is being closed */
    if (p_global_cfg->channel_error == p_cfg->channel)
    {
        R_BSP_IrqDisable(p_global_cfg->err_glb_irq);
    }

    /* Set channel to Sleep */
#if !BSP_VLAB_HARDWARE_BASED_USED
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_SLEEP);
#else
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_RESET);
#endif

    /* Clear TXQ settings */
    for (uint8_t tx_queue_index = p_ctrl->txqueue_range_min;
         tx_queue_index < p_ctrl->txqueue_range_max;
         tx_queue_index++)
    {
        if (tx_queue_index < CANFD_PRV_TXQ_CH_OFFSET)
        {
            /* Transmit queue belong this channel */
            p_canfd_reg->CFDTXQ[tx_queue_index].CC[p_cfg->channel] = 0;
        }
        else
        {
            /* Transmit queue lend from next channel */
            p_canfd_reg->CFDTXQ[tx_queue_index - CANFD_PRV_TXQ_CH_OFFSET].CC[channel_index + 1] = 0;
        }
    }

    /* Disable TX message buffer interrupt. Only disable interrupt TXMB handled by channel */
    uint32_t lend_txmb = CANFD_PRV_LEND_BUFF_GET(p_canfd_reg, p_ctrl->channel_index);
    p_canfd_reg->CFDTMIEC[p_cfg->channel * CANFD_PRV_CFDTM_CH_LENGTH]     &= CANFD_PRV_FILL_1(lend_txmb);
    p_canfd_reg->CFDTMIEC[p_cfg->channel * CANFD_PRV_CFDTM_CH_LENGTH + 1]  = 0;
    p_canfd_reg->CFDTMIEC[p_cfg->channel * CANFD_PRV_CFDTM_CH_LENGTH + 2] &= ~CANFD_PRV_FILL_1(lend_txmb);

    /* Reset global control struct pointer */
    gp_ctrl[p_cfg->channel] = NULL;

    /* If all channel is close set CANFD to SLEEP*/
    if (r_canfd_is_all_channel_close(p_global_cfg->unit))
    {
        /* Disable RX FIFO interrupt */
        R_BSP_IrqDisable(p_global_cfg->rx_fifo_irq);

        /* Clear flag manually before enter global reset mode */
#if BSP_VLAB_HARDWARE_BASED_USED

        /* Clear all RXFIFO interrupt flags */
        for (uint32_t rx_fifo = 0; rx_fifo < BSP_FEATURE_CANFD_NUM_RXFIFO; rx_fifo++)
        {
            p_canfd_reg->CFDRFSTS[rx_fifo] = 0;
        }

        /* Clear RXMB new data flags */
        for (uint32_t cfdrmnd = 0; cfdrmnd < (CANFD_PRV_RXMB_MAX >> 5); cfdrmnd++)
        {
            p_canfd_reg->CFDRMND[cfdrmnd] = 0;
        }

        for (uint32_t common_fifo = 0; common_fifo < BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT; common_fifo++)
        {
            /* Disable Common FIFO */
            p_canfd_reg->CFDCFCC_b[common_fifo].CFE = 0;

            /* Clear all status flags */
            p_canfd_reg->CFDCFSTS[common_fifo] = 0;
        }
#endif

        /* Transition to Global Reset */
        r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);

        /* Clear RX FIFO settings */
        for (uint32_t rx_fifo = 0; rx_fifo < BSP_FEATURE_CANFD_NUM_RXFIFO; rx_fifo++)
        {
            p_canfd_reg->CFDRFCC[rx_fifo] = 0;
        }

        /* Clear Common FIFO settings */
        for (uint32_t common_fifo = 0; common_fifo < BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT; common_fifo++)
        {
            p_canfd_reg->CFDCFCC[common_fifo] = 0;
        }

        /* Reset Flexible Mode/CANHUB mode */
        p_canfd_reg->CFDGFCMC = 0U;

        /* Reset Flexible Transmission buffer assignment*/
        p_canfd_reg->CFDGFTBAC = 0U;

        /* Reset Transmit History configuration */
        p_canfd_reg->CFDTHLCC[channel_index] = 0;
#ifdef BSP_FEATURE_CANFD_FFI_MODE

        /* Disable FFI mode */
        p_canfd_reg->CFDGFFIMC = (uint32_t) (CANFD_PRV_CFDGFFIMC_FFIEN_OFF | CANFD_PRV_CFDGFFIMC_KEY);

        /* Reset target Virtual Machine channel for Rx FIFO */
        p_canfd_reg->CFDVMRFCFG = 0;

        /* Reset target Virtual Machine channel for TxFX FIFO and Tx Queue */
        p_canfd_reg->CFDVMCFG[channel_index] = 0;
#endif

        /* Transition to Global Sleep */
#if !BSP_VLAB_HARDWARE_BASED_USED
        r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_SLEEP);
#endif
    }

#if BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ
    if (BSP_INTSEL_DISABLED != p_extend->intsel_irq_cfg)
    {
        R_BSP_IntSel_CfgDisable(p_extend->intsel_irq_cfg);
    }
#endif

    /* Set driver to closed */
    p_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/***************************************************************************************************************//**
 * Write data to the CANFD channel.
 *
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_CAN_TRANSMIT_NOT_READY   Transmit in progress, cannot write data at this time.
 * @retval FSP_ERR_INVALID_ARGUMENT         Data length or buffer invalid.
 * @retval FSP_ERR_INVALID_MODE             An FD option was set on a non-FD frame.
 * @retval FSP_ERR_ASSERTION                Null pointer presented
 * @retval FSP_ERR_UNSUPPORTED              Selected buffer or frame format not supported
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Write (can_ctrl_t * const p_api_ctrl, can_buffer_t buffer, can_frame_t * const p_frame)
{
    canfd_instance_ctrl_t  * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
    canfd_extended_cfg_t   * p_extend_cfg;
    canfd_global_cfg_t     * p_global_cfg;
    can_buffer_type_t        buffer_type  = (can_buffer_type_t) (buffer & CANFD_PRV_BUFFER_TYPE_MASK);
    uint32_t                 buffer_index = (buffer & CANFD_PRV_BUFFER_NUM_MASK);
    uint32_t                 channel_index;
    uint32_t                 txq_channel;
    volatile R_CANFD0_Type * p_canfd_reg;
    uint8_t can_dlc = 0;
    uint8_t can_len = 0;
#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_frame);
    FSP_ASSERT(NULL != p_ctrl->p_cfg->p_extend)

    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);

    /* Check valid buffer number */
    FSP_ERROR_RETURN(FSP_SUCCESS == r_canfd_validate_tx_buffer(p_ctrl, buffer), FSP_ERR_INVALID_ARGUMENT);

    /* Check selected frame format is supported */
    FSP_ERROR_RETURN(p_frame->format != CAN_FRAME_FORMAT_XL, FSP_ERR_UNSUPPORTED);

    /* Check DLC field */
    if (CAN_FRAME_FORMAT_CC == p_frame->format)
    {
        FSP_ERROR_RETURN(p_frame->data_length_code <= 8, FSP_ERR_INVALID_ARGUMENT);
        FSP_ERROR_RETURN(p_frame->options == 0, FSP_ERR_INVALID_MODE);
    }
    else if (CAN_FRAME_FORMAT_FD == p_frame->format)
    {
        /* Make sure the supplied data size corresponds to a valid DLC value */
        FSP_ERROR_RETURN(p_frame->data_length_code <= 64, FSP_ERR_INVALID_ARGUMENT);
    }
    else
    {
        /* Do nothing. */
    }
#endif

    volatile R_CANFD0_CFDTM_Type * p_tx_buffer = 0;
    uint32_t fdctr      = 0;
    uint32_t txq_buffer = buffer_index;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend_cfg = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend_cfg->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;
    txq_channel   = channel_index;

    if ((CAN_BUFFER_TYPE_TX_SINGLE == buffer_type))
    {
        buffer_index += channel_index * CANFD_PRV_TXMB_CHANNEL_OFFSET;

        /* Ensure buffer is ready */
        FSP_ERROR_RETURN(0U == p_canfd_reg->CFDTMSTS_b[buffer_index].TMTSTS, FSP_ERR_CAN_TRANSMIT_NOT_READY);
        p_tx_buffer = &(p_canfd_reg->CFDTM[buffer_index]);
    }
    else if (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type)
    {
        uint32_t queue_window;

        /* Get the Transmit queue buffer index and channel */
        if (buffer_index >= CANFD_PRV_TXQ_CH_OFFSET)
        {
            txq_buffer  = buffer_index - CANFD_PRV_TXQ_CH_OFFSET;
            txq_channel = channel_index + 1;
        }

        /* Ensure buffer is ready */
        FSP_ERROR_RETURN(0U == p_canfd_reg->CFDTXQ[txq_buffer].STS_b[txq_channel].TXQFLL,
                         FSP_ERR_CAN_TRANSMIT_NOT_READY);

        /* Write frame to transmit queue windows */
        queue_window = g_queue_window[txq_buffer] + txq_channel * CANFD_PRV_TXMB_CHANNEL_OFFSET;
        p_tx_buffer  = &(p_canfd_reg->CFDTM[queue_window]);
    }
    else
    {
        buffer_index += channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;

        /* Ensure buffer is ready */
        FSP_ERROR_RETURN(0U == p_canfd_reg->CFDCFSTS_b[buffer_index].CFFLL, FSP_ERR_CAN_TRANSMIT_NOT_READY);
        p_tx_buffer = (volatile R_CANFD0_CFDTM_Type *) &(p_canfd_reg->CFDCF[buffer_index]);
    }

    /* Set TX message buffer registers */
    p_tx_buffer->ID = p_frame->id | ((uint32_t) p_frame->type << R_CANFD0_CFDTM_ID_TMRTR_Pos) |
                      ((uint32_t) p_frame->id_mode << R_CANFD0_CFDTM_ID_TMIDE_Pos);

    /*Allow to store frame into TX History Entry*/
    if (p_extend_cfg->tx_history_cfg.enable)
    {
        p_tx_buffer->ID_b.THLEN = 1;
    }

    /* Convert bytes to dlc */
    can_dlc = r_canfd_bytes_to_dlc((uint8_t) p_frame->data_length_code);

    p_tx_buffer->PTR = can_dlc << R_CANFD0_CFDTM_PTR_TMDLC_Pos;

    /* Set FD bits (ESI, BRS and FDF) */
    if (CAN_FRAME_FORMAT_FD == p_frame->format)
    {
        /* Set FDF bit */
        fdctr |= R_CANFD0_CFDTM_FDCTR_TMFDF_Msk;

        if (p_frame->options & CAN_FRAME_OPTIONS_BRS)
        {
            /* Set BRS bit */
            fdctr |= R_CANFD0_CFDTM_FDCTR_TMBRS_Msk;
        }

        if (p_frame->options & CAN_FRAME_OPTIONS_ESI)
        {
            /* Set ESI */
            fdctr |= R_CANFD0_CFDTM_FDCTR_TMESI_Msk;
        }
    }

    /* Set buffer control register */
    p_tx_buffer->FDCTR = fdctr | (p_frame->labels << R_CANFD0_CFDTM_FDCTR_TMPTR_Pos);

    /* Copy data to register buffer */
    uint32_t           len    = p_frame->data_length_code;
    volatile uint8_t * p_dest = (uint8_t *) p_tx_buffer->DF;
    volatile uint8_t * p_src  = p_frame->data;
    while (len--)
    {
        *p_dest++ = *p_src++;
    }

    /* Convert dlc to actual bytes */
    can_len = dlc_to_bytes[can_dlc];

    if (can_len > p_frame->data_length_code)
    {
        for (uint8_t i = p_frame->data_length_code; i < can_len; i++)
        {
            *p_dest++ = p_extend_cfg->tx_padding_data;
        }
    }

    /* Request transmission */
    if ((CAN_BUFFER_TYPE_TX_SINGLE == buffer_type))
    {
        p_canfd_reg->CFDTMC[buffer_index] = 1;
    }
    else if (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type)
    {
        p_canfd_reg->CFDTXQ[txq_buffer].PCTR[txq_channel] = CANFD_PRV_UPDATE_POINTER_CMD;
    }
    else
    {
        p_canfd_reg->CFDCFPCTR[buffer_index] = CANFD_PRV_UPDATE_POINTER_CMD;
    }

    return FSP_SUCCESS;
}

/***************************************************************************************************************//**
 * Read data from a CANFD Message Buffer or FIFO.
 *
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_INVALID_ARGUMENT         Buffer number invalid to used.
 * @retval FSP_ERR_ASSERTION                p_api_ctrl or p_frame is NULL.
 * @retval FSP_ERR_BUFFER_EMPTY             Buffer or FIFO is empty.
 * @retval FSP_ERR_UNSUPPORTED              Selected buffer is not supported.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Read (can_ctrl_t * const p_api_ctrl, can_buffer_t buffer, can_frame_t * const p_frame)
{
    can_buffer_type_t        buffer_type  = (can_buffer_type_t) (buffer & CANFD_PRV_BUFFER_TYPE_MASK);
    uint32_t                 buffer_index = buffer & CANFD_PRV_BUFFER_NUM_MASK;
    canfd_instance_ctrl_t  * p_ctrl       = (canfd_instance_ctrl_t *) p_api_ctrl;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;
#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_frame);

    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(r_canfd_validate_rx_buffer(p_ctrl->p_cfg, buffer) == FSP_SUCCESS, FSP_ERR_INVALID_ARGUMENT);
#endif

    uint32_t not_empty = 0;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Return an error if the buffer or FIFO is empty */
    if (CAN_BUFFER_TYPE_RX_SINGLE == buffer_type)
    {
        /*Get status of select RXMB */
        uint32_t buffer_reg = buffer_index >> 5;             // Divide by 32
        uint32_t buffer_pos = buffer_index & ((1 << 5) - 1); // Get remainder of 32.
        not_empty = p_canfd_reg->CFDRMND[buffer_reg] & (1U << buffer_pos);
    }
    else if (CAN_BUFFER_TYPE_RX_FIFO == buffer_type)
    {
        if (buffer_index >= CANFD_PRV_CFIFO_OFFSET)
        {
            /* Get status of selected common FIFO */
            buffer_index += BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL * channel_index - CANFD_PRV_CFIFO_OFFSET;
            not_empty     = !p_canfd_reg->CFDCFSTS_b[buffer_index].CFEMP;
        }
        else
        {
            /* Get status of selected Rx FIFO */
            not_empty = !(p_canfd_reg->CFDFESTS & (1U << buffer_index));
        }
    }
    else
    {
        /* Do nothing */
    }

    FSP_ERROR_RETURN(not_empty, FSP_ERR_BUFFER_EMPTY);

    /* Retrieve message from buffer */
    r_canfd_mb_read(channel_index, buffer, p_frame, p_global_cfg);

    return FSP_SUCCESS;
}

/***************************************************************************************************************//**
 * Switch to a different channel, global or test mode.
 *
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_ASSERTION                Null pointer presented
 * @retval FSP_ERR_INVALID_MODE             Cannot change to the requested mode from the current global mode.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_ModeTransition (can_ctrl_t * const   p_api_ctrl,
                                  can_operation_mode_t operation_mode,
                                  can_test_mode_t      test_mode)
{
    canfd_instance_ctrl_t  * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
    canfd_extended_cfg_t   * p_extend_cfg;
    canfd_global_cfg_t     * p_global_cfg;
    fsp_err_t                err = FSP_SUCCESS;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;
#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_ctrl->p_cfg->p_extend);

    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);

    p_extend_cfg = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

    FSP_ASSERT(NULL != p_extend_cfg->p_global_cfg);

    /* Get global config */
    p_global_cfg = p_extend_cfg->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get Global Status */
    uint32_t cfdgsts = p_canfd_reg->CFDGSTS;

    /* Check to ensure the current mode is Global Halt when transitioning into or out of Internal Bus mode */
    FSP_ERROR_RETURN((cfdgsts & R_CANFD0_CFDGSTS_GHLTSTS_Msk) || !((p_ctrl->test_mode != test_mode) &&
                                                                   ((CAN_TEST_MODE_INTERNAL_BUS ==
                                                                     p_ctrl->test_mode) ||
                                                                    (CAN_TEST_MODE_INTERNAL_BUS == test_mode))),
                     FSP_ERR_INVALID_MODE);

    /* Check to ensure the current mode is Global Reset when transitioning into or out of Global Sleep (see Section
     * "Global Modes" in the RH850/U2B microcontroller User's Manual for details) */
    FSP_ERROR_RETURN(((cfdgsts & R_CANFD0_CFDGSTS_GRSTSTS_Msk) && (CAN_OPERATION_MODE_RESET & operation_mode)) ||
                     (!(cfdgsts & R_CANFD0_CFDGSTS_GSLPSTS_Msk) &&
                      (CAN_OPERATION_MODE_GLOBAL_SLEEP != operation_mode)),
                     FSP_ERR_INVALID_MODE);

    /* Check to ensure the current Global mode supports the requested Channel mode, if applicable. The requested mode
     * and the current global mode are converted into a number 0-3 corresponding to Operation, Halt, Reset and Sleep
     * respectively. The channel mode cannot be switched to a mode with an index lower than the current global mode. */
    if (operation_mode < CAN_OPERATION_MODE_GLOBAL_OPERATION)
    {
        FSP_ERROR_RETURN(g_mode_order[operation_mode] >= g_mode_order[cfdgsts & CANFD_PRV_CTR_MODE_MASK],
                         FSP_ERR_INVALID_MODE);
    }

#else
    p_extend_cfg = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend;

    /* Get global config */
    p_global_cfg = p_extend_cfg->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;
#endif

    if (p_ctrl->test_mode != test_mode)
    {
        /* Follow the procedure for switching to Internal Bus mode given in Section "Internal CAN Bus
         * Communication Test Mode" of the RH850/U2B microcontroller User's Manual */
        if (CAN_TEST_MODE_INTERNAL_BUS == test_mode)
        {
            /* Disable channel test mode */
            p_canfd_reg->CFDC[channel_index].CTR_b.CTME = 0;

            /* Link channel to internal bus */
            p_canfd_reg->CFDGTSTCFG |= 1U << channel_index;

            /* Enable internal bus test mode */
            p_canfd_reg->CFDGTSTCTR = 1;
        }
        else
        {
            if (CAN_TEST_MODE_INTERNAL_BUS == p_ctrl->test_mode)
            {
                /* Unlink channel from internal bus */
                p_canfd_reg->CFDGTSTCFG &= ~(1U << channel_index);

                /* Disable global test mode if no channels are linked */
                if (!p_canfd_reg->CFDGTSTCFG)
                {
                    p_canfd_reg->CFDGTSTCTR = 0;
                }
            }

            /* Transition to Channel Halt when changing test modes */
            r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_HALT);

            /* Set channel test mode */
            uint32_t cfdcnctr = p_canfd_reg->CFDC[channel_index].CTR;
            cfdcnctr &= ~(R_CANFD0_CFDC_CTR_CTME_Msk | R_CANFD0_CFDC_CTR_CTMS_Msk);
            p_canfd_reg->CFDC[channel_index].CTR = cfdcnctr |
                                                   ((uint32_t) test_mode << R_CANFD0_CFDC_CTR_CTME_Pos);
        }

        p_ctrl->test_mode = test_mode;
    }

    if (p_ctrl->operation_mode != operation_mode)
    {
        r_canfd_mode_transition(p_ctrl, operation_mode);

        if ((CAN_OPERATION_MODE_NORMAL == p_ctrl->operation_mode) ||
            (CAN_OPERATION_MODE_HALT == p_ctrl->operation_mode))
        {
            /* Re-enable Transmit Queue */
            /* Configure Transmit Queue */
            for (uint8_t tx_queue_index = 0;
                 tx_queue_index < BSP_FEATURE_CANFD_NUM_TXQUEUE_CHANNEL;
                 tx_queue_index++)
            {
                if (p_extend_cfg->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable)
                {
                    p_canfd_reg->CFDTXQ[tx_queue_index].CC_b[channel_index].TXQE =
                        p_extend_cfg->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable;
                }
            }

            /* Re-enable Common FIFO*/
            for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
            {
                uint32_t cfifo_index = channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL + cfifo;
                p_canfd_reg->CFDCFCC_b[cfifo_index].CFE =
                    p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable;
            }
        }
    }

    return err;
}

/***************************************************************************************************************//**
 * Get CANFD state and status information for the channel.
 *
 * @retval  FSP_SUCCESS                     Operation succeeded.
 * @retval  FSP_ERR_NOT_OPEN                Control block not open.
 * @retval  FSP_ERR_ASSERTION               Null pointer presented
 *****************************************************************************************************************/
fsp_err_t R_CANFD_InfoGet (can_ctrl_t * const p_api_ctrl, can_info_t * const p_info)
{
    canfd_instance_ctrl_t  * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
    can_cfg_t              * p_cfg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint32_t                 fdsts;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;
#if CANFD_CFG_PARAM_CHECKING_ENABLE

    /* Check pointers for NULL values */
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_info);

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    FSP_ASSERT(p_extend->p_global_cfg);
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* If channel is not open, return an error */
    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);
#else

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;
#endif

    uint32_t cfdcnsts = p_canfd_reg->CFDC[channel_index].STS;
    p_info->status               = cfdcnsts & UINT16_MAX;
    p_info->error_count_receive  = (uint8_t) ((cfdcnsts & R_CANFD0_CFDC_STS_REC_Msk) >> R_CANFD0_CFDC_STS_REC_Pos);
    p_info->error_count_transmit = (uint8_t) ((cfdcnsts & R_CANFD0_CFDC_STS_TEC_Msk) >> R_CANFD0_CFDC_STS_TEC_Pos);

    for (uint32_t rxmb_sts = 0; rxmb_sts < 4; rxmb_sts++)
    {
        p_info->rx_single_status[rxmb_sts] = p_canfd_reg->CFDRMND[rxmb_sts];
    }

    /* Get RX FIFO Empty status */
    p_info->rx_fifo_status = (~p_canfd_reg->CFDFESTS) & CANFD_PRV_CFDFESTS_RFXEMP_MSK;

    /* Get Common FIFO Empty status */
    uint32_t cfifo_empty =
        (((~p_canfd_reg->CFDFESTS_b.CFkEMP) >> (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)) & 0x07);
    p_info->rx_fifo_status |= cfifo_empty << CANFD_PRV_CFIFO_OFFSET;

    fdsts = 0;

    /* Read SOCO, EOCO, TDCVF value */
    if (p_canfd_reg->CFDC2[channel_index].FDSTS_b.TDCVF)
    {
        fdsts |= CANFD_ERROR_CHANNEL_DELAY_VIOLATE;
    }

    if (p_canfd_reg->CFDC2[channel_index].FDSTS_b.EOCO)
    {
        fdsts |= CANFD_ERROR_CHANNEL_ERR_OCCUR_OVF;
    }

    if (p_canfd_reg->CFDC2[channel_index].FDSTS_b.SOCO)
    {
        fdsts |= CANFD_ERROR_CHANNEL_SUC_OCCUR_OVF;
    }

    /* Clear SOCO, EOCO, TDVCF flags */
    p_canfd_reg->CFDC2[channel_index].FDSTS |= R_CANFD0_CFDC2_FDSTS_TDCVF_Pos | R_CANFD0_CFDC2_FDSTS_EOCO_Pos |
                                               R_CANFD0_CFDC2_FDSTS_SOCO_Pos;

    /* Read and clear channel error flags. */
    uint32_t erfl = p_canfd_reg->CFDC[channel_index].ERFL & CANFD_PRV_ERROR_CH_FLAG_REG_MASK;
    p_canfd_reg->CFDC[channel_index].ERFL = 0;

    p_info->error_code = erfl | fdsts;

    /* Dummy read to ensure that interrupt event is cleared. */
    volatile uint32_t dummy = p_canfd_reg->CFDC[channel_index].ERFL;
    FSP_PARAMETER_NOT_USED(dummy);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 * Implements @ref can_api_t::callbackSet.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_CANFD_CallbackSet (can_ctrl_t * const          p_api_ctrl,
                               void (                    * p_callback)(can_callback_args_t *),
                               void * const                p_context,
                               can_callback_args_t * const p_callback_memory)
{
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start bus idle time measurement
 * Implements @ref can_api_t::callbackSet.
 *
 * @param[in]     p_api_ctrl             Pointer to the CAN control block.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_INVALID_MODE         Bus idle counter only available in normal operation mode.
 **********************************************************************************************************************/
fsp_err_t R_CANFD_StartBusIdleCnt (can_ctrl_t * const p_api_ctrl)
{
    canfd_instance_ctrl_t  * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
    can_cfg_t              * p_cfg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    FSP_ASSERT(p_extend->p_global_cfg);
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(CAN_OPERATION_MODE_NORMAL == p_ctrl->operation_mode, FSP_ERR_INVALID_MODE);
#else

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;
#endif

    /* Enable and reset bus load counter */
    p_canfd_reg->CFDC2[p_ctrl->channel_index].BLCT = R_CANFD0_CFDC2_BLCT_BLCLD_Msk | R_CANFD0_CFDC2_BLCT_BLCE_Msk;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop bus idle time measurement and return the idle time in nanoseconds.
 * Implements @ref can_api_t::callbackSet.
 *
 * @param[in]     p_api_ctrl             Pointer to the CAN control block.
 * @param[out]    p_idle_time_ns         Pointer to memory block store idle time in nanoseconds.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_INVALID_MODE         Bus idle counter only available in normal operation mode.
 **********************************************************************************************************************/
fsp_err_t R_CANFD_StopBusIdleCnt (can_ctrl_t * const p_api_ctrl, uint32_t * p_idle_time_ns)
{
    canfd_instance_ctrl_t  * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
    can_cfg_t              * p_cfg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;
    uint32_t                 idle_cnt;
    uint32_t                 clkc_freq;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    FSP_ASSERT(p_extend->p_global_cfg);
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(CAN_OPERATION_MODE_NORMAL == p_ctrl->operation_mode, FSP_ERR_INVALID_MODE);
#else

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;
#endif

    /* Stop bus load counter and retrieve idle time counter */
    p_canfd_reg->CFDC2[p_ctrl->channel_index].BLCT = R_CANFD0_CFDC2_BLCT_BLCLD_Msk;
    idle_cnt = p_canfd_reg->CFDC2[p_ctrl->channel_index].BLSTS;

    /* Calculate idle time */
    clkc_freq       = R_FSP_SystemClockHzGet(BSP_FEATURE_CANFD_CLKC_SRC);
    *p_idle_time_ns = idle_cnt * (CANFD_PRV_SEC_TO_NS / clkc_freq);

    return FSP_SUCCESS;
}

#ifdef BSP_FEATURE_CANFD_PN_MODE

/*******************************************************************************************************************//**
 * Perform CANFD operating mode switching to PNF mode
 * Implements @ref can_api_t::callbackSet.
 *
 * @param[in]     p_api_ctrl             Pointer to the CAN control block.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_INVALID_MODE         Bus idle counter only available in normal operation mode.
 **********************************************************************************************************************/
fsp_err_t R_CANFD_PNFTransition (can_ctrl_t * const p_api_ctrl)
{
    canfd_instance_ctrl_t  * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
    can_cfg_t              * p_cfg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;

 #if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    FSP_ASSERT(p_extend->p_global_cfg);
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(CAN_OPERATION_MODE_NORMAL == p_ctrl->operation_mode, FSP_ERR_INVALID_MODE);
 #else

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;
 #endif

    /* Transition to PNF mode */
    p_canfd_reg->CFDC2[channel_index].FDCTR = (CANFD_FILTER_MODE_PNF << R_CANFD0_CFDC2_FDCTR_PNMDC_Pos) |
                                              CANFD_PRV_PNMDC_KEY;

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * Stop bus idle time measurement and return the idle time in nanoseconds.
 * Implements @ref can_api_t::callbackSet.
 *
 * @param[in]     p_api_ctrl             Pointer to the CAN control block.
 * @param[in]     p_cfg                  Pointer to the CAN configuration block.
 * @param[in]     ram_test_field         Field RAM to test.
 * @param[in]     page_id                Index of page ID.
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ALREADY_OPEN         The control block has  been opened.
 * @retval  FSP_ERR_INVALID_ARGUMENT     Page index invalid to used.
 * @retval  FSP_ERR_INVALID_MODE         Field RAM test does not support.
 * @retval  FSP_ERR_INVALID_DATA         Data is invalid.
 **********************************************************************************************************************/
fsp_err_t R_CANFD_RAMTest (can_ctrl_t * const      p_api_ctrl,
                           can_cfg_t const * const p_cfg,
                           canfd_ramtest_field_t   ram_test_field,
                           uint32_t                page_id)
{
    fsp_err_t               err    = FSP_SUCCESS;
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
    canfd_ramtest_t       * p_ram_test_infor;
    uint8_t                 unit = 0;

#if CANFD_CFG_PARAM_CHECKING_ENABLE

    /*Check null of pointer*/
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(CANFD_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(p_cfg);

    /* Get extended config */
    canfd_extended_cfg_t * p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    FSP_ASSERT(p_extend);

    /* Get global config */
    canfd_global_cfg_t * p_global_cfg = p_extend->p_global_cfg;

    FSP_ASSERT(p_global_cfg);

    unit = p_global_cfg->unit;

    /* Get RAM infor config */
    p_ram_test_infor            = p_global_cfg->p_ram_infor;
    p_ram_test_infor->ram_field = ram_test_field;

    if (ram_test_field)
    {
 #ifndef BSP_FEATURE_CANFD_PN_MODE
        FSP_ERROR_RETURN(0, FSP_ERR_INVALID_MODE);
 #endif
    }

    p_ram_test_infor->ram_max_page = r_canfd_ram_caculate_max_page(unit, ram_test_field);

    FSP_ERROR_RETURN(page_id < p_ram_test_infor->ram_max_page, FSP_ERR_INVALID_ARGUMENT);
#else

    /* Get extended config */
    canfd_extended_cfg_t * p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Get global config */
    canfd_global_cfg_t * p_global_cfg = p_extend->p_global_cfg;

    unit = p_global_cfg->unit;

    /* Get RAM infor config */
    p_ram_test_infor            = p_global_cfg->p_ram_infor;
    p_ram_test_infor->ram_field = ram_test_field;

    p_ram_test_infor->ram_max_page = r_canfd_ram_caculate_max_page(unit, ram_test_field);
#endif

    /* Initialize the control block */
    p_ctrl->p_cfg = p_cfg;

    volatile R_CANFD0_Type * p_canfd_reg = p_global_cfg->p_canfd_reg;

    /* Wait for RAM initialization
     *(see RH850 microcontroller User's Manual section "Timing of Global Mode Change") */
    FSP_HARDWARE_REGISTER_WAIT((p_canfd_reg->CFDGSTS & R_CANFD0_CFDGSTS_GRAMINIT_Msk), 0);

    /* Transition to Global Reset */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);

    /* Transition to Global Test Mode */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_HALT);

    /* Write protection data to LOCKK register */
    p_canfd_reg->CFDGLOCKK = CANFD_GLOCKK_UNLOCK_KEY1;
    p_canfd_reg->CFDGLOCKK = CANFD_GLOCKK_UNLOCK_KEY2;

    /* Enable RAM Test Mode */
    p_canfd_reg->CFDGTSTCTR |= R_CANFD0_CFDGTSTCTR_RTME_Msk;

    /* Enable RAM Test Mode */
    p_canfd_reg->CFDGTSTCFG_b.RTMPS = page_id;

    /* Caculate limit accessed byte if page id is the last page
     * Page index has value from 0 to (max page ram - 1) */
    if (page_id == (p_ram_test_infor->ram_max_page - CANFD_ONE))
    {
        p_ram_test_infor->unaccessed_ram_entry = r_canfd_ram_caculate_limit_byte(p_global_cfg);
    }

    /* Set all data ram with zero */
    r_canfd_set_ram_data(p_global_cfg, CAN_RAMTEST_DATA_0);

    /* RAM test using checker algorithm */
    err = r_canfd_ram_check_algorithm(p_global_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* RAM test using checker walkpath algorith type 0 */
    err = r_canfd_ram_walkpath_algorithm(p_global_cfg, CAN_RAMTEST_WALK_TYPE_0);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* RAM test using checker walkpath algorith type 1 */
    err = r_canfd_ram_walkpath_algorithm(p_global_cfg, CAN_RAMTEST_WALK_TYPE_1);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Set all data ram with zero */
    r_canfd_set_ram_data(p_global_cfg, CAN_RAMTEST_DATA_0);

    /*Disable RAM Test Mode*/
    p_canfd_reg->CFDGTSTCTR &= ~R_CANFD0_CFDGTSTCTR_RTME_Msk;

    /* Transition to Global Reset */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);

    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup CAN)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

#if CANFD_CFG_PARAM_CHECKING_ENABLE

/***********************************************************************************************************************
 * Check bit timing status for nominal bit rate
 * @param[in]     p_bit_timing     CAN bit timing
 **********************************************************************************************************************/
static bool r_canfd_bit_timing_nominal_parameter_check (can_bit_timing_cfg_t * const p_bit_timing)
{
    /* Check that prescaler is in range */
    FSP_ERROR_RETURN((p_bit_timing->baud_rate_prescaler <= CANFD_BAUD_RATE_PRESCALER_MAX) &&
                     (p_bit_timing->baud_rate_prescaler >= CANFD_BAUD_RATE_PRESCALER_MIN),
                     false);

    /* Check that TSEG1 > TSEG2 >= SJW for nominal bitrate per section "Bit Timing Conditions" in the
     * RH850/U2x microcontroller User's Manual. */

    /* Check Time Segment 1 is greater than Time Segment 2 */
    FSP_ERROR_RETURN((uint32_t) p_bit_timing->time_segment_1 > (uint32_t) p_bit_timing->time_segment_2, false);

    /* Check Time Segment 2 is greater than or equal to the synchronization jump width */
    FSP_ERROR_RETURN((uint32_t) p_bit_timing->time_segment_2 >= (uint32_t) p_bit_timing->synchronization_jump_width,
                     false);

    return true;
}

/***********************************************************************************************************************
 * Check bit timing status for data bit rate
 * @param[in]     p_bit_timing     CAN bit timing
 **********************************************************************************************************************/
static bool r_canfd_bit_timing_data_parameter_check (can_bit_timing_cfg_t * const p_bit_timing)
{
    /* Check that prescaler is in range */
    FSP_ERROR_RETURN((p_bit_timing->baud_rate_prescaler <= CANFD_BAUD_RATE_PRESCALER_MAX) &&
                     (p_bit_timing->baud_rate_prescaler >= CANFD_BAUD_RATE_PRESCALER_MIN),
                     false);

    /* Check that TSEG1 > TSEG2 >= SJW for nominal bitrate per section "Bit Timing Conditions" in the
     * RH850/U2x microcontroller User's Manual. */

    /* Check Time Segment 1 is greater than Time Segment 2 */
    FSP_ERROR_RETURN((uint32_t) p_bit_timing->time_segment_1 >= (uint32_t) p_bit_timing->time_segment_2, false);

    /* Check Time Segment 2 is greater than or equal to the synchronization jump width */
    FSP_ERROR_RETURN((uint32_t) p_bit_timing->time_segment_2 >= (uint32_t) p_bit_timing->synchronization_jump_width,
                     false);

    return true;
}

/*******************************************************************************************************************//**
 * Check buffer number is a valid
 * @param[in]  p_ctrl                   CANFD control block
 * @param[in]  buffer                   buffer number id
 *
 * @retval FSP_SUCCESS                  Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT     Invalid field in rtc_time_t structure
 * @retval FSP_ERR_UNSUPPORTED          Selected buffer is not supported.
 **********************************************************************************************************************/
static fsp_err_t r_canfd_validate_tx_buffer (canfd_instance_ctrl_t * p_ctrl, can_buffer_t buffer)
{
    /* Get config struct */
    const can_cfg_t * p_cfg = p_ctrl->p_cfg;

    /* Get extended config and global config*/
    canfd_extended_cfg_t * p_extend_cfg = (canfd_extended_cfg_t *) p_cfg->p_extend;
    canfd_global_cfg_t   * p_global_cfg = p_extend_cfg->p_global_cfg;

    can_buffer_type_t buffer_type = (can_buffer_type_t) (buffer & CANFD_PRV_BUFFER_TYPE_MASK);
    uint32_t          buffer_num  = buffer & CANFD_PRV_BUFFER_NUM_MASK;

    /* Check the buffer type is supported */
    FSP_ERROR_RETURN(((CAN_BUFFER_TYPE_TX_FIFO == buffer_type) ||
                      (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type) ||
                      (CAN_BUFFER_TYPE_TX_SINGLE == buffer_type)),
                     FSP_ERR_INVALID_ARGUMENT);

    /*Take index for channel when device have more than one unit*/
    uint32_t channel_index = p_cfg->channel - CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit;

    if (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type)
    {
        /* Check TXMB is not allocate in TX Queue */
        FSP_ERROR_RETURN(buffer_num <
                         (CANFD_PRV_TXMB_CHANNEL_OFFSET -
                          (p_extend_cfg->tx_queue_cfg[buffer_num].tx_queue_cfg_b.depth + 1)),
                         FSP_ERR_INVALID_ARGUMENT);

        /* Check Tx Queue is enabled if transmit buffer Transmit Queue*/
        FSP_ERROR_RETURN(p_extend_cfg->tx_queue_cfg[buffer_num].tx_queue_cfg_b.enable, FSP_ERR_INVALID_ARGUMENT);
    }

    if (CAN_BUFFER_TYPE_TX_FIFO == buffer_type)
    {
        /* CANFD only have 3 common FIFO to be configured in TX Mode */
        FSP_ERROR_RETURN(buffer_num < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL, FSP_ERR_UNSUPPORTED);
        uint32_t cfifo_index = channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL + buffer_num;

        /* Check Common FIFO is enabled */
        FSP_ERROR_RETURN(1 == p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable,
                         FSP_ERR_INVALID_ARGUMENT);

        /* Check Common FIFO is enabled and operation in transmit*/
        FSP_ERROR_RETURN(p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable, FSP_ERR_INVALID_ARGUMENT);
        FSP_ERROR_RETURN(CANFD_CFIFO_MODE_TRANSMIT == p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.mode,
                         FSP_ERR_INVALID_ARGUMENT);
    }

    if (CAN_BUFFER_TYPE_TX_SINGLE == buffer_type)
    {
        FSP_ERROR_RETURN((buffer_num >= p_ctrl->txmb_range_min) && (buffer_num < p_ctrl->txmb_range_max),
                         FSP_ERR_INVALID_ARGUMENT);

 #ifndef BSP_FEATURE_CANFD_NOT_CHECK_TXMB_RANGE

        /* Check TXMB in range 0-15 and 32-47*/
        FSP_ERROR_RETURN(CANFD_PRV_VALID_TXMB_RANGE(buffer_num) || (buffer_num >= CANFD_PRV_TXMB_CHANNEL_OFFSET),
                         FSP_ERR_INVALID_ARGUMENT);
 #endif
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Check buffer receive number is a valid
 * @param[in]  p_cfg        can config
 * @param[in]  buffer       buffer number id
 *
 * @retval FSP_SUCCESS                  Validation successful
 * @retval FSP_ERR_INVALID_ARGUMENT     Selected buffer is invalid to use.
 * @retval FSP_ERR_UNSUPPORTED          Selected buffer is not supported by.
 **********************************************************************************************************************/
static fsp_err_t r_canfd_validate_rx_buffer (const can_cfg_t * p_cfg, can_buffer_t buffer)
{
    canfd_extended_cfg_t * p_extend_cfg = (canfd_extended_cfg_t *) p_cfg->p_extend;
    canfd_global_cfg_t   * p_global_cfg = p_extend_cfg->p_global_cfg;
    can_buffer_type_t      buffer_type  = (can_buffer_type_t) (buffer & CANFD_PRV_BUFFER_TYPE_MASK);
    uint32_t               buffer_num   = buffer & CANFD_PRV_BUFFER_NUM_MASK;
    uint32_t               channel_index;

    FSP_ERROR_RETURN(((CAN_BUFFER_TYPE_RX_FIFO == buffer_type) ||
                      (CAN_BUFFER_TYPE_RX_SINGLE == buffer_type)),
                     FSP_ERR_INVALID_ARGUMENT);

    if (CAN_BUFFER_TYPE_RX_SINGLE == buffer_type)
    {
        FSP_ERROR_RETURN(buffer_num < CANFD_PRV_RXMB_MAX, FSP_ERR_UNSUPPORTED);
    }

    if (CAN_BUFFER_TYPE_RX_FIFO == buffer_type)
    {
        FSP_ERROR_RETURN(buffer_num < BSP_FEATURE_CANFD_NUM_RXFIFO + BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL,
                         FSP_ERR_UNSUPPORTED);

        if (buffer_num < CANFD_PRV_CFIFO_OFFSET)
        {
            /* Check Rx FIFO is enabled */
            FSP_ERROR_RETURN(p_global_cfg->rx_fifo_config[buffer_num] & R_CANFD0_CFDRFCC_RFE_Msk,
                             FSP_ERR_INVALID_ARGUMENT);
        }
        else if (buffer_num < CANFD_PRV_CFIFO_OFFSET + BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)
        {
            /*Take index for channel when device have more than one unit*/
            channel_index = p_cfg->channel - CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit;

            uint32_t cfifo_index = buffer_num - CANFD_PRV_CFIFO_OFFSET + channel_index *
                                   BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;

            FSP_ERROR_RETURN((buffer_num - CANFD_PRV_CFIFO_OFFSET) < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL,
                             FSP_ERR_INVALID_ARGUMENT);

            /* Check common FIFO is enabled and operation in receive mode*/
            FSP_ERROR_RETURN(p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable,
                             FSP_ERR_INVALID_ARGUMENT);
            FSP_ERROR_RETURN(CANFD_CFIFO_MODE_RECEIVE == p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.mode,
                             FSP_ERR_INVALID_ARGUMENT);
        }
        else
        {
            /* Do nothing */
        }
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Check buffer receive number is a valid
 * @param[in]  cfifo_cfg_0       common fifo config 0
 * @param[in]  cfifo_cfg_1       common fifo config 1
 **********************************************************************************************************************/
static inline bool r_canfd_cfifo_same_link_check (canfd_cfifo_cfg_t cfifo_cfg_0, canfd_cfifo_cfg_t cfifo_cfg_1)
{
    return !(cfifo_cfg_0.cfifo_cfg_b.enable &&
             cfifo_cfg_1.cfifo_cfg_b.enable &&
             CANFD_CFIFO_MODE_TRANSMIT == cfifo_cfg_0.cfifo_cfg_b.mode &&
             CANFD_CFIFO_MODE_TRANSMIT == cfifo_cfg_1.cfifo_cfg_b.mode &&
             cfifo_cfg_0.cfifo_cfg_b.txmb_link == cfifo_cfg_1.cfifo_cfg_b.txmb_link);
}

/*******************************************************************************************************************//**
 * Check common fifo transmit buffer link doesn't link to buffer in transmit queue
 *
 * @param[in]  p_cfg       canfd config
 **********************************************************************************************************************/
static inline bool r_canfd_cfifo_link_queue_check (const can_cfg_t * p_cfg)
{
    canfd_extended_cfg_t * p_extend_cfg = (canfd_extended_cfg_t *) p_cfg->p_extend;
    canfd_global_cfg_t   * p_global_cfg = (canfd_global_cfg_t *) p_extend_cfg->p_global_cfg;

    /*Take index for channel when device have more than one unit*/
    uint8_t channel_index = p_cfg->channel - CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit;

    /* Only check COM FIFO link Queue for TXQ 2 and TXQ 3
     * TXQ 0 and 1 only uses TXMB 0-15, TXQ 2 and 3 uses TXMB 32-47
     * so cfifo only use TXMB 32 to TXMB 47  */
    if (p_extend_cfg->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_2].tx_queue_cfg_b.enable)
    {
        /* Count the buffers that is used by TXQ 2  */
        uint8_t txmb_limit_txq2 = CANFD_PRV_TX_QUEUE_2_WINDOW +
                                  (p_extend_cfg->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_2].tx_queue_cfg_b.depth);
        for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
        {
            uint32_t cfifo_index = cfifo + channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;

            /* Check COM FIFO is enable and is gateway or transmit, if txmb link of COM FIFO
             * is used for TXQ*/
            if ((p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable) &&
                (CANFD_CFIFO_MODE_RECEIVE != p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.mode) &&
                (p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.txmb_link + CANFD_THIRTYTWO <=
                 txmb_limit_txq2))
            {
                return 0;
            }
        }
    }

    if (p_extend_cfg->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_3].tx_queue_cfg_b.enable)
    {
        uint8_t txmb_limit_txq3 = CANFD_PRV_TXMB_HIGH_BUFFER_NUM -
                                  p_extend_cfg->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_3].tx_queue_cfg_b.depth;
        for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
        {
            /* Check COM FIFO is enable and is gateway or transmit, if txmb link of COM FIFO
             * is used for TXQ*/
            uint32_t cfifo_index = cfifo + channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;
            if ((p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable) &&
                (CANFD_CFIFO_MODE_RECEIVE != p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.mode) &&
                (p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.txmb_link + CANFD_THIRTYTWO >
                 txmb_limit_txq3))
            {
                return 0;
            }
        }
    }

    return 1;
}

#endif

/*******************************************************************************************************************//**
 * Read from a Message Buffer or FIFO.
 *
 * @param[in]     channel            Channel of Channel Common Buffer
 * @param[in]     buffer             Index of buffer to read from (MBs 0-31, FIFOs 0-7, Common FIFO 0-2)
 * @param[in]     p_frame            Pointer to CAN frame to write to
 * @param[in]     p_global_cfg       Pointer to global configuration
 **********************************************************************************************************************/
static void r_canfd_mb_read (uint32_t             channel,
                             can_buffer_t         buffer,
                             can_frame_t * const  p_frame,
                             canfd_global_cfg_t * p_global_cfg)
{
    can_buffer_type_t        buffer_type  = (can_buffer_type_t) (buffer & CANFD_PRV_BUFFER_TYPE_MASK);
    uint32_t                 buffer_index = buffer & CANFD_PRV_BUFFER_NUM_MASK;
    volatile R_CANFD0_Type * p_canfd_reg  = p_global_cfg->p_canfd_reg;

    /* Get pointer to message buffer (FIFOs use the same buffer structure) */
    volatile R_CANFD0_CFDRM_Type * p_mb_regs = NULL;

    if (CAN_BUFFER_TYPE_RX_FIFO == buffer_type)
    {
        if (buffer_index < CANFD_PRV_CFIFO_OFFSET)
        {
            /* Get RX FIFO buffer address*/
            p_mb_regs = (volatile R_CANFD0_CFDRM_Type *) &(p_canfd_reg->CFDRF[buffer_index]);
        }
        else if (buffer_index < CANFD_PRV_CFIFO_OFFSET + BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)
        {
            /* Get CFIFO buffer address */
            uint32_t cfifo;
            cfifo     = buffer_index + channel * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL - CANFD_PRV_CFIFO_OFFSET;
            p_mb_regs = (volatile R_CANFD0_CFDRM_Type *) &(p_canfd_reg->CFDCF[cfifo]);
        }
        else
        {
            /* Selected buffer not available. Do nothing */
        }
    }
    else
    {
        /* Get RXMB buffer address */
        p_mb_regs = &(p_canfd_reg->CFDRM[buffer_index]);
    }

    /* Get frame data. */
    if (p_mb_regs == NULL)
    {
        return;
    }

    uint32_t frame_id = p_mb_regs->ID;

    /* Get the frame type */
    p_frame->type = (can_frame_type_t) ((frame_id & R_CANFD0_CFDRM_ID_RMRTR_Msk) >> R_CANFD0_CFDRM_ID_RMRTR_Pos);

    /* Get frame options */
    uint32_t fdsts = p_mb_regs->FDSTS;
    p_frame->options = CAN_FRAME_OPTIONS_NONE;

    /* Get frame format */
    if (fdsts & R_CANFD0_CFDRM_FDSTS_RMFDF_Msk)
    {
        /* FD Frame */
        p_frame->format = CAN_FRAME_FORMAT_FD;
        if (fdsts & R_CANFD0_CFDRM_FDSTS_RMBRS_Msk)
        {
            /* Get FD BRS bit */
            p_frame->options |= CAN_FRAME_OPTIONS_BRS;
        }

        if (fdsts & R_CANFD0_CFDRM_FDSTS_RMESI_Msk)
        {
            /* Get FD ESI bit */
            p_frame->options |= CAN_FRAME_OPTIONS_ESI;
        }
    }
    else
    {
        /* CAN Classic Frame */
        p_frame->format = CAN_FRAME_FORMAT_CC;
    }

    /* Get the frame ID */
    p_frame->id = frame_id & R_CANFD0_CFDRM_ID_RMID_Msk;

    /* Get the frame ID mode (IDE bit) */
    p_frame->id_mode = (can_id_mode_t) (frame_id >> R_CANFD0_CFDRM_ID_RMIDE_Pos);

    /* Get the frame data length code */
    p_frame->data_length_code = dlc_to_bytes[p_mb_regs->PTR >> R_CANFD0_CFDRM_PTR_RMDLC_Pos];

    /* Get the frame label */
    p_frame->labels = p_mb_regs->FDSTS_b.RMPTR;

    /* Get the frame time stamp */
    p_frame->timestamp = p_mb_regs->PTR_b.RMTS;

    /* Copy data to frame */
    uint32_t           len    = p_frame->data_length_code;
    volatile uint8_t * p_dest = p_frame->data;
    volatile uint8_t * p_src  = (uint8_t *) p_mb_regs->DF;
    while (len--)
    {
        *p_dest++ = *p_src++;
    }

    if (CAN_BUFFER_TYPE_RX_FIFO == buffer_type)
    {
        if (buffer_index < CANFD_PRV_CFIFO_OFFSET)
        {
            /* Increment RX FIFO pointer */
            p_canfd_reg->CFDRFPCTR[buffer_index] = CANFD_PRV_UPDATE_POINTER_CMD;
        }
        else
        {
            /* Increment Common FIFO pointer */
            uint32_t comfifo;
            comfifo = buffer_index + channel * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL - CANFD_PRV_CFIFO_OFFSET;
            p_canfd_reg->CFDCFPCTR[comfifo] = CANFD_PRV_UPDATE_POINTER_CMD;
        }
    }
    else
    {
        /* Clear RXMB New Data bit */
        uint32_t buffer_reg = buffer_index >> 5;             // Divide by 32
        uint32_t buffer_pos = buffer_index & ((1 << 5) - 1); // Get remain of 32
        p_canfd_reg->CFDRMND[buffer_reg] &= ~(1U << buffer_pos);
    }
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to CAN instance control block
 * @param[in]     p_args     Pointer to arguments on stack
 **********************************************************************************************************************/
static void r_canfd_call_callback (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args)
{
    can_callback_args_t args;

    /* Store callback arguments in memory provided by user if available. */
    can_callback_args_t * p_args_memory = p_ctrl->p_callback_memory;
    if (NULL == p_args_memory)
    {
        /* Use provided args struct on stack */
        p_args_memory = p_args;
    }
    else
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        args = *p_args_memory;

        /* Copy the stacked args to callback memory */
        *p_args_memory = *p_args;
    }

    p_ctrl->p_callback(p_args_memory);

    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }
}

/*******************************************************************************************************************//**
 * This function is used to switch the CANFD peripheral operation mode.
 * @param[in]  p_ctrl            - pointer to control structure
 * @param[in]  operation_mode    - destination operation mode
 **********************************************************************************************************************/
static void r_canfd_mode_transition (canfd_instance_ctrl_t * p_ctrl, can_operation_mode_t operation_mode)
{
    uint8_t                  channel_index;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;

    /* Get bit 7 from operation_mode to determine if this is a global mode change request */
    bool global_mode = (bool) (operation_mode >> 7);
    operation_mode &= CANFD_PRV_OPERATION_MODE_MASK;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    if (global_mode)
    {
        uint32_t cfdgctr = p_canfd_reg->CFDGCTR;

        r_canfd_mode_ctr_set(&p_canfd_reg->CFDGCTR, operation_mode);

        /* If CANFD is transitioning out of Reset the FIFOs need to be enabled. */
        if ((cfdgctr & R_CANFD0_CFDGSTS_GRSTSTS_Msk) && !(operation_mode & CAN_OPERATION_MODE_RESET))
        {
            /* Enable RX FIFOs */
            for (uint32_t i = 0; i < CANFD_PRV_RX_FIFO_MAX; i++)
            {
                p_canfd_reg->CFDRFCC[i] = p_global_cfg->rx_fifo_config[i];
            }
        }
    }
    else
    {
        uint32_t cfdcnctr = p_canfd_reg->CFDC[channel_index].CTR;

        if (((cfdcnctr & R_CANFD0_CFDC_CTR_CSLPR_Msk) && (!(CAN_OPERATION_MODE_RESET & operation_mode))) ||
            ((!(cfdcnctr & CANFD_PRV_CTR_RESET_BIT)) && (CAN_OPERATION_MODE_SLEEP == operation_mode)))
        {
            /* Transition channel to Reset if a transition to/from Sleep is requested (see Section "Channel
             * Modes" in the RH850/U2B microcontroller User's Manual for details) */
            r_canfd_mode_ctr_set(&p_canfd_reg->CFDC[channel_index].CTR, CAN_OPERATION_MODE_RESET);
        }

        /* Request transition to selected mode */
        r_canfd_mode_ctr_set(&p_canfd_reg->CFDC[channel_index].CTR, operation_mode);
    }

    p_ctrl->operation_mode =
        (can_operation_mode_t) (p_canfd_reg->CFDC[channel_index].CTR & CANFD_PRV_CTR_MODE_MASK);
}

/*******************************************************************************************************************//**
 * Sets the provided CTR register to the requested mode and waits for the associated STS register to reflect the change
 * @param[in]  p_ctr_reg            - pointer to control register
 * @param[in]  operation_mode       - requested mode (not including global bits)
 **********************************************************************************************************************/
static void r_canfd_mode_ctr_set (volatile uint32_t * p_ctr_reg, can_operation_mode_t operation_mode)
{
    volatile uint32_t * p_sts_reg = p_ctr_reg + 1;

    /* See definitions for CFDCnCTR, CFDCnSTS, CFDGCTR and CFDGSTS in the RH850/U2B microcontroller User's Manual */
    *p_ctr_reg = (*p_ctr_reg & ~CANFD_PRV_CTR_MODE_MASK) | operation_mode;
    FSP_HARDWARE_REGISTER_WAIT((*p_sts_reg & CANFD_PRV_CTR_MODE_MASK), operation_mode);
}

/*******************************************************************************************************************//**
 * Caculate number of channels for unit
 * @param[in]  unit            - Unit of global config
 **********************************************************************************************************************/
static inline uint32_t r_canfd_caculate_no_channel (uint8_t unit)
{
    /*Declare number of channel of unit*/
    uint32_t no_channel     = 0;
    uint32_t remain_channel = 0;

    /*Caculate base register*/
    uint8_t base = unit * CANFD_PRV_CHANNELS_OFFSET;

    /* Calculated number of channels of unit */
    remain_channel =
        (BSP_FEATURE_CANFD_NUM_CHANNELS <= base) ? 0U : (uint32_t) (BSP_FEATURE_CANFD_NUM_CHANNELS - base);

    no_channel =
        (remain_channel >= CANFD_PRV_CHANNELS_OFFSET) ? CANFD_PRV_CHANNELS_OFFSET : remain_channel;

    return no_channel;
}

/*******************************************************************************************************************//**
 * Caculate max ram page for chosed unit
 * @param[in]  unit            - Unit of global config
 * @param[in]     ram_field       Field RAM to test
 **********************************************************************************************************************/
static uint32_t r_canfd_ram_caculate_max_page (uint8_t unit, canfd_ramtest_field_t ram_field)
{
    /* Declare number of channel of unit */
    uint32_t no_channel = 0;

    /* Max bytes of RAM of unit */
    uint32_t max_ram_unit = 0;

    /* Max pages of RAM of unit */
    uint32_t max_ram_page = 0;

    /* Get number channels of unit */
    no_channel = r_canfd_caculate_no_channel(unit);

    /* Check current RAM test field and caculate max ram per unit */
    if (ram_field)
    {
#if BSP_FEATURE_CANFD_PN_MODE

        /* Caculate ram if ram test field is PNF field */
        max_ram_unit = no_channel * BSP_FEATURE_CANFD_PFLRAM_PER_CHANNEL;
#endif
    }
    else
    {
        /* Caculate ram if ram test field is AFL MRAM field */
        max_ram_unit = no_channel * BSP_FEATURE_CANFD_AFLMRAM_PER_CHANNEL;
    }

    /* One page RAM have 64bytes. Max pages is equal all bytes divide number bytes of 1 page */
    max_ram_page = (max_ram_unit + CANFD_RAM_NUM_BYTES_PER_PAGE - CANFD_ONE) / CANFD_RAM_NUM_BYTES_PER_PAGE;

    return max_ram_page;
}

/*******************************************************************************************************************//**
 * Caculate the unaccessed byte for the last page
 * @param[in]     p_global_cfg       Pointer to global configuration
 **********************************************************************************************************************/
static uint32_t r_canfd_ram_caculate_limit_byte (canfd_global_cfg_t * p_global_cfg)
{
    uint32_t          max_byte_per_unit    = 0;
    uint32_t          actual_byte_per_unit = 0;
    canfd_ramtest_t * p_infor_ram          = p_global_cfg->p_ram_infor;

    uint32_t no_channel = r_canfd_caculate_no_channel(p_global_cfg->unit);

    /*Check current RAM test field and caculate max ram per unit*/
    if (p_infor_ram->ram_field)
    {
#if BSP_FEATURE_CANFD_PN_MODE

        /* Caculate ram if ram test field is PNF field */
        max_byte_per_unit    = p_infor_ram->ram_max_page * CANFD_RAM_NUM_BYTES_PER_PAGE;
        actual_byte_per_unit = no_channel * BSP_FEATURE_CANFD_PFLRAM_PER_CHANNEL;
#endif
    }
    else
    {
        /* Caculate ram if ram test field is AFL MRAM field */
        max_byte_per_unit    = p_infor_ram->ram_max_page * CANFD_RAM_NUM_BYTES_PER_PAGE;
        actual_byte_per_unit = no_channel * BSP_FEATURE_CANFD_AFLMRAM_PER_CHANNEL;
    }

    /* Calculate the number of unaccessed RAM entries */
    p_infor_ram->unaccessed_ram_entry = (max_byte_per_unit - actual_byte_per_unit) / (sizeof(uint32_t));

    return p_infor_ram->unaccessed_ram_entry;
}

/*******************************************************************************************************************//**
 * Initialize CANFD RAM test data for accessible RAM pages
 * @param[in]  p_global_cfg       Pointer to global configuration
 * @param[in]  data_ram           Ram data to fill to register
 **********************************************************************************************************************/
static void r_canfd_set_ram_data (canfd_global_cfg_t * p_global_cfg, canfd_ramtest_data_t data_ram)
{
    volatile R_CANFD0_Type * p_canfd_reg   = p_global_cfg->p_canfd_reg;
    uint32_t                 ram_init_data = 0;
    uint32_t                 entry_access  = 0;
    canfd_ramtest_t        * infor_ram     = p_global_cfg->p_ram_infor;

    /* Setting data ram which is filled into register */
    if (CAN_RAMTEST_DATA_1 == data_ram)
    {
        ram_init_data = CANFD_RAM_TEST_SET_DATA;
    }
    else
    {
        ram_init_data = CANFD_RAM_TEST_CLEAR_DATA;
    }

    /* Caculate number of pages which can be access */
    if (infor_ram->unaccessed_ram_entry)
    {
        entry_access = CANFD_RAM_PAGE_SIZE_ACCESS_REG - infor_ram->unaccessed_ram_entry;
    }
    else
    {
        entry_access = CANFD_RAM_PAGE_SIZE_ACCESS_REG;
    }

    /* Fill all bits with RAM init data */
    for (uint8_t ram_entry = 0; ram_entry < entry_access; ram_entry++)
    {
        p_canfd_reg->CFDRPGACC[ram_entry] = ram_init_data;
    }
}

/*******************************************************************************************************************//**
 * Execute CANFD RAM check algorithm using alternating data patterns
 * @param[in]  p_global_cfg       Pointer to global configuration
 **********************************************************************************************************************/
static fsp_err_t r_canfd_ram_check_algorithm (canfd_global_cfg_t * p_global_cfg)
{
    volatile R_CANFD0_Type * p_canfd_reg  = p_global_cfg->p_canfd_reg;
    volatile uint32_t      * p_gacc_reg   = &p_canfd_reg->CFDRPGACC[0];
    uint32_t                 test_data    = CANFD_RAM_TEST_DATA;
    uint32_t                 entry_access = 0;
    canfd_ramtest_t        * infor_ram    = p_global_cfg->p_ram_infor;

    if (infor_ram->unaccessed_ram_entry)
    {
        /* Caculate number of pages which can be access */
        entry_access = CANFD_RAM_PAGE_SIZE_ACCESS_REG - infor_ram->unaccessed_ram_entry;
    }
    else
    {
        entry_access = CANFD_RAM_PAGE_SIZE_ACCESS_REG;
    }

    /* Write 0 and 1 in consecutive locations */
    for (uint8_t test_num = 0; test_num < 2; test_num++)
    {
        /* Write 0 and 1 in consecutive locations */
        for (uint8_t size_id = 0; size_id < entry_access; size_id++)
        {
            *(p_gacc_reg + size_id) = test_data;
            test_data               = ~test_data;
        }

        /* Read 0 and 1 in consecutive locations */
        for (uint8_t size_id = 0; size_id < entry_access; size_id++)
        {
            if (*(p_gacc_reg + size_id) != test_data)
            {
                return FSP_ERR_INVALID_DATA;
            }

            test_data = ~test_data;
        }

        test_data = ~CANFD_RAM_TEST_DATA;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Execute CANFD RAM check walkpath algorithm using alternating data patterns
 * @param[in]  p_global_cfg       Pointer to global configuration
 * @param[in]  walk_type          Walk test type (walk-0 or walk-1).
 **********************************************************************************************************************/
static fsp_err_t r_canfd_ram_walkpath_algorithm (canfd_global_cfg_t * p_global_cfg, canfd_ramtest_walk_type_t walk_type)
{
    volatile R_CANFD0_Type * p_canfd_reg   = p_global_cfg->p_canfd_reg;
    canfd_ramtest_data_t     ram_fill_data = CAN_RAMTEST_DATA_0;
    uint32_t                 ram_base_data = 0;
    uint32_t                 total_bit     = 0;
    uint32_t                 base_entry    = 0;
    uint32_t                 base_bit      = 0;
    volatile uint32_t      * p_gacc_reg;

    uint32_t          entry_access;
    canfd_ramtest_t * infor_ram = p_global_cfg->p_ram_infor;

    if (infor_ram->unaccessed_ram_entry)
    {
        /* Caculate number of pages which can be access */
        entry_access = CANFD_RAM_PAGE_SIZE_ACCESS_REG - infor_ram->unaccessed_ram_entry;
    }
    else
    {
        entry_access = CANFD_RAM_PAGE_SIZE_ACCESS_REG;
    }

    /* Setting data ram which is filled into register */
    if (CAN_RAMTEST_WALK_TYPE_0 == walk_type)
    {
        ram_fill_data = CAN_RAMTEST_DATA_1;
        ram_base_data = CANFD_RAM_TEST_SET_DATA;
    }
    else
    {
        ram_fill_data = CAN_RAMTEST_DATA_0;
        ram_base_data = CANFD_RAM_TEST_CLEAR_DATA;
    }

    /* Fill all data with either zero or one */
    r_canfd_set_ram_data(p_global_cfg, ram_fill_data);

    total_bit = entry_access << CANFD_RAM_TEST_CELL_DIVIDER;

    /* Fill all bits with RAM init data */
    for (uint32_t bit_num = 0; bit_num < total_bit; bit_num++)
    {
        base_entry = (uint8_t) (bit_num >> CANFD_RAM_TEST_CELL_DIVIDER);

        base_bit   = (uint8_t) (bit_num & sizeof(uint32_t) - 1);
        p_gacc_reg = &p_canfd_reg->CFDRPGACC[0];

        /* Invert the base bit */
        *(p_gacc_reg + base_entry) ^= (uint32_t) (1U << base_bit);

        /* Verify that only the target bit is toggled */
        for (uint32_t entry_num = 0; entry_num < entry_access; entry_num++)
        {
            if (entry_num == base_entry)
            {
                if (*(p_gacc_reg + base_entry) != (ram_base_data ^ ((uint32_t) 1 << base_bit)))
                {
                    return FSP_ERR_INVALID_DATA;
                }
            }
            else if (*(p_gacc_reg + entry_num) != ram_base_data)
            {
                return FSP_ERR_INVALID_DATA;
            }
        }

        /* Restore the original RAM content */
        *(p_gacc_reg + base_entry) ^= (uint32_t) (1U << base_bit);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Handle transmit buffer interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 * @param[in]  event        - Event to handle, CAN_EVENT_TX_COMPLETE and CAN_EVENT_TX_ABORTED only.
 **********************************************************************************************************************/
static void r_canfd_handle_txmb (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args, can_event_t event)
{
    uint32_t                 txmb;
    uint8_t                  channel_index;
    volatile uint32_t      * p_cfdtm_sts;
    uint32_t                 cfdtm_sts_index;
    uint8_t                  lend_txmb;
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index   = p_ctrl->channel_index;
    cfdtm_sts_index = channel_index << 1;

    /* Get number of lend buffer on this channel */
    lend_txmb = CANFD_PRV_LEND_BUFF_GET(p_canfd_reg, channel_index);

    /* Get relevant TX status register bank */
    if (event == CAN_EVENT_TX_COMPLETE)
    {
        p_cfdtm_sts   = (volatile uint32_t *) &p_canfd_reg->CFDTMTCSTS[cfdtm_sts_index];
        p_args->event = CAN_EVENT_TX_COMPLETE;
    }
    else
    {
        p_cfdtm_sts   = (volatile uint32_t *) &p_canfd_reg->CFDTMTASTS[cfdtm_sts_index];
        p_args->event = CAN_EVENT_TX_ABORTED;
    }

    /* Calculate lowest TXMB with the specified event */
    txmb = SCH1R(*p_cfdtm_sts & (~CANFD_PRV_FILL_1(p_ctrl->txmb_range_min))) - 1;

    /* Check whether the TXMB is out of range */
    if (txmb == CANFD_PRV_TXMB_OUT_RANGE)
    {
        /* Check the next TXMB status register within the same channel.
         * If no request transmit buffer exists, jump to use the lent transmit buffer register. */
        if (*(p_cfdtm_sts + 1) == 0)
        {
            /* Use lent TXMBs only if lending is enabled */
            if (lend_txmb != 0)
            {
                txmb = SCH1R(*(p_cfdtm_sts + 2)) - 1 + CANFD_PRV_TXMB_CHANNEL_OFFSET;
            }
        }
        else
        {
            /* TXMB found in the next status register within the same channel */
            txmb = SCH1R(*(p_cfdtm_sts + 1)) - 1 + CANFD_PRV_TXMB_OFFSET_IN_CHANNEL;
        }
    }

    /* Only handle if the txmb is belonged in this channel */
    if ((txmb >= p_ctrl->txmb_range_min) && (txmb < p_ctrl->txmb_range_max))
    {
        /* Clear TX complete/abort flags */
        p_canfd_reg->CFDTMSTS_b[txmb + (CANFD_PRV_TXMB_CHANNEL_OFFSET * channel_index)].TMTRF = 0;

        /* Dummy read to ensure that interrupt event is cleared. */
        volatile uint32_t dummy =
            p_canfd_reg->CFDTMSTS[txmb + (CANFD_PRV_TXMB_CHANNEL_OFFSET * channel_index)];
        FSP_PARAMETER_NOT_USED(dummy);

        /* Set the callback arguments */
        p_args->buffer = (can_buffer_t) (txmb);

        /* Call user callback */
        r_canfd_call_callback(p_ctrl, p_args);
    }
}

/*******************************************************************************************************************//**
 * Handle transmit queue interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 **********************************************************************************************************************/
static void r_canfd_handle_txqueue (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args)
{
    uint32_t                 queue_bit_val;
    uint32_t                 queue_num;
    uint32_t                 queue_channel;
    uint32_t                 queue_index;
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint8_t channel_index;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Check status of Transmit Queue interrupt */
    queue_bit_val = p_canfd_reg->CFDTXQISTS >> (channel_index * CANFD_PRV_TXQ_CH_OFFSET);

    while (queue_bit_val != 0)
    {
        /*Check queue buffer value*/
        queue_num = SCH1R(queue_bit_val) - 1;

        /* Only handle if the transmit queue is belonged in this channel */
        if ((queue_num >= p_ctrl->txqueue_range_min) && (queue_num < p_ctrl->txqueue_range_max))
        {
            /* Get index and channel of tx queue */
            if (queue_num < CANFD_PRV_TXQ_CH_OFFSET)
            {
                /* Transmit queue belong the channel */
                queue_index   = queue_num;
                queue_channel = channel_index;
            }
            else
            {
                /* Transmit queue lend from the consecutive channel */
                queue_index   = queue_num - CANFD_PRV_TXQ_CH_OFFSET;
                queue_channel = channel_index + 1;
            }

            /* Check Transmit Queue Event */
            if (p_canfd_reg->CFDTXQ[queue_index].STS_b[queue_channel].TXQEMP)
            {
                p_args->event = CAN_EVENT_TX_QUEUE_EMPTY;
            }
            else
            {
                p_args->event = CAN_EVENT_TX_COMPLETE;
            }

            p_args->buffer = (can_buffer_t) (CAN_BUFFER_TYPE_TX_PRIORITY | queue_num);

            /* Clear Transmit Queue Interrupt Flag */
            p_canfd_reg->CFDTXQ[queue_index].STS_b[queue_channel].TXQTXIF = 0;

            /* Call user callback */
            r_canfd_call_callback(p_ctrl, p_args);
        }

        queue_bit_val &= ~(1U << queue_num);
    }
}

/*******************************************************************************************************************//**
 * Handle transmit common fifo interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 **********************************************************************************************************************/
static void r_canfd_handle_txcfifo (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args)
{
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint8_t channel_index;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get zero-based buffer index for the specified CFIFO channel */
    uint32_t buffer =
        SCH1R(p_canfd_reg->CFDCFTISTS >> (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)) - 1;
    uint32_t buffer_index = buffer + channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;
    if (p_canfd_reg->CFDCFCC_b[buffer_index].CFTXIE)
    {
        /* Get Common FIFO Event */
        p_args->event =
            p_canfd_reg->CFDCFSTS_b[buffer_index].CFEMP ? CAN_EVENT_TX_FIFO_EMPTY :
            CAN_EVENT_TX_COMPLETE;
        p_args->buffer = (can_buffer_t) (buffer | CAN_BUFFER_TYPE_TX_FIFO);

        /* Clear Common FIFO Transmit Interrupt Flag */
        p_canfd_reg->CFDCFSTS_b[buffer_index].CFTXIF = 0;

        /* Call user callback */
        r_canfd_call_callback(p_ctrl, p_args);
    }
}

/*******************************************************************************************************************//**
 * Handle transmit history interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 **********************************************************************************************************************/
static void r_canfd_handle_txhistory (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args)
{
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;
    uint8_t  buffer_type;
    uint32_t buffer_num;
    uint8_t  channel_index;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get pointer to transmit history register */
    volatile R_CANFD0_CFDTHL_Type * p_txh_regs = NULL;

    /*Get label transmitted successfully*/
    p_txh_regs = (volatile R_CANFD0_CFDTHL_Type *) &(p_canfd_reg->CFDTHL[channel_index]);

    while (!p_canfd_reg->CFDTHLSTS_b[channel_index].THLEMP)
    {
        /*Get correct label of AFL label*/
        p_args->label = p_txh_regs->ACC1_b.TID;

        /* Caculate buffer type
         *  b001: Flat TX Message Buffer
         *  b010: TX FIFO MB Number and GW FIFO MB Number
         *  b100: TX Queue MB Number.
         */
        buffer_type = p_txh_regs->ACC0_b.BT;
        buffer_num  = p_txh_regs->ACC0_b.BN;

        if (CANFD_THL_BUFF_TYPE_TXQUEUE == buffer_type)
        {
            r_canfd_thl_handle_txqueue(p_ctrl, p_args, buffer_num, channel_index);
        }
        else if (CANFD_THL_BUFF_TYPE_TXFIFO == buffer_type)
        {
            r_canfd_thl_handle_txfifo(p_ctrl, p_args, buffer_num, channel_index);
        }
        else
        {
            r_canfd_thl_handle_txmb(p_ctrl, p_args, buffer_num, channel_index);
        }

        /*Update to next TX History Entry Slot*/
        p_canfd_reg->CFDTHLPCTR[channel_index] = CANFD_PRV_UPDATE_POINTER_CMD;
    }

#ifdef BSP_FEATURE_CANFD_FFI_MODE

    /* Get the channel if FFI mode is used. */
    if (p_canfd_reg->CFDGFFIMC_b.FFIEN)
    {
        p_args->channel = p_txh_regs->ACC0_b.CH;
    }
#endif

    /* Clear Interrupt Flag */
    p_canfd_reg->CFDTHLSTS_b[p_args->channel].THLIF = 0;
}

/*******************************************************************************************************************//**
 * Converts bytes into a DLC value
 * @param[in]  bytes       Number of payload bytes
 **********************************************************************************************************************/
static uint8_t r_canfd_bytes_to_dlc (uint16_t bytes)
{
    /* Count number of DLC values to payload size in bytes */
    uint8_t length_dlc_to_byte = sizeof(dlc_to_bytes) / sizeof(dlc_to_bytes[CANFD_PRV_DATA_LENGTH_CODE_0]);

    /* Return dlc with bytes if the value ranges from 0 to 8 */
    if (bytes <= CANFD_PRV_DATA_LENGTH_CODE_8)
    {
        return (uint8_t) bytes;
    }

    /* Return dlc with bytes if the value ranges from 9 to 64 */
    for (uint8_t dlc = CANFD_PRV_DATA_LENGTH_CODE_9; dlc < length_dlc_to_byte; dlc++)
    {
        if (bytes <= dlc_to_bytes[dlc])
        {
            return dlc;
        }
    }

    return CANFD_PRV_DATA_LENGTH_CODE_15;
}

/*******************************************************************************************************************//**
 * Check all channel is closed
 *  * @param[in]  unit       Unit number
 **********************************************************************************************************************
 **********************************************************************************************************************/
static bool r_canfd_is_all_channel_close (uint8_t unit)
{
    uint8_t start_channel = unit * CANFD_PRV_CHANNELS_OFFSET;

    /* Calculate number of channels belonging to this unit */
    uint8_t no_channel = r_canfd_caculate_no_channel(unit);

    for (uint8_t ch = start_channel; ch < no_channel + start_channel; ch++)
    {
        if (gp_ctrl[ch] != NULL)
        {
            return false;
        }
    }

    return true;
}

/*******************************************************************************************************************//**
 * Handle txfifo for Transmit history
 **********************************************************************************************************************
 **********************************************************************************************************************/
static inline void r_canfd_thl_handle_txfifo (canfd_instance_ctrl_t * const p_ctrl,
                                              can_callback_args_t * const   p_args,
                                              uint32_t                      buffer_num,
                                              uint32_t                      channel)
{
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint8_t channel_index;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get COMFIFO index */
    uint32_t cfifo = 0;
    for (cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
    {
        /* Available buffer for TXCOMFIFO is from 32 to 63, so CANFD_THIRTYTWO is offset */
        if (buffer_num ==
            ((uint32_t) (p_canfd_reg->CFDCFCC_b[cfifo * channel_index].CFTML) + (uint32_t) CANFD_THIRTYTWO))
        {
            /* Get Common FIFO Event */
            p_args->event =
                p_canfd_reg->CFDCFSTS_b[cfifo + channel *
                                        BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL].CFEMP ? CAN_EVENT_TX_FIFO_EMPTY :
                CAN_EVENT_TX_COMPLETE;
            p_args->buffer = (can_buffer_t) (cfifo | CAN_BUFFER_TYPE_TX_FIFO);
            break;
        }
    }

    /* Call user callback */
    r_canfd_call_callback(p_ctrl, p_args);

    /* Clear Common FIFO Transmit Interrupt Flag */
    p_canfd_reg->CFDCFSTS_b[cfifo * channel * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL].CFTXIF = 0;
}

/*******************************************************************************************************************//**
 * Handle txmb for Transmit history
 **********************************************************************************************************************
 **********************************************************************************************************************/
static inline void r_canfd_thl_handle_txmb (canfd_instance_ctrl_t * const p_ctrl,
                                            can_callback_args_t * const   p_args,
                                            uint32_t                      buffer_num,
                                            uint32_t                      channel)
{
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /* Default event: TX complete */
    p_args->event = CAN_EVENT_TX_COMPLETE;

    /* Only handle if TXMB belongs to this channel */
    if ((buffer_num >= p_ctrl->txmb_range_min) && (buffer_num < p_ctrl->txmb_range_max))
    {
        uint32_t buffer_index = buffer_num + (CANFD_PRV_TXMB_CHANNEL_OFFSET * channel);

        /* Clear TX complete/abort flag */
        p_canfd_reg->CFDTMSTS_b[buffer_index].TMTRF = 0;

        /* Dummy read to ensure the clear takes effect */
        volatile uint32_t dummy = p_canfd_reg->CFDTMSTS[buffer_index];
        FSP_PARAMETER_NOT_USED(dummy);

        /* Callback args */
        p_args->buffer = (can_buffer_t) (buffer_num | CAN_BUFFER_TYPE_TX_SINGLE);

        /* User callback */
        r_canfd_call_callback(p_ctrl, p_args);
    }
}

/*******************************************************************************************************************//**
 * Handle txqueue for Transmit history
 **********************************************************************************************************************
 **********************************************************************************************************************/
static inline void r_canfd_thl_handle_txqueue (canfd_instance_ctrl_t * const p_ctrl,
                                               can_callback_args_t * const   p_args,
                                               uint32_t                      buffer_num,
                                               uint32_t                      channel)
{
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint32_t                 queue_num;
    uint8_t channel_index;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    if (buffer_num < (uint32_t) CANFD_THIRTYTWO)
    {
        /* TX Queues 0 and 1 are available from TXMB0 to TXMB16 (on devices U2Bx, U2Cx) and from TXMB0 to TXMB31 (on device U2Ax, except U2A6). */
        if (buffer_num <= (uint32_t) (p_canfd_reg->CFDTXQ[CANFD_PRV_TXQ_INDEX_0].CC_b[channel_index].TXQDC))
        {
            queue_num = CANFD_PRV_TXQ_INDEX_0;
        }
        else
        {
            queue_num = CANFD_PRV_TXQ_INDEX_1;
        }
    }
    else
    {
        /* TX Queue 2, 3 are available from TXMB32 to TXMB47 (on devices U2Bx, U2Cx) and from TXMB32 to TXMB63 (on device U2Ax, except U2A6), so CANFD_THIRTYTWO is offset buffer  */
        uint32_t offset = buffer_num - (uint32_t) CANFD_THIRTYTWO;
        if (offset <= (uint32_t) (p_canfd_reg->CFDTXQ[CANFD_PRV_TXQ_INDEX_3].CC_b[channel_index].TXQDC))
        {
            queue_num = CANFD_PRV_TXQ_INDEX_2;
        }
        else
        {
            queue_num = CANFD_PRV_TXQ_INDEX_3;
        }
    }

    /* Check Transmit Queue Event */
    if (p_canfd_reg->CFDTXQ[queue_num].STS_b[channel_index].TXQEMP)
    {
        p_args->event = CAN_EVENT_TX_QUEUE_EMPTY;
    }
    else
    {
        p_args->event = CAN_EVENT_TX_COMPLETE;
    }

    /* Clear queue TX interrupt flag */
    p_canfd_reg->CFDTXQ[queue_num].STS_b[channel_index].TXQTXIF = 0;

    /* Encode buffer info (priority/queue) */
    p_args->buffer = (can_buffer_t) (queue_num | CAN_BUFFER_TYPE_TX_PRIORITY);

    /* User callback */
    r_canfd_call_callback(p_ctrl, p_args);
}

/*******************************************************************************************************************//**
 * Handle transmit queue message error interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 **********************************************************************************************************************/
static void r_canfd_handle_txqueue_error (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args)
{
    uint32_t                 queue_bit_val;
    uint32_t                 queue_num;
    uint32_t                 queue_channel;
    uint32_t                 queue_index;
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint8_t           channel_txq_index;
    volatile uint32_t dummy;
    bool              is_msg_lost = false;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /* Get the index of the lost TXQ message. */
    queue_bit_val = p_canfd_reg->CFDTXQMSTS;
    if (queue_bit_val != 0)
    {
        is_msg_lost = true;
    }
    else
    {
        /* Get the index of the overwrite TXQ message. */
        queue_bit_val = p_canfd_reg->CFDTXQOWSTS;
    }

    /* Check status of Transmit Queue message lost interrupt */
    for (uint8_t channel = 0; channel < CANFD_MAX_CHANNEL_PER_UNIT; channel++)
    {
        if (queue_bit_val & (CANFD_PRV_TXQ_MSK << (CANFD_PRV_TXQ_CH_OFFSET * channel)))
        {
            queue_bit_val   >>= (CANFD_PRV_TXQ_CH_OFFSET * channel);
            channel_txq_index = channel;
            break;
        }
    }

    while (queue_bit_val != 0)
    {
        /*Check queue buffer value*/
        queue_num = SCH1R(queue_bit_val) - 1;

        /* Only handle if the transmit queue is belonged in this channel */
        if ((queue_num >= p_ctrl->txqueue_range_min) && (queue_num < p_ctrl->txqueue_range_max))
        {
            /* Get index and channel of tx queue */
            if (queue_num < CANFD_PRV_TXQ_CH_OFFSET)
            {
                /* Transmit queue belong the channel */
                queue_index   = queue_num;
                queue_channel = channel_txq_index;
            }
            else
            {
                /* Transmit queue lend from the consecutive channel */
                queue_index   = queue_num - CANFD_PRV_TXQ_CH_OFFSET;
                queue_channel = channel_txq_index + 1;
            }

            p_args->buffer = (can_buffer_t) (queue_index);
            if (is_msg_lost)
            {
                p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel] = ~R_CANFD0_CFDTXQ_STS_TXQMLT_Msk;
            }
            else
            {
                p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel] = ~R_CANFD0_CFDTXQ_STS_TXQMOW_Msk;
            }

            /* Dummy read to ensure that interrupt event is cleared. */
            dummy = p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel];
            FSP_PARAMETER_NOT_USED(dummy);
        }

        queue_bit_val &= ~(1U << queue_num);
    }
}

#ifdef BSP_FEATURE_CANFD_FFI_MODE

/*******************************************************************************************************************//**
 * Handle ffi transmit queue message error interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 **********************************************************************************************************************/
static void r_canfd_handle_txqueue_ffi_error (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args)
{
    uint32_t                 queue_bit_val;
    uint32_t                 queue_num;
    uint32_t                 queue_channel;
    uint32_t                 queue_index;
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint8_t           channel_index;
    volatile uint32_t dummy;
    bool              is_msg_lost = false;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Check status of Transmit Queue message lost interrupt */
    queue_bit_val = p_canfd_reg->CFDTXQMSTS >> (channel_index * CANFD_PRV_TXQ_CH_OFFSET);
    if (queue_bit_val != 0)
    {
        is_msg_lost = true;
    }
    else
    {
        /* Check status of Transmit Queue message overwrite interrupt */
        queue_bit_val = p_canfd_reg->CFDTXQOWSTS >> (channel_index * CANFD_PRV_TXQ_CH_OFFSET);
    }

    while (queue_bit_val != 0)
    {
        /*Check queue buffer value*/
        queue_num = SCH1R(queue_bit_val) - 1;

        /* Only handle if the transmit queue is belonged in this channel */
        if ((queue_num >= p_ctrl->txqueue_range_min) && (queue_num < p_ctrl->txqueue_range_max))
        {
            /* Get index and channel of tx queue */
            if (queue_num < CANFD_PRV_TXQ_CH_OFFSET)
            {
                /* Transmit queue belong the channel */
                queue_index   = queue_num;
                queue_channel = channel_index;
            }
            else
            {
                /* Transmit queue lend from the consecutive channel */
                queue_index   = queue_num - CANFD_PRV_TXQ_CH_OFFSET;
                queue_channel = channel_index + 1;
            }

            p_args->buffer = (can_buffer_t) queue_index;
            if (is_msg_lost)
            {
                p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel] = ~R_CANFD0_CFDTXQ_STS_TXQMLT_Msk;
            }
            else
            {
                p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel] = ~R_CANFD0_CFDTXQ_STS_TXQMOW_Msk;
            }

            /* Dummy read to ensure that interrupt event is cleared. */
            dummy = p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel];
            FSP_PARAMETER_NOT_USED(dummy);
        }

        queue_bit_val &= ~(1U << queue_num);
    }

    if (is_msg_lost)
    {
        p_args->error |= CANFD_ERROR_CHANNEL_VM_TXQ_CFIFO_MESSAGE_LOST;
    }
    else
    {
        p_args->error |= CANFD_ERROR_CHANNEL_VM_TXQ_MESSAGE_OVERWRITE;
    }
}

#endif

/*******************************************************************************************************************//**
 * Setting filter rule for AFL and PNF
 * @param[in]  p_ctrl       - pointer to canfd control block
 **********************************************************************************************************************/
static void r_canfd_setting_filter_rule (canfd_instance_ctrl_t * p_ctrl)
{
    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    canfd_extended_cfg_t * p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    canfd_global_cfg_t     * p_global_cfg = p_extend->p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg  = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    uint8_t channel_index = p_ctrl->channel_index;

    /* Get AFL entry and limit */
    uint32_t afl_entry = 0;
    uint32_t afl_max   = p_global_cfg->afl_rule_num[0];
    for (uint32_t ch = 1; ch <= channel_index; ch++)
    {
        afl_entry += p_global_cfg->afl_rule_num[ch - 1];
        afl_max   += p_global_cfg->afl_rule_num[ch];
    }

    /* Unlock AFL */
    p_canfd_reg->CFDGAFLECTR |= R_CANFD0_CFDGAFLECTR_AFLDAE_Msk;

    /* Write all configured Receive rule entries */
    R_CANFD0_CFDGAFL_Type * p_afl = (R_CANFD0_CFDGAFL_Type *) p_extend->p_afl;
    for ( ; afl_entry < afl_max; afl_entry++)
    {
        /* Set AFL page */
        p_canfd_reg->CFDGAFLECTR = (afl_entry >> 4) | R_CANFD0_CFDGAFLECTR_AFLDAE_Msk;

        /* Get pointer to current AFL rule and set it to the rule pointed to by p_afl */
        volatile R_CANFD0_CFDGAFL_Type * cfdgafl = &p_canfd_reg->CFDGAFL[afl_entry & 0x0F];
        *cfdgafl = *p_afl;
        p_afl++;
    }

    /* Lock AFL */
    p_canfd_reg->CFDGAFLECTR = 0;

    /* Get PNFL entry and limit */
    uint32_t pnf_entry = 0;
    uint32_t pnf_max   = p_global_cfg->pnf_rule_num[0];
    for (uint32_t ch = 1; ch <= channel_index; ch++)
    {
        pnf_entry += p_global_cfg->pnf_rule_num[ch - 1];
        pnf_max   += p_global_cfg->pnf_rule_num[ch];
    }

#ifdef BSP_FEATURE_CANFD_PN_MODE

    /* Unlock PNFL */
    p_canfd_reg->CFDGPFLECTR |= R_CANFD0_CFDGPFLECTR_PFLDAE_Msk;

    /* Write all configured Receive rule entries for PNFL */
    canfd_pnf_entry_t * p_pnf = (canfd_pnf_entry_t *) p_extend->p_pnf;
    for ( ; pnf_entry < pnf_max; pnf_entry++)
    {
        /* Set PNFL page */
        p_canfd_reg->CFDGPFLECTR = (pnf_entry >> 2) | R_CANFD0_CFDGPFLECTR_PFLDAE_Msk;

        /* Get pointer to current PNFL rule and set it to the rule pointed to by p_pnf */
        volatile R_CANFD0_CFDGPFL_Type * cfdgpnf = &p_canfd_reg->CFDGPFL[pnf_entry & 0x03];

        /*Setting PNF rule into register */
        r_canfd_copy_pnf_rule(cfdgpnf, p_pnf);
        p_pnf++;
    }

    /* Lock PNFL */
    p_canfd_reg->CFDGPFLECTR = 0;
#endif
}

#ifdef BSP_FEATURE_CANFD_PN_MODE

/*******************************************************************************************************************//**
 * Setting filter rule for PNF
 * @param[in]  p_reg                - pointer to control register
 * @param[in]  p_entry              - pointer to list entry of pnf
 **********************************************************************************************************************/
static void r_canfd_copy_pnf_rule (volatile R_CANFD0_CFDGPFL_Type * p_reg, const canfd_pnf_entry_t * p_entry)
{
    /* Write the filter ID (CAN ID to match) */
    p_reg->ID = p_entry->id.id_u32;

    /* Write the ID mask (defines which bits are compared) */
    p_reg->MSK = p_entry->mask.mask_u32;

    /* Write destination information (Pointer 0 and Pointer 1)
     *  Specifies where to store or process the frame when matched*/
    p_reg->P0 = p_entry->destination.destination_u32[0];
    p_reg->P1 = p_entry->destination.destination_u32[1];

    /*Write the payload type (defines the type of data to check)*/
    p_reg->PT = p_entry->payload_type.payload_type_u32;

    /* Write payload compare values and masks:
     * - PD0/PM0: data and mask for the first part of the payload
     * - PD1/PM1: data and mask for the next part of the payload */
    p_reg->PD0 = p_entry->payload_compare.payload_compare_u32[0];
    p_reg->PM0 = p_entry->payload_compare.payload_compare_u32[1];
    p_reg->PD1 = p_entry->payload_compare.payload_compare_u32[2];
    p_reg->PM1 = p_entry->payload_compare.payload_compare_u32[3];
}

#endif

/*******************************************************************************************************************//**
 * Interrupt service routines
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Global Error ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common error function, and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_global_error_isr (void)
{
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;
    IRQn_Type                irq;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    /* Get IRQ and context */
        irq = R_FSP_CurrentIrqGet();

    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    canfd_extended_cfg_t * p_extend_cfg         = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend;
    uint8_t                global_error_channel = p_extend_cfg->p_global_cfg->channel_error;
    args.event = CAN_EVENT_ERR_GLOBAL;

    /* Read global error flags. */
    uint32_t cfdgerfl = p_canfd_reg->CFDGERFL;

    /* Global errors are in the top halfword of canfd_error_t; move and preserve ECC error flags. */
    args.error = (cfdgerfl & CANFD_PRV_ERROR_GL_FLAG_REG_MASK) << CANFD_PRV_ERROR_GLOBAL_ENUM_POS;

    /* Clear global error flags. */
    p_canfd_reg->CFDGERFL = 0;

    /* Dummy read to ensure that interrupt event DEF, CMPOF is cleared () */
    volatile uint32_t dummy = p_canfd_reg->CFDGERFL;
    FSP_PARAMETER_NOT_USED(dummy);

    if (args.error & CANFD_ERROR_GLOBAL_MESSAGE_LOST)
    {
        uint32_t            cfdfmsts = p_canfd_reg->CFDFMSTS;
        uint8_t             buffer;
        volatile uint32_t * p_sts;

        if ((cfdfmsts & R_CANFD0_CFDFMSTS_RFxMLT_Msk) != 0U)
        {
            buffer = (uint8_t) (SCH1R(cfdfmsts) - 1U);
            p_sts  = &p_canfd_reg->CFDRFSTS[buffer];
            *p_sts = (uint32_t) ~R_CANFD0_CFDRFSTS_RFMLT_Msk;
        }
        else
        {
            buffer =
                (uint8_t) (SCH1R((cfdfmsts & R_CANFD0_CFDFMSTS_CFkMLT_Msk) >> R_CANFD0_CFDFMSTS_CFkMLT_Pos) - 1U);
            p_sts  = &p_canfd_reg->CFDCFSTS[buffer];
            *p_sts = (uint32_t) ~R_CANFD0_CFDCFSTS_CFMLT_Msk;
        }

        args.buffer = (can_buffer_t) (buffer);

        /* Dummy read to ensure that interrupt event is cleared. */
        dummy = *p_sts;
        FSP_PARAMETER_NOT_USED(dummy);
    }

    if (args.error & CANFD_ERROR_GLOBAL_TXQ_MESSAGE_LOST)
    {
        r_canfd_handle_txqueue_error(p_ctrl, &args);
    }

    if (args.error & CANFD_ERROR_GLOBAL_TXQ_OVERWRITE)
    {
        r_canfd_handle_txqueue_error(p_ctrl, &args);
    }

    if (args.error & CANFD_ERROR_GLOBAL_MESSAGE_OVERWRITE)
    {
        /* Get lowest CFIFO with Message Overwrite condition and clear the flag */
        args.buffer = (can_buffer_t) (SCH1R(p_canfd_reg->CFDCFMOWSTS - 1));

        p_canfd_reg->CFDCFSTS[args.buffer] = ~R_CANFD0_CFDCFSTS_CFMOW_Msk;

        /* Dummy read to ensure that interrupt event is cleared. */
        dummy = p_canfd_reg->CFDCFSTS[args.buffer];
        FSP_PARAMETER_NOT_USED(dummy);
    }

    if (args.error & CANFD_ERROR_GLOBAL_TX_ENTRY_LOST)
    {
        for (uint8_t channel = 0; channel < CANFD_MAX_CHANNEL_PER_UNIT; channel++)
        {
            if (p_canfd_reg->CFDTHLSTS_b[channel].THLELT)
            {
                p_canfd_reg->CFDTHLSTS[channel] = ~R_CANFD0_CFDTHLSTS_THLELT_Msk;
                dummy = p_canfd_reg->CFDTHLSTS[channel];
                FSP_PARAMETER_NOT_USED(dummy);
            }
        }
    }

    /* Set channel and context based on selected global error handler channel. */
    args.channel   = global_error_channel;
    args.p_context = p_ctrl->p_context;

    /* Set remaining arguments and call callback */
    r_canfd_call_callback(p_ctrl, &args);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Channel Error ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common error function, and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_error_isr (void)
{
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;
    uint8_t   channel_index;
    IRQn_Type irq;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    /* Get IRQ and context */
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    irq = R_FSP_ORed_CurrentIrqGet();
#else
    irq = R_FSP_CurrentIrqGet();
#endif

    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    args.event = CAN_EVENT_ERR_CHANNEL;

    /* Read and clear SOCO, EOCO, TDVCF flags */
    uint32_t fdsts = (p_canfd_reg->CFDC2[channel_index].FDSTS >> R_CANFD0_CFDC2_FDSTS_TDCVF_Pos) & 0x07;
    p_canfd_reg->CFDC2[channel_index].FDSTS &= ~(0x07UL << R_CANFD0_CFDC2_FDSTS_TDCVF_Pos);

    /* Read and clear channel error flags. */

    uint32_t erfl = p_canfd_reg->CFDC[channel_index].ERFL & CANFD_PRV_ERROR_CH_FLAG_REG_MASK;
    p_canfd_reg->CFDC[channel_index].ERFL = 0;

    args.error = erfl | (fdsts << CANFD_PRV_ERROR_FD_ENUM_POS);

    /* Dummy read to ensure that interrupt event is cleared. */
    volatile uint32_t dummy = p_canfd_reg->CFDC[channel_index].ERFL;
    FSP_PARAMETER_NOT_USED(dummy);

    args.channel   = p_ctrl->p_cfg->channel;
    args.p_context = p_ctrl->p_context;
    args.buffer    = (can_buffer_t) 0U;

    /* Set remaining arguments and call callback */
    r_canfd_call_callback(p_ctrl, &args);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Receive ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common receive function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_rx_fifo_isr (void)
{
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /* Get lowest FIFO requesting interrupt */
    uint32_t fifo = SCH1R(p_canfd_reg->CFDRFISTS) - 1;

    /* Only perform ISR duties if a FIFO has requested it */
    if (fifo < CANFD_PRV_RX_FIFO_MAX)
    {
        /* Set static arguments */

        args.event  = CAN_EVENT_RX_COMPLETE;
        args.buffer = (can_buffer_t) (fifo + CAN_BUFFER_RX_FIFO_0);

        /* Get channel associated */
        args.channel = p_global_cfg->rx_fifo_cb_channel[fifo];

        /* Set the remaining callback arguments */
        args.p_context = gp_ctrl[args.channel]->p_context;
        r_canfd_call_callback(gp_ctrl[args.channel], &args);

        /* Clear RX FIFO Interrupt Flag */
        p_canfd_reg->CFDRFSTS[fifo] &= ~R_CANFD0_CFDRFSTS_RFIF_Msk;

        /* Dummy read to ensure that interrupt event is cleared. */
        volatile uint32_t dummy = p_canfd_reg->CFDRFSTS[fifo];
        FSP_PARAMETER_NOT_USED(dummy);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Common FIFO receive complete ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common receive function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_rx_common_fifo_isr (void)
{
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint8_t                  channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;
    can_cfg_t              * p_cfg;
    canfd_instance_ctrl_t  * p_ctrl;
    uint32_t                 fifo_num;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
#endif

    p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    can_callback_args_t args = {0};

    /* Get config struct */
    p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get lowest Common FIFO requesting interrupt in this channel*/
    fifo_num =
        SCH1R(p_canfd_reg->CFDCFRISTS >> (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)) - 1;

    /* Only perform ISR duties if a FIFO has requested it */
    if (fifo_num < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)
    {
        uint32_t cfifo_index = fifo_num + channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;

        /* Set static arguments */
        args.channel = p_ctrl->p_cfg->channel;
        args.buffer  = (can_buffer_t) ((fifo_num + CANFD_PRV_CFIFO_OFFSET) | CAN_BUFFER_TYPE_RX_FIFO);

        /* Handle CFIFO in receive mode */
        if (CANFD_CFIFO_MODE_RECEIVE == p_canfd_reg->CFDCFCC_b[cfifo_index].CFM)
        {
            args.event = CAN_EVENT_RX_COMPLETE;

            /* Set the remaining callback arguments */
            args.p_context = p_ctrl->p_context;
            r_canfd_call_callback(p_ctrl, &args);
        }
        else if (CANFD_CFIFO_MODE_GATEWAY == p_canfd_reg->CFDCFCC_b[cfifo_index].CFM)
        {
            if (1 == p_canfd_reg->CFDCFCC_b[cfifo_index].CFIM)
            {
                args.event = CAN_EVENT_RX_COMPLETE;
            }
            else
            {
                args.event = CAN_EVENT_FIFO_THRESHOLD_REACH;
            }

            args.p_context = p_ctrl->p_context;
            r_canfd_call_callback(p_ctrl, &args);
        }
        else
        {
            /*
             * Do nothing
             */
        }

        /* Clear CFIFO RX Interrupt Flag */
        p_canfd_reg->CFDCFSTS[cfifo_index] = ~R_CANFD0_CFDCFSTS_CFRXIF_Msk;

        /* Dummy read to ensure that interrupt event is cleared. */
        volatile uint32_t dummy = p_canfd_reg->CFDCFSTS[cfifo_index];
        FSP_PARAMETER_NOT_USED(dummy);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Transmit ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common transmit function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_tx_isr (void)
{
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint8_t                  channel_index;
    can_cfg_t              * p_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;
    canfd_instance_ctrl_t  * p_ctrl;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
#endif

    p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Set static arguments */
    can_callback_args_t args = {0};
    args.channel   = p_ctrl->p_cfg->channel;
    args.p_context = p_ctrl->p_context;

    /* Get config struct */
    p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Check the byte of CFDGTINTSTS0 that corresponds to the interrupting channel */
    volatile uint8_t * p_channel_int_status = (uint8_t *) &p_canfd_reg->CFDGTINTSTS32[0];
    uint8_t            channel_int_status   = p_channel_int_status[channel_index];

    while (channel_int_status)
    {
        /* Handle Transmit History Event */
        if ((channel_int_status & R_CANFD0_CFDGTINTSTS32_THIF0_Msk) ||
            (!p_canfd_reg->CFDTHLSTS_b[channel_index].THLEMP))
        {
            r_canfd_handle_txhistory(p_ctrl, &args);
        }
        /* Handle Transmit Message Buffer Tx complete Event */
        else if (channel_int_status & R_CANFD0_CFDGTINTSTS32_TSIF0_Msk)
        {
            r_canfd_handle_txmb(p_ctrl, &args, CAN_EVENT_TX_COMPLETE);
        }
        /* Handle Transmit Message Buffer Tx Abort Event */
        else if (channel_int_status & R_CANFD0_CFDGTINTSTS32_TAIF0_Msk)
        {
            r_canfd_handle_txmb(p_ctrl, &args, CAN_EVENT_TX_ABORTED);
        }
        /* Handle Transmit Queue Event */
        else if (channel_int_status & R_CANFD0_CFDGTINTSTS32_TQIF0_Msk)
        {
            r_canfd_handle_txqueue(p_ctrl, &args);
        }
        /* Handle Common Fifo Transmit Event */
        else if (channel_int_status & R_CANFD0_CFDGTINTSTS32_CFTIF0_Msk)
        {
            r_canfd_handle_txcfifo(p_ctrl, &args);
        }
        else
        {
            /*
             * Do nothing
             */
        }

        /* Check for more interrupts on this channel */
        channel_int_status = p_channel_int_status[channel_index];
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

#ifdef BSP_FEATURE_CANFD_FFI_MODE

/*******************************************************************************************************************//**
 * Transmit VM ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common transmit function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_tx_vm_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

 #if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
 #else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
 #endif

    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    canfd_extended_cfg_t  * p_extend;
    canfd_global_cfg_t    * p_global_cfg;

    uint8_t channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;

    /* Set static arguments */
    can_callback_args_t args = {0};
    args.channel   = p_ctrl->p_cfg->channel;
    args.p_context = p_ctrl->p_context;

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Check the byte of CFDGTINTSTS0 that corresponds to the interrupting channel */
    volatile uint8_t * p_channel_int_status = (uint8_t *) &p_canfd_reg->CFDVMISTS[channel_index];
    uint8_t            channel_int_status   = *p_channel_int_status;

    while (channel_int_status)
    {
        /* Handle Transmit History Event */
        if ((channel_int_status & R_CANFD0_CFDVMISTS_THLIF_Msk) ||
            (!p_canfd_reg->CFDTHLSTS_b[channel_index].THLEMP))
        {
            r_canfd_handle_txhistory(p_ctrl, &args);
        }
        else if (channel_int_status & R_CANFD0_CFDVMISTS_CFTXINT_Msk)
        {
            r_canfd_handle_txcfifo(p_ctrl, &args);
        }
        else if (channel_int_status & R_CANFD0_CFDVMISTS_TXQTXIF_Msk)
        {
            r_canfd_handle_txqueue(p_ctrl, &args);
        }
        else
        {
            /*
             * Do nothing
             */
        }

        /* Check for more interrupts on this channel */
        channel_int_status = p_channel_int_status[0];
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Common FIFO receive and RX FIFO virtual machine complete ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common receive function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_vm_rx_isr (void)
{
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    uint8_t                  channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

 #if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
 #else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
 #endif

    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get lowest FIFO requesting interrupt */
    uint32_t fifo = SCH1R(p_canfd_reg->CFDRFISTS) - 1;

    /* Only perform ISR duties if a FIFO has requested it */
    if (fifo < CANFD_PRV_RX_FIFO_MAX)
    {
        /* Set static arguments */

        args.event  = CAN_EVENT_RX_COMPLETE;
        args.buffer = (can_buffer_t) (fifo + CAN_BUFFER_RX_FIFO_0);

        /* Get channel associated with the AFL entry */
        args.channel = p_global_cfg->rx_fifo_cb_channel[fifo];

        /* Set the remaining callback arguments */
        args.p_context = gp_ctrl[args.channel]->p_context;
        r_canfd_call_callback(gp_ctrl[args.channel], &args);

        /* Clear RX FIFO Interrupt Flag */
        p_canfd_reg->CFDRFSTS[fifo] &= ~R_CANFD0_CFDRFSTS_RFIF_Msk;

        /* Dummy read to ensure that interrupt event is cleared. */
        volatile uint32_t dummy = p_canfd_reg->CFDRFSTS[fifo];
        FSP_PARAMETER_NOT_USED(dummy);
    }

    /* Get lowest Common FIFO requesting interrupt in this channel*/
    uint32_t fifo_num =
        SCH1R(p_canfd_reg->CFDCFRISTS >> (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)) - 1;

    /* Only perform ISR duties if a CFIFO has requested it */
    if (fifo_num < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)
    {
        uint32_t cfifo_index = fifo_num + channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;

        /* Set static arguments */
        args.channel = p_ctrl->p_cfg->channel;
        args.buffer  = (can_buffer_t) ((fifo_num + CANFD_PRV_CFIFO_OFFSET) | CAN_BUFFER_TYPE_RX_FIFO);

        /* Handle CFIFO in receive mode */
        if (CANFD_CFIFO_MODE_RECEIVE == p_canfd_reg->CFDCFCC_b[cfifo_index].CFM)
        {
            args.event = CAN_EVENT_RX_COMPLETE;

            /* Set the remaining callback arguments */
            args.p_context = p_ctrl->p_context;
            r_canfd_call_callback(p_ctrl, &args);
        }
        else if (CANFD_CFIFO_MODE_GATEWAY == p_canfd_reg->CFDCFCC_b[cfifo_index].CFM)
        {
            if (1 == p_canfd_reg->CFDCFCC_b[cfifo_index].CFIM)
            {
                args.event = CAN_EVENT_RX_COMPLETE;
            }
            else
            {
                args.event = CAN_EVENT_FIFO_THRESHOLD_REACH;
            }

            args.p_context = p_ctrl->p_context;
            r_canfd_call_callback(p_ctrl, &args);
        }
        else
        {
            /*
             * Do nothing
             */
        }

        /* Clear CFIFO RX Interrupt Flag */
        p_canfd_reg->CFDCFSTS[cfifo_index] = ~R_CANFD0_CFDCFSTS_CFRXIF_Msk;

        /* Dummy read to ensure that interrupt event is cleared. */
        volatile uint32_t dummy = p_canfd_reg->CFDCFSTS[cfifo_index];
        FSP_PARAMETER_NOT_USED(dummy);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * Virtual Machine Error ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common error function, and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_vm_err_isr (void)
{
    canfd_extended_cfg_t   * p_extend;
    canfd_global_cfg_t     * p_global_cfg;
    volatile R_CANFD0_Type * p_canfd_reg;
    uint8_t           channel_index;
    IRQn_Type         irq;
    volatile uint32_t dummy;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    /* Get IRQ and context */
 #if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    irq = R_FSP_ORed_CurrentIrqGet();
 #else
    irq = R_FSP_CurrentIrqGet();
 #endif

    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    args.event = CAN_EVENT_ERR_CHANNEL;

    /* Read virtual machine interrupt status. */
    uint32_t cfdvmists = p_canfd_reg->CFDVMISTS[channel_index];

    /* RXFIFO Message Lost*/
    if (cfdvmists & R_CANFD0_CFDVMISTS_RFMLT_Msk)
    {
        /* Get lowest RX FIFO with Message Lost condition and clear the flag */
        args.buffer = (can_buffer_t) (SCH1R(p_canfd_reg->CFDFMSTS) - 1);

        p_canfd_reg->CFDRFSTS[args.buffer] = ~R_CANFD0_CFDRFSTS_RFMLT_Msk;

        /* Dummy read to ensure that interrupt event is cleared. */
        dummy = p_canfd_reg->CFDRFSTS[args.buffer];
        FSP_PARAMETER_NOT_USED(dummy);

        args.error |= CANFD_ERROR_CHANNEL_VM_FIFO_MESSAGE_LOST;
    }
    else
    {
        /* Common FIFO Message Lost*/
        if (cfdvmists & R_CANFD0_CFDVMISTS_CFMLT_Msk)
        {
            /* Get lowest CFIFO with Message Lost condition and clear the flag */
            args.buffer =
                (can_buffer_t) (SCH1R((p_canfd_reg->CFDFMSTS & R_CANFD0_CFDFMSTS_CFkMLT_Msk) >>
                                      R_CANFD0_CFDFMSTS_CFkMLT_Pos) - 1);

            p_canfd_reg->CFDCFSTS[args.buffer] = ~R_CANFD0_CFDCFSTS_CFMLT_Msk;

            /* Dummy read to ensure that interrupt event is cleared. */
            dummy = p_canfd_reg->CFDCFSTS[args.buffer];
            FSP_PARAMETER_NOT_USED(dummy);

            args.error |= CANFD_ERROR_CHANNEL_VM_TXQ_CFIFO_MESSAGE_LOST;
        }

        /* Common FIFO Message Overwrite */
        if (cfdvmists & R_CANFD0_CFDVMISTS_CFMOW_Msk)
        {
            /* Get lowest CFIFO with Message Overwrite condition and clear the flag */
            args.buffer =
                (can_buffer_t) (SCH1R(p_canfd_reg->CFDCFMOWSTS - 1));

            p_canfd_reg->CFDCFSTS[args.buffer] = ~R_CANFD0_CFDCFSTS_CFMOW_Msk;

            /* Dummy read to ensure that interrupt event is cleared. */
            dummy = p_canfd_reg->CFDCFSTS[args.buffer];
            FSP_PARAMETER_NOT_USED(dummy);

            args.error |= CANFD_ERROR_CHANNEL_VM_CFIFO_MESSAGE_OVERWRITE;
        }

        /* Transmit Queue Message Lost */
        if (cfdvmists & R_CANFD0_CFDVMISTS_TXQMLT_Msk)
        {
            r_canfd_handle_txqueue_ffi_error(p_ctrl, &args);
        }

        /* Transmit Queue Message Overwrite */
        if (cfdvmists & R_CANFD0_CFDVMISTS_TXQMOW_Msk)
        {
            r_canfd_handle_txqueue_ffi_error(p_ctrl, &args);
        }

        /* Transmit Histroy List Entry Lost*/
        if (cfdvmists & R_CANFD0_CFDVMISTS_THLELT_Msk)
        {
            p_canfd_reg->CFDTHLSTS[channel_index] = ~R_CANFD0_CFDTHLSTS_THLELT_Msk;
            dummy = p_canfd_reg->CFDTHLSTS[channel_index];
            FSP_PARAMETER_NOT_USED(dummy);

            args.error |= (uint32_t) CANFD_ERROR_CHANNEL_VM_TX_ENTRY_LOST;
        }
    }

    /* Set channel and context based on selected global error handler channel. */
    args.channel   = p_cfg->channel;
    args.p_context = p_ctrl->p_context;

    FSP_PARAMETER_NOT_USED(dummy);

    /* Set remaining arguments and call callback */
    r_canfd_call_callback(p_ctrl, &args);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

#endif
