/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_canfd.h"                   /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define CANFD_OPEN                       (0x52434644U) // "RCFD" in ASCII

#define CANFD_BAUD_RATE_PRESCALER_MIN    (1U)          /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_BAUD_RATE_PRESCALER_MAX    (1024U)       /* PRQA S 1534 # Advisory R-2.5 */

#define CANFD_PRV_CTR_MODE_MASK          (R_CANFD0_CFDGCTR_GSLPR_Msk | R_CANFD0_CFDGCTR_GMDC_Msk)
#define CANFD_PRV_CTR_RESET_BIT          (1U)
#define CANFD_PRV_OPERATION_MODE_MASK    (0x0FU)

#define CANFD_PRV_RX_FIFO_MAX            (8U)
#define CANFD_PRV_RXMB_CHANNEL_MAX       (16U) /* PRQA S 1534 # Advisory R-2.5 */

#define CANFD_PRV_RXMB_MAX_U0            (BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT0 * CANFD_PRV_RXMB_CHANNEL_MAX)

#if (BSP_FEATURE_CANFD_NUM_UNIT == 3)
 #define CANFD_PRV_RXMB_MAX_U1           (BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT1 * CANFD_PRV_RXMB_CHANNEL_MAX)
 #define CANFD_PRV_RXMB_MAX_U2           (BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT2 * CANFD_PRV_RXMB_CHANNEL_MAX)
 #define CANFD_PRV_RXMB_MAX(x)          (((x) == 0U) ? CANFD_PRV_RXMB_MAX_U0 : \
                                         (((x) == 1U) ? CANFD_PRV_RXMB_MAX_U1 : CANFD_PRV_RXMB_MAX_U2))
#elif (BSP_FEATURE_CANFD_NUM_UNIT == 2)
 #define CANFD_PRV_RXMB_MAX_U1           (BSP_FEATURE_CANFD_NUM_CHANNELS_UNIT1 * CANFD_PRV_RXMB_CHANNEL_MAX)
 #define CANFD_PRV_RXMB_MAX(x)          (((x) == 0U) ? CANFD_PRV_RXMB_MAX_U0 : CANFD_PRV_RXMB_MAX_U1)
#else
 #define CANFD_PRV_RXMB_MAX(x)          (CANFD_PRV_RXMB_MAX_U0)              /* PRQA S 1534 , 3472 # Advisory R-2.5 , Advisory D-4.9 */
#endif

/* Number of CFDRMND registers holding the RXMB new data flags of one unit (32 flags per register, rounded up) */
#define CANFD_PRV_RXMB_ND_REG_NUM(x)    ((CANFD_PRV_RXMB_MAX(x) + 31U) >> 5) /* PRQA S 1534 # Advisory R-2.5 */

#define CANFD_PRV_TXMB_CHANNEL_OFFSET    (64U)

/* Do not update the buffer while a transmission is ongoing or a request is pending */
#define CANFD_PRV_TXMB_BUSY_MASK         (R_CANFD0_CFDTMSTS_TMTSTS_Msk | R_CANFD0_CFDTMSTS_TMTRM_Msk | \
                                          R_CANFD0_CFDTMSTS_TMTARM_Msk)

/* Bit TMTRF only cleared by the transmit interrupt, so it indicates a busy buffer for buffers enabled in interrupt only */
#define CANFD_PRV_TXMB_RESULT_MASK       (R_CANFD0_CFDTMSTS_TMTRF_Msk)

/* Number of TX Message Buffer interrupt enable bits per CFDTMIEC register */
#define CANFD_PRV_TXMB_IEC_BITS          (32U)

/* CFDTMIEC register index and single bit mask of a TX Message Buffer */
#define CANFD_PRV_TXMB_IEC_INDEX(msgnum)    ((uint32_t) (msgnum) >> 5U)                                               /* PRQA S 3472 # Advisory D-4.9 */
#define CANFD_PRV_TXMB_IEC_BIT(msgnum)      ((uint32_t) 1U << ((uint32_t) (msgnum) & (CANFD_PRV_TXMB_IEC_BITS - 1U))) /* PRQA S 3472 # Advisory D-4.9 */

#define CANFD_PRV_TXMB_OFFSET_IN_CHANNEL    (32U)
#define CANFD_PRV_STANDARD_ID_MAX           (0x7FFU)                                                                  /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_PRV_CFDFESTS_RFXEMP_MSK       (0xFFU)
#define CANFD_PRV_CFDTM_CH_LENGTH           (2U)

#define CANFD_PRV_TX_QUEUE_WINDOW           (31U)                                                                     /* PRQA S 1534 # Advisory R-2.5 */

#define CANFD_PRV_OFFSET_TXQ_ML             (4U)                                                                      /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_PRV_TXQ_MSK                   (0xfU)
#define CANFD_PRV_BUFFER_NUM_MASK           (0x00ffU)
#define CANFD_PRV_BUFFER_TYPE_MASK          (0xff00U)

#define CANFD_PRV_UPDATE_POINTER_CMD        (0xffU)

#define CANFD_PRV_ERROR_CH_FLAG_REG_MASK    (0xffU)
#define CANFD_PRV_ERROR_GL_FLAG_REG_MASK    (0xffU)
#define CANFD_PRV_VM_ERR_REG_MASK           (0xff0000U) /* PRQA S 1534 # Advisory R-2.5 */

#define CANFD_PRV_ERROR_FD_ENUM_POS         (16U)
#define CANFD_PRV_ERROR_GLOBAL_ENUM_POS     (20U)

#define CANFD_PRV_CFIFO_OFFSET              (BSP_FEATURE_CANFD_NUM_RXFIFO)
#define CANFD_PRV_CAN_HUB_OFFSET            (16U) /* PRQA S 1534 # Advisory R-2.5 */
#if defined(BSP_FEATURE_CANFD_TXMB_MAX_NUM) && (BSP_FEATURE_CANFD_TXMB_MAX_NUM == CANFD_PRV_TXMB_CHANNEL_OFFSET)
 #define CANFD_PRV_TXMB_LOW_BUFFER_NUM      (32U)
#else
 #define CANFD_PRV_TXMB_LOW_BUFFER_NUM      (16U)
#endif
#define CANFD_PRV_TXMB_HIGH_BUFFER_NUM      (48U)                                                      /* PRQA S 1534 # Advisory R-2.5 */

/* Valid txmb is 0 - (LOW_BUFFER_NUM - 1) and 32 - (HIGH_BUFFER_NUM - 1) */
#define CANFD_PRV_VALID_TXMB_RANGE(txmb)    (BSP_FEATURE_CANFD_TXMB_SUPPORTED_SLOT & (1ULL << (txmb))) /* PRQA S 1534 , 3472 # Advisory R-2.5 , Advisory D-4.9 */

#define CANFD_PRV_TXQ_INDEX_0               (0U)
#define CANFD_PRV_TXQ_INDEX_1               (1U)
#define CANFD_PRV_TXQ_INDEX_2               (2U)
#define CANFD_PRV_TXQ_INDEX_3               (3U)
#define CANFD_PRV_TXQ_INDEX_4               (4U)
#define CANFD_PRV_TXQ_INDEX_5               (5U)

#define CANFD_PRV_TXQ_CH_OFFSET             (4U)
#define CANFD_FLX_BUFF_CH_OFFSET            (8U)

#define CANFD_PRV_TXMB_OUT_RANGE            (0xFFFFFFFFU)

/* Get amount of lend buffer of the channel */
#define CANFD_PRV_LEND_BUFF_GET(reg, channel)    (((uint32_t) ((uint8_t *) &((reg)->CFDGFTBAC))[((channel)) / 2U]) * 4U) /* PRQA S 3472 # Advisory D-4.9 */

/* Fill n MSB bit 1 */
#define CANFD_PRV_FILL_1(n)                      (((uint32_t) 1U << (n)) - 1U)                                           /* PRQA S 3472 # Advisory D-4.9 */

#define CANFD_PRV_TX_QUEUE_2_WINDOW       (32U)                                                                          /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_PRV_TX_QUEUE_3_WINDOW       (63U)                                                                          /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_THIRTY_TWO                  (32U)
#define CANFD_SIXTY_FOUR                  (64U)
#define CANFD_MAX_UINT_7                  (0x7FU)

#define CANFD_PRV_SEC_TO_NS               (1000000000U)

/* Macro for value of CFDCnFDCTR.KEY */
#define CANFD_PRV_PNMDC_KEY               0xC4000000UL    /* PRQA S 1534 # Advisory R-2.5 */

/* CFDGFFIMC KEY */
#define CANFD_PRV_CFDGFFIMC_KEY           (0xC4UL << 8UL) /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_PRV_CFDGFFIMC_FFIEN         (0x01U)         /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_PRV_CFDGFFIMC_FFIEN_OFF     (0x00U)         /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_PRV_CHANNELS_OFFSET         (8U)

/* LOCKK KEY */
#define CANFD_GLOCKK_UNLOCK_KEY1          0x00007575U
#define CANFD_GLOCKK_UNLOCK_KEY2          0x00008A8AU

#define CANFD_RAM_TEST_SET_DATA           0xFFFFFFFFU
#define CANFD_RAM_TEST_CLEAR_DATA         0x00000000U

#define CANFD_RAM_PAGE_SIZE_ACCESS_REG    (64U)
#define CANFD_RAM_TEST_DATA               (0xAAAAAAAAU)

#define CANFD_RAM_TEST_CELL_DIVIDER       (0x05U)
#define CANFD_RAM_NUM_BYTES_PER_PAGE      (256U)
#define CANFD_ONE                         (1U)
#define CANFD_PRV_DATA_LENGTH_CODE_0      (0U)                                                                      /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_PRV_DATA_LENGTH_CODE_8      (8U)
#define CANFD_PRV_DATA_LENGTH_CODE_9      (9U)
#define CANFD_PRV_DATA_LENGTH_CODE_15     (15U)
#define CANFD_MAX_TRANSFER_CONTEXT        (4U * BSP_FEATURE_CANFD_NUM_CHANNELS) + (8U * BSP_FEATURE_CANFD_NUM_UNIT) /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_SET_INDEX_CONTEXT(channel, loop)                                                                      /* PRQA S 1534, 3472 # Advisory R-2.5 , Advisory D-4.9 */ \
    (((loop) + BSP_FEATURE_CANFD_NUM_RXFIFO + (4U * (channel))))

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE
typedef struct e_canfd_transfer_context     /* PRQA S 1536, 3213 # Advisory R-2.4 , Advisory R-2.4 */
{
    canfd_instance_ctrl_t * p_ctrl;         ///< CANFD instance control block.
    canfd_transfer_cfg_t  * p_transfer_cfg; ///< Transfer Configuration.
} canfd_transfer_context_t;
#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* LUT to convert DLC values to payload size in bytes */
static const uint8_t dlc_to_bytes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

#if CANFD_CFG_PARAM_CHECKING_ENABLE

/* LUT to determine the hierarchy of can_operation_mode_t modes. */
static const uint8_t g_mode_order[] = {0, 2, 1, 0, 0, 3};                        /* PRQA S 3218 # Advisory R-8.9 */
#endif

/* LUT to convert Transmit Queue into TXMB access windows */
static const uint8_t g_queue_window[] = {0, 31, 32, 63};                         /* PRQA S 3218 # Advisory R-8.9 */

/* Channel control struct array */
static canfd_instance_ctrl_t * gp_ctrl[BSP_FEATURE_CANFD_NUM_CHANNELS] = {NULL}; /* PRQA S 0686 # DP_MSR_R-9.3_UC1 */

#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE
static canfd_transfer_context_t canfd_transfer_context[CANFD_MAX_TRANSFER_CONTEXT];
#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if CANFD_CFG_PARAM_CHECKING_ENABLE
static bool        r_canfd_bit_timing_nominal_parameter_check(can_bit_timing_cfg_t const * const p_bit_timing);
static bool        r_canfd_bit_timing_data_parameter_check(can_bit_timing_cfg_t const * const p_bit_timing);
static fsp_err_t   r_canfd_validate_tx_buffer(canfd_instance_ctrl_t const * p_ctrl, can_buffer_t buffer);
static fsp_err_t   r_canfd_validate_rx_buffer(const can_cfg_t * p_cfg, can_buffer_t buffer);
static inline bool r_canfd_cfifo_link_queue_check(const can_cfg_t * p_cfg);
static inline bool r_canfd_cfifo_same_link_check(canfd_cfifo_cfg_t cfifo_cfg_0, canfd_cfifo_cfg_t cfifo_cfg_1);

#endif

static fsp_err_t r_canfd_mb_read(uint32_t                 channel,
                                 can_buffer_type_t        buffer_type,
                                 uint32_t                 buffer_index,
                                 can_frame_t * const      p_frame,
                                 volatile R_CANFD0_Type * p_canfd_reg);
static fsp_err_t r_canfd_mb_write(uint32_t                     channel,
                                  can_buffer_type_t            buffer_type,
                                  uint32_t                     buffer_index,
                                  can_frame_t const * const    p_frame,
                                  canfd_extended_cfg_t const * p_extend,
                                  volatile R_CANFD0_Type     * p_canfd_reg);
static void    r_canfd_call_callback(canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args);
static void    r_canfd_mode_transition(canfd_instance_ctrl_t * p_ctrl, can_operation_mode_t operation_mode); /* PRQA S 1710 # Advisory D-4.5 */
static void    r_canfd_mode_ctr_set(volatile uint32_t * p_ctr_reg, can_operation_mode_t operation_mode);
static uint8_t r_canfd_bytes_to_dlc(uint16_t bytes);
static bool    r_canfd_is_all_channel_close(uint8_t unit);
static void    r_canfd_handle_txhistory(canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args);

static inline void r_canfd_thl_handle_txqueue(canfd_instance_ctrl_t const * const p_ctrl,
                                              can_callback_args_t * const         p_args,
                                              volatile R_CANFD0_Type            * p_canfd_reg,
                                              canfd_extended_cfg_t const        * p_extend,
                                              uint32_t                            buffer_num);
static inline void r_canfd_thl_handle_txfifo(canfd_instance_ctrl_t const * const p_ctrl,
                                             can_callback_args_t * const         p_args,
                                             volatile R_CANFD0_Type            * p_canfd_reg,
                                             uint32_t                            buffer_num);
static inline void r_canfd_thl_handle_txmb(canfd_instance_ctrl_t const * const p_ctrl,
                                           can_callback_args_t * const         p_args,
                                           volatile R_CANFD0_Type            * p_canfd_reg,
                                           uint32_t                            buffer_num);
static void r_canfd_handle_txmb(canfd_instance_ctrl_t const * p_ctrl,
                                can_callback_args_t         * p_args,
                                can_event_t                   event);

static void r_canfd_handle_txqueue(canfd_instance_ctrl_t const * p_ctrl,
                                   can_callback_args_t         * p_args,
                                   volatile R_CANFD0_Type      * p_canfd_reg);
static void r_canfd_handle_txfifo(canfd_instance_ctrl_t const * p_ctrl,
                                  canfd_extended_cfg_t const  * p_extend,
                                  can_callback_args_t         * p_args,
                                  volatile R_CANFD0_Type      * p_canfd_reg);
static void r_canfd_handle_txhistory(canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args);

#ifdef BSP_FEATURE_CANFD_FFI_MODE
static void r_canfd_handle_txqueue_ffi_error(canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args);
static void r_canfd_handle_ffi_txqueue_one_frame(canfd_instance_ctrl_t const * p_ctrl,
                                                 can_callback_args_t         * p_args,
                                                 volatile R_CANFD0_Type      * p_canfd_reg,
                                                 uint32_t                      txq_one_frame_flag);

#endif

static void r_canfd_handle_txqueue_error(canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args);

static void r_canfd_setting_filter_rule(canfd_instance_ctrl_t const * p_ctrl);
static void r_canfd_handle_txqueue_one_frame(canfd_instance_ctrl_t const * p_ctrl,
                                             can_callback_args_t         * p_args,
                                             volatile R_CANFD0_Type      * p_canfd_reg,
                                             uint32_t                      txq_one_frame_flag);

#ifdef BSP_FEATURE_CANFD_PN_MODE
static void r_canfd_copy_pnf_rule(volatile R_CANFD0_CFDGPFL_Type * p_reg, const canfd_pnf_entry_t * p_entry);

#endif
static void      r_canfd_set_ram_data(canfd_global_cfg_t const * p_global_cfg, canfd_ramtest_data_t data_ram);
static fsp_err_t r_canfd_ram_check_algorithm(canfd_global_cfg_t const * p_global_cfg);
static fsp_err_t r_canfd_ram_walkpath_algorithm(canfd_global_cfg_t const * p_global_cfg,
                                                canfd_ramtest_walk_type_t  walk_type);
static inline uint32_t r_canfd_caculate_no_channel(uint8_t unit);
static uint32_t        r_canfd_ram_caculate_max_page(uint8_t unit, canfd_ramtest_field_t ram_field);
static uint32_t        r_canfd_ram_caculate_limit_byte(canfd_global_cfg_t const * p_global_cfg);
static uint32_t        r_canfd_ram_caculate_max_page(uint8_t unit, canfd_ramtest_field_t ram_field);

#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE
static inline bool      r_canfd_data_transfer_empty(canfd_transfer_cfg_t const * p_transfer_cfg);
static inline bool      r_canfd_data_transfer_full(canfd_transfer_cfg_t const * p_transfer_cfg);
static inline bool      r_canfd_data_transfer_last_entry(canfd_transfer_cfg_t const * p_transfer_cfg);
static void             r_canfd_data_transfer_reset(canfd_transfer_cfg_t * p_transfer_cfg);
static inline fsp_err_t r_canfd_transfer_rx_fifo_cfg(volatile R_CANFD0_Type * p_canfd_reg,
                                                     canfd_instance_ctrl_t  * p_ctrl,
                                                     canfd_transfer_cfg_t   * p_transfer_cfg,
                                                     uint8_t                  index,
                                                     canfd_rx_fifo_cfg_t      rx_fifo_config);
static inline fsp_err_t r_canfd_transfer_channel_cfg(volatile R_CANFD0_Type * p_canfd_reg,
                                                     canfd_instance_ctrl_t  * p_ctrl,
                                                     canfd_transfer_cfg_t   * p_transfer_cfg,
                                                     canfd_tx_queue_cfg_t   * p_tx_queue_cfg,
                                                     uint8_t                  index,
                                                     canfd_cfifo_cfg_t        cfifo_config);
static fsp_err_t r_canfd_transfer_cfg(canfd_instance_ctrl_t * p_ctrl,
                                      canfd_extended_cfg_t  * p_extend,
                                      canfd_global_cfg_t    * p_global_cfg);
static fsp_err_t r_canfd_transfer_rx_fifo_close(canfd_transfer_cfg_t const * p_transfer_cfg);
static fsp_err_t r_canfd_transfer_channel_close(canfd_transfer_cfg_t  ** pp_transfer_cfg,
                                                volatile R_CANFD0_Type * p_canfd_reg,
                                                uint8_t                  channel_index);
static inline fsp_err_t r_canfd_fifo_transfer_read(can_frame_t * const p_frame, canfd_transfer_cfg_t * p_transfer_cfg);
static void             r_canfd_mb_read_transfer(can_frame_t * const p_frame, canfd_rx_fifo_data_t const * p_data);
static fsp_err_t        r_canfd_transfer_write(uint32_t                     channel,
                                               can_buffer_type_t            buffer_type,
                                               uint32_t                     buffer_index,
                                               can_frame_t * const          p_frame,
                                               canfd_extended_cfg_t const * p_extend,
                                               canfd_transfer_cfg_t       * p_transfer_cfg);
static inline void r_canfd_transfer_rx_callback(canfd_transfer_context_t const * p_context,
                                                can_callback_args_t            * p_args,
                                                uint32_t                         threshold,
                                                uint32_t                         int_mode,
                                                canfd_rx_fifo_data_t           * p_dest);
static void canfd_rx_cfifo_transfer_callback(transfer_callback_args_t * p_args);
static void canfd_rx_fifo_transfer_callback(transfer_callback_args_t * p_args);
static void canfd_tx_transfer_callback(transfer_callback_args_t * p_args);

#endif

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

BSP_INTERRUPT_ATTRIBUTE void canfd_global_error_isr(void);   /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1 , DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_error_isr(void);  /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1 , DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void canfd_rx_fifo_isr(void);        /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1 , DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_tx_isr(void);     /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1 , DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void canfd_rx_common_fifo_isr(void); /* PRQA S 3449, 3451 # DP_MSR_R-8.5_UC1 , DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_tx_vm_isr(void);  /* PRQA S 3449, 3451, 1753 # DP_MSR_R-8.5_UC1 , DP_MSR_R-8.5_UC1 , DP_MSR_R-8.6_UC1 */
BSP_INTERRUPT_ATTRIBUTE void canfd_vm_rx_isr(void);          /* PRQA S 3449, 3451, 1753 # DP_MSR_R-8.5_UC1 , DP_MSR_R-8.5_UC1 , DP_MSR_R-8.6_UC1 */
BSP_INTERRUPT_ATTRIBUTE void canfd_vm_err_isr(void);         /* PRQA S 3449, 3451, 1753 # DP_MSR_R-8.5_UC1 , DP_MSR_R-8.5_UC1 , DP_MSR_R-8.6_UC1 */

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* CAN function pointers   */
const can_api_t g_can_on_canfd =              /* PRQA S 1502 # Advisory R-2.8 */
{
    .open           = R_CANFD_Open,           /* PRQA S 3635 # Advisory R-17.12 */
    .close          = R_CANFD_Close,          /* PRQA S 3635 # Advisory R-17.12 */
    .write          = R_CANFD_Write,          /* PRQA S 3635 # Advisory R-17.12 */
    .read           = R_CANFD_Read,           /* PRQA S 3635 # Advisory R-17.12 */
    .modeTransition = R_CANFD_ModeTransition, /* PRQA S 3635 # Advisory R-17.12 */
    .infoGet        = R_CANFD_InfoGet,        /* PRQA S 3635 # Advisory R-17.12 */
    .callbackSet    = R_CANFD_CallbackSet,    /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup CANFD
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

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
fsp_err_t R_CANFD_Open (can_ctrl_t * const p_api_ctrl, can_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    canfd_instance_ctrl_t  * p_ctrl;
    canfd_extended_cfg_t   * p_extend;                                                /* PRQA S 3678 # Advisory R-8.13 */
    canfd_global_cfg_t     * p_global_cfg;                                            /* PRQA S 3678 # Advisory R-8.13 */
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_cfg);                           /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_cfg->p_extend);                 /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_cfg->p_callback);               /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_cfg->p_bit_timing);             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get control block */
    p_ctrl   = (canfd_instance_ctrl_t *) p_api_ctrl;     /* PRQA S 0316 # Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    FSP_ASSERT(NULL != p_extend->p_global_cfg);          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    p_global_cfg = p_extend->p_global_cfg;

    /*Take index for channel when device have more than one unit*/
    p_ctrl->channel_index = (uint8_t) (p_cfg->channel - (CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit));
    channel_index         = p_ctrl->channel_index;

    /* Check that the module is not open, the channel is present and that it is not in use */
    FSP_ERROR_RETURN(CANFD_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);                                         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(p_cfg->channel < BSP_FEATURE_CANFD_NUM_CHANNELS, FSP_ERR_IP_CHANNEL_NOT_PRESENT);          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(NULL == gp_ctrl[p_cfg->channel], FSP_ERR_IN_USE);                                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check that mandatory interrupts are enabled */
    FSP_ERROR_RETURN((int32_t) p_global_cfg->rx_fifo_irq >= 0, FSP_ERR_CAN_INIT_FAILED);                        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN((int32_t) p_global_cfg->err_glb_irq >= 0, FSP_ERR_CAN_INIT_FAILED);                        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check nominal bit timing parameters for correctness */
    FSP_ERROR_RETURN(r_canfd_bit_timing_nominal_parameter_check(p_cfg->p_bit_timing), FSP_ERR_CAN_INIT_FAILED); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check that bit timing for FD bitrate switching is present and correct */
    can_bit_timing_cfg_t const * p_data_timing = p_extend->p_data_timing;
    FSP_ASSERT(NULL != p_data_timing);                                                                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(r_canfd_bit_timing_data_parameter_check(p_data_timing), FSP_ERR_CAN_INIT_FAILED);          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    can_bit_timing_cfg_t const * p_bit_timing = p_cfg->p_bit_timing;

    /* Check that data rate > nominal rate */
    uint32_t data_rate_clocks = p_data_timing->baud_rate_prescaler *
                                (p_data_timing->time_segment_1 + p_data_timing->time_segment_2 + 1U);
    uint32_t nominal_rate_clocks = p_bit_timing->baud_rate_prescaler *
                                   (p_bit_timing->time_segment_1 + p_bit_timing->time_segment_2 + 1U);
    FSP_ERROR_RETURN(data_rate_clocks <= nominal_rate_clocks, FSP_ERR_CAN_INIT_FAILED); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Set txmb valid range */
    uint8_t lend_txmb = (uint8_t) ((uint32_t) p_global_cfg->flex_tx_buffer[channel_index / 2U] * 4U);
    if (0U == lend_txmb)
    {
        /* Flexible buffer assignment is disabled on this channel */
        p_ctrl->txmb_range_min    = 0U;
        p_ctrl->txmb_range_max    = CANFD_PRV_TXMB_CHANNEL_OFFSET;
        p_ctrl->txqueue_range_min = 0U;
        p_ctrl->txqueue_range_max = CANFD_PRV_TXQ_CH_OFFSET;
    }
    else if (0U == (p_cfg->channel % 2U))
    {
        /* Even channel lend buffer from the consecutive odd channel */
        p_ctrl->txmb_range_min = 0U;
        p_ctrl->txmb_range_max = (uint8_t) (CANFD_PRV_TXMB_CHANNEL_OFFSET + lend_txmb);

        /* If even channel lend 16 buffer, It will also lend 2 transmit queue.
         * Else It only lend 1 transmit queue */
        p_ctrl->txqueue_range_min = 0U;
        p_ctrl->txqueue_range_max = (uint8_t) ((CANFD_PRV_TXMB_LOW_BUFFER_NUM == lend_txmb) ?
                                               (CANFD_PRV_TXQ_CH_OFFSET + 2U) : (CANFD_PRV_TXQ_CH_OFFSET + 1U));
    }
    else
    {
        /* Odd channel txmb is borrowed */
        p_ctrl->txmb_range_min = lend_txmb;
        p_ctrl->txmb_range_max = CANFD_PRV_TXMB_CHANNEL_OFFSET;

        /* If even channel lend 16 buffer, It will also lend 2 transmit queue.
         * Else It only lend 1 transmit queue */
        p_ctrl->txqueue_range_min = (uint8_t) ((CANFD_PRV_TXMB_LOW_BUFFER_NUM == lend_txmb) ? 2U : 1U);
        p_ctrl->txqueue_range_max = CANFD_PRV_TXQ_CH_OFFSET;
    }

    /* Configure Transmit Queue */
    for (uint8_t tx_queue_index = 0; tx_queue_index < CANFD_TX_QUEUE_NUM; tx_queue_index++)
    {
        if (0U != p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable)
        {
            /* Check Transmit Queue is handled by this channel or not */
            FSP_ERROR_RETURN((tx_queue_index >= p_ctrl->txqueue_range_min) &&                     /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                             (tx_queue_index < p_ctrl->txqueue_range_max),
                             FSP_ERR_CAN_INIT_FAILED);                                            /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            /* Check Transmit Queue depth > 2 when enable transmit queue */
            FSP_ERROR_RETURN((p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.depth >= 2U), /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                             FSP_ERR_CAN_INIT_FAILED);                                            /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        }
    }

    /* Check transmit buffers configuration is valid */
    FSP_ERROR_RETURN(r_canfd_cfifo_link_queue_check(p_cfg), FSP_ERR_CAN_INIT_FAILED);                        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    uint32_t cfifo_start_index = channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL;
    FSP_ERROR_RETURN(r_canfd_cfifo_same_link_check(p_global_cfg->common_fifo_config[cfifo_start_index + 0U], /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                                                   p_global_cfg->common_fifo_config[cfifo_start_index + 1U]),
                     FSP_ERR_CAN_INIT_FAILED);                                                               /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(r_canfd_cfifo_same_link_check(p_global_cfg->common_fifo_config[cfifo_start_index + 0U], /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                                                   p_global_cfg->common_fifo_config[cfifo_start_index + 2U]),
                     FSP_ERR_CAN_INIT_FAILED);                                                               /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(r_canfd_cfifo_same_link_check(p_global_cfg->common_fifo_config[cfifo_start_index + 1U], /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                                                   p_global_cfg->common_fifo_config[cfifo_start_index + 2U]),
                     FSP_ERR_CAN_INIT_FAILED);                                                               /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Maximum 4 flex mode windown per 8 channels */
    uint8_t max_channel_flex_tx =
        (uint8_t) ((BSP_FEATURE_CANFD_NUM_CHANNELS > 8U) ? 8U : BSP_FEATURE_CANFD_NUM_CHANNELS);             /* PRQA S 3493 , 3494 # DP_MSR_R-14.3_UC1 */

    for (uint32_t flex_tx_assign = 0U; flex_tx_assign < ((uint32_t) max_channel_flex_tx / 2U); flex_tx_assign++)
    {
        /* Check Flexible transmission buffer assignment configured and Flexible CAN mode configured isn't used simultaneously.*/
 #ifdef BSP_FEATURE_CANFD_CAN_HUB_MODE
        if (p_global_cfg->can_hub_mode & (3U << (flex_tx_assign * 2U + CANFD_PRV_CAN_HUB_OFFSET)))
        {
            FSP_ERROR_RETURN(CANFD_FLEX_BUFFER_LEND_0 == p_global_cfg->flex_tx_buffer[flex_tx_assign],
                             FSP_ERR_CAN_INIT_FAILED); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        }

 #else
        if (0U != (p_global_cfg->flexible_mode & ((uint32_t) 1U << flex_tx_assign)))
        {
            FSP_ERROR_RETURN(CANFD_FLEX_BUFFER_LEND_0 == p_global_cfg->flex_tx_buffer[flex_tx_assign],
                             FSP_ERR_CAN_INIT_FAILED); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        }
 #endif
    }

    /* Check Common FIFOs config */
#else
    p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;           /* PRQA S 0316 # Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /*Take index for channel when device have more than one unit*/
    p_ctrl->channel_index = (uint8_t) (p_cfg->channel - (CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit));
    channel_index         = p_ctrl->channel_index;

    /* Set txmb and tx queue valid range */
    uint8_t lend_txmb = (uint8_t) ((uint32_t) p_global_cfg->flex_tx_buffer[channel_index / 2U] * 4U);
    if (0U == lend_txmb)
    {
        /* Flexible buffer assignment is disabled on this channel */
        p_ctrl->txmb_range_min    = 0U;
        p_ctrl->txmb_range_max    = CANFD_PRV_TXMB_CHANNEL_OFFSET;
        p_ctrl->txqueue_range_min = 0U;
        p_ctrl->txqueue_range_max = CANFD_PRV_TXQ_CH_OFFSET;
    }
    else if (0U == (p_cfg->channel % 2U))
    {
        /* Even channel lend buffer from the consecutive odd channel */
        p_ctrl->txmb_range_min = 0U;
        p_ctrl->txmb_range_max = (uint8_t) (CANFD_PRV_TXMB_CHANNEL_OFFSET + lend_txmb);

        /* If even channel lend 16 buffer, It will also lend 2 transmit queue.
         * Else It only lend 1 transmit queue */
        p_ctrl->txqueue_range_min = 0U;
        p_ctrl->txqueue_range_max = (uint8_t) ((CANFD_PRV_TXMB_LOW_BUFFER_NUM == lend_txmb) ?
                                               (CANFD_PRV_TXQ_CH_OFFSET + 2U) : (CANFD_PRV_TXQ_CH_OFFSET + 1U));
    }
    else
    {
        /* Odd channel txmb is borrowed */
        p_ctrl->txmb_range_min = lend_txmb;
        p_ctrl->txmb_range_max = CANFD_PRV_TXMB_CHANNEL_OFFSET;

        /* If even channel lend 16 buffer, It will also lend 2 transmit queue.
         * Else It only lend 1 transmit queue */
        p_ctrl->txqueue_range_min = (uint8_t) ((CANFD_PRV_TXMB_LOW_BUFFER_NUM == lend_txmb) ? 2U : 1U);
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
    if (0U != (p_canfd_reg->CFDGSTS & R_CANFD0_CFDGSTS_GRSTSTS_Msk))                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /* Wait for RAM initialization
         *(see RH850 microcontroller User's Manual section "Timing of Global Mode Change") */
        FSP_HARDWARE_REGISTER_WAIT((p_canfd_reg->CFDGSTS & R_CANFD0_CFDGSTS_GRAMINIT_Msk), 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Cancel Global Sleep and wait for transition to Global Reset */
        r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);

        /* Configure global TX priority, DLC check/replace functions, external/internal clock select and payload
         * overflow behavior */
        p_canfd_reg->CFDGCFG = p_global_cfg->global_config | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) p_global_cfg->timestamp_config.clock_div << R_CANFD0_CFDGCFG_TSP_Pos) |
                               ((uint32_t) p_global_cfg->timestamp_config.clock_source << R_CANFD0_CFDGCFG_TSSS_Pos);

        /* Set CAN FD Protocol Exception response (ISO exception state or send error frame)
         * and time stamp capture config */
        p_canfd_reg->CFDGFDCFG =       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                 ((uint32_t) p_global_cfg->timestamp_config.capture_cfg <<
                                  R_CANFD0_CFDGFDCFG_TSCCFG_Pos) |
                                 ((uint32_t) p_global_cfg->prt_exception << R_CANFD0_CFDGFDCFG_RPED_Pos);

        /* Configure AFL rule count for maximum 8 channels */
        p_canfd_reg->CFDGAFLCFG[0] = ((uint32_t) p_global_cfg->afl_rule_num[1] << R_CANFD0_CFDGAFLCFG_RNC1_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                     ((uint32_t) p_global_cfg->afl_rule_num[0] << R_CANFD0_CFDGAFLCFG_RNC0_Pos);
        p_canfd_reg->CFDGAFLCFG[1] = ((uint32_t) p_global_cfg->afl_rule_num[3] << R_CANFD0_CFDGAFLCFG_RNC1_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                     ((uint32_t) p_global_cfg->afl_rule_num[2] << R_CANFD0_CFDGAFLCFG_RNC0_Pos);
        p_canfd_reg->CFDGAFLCFG[2] = ((uint32_t) p_global_cfg->afl_rule_num[5] << R_CANFD0_CFDGAFLCFG_RNC1_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                     ((uint32_t) p_global_cfg->afl_rule_num[4] << R_CANFD0_CFDGAFLCFG_RNC0_Pos);
        p_canfd_reg->CFDGAFLCFG[3] = ((uint32_t) p_global_cfg->afl_rule_num[7] << R_CANFD0_CFDGAFLCFG_RNC1_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                     ((uint32_t) p_global_cfg->afl_rule_num[6] << R_CANFD0_CFDGAFLCFG_RNC0_Pos);
#ifdef BSP_FEATURE_CANFD_PN_MODE

        /* Configure PNF rule count for maximum 8 channels */
        p_canfd_reg->CFDGPFLCFG[0] = ((uint32_t) p_global_cfg->pnf_rule_num[1] << R_CANFD0_CFDGPFLCFG_RNC1_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                     ((uint32_t) p_global_cfg->pnf_rule_num[0] << R_CANFD0_CFDGPFLCFG_RNC0_Pos) |
                                     ((uint32_t) p_global_cfg->pnf_rule_num[3] << R_CANFD0_CFDGPFLCFG_RNC3_Pos) |
                                     ((uint32_t) p_global_cfg->pnf_rule_num[2] << R_CANFD0_CFDGPFLCFG_RNC2_Pos);
        p_canfd_reg->CFDGPFLCFG[1] = ((uint32_t) p_global_cfg->pnf_rule_num[5] << R_CANFD0_CFDGPFLCFG_RNC1_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                     ((uint32_t) p_global_cfg->pnf_rule_num[4] << R_CANFD0_CFDGPFLCFG_RNC0_Pos) |
                                     ((uint32_t) p_global_cfg->pnf_rule_num[7] << R_CANFD0_CFDGPFLCFG_RNC3_Pos) |
                                     ((uint32_t) p_global_cfg->pnf_rule_num[6] << R_CANFD0_CFDGPFLCFG_RNC2_Pos);
#endif

        /* Set number and size of RX message buffers */
        p_canfd_reg->CFDRMNB = p_global_cfg->rx_mb_config; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Configure RX FIFOs and interrupt */
        for (uint32_t rx_fifo = 0U; rx_fifo < CANFD_PRV_RX_FIFO_MAX; rx_fifo++)
        {
            if (1U == (p_global_cfg->rx_fifo_config[rx_fifo].rx_fifo_cfg_b.RFE))
            {
                p_canfd_reg->CFDRFCC[rx_fifo] = p_global_cfg->rx_fifo_config[rx_fifo].rx_fifo_cfg; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
        }

        /* Configure global RXFIFO interrupt */
        R_BSP_IrqCfgEnable(p_global_cfg->rx_fifo_irq, p_global_cfg->rx_fifo_ipl, p_ctrl);

#ifdef BSP_FEATURE_CANFD_FFI_MODE

        /* Initialization of global virtual machine  */
        if (CANFD_FFI_MODE_OPTIONS_ENABLE == p_global_cfg->canfd_ffi_mode)
        {
            /* Enable FFI mode for VM ISR */
            p_canfd_reg->CFDGFFIMC = (uint32_t) (CANFD_PRV_CFDGFFIMC_FFIEN | CANFD_PRV_CFDGFFIMC_KEY); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Config Global virtual machine receive FIFO Configuration */
            p_canfd_reg->CFDVMRFCFG = p_global_cfg->ffi_fifo_vm;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Config Global virtual machine error interrupt */
            p_canfd_reg->CFDGVMEIS = p_global_cfg->ffi_err_vm.ffi_error_cfg_u32;                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Config Global virtual machine Transmit/Receive FIFO and TXQ Configuration */
            uint32_t cfdvmcfg = 0U;

            for (uint32_t txq_cfifo_channel = 0U; txq_cfifo_channel < 8U; txq_cfifo_channel++)
            {
                for (uint8_t tx_queue_index = p_ctrl->txqueue_range_min;
                     tx_queue_index < p_ctrl->txqueue_range_max;
                     tx_queue_index++)
                {
                    cfdvmcfg |= (p_global_cfg->ffi_txq_cfifo_vm[txq_cfifo_channel].ffi_txq_cfifo_cfg_u32) &
                                (R_CANFD0_CFDVMCFG_TXQ0VMN_Msk << (tx_queue_index * 4U));
                }

                for (uint32_t cfifo = 0U; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
                {
                    uint32_t cfifo_index = cfifo + (txq_cfifo_channel * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);

                    if (0U != p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable)
                    {
                        cfdvmcfg |=
                            (p_global_cfg->ffi_txq_cfifo_vm[txq_cfifo_channel].ffi_txq_cfifo_cfg_u32) &
                            (R_CANFD0_CFDVMCFG_CF0VMN_Msk << ((cfifo * 4U)));
                    }
                }

                p_canfd_reg->CFDVMCFG[txq_cfifo_channel] |= cfdvmcfg; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                cfdvmcfg = 0;
            }
        }
#endif

        /* Set global error interrupts */
        p_canfd_reg->CFDGCTR = p_global_cfg->global_interrupts; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Configure common fifo */
        for (uint32_t cfifo = 0U; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT; cfifo++)
        {
            if (0U != p_global_cfg->common_fifo_config[cfifo].cfifo_cfg_b.enable)
            {
                p_canfd_reg->CFDCFCC[cfifo]  = p_global_cfg->common_fifo_config[cfifo].cfifo_cfg_u32;                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                p_canfd_reg->CFDCFCCE[cfifo] = p_global_cfg->common_fifo_config_enhancement[cfifo].cfifo_cfg_enh_u32; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
        }

#ifdef BSP_FEATURE_CANFD_CAN_HUB_MODE

        /* Configure CAN HUB mode */
        p_canfd_reg->CFDGFCMC = p_global_cfg->can_hub_mode;  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#else

        /* Configure Flexible Mode */
        p_canfd_reg->CFDGFCMC = p_global_cfg->flexible_mode; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

        /* Configure Transmission buffer assignment*/
        for (uint32_t flex_tx_assign = 0U; flex_tx_assign < (CANFD_NUM_CHANNEL_PER_UNIT / 2U); flex_tx_assign++)
        {
            p_canfd_reg->CFDGFTBAC |= (uint32_t) p_global_cfg->flex_tx_buffer[flex_tx_assign] << /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
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

    if (0U != p_extend->tx_history_cfg.enable)
    {
        /* Setting Transmit History for TXMB, TXFIFO, TXQueue*/
        p_canfd_reg->CFDTHLCC[channel_index] = (uint32_t) p_extend->tx_history_cfg.enable << /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                               R_CANFD0_CFDTHLCC_THLDTE_Pos;
    }

    /* Configure bitrate */
    p_canfd_reg->CFDC[channel_index].NCFG = /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                            (uint32_t) (((p_cfg->p_bit_timing->baud_rate_prescaler - 1U) &
                                                         R_CANFD0_CFDC_NCFG_NBRP_Msk) <<
                                                        R_CANFD0_CFDC_NCFG_NBRP_Pos) |
                                            ((p_cfg->p_bit_timing->time_segment_1 - 1U) <<
                                             R_CANFD0_CFDC_NCFG_NTSEG1_Pos) |
                                            ((p_cfg->p_bit_timing->time_segment_2 - 1U) <<
                                             R_CANFD0_CFDC_NCFG_NTSEG2_Pos) |
                                            ((p_cfg->p_bit_timing->synchronization_jump_width - 1U) <<
                                             R_CANFD0_CFDC_NCFG_NSJW_Pos);

    /* Configure data bitrate for rate switching on FD frames */
    p_canfd_reg->CFDC2[channel_index].DCFG = /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                             (uint32_t) (((p_extend->p_data_timing->baud_rate_prescaler - 1U) &
                                                          R_CANFD0_CFDC2_DCFG_DBRP_Msk) <<
                                                         R_CANFD0_CFDC2_DCFG_DBRP_Pos) |
                                             ((p_extend->p_data_timing->time_segment_1 - 1U) <<
                                              R_CANFD0_CFDC2_DCFG_DTSEG1_Pos) |
                                             ((p_extend->p_data_timing->time_segment_2 - 1U) <<
                                              R_CANFD0_CFDC2_DCFG_DTSEG2_Pos) |
                                             ((p_extend->p_data_timing->synchronization_jump_width - 1U) <<
                                              R_CANFD0_CFDC2_DCFG_DSJW_Pos);

    /* Ensure transceiver delay offset is not larger than 7 bits */
    uint32_t tdco = p_extend->p_data_timing->time_segment_1;
    if (tdco > CANFD_MAX_UINT_7)
    {
        tdco = CANFD_MAX_UINT_7;
    }

    /* Configure transceiver delay compensation; allow user to set ESI bit manually
     */
    p_canfd_reg->CFDC2[channel_index].FDCFG = /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                              (tdco << R_CANFD0_CFDC2_FDCFG_TDCO_Pos) |
                                              ((uint32_t) p_extend->delay_compensation <<
                                               R_CANFD0_CFDC2_FDCFG_TDCE_Pos) |
                                              R_CANFD0_CFDC2_FDCFG_ESIC_Msk | 1U |
                                              ((uint32_t) p_extend->gateway_cfg.enable <<
                                               R_CANFD0_CFDC2_FDCFG_GWEN_Pos) |
                                              ((uint32_t) p_extend->gateway_cfg.frame <<
                                               R_CANFD0_CFDC2_FDCFG_GWFDF_Pos) |
                                              ((uint32_t) p_extend->gateway_cfg.brs << R_CANFD0_CFDC2_FDCFG_GWBRS_Pos);

    /* Configure protocol mode */
    p_canfd_reg->CFDC2[channel_index].FDCFG |= (uint32_t) p_extend->prt_mode << R_CANFD0_CFDC2_FDCFG_FDOE_Pos;  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#ifdef BSP_FEATURE_CANFD_PN_MODE

/* Configure PNF mode*/
    p_canfd_reg->CFDC2[channel_index].FDCFG |= (uint32_t) p_extend->pnf_mode << R_CANFD0_CFDC2_FDCFG_RPNMD_Pos; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

    /* Write TX message buffer interrupt enable bits. Only enable interrupt TXMB handled by channel */
    if (0U == (p_cfg->channel % 2U))
    {
        /* Even channel lend TXMB from odd channel. Enable TXMB interrupt of odd channel */
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH]         = p_extend->txmb_txi_enable[0];  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_canfd_reg->CFDTMIEC[(channel_index * CANFD_PRV_CFDTM_CH_LENGTH) + 1U]  = p_extend->txmb_txi_enable[1];  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_canfd_reg->CFDTMIEC[(channel_index * CANFD_PRV_CFDTM_CH_LENGTH) + 2U] |= p_extend->txmb_txi_enable[2] & /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                                                   CANFD_PRV_FILL_1(lend_txmb);   /* PRQA S 3469 # Advisory D-4.9 */
    }
    else
    {
        /* Odd channel TXMB is lent. Do not enable lent buffer*/
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH] |= p_extend->txmb_txi_enable[0] &      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                                            (~CANFD_PRV_FILL_1(lend_txmb));     /* PRQA S 3469 # Advisory D-4.9 */
        p_canfd_reg->CFDTMIEC[(channel_index * CANFD_PRV_CFDTM_CH_LENGTH) + 1U] = p_extend->txmb_txi_enable[1]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Configure channel error interrupts */
    p_canfd_reg->CFDC[channel_index].CTR = p_extend->error_interrupts | R_CANFD0_CFDC_CTR_CHMDC_Msk;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Configure Transmit Queue */
    for (uint8_t tx_queue_index = p_ctrl->txqueue_range_min;
         tx_queue_index < p_ctrl->txqueue_range_max;
         tx_queue_index++)
    {
        if (0U != p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable)
        {
            if (tx_queue_index < CANFD_PRV_TXQ_CH_OFFSET)
            {
                p_canfd_reg->CFDTXQ[tx_queue_index].CC[channel_index] = /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                                        p_extend->tx_queue_cfg[tx_queue_index].
                                                                        tx_queue_cfg_u32;
            }
            else
            {
                p_canfd_reg->CFDTXQ[tx_queue_index - CANFD_PRV_TXQ_CH_OFFSET].CC[channel_index + 1U] = /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                                                                       p_extend->
                                                                                                       tx_queue_cfg[
                    tx_queue_index].tx_queue_cfg_u32;
            }
        }
    }

    /* Enable channel interrupts */

    if ((int32_t) p_cfg->error_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->err_ored_int, p_cfg->ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->error_irq, p_cfg->ipl, p_ctrl);
#endif
    }

    if ((int32_t) p_cfg->tx_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->tx_ored_int, p_cfg->ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->tx_irq, p_cfg->ipl, p_ctrl);
#endif
    }

    if ((int32_t) p_cfg->rx_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->rx_ored_int, p_cfg->ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->rx_irq, p_cfg->ipl, p_ctrl);
#endif
    }

    if ((int32_t) p_cfg->tx_vm_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->vmtx_ored_int, p_cfg->vm_ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->tx_vm_irq, p_cfg->vm_ipl, p_ctrl);
#endif
    }

    if ((int32_t) p_cfg->rx_vm_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->vmrx_ored_int, p_cfg->vm_ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->rx_vm_irq, p_cfg->vm_ipl, p_ctrl);
#endif
    }

    if ((int32_t) p_cfg->err_vm_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        {
            R_BSP_ORed_IrqCfgEnable(p_extend->vmerr_ored_int, p_cfg->vm_ipl, p_ctrl);
        }
#else
        R_BSP_IrqCfgEnable(p_cfg->err_vm_irq, p_cfg->vm_ipl, p_ctrl);
#endif
    }

#if !defined(BSP_VLAB_HARDWARE_BASED_USED) || !(BSP_VLAB_HARDWARE_BASED_USED)

    /* Set global mode to Operation and wait for transition */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_OPERATION);

    /* Transition to Channel Operation */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_NORMAL);
#endif

#ifdef BSP_FEATURE_CANFD_PN_MODE

/* Setting PNF mode*/
    p_canfd_reg->CFDC2[channel_index].FDCTR |= ((uint32_t) p_extend->pnf_mode << R_CANFD0_CFDC2_FDCTR_PNMDC_Pos) | /* PRQA S 0662, 1710 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.5 */
                                               CANFD_PRV_PNMDC_KEY;
#endif

    /* Setting Transmit History if channel is requested*/
    if (0U != p_extend->tx_history_cfg.enable)
    {
        p_canfd_reg->CFDTHLCC[channel_index] |= R_CANFD0_CFDTHLCC_THLE_Msk | R_CANFD0_CFDTHLCC_THLIE_Msk | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                ((uint32_t) p_extend->tx_history_cfg.isr_mode <<
                                                 R_CANFD0_CFDTHLCC_THLIM_Pos);

#ifdef BSP_FEATURE_CANFD_FFI_MODE
        p_canfd_reg->CFDTHLCC[channel_index] |= /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                ((uint32_t) p_extend->tx_history_cfg.thl_entry_lost <<
                                                 R_CANFD0_CFDTHLCC_THLEIE_Pos);
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
        if (0U != p_extend->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable)
        {
            if (tx_queue_index < CANFD_PRV_TXQ_CH_OFFSET)
            {
                p_canfd_reg->CFDTXQ[tx_queue_index].CC_b[channel_index].TXQE = 1U;                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
            else
            {
                p_canfd_reg->CFDTXQ[tx_queue_index - CANFD_PRV_TXQ_CH_OFFSET].CC_b[channel_index + 1U].TXQE = 1U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
        }
    }

    /* Enable Common FIFOs can only be enable channel operation mode or channel halt mode */
    for (uint32_t cfifo = 0U; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
    {
        uint32_t cfifo_index = (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL) + cfifo;
        p_canfd_reg->CFDCFCC_b[cfifo_index].CFE = p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Set current operation modes */
    p_ctrl->operation_mode = CAN_OPERATION_MODE_NORMAL;
    p_ctrl->test_mode      = CAN_TEST_MODE_DISABLED;

#if BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ
    if (BSP_INTSEL_DISABLED != p_extend->intsel_irq_cfg) /* PRQA S 4342 # Advisory R-10.5 */
    {
        R_BSP_IntSel_CfgEnable(p_extend->intsel_irq_cfg);
    }
#endif

#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE
    err = r_canfd_transfer_cfg(p_ctrl, p_extend, p_global_cfg);
    if (FSP_SUCCESS != err)
    {
        /* Transfer configuration failed after the channel was already brought up. Roll back so the channel is
         * not left half-open (which would make it permanently unusable: Open() fails with FSP_ERR_IN_USE and
         * Close() fails with FSP_ERR_NOT_OPEN because open was never set). Return the channel to Reset mode and
         * release the tracked control pointer before returning the error. */
        r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_RESET);
        gp_ctrl[p_cfg->channel] = NULL;

        return err;
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
fsp_err_t R_CANFD_Close (can_ctrl_t * const p_api_ctrl)                     /* PRQA S 1505 # Advisory R-8.7 */
{
    canfd_instance_ctrl_t  * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    can_cfg_t const        * p_cfg;
    canfd_extended_cfg_t   * p_extend;                                      /* PRQA S 3678 # Advisory R-8.13 */
    canfd_global_cfg_t     * p_global_cfg;                                  /* PRQA S 3678 # Advisory R-8.13 */
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;
    bool global_close_sts = false;
#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get control block */
    p_ctrl   = (canfd_instance_ctrl_t *) p_api_ctrl;     /* PRQA S 0316 # Advisory R-11.5 */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;              /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    FSP_ASSERT(NULL != p_extend->p_global_cfg);          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#else

    /* Get control block */
    p_ctrl   = (canfd_instance_ctrl_t *) p_api_ctrl;                /* PRQA S 0316 # Advisory R-11.5 */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;                         /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;            /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;
#endif

    /* Disable channel interrupts */
    if ((int32_t) p_cfg->error_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        {
            R_BSP_ORed_IrqCfgDisable(p_extend->err_ored_int);
        }
#else
        R_BSP_IrqDisable(p_cfg->error_irq);
#endif
    }

    if ((int32_t) p_cfg->tx_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
        {
            R_BSP_ORed_IrqCfgDisable(p_extend->tx_ored_int);
        }
#else
        R_BSP_IrqDisable(p_cfg->tx_irq);
#endif
    }

    if ((int32_t) p_cfg->rx_irq >= 0)
    {
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
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
#if !defined(BSP_VLAB_HARDWARE_BASED_USED) || !(BSP_VLAB_HARDWARE_BASED_USED)
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
            p_canfd_reg->CFDTXQ[tx_queue_index].CC[p_ctrl->channel_index] = 0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else
        {
            /* Transmit queue lend from next channel */
            p_canfd_reg->CFDTXQ[tx_queue_index - CANFD_PRV_TXQ_CH_OFFSET].CC[channel_index + 1U] = 0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
    }

    /* Disable TX message buffer interrupt. Only disable interrupt TXMB handled by channel */
    uint32_t lend_txmb = CANFD_PRV_LEND_BUFF_GET(p_canfd_reg, channel_index);                                    /* PRQA S 0310, 0312, 0488, 0662, 3469 # DP_MSR_R-11.3_UC1 , DP_MSR_R-11.8_UC1 , Advisory R-18.4 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.9 */
    if (0U == (channel_index % 2U))
    {
        /* Even channel owns both of its words and the buffers lent from the consecutive odd channel */
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH]         = 0;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_canfd_reg->CFDTMIEC[(channel_index * CANFD_PRV_CFDTM_CH_LENGTH) + 1U]  = 0;                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_canfd_reg->CFDTMIEC[(channel_index * CANFD_PRV_CFDTM_CH_LENGTH) + 2U] &= ~CANFD_PRV_FILL_1(lend_txmb); /* PRQA S 3469, 0662 # Advisory D-4.9 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        /* Odd channel TXMB is lent. Keep the lent buffers owned by the previous even channel */
        p_canfd_reg->CFDTMIEC[channel_index * CANFD_PRV_CFDTM_CH_LENGTH]       &= CANFD_PRV_FILL_1(lend_txmb); /* PRQA S 3469, 0662 # Advisory D-4.9 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_canfd_reg->CFDTMIEC[(channel_index * CANFD_PRV_CFDTM_CH_LENGTH) + 1U] = 0;                           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Reset global control struct pointer */
    gp_ctrl[p_cfg->channel] = NULL;

    /* If all channel is close set CANFD to SLEEP*/
    global_close_sts = r_canfd_is_all_channel_close(p_global_cfg->unit);
    if (global_close_sts)
    {
        /* Disable RX FIFO interrupt */
        R_BSP_IrqDisable(p_global_cfg->rx_fifo_irq);

        /* Clear flag manually before enter global reset mode */
#ifdef BSP_VLAB_HARDWARE_BASED_USED

        /* Clear all RXFIFO interrupt flags */
        for (uint32_t rx_fifo = 0U; rx_fifo < BSP_FEATURE_CANFD_NUM_RXFIFO; rx_fifo++)
        {
            p_canfd_reg->CFDRFSTS[rx_fifo] = 0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Clear RXMB new data flags */
        for (uint32_t cfdrmnd = 0U; cfdrmnd < CANFD_PRV_RXMB_ND_REG_NUM(p_global_cfg->unit); cfdrmnd++)
        {
            p_canfd_reg->CFDRMND[cfdrmnd] = 0U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        for (uint32_t common_fifo = 0U; common_fifo < BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT; common_fifo++)
        {
            /* Disable Common FIFO */
            p_canfd_reg->CFDCFCC_b[common_fifo].CFE = 0U; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Clear all status flags */
            p_canfd_reg->CFDCFSTS[common_fifo] = 0U;      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
#endif

        /* Transition to Global Reset */
        r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);

        /* Clear RX FIFO settings */
        for (uint32_t rx_fifo = 0U; rx_fifo < BSP_FEATURE_CANFD_NUM_RXFIFO; rx_fifo++)
        {
            p_canfd_reg->CFDRFCC[rx_fifo] = 0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Clear Common FIFO settings */
        for (uint32_t common_fifo = 0U; common_fifo < BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT; common_fifo++)
        {
            p_canfd_reg->CFDCFCC[common_fifo] = 0;                                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Reset Flexible Mode/CANHUB mode */
        p_canfd_reg->CFDGFCMC = 0U;                                                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Reset Flexible Transmission buffer assignment*/
        p_canfd_reg->CFDGFTBAC = 0U;                                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Reset Transmit History configuration */
        p_canfd_reg->CFDTHLCC[channel_index] = 0U;                                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#ifdef BSP_FEATURE_CANFD_FFI_MODE

        /* Disable FFI mode */
        p_canfd_reg->CFDGFFIMC = (uint32_t) (CANFD_PRV_CFDGFFIMC_FFIEN_OFF | CANFD_PRV_CFDGFFIMC_KEY); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Reset target Virtual Machine channel for Rx FIFO */
        p_canfd_reg->CFDVMRFCFG = 0;                                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Reset target Virtual Machine channel for TxFX FIFO and Tx Queue */
        p_canfd_reg->CFDVMCFG[channel_index] = 0;                                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

        /* Transition to Global Sleep */
#if !defined(BSP_VLAB_HARDWARE_BASED_USED) || !(BSP_VLAB_HARDWARE_BASED_USED)
        r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_SLEEP);
#endif
    }

#if BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ
    if (BSP_INTSEL_DISABLED != p_extend->intsel_irq_cfg) /* PRQA S 4342 # Advisory R-10.5 */
    {
        R_BSP_IntSel_CfgDisable(p_extend->intsel_irq_cfg);
    }
#endif

#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE
    if (global_close_sts)
    {
        (void) r_canfd_transfer_rx_fifo_close(p_global_cfg->transfer_rx_fifo_cfg);
        p_global_cfg->status_transfer_cfg = 0;
    }
    (void) r_canfd_transfer_channel_close(p_extend->p_transfer_cfg, p_global_cfg->p_canfd_reg, p_ctrl->channel_index);
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
 * @retval FSP_ERR_CAN_TRANSMIT_NOT_READY   Buffer not ready: transmission in progress, or transmission or abort
 *                                          request pending. For a transmit message buffer whose interrupt is
 *                                          enabled, also returned while the result of the previous transmission
 *                                          has not been handled by the transmit interrupt yet.
 * @retval FSP_ERR_INVALID_ARGUMENT         Data length or buffer invalid.
 * @retval FSP_ERR_INVALID_MODE             An FD option was set on a non-FD frame.
 * @retval FSP_ERR_ASSERTION                Null pointer presented
 * @retval FSP_ERR_UNSUPPORTED              Selected buffer or frame format not supported
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Write (can_ctrl_t * const p_api_ctrl, can_buffer_t buffer, can_frame_t * const p_frame)             /* PRQA S 1505, 3673 # Advisory R-8.7 , Advisory R-8.13 */
{
    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;                                      /* PRQA S 0316 # Advisory R-11.5 */
    canfd_extended_cfg_t const  * p_extend;
    can_buffer_type_t             buffer_type = (can_buffer_type_t) ((uint32_t) buffer & CANFD_PRV_BUFFER_TYPE_MASK); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */
    uint32_t  buffer_index = ((uint32_t) buffer & CANFD_PRV_BUFFER_NUM_MASK);
    fsp_err_t err          = FSP_SUCCESS;
#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE
    canfd_transfer_cfg_t * p_transfer_cfg = NULL;
    uint8_t                transfer_type  = 0U;                                                            /* 0: none, 1: Transfer */
#endif

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);                                                                            /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_frame);                                                                           /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    p_extend = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend;                                           /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    FSP_ASSERT(NULL != p_extend);                                                                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check valid buffer number */
    FSP_ERROR_RETURN(FSP_SUCCESS == r_canfd_validate_tx_buffer(p_ctrl, buffer), FSP_ERR_INVALID_ARGUMENT); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check selected frame format is supported */
    FSP_ERROR_RETURN(p_frame->format != CAN_FRAME_FORMAT_XL, FSP_ERR_UNSUPPORTED);                         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check DLC field */
    if (CAN_FRAME_FORMAT_CC == p_frame->format)
    {
        FSP_ERROR_RETURN(p_frame->data_length_code <= 8U, FSP_ERR_INVALID_ARGUMENT);        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        FSP_ERROR_RETURN(CAN_FRAME_OPTIONS_NONE == p_frame->options, FSP_ERR_INVALID_MODE); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    }
    else if (CAN_FRAME_FORMAT_FD == p_frame->format)
    {
        /* Make sure the supplied data size corresponds to a valid DLC value */
        FSP_ERROR_RETURN(p_frame->data_length_code <= 64U, FSP_ERR_INVALID_ARGUMENT); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    }
    else
    {
        /* Do nothing. */
    }

#else
    p_extend = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
#endif

#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE
    transfer_type = 1U;

    if ((CAN_BUFFER_TX_PRIORITY_0 == buffer) && (NULL != p_extend->p_transfer_cfg[2]) &&
        (NULL != p_extend->p_transfer_cfg[2]->p_transfer))
    {
        p_transfer_cfg = p_extend->p_transfer_cfg[2];
    }
    else if ((CAN_BUFFER_TX_PRIORITY_3 == buffer) && (NULL != p_extend->p_transfer_cfg[3]) &&
             (NULL != p_extend->p_transfer_cfg[3]->p_transfer))
    {
        p_transfer_cfg = p_extend->p_transfer_cfg[3];
    }
    else if ((CAN_BUFFER_TX_PRIORITY_4 == buffer) && (NULL != p_extend->p_transfer_cfg[4]) &&
             (NULL != p_extend->p_transfer_cfg[4]->p_transfer))
    {
        p_transfer_cfg = p_extend->p_transfer_cfg[4];
    }
    else if ((CAN_BUFFER_TX_FIFO_2 == buffer) && (NULL != p_extend->p_transfer_cfg[1]) &&
             (NULL != p_extend->p_transfer_cfg[1]->p_transfer))
    {
        p_transfer_cfg = p_extend->p_transfer_cfg[1];
    }
    else
    {
        transfer_type = 0U;
    }

    if (1U == transfer_type)
    {
        err =
            r_canfd_transfer_write(p_ctrl->channel_index, buffer_type, buffer_index, p_frame, p_extend, p_transfer_cfg);
    }
    else
#endif
    {
        err = r_canfd_mb_write(p_ctrl->channel_index,
                               buffer_type,
                               buffer_index,
                               p_frame,
                               p_extend,
                               p_extend->p_global_cfg->p_canfd_reg);
    }

    return err;
}

/***************************************************************************************************************//**
 * Read data from a CANFD Message Buffer or FIFO.
 *
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_INVALID_ARGUMENT         Buffer number invalid to used.
 * @retval FSP_ERR_ASSERTION                p_api_ctrl or p_frame or register of buffer is NULL.
 * @retval FSP_ERR_BUFFER_EMPTY             Buffer or FIFO is empty.
 * @retval FSP_ERR_UNSUPPORTED              Selected buffer is not supported.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Read (can_ctrl_t * const p_api_ctrl, can_buffer_t buffer, can_frame_t * const p_frame)              /* PRQA S 3673, 1505 # Advisory R-8.13 , Advisory R-8.7 */
{
    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;                                      /* PRQA S 0316 # Advisory R-11.5 */
    canfd_extended_cfg_t const  * p_extend;
    can_buffer_type_t             buffer_type = (can_buffer_type_t) ((uint32_t) buffer & CANFD_PRV_BUFFER_TYPE_MASK); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */
    uint32_t  buffer_index = (uint32_t) buffer & CANFD_PRV_BUFFER_NUM_MASK;
    fsp_err_t err          = FSP_SUCCESS;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);                                                                                   /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_frame);                                                                                  /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    p_extend = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend;                                                  /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    FSP_ASSERT(NULL != p_extend);                                                                                 /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                                               /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(FSP_SUCCESS == r_canfd_validate_rx_buffer(p_ctrl->p_cfg, buffer), FSP_ERR_INVALID_ARGUMENT); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#else
    p_extend = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend;                                                  /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
#endif
    if (CAN_BUFFER_TYPE_RX_SINGLE == buffer_type)
    {
        err = r_canfd_mb_read(p_ctrl->channel_index,
                              buffer_type,
                              buffer_index,
                              p_frame,
                              p_extend->p_global_cfg->p_canfd_reg);
    }
    else
    {
#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE

        /* When a transfer instance (DMAC/DTSC) is linked to the selected FIFO, received frames have already been
         * moved from the hardware FIFO to the destination buffer in RAM by the transfer. In that case read the
         * frame from the transfer destination queue instead of the CANFD registers. */
        if ((NULL != p_extend->p_transfer_cfg[0]) && (NULL != p_extend->p_transfer_cfg[0]->p_transfer) &&
            (CAN_BUFFER_RX_FIFO_8 == buffer))
        {
            /* RX FIFO 8 is the channel Common FIFO 0 in RX mode; its transfer configuration is held per channel
             * in the extended configuration (p_transfer_cfg[0]). */
            err = r_canfd_fifo_transfer_read(p_frame, p_extend->p_transfer_cfg[0]);
        }
        else if ((p_extend->p_global_cfg->transfer_rx_fifo_cfg[(uint32_t) buffer -
                                                               (uint32_t) CAN_BUFFER_RX_FIFO_0].p_transfer != NULL) &&
                 (buffer_index < CANFD_PRV_CFIFO_OFFSET))
        {
            /* RX FIFO 0-7 are unit FIFOs; their transfer configurations are held per unit in the global
             * configuration (transfer_rx_fifo_cfg[]). */
            err =
                r_canfd_fifo_transfer_read(p_frame,
                                           &p_extend->p_global_cfg->transfer_rx_fifo_cfg[(uint32_t) buffer -
                                                                                         (uint32_t) CAN_BUFFER_RX_FIFO_0]);
        }
        else
#endif
        {
            /* No transfer instance is linked to the selected buffer: read the frame directly from the
             * Message Buffer/FIFO registers. */
            err = r_canfd_mb_read(p_ctrl->channel_index,
                                  buffer_type,
                                  buffer_index,
                                  p_frame,
                                  p_extend->p_global_cfg->p_canfd_reg);
        }
    }

    return err;
}

/***************************************************************************************************************//**
 * Switch to a different channel, global or test mode.
 *
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_ASSERTION                Null pointer presented
 * @retval FSP_ERR_INVALID_MODE             Cannot change to the requested mode from the current global mode.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_ModeTransition (can_ctrl_t * const   p_api_ctrl,              /* PRQA S 1505 # Advisory R-8.7 */
                                  can_operation_mode_t operation_mode,
                                  can_test_mode_t      test_mode)
{
    canfd_instance_ctrl_t      * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    canfd_extended_cfg_t const * p_extend_cfg;
    canfd_global_cfg_t const   * p_global_cfg;
    fsp_err_t                err = FSP_SUCCESS;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;
#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);                                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_ctrl->p_cfg->p_extend);                     /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);  /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    p_extend_cfg = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    FSP_ASSERT(NULL != p_extend_cfg->p_global_cfg);                  /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get global config */
    p_global_cfg = p_extend_cfg->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get Global Status */
    uint32_t cfdgsts = p_canfd_reg->CFDGSTS;                                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Check to ensure the current mode is Global Halt when transitioning into or out of Internal Bus mode */
    FSP_ERROR_RETURN((0U != (cfdgsts & R_CANFD0_CFDGSTS_GHLTSTS_Msk)) || !((p_ctrl->test_mode != test_mode) && /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                                                                           ((CAN_TEST_MODE_INTERNAL_BUS ==
                                                                             p_ctrl->test_mode) ||
                                                                            (CAN_TEST_MODE_INTERNAL_BUS == test_mode))),
                     FSP_ERR_INVALID_MODE);                                                                    /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check to ensure the current mode is Global Reset when transitioning into or out of Global Sleep (see Section
     * "Global Modes" in the RH850/U2B microcontroller User's Manual for details) */
    FSP_ERROR_RETURN(((0U != (cfdgsts & R_CANFD0_CFDGSTS_GRSTSTS_Msk)) &&                                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                      (0U != ((uint32_t) CAN_OPERATION_MODE_RESET & (uint32_t) operation_mode))) ||
                     ((0U == (cfdgsts & R_CANFD0_CFDGSTS_GSLPSTS_Msk)) &&
                      (CAN_OPERATION_MODE_GLOBAL_SLEEP != operation_mode)),
                     FSP_ERR_INVALID_MODE);                                                                    /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check to ensure the current Global mode supports the requested Channel mode, if applicable. The requested mode
     * and the current global mode are converted into a number 0-3 corresponding to Operation, Halt, Reset and Sleep
     * respectively. The channel mode cannot be switched to a mode with an index lower than the current global mode. */
    if (operation_mode < CAN_OPERATION_MODE_GLOBAL_OPERATION)
    {
        FSP_ERROR_RETURN(g_mode_order[operation_mode] >= g_mode_order[cfdgsts & CANFD_PRV_CTR_MODE_MASK], /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                         FSP_ERR_INVALID_MODE);                                                           /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    }

#else
    p_extend_cfg = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend;                                      /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

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
            p_canfd_reg->CFDC[channel_index].CTR_b.CTME = 0;           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Link channel to internal bus */
            p_canfd_reg->CFDGTSTCFG |= (uint32_t) 1U << channel_index; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Enable internal bus test mode */
            p_canfd_reg->CFDGTSTCTR = 1;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else
        {
            if (CAN_TEST_MODE_INTERNAL_BUS == p_ctrl->test_mode)
            {
                /* Unlink channel from internal bus */
                p_canfd_reg->CFDGTSTCFG &= ~((uint32_t) 1U << channel_index); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /* Disable global test mode if no channels are linked */
                if (0U == p_canfd_reg->CFDGTSTCFG)                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                {
                    p_canfd_reg->CFDGTSTCTR = 0;                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                }
            }

            /* Transition to Channel Halt when changing test modes */
            r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_HALT);

            /* Set channel test mode */
            uint32_t cfdcnctr = p_canfd_reg->CFDC[channel_index].CTR; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            cfdcnctr &= ~(R_CANFD0_CFDC_CTR_CTME_Msk | R_CANFD0_CFDC_CTR_CTMS_Msk);
            p_canfd_reg->CFDC[channel_index].CTR = cfdcnctr |         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
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
                if (0U != p_extend_cfg->tx_queue_cfg[tx_queue_index].tx_queue_cfg_b.enable)
                {
                    p_canfd_reg->CFDTXQ[tx_queue_index].CC_b[channel_index].TXQE = /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                                                   p_extend_cfg->tx_queue_cfg[
                        tx_queue_index].tx_queue_cfg_b.enable;
                }
            }

            /* Re-enable Common FIFO*/
            for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
            {
                uint32_t cfifo_index = (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL) + cfifo;
                p_canfd_reg->CFDCFCC_b[cfifo_index].CFE = /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                          p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.
                                                          enable;
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
fsp_err_t R_CANFD_InfoGet (can_ctrl_t * const p_api_ctrl, can_info_t * const p_info) /* PRQA S 1505, 3673 # Advisory R-8.7 , Advisory R-8.13 */
{
    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;     /* PRQA S 0316 # Advisory R-11.5 */
    can_cfg_t const             * p_cfg;
    canfd_extended_cfg_t const  * p_extend;
    canfd_global_cfg_t const    * p_global_cfg;
    uint32_t                 fdsts;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;
#if CANFD_CFG_PARAM_CHECKING_ENABLE

    /* Check pointers for NULL values */
    FSP_ASSERT(NULL != p_ctrl);                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_info);                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;              /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    FSP_ASSERT(NULL != p_extend->p_global_cfg);          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* If channel is not open, return an error */
    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#else

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;                         /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;            /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;
#endif

    uint32_t cfdcnsts = p_canfd_reg->CFDC[channel_index].STS; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_info->status               = cfdcnsts & (uint32_t) UINT16_MAX;
    p_info->error_count_receive  = (uint8_t) ((cfdcnsts & R_CANFD0_CFDC_STS_REC_Msk) >> R_CANFD0_CFDC_STS_REC_Pos);
    p_info->error_count_transmit = (uint8_t) ((cfdcnsts & R_CANFD0_CFDC_STS_TEC_Msk) >> R_CANFD0_CFDC_STS_TEC_Pos);

    for (uint32_t rxmb_sts = 0; rxmb_sts < (CANFD_PRV_RXMB_ND_REG_NUM(p_global_cfg->unit)); rxmb_sts++)
    {
        p_info->rx_single_status[rxmb_sts] = p_canfd_reg->CFDRMND[rxmb_sts];                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Get RX FIFO Empty status */
    p_info->rx_fifo_status = (~p_canfd_reg->CFDFESTS) & CANFD_PRV_CFDFESTS_RFXEMP_MSK;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Get Common FIFO Empty status */
    uint32_t cfifo_empty =
        (((~(uint32_t) p_canfd_reg->CFDFESTS_b.CFkEMP) >> (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)) & /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
         0x07U);                                                                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_info->rx_fifo_status |= cfifo_empty << CANFD_PRV_CFIFO_OFFSET;

    fdsts = 0;

    /* Read SOCO, EOCO, TDCVF value */
    if (0U != p_canfd_reg->CFDC2[channel_index].FDSTS_b.TDCVF) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        fdsts |= (uint32_t) CANFD_ERROR_CHANNEL_DELAY_VIOLATE;
    }

    if (0U != p_canfd_reg->CFDC2[channel_index].FDSTS_b.EOCO) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        fdsts |= (uint32_t) CANFD_ERROR_CHANNEL_ERR_OCCUR_OVF;
    }

    if (0U != p_canfd_reg->CFDC2[channel_index].FDSTS_b.SOCO) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        fdsts |= (uint32_t) CANFD_ERROR_CHANNEL_SUC_OCCUR_OVF;
    }

    /* Clear SOCO, EOCO, TDCVF flags. The three flags are not adjacent and are cleared by writing zero. */
    p_canfd_reg->CFDC2[channel_index].FDSTS &= ~(R_CANFD0_CFDC2_FDSTS_TDCVF_Msk | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                 R_CANFD0_CFDC2_FDSTS_EOCO_Msk |
                                                 R_CANFD0_CFDC2_FDSTS_SOCO_Msk);

    /* Read and clear channel error flags. */
    uint32_t cherfl = p_canfd_reg->CFDC[channel_index].ERFL & CANFD_PRV_ERROR_CH_FLAG_REG_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_canfd_reg->CFDC[channel_index].ERFL = 0;                                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_info->error_code = cherfl | fdsts;

    /* Dummy read to ensure that interrupt event is cleared. */
    volatile uint32_t dummy = p_canfd_reg->CFDC[channel_index].ERFL; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
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
fsp_err_t R_CANFD_CallbackSet (can_ctrl_t * const          p_api_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                               void (                    * p_callback)(can_callback_args_t * p_args),
                               void * const                p_context,
                               can_callback_args_t * const p_callback_memory)
{
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);                                            /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_callback);                                        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
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
fsp_err_t R_CANFD_StartBusIdleCnt (can_ctrl_t * const p_api_ctrl)                /* PRQA S 1503, 3673 # DP_MSR_R-2.1_UC1 , Advisory R-8.13 */
{
    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    can_cfg_t const             * p_cfg;
    canfd_extended_cfg_t const  * p_extend;
    canfd_global_cfg_t const    * p_global_cfg;
    volatile R_CANFD0_Type      * p_canfd_reg;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;              /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    FSP_ASSERT(NULL != p_extend->p_global_cfg);          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(CAN_OPERATION_MODE_NORMAL == p_ctrl->operation_mode, FSP_ERR_INVALID_MODE); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#else

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;                                                      /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;                                         /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;
#endif

    /* Enable and reset bus load counter */
    p_canfd_reg->CFDC2[p_ctrl->channel_index].BLCT = (uint32_t) R_CANFD0_CFDC2_BLCT_BLCLD_Msk | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                     (uint32_t) R_CANFD0_CFDC2_BLCT_BLCE_Msk;   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

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
fsp_err_t R_CANFD_StopBusIdleCnt (can_ctrl_t * const p_api_ctrl, uint32_t * p_idle_time_ns) /* PRQA S 1503, 3673 # DP_MSR_R-2.1_UC1 , Advisory R-8.13 */
{
    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;            /* PRQA S 0316 # Advisory R-11.5 */
    can_cfg_t const             * p_cfg;
    canfd_extended_cfg_t const  * p_extend;
    canfd_global_cfg_t const    * p_global_cfg;
    volatile R_CANFD0_Type      * p_canfd_reg;
    uint32_t idle_cnt;
    uint32_t clkc_freq;
    uint64_t idle_time_ns;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;              /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    FSP_ASSERT(NULL != p_extend->p_global_cfg);          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(CAN_OPERATION_MODE_NORMAL == p_ctrl->operation_mode, FSP_ERR_INVALID_MODE); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#else

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;                                                      /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;                                         /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;
#endif

    /* Stop bus load counter and retrieve idle time counter */
    p_canfd_reg->CFDC2[p_ctrl->channel_index].BLCT = (uint32_t) R_CANFD0_CFDC2_BLCT_BLCLD_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    idle_cnt = p_canfd_reg->CFDC2[p_ctrl->channel_index].BLSTS;                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Calculate idle time */
    clkc_freq       = R_FSP_SystemClockHzGet(BSP_FEATURE_CANFD_CLKC_SRC);
    idle_time_ns    = ((uint64_t) idle_cnt * (uint64_t) CANFD_PRV_SEC_TO_NS) / (uint64_t) clkc_freq;
    *p_idle_time_ns = (uint32_t) idle_time_ns;

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
fsp_err_t R_CANFD_PNFTransition (can_ctrl_t * const p_api_ctrl)                  /* PRQA S 1503, 3673 # DP_MSR_R-2.1_UC1 , Advisory R-8.13 */
{
    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    can_cfg_t const             * p_cfg;
    canfd_extended_cfg_t const  * p_extend;
    canfd_global_cfg_t const    * p_global_cfg;
    uint32_t                 channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;

 #if CANFD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;              /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    FSP_ASSERT(NULL != p_extend->p_global_cfg);          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(CAN_OPERATION_MODE_NORMAL == p_ctrl->operation_mode, FSP_ERR_INVALID_MODE); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
 #else

    /* Get control block */
    p_cfg    = (can_cfg_t *) p_ctrl->p_cfg;                                                      /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;                                         /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get CANFD base reg */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;
 #endif

    /* Transition to PNF mode */
    p_canfd_reg->CFDC2[channel_index].FDCTR = ((uint32_t) CANFD_FILTER_MODE_PNF << R_CANFD0_CFDC2_FDCTR_PNMDC_Pos) | /* PRQA S 0662, 1710 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.5 */
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
fsp_err_t R_CANFD_RAMTest (can_ctrl_t * const      p_api_ctrl, /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
                           can_cfg_t const * const p_cfg,
                           canfd_ramtest_field_t   ram_test_field,
                           uint32_t                page_id)
{
    fsp_err_t               err    = FSP_SUCCESS;
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    canfd_ramtest_t       * p_ram_test_infor;
    uint8_t                 unit = 0;

#if CANFD_CFG_PARAM_CHECKING_ENABLE

    /*Check null of pointer*/
    FSP_ASSERT(NULL != p_ctrl);                                                       /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ERROR_RETURN(CANFD_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);               /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    FSP_ASSERT(NULL != p_cfg);                                                        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get extended config */
    canfd_extended_cfg_t const * p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    FSP_ASSERT(NULL != p_extend);                                                     /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get global config */
    canfd_global_cfg_t const * p_global_cfg = p_extend->p_global_cfg;

    FSP_ASSERT(NULL != p_global_cfg);  /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    unit = p_global_cfg->unit;

    /* Get RAM infor config */
    p_ram_test_infor            = p_global_cfg->p_ram_infor;
    p_ram_test_infor->ram_field = ram_test_field;

    if (0U != (uint32_t) ram_test_field)
    {
 #ifndef BSP_FEATURE_CANFD_PN_MODE

        return FSP_ERR_INVALID_MODE;
 #endif
    }

    p_ram_test_infor->ram_max_page = r_canfd_ram_caculate_max_page(unit, ram_test_field);

    FSP_ERROR_RETURN(page_id < p_ram_test_infor->ram_max_page, FSP_ERR_INVALID_ARGUMENT); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
#else

    /* Get extended config */
    canfd_extended_cfg_t const * p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;     /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get global config */
    canfd_global_cfg_t const * p_global_cfg = p_extend->p_global_cfg;

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
    FSP_HARDWARE_REGISTER_WAIT((p_canfd_reg->CFDGSTS & R_CANFD0_CFDGSTS_GRAMINIT_Msk), 0U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Transition to Global Reset */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);

    /* Transition to Global Test Mode */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_HALT);

    /* Write protection data to LOCKK register */
    p_canfd_reg->CFDGLOCKK = CANFD_GLOCKK_UNLOCK_KEY1;       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_canfd_reg->CFDGLOCKK = CANFD_GLOCKK_UNLOCK_KEY2;       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Enable RAM Test Mode */
    p_canfd_reg->CFDGTSTCTR |= R_CANFD0_CFDGTSTCTR_RTME_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Enable RAM Test Mode */
    p_canfd_reg->CFDGTSTCFG_b.RTMPS = page_id;               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

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
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* RAM test using checker walkpath algorith type 0 */
    err = r_canfd_ram_walkpath_algorithm(p_global_cfg, CAN_RAMTEST_WALK_TYPE_0);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* RAM test using checker walkpath algorith type 1 */
    err = r_canfd_ram_walkpath_algorithm(p_global_cfg, CAN_RAMTEST_WALK_TYPE_1);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Set all data ram with zero */
    r_canfd_set_ram_data(p_global_cfg, CAN_RAMTEST_DATA_0);

    /*Disable RAM Test Mode*/
    p_canfd_reg->CFDGTSTCTR &= ~R_CANFD0_CFDGTSTCTR_RTME_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

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
static bool r_canfd_bit_timing_nominal_parameter_check (can_bit_timing_cfg_t const * const p_bit_timing)
{
    /* Check that prescaler is in range */
    FSP_ERROR_RETURN((p_bit_timing->baud_rate_prescaler <= CANFD_BAUD_RATE_PRESCALER_MAX) && /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                     (p_bit_timing->baud_rate_prescaler >= CANFD_BAUD_RATE_PRESCALER_MIN),
                     false);                                                                 /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check that TSEG1 > TSEG2 >= SJW for nominal bitrate per section "Bit Timing Conditions" in the
     * RH850/U2x microcontroller User's Manual. */

    /* Check Time Segment 1 is greater than Time Segment 2 */
    FSP_ERROR_RETURN((uint32_t) p_bit_timing->time_segment_1 > (uint32_t) p_bit_timing->time_segment_2, false);      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check Time Segment 2 is greater than or equal to the synchronization jump width */
    FSP_ERROR_RETURN((uint32_t) p_bit_timing->time_segment_2 >= (uint32_t) p_bit_timing->synchronization_jump_width, /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                     false);                                                                                         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    return true;
}

/***********************************************************************************************************************
 * Check bit timing status for data bit rate
 * @param[in]     p_bit_timing     CAN bit timing
 **********************************************************************************************************************/
static bool r_canfd_bit_timing_data_parameter_check (can_bit_timing_cfg_t const * const p_bit_timing)
{
    /* Check that prescaler is in range */
    FSP_ERROR_RETURN((p_bit_timing->baud_rate_prescaler <= CANFD_BAUD_RATE_PRESCALER_MAX) && /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                     (p_bit_timing->baud_rate_prescaler >= CANFD_BAUD_RATE_PRESCALER_MIN),
                     false);                                                                 /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check that TSEG1 > TSEG2 >= SJW for nominal bitrate per section "Bit Timing Conditions" in the
     * RH850/U2x microcontroller User's Manual. */

    /* Check Time Segment 1 is greater than Time Segment 2 */
    FSP_ERROR_RETURN((uint32_t) p_bit_timing->time_segment_1 >= (uint32_t) p_bit_timing->time_segment_2, false);     /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Check Time Segment 2 is greater than or equal to the synchronization jump width */
    FSP_ERROR_RETURN((uint32_t) p_bit_timing->time_segment_2 >= (uint32_t) p_bit_timing->synchronization_jump_width, /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                     false);                                                                                         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

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
static fsp_err_t r_canfd_validate_tx_buffer (canfd_instance_ctrl_t const * p_ctrl, can_buffer_t buffer)
{
    /* Get config struct */
    const can_cfg_t * p_cfg = p_ctrl->p_cfg;

    /* Get extended config and global config*/
    canfd_extended_cfg_t const * p_extend_cfg = (canfd_extended_cfg_t *) p_cfg->p_extend;                            /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    canfd_global_cfg_t const   * p_global_cfg = p_extend_cfg->p_global_cfg;

    can_buffer_type_t buffer_type = (can_buffer_type_t) ((uint32_t) buffer & (uint32_t) CANFD_PRV_BUFFER_TYPE_MASK); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */
    uint32_t          buffer_num  = (uint32_t) buffer & CANFD_PRV_BUFFER_NUM_MASK;

    /* Check the buffer type is supported */
    FSP_ERROR_RETURN(((CAN_BUFFER_TYPE_TX_FIFO == buffer_type) ||                                                    /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                      (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type) ||
                      (CAN_BUFFER_TYPE_TX_SINGLE == buffer_type)),
                     FSP_ERR_INVALID_ARGUMENT);                                                                      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /*Take index for channel when device have more than one unit*/
    uint32_t channel_index = p_cfg->channel - (CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit);

    if (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type)
    {
        /* Check TXMB is not allocate in TX Queue */
        FSP_ERROR_RETURN(buffer_num <                                                                                   /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                         (CANFD_PRV_TXMB_CHANNEL_OFFSET -
                          ((uint32_t) p_extend_cfg->tx_queue_cfg[buffer_num].tx_queue_cfg_b.depth + 1U)),
                         FSP_ERR_INVALID_ARGUMENT);                                                                     /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

        /* Check Tx Queue is enabled if transmit buffer Transmit Queue*/
        FSP_ERROR_RETURN(0U != p_extend_cfg->tx_queue_cfg[buffer_num].tx_queue_cfg_b.enable, FSP_ERR_INVALID_ARGUMENT); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    }

    if (CAN_BUFFER_TYPE_TX_FIFO == buffer_type)
    {
        /* CANFD only have 3 common FIFO to be configured in TX Mode */
        FSP_ERROR_RETURN(buffer_num < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL, FSP_ERR_UNSUPPORTED); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        uint32_t cfifo_index = (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL) + buffer_num;

        /* Check Common FIFO is enabled */
        FSP_ERROR_RETURN(1U == p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable, /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                         FSP_ERR_INVALID_ARGUMENT);                                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

        /* Check Common FIFO is enabled and operation in transmit*/
        FSP_ERROR_RETURN(0U != p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable,
                         FSP_ERR_INVALID_ARGUMENT);                                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        FSP_ERROR_RETURN(
            (uint32_t) CANFD_CFIFO_MODE_TRANSMIT == (uint32_t) p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.mode,
            /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
            FSP_ERR_INVALID_ARGUMENT);                                                           /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
    }

    if (CAN_BUFFER_TYPE_TX_SINGLE == buffer_type)
    {
        FSP_ERROR_RETURN((buffer_num >= p_ctrl->txmb_range_min) && (buffer_num < p_ctrl->txmb_range_max), /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                         FSP_ERR_INVALID_ARGUMENT);                                                       /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

 #ifndef BSP_FEATURE_CANFD_NOT_CHECK_TXMB_RANGE

        /* Check TXMB in range 0-15 and 32-47*/
        FSP_ERROR_RETURN((0U != CANFD_PRV_VALID_TXMB_RANGE(buffer_num)) || /* PRQA S 3432 , 3469 # DP_MSR_R-20.7_UC1 , Advisory D-4.9 */
                         (buffer_num >= CANFD_PRV_TXMB_CHANNEL_OFFSET),    /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                         FSP_ERR_INVALID_ARGUMENT);                        /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
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
    canfd_extended_cfg_t const * p_extend_cfg = (canfd_extended_cfg_t *) p_cfg->p_extend;                             /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    canfd_global_cfg_t const   * p_global_cfg = p_extend_cfg->p_global_cfg;
    can_buffer_type_t            buffer_type  = (can_buffer_type_t) ((uint32_t) buffer & CANFD_PRV_BUFFER_TYPE_MASK); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */
    uint32_t buffer_num = (uint32_t) buffer & CANFD_PRV_BUFFER_NUM_MASK;
    uint32_t channel_index;

    FSP_ERROR_RETURN(((CAN_BUFFER_TYPE_RX_FIFO == buffer_type) ||                                                     /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                      (CAN_BUFFER_TYPE_RX_SINGLE == buffer_type)),
                     FSP_ERR_INVALID_ARGUMENT);                                                                       /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    if (CAN_BUFFER_TYPE_RX_SINGLE == buffer_type)
    {
        FSP_ERROR_RETURN(buffer_num < CANFD_PRV_RXMB_MAX(p_global_cfg->unit), FSP_ERR_UNSUPPORTED);                   /* PRQA S 3432 , 3469 # DP_MSR_R-20.7_UC1 , Advisory D-4.9 */
    }

    if (CAN_BUFFER_TYPE_RX_FIFO == buffer_type)
    {
        FSP_ERROR_RETURN(buffer_num < (BSP_FEATURE_CANFD_NUM_RXFIFO + BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL), /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                         FSP_ERR_UNSUPPORTED);                                                              /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

        if (buffer_num < CANFD_PRV_CFIFO_OFFSET)
        {
            /* Check Rx FIFO is enabled */
            FSP_ERROR_RETURN(0U != p_global_cfg->rx_fifo_config[buffer_num].rx_fifo_cfg_b.RFE,
                             FSP_ERR_INVALID_ARGUMENT); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        }
        else if (buffer_num < (CANFD_PRV_CFIFO_OFFSET + BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL))
        {
            /*Take index for channel when device have more than one unit*/
            channel_index = p_cfg->channel - (CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit);

            uint32_t cfifo_index = (buffer_num - CANFD_PRV_CFIFO_OFFSET) +
                                   (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);

            FSP_ERROR_RETURN((buffer_num - CANFD_PRV_CFIFO_OFFSET) < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL, /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                             FSP_ERR_INVALID_ARGUMENT);                                                   /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            /* Check common FIFO is enabled and operation in receive mode*/
            FSP_ERROR_RETURN(0U != p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable,      /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                             FSP_ERR_INVALID_ARGUMENT);                                                   /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
            FSP_ERROR_RETURN((uint32_t) CANFD_CFIFO_MODE_RECEIVE ==
                             (uint32_t) p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.mode,
                                                                                                          /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
                             FSP_ERR_INVALID_ARGUMENT);                                                   /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
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
static inline bool r_canfd_cfifo_same_link_check (canfd_cfifo_cfg_t cfifo_cfg_0, canfd_cfifo_cfg_t cfifo_cfg_1) /* PRQA S 0759 # Advisory R-19.2 */
{
    return !((0U != cfifo_cfg_0.cfifo_cfg_b.enable) &&
             (0U != cfifo_cfg_1.cfifo_cfg_b.enable) &&
             ((uint32_t) CANFD_CFIFO_MODE_TRANSMIT == (uint32_t) cfifo_cfg_0.cfifo_cfg_b.mode) &&
             ((uint32_t) CANFD_CFIFO_MODE_TRANSMIT == (uint32_t) cfifo_cfg_1.cfifo_cfg_b.mode) &&
             (cfifo_cfg_0.cfifo_cfg_b.txmb_link == cfifo_cfg_1.cfifo_cfg_b.txmb_link));
}

/*******************************************************************************************************************//**
 * Check common fifo transmit buffer link doesn't link to buffer in transmit queue
 *
 * @param[in]  p_cfg       canfd config
 **********************************************************************************************************************/
static inline bool r_canfd_cfifo_link_queue_check (const can_cfg_t * p_cfg)
{
    canfd_extended_cfg_t const * p_extend_cfg = (canfd_extended_cfg_t *) p_cfg->p_extend;          /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    canfd_global_cfg_t const   * p_global_cfg = (canfd_global_cfg_t *) p_extend_cfg->p_global_cfg; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /*Take index for channel when device have more than one unit*/
    uint8_t channel_index = (uint8_t) (p_cfg->channel - (CANFD_MAX_CHANNEL_PER_UNIT * p_global_cfg->unit));

    /* Only check COM FIFO link Queue for TXQ 2 and TXQ 3
     * TXQ 0 and 1 only uses TXMB 0-15, TXQ 2 and 3 uses TXMB 32-47
     * so cfifo only use TXMB 32 to TXMB 47  */
    if (0U != p_extend_cfg->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_2].tx_queue_cfg_b.enable)
    {
        /* Count the buffers that is used by TXQ 2  */
        uint8_t txmb_limit_txq2 = (uint8_t) (CANFD_PRV_TX_QUEUE_2_WINDOW +
                                             (p_extend_cfg->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_2].tx_queue_cfg_b.depth));
        for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
        {
            uint32_t cfifo_index = cfifo + (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);

            /* Check COM FIFO is enable and is gateway or transmit, if txmb link of COM FIFO
             * is used for TXQ*/
            if ((0U != p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable) &&
                ((uint32_t) CANFD_CFIFO_MODE_RECEIVE !=
                 (uint32_t) p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.mode) &&
                ((p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.txmb_link + CANFD_THIRTY_TWO) <=
                 txmb_limit_txq2))
            {
                return false;
            }
        }
    }

    if (0U != p_extend_cfg->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_3].tx_queue_cfg_b.enable)
    {
        uint8_t txmb_limit_txq3 = (uint8_t) (CANFD_PRV_TXMB_HIGH_BUFFER_NUM -
                                             p_extend_cfg->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_3].tx_queue_cfg_b.depth);
        for (uint32_t cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
        {
            /* Check COM FIFO is enable and is gateway or transmit, if txmb link of COM FIFO
             * is used for TXQ*/
            uint32_t cfifo_index = cfifo + (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);
            if ((0U != p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.enable) &&
                ((uint32_t) CANFD_CFIFO_MODE_RECEIVE !=
                 (uint32_t) p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.mode) &&
                ((p_global_cfg->common_fifo_config[cfifo_index].cfifo_cfg_b.txmb_link + CANFD_THIRTY_TWO) >
                 txmb_limit_txq3))
            {
                return false;
            }
        }
    }

    return true;
}

#endif

/*******************************************************************************************************************//**
 * Read from a Message Buffer or FIFO.
 *
 * @param[in]     channel            Channel of Channel Common Buffer
 * @param[in]     buffer_type        Type of buffer to read
 * @param[in]     buffer_index       Index of buffer to read from (MBs 0-31, FIFOs 0-7, Common FIFO 0-2)
 * @param[out]    p_frame            Pointer to store the read frame
 * @param[in]     p_canfd_reg        Base address of the CANFD register block
 **********************************************************************************************************************/
static fsp_err_t r_canfd_mb_read (uint32_t                 channel,
                                  can_buffer_type_t        buffer_type,
                                  uint32_t                 buffer_index,
                                  can_frame_t * const      p_frame,
                                  volatile R_CANFD0_Type * p_canfd_reg)
{
    uint32_t buffer_reg  = 0;
    uint32_t buffer_pos  = 0;
    uint32_t frame_id    = 0;
    uint32_t fdsts       = 0;
    uint32_t cfifo_index = 0;

    /* Get pointer to message buffer (FIFOs use the same buffer structure) */
    volatile R_CANFD0_CFDRM_Type const * p_mb_regs = NULL;

    if (CAN_BUFFER_TYPE_RX_FIFO == buffer_type)
    {
        if (buffer_index < CANFD_PRV_CFIFO_OFFSET)
        {
            /* Get status of selected RX FIFO */
            FSP_ERROR_RETURN(0U == (p_canfd_reg->CFDFESTS & ((uint32_t) 1U << buffer_index)), FSP_ERR_BUFFER_EMPTY); /* PRQA S 3432, 0662 # DP_MSR_R-20.7_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Get RX FIFO buffer address*/
            p_mb_regs = (volatile R_CANFD0_CFDRM_Type *) &(p_canfd_reg->CFDRF[buffer_index]);                        /* PRQA S 0310, 0662 # DP_MSR_R-11.3_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else if (buffer_index < (CANFD_PRV_CFIFO_OFFSET + BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL))
        {
            /* Get status of selected common FIFO */
            cfifo_index = buffer_index + (channel * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL) - CANFD_PRV_CFIFO_OFFSET;

            FSP_ERROR_RETURN(0U == p_canfd_reg->CFDCFSTS_b[cfifo_index].CFEMP, FSP_ERR_BUFFER_EMPTY); /* PRQA S 3432, 0662 # DP_MSR_R-20.7_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Get CFIFO buffer address */
            p_mb_regs = (volatile R_CANFD0_CFDRM_Type *) &(p_canfd_reg->CFDCF[cfifo_index]);          /* PRQA S 0310, 0662 # DP_MSR_R-11.3_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else
        {
            /* Selected buffer not available. Do nothing */
        }
    }
    else
    {
        /*Get status of select RXMB */
        buffer_reg = buffer_index >> 5U;                                                                          // Divide by 32
        buffer_pos = buffer_index & (((uint32_t) 1U << 5U) - 1U);                                                 // Get remainder of 32.
        FSP_ERROR_RETURN(0U != (p_canfd_reg->CFDRMND[buffer_reg] & ((uint32_t) 1U << buffer_pos)),                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                         FSP_ERR_BUFFER_EMPTY);                                                                   /* PRQA S 3432, 0662 # DP_MSR_R-20.7_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Get RXMB buffer address */
        p_mb_regs = &(p_canfd_reg->CFDRM[buffer_index]);                                                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Get frame data. */
    FSP_ASSERT(NULL != p_mb_regs);                                                                                /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    frame_id = p_mb_regs->ID;                                                                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Get the frame type */
    p_frame->type = (can_frame_type_t) ((frame_id & R_CANFD0_CFDRM_ID_RMRTR_Msk) >> R_CANFD0_CFDRM_ID_RMRTR_Pos); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */

    /* Get frame options */
    fdsts            = p_mb_regs->FDSTS;                                                                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_frame->options = CAN_FRAME_OPTIONS_NONE;

    /* Get frame format */
    if (0U != (fdsts & R_CANFD0_CFDRM_FDSTS_RMFDF_Msk))
    {
        /* FD Frame */
        p_frame->format = CAN_FRAME_FORMAT_FD;
        if (0U != (fdsts & R_CANFD0_CFDRM_FDSTS_RMBRS_Msk))
        {
            /* Get FD BRS bit */
            p_frame->options = (can_frames_options_t) ((uint32_t) p_frame->options | (uint32_t) CAN_FRAME_OPTIONS_BRS); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */
        }

        if (0U != (fdsts & R_CANFD0_CFDRM_FDSTS_RMESI_Msk))
        {
            /* Get FD ESI bit */
            p_frame->options = (can_frames_options_t) ((uint32_t) p_frame->options | (uint32_t) CAN_FRAME_OPTIONS_ESI); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */
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
    p_frame->id_mode = (can_id_mode_t) (frame_id >> R_CANFD0_CFDRM_ID_RMIDE_Pos);             /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */

    /* Get the frame data length code */
    p_frame->data_length_code = dlc_to_bytes[p_mb_regs->PTR >> R_CANFD0_CFDRM_PTR_RMDLC_Pos]; /* PRQA S 0662, 1710 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.5 */

    /* Get the frame label */
    p_frame->labels = p_mb_regs->FDSTS_b.RMPTR;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Get the frame time stamp */
    p_frame->timestamp = p_mb_regs->PTR_b.RMTS;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Copy data to frame */
    uint32_t                 length = p_frame->data_length_code;
    volatile uint8_t       * p_dest = p_frame->data;
    volatile uint8_t const * p_src  = (uint8_t const volatile *) p_mb_regs->DF; /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    while (length > 0U)
    {
        *p_dest = *p_src;
        p_dest++;                                                               /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */
        p_src++;                                                                /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */
        length--;
    }

    if (CAN_BUFFER_TYPE_RX_FIFO == buffer_type)
    {
        if (buffer_index < CANFD_PRV_CFIFO_OFFSET)
        {
            /* Increment RX FIFO pointer */
            p_canfd_reg->CFDRFPCTR[buffer_index] = CANFD_PRV_UPDATE_POINTER_CMD; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
        else
        {
            /* Increment Common FIFO pointer */
            p_canfd_reg->CFDCFPCTR[cfifo_index] = CANFD_PRV_UPDATE_POINTER_CMD; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
    }
    else
    {
        /* Clear RXMB New Data bit */
        p_canfd_reg->CFDRMND[buffer_reg] &= ~((uint32_t) 1U << buffer_pos); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Wrtie to a Message Buffer.
 *
 * @param[in]     channel            Channel index of CANFD Unit
 * @param[out]    buffer_type        Type of buffer to write
 * @param[in]     buffer_index       Index of buffer to write
 * @param[out]    p_frame            Pointer to CAN frame
 * @param[in]     p_extend           Pointer to extend configuration of CANFD
 * @param[in]     p_canfd_reg        Base address of the CANFD register block
 **********************************************************************************************************************/
static fsp_err_t r_canfd_mb_write (uint32_t                     channel,
                                   can_buffer_type_t            buffer_type,
                                   uint32_t                     buffer_index,
                                   can_frame_t const * const    p_frame,
                                   canfd_extended_cfg_t const * p_extend,
                                   volatile R_CANFD0_Type     * p_canfd_reg)
{
    volatile R_CANFD0_CFDTM_Type * p_tx_buffer = NULL;
    uint32_t txq_channel  = channel;
    uint32_t txq_buffer   = buffer_index;
    uint32_t queue_window = 0U;
    uint32_t fdctr        = 0;         /* PRQA S 1710 # Advisory D-4.5 */
    uint8_t  can_dlc      = 0;
    uint8_t  can_len      = 0;

    if (CAN_BUFFER_TYPE_TX_SINGLE == buffer_type)
    {
        buffer_index += (channel * CANFD_PRV_TXMB_CHANNEL_OFFSET);                 /* PRQA S 1338 # Advisory R-17.8 */

        /* The result of the previous transmission is only cleared by the transmit interrupt, so take it
         * into account only when that interrupt is enabled for this buffer */
        uint32_t busy_mask = CANFD_PRV_TXMB_BUSY_MASK;
        if (0U != (p_canfd_reg->CFDTMIEC[CANFD_PRV_TXMB_IEC_INDEX(buffer_index)] & /* PRQA S 3469, 0662 # Advisory D-4.9 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                   CANFD_PRV_TXMB_IEC_BIT(buffer_index)))                          /* PRQA S 3469 # Advisory D-4.9 */
        {
            busy_mask |= CANFD_PRV_TXMB_RESULT_MASK;
        }

        /* Read the buffer status once to get a consistent view of all busy indications */
        uint32_t txmb_status = (uint32_t) p_canfd_reg->CFDTMSTS[buffer_index];             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Ensure buffer is ready: no transmission ongoing and no transmission or abort request pending */
        FSP_ERROR_RETURN(0U == (txmb_status & busy_mask), FSP_ERR_CAN_TRANSMIT_NOT_READY); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

        p_tx_buffer = &(p_canfd_reg->CFDTM[buffer_index]);                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else if (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type)
    {
        /* Get the Transmit queue buffer index and channel */
        if (buffer_index >= CANFD_PRV_TXQ_CH_OFFSET)
        {
            txq_buffer  = buffer_index - CANFD_PRV_TXQ_CH_OFFSET;
            txq_channel = channel + 1U;
        }

        /* Ensure buffer is ready */
        FSP_ERROR_RETURN(0U == p_canfd_reg->CFDTXQ[txq_buffer].STS_b[txq_channel].TXQFLL,                    /* PRQA S 3432, 0662 # DP_MSR_R-20.7_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                         FSP_ERR_CAN_TRANSMIT_NOT_READY);                                                    /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

        /* Write frame to transmit queue windows */
        queue_window = g_queue_window[txq_buffer] + (txq_channel * CANFD_PRV_TXMB_CHANNEL_OFFSET);
        p_tx_buffer  = &(p_canfd_reg->CFDTM[queue_window]);                                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        buffer_index += (channel * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);                                     /* PRQA S 1338 # Advisory R-17.8 */

        /* Ensure buffer is ready */
        FSP_ERROR_RETURN(0U == p_canfd_reg->CFDCFSTS_b[buffer_index].CFFLL, FSP_ERR_CAN_TRANSMIT_NOT_READY); /* PRQA S 3432, 0662 # DP_MSR_R-20.7_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_tx_buffer = (volatile R_CANFD0_CFDTM_Type *) &(p_canfd_reg->CFDCF[buffer_index]);                  /* PRQA S 0310, 0662 # DP_MSR_R-11.3_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Set TX message buffer registers */
    p_tx_buffer->ID = p_frame->id | ((uint32_t) p_frame->type << R_CANFD0_CFDTM_ID_TMRTR_Pos) |              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                      ((uint32_t) p_frame->id_mode << R_CANFD0_CFDTM_ID_TMIDE_Pos);

    /*Allow to store frame into TX History Entry*/
    if (0U != p_extend->tx_history_cfg.enable)
    {
        p_tx_buffer->ID_b.THLEN = 1;   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Convert bytes to dlc */
    can_dlc = r_canfd_bytes_to_dlc((uint8_t) p_frame->data_length_code);

    p_tx_buffer->PTR = (uint32_t) can_dlc << R_CANFD0_CFDTM_PTR_TMDLC_Pos; /* PRQA S 0662, 1710 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.5 */

    /* Set FD bits (ESI, BRS and FDF) */
    if (CAN_FRAME_FORMAT_FD == p_frame->format)
    {
        /* Set FDF bit */
        fdctr |= R_CANFD0_CFDTM_FDCTR_TMFDF_Msk; /* PRQA S 1710 # Advisory D-4.5 */

        if (0U != ((uint32_t) p_frame->options & (uint32_t) CAN_FRAME_OPTIONS_BRS))
        {
            /* Set BRS bit */
            fdctr |= R_CANFD0_CFDTM_FDCTR_TMBRS_Msk; /* PRQA S 1710 # Advisory D-4.5 */
        }

        if (0U != ((uint32_t) p_frame->options & (uint32_t) CAN_FRAME_OPTIONS_ESI))
        {
            /* Set ESI */
            fdctr |= R_CANFD0_CFDTM_FDCTR_TMESI_Msk; /* PRQA S 1710 # Advisory D-4.5 */
        }
    }

    /* Set buffer control register */
    p_tx_buffer->FDCTR = fdctr | (p_frame->labels << R_CANFD0_CFDTM_FDCTR_TMPTR_Pos); /* PRQA S 0662, 1710 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.5 */

    /* Copy data to register buffer */
    uint32_t                 length = p_frame->data_length_code;
    volatile uint8_t       * p_dest = (uint8_t *) p_tx_buffer->DF;                    /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    volatile uint8_t const * p_src  = p_frame->data;
    while (length > 0U)
    {
        *p_dest = *p_src;
        p_dest++;                      /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */
        p_src++;                       /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */
        length--;
    }

    /* Convert dlc to actual bytes */
    can_len = dlc_to_bytes[can_dlc];

    if (can_len > p_frame->data_length_code)
    {
        for (uint8_t pad_idx = (uint8_t) p_frame->data_length_code; pad_idx < can_len; pad_idx++)
        {
            *p_dest = p_extend->tx_padding_data;
            p_dest++;                  /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */
        }
    }

    /* Request transmission */
    if ((CAN_BUFFER_TYPE_TX_SINGLE == buffer_type))
    {
        p_canfd_reg->CFDTMC[buffer_index] = 1;                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else if (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type)
    {
        p_canfd_reg->CFDTXQ[txq_buffer].PCTR[txq_channel] = CANFD_PRV_UPDATE_POINTER_CMD; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        p_canfd_reg->CFDCFPCTR[buffer_index] = CANFD_PRV_UPDATE_POINTER_CMD;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to CAN instance control block
 * @param[in]     p_args     Pointer to arguments on stack
 **********************************************************************************************************************/
static void r_canfd_call_callback (canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args)
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

    if (NULL != p_ctrl->p_callback)
    {
        p_ctrl->p_callback(p_args_memory);
    }

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
    uint8_t channel_index;
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    volatile R_CANFD0_Type     * p_canfd_reg;

    /* Get bit 7 from operation_mode to determine if this is a global mode change request */
    bool global_mode = (0U != ((uint32_t) operation_mode >> 7U));
    operation_mode = (can_operation_mode_t) ((uint32_t) operation_mode & CANFD_PRV_OPERATION_MODE_MASK); /* PRQA S 4394, 4342, 1338 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , Advisory R-17.8 */

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;                                               /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend;                                             /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    if (global_mode)
    {
        uint32_t cfdgctr = p_canfd_reg->CFDGCTR;                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        r_canfd_mode_ctr_set(&p_canfd_reg->CFDGCTR, operation_mode); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* If CANFD is transitioning out of Reset the FIFOs need to be enabled. */
        if ((0U != (cfdgctr & R_CANFD0_CFDGSTS_GRSTSTS_Msk)) &&
            (0U == ((uint32_t) operation_mode & (uint32_t) CAN_OPERATION_MODE_RESET)))
        {
            /* Enable RX FIFOs */
            for (uint32_t rx_fifo = 0; rx_fifo < CANFD_PRV_RX_FIFO_MAX; rx_fifo++)
            {
                p_canfd_reg->CFDRFCC[rx_fifo] = p_global_cfg->rx_fifo_config[rx_fifo].rx_fifo_cfg; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
        }
    }
    else
    {
        uint32_t cfdcnctr = p_canfd_reg->CFDC[channel_index].CTR; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        if ((((cfdcnctr & R_CANFD0_CFDC_CTR_CSLPR_Msk) != 0U) &&
             (((uint32_t) CAN_OPERATION_MODE_RESET & (uint32_t) operation_mode) == 0U)) ||
            (((cfdcnctr & CANFD_PRV_CTR_RESET_BIT) == 0U) && (CAN_OPERATION_MODE_SLEEP == operation_mode)))
        {
            /* Transition channel to Reset if a transition to/from Sleep is requested (see Section "Channel
             * Modes" in the RH850/U2B microcontroller User's Manual for details) */
            r_canfd_mode_ctr_set(&p_canfd_reg->CFDC[channel_index].CTR, CAN_OPERATION_MODE_RESET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Request transition to selected mode */
        r_canfd_mode_ctr_set(&p_canfd_reg->CFDC[channel_index].CTR, operation_mode);               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    p_ctrl->operation_mode =
        (can_operation_mode_t) (p_canfd_reg->CFDC[channel_index].CTR & CANFD_PRV_CTR_MODE_MASK);   /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
}

/*******************************************************************************************************************//**
 * Sets the provided CTR register to the requested mode and waits for the associated STS register to reflect the change
 * @param[in]  p_ctr_reg            - pointer to control register
 * @param[in]  operation_mode       - requested mode (not including global bits)
 **********************************************************************************************************************/
static void r_canfd_mode_ctr_set (volatile uint32_t * p_ctr_reg, can_operation_mode_t operation_mode)
{
    volatile uint32_t const * p_sts_reg = p_ctr_reg + 1; /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */

    /* See definitions for CFDCnCTR, CFDCnSTS, CFDGCTR and CFDGSTS in the RH850/U2B microcontroller User's Manual */
    *p_ctr_reg = (*p_ctr_reg & ~CANFD_PRV_CTR_MODE_MASK) | (uint32_t) operation_mode;
    FSP_HARDWARE_REGISTER_WAIT((*p_sts_reg & CANFD_PRV_CTR_MODE_MASK), (uint32_t) operation_mode);
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
    uint8_t base = (uint8_t) (unit * CANFD_PRV_CHANNELS_OFFSET);

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
    if (0U != (uint32_t) ram_field)
    {
#ifdef BSP_FEATURE_CANFD_PN_MODE

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
static uint32_t r_canfd_ram_caculate_limit_byte (canfd_global_cfg_t const * p_global_cfg)
{
    uint32_t          max_byte_per_unit    = 0;
    uint32_t          actual_byte_per_unit = 0;
    canfd_ramtest_t * p_infor_ram          = p_global_cfg->p_ram_infor;

    uint32_t no_channel = r_canfd_caculate_no_channel(p_global_cfg->unit);

    /*Check current RAM test field and caculate max ram per unit*/
    if (0U != (uint32_t) p_infor_ram->ram_field)
    {
#ifdef BSP_FEATURE_CANFD_PN_MODE

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
static void r_canfd_set_ram_data (canfd_global_cfg_t const * p_global_cfg, canfd_ramtest_data_t data_ram)
{
    volatile R_CANFD0_Type * p_canfd_reg   = p_global_cfg->p_canfd_reg;
    uint32_t                 ram_init_data = 0;
    uint32_t                 entry_access  = 0;
    canfd_ramtest_t const  * infor_ram     = p_global_cfg->p_ram_infor;

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
    if (0U != infor_ram->unaccessed_ram_entry)
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
        p_canfd_reg->CFDRPGACC[ram_entry] = ram_init_data; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
}

/*******************************************************************************************************************//**
 * Execute CANFD RAM check algorithm using alternating data patterns
 * @param[in]  p_global_cfg       Pointer to global configuration
 **********************************************************************************************************************/
static fsp_err_t r_canfd_ram_check_algorithm (canfd_global_cfg_t const * p_global_cfg)
{
    volatile R_CANFD0_Type * p_canfd_reg  = p_global_cfg->p_canfd_reg;
    volatile uint32_t      * p_gacc_reg   = &p_canfd_reg->CFDRPGACC[0]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint32_t                 test_data    = CANFD_RAM_TEST_DATA;
    uint32_t                 entry_access = 0;
    canfd_ramtest_t const  * infor_ram    = p_global_cfg->p_ram_infor;

    if (0U != infor_ram->unaccessed_ram_entry)
    {
        /* Caculate number of pages which can be access */
        entry_access = CANFD_RAM_PAGE_SIZE_ACCESS_REG - infor_ram->unaccessed_ram_entry;
    }
    else
    {
        entry_access = CANFD_RAM_PAGE_SIZE_ACCESS_REG;
    }

    /* Write 0 and 1 in consecutive locations */
    for (uint8_t test_num = 0; test_num < 2U; test_num++)
    {
        /* Write 0 and 1 in consecutive locations */
        for (uint8_t size_id = 0; size_id < entry_access; size_id++)
        {
            p_gacc_reg[size_id] = test_data;
            test_data           = ~test_data;
        }

        /* Read 0 and 1 in consecutive locations */
        for (uint8_t size_id = 0; size_id < entry_access; size_id++)
        {
            if (p_gacc_reg[size_id] != test_data)
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
static fsp_err_t r_canfd_ram_walkpath_algorithm (canfd_global_cfg_t const * p_global_cfg,
                                                 canfd_ramtest_walk_type_t  walk_type)
{
    volatile R_CANFD0_Type * p_canfd_reg   = p_global_cfg->p_canfd_reg;
    canfd_ramtest_data_t     ram_fill_data = CAN_RAMTEST_DATA_0;
    uint32_t                 ram_base_data = 0;
    uint32_t                 total_bit     = 0;
    uint32_t                 base_entry    = 0;
    uint32_t                 base_bit      = 0;
    volatile uint32_t      * p_gacc_reg;

    uint32_t                entry_access;
    canfd_ramtest_t const * infor_ram = p_global_cfg->p_ram_infor;

    if (0U != infor_ram->unaccessed_ram_entry)
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

        base_bit   = (uint8_t) (bit_num & (sizeof(uint32_t) - 1U));
        p_gacc_reg = &p_canfd_reg->CFDRPGACC[0]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Invert the base bit */
        p_gacc_reg[base_entry] ^= (uint32_t) 1U << base_bit;

        /* Verify that only the target bit is toggled */
        for (uint32_t entry_num = 0; entry_num < entry_access; entry_num++)
        {
            if (entry_num == base_entry)
            {
                if (p_gacc_reg[base_entry] != (ram_base_data ^ ((uint32_t) 1 << base_bit)))
                {
                    return FSP_ERR_INVALID_DATA;
                }
            }
            else if (p_gacc_reg[entry_num] != ram_base_data)
            {
                return FSP_ERR_INVALID_DATA;
            }
            else
            {
                /* Do nothing */
            }
        }

        /* Restore the original RAM content */
        p_gacc_reg[base_entry] ^= (uint32_t) 1U << base_bit;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Handle transmit buffer interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 * @param[in]  event        - Event to handle, CAN_EVENT_TX_COMPLETE and CAN_EVENT_TX_ABORTED only.
 **********************************************************************************************************************/
static void r_canfd_handle_txmb (canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args, can_event_t event)
{
    uint32_t txmb;
    uint8_t  channel_index;
    volatile uint32_t const * p_cfdtm_sts;
    uint32_t                     cfdtm_sts_index;
    uint32_t                     lend_txmb;
    volatile R_CANFD0_Type     * p_canfd_reg;
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index   = p_ctrl->channel_index;
    cfdtm_sts_index = (uint32_t) channel_index << 1U;

    /* Get number of lend buffer on this channel */
    lend_txmb = CANFD_PRV_LEND_BUFF_GET(p_canfd_reg, channel_index); /* PRQA S 0310, 0312, 0488, 0662, 3469 # DP_MSR_R-11.3_UC1 , DP_MSR_R-11.8_UC1 , Advisory R-18.4 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.9 */

    /* Get relevant TX status register bank */
    if (event == CAN_EVENT_TX_COMPLETE)
    {
        p_cfdtm_sts   = (volatile uint32_t const *) &p_canfd_reg->CFDTMTCSTS[cfdtm_sts_index]; /* PRQA S 0310, 0662 # DP_MSR_R-11.3_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_args->event = CAN_EVENT_TX_COMPLETE;
    }
    else
    {
        p_cfdtm_sts   = (volatile uint32_t const *) &p_canfd_reg->CFDTMTASTS[cfdtm_sts_index]; /* PRQA S 0310, 0662 # DP_MSR_R-11.3_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_args->event = CAN_EVENT_TX_ABORTED;
    }

    /* Calculate lowest TXMB with the specified event */
    txmb = (uint32_t) SCH1R(*p_cfdtm_sts & (~CANFD_PRV_FILL_1(p_ctrl->txmb_range_min))) - 1U; /* PRQA S 3469, 4394 # Advisory D-4.9 , DP_MSR_R-10.8_UC1 */

    /* Check whether the TXMB is out of range */
    if (txmb == CANFD_PRV_TXMB_OUT_RANGE)
    {
        /* Check the next TXMB status register within the same channel.
         * If no request transmit buffer exists, jump to use the lent transmit buffer register. */
        if (0U == *(p_cfdtm_sts + 1))  /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */
        {
            /* Use lent TXMBs only if lending is enabled */
            if (lend_txmb != 0U)
            {
                txmb = (uint32_t) SCH1R(*(p_cfdtm_sts + 2)) - 1U + CANFD_PRV_TXMB_CHANNEL_OFFSET; /* PRQA S 3469, 0488, 0489 # Advisory D-4.9 , Advisory R-18.4 , Advisory R-18.4 */
            }
        }
        else
        {
            /* TXMB found in the next status register within the same channel */
            txmb = (uint32_t) SCH1R(*(p_cfdtm_sts + 1)) - 1U + CANFD_PRV_TXMB_OFFSET_IN_CHANNEL; /* PRQA S 3469, 0488, 0489 # Advisory D-4.9 , Advisory R-18.4 , Advisory R-18.4 */
        }
    }

    /* Only handle if the txmb is belonged in this channel */
    if ((txmb >= p_ctrl->txmb_range_min) && (txmb < p_ctrl->txmb_range_max))
    {
        /* Clear TX complete/abort flags */
        p_canfd_reg->CFDTMSTS_b[txmb + (CANFD_PRV_TXMB_CHANNEL_OFFSET * (uint32_t) channel_index)].TMTRF = 0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Dummy read to ensure that interrupt event is cleared. */
        volatile uint32_t dummy =
            p_canfd_reg->CFDTMSTS[txmb + (CANFD_PRV_TXMB_CHANNEL_OFFSET * (uint32_t) channel_index)];         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        FSP_PARAMETER_NOT_USED(dummy);

        /* Set the callback arguments */
        p_args->buffer = (can_buffer_t) (txmb);                                                               /* PRQA S 4342, 0662 # Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Call user callback */
        r_canfd_call_callback(p_ctrl, p_args);
    }
}

/*******************************************************************************************************************//**
 * Handle transmit queue one frame reception/transmission interrupt.
 * @param[in]  p_ctrl               Pointer to CANFD control block
 * @param[in]  p_args               Pointer to callback argument
 * @param[in]  p_canfd_reg          Base address of the CANFD register block
 * @param[in]  txq_one_frame_flag   Transmit Queue one frame reception/transmission flag
 **********************************************************************************************************************/
static void r_canfd_handle_txqueue_one_frame (canfd_instance_ctrl_t const * p_ctrl,
                                              can_callback_args_t         * p_args,
                                              volatile R_CANFD0_Type      * p_canfd_reg,
                                              uint32_t                      txq_one_frame_flag)
{
    volatile uint32_t reg_val = 0;
    uint32_t          queue_bit_val;
    uint32_t          queue_num;
    uint32_t          queue_channel;
    uint32_t          queue_index;

    if (txq_one_frame_flag == R_CANFD0_CFDGRINTSTS_QOFRIF_Msk)
    {
        /* Check status of Transmit Queue one frame reception interrupt */
        queue_bit_val = p_canfd_reg->CFDTXQOFRISTS >> (p_ctrl->channel_index * CANFD_PRV_TXQ_CH_OFFSET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else if (txq_one_frame_flag == R_CANFD0_CFDGTINTSTS32_TQOFIF0_Msk)
    {
        /* Check status of Transmit Queue one frame transmission interrupt */
        queue_bit_val = p_canfd_reg->CFDTXQOFTISTS >> (p_ctrl->channel_index * CANFD_PRV_TXQ_CH_OFFSET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        /* Do nothing */
        queue_bit_val = 0U;
    }

    while (queue_bit_val != 0U)
    {
        /*Check queue buffer value*/
        queue_num = (uint32_t) SCH1R(queue_bit_val) - 1U; /* PRQA S 3469 # Advisory D-4.9 */

        /* Only handle if the transmit queue is belonged in this channel */
        if ((queue_num >= p_ctrl->txqueue_range_min) && (queue_num < p_ctrl->txqueue_range_max))
        {
            /* Get index and channel of tx queue */
            if (queue_num < CANFD_PRV_TXQ_CH_OFFSET)
            {
                /* Transmit queue belong the channel */
                queue_index   = queue_num;
                queue_channel = p_ctrl->channel_index;
            }
            else
            {
                /* Transmit queue lend from the consecutive channel */
                queue_index   = queue_num - CANFD_PRV_TXQ_CH_OFFSET;
                queue_channel = (uint32_t) p_ctrl->channel_index + 1U;
            }

            /* Check Transmit Queue Event */
            reg_val = p_canfd_reg->CFDTXQ[queue_index].STS[queue_channel]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            if (txq_one_frame_flag == R_CANFD0_CFDGRINTSTS_QOFRIF_Msk)
            {
                if (R_CANFD0_CFDTXQ_STS_TXQOFRXIF_Msk == (reg_val & R_CANFD0_CFDTXQ_STS_TXQOFRXIF_Msk))
                {
                    p_args->event = CAN_EVENT_TX_QUEUE_ONE_FRAME_RX;
                }

                /* Get Transmit Queue Interrupt Flag */
                reg_val &= ~(R_CANFD0_CFDTXQ_STS_TXQOFRXIF_Msk);
            }
            else if (txq_one_frame_flag == R_CANFD0_CFDGTINTSTS32_TQOFIF0_Msk)
            {
                p_args->event = CAN_EVENT_TX_COMPLETE;

                /* Get Transmit Queue Interrupt Flag */
                reg_val &= ~(R_CANFD0_CFDTXQ_STS_TXQOFTXIF_Msk);
            }
            else
            {
                /* Do nothing */
            }

            p_args->buffer = (can_buffer_t) ((uint32_t) CAN_BUFFER_TYPE_TX_PRIORITY | queue_num); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Clear Transmit Queue Interrupt Flag */
            p_canfd_reg->CFDTXQ[queue_index].STS[queue_channel] = reg_val;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Call user callback */
            r_canfd_call_callback(p_ctrl, p_args);
        }

        queue_bit_val &= ~((uint32_t) 1U << queue_num);
    }
}

/*******************************************************************************************************************//**
 * Handle transmit queue interrupt
 * @param[in]  p_ctrl       Pointer to CANFD control block
 * @param[in]  p_args       Pointer to callback argument
 * @param[in]  p_canfd_reg  Base address of the CANFD register block
 **********************************************************************************************************************/
static void r_canfd_handle_txqueue (canfd_instance_ctrl_t const * p_ctrl,
                                    can_callback_args_t         * p_args,
                                    volatile R_CANFD0_Type      * p_canfd_reg)
{
    volatile uint32_t reg_val = 0;
    uint32_t          queue_bit_val;
    uint32_t          queue_num;
    uint32_t          queue_channel;
    uint32_t          queue_index;

    /* Check status of Transmit Queue interrupt */
    queue_bit_val = p_canfd_reg->CFDTXQISTS >> (p_ctrl->channel_index * CANFD_PRV_TXQ_CH_OFFSET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    while (queue_bit_val != 0U)
    {
        /*Check queue buffer value*/
        queue_num = (uint32_t) SCH1R(queue_bit_val) - 1U; /* PRQA S 3469 # Advisory D-4.9 */

        /* Only handle if the transmit queue is belonged in this channel */
        if ((queue_num >= p_ctrl->txqueue_range_min) && (queue_num < p_ctrl->txqueue_range_max))
        {
            /* Get index and channel of tx queue */
            if (queue_num < CANFD_PRV_TXQ_CH_OFFSET)
            {
                /* Transmit queue belong the channel */
                queue_index   = queue_num;
                queue_channel = p_ctrl->channel_index;
            }
            else
            {
                /* Transmit queue lend from the consecutive channel */
                queue_index   = queue_num - CANFD_PRV_TXQ_CH_OFFSET;
                queue_channel = (uint32_t) p_ctrl->channel_index + 1U;
            }

            /* Check Transmit Queue Event */
            reg_val = p_canfd_reg->CFDTXQ[queue_index].STS[queue_channel]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            if (R_CANFD0_CFDTXQ_STS_TXQEMP_Msk == (reg_val & R_CANFD0_CFDTXQ_STS_TXQEMP_Msk))
            {
                p_args->event = CAN_EVENT_TX_QUEUE_EMPTY;
            }
            else
            {
                p_args->event = CAN_EVENT_TX_COMPLETE;
            }

            p_args->buffer = (can_buffer_t) ((uint32_t) CAN_BUFFER_TYPE_TX_PRIORITY | queue_num); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Clear Transmit Queue Interrupt Flag */
            reg_val &= ~(R_CANFD0_CFDTXQ_STS_TXQTXIF_Msk);
            p_canfd_reg->CFDTXQ[queue_index].STS[queue_channel] = reg_val;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Call user callback */
            r_canfd_call_callback(p_ctrl, p_args);
        }

        queue_bit_val &= ~((uint32_t) 1U << queue_num);
    }
}

/*******************************************************************************************************************//**
 * Handle transmit common fifo interrupts
 * @param[in]  p_ctrl       Pointer to CANFD control block
 * @param[in]  p_extend     Pointer to extend configuration
 * @param[in]  p_args       Pointer to callback argument
 * @param[in]  p_canfd_reg  Base address of the CANFD register block
 **********************************************************************************************************************/
static void r_canfd_handle_txfifo (canfd_instance_ctrl_t const * p_ctrl,
                                   canfd_extended_cfg_t const  * p_extend,
                                   can_callback_args_t         * p_args,
                                   volatile R_CANFD0_Type      * p_canfd_reg)
{
    FSP_PARAMETER_NOT_USED(p_extend);

    /* Get zero-based buffer index for the specified CFIFO channel */
    uint32_t buffer =
        (uint32_t) SCH1R(p_canfd_reg->CFDCFTISTS >> (p_ctrl->channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)) - 1U; /* PRQA S 3469, 4394, 0662 # Advisory D-4.9 , DP_MSR_R-10.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint32_t buffer_index = buffer + ((uint32_t) p_ctrl->channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);

    if (0U != p_canfd_reg->CFDCFCC_b[buffer_index].CFTXIE)                                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /* Get Common FIFO Event */
        p_args->event =
            (p_canfd_reg->CFDCFSTS_b[buffer_index].CFEMP != 0U) ? CAN_EVENT_TX_FIFO_EMPTY :                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            CAN_EVENT_TX_COMPLETE;
        p_args->buffer = (can_buffer_t) (buffer | (uint32_t) CAN_BUFFER_TYPE_TX_FIFO);                                   /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Clear Common FIFO Transmit Interrupt Flag */
        p_canfd_reg->CFDCFSTS_b[buffer_index].CFTXIF = 0;                                                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Call user callback */
        r_canfd_call_callback(p_ctrl, p_args);
    }
}

/*******************************************************************************************************************//**
 * Handle transmit history interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 **********************************************************************************************************************/
static void r_canfd_handle_txhistory (canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args)
{
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    volatile R_CANFD0_Type     * p_canfd_reg;
    uint8_t  buffer_type;
    uint32_t buffer_num;
    uint8_t  channel_index;

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get pointer to transmit history register */
    volatile R_CANFD0_CFDTHL_Type const * p_txh_regs = NULL;

    /*Get label transmitted successfully*/
    p_txh_regs = (volatile R_CANFD0_CFDTHL_Type *) &(p_canfd_reg->CFDTHL[channel_index]); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    while (0U == p_canfd_reg->CFDTHLSTS_b[channel_index].THLEMP)                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /*Get correct label of AFL label*/
        p_args->label = p_txh_regs->ACC1_b.TID;                                           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Caculate buffer type
         *  b001: Flat TX Message Buffer
         *  b010: TX FIFO MB Number and GW FIFO MB Number
         *  b100: TX Queue MB Number.
         */
        buffer_type = p_txh_regs->ACC0_b.BT;                                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        buffer_num  = p_txh_regs->ACC0_b.BN;                                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        if ((uint8_t) CANFD_THL_BUFF_TYPE_TXQUEUE == buffer_type)
        {
            r_canfd_thl_handle_txqueue(p_ctrl, p_args, p_extend->p_global_cfg->p_canfd_reg, p_extend, buffer_num);
        }
        else if ((uint8_t) CANFD_THL_BUFF_TYPE_TXFIFO == buffer_type)
        {
            r_canfd_thl_handle_txfifo(p_ctrl, p_args, p_extend->p_global_cfg->p_canfd_reg, buffer_num);
        }
        else
        {
            r_canfd_thl_handle_txmb(p_ctrl, p_args, p_extend->p_global_cfg->p_canfd_reg, buffer_num);
        }

        /*Update to next TX History Entry Slot*/
        p_canfd_reg->CFDTHLPCTR[channel_index] = CANFD_PRV_UPDATE_POINTER_CMD; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

#ifdef BSP_FEATURE_CANFD_FFI_MODE

    /* Get the channel if FFI mode is used. */
    if (0U != p_canfd_reg->CFDGFFIMC_b.FFIEN)    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        p_args->channel = p_txh_regs->ACC0_b.CH; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
#endif

    /* Clear Interrupt Flag */
    p_canfd_reg->CFDTHLSTS_b[channel_index].THLIF = 0; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
}

/*******************************************************************************************************************//**
 * Converts bytes into a DLC value
 * @param[in]  bytes       Number of payload bytes
 **********************************************************************************************************************/
static uint8_t r_canfd_bytes_to_dlc (uint16_t bytes)
{
    /* Return dlc with bytes if the value ranges from 0 to 8 */
    if (bytes <= CANFD_PRV_DATA_LENGTH_CODE_8)
    {
        return (uint8_t) bytes;
    }

    /* Return dlc with bytes if the value ranges from 9 to 64 */
    for (uint8_t dlc = CANFD_PRV_DATA_LENGTH_CODE_9; dlc < sizeof(dlc_to_bytes); dlc++)
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
    uint8_t start_channel = (uint8_t) (unit * CANFD_PRV_CHANNELS_OFFSET);

    /* Calculate number of channels belonging to this unit */
    uint8_t no_channel = (uint8_t) r_canfd_caculate_no_channel(unit);

    for (uint8_t channel_idx = start_channel; channel_idx < (no_channel + start_channel); channel_idx++)
    {
        if (gp_ctrl[channel_idx] != NULL)
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
static inline void r_canfd_thl_handle_txfifo (canfd_instance_ctrl_t const * const p_ctrl,
                                              can_callback_args_t * const         p_args,
                                              volatile R_CANFD0_Type            * p_canfd_reg,
                                              uint32_t                            buffer_num)
{
    /* Get COMFIFO index */
    uint32_t cfifo = 0;
    for (cfifo = 0; cfifo < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL; cfifo++)
    {
        /* Available buffer for TXCOMFIFO is from 32 to 63, so CANFD_THIRTY_TWO is offset */
        if (buffer_num ==
            ((uint32_t) (p_canfd_reg->CFDCFCC_b[2U * p_ctrl->channel_index].CFTML) + (uint32_t) CANFD_THIRTY_TWO)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* Get Common FIFO Event */
            p_args->event =
                (p_canfd_reg->CFDCFSTS_b[cfifo + ((uint32_t) p_ctrl->channel_index *                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                  BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)].CFEMP !=
                 0U) ? CAN_EVENT_TX_FIFO_EMPTY :                                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                CAN_EVENT_TX_COMPLETE;                                                                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            p_args->buffer = (can_buffer_t) (cfifo | (uint32_t) CAN_BUFFER_TYPE_TX_FIFO);                          /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            break;
        }
    }

    /* Call user callback */
    r_canfd_call_callback(p_ctrl, p_args);

    /* Clear Common FIFO Transmit Interrupt Flag */
    /* PRQA S 0662 ++ # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_canfd_reg->CFDCFSTS_b[cfifo +
                            ((uint32_t) p_ctrl->channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)].CFTXIF = 0;

    /* PRQA S 0662 -- */
}

/*******************************************************************************************************************//**
 * Handle txmb for Transmit history
 **********************************************************************************************************************
 **********************************************************************************************************************/
static inline void r_canfd_thl_handle_txmb (canfd_instance_ctrl_t const * const p_ctrl,
                                            can_callback_args_t * const         p_args,
                                            volatile R_CANFD0_Type            * p_canfd_reg,
                                            uint32_t                            buffer_num)
{
    /* Default event: TX complete */
    p_args->event = CAN_EVENT_TX_COMPLETE;

    /* Only handle if TXMB belongs to this channel */
    if ((buffer_num >= p_ctrl->txmb_range_min) && (buffer_num < p_ctrl->txmb_range_max))
    {
        uint32_t buffer_index = buffer_num + (CANFD_PRV_TXMB_CHANNEL_OFFSET * (uint32_t) p_ctrl->channel_index);

        /* Clear TX complete/abort flag */
        p_canfd_reg->CFDTMSTS_b[buffer_index].TMTRF = 0;                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Dummy read to ensure the clear takes effect */
        volatile uint32_t dummy = p_canfd_reg->CFDTMSTS[buffer_index];                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        FSP_PARAMETER_NOT_USED(dummy);

        /* Callback args */
        p_args->buffer = (can_buffer_t) (buffer_num | (uint32_t) CAN_BUFFER_TYPE_TX_SINGLE); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* User callback */
        r_canfd_call_callback(p_ctrl, p_args);
    }
}

/*******************************************************************************************************************//**
 * Handle txqueue for Transmit history
 **********************************************************************************************************************
 **********************************************************************************************************************/
static inline void r_canfd_thl_handle_txqueue (canfd_instance_ctrl_t const * const p_ctrl,
                                               can_callback_args_t * const         p_args,
                                               volatile R_CANFD0_Type            * p_canfd_reg,
                                               canfd_extended_cfg_t const        * p_extend,
                                               uint32_t                            buffer_num)
{
    volatile uint32_t reg_val       = 0;
    uint32_t          queue_num     = 0;
    uint32_t          queue_channel = 0;
    uint32_t          queue_index   = 0;
    uint32_t          offset        = 0;

    if (buffer_num < (uint32_t) CANFD_THIRTY_TWO)
    {
        /* TX Queues 0 and 1 are available from TXMB0 to TXMB16 (on devices U2Bx, U2Cx)
         * and from TXMB0 to TXMB31 (on device U2Ax, except U2A6). */
        if (buffer_num <= (uint32_t) (p_extend->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_0].tx_queue_cfg_b.depth))
        {
            queue_num = CANFD_PRV_TXQ_INDEX_0;
        }
        else
        {
            queue_num = CANFD_PRV_TXQ_INDEX_1;
        }
    }
    else if ((buffer_num >= (uint32_t) CANFD_THIRTY_TWO) && (buffer_num < (uint32_t) CANFD_SIXTY_FOUR))
    {
        /* TX Queue 2, 3 are available from TXMB32 to TXMB47 (on devices U2Bx, U2Cx)
         * and from TXMB32 to TXMB63 (on device U2Ax, except U2A6), so CANFD_THIRTY_TWO is offset buffer  */
        offset = buffer_num - (uint32_t) CANFD_THIRTY_TWO;
        if (offset <= (uint32_t) (p_extend->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_3].tx_queue_cfg_b.depth))
        {
            queue_num = CANFD_PRV_TXQ_INDEX_2;
        }
        else
        {
            queue_num = CANFD_PRV_TXQ_INDEX_3;
        }
    }
    else
    {
        offset = buffer_num - (uint32_t) CANFD_SIXTY_FOUR;
        if (offset <= (uint32_t) (p_extend->tx_queue_cfg[CANFD_PRV_TXQ_INDEX_4].tx_queue_cfg_b.depth))
        {
            queue_num = CANFD_PRV_TXQ_INDEX_4;
        }
        else
        {
            queue_num = CANFD_PRV_TXQ_INDEX_5;
        }
    }

    /* Get index and channel of tx queue */
    if (queue_num < CANFD_PRV_TXQ_CH_OFFSET)
    {
        /* Transmit queue belong the channel */
        queue_index   = queue_num;
        queue_channel = p_ctrl->channel_index;
    }
    else
    {
        /* Transmit queue lend from the consecutive channel */
        queue_index   = queue_num - CANFD_PRV_TXQ_CH_OFFSET;
        queue_channel = (uint32_t) p_ctrl->channel_index + 1U;
    }

    /* Check Transmit Queue Event */
    reg_val = p_canfd_reg->CFDTXQ[queue_index].STS[queue_channel]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    if (R_CANFD0_CFDTXQ_STS_TXQEMP_Msk == (reg_val & R_CANFD0_CFDTXQ_STS_TXQEMP_Msk))
    {
        p_args->event = CAN_EVENT_TX_QUEUE_EMPTY;
    }
    else
    {
        p_args->event = CAN_EVENT_TX_COMPLETE;
    }

    /* Clear queue TX interrupt flag */
    reg_val &= ~(R_CANFD0_CFDTXQ_STS_TXQTXIF_Msk);
    p_canfd_reg->CFDTXQ[queue_index].STS[queue_channel] = reg_val;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Encode buffer info (priority/queue) */
    p_args->buffer = (can_buffer_t) (queue_num | (uint32_t) CAN_BUFFER_TYPE_TX_PRIORITY); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* User callback */
    r_canfd_call_callback(p_ctrl, p_args);
}

/*******************************************************************************************************************//**
 * Handle transmit queue message error interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 **********************************************************************************************************************/
static void r_canfd_handle_txqueue_error (canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args)
{
    uint32_t                     queue_bit_val;
    uint32_t                     queue_num;
    uint32_t                     queue_channel;
    uint32_t                     queue_index;
    volatile R_CANFD0_Type     * p_canfd_reg;
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    uint8_t           channel_txq_index;
    volatile uint32_t dummy;
    bool              is_msg_lost = false;

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /* Get the index of the lost TXQ message. */
    queue_bit_val = p_canfd_reg->CFDTXQMSTS; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    if (queue_bit_val != 0U)
    {
        is_msg_lost = true;
    }
    else
    {
        /* Get the index of the overwrite TXQ message. */
        queue_bit_val = p_canfd_reg->CFDTXQOWSTS; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Check status of Transmit Queue message lost interrupt */
    for (uint8_t channel = 0; channel < CANFD_MAX_CHANNEL_PER_UNIT; channel++)
    {
        if (0U != (queue_bit_val & ((uint32_t) CANFD_PRV_TXQ_MSK << (CANFD_PRV_TXQ_CH_OFFSET * channel))))
        {
            queue_bit_val   >>= (CANFD_PRV_TXQ_CH_OFFSET * channel);
            channel_txq_index = channel;
            break;
        }
    }

    while (queue_bit_val != 0U)
    {
        /*Check queue buffer value*/
        queue_num = (uint32_t) SCH1R(queue_bit_val) - 1U; /* PRQA S 3469 # Advisory D-4.9 */

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
                queue_channel = (uint32_t) channel_txq_index + 1U;
            }

            p_args->buffer = (can_buffer_t) (queue_index);                                                /* PRQA S 4342, 0662 # Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            if (is_msg_lost)
            {
                p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel] = ~R_CANFD0_CFDTXQ_STS_TXQMLT_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
            else
            {
                p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel] = ~R_CANFD0_CFDTXQ_STS_TXQMOW_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }

            /* Dummy read to ensure that interrupt event is cleared. */
            dummy = p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel];                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            FSP_PARAMETER_NOT_USED(dummy);
        }

        queue_bit_val &= ~((uint32_t) 1U << queue_num);
    }
}

#ifdef BSP_FEATURE_CANFD_FFI_MODE

/*******************************************************************************************************************//**
 * Handle ffi transmit queue message error interrupts
 * @param[in]  p_ctrl       - pointer to canfd control block
 * @param[in]  p_args       - pointer to callback argument
 **********************************************************************************************************************/
static void r_canfd_handle_txqueue_ffi_error (canfd_instance_ctrl_t const * p_ctrl, can_callback_args_t * p_args)
{
    uint32_t                     queue_bit_val;
    uint32_t                     queue_num;
    uint32_t                     queue_channel;
    uint32_t                     queue_index;
    volatile R_CANFD0_Type     * p_canfd_reg;
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    uint8_t           channel_index;
    volatile uint32_t dummy;
    bool              is_msg_lost = false;

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Check status of Transmit Queue message lost interrupt */
    queue_bit_val = p_canfd_reg->CFDTXQMSTS >> (channel_index * CANFD_PRV_TXQ_CH_OFFSET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    if (queue_bit_val != 0U)
    {
        is_msg_lost = true;
    }
    else
    {
        /* Check status of Transmit Queue message overwrite interrupt */
        queue_bit_val = p_canfd_reg->CFDTXQOWSTS >> (channel_index * CANFD_PRV_TXQ_CH_OFFSET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    while (queue_bit_val != 0U)
    {
        /*Check queue buffer value*/
        queue_num = (uint32_t) SCH1R(queue_bit_val) - 1U; /* PRQA S 3469 # Advisory D-4.9 */

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
                queue_channel = (uint32_t) channel_index + 1U;
            }

            p_args->buffer = (can_buffer_t) queue_index;                                                  /* PRQA S 4342, 0662 # Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            if (is_msg_lost)
            {
                p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel] = ~R_CANFD0_CFDTXQ_STS_TXQMLT_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
            else
            {
                p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel] = ~R_CANFD0_CFDTXQ_STS_TXQMOW_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }

            /* Dummy read to ensure that interrupt event is cleared. */
            dummy = p_canfd_reg->CFDTXQ[p_args->buffer].STS[queue_channel];                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            FSP_PARAMETER_NOT_USED(dummy);
        }

        queue_bit_val &= ~((uint32_t) 1U << queue_num);
    }

    if (is_msg_lost)
    {
        p_args->error |= (uint32_t) CANFD_ERROR_CHANNEL_VM_TXQ_CFIFO_MESSAGE_LOST;
    }
    else
    {
        p_args->error |= (uint32_t) CANFD_ERROR_CHANNEL_VM_TXQ_MESSAGE_OVERWRITE;
    }
}

/*******************************************************************************************************************//**
 * Handle transmit queue one frame reception/transmission interrupt in FFI mode
 * @param[in]  p_ctrl               Pointer to CANFD control block
 * @param[in]  p_args               Pointer to callback argument
 * @param[in]  p_canfd_reg          Base address of the CANFD register block
 * @param[in]  txq_one_frame_flag   Transmit Queue one frame reception/transmission flag
 **********************************************************************************************************************/
static void r_canfd_handle_ffi_txqueue_one_frame (canfd_instance_ctrl_t const * p_ctrl,
                                                  can_callback_args_t         * p_args,
                                                  volatile R_CANFD0_Type      * p_canfd_reg,
                                                  uint32_t                      txq_one_frame_flag)
{
    volatile uint32_t reg_val = 0;
    uint32_t          queue_bit_val;
    uint32_t          queue_num;
    uint32_t          queue_channel;
    uint32_t          queue_index;

    if (txq_one_frame_flag == R_CANFD0_CFDVMISTS_TXQOFRXIF_Msk)
    {
        /* Check status of Transmit Queue one frame reception interrupt */
        queue_bit_val = p_canfd_reg->CFDTXQOFRISTS >> (p_ctrl->channel_index * CANFD_PRV_TXQ_CH_OFFSET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else if (txq_one_frame_flag == R_CANFD0_CFDVMISTS_TXQOFTXIF_Msk)
    {
        /* Check status of Transmit Queue one frame transmission interrupt */
        queue_bit_val = p_canfd_reg->CFDTXQOFTISTS >> (p_ctrl->channel_index * CANFD_PRV_TXQ_CH_OFFSET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        /* Do nothing */
        queue_bit_val = 0U;
    }

    while (queue_bit_val != 0U)
    {
        /*Check queue buffer value*/
        queue_num = (uint32_t) SCH1R(queue_bit_val) - 1U; /* PRQA S 3469 # Advisory D-4.9 */

        /* Only handle if the transmit queue is belonged in this channel */
        if ((queue_num >= p_ctrl->txqueue_range_min) && (queue_num < p_ctrl->txqueue_range_max))
        {
            /* Get index and channel of tx queue */
            if (queue_num < CANFD_PRV_TXQ_CH_OFFSET)
            {
                /* Transmit queue belong the channel */
                queue_index   = queue_num;
                queue_channel = p_ctrl->channel_index;
            }
            else
            {
                /* Transmit queue lend from the consecutive channel */
                queue_index   = queue_num - CANFD_PRV_TXQ_CH_OFFSET;
                queue_channel = (uint32_t) p_ctrl->channel_index + 1U;
            }

            /* Check Transmit Queue Event */
            reg_val = p_canfd_reg->CFDTXQ[queue_index].STS[queue_channel]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            if (txq_one_frame_flag == R_CANFD0_CFDVMISTS_TXQOFRXIF_Msk)
            {
                if (R_CANFD0_CFDTXQ_STS_TXQOFRXIF_Msk == (reg_val & R_CANFD0_CFDTXQ_STS_TXQOFRXIF_Msk))
                {
                    p_args->event = CAN_EVENT_TX_QUEUE_ONE_FRAME_RX;
                }

                /* Get Transmit Queue Interrupt Flag */
                reg_val &= ~(R_CANFD0_CFDTXQ_STS_TXQOFRXIF_Msk);
            }
            else if (txq_one_frame_flag == R_CANFD0_CFDVMISTS_TXQOFTXIF_Msk)
            {
                p_args->event = CAN_EVENT_TX_COMPLETE;

                /* Get Transmit Queue Interrupt Flag */
                reg_val &= ~(R_CANFD0_CFDTXQ_STS_TXQOFTXIF_Msk);
            }
            else
            {
                /* Do nothing */
            }

            p_args->buffer = (can_buffer_t) ((uint32_t) CAN_BUFFER_TYPE_TX_PRIORITY | queue_num); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Clear Transmit Queue Interrupt Flag */
            p_canfd_reg->CFDTXQ[queue_index].STS[queue_channel] = reg_val;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Call user callback */
            r_canfd_call_callback(p_ctrl, p_args);
        }

        queue_bit_val &= ~((uint32_t) 1U << queue_num);
    }
}

#endif

/*******************************************************************************************************************//**
 * Setting filter rule for AFL and PNF
 * @param[in]  p_ctrl       - pointer to canfd control block
 **********************************************************************************************************************/
static void r_canfd_setting_filter_rule (canfd_instance_ctrl_t const * p_ctrl)
{
    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;                            /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    canfd_extended_cfg_t const * p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    canfd_global_cfg_t const * p_global_cfg = p_extend->p_global_cfg;
    volatile R_CANFD0_Type   * p_canfd_reg  = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    uint8_t channel_index = p_ctrl->channel_index;

    /* Get AFL entry and limit */
    uint32_t afl_entry = 0;
    uint32_t afl_max   = p_global_cfg->afl_rule_num[0];
    for (uint32_t channel_cnt = 1; channel_cnt <= channel_index; channel_cnt++)
    {
        afl_entry += p_global_cfg->afl_rule_num[channel_cnt - 1U];
        afl_max   += p_global_cfg->afl_rule_num[channel_cnt];
    }

    /* Unlock AFL */
    p_canfd_reg->CFDGAFLECTR |= R_CANFD0_CFDGAFLECTR_AFLDAE_Msk;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Write all configured Receive rule entries */
    R_CANFD0_CFDGAFL_Type const * p_afl = (R_CANFD0_CFDGAFL_Type const *) p_extend->p_afl;   /* PRQA S 0310 # DP_MSR_R-11.3_UC1 */
    for ( ; afl_entry < afl_max; afl_entry++)
    {
        /* Set AFL page */
        p_canfd_reg->CFDGAFLECTR = (afl_entry >> 4U) | R_CANFD0_CFDGAFLECTR_AFLDAE_Msk;      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Get pointer to current AFL rule and set it to the rule pointed to by p_afl */
        volatile R_CANFD0_CFDGAFL_Type * cfdgafl = &p_canfd_reg->CFDGAFL[afl_entry & 0x0FU]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        *cfdgafl = *p_afl;
        p_afl++;
    }

    /* Lock AFL */
    p_canfd_reg->CFDGAFLECTR = 0;      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Get PNFL entry and limit */
    uint32_t pnf_entry = 0;
    uint32_t pnf_max   = p_global_cfg->pnf_rule_num[0];
    for (uint32_t channel_cnt = 1; channel_cnt <= channel_index; channel_cnt++)
    {
        pnf_entry += p_global_cfg->pnf_rule_num[channel_cnt - 1U];
        pnf_max   += p_global_cfg->pnf_rule_num[channel_cnt];
    }

#ifdef BSP_FEATURE_CANFD_PN_MODE

    /* Unlock PNFL */
    p_canfd_reg->CFDGPFLECTR |= R_CANFD0_CFDGPFLECTR_PFLDAE_Msk;                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Write all configured Receive rule entries for PNFL */
    canfd_pnf_entry_t const * p_pnf = (canfd_pnf_entry_t *) p_extend->p_pnf;                 /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    for ( ; pnf_entry < pnf_max; pnf_entry++)
    {
        /* Set PNFL page */
        p_canfd_reg->CFDGPFLECTR = (pnf_entry >> 2) | R_CANFD0_CFDGPFLECTR_PFLDAE_Msk;       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Get pointer to current PNFL rule and set it to the rule pointed to by p_pnf */
        volatile R_CANFD0_CFDGPFL_Type * cfdgpnf = &p_canfd_reg->CFDGPFL[pnf_entry & 0x03U]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*Setting PNF rule into register */
        r_canfd_copy_pnf_rule(cfdgpnf, p_pnf);
        p_pnf++;
    }

    /* Lock PNFL */
    p_canfd_reg->CFDGPFLECTR = 0;      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
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
    p_reg->ID = p_entry->id.id_u32;                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Write the ID mask (defines which bits are compared) */
    p_reg->MSK = p_entry->mask.mask_u32;                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Write destination information (Pointer 0 and Pointer 1)
     *  Specifies where to store or process the frame when matched*/
    p_reg->P0 = p_entry->destination.destination_u32[0];          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_reg->P1 = p_entry->destination.destination_u32[1];          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /*Write the payload type (defines the type of data to check)*/
    p_reg->PT = p_entry->payload_type.payload_type_u32;           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Write payload compare values and masks:
     * - PD0/PM0: data and mask for the first part of the payload
     * - PD1/PM1: data and mask for the next part of the payload */
    p_reg->PD0 = p_entry->payload_compare.payload_compare_u32[0]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_reg->PM0 = p_entry->payload_compare.payload_compare_u32[1]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_reg->PD1 = p_entry->payload_compare.payload_compare_u32[2]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_reg->PM1 = p_entry->payload_compare.payload_compare_u32[3]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
}

#endif

#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE

/*******************************************************************************************************************//**
 * Check Empty of array for transfer
 *
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 **********************************************************************************************************************/
static inline bool r_canfd_data_transfer_empty (canfd_transfer_cfg_t const * p_transfer_cfg)
{
    return p_transfer_cfg->front >= p_transfer_cfg->rear;
}

/*******************************************************************************************************************//**
 * Check full of array for transfer
 *
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 **********************************************************************************************************************/
static inline bool r_canfd_data_transfer_full (canfd_transfer_cfg_t const * p_transfer_cfg)
{
    return p_transfer_cfg->rear == p_transfer_cfg->size; /* PRQA S 1710 # Advisory D-4.5 */
}

/*******************************************************************************************************************//**
 * Check last entry of array for transfer
 *
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 **********************************************************************************************************************/
static inline bool r_canfd_data_transfer_last_entry (canfd_transfer_cfg_t const * p_transfer_cfg)
{
    return p_transfer_cfg->rear == (p_transfer_cfg->size - 1);
}

/*******************************************************************************************************************//**
 * Reset pointer for array for transfer
 *
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 **********************************************************************************************************************/
static void r_canfd_data_transfer_reset (canfd_transfer_cfg_t * p_transfer_cfg)
{
    if (p_transfer_cfg->front >= p_transfer_cfg->rear)
    {
        /* Read the volatile transfer addresses into locals so one statement holds at most one volatile access */
        void const * p_src  = p_transfer_cfg->p_src;
        void       * p_dest = p_transfer_cfg->p_dest;

        p_transfer_cfg->front = 0;
        p_transfer_cfg->rear  = -1;
        p_transfer_cfg->p_transfer->p_api->reset(p_transfer_cfg->p_transfer->p_ctrl,
                                                 p_src,
                                                 p_dest,
                                                 CANFD_SIZE_DATA_WORD_BUFFER);
    }
}

/*******************************************************************************************************************//**
 * Configuration CANFD RX FIFO Transfer
 *
 * @param[in]  p_canfd_reg                  Base address of the CANFD register block
 * @param[in]  p_ctrl                       Pointer to CANFD control block
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 * @param[in]  index                        Index of CANFD transfer context
 * @param[in]  rx_fifo_config               RX FIFO configuration
 *
 * @retval FSP_SUCCESS                      Successful open transfer module.
 * @retval FSP_ERR_NOT_INITIALIZED          Unsuccessful open transfer module.
 **********************************************************************************************************************/
static inline fsp_err_t r_canfd_transfer_rx_fifo_cfg (volatile R_CANFD0_Type * p_canfd_reg,
                                                      canfd_instance_ctrl_t  * p_ctrl,
                                                      canfd_transfer_cfg_t   * p_transfer_cfg,
                                                      uint8_t                  index,
                                                      canfd_rx_fifo_cfg_t      rx_fifo_config) /* PRQA S 0759 # Advisory R-19.2 */
{
    fsp_err_t err = FSP_SUCCESS;

    p_canfd_reg->CFDCDTCT |= ((uint32_t) 1U << (uint32_t) p_transfer_cfg->type);               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    err =
        p_transfer_cfg->p_transfer->p_api->open(p_transfer_cfg->p_transfer->p_ctrl, p_transfer_cfg->p_transfer->p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, FSP_ERR_NOT_INITIALIZED);                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    p_transfer_cfg->p_src = (void *) &p_canfd_reg->CFDRF[index];                               /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    canfd_transfer_context[index].p_ctrl         = p_ctrl;
    canfd_transfer_context[index].p_transfer_cfg = p_transfer_cfg;

    (void) p_transfer_cfg->p_transfer->p_api->callbackSet(p_transfer_cfg->p_transfer->p_ctrl,
                                                          &canfd_rx_fifo_transfer_callback, /* PRQA S 3635 # Advisory R-17.12 */
                                                          &canfd_transfer_context[index],
                                                          NULL);

    p_transfer_cfg->p_transfer->p_cfg->p_info->p_src  = p_transfer_cfg->p_src;
    p_transfer_cfg->p_transfer->p_cfg->p_info->p_dest = p_transfer_cfg->p_dest;

    (void) p_transfer_cfg->p_transfer->p_api->reconfigure(p_transfer_cfg->p_transfer->p_ctrl,
                                                          p_transfer_cfg->p_transfer->p_cfg->p_info);

    (void) p_transfer_cfg->p_transfer->p_api->enable(p_transfer_cfg->p_transfer->p_ctrl);
    p_transfer_cfg->front = 0;
    p_transfer_cfg->rear  = -1;
    uint32_t rx_fifo_depth = (uint32_t) 2U << rx_fifo_config.rx_fifo_cfg_b.RFDC;
    p_transfer_cfg->size = (int16_t) rx_fifo_depth;

    return err;
}

/*******************************************************************************************************************//**
 * Configuration CANFD Common FIFO 0 - 2 and Trasnmit Queue 0 - 3 Transfer
 *
 * @param[in]  p_canfd_reg                  Base address of the CANFD register block
 * @param[in]  p_ctrl                       Pointer to CANFD control block
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 * @param[in]  p_tx_queue_cfg               Pointer to Transmit queue configuration
 * @param[in]  index                        Index of CANFD transfer context
 * @param[in]  cfifo_config                 Common FIFO Configuration
 *
 * @retval FSP_SUCCESS                      Successful open transfer module.
 * @retval FSP_ERR_NOT_INITIALIZED          Unsuccessful open transfer module.
 **********************************************************************************************************************/
static inline fsp_err_t r_canfd_transfer_channel_cfg (volatile R_CANFD0_Type * p_canfd_reg,
                                                      canfd_instance_ctrl_t  * p_ctrl,
                                                      canfd_transfer_cfg_t   * p_transfer_cfg,
                                                      canfd_tx_queue_cfg_t   * p_tx_queue_cfg, /* PRQA S 3673 # Advisory R-8.13 */
                                                      uint8_t                  index,
                                                      canfd_cfifo_cfg_t        cfifo_config)   /* PRQA S 0759 # Advisory R-19.2 */
{
    FSP_PARAMETER_NOT_USED(p_tx_queue_cfg);
    fsp_err_t err          = FSP_SUCCESS;
    uint32_t  txq_buffer   = 0;
    uint32_t  queue_window = 0;

    if (CANFD_TRANSFER_BUFFER_COMMON_FIFO_0 == p_transfer_cfg->type)
    {
        /* Common FIFO mode RX */
        p_canfd_reg->CFDCDTCT |= ((uint32_t) 1U << ((uint32_t) p_transfer_cfg->type + p_ctrl->channel_index)); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        /* Common FIFO mode TX and TXQ0/3 */
        p_canfd_reg->CFDCDTTCT |=      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                  ((uint32_t) 1U <<
                                   ((((uint32_t) p_transfer_cfg->type -
                                      (uint32_t) CANFD_TRANSFER_BUFFER_TXQ_0) * 8U) +
                                    p_ctrl->channel_index));
    }

    p_transfer_cfg->front = 0;
    p_transfer_cfg->rear  = -1;

    err =
        p_transfer_cfg->p_transfer->p_api->open(p_transfer_cfg->p_transfer->p_ctrl, p_transfer_cfg->p_transfer->p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, FSP_ERR_NOT_INITIALIZED); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    canfd_transfer_context[index].p_ctrl         = p_ctrl;
    canfd_transfer_context[index].p_transfer_cfg = p_transfer_cfg;

    if (CANFD_TRANSFER_BUFFER_COMMON_FIFO_0 == p_transfer_cfg->type)
    {
        uint32_t cfifo_depth = (uint32_t) 2U << cfifo_config.cfifo_cfg_b.depth;
        p_transfer_cfg->size = (int16_t) cfifo_depth;
        (void) p_transfer_cfg->p_transfer->p_api->callbackSet(p_transfer_cfg->p_transfer->p_ctrl,
                                                              canfd_rx_cfifo_transfer_callback,          /* PRQA S 3635 # Advisory R-17.12 */
                                                              &canfd_transfer_context[index],
                                                              NULL);
        p_transfer_cfg->p_src =
            (void *) &p_canfd_reg->CFDCF[(p_ctrl->channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)]; /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        p_transfer_cfg->p_transfer->p_cfg->p_info->p_src  = p_transfer_cfg->p_src;
        p_transfer_cfg->p_transfer->p_cfg->p_info->p_dest = p_transfer_cfg->p_dest;

        (void) p_transfer_cfg->p_transfer->p_api->reconfigure(p_transfer_cfg->p_transfer->p_ctrl,
                                                              p_transfer_cfg->p_transfer->p_cfg->p_info);

        (void) p_transfer_cfg->p_transfer->p_api->enable(p_transfer_cfg->p_transfer->p_ctrl);
    }
    else if (CANFD_TRANSFER_BUFFER_COMMON_FIFO_2 == p_transfer_cfg->type)
    {
        p_transfer_cfg->size   = 1;
        p_transfer_cfg->p_dest =
            (void *) &p_canfd_reg->CFDCF[(p_ctrl->channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL) + 2U]; /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        (void) p_transfer_cfg->p_transfer->p_api->callbackSet(p_transfer_cfg->p_transfer->p_ctrl,
                                                              canfd_tx_transfer_callback,                     /* PRQA S 3635 # Advisory R-17.12 */
                                                              &canfd_transfer_context[index],
                                                              NULL);
    }
    else                                                                                                      /* TXQ */
    {
        txq_buffer   = 3U * ((uint32_t) p_transfer_cfg->type - (uint32_t) CANFD_TRANSFER_BUFFER_TXQ_0);
        queue_window = g_queue_window[txq_buffer] +
                       ((uint32_t) p_ctrl->channel_index * CANFD_PRV_TXMB_CHANNEL_OFFSET);
        p_transfer_cfg->p_dest = (void *) &p_canfd_reg->CFDTM[queue_window];              /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_transfer_cfg->size   = 1;

        (void) p_transfer_cfg->p_transfer->p_api->callbackSet(p_transfer_cfg->p_transfer->p_ctrl,
                                                              canfd_tx_transfer_callback, /* PRQA S 3635 # Advisory R-17.12 */
                                                              &canfd_transfer_context[index],
                                                              NULL);
    }

    return err;
}

/*******************************************************************************************************************//**
 * Configuration CANFD Transfer
 *
 * @param[in]  p_ctrl                       Pointer to CANFD control block
 * @param[in]  p_extend                     Pointer to extend configuration
 * @param[in]  p_global_cfg                 Pointer to global configuration
 *
 * @retval FSP_SUCCESS                      Successful open transfer module.
 * @retval FSP_ERR_NOT_INITIALIZED          Unsuccessful open transfer module.
 **********************************************************************************************************************/
static fsp_err_t r_canfd_transfer_cfg (canfd_instance_ctrl_t * p_ctrl,
                                       canfd_extended_cfg_t  * p_extend,
                                       canfd_global_cfg_t    * p_global_cfg)
{
    fsp_err_t err          = FSP_SUCCESS;
    uint8_t   offset_cfifo = (uint8_t) (p_ctrl->channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);
    uint8_t   offset_txq   = 0;

    /* Unit RX FIFO transfers are shared by all channels: configure them only once per unit */
    if (0U == p_global_cfg->status_transfer_cfg)
    {
        for (uint8_t fifo_idx = 0; fifo_idx < BSP_FEATURE_CANFD_NUM_RXFIFO; fifo_idx++)
        {
            if (NULL != p_global_cfg->transfer_rx_fifo_cfg[fifo_idx].p_transfer)
            {
                err = r_canfd_transfer_rx_fifo_cfg(p_global_cfg->p_canfd_reg,
                                                   p_ctrl,
                                                   &p_global_cfg->transfer_rx_fifo_cfg[fifo_idx],
                                                   fifo_idx,
                                                   p_global_cfg->rx_fifo_config[fifo_idx]);
                FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
            }
        }

        p_global_cfg->status_transfer_cfg = 1;
    }

    /* Configure this channel's Common FIFO / TX Queue transfers */
    for (uint8_t cfg_idx = 0; cfg_idx < (CANFD_CHANNEL_MAX_TRANSFER_CFG - 1U); cfg_idx++)
    {
        if ((NULL != p_extend->p_transfer_cfg) && (NULL != p_extend->p_transfer_cfg[cfg_idx]) &&
            (NULL != p_extend->p_transfer_cfg[cfg_idx]->p_transfer))
        {
            /* Advance the CFIFO/TXQ register offsets after each entry pair */
            if (1U == (cfg_idx & 1U))
            {
                offset_cfifo = (uint8_t) (offset_cfifo + 2U);
                offset_txq   = (uint8_t) (offset_txq + 3U);
            }

            err = r_canfd_transfer_channel_cfg(p_global_cfg->p_canfd_reg,
                                               p_ctrl,
                                               p_extend->p_transfer_cfg[cfg_idx],
                                               p_extend->tx_queue_cfg,
                                               (uint8_t) CANFD_SET_INDEX_CONTEXT(p_ctrl->p_cfg->channel, cfg_idx), /* PRQA S 3469 # Advisory D-4.9 */
                                               p_global_cfg->common_fifo_config[offset_cfifo]);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);                                                             /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */
        }
    }

    /* Odd channel: in flexible buffer mode, share its TXQ0 transfer with the even channel of the pair
     * through the global flexible TX configuration */
    if (1U == (p_ctrl->channel_index & 1U))
    {
        if (p_global_cfg->flex_tx_buffer[p_ctrl->channel_index >> 1] > CANFD_FLEX_BUFFER_LEND_0)
        {
            p_global_cfg->transfer_flex_tx_cfg[p_ctrl->channel_index >> 1].rear   = -1;
            p_global_cfg->transfer_flex_tx_cfg[p_ctrl->channel_index >> 1].size   = 1;
            p_global_cfg->transfer_flex_tx_cfg[p_ctrl->channel_index >> 1].p_dest = p_extend->p_transfer_cfg[2]->p_dest;
            p_global_cfg->transfer_flex_tx_cfg[p_ctrl->channel_index >> 1].p_src  = p_extend->p_transfer_cfg[2]->p_src;
            p_global_cfg->transfer_flex_tx_cfg[p_ctrl->channel_index >>
                                               1].p_transfer = p_extend->p_transfer_cfg[2]->p_transfer;
            p_global_cfg->transfer_flex_tx_cfg[p_ctrl->channel_index >> 1].type = CANFD_TRANSFER_BUFFER_TXQ_0;

            canfd_transfer_context[CANFD_SET_INDEX_CONTEXT(p_ctrl->p_cfg->channel, 2U)].p_transfer_cfg = /* PRQA S 3469 # Advisory D-4.9 */
                                                                                                         &p_global_cfg->
                                                                                                         transfer_flex_tx_cfg
                                                                                                         [p_ctrl->
                                                                                                          channel_index
                                                                                                          >>
                                                                                                          1];
        }
    }
    else                               /* Even channel: pick up the shared flexible TX transfer for its lend TX queue */
    {
        if (1U == p_extend->tx_queue_cfg[4].tx_queue_cfg_b.enable)
        {
            p_extend->p_transfer_cfg[4] = &p_global_cfg->transfer_flex_tx_cfg[p_ctrl->channel_index >> 1];
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Close RX FIFO Transfer
 *
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 *
 * @retval FSP_SUCCESS                      Successful close transfer module.
 * @retval FSP_ERR_ASSERTION                An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN                 The transfer has not been opened.
 **********************************************************************************************************************/
static fsp_err_t r_canfd_transfer_rx_fifo_close (canfd_transfer_cfg_t const * p_transfer_cfg)
{
    fsp_err_t err = FSP_SUCCESS;

    for (uint8_t rx_fifo_idx = 0; rx_fifo_idx < BSP_FEATURE_CANFD_NUM_RXFIFO; rx_fifo_idx++)
    {
        if (NULL != p_transfer_cfg[rx_fifo_idx].p_transfer)
        {
            err = p_transfer_cfg[rx_fifo_idx].p_transfer->p_api->close(p_transfer_cfg[rx_fifo_idx].p_transfer->p_ctrl);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            canfd_transfer_context[rx_fifo_idx].p_ctrl         = (canfd_instance_ctrl_t *) NULL;
            canfd_transfer_context[rx_fifo_idx].p_transfer_cfg = (canfd_transfer_cfg_t *) NULL;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Close Common FIFO and TXQ Transfer
 *
 * @param[in]  pp_transfer_cfg              Pointer to Array CANFD transfer configuration
 * @param[in]  p_canfd_reg                  Base address of the CANFD register block
 * @param[in]  channel_index                CANFD channel index
 *
 * @retval FSP_SUCCESS                      Successful close transfer module.
 * @retval FSP_ERR_ASSERTION                An input parameter is invalid.
 * @retval FSP_ERR_NOT_OPEN                 The transfer has not been opened.
 **********************************************************************************************************************/
static fsp_err_t r_canfd_transfer_channel_close (canfd_transfer_cfg_t  ** pp_transfer_cfg, /* PRQA S 3673 # Advisory R-8.13 */
                                                 volatile R_CANFD0_Type * p_canfd_reg,
                                                 uint8_t                  channel_index)
{
    fsp_err_t err = FSP_SUCCESS;

    for (uint8_t ch_cfg_idx = 0; ch_cfg_idx < (CANFD_CHANNEL_MAX_TRANSFER_CFG - 1U); ch_cfg_idx++)
    {
        if ((NULL != pp_transfer_cfg) && (NULL != pp_transfer_cfg[ch_cfg_idx]) &&
            (NULL != pp_transfer_cfg[ch_cfg_idx]->p_transfer))
        {
            if (CANFD_TRANSFER_BUFFER_COMMON_FIFO_0 == pp_transfer_cfg[ch_cfg_idx]->type)
            {
                /* Common FIFO mode RX */
                p_canfd_reg->CFDCDTCT &= /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                         ~((uint32_t) 1U <<
                                           ((uint32_t) pp_transfer_cfg[ch_cfg_idx]->type + channel_index));
            }
            else
            {
                /* Common FIFO mode TX and TXQ0/3 */
                p_canfd_reg->CFDCDTTCT &= /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                          ~((uint32_t) 1U <<
                                            ((((uint32_t) pp_transfer_cfg[ch_cfg_idx]->type -
                                               (uint32_t) CANFD_TRANSFER_BUFFER_TXQ_0) *
                                              8U) +
                                             channel_index));
            }

            err =
                pp_transfer_cfg[ch_cfg_idx]->p_transfer->p_api->close(pp_transfer_cfg[ch_cfg_idx]->p_transfer->p_ctrl);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

            canfd_transfer_context[ch_cfg_idx +
                                   BSP_FEATURE_CANFD_NUM_RXFIFO].p_ctrl = (canfd_instance_ctrl_t *) NULL;
            canfd_transfer_context[ch_cfg_idx + BSP_FEATURE_CANFD_NUM_RXFIFO].p_transfer_cfg =
                (canfd_transfer_cfg_t *) NULL;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Perform FIFO Transfer Read.
 *
 * @param[in]  p_frame                      Pointer to CAN frame to read
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 *
 * @retval FSP_SUCCESS                      Successful close transfer module.
 * @retval FSP_ERR_BUFFER_EMPTY             Buffer empty.
 **********************************************************************************************************************/
static inline fsp_err_t r_canfd_fifo_transfer_read (can_frame_t * const p_frame, canfd_transfer_cfg_t * p_transfer_cfg)
{
    /* Get status of selected FIFO */
    FSP_ERROR_RETURN(!r_canfd_data_transfer_empty(p_transfer_cfg), FSP_ERR_BUFFER_EMPTY);                         /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    /* Get CAN Frame */
    r_canfd_mb_read_transfer(p_frame, &((canfd_rx_fifo_data_t *) p_transfer_cfg->p_dest)[p_transfer_cfg->front]); /* PRQA S 0316 # Advisory R-11.5 */

    p_transfer_cfg->front++;

    r_canfd_data_transfer_reset(p_transfer_cfg);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read from a Message Buffer.
 *
 * @param[out]    p_frame   Pointer to CAN frame to read to
 * @param[in]     p_data    Pointer to data receive with transfer feature
 **********************************************************************************************************************/
static void r_canfd_mb_read_transfer (can_frame_t * const p_frame, canfd_rx_fifo_data_t const * p_data)
{
    uint32_t                 length = 0;
    volatile uint8_t       * p_dest;
    volatile uint8_t const * p_src;

    /* Get frame options */
    p_frame->options = CAN_FRAME_OPTIONS_NONE;

    /* Get frame format */
    if (0U != (p_data->FDSTS & R_CANFD0_CFDRM_FDSTS_RMFDF_Msk))                                                         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /* FD Frame */
        p_frame->format = CAN_FRAME_FORMAT_FD;
        if (0U != (p_data->FDSTS & R_CANFD0_CFDRM_FDSTS_RMBRS_Msk))                                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* Get FD BRS bit */
            p_frame->options = (can_frames_options_t) ((uint32_t) p_frame->options | (uint32_t) CAN_FRAME_OPTIONS_BRS); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */
        }

        if (0U != (p_data->FDSTS & R_CANFD0_CFDRM_FDSTS_RMESI_Msk))                                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* Get FD ESI bit */
            p_frame->options = (can_frames_options_t) ((uint32_t) p_frame->options | (uint32_t) CAN_FRAME_OPTIONS_ESI); /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */
        }
    }
    else
    {
        /* CAN Classic Frame */
        p_frame->format = CAN_FRAME_FORMAT_CC;
    }

    /* Get the frame ID */
    p_frame->id = p_data->ID & R_CANFD0_CFDRM_ID_RMID_Msk;                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Get the frame ID mode (IDE bit) */
    p_frame->id_mode = (can_id_mode_t) (p_frame->id >> R_CANFD0_CFDRM_ID_RMIDE_Pos);       /* PRQA S 4394, 4342 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 */

    /* Get the frame data length code */
    p_frame->data_length_code = dlc_to_bytes[p_data->PTR >> R_CANFD0_CFDRM_PTR_RMDLC_Pos]; /* PRQA S 0662, 1710 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.5 */

    /* Get the frame label */
    p_frame->labels = p_data->FDSTS_b.RFPTR;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Get the frame time stamp */
    p_frame->timestamp = p_data->PTR_b.RFTS;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Copy data to frame */
    length = p_frame->data_length_code;
    p_dest = p_frame->data;
    p_src  = (uint8_t const volatile *) p_data->DF; /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    while (length > 0U)
    {
        *p_dest = *p_src;
        p_dest++;                                   /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */
        p_src++;                                    /* PRQA S 0488, 0489 # Advisory R-18.4 , Advisory R-18.4 */
        length--;
    }
}

/*******************************************************************************************************************//**
 * Perform Transfer Write.
 *
 * @param[in]  p_frame                      Pointer to CAN frame to write to
 * @param[in]  p_extend                     Pointer to extend configuration of CANFD
 * @param[in]  p_transfer_cfg               Pointer to CANFD transfer configuration
 *
 * @retval FSP_SUCCESS                      Successful to transfer.
 * @retval FSP_ERR_CAN_TRANSMIT_NOT_READY   Buffer Full.
 **********************************************************************************************************************/
static fsp_err_t r_canfd_transfer_write (uint32_t                     channel,
                                         can_buffer_type_t            buffer_type,
                                         uint32_t                     buffer_index,
                                         can_frame_t * const          p_frame,
                                         canfd_extended_cfg_t const * p_extend,
                                         canfd_transfer_cfg_t       * p_transfer_cfg)
{
    canfd_cfifo_data_t * p_tx_buffer   = (canfd_cfifo_data_t *) p_transfer_cfg->p_dest; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    uint32_t             bit_ctrl      = 0;
    uint8_t              can_dlc       = 0;
    uint8_t              can_len       = 0;
    uint8_t              size_transfer = 0;

    /* Ensure the software transfer queue is not full. The TX transfer completion callback */
    FSP_ERROR_RETURN(!r_canfd_data_transfer_full(p_transfer_cfg), FSP_ERR_CAN_TRANSMIT_NOT_READY); /* PRQA S 3432 # DP_MSR_R-20.7_UC1 */

    if (CAN_BUFFER_TYPE_TX_FIFO == buffer_type)
    {
        buffer_index += (channel * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);                           /* PRQA S 1338 # Advisory R-17.8 */
    }

    p_tx_buffer->ID = p_frame->id | ((uint32_t) p_frame->type << R_CANFD0_CFDTM_ID_TMRTR_Pos) |    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                      ((uint32_t) p_frame->id_mode << R_CANFD0_CFDTM_ID_TMIDE_Pos) |
                      ((uint32_t) p_extend->tx_history_cfg.enable << R_CANFD0_CFDTM_ID_THLEN_Pos);

    /* Convert bytes to dlc */
    can_dlc = r_canfd_bytes_to_dlc(p_frame->data_length_code);

    p_tx_buffer->PTR = (uint32_t) can_dlc << R_CANFD0_CFDTM_PTR_TMDLC_Pos; /* PRQA S 0662, 1710 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 , Advisory D-4.5 */

    /* Set FD bits (ESI, BRS and FDF) */
    if (CAN_FRAME_FORMAT_FD == p_frame->format)
    {
        bit_ctrl = R_CANFD0_CFDTM_FDCTR_TMFDF_Msk | ((uint32_t) p_frame->options >> 1U);
    }

    /* Set buffer control register */
    p_tx_buffer->FDCSTS = bit_ctrl | (p_frame->labels << R_CANFD0_CFDTM_FDCTR_TMPTR_Pos); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Convert dlc to actual bytes */
    can_len = dlc_to_bytes[can_dlc];

    if (can_len > p_frame->data_length_code)
    {
        for (uint8_t pad_idx = (uint8_t) p_frame->data_length_code; pad_idx < can_len; pad_idx++)
        {
            p_frame->data[pad_idx] = p_extend->tx_padding_data;
        }
    }

    if (CAN_BUFFER_TYPE_TX_PRIORITY == buffer_type)
    {
        size_transfer = CANFD_SIZE_DATA_WORD_BUFFER - 3;
    }
    else
    {
        size_transfer = dlc_to_bytes[p_extend->p_global_cfg->p_canfd_reg->CFDCFCC_b[buffer_index].CFPLS + 8U] >> 2; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    (void) p_transfer_cfg->p_transfer->p_api->reset(p_transfer_cfg->p_transfer->p_ctrl,
                                                    p_frame->data,
                                                    (void *) p_tx_buffer->DF, /* PRQA S 0312, 0662 # DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                    size_transfer);

    p_transfer_cfg->rear += 2;
    (void) p_transfer_cfg->p_transfer->p_api->softwareStart(p_transfer_cfg->p_transfer->p_ctrl,
                                                            TRANSFER_START_MODE_SINGLE);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Handle Transfer Receive Callback.
 *
 * @param[in]   p_context       Pointer to context of callback
 * @param[in]   p_args          Pointer to callback argument
 * @param[in]   threshold       Threshold of RX FIFO or CFIFO mode RX Configuration
 * @param[in]   int_mode        FIFO Interrupt Mode
 * @param[in]   p_dest          Pointer to destination of data transfer
 **********************************************************************************************************************/
static inline void r_canfd_transfer_rx_callback (canfd_transfer_context_t const * p_context,
                                                 can_callback_args_t            * p_args,
                                                 uint32_t                         threshold,
                                                 uint32_t                         int_mode,
                                                 canfd_rx_fifo_data_t           * p_dest)
{
    bool callback_required = false;
    bool last_entry        = false;

    if (r_canfd_data_transfer_full(p_context->p_transfer_cfg))
    {
        /* Destination queue is full: the new frame cannot be stored */
        p_args->event     = CAN_EVENT_FIFO_MESSAGE_LOST;
        callback_required = true;
    }
    else
    {
        /* When interrupt at FIFO full (threshold == 0), check if this frame fills the last queue entry */
        last_entry = (0U == threshold) && r_canfd_data_transfer_last_entry(p_context->p_transfer_cfg);

        /* Notify RX complete when: interrupt on every frame (int_mode == 1), the queue just became full,
         * or the number of queued frames reached the configured threshold */
        if ((1U == int_mode) ||
            last_entry ||
            ((0U < threshold) &&
             (((((uint32_t) p_context->p_transfer_cfg->rear - (uint32_t) p_context->p_transfer_cfg->front)) ==
               (threshold - 1U)) ||
              ((-1 == p_context->p_transfer_cfg->rear) && (1U == threshold)))))
        {
            p_args->event     = CAN_EVENT_RX_COMPLETE;
            callback_required = true;
        }

        /* Advance rear to the next free entry (-1 means the queue was empty and entry 0 was just filled) */
        p_context->p_transfer_cfg->rear += (-1 == p_context->p_transfer_cfg->rear) ? 2 : 1;
    }

    /* Re-arm the transfer to receive the next frame into the next free queue entry */
    p_context->p_transfer_cfg->p_transfer->p_api->reset(p_context->p_transfer_cfg->p_transfer->p_ctrl,
                                                        p_context->p_transfer_cfg->p_src,
                                                        &p_dest[p_context->p_transfer_cfg->rear],
                                                        CANFD_SIZE_DATA_WORD_BUFFER);

    if (callback_required)
    {
        r_canfd_call_callback(p_context->p_ctrl, p_args);
    }
}

/*******************************************************************************************************************//**
 * Receive data in RX Common FIFO interrupt routine called when transferring is completed.
 *
 * @param[in]     p_args     Pointer to argument.
 **********************************************************************************************************************/
static void canfd_rx_cfifo_transfer_callback (transfer_callback_args_t * p_args)                                       /* PRQA S 3673 # Advisory R-8.13 */
{
    canfd_transfer_context_t const * p_context          = (canfd_transfer_context_t *) p_args->p_context;              /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    canfd_extended_cfg_t const     * p_extend           = (canfd_extended_cfg_t *) p_context->p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    canfd_rx_fifo_data_t           * p_dest             = (canfd_rx_fifo_data_t *) p_context->p_transfer_cfg->p_dest;  /* PRQA S 0316 # Advisory R-11.5 */
    can_callback_args_t              args               = {0};
    canfd_cfifo_cfg_t                common_fifo_config =                                                              /* PRQA S 0759 # Advisory R-19.2 */
                                                          p_extend->p_global_cfg->common_fifo_config[p_context->p_ctrl->
                                                                                                     channel_index *
                                                                                                     BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL
                                                          ];
    uint32_t threshold = 0U;

    if ((TRANSFER_EVENT_TRANS_END_FLAG == p_args->event) || (TRANSFER_EVENT_TRANS_COMPLETION == p_args->event))
    {
        /* Get RX CFIFO Threshold Configuration */
        threshold =
            ((uint32_t) p_context->p_transfer_cfg->size *
             ((uint32_t) common_fifo_config.cfifo_cfg_b.rx_int_threshold + 1U)) >> 3;

        args.p_context = p_context->p_ctrl->p_context;
        args.channel   = p_context->p_ctrl->p_cfg->channel;
        args.buffer    = CAN_BUFFER_RX_FIFO_8; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Handle CFIFO in receive mode */
        if ((uint32_t) CANFD_CFIFO_MODE_RECEIVE == (uint32_t) common_fifo_config.cfifo_cfg_b.mode)
        {
            r_canfd_transfer_rx_callback(p_context,
                                         &args,
                                         threshold,
                                         common_fifo_config.cfifo_cfg_b.interrupt_select,
                                         p_dest);
        }
        else if ((uint32_t) CANFD_CFIFO_MODE_GATEWAY == (uint32_t) common_fifo_config.cfifo_cfg_b.mode)
        {
        }
        else
        {
            /* Do nothing */
        }
    }
}

/*******************************************************************************************************************//**
 * Receive data in RX FIFO interrupt routine called when transferring is completed.
 *
 * @param[in]     p_args     Pointer to argument.
 **********************************************************************************************************************/
static void canfd_rx_fifo_transfer_callback (transfer_callback_args_t * p_args)                                    /* PRQA S 3673 # Advisory R-8.13 */
{
    canfd_transfer_context_t const * p_context      = (canfd_transfer_context_t *) p_args->p_context;              /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    canfd_extended_cfg_t const     * p_extend       = (canfd_extended_cfg_t *) p_context->p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    canfd_rx_fifo_data_t           * p_dest         = (canfd_rx_fifo_data_t *) p_context->p_transfer_cfg->p_dest;  /* PRQA S 0316 # Advisory R-11.5 */
    can_callback_args_t              args           = {0};
    canfd_rx_fifo_cfg_t              rx_fifo_config =                                                              /* PRQA S 0759 # Advisory R-19.2 */
                                                      p_extend->p_global_cfg->rx_fifo_config[p_context->p_transfer_cfg->
                                                                                             type];                /* PRQA S 0759 # Advisory R-19.2 */
    uint32_t threshold = 0U;

    if ((TRANSFER_EVENT_TRANS_END_FLAG == p_args->event) || (TRANSFER_EVENT_TRANS_COMPLETION == p_args->event))
    {
        /* Get RX FIFO Threshold Configuration */
        threshold =
            ((uint32_t) p_context->p_transfer_cfg->size * ((uint32_t) rx_fifo_config.rx_fifo_cfg_b.RFIGCV + 1U)) >> 3;

        args.p_context = p_context->p_ctrl->p_context;
        args.channel   = p_context->p_ctrl->p_cfg->channel;
        args.buffer    = (can_buffer_t) ((uint32_t) p_context->p_transfer_cfg->type + (uint32_t) CAN_BUFFER_RX_FIFO_0); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        r_canfd_transfer_rx_callback(p_context, &args, threshold, rx_fifo_config.rx_fifo_cfg_b.RFIM, p_dest);
    }
}

/*******************************************************************************************************************//**
 * Transmit data in Common FIFO / Transmit Queue interrupt routine called when transferring is completed.
 *
 * @param[in]     p_args     Pointer to argument.
 **********************************************************************************************************************/
static void canfd_tx_transfer_callback (transfer_callback_args_t * p_args)                       /* PRQA S 3673 # Advisory R-8.13 */
{
    canfd_transfer_context_t const * p_context = (canfd_transfer_context_t *) p_args->p_context; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    if ((TRANSFER_EVENT_TRANS_END_FLAG == p_args->event) || (TRANSFER_EVENT_TRANS_COMPLETION == p_args->event))
    {
        if (r_canfd_data_transfer_full(p_context->p_transfer_cfg))
        {
            p_context->p_transfer_cfg->rear = -1;
        }
    }
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
BSP_INTERRUPT_ATTRIBUTE void canfd_global_error_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    volatile R_CANFD0_Type     * p_canfd_reg;
    IRQn_Type irq;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                   /* PRQA S 3469 # Advisory D-4.9 */

    /* Get IRQ and context */
        irq = R_FSP_CurrentIrqGet();

    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    canfd_extended_cfg_t const * p_extend_cfg = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    uint8_t global_error_channel              = p_extend_cfg->p_global_cfg->channel_error;
    args.event = CAN_EVENT_ERR_GLOBAL;

    /* Read global error flags. */
    uint32_t cfdgerfl = p_canfd_reg->CFDGERFL; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Global errors are in the top halfword of canfd_error_t; move and preserve ECC error flags. */
    args.error = (cfdgerfl & CANFD_PRV_ERROR_GL_FLAG_REG_MASK) << CANFD_PRV_ERROR_GLOBAL_ENUM_POS;

    /* Clear global error flags. */
    p_canfd_reg->CFDGERFL = 0;                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Dummy read to ensure that interrupt event DEF, CMPOF is cleared () */
    volatile uint32_t dummy = p_canfd_reg->CFDGERFL; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_PARAMETER_NOT_USED(dummy);

    if (0U != (args.error & (uint32_t) CANFD_ERROR_GLOBAL_MESSAGE_LOST))
    {
        uint32_t            cfdfmsts = p_canfd_reg->CFDFMSTS; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        uint8_t             buffer;
        volatile uint32_t * p_sts;

        if ((cfdfmsts & R_CANFD0_CFDFMSTS_RFxMLT_Msk) != 0U)
        {
            buffer = (uint8_t) ((uint32_t) SCH1R(cfdfmsts) - 1U); /* PRQA S 3469 # Advisory D-4.9 */
            p_sts  = &p_canfd_reg->CFDRFSTS[buffer];              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            *p_sts = (uint32_t) ~R_CANFD0_CFDRFSTS_RFMLT_Msk;
        }
        else
        {
            buffer =
                (uint8_t) ((uint32_t) SCH1R((cfdfmsts & R_CANFD0_CFDFMSTS_CFkMLT_Msk) >> R_CANFD0_CFDFMSTS_CFkMLT_Pos) - /* PRQA S 3469, 4394 # Advisory D-4.9 , DP_MSR_R-10.8_UC1 */
                           1U);                                                                                          /* PRQA S 3469, 4394 # Advisory D-4.9 , DP_MSR_R-10.8_UC1 */
            p_sts  = &p_canfd_reg->CFDCFSTS[buffer];                                                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            *p_sts = ~R_CANFD0_CFDCFSTS_CFMLT_Msk;
        }

        args.buffer = (can_buffer_t) (buffer);                                                                           /* PRQA S 4342, 0662 # Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Dummy read to ensure that interrupt event is cleared. */
        dummy = *p_sts;
        FSP_PARAMETER_NOT_USED(dummy);
    }

    if (0U != (args.error & (uint32_t) CANFD_ERROR_GLOBAL_TXQ_MESSAGE_LOST))
    {
        r_canfd_handle_txqueue_error(p_ctrl, &args);
    }

    if (0U != (args.error & (uint32_t) CANFD_ERROR_GLOBAL_TXQ_OVERWRITE))
    {
        r_canfd_handle_txqueue_error(p_ctrl, &args);
    }

    if (0U != (args.error & (uint32_t) CANFD_ERROR_GLOBAL_MESSAGE_OVERWRITE))
    {
        /* Get lowest CFIFO with Message Overwrite condition and clear the flag */
        args.buffer = (can_buffer_t) ((uint32_t) SCH1R(p_canfd_reg->CFDCFMOWSTS) - 1U); /* PRQA S 3469, 4394, 4342, 0662 # Advisory D-4.9 , DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        p_canfd_reg->CFDCFSTS[args.buffer] = ~R_CANFD0_CFDCFSTS_CFMOW_Msk;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Dummy read to ensure that interrupt event is cleared. */
        dummy = p_canfd_reg->CFDCFSTS[args.buffer];                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        FSP_PARAMETER_NOT_USED(dummy);
    }

    if (0U != (args.error & (uint32_t) CANFD_ERROR_GLOBAL_TX_ENTRY_LOST))
    {
        for (uint8_t channel = 0; channel < CANFD_MAX_CHANNEL_PER_UNIT; channel++)
        {
            if (0U != p_canfd_reg->CFDTHLSTS_b[channel].THLELT)                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                p_canfd_reg->CFDTHLSTS[channel] = ~R_CANFD0_CFDTHLSTS_THLELT_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                dummy = p_canfd_reg->CFDTHLSTS[channel];                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
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
    FSP_CONTEXT_RESTORE                                     /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Channel Error ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common error function, and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_error_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    volatile R_CANFD0_Type     * p_canfd_reg;               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint8_t   channel_index;
    IRQn_Type irq;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                   /* PRQA S 3469 # Advisory D-4.9 */

    /* Get IRQ and context */
#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    irq = R_FSP_ORed_CurrentIrqGet();
#else
    irq = R_FSP_CurrentIrqGet();
#endif

    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    args.event = CAN_EVENT_ERR_CHANNEL;

    /* Read and clear SOCO, EOCO, TDCVF flags. They sit at bit 15, 9 and 8 of FDSTS, so they cannot be
     * extracted as one contiguous field, and they are packed here in the order the CANFD error enums expect. */
    uint32_t fdsts_reg = p_canfd_reg->CFDC2[channel_index].FDSTS; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint32_t fdsts     = ((fdsts_reg & R_CANFD0_CFDC2_FDSTS_TDCVF_Msk) >> R_CANFD0_CFDC2_FDSTS_TDCVF_Pos) |
                         ((fdsts_reg & R_CANFD0_CFDC2_FDSTS_EOCO_Msk) >>
                          (R_CANFD0_CFDC2_FDSTS_EOCO_Pos - 1U)) |
                         ((fdsts_reg & R_CANFD0_CFDC2_FDSTS_SOCO_Msk) >> (R_CANFD0_CFDC2_FDSTS_SOCO_Pos - 2U));

    /* PRQA S 0662 ++ # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_canfd_reg->CFDC2[channel_index].FDSTS &=
        ~(R_CANFD0_CFDC2_FDSTS_TDCVF_Msk | R_CANFD0_CFDC2_FDSTS_EOCO_Msk |
          R_CANFD0_CFDC2_FDSTS_SOCO_Msk); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    /* PRQA S 0662 -- */

    /* Read and clear channel error flags. */

    uint32_t cherfl = p_canfd_reg->CFDC[channel_index].ERFL & CANFD_PRV_ERROR_CH_FLAG_REG_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_canfd_reg->CFDC[channel_index].ERFL = 0;                                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    args.error = cherfl | (fdsts << CANFD_PRV_ERROR_FD_ENUM_POS);

    /* Dummy read to ensure that interrupt event is cleared. */
    volatile uint32_t dummy = p_canfd_reg->CFDC[channel_index].ERFL; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_PARAMETER_NOT_USED(dummy);

    args.channel   = p_ctrl->p_cfg->channel;
    args.p_context = p_ctrl->p_context;
    args.buffer    = (can_buffer_t) 0U; /* PRQA S 4342, 0662 # Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Set remaining arguments and call callback */
    r_canfd_call_callback(p_ctrl, &args);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE                                                                        /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Receive ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common receive function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_rx_fifo_isr (void)                                          /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                                                                           /* PRQA S 3469 # Advisory D-4.9 */

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    can_callback_args_t args = {0};

    /* Get extended config */
    canfd_extended_cfg_t const * p_extend = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get lowest FIFO requesting interrupt */
    uint32_t fifo = (uint32_t) SCH1R(p_extend->p_global_cfg->p_canfd_reg->CFDRFISTS) - 1U;    /* PRQA S 3469, 0662 # Advisory D-4.9 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Only perform ISR duties if a FIFO has requested it */
    if (fifo < CANFD_PRV_RX_FIFO_MAX)
    {
        /* Set static arguments */

        args.event  = CAN_EVENT_RX_COMPLETE;
        args.buffer = (can_buffer_t) (fifo + (uint32_t) CAN_BUFFER_RX_FIFO_0); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Get channel associated */
        args.channel = p_extend->p_global_cfg->rx_fifo_cb_channel[fifo];

        if (NULL != gp_ctrl[args.channel])
        {
            /* Set the remaining callback arguments */
            args.p_context = gp_ctrl[args.channel]->p_context;
            r_canfd_call_callback(gp_ctrl[args.channel], &args);
        }

        /* Clear RX FIFO Interrupt Flag */
        p_extend->p_global_cfg->p_canfd_reg->CFDRFSTS[fifo] &= ~R_CANFD0_CFDRFSTS_RFIF_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Dummy read to ensure that interrupt event is cleared. */
        volatile uint32_t dummy = p_extend->p_global_cfg->p_canfd_reg->CFDRFSTS[fifo];      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        FSP_PARAMETER_NOT_USED(dummy);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE                                      /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Common FIFO receive complete ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common receive function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_rx_common_fifo_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    uint8_t channel_index;
    volatile R_CANFD0_Type      * p_canfd_reg;
    can_cfg_t const             * p_cfg;
    canfd_instance_ctrl_t const * p_ctrl;
    uint32_t fifo_num;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                   /* PRQA S 3469 # Advisory D-4.9 */

#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
#endif

    p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    can_callback_args_t args = {0};

    /* Get config struct */
    p_cfg = (can_cfg_t *) p_ctrl->p_cfg;                     /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Get status of global receive interrupt */
    uint32_t channel_int_status = p_global_cfg->p_canfd_reg->CFDGRINTSTS[channel_index]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CFIFO FDC level Full and RX interrupt */
    if ((0U != (channel_int_status & R_CANFD0_CFDGRINTSTS_CFRIF_Msk)) ||
        (0U != (channel_int_status & R_CANFD0_CFDGRINTSTS_CFRFIF_Msk)))
    {
        /* Get lowest Common FIFO requesting interrupt in this channel*/
        uint32_t cfifo_rx_ists = p_canfd_reg->CFDCFRISTS >> (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        fifo_num = (uint32_t) SCH1R(cfifo_rx_ists) - 1U;                                                           /* PRQA S 3469 # Advisory D-4.9 */

        /* Only perform ISR duties if a FIFO has requested it */
        if (fifo_num < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)
        {
            uint32_t cfifo_index = fifo_num + (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);

            /* Set static arguments */
            args.channel = p_ctrl->p_cfg->channel;
            args.buffer  = (can_buffer_t) ((fifo_num + CANFD_PRV_CFIFO_OFFSET) | (uint32_t) CAN_BUFFER_TYPE_RX_FIFO); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Handle CFIFO in receive mode */
            if ((uint32_t) CANFD_CFIFO_MODE_RECEIVE == p_canfd_reg->CFDCFCC_b[cfifo_index].CFM)                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                args.event = CAN_EVENT_RX_COMPLETE;

                /* Set the remaining callback arguments */
                args.p_context = p_ctrl->p_context;
                r_canfd_call_callback(p_ctrl, &args);
            }
            else if ((uint32_t) CANFD_CFIFO_MODE_GATEWAY == p_canfd_reg->CFDCFCC_b[cfifo_index].CFM) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                if (1U == p_canfd_reg->CFDCFCC_b[cfifo_index].CFIM)                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
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
            p_canfd_reg->CFDCFSTS[cfifo_index] = ~R_CANFD0_CFDCFSTS_CFRXIF_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Dummy read to ensure that interrupt event is cleared. */
            volatile uint32_t dummy = p_canfd_reg->CFDCFSTS[cfifo_index];       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            FSP_PARAMETER_NOT_USED(dummy);
        }
    }
    /* Transmit Queue one frame reception interrupt. */
    else if ((0U != (channel_int_status & R_CANFD0_CFDGRINTSTS_QOFRIF_Msk)) ||
             (0U != (channel_int_status & R_CANFD0_CFDGRINTSTS_BQOFRIF_Msk)))
    {
        r_canfd_handle_txqueue_one_frame(p_ctrl, &args, p_canfd_reg, (uint32_t) R_CANFD0_CFDGRINTSTS_QOFRIF_Msk);
    }
    else
    {
        /* Do nothing */
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE                                  /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Transmit ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common transmit function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_tx_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    canfd_extended_cfg_t const  * p_extend;
    can_callback_args_t           args = {0};
    canfd_instance_ctrl_t const * p_ctrl;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                   /* PRQA S 3469 # Advisory D-4.9 */

#if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
#else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
#endif

    p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Set static arguments */
    args.channel   = p_ctrl->p_cfg->channel;
    args.p_context = p_ctrl->p_context;

    /* Get extended config */
    p_extend = (canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    volatile R_CANFD0_Type * p_canfd_reg = p_extend->p_global_cfg->p_canfd_reg;

    /* Check the byte of CFDGTINTSTS0 that corresponds to the interrupting channel */
    volatile uint8_t const * p_channel_int_status = (uint8_t *) &p_canfd_reg->CFDGTINTSTS32[0]; /* PRQA S 0310, 0311, 0312, 0662 # DP_MSR_R-11.3_UC1 , DP_MSR_R-11.8_UC1 , DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint8_t channel_int_status = p_channel_int_status[p_ctrl->channel_index];

    while (0U != channel_int_status)
    {
        /* Handle Transmit History Event */
        if (((channel_int_status & R_CANFD0_CFDGTINTSTS32_THIF0_Msk) != 0U) ||
            (0U == p_canfd_reg->CFDTHLSTS_b[p_ctrl->channel_index].THLEMP)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            r_canfd_handle_txhistory(p_ctrl, &args);
        }
        /* Handle Transmit Message Buffer Tx complete Event */
        else if (0U != (channel_int_status & R_CANFD0_CFDGTINTSTS32_TSIF0_Msk))
        {
            r_canfd_handle_txmb(p_ctrl, &args, CAN_EVENT_TX_COMPLETE);
        }
        /* Handle Transmit Message Buffer Tx Abort Event */
        else if (0U != (channel_int_status & R_CANFD0_CFDGTINTSTS32_TAIF0_Msk))
        {
            r_canfd_handle_txmb(p_ctrl, &args, CAN_EVENT_TX_ABORTED);
        }
        /* Handle Transmit Queue Event (One Frame Mode Disable) */
        else if (0U != (channel_int_status & R_CANFD0_CFDGTINTSTS32_TQIF0_Msk))
        {
            r_canfd_handle_txqueue(p_ctrl, &args, p_canfd_reg);
        }
        /* Handle Transmit Queue Event (One Frame Mode Enable) */
        else if (0U != (channel_int_status & R_CANFD0_CFDGTINTSTS32_TQOFIF0_Msk))
        {
            r_canfd_handle_txqueue_one_frame(p_ctrl, &args, p_canfd_reg, (uint32_t) R_CANFD0_CFDGTINTSTS32_TQOFIF0_Msk);
        }
        /* Handle Common Fifo Transmit Event */
        else if (0U != (channel_int_status & R_CANFD0_CFDGTINTSTS32_CFTIF0_Msk))
        {
            r_canfd_handle_txfifo(p_ctrl, p_extend, &args, p_canfd_reg);
        }
        else
        {
            /*
             * Do nothing
             */
        }

        /* Check for more interrupts on this channel */
        channel_int_status = p_channel_int_status[p_ctrl->channel_index];
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE                /* PRQA S 3469 # Advisory D-4.9 */
}

#ifdef BSP_FEATURE_CANFD_FFI_MODE

/*******************************************************************************************************************//**
 * Transmit VM ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common transmit function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_channel_tx_vm_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                                        /* PRQA S 3469 # Advisory D-4.9 */

 #if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
 #else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
 #endif

    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */
    canfd_extended_cfg_t const  * p_extend;
    canfd_global_cfg_t const    * p_global_cfg;

    uint8_t channel_index;
    volatile R_CANFD0_Type const * p_canfd_reg;

    /* Set static arguments */
    can_callback_args_t args = {0};
    args.channel   = p_ctrl->p_cfg->channel;
    args.p_context = p_ctrl->p_context;

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Check the byte of CFDGTINTSTS0 that corresponds to the interrupting channel */
    volatile uint8_t const * p_channel_int_status = (uint8_t *) &p_canfd_reg->CFDVMISTS[channel_index]; /* PRQA S 0310, 0311, 0312, 0662 # DP_MSR_R-11.3_UC1 , DP_MSR_R-11.8_UC1 , DP_MSR_R-11.8_UC1 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint8_t channel_int_status = *p_channel_int_status;

    while (0U != channel_int_status)
    {
        /* Handle Transmit History Event */
        if ((0U != (channel_int_status & R_CANFD0_CFDVMISTS_THLIF_Msk)) ||
            (0U == p_canfd_reg->CFDTHLSTS_b[channel_index].THLEMP)) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            r_canfd_handle_txhistory(p_ctrl, &args);
        }
        else if (0U != (channel_int_status & R_CANFD0_CFDVMISTS_CFTXINT_Msk))
        {
            r_canfd_handle_txfifo(p_ctrl, p_extend, &args, p_extend->p_global_cfg->p_canfd_reg);
        }
        else if (0U != (channel_int_status & R_CANFD0_CFDVMISTS_TXQTXIF_Msk))
        {
            r_canfd_handle_txqueue(p_ctrl, &args, p_extend->p_global_cfg->p_canfd_reg);
        }
        else if (0U != (channel_int_status & R_CANFD0_CFDVMISTS_TXQOFTXIF_Msk))
        {
            r_canfd_handle_ffi_txqueue_one_frame(p_ctrl,
                                                 &args,
                                                 p_extend->p_global_cfg->p_canfd_reg,
                                                 (uint32_t) R_CANFD0_CFDVMISTS_TXQOFTXIF_Msk);
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
    FSP_CONTEXT_RESTORE                             /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Common FIFO receive and RX FIFO virtual machine complete ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common receive function
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_vm_rx_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    uint8_t channel_index;
    volatile R_CANFD0_Type * p_canfd_reg;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                   /* PRQA S 3469 # Advisory D-4.9 */

 #if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    IRQn_Type irq = R_FSP_ORed_CurrentIrqGet();
 #else
    IRQn_Type irq = R_FSP_CurrentIrqGet();
 #endif

    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    /* Read virtual machine interrupt status. */
    uint32_t cfdvmists = p_canfd_reg->CFDVMISTS[channel_index]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if ((0U != (cfdvmists & R_CANFD0_CFDVMISTS_RFIF_Msk)) || (0U != (cfdvmists & R_CANFD0_CFDVMISTS_RFFIF_Msk)))
    {
        /* Get lowest FIFO requesting interrupt */
        uint32_t fifo = (uint32_t) SCH1R(p_canfd_reg->CFDRFISTS) - 1U; /* PRQA S 3469, 0662 # Advisory D-4.9 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Only perform ISR duties if a FIFO has requested it */
        if (fifo < CANFD_PRV_RX_FIFO_MAX)
        {
            /* Set static arguments */

            args.event  = CAN_EVENT_RX_COMPLETE;
            args.buffer = (can_buffer_t) (fifo + (uint32_t) CAN_BUFFER_RX_FIFO_0); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Get channel associated with the AFL entry */
            args.channel = p_global_cfg->rx_fifo_cb_channel[fifo];

            /* Set the remaining callback arguments */
            args.p_context = gp_ctrl[args.channel]->p_context;
            r_canfd_call_callback(gp_ctrl[args.channel], &args);

            /* Clear RX FIFO Interrupt Flag */
            p_canfd_reg->CFDRFSTS[fifo] &= ~R_CANFD0_CFDRFSTS_RFIF_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Dummy read to ensure that interrupt event is cleared. */
            volatile uint32_t dummy = p_canfd_reg->CFDRFSTS[fifo];      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            FSP_PARAMETER_NOT_USED(dummy);
        }
    }
    else if ((0U != (cfdvmists & R_CANFD0_CFDVMISTS_CFRXINT_Msk)) || (0U != (cfdvmists & R_CANFD0_CFDVMISTS_CFFIF_Msk)))
    {
        /* Get lowest Common FIFO requesting interrupt in this channel*/
        uint32_t cfifo_rx_ists = p_canfd_reg->CFDCFRISTS >> (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        uint32_t fifo_num      = (uint32_t) SCH1R(cfifo_rx_ists) - 1U;                                             /* PRQA S 3469 # Advisory D-4.9 */

        /* Only perform ISR duties if a CFIFO has requested it */
        if (fifo_num < BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL)
        {
            uint32_t cfifo_index = fifo_num + (channel_index * BSP_FEATURE_CANFD_NUM_CFIFO_CHANNEL);

            /* Set static arguments */
            args.channel = p_ctrl->p_cfg->channel;
            args.buffer  = (can_buffer_t) ((fifo_num + CANFD_PRV_CFIFO_OFFSET) | (uint32_t) CAN_BUFFER_TYPE_RX_FIFO); /* PRQA S 4394, 4342, 0662 # DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Handle CFIFO in receive mode */
            if ((uint32_t) CANFD_CFIFO_MODE_RECEIVE == p_canfd_reg->CFDCFCC_b[cfifo_index].CFM)                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                args.event = CAN_EVENT_RX_COMPLETE;

                /* Set the remaining callback arguments */
                args.p_context = p_ctrl->p_context;
                r_canfd_call_callback(p_ctrl, &args);
            }
            else if ((uint32_t) CANFD_CFIFO_MODE_GATEWAY == p_canfd_reg->CFDCFCC_b[cfifo_index].CFM) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                if (1U == p_canfd_reg->CFDCFCC_b[cfifo_index].CFIM)                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
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
            p_canfd_reg->CFDCFSTS[cfifo_index] = ~R_CANFD0_CFDCFSTS_CFRXIF_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Dummy read to ensure that interrupt event is cleared. */
            volatile uint32_t dummy = p_canfd_reg->CFDCFSTS[cfifo_index];       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            FSP_PARAMETER_NOT_USED(dummy);
        }
    }
    /* Transmit Queue one frame reception */
    else if (0U != (cfdvmists & R_CANFD0_CFDVMISTS_TXQOFRXIF_Msk))
    {
        r_canfd_handle_ffi_txqueue_one_frame(p_ctrl,
                                             &args,
                                             p_extend->p_global_cfg->p_canfd_reg,
                                             (uint32_t) R_CANFD0_CFDVMISTS_TXQOFRXIF_Msk);
    }
    else
    {
        /* Do nothing */
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE                              /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Virtual Machine Error ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls common error function, and restores context if RTOS is used.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void canfd_vm_err_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    canfd_extended_cfg_t const * p_extend;
    canfd_global_cfg_t const   * p_global_cfg;
    volatile R_CANFD0_Type     * p_canfd_reg;
    uint8_t           channel_index;
    IRQn_Type         irq;
    volatile uint32_t dummy;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE                   /* PRQA S 3469 # Advisory D-4.9 */

    /* Get IRQ and context */
 #if BSP_FEATURE_DEVICE_HAS_ORED_IRQ
    irq = R_FSP_ORed_CurrentIrqGet();
 #else
    irq = R_FSP_CurrentIrqGet();
 #endif

    canfd_instance_ctrl_t const * p_ctrl = (canfd_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    can_callback_args_t args = {0};

    /* Get config struct */
    can_cfg_t const * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;   /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */

    /* Get extended config */
    p_extend     = (canfd_extended_cfg_t *) p_cfg->p_extend; /* PRQA S 0311, 0316 # DP_MSR_R-11.8_UC1 , Advisory R-11.5 */
    p_global_cfg = p_extend->p_global_cfg;

    /* Get the CANFD register block base */
    p_canfd_reg = p_global_cfg->p_canfd_reg;

    /*Take index for channel when device have more than one unit*/
    channel_index = p_ctrl->channel_index;

    args.event = CAN_EVENT_ERR_CHANNEL;

    /* Read virtual machine interrupt status. */
    uint32_t cfdvmists = p_canfd_reg->CFDVMISTS[channel_index]; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* RXFIFO Message Lost*/
    if (0U != (cfdvmists & R_CANFD0_CFDVMISTS_RFMLT_Msk))
    {
        /* Get lowest RX FIFO with Message Lost condition and clear the flag */
        args.buffer = (can_buffer_t) ((uint32_t) SCH1R(p_canfd_reg->CFDFMSTS) - 1U); /* PRQA S 3469, 4394, 4342, 0662 # Advisory D-4.9 , DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        p_canfd_reg->CFDRFSTS[args.buffer] = ~R_CANFD0_CFDRFSTS_RFMLT_Msk;           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Dummy read to ensure that interrupt event is cleared. */
        dummy = p_canfd_reg->CFDRFSTS[args.buffer];                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        FSP_PARAMETER_NOT_USED(dummy);

        args.error |= (uint32_t) CANFD_ERROR_CHANNEL_VM_FIFO_MESSAGE_LOST;
    }
    else
    {
        /* Common FIFO Message Lost*/
        if (0U != (cfdvmists & R_CANFD0_CFDVMISTS_CFMLT_Msk))
        {
            /* Get lowest CFIFO with Message Lost condition and clear the flag */
            args.buffer =                                                                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                          (can_buffer_t) ((uint32_t) SCH1R((p_canfd_reg->CFDFMSTS & R_CANFD0_CFDFMSTS_CFkMLT_Msk) >> /* PRQA S 3469, 4394, 4342, 0662 # Advisory D-4.9 , DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                                           R_CANFD0_CFDFMSTS_CFkMLT_Pos) - 1U);                      /* PRQA S 4393 # DP_MSR_R-10.8_UC1 */

            p_canfd_reg->CFDCFSTS[args.buffer] = ~R_CANFD0_CFDCFSTS_CFMLT_Msk;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Dummy read to ensure that interrupt event is cleared. */
            dummy = p_canfd_reg->CFDCFSTS[args.buffer];                                                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            FSP_PARAMETER_NOT_USED(dummy);

            args.error |= (uint32_t) CANFD_ERROR_CHANNEL_VM_TXQ_CFIFO_MESSAGE_LOST;
        }

        /* Common FIFO Message Overwrite */
        if (0U != (cfdvmists & R_CANFD0_CFDVMISTS_CFMOW_Msk))
        {
            /* Get lowest CFIFO with Message Overwrite condition and clear the flag */
            args.buffer =                                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                          (can_buffer_t) ((uint32_t) SCH1R(p_canfd_reg->CFDCFMOWSTS) - 1U); /* PRQA S 3469, 4394, 4342, 0662 # Advisory D-4.9 , DP_MSR_R-10.8_UC1 , Advisory R-10.5 , DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            p_canfd_reg->CFDCFSTS[args.buffer] = ~R_CANFD0_CFDCFSTS_CFMOW_Msk;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Dummy read to ensure that interrupt event is cleared. */
            dummy = p_canfd_reg->CFDCFSTS[args.buffer];                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            FSP_PARAMETER_NOT_USED(dummy);

            args.error |= (uint32_t) CANFD_ERROR_CHANNEL_VM_CFIFO_MESSAGE_OVERWRITE;
        }

        /* Transmit Queue Message Lost */
        if (0U != (cfdvmists & R_CANFD0_CFDVMISTS_TXQMLT_Msk))
        {
            r_canfd_handle_txqueue_ffi_error(p_ctrl, &args);
        }

        /* Transmit Queue Message Overwrite */
        if (0U != (cfdvmists & R_CANFD0_CFDVMISTS_TXQMOW_Msk))
        {
            r_canfd_handle_txqueue_ffi_error(p_ctrl, &args);
        }

        /* Transmit Histroy List Entry Lost*/
        if (0U != (cfdvmists & R_CANFD0_CFDVMISTS_THLELT_Msk))
        {
            p_canfd_reg->CFDTHLSTS[channel_index] = ~R_CANFD0_CFDTHLSTS_THLELT_Msk; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            dummy = p_canfd_reg->CFDTHLSTS[channel_index];                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
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
    FSP_CONTEXT_RESTORE                /* PRQA S 3469 # Advisory D-4.9 */
}

#endif
