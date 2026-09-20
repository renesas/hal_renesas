/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_STORAGE_INTERFACES
 * @defgroup MMCA_API MMC Interface
 * @brief Interface for accessing eMMC only.
 *
 * @section MMCA_API_SUMMARY Summary
 * The r_mmca interface provides standard eMMC media functionality Standard JESD-A441.
 *
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_MMCA_API_H
#define R_MMCA_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#include "r_transfer_api.h"
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** MMC data bus is 1, 4 or 8 bits wide. */
typedef enum e_mmca_bus_width
{
    MMCA_BUS_WIDTH_1_BIT = 0,          ///< Data bus is 1 bit wide.
    MMCA_BUS_WIDTH_4_BIT = 1,          ///< Data bus is 4 bits wide.
    MMCA_BUS_WIDTH_8_BIT = 2,          ///< Data bus is 8 bits wide.
} mmca_bus_width_t;

/** Configuration for writing to EXT_CSD. All fields are raw values per eMMC specification */
typedef struct st_mmca_boot_cfg
{
    uint8_t boot_ack;                  ///< 0: disable, 1: enable (EXT_CSD[179].bit6)
    uint8_t boot_partition_enable;     ///< 3-bit (EXT_CSD[179].bits5:3)
    uint8_t boot_partition_access;     ///< 3-bit (EXT_CSD[179].bits2:0)
    uint8_t boot_bus_width;            ///< 3-bit (EXT_CSD[177].bits2:0): 0=x1, 1=x4, 2=x8
    uint8_t reset_boot_bus_width;      ///< 3-bit (EXT_CSD[177].bits5:3)
} mmca_boot_cfg_t;

/** Interrupt Events */
typedef enum e_mmca_event
{
    MMCA_EVENT_CMD12_AND_BUFFER_READ        = ((uint32_t) 1U << 26), ///< Automatic CMD12 Issuance & Buffer Read Complete
    MMCA_EVENT_CMD12_RESPONSE_BUSY_COMPLETE = ((uint32_t) 1U << 25), ///< Automatic CMD12 Issuance Response Busy Complete
    MMCA_EVENT_CMD12_RESPONSE_COMPLETE      = ((uint32_t) 1U << 24), ///< Automatic CMD12 Response Complete
    MMCA_EVENT_DATA_TRANSMISSION_COMPLETE   = ((uint32_t) 1U << 23), ///< Data Transmission Complete
    MMCA_EVENT_BUFFER_READ_COMPLETE         = ((uint32_t) 1U << 22), ///< Buffer Read Complete
    MMCA_EVENT_BUFFER_WRITE_READY           = ((uint32_t) 1U << 21), ///< Buffer Write Ready
    MMCA_EVENT_BUFFER_READ_READY            = ((uint32_t) 1U << 20), ///< Buffer Read Ready
    MMCA_EVENT_RESPONSE_BUSY_COMPLETE       = ((uint32_t) 1U << 17), ///< Response Busy Complete
    MMCA_EVENT_COMMAND_RESPONSE_COMPLETE    = ((uint32_t) 1U << 16), ///< Command/Response Complete
    MMCA_EVENT_COMMAND_ERROR                = ((uint32_t) 1U << 15), ///< Command Issuance Error
    MMCA_EVENT_BUFFER_ACCESS_ERROR          = ((uint32_t) 1U << 14), ///< Buffer Access Error
    MMCA_EVENT_WRITE_DATA_ERROR             = ((uint32_t) 1U << 11), ///< Write Data Error
    MMCA_EVENT_READ_ERROR            = ((uint32_t) 1U << 10),        ///< Read Data Error
    MMCA_EVENT_RESPONSE_INDEX_ERROR  = ((uint32_t) 1U << 9),         ///< Response Index Error
    MMCA_EVENT_RESPONSE_ERROR        = ((uint32_t) 1U << 8),         ///< Response Error
    MMCA_EVENT_CRC_STATUS_TIMEOUT    = ((uint32_t) 1U << 4),         ///< CRC Status Timeout
    MMCA_EVENT_WRITE_DATA_TIMEOUT    = ((uint32_t) 1U << 3),         ///< Write Data Timeout
    MMCA_EVENT_READ_TIMEOUT          = ((uint32_t) 1U << 2),         ///< Read Data Timeout
    MMCA_EVENT_RESPONSE_BUSY_TIMEOUT = ((uint32_t) 1U << 1),         ///< Response Busy Timeout
    MMCA_EVENT_RESPONSE_TIMEOUT      = ((uint32_t) 1U << 0),         ///< Response Timeout
    MMCA_EVENT_DMA_TX_COMPLETED      = ((uint32_t) 1U << 27),        ///< Buffer Write Complete (DMAC) - synthetic, not a hardware register bit
    MMCA_EVENT_DMA_RX_COMPLETED      = ((uint32_t) 1U << 28),        ///< Buffer Read Complete (DMAC) - synthetic, not a hardware register bit
    MMCA_EVENT_INITIALIZATION        = 0U,                           ///< Initialize event
} mmca_event_t;

/** Card state when receiving the prior command. */
typedef enum e_mmca_r1_state
{
    MMCA_R1_STATE_IDLE     = 0,        ///< Idle State           - Card identification mode
    MMCA_R1_STATE_READY    = 1,        ///< Ready State          - Card identification mode
    MMCA_R1_STATE_IDENT    = 2,        ///< Identification State - Card identification mode
    MMCA_R1_STATE_STBY     = 3,        ///< Stand-by State       - Data Transfer mode
    MMCA_R1_STATE_TRAN     = 4,        ///< Transfer State       - Data Transfer mode
    MMCA_R1_STATE_DATA     = 5,        ///< Sending-data State   - Data Transfer mode
    MMCA_R1_STATE_RCV      = 6,        ///< Receive-data State   - Data Transfer mode
    MMCA_R1_STATE_PRG      = 7,        ///< Programming State    - Data Transfer mode
    MMCA_R1_STATE_DIS      = 8,        ///< Disconnect State     - Data Transfer mode
    MMCA_R1_STATE_PRE_IDLE = 9,        ///< Pre-Idle State       - Boot mode
    MMCA_R1_STATE_PRE_BOOT = 10,       ///< Pre-boot State       - Boot mode
    MMCA_R1_STATE_BOOT     = 11,       ///< Boot State           - Boot mode
} mmca_r1_state_t;

/** MMCA High Speed Mode */
typedef enum e_mmca_high_speed_mode
{
    MMCA_HIGH_SPEED_MODE_DISABLED = 0, ///< Disabled
    MMCA_HIGH_SPEED_MODE_ENABLED  = 1, ///< Enabled
} mmca_high_speed_mode_t;

/** MMCA Background Operation Auto enabled. */
typedef enum e_mmca_bkops_auto_enabled
{
    MMCA_BKOPS_AUTO_DISABLE = 0,       ///< Background Operation Auto is disabled
    MMCA_BKOPS_AUTO_ENABLED = 1,       ///< Background Operation Auto is enabled
} mmca_bkops_auto_enabled_t;

/** MMCA boot information */
typedef struct st_mmca_boot_info
{
    uint32_t boot_size;                ///< Boot partition sector size
    uint8_t  boot_ack;                 ///< Boot acknowledge
    uint8_t  boot_enab;                ///< Boot partition enable
    uint8_t  boot_acc;                 ///< Boot partition access
    uint8_t  reset_boot_width;         ///< Reset or retain boot bus width
    uint8_t  boot_width;               ///< Current boot bus width
} mmca_boot_info_t;

/** MMCA boot access guard parameter */
typedef struct st_mmca_boot_access_guard
{
    uint8_t saved_boot_cfg;            ///< EXT_CSD[179] value before modification
    uint8_t valid;                     ///< 1: guard is valid and needs restore
} mmca_boot_access_guard_t;

/** Structure for decoding the response of a command.  For advanced use only. */
typedef union u_mmc_response
{
    union
    {
        uint32_t status;

        /** MMCA Card Status Register. */
        struct
        {
            uint32_t reserved_test  : 2; ///< Reserved for manufacturer Test Mode
            uint32_t reserved_app   : 2; ///< Reserved for Application Specific commands
            uint32_t reserved_4     : 1;
            uint32_t app_cmd        : 1; ///< The card will expect ACMD, or an indication that the command has been interpreted as ACMD
            uint32_t urgent_bkops   : 1; ///< Extension Functions may set this bit to get host to deal with events
            uint32_t switch_error   : 1;
            uint32_t ready_for_data : 1; ///< Corresponds to the buffer empty signaling on the bus

            /* The state of the card when receiving the command. If the command execution causes a state change, it will be
             *   visible to the host in the response to the next command. */
            unsigned int current_state      : 4;
            uint32_t     erase_reset        : 1;
            uint32_t     reserved_14        : 1;
            uint32_t     wp_erase_skip      : 1;
            uint32_t     cid_csd_overwrite  : 1;
            uint32_t     overrun            : 2;
            uint32_t     error              : 1;
            uint32_t     cc_error           : 1;
            uint32_t     card_ecc_failed    : 1;
            uint32_t     illegal_command    : 1;
            uint32_t     com_crc_error      : 1;
            uint32_t     lock_unlock_failed : 1;
            uint32_t     card_is_locked     : 1;
            uint32_t     wp_violation       : 1;
            uint32_t     erase_param        : 1;
            uint32_t     erase_seq_error    : 1;
            uint32_t     block_len_error    : 1;
            uint32_t     address_error      : 1;
            uint32_t     out_of_range       : 1;
        } status_b;
    } card_status;

    union
    {
        uint8_t array[16];

        /** All structures in this union must be packed since some bitfields span more than one 32-bit word. */
        struct
        {
            volatile uint32_t response0;
            volatile uint32_t response1;
            volatile uint32_t response2;
            volatile uint32_t response3;
        } reg;

        /** All structures in this union must be packed since some bitfields span more than one 32-bit word. */
        struct
        {
            uint32_t reserved_8         : 2;  ///< [9:8]
            uint32_t file_format        : 2;  ///< [11:10] file format
            uint32_t tmp_write_protect  : 1;  ///< [12]    temporary write protection
            uint32_t perm_write_protect : 1;  ///< [13]    permanent write protection
            uint32_t copy               : 1;  ///< [14]    copy flag
            uint32_t file_format_grp    : 1;  ///< [15]    file format group
            uint32_t content_prot_app   : 1;  ///< [16]    Content protection application
            uint32_t reserved_17        : 2;  ///< [20:17]
            uint32_t write_bl_partial   : 1;  ///< [21]    partial blocks for write allowed
            uint32_t write_bl_len       : 4;  ///< [25:22] max. write data block length
            uint32_t r2w_factor         : 3;  ///< [28:26] write speed factor
            uint32_t default_ecc        : 2;  ///< [30:29] write speed factor
            uint32_t wp_grp_enable      : 1;  ///< [31]    write protect group enable
            uint32_t wp_grp_size        : 7;  ///< [36:32] write protect group size
            uint32_t erase_grp_mult     : 5;  ///< [41:37] erase group size multiplier
            uint32_t erase_grp_size     : 5;  ///< [46:42] erase sector size
            uint32_t c_size_mult        : 3;  ///< [49:47] device size multiplier
            uint32_t vdd_w_curr_max     : 3;  ///< [52:50] max. write current for vdd max
            uint32_t vdd_w_curr_min     : 3;  ///< [55:53] max. write current for vdd min
            uint32_t vdd_r_curr_max     : 3;  ///< [58:56] max. read current for vdd max
            uint32_t vdd_r_curr_min     : 3;  ///< [61:59] max. read current for vdd min
            uint32_t c_size             : 12; ///< [73:62] device size
            uint32_t reserved_74        : 2;  ///< [75:74]
            uint32_t dsr_imp            : 1;  ///< [76]    dsr implemented
            uint32_t read_blk_misalign  : 1;  ///< [77]    read block misalignment
            uint32_t write_blk_misalign : 1;  ///< [78]    write block misalignment
            uint32_t read_bl_partial    : 1;  ///< [79]    partial blocks for read allowed
            uint32_t read_bl_len        : 4;  ///< [83:80] max read data block length
            uint32_t ccc                : 12; ///< [95:84] card command classes
            uint32_t tran_speed         : 8;  ///< [103:96] max. data transfer rate
            uint32_t nsac               : 8;  ///< [111:104] data read access-time-2 clk cycles
            uint32_t taac               : 8;  ///< [119:112] data read access-time-1
            uint32_t reserved_120       : 6;  ///< [121:120]
            uint32_t spec_vers          : 4;  ///< [125:122] system specification version
            uint32_t csd_structure      : 2;  ///< [127:126] csd structure
            uint32_t reserved_128       : 8;  ///< [135:128]
        } csd_v1_b;

        /** All structures in this union must be packed since some bitfields span more than one 32-bit word. */
        struct
        {
            uint32_t mfg_id           : 8;  ///< [127:120] Manufacturer ID
            uint32_t reserved_114     : 6;  ///< [119:114] Reserved
            uint32_t cbx              : 2;  ///< [113:112] Device/BGA
            uint32_t oem_id           : 8;  ///< [111:104] OEM ID
            uint64_t product_name     : 48; ///< [103:56] Product name (6 chars)
            uint32_t product_revision : 8;  ///< [55:48] Product revision
            uint32_t serial_number    : 32; ///< [47:16] Serial number
            uint32_t mfg_date         : 8;  ///< [15:8] Manufacturing date
            uint32_t crc              : 7;  ///< [7:1] CRC7 Checksum
            uint32_t reserved_0       : 1;  ///< [0:0] Not used, always "1"
        } cid;
    } card_response;

    union
    {
        uint32_t r3;

        /** OCR register */
        struct
        {
            uint32_t reserved_0           : 7;
            uint32_t reserved_lvr         : 1;
            uint32_t reserved_8           : 7;
            uint32_t v_27_28              : 1; ///< Operation VDD Voltage 2.7-2.8
            uint32_t v_28_29              : 1; ///< Operation VDD Voltage 2.8-2.9
            uint32_t v_29_30              : 1; ///< Operation VDD Voltage 2.9-3.0
            uint32_t v_30_31              : 1; ///< Operation VDD Voltage 3.0-3.1
            uint32_t v_31_32              : 1; ///< Operation VDD Voltage 3.1-3.2
            uint32_t v_32_33              : 1; ///< Operation VDD Voltage 3.2-3.3
            uint32_t v_33_34              : 1; ///< Operation VDD Voltage 3.3-3.4
            uint32_t v_34_35              : 1; ///< Operation VDD Voltage 3.4-3.5
            uint32_t v_35_36              : 1; ///< Operation VDD Voltage 3.5-3.6
            uint32_t s18A                 : 1;
            uint32_t reserved_25          : 5;
            uint32_t card_capacity_status : 1; ///< Access mode
            uint32_t power_up_status      : 1; ///< Power up status bit
        } r3_b;
    } ocr;

    union
    {
        uint32_t r4;

        struct
        {
            uint32_t ocr          : 24;
            uint32_t reserved_24  : 3;
            uint32_t memory       : 1;
            uint32_t io_functions : 3;
            uint32_t ready        : 1;
        } r4_b;
    } fast_io;

    union
    {
        uint32_t r5;
        struct
        {
            uint32_t read_write_data  : 8;
            uint32_t out_of_range     : 1;
            uint32_t invalid_function : 1;
            uint32_t rfu              : 1;
            uint32_t error            : 1;
            uint32_t current_state    : 2;
            uint32_t illegal_command  : 1;
            uint32_t crc_error        : 1;
            uint32_t reserved_16      : 16;
        } r5_b;
    } interrupt_request;
} mmc_response_t;

/** MMCA clock control data */
typedef union u_mmca_clock_control
{
    union
    {
        uint32_t clk_ctrl_u32;
        struct
        {
            uint32_t                    : 4;  ///< Reserved
            uint32_t trans_data_timeout : 4;  ///< Read/write data timeout
            uint32_t res_busy_timeout   : 4;  ///< Response busy timeout
            uint32_t res_timeout        : 2;  ///< Response timeout
            uint32_t                    : 2;  ///< Reserved
            uint32_t clock_freq         : 4;  ///< MMCA module clock frequency
            uint32_t                    : 12; ///< Reserved
        } clk_ctrl_b;
    } clk_ctrl;
    union
    {
        uint32_t boot_clk_ctrl_u32;
        struct
        {
            uint32_t              : 16; ///< Reserved
            uint32_t              : 4;  ///< Reserved
            uint32_t              : 4;  ///< Reserved
            uint32_t              : 4;  ///< Reserved
            uint32_t boot_clk_div : 4;  ///< Boot area access clock frequency divider
        } boot_clk_ctrl_b;
    } boot_clk_ctrl;
} mmca_clock_control_t;

/** Current status of the MMCA module (host). */
typedef struct st_mmca_status
{
    /** False if card was removed (only applies if MCU supports card detection and SDnCD pin is connected), true otherwise.
     *
     *  If ready is false, call @ref mmca_api_t::mediaInit to reinitialize it
     */
    bool initialized_mmc;
    bool transfer_in_progress;         ///< true = Card is busy
} mmca_status_t;

/** Information obtained from the media device. */
typedef struct st_mmca_device
{
    uint32_t sector_count;             ///< Sector count
    uint32_t sector_size_bytes;        ///< Sector size
    uint32_t erase_sector_count;       ///< Minimum erasable unit (in 512 byte sectors)
    bool     hpi_enabled;              ///< True if High Priority Interrupt (HPI) is currently enabled
    bool     hpi_supported;            ///< True if device supports HPI feature
    bool     hpi_implementation;       ///< HPI implementation type (defined by device spec)
    uint32_t correctly_prg_sectors;    ///< Number of sectors that were correctly programmed during last write
    bool     operation_interrupted;    ///< True if the last operation was interrupted
    uint8_t  interrupted_command;      ///< Command index of the last interrupted operation
    bool     bkops_enabled;            ///< True if background operations (BKOPS) are enabled
    uint8_t  bkops_status;             ///< Current background operations status reported by the device
    bool     bkops_supported;          ///< Background Operation supported
} mmca_device_t;

/** Callback function parameter data */
typedef struct st_mmca_callback_args
{
    mmc_response_t response;           ///< Response from card, only valid if MMCA_EVENT_RESPONSE is set in event.
    mmca_event_t   event;              ///< The event can be used to identify what caused the callback.
    void const   * p_context;          ///< Placeholder for user data.
} mmca_callback_args_t;

/** Configuration for MMCA channel */
typedef struct st_mmca_cfg
{
    /* MMC generic configuration */
    uint8_t                channel;                     ///< Channel of MMC host interface.
    mmca_bus_width_t       bus_width;                   ///< Device bus width is 1, 4 or 8 bits wide.
    mmca_high_speed_mode_t high_speed_mode;             ///< High speed mode selection
    mmca_clock_control_t   clock_control;               ///< MMCA clock control

    transfer_instance_t const * p_lower_lvl_transfer;   ///< DMA or DTS transfer instances

    /* Configuration for MMC Event processing */
    void (* p_callback)(mmca_callback_args_t * p_args); ///< Pointer to callback function
    void const * p_context;                             ///< User defined context passed into callback function

    transfer_instance_t const * p_transfer;             ///< To use DMA during read/write, link a DMA instance here. Set to NULL if unused.

    /* Pointer to MMC peripheral specific configuration */
    void const * p_extend;                              ///< MMC hardware dependent configuration

    /** Block size in bytes.  Block size must be 512 bytes eMMC devices, but it can be changed via mmca.*/
    uint16_t block_size;                                ///< Block size

    IRQn_Type mmc_response_irq;                         ///< MMC request IRQ number
    IRQn_Type dma_req_irq;                              ///< DMA request IRQ number

    uint8_t mmc_response_ipl;                           ///< Card (MMC device) interrupt priority
    uint8_t dma_req_ipl;                                ///< DMA request interrupt priority
} mmca_cfg_t;

/** MMC control block.  Allocate an instance specific control block to pass into the MMC API calls.
 */
typedef void mmca_ctrl_t;

/** MMC functions implemented at the HAL layer API. */
typedef struct st_mmca_api
{
    /** Open the MMC driver.
     *
     *
     * @param[in]     p_ctrl    Pointer to MMC instance control block.
     * @param[in]     p_cfg     Pointer to MMC instance configuration structure.
     */
    fsp_err_t (* open)(mmca_ctrl_t * const p_ctrl, mmca_cfg_t const * const p_cfg);

    /** Close open MMC device.
     *
     *
     * @param[in]     p_ctrl    Pointer to an open MMC instance control block.
     */
    fsp_err_t (* close)(mmca_ctrl_t * const p_ctrl);

    /** Initializes an MMC device.  If the device is a card, the card must be plugged in prior to calling this API.
     * This API blocks until the device initialization procedure is complete.
     *
     *
     * @param[in]     p_ctrl    Pointer to MMC instance control block.
     * @param[out]    p_device  Pointer to store device information.
     */
    fsp_err_t (* mediaInit)(mmca_ctrl_t * const p_ctrl, mmca_device_t * const p_device);

    /** Read data from an MMCA channel.
     *
     *
     * @param[in]     p_ctrl          Pointer to an open MMCA instance control block.
     * @param[out]    p_dest          Pointer to data buffer to read data to.
     * @param[in]     start_sector    First sector address to read.
     * @param[in]     sector_count    Number of sectors to read.  All sectors must be in the range of
     *                                mmca_device_t::sector_count.
     */
    fsp_err_t (* read)(mmca_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const start_sector,
                       uint32_t const sector_count);

    /** Write data from an MMCA channel.
     *
     *
     * @param[in]     p_ctrl          Pointer to an open MMCA instance control block.
     * @param[out]    p_dest          Pointer to data buffer to write data to.
     * @param[in]     start_sector    First sector address to read.
     * @param[in]     sector_count    Number of sectors to read. All sectors must be in the range of
     *                                mmca_device_t::sector_count.
     */
    fsp_err_t (* write)(mmca_ctrl_t * const p_ctrl, uint8_t const * const p_source, uint32_t const start_sector,
                        uint32_t const sector_count);

    /** Erase sectors. The sector size for erase is fixed at 512 bytes.
     *
     * @param[in]     p_ctrl        Pointer to an open eMMC instance control block.
     * @param[in]     start_sector  First sector to erase. Must be a multiple of mmca_device_t::erase_sector_count.
     * @param[in]     sector_count  Number of sectors to erase. Must be a multiple of mmca_device_t::erase_sector_count.
     *                              All sectors must be in the range of mmca_device_t::sector_count.
     */
    fsp_err_t (* erase)(mmca_ctrl_t * const p_ctrl, uint32_t const start_sector, uint32_t const sector_count);

    /** Provides driver status.
     *
     * @param[in]     p_ctrl        Pointer to an open eMMC instance control block.
     * @param[out]    p_status      Pointer to current driver status.
     */
    fsp_err_t (* statusGet)(mmca_ctrl_t * const p_api_ctrl, mmca_status_t * const p_status);

    /** Specify callback function and optional context pointer and working memory pointer.
     *
     * @param[in]   p_ctrl                   Control block set in @ref mmca_api_t::open call.
     * @param[in]   p_callback               Callback function to register
     * @param[in]   p_context                Pointer to send to callback function
     * @param[in]   p_working_memory         Pointer to volatile memory where callback structure can be allocated.
     *                                       Callback arguments allocated here are only valid during the callback.
     */
    fsp_err_t (* callbackSet)(mmca_ctrl_t * const p_api_ctrl, void (* p_callback)(mmca_callback_args_t * p_args),
                              void const * const p_context, mmca_callback_args_t * const p_callback_memory);

    /** Trigger HPI (High Priority Interrupt) to abort or interrupt an ongoing command sequence.
     *
     * @param[in]   p_ctrl        Pointer to an open eMMC instance control block.
     *
     */
    fsp_err_t (* hpiTrigger)(mmca_ctrl_t * const p_api_ctrl);

    /** Write multiple sectors asynchronously. The write operation is non-blocking
     *  and completion is notified through the callback function if registered.
     *
     * @param[in]   p_ctrl        Pointer to an open eMMC instance control block.
     * @param[in]   p_source      Pointer to buffer containing data to be written.
     * @param[in]   start_sector  Logical start sector address to begin the write.
     * @param[in]   sector_count  Number of sectors to write.
     *
     */
    fsp_err_t (* writeAsync)(mmca_ctrl_t * const p_api_ctrl, uint8_t const * const p_source,
                             uint32_t const start_sector, uint32_t const sector_count);

    /** Erase multiple sectors asynchronously. The erase operation is non-blocking
     *  and completion is notified through the callback function if registered.
     *
     * @param[in]   p_ctrl        Pointer to an open eMMC instance control block.
     * @param[in]   start_sector  Logical start sector address to begin the erase.
     * @param[in]   sector_count  Number of sectors to erase.
     *
     */
    fsp_err_t (* eraseAsync)(mmca_ctrl_t * const p_api_ctrl, uint32_t const start_sector, uint32_t const sector_count);

    /** Manual start Background Operation for an MMCA channel.
     *
     * @param[in]     p_api_ctrl    Pointer to MMC instance control block.
     */
    fsp_err_t (* bkopsManual)(mmca_ctrl_t * const p_api_ctrl);

    /** Manual start Background Operation for an MMCA channel.
     *
     * @param[in]     p_api_ctrl    Pointer to MMC instance control block.
     * @param[in]     enable        Enable or disable automatic background operations.
     */
    fsp_err_t (* bkopsAutoSet)(mmca_ctrl_t * const p_api_ctrl, mmca_bkops_auto_enabled_t enable);

    /** Get Background Operation Status of an MMCA channel.
     *
     * @param[in]     p_api_ctrl    Pointer to MMC instance control block.
     * @param[out]     p_status     Pointer to the variable to store the status.
     */
    fsp_err_t (* bkopsStatusGet)(mmca_ctrl_t * const p_api_ctrl, uint8_t * const p_status);

    /** Reads boot-related information from EXT_CSD.
     *
     * @param[in]     p_api_ctrl   Pointer to MMC instance control block.
     * @param[out]    p_boot_info  Pointer to boot information structure.
     */
    fsp_err_t (* bootInfo)(mmca_ctrl_t * const p_api_ctrl, mmca_boot_info_t * p_boot_info);

    /** Configures boot-related settings in EXT_CSD.
     *
     *
     * @param[in]     p_api_ctrl   Pointer to MMC instance control block.
     * @param[out]    p_boot_cfg   Pointer to boot configuration structure.
     */
    fsp_err_t (* bootConfigure)(mmca_ctrl_t * const p_api_ctrl, mmca_boot_cfg_t const * const p_boot_cfg);

    /** Begin temporary access to a specific partition
     *  in Normal mode by updating PARTITION_ACCESS (EXT_CSD[179] bits[2:0]).
     *
     * @param[in]     p_api_ctrl       Pointer to MMC instance control block.
     * @param[in]     partition_access Target PARTITION_ACCESS value (0..7).
     * @param[out]    p_guard          Pointer to guard structure to store the original BOOT_CONFIG.
     */
    fsp_err_t (* bootPartitionAccessBegin)(mmca_ctrl_t * const p_api_ctrl, uint8_t partition_access,
                                           mmca_boot_access_guard_t * const p_guard);

    /** End temporary access by restoring BOOT_CONFIG (EXT_CSD[179]) to the value saved by
     *  R_MMCA_Boot_PartitionAccessBegin().
     *
     * @param[in]     p_api_ctrl   Pointer to MMC instance control block.
     * @param[out]    p_guard      Pointer to guard containing the saved BOOT_CONFIG.
     */
    fsp_err_t (* bootPartitionAccessEnd)(mmca_ctrl_t * const p_api_ctrl, mmca_boot_access_guard_t * const p_guard);
} mmca_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_mmca_instance
{
    mmca_ctrl_t      * p_ctrl;         ///< Pointer to the control structure for this instance
    mmca_cfg_t const * p_cfg;          ///< Pointer to the configuration structure for this instance
    mmca_api_t const * p_api;          ///< Pointer to the API structure for this instance
} mmca_instance_t;

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************//**
 * @} (end defgroup MMCA_API)
 **********************************************************************************************************************/
