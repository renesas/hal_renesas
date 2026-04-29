/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef R_ETHER_T1S_H
#define R_ETHER_T1S_H

/*******************************************************************************************************************//**
 * @addtogroup ETHER_T1S
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER
#include "r_ether_api.h"
#include "r_ether_t1s_cfg.h"

/** Filled in Interface API structure for this Instance. */
extern const ether_api_t g_ether_t1s_on_ether_t1s;

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETH_TXQ_NUM         (4UL)
#define ETH_RXQ_NUM         (18UL)
#define ETH_RX_BUF_TOTAL    (0x0008U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Ethernet buffer identifier type. */
typedef uint32_t ether_t1s_buf_idx_type;
typedef uint8_t  ether_t1s_data_type;
typedef uint16_t ether_t1s_frame_type;
typedef enum e_ether_t1s_loopback_config
{
    ETHER_T1S_LOOPBACK_ON  = 0,        ///< Enable lookback mode
    ETHER_T1S_LOOPBACK_OFF = 1,        ///< Disable lookback mode
} ether_t1s_loopback_config_t;

/** Ethernet Network Speed */
typedef enum e_ether_t1s_network_speed
{
    ETHER_T1S_SPEED_10MBPS     = 0,    ///< Ethernet network speed 10mbps
    ETHER_T1S_SPEED_100MBPS    = 1,    ///< Ethernet network speed 100mbps
    ETHER_T1S_SPEED_PROHIBITED = 2,    ///< Ethernet network speed Setting prohibited
} ether_t1s_network_speed_t;

/** Ethernet PHY Interface Select  */
typedef enum e_ether_t1s_phy_interface
{
    ETHER_T1S_MODE               = 0,  ///< Ethernet T1S interface
    ETHER_T1S_RMII               = 1,  ///< Ethernet RMII interface
    ETHER_T1S_SETTING_PROHIBITED = 2,  ///< Ethernet setting prohibited
} ether_t1s_phy_interface_t;

/** Header data added type */
typedef enum e_ether_t1s_header_type
{
    ETHER_T1S_HEADER_ADDED  = 0,       ///< Header added in configuration
    ETHER_T1S_HEADER_CUSTOM = 1,       ///< Added header in buffer manual
} ether_t1s_header_type_t;

typedef enum e_ether_t1s_previous_link_status
{
    ETHER_T1S_PREVIOUS_LINK_STATUS_DOWN = 0, ///< Previous link status is down
    ETHER_T1S_PREVIOUS_LINK_STATUS_UP   = 1, ///< Previous link status is up
} ether_t1s_previous_link_status_t;

typedef enum e_ether_t1s_link_change
{
    ETHER_T1S_LINK_CHANGE_NO_CHANGE = 0, ///< Link status is no change
    ETHER_T1S_LINK_CHANGE_LINK_DOWN = 1, ///< Link status changes to down
    ETHER_T1S_LINK_CHANGE_LINK_UP   = 2, ///< Link status changes to up
} ether_t1s_link_change_t;

typedef enum e_ether_t1s_link_establish_status
{
    ETHER_T1S_LINK_ESTABLISH_STATUS_DOWN = 0, ///< Link establish status is down
    ETHER_T1S_LINK_ESTABLISH_STATUS_UP   = 1, ///< Link establish status is up
} ether_t1s_link_establish_status_t;

/** AVB operation status. */
typedef enum e_ether_t1s_operation_status_type_t
{
    ETHER_T1S_RESET     = 0,           ///< Reset
    ETHER_T1S_CONFIG    = 1,           ///< Configuration
    ETHER_T1S_OPERATION = 2,           ///< Operation
    ETHER_T1S_STANDBY   = 3,           ///< Standby
    ETHER_T1S_UNINIT    = 4            ///< Not initialized
} ether_t1s_operation_status_type_t;

/** Setting option. */
typedef enum e_ether_t1s_option_type_t
{
    ETHER_T1S_DISABLE = 0,             ///< Disable
    ETHER_T1S_ENABLE  = 1              ///< Enable
} ether_t1s_option_type_t;

/** controller state type structure */
typedef enum e_ether_t1s_mode_type
{
    ETHER_T1S_MODE_DOWN              = 0, ///< Controller Disabled Mode
    ETHER_T1S_MODE_ACTIVE            = 1, ///< Controller Enabled Mode
    ETHER_T1S_MODE_STANDBY           = 2, ///< Controller Standby Mode
    ETHER_T1S_MODE_ACTIVE_TX_OFFLINE = 3  ///< Controller Silent Communication Mode
} ether_t1s_mode_type_t;

/** Enumeration for quality information regarding the evaluated time stamp.  */
/** Meaning of each value is depending on the HW. */
typedef enum
{
    ETHER_T1S_VALID     = 0,           ///< Valid
    ETHER_T1S_INVALID   = 1,           ///< Invalid
    ETHER_T1S_UNCERTAIN = 2            ///< Uncertain
} ether_t1s_time_stamp_qual_type;

typedef enum e_ether_t1s_tx_ch_type
{
    ETHER_T1S_TX_BE = 0,               ///< Transmit queue 0
    ETHER_T1S_TX_NC = 1,               ///< Transmit queue 1
    ETHER_T1S_TX_SB = 2,               ///< Transmit queue 2
    ETHER_T1S_TX_SA = 3                ///< Transmit queue 3
} ether_t1s_tx_ch_type;

typedef enum e_ether_t1s_tx_sync_mode_type
{
    ETHER_T1S_TXNORMAL = 0,            ///< Sync Mode Transmit Normal
    ETHER_T1S_TXNOWB   = 1             ///< Sync Mode Transmit No Write Back
} ether_t1s_tx_sync_mode_type;

typedef enum e_ether_t1s_sf_option_type
{
    ETHER_T1S_SFDISABLE = 0,           ///< Stream Filtering Disable
    ETHER_T1S_SEPFILTER = 1,           ///< Separation Filtering only
    ETHER_T1S_AVBNMDISC = 2,           ///< AVB Stream with Separation Filtering, no matching frames discarded
    ETHER_T1S_AVBNMQUE0 = 3            ///< AVB Stream with Separation Filtering, no matching frames into queue0
} ether_t1s_sf_option_type;

typedef enum e_ether_t1s_tx_q_prio_type
{
    ETHER_T1S_AVBDEF = 0,              ///< Default AVB Priority (Q3 > Q2 > Q1 > Q0)
    ETHER_T1S_AVBALT = 1               ///< Alternate AVB Priority (Q1 > Q3 > Q2 > Q0)
} ether_t1s_tx_q_prio_type;
typedef enum e_ether_t1s_rx_ch_type
{
    ETHER_T1S_RX_BE = 0,               ///< Receive Best Effort Channel
    ETHER_T1S_RX_NC = 1,               ///< Receive Network Control Channel
    ETHER_T1S_RX_S  = 2                ///< Receive Stream Channel
} ether_t1s_rx_ch_type;

typedef enum e_ether_t1s_pia_config_type
{
    ETHER_T1S_GAP32 = 0,               ///< Frame data starts always 32 bit aligned
    ETHER_T1S_NOGAP = 1                ///< No gaps between frame data in incremental data area
} ether_t1s_pia_config_type;

typedef enum e_ether_t1s_ufc_config_type
{
    ETHER_T1S_UFCC0 = 0,               ///< Unread Frame Counter Configuration 0
    ETHER_T1S_UFCC1 = 1,               ///< Unread Frame Counter Configuration 1
    ETHER_T1S_UFCC2 = 2,               ///< Unread Frame Counter Configuration 2
    ETHER_T1S_UFCC3 = 3                ///< Unread Frame Counter Configuration 3
} ether_t1s_ufc_config_type;

typedef enum e_ether_t1s_rx_sync_mode_type
{
    ETHER_T1S_RXNORMAL = 0,            ///< Normal mode (full descriptor write back)
    ETHER_T1S_RXKEEPDT = 1,            ///< Keep DT mode (no update of DT field at descriptor write back)
    ETHER_T1S_RXNOWB   = 2             ///< No Write Back (no descriptor write back)
} ether_t1s_rx_sync_mode_type;

typedef enum e_ether_t1s_direction_type
{
    ETHER_T1S_TX = 0,                  ///< Transmit
    ETHER_T1S_RX = 1                   ///< Receive
} ether_t1s_direction_type;

/* Type of Shaper for a specific Queue */
typedef enum e_ether_t1s_queue_policy_type
{
    ETHER_T1S_NONE = 0,                ///< None
    ETHER_T1S_CBS  = 1                 ///< Credit Based Shaping
} ether_t1s_queue_policy_type;

/** Setting control mode type */
typedef enum e_ether_t1s_control_mode
{
    ETHER_T1S_CTRL_INTERRUPT_MODE = 0, ///< Pre-compile option for Control Enable for receive Interrupt
    ETHER_T1S_CTRL_POLLING_MODE   = 1, ///< Pre-compile option for Control Enable for receive polling
} ether_t1s_control_mode_t;

/** Filter action type ethernet */
typedef enum e_ether_t1s_filter_action_type
{
    ETHER_T1S_ADD_TO_FILTER      = 0,  ///< Add the MAC address to the filter
    ETHER_T1S_REMOVE_FROM_FILTER = 1   ///< Remove the MAC address from the filter
} ether_t1s_filter_action_type_t;

/** Buffer request return type ethernet */
typedef enum e_ether_t1s_buf_req_return_type
{
    ETHER_T1S_BUFREQ_OK       = 0,     ///< Buffer confirm OK.
    ETHER_T1S_BUFREQ_E_NOT_OK = 1,     ///< Buffer confirm Not OK.
    ETHER_T1S_BUFREQ_E_BUSY   = 2,     ///< Buffer confirm busy.
    ETHER_T1S_BUFREQ_E_OVFL   = 3      ///< Buffer confirm overflow.
} ether_t1s_buf_req_return_type_t;

/** Receive status type ethernet */
typedef enum e_ether_t1s_rx_status_type
{
    ETHER_T1S_RECEIVED                     = 0, ///< There was one received frame.
    ETHER_T1S_NOT_RECEIVED                 = 1, ///< There was no received frame.
    ETHER_T1S_RECEIVED_MORE_DATA_AVAILABLE = 2  ///< There were more than one received frames.
} ether_t1s_rx_status_type_t;

/** MDIO interface type for T1S Ethernet specific */
typedef enum e_ether_t1s_mdio_interface_type
{
    ETHER_T1S_MDIO_INTERFACE_DEDICATED = 0, ///< Use ETS0_MDC, ETS0_MDIO (use a dedicated MDIO interface of Transceiver)
    ETHER_T1S_MDIO_INTERFACE_SHARED    = 1, ///< Use ETS0_RX_MDC, ETS0_ED_MDIO
} ether_t1s_mdio_interface_type_t;

/** PMA type for T1S Ethernet specific */
typedef enum e_ether_t1s_pma_type
{
    ETHER_T1S_PMA_UNFILTERED = 0,      ///< Handle an unfiltered
    ETHER_T1S_PMA_FILTERED   = 1,      ///< Handle a filtered
} ether_t1s_pma_type_t;

/** Initialization occurs when open is called. */
typedef struct st_ether_t1s_instance_ctrl
{
    uint32_t open;                                           ///< Used to determine if the channel is configured.

    /* Configuration of ethernet module. */
    ether_cfg_t const * p_ether_cfg;                         ///< Pointer to initial configurations.

    R_ETNF0_Type     * p_reg_etnf0;                          ///< Pointer to initial etnf0 type.
    R_ETNF0_T1S_Type * p_reg_selb_etnf0;                     ///< Pointer to initial T1S control register type.

    /* Status of ethernet driver. */
    ether_t1s_previous_link_status_t  previous_link_status;  ///< Previous link status
    ether_t1s_link_change_t           link_change;           ///< status of link change
    ether_t1s_link_establish_status_t link_establish_status; ///< Current Link status

    /* Pointer to callback and optional working memory */
    void (* p_callback)(ether_callback_args_t *);
    ether_callback_args_t * p_callback_memory;

    /* Pointer to context to be passed into callback function */
    void * p_context;
} ether_t1s_instance_ctrl_t;

/** MAC address type structure */
typedef struct st_ether_t1s_mac_address_type
{
    /* e.g. when a address is "12:34:56:78:9A:BC": */
    uint32_t h32;                      ///< has 0x12345678.
    uint32_t l16;                      ///< has 0x00009ABC.
} ether_t1s_mac_address_type_t;

/** Transmit and receive status type structure */
typedef struct st_ether_t1s_tx_rx_stats_type
{
    /* The total number of good packets received that were directed to the broadcast address.*/
    uint32_t stats_broad_cast_pkts;

    /* The total number of packets (including bad packets, broadcast packets, and multicast packets) received. */
    uint32_t rx_stats_pkts;

    /* The total number of octets of data (including those in bad packets) received on the network
     * (excluding framing bits but including FCS octets). */
    uint32_t rx_stats_octets;

    /* The total number of packets received that had a length of between 64 and 1518 octets
     * that had either a bad Frame Check Sequence (FCS) with an integral number of octets
     * (FCS Error) or a bad FCS with a non-integral number of octets (Alignment Error). */
    uint32_t rx_stats_crc_align_errors;

    /* The total number of packets received that were less than 64 octets long (excluding
     * framing bits, but including FCS octets) and were otherwise well formed.
     * Also described in IETF RFC 2819 MIB etherStatsUndersizePkts. */
    uint32_t rx_stats_under_size_pkts;

    /* The total number of packets received that were longer than 1518 octets (excluding
     * framing bits, but including FCS octets) and were otherwise well formed.
     * Also described in IETF RFC 2819 MIB etherStatsOversizePkts */
    uint32_t rx_stats_over_size_pkts;

    /* The total number of packets (including bad packets) received that were 64 octets in length.  */
    uint32_t rx_stats_pkts64_octets;

    /* The total number of packets (including bad packets) received that were between 65 and 127 octets in length. */
    uint32_t rx_stats_pkts65_to_127_octets;

    /* The total number of packets (including bad packets) received that were between 128 and 255 octets in length. */
    uint32_t rx_stats_pkts128_to_255_octets;

    /* The total number of packets (including bad packets) received that were between 256 and 511 octets in length. */
    uint32_t rx_stats_pkts256_to_511_octets;

    /* The total number of packets (including bad packets) received that were between 512 and 1023 octets in length. */
    uint32_t rx_stats_pkts512_to_1023_octets;

    /* The total number of packets (including bad packets) received that were between 1024 and 1518 octets in length. */
    uint32_t rx_stats_pkts1024_to_1518_octets;

    /* The number of subnetwork-unicast packets delivered to a higher-layer protocol. */
    uint32_t rx_unicast_frames;

    /* The total number of octets transmitted out of the interface, including framing characters. */
    uint32_t tx_number_of_octets;

    /* The total number of packets that higher-level protocols requested be transmitted to a
     * non-unicast address, including those that were discarded or not sent. */
    uint32_t tx_nu_cast_pkts;

    /* The total number of packets that higher-level protocols requested be transmitted to a subnetwork-unicast address,
     * including those that were discarded or not sent. */
    uint32_t tx_uni_cast_pkts;

    /* dropped packets due to CRC errors */
    uint32_t drop_pkt_crc;

    /* number of undersize packets which were less than 64 octets long
     * (excluding framing bits, but including FCS octets) and were otherwise well formed. (see IETF RFC 1757) */
    uint32_t under_size_pkt;

    /* number of oversize packets which are longer than 1518 octets
     * (excluding framing bits, but including FCS octets) and were otherwise well formed. (see IETF RFC 1757) */
    uint32_t over_size_pkt;

    /* number of alignment errors, i.e.packets which are received and
     * are not an integral number of octets in length and do not pass the CRC. */
    uint32_t algnmt_err;
} ether_t1s_tx_rx_stats_type_t;

/** Hardware status type structure */
typedef struct st_ether_t1s_hw_status_type
{
    uint32_t                          desc_table_addr;                               ///< Descriptor Table Address
    uint32_t                          buf_tx_cnt[ETH_TXQ_NUM];                       ///< Buffer Transmit Count
    uint32_t                          head_tx_desc[ETH_TXQ_NUM];                     ///< Head Transmit Descriptor
    uint32_t                          current_tx_desc[ETH_TXQ_NUM];                  ///< Current Transmit Descriptor
    uint32_t                          last_tx_desc[ETH_TXQ_NUM];                     ///< Last Transmit Descriptor
    uint32_t                          next_rx_desc[ETH_RXQ_NUM];                     ///< Next Receive Descriptor
    uint32_t                          current_rx_desc_update[ETH_RXQ_NUM];           ///< Current Receive Descriptor for Update
    uint32_t                          current_rx_desc_release[ETH_RXQ_NUM];          ///< Current Receive Descriptor for Release
    ether_t1s_option_type_t           rx_be_time_stamp;                              ///< Receive Best Effort Time Stamp
    ether_t1s_option_type_t           rx_s_time_stamp;                               ///< Receive Stream Time Stamp
    uint32_t                          rx_be_queue_buf[ETH_RX_BUF_TOTAL];             ///< Receive Best Effort Queue Buffer
    uint32_t                          rx_nc_queue_buf[ETH_RX_BUF_TOTAL];             ///< Receive Network Control Queue Buffer
    uint32_t                          rx_s_queue_buf[ETH_RXQ_NUM][ETH_RX_BUF_TOTAL]; ///< Receive Stream Queue Buffer
    ether_t1s_operation_status_type_t dev_status;                                    ///< Operation Status
} ether_t1s_hw_status_type_t;

/** Status of each controller */
typedef struct st_ether_t1s_controller_status_type
{
    /* MAC address of this controller */
    ether_t1s_mac_address_type_t mac_addr;

    /* Current mode, DOWN or ACTIVE */
    ether_t1s_mode_type_t mode;

    /* Whether a controller is in promiscuous mode.
     * This is the logical state, not necessarily equal to the HW state. */
    bool promiscuous;

    /* Whether the filter operation is activated */
    uint32_t active_filter_bits;

    /* Pointer to the head of tx buffer ring */
    uint32_t tx_buf_head;

    /* Pointer to the tail of tx buffer ring */
    uint32_t tx_buf_tail;

#if (ETHER_T1S_CFG_GLOBAL_TIME_SUPPORT)

    /* Pointer to the tail of ts descriptor ring */
    uint32_t ts_desc_tail;
#endif

    /* Transmit and receive status type structure */
    ether_t1s_tx_rx_stats_type_t tx_rx_stat;

    /* LLDriver specific status */
    ether_t1s_hw_status_type_t hw_stat_etnf;
} ether_t1s_controller_status_type_t;

typedef struct
{
    uint32_t nanoseconds;              ///< Nanoseconds
    uint32_t seconds;                  ///< Seconds
    uint16_t secondsHi;                ///< Seconds High byte
} ether_t1s_time_stamp_type;

/** Heap manager type structure */
typedef struct st_ether_t1s_double_list_node_type
{
    uint32_t size;                                      ///< Node size.
    struct st_ether_t1s_double_list_node_type * p_next; ///< Next size.
    struct st_ether_t1s_double_list_node_type * p_prev; ///< Prev size.
} ether_t1s_double_list_node_type_t;

typedef struct st_ether_t1s_buf_handler_type
{
    ether_t1s_double_list_node_type_t node; ///< The Node for linked list management
    uint32_t             buf_idx;           ///< The Index of buffer
    uint32_t             buf_addr;          ///< Pointer to the Frame to be transmitted
    uint32_t             tx_length;         ///< Transmit request size
    uint32_t             eth_type_addr;     ///< Ethernet Type Address
    ether_t1s_tx_ch_type channel;           ///< Channel
    bool                 enable_ts;         ///< Flag to store if TimeStamp shall be stored
    bool                 tx_confirm;        ///< The Tx Confirmation flag
    uint8_t              dummy[3];          ///< Dummy data
} ether_t1s_buf_handler_type_t;

/** Setting transmit buffer type structure */
typedef struct st_ether_t1s_tx_buffer_type
{
    ether_t1s_buf_handler_type_t * p_buffer_hdr; ///< Pointer for buffer address.
    bool    tx_ongoing;                          ///< Transmit ongoing status.
    uint8_t dummy[3];                            ///< Dummy data.
#if (ETH_GLOBAL_TIME_SUPPORT == STD_ON)
    ether_t1s_time_stamp_type      time_stamp;   ///< Timestamp Information
    ether_t1s_time_stamp_qual_type time_qual;    ///< Timestamp Quality
#endif
    bool locked;                                 ///< Transmit locked status
} ether_t1s_tx_buffer_type_t;

/** Rx Timestamp structure  */
typedef struct st_ether_t1s_desc_time_stamp_type
{
    uint32_t time_stamp0;              ///< Timestamp[31:0]
    uint32_t time_stamp1;              ///< Timestamp[63:32]
    uint16_t time_stamp2;              ///< Timestamp[79:64]

    /* Add dummy variable */
    uint16_t dummy;
} ether_t1s_desc_time_stamp_type;

/** Rx frame information structure */
typedef struct st_ether_t1s_rx_frame_type
{
    uint32_t frame_addr;               ///< Frame address.
    uint32_t eth_type_addr;            ///< Ethernet type address.
    uint32_t frame_length;             ///< Frame length.
    ether_t1s_desc_time_stamp_type time_stamp;
    ether_t1s_time_stamp_qual_type time_qual;
} ether_t1s_rx_frame_type_t;

/** Heap manager type structure */
typedef struct st_ether_t1s_list_type
{
    uint32_t count;                             ///< Number of node.
    ether_t1s_double_list_node_type_t * p_last; ///< Last node.
    ether_t1s_double_list_node_type_t * p_head; ///< Head node.
} ether_t1s_list_type_t;
typedef uint8_t ether_t1s_type;

/** Ethernet frame type */
typedef struct st_ether_t1s_eth_frame_type_etnf
{
    uint8_t        dst_addr[6];        ///< Destination Address
    uint8_t        src_addr[6];        ///< Source Address
    uint8_t        frame_type[2];      ///< EtherType
    ether_t1s_type payload;            ///< Payload
} ether_t1s_frame_type_etnf;

/* OS Tick reference type */
typedef uint32_t ether_t1s_tick_type;

/* OS Tick reference type */
typedef ether_t1s_tick_type ether_t1s_tick_ref_type;

/* Contains data to be used for register read/write check */
typedef struct st_ether_t1s_etnf_check_rw_register_type
{
    uint32_t offset;                   ///< Register offset address
    uint32_t mask_value;               ///< Mask Value
    uint32_t write_pattern0;           ///< Pattern 0 to write to register
    uint32_t write_pattern1;           ///< Pattern 1 to write to register
} ether_t1s_etnf_check_rw_register_type;

/* Contains data to be used for register read check */
typedef struct st_ether_t1s_etnf_check_ro_register_type
{
    uint32_t offset;                   ///< Register offset address
    uint32_t mask_value;               ///< Mask Value
    uint32_t expected_value;           ///< Expected Value
} ether_t1s_etnf_check_ro_register_type;

typedef struct st_ether_t1s_rx_config_type_etnf
{
    ether_t1s_option_type_t  encf;     ///< Enable Network Control Filtering
    ether_t1s_sf_option_type esf;      ///< Enable Stream Filtering
    ether_t1s_option_type_t  ets0;     ///< Enable Timestamp Storage (Best Effort)
    ether_t1s_option_type_t  ets2;     ///< Enable Timestamp Storage (Stream)
    uint32_t                 rfcl;     ///< Reception FIFO Critical Level
#if (ETHER_T1S_CFG_STREAM_FILTERING)
    ether_t1s_option_type_t srp_talker_filtering;
#endif
} ether_t1s_rx_config_type_etnf;

typedef struct st_ether_t1s_tx_config_type_etnf
{
    ether_t1s_tx_sync_mode_type tsm0;  ///< Transmit Synchronization Mode0 (BE class)
    ether_t1s_tx_sync_mode_type tsm1;  ///< Transmit Synchronization Mode1 (NC class)
    ether_t1s_tx_sync_mode_type tsm2;  ///< Transmit Synchronization Mode2 (Stream Class B)
    ether_t1s_tx_sync_mode_type tsm3;  ///< Transmit Synchronization Mode2 (Stream Class A)
    ether_t1s_option_type_t     ecbs;  ///< Enable Credit Based Shaping
    ether_t1s_tx_q_prio_type    tqp;   ///< Transmission Queues Priority
} ether_t1s_tx_config_type_etnf;

typedef struct st_ether_t1s_avb_config_type
{
    ether_t1s_rx_config_type_etnf rx_config; ///< Rx Configuration
    ether_t1s_tx_config_type_etnf tx_config; ///< Tx Configuration
} ether_t1s_avb_config_type_t;

typedef struct st_ether_t1s_rx_ch_config_type
{
    ether_t1s_rx_ch_type ch_type;           ///< Channel Type
    uint8_t              ch_num;            ///< Channel Number
    uint8_t              dummy[3];          ///< Dummy data
    uint8_t              pattern_stream[8]; ///< Pattern Stream to be written into ETNFnSFPi
} ether_t1s_rx_ch_config_type;

typedef struct st_ether_t1s_queue_config_type
{
    ether_t1s_pia_config_type   pia;   ///< Packed Incremental Data Area
    ether_t1s_ufc_config_type   ufcc;  ///< Unread Frame Counter Configuration
    ether_t1s_rx_sync_mode_type rsm;   ///< Receive Synchronization Mode
} ether_t1s_queue_config_type;

typedef struct st_ether_t1s_mem_manager_type
{
    struct st_ether_t1s_mem_manager_type * p_next; ///< Next node
    struct st_ether_t1s_mem_manager_type * p_prev; ///< Previous node
    uint32_t heap_size;                            ///< Memory block size
    bool     heap_lock;                            ///< Memory lock flag
    uint8_t  dummy[3];                             ///< Dummy data
} ether_t1s_mem_manager_type;

typedef struct st_ether_t1s_link_desc_l_type
{
    uint32_t res : 24;                 ///< Reserved
    uint32_t die : 4;                  ///< Descriptor Interrupt Enable
    uint32_t dt  : 4;                  ///< Descriptor Type
} ether_t1s_link_desc_l_type;

typedef struct st_ether_t1s_data_desc_l_type
{
    uint32_t ds   : 12;                ///< Size of descriptor data
    uint32_t ctrl : 12;                ///< Content control code for Tx/Rx
    uint32_t die  : 4;                 ///< Descriptor Interrupt Enable
    uint32_t dt   : 4;                 ///< Descriptor Type
} ether_t1s_data_desc_l_type;

typedef struct st_ether_t1s_link_desc_type
{
    ether_t1s_link_desc_l_type header; ///< Low word of the link descriptor
    uint32_t dptr;                     ///< Point to the next descriptor
} ether_t1s_link_desc_type;

typedef struct st_ether_t1s_data_desc_type
{
    ether_t1s_data_desc_l_type header; ///< Low word of the data descriptor
    uint32_t dptr;                     ///< Point to the descriptor data
} ether_t1s_data_desc_type;

typedef struct st_ether_t1s_ext_data_desc_type
{
    ether_t1s_data_desc_l_type header;         ///< Low word of the data descriptor
    uint32_t dptr;                             ///< Point to the descriptor data
    ether_t1s_desc_time_stamp_type time_stamp; ///< Timestamp for received frame
} ether_t1s_ext_data_desc_type;

#if (ETHER_T1S_CFG_QOS_SUPPORT)

/*******Traffic Priorities Table****************/
typedef struct st_ether_t1s_priority_mapping_type
{
    uint8_t eth_ctrl_priority_value;   ///< Ethernet Control Priority Value
    uint8_t eth_ctrl_tx_queue_id;      ///< Ethernet Control Tx Queue ID
} ether_t1s_priority_mapping_type;

typedef struct st_ether_t1s_ctrl_priority_type
{
    /* Array to collect all configured Priorities */
    const ether_t1s_priority_mapping_type * p_priority_table;

    /* Store the number of Traffic Priorities configured */
    uint8_t number_of_priorities;

    /* Default Tx Queue for unmapped traffic */
    uint8_t dfl_tx_queue;
} ether_t1s_ctrl_priority_type;
#endif

#if (ETHER_T1S_CFG_QOS_SUPPORT)

/* Tx Queue Shaper */
typedef struct st_ether_t1s_queue_shaper_type
{
    ether_t1s_queue_policy_type tx_q_policy; ///< Type of shape
    uint32_t civ;                            ///< Credit Increment value should be [1..65535]
    signed   cdv;                            ///< Credit Decrement value should be [-65536 ..-1]
} ether_t1s_queue_shaper_type;
#endif

/* Tx Queue Struct */
typedef struct st_ether_t1s_tx_queue_type_etnf
{
#if (ETHER_T1S_CFG_QOS_SUPPORT)
    ether_t1s_queue_shaper_type tx_queue_shaper;
#endif
    uint16_t eth_tx_queue_bufs;        ///< Ethernet Tx Queue Buffer
    uint8_t  eth_tx_queue_id;          ///< Ethernet Tx Queue ID
} ether_t1s_tx_queue_type_etnf;

/* Rx Queue struct */
typedef struct st_ether_t1s_rx_queue_type_etnf
{
    uint16_t eth_rx_queue_bufs;        ///< Ethernet Rx Queue Buffer
    uint8_t  eth_rx_queue_id;          ///< Ethernet Rx Queue ID
#if (ETHER_T1S_CFG_STREAM_FILTERING)
    uint8_t eth_pattern_stream[8];     ///< Pattern Stream to be written into ETNFnSFPi
#endif
} ether_t1s_rx_queue_type_etnf;

typedef struct st_ether_t1s_queue_config_type_etnf
{
    const ether_t1s_tx_queue_type_etnf * p_tx_queue_config; ///< Pointer to Tx Queue Configuration
    const ether_t1s_rx_queue_type_etnf * p_rx_queue_config; ///< Pointer to Rx Queue Configuration
    uint8_t number_of_tx_queue;                             ///< Number of Tx Queue
    uint8_t number_of_rx_queue;                             ///< Number of Rx Queue
} ether_t1s_queue_config_type_etnf;

/* ETNF LLDriver specific hardware configuration */
typedef struct st_ether_t1s_etnf_config_type
{
    ether_t1s_queue_config_type_etnf queue_config; ///< Queue Configuration
    ether_t1s_rx_config_type_etnf    rx_config;    ///< Rx Configuration
    ether_t1s_tx_q_prio_type         tx_config;    ///< Tx Configuration
    ether_t1s_option_type_t          ecbs;         ///< Enable Credit Based Shaping
#if (ETHER_T1S_CFG_QOS_SUPPORT)
    ether_t1s_ctrl_priority_type ctrl_priority;    ///< Control Priority
#endif
} ether_t1s_etnf_config_type;

/** ETHER extension configures the buffer descriptor for ETHER. */
typedef struct st_ether_t1s_extended_cfg
{
    uint8_t eth_mac_addr[6];
    ether_t1s_control_mode_t       ether_t1s_cfg_type_tx_isr;    ///< Setting option for tx interrupt type.
    ether_t1s_control_mode_t       ether_t1s_cfg_type_rx_isr;    ///< Setting option for rx interrupt type.
    ether_t1s_control_mode_t       ether_t1s_ctl_mode;           ///< Setting option for polling or interrupt mode.
    ether_t1s_filter_action_type_t ether_t1s_filter_action_type; ///< Configuration filter action type.
    uint8_t                   * p_mac_address_filter;            ///< MAC address filter
    ether_t1s_header_type_t     header_data_custom;              ///< Header data custom enable
    const void                * p_hw_unit_config;                ///< Hardware configuration for Ethernet T1S specific.
    ether_t1s_loopback_config_t ether_loopback_config;           ///< Loopback mode enable
    ether_t1s_network_speed_t   ether_network_speed;             ///< Transfer Rate
    ether_t1s_phy_interface_t   ether_phy_interface;             ///< PHY Interface Select
    uint8_t                   * p_mac_address_target;            ///< Mac address target
    uint8_t                         number_of_addr_filter;       ///< Used to determine number of address add to list multicast.
    uint16_t                        ether_type;                  ///< Ethernet type in Header data
    ether_t1s_pma_type_t            pma_type;                    ///< Configures the PMA to handle a filtered or unfiltered T1S energy detection pin (ETNF0ED_MDIO)
    ether_t1s_mdio_interface_type_t mdio_interface_type;         ///< Configures the MDIO interface
    uint8_t                         mdio_slave_address;          ///< Specifies the address of the mdio slave module (PHY MDIO address)
    uint32_t                        ether_t1s_ram_size;          ///< Ethernet RAM size
    uint8_t                       * p_mem_pool_buffer_table;     ///< Ethernet RAM memory pool buffer table
} ether_t1s_extended_cfg_t;

typedef struct st_ether_t1s_tx_ctrl_tag_type
{
    uint32_t tag : 10;                 ///< Frame tag
    uint32_t tsr : 1;                  ///< Timestamp storage request
    uint32_t msr : 1;                  ///< MAC Status storage Request
} ether_t1s_tx_ctrl_tag_type;

typedef union u_ether_t1s_tx_rx_ctrl
{
    ether_t1s_tx_ctrl_tag_type tx_ctrl; ///< Tx Control field
    uint32_t word;                      ///< Allow access of the whole word
} ether_t1s_tx_rx_ctrl;

/** @endcond */

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHER_T1S_Open(ether_ctrl_t * const p_ctrl, ether_cfg_t const * const p_cfg);

fsp_err_t R_ETHER_T1S_Close(ether_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_T1S_Read(ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t * const length_bytes);

fsp_err_t R_ETHER_T1S_Write(ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t const frame_length);

fsp_err_t R_ETHER_T1S_CallbackSet(ether_ctrl_t * const          p_ctrl,
                                  void (                      * p_callback)(ether_callback_args_t *),
                                  void * const                  p_context,
                                  ether_callback_args_t * const p_callback_memory);

fsp_err_t R_ETHER_T1S_TxStatusGet(ether_ctrl_t * const p_ctrl, void * const p_buffer_address);

fsp_err_t R_ETHER_T1S_BufferRelease(ether_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_T1S_RxBufferUpdate(ether_ctrl_t * const p_ctrl, void * const p_buffer);

fsp_err_t R_ETHER_T1S_LinkProcess(ether_ctrl_t * const p_ctrl);

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_T1S)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif                                 /* R_ETHER_T1S_H */
