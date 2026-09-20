/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_ETHER_TSN_H
#define R_ETHER_TSN_H

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup ETHER_TSN
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER
#include "r_ether_api.h"

#include "r_ether_tsn_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Copy the MAC address */
#define ETHER_TSN_COPY_MAC_ADDRESS(src, dst) /* PRQA S 3472 # Advisory D-4.9 */ \
    do                                                                          \
    {                                                                           \
        (dst)[0] = (src)[0];                                                    \
        (dst)[1] = (src)[1];                                                    \
        (dst)[2] = (src)[2];                                                    \
        (dst)[3] = (src)[3];                                                    \
        (dst)[4] = (src)[4];                                                    \
        (dst)[5] = (src)[5];                                                    \
    } while (0)

#define ETHER_TSN_UNPACK_ADDRESS_TO_8(src, dst) /* PRQA S 3472 # Advisory D-4.9 */ \
    do                                                                             \
    {                                                                              \
        (dst)[0] = (uint8_t) ((src).h32 >> 24UL);                                  \
        (dst)[1] = (uint8_t) ((src).h32 >> 16UL);                                  \
        (dst)[2] = (uint8_t) ((src).h32 >> 8UL);                                   \
        (dst)[3] = (uint8_t) (src).h32;                                            \
        (dst)[4] = (uint8_t) ((src).l16 >> 8U);                                    \
        (dst)[5] = (uint8_t) (src).l16;                                            \
    } while (0)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef uint8_t ether_tsn_type;

/* OS ID Type */
typedef uint32_t ether_tsn_counter_type; /* PRQA S 1535 # Rule-2.3 */

/* OS Tick reference type */
typedef uint32_t ether_tsn_tick_type;

/* OS Tick reference type */
typedef ether_tsn_tick_type ether_tsn_tick_ref_type;

/* Ethernet buffer identifier type. */
typedef uint32_t ether_tsn_buf_idx_type;

/* Ethernet frame type. */
typedef uint16_t ether_tsn_frame_type;

typedef enum e_ether_tsn_option_type   /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_OPTION_DISABLE = 0,      ///< setting is disabled
    ETHER_TSN_OPTION_ENABLE  = 1,      ///< setting is enabled
} ether_tsn_option_type_t;

typedef enum e_ether_tsn_frame_type    /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_EXPRESS_FRAME    = 0,
    ETHER_TSN_PREEMPTION_FRAME = 1
} ether_tsn_trame_type_t;

typedef enum e_ether_tsn_sgmii_operation_mode           /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_ENABLE = 0,                               ///< PHY-LSI is bypassed
    ETHER_TSN_DISABLE,                                  ///< PHY-LSI is not bypassed
    ETHER_TSN_NOT_SUPPORTED,                            ///< PHY-LSI is not supported
} ether_tsn_sgmii_operation_mode_t;                     /* PRQA S 1535 # Rule-2.3 */

typedef enum e_ether_tsn_loopback_config                /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_LOOPBACK_ON = 0,                          ///< Enable loop back mode
    ETHER_TSN_LOOPBACK_OFF,                             ///< Disable loop back mode
} ether_tsn_loopback_config_t;

/** Ethernet Network Speed */
typedef enum e_ether_tsn_network_speed                  /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_SPEED_10MBPS = 0,                         ///< Ethernet network speed 10mbps
    ETHER_TSN_SPEED_100MBPS,                            ///< Ethernet network speed 100mbps
    ETHER_TSN_SPEED_1GBPS,                              ///< Ethernet network speed 1gbps
    ETHER_TSN_SPEED_PROHIBITED,                         ///< Ethernet network speed setting prohibited
} ether_tsn_network_speed_t;

/** Ethernet PHY Interface Select  */
typedef enum e_ether_tsn_phy_interface                  /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_MII_RMII = 0,                             ///< Ethernet network speed 10mbps and 100mbps
    ETHER_TSN_SETTING_PROHIBITED,                       ///< Ethernet network speed setting prohibited
    ETHER_TSN_SGMII,                                    ///< Ethernet network speed 1gbps
} ether_tsn_phy_interface_t;

/** Broadcast Filter */
typedef enum e_ether_tsn_broadcast                      /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_BROADCAST_DISABLE = 0,                    ///< Disable reception of broadcast frames
    ETHER_TSN_BROADCAST_ENABLE  = 1,                    ///< Enable reception of broadcast frames
} ether_tsn_broadcast_t;

/** Unicast Filter */
typedef enum e_ether_tsn_unicast                        /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_UNICAST_DISABLE = 0,                      ///< Disable reception of unicast frames
    ETHER_TSN_UNICAST_ENABLE  = 1,                      ///< Enable reception of unicast frames
} ether_tsn_unicast_t;

/** Preemption Link Verification */
typedef enum e_ether_tsn_preemption_link_verification   /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_PREEMPTION_LINK_VERIFICATION_DISABLE = 0, ///< Preemption Link Verification not requested
    ETHER_TSN_PREEMPTION_LINK_VERIFICATION_ENABLE  = 1, ///< Preemption Link Verification requested
} ether_tsn_preemption_link_verification_t;

/** Pause frame type */
typedef enum e_ether_tsn_pause_frame_type               /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_PAUSE_FRAME_ENABLE  = 0,                  ///< Enable pause frame for flow control
    ETHER_TSN_PAUSE_FRAME_DISABLE = 1,                  ///< Disable pause frame
} ether_tsn_pause_frame_type_t;

typedef enum e_ether_tsn_pause_frame_zero_type          /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_PAUSE_FRAME_ZERO_ENABLE  = 0,             ///< Enable pause frame for flow control
    ETHER_TSN_PAUSE_FRAME_ZERO_DISABLE = 1,             ///< Disable pause frame
} ether_tsn_pause_frame_zero_type_t;

/** Header data added type */
typedef enum e_ether_tsn_header_type                    /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_HEADER_ADDED  = 0,                        ///< Header added in configuration
    ETHER_TSN_HEADER_CUSTOM = 1,                        ///< Added header in buffer manual
} ether_tsn_header_type_t;

typedef enum e_ether_tsn_previous_link_status           /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_PREVIOUS_LINK_STATUS_DOWN = 0,            ///< Previous link status is down
    ETHER_TSN_PREVIOUS_LINK_STATUS_UP   = 1,            ///< Previous link status is up
} ether_tsn_previous_link_status_t;

typedef enum e_ether_tsn_link_change                    /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_LINK_CHANGE_NO_CHANGE = 0,                ///< Link status is no change
    ETHER_TSN_LINK_CHANGE_LINK_DOWN = 1,                ///< Link status changes to down
    ETHER_TSN_LINK_CHANGE_LINK_UP   = 2,                ///< Link status changes to up
} ether_tsn_link_change_t;

typedef enum e_ether_tsn_link_establish_status          /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_LINK_ESTABLISH_STATUS_DOWN = 0,           ///< Link establish status is down
    ETHER_TSN_LINK_ESTABLISH_STATUS_UP   = 1,           ///< Link establish status is up
} ether_tsn_link_establish_status_t;

/** controller state type structure */
typedef enum e_ether_tsn_mode_type                      /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_MODE_DOWN              = 0,               ///< Controller state down.
    ETHER_TSN_MODE_ACTIVE            = 1,               ///< Controller state active.
    ETHER_TSN_MODE_STANDBY           = 2,               ///< Controller state standby.
    ETHER_TSN_MODE_ACTIVE_TX_OFFLINE = 3                ///< Controller state active transmit offline.
} ether_tsn_mode_type_t;

/** Buffer request return type Ethernet */
typedef enum e_ether_tsn_buf_req_return_type            /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_BUFREQ_OK       = 0,                      ///< Buffer confirm oK.
    ETHER_TSN_BUFREQ_E_NOT_OK = 1,                      ///< Buffer confirm Not oK.
    ETHER_TSN_BUFREQ_E_BUSY   = 2,                      ///< Buffer confirm busy.
    ETHER_TSN_BUFREQ_E_OVFL   = 3                       ///< Buffer confirm overflap.
} ether_tsn_buf_req_return_type_t;

/** ETag receive status type*/
typedef enum e_ether_tsn_etag_rx_ext_status_type        /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_EXT_RECEIVED                         = 0, ///< There was one received frame.
    ETHER_TSN_EXT_NOT_RECEIVED                     = 1, ///< There was one not received frame
    ETHER_TSN_EXT_RECEIVED_MORE_DATA_AVAILABLE     = 2, ///< There were more than one received frames.
    ETHER_TSN_EXT_NOT_RECEIVED_MORE_DATA_AVAILABLE = 3  ///< There was no received, but there are still receive frames.
} ether_tsn_etag_rx_ext_status_type_t;

/** Receive status type ethernet */
typedef enum e_ether_tsn_rx_status_type                 /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_RECEIVED                     = 0,         ///< There was one received frame.
    ETHER_TSN_NOT_RECEIVED                 = 1,         ///< There was no received frame.
    ETHER_TSN_RECEIVED_MORE_DATA_AVAILABLE = 2          ///< There were more than one received frames.
} ether_tsn_rx_status_type_t;

/** Transmission Fragment Size Configuration */
typedef enum e_ether_tsn_transmission_fragment_size     /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_TRANSMISSION_FRAGMENT_SIZE_64  = 0,       ///< setting is 64 bytes.
    ETHER_TSN_TRANSMISSION_FRAGMENT_SIZE_128 = 1,       ///< setting is 128 bytes.
    ETHER_TSN_TRANSMISSION_FRAGMENT_SIZE_192 = 2,       ///< setting is 192 bytes.
    ETHER_TSN_TRANSMISSION_FRAGMENT_SIZE_256 = 3        ///< setting is 256 bytes.
} ether_tsn_transmission_fragment_size_t;

/** PMAC/EMAC configuration for Tx Queue */
typedef enum e_ether_tsn_queue_contanins_traffic        /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_E_MAC = 0,                                ///< Queue contains eMAC traffic.
    ETHER_TSN_P_MAC = 1                                 ///< Queue contains pMAC traffic.
} ether_tsn_queue_contanins_traffic_t;

/** Setting option for Tx interrupt type */
typedef enum e_ether_tsn_optionType_tx_isr              /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_CTRL_DISABLE_TX_INTERRUPT = 0,            ///< Disable Tx interrupt.
    ETHER_TSN_CTRL_ENABLE_TX_INTERRUPT  = 1,            ///< Enable Tx interrupt.
} ether_tsn_optionType_tx_isr_t;

/** Setting option for Rx interrupt type */
typedef enum e_ether_tsn_optionType_rx_isr              /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_CTRL_DISABLE_RX_INTERRUPT = 0,            ///< Disable Rx interrupt.
    ETHER_TSN_CTRL_ENABLE_RX_INTERRUPT  = 1,            ///< Enable Rx interrupt.
} ether_tsn_optionType_rx_isr_t;

/** Setting control mode type */
typedef enum e_ether_tsn_control_mode                   /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_CTRL_INTERRUPT_MODE = 0,                  ///< Pre-compile option for Control Enable for receive Interrupt
    ETHER_TSN_CTRL_POLLING_MODE   = 1,                  ///< Pre-compile option for Control Enable for receive polling
} ether_tsn_control_mode_t;

/** Filter action type Ethernet */
typedef enum e_ether_tsn_filter_action_type             /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_ADD_TO_FILTER      = 0,                   ///< Add the MAC address to the filter
    ETHER_TSN_REMOVE_FROM_FILTER = 1                    ///< Remove the MAC address from the filter
} ether_tsn_filter_action_type_t;
typedef enum e_ether_tsn_time_qual_type                 /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_TSN_VALID     = 0,                            ///< Time stamp is valid and can be read
    ETHER_TSN_INVALID   = 1,                            ///< Time stamp is invalid, no time stamp receive
    ETHER_TSN_UNCERTAIN = 2                             ///< Time stamp still not reliable to read
} ether_tsn_time_qual_type_t;
typedef struct st_ether_tsn_time_stamp_type             /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t nano_seconds;                              ///< Nanosecond part of the GPTP timer
    uint32_t seconds;                                   ///< Lower 32 bit of second part of the GPTP timer
    uint16_t seconds_hi;                                ///< Upper 16 bit of second part of the GPTP timer
} ether_tsn_time_stamp_type_t;

/** Data descriptor structure low word */
typedef struct st_ether_tsn_data_desc_low_type          /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t ds    : 12;                                ///< Descriptor size.
    uint32_t info0 : 13;                                ///< Information 0 between CPU and MHD.
    uint32_t dse   : 1;                                 ///< Data Size Error.
    uint32_t axie  : 1;                                 ///< AXI Bus Error.
    uint32_t die   : 1;                                 ///< Descriptor Interrupt Enable.
    uint32_t dt    : 4;                                 ///< Descriptor Type.
} ether_tsn_data_desc_low_type_t;

/** Extended data descriptor structure */
typedef struct st_ether_tsn_ext_desc_type               /* PRQA S 1536 # Advisory R-2.4 */
{
    ether_tsn_data_desc_low_type_t st_header;           ///< Low word of the data descriptor.
    uint32_t dptr;                                      ///< Descriptor data.
    uint32_t info1[2];                                  ///< Information 1 between CPU and MHD.
} ether_tsn_ext_desc_type_t;

/* PRQA S 1506 ++ # DP_MSR_R-5.6_UC1 */
/** Heap manager type structure */
typedef struct st_ether_tsn_heap_manager_type      /* PRQA S 1536 # Advisory R-2.4 */
{
    struct ether_tsn_heap_manager_type_t * p_next; ///< Next node in the linked list.
    struct ether_tsn_heap_manager_type_t * p_prev; ///< Previous node in the linked list.
    uint32_t heap_size;                            ///< Memory block size.
    bool     heap_lock;                            ///< Memory lock flag.
    uint8_t  dummy[3];                             ///< Padding for alignment (optional).
} ether_tsn_heap_manager_type_t;

/* PRQA S 1506 --  */
/** Extended data descriptor with Time stamp structure */
typedef struct st_ether_tsn_ext_desc_with_ts_type                    /* PRQA S 1536 # Advisory R-2.4 */
{
    ether_tsn_data_desc_low_type_t st_header;                        ///< Low word of the data descriptor.
    uint32_t dptr;                                                   ///< Descriptor data.
    uint32_t info1[2];                                               ///< Information 1 between CPU and MHD.
    uint32_t tsns;                                                   ///< Time stamps nanosecond.
    uint32_t tss;                                                    ///< Time stamp second.
} ether_tsn_ext_desc_with_ts_type_t;

/** MAC address type structure */
typedef struct st_ether_tsn_mac_address_type                         /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t h32;                                                    ///< has 0x12345678.
    uint32_t l16;                                                    ///< has 0x00009ABC.
} ether_tsn_mac_address_type_t;

/** Hardware status type structure */
typedef struct st_ether_tsn_hw_status_type                           /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t * p_buf_tx_cnt;                                         ///< Pointer to a uint32 variable (transmit buffer count).
    uint32_t * p_tx_alloc_cnt;                                       ///< Pointer to a uint32 variable (allocated transmit count).
    ether_tsn_ext_desc_type_t         ** pp_head_tx_desc;            ///< Double pointer to an Eth_ExtDescType (head of transmit descriptor).
    ether_tsn_ext_desc_type_t         ** pp_next_tx_desc;            ///< Double pointer to an Eth_ExtDescType (next transmit descriptor).
    ether_tsn_ext_desc_with_ts_type_t ** pp_head_rx_desc;            ///< Double pointer to an Eth_ExtDescWithTsType (head of receive descriptor).
    ether_tsn_ext_desc_with_ts_type_t ** pp_current_rx_desc_update;  ///< Double pointer to an Eth_ExtDescWithTsType (current of receive descriptor).
    ether_tsn_ext_desc_with_ts_type_t ** pp_current_rx_desc_release; ///< Double pointer to an Eth_ExtDescWithTsType (current of receive descriptor).
} ether_tsn_hw_status_type_t;

/** Transmit and receive status type structure */
typedef struct st_ether_tsn_tx_rx_stats_type                         /* PRQA S 1536 # Advisory R-2.4 */
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
    uint32_t rx_stats_pkts64O_ctets;

    /* The total number of packets (including bad packets) received that were between 65 and 127 octets in length. */
    uint32_t rx_stats_pkts65_to_127O_ctets;

    /* The total number of packets (including bad packets) received that were between 128 and 255 octets in length. */
    uint32_t rx_stats_pkts128_to_255O_ctets;

    /* The total number of packets (including bad packets) received that were between 256 and 511 octets in length. */
    uint32_t rx_stats_pkts256_to_511O_ctets;

    /* The total number of packets (including bad packets) received that were between 512 and 1023 octets in length. */
    uint32_t rx_stats_pkts512_to_1023O_ctets;

    /* The total number of packets (including bad packets) received that were between 1024 and 1518 octets in length. */
    uint32_t rx_stats_pkts1024_to_1518O_ctets;

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

    /* number of under size packets which were less than 64 octets long
     * (excluding framing bits, but including FCS octets) and were otherwise well formed. (see IETF RFC 1757) */
    uint32_t under_size_pkt;

    /* number of over size packets which are longer than 1518 octets
     * (excluding framing bits, but including FCS octets) and were otherwise well formed. (see IETF RFC 1757) */
    uint32_t over_size_pkt;

    /* number of alignment errors, i.e.packets which are received and
     * are not an integral number of octets in length and do not pass the CRC. */
    uint32_t algnmt_err;
} ether_tsn_tx_rx_stats_type_t;

/** Status of each controller */
typedef struct st_ether_tsn_controller_status_type /* PRQA S 1536 # Advisory R-2.4 */
{
    /* MAC address of this controller */
    ether_tsn_mac_address_type_t st_mac_addr;

    /* Current mode, DOWN or ACTIVE */
    ether_tsn_mode_type_t mode;

    /* LLDriver specific status */
    ether_tsn_hw_status_type_t hw_stat;

    /* Transmit and receive status type structure */
    ether_tsn_tx_rx_stats_type_t tx_rx_stat;

    /* Whether a controller is in promiscuous mode.
     * This is the logical state, not necessarily equal to the HW state. */
    bool promiscuous;

    /* Whether the filter operation is activated */
    uint32_t active_filter_bits;

    /* Pointer to the head of Tx buffer ring */
    uint32_t tx_buf_head;

    /* Pointer to the tail of Tx buffer ring */
    uint32_t tx_buf_tail;

    /* Pointer to the tail of Ts descriptor ring */
    uint32_t ts_desc_tail;
} ether_tsn_controller_status_type_t;

/** Rx frame information structure */
typedef struct st_ether_tsn_rx_frame_type    /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t frame_addr;                     ///< Frame address.
    uint32_t eth_type_addr;                  ///< Ethernet type address.
    uint32_t frame_length;                   ///< Frame length.
    uint32_t tsns;                           ///< Time stamps nanosecond.
    uint32_t tss;                            ///< Time stamp second.
} ether_tsn_rx_frame_type_t;

/** Setting option buffer handler structure */
typedef struct st_ether_tsn_buf_handler_type /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t buf_idx;                        ///< The Index of buffer.
    uint32_t buf_addr;                       ///< Pointer to the Frame in URAM  to be transmitted.
    uint32_t tx_length;                      ///< Transmit request size.
    uint32_t eth_type_addr;                  ///< Ethernet type address.
    uint8_t  priority;                       ///< The Index of buffer.
    bool     enable_ts;                      ///< Flag to store if TimeStamp shall be stored.
    bool     tx_confirm;                     ///< The Tx Confirmation flag .
#if (ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT)
    ether_tsn_time_stamp_type_t time_stamp;
    ether_tsn_time_qual_type_t  time_qual;
#endif
    uint8_t dummy;                                ///< Dummy data.
} ether_tsn_buf_handler_type_t;

/** Setting trannsmit buffer type structure */
typedef struct st_ether_tsn_tx_buffer_type        /* PRQA S 1536 # Advisory R-2.4 */
{
    ether_tsn_buf_handler_type_t * p_buffer_hdr;  ///< Pointer for buffer address.
    bool    tx_ongoing;                           ///< Transmit ongoing status.
    uint8_t dummy[3];                             ///< Dummy data.
} ether_tsn_tx_buffer_type_t;

/** Tx specific control field structure */
typedef struct st_ether_tsn_tx_ctrl_info0_type    /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t fi   : 1;                            ///< FCS in.
    uint32_t fr   : 1;                            ///< FCS Replacement.
    uint32_t txc  : 1;                            ///< Tx Capture.
    uint32_t iet  : 1;                            ///< Insert regress time stamp.
    uint32_t tsun : 8;                            ///< Time stamp unique number.
    uint32_t tn   : 1;                            ///< Timer number.
} ether_tsn_tx_ctrl_info0_type_t;

/** Simplifies Bit and Byte access structure */
typedef union u_ether_tsn_tx_ctrl_info0_word_type /* PRQA S 1536 # Advisory R-2.4 */
{                                                 /* PRQA S 0750 # Advisory R-19.2 */
    ether_tsn_tx_ctrl_info0_type_t st_tx_ctrl;    ///< Tx control field.
    uint32_t word;                                ///< Allow access of the whole word.
} ether_tsn_tx_ctrl_info0_word_type_t;

/** Tx control information structure */
typedef struct st_ether_tsn_tx_ctrl_info1_type    /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t tfl       : 16;                      ///< Total Frame Length.
    uint32_t crt       : 1;                       ///< Calculate residence time (This bit should be always set to 0).
    uint32_t reserved0 : 15;                      ///< Reserved
} ether_tsn_tx_ctrl_info1_type_t;

/** Simplifies Bit and Byte access */
typedef union u_ether_tsn_tx_ctrl_info1_word_type /* PRQA S 1536 # Advisory R-2.4 */
{                                                 /* PRQA S 0750 # Advisory R-19.2 */
    ether_tsn_tx_ctrl_info1_type_t st_tx_ctrl;    ///< Tx control field.
    uint32_t word;                                ///< Allow access of the whole word.
} ether_tsn_tx_ctrl_info1_word_type_t;

/** Ethernet frame type */
typedef struct ether_tsn_eth_frame_type           /* PRQA S 1536 # Advisory R-2.4 */
{
    uint8_t        dst_addr[6];
    uint8_t        src_addr[6];
    uint8_t        frame_type[2];
    ether_tsn_type payload;
} ether_tsn_frame_type_t;

/* CBS */
typedef struct ether_tsn_cbs_config_type /* PRQA S 1536 # Advisory R-2.4 */
{
    ether_tsn_option_type_t ce;
    uint32_t                civ;         /* Credit Increment Value */
    uint32_t                cul;         /* Credit Upper Limit */
} ether_tsn_cbs_config_type_t;

typedef struct ether_tsn_tx_queue_type   /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t                    queue_bufs;
    uint32_t                    queue_id;
    ether_tsn_trame_type_t      frame_type;
    uint32_t                    max_frame_size;
    ether_tsn_cbs_config_type_t cbs_config;
} ether_tsn_tx_queue_type_t;

typedef struct ether_tsn_queue_config_type /* PRQA S 1536 # Advisory R-2.4 */
{
    ether_tsn_tx_queue_type_t const * p_tx_queue_config;
    uint8_t number_of_tx_queue;
} ether_tsn_queue_config_type_t;

/** ETHER extension configures the buffer descriptor for ETHER. */
typedef struct st_ether_tsn_extended_cfg                                         /* PRQA S 1536 # Advisory R-2.4 */
{
    ether_tsn_transmission_fragment_size_t ether_tsn_transmission_fragment_size; ///< Extended data descriptor structure.
    ether_tsn_queue_contanins_traffic_t    ether_tsn_queue_contanins_traffic;    ///< Extended contanins_traffic.
    ether_tsn_optionType_tx_isr_t          ether_tsn_cfg_type_tx_isr;            ///< Setting option for Tx interrupt type.
    ether_tsn_optionType_rx_isr_t          ether_tsn_cfg_type_rx_isr;            ///< Setting option for Rx interrupt type.
    ether_tsn_control_mode_t               ether_tsn_ctl_mode;                   ///< Control Enable for receive mode.
    ether_tsn_filter_action_type_t         ether_tsn_filter_action_type;         ///< Configure filter action type.
    uint8_t                    * p_mac_address_filter;                           ///< MAC address target
    ether_tsn_header_type_t      header_data_custom;                             ///< Header data
    ether_tsn_pause_frame_type_t pause_frame;                                    ///< RMACA configure pause frame
    uint32_t pause_frame_retransmission_time;                                    ///< Pause frame retransmission time
    uint32_t pause_time;                                                         ///< Transmission of frames is paused for the following time.
    ether_tsn_pause_frame_zero_type_t        pause_time_zero;                    ///< Pause Frame Transmission and Reception with TIME = 0
    ether_tsn_loopback_config_t              ether_loopback_config;              ///< Loopback mode enable
    ether_tsn_network_speed_t                ether_network_speed;                ///< Transfer Rate
    ether_tsn_phy_interface_t                ether_phy_interface;                ///< PHY Interface Select
    ether_tsn_broadcast_t                    ether_broadcast;                    ///< Reception of broadcast frames
    ether_tsn_unicast_t                      ether_unicast;                      ///< Reception of unicast frames
    ether_tsn_preemption_link_verification_t ether_preemption_link_verification; ///< Preemption Link Verification
    uint8_t * p_mac_address_target;                                              ///< Mac address target
    uint8_t   number_of_addr_filter;                                             ///< Used to determine number of address add to list muticast.
    uint16_t  ether_type;                                                        ///< Ethernet type in Header data
    uint32_t  ether_tsn_ram_size;                                                ///< Ethernet RAM size
    uint8_t * p_mem_pool_buffer_table;                                           ///< Ethernet RAM memory pool buffer table
#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)
    ether_tsn_sgmii_operation_mode_t ether_bypassMode;                           ///< SGMII operation mode select
    IRQn_Type sgmii_irq;                                                         ///< General Interrupt number for SGMII
    uint32_t  sgmii_interrupt_priority;                                          ///< General Interrupt priority for SGMII
#endif
    IRQn_Type gptp_irq;                                                          ///< GPTP Interrupt number
    uint32_t  interrupt_priority;                                                ///< Interrupt priority
    ether_tsn_queue_config_type_t queue_config;                                  ///< Queue config
} ether_tsn_extended_cfg_t;

/** Initialization occurs when open is called. */
typedef struct st_ether_tsn_instance_ctrl                                        /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t open;                                                               ///< Used to determine if the channel is configured.

    /* Configuration of ethernet module. */
    ether_cfg_t const * p_ether_cfg;                                             ///< Pointer to initial configurations.

    /* Channel 0 */
    R_TSNES0_Type  * p_reg_tsnes;                                                ///< Pointer to initial tsnes type.
    R_AXIBMI0_Type * p_reg_axibmi;                                               ///< Pointer to initial axibmi type.
    R_RMACA0_Type  * p_reg_rmaca;                                                ///< Pointer to initial rmaca type.
    R_PWRCTL0_Type * p_reg_pwrctl;                                               ///< Pointer to initial pwrctl type.
    R_GPTMA0_Type  * p_reg_gptma;                                                ///< Pointer to initial gptma type.
#if (2 == BSP_FEATURE_ETH_TSN_NUM_CHANNELS)

    /* Channel 1 */
    R_SGMII1_Type * p_reg_sgmii;                                                 ///< Pointer to initial sgmii1 type.
#endif

    /* Status of Ethernet driver. */
    ether_tsn_previous_link_status_t  previous_link_status;                      ///< Previous link status
    ether_tsn_link_change_t           link_change;                               ///< status of link change
    ether_tsn_link_establish_status_t link_establish_status;                     ///< Current Link status

    /* Pointer to callback and optional working memory */
    void (* p_callback)(ether_callback_args_t *);                                /* PRQA S 1336 # Rule-8.2 */
    ether_callback_args_t * p_callback_memory;

    /* Pointer to context to be passed into callback function */
    void * p_context;
} ether_tsn_instance_ctrl_t;

/** Filled in Interface API structure for this Instance. */
extern const ether_api_t g_ether_tsn_on_ether_tsn;

/** @endcond */

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHER_TSN_Open(ether_ctrl_t * const p_ctrl, ether_cfg_t const * const p_cfg);

fsp_err_t R_ETHER_TSN_Close(ether_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_TSN_Read(ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t * const length_bytes);

fsp_err_t R_ETHER_TSN_Write(ether_ctrl_t * const p_ctrl, void * const p_buffer, uint32_t const frame_length);

fsp_err_t R_ETHER_TSN_CallbackSet(ether_ctrl_t * const          p_ctrl,
                                  void (                      * p_callback)(ether_callback_args_t *), /* PRQA S 1336 # Rule-8.2 */
                                  void * const                  p_context,
                                  ether_callback_args_t * const p_callback_memory);

fsp_err_t R_ETHER_TSN_TxStatusGet(ether_ctrl_t * const p_ctrl, void * const p_buffer_address);

fsp_err_t R_ETHER_TSN_BufferRelease(ether_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_TSN_RxBufferUpdate(ether_ctrl_t * const p_ctrl, void * const p_buffer);

fsp_err_t R_ETHER_TSN_LinkProcess(ether_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_TSN_WriteBuffer(ether_ctrl_t * const p_ctrl, uint32_t buf_id, uint32_t const frame_length);
fsp_err_t R_ETHER_TSN_GetTxBuffer(ether_ctrl_t * const p_ctrl, /* PRQA S 1710 # Advisory D-4.5 */
                                  uint8_t              priority,
                                  uint32_t           * p_buf_id,
                                  void * const         p_buffer,
                                  uint32_t * const     frame_length);
fsp_err_t R_ETHER_TSN_TxConfirm(ether_ctrl_t * const p_ctrl);
fsp_err_t R_ETHER_TSN_SetMACAddr(ether_ctrl_t * const p_ctrl, const uint8_t * p_mac_address);
fsp_err_t R_ETHER_TSN_GetMACAddr(ether_ctrl_t * const p_ctrl, uint8_t * p_mac_address);

fsp_err_t R_ETHER_TSN_EnableTxTimestamp(ether_ctrl_t * const p_ctrl, uint32_t buf_id);
fsp_err_t R_ETHER_TSN_GetTxTimestamp(ether_ctrl_t * const          p_ctrl,
                                     uint32_t                      buf_id,
                                     ether_tsn_time_stamp_type_t * p_time_stamp);
fsp_err_t R_ETHER_TSN_GetRxTimestamp(ether_ctrl_t * const p_ctrl, ether_tsn_time_stamp_type_t * p_time_stamp);

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_TSN)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif                                 /* R_ETHER_TSN_H */
