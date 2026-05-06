/* ${REA_DISCLAIMER_PLACEHOLDER} */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <string.h>
#include "r_ether_t1s.h"
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "ETNF" in ASCII, used to determine if channel is open. */
#define ETHER_T1S_OPEN                         (0x45544E46ULL)
#define ETHER_T1S_INIT_CONTROLLER_INDEX        (0UL)
#define ETHER_T1S_NULL_PTR                     ((void *) NULL)
#define ETHER_T1S_TOTAL_CTRL_CONFIG            (0x01UL)
#define ETH_ETNF_CCC_OPC_MASK                  (0x00000003UL)
#define ETH_ETNF_CCC_OPC_OPERATION             (0x00000002UL)
#define ETH_ETNF_CCC_DTSR                      (0x00000100UL)
#define ETHER_T1S_NOT_OK                       (0x01UL)
#define ETHER_T1S_OK                           (0x00UL)
#define ETHER_T1S_COUNTER_MAX_VALUE            (0xFFFFU)
#define ETH_ETNF_CSR_DTS                       (0x00000100UL)
#define ETHER_T1S_TIMEOUT_COUNT                (63158UL)
#define ETH_ETNF_CSR_OPS                       0x0000000FUL
#define ETH_RX_FIFO_CRIT_LVL                   (uint32_t) 0x1800U /* Rx FIFO Critical Level */
#define ETH_BECHANNEL                          (uint8_t) 0U       /* best effort channel */
#define ETH_NCCHANNEL                          (uint8_t) 1U       /* network control channel */
#define ETH_RX_BUF_LENGTH                      (1518U)
#define ETH_FCS_LENGTH                         (4UL)
#define ETH_ETNF_CCC_LBME                      (0x01000000UL)
#define ETH_ETNF_EIC_ERROR                     (0x00000004UL)
#define ETH_ETNF_RCR_ENCF_SHIFT                (1UL)
#define ETH_ETNF_RCR_ESF_SHIFT                 (2UL)
#define ETH_ETNF_RCR_ETS0_SHIFT                (4UL)
#define ETH_ETNF_RCR_ETS2_SHIFT                (5UL)
#define ETH_ETNF_RCR_RFCL_SHIFT                (16UL)
#define ETH_ETNF_TGC_TSM1_SHIFT                (1U)
#define ETH_ETNF_TGC_TSM2_SHIFT                (2U)
#define ETH_ETNF_TGC_TSM3_SHIFT                (3U)
#define ETH_ETNF_TGC_TQP0_SHIFT                (4U)
#define ETH_ETNF_TGC_TQP1_SHIFT                (5U)
#define ETH_ETNF_TGC_TBDn_VALUE                (0x00222200UL)
#define ETH_ETNF_SFO_FBP_VALUE                 (0x16U)
#define ETH_ETNF_SFM0_TALKERFILTER             (0xFFFFFFFFUL)
#define ETH_ETNF_SFM1_TALKERFILTER             (0x0000FFFFUL)
#define ETH_ETNF_SFM0_LISNERFILTER             (0xFFFFFFFFUL)
#define ETH_ETNF_SFM1_LISNERFILTER             (0xFFFFFFFFUL)
#define ETH_RX_QUEUE_INDEX_AVAILABLE_FILTER    ((uint8_t) 2U)
#define ETH_NO_LOAD_REQ                        (0x1FUL)
#define ETH_ETNF_GET_RQCi(LulQIndex)               ((LulQIndex) / 4UL)
#define ETH_ETNF_SET_RQCi_RSMr(val, LulQIndex)     ((uint32_t) (val) << (((LulQIndex) % 4UL) * 8UL))
#define ETH_ETNF_SET_RQCi_UFCCr(val, LulQIndex)    ((uint32_t) (val) << ((((LulQIndex) % 4UL) * 8UL) + 4UL))
#define ETH_ETNF_SET_RQCi_PIAr(val, LulQIndex)     ((uint32_t) (val) << ((((LulQIndex) % 4UL) * 8UL) + 6UL))
#define ETH_ETNF_GET_UFCVi(LulQIndex)              ((LulQIndex) / 4UL)
#define ETH_ETNF_GET_UFCVi_CVr(val, LulQIndex)     ((uint32_t) (((val) >> (((LulQIndex) % 4UL) * 8UL)) & 0x3FUL))
#define ETH_ETNF_GET_UFCDi(q_idx)                  ((q_idx) / 4U)
#define ETH_ETNF_SET_UFCDi_DVr(val, q_idx)         ((val) << (((uint32_t) (q_idx) % 4UL) * 8UL))
#define ETH_ETNF_SFO_FBP_VALUE               (0x16U)
#define ETH_ETNF_GET_LOW_SFPi(LucSFPidx)           ((LucSFPidx) * 2U)
#define ETH_ETNF_CCC_CSEL_PTP_CLK_SRC        (0x00030000UL)
#define ETH_ETNF_TCCR_TFEN                   (0x00000100UL)
#define ETH_PHY_PREAMBLE_SIZE                (32UL)
#define ETH_CPUCLK_MHZ                       (240UL)
#define ETH_NS2HZ_AVB(nano_sec)                    ((ETH_CPUCLK_MHZ * (nano_sec)) / 1000UL)
#define ETH_ETNF_PIR_MMD                     (0x00000002UL)
#define ETH_ETNF_PIR_MDO                     (0x00000004UL)
#define ETH_MDIO_SETUP_TIME                  (10UL)
#define ETH_ETNF_WORST_SLEW_RATE             (90UL)
#define ETH_ETNF_PIR_MDC                     (0x00000001UL)
#define ETH_ETNF_GET_MDI(regval)    (((uint32_t) (regval) >> 3UL) & 1UL)

#define ETHER_T1S_FALSE                      (0UL)
#define ETHER_T1S_TRUE                       (1UL)
#define ETH_DWORD_SIZE                       (4UL)
#define ETH_ALIGN_TO_32BIT(n)       ((((n) + ETH_DWORD_SIZE) - 1UL) & 0xFFFFFFFCUL)
#define ETH_NORMAL_DESC_SIZE                 (8U)  /* normal descriptor size(in byte)*/
#define ETHER_T1S_TX_BUFFER                  (8UL)
#define ETH_CYCLIC_DESC_NUM                  (1U)  /* Number of descriptors for the Cyclic */
#define ETH_SPECIAL_DESC_SIZE                (20U) /*special descriptor size(in byte)*/
/* Descriptor Types */
#define ETH_DESC_FSINGLE_AVB                 (7U)
#define ETH_DESC_LINK_AVB                    (8U)
#define ETH_DESC_LINKFIX                     (9U)
#define ETH_DESC_FEMPTY_AVB                  (12U)
#define ETH_DESC_RETAIN_TS                   (1U)
#define ETH_DESC_MAC_FIFO                    (1U)
#define ETH_HZ2NS(clock_hz)    (1000000000UL / (clock_hz))

/* Clear status */
#define ETH_ETNF_RIS0_FRF_MARK               (0x0003FFFFUL)

/* Maximum number of tx descriptors */
#define ETH_TX_DESC_MAX                      (64U)
#define ETH_TIC_TFUE                         (0x00000100UL)

/* CBS Limits */
#define ETH_MAX_CIV                          (0x0000FFFFU) /* 65535 */
#define ETH_MIN_CDV                          (0xFFFF0000U) /* -65536 */
#define ETH_RXBEQ_OFFSET                     ((uint8_t) 4)
#define ETH_ETNF_ECMR_ZPF_CRS_NOT_CHECKED    (0x00080000UL)
#define ETH_ETNF_ECMR_PRM                    (0x00000001UL)
#define ETH_ETNF_GCCR_PGM                    (0x00010000UL)
#define ETH_TCCR_TSRQX_MASK                  (0x0000000FUL)
#define ETH_CSR_TDUO_RPO_TPOX_MASK           (0x003F0000UL)
#define ETH_CSR_TPOX_MASK                    (0x000F0000UL)
#define ETH_CSR_RPO_MASK                     (0x00100000UL)
#define ETH_ETNF_CSR_TDUO_RPO                (0x00300000UL)

/* Compare two MAC addresses, return 0 when both are equal */
#define ETHER_COMPARE_MAC(a, b)            (((a).h32 ^ (b).h32) | ((a).l16 ^ ((b).l16)))
#define ETHER_T1S_INVALID_FILTER_INDEX       (0xFFFFFFFFUL)
#define ETHER_T1S_CHECK_MULTICAST(addr)    ((addr).h32 & 0x01000000UL)
#define ETHER_T1S_MAX_FILTERS                (32UL)
#define ETH_VLAN_SIZE                        (4UL)
#define ETHER_T1S_PRV_SYS_CLK_HZ             (10UL)
#define ETHER_T1S_CLK_CYCLE_TIME             (100UL)
#define ETH_MIN_PAYLOAD_SIZE_AVB             (1UL)
#define ETH_MAX_TX_PAYLOAD_LENGTH            (1500UL + ETH_VLAN_SIZE)
#define ETH_HEADER_SIZE                      (14UL)
#define ETH_LIST_MAX_NODES                   (0xFFFFFFFFUL)
#define ETH_BYTE_BITS                        (8UL)
#define ETH_DESCR_TAG_MASK                   (0x000003FFUL)

/* Check whehter a address has the multicast bit */
#define ETH_CHECK_MULTICAST(addr)    ((addr).h32 & 0x01000000UL)
#define ETHER_T1S_VLFRAME_SIZE               (1522U)
#define ETH_SRC_DST_ADDRESS_SIZE             (12UL)
#define ETHER_T1S_ADDRESS_SIZE               (6UL)
#define ETH_ETHERTYPE_SIZE                   (2UL)
#define ETH_ETNF_ECMR_DM_T1S                 (0x00000000UL)
#define ETH_ETNF_ECMR_DM_RMII                (0x00000002UL)
#define ETH_ETNF_ECMR_RE_TE                  (0x00000060UL)
#define ETH_ETNF_ECMR_TE                     (0x00000020UL)
#define ETH_ETNF_ECMR_RE                     (0x00000040UL)
#define ETH_MACADDR_SIZE                     (6UL)
#define ETH_PHY_DATA_SIZE                    (16UL)
#define ETH_PHY_HEADER_SIZE                  (14UL)
#define ETH_RX_DPTR_OFFSET                   (2UL)
#define ETH_TIMEOUT_COUNT                    ((uint32_t) 63158)
#define ETHER_T1S_SRC_MACADDR_LEN            (6U)
#define ETHER_T1S_TYPECAST_HEADER_TAG        (0x3FFU)
#define ETHER_T1S_TYPECAST_HEADER_DIE        (0xFU)
#define ETHER_T1S_TYPECAST_HEADER_CTRL       (0xFFFU)
#define ETHER_T1S_TYPECAST_HEADER_DS         (0xFFFU)

/* Copy the stream filter address */
#define ETH_COPY_STREAM_ADDRESS(src, dst) \
    do                                    \
    {                                     \
        (dst)[0] = (src)[0];              \
        (dst)[1] = (src)[1];              \
        (dst)[2] = (src)[2];              \
        (dst)[3] = (src)[3];              \
        (dst)[4] = (src)[4];              \
        (dst)[5] = (src)[5];              \
        (dst)[6] = (src)[6];              \
        (dst)[7] = (src)[7];              \
    } while (0)

/* Macros to convert between byte array and the structure */
#define ETHER_PACK_ADDRESS_FROM_8(src, dst)                                                                  \
    do                                                                                                       \
    {                                                                                                        \
        (dst).h32 =                                                                                          \
            ((uint32_t) (src)[0U] << 24UL) | ((uint32_t) (src)[1U] << 16UL) | ((uint32_t) (src)[2U] << 8U) | \
            (uint32_t) (src)[3U];                                                                            \
        (dst).l16 = ((uint32_t) (src)[4U] << 8UL) | (uint32_t) (src)[5U];                                    \
    } while (0)

/* Copy the MAC address */
#define ETH_COPY_MAC_ADDRESS(src, dst) \
    do                                 \
    {                                  \
        (dst)[0] = (src)[0];           \
        (dst)[1] = (src)[1];           \
        (dst)[2] = (src)[2];           \
        (dst)[3] = (src)[3];           \
        (dst)[4] = (src)[4];           \
        (dst)[5] = (src)[5];           \
    } while (0)

#define ETH_UNPACK_ADDRESS_TO_8(src, dst)         \
    do                                            \
    {                                             \
        (dst)[0] = (uint8_t) ((src).h32 >> 24UL); \
        (dst)[1] = (uint8_t) ((src).h32 >> 16UL); \
        (dst)[2] = (uint8_t) ((src).h32 >> 8UL);  \
        (dst)[3] = (uint8_t) (src).h32;           \
        (dst)[4] = (uint8_t) ((src).l16 >> 8U);   \
        (dst)[5] = (uint8_t) (src).l16;           \
    } while (0)

/* Macros to convert between byte array and the structure */
#define ETH_PACK_ADDRESS_FROM_8(src, dst)                                                                    \
    do                                                                                                       \
    {                                                                                                        \
        (dst).h32 =                                                                                          \
            ((uint32_t) (src)[0U] << 24UL) | ((uint32_t) (src)[1U] << 16UL) | ((uint32_t) (src)[2U] << 8U) | \
            (uint32_t) (src)[3U];                                                                            \
        (dst).l16 = ((uint32_t) (src)[4U] << 8UL) | (uint32_t) (src)[5U];                                    \
    } while (0)

#define ETH_TSR_TFFL_MASK              0x00000700UL
#define ETH_AVB_GET_TFFL(val)    (((val) & ETH_TSR_TFFL_MASK) >> 8UL)
#define ETH_AVB_GET_TST(val)     ((uint32_t) (val) >> 16UL)
#define ETH_TFA2_TSV_MASK              0x0000FFFFUL
#define ETH_AVB_TCCR_TFR               0x00000200UL

/***********************************************************************************************************************
**                                              Register address offset                                               **
***********************************************************************************************************************/
#define ETH_CCC                        (0x0000UL)
#define ETH_DBAT                       (0x0004UL)
#define ETH_DLR                        (0x0008UL)
#define ETH_CSR                        (0x000CUL)
#define ETH_ESR                        (0x0088UL)
#define ETH_RIMR                       (0x008CUL)
#define ETH_RCR                        (0x0090UL)
#define ETH_RQC0                       (0x0094UL)
#define ETH_RQC1                       (0x0098UL)
#define ETH_RQC2                       (0x009CUL)
#define ETH_RQC3                       (0x00A0UL)
#define ETH_RQC4                       (0x00A4UL)
#define ETH_RPC                        (0x00B0UL)
#define ETH_UFCV0                      (0x00C4UL)
#define ETH_UFCV1                      (0x00C8UL)
#define ETH_UFCV2                      (0x00CCUL)
#define ETH_UFCV3                      (0x00D0UL)
#define ETH_UFCV4                      (0x00D4UL)
#define ETH_UFCD0                      (0x00E0UL)
#define ETH_UFCD1                      (0x00E4UL)
#define ETH_UFCD2                      (0x00E8UL)
#define ETH_UFCD3                      (0x00ECUL)
#define ETH_UFCD4                      (0x00F0UL)
#define ETH_SFO                        (0x00FCUL)
#define ETH_SFP00                      (0x0100UL)
#define ETH_SFP01                      (0x0104UL)
#define ETH_SFP02                      (0x0108UL)
#define ETH_SFP03                      (0x010CUL)
#define ETH_SFP04                      (0x0110UL)
#define ETH_SFP05                      (0x0114UL)
#define ETH_SFP06                      (0x0118UL)
#define ETH_SFP07                      (0x011CUL)
#define ETH_SFP08                      (0x0120UL)
#define ETH_SFP09                      (0x0124UL)
#define ETH_SFP10                      (0x0128UL)
#define ETH_SFP11                      (0x012CUL)
#define ETH_SFP12                      (0x0130UL)
#define ETH_SFP13                      (0x0134UL)
#define ETH_SFP14                      (0x0138UL)
#define ETH_SFP15                      (0x013CUL)
#define ETH_SFP16                      (0x0140UL)
#define ETH_SFP17                      (0x0144UL)
#define ETH_SFP18                      (0x0148UL)
#define ETH_SFP19                      (0x014CUL)
#define ETH_SFP20                      (0x0150UL)
#define ETH_SFP21                      (0x0154UL)
#define ETH_SFP22                      (0x0158UL)
#define ETH_SFP23                      (0x015CUL)
#define ETH_SFP24                      (0x0160UL)
#define ETH_SFP25                      (0x0164UL)
#define ETH_SFP26                      (0x0168UL)
#define ETH_SFP27                      (0x016CUL)
#define ETH_SFP28                      (0x0170UL)
#define ETH_SFP29                      (0x0174UL)
#define ETH_SFP30                      (0x0178UL)
#define ETH_SFP31                      (0x017CUL)
#define ETH_SFM0                       (0x01C0UL)
#define ETH_SFM1                       (0x01C4UL)

#define ETH_TGC                        (0x0300UL)
#define ETH_TCCR                       (0x0304UL)
#define ETH_TFA0                       (0x0310UL)
#define ETH_TFA1                       (0x0314UL)
#define ETH_TFA2                       (0x0318UL)
#define ETH_CIVR0                      (0x0320UL)
#define ETH_CIVR1                      (0x0324UL)
#define ETH_CDVR0                      (0x0328UL)
#define ETH_CDVR1                      (0x032CUL)
#define ETH_CUL0                       (0x0330UL)
#define ETH_CUL1                       (0x0334UL)
#define ETH_CLL0                       (0x0338UL)
#define ETH_CLL1                       (0x033CUL)

#define ETH_DIC                        (0x0350UL)
#define ETH_DIS                        (0x0354UL)
#define ETH_EIC                        (0x0358UL)
#define ETH_EIS                        (0x035CUL)
#define ETH_RIC0                       (0x0360UL)
#define ETH_RIS0                       (0x0364UL)
#define ETH_RIC2                       (0x0370UL)
#define ETH_TIC                        (0x0378UL)
#define ETH_GCCR                       (0x0390UL)
#define ETH_GTI                        (0x039CUL)
#define ETH_GTO0                       (0x03A0UL)
#define ETH_GTO1                       (0x03A4UL)
#define ETH_GTO2                       (0x03A8UL)
#define ETH_GIC                        (0x03ACUL)
#define ETH_RIC3                       (0x0388UL)

#define ETH_ECMR                       (0x0500UL) /* MAC mode reg                 */
#define ETH_RFLR                       (0x0508UL) /* receive frame length reg     */
#define ETH_ECSIPR                     (0x0518UL) /* MAC interrupt permission reg */
#define ETH_MAHR                       (0x05C0UL) /* MAC address high reg         */
#define ETH_MALR                       (0x05C8UL) /* MAC address low reg          */

#define ETH_DCDC                       (0x0708UL) /* Delay collisions counter           */
#define ETH_CEFCR                      (0x0740UL) /* CRC error frame receive counter    */
#define ETH_FRECR                      (0x0748UL) /* frame receive error counter reg    */
#define ETH_TSFRCR                     (0x0750UL) /* too short frame receive counter    */
#define ETH_TLFRCR                     (0x0758UL) /* too long frame receive counter     */
#define ETH_RFCR                       (0x0760UL) /* residual-bit frame receive counter */
#define ETH_MAFCR                      (0x0778UL) /* mUL)ticast address frame counter    */
#define ETH_SFL_MASK                   (0x0000001FUL)

#define ETH_ETNF_NULL_ADDR             (0x00000000UL)
#define ETH_ETNF_BROADCAST_H32         (0xFFFFFFFFUL)
#define ETH_ETNF_BROADCAST_L16         (0x0000FFFFU)

#define ETH_ETNF_COUNTER_VALUE_INIT    (0x0000U)
#define ETH_ETNF_COUNTER_VALUE_MAX     (0xFFFFU)

/* Peripheral clock value for the ETH Macro in Hz */
#define ETH_PERI_CLOCK_HZ              100000000UL
#define ETH_ETNF_SET_GTI_TIV(val)    ((uint32_t) (val) << 20UL)
#define ETH_ETNF_GCCR_LTI              0x00000008UL

/* Position of bit in T1S control register 0 */
#define ETH_T1SCTL0_PMA_CFG_POS        (24U)
#define ETH_T1SCTL0_MDIO_SHARE_POS     (16U)

/*******************************************************************************************************************//**
 * Private global variables
 ***********************************************************************************************************************/
ether_t1s_controller_status_type_t g_ether_t1s_ctrl_stat[ETHER_T1S_TOTAL_CTRL_CONFIG];
volatile ether_t1s_rx_frame_type_t          g_ether_t1s_rx_frame[ETHER_T1S_TOTAL_CTRL_CONFIG];
volatile ether_t1s_tx_buffer_type_t         gp_ether_t1s_tx_buffer_index[ETHER_T1S_TOTAL_CTRL_CONFIG][ETH_TX_DESC_MAX];
volatile ether_t1s_list_type_t              g_ether_t1s_buffer_lock[ETHER_T1S_TOTAL_CTRL_CONFIG][ETH_TXQ_NUM];
ether_t1s_avb_config_type_t        g_ether_t1s_avb_config[ETHER_T1S_TOTAL_CTRL_CONFIG];
ether_t1s_rx_ch_config_type        g_ether_t1s_rx_config[ETH_RXQ_NUM];
ether_t1s_queue_config_type        g_ether_t1s_queue_config[ETH_RXQ_NUM];
ether_t1s_mem_manager_type       * g_ether_t1s_ram_manager[ETHER_T1S_TOTAL_CTRL_CONFIG];

uint32_t g_ether_rx_buffer_index[ETHER_T1S_TOTAL_CTRL_CONFIG];
const ether_t1s_mac_address_type_t g_ether_t1s_null_addr =
{
    ETH_ETNF_NULL_ADDR,
    ETH_ETNF_NULL_ADDR
};
ether_t1s_mac_address_type_t g_ether_t1s_address_filters[ETHER_T1S_TOTAL_CTRL_CONFIG][ETHER_T1S_MAX_FILTERS];

/* Const values of broadcast and null address to be compared */
const ether_t1s_mac_address_type_t g_ether_t1s_broadcast_addr =
{
    ETH_ETNF_BROADCAST_H32, ETH_ETNF_BROADCAST_L16
};
volatile uint8_t g_ether_t1s_tx_confirmed[ETHER_T1S_TOTAL_CTRL_CONFIG];
volatile ether_t1s_rx_status_type_t g_ether_t1s_rx_status[ETHER_T1S_TOTAL_CTRL_CONFIG];
volatile uint8_t              g_ether_t1s_rx_eth_frame[ETHER_T1S_TOTAL_CTRL_CONFIG][ETHER_T1S_VLFRAME_SIZE];
uint16_t             g_ether_t1s_msg_length[ETHER_T1S_TOTAL_CTRL_CONFIG];
uint8_t              g_ether_t1s_rx_src_addr[ETHER_T1S_TOTAL_CTRL_CONFIG][ETHER_T1S_SRC_MACADDR_LEN];
ether_t1s_frame_type g_ether_t1s_rx_frame_type[ETHER_T1S_TOTAL_CTRL_CONFIG];
uint16_t             g_ether_t1s_rx_frame_cnt[ETHER_T1S_TOTAL_CTRL_CONFIG];
volatile uint16_t             g_ether_t1s_rx_len_byte[ETHER_T1S_TOTAL_CTRL_CONFIG];
uint32_t             g_ether_t1s_common_interrupt[ETHER_T1S_TOTAL_CTRL_CONFIG];
volatile uint32_t             g_ether_t1s_counter_value = ETH_ETNF_COUNTER_VALUE_INIT;
volatile uint32_t             g_ether_t1s_tx_buffer_addr;

#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
ether_t1s_time_stamp_qual_type g_tx_time_qual;
ether_t1s_time_stamp_qual_type g_rx_time_qual;
ether_t1s_time_stamp_type      g_tx_time_stamp[ETHER_T1S_TOTAL_CTRL_CONFIG];
ether_t1s_time_stamp_type      g_rx_time_stamp[ETHER_T1S_TOTAL_CTRL_CONFIG];
#endif

/*******************************************************************************************************************//**
 * Private function prototypes
 **********************************************************************************************************************/
#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
static fsp_err_t r_ether_t1s_open_param_check(ether_t1s_instance_ctrl_t const * const p_ctrl,
                                              ether_cfg_t const * const               p_cfg);

#endif
static void           r_ether_t1s_initialize_buffer(uint32_t ctrl_idx);
static ether_t1s_type r_ether_t1s_hw_init(ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx);
static ether_t1s_type r_ether_t1s_set_controller_mode(ether_t1s_instance_ctrl_t * p_ctrl,
                                                      uint32_t                    ctrl_idx,
                                                      ether_t1s_mode_type_t       ctrl_mode);
static ether_t1s_buf_req_return_type_t r_ether_t1s_get_tx_buffer(ether_t1s_instance_ctrl_t    * p_ctrl,
                                                                 uint8_t                        ctrl_idx,
                                                                 uint8_t                        priority,
                                                                 ether_t1s_buf_idx_type * const p_buf_idx,
                                                                 uint8_t ** const               p_buf,
                                                                 uint16_t * const               p_len_byte);
static ether_t1s_buf_req_return_type_t r_ether_t1s_provide_tx_buffer(ether_t1s_instance_ctrl_t * p_ctrl,
                                                                     uint8_t                     ctrl_idx,
                                                                     uint8_t                     priority,
                                                                     ether_t1s_buf_idx_type    * p_buf_idx,
                                                                     uint8_t                  ** p_buf,
                                                                     uint32_t                  * p_len_byte);
static ether_t1s_type r_ether_t1s_transmit(ether_t1s_instance_ctrl_t * p_ctrl,
                                           uint8_t                     ctrl_idx,
                                           ether_t1s_buf_idx_type      buf_idx,
                                           ether_t1s_frame_type        frame_type,
                                           bool                        tx_confirmation,
                                           uint16_t                    len_byte,
                                           const uint8_t             * p_phys_addr);
static void r_ether_t1s_set_phys_addr(uint8_t ctrl_idx, const uint8_t mac_addr[]);

static void      r_ether_t1s_call_callback(ether_t1s_instance_ctrl_t * p_instance_ctrl, ether_event_t event);
static fsp_err_t r_ether_t1s_send_data(ether_t1s_instance_ctrl_t * p_ctrl, uint8_t * buf, uint32_t len_byte);
static void      r_ether_t1s_tx_confirmation(ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx);
static void      r_ether_t1s_receive(ether_t1s_instance_ctrl_t  * p_ctrl,
                                     uint8_t                      queue_idx,
                                     ether_t1s_rx_status_type_t * p_rx_status);
static uint32_t r_ether_t1s_rx_desc_chain_update(uint32_t                                  ctrl_idx,
                                                 const uint32_t                            desc_ptr,
                                                 const ether_t1s_rx_ch_config_type * const p_ch_config);

static fsp_err_t ether_link_status_check(ether_t1s_instance_ctrl_t const * const p_instance_ctrl);

#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)
static void           r_ether_t1s_clear_all_address_filters(uint32_t ctrl_idx);
static ether_t1s_type r_ether_t1s_update_phys_addr_filter(uint32_t                       ctrl_idx,
                                                          uint8_t                      * p_phys_addr,
                                                          ether_t1s_filter_action_type_t action);
static uint32_t r_ether_t1s_get_filter_index(uint32_t                                   ctrl_idx,
                                             const ether_t1s_mac_address_type_t * const p_mac_addr);
static void r_ether_t1s_add_address_filter(uint32_t                                   ctrl_idx,
                                           const ether_t1s_mac_address_type_t * const p_mac_addr);
static void r_ether_t1s_remove_address_filter(uint32_t ctrl_idx, uint32_t filter_idx);
static bool r_ether_t1s_is_rx_frame_valid(uint32_t                                ctrl_idx,
                                          const ether_t1s_rx_frame_type_t * const p_rx_frame);

#endif
BSP_INTERRUPT_ATTRIBUTE void r_ether_t1s_isr(void);
static void                  r_ether_t1s_disable_icu(ether_t1s_instance_ctrl_t * p_instance_ctrl);
static void                  r_ether_t1s_enable_icu(ether_t1s_instance_ctrl_t * p_instance_ctrl);
static ether_t1s_tick_type   r_ether_t1s_get_time_out_value(ether_t1s_tick_type * time_out_count);

#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
static ether_t1s_type r_ether_t1s_wait_ptp_request_completed(ether_t1s_instance_ctrl_t * const p_ctrl, uint32_t mask);
static ether_t1s_type r_ether_t1s_set_increment_time(ether_t1s_instance_ctrl_t * p_instance_ctrl, uint32_t inc_val);
static ether_t1s_type r_ether_t1s_hw_enable_egress_time_stamp(uint8_t ctrl_idx, uint32_t buf_idx);
static void           r_ether_t1s_hw_get_ingress_time_stamp(uint8_t                          ctrl_idx,
                                                            ether_t1s_data_type            * p_data,
                                                            ether_t1s_time_stamp_qual_type * p_time_qual,
                                                            ether_t1s_time_stamp_type      * p_time_stamp);
static void r_ether_t1s_hw_get_egress_time_stamp(ether_t1s_instance_ctrl_t      * p_instance_ctrl,
                                                 uint32_t                         buf_idx,
                                                 ether_t1s_time_stamp_qual_type * p_time_qual,
                                                 ether_t1s_time_stamp_type      * p_time_stamp);

#endif

/*******************************************************************************************************************//**
 * Global Variables
 **********************************************************************************************************************/

/** ETHER_T1S HAL API mapping for Ethernet Controller interface. */
const ether_api_t g_ether_t1s_on_ether_t1s =
{
    .open           = R_ETHER_T1S_Open,
    .close          = R_ETHER_T1S_Close,
    .write          = R_ETHER_T1S_Write,
    .read           = R_ETHER_T1S_Read,
    .callbackSet    = R_ETHER_T1S_CallbackSet,
    .txStatusGet    = R_ETHER_T1S_TxStatusGet,
    .bufferRelease  = R_ETHER_T1S_BufferRelease,
    .rxBufferUpdate = R_ETHER_T1S_RxBufferUpdate,
    .linkProcess    = R_ETHER_T1S_LinkProcess,
};

/*******************************************************************************************************************//**
 * @addtogroup ETHER_T1S
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Open the ETHER_T1S driver module.
 * Implements ref ether_api_t::open
 * @param[in] p_ctrl                                Pointer to control block.
 * @param[in] p_cfg                                 Pointer to configuration block.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_T1S control block is NULL.
 * @retval  FSP_SUCCESS                             Channel opened successfully.
 * @retval  FSP_ERR_INVALID_POINTER                 Pointer points to invalid memory location
 * @retval  FSP_ERR_ALREADY_OPEN                    Requested channel is already open in a different configuration.
 * @retval  FSP_ERR_INVALID_CHANNEL                 Selected channel does not exist
 **********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_Open (ether_ctrl_t * const p_ctrl, ether_cfg_t const * const p_cfg)
{
    volatile fsp_err_t          err             = FSP_SUCCESS;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
    fsp_err_t phy_ret = FSP_SUCCESS;

    /*Initialize controller index*/
    uint8_t ctrl_idx = ETHER_T1S_INIT_CONTROLLER_INDEX;

#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
    err = r_ether_t1s_open_param_check(p_instance_ctrl, p_cfg);
    FSP_ERROR_RETURN((FSP_SUCCESS == err), err);
#endif
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_cfg->p_extend);
    p_instance_ctrl->p_reg_etnf0      = (R_ETNF0_Type *) R_ETNF0_BASE;
    p_instance_ctrl->p_reg_selb_etnf0 = (R_ETNF0_T1S_Type *) R_ETNF0_T1S_BASE;

    /* Initialize configuration of Ethernet module. */
    p_instance_ctrl->p_ether_cfg = p_cfg;

    /* Initialize the flags */
    p_instance_ctrl->link_establish_status = ETHER_T1S_LINK_ESTABLISH_STATUS_DOWN;
    p_instance_ctrl->link_change           = ETHER_T1S_LINK_CHANGE_NO_CHANGE;
    p_instance_ctrl->previous_link_status  = ETHER_T1S_PREVIOUS_LINK_STATUS_DOWN;

    ETHER_PACK_ADDRESS_FROM_8(p_cfg->p_mac_address, g_ether_t1s_ctrl_stat[ctrl_idx].mac_addr);

    /* Initialize controller state as DOWN */
    g_ether_t1s_ctrl_stat[ctrl_idx].mode = ETHER_T1S_MODE_DOWN;

    /* Initialize address filter */
#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)
    r_ether_t1s_clear_all_address_filters((uint32_t) ctrl_idx);
#endif

    /* Set callback and context pointers, if configured */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    /* Initialize the Ethernet buffer */
    r_ether_t1s_initialize_buffer(ctrl_idx);

    /* Invoke r_ether_t1s_hw_init to initialize the Ethernet Driver */
    r_ether_t1s_hw_init(p_instance_ctrl, ctrl_idx);

    /* Software reset the PHY */
    /* Invoke r_ether_t1s_set_controller_mode to Changes the state of an ETNF controller as ACTIVE */
    (void) r_ether_t1s_set_controller_mode(p_instance_ctrl, (uint32_t) ctrl_idx, ETHER_T1S_MODE_ACTIVE);

    /* Sets the physical source address used by the indexed controller */
    r_ether_t1s_set_phys_addr(ctrl_idx, p_instance_ctrl->p_ether_cfg->p_mac_address);

    /* Configure T1S control register */
    p_instance_ctrl->p_reg_selb_etnf0->T1SCTL0 =
        p_extend->pma_type << ETH_T1SCTL0_PMA_CFG_POS |                   /* Configures the PMA */
            p_extend->mdio_interface_type << ETH_T1SCTL0_MDIO_SHARE_POS | /* Configures the MDIO interface */
            p_extend->mdio_slave_address;                                 /* Address of the MDIO slave module (PHY MDIO address) */

//     /* Software reset the PHY */
//     phy_ret = p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->open(
//         p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_ctrl,
//         p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_cfg);

// #if !ETHER_PHY_CFG_INIT_PHY_LSI_AUTOMATIC

//     /* Initialize the PHY */
//     if (FSP_SUCCESS == phy_ret)
//     {
//         phy_ret = p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_api->chipInit(
//             p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_ctrl,
//             p_instance_ctrl->p_ether_cfg->p_ether_phy_instance->p_cfg);
//     }
// #endif
    if (FSP_SUCCESS == phy_ret)
    {
        /* Start open Ethernet T1S */
        p_instance_ctrl->open = ETHER_T1S_OPEN;
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
#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)
    for (uint8_t index_addr_filter = 0;
         index_addr_filter < p_extend->number_of_addr_filter;
         index_addr_filter++)
    {
        r_ether_t1s_update_phys_addr_filter(ctrl_idx,
                                            &(p_extend->p_mac_address_filter)[index_addr_filter * ETHER_T1S_ADDRESS_SIZE],
                                            p_extend->ether_t1s_filter_action_type);
    }
#endif

    /* Start open Ethernet t1s */
    p_instance_ctrl->open = ETHER_T1S_OPEN;

    return err;
}

#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * @brief Parameter error check function for open.
 * @param[in] p_ctrl                     Pointer to the control block for the channel
 * @param[in] p_cfg                      Pointer to the configuration structure specific to UART mode
 * @retval  FSP_SUCCESS                  No parameter error found
 * @retval  FSP_ERR_INVALID_POINTER      Pointer points to invalid memory location
 * @retval  FSP_ERR_ALREADY_OPEN         Requested channel is already open in a different configuration.
 * @retval  FSP_ERR_INVALID_CHANNEL      Selected channel does not exist
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_T1S control block is NULL.
 **********************************************************************************************************************/
static fsp_err_t r_ether_t1s_open_param_check (ether_t1s_instance_ctrl_t const * const p_ctrl,
                                               ether_cfg_t const * const               p_cfg)
{
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN((NULL != p_cfg), FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN((NULL != p_cfg->p_mac_address), FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN((NULL != p_cfg->p_extend), FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN((ETHER_T1S_OPEN != p_ctrl->open), FSP_ERR_ALREADY_OPEN);
    FSP_ERROR_RETURN((1 > p_cfg->channel), FSP_ERR_INVALID_CHANNEL);
    FSP_ERROR_RETURN((ETHER_T1S_OPEN != p_ctrl->open), FSP_ERR_ALREADY_OPEN);

    return FSP_SUCCESS;
}

#endif

/********************************************************************************************************************//**
 * @brief Disables interrupts. Removes power and releases hardware lock.
 * Implements @ref ether_api_t::close.
 * @param[in] p_ctrl                                    Pointer to the control block for the channel
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_T1S control block is NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_Close (ether_ctrl_t * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;

#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_T1S_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_instance_ctrl->p_ether_cfg->p_extend);
    if (p_extend->ether_t1s_ctl_mode == ETHER_T1S_CTRL_INTERRUPT_MODE)
    {
        /* Disable interrupt */
        r_ether_t1s_disable_icu(p_instance_ctrl);
    }

    (void) r_ether_t1s_set_controller_mode(p_instance_ctrl, (uint32_t) 0UL, ETHER_T1S_MODE_DOWN);
    err = FSP_SUCCESS;
    r_ether_t1s_call_callback(p_ctrl, ETHER_CLOSE_SUCCEED);

    /* Clear configure block parameters. */
    p_instance_ctrl->p_ether_cfg = NULL;

    /* Mark the channel not open so other APIs cannot use it. */
    p_instance_ctrl->open = 0UL;

    return err;
}

/********************************************************************************************************************//**
 * @brief Transmit Ethernet frame. Transmits data from the location specified by the pointer to the transmit
 * buffer, with the data size equal to the specified frame length ether t1s.
 * In the non zero copy mode, transmits data after being copied to the internal buffer.
 * Implements @ref ether_api_t::write.
 * @param[in] p_ctrl                     Pointer to the control block for the channel
 * @param[in] p_buffer                   Pointer to the write buffer
 * @param[in] frame_length               Frame length buffer write data
 * @retval  FSP_SUCCESS                  Processing completed successfully.
 * @retval  FSP_ERR_NOT_OPEN             Requested channel is not configured or API not open.
 * @retval  FSP_ERR_INVALID_POINTER      Pointer points to invalid memory location.
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_T1S control block is NULL.
 * @retval  FSP_ERR_IN_USE               Buffer still in use.
 * @retval  FSP_ERR_INVALID_SIZE         Report error when size of length invalid.
 * @retval  FSP_ERR_ETHER_ERROR_LINK     Auto-negotiation is not completed, and reception is not enabled.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_Write (ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t const frame_length)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;

#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_T1S_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
#endif

    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_instance_ctrl->p_ether_cfg->p_extend);

    if (ETHER_T1S_LOOPBACK_OFF == p_extend->ether_loopback_config)
    {
        /* When the Link up processing is not completed, return error */
        FSP_ERROR_RETURN(ETHER_T1S_LINK_ESTABLISH_STATUS_UP == p_instance_ctrl->link_establish_status,
                         FSP_ERR_ETHER_ERROR_LINK);
    }

    if (ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
    {
        g_ether_t1s_tx_buffer_addr = (uint32_t) p_buffer;
    }

    /* Send data via ethernet*/
    err = r_ether_t1s_send_data(p_instance_ctrl, p_buffer, frame_length);

    return err;
}

/********************************************************************************************************************//**
 * @brief Receive Ethernet frame. Receives data to the location specified by the pointer to the receive buffer.
 * In zero copy mode, the address of the receive buffer is returned.
 * In non zero copy mode, the received data in the internal buffer is copied to the pointer passed by the argument.
 * Implements @ref ether_api_t::read.
 * @param[in]      p_ctrl       Pointer to control structure.
 * @param[in]      p_buffer     Pointer to where to store read data.
 * @param[in,out]  length_bytes Number of bytes in buffer
 * @retval  FSP_SUCCESS                  Processing completed successfully.
 * @retval  FSP_ERR_ETHER_ERROR_NO_DATA  No Data in Receive buffer.
 * @retval  FSP_ERR_INVALID_POINTER      Pointer points to invalid memory location.
 * @retval  FSP_ERR_NOT_OPEN             Requested channel is not configured or API not open.
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_T1S control block is NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_Read (ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t * const length_bytes)
{
    fsp_err_t err = FSP_ERR_ETHER_ERROR_NO_DATA;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
    uint8_t ** pp_read_buffer = (uint8_t **) p_buffer;

    /* Check argument */
#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_T1S_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN(NULL != length_bytes, FSP_ERR_INVALID_POINTER);
#endif
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    /* Get/save the current state of interrupts */
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

    /* Check for received message */
    r_ether_t1s_receive(p_instance_ctrl, ctrl_idx, &g_ether_t1s_rx_status[ctrl_idx]);

    /* Confirm received message */
    if (ETHER_T1S_NOT_RECEIVED != g_ether_t1s_rx_status[ctrl_idx])
    {
        if (ETHER_ZEROCOPY_DISABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
        {
            /* Copy data to user buffer. */
            memcpy((uint8_t *) p_buffer, (uint8_t *) g_ether_t1s_rx_eth_frame[ctrl_idx],
                   g_ether_t1s_rx_len_byte[ctrl_idx]);
        }
        else
        {
            *pp_read_buffer = (uint8_t *) g_ether_t1s_rx_frame[ctrl_idx].frame_addr;
        }

        err = FSP_SUCCESS;

        /* Get data length to user */
        *length_bytes = g_ether_t1s_rx_len_byte[ctrl_idx];
    }
    else
    {
        *length_bytes = 0U;
    }

    /* Restore the interrupt state */
    FSP_CRITICAL_SECTION_EXIT;

    return err;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 * Implements @ref ether_api_t::callbackSet.
 * @param[in] p_ctrl              Pointer to ETHER_T1S control structure.
 * @param[in] p_callback          Callback function pointer input.
 * @param[in] p_context           Context pointer input.
 * @param[in] p_callback_memory   Callback memory pointer input.
 * @retval    FSP_SUCCESS         Callback function is set successfully.
 * @retval    FSP_ERR_NOT_OPEN    The control block has not been opened.
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_T1S control block is NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_CallbackSet (ether_ctrl_t * const          p_ctrl,
                                   void (                      * p_callback)(ether_callback_args_t *),
                                   void * const                  p_context,
                                   ether_callback_args_t * const p_callback_memory)
{
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;

#if ETHER_T1S_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_callback);

    /* Verify this unit has not been initialized */
    FSP_ERROR_RETURN(ETHER_T1S_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/**********************************************************************************************************************//**
 * Provides status of Ethernet driver in the user provided pointer.
 * Implements @ref ether_api_t::txStatusGet.
 * @param[in] p_ctrl                     Pointer to the control block for the channel
 * @param[in] p_buffer_address           Pointer to stored in provided p_buffer_address
 * @retval  FSP_SUCCESS                  Transmit buffer address is stored in provided p_buffer_address.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER      p_status is NULL.
 * @retval  FSP_ERR_NOT_FOUND            The requested item could not be found.
 * @retval  FSP_ERR_ASSERTION            Pointer to ETHER_T1S control block is NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_TxStatusGet (ether_ctrl_t * const p_ctrl, void * const p_buffer_address)
{
    fsp_err_t err = FSP_ERR_NOT_FOUND;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
    ether_t1s_data_desc_type  * p_data_desc     = ETHER_T1S_NULL_PTR;
    uint8_t ** pp_sent_buffer_address           = (uint8_t **) p_buffer_address;
    uint32_t   q_idx    = 0;
    uint8_t    ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;
#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN((uint32_t) ETHER_T1S_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer_address, FSP_ERR_INVALID_POINTER);
#endif

    /* Check if packet is transmited */
    if (g_ether_t1s_tx_confirmed[ctrl_idx] != 0U)
    {
        /* Check the status of the current tx descriptor */
        p_data_desc = (ether_t1s_data_desc_type *) g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_tx_desc[q_idx];

        /* Descriptor is NULL, when no packet transmitted. */
        if (NULL != p_data_desc)
        {
            *pp_sent_buffer_address = (uint8_t *) p_data_desc;

            /* Reset packet confirm status */
            g_ether_t1s_tx_confirmed[ctrl_idx] = 0U;
            err = FSP_SUCCESS;
        }
    }
    else
    {
        ;                              /* No operation */
    }

    return err;
}

/******************************************************************************************************************//**
 * @brief Move to the next buffer in the circular receive buffer list.
 * Implements @ref ether_api_t::bufferRelease.
 * @param[in] p_ctrl                                Pointer to ETHER_T1S control structure.
 * @retval  FSP_SUCCESS                             Processing completed successfully.
 * @retval  FSP_ERR_NOT_OPEN                        The control block has not been opened
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_T1S control block is NULL.
 * @retval  FSP_ERR_ETHER_RECEIVE_BUFFER_ACTIVE     Receive buffer is active.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_BufferRelease (ether_ctrl_t * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
    uint32_t desc_addr;
    const ether_t1s_rx_ch_config_type * p_ch_config;
    ether_t1s_data_desc_type          * p_data_desc;

    /* Check argument */
#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN((uint32_t) ETHER_T1S_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy, FSP_ERR_INVALID_MODE);
#endif
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;
    p_ch_config = &g_ether_t1s_rx_config[ctrl_idx];
    desc_addr   = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_rx_desc_release[p_ch_config->ch_num];
    p_data_desc = (ether_t1s_data_desc_type *) desc_addr;

    /* When receive data exists */
    if (ETHER_T1S_NOT_RECEIVED != g_ether_t1s_rx_status[ctrl_idx])
    {
        if (ETH_DESC_FEMPTY_AVB != p_data_desc->header.dt)
        {
            (void) r_ether_t1s_rx_desc_chain_update(ctrl_idx, desc_addr, p_ch_config);
        }
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
 * @param[in] p_ctrl                                Pointer to the control block for the channel
 * @param[in] p_buffer                              Pointer to the read buffer update data
 * @retval  FSP_SUCCESS                             Processing completed successfully.
 * @retval  FSP_ERR_NOT_OPEN                        Requested channel is not configured or API not open.
 * @retval  FSP_ERR_INVALID_POINTER                 Pointer points to invalid memory location.
 * @retval  FSP_ERR_ETHER_RECEIVE_BUFFER_ACTIVE     Receive buffer is active.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_T1S control block is NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_RxBufferUpdate (ether_ctrl_t * const p_ctrl, void * const p_buffer)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_T1S_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(NULL != p_buffer, FSP_ERR_INVALID_POINTER);
    FSP_ERROR_RETURN(ETHER_ZEROCOPY_ENABLE == p_instance_ctrl->p_ether_cfg->zerocopy, FSP_ERR_INVALID_MODE);
#endif
    uint32_t desc_addr;
    const ether_t1s_rx_ch_config_type * p_ch_config;
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;
    p_ch_config = &g_ether_t1s_rx_config[ctrl_idx];
    desc_addr   = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_rx_desc_update[p_ch_config->ch_num];
    ether_t1s_data_desc_type * p_data_desc = (ether_t1s_data_desc_type *) desc_addr;

    p_data_desc->dptr = (uint32_t) p_buffer;
    p_data_desc++;

    while ((p_data_desc->header.dt == ETH_DESC_LINKFIX) ||
           (p_data_desc->header.dt == ETH_DESC_LINK_AVB))
    {
        p_data_desc = (ether_t1s_data_desc_type *) p_data_desc->dptr;
    }

    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_rx_desc_update[p_ch_config->ch_num] = (uint32_t) p_data_desc;

    return err;
}

/********************************************************************************************************************//**
 * @brief The Link up processing, the Link down processing, and the magic packet detection processing are executed.
 * Implements @ref ether_api_t::linkProcess.
 * @param[in] p_ctrl                                    Pointer to the control block for the channel
 * @retval  FSP_SUCCESS                                 Link is up.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                Initialization of the PHY-LSI failed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_T1S control block is NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_LinkProcess (ether_ctrl_t * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
    ether_t1s_type              result          = ETHER_T1S_OK;

#if (ETHER_T1S_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(ETHER_T1S_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check the state register of the PHY when Initialization PHY in R_ETHER_T1S_Open */
    /* Software reset */
    // err = ether_link_status_check(p_instance_ctrl);

    // /* The state of the link status in PHY-LSI is confirmed and Link Up/Down is judged. */
    // if (FSP_SUCCESS == err)
    // {
    //     /* When becoming Link up */
    //     if (ETHER_T1S_PREVIOUS_LINK_STATUS_DOWN == p_instance_ctrl->previous_link_status)
    //     {
    //         p_instance_ctrl->link_change = ETHER_T1S_LINK_CHANGE_LINK_UP;

    //         /* Update Link status */
    //         p_instance_ctrl->previous_link_status = ETHER_T1S_PREVIOUS_LINK_STATUS_UP;
    //     }
    // }
    // else
    // {
    //     /* When becoming Link down */
    //     if (ETHER_T1S_PREVIOUS_LINK_STATUS_UP == p_instance_ctrl->previous_link_status)
    //     {
    //         p_instance_ctrl->link_change = ETHER_T1S_LINK_CHANGE_LINK_DOWN;

    //         /* Update Link status */
    //         p_instance_ctrl->previous_link_status = ETHER_T1S_PREVIOUS_LINK_STATUS_DOWN;
    //     }
    // }

    /* When the link is up */
    if (ETHER_T1S_LINK_CHANGE_LINK_UP == p_instance_ctrl->link_change)
    {
        p_instance_ctrl->link_change = ETHER_T1S_LINK_CHANGE_NO_CHANGE;
        err = FSP_SUCCESS;
        r_ether_t1s_call_callback(p_instance_ctrl, ETHER_EVENT_LINK_ON);
        p_instance_ctrl->link_establish_status = ETHER_T1S_LINK_ESTABLISH_STATUS_UP;

        /* Configuration MAC address and Link Verification */
        result = r_ether_t1s_set_controller_mode(p_instance_ctrl,
                                                 p_instance_ctrl->p_ether_cfg->channel,
                                                 ETHER_T1S_MODE_ACTIVE);

        if (ETHER_T1S_OK == result)
        {
            err = FSP_SUCCESS;
            r_ether_t1s_call_callback(p_instance_ctrl, ETHER_LINK_VERIFICATION_SUCCEED);
        }
        else
        {
            /* When PHY link verification is not completed */
            p_instance_ctrl->link_establish_status = ETHER_T1S_LINK_ESTABLISH_STATUS_DOWN;
            p_instance_ctrl->link_change           = ETHER_T1S_LINK_CHANGE_LINK_UP;
            err = FSP_ERR_ETHER_PHY_ERROR_LINK;
            r_ether_t1s_call_callback(p_instance_ctrl, ETHER_LINK_VERIFICATION_FAIL);
        }
    }
    else if (ETHER_T1S_LINK_CHANGE_LINK_DOWN == p_instance_ctrl->link_change)
    {
        p_instance_ctrl->link_change           = ETHER_T1S_LINK_CHANGE_NO_CHANGE;
        p_instance_ctrl->link_establish_status = ETHER_T1S_LINK_ESTABLISH_STATUS_DOWN;
        r_ether_t1s_call_callback(p_instance_ctrl, ETHER_EVENT_LINK_OFF);
    }
    else
    {
        ;                              /* no operation */
    }

    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_T1S)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_util_init_linked_list
 * Description                   This Api initialize linked list
 * @param[in] p_list             Linked list
 ***********************************************************************************************************************/
static void r_ether_t1s_util_init_linked_list (ether_t1s_list_type_t * const p_list) {
    if (ETHER_T1S_NULL_PTR != p_list)
    {
        p_list->count  = 0UL;
        p_list->p_last = ETHER_T1S_NULL_PTR;
        p_list->p_head = ETHER_T1S_NULL_PTR;
    }
    else
    {
        /* No action required */
    }
}

#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name          r_ether_t1s_clear_all_address_filters
 * Description            Remove all filters from the filter list and turn off promiscuous mode
 * @param[in] ctrl_idx    Index of the controller
 ***********************************************************************************************************************/
static void r_ether_t1s_clear_all_address_filters (uint32_t ctrl_idx)
{
    g_ether_t1s_ctrl_stat[ctrl_idx].active_filter_bits = 0UL;
    g_ether_t1s_ctrl_stat[ctrl_idx].promiscuous        = ETHER_T1S_FALSE;
}

#endif

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_initialize_buffer
 * Description               Initialize the Tx buffer ring
 * @param[in] ctrl_idx       Index of a Queue
 ***********************************************************************************************************************/
static void r_ether_t1s_initialize_buffer (uint32_t ctrl_idx)
{
    uint16_t cnt;
    ether_t1s_tx_buffer_type_t * volatile p_tx_buffer;

    /* Initialize resource information */
    g_ether_rx_buffer_index[ctrl_idx]                     = 0UL;
    g_ether_t1s_rx_frame[ctrl_idx].frame_addr             = 0UL;
    g_ether_t1s_rx_frame[ctrl_idx].frame_length           = 0UL;
    g_ether_t1s_rx_frame[ctrl_idx].eth_type_addr          = 0UL;
    g_ether_t1s_rx_frame[ctrl_idx].time_stamp.time_stamp0 = 0UL;
    g_ether_t1s_rx_frame[ctrl_idx].time_stamp.time_stamp1 = 0UL;
    g_ether_t1s_rx_frame[ctrl_idx].time_stamp.time_stamp2 = 0UL;
    g_ether_t1s_rx_frame[ctrl_idx].time_qual              = ETHER_T1S_INVALID;

    /* Initialize Tx buffer index list */
    g_ether_t1s_ctrl_stat[ctrl_idx].tx_buf_tail = 0UL;
    for (cnt = 0; cnt < ETH_TX_DESC_MAX; cnt++)
    {
        p_tx_buffer               = (ether_t1s_tx_buffer_type_t *) &gp_ether_t1s_tx_buffer_index[ctrl_idx][cnt];
        p_tx_buffer->p_buffer_hdr = ETHER_T1S_NULL_PTR;
        p_tx_buffer->locked       = ETHER_T1S_FALSE;

#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
        p_tx_buffer->time_qual              = ETHER_T1S_INVALID;
        p_tx_buffer->time_stamp.nanoseconds = (uint32_t) 0U;
        p_tx_buffer->time_stamp.seconds     = (uint32_t) 0U;
        p_tx_buffer->time_stamp.secondsHi   = (uint16_t) 0U;
#endif
    }

    for (cnt = 0; cnt < ETH_TXQ_NUM; cnt++)
    {
        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.buf_tx_cnt[cnt] = 0UL;
        r_ether_t1s_util_init_linked_list(&g_ether_t1s_buffer_lock[ctrl_idx][cnt]);
    }
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_t1s_get_counter_value
 * Description           This API Reads a list with drop counter values of the corresponding controller.
 * @param[in] p_value    Pointer to value check counter
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_get_counter_value (ether_t1s_tick_ref_type * p_value)
{
    /* Reset counter if it reaches the max value */
    if (g_ether_t1s_counter_value == ETH_ETNF_COUNTER_VALUE_MAX)
    {
        g_ether_t1s_counter_value = ETH_ETNF_COUNTER_VALUE_INIT;
    }

    /* Increment counter */
    g_ether_t1s_counter_value++;

    /* Output the updated value */
    *p_value = g_ether_t1s_counter_value;

    return ETHER_T1S_OK;
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_get_time_out_value
 * Description                       This returns the elapsed timeout value of  Os timer
 * @param[in] time_out_count         Update counter init value
 ***********************************************************************************************************************/
static ether_t1s_tick_type r_ether_t1s_get_time_out_value (ether_t1s_tick_type * time_out_count)
{
    ether_t1s_tick_type time_out_count_curr = 0;
    ether_t1s_tick_type time_out_count_elapsed;
    ether_t1s_tick_type time_out_count_init = *time_out_count;

    /* Get the current tick value from OS Counter */
    (void) r_ether_t1s_get_counter_value(&time_out_count_curr);
    if (time_out_count_curr >= time_out_count_init)
    {
        time_out_count_elapsed = time_out_count_curr - time_out_count_init;
    }
    else
    {
        time_out_count_elapsed = ((uint32_t) ETHER_T1S_COUNTER_MAX_VALUE - time_out_count_init) + time_out_count_curr;
    }

    *time_out_count = time_out_count_curr;

    return time_out_count_elapsed;
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_hw_etnf_operation_mode_change
 * Description                       To change Operating Mode.
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] ctrl_idx               Index of a Queue
 * @param[in] len_mode               Mode to change
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_hw_etnf_operation_mode_change (ether_t1s_instance_ctrl_t       * p_ctrl,
                                                                 uint32_t                          ctrl_idx,
                                                                 ether_t1s_operation_status_type_t len_mode)
{
    ether_t1s_type      result;
    uint32_t            reg_val;
    ether_t1s_tick_type time_tut_count_init;
    ether_t1s_tick_type time_out_count_elap;

    time_tut_count_init = 0UL;
    result              = ETHER_T1S_OK;
    if (g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.dev_status != len_mode)
    {
        reg_val = p_ctrl->p_reg_etnf0->CCC & ~ETH_ETNF_CCC_OPC_MASK;
        p_ctrl->p_reg_etnf0->CCC = reg_val | (uint32_t) len_mode;

        (void) r_ether_t1s_get_counter_value(&time_tut_count_init);
        do
        {
            time_out_count_elap = r_ether_t1s_get_time_out_value(&time_tut_count_init);
            reg_val             = p_ctrl->p_reg_etnf0->CSR & ETH_ETNF_CSR_OPS;
        } while ((reg_val != (1UL << (uint32_t) len_mode)) && (time_out_count_elap <= ETHER_T1S_TIMEOUT_COUNT));

        if ((1UL << (uint32_t) len_mode) == reg_val)
        {
            /* No Time-out - mode changed  */
            g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.dev_status = len_mode;
            result = ETHER_T1S_OK;
        }
        else
        {
            /* Return Error - as mode not changed */
            result = ETHER_T1S_NOT_OK;
        }
    }
    else
    {
        /* Controller is already in the requested state */
        result = ETHER_T1S_OK;
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_hw_etnf_dmac_struct_config
 * Description                       This Initializes and enables Ethernet peripheral.
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] ctrl_idx               Index of a Queue
 ***********************************************************************************************************************/
static void r_ether_t1s_hw_etnf_dmac_struct_config (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    uint32_t index;
    uint8_t  q_idx;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    ether_t1s_extended_cfg_t   * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    /* AVB part */
    g_ether_t1s_avb_config[ctrl_idx].rx_config.encf = lp_hw_unit_config->rx_config.encf;
    g_ether_t1s_avb_config[ctrl_idx].rx_config.esf  = lp_hw_unit_config->rx_config.esf;
    g_ether_t1s_avb_config[ctrl_idx].rx_config.ets0 = lp_hw_unit_config->rx_config.ets0;
    g_ether_t1s_avb_config[ctrl_idx].rx_config.ets2 = lp_hw_unit_config->rx_config.ets2;
    g_ether_t1s_avb_config[ctrl_idx].rx_config.rfcl = ETH_RX_FIFO_CRIT_LVL;
#if (ETHER_T1S_CFG_STREAM_FILTERING)
    g_ether_t1s_avb_config[ctrl_idx].rx_config.srp_talker_filtering = lp_hw_unit_config->rx_config.srp_talker_filtering;
#endif
    g_ether_t1s_avb_config[ctrl_idx].tx_config.tsm0 = ETHER_T1S_TXNORMAL;
    g_ether_t1s_avb_config[ctrl_idx].tx_config.tsm1 = ETHER_T1S_TXNORMAL;
    g_ether_t1s_avb_config[ctrl_idx].tx_config.tsm2 = ETHER_T1S_TXNORMAL;
    g_ether_t1s_avb_config[ctrl_idx].tx_config.tsm3 = ETHER_T1S_TXNORMAL;
    g_ether_t1s_avb_config[ctrl_idx].tx_config.ecbs = lp_hw_unit_config->ecbs;
    g_ether_t1s_avb_config[ctrl_idx].tx_config.tqp  = lp_hw_unit_config->tx_config;

    /* Read the number of Rx Queue Configured */
    for (index = 0UL; index < (uint32_t) lp_hw_unit_config->queue_config.number_of_rx_queue; index++)
    {
        q_idx = lp_hw_unit_config->queue_config.p_rx_queue_config[index].eth_rx_queue_id;

        g_ether_t1s_rx_config[q_idx].ch_num = q_idx;

        if (ETH_BECHANNEL == g_ether_t1s_rx_config[q_idx].ch_num)
        {
            /* This is Rx BE */
            g_ether_t1s_rx_config[q_idx].ch_type = ETHER_T1S_RX_BE;
        }
        else if (ETH_NCCHANNEL == g_ether_t1s_rx_config[q_idx].ch_num)
        {
            /* This is Rx NC */
            g_ether_t1s_rx_config[q_idx].ch_type = ETHER_T1S_RX_NC;
        }
        else
        {
            /* Rx Stream */
            g_ether_t1s_rx_config[q_idx].ch_type = ETHER_T1S_RX_S;
        }

        /* Queue Configuration */
        g_ether_t1s_queue_config[index].pia  = ETHER_T1S_GAP32;
        g_ether_t1s_queue_config[index].ufcc = ETHER_T1S_UFCC0;
        g_ether_t1s_queue_config[index].rsm  = ETHER_T1S_RXNORMAL;

#if (ETHER_T1S_CFG_STREAM_FILTERING)

        /* Save Pattern Address if Filtering is enabled */
        ETH_COPY_STREAM_ADDRESS(lp_hw_unit_config->queue_config.p_rx_queue_config[index].eth_pattern_stream,
                                g_ether_t1s_rx_config[q_idx].pattern_stream);
#endif
    }
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_tx_rx_config
 * Description                       Tx Rx parameters Config
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] ctrl_idx               Index of a Queue
 * @param[in] p_tx_config            Tx Config Pointer
 * @param[in] p_rx_config            Rx Config Pointer
 ***********************************************************************************************************************/
static void r_ether_t1s_tx_rx_config (ether_t1s_instance_ctrl_t                 * p_ctrl,
                                      uint32_t                                    ctrl_idx,
                                      const ether_t1s_tx_config_type_etnf * const p_tx_config,
                                      const ether_t1s_rx_config_type_etnf * const p_rx_config)
{
    uint32_t reg_val;
    reg_val = (uint32_t) (((uint32_t) p_rx_config->encf << ETH_ETNF_RCR_ENCF_SHIFT) |
                          ((uint32_t) p_rx_config->esf << ETH_ETNF_RCR_ESF_SHIFT) |
                          ((uint32_t) p_rx_config->ets0 << ETH_ETNF_RCR_ETS0_SHIFT) |
                          ((uint32_t) p_rx_config->ets2 << ETH_ETNF_RCR_ETS2_SHIFT) |
                          ((uint32_t) p_rx_config->rfcl << ETH_ETNF_RCR_RFCL_SHIFT));

    p_ctrl->p_reg_etnf0->RCR = reg_val;

    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_be_time_stamp = p_rx_config->ets0;
    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_s_time_stamp  = p_rx_config->ets2;
    reg_val = (uint32_t) (((uint32_t) p_tx_config->tsm0) |
                          ((uint32_t) p_tx_config->tsm1 << ETH_ETNF_TGC_TSM1_SHIFT) |
                          ((uint32_t) p_tx_config->tsm2 << ETH_ETNF_TGC_TSM2_SHIFT) |
                          ((uint32_t) p_tx_config->tsm3 << ETH_ETNF_TGC_TSM3_SHIFT) |
                          ((uint32_t) p_tx_config->ecbs << ETH_ETNF_TGC_TQP0_SHIFT) |
                          ((uint32_t) p_tx_config->tqp << ETH_ETNF_TGC_TQP1_SHIFT) |
                          ETH_ETNF_TGC_TBDn_VALUE);

    p_ctrl->p_reg_etnf0->TGC = reg_val;
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_tx_rx_int_config
 * Description                       Tx Rx parameters Config
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 ***********************************************************************************************************************/
static void r_ether_t1s_tx_rx_int_config (ether_t1s_instance_ctrl_t * p_ctrl)
{
    uint8_t index;
    uint8_t q_idx;
    uint8_t bit_no;

    ether_t1s_etnf_config_type * lp_hw_unit_config;
    ether_t1s_extended_cfg_t   * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;
    if (ETHER_T1S_CTRL_INTERRUPT_MODE == p_extend->ether_t1s_cfg_type_rx_isr)
    {
        /* set RIC0 interrupt */
        for (index = 0U; index < lp_hw_unit_config->queue_config.number_of_rx_queue; index++)
        {
            bit_no = lp_hw_unit_config->queue_config.p_rx_queue_config[index].eth_rx_queue_id;
            p_ctrl->p_reg_etnf0->RIC0 |= (1UL << bit_no);
        }
    }
    else
    {
        /* No action required */
    }

    if (ETHER_T1S_CTRL_INTERRUPT_MODE == p_extend->ether_t1s_cfg_type_tx_isr)
    {
        /* set descriptor interrupt */
        for (index = 0U; index < lp_hw_unit_config->queue_config.number_of_tx_queue; index++)
        {
            /* Get Tx queue id */
            q_idx = lp_hw_unit_config->queue_config.p_tx_queue_config[index].eth_tx_queue_id;

            /* Enable descriptor interrupt
             * Queue n (n = 0..3): corresponding to descriptor interrupt (n + 1) */
            p_ctrl->p_reg_etnf0->DIC |= (1UL << (q_idx + 1U));
        }
    }
    else
    {
        /* No action required */
    }

#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
    p_ctrl->p_reg_etnf0->TIC = ETH_TIC_TFUE;
#endif
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_rx_queue_set
 * Description                       Set Receive Queue
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] ctrl_idx               Index of a Queue
 ***********************************************************************************************************************/
static void r_ether_t1s_rx_queue_set (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    uint32_t index;
    uint32_t q_index;
    uint32_t config;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    uint32_t rqci;
    (void) ctrl_idx;
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    for (index = 0UL; index < (uint32_t) lp_hw_unit_config->queue_config.number_of_rx_queue; index++)
    {
        q_index = (uint32_t) lp_hw_unit_config->queue_config.p_rx_queue_config[index].eth_rx_queue_id;

        config = (uint32_t) (ETH_ETNF_SET_RQCi_RSMr(g_ether_t1s_queue_config[index].rsm, q_index) |
                             ETH_ETNF_SET_RQCi_UFCCr(g_ether_t1s_queue_config[index].ufcc, q_index) |
                             ETH_ETNF_SET_RQCi_PIAr(g_ether_t1s_queue_config[index].pia, q_index));
        rqci = ETH_ETNF_GET_RQCi(q_index);
        switch (rqci)
        {
            case 0:
            {
                p_ctrl->p_reg_etnf0->RQC0 = config;
                break;
            }

            case 1:
            {
                p_ctrl->p_reg_etnf0->RQC1 = config;
                break;
            }

            case 2:
            {
                p_ctrl->p_reg_etnf0->RQC2 = config;
                break;
            }

            case 3:
            {
                p_ctrl->p_reg_etnf0->RQC3 = config;
                break;
            }

            case 4:
            {
                p_ctrl->p_reg_etnf0->RQC4 = config;
                break;
            }

            default:
            {
                break;
            }
        }
    }
}

#if (ETHER_T1S_CFG_STREAM_FILTERING)

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_write_into_sfp_reg
 * Description                       Write In corresponding SFPi the passed address (for Rx Filter)
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] ctrl_idx               Index of a Queue
 * @param[in] p_mac_addr_ptr         Pointer to the address to be stored
 * @param[in] queue_idx              In case of clear this is the index of the SFP register minus offset
 ***********************************************************************************************************************/
static void r_ether_t1s_write_into_sfp_reg (ether_t1s_instance_ctrl_t * p_ctrl,
                                            uint32_t                    ctrl_idx,
                                            const uint8_t * const       p_mac_addr_ptr,
                                            const uint8_t               queue_idx)
{
    uint32_t val_high;
    uint32_t val_low;
    uint32_t val;
    uint8_t  sfp_idx;
    uint32_t low_sfpi;

    /************ Little Endian Core **************/
    sfp_idx = queue_idx - ETH_RX_QUEUE_INDEX_AVAILABLE_FILTER;

    /* Write into SFP1,3,...31 */
    if (ETH_ENABLE == g_ether_t1s_avb_config[ctrl_idx].rx_config.srp_talker_filtering)
    {
        val_high = ((uint32_t) p_mac_addr_ptr[5] << 8U) | ((uint32_t) p_mac_addr_ptr[4]);
    }
    else
    {
        val_high = ((uint32_t) p_mac_addr_ptr[7] << 24U) | ((uint32_t) p_mac_addr_ptr[6] << 16U) |
                   ((uint32_t) p_mac_addr_ptr[5] << 8U) | ((uint32_t) p_mac_addr_ptr[4]);
    }

    /* Write into SFP0,2,..30 */
    val_low = ((uint32_t) p_mac_addr_ptr[3] << 24U) | ((uint32_t) p_mac_addr_ptr[2] << 16U) |
              ((uint32_t) p_mac_addr_ptr[1] << 8U) | ((uint32_t) p_mac_addr_ptr[0]);

    /* Read Opc */
    if (ETH_OPERATION == g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.dev_status)
    {
        p_ctrl->p_reg_etnf0->SFV0 = val_low;
        p_ctrl->p_reg_etnf0->SFV1 = val_high;

        /* Check it can be updated */

        do
        {
            val = p_ctrl->p_reg_etnf0->SFL;
        } while (ETH_NO_LOAD_REQ != (val & ETH_SFL_MASK));

        /* SFP update request */
        val = (uint32_t) sfp_idx;
        p_ctrl->p_reg_etnf0->SFL = val;
    }
    else
    {
        low_sfpi = ETH_ETNF_GET_LOW_SFPi(sfp_idx);

        switch (low_sfpi)
        {
            case 0:
            {
                p_ctrl->p_reg_etnf0->SFP0 = val_low;
                p_ctrl->p_reg_etnf0->SFP1 = val_high;
                break;
            }

            case 1:
            {
                p_ctrl->p_reg_etnf0->SFP1 = val_low;
                p_ctrl->p_reg_etnf0->SFP2 = val_high;
                break;
            }

            case 2:
            {
                p_ctrl->p_reg_etnf0->SFP2 = val_low;
                p_ctrl->p_reg_etnf0->SFP3 = val_high;
                break;
            }

            case 3:
            {
                p_ctrl->p_reg_etnf0->SFP3 = val_low;
                p_ctrl->p_reg_etnf0->SFP4 = val_high;
                break;
            }

            case 4:
            {
                p_ctrl->p_reg_etnf0->SFP4 = val_low;
                p_ctrl->p_reg_etnf0->SFP5 = val_high;
                break;
            }

            case 5:
            {
                p_ctrl->p_reg_etnf0->SFP5 = val_low;
                p_ctrl->p_reg_etnf0->SFP6 = val_high;
                break;
            }

            case 6:
            {
                p_ctrl->p_reg_etnf0->SFP6 = val_low;
                p_ctrl->p_reg_etnf0->SFP7 = val_high;
                break;
            }

            case 7:
            {
                p_ctrl->p_reg_etnf0->SFP7 = val_low;
                p_ctrl->p_reg_etnf0->SFP8 = val_high;
                break;
            }

            case 8:
            {
                p_ctrl->p_reg_etnf0->SFP8 = val_low;
                p_ctrl->p_reg_etnf0->SFP9 = val_high;
                break;
            }

            case 9:
            {
                p_ctrl->p_reg_etnf0->SFP9  = val_low;
                p_ctrl->p_reg_etnf0->SFP10 = val_high;
                break;
            }

            case 10:
            {
                p_ctrl->p_reg_etnf0->SFP10 = val_low;
                p_ctrl->p_reg_etnf0->SFP11 = val_high;
                break;
            }

            case 11:
            {
                p_ctrl->p_reg_etnf0->SFP11 = val_low;
                p_ctrl->p_reg_etnf0->SFP12 = val_high;
                break;
            }

            case 12:
            {
                p_ctrl->p_reg_etnf0->SFP12 = val_low;
                p_ctrl->p_reg_etnf0->SFP13 = val_high;
                break;
            }

            case 13:
            {
                p_ctrl->p_reg_etnf0->SFP13 = val_low;
                p_ctrl->p_reg_etnf0->SFP14 = val_high;
                break;
            }

            case 14:
            {
                p_ctrl->p_reg_etnf0->SFP14 = val_low;
                p_ctrl->p_reg_etnf0->SFP15 = val_high;
                break;
            }

            case 15:
            {
                p_ctrl->p_reg_etnf0->SFP15 = val_low;
                p_ctrl->p_reg_etnf0->SFP16 = val_high;
                break;
            }

            case 16:
            {
                p_ctrl->p_reg_etnf0->SFP16 = val_low;
                p_ctrl->p_reg_etnf0->SFP17 = val_high;
                break;
            }

            case 17:
            {
                p_ctrl->p_reg_etnf0->SFP17 = val_low;
                p_ctrl->p_reg_etnf0->SFP18 = val_high;
                break;
            }

            case 18:
            {
                p_ctrl->p_reg_etnf0->SFP18 = val_low;
                p_ctrl->p_reg_etnf0->SFP19 = val_high;
                break;
            }

            case 19:
            {
                p_ctrl->p_reg_etnf0->SFP19 = val_low;
                p_ctrl->p_reg_etnf0->SFP20 = val_high;
                break;
            }

            case 20:
            {
                p_ctrl->p_reg_etnf0->SFP20 = val_low;
                p_ctrl->p_reg_etnf0->SFP21 = val_high;
                break;
            }

            case 21:
            {
                p_ctrl->p_reg_etnf0->SFP21 = val_low;
                p_ctrl->p_reg_etnf0->SFP22 = val_high;
                break;
            }

            case 22:
            {
                p_ctrl->p_reg_etnf0->SFP22 = val_low;
                p_ctrl->p_reg_etnf0->SFP23 = val_high;
                break;
            }

            case 23:
            {
                p_ctrl->p_reg_etnf0->SFP23 = val_low;
                p_ctrl->p_reg_etnf0->SFP24 = val_high;
                break;
            }

            case 24:
            {
                p_ctrl->p_reg_etnf0->SFP24 = val_low;
                p_ctrl->p_reg_etnf0->SFP25 = val_high;
                break;
            }

            case 25:
            {
                p_ctrl->p_reg_etnf0->SFP25 = val_low;
                p_ctrl->p_reg_etnf0->SFP26 = val_high;
                break;
            }

            case 26:
            {
                p_ctrl->p_reg_etnf0->SFP26 = val_low;
                p_ctrl->p_reg_etnf0->SFP27 = val_high;
                break;
            }

            case 27:
            {
                p_ctrl->p_reg_etnf0->SFP27 = val_low;
                p_ctrl->p_reg_etnf0->SFP28 = val_high;
                break;
            }

            case 28:
            {
                p_ctrl->p_reg_etnf0->SFP28 = val_low;
                p_ctrl->p_reg_etnf0->SFP29 = val_high;
                break;
            }

            case 29:
            {
                p_ctrl->p_reg_etnf0->SFP29 = val_low;
                p_ctrl->p_reg_etnf0->SFP30 = val_high;
                break;
            }

            default:
            {
                p_ctrl->p_reg_etnf0->SFP30 = val_low;
                p_ctrl->p_reg_etnf0->SFP31 = val_high;
                break;
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_set_rx_filter
 * Description                       Configure SFO, SFM, SFP
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] ctrl_idx               Index of a Queue
 ***********************************************************************************************************************/
static void r_ether_t1s_set_rx_filter (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
 #if (ETHER_T1S_CFG_STREAM_FILTERING)
    uint8_t number_of_rx_queue;
    uint8_t count;
    uint8_t q_index;
 #endif
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    ether_t1s_extended_cfg_t   * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config        = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;
    p_ctrl->p_reg_etnf0->SFO = ETH_ETNF_SFO_FBP_VALUE;
    if (ETH_ENABLE == g_ether_t1s_avb_config[ctrl_idx].rx_config.srp_talker_filtering)
    {
        p_ctrl->p_reg_etnf0->SFM0 = ETH_ETNF_SFM0_TALKERFILTER;
        p_ctrl->p_reg_etnf0->SFM1 = ETH_ETNF_SFM1_TALKERFILTER;
    }
    else
    {
        p_ctrl->p_reg_etnf0->SFM0 = ETH_ETNF_SFM0_LISNERFILTER;
        p_ctrl->p_reg_etnf0->SFM1 = ETH_ETNF_SFM1_LISNERFILTER;
    }

 #if (ETHER_T1S_CFG_STREAM_FILTERING)

    /* Get number of configured queues */
    number_of_rx_queue = lp_hw_unit_config->queue_config.number_of_rx_queue;
    for (count = 0U; count < number_of_rx_queue; count++)
    {
        q_index = lp_hw_unit_config->queue_config.p_rx_queue_config[count].eth_rx_queue_id;

        /* If Rx Queue is Stream set corresponding SPFI register */
        if (ETH_NCCHANNEL < q_index)
        {
            /* Write into SFP Register */
            r_ether_t1s_write_into_sfp_reg(p_ctrl, ctrl_idx, g_ether_t1s_rx_config[count].pattern_stream, q_index);
        }
        else
        {
            /* NO action as this is not a RX Stream Queue */
        }
    }
 #endif
}

#endif

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_set_cbs_parameter
 * Description                       Set Tx queues configured
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] ctrl_idx               Index of a Queue
 ***********************************************************************************************************************/
#if (ETHER_T1S_CFG_QOS_SUPPORT)
static void r_ether_t1s_set_cbs_parameter (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    uint32_t q_idx;
    uint32_t civ_tmp;
    uint32_t cdv_tmp;                  /* negative number represented in 2'complement */
    (void) ctrl_idx;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    ether_t1s_extended_cfg_t   * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    /* loop through all Tx Queues configured */
    q_idx = 0UL;
    do
    {
        if (((uint8_t) (ETH_TX_SB) == lp_hw_unit_config->queue_config.p_tx_queue_config[q_idx].eth_tx_queue_id) &&
            (ETH_CBS == lp_hw_unit_config->queue_config.p_tx_queue_config[q_idx].tx_queue_shaper.tx_q_policy))
        {
            /* Q2 */
            civ_tmp = (uint32_t) (lp_hw_unit_config->queue_config.p_tx_queue_config[q_idx].tx_queue_shaper.civ);

            cdv_tmp = (uint32_t) (lp_hw_unit_config->queue_config.p_tx_queue_config[q_idx].tx_queue_shaper.cdv);

            /* Write into CIVR0 , CDVR0 */
            p_ctrl->p_reg_etnf0->CIVR0 = civ_tmp;
            p_ctrl->p_reg_etnf0->CDVR0 = cdv_tmp;

            /* Set Upper Limit => 65535 */
            p_ctrl->p_reg_etnf0->CUL0 = ETH_MAX_CIV;

            /* Set Lower Limit => -65536 */
            p_ctrl->p_reg_etnf0->CLL0 = ETH_MIN_CDV;
        }
        else if (((uint8_t) (ETH_TX_SA) == lp_hw_unit_config->queue_config.p_tx_queue_config[q_idx].eth_tx_queue_id) &&
                 (ETH_CBS == lp_hw_unit_config->queue_config.p_tx_queue_config[q_idx].tx_queue_shaper.tx_q_policy))
        {
            /* Q3 */
            civ_tmp = (uint32_t) (lp_hw_unit_config->queue_config.p_tx_queue_config[q_idx].tx_queue_shaper.civ);

            cdv_tmp = (uint32_t) (lp_hw_unit_config->queue_config.p_tx_queue_config[q_idx].tx_queue_shaper.cdv);

            /* Write into CIVR1 , CDVR1 */
            p_ctrl->p_reg_etnf0->CIVR1 = civ_tmp;
            p_ctrl->p_reg_etnf0->CDVR1 = cdv_tmp;

            /* Set Upper Limit => 65535 */
            p_ctrl->p_reg_etnf0->CUL1 = ETH_MAX_CIV;

            /* Set Lower Limit => -65536 */
            p_ctrl->p_reg_etnf0->CLL1 = ETH_MIN_CDV;
        }
        else
        {
            /* Nothing to set for this Tx Queue */
        }

        q_idx++;
    } while (q_idx < (uint32_t) lp_hw_unit_config->queue_config.number_of_tx_queue);
}

#endif

/*******************************************************************************************************************//**
 * Function Name                     r_ether_t1s_hw_etnf_dmac_init
 * Description                       Initialize DMAC
 * @param[in] p_ctrl                 Pointer to the control block for the channel
 * @param[in] ctrl_idx               Index of a Queue
 ***********************************************************************************************************************/
static void r_ether_t1s_hw_etnf_dmac_init (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);

    /* configure CCC register */
    if (ETHER_T1S_LOOPBACK_ON == p_extend->ether_loopback_config)
    {
        p_ctrl->p_reg_etnf0->CCC |= ETH_ETNF_CCC_LBME;
    }
    else
    {
        /* No action required */
    }

    /* configure Error interrupt mask - EIC register */
    p_ctrl->p_reg_etnf0->EIC = ETH_ETNF_EIC_ERROR;

    /* configure reception part - RCR register */
    /* configure transmission part - TGC register */
    r_ether_t1s_tx_rx_config((ether_t1s_instance_ctrl_t *) p_ctrl,
                             (uint32_t) ctrl_idx,
                             &(g_ether_t1s_avb_config[ctrl_idx].tx_config),
                             &(g_ether_t1s_avb_config[ctrl_idx].rx_config));

    r_ether_t1s_tx_rx_int_config((ether_t1s_instance_ctrl_t *) p_ctrl);

#if (ETHER_T1S_CFG_QOS_SUPPORT)

    /* Set CBS Counter parameters (CIV and CDV)  */
    r_ether_t1s_set_cbs_parameter((ether_t1s_instance_ctrl_t *) p_ctrl, (uint32_t) ctrl_idx);
#endif

    /* set GIC interrupt */
    p_ctrl->p_reg_etnf0->GIC = 0UL;

    /* Set RQCi register */
    r_ether_t1s_rx_queue_set((ether_t1s_instance_ctrl_t *) p_ctrl, (uint32_t) ctrl_idx);

    /* Set padding to zero - Padding Not used */
    p_ctrl->p_reg_etnf0->RPC = 0UL;

    /* Set Common RX Filter if enabled */
#if (ETHER_T1S_CFG_STREAM_FILTERING)
    r_ether_t1s_set_rx_filter(p_ctrl, ctrl_idx);
#endif
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_hw_init
 * Description               Initialize a ETNF HWUnit
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of a Queue
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_hw_init (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_t1s_type      result;
    uint32_t            reg_val;
    ether_t1s_tick_type time_tut_count_init;
    ether_t1s_tick_type time_out_count_elap;

    result = ETHER_T1S_OK;
    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.dev_status = ETHER_T1S_UNINIT;
    reg_val = p_ctrl->p_reg_etnf0->CCC & ETH_ETNF_CCC_OPC_MASK;
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    if (ETH_ETNF_CCC_OPC_OPERATION == reg_val)
    {
        p_ctrl->p_reg_etnf0->CCC |= ETH_ETNF_CCC_DTSR;
        time_tut_count_init       = 0UL;
        (void) r_ether_t1s_get_counter_value(&time_tut_count_init);
        do
        {
            time_out_count_elap = r_ether_t1s_get_time_out_value(&time_tut_count_init);
            reg_val             = p_ctrl->p_reg_etnf0->CSR & ETH_ETNF_CSR_DTS;
        } while ((reg_val != ETH_ETNF_CSR_DTS) && (time_out_count_elap <= ETHER_T1S_TIMEOUT_COUNT));
    }
    else
    {
        /* HW UM does not require anything for transition from Operation mode to other than Reset mode. */
    }

    {
        /* Initialize AVB DMAC Configuration Structure */
        r_ether_t1s_hw_etnf_dmac_struct_config(p_ctrl, ctrl_idx);

        /* Initialize Statistics */
#if ((ETHER_T1S_CFG_GET_RX_STATS_API) || (ETHER_T1S_CFG_GET_TX_STATS_API))
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.stats_broad_cast_pkts            = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts                    = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_octets                  = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_crc_align_errors        = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_under_size_pkts         = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_over_size_pkts          = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts64_octets           = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts65_to_127_octets    = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts128_to_255_octets   = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts256_to_511_octets   = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts512_to_1023_octets  = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts1024_to_1518_octets = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_unicast_frames                = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.tx_number_of_octets              = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.tx_nu_cast_pkts                  = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.tx_uni_cast_pkts                 = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.drop_pkt_crc   = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.under_size_pkt = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.over_size_pkt  = 0UL;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.algnmt_err     = 0UL;
#endif

        /* Change mode to ETH_RESET mode */
        (void) r_ether_t1s_hw_etnf_operation_mode_change(p_ctrl, ctrl_idx, ETHER_T1S_RESET);

        /* Change mode to ETH_CONFIG mode */
        result = r_ether_t1s_hw_etnf_operation_mode_change(p_ctrl, ctrl_idx, ETHER_T1S_CONFIG);

        if (ETHER_T1S_OK == result)
        {
            /* configure MAC */
            /* Set MAC address */
            /************ Little Endian Core **************/
            /* Write into MALR */
            p_ctrl->p_reg_etnf0->MALR = g_ether_t1s_ctrl_stat[ctrl_idx].mac_addr.l16;

            /* Write into MAHR */
            p_ctrl->p_reg_etnf0->MAHR = g_ether_t1s_ctrl_stat[ctrl_idx].mac_addr.h32;

            /*********** Big Endian ***********************/
            /* V2H */

            /* Set the receive frame length including the FCS length */
            p_ctrl->p_reg_etnf0->RFLR = (uint32_t) (ETH_RX_BUF_LENGTH + ETH_FCS_LENGTH);

            /* set MAC interrupt */
            p_ctrl->p_reg_etnf0->ECSIPR = 0UL;

            /* for RMII speed setting */
            if (ETHER_T1S_RMII == p_extend->ether_phy_interface)
            {
                p_ctrl->p_reg_etnf0->RIMR = (uint32_t) p_extend->ether_network_speed;
            }
            else
            {
                /* No action required */
            }

            /* configure DMAC */
            r_ether_t1s_hw_etnf_dmac_init(p_ctrl, ctrl_idx);
            if (p_extend->ether_t1s_ctl_mode == ETHER_T1S_CTRL_INTERRUPT_MODE)
            {
                /* configure interrupt */
                r_ether_t1s_enable_icu(p_ctrl);
            }

#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)

            /* Select the peripheral bus clock for gPTP clock */
            p_ctrl->p_reg_etnf0->CCC |= ETH_ETNF_CCC_CSEL_PTP_CLK_SRC;

            /* enable Time Stamping */
            p_ctrl->p_reg_etnf0->TCCR |= ETH_ETNF_TCCR_TFEN;
#endif
        }
        else
        {
            /* No action required */
        }
    }
    return result;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_ram_init
 * Description                   Secure the RAM area.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] base_addr          Start Address for the Ram
 * @param[in] ram_size           Size of all available memory (in Byte)
 ***********************************************************************************************************************/
static void r_ether_t1s_ram_init (ether_t1s_instance_ctrl_t * p_ctrl,
                                  uint32_t                    ctrl_idx,
                                  const uint32_t              base_addr,
                                  const uint32_t              ram_size)
{
    (void) p_ctrl;
    g_ether_t1s_ram_manager[ctrl_idx]            = (ether_t1s_mem_manager_type *) base_addr;
    g_ether_t1s_ram_manager[ctrl_idx]->p_next    = g_ether_t1s_ram_manager[ctrl_idx];
    g_ether_t1s_ram_manager[ctrl_idx]->p_prev    = g_ether_t1s_ram_manager[ctrl_idx];
    g_ether_t1s_ram_manager[ctrl_idx]->heap_size = ram_size;
    g_ether_t1s_ram_manager[ctrl_idx]->heap_lock = ETHER_T1S_FALSE;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_ram_alloc
 * Description                   This Allocate RAM with given size
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] ram_size           Size of memory to be allocated
 ***********************************************************************************************************************/
static uint32_t r_ether_t1s_ram_alloc (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx, const uint32_t ram_size)
{
    const uint32_t required_size = ETH_ALIGN_TO_32BIT((ram_size + (uint32_t) sizeof(ether_t1s_mem_manager_type)));
    (void) p_ctrl;
    uint32_t ret_address;
    ether_t1s_mem_manager_type * p_heap_node;
    ether_t1s_mem_manager_type * p_new_block;

    ret_address = 0U;

    /* Allocation algorithm with circular list */
    p_heap_node = g_ether_t1s_ram_manager[ctrl_idx];
    do
    {
        /* If block of heap node is not reserved and block size is more than required size */
        if ((ETHER_T1S_FALSE == p_heap_node->heap_lock) && (p_heap_node->heap_size >= required_size))
        {
            /* If block size of heap node-whether required size exceeds management area size of heap node */
            if ((p_heap_node->heap_size - required_size) > (uint32_t) sizeof(ether_t1s_mem_manager_type))
            {
                /* The create heap node */
                p_new_block            = (ether_t1s_mem_manager_type *) ((uint8_t *) p_heap_node + required_size);
                p_new_block->heap_size = p_heap_node->heap_size - required_size;
                p_new_block->heap_lock = ETHER_T1S_FALSE;

                /* Re-chain as circular list */
                p_heap_node->p_next->p_prev = p_new_block;
                p_new_block->p_next         = p_heap_node->p_next;
                p_heap_node->p_next         = p_new_block;
                p_new_block->p_prev         = p_heap_node;

                /* Reserved this heap block */
                p_heap_node->heap_size = required_size;
                p_heap_node->heap_lock = ETHER_T1S_TRUE;

                /* Set next heap node search start position */
                g_ether_t1s_ram_manager[ctrl_idx] = p_new_block;
            }
            else
            {
                /* Allocate this heap node without separate the heap node */
                p_heap_node->heap_lock = ETHER_T1S_TRUE;
            }

            ret_address = (uint32_t) p_heap_node + (uint32_t) sizeof(ether_t1s_mem_manager_type);
            break;
        }
        else
        {
            p_heap_node = p_heap_node->p_next;
        }
    } while (p_heap_node != g_ether_t1s_ram_manager[ctrl_idx]);

    return ret_address;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_is_queue_configured
 * Description                   Use the Binary Search Algo to discover if
 *                               a Queue is configured and in this case return
 *                               Direction otherwise return false and
 *                               Direction is irrelevant
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] q_idx              Queue Index to see if configured
 * @param[in] dir                Direction Type
 ***********************************************************************************************************************/
static bool r_ether_t1s_is_queue_configured (ether_t1s_instance_ctrl_t    * p_ctrl,
                                             uint32_t                       ctrl_idx,
                                             const uint8_t                  q_idx,
                                             const ether_t1s_direction_type dir)
{
    uint8_t val;
    bool    return_val;
    (void) ctrl_idx;
    val        = 0U;
    return_val = ETHER_T1S_FALSE;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    ether_t1s_extended_cfg_t   * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    if (ETHER_T1S_TX == dir)
    {
        /* Tx Queues */
        do
        {
            if (q_idx == lp_hw_unit_config->queue_config.p_tx_queue_config[val].eth_tx_queue_id)
            {
                return_val = ETHER_T1S_TRUE;
            }
            else
            {
                val++;
            }
        } while ((ETHER_T1S_FALSE == return_val) && (val < lp_hw_unit_config->queue_config.number_of_tx_queue));
    }
    else
    {
        /* Rx Queues */
        do
        {
            if (q_idx == lp_hw_unit_config->queue_config.p_rx_queue_config[val].eth_rx_queue_id)
            {
                return_val = ETHER_T1S_TRUE;
            }
            else
            {
                val++;
            }
        } while ((ETHER_T1S_FALSE == return_val) && (val < lp_hw_unit_config->queue_config.number_of_rx_queue));
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_alloc_mem_for_desc
 * Description                   Alloc Memory for Descriptor Chain
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] q_idx              Queue Index
 * @param[in] q_dir              Queue Direction
 ***********************************************************************************************************************/
static uint32_t r_ether_t1s_alloc_mem_for_desc (ether_t1s_instance_ctrl_t    * p_ctrl,
                                                uint32_t                       ctrl_idx,
                                                const uint8_t                  q_idx,
                                                const ether_t1s_direction_type q_dir)
{
    uint32_t desc_addr;
    uint32_t cfg_idx;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    ether_t1s_extended_cfg_t   * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    if (ETHER_T1S_TX == q_dir)
    {
        /* Search for Tx configuration index */
        for (cfg_idx = 0; cfg_idx < lp_hw_unit_config->queue_config.number_of_tx_queue; cfg_idx++)
        {
            if (q_idx == lp_hw_unit_config->queue_config.p_tx_queue_config[cfg_idx].eth_tx_queue_id)
            {
                break;
            }
            else
            {
                /* No action required */
            }
        }

        /* TX */
        desc_addr = r_ether_t1s_ram_alloc(p_ctrl,
                                          ctrl_idx,
                                          (uint32_t) ((lp_hw_unit_config->queue_config.p_tx_queue_config[cfg_idx].
                                                       eth_tx_queue_bufs + ETH_CYCLIC_DESC_NUM) *
                                                      ETH_NORMAL_DESC_SIZE));

        /* Store the current Tx descriptor address */
        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.last_tx_desc[q_idx] = desc_addr;
        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.head_tx_desc[q_idx] = desc_addr;
    }
    else
    {
        /* Search for Rx configuration index */
        for (cfg_idx = 0; cfg_idx < lp_hw_unit_config->queue_config.number_of_rx_queue; cfg_idx++)
        {
            if (q_idx == lp_hw_unit_config->queue_config.p_rx_queue_config[cfg_idx].eth_rx_queue_id)
            {
                break;
            }
            else
            {
                /* No action required */
            }
        }

        /* Rx */
        switch (q_idx)
        {
            case ETH_BECHANNEL:
            {
                /* RX BE */
                if (ETHER_T1S_ENABLE == lp_hw_unit_config->rx_config.ets0)
                {
                    /* TS Enabled for RX BE */
                    desc_addr = r_ether_t1s_ram_alloc(p_ctrl,
                                                      ctrl_idx,
                                                      (uint32_t) ((lp_hw_unit_config->queue_config.p_rx_queue_config[
                                                                       cfg_idx].eth_rx_queue_bufs +
                                                                   ETH_CYCLIC_DESC_NUM) *
                                                                  ETH_SPECIAL_DESC_SIZE));
                }
                else
                {
                    /* TS Disabled for RX BE */
                    desc_addr = r_ether_t1s_ram_alloc(p_ctrl,
                                                      ctrl_idx,
                                                      (uint32_t) ((lp_hw_unit_config->queue_config.p_rx_queue_config[
                                                                       cfg_idx].eth_rx_queue_bufs +
                                                                   ETH_CYCLIC_DESC_NUM) *
                                                                  ETH_NORMAL_DESC_SIZE));
                }

                break;
            }

            case ETH_NCCHANNEL:
            {
                /* RX NC */
                desc_addr = r_ether_t1s_ram_alloc(p_ctrl,
                                                  ctrl_idx,
                                                  (uint32_t) ((lp_hw_unit_config->queue_config.p_rx_queue_config[cfg_idx
                                                               ].eth_rx_queue_bufs + ETH_CYCLIC_DESC_NUM) *
                                                              ETH_SPECIAL_DESC_SIZE));
                break;
            }

            default:
            {
                /* RX Stream */
                if (ETHER_T1S_ENABLE == g_ether_t1s_avb_config[ctrl_idx].rx_config.ets2)
                {
                    /* TS enabled for Rx Stream */
                    desc_addr = r_ether_t1s_ram_alloc(p_ctrl,
                                                      ctrl_idx,
                                                      (uint32_t) (((lp_hw_unit_config->queue_config.p_rx_queue_config[
                                                                        cfg_idx].eth_rx_queue_bufs +
                                                                    ETH_CYCLIC_DESC_NUM) *
                                                                   ETH_SPECIAL_DESC_SIZE)));
                }
                else
                {
                    /* TS disabled for Rx Stream  */
                    desc_addr = r_ether_t1s_ram_alloc(p_ctrl,
                                                      ctrl_idx,
                                                      (uint32_t) ((lp_hw_unit_config->queue_config.p_rx_queue_config[
                                                                       cfg_idx].eth_rx_queue_bufs +
                                                                   ETH_CYCLIC_DESC_NUM) *
                                                                  ETH_NORMAL_DESC_SIZE));
                }

                break;
            }
        }
    }

    return desc_addr;
}

/*******************************************************************************************************************//**
 * Function Name                    r_ether_t1s_fill_desc_memory
 * Description                      Set LinkFix for all DBA for each Queue
 * @param[in] p_link_descr          Input descriptor
 * @param[in] desc_addr             Input descriptor address
 ***********************************************************************************************************************/
static void r_ether_t1s_fill_desc_memory (ether_t1s_link_desc_type * const p_link_descr, const uint32_t desc_addr)
{
    if (ETHER_T1S_NULL_PTR != p_link_descr)
    {
        p_link_descr->header.die = 0U;
        p_link_descr->header.res = 0U;
        p_link_descr->dptr       = desc_addr;
        p_link_descr->header.dt  = ETH_DESC_LINKFIX;
    }
    else
    {
        /* No action at this level */
    }
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_sys_alloc_desc_buffer
 * Description                   Allocate memory for DBA (Descriptor Base Address)
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_sys_alloc_desc_buffer (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_t1s_direction_type q_direction;
    uint8_t        q_id;
    uint8_t        index;
    uint32_t       desc_addr;
    bool           queue_config;
    ether_t1s_type return_value;

    ether_t1s_link_desc_type * link_desc;

    return_value = ETHER_T1S_OK;

    link_desc = (ether_t1s_link_desc_type *)
                r_ether_t1s_ram_alloc(p_ctrl, ctrl_idx, (ETH_TXQ_NUM + ETH_RXQ_NUM) * (uint32_t) ETH_NORMAL_DESC_SIZE);
    if (ETHER_T1S_NULL_PTR != link_desc)
    {
        for (index = 0U; index < (ETH_TXQ_NUM + ETH_RXQ_NUM); index++)
        {
            /* !!!!!!!Attention index need to be reduced by 4 for a RX Queue !!!!! */
            if (ETH_TXQ_NUM > index)
            {
                /* TX Queue */
                q_direction = ETHER_T1S_TX;
                q_id        = index;
            }
            else
            {
                /* Rx Queue */
                q_direction = ETHER_T1S_RX;
                q_id        = (index - (uint8_t) ETH_TXQ_NUM);
            }

            queue_config = r_ether_t1s_is_queue_configured(p_ctrl, ctrl_idx, q_id, q_direction);

            if (ETHER_T1S_TRUE == queue_config)
            {
                /* Queue has been configured */
                /* Alloc Desc Chain Memory */

                desc_addr = r_ether_t1s_alloc_mem_for_desc(p_ctrl, ctrl_idx, q_id, q_direction);
                if (0UL != desc_addr)
                {
                    r_ether_t1s_fill_desc_memory(link_desc, desc_addr);
                }
                else
                {
                    /* Memory allocation error due to lack of resources */
                    return_value = ETHER_T1S_NOT_OK;
                    break;
                }
            }
            else
            {
                /* Queue not configured */
                /* Set Null Dptr field in Link Descriptor */
                r_ether_t1s_fill_desc_memory(link_desc, (uint32_t) 0x0U);
            }

            /* Go to the next DBA element */
            link_desc++;
        }                              /* end loop for all Tx and Rx Queues  */
    }
    else
    {
        /* Memory allocation error due to lack of resources */
        return_value = ETHER_T1S_NOT_OK;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                    r_ether_t1s_etnf_rx_mem_alloc
 * Description                      Allocate buffer memory for Rx queues.
 * @param[in] p_ctrl                Pointer to the control block for the channel
 * @param[in] ctrl_idx              Control index
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_etnf_rx_mem_alloc (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    uint8_t        inc;
    uint8_t        q_id;
    uint16_t       buf_idx;
    uint32_t       buf_address;
    ether_t1s_type return_value;
    uint32_t       rx_dptr_offset;
    return_value = ETHER_T1S_OK;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    rx_dptr_offset = (uint32_t) sizeof(ether_t1s_buf_idx_type) + ETH_RX_DPTR_OFFSET;
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    for (inc = 0U; inc < lp_hw_unit_config->queue_config.number_of_rx_queue; inc++)
    {
        q_id    = lp_hw_unit_config->queue_config.p_rx_queue_config[inc].eth_rx_queue_id;
        buf_idx = 0U;
        do
        {
            buf_address =
                r_ether_t1s_ram_alloc(p_ctrl,
                                      ctrl_idx,
                                      (uint32_t) ETH_RX_BUF_LENGTH + (uint32_t) sizeof(ether_t1s_buf_idx_type) +
                                      ETH_RX_DPTR_OFFSET);
            if (0UL == buf_address)
            {
                /* Allocation memory error due to lack of resources */
                return_value = ETHER_T1S_NOT_OK;
                inc          = lp_hw_unit_config->queue_config.number_of_rx_queue;
                break;
            }
            else
            {
                /* Add receive buffer index */
                *(uint32_t *) buf_address = g_ether_rx_buffer_index[ctrl_idx];
                g_ether_rx_buffer_index[ctrl_idx]++;
            }

            switch (q_id)
            {
                case ETH_BECHANNEL:
                {
                    /* Best Effort Queue */
                    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_be_queue_buf[buf_idx] = buf_address +
                                                                                            rx_dptr_offset;
                    break;
                }

                case ETH_NCCHANNEL:
                {
                    /* Network Control Queue */
                    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_nc_queue_buf[buf_idx] = buf_address +
                                                                                            rx_dptr_offset;
                    break;
                }

                default:
                {
                    /* Stream Queue */
                    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_s_queue_buf[q_id][buf_idx] = buf_address +
                                                                                                 rx_dptr_offset;
                    break;
                }
            }

            buf_idx++;
        } while (buf_idx < lp_hw_unit_config->queue_config.p_rx_queue_config[inc].eth_rx_queue_bufs);
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                    r_ether_t1s_set_circular_addr
 * Description                      This set the circular address
 * @param[in] ctrl_idx              Control index
 ***********************************************************************************************************************/
static void r_ether_t1s_set_circular_addr (uint32_t ctrl_idx)
{
    g_ether_t1s_ram_manager[ctrl_idx]->p_next = g_ether_t1s_ram_manager[ctrl_idx];
    g_ether_t1s_ram_manager[ctrl_idx]->p_prev = g_ether_t1s_ram_manager[ctrl_idx];
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_enable_ram_area
 * Description                   Secure the RAM area.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_enable_ram_area (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    ether_t1s_type             return_value;

    /* URAM Base Address Init  */
    r_ether_t1s_ram_init(p_ctrl,
                         ctrl_idx,
                         (uint32_t) p_extend->p_mem_pool_buffer_table,
                         (uint32_t) p_extend->ether_t1s_ram_size);


    /* Enable the DMAC and MAC of Controller */
    /* configure message handler */
    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.desc_table_addr = (uint32_t) g_ether_t1s_ram_manager[ctrl_idx] +
                                                                   (uint32_t) sizeof(ether_t1s_mem_manager_type);

    /* Allocate memory for base address table and Tx,Rx descriptor chain */
    return_value = r_ether_t1s_sys_alloc_desc_buffer(p_ctrl, ctrl_idx);

    if (ETHER_T1S_OK == return_value)
    {
        /* Allocate Rx Buffers */
        return_value = r_ether_t1s_etnf_rx_mem_alloc(p_ctrl, ctrl_idx);
    }
    else
    {
        /* No action required */
    }

    if (ETHER_T1S_OK == return_value)
    {
        /* Set circular address for Tx bufffers */
        r_ether_t1s_set_circular_addr(ctrl_idx);

        /* configure DBAT register */
        p_ctrl->p_reg_etnf0->DBAT = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.desc_table_addr;
    }
    else
    {
        /* No action required */
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_set_link_fix
 * Description                   Set LinkFix in the descriptor chain passed as first Argument
 * @param[in] link_desc          Pointer to the Descriptor to be set
 * @param[in] addr_to_link       Link Address to be set
 ***********************************************************************************************************************/
static void r_ether_t1s_set_link_fix (const uint32_t link_desc, const uint32_t addr_to_link)
{
    ether_t1s_link_desc_type * p_link_desc;
    p_link_desc = (ether_t1s_link_desc_type *) link_desc;

    p_link_desc->header.die = 0U;
    p_link_desc->header.res = 0U;
    p_link_desc->dptr       = addr_to_link;
    p_link_desc->header.dt  = ETH_DESC_LINKFIX;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_set_ext_fempty
 * Description                   Set Fempty in the extended (20Bytes) descriptor chain for passed Queue and Buffer Index
 * @param[in] ctrl_idx           Control index
 * @param[in] p_descr            Pointer to a Data Descriptor
 * @param[in] q_idx              Index of the Queue (0..3 for Tx 0..17 for Rx)
 * @param[in] dir                Queue Direction (e.g. TX or RX)
 * @param[in] buf_idx            Index ot the TX or RX buffer
 ***********************************************************************************************************************/
static void r_ether_t1s_set_ext_fempty (uint32_t                             ctrl_idx,
                                        ether_t1s_ext_data_desc_type * const p_descr,
                                        const uint8_t                        q_idx,
                                        const ether_t1s_direction_type       dir,
                                        const uint32_t                       buf_idx)
{
    if (((ETHER_T1S_NULL_PTR != p_descr) && (ETHER_T1S_TX != dir)))
    {
        p_descr->header.die             = 0U;
        p_descr->header.ctrl            = 0U;
        p_descr->header.ds              = ETH_RX_BUF_LENGTH;
        p_descr->time_stamp.time_stamp0 = 0x0U;
        p_descr->time_stamp.time_stamp1 = 0x0U;
        p_descr->time_stamp.time_stamp2 = 0x0U;
        if (ETH_BECHANNEL == q_idx)
        {
            /* RX BE */
            p_descr->dptr = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_be_queue_buf[buf_idx];
        }
        else if (ETH_NCCHANNEL == q_idx)
        {
            /* RX NC */
            p_descr->dptr = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_nc_queue_buf[buf_idx];
        }
        else
        {
            /* RX S */
            /* Attention Q index!*/
            p_descr->dptr = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_s_queue_buf[q_idx][buf_idx];
        }

        p_descr->header.dt = ETH_DESC_FEMPTY_AVB;
    }
    else
    {
        /* Cannot Set Fempty for NULL Address */
        /* Error as TX Descr are always Standard and not extended */
    }
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_set_fempty
 * Description                   Set Fempty in the standard (8Bytes) descriptor chain for passed Queue and Buffer Index
 * @param[in] ctrl_idx           Control index
 * @param[in] p_descr            Pointer to a Data Descriptor
 * @param[in] q_idx              Index of the Queue (0..3 for Tx 0..17 for Rx)
 * @param[in] dir                Queue Direction (e.g. TX or RX)
 * @param[in] buf_idx            Index ot the TX or RX buffer
 ***********************************************************************************************************************/
static void r_ether_t1s_set_fempty (uint32_t                         ctrl_idx,
                                    ether_t1s_data_desc_type * const p_descr,
                                    const uint8_t                    q_idx,
                                    const ether_t1s_direction_type   dir,
                                    const uint32_t                   buf_idx)
{
    if (ETHER_T1S_NULL_PTR != p_descr)
    {
        if (ETHER_T1S_TX == dir)
        {
            /* TX Queue */
            p_descr->header.die  = 0U;
            p_descr->header.ctrl = 0U;

            /* It will be set only during Transmission at run time */
            p_descr->header.ds = 0U;
            p_descr->dptr      = (uint32_t) 0x0U;
            p_descr->header.dt = ETH_DESC_FEMPTY_AVB;
        }
        else if (ETH_BECHANNEL == q_idx)
        {
            /* RX BE */
            p_descr->header.die  = 0U;
            p_descr->header.ctrl = 0U;
            p_descr->header.ds   = ETH_RX_BUF_LENGTH;
            p_descr->dptr        = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_be_queue_buf[buf_idx];
            p_descr->header.dt   = ETH_DESC_FEMPTY_AVB;
        }
        else if (ETH_NCCHANNEL == q_idx)
        {
            /* RX NC */
            /* This is an Error as RX NC is always Extended Descriptor */
        }
        else
        {
            /* RX S */
            p_descr->header.die  = 0U;
            p_descr->header.ctrl = 0U;
            p_descr->header.ds   = ETH_RX_BUF_LENGTH;

            /* Attention Q index!*/
            p_descr->dptr      = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_s_queue_buf[q_idx][buf_idx];
            p_descr->header.dt = ETH_DESC_FEMPTY_AVB;
        }
    }
    else
    {
        /* Cannot Set Fempty for NULL Address */
    }
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_set_desc_chain
 * Description                   Configured the Descriptor chain for each Queue (Circular Chain).
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] addr               Addr of the first descriptor in Queue
 * @param[in] q_idx              Index of the Queue (0..3 for Tx 0..17 for Rx)
 * @param[in] dir                Queue Direction (e.g. TX or RX)
 ***********************************************************************************************************************/
static void r_ether_t1s_set_desc_chain (ether_t1s_instance_ctrl_t    * p_ctrl,
                                        uint32_t                       ctrl_idx,
                                        const uint32_t                 addr,
                                        const uint8_t                  q_idx,
                                        const ether_t1s_direction_type dir)
{
    uint32_t                       idx;
    uint32_t                       cfg_idx;
    ether_t1s_option_type_t        rx_time_stamp;
    ether_t1s_data_desc_type     * p_current_data_desc;
    ether_t1s_ext_data_desc_type * p_current_ext_data_desc;
    ether_t1s_etnf_config_type   * lp_hw_unit_config;
    ether_t1s_extended_cfg_t     * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    /* Initialize the counter for any Queue */
    idx     = 0UL;
    cfg_idx = 0;
    if (0UL != addr)
    {
        if (ETHER_T1S_TX == dir)
        {
            /* TX */
            /* Search for Tx configuration index */
            for (cfg_idx = 0; cfg_idx < lp_hw_unit_config->queue_config.number_of_tx_queue; cfg_idx++)
            {
                if (q_idx == lp_hw_unit_config->queue_config.p_tx_queue_config[cfg_idx].eth_tx_queue_id)
                {
                    break;
                }
                else
                {
                    /* No action required */
                }
            }

            p_current_data_desc = (ether_t1s_data_desc_type *) (addr);
            do
            {
                if ((uint32_t) (lp_hw_unit_config->queue_config.p_tx_queue_config[cfg_idx].eth_tx_queue_bufs -
                                (uint16_t) ETH_CYCLIC_DESC_NUM) >= idx)
                {
                    /* Set FEmpty */
                    r_ether_t1s_set_fempty(ctrl_idx, p_current_data_desc, q_idx, ETHER_T1S_TX, idx);
                }
                else
                {
                    /* Set LinkFix */
                    r_ether_t1s_set_link_fix((uint32_t) p_current_data_desc, addr);
                }

                p_current_data_desc++;
                idx++;
            } while (idx <= (uint32_t) lp_hw_unit_config->queue_config.p_tx_queue_config[cfg_idx].eth_tx_queue_bufs);
        }
        else
        {
            /* RX */
            /* Search for Rx configuration index */
            for (cfg_idx = 0; cfg_idx < lp_hw_unit_config->queue_config.number_of_rx_queue; cfg_idx++)
            {
                if (q_idx == lp_hw_unit_config->queue_config.p_rx_queue_config[cfg_idx].eth_rx_queue_id)
                {
                    break;
                }
                else
                {
                    /* No action required */
                }
            }

            /* Get Time Stamp status */
            if (ETH_NCCHANNEL == q_idx)
            {
                rx_time_stamp = ETHER_T1S_ENABLE;
            }
            else if (ETH_BECHANNEL == q_idx)
            {
                rx_time_stamp = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_be_time_stamp;
            }
            else
            {
                rx_time_stamp = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_s_time_stamp;
            }

            if (ETHER_T1S_ENABLE == rx_time_stamp)
            {
                /* TS Enable */
                p_current_ext_data_desc = (ether_t1s_ext_data_desc_type *) (addr);

                do
                {
                    if ((uint32_t) lp_hw_unit_config->queue_config.p_rx_queue_config[cfg_idx].eth_rx_queue_bufs > idx)
                    {
                        /* Extended Data Descriptor */
                        r_ether_t1s_set_ext_fempty(ctrl_idx, p_current_ext_data_desc, q_idx, ETHER_T1S_RX, idx);
                    }
                    else
                    {
                        r_ether_t1s_set_link_fix((uint32_t) p_current_ext_data_desc, addr);
                    }

                    idx++;
                    p_current_ext_data_desc++;
                } while (idx <=
                         (uint32_t) lp_hw_unit_config->queue_config.p_rx_queue_config[cfg_idx].eth_rx_queue_bufs);
            }
            else
            {
                /* TS Disable */
                p_current_data_desc = (ether_t1s_data_desc_type *) (addr);
                do
                {
                    if ((uint32_t) lp_hw_unit_config->queue_config.p_rx_queue_config[cfg_idx].eth_rx_queue_bufs > idx)
                    {
                        r_ether_t1s_set_fempty(ctrl_idx, p_current_data_desc, q_idx, ETHER_T1S_RX, idx);
                    }
                    else
                    {
                        r_ether_t1s_set_link_fix((uint32_t) p_current_data_desc, addr);
                    }

                    p_current_data_desc++;

                    idx++;
                } while (idx <=
                         (uint32_t) lp_hw_unit_config->queue_config.p_rx_queue_config[cfg_idx].eth_rx_queue_bufs);

                /* Better to repeat this code for any Queue */
                /* as in future we may have different No of buffers */
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_etnf_desc_config
 * Description                   Configure descriptor.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 ***********************************************************************************************************************/
static void r_ether_t1s_etnf_desc_config (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    uint8_t q_id;                      /* Index for the Queue */
    uint8_t cnt;
    ether_t1s_link_desc_type   * p_link_addr;
    ether_t1s_link_desc_type   * p_target_addr;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    ether_t1s_extended_cfg_t   * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    /* init link descriptor */
    p_link_addr = (ether_t1s_link_desc_type *) (g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.desc_table_addr);

    /*======TX Queues ========*/
    for (cnt = 0U; cnt < lp_hw_unit_config->queue_config.number_of_tx_queue; cnt++)
    {
        /* Calculate target descriptor table address */
        p_target_addr = p_link_addr + lp_hw_unit_config->queue_config.p_tx_queue_config[cnt].eth_tx_queue_id;
        if (0UL != p_target_addr->dptr)
        {
            /* Get Tx q_id */
            q_id = lp_hw_unit_config->queue_config.p_tx_queue_config[cnt].eth_tx_queue_id;
            r_ether_t1s_set_desc_chain(p_ctrl, ctrl_idx, (p_target_addr->dptr), q_id, ETHER_T1S_TX);
        }
        else
        {
            /* This Queue is not configured */
        }
    }

    /* Set Rx Queues start position */
    p_link_addr = p_link_addr + ETH_RXBEQ_OFFSET;

    /*======RX Queues ========*/
    for (cnt = 0U; cnt < lp_hw_unit_config->queue_config.number_of_rx_queue; cnt++)
    {
        /* Calculate target descriptor table address */
        p_target_addr = p_link_addr + lp_hw_unit_config->queue_config.p_rx_queue_config[cnt].eth_rx_queue_id;
        if (0UL != p_target_addr->dptr)
        {
            q_id = lp_hw_unit_config->queue_config.p_rx_queue_config[cnt].eth_rx_queue_id;
            r_ether_t1s_set_desc_chain(p_ctrl, ctrl_idx, (p_target_addr->dptr), q_id, ETHER_T1S_RX);
        }
        else
        {
            /* This Queue is not configured */
        }
    }
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_desc_update
 * Description                   Update descriptor chain (without timestamp support).
 * @param[in] desc_ptr           Pointer to descriptor to be updated
 ***********************************************************************************************************************/
static uint32_t r_ether_t1s_desc_update (const uint32_t desc_ptr)
{
    ether_t1s_data_desc_type * p_data_desc;
    p_data_desc = (ether_t1s_data_desc_type *) desc_ptr;

    p_data_desc->header.die  = 0U;
    p_data_desc->header.ctrl = 0U;
    p_data_desc->header.ds   = ETH_RX_BUF_LENGTH;
    p_data_desc->header.dt   = ETH_DESC_FEMPTY_AVB;

    return (uint32_t) p_data_desc;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_desc_ts_update
 * Description                   Update descriptor chain (with timestamp support).
 * @param[in] desc_ptr           Pointer to descriptor to be updated
 ***********************************************************************************************************************/
static uint32_t r_ether_t1s_desc_ts_update (const uint32_t desc_ptr)
{
    ether_t1s_ext_data_desc_type * p_ext_data_desc;
    p_ext_data_desc = (ether_t1s_ext_data_desc_type *) desc_ptr;

    p_ext_data_desc->header.die             = 0U;
    p_ext_data_desc->header.ctrl            = 0U;
    p_ext_data_desc->header.ds              = ETH_RX_BUF_LENGTH;
    p_ext_data_desc->time_stamp.time_stamp0 = 0UL;
    p_ext_data_desc->time_stamp.time_stamp1 = 0UL;
    p_ext_data_desc->time_stamp.time_stamp2 = 0U;
    p_ext_data_desc->header.dt              = ETH_DESC_FEMPTY_AVB;

    return (uint32_t) p_ext_data_desc;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_rx_desc_chain_update
 * Description                   Update receive descriptor chain
 * @param[in] ctrl_idx           Control index
 * @param[in] desc_ptr           Descriptor pointer
 * @param[in] p_ch_config        Channel Config Pointer
 ***********************************************************************************************************************/
static uint32_t r_ether_t1s_rx_desc_chain_update (uint32_t                                  ctrl_idx,
                                                  const uint32_t                            desc_ptr,
                                                  const ether_t1s_rx_ch_config_type * const p_ch_config)
{
    uint32_t last_desc;

    switch (p_ch_config->ch_type)
    {
        case ETHER_T1S_RX_BE:
        {
            if (ETHER_T1S_DISABLE == g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_be_time_stamp)
            {
                last_desc = r_ether_t1s_desc_update(desc_ptr);
            }
            else
            {
                last_desc = r_ether_t1s_desc_ts_update(desc_ptr);
            }

            break;
        }

        case ETHER_T1S_RX_S:
        {
            if (ETHER_T1S_DISABLE == g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_s_time_stamp)
            {
                last_desc = r_ether_t1s_desc_update(desc_ptr);
            }
            else
            {
                last_desc = r_ether_t1s_desc_ts_update(desc_ptr);
            }

            break;
        }

        /* ETH_RX_NC */
        default:
        {
            last_desc = r_ether_t1s_desc_ts_update(desc_ptr);
            break;
        }
    }

    return last_desc;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_hw_etnf_rx_desc_chain_config
 * Description                   Configure Rx descriptor chain
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] p_ch_config        Channel Config Pointer Index
 ***********************************************************************************************************************/
static void r_ether_t1s_hw_etnf_rx_desc_chain_config (ether_t1s_instance_ctrl_t               * p_ctrl,
                                                      uint32_t                                  ctrl_idx,
                                                      const ether_t1s_rx_ch_config_type * const p_ch_config)
{
    ether_t1s_link_desc_type     * p_link_addr;
    ether_t1s_data_desc_type     * p_data_desc;
    ether_t1s_ext_data_desc_type * p_ext_data_desc;
    p_link_addr = (ether_t1s_link_desc_type *)
                  (g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.desc_table_addr);

    p_link_addr += (ETH_RXBEQ_OFFSET + p_ch_config->ch_num);
    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[p_ch_config->ch_num] =
        (uint32_t) (p_link_addr->dptr);
    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_rx_desc_update[p_ch_config->ch_num] =
        (uint32_t) (p_link_addr->dptr);
    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_rx_desc_release[p_ch_config->ch_num] =
        (uint32_t) (p_link_addr->dptr);

    if (ETH_BECHANNEL == p_ch_config->ch_num)
    {
        /* best effort channel */
        if (ETHER_T1S_DISABLE == g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_be_time_stamp)
        {
            p_data_desc = (ether_t1s_data_desc_type *)
                          g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[p_ch_config->ch_num];
            do
            {
                p_data_desc = (ether_t1s_data_desc_type *)
                              r_ether_t1s_rx_desc_chain_update(ctrl_idx, (uint32_t) p_data_desc, p_ch_config);
                p_data_desc++;
            } while (p_data_desc->header.dt != ETH_DESC_LINKFIX);
        }
        else
        {
            p_ext_data_desc = (ether_t1s_ext_data_desc_type *)
                              g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[p_ch_config->ch_num];
            do
            {
                p_ext_data_desc = (ether_t1s_ext_data_desc_type *)
                                  r_ether_t1s_rx_desc_chain_update(ctrl_idx, (uint32_t) p_ext_data_desc, p_ch_config);
                p_ext_data_desc++;
            } while (p_ext_data_desc->header.dt != ETH_DESC_LINKFIX);
        }
    }
    else if (ETH_NCCHANNEL == p_ch_config->ch_num)
    {
        /* network control channel */
        p_ext_data_desc = (ether_t1s_ext_data_desc_type *)
                          g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[p_ch_config->ch_num];
        do
        {
            p_ext_data_desc = (ether_t1s_ext_data_desc_type *)
                              r_ether_t1s_rx_desc_chain_update(ctrl_idx, (uint32_t) p_ext_data_desc, p_ch_config);
            p_ext_data_desc++;
        } while (p_ext_data_desc->header.dt != ETH_DESC_LINKFIX);
    }
    else
    {
        /* stream channel */
        if (ETHER_T1S_DISABLE == g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_s_time_stamp)
        {
            p_data_desc = (ether_t1s_data_desc_type *)
                          g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[p_ch_config->ch_num];
            do
            {
                p_data_desc = (ether_t1s_data_desc_type *)
                              r_ether_t1s_rx_desc_chain_update(ctrl_idx, (uint32_t) p_data_desc, p_ch_config);

                p_data_desc++;
            } while (p_data_desc->header.dt != ETH_DESC_LINKFIX);
        }
        else
        {
            p_ext_data_desc = (ether_t1s_ext_data_desc_type *)
                              g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[p_ch_config->ch_num];
            do
            {
                p_ext_data_desc = (ether_t1s_ext_data_desc_type *)
                                  r_ether_t1s_rx_desc_chain_update(ctrl_idx, (uint32_t) p_ext_data_desc, p_ch_config);

                p_ext_data_desc++;
            } while (p_ext_data_desc->header.dt != ETH_DESC_LINKFIX);
        }
    }

    /* reload desc. base address */
    if (ETHER_T1S_OPERATION == g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.dev_status)
    {
        p_ctrl->p_reg_etnf0->DLR = (1UL << (p_ch_config->ch_num + ETH_RXBEQ_OFFSET));
    }
    else
    {
        /* No action required */
    }
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_hw_enable_controller
 * Description                   Changes the state of an ETNF controller as ACTIVE
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Controller index
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_hw_enable_controller (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    uint32_t                     q_count;
    uint8_t                      q_idx;
    ether_t1s_type               return_value;
    uint32_t                     ecmr_val;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
    uint32_t       time_out_count_init;
    uint32_t       time_out_count_elap;
    uint32_t       increment_value;
    ether_t1s_type increment_result;
#endif
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;

    /* Entering Operation mode, must enter Config mode.  */
    return_value = r_ether_t1s_hw_etnf_operation_mode_change(p_ctrl, ctrl_idx, ETHER_T1S_CONFIG);
    if (ETHER_T1S_OK == return_value)
    {
        return_value = r_ether_t1s_enable_ram_area(p_ctrl, ctrl_idx);
    }
    else
    {
        /* No action required */
    }

    if (ETHER_T1S_OK == return_value)
    {
        /* Configure Tx and Rx Descriptor chain */
        r_ether_t1s_etnf_desc_config(p_ctrl, ctrl_idx);

        /* Initialize variable */
        q_count = 0UL;

        do
        {
            q_idx = lp_hw_unit_config->queue_config.p_rx_queue_config[q_count].eth_rx_queue_id;

            (void) r_ether_t1s_hw_etnf_rx_desc_chain_config(p_ctrl, ctrl_idx, &(g_ether_t1s_rx_config[q_idx]));
            q_count++;
        } while (q_count < (uint32_t) lp_hw_unit_config->queue_config.number_of_rx_queue);

        if (ETHER_T1S_MODE == p_extend->ether_phy_interface)
        {
            /* Set MAC mode as T1S mode and enable Rx/Tx */
            ecmr_val = ETH_ETNF_ECMR_DM_T1S | ETH_ETNF_ECMR_RE_TE;

            /* Set CRS assert check in T1S mode */
#if (!ETHER_T1S_CFG_CRS_ASSERT_CHECK)
            ecmr_val |= ETH_ETNF_ECMR_ZPF_CRS_NOT_CHECKED;
#endif
        }
        else
        {
            /* Set MAC mode as RMII Full-duplex mode and enable Rx/Tx */
            ecmr_val = ETH_ETNF_ECMR_DM_RMII | ETH_ETNF_ECMR_RE_TE;
        }

        /* Set MAC address */
        /************ Little Endian Core **************/
        /* Write into MALR */
        p_ctrl->p_reg_etnf0->MALR = g_ether_t1s_ctrl_stat[ctrl_idx].mac_addr.l16;

        /* Write into MAHR */
        p_ctrl->p_reg_etnf0->MAHR = g_ether_t1s_ctrl_stat[ctrl_idx].mac_addr.h32;

        /*********** Big Endian ***********************/
        /* V2H */
#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)
        if (ETHER_T1S_TRUE == g_ether_t1s_ctrl_stat[ctrl_idx].promiscuous)
        {
            ecmr_val |= ETH_ETNF_ECMR_PRM;
        }
        else
        {
            ecmr_val &= ~ETH_ETNF_ECMR_PRM;
        }
#endif
        p_ctrl->p_reg_etnf0->ECMR = ecmr_val;

        /* Set Pulse Generation Mode */
#if (ETHER_T1S_CFG_PPS_CORRECTED_GPTP)
        p_ctrl->p_reg_etnf0->GCCR = ETH_ETNF_GCCR_PGM;
#endif                                 /* (ETHER_T1S_CFG_PPS_CORRECTED_GPTP) */

        /* Set operating mode to operation mode */
        return_value = r_ether_t1s_hw_etnf_operation_mode_change(p_ctrl, ctrl_idx, ETHER_T1S_OPERATION);
#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
        increment_value = ETH_HZ2NS(ETH_PERI_CLOCK_HZ);
        increment_value = ETH_ETNF_SET_GTI_TIV(increment_value);

        /* Set Increment into HW */
        time_out_count_init = 0UL;
        time_out_count_elap = 0UL;
        (void) r_ether_t1s_get_counter_value(&time_out_count_init);
        do
        {
            time_out_count_elap = time_out_count_elap + r_ether_t1s_get_time_out_value(&time_out_count_init);
            increment_result    = r_ether_t1s_set_increment_time(p_ctrl, increment_value);
        } while ((increment_result != ETHER_T1S_OK) && (time_out_count_elap <= ETH_TIMEOUT_COUNT));
        return_value |= increment_result;
#endif
    }
    else
    {
        /* No action required */
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_hw_disable_controller
 * Description                   Changes the state of an ETNF controller as DOWN
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Controller index
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_hw_disable_controller (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_t1s_type      error_value;
    ether_t1s_tick_type time_out_count_init;
    ether_t1s_tick_type time_out_count_elap;
    uint32_t            reg_val;

    if (ETHER_T1S_OPERATION == g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.dev_status)
    {
        /* Disable the DMAC and MAC of Controller for pending Rx */
        p_ctrl->p_reg_etnf0->ECMR &= ~ETH_ETNF_ECMR_RE;

        /* Wait for completion of reception process */
        time_out_count_init = 0UL;
        (void) r_ether_t1s_get_counter_value(&time_out_count_init);
        do
        {
            time_out_count_elap = r_ether_t1s_get_time_out_value(&time_out_count_init);
            reg_val             = p_ctrl->p_reg_etnf0->CSR & ETH_CSR_RPO_MASK;
        } while ((reg_val != 0UL) && (time_out_count_elap <= ETH_TIMEOUT_COUNT));

        /* Wait for completion of transmission request */
        time_out_count_init = 0UL;
        (void) r_ether_t1s_get_counter_value(&time_out_count_init);
        do
        {
            time_out_count_elap = r_ether_t1s_get_time_out_value(&time_out_count_init);
            reg_val             = p_ctrl->p_reg_etnf0->TCCR & ETH_TCCR_TSRQX_MASK;
        } while ((reg_val != 0UL) && (time_out_count_elap <= ETH_TIMEOUT_COUNT));

        /* Wait for completion of transmission process */
        time_out_count_init = 0UL;
        (void) r_ether_t1s_get_counter_value(&time_out_count_init);
        do
        {
            time_out_count_elap = r_ether_t1s_get_time_out_value(&time_out_count_init);
            reg_val             = p_ctrl->p_reg_etnf0->CSR & ETH_CSR_TPOX_MASK;
        } while ((reg_val != 0UL) && (time_out_count_elap <= ETH_TIMEOUT_COUNT));

        /* Disable the DMAC and MAC of Controller for pending Tx */
        p_ctrl->p_reg_etnf0->ECMR &= ~ETH_ETNF_ECMR_TE;

        /* Process all required status information */
        /* In ETNF, necessary information is not reset in CONFIG and STANDBY mode.*/
        p_ctrl->p_reg_etnf0->CCC |= ETH_ETNF_CCC_DTSR;

        time_out_count_init = 0UL;
        (void) r_ether_t1s_get_counter_value(&time_out_count_init);
        do
        {
            time_out_count_elap = r_ether_t1s_get_time_out_value(&time_out_count_init);
            reg_val             = p_ctrl->p_reg_etnf0->CSR & ETH_ETNF_CSR_DTS;
        } while ((reg_val != ETH_ETNF_CSR_DTS) && (time_out_count_elap <= ETH_TIMEOUT_COUNT));
    }
    else
    {
        /* No action required */
    }

    error_value = r_ether_t1s_hw_etnf_operation_mode_change(p_ctrl, ctrl_idx, ETHER_T1S_CONFIG);

    if (ETHER_T1S_CONFIG != g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.dev_status)
    {
        reg_val = p_ctrl->p_reg_etnf0->CCC & ~ETH_ETNF_CCC_OPC_MASK;
        p_ctrl->p_reg_etnf0->CCC  = reg_val | ETH_ETNF_CCC_OPC_OPERATION;
        p_ctrl->p_reg_etnf0->CCC &= ~ETH_ETNF_CCC_DTSR;

        /* Wait for completion of reception/transmit process */
        time_out_count_init = 0UL;
        (void) r_ether_t1s_get_counter_value(&time_out_count_init);
        do
        {
            time_out_count_elap = r_ether_t1s_get_time_out_value(&time_out_count_init);
            reg_val             = p_ctrl->p_reg_etnf0->CSR & ETH_ETNF_CSR_TDUO_RPO;
        } while ((reg_val != 0UL) && (time_out_count_elap <= ETH_TIMEOUT_COUNT));

        p_ctrl->p_reg_etnf0->CCC |= ETH_ETNF_CCC_DTSR;

        time_out_count_init = 0UL;
        (void) r_ether_t1s_get_counter_value(&time_out_count_init);
        do
        {
            time_out_count_elap = r_ether_t1s_get_time_out_value(&time_out_count_init);
            reg_val             = p_ctrl->p_reg_etnf0->CSR & ETH_ETNF_CSR_DTS;
        } while ((reg_val != ETH_ETNF_CSR_DTS) && (time_out_count_elap <= ETH_TIMEOUT_COUNT));

        error_value = r_ether_t1s_hw_etnf_operation_mode_change(p_ctrl, ctrl_idx, ETHER_T1S_CONFIG);
    }
    else
    {
        /* No action required */
    }

    p_ctrl->p_reg_etnf0->CCC &= ~ETH_ETNF_CCC_DTSR;

    return error_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_set_controller_mode
 * Description                   This API performs enabling / disabling  of the indexed controller.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of an ether controller
 * @param[in] ctrl_mode          Enable/Disable controller
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_set_controller_mode (ether_t1s_instance_ctrl_t * p_ctrl,
                                                       uint32_t                    ctrl_idx,
                                                       ether_t1s_mode_type_t       ctrl_mode)
{
    ether_t1s_type return_value;

    if (ETHER_T1S_MODE_ACTIVE == ctrl_mode)
    {
        /* Invoke r_ether_t1s_hw_enable_controller to activate HW */
        return_value = r_ether_t1s_hw_enable_controller(p_ctrl, (uint32_t) ctrl_idx);
        if (ETHER_T1S_OK == return_value)
        {
            /* To prevent MainFunction be executed during the transition,
             * set the controller state as ACTIVE after the transition. */
            g_ether_t1s_ctrl_stat[ctrl_idx].mode = ETHER_T1S_MODE_ACTIVE;
        }                              /* else No action required */
    }
    else
    {
        /* To prevent MainFunction be executed during the transition,
         * set the controller state as DOWN before the transition. */
        g_ether_t1s_ctrl_stat[ctrl_idx].mode = ctrl_mode;

        /* Invoke r_ether_t1s_hw_disable_controller to stop HW */
        return_value = r_ether_t1s_hw_disable_controller(p_ctrl, (uint32_t) ctrl_idx);

        /* Retreive all provided Tx Buffers */
        if (ETHER_T1S_OK == return_value)
        {
            /* Invoke r_ether_t1s_initialize_buffer to initialize Tx/Rx buffer pool */
            r_ether_t1s_initialize_buffer(ctrl_idx);
        }
    }
    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_set_phys_addr
 * Description               Sets the physical source address used by the indexed controller.
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] mac_addr       Convert mac address data
 ***********************************************************************************************************************/
static void r_ether_t1s_set_phys_addr (uint8_t ctrl_idx, const uint8_t mac_addr[])
{
    ETHER_PACK_ADDRESS_FROM_8(mac_addr, g_ether_t1s_ctrl_stat[ctrl_idx].mac_addr);
}

#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_update_phys_addr_filter
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] p_phys_addr    Pointer to memory containing the physical destination address (MAC address) in network byte order.
 * @param[in] action         Add or remove the address from the Ethernet controllers
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_update_phys_addr_filter (uint32_t                       ctrl_idx,
                                                           uint8_t                      * p_phys_addr,
                                                           ether_t1s_filter_action_type_t action)
{
    ether_t1s_type               result;
    ether_t1s_mac_address_type_t st_mac_addr;
    uint32_t filter_idx;

    /* Pack MAC address from user to the structure to be compared */
    ETHER_PACK_ADDRESS_FROM_8(p_phys_addr, st_mac_addr);

    /* FF:..:FF and 00:..:00 are not allowed while the controller is ACTVIE */
    if ((ETHER_T1S_MODE_ACTIVE == g_ether_t1s_ctrl_stat[ctrl_idx].mode) &&
        ((0UL == ETHER_COMPARE_MAC(st_mac_addr, g_ether_t1s_broadcast_addr)) ||
         (0UL == ETHER_COMPARE_MAC(st_mac_addr, g_ether_t1s_null_addr))))
    {
        result = ETHER_T1S_NOT_OK;
    }
    else
    {
        result = ETHER_T1S_OK;

        if (0UL == ETHER_COMPARE_MAC(st_mac_addr, g_ether_t1s_broadcast_addr))
        {
            /* If FF:..:FF is specified, enter promiscuous mode */
            g_ether_t1s_ctrl_stat[ctrl_idx].promiscuous = ETHER_T1S_TRUE;
        }
        else if (0UL == ETHER_COMPARE_MAC(st_mac_addr, g_ether_t1s_null_addr))
        {
            /* If 00:..:00 is specified, clear promisoucs mode and all filters */
            r_ether_t1s_clear_all_address_filters((uint32_t) ctrl_idx);
        }
        else
        {
            if (ETHER_T1S_ADD_TO_FILTER == action)
            {
                filter_idx = r_ether_t1s_get_filter_index((uint32_t) ctrl_idx, &st_mac_addr);
                if ((0UL == ETHER_T1S_CHECK_MULTICAST(st_mac_addr)) ||
                    (ETHER_T1S_INVALID_FILTER_INDEX != filter_idx) ||
                    (ETHER_T1S_INVALID_FILTER_INDEX == g_ether_t1s_ctrl_stat[ctrl_idx].active_filter_bits))
                {
                    result = ETHER_T1S_NOT_OK;
                }
                else
                {
                    r_ether_t1s_add_address_filter((uint32_t) ctrl_idx, &st_mac_addr);
                }
            }
            else
            {
                filter_idx = r_ether_t1s_get_filter_index((uint32_t) ctrl_idx, &st_mac_addr);
                if (ETHER_T1S_INVALID_FILTER_INDEX == filter_idx)
                {
                    result = ETHER_T1S_NOT_OK;
                }
                else
                {
                    r_ether_t1s_remove_address_filter((uint32_t) ctrl_idx, filter_idx);
                }
            }
        }
    }

    return result;
}

#endif
#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_add_address_filter
 * Description               Add a address filter to the filter list
 * @param[in] ctrl_idx       Index of a controller
 * @param[in] p_mac_addr     Pointer to a MAC address to be searched
 ***********************************************************************************************************************/
static void r_ether_t1s_add_address_filter (uint32_t ctrl_idx, const ether_t1s_mac_address_type_t * const p_mac_addr)
{
    uint32_t filter_idx;
    bool     registered;
    registered = ETHER_T1S_FALSE;
    for (filter_idx = 0UL; (ETHER_T1S_FALSE == registered) && (filter_idx < ETHER_T1S_MAX_FILTERS); filter_idx++)
    {
        if (0UL == (g_ether_t1s_ctrl_stat[ctrl_idx].active_filter_bits & (1UL << (filter_idx))))
        {
            g_ether_t1s_address_filters[ctrl_idx][filter_idx]   = *p_mac_addr;
            g_ether_t1s_ctrl_stat[ctrl_idx].active_filter_bits |= (1UL << filter_idx);
            registered = ETHER_T1S_TRUE;
        }                              /*else : no action required */
    }
}

#endif
#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name         : r_ether_t1s_remove_address_filter
 * Description           : Remove a filter from the filter list
 * @param[in] ctrl_idx       Index of a controller
 * @param[in] filter_idx     Index of an address in the filter list
 ***********************************************************************************************************************/
static void r_ether_t1s_remove_address_filter (uint32_t ctrl_idx, uint32_t filter_idx)
{
    /* Clear an active bit */
    g_ether_t1s_ctrl_stat[ctrl_idx].active_filter_bits &= ~(1UL << filter_idx);
}

#endif

#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name         r_ether_t1s_get_filter_index
 * Description           Get the index of the specified MAC address if exists
 * @param[in] ctrl_idx    Index of a controller
 * @param[in] p_mac_addr  Pointer to a MAC address to be searched
 ***********************************************************************************************************************/
static uint32_t r_ether_t1s_get_filter_index (uint32_t ctrl_idx, const ether_t1s_mac_address_type_t * const p_mac_addr)
{
    volatile uint32_t filter_idx;
    uint32_t          found_idx;

    found_idx = ETHER_T1S_INVALID_FILTER_INDEX; /* Invalid filter index */

    /* Check the specified address is already registered */
    for (filter_idx = 0UL;
         (ETHER_T1S_INVALID_FILTER_INDEX == found_idx) && (filter_idx < ETHER_T1S_MAX_FILTERS);
         filter_idx++)
    {
        if ((0UL != (g_ether_t1s_ctrl_stat[ctrl_idx].active_filter_bits & (1UL << filter_idx))) &&
            (0UL == ETHER_COMPARE_MAC(*p_mac_addr, g_ether_t1s_address_filters[ctrl_idx][filter_idx])))
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
 * Function Name: r_ether_t1s_call_callback
 * Calls user callback.
 * @param[in]     p_instance_ctrl      Pointer to ether instance control block
 * @param[in]     event                Event code
 **********************************************************************************************************************/
static void r_ether_t1s_call_callback (ether_t1s_instance_ctrl_t * p_instance_ctrl, ether_event_t event)
{
    if (NULL == p_instance_ctrl->p_callback)
    {
        return;
    }

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

    /* If the project is not Trustzone Secure, then it will never need to change security state in order to call the callback. */
    p_instance_ctrl->p_callback(p_args);

    if (NULL != p_instance_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_instance_ctrl->p_callback_memory = args;
    }
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_send_data
 * Description               This API performs Write the transmit data to the transmit buffer.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] buf            Pointer to buffer data
 * @param[in] len_byte       Data length in byte
 ***********************************************************************************************************************/
static fsp_err_t r_ether_t1s_send_data (ether_t1s_instance_ctrl_t * p_ctrl, uint8_t * buf, uint32_t len_byte)
{
    fsp_err_t                       return_value     = FSP_SUCCESS;
    volatile ether_t1s_type         result           = ETHER_T1S_OK;
    ether_t1s_buf_req_return_type_t buf_return_value = ETHER_T1S_BUFREQ_OK;
    ether_t1s_instance_ctrl_t     * p_instance_ctrl  = (ether_t1s_instance_ctrl_t *) p_ctrl;
    ether_t1s_extended_cfg_t      * p_extend         = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    ether_t1s_type                * p_buf            = ETHER_T1S_NULL_PTR;
    ether_t1s_buf_idx_type          buf_idx          = 0U;
    uint8_t  priority   = 0U;
    uint32_t lus_length = 0U;
    uint8_t  ctrl_idx   = p_instance_ctrl->p_ether_cfg->channel;
    uint32_t q_idx      = 0U;
    lus_length       = len_byte;
    buf_return_value = r_ether_t1s_provide_tx_buffer(p_ctrl,
                                                     ctrl_idx,
                                                     priority,
                                                     &buf_idx,
                                                     (uint8_t **) &p_buf,
                                                     &lus_length);
    if ((buf_return_value == ETHER_T1S_BUFREQ_OK) && (p_buf != ETHER_T1S_NULL_PTR))
    {
        if (ETHER_ZEROCOPY_DISABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
        {
            if (ETHER_T1S_NULL_PTR != buf)
            {
                memcpy(p_buf, buf, len_byte);
            }
        }

#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
        r_ether_t1s_hw_enable_egress_time_stamp(ctrl_idx, buf_idx);
#endif
        result = r_ether_t1s_transmit(p_ctrl,
                                      ctrl_idx,
                                      buf_idx,
                                      p_extend->ether_type,
                                      ETHER_T1S_TRUE,
                                      (uint16_t) len_byte,
                                      p_extend->p_mac_address_target);

        if (ETHER_T1S_NOT_OK == result)
        {
            /* If a callback is provided, then call it with callback argument. */
            r_ether_t1s_call_callback(p_ctrl, ETHER_EVENT_TX_ABORTED);
            return_value = FSP_ERR_IN_USE;
        }
        else
        {
            if (p_extend->ether_t1s_cfg_type_tx_isr == ETHER_T1S_CTRL_POLLING_MODE)
            {
                /* In polling mode, confirm no packet in transmit counter after transmission. */
                do
                {
                    /* Polling for Tx confirmation */
                    r_ether_t1s_tx_confirmation(p_ctrl, ctrl_idx);
                } while (g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.buf_tx_cnt[q_idx] != 0U);

                r_ether_t1s_call_callback(p_ctrl, ETHER_EVENT_TX_COMPLETE);
            }
        }
    }
    else if (buf_return_value == ETHER_T1S_BUFREQ_E_BUSY)
    {
        /* Buffer still in use */
        if (p_extend->ether_t1s_cfg_type_tx_isr == ETHER_T1S_CTRL_POLLING_MODE)
        {
            /* Polling for Tx confirmation */
            r_ether_t1s_tx_confirmation(p_ctrl, ctrl_idx);
        }

        /* If a callback is provided, then call it with callback argument. */
        r_ether_t1s_call_callback(p_ctrl, ETHER_EVENT_TX_ABORTED);
        return_value = FSP_ERR_IN_USE;
    }
    else
    {
        /* Report error when size of length invalid */
        /* If a callback is provided, then call it with callback argument. */
        r_ether_t1s_call_callback(p_ctrl, ETHER_EVENT_TX_ABORTED);
        return_value = FSP_ERR_INVALID_SIZE;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_provide_tx_buffer
 * Description               This API Provides access to a transmit buffer of the specified controller
 * @param[in] p_ctrl         Pointer to control block.
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] priority       Frame priority for transmit buffer FIFO selection
 * @param[in] p_buf_idx      Index to the granted buffer resource
 * @param[in] p_buf          Pointer to the granted buffer
 * @param[in] p_len_byte     Pointer to data length in byte
 ***********************************************************************************************************************/
static ether_t1s_buf_req_return_type_t r_ether_t1s_provide_tx_buffer (ether_t1s_instance_ctrl_t * p_ctrl,
                                                                      uint8_t                     ctrl_idx,
                                                                      uint8_t                     priority,
                                                                      ether_t1s_buf_idx_type    * p_buf_idx,
                                                                      uint8_t                  ** p_buf,
                                                                      uint32_t                  * p_len_byte)
{
    ether_t1s_buf_req_return_type_t return_value;
    return_value =
        r_ether_t1s_get_tx_buffer(p_ctrl, ctrl_idx, priority, (uint32_t *) p_buf_idx, p_buf, (uint16_t *) p_len_byte);

    return return_value;
}

#if (ETHER_T1S_CFG_QOS_SUPPORT)

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_find_tx_queue
 * Description                   This function find the Tx Queue associated
 *                               with the passed priority. Otherwise return the default Queue.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Controller index
 * @param[in] priority           Frame priority for transmit buffer FIFO selection
 ***********************************************************************************************************************/
static ether_t1s_tx_ch_type r_ether_t1s_find_tx_queue (ether_t1s_instance_ctrl_t * p_ctrl,
                                                       uint8_t                     ctrl_idx,
                                                       const uint8_t               priority)

{
    uint8_t ret_q_idx;
    uint8_t idx;
    uint8_t total_priorities;
    ether_t1s_etnf_config_type * lp_hw_unit_config;
    (void) ctrl_idx;
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;
    ret_q_idx         = lp_hw_unit_config->ctrl_priority.dfl_tx_queue;
    total_priorities  = lp_hw_unit_config->ctrl_priority.number_of_priorities;

    for (idx = 0U; idx < total_priorities; idx++)
    {
        if (lp_hw_unit_config->ctrl_priority.p_priority_table[idx].eth_ctrl_priority_value == priority)
        {
            /* Found Traffic associated to the passed Priority */
            /* Return Corresponding Tx Queue and exit */
            ret_q_idx = lp_hw_unit_config->ctrl_priority.p_priority_table[idx].eth_ctrl_tx_queue_id;

            /* Exit the loop */
            break;
        }
        else
        {
            /* No action required */
        }
    }

    return (ether_t1s_tx_ch_type) ret_q_idx;
}

#endif

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_util_get_count_from_list
 * Description                   This Api initialize linked list
 * @param[in] p_list             Linked list
 ***********************************************************************************************************************/
static uint32_t r_ether_t1s_util_get_count_from_list (ether_t1s_list_type_t * const p_list)
{
    uint32_t return_value;

    return_value = 0UL;

    if (ETHER_T1S_NULL_PTR != p_list)
    {
        return_value = p_list->count;
    }
    else
    {
        /* No action required */
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_util_enq_to_list
 * Description                   This Api enqueue into the list (FIFO method)
 * @param[in] p_list             Linked list
 * @param[in] p_elem             Node data
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_util_enq_to_list (ether_t1s_list_type_t * const p_list, void * const p_elem)
{
    ether_t1s_double_list_node_type_t * p_node = (ether_t1s_double_list_node_type_t *) p_elem;
    ether_t1s_type return_value;

    return_value = ETHER_T1S_NOT_OK;

    if ((ETHER_T1S_NULL_PTR != p_list) && (ETHER_T1S_NULL_PTR != p_node) && (ETH_LIST_MAX_NODES != p_list->count))
    {
        if (0UL == p_list->count)
        {
            /* Enqueue at the head */
            p_node->p_prev = ETHER_T1S_NULL_PTR;
            p_list->p_head = p_node;
            p_node->p_next = ETHER_T1S_NULL_PTR;
            p_list->p_last = p_node;
        }
        else
        {
            /* Enqueue at the last */
            p_list->p_last->p_next = p_node;
            p_node->p_next         = ETHER_T1S_NULL_PTR;
            p_node->p_prev         = p_list->p_last;
            p_list->p_last         = p_node;
        }

        p_list->count++;

        return_value = ETHER_T1S_OK;
    }
    else
    {
        /* No action required */
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_get_tx_buffer
 * Description                   Get a buffer from the Tx buffer ring
 * @param[in] p_ctrl             Pointer to control block.
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] priority           Frame priority for transmit buffer FIFO selection
 * @param[in] p_buf_idx          buffer index data
 * @param[in] p_buf              Pointer to the granted buffer
 * @param[in] p_len_byte         Pointer to data length in byte
 ***********************************************************************************************************************/
static ether_t1s_buf_req_return_type_t r_ether_t1s_get_tx_buffer (ether_t1s_instance_ctrl_t    * p_ctrl,
                                                                  uint8_t                        ctrl_idx,
                                                                  uint8_t                        priority,
                                                                  ether_t1s_buf_idx_type * const p_buf_idx,
                                                                  uint8_t ** const               p_buf,
                                                                  uint16_t * const               p_len_byte)
{
    ether_t1s_buf_req_return_type_t return_value = ETHER_T1S_BUFREQ_OK;
    uint32_t                       cfg_idx       = 0U;
    ether_t1s_tx_ch_type           q_id          = ETHER_T1S_TX_BE;
    ether_t1s_buf_handler_type_t * volatile p_tx_buffer_node;
    uint32_t                       ring_idx        = 0U;
    uint32_t                       tx_alloc_cnt    = 0U;
    uint32_t                       header_custom   = ETH_HEADER_SIZE;
    ether_t1s_instance_ctrl_t    * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
    ether_t1s_etnf_config_type   * lp_hw_unit_config;
    ether_t1s_extended_cfg_t     * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    lp_hw_unit_config = (ether_t1s_etnf_config_type *) p_extend->p_hw_unit_config;
    (void) priority;
#if (ETHER_T1S_CFG_QOS_SUPPORT)
    uint32_t cnt;
#endif
    uint16_t req_size;

    /* If the requested size is smaller than the minimum size, expand it to the minimum size */
    if ((uint16_t) ETH_MIN_PAYLOAD_SIZE_AVB > *p_len_byte)
    {
        *p_len_byte = (uint16_t) ETH_MIN_PAYLOAD_SIZE_AVB;
    }
    else
    {
        /* No action required */
    }

    req_size = *p_len_byte;

    /* Get the current tail pointer */
    /* If the requested size is larger than the buffer, return error */
    if (p_extend->header_data_custom == ETHER_T1S_HEADER_ADDED)
    {
        header_custom = 0U;
    }

    if (req_size > (uint16_t) (ETH_MAX_TX_PAYLOAD_LENGTH + header_custom))
    {
        *p_len_byte  = (uint16_t) ((uint16_t) ETH_MAX_TX_PAYLOAD_LENGTH + header_custom);
        return_value = ETHER_T1S_BUFREQ_E_OVFL;
    }
    else
    {
#if (ETHER_T1S_CFG_QOS_SUPPORT)
        q_id = r_ether_t1s_find_tx_queue(p_ctrl, ctrl_idx, priority);
#else

        /* Use BE Queue for any Traffic */
        q_id = ETHER_T1S_TX_BE;
#endif

        /* Disable interrupt for prepare buffer process */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        /* Check the number of buffers allocated for a specific queue */
#if (ETHER_T1S_CFG_QOS_SUPPORT)
        tx_alloc_cnt = 0UL;
        for (cnt = 0UL; cnt < (uint32_t) ETH_TXQ_NUM; cnt++)
        {
            tx_alloc_cnt += r_ether_t1s_util_get_count_from_list(&g_ether_t1s_buffer_lock[ctrl_idx][cnt]);
        }

#else
        tx_alloc_cnt = r_ether_t1s_util_get_count_from_list(&g_ether_t1s_buffer_lock[ctrl_idx][q_id]);
#endif

        if (tx_alloc_cnt < (uint32_t) lp_hw_unit_config->queue_config.p_tx_queue_config[cfg_idx].eth_tx_queue_bufs)
        {
            /* Get the Tx buffer management node and Tx buffer */
            p_tx_buffer_node =
                (ether_t1s_buf_handler_type_t *) r_ether_t1s_ram_alloc(p_ctrl,
                                                                       ctrl_idx,
                                                                       (uint32_t) (req_size + ETH_HEADER_SIZE +
                                                                                   sizeof(ether_t1s_buf_handler_type_t)));
            if (ETHER_T1S_NULL_PTR != p_tx_buffer_node)
            {
                /* Get tx buffer index */
                ring_idx = g_ether_t1s_ctrl_stat[ctrl_idx].tx_buf_tail;

                /* This process never goes into an infinite loop */
                while (ETHER_T1S_FALSE != gp_ether_t1s_tx_buffer_index[ctrl_idx][ring_idx].locked)
                {
                    /* Next buffer index */
                    ring_idx = (ring_idx + 1UL) %
                               lp_hw_unit_config->queue_config.p_tx_queue_config[cfg_idx].eth_tx_queue_bufs;
                }

                /* Set next buffer index */
                g_ether_t1s_ctrl_stat[ctrl_idx].tx_buf_tail = (ring_idx + 1UL) %
                                                              lp_hw_unit_config->queue_config.p_tx_queue_config[cfg_idx]
                                                              .eth_tx_queue_bufs;

                p_tx_buffer_node->buf_idx = ring_idx;
                gp_ether_t1s_tx_buffer_index[ctrl_idx][ring_idx].p_buffer_hdr = p_tx_buffer_node;
                gp_ether_t1s_tx_buffer_index[ctrl_idx][ring_idx].locked       = ETHER_T1S_TRUE;
#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
                gp_ether_t1s_tx_buffer_index[ctrl_idx][ring_idx].time_qual = ETHER_T1S_INVALID;
#endif

                if (ETHER_ZEROCOPY_DISABLE == p_instance_ctrl->p_ether_cfg->zerocopy)
                {
                    p_tx_buffer_node->buf_addr = (uint32_t) p_tx_buffer_node +
                                                 (uint32_t) sizeof(ether_t1s_buf_handler_type_t);
                }
                else
                {
                    p_tx_buffer_node->buf_addr = (uint32_t) g_ether_t1s_tx_buffer_addr;
                }

                p_tx_buffer_node->eth_type_addr = p_tx_buffer_node->buf_addr + ETH_SRC_DST_ADDRESS_SIZE;
                p_tx_buffer_node->tx_length     = 0UL;
                p_tx_buffer_node->channel       = q_id;
                p_tx_buffer_node->enable_ts     = ETHER_T1S_FALSE;
                p_tx_buffer_node->tx_confirm    = ETHER_T1S_FALSE;

                *p_buf_idx = (ether_t1s_buf_idx_type) p_tx_buffer_node->buf_idx;
                *p_buf     = (uint8_t *) (p_tx_buffer_node->buf_addr + (uint32_t) ETH_HEADER_SIZE);

                return_value = ETHER_T1S_BUFREQ_OK;

                /* Enqueue Tx buffer management node */
                (void) r_ether_t1s_util_enq_to_list(&g_ether_t1s_buffer_lock[ctrl_idx][q_id], p_tx_buffer_node);
            }
            else
            {
                return_value = ETHER_T1S_BUFREQ_E_BUSY;
            }
        }
        else
        {
            return_value = ETHER_T1S_BUFREQ_E_BUSY;
        }

        /* Exit critical section */
        FSP_CRITICAL_SECTION_EXIT;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_check_provide_buffer
 * Description                   This function checks whether the memory
 *                               at the specified buffer index has been allocated.
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] buf_Idx            Buffer index
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_check_provide_buffer (uint32_t ctrl_idx, const uint32_t buf_Idx)
{
    ether_t1s_type return_value;

    /* Find tx buffer handler */
    if (ETHER_T1S_TRUE == gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_Idx].locked)
    {
        return_value = ETHER_T1S_OK;
    }
    else
    {
        return_value = ETHER_T1S_NOT_OK;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_pre_process_frame
 ** Description                   This function sets the destination MAC address
 **                               and Ethernet type for the Tx buffer.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] buf_idx            Buffer index
 * @param[in] frame_type         Frame type
 * @param[in] p_phys_addr        Destination address
 ***********************************************************************************************************************/
static void r_ether_t1s_pre_process_frame (ether_t1s_instance_ctrl_t * p_ctrl,
                                           uint32_t                    ctrl_idx,
                                           uint32_t                    buf_idx,
                                           uint32_t                    frame_type,
                                           const uint8_t * const       p_phys_addr)
{
    ether_t1s_data_type          * p_buf_ptr;
    ether_t1s_buf_handler_type_t * p_buf_handler_ptr;

    /* Protection Data _ Critical section */
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

    /* Get Tx buffer address */
    p_buf_handler_ptr = gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx].p_buffer_hdr;

    FSP_CRITICAL_SECTION_EXIT;

    if (ETHER_ZEROCOPY_DISABLE == p_ctrl->p_ether_cfg->zerocopy)
    {
        /* store the address of buffer (Pointer to the Frame in URAM  to be transmitted) */
        p_buf_ptr = (ether_t1s_data_type *) p_buf_handler_ptr->buf_addr;

        /* Copy destination address */
        ETH_COPY_MAC_ADDRESS((uint8_t *) p_phys_addr, (uint8_t *) p_buf_ptr);

        p_buf_ptr = p_buf_ptr + ETH_MACADDR_SIZE;

        ETH_UNPACK_ADDRESS_TO_8(g_ether_t1s_ctrl_stat[ctrl_idx].mac_addr, p_buf_ptr);

        /* Casted to uint8 to extract the required 1 byte. */
        p_buf_ptr  = (ether_t1s_data_type *) p_buf_handler_ptr->eth_type_addr;
        *p_buf_ptr = (ether_t1s_data_type) ((uint8_t) (frame_type >> ETH_BYTE_BITS));
        p_buf_ptr++;

        /* Casted to uint8 to extract the required 1 byte. */
        *p_buf_ptr = (ether_t1s_data_type) ((uint8_t) frame_type);
    }
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_pre_process_frame
 ** Description                   send frame (single frame single buffer)
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] p_tx_buf_hdr       Pointer to Tx buffer
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_etnf_single_desc_frame_send (ether_t1s_instance_ctrl_t          * p_ctrl,
                                                               uint32_t                             ctrl_idx,
                                                               const ether_t1s_buf_handler_type_t * p_tx_buf_hdr)
{
    uint32_t                   buf_idx;
    ether_t1s_type             tx_buffer_provide;
    ether_t1s_data_desc_type * p_data_desc;
    ether_t1s_tx_rx_ctrl       tx_control;
    ether_t1s_type             return_value;
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    tx_buffer_provide = ETHER_T1S_NOT_OK;
    return_value      = ETHER_T1S_OK;

    /* Get next free descriptor */
    p_data_desc = (ether_t1s_data_desc_type *)
                  g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.last_tx_desc[p_tx_buf_hdr->channel];

    if (0UL != p_data_desc->dptr)
    {
        /* Check previous TxBuffer release status (The memory leak protect) */
        buf_idx           = (uint32_t) ((p_data_desc->header.ctrl) & ETH_DESCR_TAG_MASK);
        tx_buffer_provide = r_ether_t1s_check_provide_buffer(ctrl_idx, buf_idx);
    }
    else
    {
        /* No action required */
    }

    if ((ETH_DESC_FEMPTY_AVB == p_data_desc->header.dt) && (ETHER_T1S_NOT_OK == tx_buffer_provide))
    {
        tx_control.word = 0UL;
        if (ETHER_T1S_TRUE == p_tx_buf_hdr->enable_ts)
        {
            tx_control.tx_ctrl.tsr = ETH_DESC_RETAIN_TS;
        }
        else
        {
            /* No action required */
        }

        /* Justify typecast to smaller datatype */
        tx_control.tx_ctrl.tag = p_tx_buf_hdr->buf_idx & ETHER_T1S_TYPECAST_HEADER_TAG;

        /* Configures if MAC transmission status of the frame is stored in MAC status FIFO */
        tx_control.tx_ctrl.msr = ETH_DESC_MAC_FIFO;

        /* build the descriptor */
        p_data_desc->header.die  = (p_tx_buf_hdr->channel + 1U) & ETHER_T1S_TYPECAST_HEADER_DIE;
        p_data_desc->header.ctrl = tx_control.word & ETHER_T1S_TYPECAST_HEADER_CTRL;
        p_data_desc->header.ds   = p_tx_buf_hdr->tx_length & ETHER_T1S_TYPECAST_HEADER_DS;
        if (p_extend->header_data_custom == ETHER_T1S_HEADER_ADDED)
        {
            p_data_desc->dptr = p_tx_buf_hdr->buf_addr;
        }
        else
        {
            if (ETHER_ZEROCOPY_ENABLE == p_ctrl->p_ether_cfg->zerocopy)
            {
                p_data_desc->dptr = p_tx_buf_hdr->buf_addr;
            }
            else
            {
                p_data_desc->dptr = p_tx_buf_hdr->buf_addr + ETH_HEADER_SIZE;
            }
        }

        p_data_desc->header.dt = ETH_DESC_FSINGLE_AVB;

        /* Capture the address of the buffer to be sent */
        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_tx_desc[p_tx_buf_hdr->channel] = p_data_desc->dptr;

        /* Set next descriptor */
        p_data_desc++;
        while ((p_data_desc->header.dt == ETH_DESC_LINKFIX) || (p_data_desc->header.dt == ETH_DESC_LINK_AVB))
        {
            p_data_desc = (ether_t1s_data_desc_type *) p_data_desc->dptr;
        }

        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.last_tx_desc[p_tx_buf_hdr->channel] = (uint32_t) p_data_desc;

        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        /* Increase the number of buffer of current Tx queue */
        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.buf_tx_cnt[p_tx_buf_hdr->channel]++;

        FSP_CRITICAL_SECTION_EXIT;

        /* Transmit start request */
        p_ctrl->p_reg_etnf0->TCCR |= (uint32_t) (1UL << (uint32_t) p_tx_buf_hdr->channel);
    }
    else
    {
        return_value = ETHER_T1S_NOT_OK;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_hw_transmit
 * Description                   Initiates a transmission on an ETHER_T1S controller.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Control index
 * @param[in] buf_idx            Buffer index
 * @param[in] len_byte           Set Tx request length
 * @param[in] confirmation       Set Tx confirmation flag
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_hw_transmit (ether_t1s_instance_ctrl_t * p_ctrl,
                                               uint32_t                    ctrl_idx,
                                               uint32_t                    buf_idx,
                                               uint32_t                    len_byte,
                                               bool                        confirmation)
{
    ether_t1s_type                 return_value;
    ether_t1s_buf_handler_type_t * p_buf_handler_ptr;
    ether_t1s_extended_cfg_t     * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

    /* Get Tx buffer address */
    p_buf_handler_ptr = gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx].p_buffer_hdr;

    FSP_CRITICAL_SECTION_EXIT;

    /* Set Tx request length */
    if (p_extend->header_data_custom == ETHER_T1S_HEADER_ADDED)
    {
        p_buf_handler_ptr->tx_length = (uint32_t) ETH_HEADER_SIZE + len_byte;
    }
    else
    {
        p_buf_handler_ptr->tx_length = len_byte;
    }

    /* Set Tx confirmation flag */
    p_buf_handler_ptr->tx_confirm = confirmation;

    /* Transmit request */
    return_value = r_ether_t1s_etnf_single_desc_frame_send(p_ctrl, ctrl_idx, p_buf_handler_ptr);

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_transmit
 * Description                   This API triggers transmission of a previously filled transmit buffer
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] buf_idx            Index of the buffer resource
 * @param[in] frame_type         Set Tx request length
 * @param[in] tx_confirmation    Activates transmission confirmation
 * @param[in] len_byte           Data length in byte
 * @param[in] p_phys_addr        Physical target address (MAC address)
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_transmit (ether_t1s_instance_ctrl_t * p_ctrl,
                                            uint8_t                     ctrl_idx,
                                            ether_t1s_buf_idx_type      buf_idx,
                                            ether_t1s_frame_type        frame_type,
                                            bool                        tx_confirmation,
                                            uint16_t                    len_byte,
                                            const uint8_t             * p_phys_addr)
{
    volatile ether_t1s_type return_value;
    return_value = r_ether_t1s_check_provide_buffer((uint32_t) ctrl_idx, (uint32_t) buf_idx);
    ether_t1s_extended_cfg_t * p_extend = ((ether_t1s_extended_cfg_t *) p_ctrl->p_ether_cfg->p_extend);
    if (ETHER_T1S_OK != return_value)
    {
        return return_value;
    }

    if (p_extend->header_data_custom == ETHER_T1S_HEADER_ADDED)
    {
        /* Prepare a frame for transmission.*/
        r_ether_t1s_pre_process_frame(p_ctrl,
                                      (uint32_t) ctrl_idx,
                                      (uint32_t) buf_idx,
                                      (uint32_t) frame_type,
                                      p_phys_addr);
    }

    /* Send a transmit request to the low level driver */
    return_value = r_ether_t1s_hw_transmit(p_ctrl,
                                           (uint32_t) ctrl_idx,
                                           (uint32_t) buf_idx,
                                           (uint32_t) len_byte,
                                           tx_confirmation);

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_if_tx_confirmation
 * Description               This API Triggers frame transmission confirmation
 * @param[in] p_instance_ctrl       Pointer to the control block for the channel
 * @param[in] buf_idx       Index of buffer
 ***********************************************************************************************************************/
static void r_ether_t1s_if_tx_confirmation (ether_t1s_instance_ctrl_t * p_instance_ctrl, uint32_t buf_idx)
{
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;
#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
    r_ether_t1s_hw_get_egress_time_stamp(p_instance_ctrl, buf_idx, &g_tx_time_qual, &g_tx_time_stamp[ctrl_idx]);
#else
    (void) buf_idx;
#endif

    g_ether_t1s_tx_confirmed[ctrl_idx] = 1U;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_util_remove_to_list
 * Description               This Api remove node from the list
 * @param[in] p_list         Linked list
 * @param[in] p_elem         Remove node data
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_util_remove_to_list (ether_t1s_list_type_t * const p_list, void ** p_elem)
{
    ether_t1s_double_list_node_type_t ** p_node = (ether_t1s_double_list_node_type_t **) p_elem;
    ether_t1s_double_list_node_type_t  * p_rmv_node;
    ether_t1s_type return_value;

    return_value = ETHER_T1S_NOT_OK;

    if ((ETHER_T1S_NULL_PTR != p_list) && (ETHER_T1S_NULL_PTR != p_node) && (ETHER_T1S_NULL_PTR != *(p_node)))
    {
        if (0UL != p_list->count)
        {
            /* Re-chain */
            p_rmv_node = *p_node;
            if (ETHER_T1S_NULL_PTR == p_rmv_node->p_prev)
            {
                p_list->p_head = p_rmv_node->p_next;
            }
            else
            {
                p_rmv_node->p_prev->p_next = p_rmv_node->p_next;
            }

            if (ETHER_T1S_NULL_PTR == p_rmv_node->p_next)
            {
                p_list->p_last = p_rmv_node->p_prev;
            }
            else
            {
                p_rmv_node->p_next->p_prev = p_rmv_node->p_prev;
            }

            p_list->count--;
            *p_node = p_rmv_node->p_next;

            return_value = ETHER_T1S_OK;
        }
        else
        {
            /* No action required */
        }
    }
    else
    {
        /* No action required */
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_ram_free
 * Description               This free RAM at given address
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] mem_addr       Memory Address need to be freed
 ***********************************************************************************************************************/
static void r_ether_t1s_ram_free (uint32_t ctrl_idx, const uint32_t mem_addr)
{
    ether_t1s_mem_manager_type * p_free_node;

    if (0UL != mem_addr)
    {
        /* Get the address of memory node */
        p_free_node = (ether_t1s_mem_manager_type *) (mem_addr - (uint32_t) sizeof(ether_t1s_mem_manager_type));

        /* If the next node is empty and the next node does not to p_free_node */
        if ((ETHER_T1S_FALSE == p_free_node->p_next->heap_lock) && (p_free_node != p_free_node->p_next))
        {
            if (p_free_node < p_free_node->p_next)
            {
                /* Marge next heap block into the p_free_node */
                if (p_free_node->p_next == g_ether_t1s_ram_manager[ctrl_idx])
                {
                    /* Change heap search start position to the p_free_node */
                    g_ether_t1s_ram_manager[ctrl_idx] = p_free_node;
                }
                else
                {
                    /* No action required */
                }

                /* Re-chain circular list */
                p_free_node->heap_size             += p_free_node->p_next->heap_size;
                p_free_node->p_next->p_next->p_prev = p_free_node;
                p_free_node->p_next                 = p_free_node->p_next->p_next;
            }
            else
            {
                /* No action required */
            }
        }
        else
        {
            /* No action required */
        }

        /* If the prev node is empty and the prev node does not to p_free_node */
        if ((ETHER_T1S_FALSE == p_free_node->p_prev->heap_lock) && (p_free_node != p_free_node->p_prev))
        {
            if (p_free_node > p_free_node->p_prev)
            {
                /* Marge p_free_node into the prev node */
                if (p_free_node == g_ether_t1s_ram_manager[ctrl_idx])
                {
                    /* Change heap search start position to the prev node */
                    g_ether_t1s_ram_manager[ctrl_idx] = p_free_node->p_prev;
                }
                else
                {
                    /* No action required */
                }

                /* Re-chain circular list */
                p_free_node->p_prev->heap_size += p_free_node->heap_size;
                p_free_node->p_next->p_prev     = p_free_node->p_prev;
                p_free_node->p_prev->p_next     = p_free_node->p_next;
            }
            else
            {
                /* No action required */
            }
        }
        else
        {
            /* No action required */
        }

        p_free_node->heap_lock = ETHER_T1S_FALSE;
    }
    else
    {
        /* No action required */
    }
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_release_tx_buffer
 * Description               Release a Tx buffer and store it to the tail of the
 *                           Tx buffer ring
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] buf_idx        Index of a tx buffer
 ***********************************************************************************************************************/
static void r_ether_t1s_release_tx_buffer (uint32_t ctrl_idx, const uint32_t buf_idx)
{
    ether_t1s_buf_handler_type_t * volatile p_temp;
 
    /* Release tx buffer handler */
    p_temp = gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx].p_buffer_hdr;
    (void) r_ether_t1s_util_remove_to_list(&g_ether_t1s_buffer_lock[ctrl_idx][p_temp->channel], (void **) &p_temp);
    r_ether_t1s_ram_free(ctrl_idx, (uint32_t) gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx].p_buffer_hdr);
    gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx].p_buffer_hdr = ETHER_T1S_NULL_PTR;
    gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx].locked       = ETHER_T1S_FALSE;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_t1s_hw_tx_confirmation
 * Description                   Performs transmission processing, notifies the upper layer
 *                               of transmission completion and releases the Tx buffer.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of the controller
 ***********************************************************************************************************************/
static void r_ether_t1s_hw_tx_confirmation (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    uint32_t q_idx;
    uint32_t cnt;
    uint32_t tx_cnt;
    uint32_t buf_idx;
    ether_t1s_data_desc_type           * p_data_desc;
    const ether_t1s_buf_handler_type_t * p_buf_handler_ptr;
#if (ETHER_T1S_CFG_GET_TX_STATS_API)
    ether_t1s_mac_address_type_t mac_addr;
#endif

    /* Get and clear descriptor interrupt status */
    p_ctrl->p_reg_etnf0->DIS = ~p_ctrl->p_reg_etnf0->DIS;

    /* Check the status of the tx descriptor */
    for (q_idx = 0UL; q_idx < (uint32_t) ETH_TXQ_NUM; q_idx++)
    {
        /* Get on-transmit count */
        tx_cnt = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.buf_tx_cnt[q_idx];
        if (0UL == tx_cnt)
        {
            /* Skip because the transmit is not completed */
            continue;
        }

        p_data_desc = (ether_t1s_data_desc_type *) g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.head_tx_desc[q_idx];
        for (cnt = 0UL; cnt < tx_cnt; cnt++)
        {
            if (ETH_DESC_FEMPTY_AVB != p_data_desc->header.dt)
            {
                /* Transmit not completed yet */
                break;
            }
            else
            {
                /* No action required */
            }

            /* Get Tx buffer handler */
            buf_idx           = (uint32_t) ((p_data_desc->header.ctrl) & ETH_DESCR_TAG_MASK);
            p_buf_handler_ptr = gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx].p_buffer_hdr;

#if (ETHER_T1S_CFG_GET_TX_STATS_API)
            g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.tx_number_of_octets += p_buf_handler_ptr->tx_length;
            ETH_PACK_ADDRESS_FROM_8(((uint8_t *) p_buf_handler_ptr->buf_addr), mac_addr);
            if ((0UL == ETHER_COMPARE_MAC(mac_addr, g_ether_t1s_broadcast_addr)) ||
                (0UL != ETH_CHECK_MULTICAST(mac_addr)))
            {
                g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.tx_nu_cast_pkts++;
            }
            else
            {
                g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.tx_uni_cast_pkts++;
            }
#endif

            /* Check whether the transmission confirmation was enabled */
            if (ETHER_T1S_TRUE == p_buf_handler_ptr->tx_confirm)
            {
                /* Call the callback function */
                /* Since the maximum value of controller index is 1, casting to uint8 does no problem. */
                r_ether_t1s_if_tx_confirmation(p_ctrl, p_buf_handler_ptr->buf_idx);
            }
            else
            {
                /* No action required */
            }

            FSP_CRITICAL_SECTION_DEFINE;
            FSP_CRITICAL_SECTION_ENTER;

            /* Release the tx buffer resource */
            r_ether_t1s_release_tx_buffer(ctrl_idx, p_buf_handler_ptr->buf_idx);

            /* Clear tx descriptor */
            p_data_desc->header.die  = 0U;
            p_data_desc->header.ctrl = 0U;
            p_data_desc->header.ds   = 0U;
            p_data_desc->dptr        = 0U;

            /* Decrease the number of on-transmission buffer of current Tx queue */
            g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.buf_tx_cnt[q_idx]--;

            FSP_CRITICAL_SECTION_EXIT;

            /* Update start descriptor */
            p_data_desc++;
            while ((p_data_desc->header.dt == ETH_DESC_LINKFIX) || (p_data_desc->header.dt == ETH_DESC_LINK_AVB))
            {
                p_data_desc = (ether_t1s_data_desc_type *) p_data_desc->dptr;
            }

            g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.head_tx_desc[q_idx] = (uint32_t) p_data_desc;
        }
    }
}

/*******************************************************************************************************************//**
 * Function Name         r_ether_t1s_tx_confirmation
 * Description           This API Triggers frame transmission confirmation
 * @param[in] p_ctrl     Pointer to control block.
 * @param[in] ctrl_idx   Index of the controller
 ***********************************************************************************************************************/
static void r_ether_t1s_tx_confirmation (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    if (ETHER_T1S_MODE_ACTIVE == g_ether_t1s_ctrl_stat[ctrl_idx].mode)
    {
        r_ether_t1s_hw_tx_confirmation(p_ctrl, ctrl_idx);
    }
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_hw_uf_counter_get
 * Description               Get Unread Frame Counter
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] queue_idx      Rx queue index
 ***********************************************************************************************************************/
static uint32_t r_ether_t1s_hw_uf_counter_get (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx,
                                               uint32_t queue_idx)
{
    uint32_t ufcv;
    uint32_t uf_count;
    uint32_t ufcvi;
    (void) ctrl_idx;
    ufcvi = ETH_ETNF_GET_UFCVi(queue_idx);
    switch (ufcvi)
    {
        case 0:
        {
            ufcv = p_ctrl->p_reg_etnf0->UFCV0;
            break;
        }

        case 1:
        {
            ufcv = p_ctrl->p_reg_etnf0->UFCV1;
            break;
        }

        case 2:
        {
            ufcv = p_ctrl->p_reg_etnf0->UFCV2;
            break;
        }

        case 3:
        {
            ufcv = p_ctrl->p_reg_etnf0->UFCV3;
            break;
        }

        default:
            ufcv = p_ctrl->p_reg_etnf0->UFCV4;
    }

    uf_count = ETH_ETNF_GET_UFCVi_CVr(ufcv, queue_idx);

    return uf_count;
}

/*******************************************************************************************************************//**
 * Function Name                    r_ether_t1s_rx_be_process
 * Description                      This process best effort queue
 *                                  (The PS bit isn't checked.)
 * @param[in] ctrl_idx              Ethernet channel number
 * @param[in] p_single_rx_frame_ptr pointer to rx frame
 * @param[in] desc_ptr              pointer to rx Descriptor
 ***********************************************************************************************************************/
static void r_ether_t1s_rx_be_process (uint32_t                          ctrl_idx,
                                       ether_t1s_rx_frame_type_t * const p_single_rx_frame_ptr,
                                       const uint32_t                    desc_ptr)
{
    ether_t1s_data_desc_type     * p_data_desc;
    ether_t1s_ext_data_desc_type * p_ext_data_desc;
    ether_t1s_option_type_t        rx_be_time_stamp;

    rx_be_time_stamp = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_be_time_stamp;

    /* single frame single buffer */
    if (ETHER_T1S_ENABLE == rx_be_time_stamp)
    {
        /* timestamp enabled */
        p_ext_data_desc = (ether_t1s_ext_data_desc_type *) desc_ptr;

        if (ETH_DESC_FSINGLE_AVB == p_ext_data_desc->header.dt)
        {
            /* descriptor type correct */
            p_single_rx_frame_ptr->frame_addr    = p_ext_data_desc->dptr;
            p_single_rx_frame_ptr->eth_type_addr = p_ext_data_desc->dptr + ETH_SRC_DST_ADDRESS_SIZE;
            p_single_rx_frame_ptr->frame_length  = p_ext_data_desc->header.ds;
            p_single_rx_frame_ptr->time_stamp    = p_ext_data_desc->time_stamp;
            p_single_rx_frame_ptr->time_qual     = ETHER_T1S_VALID;
        }
        else
        {
            /* descriptor type incorrect */
            p_single_rx_frame_ptr->frame_addr   = 0UL;
            p_single_rx_frame_ptr->frame_length = 0UL;
        }

        p_ext_data_desc++;
        while ((p_ext_data_desc->header.dt == ETH_DESC_LINKFIX) ||
               (p_ext_data_desc->header.dt == ETH_DESC_LINK_AVB))
        {
            p_ext_data_desc = (ether_t1s_ext_data_desc_type *) p_ext_data_desc->dptr;
        }

        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[(uint32_t) ETH_BECHANNEL] =
            (uint32_t) p_ext_data_desc;
    }
    else
    {
        /* timestamp disabled */
        p_data_desc = (ether_t1s_data_desc_type *) desc_ptr;

        if (ETH_DESC_FSINGLE_AVB == p_data_desc->header.dt)
        {
            /* descriptor type correct */
            p_single_rx_frame_ptr->frame_addr    = p_data_desc->dptr;
            p_single_rx_frame_ptr->eth_type_addr = p_data_desc->dptr + ETH_SRC_DST_ADDRESS_SIZE;
            p_single_rx_frame_ptr->frame_length  = p_data_desc->header.ds;
            p_single_rx_frame_ptr->time_qual     = ETHER_T1S_INVALID;
        }
        else
        {
            /* descriptor type incorrect */
            p_single_rx_frame_ptr->frame_addr   = 0UL;
            p_single_rx_frame_ptr->frame_length = 0UL;
        }

        p_data_desc++;

        while ((p_data_desc->header.dt == ETH_DESC_LINKFIX) ||
               (p_data_desc->header.dt == ETH_DESC_LINK_AVB))
        {
            p_data_desc = (ether_t1s_data_desc_type *) p_data_desc->dptr;
        }

        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[(uint32_t) ETH_BECHANNEL] = (uint32_t) p_data_desc;
    }
}

/*******************************************************************************************************************//**
 * Function Name                    r_ether_t1s_rx_nc_process
 * Description                      This  process network control queue
 *                                  (The PS bit isn't checked.)
 * @param[in] ctrl_idx              Ethernet channel number
 * @param[in] p_single_rx_frame_ptr pointer to rx frame
 * @param[in] desc_ptr              pointer to rx Descriptor
 ***********************************************************************************************************************/
static void r_ether_t1s_rx_nc_process (uint32_t                          ctrl_idx,
                                       ether_t1s_rx_frame_type_t * const p_single_rx_frame_ptr,
                                       const uint32_t                    desc_ptr)
{
    ether_t1s_ext_data_desc_type * p_ext_data_desc;

    p_ext_data_desc = (ether_t1s_ext_data_desc_type *) desc_ptr;

    if (ETH_DESC_FSINGLE_AVB == p_ext_data_desc->header.dt)
    {
        /* descriptor type correct */
        p_single_rx_frame_ptr->frame_addr    = p_ext_data_desc->dptr;
        p_single_rx_frame_ptr->eth_type_addr = p_ext_data_desc->dptr + ETH_SRC_DST_ADDRESS_SIZE;
        p_single_rx_frame_ptr->frame_length  = p_ext_data_desc->header.ds;
        p_single_rx_frame_ptr->time_stamp    = p_ext_data_desc->time_stamp;
        p_single_rx_frame_ptr->time_qual     = ETHER_T1S_VALID;
    }
    else
    {
        /* descriptor type incorrect */
        p_single_rx_frame_ptr->frame_addr   = 0UL;
        p_single_rx_frame_ptr->frame_length = 0UL;
    }

    p_ext_data_desc++;
    while ((p_ext_data_desc->header.dt == ETH_DESC_LINKFIX) ||
           (p_ext_data_desc->header.dt == ETH_DESC_LINK_AVB))
    {
        p_ext_data_desc = (ether_t1s_ext_data_desc_type *) p_ext_data_desc->dptr;
    }

    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[(uint32_t) ETH_NCCHANNEL] = (uint32_t) p_ext_data_desc;
}

/*******************************************************************************************************************//**
 * Function Name                    r_ether_t1s_rx_s_process
 * Description                      This  process stream queue
 *                                  (The PS bit isn't checked.)
 * @param[in] ctrl_idx              Ethernet channel number
 * @param[in] p_single_rx_frame_ptr pointer to rx frame
 * @param[in] channel_num           stream channel number
 * @param[in] desc_ptr              pointer to rx Descriptor
 ***********************************************************************************************************************/
static void r_ether_t1s_rx_s_process (uint32_t                          ctrl_idx,
                                      ether_t1s_rx_frame_type_t * const p_single_rx_frame_ptr,
                                      const uint32_t                    channel_num,
                                      const uint32_t                    desc_ptr)
{
    ether_t1s_data_desc_type     * p_data_desc;
    ether_t1s_ext_data_desc_type * p_ext_data_desc;
    ether_t1s_option_type_t        rx_s_time_stamp;

    rx_s_time_stamp = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.rx_s_time_stamp;

    /* single frame single buffer */
    if (ETHER_T1S_ENABLE == rx_s_time_stamp)
    {
        /* timestamp enabled */
        p_ext_data_desc = (ether_t1s_ext_data_desc_type *) desc_ptr;

        if (ETH_DESC_FSINGLE_AVB == p_ext_data_desc->header.dt)
        {
            /* descriptor type correct */
            p_single_rx_frame_ptr->frame_addr    = p_ext_data_desc->dptr;
            p_single_rx_frame_ptr->eth_type_addr = p_ext_data_desc->dptr + ETH_SRC_DST_ADDRESS_SIZE;
            p_single_rx_frame_ptr->frame_length  = p_ext_data_desc->header.ds;
            p_single_rx_frame_ptr->time_stamp    = p_ext_data_desc->time_stamp;
            p_single_rx_frame_ptr->time_qual     = ETHER_T1S_VALID;
        }
        else
        {
            /* descriptor type incorrect */
            p_single_rx_frame_ptr->frame_addr   = 0UL;
            p_single_rx_frame_ptr->frame_length = 0UL;
        }

        p_ext_data_desc++;
        while ((p_ext_data_desc->header.dt == ETH_DESC_LINKFIX) ||
               (p_ext_data_desc->header.dt == ETH_DESC_LINK_AVB))
        {
            p_ext_data_desc = (ether_t1s_ext_data_desc_type *) p_ext_data_desc->dptr;
        }

        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[channel_num] = (uint32_t) p_ext_data_desc;
    }
    else
    {
        /* timestamp disabled */
        p_data_desc = (ether_t1s_data_desc_type *) desc_ptr;

        if (ETH_DESC_FSINGLE_AVB == p_data_desc->header.dt)
        {
            /* descriptor type correct */
            p_single_rx_frame_ptr->frame_addr    = p_data_desc->dptr;
            p_single_rx_frame_ptr->eth_type_addr = p_data_desc->dptr + ETH_SRC_DST_ADDRESS_SIZE;
            p_single_rx_frame_ptr->frame_length  = p_data_desc->header.ds;
            p_single_rx_frame_ptr->time_qual     = ETHER_T1S_INVALID;
        }
        else
        {
            /* descriptor type incorrect */
            p_single_rx_frame_ptr->frame_addr   = 0UL;
            p_single_rx_frame_ptr->frame_length = 0UL;
        }

        p_data_desc++;
        while ((p_data_desc->header.dt == ETH_DESC_LINKFIX) ||
               (p_data_desc->header.dt == ETH_DESC_LINK_AVB))
        {
            p_data_desc = (ether_t1s_data_desc_type *) p_data_desc->dptr;
        }

        g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[channel_num] = (uint32_t) p_data_desc;
    }
}

#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)

/*******************************************************************************************************************//**
 * Function Name                    r_ether_t1s_is_rx_frame_valid
 * Description                      Compare Mac Address Frame just received with
 *                                  Broadcast and Controller Mac Address.
 * @param[in] ctrl_idx              Ethernet channel number
 * @param[in] p_rx_frame            Pointer to the Frame just received
 ***********************************************************************************************************************/
static bool r_ether_t1s_is_rx_frame_valid (uint32_t ctrl_idx, const ether_t1s_rx_frame_type_t * const p_rx_frame)
{
    ether_t1s_mac_address_type_t dst_address;
    bool     pass;
    uint32_t filter_idx;
    uint32_t remain_bits;

    pass = ETHER_T1S_FALSE;
    ETH_PACK_ADDRESS_FROM_8(((uint8_t *) p_rx_frame->frame_addr), dst_address);

    /* Check whether promiscuous mode, broadcast, own unicast, multicast address is not registed */
    if ((ETHER_T1S_TRUE == g_ether_t1s_ctrl_stat[ctrl_idx].promiscuous) ||
        (0UL == ETHER_COMPARE_MAC(dst_address, g_ether_t1s_broadcast_addr)) ||
        (0UL == ETHER_COMPARE_MAC(dst_address, g_ether_t1s_ctrl_stat[ctrl_idx].mac_addr)))
    {
        pass = ETHER_T1S_TRUE;
    }
    else
    {
        /* Otherwise, do the filter operation */

        /* To skip empty filters efficiently, copy active bits to the local var */
        remain_bits = g_ether_t1s_ctrl_stat[ctrl_idx].active_filter_bits;
        filter_idx  = 0UL;

        /* Loop until any filter hits or pass all active filters */
        while (0UL != remain_bits)
        {
            if ((0UL != (remain_bits & (1UL << filter_idx))) &&
                (0UL == ETHER_COMPARE_MAC(dst_address, g_ether_t1s_address_filters[ctrl_idx][filter_idx])))
            {
                pass = ETHER_T1S_TRUE;
                break;
            }
            else
            {
                /* No action required */
            }

            /* Clear the compared filter bit */
            remain_bits &= ~(1UL << filter_idx);
            filter_idx++;
        }
    }

    return pass;
}

#endif                                 /* (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER) */

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_if_rx_indication
 * Description               Since the maximum value of Controller Index is 1, casting to uint8 does no problem
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] frame_type     Set Rx request length
 * @param[in] p_phys_addr    Pointer to the Frame Address just received
 * @param[in] p_data         Pointer to data received
 * @param[in] len_byte       lenght data to receive
 ***********************************************************************************************************************/
static void r_ether_t1s_if_rx_indication (ether_t1s_instance_ctrl_t * p_ctrl,
                                          uint8_t                     ctrl_idx,
                                          ether_t1s_frame_type        frame_type,
                                          const uint8_t             * p_phys_addr,
                                          const ether_t1s_type      * p_data,
                                          uint16_t                    len_byte)
{
    /* LenByte contains the payload length */
    g_ether_t1s_msg_length[ctrl_idx] = len_byte;
    if (ETHER_ZEROCOPY_DISABLE == p_ctrl->p_ether_cfg->zerocopy)
    {
        /* Memcpy Received message */
        memcpy((uint8_t *) g_ether_t1s_rx_eth_frame[ctrl_idx], (uint8_t *) p_data, g_ether_t1s_msg_length[ctrl_idx]);
        memcpy(&g_ether_t1s_rx_src_addr[ctrl_idx], p_phys_addr, ETH_MACADDR_SIZE);
    }

    g_ether_t1s_rx_frame_type[ctrl_idx] = frame_type;
#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)
    r_ether_t1s_hw_get_ingress_time_stamp(ctrl_idx, (ether_t1s_data_type *) p_data, &g_rx_time_qual,
                                          &g_rx_time_stamp[ctrl_idx]);
#endif

    /* Store entire frame size (with header) to g_ether_t1s_rx_len_byte */
    g_ether_t1s_rx_len_byte[ctrl_idx] = len_byte;
    g_ether_t1s_rx_frame_cnt[ctrl_idx]++;
}

/*******************************************************************************************************************//**
 * Function Name             ether_t1s_rx_call_eth_if
 * Description               Wrapper for the Callback to the Eth Interface for each frame received
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] p_frame        Address of the Received Frame in URAM
 ***********************************************************************************************************************/
static void ether_t1s_rx_call_eth_if (ether_t1s_instance_ctrl_t             * p_ctrl,
                                      uint32_t                                ctrl_idx,
                                      const ether_t1s_rx_frame_type_t * const p_frame)
{
    const ether_t1s_data_type  * p_frame_type;
    ether_t1s_frame_type         frame_type;
    ether_t1s_mac_address_type_t mac_addr;

    const ether_t1s_frame_type_etnf * ether_frame;

    ether_frame = (const ether_t1s_frame_type_etnf *) p_frame->frame_addr;
    if (ether_frame != NULL)
    {
        ETH_PACK_ADDRESS_FROM_8(ether_frame->dst_addr, mac_addr);
    }
    else
    {
        /* No action required */
    }

    if (0UL == ETHER_COMPARE_MAC(mac_addr, g_ether_t1s_broadcast_addr))
    {
#if (ETHER_T1S_CFG_GET_RX_STATS_API)
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.stats_broad_cast_pkts++;
#endif
    }
    else
    {
#if (ETHER_T1S_CFG_GET_RX_STATS_API)
        if (0UL == ETH_CHECK_MULTICAST(mac_addr))
        {
            /* Unicast frame */
            g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_unicast_frames++;
        }
        else
        {
            /* No action required */
        }
#endif
    }

    p_frame_type = (const ether_t1s_data_type *) p_frame->eth_type_addr;
    frame_type   = (ether_t1s_frame_type) ((uint32_t) p_frame_type[0] << ETH_BYTE_BITS);
    frame_type  |= (ether_t1s_frame_type) p_frame_type[1];

    /* Since the maximum value of Controller Index is 1, casting to uint8 does no problem. */
    r_ether_t1s_if_rx_indication(p_ctrl,
                                 (uint8_t) ctrl_idx,
                                 frame_type,
                                 ether_frame->src_addr,
                                 (ether_t1s_type *) ether_frame,
                                 (uint16_t) (p_frame->frame_length));
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_hw_rx_queue_process
 * Description               Process Receive Queue.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] queue_idx      Rx queue index
 ***********************************************************************************************************************/
static bool r_ether_t1s_hw_rx_queue_process (ether_t1s_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx, uint32_t queue_idx)
{
    uint32_t desc_addr;
    bool     rx_frame_valid;
    uint32_t ufcdi;
    const ether_t1s_rx_ch_config_type * p_ch_config;

#if (ETHER_T1S_CFG_GET_RX_STATS_API)
    uint32_t length_with_fcs;
#endif

    rx_frame_valid = ETHER_T1S_TRUE;

    /*Get Rx queue configuration */
    p_ch_config = &g_ether_t1s_rx_config[queue_idx];

    desc_addr = g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.next_rx_desc[p_ch_config->ch_num];

    /* Get current rx descriptor address */
    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_rx_desc_update[p_ch_config->ch_num]  = desc_addr;
    g_ether_t1s_ctrl_stat[ctrl_idx].hw_stat_etnf.current_rx_desc_release[p_ch_config->ch_num] = desc_addr;

    if (ETH_BECHANNEL == p_ch_config->ch_num)
    {
        /* best effort channel */
        r_ether_t1s_rx_be_process(ctrl_idx, &g_ether_t1s_rx_frame[ctrl_idx], desc_addr);

#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)

        /* check whether received frame is valid or not */
        rx_frame_valid = r_ether_t1s_is_rx_frame_valid(ctrl_idx, &g_ether_t1s_rx_frame[ctrl_idx]);
#endif
    }
    else if (ETH_NCCHANNEL == p_ch_config->ch_num)
    {
        /* network control channel */
        r_ether_t1s_rx_nc_process(ctrl_idx, &g_ether_t1s_rx_frame[ctrl_idx], desc_addr);
    }
    else
    {
        /* stream channel */
        r_ether_t1s_rx_s_process(ctrl_idx, &g_ether_t1s_rx_frame[ctrl_idx], (uint32_t) p_ch_config->ch_num, desc_addr);

#if (ETHER_T1S_CFG_UPDATE_PHYS_ADDR_FILTER)

        /* check whether received frame is valid or not */
        rx_frame_valid = r_ether_t1s_is_rx_frame_valid(ctrl_idx, &g_ether_t1s_rx_frame[ctrl_idx]);
#endif
    }

    /* Decrement ETNFnUFCD.DVr by 1 */
    ufcdi = ETH_ETNF_GET_UFCDi(queue_idx);
    switch (ufcdi)
    {
        case 0:
        {
            p_ctrl->p_reg_etnf0->UFCD0 = ETH_ETNF_SET_UFCDi_DVr(1UL, queue_idx);
            break;
        }

        case 1:
        {
            p_ctrl->p_reg_etnf0->UFCD1 = ETH_ETNF_SET_UFCDi_DVr(1UL, queue_idx);
            break;
        }

        case 2:
        {
            p_ctrl->p_reg_etnf0->UFCD2 = ETH_ETNF_SET_UFCDi_DVr(1UL, queue_idx);
            break;
        }

        case 3:
        {
            p_ctrl->p_reg_etnf0->UFCD3 = ETH_ETNF_SET_UFCDi_DVr(1UL, queue_idx);
            break;
        }

        default:
            p_ctrl->p_reg_etnf0->UFCD4 = ETH_ETNF_SET_UFCDi_DVr(1UL, queue_idx);
    }

    if (ETHER_T1S_TRUE == rx_frame_valid)
    {
#if (ETHER_T1S_CFG_GET_RX_STATS_API)
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts++;
        length_with_fcs = g_ether_t1s_rx_frame[ctrl_idx].frame_length + ETH_FCS_LENGTH;
        g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_octets += length_with_fcs;
        if (length_with_fcs <= 64UL)
        {
            /* Since the receive data size that does not include padding data is set in the receive descriptor,
             * frames of 64 or less are collected by the statistical counter. */
            g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts64_octets++;
        }
        else if (length_with_fcs <= 127UL)
        {
            g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts65_to_127_octets++;
        }
        else if (length_with_fcs <= 255UL)
        {
            g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts128_to_255_octets++;
        }
        else if (length_with_fcs <= 511UL)
        {
            g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts256_to_511_octets++;
        }
        else if (length_with_fcs <= 1023UL)
        {
            g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts512_to_1023_octets++;
        }
        else
        {
            /* When VLAN tag is supported, the maximum data size is 1522. */
            /* The maximum frame size that HW can receive is set to 1522. */
            g_ether_t1s_ctrl_stat[ctrl_idx].tx_rx_stat.rx_stats_pkts1024_to_1518_octets++;
        }
#endif

        /* Call EthIf if the Frame Received is valid */
        ether_t1s_rx_call_eth_if(p_ctrl, ctrl_idx, &g_ether_t1s_rx_frame[ctrl_idx]);
    }
    else
    {
        /* Frame Invalid - E.g. Multicast to be discarded */
        /* No Call of EthIf */
        if (ETHER_ZEROCOPY_ENABLE == p_ctrl->p_ether_cfg->zerocopy)
        {
            /* update descriptor chain */
            (void) r_ether_t1s_rx_desc_chain_update(ctrl_idx, desc_addr, p_ch_config);
        }
    }

    if (ETHER_ZEROCOPY_DISABLE == p_ctrl->p_ether_cfg->zerocopy)
    {
        /* update descriptor chain */
        (void) r_ether_t1s_rx_desc_chain_update(ctrl_idx, desc_addr, p_ch_config);
    }

    return rx_frame_valid;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_hw_receive
 * Description               Performs reception processing in polling mode.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] queue_idx      Rx queue index
 ***********************************************************************************************************************/
static ether_t1s_rx_status_type_t r_ether_t1s_hw_receive (ether_t1s_instance_ctrl_t * p_ctrl,
                                                          uint32_t                    ctrl_idx,
                                                          uint32_t                    queue_idx)
{
    uint32_t cnt;
    uint32_t unread_frame_cnt;
    bool     queue_valid;
    ether_t1s_rx_status_type_t ret_value;

    /* Initialize receive status */
    ret_value = ETHER_T1S_NOT_RECEIVED;

    /* Read UFCV from the ctrl_idx and queue_idx */
    unread_frame_cnt = r_ether_t1s_hw_uf_counter_get(p_ctrl, ctrl_idx, queue_idx);

    for (cnt = 0UL; cnt < unread_frame_cnt; cnt++)
    {
        queue_valid = r_ether_t1s_hw_rx_queue_process(p_ctrl, ctrl_idx, (uint8_t) queue_idx);
        if (ETHER_T1S_TRUE == queue_valid)
        {
            /* A frame is processed, decrease the counter */
            unread_frame_cnt = unread_frame_cnt - cnt;
            if (1UL < unread_frame_cnt)
            {
                /* More frames are available */
                ret_value = ETHER_T1S_RECEIVED_MORE_DATA_AVAILABLE;
            }
            else
            {
                /* Frame valid - Update status */
                ret_value = ETHER_T1S_RECEIVED;
            }

            break;
        }
        else
        {
            /* No action required */
        }
    }

    return ret_value;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_receive
 * Description               This API Triggers frame reception
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] queue_idx      Specifies the related FIFO
 * @param[in] p_rx_status    Pointer to check status
 ***********************************************************************************************************************/
static void r_ether_t1s_receive (ether_t1s_instance_ctrl_t  * p_ctrl,
                                 uint8_t                      queue_idx,
                                 ether_t1s_rx_status_type_t * p_rx_status)
{
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;
    if (ETHER_T1S_NULL_PTR != p_rx_status)
    {
        /* Initial Value Set */
        *p_rx_status = ETHER_T1S_NOT_RECEIVED;
    }

    if (ETHER_T1S_MODE_ACTIVE == g_ether_t1s_ctrl_stat[ctrl_idx].mode)
    {
        *p_rx_status = r_ether_t1s_hw_receive(p_ctrl, (uint32_t) ctrl_idx, (uint32_t) queue_idx);

        r_ether_t1s_call_callback(p_ctrl, ETHER_EVENT_RX_COMPLETE);
    }
}

/*******************************************************************************************************************//**
 * @brief Verifies the Etherent link is up or not.
 *
 * @param[in] p_instance_ctrl   Pointer to the control block for the channel
 *
 * @retval  FSP_SUCCESS:                  Link is up
 * @retval  FSP_ERR_ETHER_ERROR_LINK:     Link is down
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK  Initialization of PHY-LSI failed.
 **********************************************************************************************************************/
static fsp_err_t ether_link_status_check (ether_t1s_instance_ctrl_t const * const p_instance_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    fsp_err_t link_status;

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
}                                      /* End of function ether_link_status_check() */

#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)

/*******************************************************************************************************************//**
 * Function Name: r_ether_t1s_hw_get_egress_time_stamp
 * Read TimeStamp from a message just transmitted
 * @param[in]     p_instance_ctrl     Pointer to the control block
 * @param[in]     buff_idx            Buffer index
 * @param[in]     p_time_qual         Quality of the Time Stamp
 * @param[in]     p_time_stamp        TS read from the Tx
 **********************************************************************************************************************/
static void r_ether_t1s_hw_get_egress_time_stamp (ether_t1s_instance_ctrl_t      * p_instance_ctrl,
                                                  uint32_t                         buf_idx,
                                                  ether_t1s_time_stamp_qual_type * p_time_qual,
                                                  ether_t1s_time_stamp_type      * p_time_stamp)
{
    volatile uint8_t             fifo_counter;
    uint16_t                     tx_buffer_idx;
    ether_t1s_tx_buffer_type_t * p_tx_buffer;
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;

    /* Get timestamp fifo counter by reading AVBnTSR.TFFL */
    fifo_counter = (uint8_t) (ETH_AVB_GET_TFFL(p_instance_ctrl->p_reg_etnf0->TSR));
    while (0UL < fifo_counter)
    {
        /* Read buffer index from AVBnTFA2.TST */
        tx_buffer_idx = (uint16_t) (ETH_AVB_GET_TST(p_instance_ctrl->p_reg_etnf0->TFA2));

        /* Read timestamp value to Tx buffer */
        p_tx_buffer = (ether_t1s_tx_buffer_type_t *) &gp_ether_t1s_tx_buffer_index[ctrl_idx][tx_buffer_idx];

        /* Set timestamp quality information as ETH_VALID */
        p_tx_buffer->time_qual = ETHER_T1S_VALID;

        p_tx_buffer->time_stamp.nanoseconds = p_instance_ctrl->p_reg_etnf0->TFA0;
        p_tx_buffer->time_stamp.seconds     = p_instance_ctrl->p_reg_etnf0->TFA1;
        p_tx_buffer->time_stamp.secondsHi   = (uint16_t) (p_instance_ctrl->p_reg_etnf0->TFA2 & ETH_TFA2_TSV_MASK);

        /* Release oldest entry in Timestamp FIFO - TCCR.TFR = 1 */
        p_instance_ctrl->p_reg_etnf0->TCCR |= ETH_AVB_TCCR_TFR;
        fifo_counter--;
    }

    p_tx_buffer   = (ether_t1s_tx_buffer_type_t *) &gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx];
    *p_time_qual  = p_tx_buffer->time_qual;
    *p_time_stamp = p_tx_buffer->time_stamp;
}

/*******************************************************************************************************************//**
 * Function Name: r_ether_t1s_hw_get_ingress_time_stamp
 * Read TimeStamp from a message received.
 * @param[in]     ctrl_idx            Index of a controller
 * @param[in]     p_data              Pointer to the message buffer
 * @param[in]     p_time_qual         Quality of the Time Stamp
 * @param[in]     p_time_stamp        TS read from the Rx
 **********************************************************************************************************************/
static void r_ether_t1s_hw_get_ingress_time_stamp (uint8_t                          ctrl_idx,
                                                   ether_t1s_data_type            * p_data,
                                                   ether_t1s_time_stamp_qual_type * p_time_qual,
                                                   ether_t1s_time_stamp_type      * p_time_stamp)
{
    ether_t1s_data_type * p_compare;
    p_compare = (ether_t1s_data_type *) (g_ether_t1s_rx_frame[ctrl_idx].frame_addr);
    if (p_compare == p_data)
    {
        p_time_stamp->nanoseconds = g_ether_t1s_rx_frame[ctrl_idx].time_stamp.time_stamp0;
        p_time_stamp->seconds     = g_ether_t1s_rx_frame[ctrl_idx].time_stamp.time_stamp1;
        p_time_stamp->secondsHi   = g_ether_t1s_rx_frame[ctrl_idx].time_stamp.time_stamp2;

        /* TimeStamp is valid  */
        *p_time_qual = ETHER_T1S_VALID;
    }
    else
    {
        p_time_stamp->nanoseconds = 0UL;
        p_time_stamp->seconds     = 0UL;
        p_time_stamp->secondsHi   = (uint16_t) 0U;

        /* TimeStamp is invalid  */
        *p_time_qual = ETHER_T1S_INVALID;
    }
}

/*******************************************************************************************************************//**
 * Enable TimeStamp capture for the message that will be transmitted
 * @param[in] ctrl_idx                  Index of a controller
 * @param[in] buf_idx                   Buffer index of Tx buffer
 * @retval ETHER_T1S_OK                 Enable egress time successful
 * @retval ETHER_T1S_NOT_OK             Cannot enable egress time
 **********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_hw_enable_egress_time_stamp (uint8_t ctrl_idx, uint32_t buf_idx)
{
    ether_t1s_buf_handler_type_t * p_buf_handler;
    volatile ether_t1s_type        return_value = ETHER_T1S_NOT_OK;
    return_value = r_ether_t1s_check_provide_buffer((uint32_t) ctrl_idx, (uint32_t) buf_idx);

    if (return_value == ETHER_T1S_OK)
    {
        p_buf_handler = (ether_t1s_buf_handler_type_t *) gp_ether_t1s_tx_buffer_index[ctrl_idx][buf_idx].p_buffer_hdr;
        if (NULL != p_buf_handler)
        {
            p_buf_handler->enable_ts = ETHER_T1S_TRUE;
            return_value             = ETHER_T1S_OK;
        }
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Enable TimeStamp capture for the message that will be transmitted
 * @param[in] p_instance_ctrl         Pointer to the control block
 * @param[in] inc_val                 Increment value for gPTP
 * @retval ETHER_T1S_OK               Set increment time successful
 * @retval ETHER_T1S_NOT_OK           Cannot set increment time
 **********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_set_increment_time (ether_t1s_instance_ctrl_t * p_instance_ctrl, uint32_t inc_val)
{
    volatile ether_t1s_type return_value = ETHER_T1S_OK;
    uint32_t                operation_mode;
    uint32_t                load_time_inc;

    operation_mode = p_instance_ctrl->p_reg_etnf0->CCC & ETH_ETNF_CCC_OPC_MASK;
    load_time_inc  = p_instance_ctrl->p_reg_etnf0->GCCR & ETH_ETNF_GCCR_LTI;

    if (0UL == ((ETH_ETNF_CCC_OPC_OPERATION == operation_mode) && (0UL != load_time_inc)))
    {
        /* Set GTI */
        p_instance_ctrl->p_reg_etnf0->GTI = inc_val;

        /* Set Request to load LTI */
        p_instance_ctrl->p_reg_etnf0->GCCR |= ETH_ETNF_GCCR_LTI;

        return_value = r_ether_t1s_wait_ptp_request_completed(p_instance_ctrl, ETH_ETNF_GCCR_LTI);
    }
    else
    {
        /* Another Request is pending to gPTP */
        return_value = ETHER_T1S_NOT_OK;
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_t1s_wait_ptp_request_completed
 * Description               Service Api to wait a Gptp request is complete
 * @param[in] p_ctrl         Pointer to the control block
 * @param[in] mask           Which Bit to poll in GCCR Register
 * @retval ETHER_T1S_OK      PTP request completed
 * @retval ETHER_T1S_NOT_OK  PTP request not completed
 ***********************************************************************************************************************/
static ether_t1s_type r_ether_t1s_wait_ptp_request_completed (ether_t1s_instance_ctrl_t * const p_ctrl, uint32_t mask)
{
    ether_t1s_type              return_value = ETHER_T1S_OK;
    uint32_t                    reg_value;
    uint32_t                    time_out_count_init;
    uint32_t                    time_out_count_elap;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) p_ctrl;
    time_out_count_init = 0UL;
    time_out_count_elap = 0UL;
    (void) r_ether_t1s_get_counter_value(&time_out_count_init);

    do
    {
        time_out_count_elap = time_out_count_elap + r_ether_t1s_get_counter_value(&time_out_count_init);
        reg_value           = p_instance_ctrl->p_reg_etnf0->GCCR & mask;
    } while ((reg_value != 0UL) && (time_out_count_elap <= ETHER_T1S_TIMEOUT_COUNT));

    if (ETHER_T1S_TIMEOUT_COUNT < time_out_count_elap)
    {
        return_value = ETHER_T1S_NOT_OK;
    }

    return return_value;
}                                      /* End of function r_ether_gptp_wait_ptp_request_completed() */

#endif

/*******************************************************************************************************************//**
 * Function Name: r_ether_t1s_enable_icu
 * @param[in] p_instance_ctrl    Pointer to the control block for the channel
 ***********************************************************************************************************************/
static void r_ether_t1s_enable_icu (ether_t1s_instance_ctrl_t * p_instance_ctrl)
{
    /* Configure the Ethernet common interrupt. */
    R_BSP_IrqCfgEnable(p_instance_ctrl->p_ether_cfg->irq,
                       p_instance_ctrl->p_ether_cfg->interrupt_priority,
                       p_instance_ctrl);
}

/*******************************************************************************************************************//**
 * Function Name: r_ether_t1s_disable_icu
 * @param[in] p_instance_ctrl    Pointer to the control block for the channel
 ***********************************************************************************************************************/
static void r_ether_t1s_disable_icu (ether_t1s_instance_ctrl_t * p_instance_ctrl)
{
    /* Disables interrupts in the NVIC. */
    R_BSP_IrqDisable(p_instance_ctrl->p_ether_cfg->irq);

    /* Clears the control block from the vector information array. */
    R_FSP_IsrContextSet(p_instance_ctrl->p_ether_cfg->irq, NULL);
}

/*******************************************************************************************************************//**
 * ISR called when data is loaded into ETH T1S data register from the shift register.
 * ISR called when ETH T1S finished to receive data and can be read data from register.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void r_ether_t1s_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    IRQn_Type irq = R_FSP_CurrentIrqGet();
    uint8_t   ctrl_idx;
    ether_t1s_instance_ctrl_t * p_instance_ctrl = (ether_t1s_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    ctrl_idx = p_instance_ctrl->p_ether_cfg->channel;
    uint32_t rxi_stat;

    /* Get Rx status interrupt */
    rxi_stat = p_instance_ctrl->p_reg_etnf0->RIS0;

    if (rxi_stat)
    {
        /* Clear the RX queue Data Interrupt Status */
        /* Consider clear queue index inther next time */
        p_instance_ctrl->p_reg_etnf0->RIS0 &= ~ETH_ETNF_RIS0_FRF_MARK;

        /* If a callback is provided, then call it with callback argument. */
        r_ether_t1s_call_callback(p_instance_ctrl, ETHER_EVENT_RX_INTERRUPT);
    }
    else
    {
        /* Clear the TX Descriptor Data Interrupt Status */
        p_instance_ctrl->p_reg_etnf0->DIS = ~p_instance_ctrl->p_reg_etnf0->DIS;

        /* If a callback is provided, then call it with callback argument. */
        r_ether_t1s_call_callback(p_instance_ctrl, ETHER_EVENT_TX_INTERRUPT);

        /* Tx confirmation */
        r_ether_t1s_tx_confirmation(p_instance_ctrl, ctrl_idx);

        /* If a callback is provided, then call it with callback argument. */
        r_ether_t1s_call_callback(p_instance_ctrl, ETHER_EVENT_TX_COMPLETE);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;
}
