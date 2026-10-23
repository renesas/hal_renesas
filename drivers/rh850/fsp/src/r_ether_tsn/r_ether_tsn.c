/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <string.h>
#include "r_ether_tsn.h"               /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "ETND" in ASCII, used to determine if channel is open. */
#define ETHER_TSNES_REG_SIZE                  (0x10000)
#define ETHER_AXIBMI_REG_SIZE                 (0x10000)
#define ETHER_RMACA_REG_SIZE                  (0x10000)
#define ETHER_PWRCTL_REG_SIZE                 (0x400)
#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
 #define ETHER_TSN_CHANNEL_1                  (1U)
 #define ETHER_TSN_SGSRST_SRST                (0x00000001U)
 #define ETHER_TSN_SGSDS_PWS_ON               (0x03U)
 #define ETHER_TSN_WAIT_FOR_SGMII             (4U)
 #define ETHER_TSN_SGOPMC_10M                 (0x00000003U)
 #define ETHER_TSN_SGOPMC_100M                (0x00000007U)
 #define ETHER_TSN_SGOPMC_1G                  (0x0000000BU)
 #define ETHER_TSN_SGINTM_ENABLE              (0x00000000U)
 #define ETHER_TSN_SGCLKSEL_SEL               (0x01U)
 #define ETHER_TSN_SGRCIE_RCIE                (0x01U)
 #define ETHER_TSN_SGSDS_SUC                  (0x04U)
 #define ETHER_TSN_SGINTS_URDY                (0x0002U)
 #define ETHER_TSN_SGINTS_RDY                 (0x0001U)
 #define ETHER_TSN_SGINTS_RDY_UNRDY           (ETHER_TSN_SGINTS_RDY | ETHER_TSN_SGINTS_URDY)
 #define ETH_TSN_SGOPMS_DATA_COM              (0x00020000U)
 #define ETH_TSN_SGOPMS_XMIT_MASK             (0x00030000U)
 #define ETH_TSN_SGOPMS_SPEED_MASK            (0x00000038U)
#endif
#define ETHER_TSN_OPEN                        (0x45544E44U)
#define ETHER_TSN_TOTAL_CTRL_CONFIG           (BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
#define ETHER_TSN_AXIWC_CONFIG                (0x00014411U)
#define ETHER_TSN_AXIRC_CONFIG                (0x00011144U)
#define ETHER_TSN_TDPC_CONFIG                 (0x76543210U)
#define ETHER_TSN_TFT_CONFIG                  (0x00000000U)
#define ETHER_TSN_INIT_TFS0_CONFIG            (0x00000100U)
#define ETHER_TSN_INIT_TMS_CONFIG             (0x00004000U)
#define ETHER_TSN_MCC_TXE_MASK                (0x00000002U)
#define ETHER_TSN_INIT_DESC_NUM               (16U)
#define ETHER_TSN_EOS_DESC_NUM                (1U)
#define ETHER_TSN_INIT_DESC_LOOP_NUM          (2U)
#define ETHER_TSN_MIDDLE_DESC_NUM             (6U)
#define ETHER_TSN_TRCR0_TRANSMIT_ENABLE       (0x00000001U)
#define ETHER_TSN_TDIS_0_MASK                 (0x00000001U)
#define ETHER_TSN_RDIS_0_MASK                 (0x00000001U)
#define ETHER_TSN_SOFTWARE_RESET_ENABLE       (0x00000001U)
#define ETHER_TSN_GIS_SWRCF_MASK              (0x00010000U)
#define ETHER_TSN_32BIT_ALIGN                 (4U)
#define ETHER_TSN_ALIGN_TO_CPU(n)    ((((n) + ETHER_TSN_32BIT_ALIGN) - 1U) & 0xFFFFFFFCU) /* PRQA S 3472 # Advisory D-4.9 */
#define ETHER_TSN_FALSE                       (0U)
#define ETHER_TSN_TRUE                        (1U)
#define ETHER_TSN_COUNTER_MAX_VALUE           (0xFFFFU)
#define ETHER_TSN_TIMEOUT_COUNT               (0xF6B6U)
#define ETHER_TSN_INIT_COUNTER_VALUE          (0U)
#define ETHER_TSN_DESC_FCS_INCLUDED           (0x00000001U)
#define ETHER_TSN_INIT_FRAME_LENGTH           (0x00004000U)
#define ETHER_TSN_DESC_FEMPTY_ND              (3U)
#define ETHER_TSN_DESC_FEMPTY                 (4U)
#define ETHER_TSN_DESC_FSINGLE                (8U)
#define ETHER_TSN_DESC_FSTART                 (9U)
#define ETHER_TSN_DESC_FMID                   (10U)
#define ETHER_TSN_DESC_FEND                   (11U)
#define ETHER_TSN_DESC_LEMPTY                 (12U)
#define ETHER_TSN_DESC_LINK                   (14U)
#define ETHER_TSN_DESC_EOS                    (15U)
#define ETHER_TSN_TATLS0_CONFIG               (0x00000002U)
#define ETHER_TSN_TATLR_LEARNING              (0x80000000U)
#define ETHER_TSN_TATLR_FAILED                (0x00000001U)
#define ETHER_TSN_RATLS0_CONFIG               (0x0000000CU)
#define ETHER_TSN_RATLR_LEARNING              (0x80000000U)
#define ETHER_TSN_RATLR_FAILED                (0x00000001U)
#define ETHER_TSN_RATSR2_SEARCHING            (0x80000000U)
#define ETHER_TSN_RATSR0_CHAIN_ENTRY_VALID    (0x00000040U)
#define ETHER_TSN_MHD_DISABLE_MODE            (0x00000000U)
#define ETHER_TSN_MHD_CONFIG_MODE             (0x00000001U)
#define ETHER_TSN_MHD_OPERATION_MODE          (0x00000002U)
#define ETHER_TSN_RATRR_RESET                 (0x00000001U)
#define ETHER_TSN_TATRR_RESET                 (0x00000002U)
#define ETHER_TSN_EIE0_CONFIG                 (0x00000003U)
#define ETHER_TSN_DIE_CONFIG                  (0x00010101U)
#define ETHER_TSN_TDIE0_CONFIG                (0x000000FFU)
#define ETHER_TSN_RDIE0_CONFIG                (0x00000001U)
#define ETHER_TSN_RFEIE0_CONFIG               (0x00000001U)
#define ETHER_TSN_CYCLIC_DESC_NUM             (1U)
#define ETHER_TSN_EXT_DESC_SIZE               (16U)
#define ETHER_TSN_EXT_DESC_TS_SIZE            (24U)
#define ETHER_TSN_RX_DPTR_OFFSET              (2U)
#define ETHER_TSN_TX_DESC_DIE_ENABLE          (1U)
#define ETHER_TSN_RX_DESC_DIE_ENABLE          (1U)
#define ETHER_TSN_TCIE_CONFIG                 (0x00000400U)
#define ETHER_TSN_TIE2_CONFIG                 (0xFF010000U)
#define ETHER_TSN_RIE_CONFIG                  (0x00000010U)
#define ETHER_TSN_TGC1_CONFIG                 (0x0000FF05U)
#define ETHER_TSN_TCR_CONFIG                  (0x00000000U)
#define ETHER_TSN_TFS_CONFIG                  (0x00200020)
#define ETHER_TSN_CBS_CONFIG_NUM              (0x08U)
#define ETHER_TSN_RGC_CONFIG                  (0x88A80000U)
#define ETHER_TSN_RDFCR_CONFIG                (0x04000800U)
#define ETHER_TSN_RCFCR_CONFIG                (0x00800100U)
#define ETHER_TSN_GPTP_TIMER_DOMAIN           (0x00000000U)
#define ETHER_TSN_TRANSMISSION_MIN_LATENCY    (0x480U)
#define ETHER_TSN_TRANSMISSION_JITTER         (0x48U)
#define ETHER_TSN_MLBC_CONFIG                 (0x00000001U)
#define ETHER_TSN_MRAFC_DA_SA_SAME_FRAMES     (0x00000100U)
#define ETHER_TSN_MTATC_CONFIG                (0x02000000U)
#define ETHER_TSN_MTFFC_CONFIG                (0x00000000U)

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
 #define ETHER_TSN_MTRC_CONFIG                (0x00020000U)
#else
 #define ETHER_TSN_MTRC_CONFIG                (0x03020000U)
#endif

#define ETHER_TSN_MRSCE_CONFIG                ((0x003FU << 16U) | (0x003FU))
#define ETHER_TSN_MRSCP_CONFIG                ((0x003FU << 16U) | (0x003FU))
#define ETHER_TSN_MPIC_CONFIG                 ((0x1UL << 28U) | (0x2UL << 24U) | (0x13UL << 16U))
#define ETHER_TSN_TX_BUF_TOTAL                (64U)
#define ETHER_TSN_MACADDR_SIZE                (6U)
#define ETHER_TSN_FCS_LENGTH                  (4U)
#define ETHER_TSN_BYTE_BITS                   (8U)
#define ETHER_TSN_NOT_OK                      (0x01U)
#define ETHER_TSN_OK                          (0x00U)
#define ETHER_TSN_HEADER_SIZE                 (14U)
#define ETHER_TSN_TX_DESC_TCX_ENABLE          (1U)
#define ETHER_TSN_TX_DESC_TN_DOMAIN           (ETHER_TSN_GPTP_TIMER_DOMAIN)
#define ETHER_TSN_MIN_PAYLOAD_SIZE            (14U)
#define ETHER_TSN_SRC_DST_ADDRESS_SIZE        (12UL)
#define ETHER_TSN_ADDRESS_SIZE                (6UL)
#define ETHER_TSN_PROMISCUOUS_MODE            (0x00010001U)
#define ETHER_TSN_MLVC_CONFIG                 ((0x00000100U) | 9U)
#define ETHER_TSN_TX_BUFFER_MAX               (8U)
#define ETHER_TSN_MLVC_PLV_MASK               (0x00020000U)
#define ETHER_TSN_INFO0_SEI_MASK              (0x00000002U)
#define ETHER_TSN_TX_DESCR_TSUN_MASK          (0x00000FF0U)
#define ETHER_TSN_MRMAC_CONFIG_MAC            (16UL)
#define ETHER_TSN_PACK_ADDRESS_FROM_8(src, dst) /* PRQA S 3472 # Advisory D-4.9 */                           \
    do                                                                                                       \
    {                                                                                                        \
        (dst).h32 =                                                                                          \
            ((uint32_t) (src)[0U] << 24UL) | ((uint32_t) (src)[1U] << 16UL) | ((uint32_t) (src)[2U] << 8U) | \
            (uint32_t) (src)[3U];                                                                            \
        (dst).l16 = ((uint32_t) (src)[4U] << 8UL) | (uint32_t) (src)[5U];                                    \
    } while (0)

#define ETHER_TSN_SRC_MACADDR_LEN             (6U)
#define ETHER_TSN_CHECK_MULTICAST(addr)                     ((addr).h32 & 0x01000000UL)                   /* PRQA S 3472 # Advisory D-4.9 */

#define ETHER_TSN_GET_RX_FULL_INT_STAT(que, val, offset)                                                  /* PRQA S 3472 # Advisory D-4.9 */ \
    ((uint32_t) (val) & ((1UL << (uint32_t) (que)) << (uint32_t) (offset)))

#define ETHER_TSN_COMPARE_MAC(a, b)                         (((a).h32 ^ (b).h32) | ((a).l16 ^ ((b).l16))) /* PRQA S 3472 # Advisory D-4.9 */
#define ETHER_TSN_MAX_FILTERS                (32U)
#define ETHER_TSN_INVALID_FILTER_INDEX       (0xFFFFFFFFU)
#define ETHER_TSN_CONVERT_TO_SMALL_BIT       (0xFFFU)
#define ETHER_TSN_STATUS_PKT_LENGHT_640      (64U)                                                        /* PRQA S 1534 # Rule-2.5 */
#define ETHER_TSN_STATUS_PKT_LENGHT_1270     (127U)                                                       /* PRQA S 1534 # Rule-2.5 */
#define ETHER_TSN_STATUS_PKT_LENGHT_2550     (255U)                                                       /* PRQA S 1534 # Rule-2.5 */
#define ETHER_TSN_STATUS_PKT_LENGHT_5110     (511U)                                                       /* PRQA S 1534 # Rule-2.5 */
#define ETHER_TSN_STATUS_PKT_LENGHT_10230    (1023U)                                                      /* PRQA S 1534 # Rule-2.5 */
#define ETHER_TSN_NULL_PTR                   ((void *) NULL)
#define ETHER_TSN_WAIT_LINK_STABLE           (500U)
#define ETHER_TSN_MTPFC_PFTTZ_POS            (24U)
#define ETHER_TSN_MTPFC_PFRT_POS             (16U)
#define ETHER_TSN_MTPFC_PT_POS               (0U)
#define ETHER_TSN_DESC_TOTAL                 (64U)
#define ETHER_TSN_TS_DESC_DIE_ENABLE         (1U)
#define ETHER_TSN_ETND_TSS_CONFIG            (0x00000005U)
#define ETHER_TSN_ETND_TSDIE_CONFIG          (0x00000001U)
#define ETHER_TSN_ETND_TSDIS_0_MASK          (0x00000001U)
#define ETHER_TSN_BROADCAST_H32              (0xFFFFFFFFU)
#define ETHER_TSN_BROADCAST_L16              (0x0000FFFFU)
#define ETHER_TSN_NULL_ADDR                  (0x00000000U)
#define ETHER_TSN_2KB_FRAME_SIZE             (0x00000800U)
#define ETHER_TSN_MAX_TX_PRIORITY            (7U)
#define ETHER_TSN_MAX_RX_QUEUE               (1U)

/* Function to get  */
#define ETHER_TSN_SEARCH_MSB_1(val)    ((uint32_t) (SCH1R(val)) - 1U) /* PRQA S 3472 # Advisory D-4.9 */
#define ETHER_TSN_GID_DISABLE_ISR            (3U)
#define ETHER_TSN_GIS_CLEAR_ISR              (3U)

/*******************************************************************************************************************//**
 * Private global variables
 ***********************************************************************************************************************/

static uint32_t tsn_counter_check = 0;                                                 /* PRQA S 3218, 1527 # Advisory R-8.9, Advisory R-5.9 */

uint32_t g_ether_tsn_tx_ram_init_state[ETHER_TSN_TOTAL_CTRL_CONFIG];                   /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
uint32_t g_ether_tsn_rx_full = ETHER_TSN_FALSE;                                        /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
ether_tsn_controller_status_type_t g_ether_tsn_ctrl_stat[ETHER_TSN_TOTAL_CTRL_CONFIG]; /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */

/* LINK descriptor closing each RX descriptor ring */
static ether_tsn_ext_desc_with_ts_type_t * gp_ether_tsn_rx_link_desc[ETHER_TSN_TOTAL_CTRL_CONFIG][ETHER_TSN_MAX_RX_QUEUE];

void * gp_ether_tsn_heap[ETHER_TSN_TOTAL_CTRL_CONFIG];                                 /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
ether_tsn_rx_frame_type_t  g_ether_tsn_rx_frame[ETHER_TSN_TOTAL_CTRL_CONFIG];          /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
ether_tsn_tx_buffer_type_t g_ether_tsn_tx_buffer_mgr_0[ETHER_TSN_TX_BUF_TOTAL];        /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
ether_tsn_tx_buffer_type_t g_ether_tsn_tx_buffer_mgr_1[ETHER_TSN_TX_BUF_TOTAL];
#endif
ether_tsn_tx_buffer_type_t * gp_ether_tsn_tx_buffer_mgr_table[] =                      /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
{
    g_ether_tsn_tx_buffer_mgr_0,                                                       /* Channel 0 */
#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
    g_ether_tsn_tx_buffer_mgr_1                                                        /* Channel 1 */
#endif
};

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
ether_tsn_ext_desc_with_ts_type_t g_ether_tsn_desc_ts_chain_0[ETHER_TSN_DESC_TOTAL + 1U]; /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
 #if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
ether_tsn_ext_desc_with_ts_type_t g_ether_tsn_desc_ts_chain_1[ETHER_TSN_DESC_TOTAL + 1U];
 #endif
ether_tsn_ext_desc_with_ts_type_t * gp_ether_tsn_desc_ts_chain_table[] =                  /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
{
    g_ether_tsn_desc_ts_chain_0,                                                          /* Channel 0 */
 #if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
    g_ether_tsn_desc_ts_chain_1                                                           /* Channel 1 */
 #endif
};
#endif

uint32_t g_ether_tsn_tx_buffer_total[] = /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
{
    (uint32_t) ETHER_TSN_TX_BUF_TOTAL,
#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
    (uint32_t) ETHER_TSN_TX_BUF_TOTAL,
#endif
};

ether_tsn_mac_address_type_t g_ether_tsn_ts_desc_total = /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
{
    ETHER_TSN_BROADCAST_H32,
    ETHER_TSN_BROADCAST_L16
};
ether_tsn_mac_address_type_t g_ether_tsn_null_addr = /* PRQA S 3408,1514 # DP_MSR_R-8.4_UC1,Advisory R-8.7,Advisory R-8.9 */
{
    ETHER_TSN_NULL_ADDR,
    ETHER_TSN_NULL_ADDR
};
ether_tsn_mac_address_type_t g_ether_tsn_address_filters[ETHER_TSN_TOTAL_CTRL_CONFIG][ETHER_TSN_MAX_FILTERS];  /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
uint8_t                    g_ether_tsn_rx_src_addr[ETHER_TSN_TOTAL_CTRL_CONFIG][ETHER_TSN_SRC_MACADDR_LEN];    /* PRQA S 3408,1514 # DP_MSR_R-8.4_UC1,Advisory R-8.7,Advisory R-8.9 */
ether_tsn_frame_type       g_ether_tsn_rx_frame_type[ETHER_TSN_TOTAL_CTRL_CONFIG];                             /* PRQA S 3408,1514 # DP_MSR_R-8.4_UC1,Advisory R-8.7,Advisory R-8.9 */
uint16_t                   g_ether_tsn_rx_frame_cnt[ETHER_TSN_TOTAL_CTRL_CONFIG];                              /* PRQA S 3408,1514 # DP_MSR_R-8.4_UC1,Advisory R-8.7,Advisory R-8.9 */
uint16_t                   g_ether_tsn_rx_len_byte[ETHER_TSN_TOTAL_CTRL_CONFIG];                               /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
uint8_t                    g_ether_tsn_tx_confirmed[ETHER_TSN_TOTAL_CTRL_CONFIG];                              /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
ether_tsn_rx_status_type_t g_ether_tsn_rx_status[ETHER_TSN_TOTAL_CTRL_CONFIG];                                 /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
uint32_t                   g_ether_tsn_tx_buffer_addr[ETHER_TSN_TOTAL_CTRL_CONFIG];                            /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
uint32_t                   g_ether_tsn_current_tx_descr[ETHER_TSN_TOTAL_CTRL_CONFIG][ETHER_TSN_TX_BUFFER_MAX]; /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
uint8_t                    g_ether_tsn_current_priority[ETHER_TSN_TOTAL_CTRL_CONFIG];                          /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
uint8_t                  * gp_ether_tsn_user_rx_buf[ETHER_TSN_TOTAL_CTRL_CONFIG];                              /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
uint32_t                   g_ether_tsn_user_rx_buf_len[ETHER_TSN_TOTAL_CTRL_CONFIG];                           /* PRQA S 3408,1514,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7,Advisory R-8.9 */
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
ether_tsn_time_qual_type_t  g_ether_tsn_tx_time_qual;                                                          /* PRQA S 3408,1514 # DP_MSR_R-8.4_UC1,Advisory R-8.7,Advisory R-8.9 */
ether_tsn_time_qual_type_t  g_ether_tsn_rx_time_qual;                                                          /* PRQA S 3408,1514 # DP_MSR_R-8.4_UC1,Advisory R-8.7,Advisory R-8.9 */
ether_tsn_time_stamp_type_t g_ether_tsn_tx_time_stamp[ETHER_TSN_TOTAL_CTRL_CONFIG];                            /* PRQA S 3408,1514 # DP_MSR_R-8.4_UC1,Advisory R-8.7,Advisory R-8.9 */

ether_tsn_time_stamp_type_t * gp_ether_tsn_rx_time_stamp[ETHER_TSN_TOTAL_CTRL_CONFIG];                         /* PRQA S 3408,1504 # DP_MSR_R-8.4_UC1,Advisory R-8.7 */
#endif

/*******************************************************************************************************************//**
 * Private function prototypes
 **********************************************************************************************************************/
#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
static fsp_err_t r_ether_tsn_open_param_check(ether_tsn_instance_ctrl_t const * const p_ctrl,
                                              ether_cfg_t const * const               p_cfg);

#endif
static void              * r_ether_tsn_util_ram_init_ptr(void * const p_base_addr, const uint32_t ram_size);
static ether_tsn_type      r_ether_tsn_tx_ram_init(ether_tsn_instance_ctrl_t * p_ctrl, const uint32_t ctrl_idx);
static void              * r_ether_tsn_util_ram_alloc_ptr(void ** const pp_heap, const uint32_t ram_size);
static ether_tsn_type      r_ether_tsn_mhd_mode_setting(ether_tsn_instance_ctrl_t * p_ctrl, uint32_t mode);
static ether_tsn_type      r_ether_tsn_get_counter_value(ether_tsn_tick_ref_type * p_value);
static ether_tsn_tick_type r_ether_tsn_get_time_out_value(ether_tsn_tick_type time_out_count_init);
static void                r_ether_tsn_tx_ram_init_desc_config(ether_tsn_instance_ctrl_t       * p_ctrl,
                                                               ether_tsn_ext_desc_type_t * const p_desc_chain,
                                                               const uint32_t                    dptr);
static ether_tsn_type r_ether_tsn_tx_desc_learning(ether_tsn_instance_ctrl_t * p_ctrl,
                                                   uint32_t                    entry_number,
                                                   uint32_t                    chain_addr);
static ether_tsn_type r_ether_tsn_rx_desc_learning(ether_tsn_instance_ctrl_t * p_ctrl,
                                                   uint32_t                    entry_number,
                                                   uint32_t                    chain_addr);
static void           r_ether_tsn_rx_desc_relearn_if_full(ether_tsn_instance_ctrl_t * p_ctrl, uint32_t queue_idx);
static void           r_ether_tsn_util_ram_free(void ** pp_heap, void * p_mem_addr);
static ether_tsn_type r_ether_tsn_axbmi_init(ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx);
static ether_tsn_type r_ether_tsn_tx_rx_desc_config(ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx);
static void           r_ether_tsn_util_ram_set_circular_addr(void * p_heap);
static void           r_ether_tsn_mhd_init(ether_tsn_instance_ctrl_t * p_ctrl);
static void           r_ether_tsn_rmac_init(ether_tsn_instance_ctrl_t * p_ctrl);

#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
static ether_tsn_type        r_ether_tsn_sgmii_init(ether_tsn_instance_ctrl_t * p_ctrl);
BSP_INTERRUPT_ATTRIBUTE void r_ether_tsn_gerenal_isr(void);

#endif

static void           r_ether_tsn_initialize_buffer(uint32_t ctrl_idx);
static ether_tsn_type r_ether_tsn_hw_init(ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx);

static fsp_err_t r_ether_tsn_send_data(ether_tsn_instance_ctrl_t * p_ctrl,
                                       uint8_t                   * buf,
                                       uint32_t                    len_byte);
static void           r_ether_tsn_set_phys_addr(uint32_t ctrl_idx, const uint8_t mac_addr[]);
static ether_tsn_type r_ether_tsn_set_controller_mode(ether_tsn_instance_ctrl_t * p_ctrl,
                                                      uint32_t                    ctrl_idx,
                                                      ether_tsn_mode_type_t       ctrl_mode);
static void r_ether_tsn_if_rx_call(ether_tsn_instance_ctrl_t             * p_ctrl,
                                   uint32_t                                ctrl_idx,
                                   const ether_tsn_rx_frame_type_t * const p_frame);
static void r_ether_tsn_if_rx_indication(ether_tsn_instance_ctrl_t    * p_ctrl,
                                         uint32_t                       ctrl_idx,
                                         ether_tsn_frame_type           frame_type,
                                         const uint8_t                * p_phys_addr,
                                         const ether_tsn_frame_type_t * p_data,
                                         uint16_t                       len_byte);
static ether_tsn_etag_rx_ext_status_type_t r_ether_tsn_rx_queue_process(ether_tsn_instance_ctrl_t * p_ctrl,
                                                                        uint32_t                    ctrl_idx,
                                                                        uint32_t                    queue_idx);
static ether_tsn_rx_status_type_t r_ether_tsn_hw_receive(ether_tsn_instance_ctrl_t * p_ctrl,
                                                         uint32_t                    ctrl_idx,
                                                         uint32_t                    queue_idx);
static void r_ether_tsn_if_tx_confirmation(uint32_t ctrl_idx, uint32_t buf_idx);
static void r_ether_tsn_hw_tx_confirmation(ether_tsn_instance_ctrl_t * p_ctrl,
                                           uint32_t                    ctrl_idx);
static void r_ether_tsn_tx_confirmation(ether_tsn_instance_ctrl_t * p_ctrl,
                                        uint32_t                    ctrl_idx);
static void r_ether_tsn_receive(ether_tsn_instance_ctrl_t  * p_ctrl,
                                uint8_t                      queue_idx,
                                ether_tsn_rx_status_type_t * p_rx_status);
static void r_ether_tsn_call_callback(ether_tsn_instance_ctrl_t * p_instance_ctrl,
                                      ether_event_t               event);
static ether_tsn_type r_ether_tsn_check_provide_buffer(uint32_t ctrl_idx, uint32_t buf_idx);
static void           r_ether_tsn_pre_process_frame(ether_tsn_instance_ctrl_t * p_ctrl,
                                                    uint32_t                    ctrl_idx,
                                                    uint32_t                    buf_idx,
                                                    uint32_t                    frame_type,
                                                    const uint8_t * const       p_phys_addr);
static ether_tsn_type r_ether_tsn_hw_transmit(ether_tsn_instance_ctrl_t * p_ctrl,
                                              uint32_t                    ctrl_idx,
                                              uint32_t                    buf_idx,
                                              uint32_t                    len_byte,
                                              bool                        confirmation);
static ether_tsn_type r_ether_tsn_transmit(ether_tsn_instance_ctrl_t * p_ctrl,
                                           uint32_t                    ctrl_idx,
                                           ether_tsn_buf_idx_type      buf_idx,
                                           ether_tsn_frame_type        frame_type,
                                           bool                        tx_confirmation,
                                           uint16_t                    len_byte,
                                           const uint8_t             * p_phys_addr);
static ether_tsn_buf_req_return_type_t r_ether_tsn_get_tx_buffer(ether_tsn_instance_ctrl_t    * p_ctrl,
                                                                 uint32_t                       ctrl_idx,
                                                                 uint8_t                        priority,
                                                                 ether_tsn_buf_idx_type * const p_buf_idx,
                                                                 uint8_t ** const               p_buf,
                                                                 uint16_t * const               p_len_byte);
static ether_tsn_buf_req_return_type_t r_ether_tsn_provide_tx_buffer(ether_tsn_instance_ctrl_t * p_ctrl,
                                                                     uint32_t                    ctrl_idx,
                                                                     uint8_t                     priority,
                                                                     ether_tsn_buf_idx_type    * p_buf_idx,
                                                                     uint8_t                  ** p_buf,
                                                                     uint32_t                  * p_len_byte);
static ether_tsn_type r_ether_tsn_hw_enable_controller(ether_tsn_instance_ctrl_t * p_ctrl,
                                                       uint32_t                    ctrl_idx);
static void      r_ether_tsn_release_tx_buffer(uint32_t ctrl_idx, uint32_t buf_idx);
static void      r_ether_tsn_enable_icu(ether_tsn_instance_ctrl_t * const p_instance_ctrl);
static void      r_ether_tsn_disable_icu(ether_tsn_instance_ctrl_t * const p_instance_ctrl);
static fsp_err_t ether_tsn_link_status_check(
    ether_tsn_instance_ctrl_t const * const p_instance_ctrl);

#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)
static void r_ether_tsn_clear_all_address_filters(uint32_t ctrl_idx);
static void r_ether_tsn_remove_address_filter(uint32_t ctrl_idx, uint32_t filter_idx);
static void r_ether_tsn_add_address_filter(uint32_t                                   ctrl_idx,
                                           const ether_tsn_mac_address_type_t * const p_mac_addr);
static uint32_t r_ether_tsn_get_filter_index(uint32_t ctrl_idx, const ether_tsn_mac_address_type_t * const p_mac_addr);

static ether_tsn_type r_ether_tsn_update_phys_addr_filter(uint32_t                       ctrl_idx,
                                                          uint8_t                      * p_phys_addr,
                                                          ether_tsn_filter_action_type_t action);
static bool r_ether_tsn_rx_frame_valid(uint32_t ctrl_idx, uint32_t frame_addr);

#endif

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
static void           r_ether_tsn_ts_desc_config(ether_tsn_instance_ctrl_t * p_instance_ctrl, uint32_t ctrl_idx);
static ether_tsn_type r_ether_tsn_hw_get_egress_time_stamp(uint32_t                      ctrl_idx,
                                                           uint32_t                      buff_idx,
                                                           ether_tsn_time_qual_type_t  * p_time_qual,
                                                           ether_tsn_time_stamp_type_t * p_time_stamp);
static ether_tsn_type r_ether_tsn_hw_get_ingress_time_stamp(uint32_t                      ctrl_idx,
                                                            uint8_t                     * p_data,
                                                            ether_tsn_time_qual_type_t  * p_time_qual,
                                                            ether_tsn_time_stamp_type_t * p_time_stamp);

#endif
BSP_INTERRUPT_ATTRIBUTE void r_ether_tsn_data_isr(void); /* PRQA S 3451, 3449 # DP_MSR_R-8.5_UC1 */
BSP_INTERRUPT_ATTRIBUTE void r_ether_tsn_gptp_isr(void); /* PRQA S 3451, 3449 # DP_MSR_R-8.5_UC1 */

/*******************************************************************************************************************//**
 * Global Variables
 **********************************************************************************************************************/

/** ETHER_TSN HAL API mapping for Ethernet Controller interface. */
const ether_api_t g_ether_tsn_on_ether_tsn =      /* PRQA S 1502 # Advisory R-2.8 */
{
    .open           = R_ETHER_TSN_Open,           /* PRQA S 3635 # Advisory R-17.12 */
    .close          = R_ETHER_TSN_Close,          /* PRQA S 3635 # Advisory R-17.12 */
    .write          = R_ETHER_TSN_Write,          /* PRQA S 3635 # Advisory R-17.12 */
    .read           = R_ETHER_TSN_Read,           /* PRQA S 3635 # Advisory R-17.12 */
    .callbackSet    = R_ETHER_TSN_CallbackSet,    /* PRQA S 3635 # Advisory R-17.12 */
    .txStatusGet    = R_ETHER_TSN_TxStatusGet,    /* PRQA S 3635 # Advisory R-17.12 */
    .bufferRelease  = R_ETHER_TSN_BufferRelease,  /* PRQA S 3635 # Advisory R-17.12 */
    .rxBufferUpdate = R_ETHER_TSN_RxBufferUpdate, /* PRQA S 3635 # Advisory R-17.12 */
    .linkProcess    = R_ETHER_TSN_LinkProcess,    /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup ETHER_TSN
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/*******************************************************************************************************************//**
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Open the ETHER_TSN driver module.
 * Implements @ref ether_api_t::open
 * @param[in] p_ctrl                                Pointer to control block.
 * @param[in] p_cfg                                 Pointer to configuration block.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_TSN control block is NULL.
 * @retval  FSP_SUCCESS                             Channel opened successfully.
 * @retval  FSP_ERR_INVALID_POINTER                 Pointer points to invalid memory location.
 * @retval  FSP_ERR_ALREADY_OPEN                    Requested channel is already open in a different configuration.
 * @retval  FSP_ERR_INVALID_CHANNEL                 Selected channel does not exist.
 * @retval  FSP_ERR_ETHER_ERROR_PHY_COMMUNICATION   Initialization of PHY-LSI failed.
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK            Initialization of PHY-LSI failed.
 * @retval  FSP_ERR_TIMEOUT                         PHY-LSI Reset wait timeout.
 **********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_Open (ether_ctrl_t * const p_ctrl, ether_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) p_ctrl;   /* PRQA S 0316 # Advisory R-11.5 */
    volatile fsp_err_t          err             = FSP_SUCCESS;
    fsp_err_t phy_ret = FSP_SUCCESS;

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    err = r_ether_tsn_open_param_check(p_instance_ctrl, p_cfg);
    FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
#endif

    /* Initialize channel index */
    uint32_t ctrl_idx = p_cfg->channel;

    /* Initialize ETND register */
    p_instance_ctrl->p_reg_tsnes  = (R_TSNES0_Type *) (R_TSNES0_BASE + (ctrl_idx * ETHER_TSNES_REG_SIZE));    /* PRQA S 0306,1840 # Advisory R-11.4 & Advisory R-10.4 */
    p_instance_ctrl->p_reg_axibmi = (R_AXIBMI0_Type *) (R_AXIBMI0_BASE + (ctrl_idx * ETHER_AXIBMI_REG_SIZE)); /* PRQA S 0306,1840 # Advisory R-11.4 & Advisory R-10.4 */
    p_instance_ctrl->p_reg_rmaca  = (R_RMACA0_Type *) (R_RMACA0_BASE + (ctrl_idx * ETHER_RMACA_REG_SIZE));    /* PRQA S 0306,1840 # Advisory R-11.4 & Advisory R-10.4 */
    p_instance_ctrl->p_reg_pwrctl = (R_PWRCTL0_Type *) (R_PWRCTL0_BASE + (ctrl_idx * ETHER_PWRCTL_REG_SIZE)); /* PRQA S 0306,1840 # Advisory R-11.4 & Advisory R-10.4 */
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    p_instance_ctrl->p_reg_gptma = (R_GPTMA0_Type *) (R_GPTMA0_BASE);                                         /* PRQA S 0306 # Advisory R-11.4 */
#endif
#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
    p_instance_ctrl->p_reg_sgmii = (R_SGMII1_Type *) R_SGMII1_BASE;
#endif

    /* Initialize configuration of Ethernet module. */
    p_instance_ctrl->p_ether_cfg = p_cfg;

    /* Initialize the flags */
    p_instance_ctrl->link_establish_status = ETHER_TSN_LINK_ESTABLISH_STATUS_DOWN;
    p_instance_ctrl->link_change           = ETHER_TSN_LINK_CHANGE_NO_CHANGE;
    p_instance_ctrl->previous_link_status  = ETHER_TSN_PREVIOUS_LINK_STATUS_DOWN;

    /* Initialize controller state as DOWN */
    g_ether_tsn_ctrl_stat[ctrl_idx].mode = ETHER_TSN_MODE_DOWN;

    /* Initialize address filter */
#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)
    r_ether_tsn_clear_all_address_filters(ctrl_idx);
#endif

    /* Set callback and context pointers, if configured */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    /* Initialize the Ethernet buffer */
    r_ether_tsn_initialize_buffer(ctrl_idx);

    /* Invoke Eth_Init to initialize the Ethernet Driver */
    r_ether_tsn_hw_init(p_instance_ctrl, ctrl_idx); /* PRQA S 3200 # Required R-17.7 */

    /* Sets the physical source address used by the indexed controller */
    r_ether_tsn_set_phys_addr(ctrl_idx, p_instance_ctrl->p_ether_cfg->p_mac_address);

    /* Software reset the PHY */
    phy_ret = p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->open(
        p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_ctrl,
        p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_cfg);

#if (!ETHER_PHY_CFG_INIT_PHY_LSI_AUTOMATIC) /* PRQA S 3332 # Rule-20.9 */
    /* Initialize the PHY */
    if (FSP_SUCCESS == phy_ret)
    {
        phy_ret = p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->chipInit(
            p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_ctrl,
            p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_cfg);
    }
#endif
    if (FSP_SUCCESS == phy_ret)
    {
        p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->startAutoNegotiate(
            p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_ctrl);

        /* Start open ether tsn */
        p_instance_ctrl->open = ETHER_TSN_OPEN;
        err = FSP_SUCCESS;
    }
    else
    {
        if (FSP_ERR_ETHER_PHY_ERROR_LINK == phy_ret)
        {
            err = FSP_ERR_ETHER_ERROR_PHY_COMMUNICATION;
        }
        else
        {
            err = phy_ret;
        }
    }

    /* Configure frame filter add addition multicast address for this controller to listen to */
#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)
    ether_tsn_extended_cfg_t const * p_extend = ((ether_tsn_extended_cfg_t const *) p_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    for (uint8_t index_addr_filter = 0U;
         index_addr_filter < p_extend->number_of_addr_filter;
         index_addr_filter++)
    {
        (void) r_ether_tsn_update_phys_addr_filter(ctrl_idx,
                                       /* PRQA S 3200 # Required R-17.7 */
                                                   &(p_extend->p_mac_address_filter)[index_addr_filter *
                                                                                     ETHER_TSN_ADDRESS_SIZE],
                                                   p_extend->ether_tsn_filter_action_type);
    }
#endif

    return err;
}

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * @brief Parameter error check function for open.
 * @param[in] p_ctrl                     Pointer to the control block for the channel.
 * @param[in] p_cfg                      Pointer to configuration block.
 * @retval  FSP_SUCCESS                  No parameter error found.
 * @retval  FSP_ERR_INVALID_POINTER      Pointer points to invalid memory location.
 * @retval  FSP_ERR_ALREADY_OPEN         Requested channel is already open in a different configuration.
 * @retval  FSP_ERR_INVALID_CHANNEL      Selected channel does not exist.
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_TSN control block is NULL.
 **********************************************************************************************************************/
static fsp_err_t r_ether_tsn_open_param_check (ether_tsn_instance_ctrl_t const * const p_ctrl,
                                               ether_cfg_t const * const               p_cfg)
{
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN((NULL != p_cfg), FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN((NULL != p_cfg->p_mac_address), FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN((NULL != p_cfg->p_extend), FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN((ETHER_TSN_OPEN != p_ctrl->open), FSP_ERR_ALREADY_OPEN);
    FSP_ERROR_RETURN((BSP_FEATURE_ETH_TSN_NUM_CHANNELS > p_cfg->channel), FSP_ERR_INVALID_CHANNEL);

    return FSP_SUCCESS;
}

#endif

/********************************************************************************************************************//**
 * @brief Transmit Ethernet frame. Transmits data from the location specified by the pointer to the transmit
 * buffer, with the data size equal to the specified frame length ether tsn.
 * In the non zero copy mode, transmits data after being copied to the internal buffer.
 * Implements @ref ether_api_t::write.
 * @param[in] p_ctrl                     Pointer to the control block for the channel.
 * @param[in] p_buffer                   Pointer to the write buffer.
 * @param[in] frame_length               Frame length buffer write data.
 * @retval  FSP_SUCCESS                  Processing completed successfully.
 * @retval  FSP_ERR_NOT_OPEN             Requested channel is not configured or API not open.
 * @retval  FSP_ERR_INVALID_POINTER      Pointer points to invalid memory location.
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_TSN control block is NULL.
 * @retval  FSP_ERR_IN_USE               Buffer still in use.
 * @retval  FSP_ERR_INVALID_SIZE         Report error when size of length invalid.
 * @retval  FSP_ERR_ETHER_ERROR_LINK     Auto-negotiation is not completed, and reception is not enabled.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_Write (ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t const frame_length) /* PRQA S 1505 # Advisory R-8.7 */
{
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) p_ctrl;                       /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t err = FSP_SUCCESS;

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
#endif

    /* Initialize controller index */
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    /* When the Link up processing is not completed, return error */
    FSP_ERROR_RETURN(ETHER_TSN_LINK_ESTABLISH_STATUS_UP == p_instance_ctrl->link_establish_status,
                     FSP_ERR_ETHER_ERROR_LINK);

    if (ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
    {
        g_ether_tsn_tx_buffer_addr[ctrl_idx] = (uint32_t) p_buffer;       /* PRQA S 0326 # DP_MSR_R-11.6_UC1 */
    }

    /* Send data via Ethernet */
    err = r_ether_tsn_send_data(p_instance_ctrl, p_buffer, frame_length); /* PRQA S 0317 # Advisory R-11.5 */

    return err;
}

/********************************************************************************************************************//**
 * @brief Transmit Ethernet frame using a provided transmit buffer ID.
 * Requests transmission of an Ethernet frame that has already been prepared in the
 * internal transmit buffer identified by buf_id.
 * @param[in] p_ctrl                     Pointer to the control block for the channel.
 * @param[in] buf_id                     Transmit buffer identifier obtained from R_ETHER_TSN_GetTxBuffer().
 * @param[in] frame_length               Length of the Ethernet frame in bytes.
 * @retval FSP_SUCCESS                   Processing completed successfully.
 * @retval FSP_ERR_NOT_OPEN             Requested channel is not configured or API not open.
 * @retval FSP_ERR_ASSERTION            Pointer to ETHER_TSN control block is NULL.
 * @retval FSP_ERR_ETHER_ERROR_LINK     Auto-negotiation is not completed, and reception is not enabled.
 * @retval FSP_ERR_ABORTED              Transmit failed, cannot find buffer for transmit.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_WriteBuffer (ether_ctrl_t * const p_ctrl, uint32_t buf_id, uint32_t const frame_length) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    fsp_err_t err = FSP_SUCCESS;
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl;           /* PRQA S 0316 # Advisory R-11.5 */
#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    volatile ether_tsn_type          result   = ETHER_TSN_OK;
    ether_tsn_extended_cfg_t const * p_extend =                                                                    /* PRQA S 0316 # Advisory R-11.5 */
                                                ((ether_tsn_extended_cfg_t const *) p_instance_ctrl->p_ether_cfg-> /* PRQA S 0316 # Advisory R-11.5 */
                                                 p_extend);
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    /* When the Link up processing is not completed, return error */
    FSP_ERROR_RETURN(ETHER_TSN_LINK_ESTABLISH_STATUS_UP == p_instance_ctrl->link_establish_status,
                     FSP_ERR_ETHER_ERROR_LINK);

    /* Send data via ethernet*/
    result =
        r_ether_tsn_transmit(p_ctrl,         /* PRQA S 0317 # Advisory R-11.5 */
                             ctrl_idx,
                             buf_id,
                             (uint16_t) p_extend->ether_type,
                             ETHER_TSN_TRUE, /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
                             (uint16_t) frame_length,
                             p_extend->p_mac_address_target);

    if (ETHER_TSN_NOT_OK == result)
    {
        /* If a callback is provided, then call it with callback argument. */
        r_ether_tsn_call_callback(p_ctrl, ETHER_EVENT_TX_ABORTED); /* PRQA S 0317 # Advisory R-11.5 */
        err = FSP_ERR_ABORTED;
    }

    return err;
}

/********************************************************************************************************************//**
 * @brief Acquire a transmit buffer for frame preparation.
 * Obtains an available transmit buffer from the controller. In non zero-copy mode,
 * the frame data provided by p_buffer is copied into the internal transmit buffer.
 * In zero-copy mode, the caller can access the allocated transmit buffer directly
 * through the returned buffer identifier.
 * @param[in]      p_ctrl                Pointer to the control block for the channel.
 * @param[in]      priority              Priority associated with the transmit request.
 * @param[out]     p_buf_id              Pointer to store the allocated transmit buffer ID.
 * @param[in]      p_buffer              Pointer to frame data to be copied (non zero-copy mode only).
 * @param[in,out]  frame_length          Pointer to frame length in bytes. May be updated
 * @retval FSP_SUCCESS                   Processing completed successfully.
 * @retval FSP_ERR_NOT_OPEN             Requested channel is not configured or API not open.
 * @retval FSP_ERR_INVALID_POINTER      Pointer points to invalid memory location.
 * @retval FSP_ERR_ASSERTION            Pointer to ETHER_TSN control block is NULL.
 * @retval FSP_ERR_IN_USE               Buffer still in use.
 * @retval FSP_ERR_INVALID_SIZE         Report error when size of length invalid.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_GetTxBuffer (ether_ctrl_t * const p_ctrl,                                     /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
                                   uint8_t              priority,
                                   uint32_t           * p_buf_id,
                                   void * const         p_buffer,                                   /* PRQA S 3673 # Advisory R-8.13 */
                                   uint32_t * const     frame_length)                               /* PRQA S 3673 # Advisory R-8.13 */
{
    fsp_err_t err = FSP_SUCCESS;
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
#endif

    ether_tsn_buf_req_return_type_t  buf_return_value = ETHER_TSN_BUFREQ_OK;
    ether_tsn_extended_cfg_t const * p_extend         =
        ((ether_tsn_extended_cfg_t const *) p_instance_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    ether_tsn_type * p_buf      = ETHER_TSN_NULL_PTR;                                /* PRQA S 0317 # Advisory R-11.5 */
    uint32_t         lus_length = 0;
    uint8_t          ctrl_idx   = p_instance_ctrl->p_ether_cfg->channel;
    lus_length = *frame_length;

    if (ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
    {
        g_ether_tsn_tx_buffer_addr[ctrl_idx] = (uint32_t) p_buffer; /* PRQA S 0326 # DP_MSR_R-11.6_UC1 */
    }

    buf_return_value = r_ether_tsn_provide_tx_buffer(p_ctrl,                         /* PRQA S 0317 # Advisory R-11.5 */
                                                     ctrl_idx,
                                                     priority,
                                                     p_buf_id,
                                                     (uint8_t **) &p_buf,
                                                     &lus_length);

    /* Send data via ethernet*/
    if ((buf_return_value == ETHER_TSN_BUFREQ_OK) && (p_buf != ETHER_TSN_NULL_PTR))
    {
        if (ETHER_ZEROCOPY_DISABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
        {
            if (ETHER_TSN_NULL_PTR != p_buffer)
            {
                (void) memcpy(p_buf, p_buffer, lus_length); /* PRQA S 1496 # DP_MSR_R-21.15_UC1 */
            }
        }
    }
    else if (buf_return_value == ETHER_TSN_BUFREQ_E_BUSY)
    {
        /* Buffer still in use */
        if (p_extend->ether_tsn_cfg_type_tx_isr == ETHER_TSN_CTRL_DISABLE_TX_INTERRUPT)
        {
            /* Polling for Tx confirmation */
            r_ether_tsn_tx_confirmation(p_ctrl, ctrl_idx); /* PRQA S 0317 # Advisory R-11.5 */
        }

        err = FSP_ERR_IN_USE;
    }
    else
    {
        /* Report error when size of length invalid */
        err = FSP_ERR_INVALID_SIZE;
    }

    return err;
}

/********************************************************************************************************************//**
 * @brief Confirm completion of transmit operations.
 * In polling mode, checks the transmission status and performs transmission
 * confirmation processing. In interrupt mode, this function only validates the
 * controller state.
 * @param[in] p_ctrl                     Pointer to the control block for the channel.
 * @retval FSP_SUCCESS                   Processing completed successfully.
 * @retval FSP_ERR_NOT_OPEN              Requested channel is not configured or API is not open.
 * @retval FSP_ERR_ASSERTION             Pointer to ETHER_TSN control block is NULL.
 * @retval FSP_ERR_ETHER_ERROR_LINK      Link establishment is not completed.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_TxConfirm (ether_ctrl_t * const p_ctrl)                                       /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    fsp_err_t err = FSP_SUCCESS;
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    ether_tsn_extended_cfg_t const * p_extend =                                                                    /* PRQA S 0316 # Advisory R-11.5 */
                                                ((ether_tsn_extended_cfg_t const *) p_instance_ctrl->p_ether_cfg-> /* PRQA S 0316 # Advisory R-11.5 */
                                                 p_extend);
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    /* When the Link up processing is not completed, return error */
    FSP_ERROR_RETURN(ETHER_TSN_LINK_ESTABLISH_STATUS_UP == p_instance_ctrl->link_establish_status,
                     FSP_ERR_ETHER_ERROR_LINK);

    if (ETHER_TSN_CTRL_DISABLE_TX_INTERRUPT == p_extend->ether_tsn_cfg_type_tx_isr)
    {
        /* Polling for Tx confirmation */
        r_ether_tsn_tx_confirmation(p_ctrl, ctrl_idx); /* PRQA S 0317 # Advisory R-11.5 */
    }

    return err;
}

/********************************************************************************************************************//**
 * @brief Enable transmit time stamp generation for a transmit buffer.
 * Enables hardware time stamp capture for the specified transmit buffer. When the
 * associated Ethernet frame is transmitted, a transmit time stamp can be retrieved
 * using R_ETHER_TSN_GetTxTimestamp().
 * @param[in] p_ctrl                     Pointer to the control block for the channel.
 * @param[in] buf_id                     Transmit buffer identifier obtained from R_ETHER_TSN_GetTxBuffer().
 * @retval FSP_SUCCESS                   Processing completed successfully.
 * @retval FSP_ERR_NOT_OPEN              Requested channel is not configured or API is not open.
 * @retval FSP_ERR_ASSERTION             Pointer to ETHER_TSN control block is NULL.
 * @retval FSP_ERR_NOT_ENABLED           ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT is not enabled.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_EnableTxTimestamp (ether_ctrl_t * const p_ctrl, uint32_t buf_id) /* PRQA S 1503,3673 # DP_MSR_R-2.1_UC1,Advisory R-8.13 */
{
    fsp_err_t err = FSP_SUCCESS;
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    ether_tsn_type return_value;
    ether_tsn_instance_ctrl_t const * p_instance_ctrl   = (ether_tsn_instance_ctrl_t const *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    ether_tsn_buf_handler_type_t    * p_buf_handler_ptr = NULL;
 #if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;
    return_value = r_ether_tsn_check_provide_buffer((uint32_t) ctrl_idx, (uint32_t) buf_id);
    if (ETHER_TSN_OK == return_value)
    {
        /* Get Tx buffer address */
        p_buf_handler_ptr = gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_id].p_buffer_hdr;
    }

    if (NULL != p_buf_handler_ptr)
    {
        p_buf_handler_ptr->enable_ts = ETHER_TSN_TRUE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
    }

#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(buf_id);
    err = FSP_ERR_NOT_ENABLED;
#endif

    return err;
}

/********************************************************************************************************************//**
 * @brief Get the transmit time stamp associated with a transmitted frame.
 * Retrieves the hardware time stamp captured for the specified transmit buffer.
 * The time stamp is valid only if transmit time stamping was enabled before frame
 * transmission.
 * @param[in]  p_ctrl                    Pointer to the control block for the channel.
 * @param[in]  buf_id                    Transmit buffer identifier.
 * @param[out] p_time_stamp              Pointer to store the retrieved time stamp.
 * @retval FSP_SUCCESS                   Processing completed successfully.
 * @retval FSP_ERR_NOT_OPEN              Requested channel is not configured or API is not open.
 * @retval FSP_ERR_ASSERTION             Pointer to ETHER_TSN control block is NULL.
 * @retval FSP_ERR_NOT_ENABLED           ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT is not enabled.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_GetTxTimestamp (ether_ctrl_t * const          p_ctrl, /* PRQA S 1503,3673 # DP_MSR_R-2.1_UC1,Advisory R-8.13 */
                                      uint32_t                      buf_id,
                                      ether_tsn_time_stamp_type_t * p_time_stamp)
{
    fsp_err_t err = FSP_SUCCESS;
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
 #if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    ether_tsn_buf_handler_type_t const * p_tx_buffer_node;

    /* Protect update Time Qual from interrupt */
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

    /* Get Tx buffer handler from the Tx buffer manager table */
    p_tx_buffer_node =
        (ether_tsn_buf_handler_type_t *) gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_id].p_buffer_hdr;

    *p_time_stamp = p_tx_buffer_node->time_stamp;

    /* Exit critical section */
    FSP_CRITICAL_SECTION_EXIT;
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(buf_id);
    FSP_PARAMETER_NOT_USED(p_time_stamp);
    err = FSP_ERR_NOT_ENABLED;
#endif

    return err;
}

/********************************************************************************************************************//**
 * @brief Register a receive time stamp storage location.
 * Sets the destination where the driver stores the time stamp associated with the
 * next received Ethernet frame when hardware time stamping is enabled.
 * @param[in]  p_ctrl                    Pointer to the control block for the channel.
 * @param[out] p_time_stamp              Pointer to a time stamp structure that will receive the hardware Rx time stamp.
 * @retval FSP_SUCCESS                   Processing completed successfully.
 * @retval FSP_ERR_NOT_OPEN              Requested channel is not configured or API is not open.
 * @retval FSP_ERR_ASSERTION             Pointer to ETHER_TSN control block is NULL.
 * @retval FSP_ERR_NOT_ENABLED           ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT is not enabled.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_GetRxTimestamp (ether_ctrl_t * const p_ctrl, ether_tsn_time_stamp_type_t * p_time_stamp) /* PRQA S 1503,3673 # DP_MSR_R-2.1_UC1,Advisory R-8.13 */
{
    fsp_err_t err = FSP_SUCCESS;
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl;            /* PRQA S 0316 # Advisory R-11.5 */

 #if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    gp_ether_tsn_rx_time_stamp[ctrl_idx] = p_time_stamp;
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_time_stamp);
    err = FSP_ERR_NOT_ENABLED;
#endif

    return err;
}

/*******************************************************************************************************************//**
 * Update the local MAC (physical source) address used by the controller. Provides runtime configuration
 * of the MAC address which is otherwise only taken from the configuration structure at Open().
 *
 * @param[in] p_ctrl                 Pointer to the control block
 * @param[in] p_mac_address          Pointer to the 6-byte MAC address in network byte order
 * @retval FSP_SUCCESS               MAC address updated
 * @retval FSP_ERR_ASSERTION         p_ctrl is NULL
 * @retval FSP_ERR_NOT_OPEN          The control block has not been opened
 * @retval FSP_ERR_INVALID_POINTER   p_mac_address is NULL
 * @retval FSP_ERR_INVALID_MODE      Not in valid mode to set MAC address.
 **********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_SetMACAddr (ether_ctrl_t * const p_ctrl, const uint8_t * p_mac_address)       /* PRQA S 1503,3673 # DP_MSR_R-2.1_UC1,Advisory R-8.13 */
{
    fsp_err_t err = FSP_SUCCESS;
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_mac_address, FSP_ERR_INVALID_POINTER);
#endif

    /* When the Link up processing is not completed, return error */
    FSP_ERROR_RETURN(ETHER_TSN_LINK_ESTABLISH_STATUS_UP != p_instance_ctrl->link_establish_status,
                     FSP_ERR_INVALID_MODE);
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    /* Store the new address in the driver state; it is programmed into MAHR/MALR (in CONFIG mode)
     * when the controller is activated. */
    r_ether_tsn_set_phys_addr(ctrl_idx, p_mac_address);

    return err;
}

/*******************************************************************************************************************//**
 * Get the local MAC (physical source) of controller.
 * @param[in] p_ctrl                 Pointer to the control block
 * @param[in,out] p_mac_address      Pointer to the 6-byte MAC address in network byte order
 * @retval FSP_SUCCESS               MAC address updated
 * @retval FSP_ERR_ASSERTION         p_ctrl is NULL
 * @retval FSP_ERR_NOT_OPEN          The control block has not been opened
 * @retval FSP_ERR_INVALID_POINTER   p_mac_address is NULL
 **********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_GetMACAddr (ether_ctrl_t * const p_ctrl, uint8_t * p_mac_address)             /* PRQA S 1503,3673 # DP_MSR_R-2.1_UC1,Advisory R-8.13 */
{
    fsp_err_t err = FSP_SUCCESS;
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_mac_address, FSP_ERR_INVALID_POINTER);
#endif
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    /* Get data from global variable stored the MAC address. */
    ETHER_TSN_UNPACK_ADDRESS_TO_8(g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr, p_mac_address); /* PRQA S 3469 # Advisory D-4.9 */

    return err;
}

/********************************************************************************************************************//**
 * @brief Receive Ethernet frame. Receives data to the location specified by the pointer to the receive buffer.
 * In zero copy mode, the address of the receive buffer is returned.
 * In non zero copy mode, the received data in the internal buffer is copied to the pointer passed by the argument.
 * Implements @ref ether_api_t::read.
 * @param[in]      p_ctrl       Pointer to control structure.
 * @param[in]      p_buffer     Pointer to where to store read data.
 * @param[in,out]  length_bytes Number of bytes in buffer.
 * @retval  FSP_SUCCESS                  Processing completed successfully.
 * @retval  FSP_ERR_ETHER_ERROR_NO_DATA  No Data in Receive buffer.
 * @retval  FSP_ERR_INVALID_POINTER      Pointer points to invalid memory location.
 * @retval  FSP_ERR_NOT_OPEN             Requested channel is not configured or API not open.
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_TSN control block is NULL.
 * @retval  FSP_ERR_INVALID_SIZE         Buffer input size not enough to receive frame.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_Read (ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t * const length_bytes) /* PRQA S 1505 # Advisory R-8.7 */
{
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) p_ctrl;                        /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t  err            = FSP_ERR_ETHER_ERROR_NO_DATA;
    uint8_t ** pp_read_buffer = (uint8_t **) p_buffer;                                                         /* PRQA S 0316 # Advisory R-11.5 */

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN(NULL != length_bytes, FSP_ERR_INVALID_POINTER);
#endif

    /* Initialize controller index */
    uint32_t ctrl_idx  = p_instance_ctrl->p_ether_cfg->channel;
    uint8_t  queue_idx = 0U;
    ether_tsn_ext_desc_with_ts_type_t * p_link_desc;

    /* Get/save the current state of interrupts */
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

    p_link_desc = gp_ether_tsn_rx_link_desc[ctrl_idx][queue_idx];
    if ((ETHER_TSN_NULL_PTR != p_link_desc) && (ETHER_TSN_DESC_LEMPTY == p_link_desc->st_header.dt))
    {
        p_link_desc->st_header.dt = (uint32_t) ETHER_TSN_DESC_LINK;
    }

    /* Get user input buffer and length */
    gp_ether_tsn_user_rx_buf[ctrl_idx]    = (uint8_t *) p_buffer; /* PRQA S 0316 # Advisory R-11.5 */
    g_ether_tsn_user_rx_buf_len[ctrl_idx] = *length_bytes;

    /* Check for received message */
    r_ether_tsn_receive(p_instance_ctrl, queue_idx, &g_ether_tsn_rx_status[ctrl_idx]);

    /* Confirm received message */
    if (ETHER_TSN_NOT_RECEIVED != g_ether_tsn_rx_status[ctrl_idx])
    {
#if (ETHER_TSN_CFG_READ_BUFFER_SIZE_CHECK)                           /* PRQA S 3332 # Rule-20.9 */
        if (g_ether_tsn_rx_len_byte[ctrl_idx] > *length_bytes)
        {
            gp_ether_tsn_user_rx_buf[ctrl_idx] = ETHER_TSN_NULL_PTR; /* PRQA S 0317 # Advisory R-11.5 */
            FSP_CRITICAL_SECTION_EXIT;

            return FSP_ERR_INVALID_SIZE;
        }
#endif
        if (ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
        {
            *pp_read_buffer = (uint8_t *) g_ether_tsn_rx_frame[ctrl_idx].frame_addr; /* PRQA S 0306 # Advisory R-11.4 */
        }

        err = FSP_SUCCESS;

        /* Get data length to user */
        *length_bytes = g_ether_tsn_rx_len_byte[ctrl_idx];
    }
    else
    {
        *length_bytes = 0U;
    }

    /* Clear user input buffer and length */
    gp_ether_tsn_user_rx_buf[ctrl_idx]    = ETHER_TSN_NULL_PTR; /* PRQA S 0317 # Advisory R-11.5 */
    g_ether_tsn_user_rx_buf_len[ctrl_idx] = 0U;

    /* Restore the interrupt state */
    FSP_CRITICAL_SECTION_EXIT;

    return err;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 * Implements @ref ether_api_t::callbackSet.
 * @param[in] p_ctrl              Pointer to ETHER_TSN control structure.
 * @param[in] p_callback          Callback function pointer input.
 * @param[in] p_context           Context pointer input.
 * @param[in] p_callback_memory   Callback memory pointer input.
 * @retval    FSP_SUCCESS         Callback function is set successfully.
 * @retval    FSP_ERR_NOT_OPEN    The control block has not been opened.
 * @retval    FSP_ERR_ASSERTION   Pointer to ETHER_TSN control block or p_callback is NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_CallbackSet (ether_ctrl_t * const          p_ctrl,                               /* PRQA S 1505 # Advisory R-8.7 */
                                   void (                      * p_callback)(ether_callback_args_t *), /* PRQA S 1336 # Rule-8.2 */
                                   void * const                  p_context,
                                   ether_callback_args_t * const p_callback_memory)
{
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) p_ctrl;                /* PRQA S 0316 # Advisory R-11.5 */

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_callback);

    /* Verify this unit has not been initialized */
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief Move to the next buffer in the circular receive buffer list.
 * Implements @ref ether_api_t::bufferRelease.
 * @param[in] p_ctrl                                Pointer to ETHER_TSN control structure.
 * @retval  FSP_SUCCESS                             Processing completed successfully.
 * @retval  FSP_ERR_NOT_OPEN                        The control block has not been opened.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_TSN control block is NULL.
 * @retval  FSP_ERR_INVALID_MODE                    Driver is configured to non zero copy mode.
 * @retval  FSP_ERR_ETHER_ERROR_NO_DATA             There is no data in receive buffer.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_BufferRelease (ether_ctrl_t * const p_ctrl)                       /* PRQA S 1505 # Advisory R-8.7 */
{
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t err = FSP_SUCCESS;

    /* Pointer to the currently referenced transmit descriptor*/
    ether_tsn_ext_desc_with_ts_type_t * p_rx_desc_chain;
    uint32_t queue_idx = 0U;

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN((uint32_t) ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy, FSP_ERR_INVALID_MODE);
#endif

    /* Initialize controller index */
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    /* When receive data exists */
    if (ETHER_TSN_NOT_RECEIVED != g_ether_tsn_rx_status[ctrl_idx])
    {
        /* Reset current Rx descriptor */
        p_rx_desc_chain = g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_release[queue_idx];
        if (ETHER_TSN_DESC_FEMPTY != p_rx_desc_chain->st_header.dt)
        {
            p_rx_desc_chain->st_header.die   = (uint32_t) ETHER_TSN_RX_DESC_DIE_ENABLE;
            p_rx_desc_chain->st_header.axie  = (uint32_t) 0UL;
            p_rx_desc_chain->st_header.dse   = (uint32_t) 0UL;
            p_rx_desc_chain->st_header.info0 = (uint32_t) 0UL;
            p_rx_desc_chain->st_header.ds    =
                (p_instance_ctrl->p_ether_cfg->ether_buffer_size & ETHER_TSN_CONVERT_TO_SMALL_BIT);
            p_rx_desc_chain->info1[0]     = 0U;
            p_rx_desc_chain->info1[1]     = 0U;
            p_rx_desc_chain->tsns         = 0U;
            p_rx_desc_chain->tss          = 0U;
            p_rx_desc_chain->st_header.dt = (uint32_t) ETHER_TSN_DESC_FEMPTY;
        }

        /* Re-learn the RX descriptor chain (using the HW-tracked current address) if the ring is/was full */
        r_ether_tsn_rx_desc_relearn_if_full(p_instance_ctrl, queue_idx);
    }
    else
    {
        err = FSP_ERR_ETHER_ERROR_NO_DATA;
    }

    return err;
}

/********************************************************************************************************************//**
 * @brief Change the buffer pointer of the current rx buffer descriptor.
 * Implements @ref ether_api_t::rxBufferUpdate.
 * @param[in] p_ctrl                                Pointer to the control block for the channel.
 * @param[in] p_buffer                              Pointer to the read buffer update data.
 * @retval  FSP_SUCCESS                             Processing completed successfully.
 * @retval  FSP_ERR_NOT_OPEN                        Requested channel is not configured or API not open.
 * @retval  FSP_ERR_INVALID_POINTER                 Pointer points to invalid memory location.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_TSN control block is NULL.
 * @retval  FSP_ERR_INVALID_MODE                    Driver is configured to non zero copy mode.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_RxBufferUpdate (ether_ctrl_t * const p_ctrl, void * const p_buffer)           /* PRQA S 1503,3673,1505 # DP_MSR_R-2.1_UC1,Advisory R-8.13,Advisory R-8.7 */
{
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t err = FSP_SUCCESS;
    ether_tsn_ext_desc_with_ts_type_t * p_rx_desc_chain;
    uint32_t queue_idx = 0U;

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN(ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy, FSP_ERR_INVALID_MODE);
#endif

    /* Initialize controller index */
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    p_rx_desc_chain = g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_update[queue_idx];

    p_rx_desc_chain->dptr = (uint32_t) p_buffer; /* PRQA S 0326 # DP_MSR_R-11.6_UC1 */

    /* Next descriptor */
    p_rx_desc_chain++;
    while ((ETHER_TSN_DESC_LEMPTY == p_rx_desc_chain->st_header.dt) ||
           (ETHER_TSN_DESC_LINK == p_rx_desc_chain->st_header.dt))
    {
        p_rx_desc_chain->st_header.dt = (uint32_t) ETHER_TSN_DESC_LINK;

        p_rx_desc_chain = (ether_tsn_ext_desc_with_ts_type_t *) p_rx_desc_chain->dptr; /* PRQA S 0306 # Advisory R-11.4 */
    }

    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_update[queue_idx] = p_rx_desc_chain;

    return err;
}

/********************************************************************************************************************//**
 * @brief Disables interrupts. Removes power and releases hardware lock.
 * Implements @ref ether_api_t::close.
 * @param[in] p_ctrl                                    Pointer to the control block for the channel.
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_TSN control block is NULL.
 * @retval  FSP_ERR_TIMEOUT                             Cannot set to disable mode
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_Close (ether_ctrl_t * const p_ctrl)                               /* PRQA S 1505 # Advisory R-8.7 */
{
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t           err        = FSP_SUCCESS;
    ether_tsn_type      result     = ETHER_TSN_NOT_OK;
    ether_tsn_tick_type tick_start = 0U;
    ether_tsn_tick_type tick_elap  = 0U;

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Initialize controller index */
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->close(
        p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_ctrl);

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    gp_ether_tsn_rx_time_stamp[ctrl_idx] = NULL;
#endif

    result = r_ether_tsn_mhd_mode_setting(p_instance_ctrl, ETHER_TSN_MHD_DISABLE_MODE);

    if (ETHER_TSN_OK == result)
    {
        p_instance_ctrl->p_reg_tsnes->SWR = ETHER_TSN_SOFTWARE_RESET_ENABLE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        (void) r_ether_tsn_get_counter_value(&tick_start);
        do
        {
            tick_elap = r_ether_tsn_get_time_out_value(tick_start);
        } while ((ETHER_TSN_GIS_SWRCF_MASK != (p_instance_ctrl->p_reg_tsnes->GIS & ETHER_TSN_GIS_SWRCF_MASK)) && /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                 (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

        if (ETHER_TSN_TIMEOUT_COUNT < tick_elap)
        {
            result = ETHER_TSN_NOT_OK;
            err    = FSP_ERR_TIMEOUT;
        }
        else
        {
            /* Clear SoftWare Reset Completion Flag */
            p_instance_ctrl->p_reg_tsnes->GIS = ETHER_TSN_GIS_SWRCF_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Set controller state as DOWN */
            g_ether_tsn_ctrl_stat[ctrl_idx].mode = ETHER_TSN_MODE_DOWN;
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)

            /* Disable GPTMA interrupt */
            p_instance_ctrl->p_reg_gptma->GID = ETHER_TSN_GID_DISABLE_ISR; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Clear compare match flag */
            p_instance_ctrl->p_reg_gptma->GIS = ETHER_TSN_GIS_CLEAR_ISR;   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

            err = FSP_SUCCESS;

            r_ether_tsn_call_callback(p_ctrl, ETHER_CLOSE_SUCCEED); /* PRQA S 0317 # Advisory R-11.5 */
        }
    }
    else
    {
        err = FSP_ERR_TIMEOUT;
    }

    /* Disable interrupt */
    r_ether_tsn_disable_icu(p_instance_ctrl);

    /* Clear configure block parameters. */
    p_instance_ctrl->p_ether_cfg = NULL;

    /* Mark the channel not open so other APIs cannot use it. */
    p_instance_ctrl->open = 0U;

    return err;
}

/**********************************************************************************************************************//**
 * Provides status of Ethernet driver in the user provided pointer.
 * Implements @ref ether_api_t::txStatusGet.
 * @param[in] p_ctrl                     Pointer to the control block for the channel.
 * @param[in] p_buffer_address           Pointer to stored in provided p_buffer_address.
 * @retval  FSP_SUCCESS                  Transmit buffer address is stored in provided p_buffer_address.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER      p_status is NULL.
 * @retval  FSP_ERR_NOT_FOUND            The requested item could not be found.
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_TSN control block is NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_TxStatusGet (ether_ctrl_t * const p_ctrl, void * const p_buffer_address)      /* PRQA S 1503,3673,1505 # DP_MSR_R-2.1_UC1,Advisory R-8.13,Advisory R-8.7 */
{
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t        err                    = FSP_ERR_NOT_FOUND;
    uint32_t const * p_data_desc            = ETHER_TSN_NULL_PTR;                                   /* PRQA S 0317 # Advisory R-11.5 */
    uint8_t       ** pp_sent_buffer_address = (uint8_t **) p_buffer_address;                        /* PRQA S 0316 # Advisory R-11.5 */

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN((uint32_t) ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer_address, FSP_ERR_INVALID_POINTER);
#endif

    /* Initialize controller index */
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    if (g_ether_tsn_tx_confirmed[ctrl_idx] != 0U)
    {
        /* Check the status of the current tx descriptor */
        p_data_desc = (uint32_t *) g_ether_tsn_current_tx_descr[ctrl_idx][g_ether_tsn_current_priority[ctrl_idx]]; /* PRQA S 0306 # Advisory R-11.4 */
    }

    /* Descriptor is NULL, when no packet transmitted. */
    if (NULL != p_data_desc)
    {
        *pp_sent_buffer_address = (uint8_t *) p_data_desc; /* PRQA S 0311 # DP_MSR_R-11.8_UC1 */

        /* Reset packet confirm status */
        g_ether_tsn_tx_confirmed[ctrl_idx] = 0U;
        err = FSP_SUCCESS;
    }

    return err;
}

/********************************************************************************************************************//**
 * @brief The Link up processing, the Link down processing, and the magic packet detection processing are executed.
 * Implements @ref ether_api_t::linkProcess.
 * @param[in] p_ctrl                                    Pointer to the control block for the channel.
 * @retval  FSP_SUCCESS                                 Link is up.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                Initialization of the PHY-LSI failed.
 * @retval  FSP_ERR_ETHER_ERROR_LINK                    Link is down.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_TSN control block is NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_LinkProcess (ether_ctrl_t * const p_ctrl)                         /* PRQA S 1505 # Advisory R-8.7 */
{
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t      err    = FSP_SUCCESS;
    ether_tsn_type result = ETHER_TSN_NOT_OK;

#if (ETHER_TSN_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_TSN_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    err = ether_tsn_link_status_check(p_instance_ctrl);

    /* The state of the link status in PHY-LSI is confirmed and Link Up/Down is judged. */
    if (FSP_SUCCESS == err)
    {
        /* When becoming Link up */
        if (ETHER_TSN_PREVIOUS_LINK_STATUS_DOWN == p_instance_ctrl->previous_link_status)
        {
            p_instance_ctrl->link_change = ETHER_TSN_LINK_CHANGE_LINK_UP;

            /* Update Link status */
            p_instance_ctrl->previous_link_status = ETHER_TSN_PREVIOUS_LINK_STATUS_UP;
        }
    }
    else
    {
        /* When becoming Link down */
        if (ETHER_TSN_PREVIOUS_LINK_STATUS_UP == p_instance_ctrl->previous_link_status)
        {
            p_instance_ctrl->link_change = ETHER_TSN_LINK_CHANGE_LINK_DOWN;

            /* Update Link status */
            p_instance_ctrl->previous_link_status = ETHER_TSN_PREVIOUS_LINK_STATUS_DOWN;
        }
    }

    /* When the link is up */
    if (ETHER_TSN_LINK_CHANGE_LINK_UP == p_instance_ctrl->link_change)
    {
        p_instance_ctrl->link_change = ETHER_TSN_LINK_CHANGE_NO_CHANGE;
        err = FSP_SUCCESS;
        r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_LINK_ON);
        p_instance_ctrl->link_establish_status = ETHER_TSN_LINK_ESTABLISH_STATUS_UP;

        /* Configuration MAC address and Link Verification */
        result = r_ether_tsn_set_controller_mode(p_instance_ctrl,
                                                 p_instance_ctrl->p_ether_cfg->channel,
                                                 ETHER_TSN_MODE_ACTIVE);

        if (ETHER_TSN_OK == result)
        {
            err = FSP_SUCCESS;
            R_BSP_SoftwareDelay(ETHER_TSN_WAIT_LINK_STABLE, BSP_DELAY_UNITS_MILLISECONDS);
            r_ether_tsn_call_callback(p_instance_ctrl, ETHER_LINK_VERIFICATION_SUCCEED);
        }
        else
        {
            /* When PHY link verification is not completed */
            p_instance_ctrl->link_establish_status = ETHER_TSN_LINK_ESTABLISH_STATUS_DOWN;
            p_instance_ctrl->link_change           = ETHER_TSN_LINK_CHANGE_LINK_UP;
            err = FSP_ERR_ETHER_PHY_ERROR_LINK;
            r_ether_tsn_call_callback(p_instance_ctrl, ETHER_LINK_VERIFICATION_FAIL);
        }
    }
    else if (ETHER_TSN_LINK_CHANGE_LINK_DOWN == p_instance_ctrl->link_change)
    {
        p_instance_ctrl->link_change           = ETHER_TSN_LINK_CHANGE_NO_CHANGE;
        p_instance_ctrl->link_establish_status = ETHER_TSN_LINK_ESTABLISH_STATUS_DOWN;
        r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_LINK_OFF);
    }
    else
    {
        ;                              /* no operation */
    }

    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_TSN)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name                 r_ether_tsn_util_ram_init_ptr
 * Description                   This Initialize base address of RAM for application
 * @param[in] p_base_addr        Pointer to initialize base address linked list and heap management structure
 * @param[in] ram_size           Initialize Ram size
 ***********************************************************************************************************************/
static void * r_ether_tsn_util_ram_init_ptr (void * const p_base_addr, const uint32_t ram_size) {
    ether_tsn_heap_manager_type_t * p_mem_mgr = (ether_tsn_heap_manager_type_t *) p_base_addr; /* PRQA S 0316 # Advisory R-11.5 */

    if (ETHER_TSN_NULL_PTR != p_mem_mgr)
    {
        /* Initialize circular linked list and heap management structure */
        /* Old syntax will occur warning */
        p_mem_mgr->p_next    = p_base_addr;     /* PRQA S 0317 # Advisory R-11.5 */
        p_mem_mgr->p_prev    = p_base_addr;     /* PRQA S 0317 # Advisory R-11.5 */
        p_mem_mgr->heap_size = ram_size;
        p_mem_mgr->heap_lock = ETHER_TSN_FALSE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
    }

    /* Return pointer to the heap manager structure */
    return p_mem_mgr;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_tsn_tx_ram_init
 * Description                   TxRAM initialization
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of a Queue
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_tx_ram_init (ether_tsn_instance_ctrl_t * p_ctrl, const uint32_t ctrl_idx)
{
    ether_tsn_ext_desc_type_t * p_desc_chain      = ETHER_TSN_NULL_PTR; /* PRQA S 0317 # Advisory R-11.5 */
    ether_tsn_ext_desc_type_t * p_desc_chain_temp = ETHER_TSN_NULL_PTR; /* PRQA S 0317 # Advisory R-11.5 */
    uint8_t           * p_data     = ETHER_TSN_NULL_PTR;                /* PRQA S 0317 # Advisory R-11.5 */
    uint32_t            cnt_i      = 0U;
    uint32_t            cnt_j      = 0U;
    ether_tsn_tick_type tick_start = 0U;
    ether_tsn_tick_type tick_elap  = 0U;
    ether_tsn_type      result     = ETHER_TSN_OK;

    /* MHD configure mode */
    result  = r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_DISABLE_MODE);
    result |= r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_CONFIG_MODE);

    if (ETHER_TSN_OK == result)
    {
        /*AXI Bus Write Configuration.*/
        p_ctrl->p_reg_axibmi->AXIWC = ETHER_TSN_AXIWC_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*AXI Bus Read Configuration.*/
        p_ctrl->p_reg_axibmi->AXIRC = ETHER_TSN_AXIRC_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*TX Descriptor chain Priority Configuration 0.*/
        p_ctrl->p_reg_axibmi->TDPC0 = ETHER_TSN_TDPC_CONFIG;                                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*TX Descriptor Chain Frame Type.*/
        p_ctrl->p_reg_axibmi->TFT = ETHER_TSN_TFT_CONFIG;                                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Transmission FIFO Size 0 */
        p_ctrl->p_reg_tsnes->TFS0 = ETHER_TSN_INIT_TFS0_CONFIG;                                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Transmission FIFO Size 1 */
        p_ctrl->p_reg_tsnes->TFS1 = (uint32_t) 0UL;                                                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Transmission FIFO Size 2 */
        p_ctrl->p_reg_tsnes->TFS2 = (uint32_t) 0UL;                                                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Transmission FIFO Size 3 */
        p_ctrl->p_reg_tsnes->TFS3 = (uint32_t) 0UL;                                                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*Transmission General Configuration register*/
        p_ctrl->p_reg_tsnes->TGC1 = ETHER_TSN_TGC1_CONFIG;                                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*Transmission Configuration and Control Register 1*/
        p_ctrl->p_reg_tsnes->TGC2 = (uint32_t) 0UL;                                                             /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*Transmission Maximum frame Size 0*/
        p_ctrl->p_reg_tsnes->TMS0 = ETHER_TSN_INIT_TMS_CONFIG;                                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*MAC common configuration*/
        p_ctrl->p_reg_rmaca->MCC &= (uint8_t) ~ETHER_TSN_MCC_TXE_MASK;                                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        p_data =
            (uint8_t *) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx], ETHER_TSN_2KB_FRAME_SIZE); /* PRQA S 0316 # Advisory R-11.5 */
        p_desc_chain =
            (ether_tsn_ext_desc_type_t *) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx],          /* PRQA S 0316 # Advisory R-11.5 */
                                                                         (uint32_t) ((ETHER_TSN_INIT_DESC_NUM +
                                                                                      ETHER_TSN_EOS_DESC_NUM) *
                                                                                     ETHER_TSN_EXT_DESC_SIZE));
    }

    if ((ETHER_TSN_NULL_PTR != p_desc_chain) && (ETHER_TSN_NULL_PTR != p_data))
    {
        /* Copy the original address of the descriptor */
        p_desc_chain_temp = p_desc_chain;
        for (cnt_i = 0U; cnt_i < ETHER_TSN_INIT_DESC_LOOP_NUM; cnt_i++)
        {
            /* Set start frame type */
            r_ether_tsn_tx_ram_init_desc_config(p_ctrl, p_desc_chain, (uint32_t) p_data); /* PRQA S 0329 # Advisory R-11.4 */
            p_desc_chain->st_header.dt = (uint32_t) ETHER_TSN_DESC_FSTART;

            /* Next data descriptor */
            p_desc_chain++;

            /* Set middle frame type */
            for (cnt_j = 0U; cnt_j < ETHER_TSN_MIDDLE_DESC_NUM; cnt_j++)
            {
                r_ether_tsn_tx_ram_init_desc_config(p_ctrl, p_desc_chain, (uint32_t) p_data); /* PRQA S 0329 # Advisory R-11.4 */
                p_desc_chain->st_header.dt = (uint32_t) ETHER_TSN_DESC_FMID;

                /* Next data descriptor */
                p_desc_chain++;
            }

            /* Set end frame type */
            r_ether_tsn_tx_ram_init_desc_config(p_ctrl, p_desc_chain, (uint32_t) p_data); /* PRQA S 0329 # Advisory R-11.4 */
            p_desc_chain->st_header.dt = (uint32_t) ETHER_TSN_DESC_FEND;

            /* Next data descriptor */
            p_desc_chain++;
        }

        /* Set EOS frame type */
        p_desc_chain->st_header.die   = (uint32_t) ETHER_TSN_TX_DESC_DIE_ENABLE;
        p_desc_chain->st_header.axie  = (uint32_t) 0U;
        p_desc_chain->st_header.dse   = (uint32_t) 0U;
        p_desc_chain->st_header.info0 = (uint32_t) 0U;
        p_desc_chain->st_header.ds    = (uint32_t) 0U;
        p_desc_chain->dptr            = (uint32_t) p_desc_chain_temp; /* PRQA S 0329 # Advisory R-11.4 */
        p_desc_chain->info1[0]        = 0U;
        p_desc_chain->info1[1]        = 0U;
        p_desc_chain->st_header.dt    = (uint32_t) ETHER_TSN_DESC_EOS;

        result = r_ether_tsn_tx_desc_learning(p_ctrl, 0U, (uint32_t) p_desc_chain_temp); /* PRQA S 0329 # Advisory R-11.4 */

        /* Transmission Request */
        if (ETHER_TSN_OK == result)
        {
            result  = r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_DISABLE_MODE);
            result |= r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_OPERATION_MODE);
            if (ETHER_TSN_OK == result)
            {
                /* Transmit start request */
                p_ctrl->p_reg_axibmi->TRCR0 = ETHER_TSN_TRCR0_TRANSMIT_ENABLE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                tick_start = 0U;
                (void) r_ether_tsn_get_counter_value(&tick_start);
                do
                {
                    tick_elap = r_ether_tsn_get_time_out_value(tick_start);
                } while ((ETHER_TSN_TDIS_0_MASK !=
                          (p_ctrl->p_reg_axibmi->TDIS0 & ETHER_TSN_TDIS_0_MASK)) && /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                         (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

                if (ETHER_TSN_TIMEOUT_COUNT < tick_elap)
                {
                    result = ETHER_TSN_NOT_OK;
                }
                else
                {
                    /* Clear the interrupt status bit */
                    p_ctrl->p_reg_axibmi->TDIS0 = ETHER_TSN_TDIS_0_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                }
            }
        }
    }
    else
    {
        result = ETHER_TSN_NOT_OK;
    }

    /* Software Reset */
    if (ETHER_TSN_OK == result)
    {
        result = r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_DISABLE_MODE);
        if (ETHER_TSN_OK == result)
        {
            p_ctrl->p_reg_tsnes->SWR = ETHER_TSN_SOFTWARE_RESET_ENABLE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            tick_start               = 0U;
            (void) r_ether_tsn_get_counter_value(&tick_start);
            do
            {
                tick_elap = r_ether_tsn_get_time_out_value(tick_start);
            } while ((ETHER_TSN_GIS_SWRCF_MASK != (p_ctrl->p_reg_tsnes->GIS & ETHER_TSN_GIS_SWRCF_MASK)) && /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                     (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

            if (ETHER_TSN_TIMEOUT_COUNT < tick_elap)
            {
                result = ETHER_TSN_NOT_OK;
            }
            else
            {
                /* Clear SoftWare Reset Completion Flag */
                p_ctrl->p_reg_tsnes->GIS                = ETHER_TSN_GIS_SWRCF_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                g_ether_tsn_tx_ram_init_state[ctrl_idx] = ETHER_TSN_TRUE;
            }
        }
    }

    /* Free allocated RAM memories */
    if (ETHER_TSN_NULL_PTR != p_data)
    {
        r_ether_tsn_util_ram_free(&gp_ether_tsn_heap[ctrl_idx], (void *) p_data);
    }

    if (ETHER_TSN_NULL_PTR != p_desc_chain_temp)
    {
        r_ether_tsn_util_ram_free(&gp_ether_tsn_heap[ctrl_idx], (void *) p_desc_chain_temp);
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_tsn_util_ram_alloc_ptr
 * Description                   This Allocate RAM with given size
 * @param[in] pp_heap            Pointer to Heap manager type structure
 * @param[in] ram_size           Add Ram size to CPU
 ***********************************************************************************************************************/
static void * r_ether_tsn_util_ram_alloc_ptr (void ** const pp_heap, const uint32_t ram_size)
{
    ether_tsn_heap_manager_type_t ** pp_mem_mgr = (ether_tsn_heap_manager_type_t **) pp_heap; /* PRQA S 0310 # DP_MSR_R-11.3_UC1 */

    /* Calculate required size with alignment */
    const uint32_t required_size =
        ETHER_TSN_ALIGN_TO_CPU(ram_size + (uint32_t) sizeof(ether_tsn_heap_manager_type_t));  /* PRQA S 3469 # Advisory D-4.9 */

    ether_tsn_heap_manager_type_t * p_heap_node;
    ether_tsn_heap_manager_type_t * p_new_block;
    void * p_ret_address;
    p_ret_address = ETHER_TSN_NULL_PTR;

    if (ETHER_TSN_NULL_PTR != pp_mem_mgr)
    {
        /* Loop through linked list of heap nodes */
        p_heap_node = *pp_mem_mgr;
        do
        {
            /* Check if block is free and large enough */
            if ((ETHER_TSN_FALSE == p_heap_node->heap_lock) && (p_heap_node->heap_size >= required_size)) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            {
                /* Check if enough space for separate allocation */
                if ((p_heap_node->heap_size - required_size) > ((uint32_t) sizeof(ether_tsn_heap_manager_type_t)))
                {
                    /* Create a new heap node for remaining space */
                    p_new_block =
                        (ether_tsn_heap_manager_type_t *) ((uint8_t *) p_heap_node + required_size);              /* PRQA S 0310,0488,3305 # DP_MSR_R-11.3_UC1, Advisory R-18.4, DP_MSR_R-11.3_UC2 */
                    p_new_block->heap_size = p_heap_node->heap_size - required_size;
                    p_new_block->heap_lock = ETHER_TSN_FALSE;                                                     /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */

                    /* Update linked list pointers */
                    ether_tsn_heap_manager_type_t * temp = (ether_tsn_heap_manager_type_t *) p_heap_node->p_next; /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */

                    temp->p_prev = (struct ether_tsn_heap_manager_type_t *) p_new_block;                          /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */

                    p_new_block->p_next = p_heap_node->p_next;

                    p_heap_node->p_next = (struct ether_tsn_heap_manager_type_t *) p_new_block;                   /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */

                    p_new_block->p_prev = (struct ether_tsn_heap_manager_type_t *) p_heap_node;                   /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */

                    /* Update original block info */
                    p_heap_node->heap_size = required_size;
                    p_heap_node->heap_lock = ETHER_TSN_TRUE;                                                      /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */

                    /* Set next heap node search start position */
                    *pp_mem_mgr = p_new_block;
                }
                else
                {
                    /* Use entire block without splitting */
                    p_heap_node->heap_lock = ETHER_TSN_TRUE;                                                /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
                }

                p_ret_address = (void *) ((uint8_t *) p_heap_node + sizeof(ether_tsn_heap_manager_type_t)); /* PRQA S 0488 # Advisory R-18.4 */
                break;
            }
            else
            {
                p_heap_node = (ether_tsn_heap_manager_type_t *) p_heap_node->p_next; /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
            }
        } while (p_heap_node != *pp_mem_mgr);
    }

    return p_ret_address;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_tsn_mhd_mode_setting
 * Description                   Set the mode of MHD
 * @param[in] p_ctrl            Pointer to the control block for the channel
 * @param[in] mode               Config mode for ether tsn
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_mhd_mode_setting (ether_tsn_instance_ctrl_t * p_ctrl, uint32_t mode) /* PRQA S 3673 # Advisory R-8.13 */
{
    ether_tsn_tick_type tick_start = 0U;
    ether_tsn_tick_type tick_elap  = 0U;
    ether_tsn_type      result     = (uint8_t) ETHER_TSN_NOT_OK;

    p_ctrl->p_reg_tsnes->OCR = (uint8_t) mode; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Wait while CONFIG mode is achieved (timeout handling)*/
    tick_start = 0;
    (void) r_ether_tsn_get_counter_value(&tick_start);
    do
    {
        tick_elap = r_ether_tsn_get_time_out_value(tick_start);

        /* Check specific OSR.OPS bit*/
    } while (((p_ctrl->p_reg_tsnes->OSR & (1UL << mode)) == 0UL) && (tick_elap <= ETHER_TSN_TIMEOUT_COUNT)); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (tick_elap <= ETHER_TSN_TIMEOUT_COUNT)
    {
        /* Success if mode change happened within timeout */
        result = (uint8_t) ETHER_TSN_OK;
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_get_counter_value
 * Description           This API Reads a list with drop counter values of the corresponding controller.
 * @param[in] p_value    Pointer to value check counter
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_get_counter_value (ether_tsn_tick_ref_type * p_value)
{
    if (tsn_counter_check == ETHER_TSN_COUNTER_MAX_VALUE)
    {
        /* Reset counter */
        tsn_counter_check = ETHER_TSN_INIT_COUNTER_VALUE;
    }

    tsn_counter_check++;
    *p_value = tsn_counter_check;

    return ETHER_TSN_OK;
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_tsn_get_time_out_value
 * Description                       This returns the elapsed timeout value of  Os timer
 * @param[in] time_out_count_init    Update counter init value
 ***********************************************************************************************************************/
static ether_tsn_tick_type r_ether_tsn_get_time_out_value (ether_tsn_tick_type time_out_count_init)
{
    ether_tsn_tick_type time_out_count_curr    = 0U;
    ether_tsn_tick_type time_out_count_elapsed = 0U;

    /* Get the current tick value from OS Counter */
    (void) r_ether_tsn_get_counter_value(&time_out_count_curr);
    if (time_out_count_curr >= time_out_count_init)
    {
        time_out_count_elapsed = time_out_count_curr - time_out_count_init;
    }
    else
    {
        time_out_count_elapsed = ((uint32_t) ETHER_TSN_COUNTER_MAX_VALUE - time_out_count_init) + time_out_count_curr;
    }

    return time_out_count_elapsed;
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_tsn_tx_ram_init_desc_config
 * Description                       TxRam Description Configuration
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] p_desc_chain           Pointer to descriptor chain
 * @param[in] dptr                   Adress of allocated RAM
 ***********************************************************************************************************************/
static void r_ether_tsn_tx_ram_init_desc_config (ether_tsn_instance_ctrl_t       * p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                                 ether_tsn_ext_desc_type_t * const p_desc_chain,
                                                 const uint32_t                    dptr)
{
    p_desc_chain->st_header.die  = (uint32_t) 0UL;
    p_desc_chain->st_header.axie = (uint32_t) 0UL;
    p_desc_chain->st_header.dse  = (uint32_t) 0UL;

    /* FCS needs to be already included in the tx frame since */
    /* without it, the frame size exceeds the maximum frame size (16KB)*/
    p_desc_chain->st_header.info0 = (uint32_t) ETHER_TSN_DESC_FCS_INCLUDED;
    p_desc_chain->st_header.ds    = (p_ctrl->p_ether_cfg->ether_buffer_size & ETHER_TSN_CONVERT_TO_SMALL_BIT);
    p_desc_chain->dptr            = (uint32_t) dptr;
    p_desc_chain->info1[0]        = (uint32_t) ETHER_TSN_INIT_FRAME_LENGTH;
    p_desc_chain->info1[1]        = (uint32_t) 0UL;
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_tsn_tx_desc_learning
 * Description                       Learning tx descriptor chain table
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] entry_number           Number check Tx descriptor address table learning Setting
 * @param[in] chain_addr             Adress of allocated RAM
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_tx_desc_learning (ether_tsn_instance_ctrl_t * p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                                    uint32_t                    entry_number,
                                                    uint32_t                    chain_addr)
{
    ether_tsn_tick_type tick_start = 0U;
    ether_tsn_tick_type tick_elap  = 0U;
    ether_tsn_type      result     = (uint8_t) ETHER_TSN_NOT_OK;

    p_ctrl->p_reg_axibmi->TATLS0 = ETHER_TSN_TATLS0_CONFIG | (entry_number << 24UL); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->p_reg_axibmi->TATLS1 = chain_addr;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    p_ctrl->p_reg_axibmi->TATLR = ETHER_TSN_TATLR_LEARNING;                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Get current tick count */
    (void) r_ether_tsn_get_counter_value(&tick_start);

    /* Loop until learning completes or timeout occurs */
    do
    {
        tick_elap = r_ether_tsn_get_time_out_value(tick_start);
    } while (((p_ctrl->p_reg_axibmi->TATLR & ETHER_TSN_TATLR_LEARNING) != 0UL) &&                                 /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
             (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

    /* Check for timeout or failure */
    if ((tick_elap > ETHER_TSN_TIMEOUT_COUNT) || (0UL != (p_ctrl->p_reg_axibmi->TATLR & ETHER_TSN_TATLR_FAILED))) /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /* Timeout or learning failed */
        result = ETHER_TSN_NOT_OK;
    }
    else
    {
        /* Learning successful */
        result = ETHER_TSN_OK;
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_tsn_util_ram_free
 * Description                       this free RAM at given address
 * @param[in] pp_heap                Pointer to heap manager type structure
 * @param[in] p_mem_addr             Pointer to number get the address of memory node
 ***********************************************************************************************************************/
static void r_ether_tsn_util_ram_free (void ** pp_heap, void * p_mem_addr)
{
    ether_tsn_heap_manager_type_t ** pp_mem_mgr = (ether_tsn_heap_manager_type_t **) pp_heap; /* PRQA S 0310 # DP_MSR_R11.3_UC1 */
    ether_tsn_heap_manager_type_t  * p_free_node;

    if ((ETHER_TSN_NULL_PTR != pp_mem_mgr) && (ETHER_TSN_NULL_PTR != p_mem_addr))
    {
        /* Get the address of memory node */
        p_free_node =
            (ether_tsn_heap_manager_type_t *) ((uint8_t *) p_mem_addr - sizeof(ether_tsn_heap_manager_type_t));    /* PRQA S 0310,0488,3305,0316 # DP_MSR_R11.3_UC1, Advisory R-18.4, DP_MSR_R11.3_UC2,Advisory R-11.5 */

        /* If the next node is empty and the next node does not to LpFreeNode */
        ether_tsn_heap_manager_type_t const * p_node_temp = (ether_tsn_heap_manager_type_t *) p_free_node->p_next; /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
        if ((p_node_temp->heap_lock == ETHER_TSN_FALSE) &&                                                         /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            ((struct ether_tsn_heap_manager_type_t *) p_free_node != p_free_node->p_next))                         /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
        {
            if ((struct ether_tsn_heap_manager_type_t *) p_free_node < p_free_node->p_next)                        /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
            {
                /* Merge next block into current block */
                if (p_free_node->p_next == (struct ether_tsn_heap_manager_type_t *) *pp_mem_mgr)                   /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
                {
                    /* Update starting point for next search */
                    *pp_mem_mgr = p_free_node;
                }

                /* Update linked list pointers */
                ether_tsn_heap_manager_type_t const * p_node_temp_update =
                    (ether_tsn_heap_manager_type_t const *) p_free_node->p_next;                             /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
                p_free_node->heap_size += p_node_temp_update->heap_size;

                ether_tsn_heap_manager_type_t const * p_node_temp_update_next =
                    (ether_tsn_heap_manager_type_t const *) p_free_node->p_next;                             /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */

                ether_tsn_heap_manager_type_t * p_node_temp_update_next_next =
                    (ether_tsn_heap_manager_type_t *) p_node_temp_update_next->p_next;                       /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */

                p_node_temp_update_next_next->p_prev = (struct ether_tsn_heap_manager_type_t *) p_free_node; /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */

                ether_tsn_heap_manager_type_t const * p_node_temp_update_next_next_next =
                    (ether_tsn_heap_manager_type_t const *) p_free_node->p_next;                             /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
                p_free_node->p_next = p_node_temp_update_next_next_next->p_next;
            }
        }

        ether_tsn_heap_manager_type_t const * p_node_temp_prev =
            (ether_tsn_heap_manager_type_t const *) p_free_node->p_prev;                    /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */

        /* Check if previous node is free and not the same node */
        if ((ETHER_TSN_FALSE == p_node_temp_prev->heap_lock) &&                             /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            ((struct ether_tsn_heap_manager_type_t *) p_free_node != p_free_node->p_prev))  /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
        {
            if ((struct ether_tsn_heap_manager_type_t *) p_free_node > p_free_node->p_prev) /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
            {
                /* Merge current block into previous block */
                if (p_free_node == *pp_mem_mgr)
                {
                    /* Update starting point for next search */
                    *pp_mem_mgr = (ether_tsn_heap_manager_type_t *) p_free_node->p_prev;                                 /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
                }

                /* Update linked list pointers */
                ether_tsn_heap_manager_type_t * p_previous_node = (ether_tsn_heap_manager_type_t *) p_free_node->p_prev; /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
                p_previous_node->heap_size += p_free_node->heap_size;
                ether_tsn_heap_manager_type_t * p_previous_node_update =
                    (ether_tsn_heap_manager_type_t *) p_free_node->p_next;                                               /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
                p_previous_node_update->p_prev = p_free_node->p_prev;

                ether_tsn_heap_manager_type_t * p_previous_node_update_prev =
                    (ether_tsn_heap_manager_type_t *) p_free_node->p_prev;                                               /* PRQA S 0308,0310 # DP_MSR_R11.2_UC1 & DP_MSR_R11.3_UC1 */
                p_previous_node_update_prev->p_next = p_free_node->p_next;
            }
        }

        p_free_node->heap_lock = ETHER_TSN_FALSE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
    }
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_axbmi_init
 * Description               Initialization process for AXIBMI
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of a Queue
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_axbmi_init (ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_tsn_extended_cfg_t const * p_extend = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    ether_tsn_type      result                = ETHER_TSN_OK;
    ether_tsn_tick_type tick_start            = 0U;
    ether_tsn_tick_type tick_elap             = 0U;
    uint32_t            queue_piority_val     = 0U;
    uint32_t            frame_type_val        = 0U;
    uint32_t            count                 = 0U;

    /* Configure AXIBMI */
    result  = r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_DISABLE_MODE);
    result |= r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_CONFIG_MODE);

    if (ETHER_TSN_OK == result)
    {
        p_ctrl->p_reg_axibmi->RR = (uint8_t) (ETHER_TSN_RATRR_RESET | ETHER_TSN_TATRR_RESET); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* RAM Reset Waiting */
        (void) r_ether_tsn_get_counter_value(&tick_start);
        do
        {
            tick_elap = r_ether_tsn_get_time_out_value(tick_start);
        } while (((uint32_t) (ETHER_TSN_RATRR_RESET | ETHER_TSN_TATRR_RESET) !=
                  (uint32_t) (p_ctrl->p_reg_axibmi->RR)) && /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                 (tick_elap <= (uint32_t) ETHER_TSN_TIMEOUT_COUNT));

        if (ETHER_TSN_TIMEOUT_COUNT < tick_elap)
        {
            result = ETHER_TSN_NOT_OK;
        }
    }

    if (ETHER_TSN_OK == result)
    {
        /* Use Express frame (classical frame) */
        p_ctrl->p_reg_axibmi->AXIWC = (uint32_t) ETHER_TSN_AXIWC_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_axibmi->AXIRC = (uint32_t) ETHER_TSN_AXIRC_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Set Tx Queue priority and Frame type */
        for (count = 0U; count < (uint32_t) p_extend->queue_config.number_of_tx_queue; count++)
        {
            queue_piority_val |= p_extend->queue_config.p_tx_queue_config[count].queue_id << (4UL * count);
            frame_type_val    |= (uint32_t) p_extend->queue_config.p_tx_queue_config[count].frame_type << count;
        }

        p_ctrl->p_reg_axibmi->TDPC0 = queue_piority_val;        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_axibmi->TFT   = (uint8_t) frame_type_val; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Tx/Rx descriptor chain setting */
        result = r_ether_tsn_tx_rx_desc_config(p_ctrl, ctrl_idx);

        if (ETHER_TSN_OK == result)
        {
            /* Set circular address for Tx bufffers */
            r_ether_tsn_util_ram_set_circular_addr(gp_ether_tsn_heap[ctrl_idx]);
        }

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)

        /* TS descriptor chain setting */
        r_ether_tsn_ts_desc_config(p_ctrl, ctrl_idx);

        /* TS Descriptor Data Interrupt */
        p_ctrl->p_reg_axibmi->TSDIE = (uint8_t) ETHER_TSN_ETND_TSDIE_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

        /* Set AXIBMI Error Interrupt */
        p_ctrl->p_reg_axibmi->EIE0 = (uint16_t) ETHER_TSN_EIE0_CONFIG;       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* TX Descriptor Data Interrupt */
        if (p_extend->ether_tsn_ctl_mode == ETHER_TSN_CTRL_INTERRUPT_MODE)
        {
            /*
             *  Data Interrupt Enable
             *  7 to 0: TX Descriptor Data Interrupt Common Enable j.
             *  15 to 8: RX Descriptor Data Interrupt Common Enable i.
             *  16: TS Descriptor Data Interrupt Common Enable.Interrupt enable setting of DIS.TSDICS.
             */
            p_ctrl->p_reg_axibmi->DIE = (uint32_t) ETHER_TSN_DIE_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* TX Descriptor Data Interrupt */
            if (p_extend->ether_tsn_cfg_type_tx_isr == ETHER_TSN_CTRL_ENABLE_TX_INTERRUPT)
            {
                p_ctrl->p_reg_axibmi->TDIE0 = (uint32_t) ETHER_TSN_TDIE0_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }

            /* RX Descriptor Data Interrupt */
            if (p_extend->ether_tsn_cfg_type_rx_isr == ETHER_TSN_CTRL_ENABLE_RX_INTERRUPT)
            {
                p_ctrl->p_reg_axibmi->RDIE0 = (uint32_t) ETHER_TSN_RDIE0_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }

            /* RX Descriptor Data Interrupt */
            if (p_extend->ether_tsn_cfg_type_rx_isr == ETHER_TSN_CTRL_ENABLE_RX_INTERRUPT)
            {
                p_ctrl->p_reg_axibmi->RFEIE0 = (uint32_t) ETHER_TSN_RFEIE0_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            }
        }
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_tx_rx_desc_config
 * Description               Create descriptor chain & learning descriptor.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of a Queue
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_tx_rx_desc_config (ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_tsn_type result = ETHER_TSN_OK;
    ether_tsn_extended_cfg_t const * p_extend = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */

    /* Pointer to ether_tsn_ext_desc_type (variable) */
    ether_tsn_ext_desc_type_t * p_desc_chain = ETHER_TSN_NULL_PTR;                                                  /* PRQA S 0317 # Advisory R-11.5 */

    /* Pointer to ether_tsn_ext_desc_with_ts_type (variable) */
    ether_tsn_ext_desc_with_ts_type_t * p_desc_ts_chain = ETHER_TSN_NULL_PTR;                                       /* PRQA S 0317 # Advisory R-11.5 */

    uint8_t * p_rx_buffer     = ETHER_TSN_NULL_PTR;                                                                 /* PRQA S 0317 # Advisory R-11.5 */
    uint32_t  tx_queue_max    = 0U;
    uint32_t  rx_queue_max    = 0U;
    uint32_t  rx_buffer_index = 0U;
    uint32_t  cnt_i           = 0U;
    uint32_t  cnt_j           = 0U;

    tx_queue_max = p_extend->queue_config.number_of_tx_queue;
    rx_queue_max = ETHER_TSN_MAX_RX_QUEUE;

    /* Typecase to (uint32_t) pointer. */
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_buf_tx_cnt =
        (uint32_t *) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx], sizeof(uint32_t) * tx_queue_max); /* PRQA S 0316 # Advisory R-11.5 */
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_tx_alloc_cnt =
        (uint32_t *) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx], sizeof(uint32_t) * tx_queue_max); /* PRQA S 0316 # Advisory R-11.5 */
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_tx_desc =
        (ether_tsn_ext_desc_type_t **) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[                           /* PRQA S 0316 # Advisory R-11.5 */
                                                                          ctrl_idx],
                                                                      sizeof(
                                                                          ether_tsn_ext_desc_type_t
                                                                          *) *
                                                                      tx_queue_max);
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_next_tx_desc =
        (ether_tsn_ext_desc_type_t **) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ /* PRQA S 0316 # Advisory R-11.5 */
                                                                          ctrl_idx],
                                                                      sizeof(
                                                                          ether_tsn_ext_desc_type_t
                                                                          *) *
                                                                      tx_queue_max);

    /* PRQA S 0316 ++ # Advisory R-11.5 */
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_rx_desc =
        (ether_tsn_ext_desc_with_ts_type_t **) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx],
                                                                              sizeof(ether_tsn_ext_desc_with_ts_type_t *) *
                                                                              rx_queue_max);
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_update =
        (ether_tsn_ext_desc_with_ts_type_t **) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx],
                                                                              sizeof(ether_tsn_ext_desc_with_ts_type_t *) *
                                                                              rx_queue_max);
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_release =
        (ether_tsn_ext_desc_with_ts_type_t **) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx],
                                                                              sizeof(ether_tsn_ext_desc_with_ts_type_t *) *
                                                                              rx_queue_max);

    /* PRQA S 0316 -- */

    if ((ETHER_TSN_NULL_PTR == g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_buf_tx_cnt) ||
        (ETHER_TSN_NULL_PTR == g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_tx_alloc_cnt) ||
        (ETHER_TSN_NULL_PTR == g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_tx_desc) ||
        (ETHER_TSN_NULL_PTR == g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_next_tx_desc) ||
        (ETHER_TSN_NULL_PTR == g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_rx_desc) ||
        (ETHER_TSN_NULL_PTR == g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_update) ||
        (ETHER_TSN_NULL_PTR == g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_release))
    {
        result = ETHER_TSN_NOT_OK;
    }

    /* Create Tx descriptor */
    for (cnt_i = 0U; (cnt_i < tx_queue_max) && (ETHER_TSN_OK == result); cnt_i++)
    {
        g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_buf_tx_cnt[cnt_i]   = (uint32_t) 0UL;
        g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_tx_alloc_cnt[cnt_i] = (uint32_t) 0UL;
        p_desc_chain =
            (ether_tsn_ext_desc_type_t *) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx], /* PRQA S 0316 # Advisory R-11.5 */
                                                                         (uint32_t) (((uint32_t) p_extend->
                                                                                      queue_config.p_tx_queue_config[
                                                                                          cnt_i].queue_bufs +
                                                                                      ETHER_TSN_CYCLIC_DESC_NUM) *
                                                                                     ETHER_TSN_EXT_DESC_SIZE));
        if (ETHER_TSN_NULL_PTR == p_desc_chain)
        {
            result = ETHER_TSN_NOT_OK;
            break;
        }

        /* Store the head Tx descriptor address */
        g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_next_tx_desc[cnt_i] = p_desc_chain;
        g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_tx_desc[cnt_i] = p_desc_chain;

        /* Set descriptor chain */
        for (cnt_j = 0U; cnt_j < (uint32_t) p_extend->queue_config.p_tx_queue_config[cnt_i].queue_bufs; cnt_j++)
        {
            /* Set frame empty type */
            p_desc_chain->st_header.die   = (uint32_t) 0U;
            p_desc_chain->st_header.axie  = (uint32_t) 0U;
            p_desc_chain->st_header.dse   = (uint32_t) 0U;
            p_desc_chain->st_header.info0 = (uint32_t) 0U;
            p_desc_chain->st_header.ds    = (uint32_t) 0U;
            p_desc_chain->dptr            = 0U;
            p_desc_chain->info1[0]        = 0U;
            p_desc_chain->info1[1]        = 0U;
            p_desc_chain->st_header.dt    = (uint32_t) ETHER_TSN_DESC_FEMPTY;

            /* Next data descriptor */
            p_desc_chain++;
        }

        /* Set link type for cyclic descriptor */
        p_desc_chain->st_header.die   = (uint32_t) 0U;
        p_desc_chain->st_header.axie  = (uint32_t) 0U;
        p_desc_chain->st_header.dse   = (uint32_t) 0U;
        p_desc_chain->st_header.info0 = (uint32_t) 0U;
        p_desc_chain->st_header.ds    = (uint32_t) 0U;
        p_desc_chain->dptr            = (uint32_t) g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_tx_desc[cnt_i]; /* PRQA S 0329 # Advisory R-11.4 */ // Typecase address to uint32_t.
        p_desc_chain->info1[0]        = 0U;
        p_desc_chain->info1[1]        = 0U;
        p_desc_chain->st_header.dt    = (uint32_t) ETHER_TSN_DESC_LINK;
    }

    /* Create Rx descriptor */
    rx_buffer_index = (uint32_t) 0UL;
    for (cnt_i = 0U; (cnt_i < rx_queue_max) && (ETHER_TSN_OK == result); cnt_i++)
    {
        p_desc_ts_chain =
            (ether_tsn_ext_desc_with_ts_type_t *) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx], /* PRQA S 0316 # Advisory R-11.5 */
                                                                                 (uint32_t) (((uint32_t) p_ctrl->
                                                                                              p_ether_cfg->
                                                                                              num_rx_descriptors +
                                                                                              ETHER_TSN_CYCLIC_DESC_NUM)
                                                                                             *
                                                                                             ETHER_TSN_EXT_DESC_TS_SIZE));
        if (ETHER_TSN_NULL_PTR == p_desc_ts_chain)
        {
            result = ETHER_TSN_NOT_OK;
            break;
        }

        /* Store the head Rx descriptor address */
        g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_rx_desc[cnt_i] = p_desc_ts_chain;

        /* Store current Rx descriptor for ZeroCopy usage to rx buffer */
        g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_update[cnt_i] = p_desc_ts_chain;

        /* Set Rx descriptor chain */
        for (cnt_j = 0U; cnt_j < (uint32_t) p_ctrl->p_ether_cfg->num_rx_descriptors; cnt_j++)
        {
            p_rx_buffer =
                (uint8_t *) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx], /* PRQA S 0316 # Advisory R-11.5 */
                                                           ((uint32_t) p_ctrl->p_ether_cfg->ether_buffer_size +
                                                            (uint32_t) sizeof(ether_tsn_buf_idx_type) +
                                                            (uint32_t) ETHER_TSN_RX_DPTR_OFFSET));
            if (ETHER_TSN_NULL_PTR == p_rx_buffer)
            {
                /* If a callback is provided, then call it with callback argument. */
                r_ether_tsn_call_callback(p_ctrl, ETHER_EVENT_ERROR_LACK_OF_RESOURCES);

                /* Allocation memory error due to lack of resources */
                result = ETHER_TSN_NOT_OK;
                break;
            }

            /* Add receive buffer index */
            *((ether_tsn_buf_idx_type *) p_rx_buffer) = rx_buffer_index; /* PRQA S 0310, 3305 # DP_MSR_R11.3_UC1 & DP_MSR_R11.3_UC2 */
            rx_buffer_index++;

            /* Set frame empty type */
            p_desc_ts_chain->st_header.die   = (uint32_t) ETHER_TSN_RX_DESC_DIE_ENABLE;
            p_desc_ts_chain->st_header.axie  = (uint32_t) 0U;
            p_desc_ts_chain->st_header.dse   = (uint32_t) 0U;
            p_desc_ts_chain->st_header.info0 = (uint32_t) 0U;
            p_desc_ts_chain->st_header.ds    =
                (p_ctrl->p_ether_cfg->ether_buffer_size & ETHER_TSN_CONVERT_TO_SMALL_BIT);
            p_desc_ts_chain->dptr =
                (uint32_t) (p_rx_buffer + (uint32_t) sizeof(ether_tsn_buf_idx_type) + ETHER_TSN_RX_DPTR_OFFSET); /* PRQA S 0488,0329 # Advisory R-18.4, Advisory R-11.4 */
            p_desc_ts_chain->info1[0]     = 0U;
            p_desc_ts_chain->info1[1]     = 0U;
            p_desc_ts_chain->tsns         = 0U;
            p_desc_ts_chain->tss          = 0U;
            p_desc_ts_chain->st_header.dt = (uint32_t) ETHER_TSN_DESC_FEMPTY;

            /* Next data descriptor */
            p_desc_ts_chain++;
        }

        /* Set link type for cyclic descriptor */
        p_desc_ts_chain->st_header.die   = (uint32_t) 0U;
        p_desc_ts_chain->st_header.axie  = (uint32_t) 0U;
        p_desc_ts_chain->st_header.dse   = (uint32_t) 0U;
        p_desc_ts_chain->st_header.info0 = (uint32_t) 0U;
        p_desc_ts_chain->st_header.ds    = (uint32_t) 0U;
        p_desc_ts_chain->dptr            = (uint32_t) g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_rx_desc[cnt_i]; /* PRQA S 0329 # Advisory R-11.4 */
        p_desc_ts_chain->info1[0]        = 0U;
        p_desc_ts_chain->info1[1]        = 0U;
        p_desc_ts_chain->tsns            = 0U;
        p_desc_ts_chain->tss             = 0U;
        p_desc_ts_chain->st_header.dt    = (uint32_t) ETHER_TSN_DESC_LINK;

        /* Store the LINK descriptor, R_ETHER_TSN_Read() restores it after the controller used it */
        gp_ether_tsn_rx_link_desc[ctrl_idx][cnt_i] = p_desc_ts_chain;
    }

    if (ETHER_TSN_OK == result)
    {
        /* Learning Tx descriptor table */
        for (cnt_i = 0U; cnt_i < tx_queue_max; cnt_i++)
        {
            result |=
                r_ether_tsn_tx_desc_learning(p_ctrl,
                                             cnt_i,
                                             (uint32_t) g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_tx_desc[cnt_i]); /* PRQA S 0329 # Advisory R-11.4 */
        }

        for (cnt_i = 0U; cnt_i < rx_queue_max; cnt_i++)
        {
            result |=
                r_ether_tsn_rx_desc_learning(p_ctrl,
                                             cnt_i,
                                             (uint32_t) g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_rx_desc[cnt_i]); /* PRQA S 0329 # Advisory R-11.4 */
        }
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_util_ram_set_circular_addr
 * Description           This set the circular address
 * @param[in] p_heap     Pointer to heap manager type structure
 ***********************************************************************************************************************/
static void r_ether_tsn_util_ram_set_circular_addr (void * p_heap)
{
    /* Cast pointer */
    ether_tsn_heap_manager_type_t * p_mem_mgr = (ether_tsn_heap_manager_type_t *) p_heap; /* PRQA S 0316 # Advisory R-11.5 */

    if (ETHER_TSN_NULL_PTR != p_mem_mgr)
    {
        /* Set next and previous pointers to point to itself */
        p_mem_mgr->p_next = p_heap;    /* PRQA S 0317 # Advisory R-11.5 */
        p_mem_mgr->p_prev = p_heap;    /* PRQA S 0317 # Advisory R-11.5 */
    }
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_mhd_init
 * Description           Initialization process of MHD, TAS, CBS Function
 * @param[in] p_ctrl     Pointer to the control block for the channel
 ***********************************************************************************************************************/
static void r_ether_tsn_mhd_init (ether_tsn_instance_ctrl_t * p_ctrl)                                               /* PRQA S 3673 # Advisory R-8.13 */
{
    ether_tsn_extended_cfg_t const * p_extend = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */

    p_ctrl->p_reg_tsnes->TCIE = (uint32_t) ETHER_TSN_TCIE_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TIE2 = (uint32_t) ETHER_TSN_TIE2_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->RIE  = (uint32_t) ETHER_TSN_RIE_CONFIG;                                                    /* PRQA S 4464,0662 # DP_MSR_R-10.3_UC1,DP_MSR_D-1.1_UC1,DP_MSR_R-1.2_UC1 */

    /*Max Frame Size 0x4095*/
    p_ctrl->p_reg_tsnes->TMS0 = (uint16_t) p_extend->queue_config.p_tx_queue_config[0].max_frame_size;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TMS1 = (uint16_t) p_extend->queue_config.p_tx_queue_config[1].max_frame_size;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TMS2 = (uint16_t) p_extend->queue_config.p_tx_queue_config[2].max_frame_size;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TMS3 = (uint16_t) p_extend->queue_config.p_tx_queue_config[3].max_frame_size;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TMS4 = (uint16_t) p_extend->queue_config.p_tx_queue_config[4].max_frame_size;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TMS5 = (uint16_t) p_extend->queue_config.p_tx_queue_config[5].max_frame_size;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TMS6 = (uint16_t) p_extend->queue_config.p_tx_queue_config[6].max_frame_size;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TMS7 = (uint16_t) p_extend->queue_config.p_tx_queue_config[7].max_frame_size;              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /*Transmission FIFO Size*/
    p_ctrl->p_reg_tsnes->TFS0 = (uint32_t) ETHER_TSN_TFS_CONFIG;                                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TFS1 = (uint32_t) ETHER_TSN_TFS_CONFIG;                                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TFS2 = (uint32_t) ETHER_TSN_TFS_CONFIG;                                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TFS3 = (uint32_t) ETHER_TSN_TFS_CONFIG;                                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /*Transmission General Configuration register 1*/
    p_ctrl->p_reg_tsnes->TGC1 = (uint16_t) ETHER_TSN_TGC1_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /*Transmission General Configuration register 2*/
    p_ctrl->p_reg_tsnes->TGC2 |=                                                                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                 ((uint32_t) p_extend->ether_tsn_queue_contanins_traffic << R_TSNES0_TGC2_TQPE_Pos);
    p_ctrl->p_reg_tsnes->TGC2 |=                                                                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                 ((uint32_t) p_extend->ether_tsn_transmission_fragment_size << R_TSNES0_TGC2_TFC_Pos);

    /*Transmission Configuration Register 1,2,3,4*/
    p_ctrl->p_reg_tsnes->TCR1  = (uint32_t) ETHER_TSN_TCR_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TCR2  = (uint32_t) ETHER_TSN_TCR_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TCR3  = (uint32_t) ETHER_TSN_TCR_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TCR4  = (uint32_t) ETHER_TSN_TCR_CONFIG;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->RGC   = ETHER_TSN_RGC_CONFIG;                                                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->RDFCR = ETHER_TSN_RDFCR_CONFIG;                                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->RCFCR = ETHER_TSN_RCFCR_CONFIG;                                                            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* TAS configuration */
    p_ctrl->p_reg_tsnes->TPTPC = ETHER_TSN_GPTP_TIMER_DOMAIN;                                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TTML  = ETHER_TSN_TRANSMISSION_MIN_LATENCY;                                                /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_tsnes->TTJ   = ETHER_TSN_TRANSMISSION_JITTER;                                                     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CBS configuration */
    for (uint32_t cnt = 0U; cnt < (uint32_t) ETHER_TSN_CBS_CONFIG_NUM; cnt++)
    {
        /*CBS Admin Common configuration*/
        p_ctrl->p_reg_tsnes->CACC |= (uint8_t) (0x0U << cnt); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*CBS Admin Increment Value for 8 tx queues*/
        p_ctrl->p_reg_tsnes->CAIV0 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAIV1 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAIV2 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAIV3 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAIV4 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAIV5 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAIV6 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAIV7 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*CBS Admin Upper Limit for 8 tx queues*/
        p_ctrl->p_reg_tsnes->CAUL0 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAUL1 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAUL2 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAUL3 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAUL4 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAUL5 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAUL6 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_tsnes->CAUL7 = (uint32_t) 0;            /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*CBS Configuration Setting*/
        p_ctrl->p_reg_tsnes->CCS |= (uint8_t) (0x0U << cnt);  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_rmac_init
 * Description           Initialization process of RMAC System
 * @param[in] p_ctrl     Pointer to the control block for the channel
 ***********************************************************************************************************************/
static void r_ether_tsn_rmac_init (ether_tsn_instance_ctrl_t * p_ctrl)                                              /* PRQA S 3673 # Advisory R-8.13 */
{
    ether_tsn_extended_cfg_t const * p_extend = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */

    /* Initialize controller index */
    uint32_t ctrl_idx = p_ctrl->p_ether_cfg->channel;

    /* Set up MAC address upper part 16 bit */
    p_ctrl->p_reg_rmaca->MRMAC0 =      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                  (uint16_t) (g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr.h32 >>
                                              ETHER_TSN_MRMAC_CONFIG_MAC);

    /* Set up MAC address downer part 32 bit */
    p_ctrl->p_reg_rmaca->MRMAC1 =                                                                          /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                  (g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr.h32 << ETHER_TSN_MRMAC_CONFIG_MAC) |
                                  (g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr.l16);
    p_ctrl->p_reg_rmaca->MTFFC = (uint32_t) ETHER_TSN_MTFFC_CONFIG;                                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (ETHER_TSN_PAUSE_FRAME_ENABLE == p_extend->pause_frame)
    {
        p_ctrl->p_reg_rmaca->MTPFC =                                                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                     ((uint32_t) p_extend->pause_time_zero << ETHER_TSN_MTPFC_PFTTZ_POS) | /* PRQA S 1821 # DP_MSR_R-10.4_UC1 */
                                     (p_extend->pause_frame_retransmission_time << ETHER_TSN_MTPFC_PFRT_POS) |
                                     (p_extend->pause_time << ETHER_TSN_MTPFC_PT_POS);
    }

    /* Do not use automatic timestamp */
    p_ctrl->p_reg_rmaca->MTATC0 = (uint32_t) ETHER_TSN_MTATC_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Set Rx function */
    if (ETHER_TSN_PAUSE_FRAME_ENABLE == p_extend->pause_frame)
    {
        p_ctrl->p_reg_rmaca->MRGC =    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                    (uint8_t) (((uint32_t) p_extend->pause_time_zero << 2U) |
                                               ((uint32_t) p_extend->pause_frame << 1U));
    }

    /* 3 e-Frame */
    p_ctrl->p_reg_rmaca->MRFSCE = (uint32_t) p_ctrl->p_ether_cfg->ether_buffer_size;                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* 3 p-Frame */
    p_ctrl->p_reg_rmaca->MRFSCP = (uint32_t) p_ctrl->p_ether_cfg->ether_buffer_size;                                    /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Timestamp capture settings for Rx descriptor */
    p_ctrl->p_reg_rmaca->MTRC = ETHER_TSN_MTRC_CONFIG;                                                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Set Address Filtering function */
    p_ctrl->p_reg_rmaca->MRSCE = (uint32_t) ETHER_TSN_MRSCE_CONFIG;                                                     /* PRQA S 0499,0662 # DP_MSR_R-12.2_UC1,DP_MSR_D-1.1_UC1,DP_MSR_R-1.2_UC1 */
    p_ctrl->p_reg_rmaca->MRSCP = (uint32_t) ETHER_TSN_MRSCP_CONFIG;                                                     /* PRQA S 0499,0662 # DP_MSR_R-12.2_UC1,DP_MSR_D-1.1_UC1,DP_MSR_R-1.2_UC1 */

    /* Enable loopback mode */
    if (ETHER_TSN_LOOPBACK_ON == (uint32_t) p_extend->ether_loopback_config)                                            /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        p_ctrl->p_reg_rmaca->MLBC   = (uint8_t) ETHER_TSN_MLBC_CONFIG;                                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_rmaca->MRAFC |= (uint32_t) (ETHER_TSN_MRAFC_DA_SA_SAME_FRAMES);                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    p_ctrl->p_reg_rmaca->MRAFC |= (uint32_t) (((uint32_t) p_ctrl->p_ether_cfg->multicast << R_RMACA0_MRAFC_MCENE_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                              ((uint32_t) p_extend->ether_broadcast << R_RMACA0_MRAFC_BCENE_Pos) |
                                              ((uint32_t) p_extend->ether_unicast << R_RMACA0_MRAFC_UCENE_Pos));

    /* PHY Interface configuration */
    p_ctrl->p_reg_rmaca->MPIC = ((uint32_t) p_extend->ether_network_speed << R_RMACA0_MPIC_LSC_Pos) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                ((uint32_t) p_extend->ether_phy_interface << R_RMACA0_MPIC_PIS_Pos);
    p_ctrl->p_reg_rmaca->MPIC |= ETHER_TSN_MPIC_CONFIG;                                               /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
}

#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_sgmii_init
 * Description           Initialize SGMII for ETND HWUnit
 * @param[in] p_ctrl     Pointer to the control block for the channel
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_sgmii_init (ether_tsn_instance_ctrl_t * p_ctrl)
{
    ether_tsn_extended_cfg_t * p_extend = ((ether_tsn_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    ether_tsn_type             result   = ETHER_TSN_OK;

    ether_tsn_tick_type tick_start = 0U;
    ether_tsn_tick_type tick_elap  = 0U;

    /* Execute SW-Reset on SGMII interface */
    p_ctrl->p_reg_sgmii->ETNDzSGSRST = ETHER_TSN_SGSRST_SRST;
    p_ctrl->p_reg_sgmii->ETNDzSGSRST = 0UL;

    /* Wait 4us */
    R_BSP_SoftwareDelay(ETHER_TSN_WAIT_FOR_SGMII, BSP_DELAY_UNITS_MICROSECONDS);

    /* Wait until the power supply for the SGMII SerDes is turned on */
    /* (ETNB1SGSDS.PWS[1:0] == 11B) */
    (void) r_ether_tsn_get_counter_value(&tick_start);
    do
    {
        tick_elap = r_ether_tsn_get_time_out_value(tick_start);
    } while ((ETHER_TSN_SGSDS_PWS_ON != (p_ctrl->p_reg_pwrctl->ETNDzSGSDS & ETHER_TSN_SGSDS_PWS_ON)) &&
             (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

    if (ETHER_TSN_TIMEOUT_COUNT < tick_elap)
    {
        result = ETHER_TSN_NOT_OK;
    }

    /* Select the reference clock */
    /* Set the reference clock, enable the clock input and wait until it is stable, if applicable */
    /* This setting is allowed only when MOSC is 20 MHz. */
    p_ctrl->p_reg_pwrctl->ETNDzSGCLKSEL = ETHER_TSN_SGCLKSEL_SEL;

    p_ctrl->p_reg_pwrctl->ETNDzSGRCIE = ETHER_TSN_SGRCIE_RCIE;

    /* Wait until the SerDes startup is completed */
    tick_start = 0UL;
    tick_elap  = 0U;
    (void) r_ether_tsn_get_counter_value(&tick_start);
    do
    {
        tick_elap = r_ether_tsn_get_time_out_value(tick_start);
    } while ((ETHER_TSN_SGSDS_SUC != (p_ctrl->p_reg_pwrctl->ETNDzSGSDS & ETHER_TSN_SGSDS_SUC)) &&
             (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

    if (ETHER_TSN_TIMEOUT_COUNT < tick_elap)
    {
        result = ETHER_TSN_NOT_OK;
    }

    if (ETHER_TSN_ENABLE == p_extend->ether_bypassMode)
    {
        /* Manual set the operation mode */
        switch (p_extend->ether_network_speed)
        {
            case ETHER_TSN_SPEED_10MBPS:
            {
                /* Set operation mode to 10base */
                p_ctrl->p_reg_sgmii->ETNDzSGOPMC = ETHER_TSN_SGOPMC_10M;
                break;
            }

            case ETHER_TSN_SPEED_100MBPS:
            {
                /* Set operation mode to 100base */
                p_ctrl->p_reg_sgmii->ETNDzSGOPMC = ETHER_TSN_SGOPMC_100M;
                break;
            }

            default:
            {
                /* Set operation mode to 1000base */
                p_ctrl->p_reg_sgmii->ETNDzSGOPMC = ETHER_TSN_SGOPMC_1G;
                break;
            }
        }
    }                                  /* else: No action required */

    /* Enable Ready Interrupt */
    p_ctrl->p_reg_sgmii->ETNDzSGINTM = ETHER_TSN_SGINTM_ENABLE;

    return result;
}

#endif

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_initialize_buffer
 * Description               Initialize the Tx buffer ring
 * @param[in] ctrl_idx       Index of a Queue
 ***********************************************************************************************************************/
static void r_ether_tsn_initialize_buffer (uint32_t ctrl_idx)
{
    ether_tsn_tx_buffer_type_t * p_tx_buffer;
    uint32_t cnt = 0U;

    /* Initialize resource information */
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_buf_tail  = 0U;
    g_ether_tsn_rx_frame[ctrl_idx].frame_addr    = 0U;
    g_ether_tsn_rx_frame[ctrl_idx].eth_type_addr = 0U;
    g_ether_tsn_rx_frame[ctrl_idx].frame_length  = 0U;
    g_ether_tsn_rx_frame[ctrl_idx].tsns          = 0U;
    g_ether_tsn_rx_frame[ctrl_idx].tss           = 0U;

    /* Initialize Tx buffer index list */
    for (cnt = 0; cnt < g_ether_tsn_tx_buffer_total[ctrl_idx]; cnt++)
    {
        p_tx_buffer               = &gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][cnt];
        p_tx_buffer->p_buffer_hdr = ETHER_TSN_NULL_PTR; /* PRQA S 0317 # Advisory R-11.5 */

        /* Assuming ETHER_TSN_FALSE is a Boolean value */
        p_tx_buffer->tx_ongoing = ETHER_TSN_FALSE;      /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
    }
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_hw_init
 * Description               Initialize a ETND HWUnit
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of a Queue
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_hw_init (ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_tsn_extended_cfg_t const * p_extend = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    ether_tsn_type result = ETHER_TSN_OK;

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    g_ether_tsn_ctrl_stat[ctrl_idx].ts_desc_tail = 0U;
#endif

/* PRQA S 3332 ++ # Rule-20.9 */
#if ((ETHER_TSN_GET_COUNTER_VALUES_API) || (ETHER_TSN_CFG_GET_TX_STATS_API) || \
    (ETHER_TSN_CFG_GET_RX_STATS_API))

/* PRQA S 3332 -- */
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.stats_broad_cast_pkts            = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts                    = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_octets                  = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_crc_align_errors        = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_under_size_pkts         = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_over_size_pkts          = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts64O_ctets           = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts65_to_127O_ctets    = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts128_to_255O_ctets   = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts256_to_511O_ctets   = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts512_to_1023O_ctets  = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts1024_to_1518O_ctets = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_unicast_frames                = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.tx_number_of_octets              = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.tx_nu_cast_pkts                  = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.tx_uni_cast_pkts                 = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.drop_pkt_crc   = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.under_size_pkt = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.over_size_pkt  = 0UL;
    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.algnmt_err     = 0UL;
#endif

    gp_ether_tsn_heap[ctrl_idx] =
        r_ether_tsn_util_ram_init_ptr(p_extend->p_mem_pool_buffer_table, (uint32_t) p_extend->ether_tsn_ram_size);

    if (ETHER_TSN_NULL_PTR != gp_ether_tsn_heap[ctrl_idx])
    {
#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
        if (ETHER_TSN_CHANNEL_1 == ctrl_idx)
        {
            result = r_ether_tsn_sgmii_init(p_ctrl);
        }
#endif
        if (ETHER_TSN_TRUE != g_ether_tsn_tx_ram_init_state[ctrl_idx]) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        {
            result |= r_ether_tsn_tx_ram_init(p_ctrl, ctrl_idx);
        }
        else
        {
            /* else: No action required */
        }

        if (ETHER_TSN_OK == result)
        {
            /* Initialization for TSNES */
            result = r_ether_tsn_axbmi_init(p_ctrl, ctrl_idx);
            if (ETHER_TSN_OK == result)
            {
                r_ether_tsn_mhd_init(p_ctrl);
                r_ether_tsn_rmac_init(p_ctrl);
                r_ether_tsn_enable_icu(p_ctrl);
            }
        }
    }
    else
    {
        result = ETHER_TSN_NOT_OK;
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_check_provide_buffer
 * Description               This function checks whether the memory at the specified buffer index has been allocated.
 * @param[in] ctrl_idx       Control index
 * @param[in] buf_idx        Buffer index
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_check_provide_buffer (uint32_t ctrl_idx, uint32_t buf_idx)
{
    ether_tsn_type return_value = ETHER_TSN_NOT_OK;

    /* Find tx buffer handler */
    if ((ETHER_TSN_NULL_PTR != gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].p_buffer_hdr) &&
        (ETHER_TSN_FALSE == gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].tx_ongoing)) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        return_value = ETHER_TSN_OK;
    }
    else
    {
        return_value = ETHER_TSN_NOT_OK;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_tsn_pre_process_frame
 * Description                   This function sets the destination MAC address and Ethernet type for the Tx buffer.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] buf_idx            Buffer index
 * @param[in] frame_type         Frame index
 * @param[in] p_phys_addr        Physical address index
 ***********************************************************************************************************************/
static void r_ether_tsn_pre_process_frame (ether_tsn_instance_ctrl_t * p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                           uint32_t                    ctrl_idx,
                                           uint32_t                    buf_idx,
                                           uint32_t                    frame_type,
                                           const uint8_t * const       p_phys_addr)
{
    ether_tsn_type * p_buf;

    ether_tsn_buf_handler_type_t const * p_buf_handler;

    /* Protection Data _ Critical section */
    FSP_CRITICAL_SECTION_DEFINE;

    /* Get/save the current state of interrupts */
    FSP_CRITICAL_SECTION_ENTER;

    /* Get Tx buffer address */
    p_buf_handler = gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].p_buffer_hdr;

    /* Protection Data _ Critical section */
    FSP_CRITICAL_SECTION_EXIT;
    if (ETHER_ZEROCOPY_DISABLE == p_ctrl->p_ether_cfg->zerocopy)
    {
        /* store the address of buffer (Pointer to the Frame in URAM  to be transmitted) */
        p_buf = (ether_tsn_type *) p_buf_handler->buf_addr;                                /* PRQA S 0306 # Advisory R-11.4 */

        /* Copy destination address */
        ETHER_TSN_COPY_MAC_ADDRESS((uint8_t *) p_phys_addr, (uint8_t *) p_buf);            /* PRQA S 0311,3469 # DP_MSR_R-11.8_UC1,Advisory D-4.9 */
        p_buf = p_buf + ETHER_TSN_MACADDR_SIZE;                                            /* PRQA S 0488 # Advisory R-18.4 */

        ETHER_TSN_UNPACK_ADDRESS_TO_8(g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr, p_buf); /* PRQA S 3469 # Advisory D-4.9 */

        /* Casted to uint8 to extract the required 1 byte. */
        p_buf  = (ether_tsn_type *) p_buf_handler->eth_type_addr;                          /* PRQA S 0306 # Advisory R-11.4 */
        *p_buf = (ether_tsn_type) ((uint8_t) (frame_type >> ETHER_TSN_BYTE_BITS));
        p_buf++;

        /* Casted to uint8 to extract the required 1 byte. */
        *p_buf = (ether_tsn_type) ((uint8_t) frame_type);
    }
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_tsn_hw_transmit
 * Description                   Initiates a transmission on an ETHER_TSN controller.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] buf_idx            Buffer index
 * @param[in] len_byte           Set Tx request length
 * @param[in] confirmation       Set Tx confirmation flag
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_hw_transmit (ether_tsn_instance_ctrl_t * p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                               uint32_t                    ctrl_idx,
                                               uint32_t                    buf_idx,
                                               uint32_t                    len_byte,
                                               bool                        confirmation)
{
    ether_tsn_extended_cfg_t const    * p_extend = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    ether_tsn_ext_desc_type_t         * p_data_desc;
    ether_tsn_buf_handler_type_t      * p_buf_handler;
    ether_tsn_tx_ctrl_info0_word_type_t tx_ctrl_info0;                                                                 /* PRQA S 0759 # Advisory R-19.2 */
    ether_tsn_tx_ctrl_info1_word_type_t tx_ctrl_info1;                                                                 /* PRQA S 0759 # Advisory R-19.2 */

    /* Get/save the current state of interrupts */
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

    /* Get Tx buffer handler */
    gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].tx_ongoing = ETHER_TSN_TRUE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
    p_buf_handler = gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].p_buffer_hdr;

    /* Restore the interrupt state */
    FSP_CRITICAL_SECTION_EXIT;

    /* Set Tx request length */
    if (p_extend->header_data_custom == ETHER_TSN_HEADER_ADDED)
    {
        p_buf_handler->tx_length = (uint32_t) ETHER_TSN_HEADER_SIZE + len_byte;
    }
    else
    {
        p_buf_handler->tx_length = (uint32_t) len_byte;
    }

    /* Set Tx confirmation flag */
    p_buf_handler->tx_confirm = confirmation;

    /* Get next free descriptor */
    p_data_desc = g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_next_tx_desc[p_buf_handler->priority];

    tx_ctrl_info0.word = 0U;
    tx_ctrl_info1.word = 0U;

    /* Justify typecast to smaller datatype */
    tx_ctrl_info0.st_tx_ctrl.tsun = (uint8_t) (p_buf_handler->buf_idx);

    if (ETHER_TSN_TRUE == p_buf_handler->enable_ts) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        tx_ctrl_info0.st_tx_ctrl.txc = (uint32_t) ETHER_TSN_TX_DESC_TCX_ENABLE;
        tx_ctrl_info0.st_tx_ctrl.tn  = (uint32_t) ETHER_TSN_TX_DESC_TN_DOMAIN;
    }

    tx_ctrl_info1.st_tx_ctrl.tfl = (p_buf_handler->tx_length & ETHER_TSN_CONVERT_TO_SMALL_BIT);

    /* Build the descriptor */
    p_data_desc->st_header.die   = (uint32_t) ETHER_TSN_TX_DESC_DIE_ENABLE;
    p_data_desc->st_header.info0 = (tx_ctrl_info0.word & ETHER_TSN_CONVERT_TO_SMALL_BIT);
    p_data_desc->st_header.ds    = (p_buf_handler->tx_length & ETHER_TSN_CONVERT_TO_SMALL_BIT);

    if (p_extend->header_data_custom == ETHER_TSN_HEADER_ADDED)
    {
        p_data_desc->dptr = p_buf_handler->buf_addr;
    }
    else
    {
        /* HEADER_CUSTOM: the caller wrote the full frame (Ethernet header + payload)
         * starting at buf_addr, in both zero-copy and non-zero-copy modes. */
        p_data_desc->dptr = p_buf_handler->buf_addr;
    }

    p_data_desc->info1[0] = tx_ctrl_info1.word;

    /* Use for ETND */
    p_data_desc->info1[1] = 0U;

    /* Do not move this process due to descriptor exclusive control */
    p_data_desc->st_header.dt = (uint32_t) ETHER_TSN_DESC_FSINGLE;

    /* Store priority buffer value  */
    g_ether_tsn_current_priority[ctrl_idx] = p_buf_handler->priority;

    /* Store current descriptor */
    g_ether_tsn_current_tx_descr[ctrl_idx][g_ether_tsn_current_priority[ctrl_idx]] = p_data_desc->dptr;

    /* Set next descriptor */
    p_data_desc++;

    while ((ETHER_TSN_DESC_LEMPTY == p_data_desc->st_header.dt) || (ETHER_TSN_DESC_LINK == p_data_desc->st_header.dt))
    {
        p_data_desc->st_header.dt = (uint32_t) ETHER_TSN_DESC_LINK;
        p_data_desc               = (ether_tsn_ext_desc_type_t *) p_data_desc->dptr; /* PRQA S 0306 # Advisory R-11.4 */
    }

    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_next_tx_desc[p_buf_handler->priority] = p_data_desc;

    FSP_CRITICAL_SECTION_ENTER;

    /* Increase the number of buffer of current Tx queue */
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_buf_tx_cnt[p_buf_handler->priority]++;

    /* Restore the interrupt state */
    FSP_CRITICAL_SECTION_EXIT;

    /* Transmit start request 0-7 */
    p_ctrl->p_reg_axibmi->TRCR0 |= (uint32_t) (1UL << ((uint32_t) p_buf_handler->priority)); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return ETHER_TSN_OK;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_tsn_transmit
 * Description                   This API triggers transmission of a previously filled transmit buffer
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] buf_idx            Index of the buffer resource
 * @param[in] frame_type         Set Tx request length
 * @param[in] tx_confirmation    Activates transmission confirmation
 * @param[in] len_byte           Data length in byte
 * @param[in] p_phys_addr        Physical target address (MAC address)
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_transmit (ether_tsn_instance_ctrl_t * p_ctrl,
                                            uint32_t                    ctrl_idx,
                                            ether_tsn_buf_idx_type      buf_idx,
                                            ether_tsn_frame_type        frame_type,
                                            bool                        tx_confirmation,
                                            uint16_t                    len_byte,
                                            const uint8_t             * p_phys_addr)
{
    ether_tsn_extended_cfg_t const * p_extend     = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    volatile ether_tsn_type          return_value = ETHER_TSN_OK;

    return_value = r_ether_tsn_check_provide_buffer(ctrl_idx, (uint32_t) buf_idx);

    if (ETHER_TSN_OK != return_value)
    {
        return return_value;
    }

    if (p_extend->header_data_custom == ETHER_TSN_HEADER_ADDED)
    {
        /* Prepare a frame for transmission.*/
        r_ether_tsn_pre_process_frame(p_ctrl, ctrl_idx, (uint32_t) buf_idx, (uint32_t) frame_type, p_phys_addr);
    }

    /* Send a transmit request to the low level driver */
    return_value = r_ether_tsn_hw_transmit(p_ctrl, ctrl_idx, (uint32_t) buf_idx, (uint32_t) len_byte, tx_confirmation);

    return return_value;
}

#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name          r_ether_tsn_clear_all_address_filters
 * Description            Remove all filters from the filter list and turn off promiscuous mode
 * @param[in] ctrl_idx    Index of the controller
 ***********************************************************************************************************************/
static void r_ether_tsn_clear_all_address_filters (uint32_t ctrl_idx)
{
    g_ether_tsn_ctrl_stat[ctrl_idx].active_filter_bits = 0U;
    g_ether_tsn_ctrl_stat[ctrl_idx].promiscuous        = ETHER_TSN_FALSE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
}

#endif

/*******************************************************************************************************************//**
 * Function Name                 r_ether_tsn_get_tx_buffer
 * Description                   Get a buffer from the Tx buffer ring
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] priority           Frame priority for transmit buffer FIFO selection
 * @param[in] p_buf_idx          buffer index data
 * @param[in] p_buf              Pointer to the granted buffer
 * @param[in] p_len_byte         Pointer to data length in byte
 ***********************************************************************************************************************/
static ether_tsn_buf_req_return_type_t r_ether_tsn_get_tx_buffer (ether_tsn_instance_ctrl_t    * p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                                                  uint32_t                       ctrl_idx,
                                                                  uint8_t                        priority,
                                                                  ether_tsn_buf_idx_type * const p_buf_idx,
                                                                  uint8_t ** const               p_buf,
                                                                  uint16_t * const               p_len_byte)
{
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl;
    ether_tsn_extended_cfg_t const  * p_extend        =
        ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    ether_tsn_buf_req_return_type_t return_value = ETHER_TSN_BUFREQ_OK;
    ether_tsn_buf_handler_type_t  * p_tx_buffer_node;
    uint32_t          ring_idx      = 0U;
    volatile uint32_t tx_buffer_max = 0U;
    uint32_t          queue_depth   = 0U;
    uint16_t          req_size      = 0U;
    uint32_t          header_custom = ETHER_TSN_HEADER_SIZE;

    if (priority > ETHER_TSN_MAX_TX_PRIORITY)
    {
        return ETHER_TSN_BUFREQ_E_NOT_OK;
    }

    tx_buffer_max = p_extend->queue_config.p_tx_queue_config[priority].max_frame_size;
    queue_depth   = p_extend->queue_config.p_tx_queue_config[priority].queue_bufs;

    if (p_extend->header_data_custom == ETHER_TSN_HEADER_ADDED)
    {
        header_custom = 0U;
    }

    req_size = *p_len_byte;

    if ((0UL == tx_buffer_max) || (req_size < (ETHER_TSN_MIN_PAYLOAD_SIZE + header_custom)))
    {
        return_value = ETHER_TSN_BUFREQ_E_NOT_OK;
    }
    else if (req_size > ((tx_buffer_max - ETHER_TSN_HEADER_SIZE + header_custom) - ETHER_TSN_FCS_LENGTH))
    {
        /* If the requested size is larger than the buffer, return error */
        *p_len_byte  = (uint16_t) tx_buffer_max;
        return_value = ETHER_TSN_BUFREQ_E_OVFL;
    }
    else
    {
        /* Protect update global for buffer information from interrupt */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        /* Check the number of the Tx buffers allocated */
        if (g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_tx_alloc_cnt[priority] < queue_depth)
        {
            /* Get Tx buffer index */
            ring_idx = g_ether_tsn_ctrl_stat[ctrl_idx].tx_buf_tail;

            /* This process never goes into an infinite loop */
            while (ETHER_TSN_NULL_PTR != gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][ring_idx].p_buffer_hdr)
            {
                /* Next buffer index */
                ring_idx = (ring_idx + 1UL) % g_ether_tsn_tx_buffer_total[ctrl_idx];
            }

            /* Get the Tx buffer management node and Tx buffer */
            /* PRQA S 0316 ++ # Advisory R-11.5 */
            p_tx_buffer_node =
                (ether_tsn_buf_handler_type_t *) r_ether_tsn_util_ram_alloc_ptr(&gp_ether_tsn_heap[ctrl_idx],
                                                                                (uint32_t) (req_size +
                                                                                            ETHER_TSN_HEADER_SIZE + /* PRQA S 1891 # Advisory R-10.7 */
                                                                                            sizeof(
                                                                                                ether_tsn_buf_handler_type_t)));

            /* PRQA S 0316 -- */

            if (ETHER_TSN_NULL_PTR != p_tx_buffer_node)
            {
                /* Set next buffer index */
                g_ether_tsn_ctrl_stat[ctrl_idx].tx_buf_tail = (ring_idx + 1UL) % g_ether_tsn_tx_buffer_total[ctrl_idx];

                gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][ring_idx].p_buffer_hdr = p_tx_buffer_node;
                gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][ring_idx].tx_ongoing   = ETHER_TSN_FALSE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */

                p_tx_buffer_node->buf_idx = ring_idx;
                if (ETHER_ZEROCOPY_DISABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
                {
                    p_tx_buffer_node->buf_addr = (uint32_t) p_tx_buffer_node + /* PRQA S 0329 # Advisory R-11.4 */
                                                 (uint32_t) sizeof(ether_tsn_buf_handler_type_t);
                }
                else
                {
                    p_tx_buffer_node->buf_addr = (uint32_t) g_ether_tsn_tx_buffer_addr[ctrl_idx];
                }

                p_tx_buffer_node->tx_length  = 0U;
                p_tx_buffer_node->priority   = priority;
                p_tx_buffer_node->enable_ts  = ETHER_TSN_FALSE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
                p_tx_buffer_node->tx_confirm = ETHER_TSN_FALSE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
                p_tx_buffer_node->time_qual               = ETHER_TSN_INVALID;
                p_tx_buffer_node->time_stamp.nano_seconds = (uint32_t) 0UL;
                p_tx_buffer_node->time_stamp.seconds      = (uint32_t) 0UL;
                p_tx_buffer_node->time_stamp.seconds_hi   = (uint16_t) 0U;
#endif

                *p_buf_idx = (ether_tsn_buf_idx_type) p_tx_buffer_node->buf_idx;

                /* In HEADER_CUSTOM mode the caller writes the full frame (Ethernet header +
                 * payload) starting at the buffer's first byte, so p_buf must not skip HEADER_SIZE
                 * here. Only HEADER_ADDED mode reserves that space for the driver's own header. */
                if (p_extend->header_data_custom == ETHER_TSN_HEADER_ADDED)
                {
                    *p_buf = (uint8_t *) (p_tx_buffer_node->buf_addr + (uint32_t) ETHER_TSN_HEADER_SIZE); /* PRQA S 0306 # Advisory R-11.4 */
                }
                else
                {
                    *p_buf = (uint8_t *) (p_tx_buffer_node->buf_addr);                                    /* PRQA S 0306 # Advisory R-11.4 */
                }

                p_tx_buffer_node->eth_type_addr = p_tx_buffer_node->buf_addr + ETHER_TSN_SRC_DST_ADDRESS_SIZE;

                return_value = ETHER_TSN_BUFREQ_OK;

                /* Increment Tx buffer counter */
                g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_tx_alloc_cnt[priority]++;
            }
            else
            {
                return_value = ETHER_TSN_BUFREQ_E_BUSY;
            }
        }
        else
        {
            return_value = ETHER_TSN_BUFREQ_E_BUSY;
        }

        /* Exit critical section */
        FSP_CRITICAL_SECTION_EXIT;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_provide_tx_buffer
 * Description               This API Provides access to a transmit buffer of the specified controller
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] priority       Frame priority for transmit buffer FIFO selection
 * @param[in] p_buf_idx      Index to the granted buffer resource
 * @param[in] p_buf          Pointer to the granted buffer
 * @param[in] p_len_byte     Pointer to data length in byte
 ***********************************************************************************************************************/
static ether_tsn_buf_req_return_type_t r_ether_tsn_provide_tx_buffer (ether_tsn_instance_ctrl_t * p_ctrl,
                                                                      uint32_t                    ctrl_idx,
                                                                      uint8_t                     priority,
                                                                      ether_tsn_buf_idx_type    * p_buf_idx,
                                                                      uint8_t                  ** p_buf,
                                                                      uint32_t                  * p_len_byte)
{
    ether_tsn_buf_req_return_type_t return_value = ETHER_TSN_BUFREQ_OK;

    return_value =
        r_ether_tsn_get_tx_buffer(p_ctrl, ctrl_idx, priority, (uint32_t *) p_buf_idx, p_buf, (uint16_t *) p_len_byte); /* PRQA S 0310 # DP_MSR_R11.3_UC1 */

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_hw_enable_controller
 * Description           Changes the state of an ETHER_TSN controller as ACTIVE
 * @param[in] p_ctrl     Pointer to the control block for the channel
 * @param[in] ctrl_idx   PIndex of the controller
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_hw_enable_controller (ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_tsn_extended_cfg_t const * p_extend = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    ether_tsn_type      result                = ETHER_TSN_OK;
    ether_tsn_tick_type tick_start            = 0U;
    ether_tsn_tick_type tick_elap             = 0U;

    /* Set MAC address */
    /* When MAC address is 01-23-45-67-89-AB, set 0x0123, high-bit is set in MRMAC0 */
    p_ctrl->p_reg_rmaca->MRMAC0 =      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                  (uint16_t) (g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr.h32 >>
                                              ETHER_TSN_MRMAC_CONFIG_MAC);

    /* When MAC address is 01-23-45-67-89-AB, set 0x456789AB */
    p_ctrl->p_reg_rmaca->MRMAC1 = (g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr.h32 << ETHER_TSN_MRMAC_CONFIG_MAC) | /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                  g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr.l16;

    if (ETHER_TSN_TRUE == g_ether_tsn_ctrl_stat[ctrl_idx].promiscuous)                                              /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        p_ctrl->p_reg_rmaca->MRAFC |= ETHER_TSN_PROMISCUOUS_MODE;                                                   /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
    else
    {
        /* Normal receive mode
         * Same value with initial value of MRAFC ref in RMAC_Init().
         * */
        p_ctrl->p_reg_rmaca->MRAFC &= ~ETHER_TSN_PROMISCUOUS_MODE; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Link Verification */
    /* Set Preemption auto response and Link Verification Timer */
    p_ctrl->p_reg_rmaca->MLVC = (uint32_t) ETHER_TSN_MLVC_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Request Link Verification (HW will clear this bit after completing link verification) */
    p_ctrl->p_reg_rmaca->MLVC |=                                  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                 ((uint32_t) p_extend->ether_preemption_link_verification << R_RMACA0_MLVC_PLV_Pos);

    /* Wait until completing Link Verification */
    (void) r_ether_tsn_get_counter_value(&tick_start);
    do
    {
        tick_elap = r_ether_tsn_get_time_out_value(tick_start);
    } while ((0UL != (p_ctrl->p_reg_rmaca->MLVC & ETHER_TSN_MLVC_PLV_MASK)) && /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
             (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

    if (ETHER_TSN_TIMEOUT_COUNT < tick_elap)
    {
        result = ETHER_TSN_NOT_OK;
    }

    if (ETHER_TSN_OK == result)
    {
        /* Set EAMC mode to OPERATION */
        result  = r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_DISABLE_MODE);
        result |= r_ether_tsn_mhd_mode_setting(p_ctrl, ETHER_TSN_MHD_OPERATION_MODE);
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_release_tx_buffer
 * Description               Release a Tx buffer and store it to the tail of the Tx buffer ring
 * @param[in] ctrl_idx      Index of the buffer resource
 * @param[in] buf_idx       PIndex of the controller
 ***********************************************************************************************************************/
static void r_ether_tsn_release_tx_buffer (uint32_t ctrl_idx, uint32_t buf_idx)
{
    uint32_t priority = gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].p_buffer_hdr->priority;

    /* Release Tx buffer handler */
    r_ether_tsn_util_ram_free(&gp_ether_tsn_heap[ctrl_idx],
                              (void *) gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].p_buffer_hdr);
    gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].p_buffer_hdr = ETHER_TSN_NULL_PTR; /* PRQA S 0317 # Advisory R-11.5 */
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_tx_alloc_cnt[priority]--;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_set_controller_mode
 * Description               This API performs enabling / disabling  of the indexed controller.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] ctrl_mode      Controller state type structure
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_set_controller_mode (ether_tsn_instance_ctrl_t * p_ctrl,
                                                       uint32_t                    ctrl_idx,
                                                       ether_tsn_mode_type_t       ctrl_mode)
{
    ether_tsn_type result = ETHER_TSN_OK;

    if (ETHER_TSN_MODE_ACTIVE == ctrl_mode)
    {
        /* Activate HW */
        result = r_ether_tsn_hw_enable_controller(p_ctrl, ctrl_idx);

        if (ETHER_TSN_OK == result)
        {
            g_ether_tsn_ctrl_stat[ctrl_idx].mode = ETHER_TSN_MODE_ACTIVE;
        }
    }
    else
    {
        /* Do nothing */
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_send_data
 * Description               This API performs Write the transmit data to the transmit buffer.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] buf            Pointer to buffer data
 * @param[in] len_byte       Data length in byte
 ***********************************************************************************************************************/
static fsp_err_t r_ether_tsn_send_data (ether_tsn_instance_ctrl_t * p_ctrl, uint8_t * buf, uint32_t len_byte) /* PRQA S 3673 # Advisory R-8.13 */
{
    ether_tsn_instance_ctrl_t const * p_instance_ctrl = (ether_tsn_instance_ctrl_t const *) p_ctrl;
    ether_tsn_extended_cfg_t const  * p_extend        =
        ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend);                                   /* PRQA S 0316 # Advisory R-11.5 */
    fsp_err_t                       return_value     = FSP_SUCCESS;
    volatile ether_tsn_type         result           = ETHER_TSN_OK;
    ether_tsn_buf_req_return_type_t buf_return_value = ETHER_TSN_BUFREQ_OK;
    ether_tsn_type                * p_buf            = ETHER_TSN_NULL_PTR;                                    /* PRQA S 0317 # Advisory R-11.5 */
    ether_tsn_buf_idx_type          buf_idx          = 0;
    uint32_t lus_length = 0;
    uint32_t q_idx      = 0U;

    /* Initialize controller index */
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    lus_length       = len_byte;
    buf_return_value = r_ether_tsn_provide_tx_buffer(p_ctrl, ctrl_idx, 0, &buf_idx, (uint8_t **) &p_buf, &lus_length);

    if ((buf_return_value == ETHER_TSN_BUFREQ_OK) && (p_buf != ETHER_TSN_NULL_PTR))
    {
        if (ETHER_ZEROCOPY_DISABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
        {
            if (ETHER_TSN_NULL_PTR != buf)
            {
                (void) memcpy(p_buf, buf, lus_length);
            }
        }

        result =
            r_ether_tsn_transmit(p_ctrl,
                                 ctrl_idx,
                                 buf_idx,
                                 (uint16_t) p_extend->ether_type,
                                 ETHER_TSN_TRUE, /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
                                 (uint16_t) lus_length,
                                 p_extend->p_mac_address_target);

        if (ETHER_TSN_NOT_OK == result)
        {
            /* If a callback is provided, then call it with callback argument. */
            r_ether_tsn_call_callback(p_ctrl, ETHER_EVENT_TX_ABORTED);
            return_value = FSP_ERR_IN_USE;
        }
        else
        {
            if (p_extend->ether_tsn_cfg_type_tx_isr == ETHER_TSN_CTRL_DISABLE_TX_INTERRUPT)
            {
                /* In polling mode, confirm no packet in transmit counter after transmission. */
                do
                {
                    /* Polling for Tx confirmation */
                    r_ether_tsn_tx_confirmation(p_ctrl, ctrl_idx);
                } while (g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_buf_tx_cnt[q_idx] != 0U);

                r_ether_tsn_call_callback(p_ctrl, ETHER_EVENT_TX_COMPLETE);
            }
        }
    }
    else if (buf_return_value == ETHER_TSN_BUFREQ_E_BUSY)
    {
        /* Buffer still in use */
        if (p_extend->ether_tsn_cfg_type_tx_isr == ETHER_TSN_CTRL_DISABLE_TX_INTERRUPT)
        {
            /* Polling for Tx confirmation */
            r_ether_tsn_tx_confirmation(p_ctrl, ctrl_idx);
        }

        r_ether_tsn_call_callback(p_ctrl, ETHER_EVENT_TX_ABORTED);
        return_value = FSP_ERR_IN_USE;
    }
    else
    {
        /* Report error when size of length invalid */
        /* If a callback is provided, then call it with callback argument. */
        r_ether_tsn_call_callback(p_ctrl, ETHER_EVENT_TX_ABORTED);
        return_value = FSP_ERR_INVALID_SIZE;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_set_phys_addr
 * Description               Sets the physical source address used by the indexed controller.
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] mac_addr       Convert mac address data
 ***********************************************************************************************************************/
static void r_ether_tsn_set_phys_addr (uint32_t ctrl_idx, const uint8_t mac_addr[])
{
    ETHER_TSN_PACK_ADDRESS_FROM_8(mac_addr, g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr); /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_rx_desc_learning
 * Description               Learning Tx descriptor chain table
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] entry_number   Number check Tx descriptor address table learning Setting
 * @param[in] chain_addr     Adress of allocated RAM
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_rx_desc_learning (ether_tsn_instance_ctrl_t * p_ctrl,   /* PRQA S 3673 # Advisory R-8.13 */
                                                    uint32_t                    entry_number,
                                                    uint32_t                    chain_addr)
{
    ether_tsn_tick_type tick_start = 0U;
    ether_tsn_tick_type tick_elap  = 0U;
    ether_tsn_type      result     = ETHER_TSN_NOT_OK;
    tick_start = 0U;

    (void) r_ether_tsn_get_counter_value(&tick_start);
    do
    {
        /* Base address of RX descriptor chain */
        p_ctrl->p_reg_axibmi->RATLS0 = ETHER_TSN_RATLS0_CONFIG | (entry_number << 24U); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_axibmi->RATLS1 = chain_addr;                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_ctrl->p_reg_axibmi->RATLR  = ETHER_TSN_RATLR_LEARNING;                        /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        do
        {
            tick_elap = r_ether_tsn_get_time_out_value(tick_start);
        } while ((0UL != (p_ctrl->p_reg_axibmi->RATLR & ETHER_TSN_RATLR_LEARNING)) && /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                 (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

        if (0U == (p_ctrl->p_reg_axibmi->RATLR & ETHER_TSN_RATLR_LEARNING))           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            if (0U == (p_ctrl->p_reg_axibmi->RATLR & ETHER_TSN_RATLR_FAILED))         /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                result = ETHER_TSN_OK;
                break;
            }
        }
    } while (ETHER_TSN_TIMEOUT_COUNT <= tick_elap);

    return result;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_rx_desc_relearn_if_full
 * Description               Re-learning rx descriptor chain table when rx descriptor full, using RATSR1
 *                           (HW-tracked current address) instead of a software-guessed descriptor address.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] queue_idx      Index of a Queue
 ***********************************************************************************************************************/
static void r_ether_tsn_rx_desc_relearn_if_full (ether_tsn_instance_ctrl_t * p_ctrl, uint32_t queue_idx)
{
    ether_tsn_tick_type tick_start      = 0U;
    ether_tsn_tick_type tick_elap       = 0U;
    uint32_t            rx_queue_offset = 0U;
    uint32_t            int_stat_hw;
    uint32_t            rats;
    uint32_t            relearn_addr;

    int_stat_hw = ETHER_TSN_GET_RX_FULL_INT_STAT(queue_idx, p_ctrl->p_reg_axibmi->RFEIS0, rx_queue_offset);

    if ((ETHER_TSN_TRUE == g_ether_tsn_rx_full) || (0UL != int_stat_hw))
    {
        /* Searching rx descriptor chain table */
        p_ctrl->p_reg_axibmi->RATS   = queue_idx + rx_queue_offset;
        p_ctrl->p_reg_axibmi->RATSR2 = ETHER_TSN_RATSR2_SEARCHING;

        /* Rx Descriptor Address Table searching waiting */
        (void) r_ether_tsn_get_counter_value(&tick_start);
        do
        {
            tick_elap = r_ether_tsn_get_time_out_value(tick_start);
        } while ((0UL != p_ctrl->p_reg_axibmi->RATSR2) && (tick_elap <= ETHER_TSN_TIMEOUT_COUNT));

        if (ETHER_TSN_TIMEOUT_COUNT >= tick_elap)
        {
            if (0UL == (p_ctrl->p_reg_axibmi->RATSR0 & ETHER_TSN_RATSR0_CHAIN_ENTRY_VALID))
            {
                /* Snapshot RATS and RATSR1 together, before any other register access can let a
                 * concurrent search for another queue overwrite the shared RATS register. */
                rats         = (uint32_t) p_ctrl->p_reg_axibmi->RATS;
                relearn_addr = p_ctrl->p_reg_axibmi->RATSR1;

                /* Clear rx descriptor full interrupt status */
                g_ether_tsn_rx_full          = ETHER_TSN_FALSE;
                p_ctrl->p_reg_axibmi->RFEIS0 = int_stat_hw;

                /* Re-learning rx descriptor chain table */
                (void) r_ether_tsn_rx_desc_learning(p_ctrl, rats, relearn_addr);
            }
        }
    }
}

#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_rx_frame_valid
 * Description               Compare Mac Address Frame just received with Broadcast and Controller Mac Address.
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] frame_addr     Frame address
 ***********************************************************************************************************************/
static bool r_ether_tsn_rx_frame_valid (uint32_t ctrl_idx, uint32_t frame_addr)
{
    ether_tsn_mac_address_type_t dst_address;
    bool     pass        = ETHER_TSN_FALSE;                                                      /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
    uint32_t filter_idx  = 0U;
    uint32_t remain_bits = 0U;

    ETHER_TSN_PACK_ADDRESS_FROM_8((uint8_t *) frame_addr, dst_address);                          /* PRQA S 0306,3469 # Advisory R-11.4,Advisory D-4.9 */

    /* Check whether promiscuous mode, broadcast, own unicast, multicast address is not registed */
    if ((ETHER_TSN_TRUE == g_ether_tsn_ctrl_stat[ctrl_idx].promiscuous) ||                       /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
        (0U == ETHER_TSN_COMPARE_MAC(dst_address, g_ether_tsn_ts_desc_total)) ||                 /* PRQA S 3469 # Advisory D-4.9 */
        (0U == ETHER_TSN_COMPARE_MAC(dst_address, g_ether_tsn_ctrl_stat[ctrl_idx].st_mac_addr))) /* PRQA S 3469 # Advisory D-4.9 */
    {
        pass = ETHER_TSN_TRUE;                                                                   /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
    }
    else
    {
        /* To skip empty filters efficiently, copy active bits to the local var */
        remain_bits = g_ether_tsn_ctrl_stat[ctrl_idx].active_filter_bits;

        /* Loop until any filter hits or pass all active filters */
        while (0UL != remain_bits)
        {
            if ((0UL != (remain_bits & (1UL << filter_idx))) &&
                (0UL == ETHER_TSN_COMPARE_MAC(dst_address, g_ether_tsn_address_filters[ctrl_idx][filter_idx]))) /* PRQA S 3469 # Advisory D-4.9 */
            {
                pass = ETHER_TSN_TRUE;                                                                          /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
                break;
            }

            /* Clear the compared filter bit */
            remain_bits &= ~(1UL << filter_idx);
            filter_idx++;
        }
    }

    return pass;
}

#endif                                                                           /* (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER) */

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_if_rx_indication
 * Description               Since the maximum value of Controller Index is 1, casting to uint8 does no problem
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] frame_type     Set Rx request length
 * @param[in] p_phys_addr    Pointer to the Frame Address just received
 * @param[in] p_data         Pointer to data received
 * @param[in] len_byte       lenght data to receive
 ***********************************************************************************************************************/
static void r_ether_tsn_if_rx_indication (ether_tsn_instance_ctrl_t    * p_ctrl, /* PRQA S 3673 # Advisory R-8.13 */
                                          uint32_t                       ctrl_idx,
                                          ether_tsn_frame_type           frame_type,
                                          const uint8_t                * p_phys_addr,
                                          const ether_tsn_frame_type_t * p_data,
                                          uint16_t                       len_byte)
{
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    if (gp_ether_tsn_rx_time_stamp[ctrl_idx] != NULL)
    {
        r_ether_tsn_hw_get_ingress_time_stamp(ctrl_idx,           /* PRQA S 3200 # Required R-17.7 */
                                              (uint8_t *) p_data, /* PRQA S 0311 # DP_MSR_R-11.8_UC1 */
                                              &g_ether_tsn_rx_time_qual,
                                              gp_ether_tsn_rx_time_stamp[ctrl_idx]);
    }
#endif

    if (ETHER_ZEROCOPY_DISABLE == p_ctrl->p_ether_cfg->zerocopy)
    {
        /* Memcpy Received message */
        if ((ETHER_TSN_NULL_PTR != gp_ether_tsn_user_rx_buf[ctrl_idx])
#if (ETHER_TSN_CFG_READ_BUFFER_SIZE_CHECK) /* PRQA S 3332 # Rule-20.9 */
            && (len_byte <= g_ether_tsn_user_rx_buf_len[ctrl_idx])
#endif
            )
        {
            (void) memcpy(gp_ether_tsn_user_rx_buf[ctrl_idx], (uint8_t *) p_data, len_byte);    /* PRQA S 0311 # DP_MSR_R-11.8_UC1 */
        }

        (void) memcpy(&g_ether_tsn_rx_src_addr[ctrl_idx], p_phys_addr, ETHER_TSN_MACADDR_SIZE); /* PRQA S 1495 # DP_MSR_R-21.15_UC1 */
    }

    g_ether_tsn_rx_frame_type[ctrl_idx] = frame_type;

    /* Store entire frame size (with header) to g_ether_tsn_rx_len_byte */
    g_ether_tsn_rx_len_byte[ctrl_idx] = len_byte;
    g_ether_tsn_rx_frame_cnt[ctrl_idx]++;
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_if_rx_call
 * Description           Wrapper for the Callback to the Eth Interface
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx   Index of the controller
 * @param[in] p_frame    Address of the Received Frame in URAM
 ***********************************************************************************************************************/
static void r_ether_tsn_if_rx_call (ether_tsn_instance_ctrl_t             * p_ctrl,
                                    uint32_t                                ctrl_idx,
                                    const ether_tsn_rx_frame_type_t * const p_frame)
{
    ether_tsn_type const         * p_frame_type;
    ether_tsn_frame_type           frame_type = 0U;
    ether_tsn_mac_address_type_t   st_mac_addr;
    ether_tsn_frame_type_t const * p_ether_frame;

    p_ether_frame = (ether_tsn_frame_type_t const *) (p_frame->frame_addr);   /* PRQA S 0306 # Advisory R-11.4 */

    ETHER_TSN_PACK_ADDRESS_FROM_8(p_ether_frame->dst_addr, st_mac_addr);      /* PRQA S 3469 # Advisory D-4.9 */

    if (0UL == ETHER_TSN_COMPARE_MAC(st_mac_addr, g_ether_tsn_ts_desc_total)) /* PRQA S 3469 # Advisory D-4.9 */
    {
        /*Do nothing*/
    }
    else
    {
#if (ETHER_TSN_CFG_GET_RX_STATS_API)   /* PRQA S 3332 # Rule-20.9 */
        if (0UL == ETHER_TSN_CHECK_MULTICAST(st_mac_addr))
        {
            g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_unicast_frames++;
        }
#endif
    }

    p_frame_type = (ether_tsn_type const *) p_frame->eth_type_addr; /* PRQA S 0306 # Advisory R-11.4 */
    frame_type   = (ether_tsn_frame_type) ((uint32_t) p_frame_type[0] << ETHER_TSN_BYTE_BITS);
    frame_type  |= (ether_tsn_frame_type) p_frame_type[1];

    /* Since the maximum value of Controller Index is 1, casting to uint8 does no problem. */
    r_ether_tsn_if_rx_indication(p_ctrl, ctrl_idx, frame_type, p_ether_frame->src_addr, p_ether_frame,
                                 (uint16_t) (p_frame->frame_length));
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_rx_queue_process
 * Description               Process Receive Queue.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] queue_idx      Rx queue index
 ***********************************************************************************************************************/
static ether_tsn_etag_rx_ext_status_type_t r_ether_tsn_rx_queue_process (ether_tsn_instance_ctrl_t * p_ctrl,
                                                                         uint32_t                    ctrl_idx,
                                                                         uint32_t                    queue_idx)
{
    ether_tsn_etag_rx_ext_status_type_t result = ETHER_TSN_EXT_NOT_RECEIVED;
    ether_tsn_ext_desc_with_ts_type_t * p_data_desc;
    ether_tsn_ext_desc_with_ts_type_t * p_temp_desc;
    bool rx_frame_valid = ETHER_TSN_TRUE; /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */

#if (ETHER_TSN_CFG_GET_RX_STATS_API)   /* PRQA S 3332 # Rule-20.9 */
    uint32_t length_with_fcs;
#endif

    /* Get descriptor for read */
    p_data_desc = g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_rx_desc[queue_idx];
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_update[queue_idx]  = p_data_desc;
    g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_current_rx_desc_release[queue_idx] = p_data_desc;
    p_temp_desc = p_data_desc;

    if (ETHER_TSN_DESC_FSINGLE == p_data_desc->st_header.dt)
    {
        /* Set descriptor to be read next */
        p_data_desc++;
        while ((ETHER_TSN_DESC_LINK == p_data_desc->st_header.dt) ||
               (ETHER_TSN_DESC_LEMPTY == p_data_desc->st_header.dt))
        {
            p_data_desc->st_header.dt = (uint32_t) ETHER_TSN_DESC_LINK;
            p_data_desc               = (ether_tsn_ext_desc_with_ts_type_t *) p_data_desc->dptr; /* PRQA S 0306 # Advisory R-11.4 */
        }

        g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_rx_desc[queue_idx] = p_data_desc;

#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)

        /* Check whether received frame is valid or not */
        rx_frame_valid = r_ether_tsn_rx_frame_valid(ctrl_idx, p_temp_desc->dptr);
#endif

        if ((ETHER_TSN_TRUE == rx_frame_valid) && /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
            (ETHER_TSN_INFO0_SEI_MASK != (ETHER_TSN_INFO0_SEI_MASK & p_temp_desc->st_header.info0)))
        {
            /* Set Rx descriptor info */
            g_ether_tsn_rx_frame[ctrl_idx].frame_addr    = p_temp_desc->dptr;
            g_ether_tsn_rx_frame[ctrl_idx].eth_type_addr = p_temp_desc->dptr + ETHER_TSN_SRC_DST_ADDRESS_SIZE;
            g_ether_tsn_rx_frame[ctrl_idx].frame_length  = p_temp_desc->st_header.ds;
            g_ether_tsn_rx_frame[ctrl_idx].tsns          = p_temp_desc->tsns;
            g_ether_tsn_rx_frame[ctrl_idx].tss           = p_temp_desc->tss;

            if (ETHER_TSN_DESC_FSINGLE == p_data_desc->st_header.dt)
            {
                /* More frames are available */
                result = ETHER_TSN_EXT_RECEIVED_MORE_DATA_AVAILABLE;
            }
            else
            {
                /* Frame valid - Update status */
                result = ETHER_TSN_EXT_RECEIVED;
            }

#if (ETHER_TSN_CFG_GET_RX_STATS_API)   /* PRQA S 3332 # Rule-20.9 */
            g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts++;
            length_with_fcs = g_ether_tsn_rx_frame[ctrl_idx].frame_length + ETHER_TSN_FCS_LENGTH;
            g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_octets += length_with_fcs;
            if (length_with_fcs <= ETHER_TSN_STATUS_PKT_LENGHT_640)
            {
                /* Since the receive data size that does not include padding data is set in the receive descriptor,
                 * frames of 64 or less are collected by the statistical counter. */
                g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts64O_ctets++;
            }
            else if (length_with_fcs <= ETHER_TSN_STATUS_PKT_LENGHT_1270)
            {
                g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts65_to_127O_ctets++;
            }
            else if (length_with_fcs <= ETHER_TSN_STATUS_PKT_LENGHT_2550)
            {
                g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts128_to_255O_ctets++;
            }
            else if (length_with_fcs <= ETHER_TSN_STATUS_PKT_LENGHT_5110)
            {
                g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts256_to_511O_ctets++;
            }
            else if (length_with_fcs <= ETHER_TSN_STATUS_PKT_LENGHT_10230)
            {
                g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts512_to_1023O_ctets++;
            }
            else
            {
                /* When VLAN tag is supported, the maximum data size is 1522. */
                /* The maximum frame size that HW can receive is set to 1522. */
                g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts1024_to_1518O_ctets++;
            }
#endif

            /* Call EthIf if the Frame Received is valid */
            r_ether_tsn_if_rx_call(p_ctrl, ctrl_idx, &g_ether_tsn_rx_frame[ctrl_idx]);
        }
        else
        {
            /* Frame Invalid - E.g. Multicast to be discarded */
            /* No Call of EthIf */
            if (ETHER_TSN_DESC_FSINGLE == p_data_desc->st_header.dt)
            {
                /* Not received, but more frames are available */
                result = ETHER_TSN_EXT_NOT_RECEIVED_MORE_DATA_AVAILABLE;
            }

            if (ETHER_ZEROCOPY_ENABLE == p_ctrl->p_ether_cfg->zerocopy)
            {
                /* Reset Rx descriptor */
                p_temp_desc->st_header.die   = (uint32_t) ETHER_TSN_RX_DESC_DIE_ENABLE;
                p_temp_desc->st_header.axie  = (uint32_t) 0U;
                p_temp_desc->st_header.dse   = (uint32_t) 0U;
                p_temp_desc->st_header.info0 = (uint32_t) 0U;
                p_temp_desc->st_header.ds    =
                    (p_ctrl->p_ether_cfg->ether_buffer_size & ETHER_TSN_CONVERT_TO_SMALL_BIT);
                p_temp_desc->info1[0] = 0U;
                p_temp_desc->info1[1] = 0U;
                p_temp_desc->tsns     = 0U;
                p_temp_desc->tss      = 0U;

                /* Do not move this process due to descriptor exclusive control */
                p_temp_desc->st_header.dt = (uint32_t) ETHER_TSN_DESC_FEMPTY;

                /* Re-learn the RX descriptor chain (using the HW-tracked current address) if the ring is/was full */
                r_ether_tsn_rx_desc_relearn_if_full(p_ctrl, queue_idx);
            }
        }

        if (ETHER_ZEROCOPY_DISABLE == p_ctrl->p_ether_cfg->zerocopy)
        {
            /* Reset Rx descriptor */
            p_temp_desc->st_header.die   = (uint32_t) ETHER_TSN_RX_DESC_DIE_ENABLE;
            p_temp_desc->st_header.axie  = (uint32_t) 0U;
            p_temp_desc->st_header.dse   = (uint32_t) 0U;
            p_temp_desc->st_header.info0 = (uint32_t) 0U;
            p_temp_desc->st_header.ds    = (p_ctrl->p_ether_cfg->ether_buffer_size & ETHER_TSN_CONVERT_TO_SMALL_BIT);
            p_temp_desc->info1[0]        = 0U;
            p_temp_desc->info1[1]        = 0U;
            p_temp_desc->tsns            = 0U;
            p_temp_desc->tss             = 0U;

            /* Do not move this process due to descriptor exclusive control */
            p_temp_desc->st_header.dt = (uint32_t) ETHER_TSN_DESC_FEMPTY;

            /* Re-learn the RX descriptor chain (using the HW-tracked current address) if the ring is/was full */
            r_ether_tsn_rx_desc_relearn_if_full(p_ctrl, queue_idx);
        }
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_hw_receive
 * Description               Performs reception processing in polling mode.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] queue_idx      Rx queue index
 ***********************************************************************************************************************/
static ether_tsn_rx_status_type_t r_ether_tsn_hw_receive (ether_tsn_instance_ctrl_t * p_ctrl,
                                                          uint32_t                    ctrl_idx,
                                                          uint32_t                    queue_idx)
{
    ether_tsn_etag_rx_ext_status_type_t return_value = ETHER_TSN_EXT_NOT_RECEIVED;
    uint32_t ret_value_as_int = 0U;

    /* Receive the specified queue */
    do
    {
        return_value = r_ether_tsn_rx_queue_process(p_ctrl, ctrl_idx, queue_idx);
    } while (ETHER_TSN_EXT_NOT_RECEIVED_MORE_DATA_AVAILABLE == return_value);

    /* Cast Eth_ExtRxStatusType to Integer  */
    ret_value_as_int = (uint32_t) return_value;

    /* Cast Integer to Eth_RxStatusType and return value */
    return (ether_tsn_rx_status_type_t) ret_value_as_int; /* PRQA S 4342 # Advisory R-10.5 */
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_if_tx_confirmation
 * Description               This API Triggers frame transmission confirmation
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] buf_idx        Buffer of Tx
 ***********************************************************************************************************************/
static void r_ether_tsn_if_tx_confirmation (uint32_t ctrl_idx, uint32_t buf_idx)
{
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    (void) r_ether_tsn_hw_get_egress_time_stamp(ctrl_idx,
                                                buf_idx,
                                                &g_ether_tsn_tx_time_qual, /* PRQA S 3200 # Required R-17.7 */
                                                &g_ether_tsn_tx_time_stamp[ctrl_idx]);
#else
    FSP_PARAMETER_NOT_USED(buf_idx);
#endif
    g_ether_tsn_tx_confirmed[ctrl_idx] = 1U;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_hw_tx_confirmation
 * Description               Performs transmission processing, notifies the upper layer
 *                           of transmission completion and releases the Tx buffer.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 ***********************************************************************************************************************/
static void r_ether_tsn_hw_tx_confirmation (ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx) /* PRQA S 3673 # Advisory R-8.13 */
{
    uint32_t q_idx;
    uint32_t cnt;
    uint32_t tx_cnt;
    uint32_t buf_idx;
    ether_tsn_extended_cfg_t const     * p_extend = ((ether_tsn_extended_cfg_t const *) p_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */
    ether_tsn_ext_desc_type_t          * p_data_desc;
    const ether_tsn_buf_handler_type_t * p_buf_handler;

#if (ETHER_TSN_CFG_GET_TX_STATS_API)                                                                                    /* PRQA S 3332 # Rule-20.9 */
    ether_tsn_mac_address_type_t st_mac_addr;
#endif

    /* Check the status of the Tx descriptor */
    for (q_idx = 0U; q_idx < (uint32_t) p_extend->queue_config.number_of_tx_queue; q_idx++)
    {
        /* Get on-transmit count */
        tx_cnt = g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_buf_tx_cnt[q_idx];

        /* Check if no frame transmitted */
        if (0U == tx_cnt)
        {
            /* No frame in Tx buffer counter. */
            /* Skip because the transmit is not completed */
            continue;
        }

        p_data_desc = g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_tx_desc[q_idx];

        for (cnt = 0U; cnt < tx_cnt; cnt++) /* PRQA S 0771 # Advisory R-15.4 */
        {
            if (ETHER_TSN_DESC_FEMPTY == p_data_desc->st_header.dt)
            {
                /* Get Tx buffer handler */
                buf_idx       = (p_data_desc->st_header.info0 & ETHER_TSN_TX_DESCR_TSUN_MASK) >> 4UL;                /* PRQA S 4491 # DP_MSR_R-10.6_UC1 */
                p_buf_handler = gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].p_buffer_hdr;
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
                if ((ETHER_TSN_TRUE == p_buf_handler->enable_ts) && (ETHER_TSN_INVALID == p_buf_handler->time_qual)) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
                {
                    /* Transmit not completed yet */
                    break;
                }
#endif
#if (ETHER_TSN_CFG_GET_TX_STATS_API)   /* PRQA S 3332 # Rule-20.9 */
                g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.tx_number_of_octets += p_buf_handler->tx_length;
                ETHER_TSN_PACK_ADDRESS_FROM_8(((uint8_t *) p_buf_handler->buf_addr), st_mac_addr);
                if ((0UL == ETHER_TSN_COMPARE_MAC(st_mac_addr, g_ether_tsn_ts_desc_total)) ||
                    (0UL != ETHER_TSN_CHECK_MULTICAST(st_mac_addr)))
                {
                    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.tx_nu_cast_pkts++;
                }
                else
                {
                    g_ether_tsn_ctrl_stat[ctrl_idx].tx_rx_stat.tx_uni_cast_pkts++;
                }
#endif

                /* Check whether the transmission confirmation was enabled */
                if (ETHER_TSN_TRUE == p_buf_handler->tx_confirm) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
                {
                    /* Call the callback function */
                    /* Since the maximum value of controller index is 1, casting to uint8 does no problem. */
                    r_ether_tsn_if_tx_confirmation((uint8_t) ctrl_idx, buf_idx);
                }

                /* Get/save the current state of interrupts */
                FSP_CRITICAL_SECTION_DEFINE;
                FSP_CRITICAL_SECTION_ENTER;

                /* Release the Tx buffer resource */
                r_ether_tsn_release_tx_buffer(ctrl_idx, buf_idx);

                /* Clear tx descriptor */
                p_data_desc->st_header.die   = (uint32_t) 0U;
                p_data_desc->st_header.axie  = (uint32_t) 0U;
                p_data_desc->st_header.dse   = (uint32_t) 0U;
                p_data_desc->st_header.info0 = (uint32_t) 0U;
                p_data_desc->st_header.ds    = (uint32_t) 0U;
                p_data_desc->dptr            = 0U;
                p_data_desc->info1[0]        = 0U;
                p_data_desc->info1[1]        = 0U;

                /* Decrease the number of on-transmission buffer of current Tx queue */
                g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.p_buf_tx_cnt[q_idx]--;

                /* Restore the interrupt state */
                FSP_CRITICAL_SECTION_EXIT;

                /* Update start descriptor */
                p_data_desc++;
                while ((ETHER_TSN_DESC_LEMPTY == p_data_desc->st_header.dt) ||
                       (ETHER_TSN_DESC_LINK == p_data_desc->st_header.dt))
                {
                    p_data_desc->st_header.dt = (uint32_t) ETHER_TSN_DESC_LINK;

                    p_data_desc = (ether_tsn_ext_desc_type_t *) (p_data_desc->dptr); /* PRQA S 0306 # Advisory R-11.4 */
                }

                g_ether_tsn_ctrl_stat[ctrl_idx].hw_stat.pp_head_tx_desc[q_idx] = p_data_desc;
            }
            else
            {
                /* Transmit not completed yet */
                break;
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_tx_confirmation
 * Description           This API Triggers frame transmission confirmation
 * @param[in] p_ctrl     Pointer to the control block for the channel
 * @param[in] ctrl_idx   Index of the controller
 ***********************************************************************************************************************/
static void r_ether_tsn_tx_confirmation (ether_tsn_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    if (ETHER_TSN_MODE_ACTIVE == g_ether_tsn_ctrl_stat[ctrl_idx].mode)
    {
        r_ether_tsn_hw_tx_confirmation(p_ctrl, ctrl_idx);
    }
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_receive
 * Description               This API Triggers frame reception
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] queue_idx      Specifies the related FIFO
 * @param[in] p_rx_status    Pointer to check status
 ***********************************************************************************************************************/
static void r_ether_tsn_receive (ether_tsn_instance_ctrl_t  * p_ctrl,
                                 uint8_t                      queue_idx,
                                 ether_tsn_rx_status_type_t * p_rx_status)
{
    /* Initialize controller index */
    uint32_t ctrl_idx = p_ctrl->p_ether_cfg->channel;

    if (ETHER_TSN_NULL_PTR != p_rx_status)
    {
        /* Initial Value Set */
        *p_rx_status = ETHER_TSN_NOT_RECEIVED;
    }

    if (ETHER_TSN_MODE_ACTIVE == g_ether_tsn_ctrl_stat[ctrl_idx].mode)
    {
        *p_rx_status = r_ether_tsn_hw_receive(p_ctrl, ctrl_idx, (uint32_t) queue_idx);

        r_ether_tsn_call_callback(p_ctrl, ETHER_EVENT_RX_COMPLETE);
    }
}

#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_update_phys_addr_filter
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] p_phys_addr    Pointer to memory containing the physical destination address (MAC address) in network byte order.
 * @param[in] action         Add or remove the address from the Ethernet controllers
 ***********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_update_phys_addr_filter (uint32_t                       ctrl_idx,
                                                           uint8_t                      * p_phys_addr, /* PRQA S 3673 # Advisory R-8.13 */
                                                           ether_tsn_filter_action_type_t action)
{
    ether_tsn_type               result = ETHER_TSN_OK;
    ether_tsn_mac_address_type_t st_mac_addr;
    uint32_t filter_idx = 0U;

    /* Pack MAC address from user to the structure to be compared */
    ETHER_TSN_PACK_ADDRESS_FROM_8(p_phys_addr, st_mac_addr);                       /* PRQA S 3469 # Advisory D-4.9 */

    /* FF:..:FF and 00:..:00 are not allowed while the controller is ACTVIE */
    if ((ETHER_TSN_MODE_ACTIVE == g_ether_tsn_ctrl_stat[ctrl_idx].mode) &&
        ((0UL == ETHER_TSN_COMPARE_MAC(st_mac_addr, g_ether_tsn_ts_desc_total)) || /* PRQA S 3469 # Advisory D-4.9 */
         (0UL == ETHER_TSN_COMPARE_MAC(st_mac_addr, g_ether_tsn_null_addr))))      /* PRQA S 3469 # Advisory D-4.9 */
    {
        result = ETHER_TSN_NOT_OK;
    }
    else
    {
        result = ETHER_TSN_OK;

        if (0UL == ETHER_TSN_COMPARE_MAC(st_mac_addr, g_ether_tsn_ts_desc_total))  /* PRQA S 3469 # Advisory D-4.9 */
        {
            /* If FF:..:FF is specified, enter promiscuous mode */
            g_ether_tsn_ctrl_stat[ctrl_idx].promiscuous = ETHER_TSN_TRUE;          /* PRQA S 1295 # DP_MSR_R-10.3_UC1 */
        }
        else if (0UL == ETHER_TSN_COMPARE_MAC(st_mac_addr, g_ether_tsn_null_addr)) /* PRQA S 3469 # Advisory D-4.9 */
        {
            /* If 00:..:00 is specified, clear promiscuous mode and all filters */
            r_ether_tsn_clear_all_address_filters(ctrl_idx);
        }
        else
        {
            if (ETHER_TSN_ADD_TO_FILTER == action)
            {
                filter_idx = r_ether_tsn_get_filter_index(ctrl_idx, &st_mac_addr);
                if ((0UL == ETHER_TSN_CHECK_MULTICAST(st_mac_addr)) || /* PRQA S 3469 # Advisory D-4.9 */
                    (ETHER_TSN_INVALID_FILTER_INDEX != filter_idx) ||
                    (ETHER_TSN_INVALID_FILTER_INDEX == g_ether_tsn_ctrl_stat[ctrl_idx].active_filter_bits))
                {
                    result = ETHER_TSN_NOT_OK;
                }
                else
                {
                    r_ether_tsn_add_address_filter(ctrl_idx, &st_mac_addr);
                }
            }
            else
            {
                filter_idx = r_ether_tsn_get_filter_index(ctrl_idx, &st_mac_addr);
                if (ETHER_TSN_INVALID_FILTER_INDEX == filter_idx)
                {
                    result = ETHER_TSN_NOT_OK;
                }
                else
                {
                    r_ether_tsn_remove_address_filter(ctrl_idx, filter_idx);
                }
            }
        }
    }

    return result;
}

#endif
#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name             r_ether_tsn_add_address_filter
 * Description               Add a address filter to the filter list
 * @param[in] ctrl_idx       Index of a controller
 * @param[in] p_mac_addr     Pointer to a MAC address to be searched
 ***********************************************************************************************************************/
static void r_ether_tsn_add_address_filter (uint32_t ctrl_idx, const ether_tsn_mac_address_type_t * const p_mac_addr)
{
    uint32_t filter_idx = 0U;
    uint32_t registered = ETHER_TSN_FALSE;

    for (filter_idx = 0U; (ETHER_TSN_FALSE == registered) && (filter_idx < ETHER_TSN_MAX_FILTERS); filter_idx++) /* PRQA S 1881 # DP_MSR_R-10.4_UC1 */
    {
        if (0U == (g_ether_tsn_ctrl_stat[ctrl_idx].active_filter_bits & (1U << (filter_idx))))                   /* PRQA S 1891 # Advisory R-10.7 */
        {
            g_ether_tsn_address_filters[ctrl_idx][filter_idx]   = *p_mac_addr;
            g_ether_tsn_ctrl_stat[ctrl_idx].active_filter_bits |= (1U << filter_idx);                            /* PRQA S 1891 # Advisory R-10.7 */
            registered = ETHER_TSN_TRUE;
        }                                                                                                        /* else : no action required */
    }
}

#endif
#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name         : r_ether_tsn_remove_address_filter
 * Description           : Remove a filter from the filter list
 * @param[in] ctrl_idx       Index of a controller
 * @param[in] filter_idx     Index of an address in the filter list
 ***********************************************************************************************************************/
static void r_ether_tsn_remove_address_filter (uint32_t ctrl_idx, uint32_t filter_idx)
{
    /* Clear an active bit */
    g_ether_tsn_ctrl_stat[ctrl_idx].active_filter_bits &= ~(1U << filter_idx); /* PRQA S 1891 # Advisory R-10.7 */
}

#endif

#if (ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name         r_ether_tsn_get_filter_index
 * Description           Get the index of the specified MAC address if exists
 * @param[in] ctrl_idx    Index of a controller
 * @param[in] p_mac_addr  Pointer to a MAC address to be searched
 ***********************************************************************************************************************/
static uint32_t r_ether_tsn_get_filter_index (uint32_t ctrl_idx, const ether_tsn_mac_address_type_t * const p_mac_addr)
{
    uint32_t filter_idx = 0U;
    uint32_t found_idx  = ETHER_TSN_INVALID_FILTER_INDEX; /* Invalid filter index */

    /* Check the specified address is already registered */
    for (filter_idx = 0U;
         (ETHER_TSN_INVALID_FILTER_INDEX == found_idx) && (filter_idx < ETHER_TSN_MAX_FILTERS);
         filter_idx++)
    {
        if ((0U != (g_ether_tsn_ctrl_stat[ctrl_idx].active_filter_bits & (1U << filter_idx))) &&           /* PRQA S 1891 # Advisory R-10.7 */
            (0U == ETHER_TSN_COMPARE_MAC(*p_mac_addr, g_ether_tsn_address_filters[ctrl_idx][filter_idx]))) /* PRQA S 3469 # Advisory D-4.9 */
        {
            found_idx = filter_idx;
        }
        else
        {
            /* No action required */
        }
    }

    return found_idx;
}

#endif

/*******************************************************************************************************************//**
 * Function Name: r_ether_tsn_call_callback
 * Calls user callback.
 * @param[in]     p_instance_ctrl      Pointer to ether instance control block
 * @param[in]     event                Event code
 **********************************************************************************************************************/
static void r_ether_tsn_call_callback (ether_tsn_instance_ctrl_t * p_instance_ctrl, ether_event_t event) /* PRQA S 3673 # Advisory R-8.13 */
{
    ether_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    ether_callback_args_t * p_args = p_instance_ctrl->p_callback_memory;

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

    p_args->channel   = p_instance_ctrl->p_ether_cfg->channel;
    p_args->event     = event;
    p_args->p_context = p_instance_ctrl->p_context;

    /* If the project is not trust zone Secure, then it will never need to change security state in order to call the callback. */
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

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)

/*******************************************************************************************************************//**
 * Function Name: r_ether_tsn_hw_get_ingress_time_stamp
 * Read TimeStamp from a message received.
 * @param[in]     ctrl_idx            Index of a controller
 * @param[in]     p_data              Pointer to the message buffer
 * @param[in]     p_time_qual         Quality of the Time Stamp
 * @param[in]     p_time_stamp        TS read from the Rx
 **********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_hw_get_ingress_time_stamp (uint32_t                      ctrl_idx,
                                                             uint8_t                     * p_data, /* PRQA S 3673 # Advisory R-8.13 */
                                                             ether_tsn_time_qual_type_t  * p_time_qual,
                                                             ether_tsn_time_stamp_type_t * p_time_stamp)
{
    uint8_t const * p_comp;
    ether_tsn_type  return_value = ETHER_TSN_OK;

    /* Read TimeStamp from the Rx frame.Timestamp */
    p_comp = (uint8_t const *) (g_ether_tsn_rx_frame[ctrl_idx].frame_addr); /* PRQA S 0306 # Advisory R-11.4 */
    if (p_comp == p_data)
    {
        p_time_stamp->nano_seconds = g_ether_tsn_rx_frame[ctrl_idx].tsns;
        p_time_stamp->seconds      = g_ether_tsn_rx_frame[ctrl_idx].tss;
        p_time_stamp->seconds_hi   = 0U;

        /* TimeStamp is valid  */
        *p_time_qual = ETHER_TSN_VALID;
        return_value = ETHER_TSN_OK;
    }
    else
    {
        p_time_stamp->nano_seconds = 0U;
        p_time_stamp->seconds      = 0U;
        p_time_stamp->seconds_hi   = 0U;

        /* TimeStamp is invalid  */
        *p_time_qual = ETHER_TSN_INVALID;
        return_value = ETHER_TSN_NOT_OK;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name: r_ether_tsn_hw_get_egress_time_stamp
 * Read TimeStamp from a message just transmitted
 * @param[in]     ctrl_idx            Index of a controller
 * @param[in]     buff_idx            Buffer index
 * @param[in]     p_time_qual         Quality of the Time Stamp
 * @param[in]     p_time_stamp        TS read from the Tx
 **********************************************************************************************************************/
static ether_tsn_type r_ether_tsn_hw_get_egress_time_stamp (uint32_t                      ctrl_idx,
                                                            uint32_t                      buff_idx,
                                                            ether_tsn_time_qual_type_t  * p_time_qual,
                                                            ether_tsn_time_stamp_type_t * p_time_stamp)
{
    ether_tsn_buf_handler_type_t const * p_tx_buffer_node;

    /* Protect update Time Qual from interrupt */
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

    /* Get Tx buffer handler from the Tx buffer manager table */
    p_tx_buffer_node =
        (ether_tsn_buf_handler_type_t const *) gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buff_idx].p_buffer_hdr;
    *p_time_qual  = p_tx_buffer_node->time_qual;
    *p_time_stamp = p_tx_buffer_node->time_stamp;

    /* Exit critical section */
    FSP_CRITICAL_SECTION_EXIT;

    return ETHER_TSN_OK;
}

/*******************************************************************************************************************//**
 * Function Name: r_ether_tsn_ts_desc_config
 * Create timestamp descriptor chain & learning descriptor.
 * @param[in]     p_instance_ctrl      Pointer to ether instance control block
 * @param[in]     ctrl_idx             Index of a controller
 **********************************************************************************************************************/
static void r_ether_tsn_ts_desc_config (ether_tsn_instance_ctrl_t * p_instance_ctrl, uint32_t ctrl_idx) /* PRQA S 3673 # Advisory R-8.13 */
{
    ether_tsn_ext_desc_with_ts_type_t * p_desc_ts_chain;
    uint32_t cnt = 0U;

    /* TS descriptor chain setting */
    for (cnt = 0U; cnt < ETHER_TSN_DESC_TOTAL; cnt++)
    {
        p_desc_ts_chain = &gp_ether_tsn_desc_ts_chain_table[ctrl_idx][cnt];

        /* Set frame empty type */
        p_desc_ts_chain->st_header.die   = (uint32_t) ETHER_TSN_TS_DESC_DIE_ENABLE;
        p_desc_ts_chain->st_header.axie  = (uint32_t) 0U;
        p_desc_ts_chain->st_header.dse   = (uint32_t) 0U;
        p_desc_ts_chain->st_header.info0 = (uint32_t) 0U;
        p_desc_ts_chain->st_header.ds    = (uint32_t) 0U;
        p_desc_ts_chain->dptr            = 0U;
        p_desc_ts_chain->info1[0]        = 0U;
        p_desc_ts_chain->info1[1]        = 0U;
        p_desc_ts_chain->tsns            = 0U;
        p_desc_ts_chain->tss             = 0U;
        p_desc_ts_chain->st_header.dt    = (uint32_t) ETHER_TSN_DESC_FEMPTY_ND;
    }

    /* Set link type for cyclic descriptor */
    p_desc_ts_chain                  = &gp_ether_tsn_desc_ts_chain_table[ctrl_idx][cnt];
    p_desc_ts_chain->st_header.die   = (uint32_t) 0U;
    p_desc_ts_chain->st_header.axie  = (uint32_t) 0U;
    p_desc_ts_chain->st_header.dse   = (uint32_t) 0U;
    p_desc_ts_chain->st_header.info0 = (uint32_t) 0U;
    p_desc_ts_chain->st_header.ds    = (uint32_t) 0U;
    p_desc_ts_chain->dptr            = (uint32_t) &gp_ether_tsn_desc_ts_chain_table[ctrl_idx][0]; /* PRQA S 0329 # Advisory R-11.4 */
    p_desc_ts_chain->info1[0]        = 0U;
    p_desc_ts_chain->info1[1]        = 0U;
    p_desc_ts_chain->tsns            = 0U;
    p_desc_ts_chain->tss             = 0U;
    p_desc_ts_chain->st_header.dt    = (uint32_t) ETHER_TSN_DESC_LINK;

    /* Learning Ts descriptor table */
    p_instance_ctrl->p_reg_axibmi->TSS0 = ETHER_TSN_ETND_TSS_CONFIG; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg_axibmi->TSA0 = p_desc_ts_chain->dptr;     /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
}

#endif

/*******************************************************************************************************************//**
 * Function Name: r_ether_tsn_enable_icu
 * @param[in] p_instance_ctrl    Pointer to the control block for the channel
 ***********************************************************************************************************************/
static void r_ether_tsn_enable_icu (ether_tsn_instance_ctrl_t * const p_instance_ctrl)
{
    ether_tsn_extended_cfg_t const * p_extend =
        ((ether_tsn_extended_cfg_t const *) p_instance_ctrl->p_ether_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */

#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    if (ETHER_TSN_CHANNEL_1 == ctrl_idx)
    {
        /* Configure the general interrupt. */
        R_BSP_IrqCfgEnable(p_extend->sgmii_irq, p_extend->sgmii_interrupt_priority, p_instance_ctrl);
    }
#endif

    /* Configure the Ethernet interrupt. */
    R_BSP_IrqCfgEnable(p_instance_ctrl->p_ether_cfg->irq,
                       (uint16_t) (p_instance_ctrl->p_ether_cfg->interrupt_priority),
                       p_instance_ctrl);

    /* Configure the gptp interrupt. */
    R_BSP_IrqCfgEnable(p_extend->gptp_irq, (uint16_t) (p_extend->interrupt_priority), p_instance_ctrl);
}

/*******************************************************************************************************************//**
 * Function Name: r_ether_tsn_disable_icu
 * @param[in] p_instance_ctrl    Pointer to the control block for the channel
 ***********************************************************************************************************************/
static void r_ether_tsn_disable_icu (ether_tsn_instance_ctrl_t * const p_instance_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    ether_tsn_extended_cfg_t const * p_extend =
        ((ether_tsn_extended_cfg_t const *) p_instance_ctrl->p_ether_cfg->p_extend);    /* PRQA S 0316 # Advisory R-11.5 */

#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    if (ETHER_TSN_CHANNEL_1 == ctrl_idx)
    {
        /* Disables interrupts in the NVIC for ETND1. */
        R_BSP_IrqDisable(p_extend->sgmii_irq);

        /* Clears the control block from the vector information array for ETND1. */
        R_FSP_IsrContextSet(p_extend->sgmii_irq, NULL);
    }
#endif

    /* Disables interrupts in the NVIC. */
    R_BSP_IrqDisable(p_instance_ctrl->p_ether_cfg->irq);
    R_BSP_IrqDisable(p_extend->gptp_irq);

    /* Clears the control block from the vector information array. */
    R_FSP_IsrContextSet(p_instance_ctrl->p_ether_cfg->irq, NULL);
    R_FSP_IsrContextSet(p_extend->gptp_irq, NULL);
}

/*******************************************************************************************************************//**
 * ISR called when EHT TSN detect data transmit or receive can be read data from register.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void r_ether_tsn_data_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;
    uint32_t txi_stat        = 0U;
    uint32_t rxi_full_stat   = 0U;
    uint32_t rx_queue_offset = 0U;
    uint32_t queue_idx       = 0U;
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    uint32_t ts_int_stat;
    uint32_t buf_idx  = 0U;
    uint32_t ring_idx = 0U;
    ether_tsn_buf_handler_type_t      * p_tx_buffer_node;
    ether_tsn_ext_desc_with_ts_type_t * p_desc_ts_chain;
    ether_tsn_ext_desc_with_ts_type_t * p_link_desc;                                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */

    /* Initialize controller index */
    uint32_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    ts_int_stat = p_instance_ctrl->p_reg_axibmi->TSDIS & ETHER_TSN_ETND_TSDIS_0_MASK; /* PRQA S 4491,0662 # DP_MSR_R-10.6_UC1,DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    if (0UL != ts_int_stat)
    {
        /* Clear the TS Descriptor Data Interrupt Status */
        p_instance_ctrl->p_reg_axibmi->TSDIS = ETHER_TSN_ETND_TSDIS_0_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        p_link_desc = &gp_ether_tsn_desc_ts_chain_table[ctrl_idx][ETHER_TSN_DESC_TOTAL];

        /* Check status of link descriptor */
        if (ETHER_TSN_DESC_LEMPTY == p_link_desc->st_header.dt)
        {
            /* Reset link descriptor status */
            p_link_desc->st_header.dt = (uint32_t) ETHER_TSN_DESC_LINK;
        }

        /* Search timestamp descriptor */
        ring_idx        = g_ether_tsn_ctrl_stat[ctrl_idx].ts_desc_tail;
        p_desc_ts_chain = &gp_ether_tsn_desc_ts_chain_table[ctrl_idx][ring_idx];

        while (ETHER_TSN_DESC_FSINGLE == p_desc_ts_chain->st_header.dt)
        {
            buf_idx          = (p_desc_ts_chain->st_header.info0 & ETHER_TSN_TX_DESCR_TSUN_MASK) >> 4UL; /* PRQA S 4491 # DP_MSR_R-10.6_UC1 */
            p_tx_buffer_node =
                (ether_tsn_buf_handler_type_t *) gp_ether_tsn_tx_buffer_mgr_table[ctrl_idx][buf_idx].p_buffer_hdr;

            /* Read timestamp value to Tx buffer */
            p_tx_buffer_node->time_stamp.nano_seconds = p_desc_ts_chain->tsns;
            p_tx_buffer_node->time_stamp.seconds      = p_desc_ts_chain->tss;
            p_tx_buffer_node->time_stamp.seconds_hi   = (uint16_t) 0U;

            /* Set timestamp quality information as ETH_VALID */
            p_tx_buffer_node->time_qual = ETHER_TSN_VALID;

            /* Release timestamp descriptor */
            p_desc_ts_chain->st_header.die   = (uint32_t) ETHER_TSN_RX_DESC_DIE_ENABLE;
            p_desc_ts_chain->st_header.axie  = (uint32_t) 0U;
            p_desc_ts_chain->st_header.dse   = (uint32_t) 0U;
            p_desc_ts_chain->st_header.info0 = (uint32_t) 0U;
            p_desc_ts_chain->st_header.ds    = (uint32_t) 0U;
            p_desc_ts_chain->dptr            = 0U;
            p_desc_ts_chain->info1[0]        = 0U;
            p_desc_ts_chain->info1[1]        = 0U;
            p_desc_ts_chain->tsns            = 0U;
            p_desc_ts_chain->tss             = 0U;

            /* Set descriptor type to EMPTY */
            p_desc_ts_chain->st_header.dt = (uint32_t) ETHER_TSN_DESC_FEMPTY_ND;

            /* Get buffer number by increase ring index to 1 and reset if value greater than maximum descriptors */
            ring_idx = (ring_idx + 1UL) % ((uint32_t) (ETHER_TSN_DESC_TOTAL + ETHER_TSN_CYCLIC_DESC_NUM));

            /* Get buffer number by increase ring index to 1 */
            p_desc_ts_chain = &gp_ether_tsn_desc_ts_chain_table[ctrl_idx][ring_idx];

            /* Set descriptor to be read next */
            /* If descriptor type is LINK or EMPTY, update current descriptor to next descriptor address */
            while ((ETHER_TSN_DESC_LINK == p_desc_ts_chain->st_header.dt) ||
                   (ETHER_TSN_DESC_LEMPTY == p_desc_ts_chain->st_header.dt))
            {
                p_desc_ts_chain->st_header.dt = (uint32_t) ETHER_TSN_DESC_LINK;
                p_desc_ts_chain               = (ether_tsn_ext_desc_with_ts_type_t *) p_desc_ts_chain->dptr; /* PRQA S 0306 # Advisory R-11.4 */
                ring_idx = (ring_idx + 1UL) % ((uint32_t) (ETHER_TSN_DESC_TOTAL + ETHER_TSN_CYCLIC_DESC_NUM));
            }
        }

        g_ether_tsn_ctrl_stat[ctrl_idx].ts_desc_tail = ring_idx;
        r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_TX_TIMESTAMP);
    }
#endif

    /* Get Tx status interrupt */
    txi_stat      = p_instance_ctrl->p_reg_axibmi->TDIS0;                                                              /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    rxi_full_stat = ETHER_TSN_GET_RX_FULL_INT_STAT(queue_idx, p_instance_ctrl->p_reg_axibmi->RFEIS0, rx_queue_offset); /* PRQA S 0662,3469 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1,Advisory D-4.9 */

    if (0U != txi_stat)
    {
        /* Tx interrupt */
        /* Clear the TX Descriptor Data Interrupt Status */
        p_instance_ctrl->p_reg_axibmi->TDIS0 = ETHER_TSN_TDIS_0_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* If a callback is provided, then call it with callback argument. */
        r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_TX_INTERRUPT);

        /* Tx confirmation */
        r_ether_tsn_tx_confirmation(p_instance_ctrl, ctrl_idx);

        /* If a callback is provided, then call it with callback argument. */
        r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_TX_COMPLETE);
    }
    else
    {
        /* Rx interrupt */
        /* Clear the RX Descriptor Data Interrupt Status */
        p_instance_ctrl->p_reg_axibmi->RDIS0 = ETHER_TSN_RDIS_0_MASK; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* If a callback is provided, then call it with callback argument. */
        r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_RX_INTERRUPT);
        if (0U != rxi_full_stat)
        {
            /* Inform that rx descriptor is full */
            g_ether_tsn_rx_full = ETHER_TSN_TRUE;

            /* Clear rx descriptor full interrupt status */
            p_instance_ctrl->p_reg_axibmi->RFEIS0 = rxi_full_stat; /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* If a callback is provided, then call it with callback argument. */
            r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_RX_MESSAGE_LOST);
        }
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}

#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)

/*******************************************************************************************************************//**
 * General ISR called when EHT TSN SGMII detect operation mode OFF.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void r_ether_tsn_gerenal_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    uint32_t irq_stat = 0U;
    uint32_t sgmii_stat;
    uint32_t sgmii_mode;

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    irq_stat = p_instance_ctrl->p_reg_sgmii->ETNDzSGINTS & ETHER_TSN_SGINTS_RDY_UNRDY;

    if (0 != irq_stat)
    {
        sgmii_stat = p_instance_ctrl->p_reg_sgmii->ETNDzSGINTS;
        sgmii_mode = p_instance_ctrl->p_reg_sgmii->ETNDzSGOPMS;

        if ((ETHER_TSN_SGINTS_RDY == sgmii_stat) ||
            ((ETHER_TSN_SGINTS_RDY_UNRDY == sgmii_stat) &&
             (ETH_TSN_SGOPMS_DATA_COM == (ETH_TSN_SGOPMS_XMIT_MASK & sgmii_mode))))
        {
            p_instance_ctrl->p_reg_rmaca->MPIC = (p_instance_ctrl->p_reg_rmaca->MPIC & ~ETH_TSN_SGOPMS_SPEED_MASK) |
                                                 (sgmii_mode & ETH_TSN_SGOPMS_SPEED_MASK);

            /* PHY SGMII link-up */
            r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_LINK_ON);
        }
        else
        {
            p_instance_ctrl->p_reg_rmaca->MPIC &= ~ETH_TSN_SGOPMS_SPEED_MASK;

            /* PHY SGMII link-down */
            r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_LINK_OFF);
        }

        p_instance_ctrl->p_reg_sgmii->ETNDzSGINTS = sgmii_stat;
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}

#endif

/*******************************************************************************************************************//**
 * @brief Verifies the Ethernet link is up or not.
 *
 * @param[in] p_instance_ctrl   Pointer to the control block for the channel
 *
 * @retval  FSP_SUCCESS:                  Link is up
 * @retval  FSP_ERR_ETHER_ERROR_LINK:     Link is down
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK  Initialization of PHY-LSI failed.
 **********************************************************************************************************************/
static fsp_err_t ether_tsn_link_status_check (ether_tsn_instance_ctrl_t const * const p_instance_ctrl)
{
    fsp_err_t err         = FSP_SUCCESS;
    fsp_err_t link_status = FSP_SUCCESS;

    link_status = p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->linkStatusGet(
        p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_ctrl);

    if (FSP_ERR_ETHER_PHY_ERROR_LINK == link_status)
    {
        /* Link is down */
        err = FSP_ERR_ETHER_ERROR_LINK;
    }
    else
    {
        /* Link is up */
        err = FSP_SUCCESS;
    }

    return err;
}                                                        /* End of function ether_tsn_link_status_check() */

BSP_INTERRUPT_ATTRIBUTE void r_ether_tsn_gptp_isr (void) /* PRQA S 1503 # DP_MSR_R-2.1_UC1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    IRQn_Type irq = R_FSP_CurrentIrqGet();
    ether_tsn_instance_ctrl_t * p_instance_ctrl = (ether_tsn_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t gis_stat;

    /* Get the lowest time compare match interrupt */
    gis_stat = ETHER_TSN_SEARCH_MSB_1(p_instance_ctrl->p_reg_gptma->GIS);                                 /* PRQA S 0662,3469 # DP_MSR_D-1.1_UC1,DP_MSR_R-1.2_UC1,Advisory D-4.9 */

    /* Clear time compare match flag */
    p_instance_ctrl->p_reg_gptma->GIS |= (uint8_t) (1U << gis_stat);                                      /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* If a callback is provided, then call it with callback argument. */
    r_ether_tsn_call_callback(p_instance_ctrl, ETHER_EVENT_TIME_COMPARE_MATCH);
#endif

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}
