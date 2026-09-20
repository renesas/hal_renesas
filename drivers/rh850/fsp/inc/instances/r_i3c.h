/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_I3C_H
#define R_I3C_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_i3c_api.h"
#include "r_i3c_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup I3C
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Index for selecting the device defined in the extended address table. */
#define I3C_DEVICE_INDEX_EXTENDED_DEVICE             (1U << 5U)

/* PRQA S 1534 ++ # Advisory R-2.5 */
/** Event Status Provided by the callback. */
#define I3C_EVENT_STATUS_SUCCESS                     (0x0U)  ///< The transfer was completed as expected.
#define I3C_EVENT_STATUS_PARITY                      (0x2U)  ///< A parity error was detected.
#define I3C_EVENT_STATUS_FRAME                       (0x3U)  ///< A frame error was detected.
#define I3C_EVENT_STATUS_ADDRESS_HEADER              (0x4U)  ///< An Address Header error was detected.
#define I3C_EVENT_STATUS_NACK                        (0x5U)  ///< The transfer was NACK'd.
#define I3C_EVENT_STATUS_OVERFLOW                    (0x6U)  ///< A Receive FIFO overflow or Transmit FIFO underflow occurred.
#define I3C_EVENT_STATUS_ABORTED_TO_MASTER           (0x7U)  ///< In slave mode, the write transfer was ended via the 'T' bit.
#define I3C_EVENT_STATUS_ABORTED                     (0x8U)  ///< In master mode, the transfer was aborted.
#define I3C_EVENT_STATUS_NOT_SUPPORTED               (0xAU)  ///< Operation is not supported.
#define I3C_EVENT_STATUS_IBI_NACK_DISABLED           (0x20U) ///< An IBI was NACK'd and the a DISEC command was sent.

/* PRQA S 1534 -- */

/* Bitfield definitions for Command Descriptor fields. */
#define I3C_CMD_DESC_CMD_ATTR_Pos                    (0U)
#define I3C_CMD_DESC_CMD_ATTR_Msk                    ((uint32_t) 7U << I3C_CMD_DESC_CMD_ATTR_Pos) /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_CMD_DESC_CMD_ATTR_XFER                   (0U)                                         /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_CMD_DESC_CMD_ATTR_IMMED_DATA_XFER        (1U)
#define I3C_CMD_DESC_CMD_ATTR_ADDR_ASSGN_CMD         (2U)
#define I3C_CMD_DESC_CMD_ATTR_INTERNAL_CONTROL       (7U)
#define I3C_CMD_DESC_MIPI_CMD_Pos                    (8U)
#define I3C_CMD_DESC_ON_OFF_Pos                      (12U)
#define I3C_CMD_DESC_TID_Pos                         (3U)
#define I3C_CMD_DESC_TID_Msk                         ((uint32_t) 0x0FU << I3C_CMD_DESC_TID_Pos) /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_CMD_DESC_CMD_Pos                         (7U)
#define I3C_CMD_DESC_CMD_Msk                         ((uint32_t) 0xFFU << I3C_CMD_DESC_CMD_Pos) /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_CMD_DESC_DEV_INDEX_Pos                   (16U)
#define I3C_CMD_DESC_DEV_INDEX_Msk                   ((uint32_t) 0x01FFU << I3C_CMD_DESC_DEV_INDEX_Pos)
#define I3C_CMD_DESC_ROC_Pos                         (30U)
#define I3C_CMD_DESC_ROC_Msk                         ((uint32_t) 1U << I3C_CMD_DESC_ROC_Pos)
#define I3C_CMD_DESC_TOC_Pos                         (31U)
#define I3C_CMD_DESC_TOC_Msk                         ((uint32_t) 1U << I3C_CMD_DESC_TOC_Pos)

/* Command Descriptor definitions for Address Assign Commands. */
#define I3C_CMD_DESC_ADDR_ASSGN_DEV_COUNT_Pos        (26U)
#define I3C_CMD_DESC_ADDR_ASSGN_DEV_COUNT_Msk        ((uint32_t) 0x0FU << I3C_CMD_DESC_ADDR_ASSGN_DEV_COUNT_Pos) /* PRQA S 1534 # Advisory R-2.5 */

/* Command Descriptor definitions for Regular Transfer Commands. */
#define I3C_CMD_DESC_XFER_CP_Pos                     (15U)
#define I3C_CMD_DESC_XFER_CP_Msk                     ((uint32_t) 1U << I3C_CMD_DESC_XFER_CP_Pos)
#define I3C_CMD_DESC_XFER_HJ_Pos                     (15U)
#define I3C_CMD_DESC_XFER_HJ_Msk                     ((uint32_t) 1U << I3C_CMD_DESC_XFER_HJ_Pos)
#define I3C_CMD_DESC_XFER_LENGTH_Pos                 (16U)
#define I3C_CMD_DESC_XFER_LENGTH_Msk                 ((uint32_t) 0xFFFFU << I3C_CMD_DESC_XFER_LENGTH_Pos)
#define I3C_CMD_DESC_XFER_MODE_Pos                   (26U)
#define I3C_CMD_DESC_XFER_MODE_Msk                   ((uint32_t) 0x07U << I3C_CMD_DESC_XFER_MODE_Pos)
#define I3C_CMD_DESC_XFER_RNW_Pos                    (29U)
#define I3C_CMD_DESC_XFER_RNW_Msk                    ((uint32_t) 1U << I3C_CMD_DESC_XFER_RNW_Pos)
#define I3C_CMD_DESC_XFER_MODE_I2C_STDBR             (0U) /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_CMD_DESC_XFER_MODE_I2C_EXTBR             (1U) /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_CMD_DESC_XFER_MODE_I3C_SDR_EXTBR_X2      (3U) /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_CMD_DESC_XFER_MODE_I3C_SDR_EXTBR_X4      (4U) /* PRQA S 1534 # Advisory R-2.5 */

/* Command Descriptor definitions for Immediate Data Transfer Commands. */
#define I3C_CMD_DESC_IMMED_DATA_XFER_BYTE_CNT_Pos    (23U)

/* Bitfield definitions for the Response Status Descriptor fields. */
#define I3C_RESP_STATUS_DESC_DATA_LENGTH_Pos         (0U)
#define I3C_RESP_STATUS_DESC_DATA_LENGTH_Msk         ((uint32_t) 0xFFFFU << I3C_RESP_STATUS_DESC_DATA_LENGTH_Pos)
#define I3C_RESP_STATUS_DESC_TID_Pos                 (24U)                                              /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_RESP_STATUS_DESC_TID_Msk                 ((uint32_t) 0x0FU << I3C_RESP_STATUS_DESC_TID_Pos) /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_RESP_STATUS_DESC_ERR_STATUS_Pos          (28U)
#define I3C_RESP_STATUS_DESC_ERR_STATUS_Msk          ((uint32_t) 0x0FU << I3C_RESP_STATUS_DESC_ERR_STATUS_Pos)

/* Bitfield definitions for Receive Status Descriptor fields. */
#define I3C_RECV_STATUS_DESC_DATA_LENGTH_Pos         (0U)
#define I3C_RECV_STATUS_DESC_DATA_LENGTH_Msk         ((uint32_t) 0xFFFFU << I3C_RECV_STATUS_DESC_DATA_LENGTH_Pos)
#define I3C_RECV_STATUS_DESC_CMD_Pos                 (16U)
#define I3C_RECV_STATUS_DESC_CMD_Msk                 ((uint32_t) 0xFFU << I3C_RECV_STATUS_DESC_CMD_Pos)
#define I3C_RECV_STATUS_DESC_SDR_R_W_TYPE_Pos        (23U)
#define I3C_RECV_STATUS_DESC_SDR_R_W_TYPE_Msk        ((uint32_t) 0x01U << I3C_RECV_STATUS_DESC_SDR_R_W_TYPE_Pos)
#define I3C_RECV_STATUS_DESC_ERR_STATUS_Pos          (24U)
#define I3C_RECV_STATUS_DESC_ERR_STATUS_Msk          ((uint32_t) 0x07U << I3C_RECV_STATUS_DESC_ERR_STATUS_Pos)
#define I3C_RECV_STATUS_DESC_TRANSFER_TYPE_Pos       (27U)
#define I3C_RECV_STATUS_DESC_TRANSFER_TYPE_Msk       ((uint32_t) 0x03U << I3C_RECV_STATUS_DESC_TRANSFER_TYPE_Pos)
#define I3C_RECV_STATUS_DESC_DEV_INDEX_Pos           (29U)                                                    /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_RECV_STATUS_DESC_DEV_INDEX_Msk           ((uint32_t) 0x07U << I3C_RECV_STATUS_DESC_DEV_INDEX_Pos) /* PRQA S 1534 # Advisory R-2.5 */

/* Bitfield definitions for IBI Status Descriptor fields. */
#define I3C_IBI_STATUS_DESC_LENGTH_Pos               (0U)
#define I3C_IBI_STATUS_DESC_LENGTH_Msk               ((uint32_t) 0xFFU << I3C_IBI_STATUS_DESC_LENGTH_Pos)
#define I3C_IBI_STATUS_DESC_IBI_ID_Pos               (8U)
#define I3C_IBI_STATUS_DESC_IBI_ID_Msk               ((uint32_t) 0xFFU << I3C_IBI_STATUS_DESC_IBI_ID_Pos)
#define I3C_IBI_STATUS_DESC_LAST_STATUS_Pos          (24U)
#define I3C_IBI_STATUS_DESC_LAST_STATUS_Msk          ((uint32_t) 1U << I3C_IBI_STATUS_DESC_LAST_STATUS_Pos)
#define I3C_IBI_STATUS_DESC_TS_Pos                   (25U)                                         /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_IBI_STATUS_DESC_TS_Msk                   ((uint32_t) 1U << I3C_IBI_STATUS_DESC_TS_Pos) /* PRQA S 1534 # Advisory R-2.5 */
#define I3C_IBI_STATUS_DESC_ERR_STATUS_Pos           (26U)
#define I3C_IBI_STATUS_DESC_ERR_STATUS_Msk           ((uint32_t) 0x7U << I3C_IBI_STATUS_DESC_ERR_STATUS_Pos)
#define I3C_IBI_STATUS_DESC_IBI_ST_Pos               (31U)
#define I3C_IBI_STATUS_DESC_IBI_ST_Msk               ((uint32_t) 1U << I3C_IBI_STATUS_DESC_IBI_ST_Pos)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Bitrate settings that can be selected at run-time using @ref i3c_api_t::deviceSelect. */
typedef enum e_i3c_bitrate_setting               /* PRQA S 1536 # Advisory R-2.4 */
{
    I3C_BITRATE_MODE_I2C_STDBR         = 0U,     ///< Use standard period setting for subsequent I2C transfers.
    I3C_BITRATE_MODE_I2C_EXTBR         = 1U,     ///< Use extended period setting for subsequent I2C transfers.
    I3C_BITRATE_MODE_I3C_SDR0_STDBR    = 0U,     ///< Use standard period setting for subsequent I3C SDR transfers.
    I3C_BITRATE_MODE_I3C_SDR1_EXTBR    = 1U,     ///< Use extended period setting for subsequent I3C SDR transfers.
    I3C_BITRATE_MODE_I3C_SDR2_STDBR_X2 = 2U,     ///< Use standard period setting x 2 for subsequent I3C SDR transfers.
    I3C_BITRATE_MODE_I3C_SDR3_EXTBR_X2 = 3U,     ///< Use extended period setting x 2 for subsequent I3C SDR transfers.
    I3C_BITRATE_MODE_I3C_SDR4_EXTBR_X4 = 4U,     ///< Use extended period setting x 4 for subsequent I3C SDR transfers.
    I3C_BITRATE_MODE_I3C_HDR_DDR_STDBR = 6U,     ///< Use standard period setting for subsequent I3C HDR-DDR transfers.
} i3c_bitrate_mode_t;

/** Supported activity states for ENTASn Command (See ENTASn in the MIPI I3C Specification v1.0). */
typedef enum e_i3c_activity_state                /* PRQA S 1536 # Advisory R-2.4 */
{
    I3C_ACTIVITY_STATE_ENTAS0 = 0U,              ///< Activity Interval (1 microsecond).
    I3C_ACTIVITY_STATE_ENTAS1 = 1U,              ///< Activity Interval (100 microseconds).
    I3C_ACTIVITY_STATE_ENTAS2 = 2U,              ///< Activity Interval (2 milliseconds).
    I3C_ACTIVITY_STATE_ENTAS3 = 3U,              ///< Activity Interval (50 milliseconds).
} i3c_activity_state_t;

/** Supported MIPI Alliance Command */
typedef enum e_i3c_mipi_cmd                      /* PRQA S 1536 # Advisory R-2.4 */
{
    I3C_MIPI_CMD_IGNORE_IBA  = 0U,               ///< ignore include IBA
    I3C_MIPI_CMD_INCLUDE_IBA = 2U,               ///< Include IBA
} i3c_mipi_cmd_t;

/** Maximum Sustained Data Rate for non-CCC messages sent by Master Device to Slave Device (See GETMXDS in the MIPI I3C Specification v1.0). */
typedef enum e_i3c_data_rate_setting             /* PRQA S 1536 # Advisory R-2.4 */
{
    I3C_DATA_RATE_SETTING_FSCL_MAX = 0U,         ///< There is no data rate limit.
    I3C_DATA_RATE_SETTING_8MHZ     = 1U,         ///< The max sustained data rate is 8 Mhz.
    I3C_DATA_RATE_SETTING_6MHZ     = 2U,         ///< The max sustained data rate is 6 Mhz.
    I3C_DATA_RATE_SETTING_4MHZ     = 3U,         ///< The max sustained data rate is 4 Mhz.
    I3C_DATA_RATE_SETTING_2MHZ     = 4U,         ///< The max sustained data rate is 2 Mhz.
} i3c_data_rate_setting_t;

/** Clock to Data Turnaround Time (See GETMXDS in the MIPI I3C Specification v1.0). */
typedef enum e_i3c_clock_data_turnaround         /* PRQA S 1536 # Advisory R-2.4 */
{
    I3C_CLOCK_DATA_TURNAROUND_8NS      = 0U,     ///< Clock to turnaround time is 8 nanoseconds or less.
    I3C_CLOCK_DATA_TURNAROUND_9NS      = 1U,     ///< Clock to turnaround time is 9 nanoseconds or less.
    I3C_CLOCK_DATA_TURNAROUND_10NS     = 2U,     ///< Clock to turnaround time is 10 nanoseconds or less.
    I3C_CLOCK_DATA_TURNAROUND_11NS     = 3U,     ///< Clock to turnaround time is 11 nanoseconds or less.
    I3C_CLOCK_DATA_TURNAROUND_12NS     = 4U,     ///< Clock to turnaround time is 12 nanoseconds or less.
    I3C_CLOCK_DATA_TURNAROUND_EXTENDED = 7U,     ///< Clock to turnaround time is greater than 12 nanoseconds.
} i3c_clock_data_turnaround_t;

/** Clock stalling settings. */
typedef struct st_i3c_clock_stalling             /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t assigned_address_phase_enable : 1;  ///< Enable Clock Stalling during the address phase of the ENTDAA command.
    uint32_t transition_phase_enable       : 1;  ///< Enable Clock Stalling during the transition bit in read transfers.
    uint32_t parity_phase_enable           : 1;  ///< Enable Clock Stalling during the parity bit period in write transfers.
    uint32_t ack_phase_enable              : 1;  ///< Enable Clock Stalling during the ACK/NACK phase.
    uint32_t reserved                      : 28; ///< Reserved bits
    uint16_t clock_stalling_time;                ///< The amount of time to stall the clock in I3C source clock ticks.
} i3c_clock_stalling_t;

/** Bitrate settings for configuring the SCL clock frequency. */
typedef struct st_i3c_bitrate_settings           /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t stdbr;                              ///< The standard bitrate settings.
    uint32_t extbr;                              ///< The extended bitrate settings.

    /** Clock Stalling settings (See Master Clock Stalling in the MIPI I3C Specification v1.0). */
    i3c_clock_stalling_t clock_stalling;
} i3c_bitrate_settings_t;

/** Settings for controlling the drivers behavior in response to IBIs. */
typedef struct st_i3c_ibi_control                     /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t hot_join_acknowledge                : 1; ///< If false, NACK all Hot Join requests.
    uint32_t notify_rejected_hot_join_requests   : 1; ///< Notify the application when an IBI Hot-Join request has been NACK'd.
    uint32_t notify_rejected_mastership_requests : 1; ///< Notify the application when an IBI Mastership request has been NACK'd.
    uint32_t notify_rejected_interrupt_requests  : 1; ///< Notify the application when an IBI Interrupt request has been NACK'd.
} i3c_ibi_control_t;

/** Select High Priority or Normal FIFO Buffer Transfer (for master only)**/
typedef enum e_i3c_fifo_buffer_transfer               /* PRQA S 1536 # Advisory R-2.4 */
{
    I3C_FIFO_BUFFER_TRANSFER_NORMAL = 0U,             ///< Master uses Normal FIFO Buffer Transfer
    I3C_FIFO_BUFFER_TRANSFER_HIGH_PRIORITY            ///< Master uses High Priority FIFO Buffer Transfer
} i3c_fifo_buffer_transfer_t;

/** Default configuration settings for the slave response to Direct Get Common Command Codes. */
typedef struct st_i3c_slave_command_response_info     /* PRQA S 1536 # Advisory R-2.4 */
{
    /** Slave Event Settings (See ENEC and DISEC in the MIPI I3C Specification v1.0). */
    bool inband_interrupt_enable;                     ///< Enable IBI interrupts.
    bool hotjoin_request_enable;                      ///< Enable Hot-Join requests.

    /** Starting Activity State (See ENTASn in the MIPI I3C Specification v1.0). */
    i3c_activity_state_t activity_state;

    /** Max Write Length (See SETMWL and GETMWL in the MIPI I3C Specification v1.0). */
    uint16_t write_length;

    /** Max Read Length (See SETMRL and GETMRL in the MIPI I3C Specification v1.0). */
    uint16_t read_length;

    /** Number of bytes that will be written by an IBI (See SETMRL and GETMRL in the MIPI I3C Specification v1.0). */
    uint8_t ibi_payload_length;

    /** Max Data Rate Settings (See GETMXDS in the MIPI I3C Specification v1.0). */
    i3c_data_rate_setting_t     write_data_rate;       ///< Max Write Data Rate.
    i3c_data_rate_setting_t     read_data_rate;        ///< Max Read Data Rate.
    i3c_clock_data_turnaround_t clock_data_turnaround; ///< Max Data Speed Turnaround.
    bool     read_turnaround_time_enable;              ///< Enable transmission of the of the Max Read Turnaround Time.
    uint32_t read_turnaround_time;                     ///< Max Read Turnaround Time.

    /** This byte represents the Slave's internal oscillator frequency in increments of 0.5 MHz (500kHz), up to 127.5 MHz.
     * (See GETXTIME in the MIPI I3C Specification v1.1). */
    uint8_t oscillator_frequency;

    /** Oscillator inaccuracy in 0.5% increments of 0% up to 25.5% (See GETXTIME in the MIPI I3C Specification v1.1). */
    uint8_t oscillator_inaccuracy;

    bool hdr_ddr_support;                    ///< HDR-DDR mode is supported.
    bool hdr_tsp_support;                    ///< HDR-TSP mode is supported.
    bool hdr_tsl_support;                    ///< HDR-TSL mode is supported.
} i3c_slave_command_response_info_t;

/* Buffer descriptor for keeping track of a read transfer. */
typedef struct st_i3c_read_buffer_descriptor /* PRQA S 1536 # Advisory R-2.4 */
{
    uint8_t * p_buffer;                      ///< Pointer to a buffer for storing data that has been read.
    uint32_t  count;                         ///< Number of bytes that have been read.
    uint32_t  buffer_size;                   ///< Total size of the buffer.

    /** Flag for keeping track of whether the @ref I3C_EVENT_READ_BUFFER_FULL callback has been issued. */
    bool read_request_issued;
} i3c_read_buffer_descriptor_t;

/* Buffer descriptor for keeping track of a write transfer. */
typedef struct st_i3c_write_buffer_descriptor                /* PRQA S 1536 # Advisory R-2.4 */
{
    uint8_t * p_buffer;                                      ///< Pointer to a buffer that will be read during a write transfer.
    uint32_t  count;                                         ///< Number of bytes that have been written.
    uint32_t  buffer_size;                                   ///< Total size of the buffer.
} i3c_write_buffer_descriptor_t;

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when @ref i3c_api_t::open is called. */
typedef struct st_i3c_instance_ctrl                          /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t           open;                                 ///< Indicates whether the open() API has been successfully called.
    R_RI3C0_Type     * p_reg;                                ///< Base register for this channel
    volatile uint32_t  internal_state;                       ///< Used to track the current state of the driver.
    uint8_t            current_command_code;                 ///< The current Common Command Code that is being transferred.
    uint32_t           device_index;                         ///< The device index selected using @ref i3c_api_t::deviceSelect.
    i3c_bitrate_mode_t device_bitrate_mode;                  ///< Runtime bitrate settings to use for the next transfer.
    i3c_slave_info_t   current_slave_info;                   ///< The last @ref i3c_slave_info_t read during ENTDAA.

    uint32_t next_word;                                      ///< The next word that will be written to the FIFO.
    uint32_t ibi_next_word;                                  ///< The next word that will be written to the IBI FIFO.
    i3c_write_buffer_descriptor_t write_buffer_descriptor;   ///< Buffer descriptor for keeping track of a write transfer.
    i3c_read_buffer_descriptor_t  read_buffer_descriptor;    ///< Buffer descriptor for keeping track of a read transfer.
    i3c_read_buffer_descriptor_t  ibi_buffer_descriptor;     ///< Buffer descriptor for keeping track of an IBI read/write transfer.
    volatile bool                 activation_on_rxi;         ///< Tracks whether the transfer is activated on RXI interrupt
    volatile bool                 activation_on_txi;         ///< Tracks whether the transfer is activated on TXI interrupt
    volatile uint32_t             read_transfer_count_final; ///< The total number of bytes read during a read transfer.
    volatile uint32_t             ibi_transfer_count_final;  ///< The total number of bytes read during an IBI transfer.
    i3c_cfg_t const             * p_cfg;                     ///< A pointer to the configuration structure provided during open.
} i3c_instance_ctrl_t;

/** Extended configuration for r_i3c. */
typedef struct st_i3c_extended_cfg                           /* PRQA S 1536 # Advisory R-2.4 */
{
    i3c_bitrate_settings_t bitrate_settings;                 ///< Bitrate settings configuring the frequency and duty cycle for SCL.
    i3c_ibi_control_t      ibi_control;                      ///< Configure the driver's behavior in response to IBIs.

    /**
     * The time in I3C reference clock ticks needed in order to detect the bus free condition
     * (See "Bus Free Condition" in the MIPI I3C Specification v1.0).
     */
    uint32_t bus_free_detection_time;

    /**
     * The time in I3C reference clock ticks needed in order to detect the bus available condition
     * (See "Bus Available Condition" in the MIPI I3C Specification v1.0).
     */
    uint32_t bus_available_detection_time;

    /**
     * The time in I3C reference clock ticks needed in order to detect the bus idle condition
     * (See "Bus Idle Condition" in the MIPI I3C Specification v1.0).
     */
    uint32_t bus_idle_detection_time;

    bool timeout_detection_enable;            ///< Notify the application if SCL is stuck high or low.

    i3c_fifo_buffer_transfer_t transfer_mode; ///< Select High Priority or Normal FIFO Buffer Transfer

    bool legacy_inclusive_bus_mode;           ///< Enables the Bus mode selection for Legacy inclusive.

    bool broadcast_address;                   ///< Enables the transmission of the broadcast address on the I3C bus.

    bool secondary_master_enable;             ///< Enables Secondary master device type.

    i3c_mipi_cmd_t mipi_cmd;                  ///< The MIPI Alliance command will either be ignored or include IBA.

    /** Initial settings for configuring the slave's responses to received commands. */
    i3c_slave_command_response_info_t slave_command_response_info;

    IRQn_Type resp_irq;                ///< Response Queue Full IRQ number.
    IRQn_Type rx_irq;                  ///< Receive FIFO Full IRQ number.
    IRQn_Type tx_irq;                  ///< Transmit FIFO Empty IRQ number.
    IRQn_Type rcv_irq;                 ///< Receive Status Queue Full IRQ number.
    IRQn_Type ibi_irq;                 ///< IBI IRQ number.
    IRQn_Type eei_irq;                 ///< EEI IRQ number.
    IRQn_Type hresp_irq;               ///< High Response Queue Full IRQ number.
    IRQn_Type hrx_irq;                 ///< High Receive FIFO Full IRQ number.
    IRQn_Type htx_irq;                 ///< High Transmit FIFO Empty IRQ number.
    uint8_t   ipl;                     ///< Interrupt Priority for Resp, Rx, Tx, and RCV IRQs.
    uint8_t   eei_ipl;                 ///< Error and Event Interrupt Priority.
} i3c_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const i3c_api_t g_i3c_on_i3c;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_I3C_Open(i3c_ctrl_t * const p_api_ctrl, i3c_cfg_t const * const p_cfg);
fsp_err_t R_I3C_Enable(i3c_ctrl_t * const p_api_ctrl);
fsp_err_t R_I3C_DeviceCfgSet(i3c_ctrl_t * const p_api_ctrl, i3c_device_cfg_t const * const p_device_cfg);
fsp_err_t R_I3C_MasterDeviceTableSet(i3c_ctrl_t * const                   p_api_ctrl,
                                     uint32_t                             device_index,
                                     i3c_device_table_cfg_t const * const p_device_table_cfg);
fsp_err_t R_I3C_SlaveStatusSet(i3c_ctrl_t * const p_api_ctrl, i3c_device_status_t status);
fsp_err_t R_I3C_DeviceSelect(i3c_ctrl_t * const p_api_ctrl, uint32_t device_index, uint32_t bitrate_mode);
fsp_err_t R_I3C_DynamicAddressAssignmentStart(i3c_ctrl_t * const            p_api_ctrl,
                                              i3c_address_assignment_mode_t address_assignment_mode,
                                              uint32_t                      starting_device_index,
                                              uint32_t                      device_count);
fsp_err_t R_I3C_CommandSend(i3c_ctrl_t * const p_api_ctrl, i3c_command_descriptor_t const * const p_command_descriptor);
fsp_err_t R_I3C_Write(i3c_ctrl_t * const p_api_ctrl, uint8_t const * const p_data, uint32_t length, bool restart);
fsp_err_t R_I3C_Read(i3c_ctrl_t * const p_api_ctrl, uint8_t * const p_data, uint32_t length, bool restart);
fsp_err_t R_I3C_IbiWrite(i3c_ctrl_t * const    p_api_ctrl,
                         i3c_ibi_type_t        ibi_type,
                         uint8_t const * const p_data,
                         uint32_t              length);
fsp_err_t R_I3C_IbiRead(i3c_ctrl_t * const p_api_ctrl, uint8_t * const p_data, uint32_t length);
fsp_err_t R_I3C_Close(i3c_ctrl_t * const p_api_ctrl);

/*******************************************************************************************************************//**
 * @} (end addtogroup I3C)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_I3C_H */
