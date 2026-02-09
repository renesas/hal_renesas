/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2BX
 * @defgroup BSP_OVERRIDE_U2BX BSP Type Override Definitions
 *
 * @{
 **********************************************************************************************************************/

#ifndef BSP_OVERRIDE_H
#define BSP_OVERRIDE_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Overrides for UART */
#define BSP_OVERRIDE_UART_DATA_BITS_T
#define BSP_OVERRIDE_UART_EVENT_T
#define BSP_OVERRIDE_UART_PARITY_T

/* Override for MSPI */
#define BSP_OVERRIDE_SPI_BIT_WIDTH_T

/* Override for Timer */
#define BSP_OVERRIDE_TIMER_EVENT_T           (1U)
#define BSP_OVERRIDE_CALLBACK_PARAMETER_T    (1U)

/* Override for WDT */
#define BSP_OVERRIDE_WDT_TIMEOUT_T           (1U)
#define BSP_OVERRIDE_WDT_CLOCK_DIVISION_T    (1U)

/* Override for Transfer Module */
#define BSP_OVERRIDE_TRANSFER_MODE_T
#define BSP_OVERRIDE_TRANSFER_REQUEST_T
#define BSP_OVERRIDE_TRANSFER_SIZE_T
#define BSP_OVERRIDE_TRANSFER_ADDR_MODE_T
#define BSP_OVERRIDE_TRANSFER_CHAIN_MODE_T
#define BSP_OVERRIDE_TRANSFER_RELOAD_FUNCTION_1_T
#define BSP_OVERRIDE_TRANSFER_RELOAD_FUNCTION_2_T
#define BSP_OVERRIDE_TRANSFER_IRQ_T
#define BSP_OVERRIDE_TRANSFER_CALLBACK_ARGS_T
#define BSP_OVERRIDE_TRANSFER_INFO_T

/* Override for ICU */
#define BSP_OVERRIDE_EXTERNAL_IRQ_TRIGGER_T

/* Overrides for ADCK */
#define BSP_OVERRIDE_ADC_INCLUDE
#define BSP_OVERRIDE_ADC_EVENT_T
#define BSP_OVERRIDE_ADC_CHANNEL_T
#define BSP_OVERRIDE_ADC_INFO_T
#define BSP_OVERRIDE_ADC_NUMBER_OF_SG    (5U)

/* Override for SFMA */
#define BSP_OVERRIDE_SPI_FLASH_ADDRESS_BYTE_T

/* Override for ETHER */
#define BSP_OVERRIDE_ETHER_EVENT_T

/* Override for ETHER_PHY */
#define BSP_OVERRIDE_ETHER_PHY_LSI_TYPE_T

/* Override for CRC */
#define BSP_OVERRIDE_CRC_BIT_ORDER_T
#define BSP_OVERRIDE_CRC_64_BIT_POLY_T
#define BSP_OVERRIDE_CRC_RESULT_T

/* Override for MOTOR_SPEED */
#define BSP_OVERRIDE_MOTOR_SPEED_EVENT_T

/* Override for IOPORT */
#define BSP_OVERRIDE_IOPORT_FILTER_SIGNAL_T

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Transfer request type selection when using DMAC */
typedef enum e_transfer_request
{
    TRANSFER_REQUEST_AUTO     = 0,     ///< Transfer is triggered by software
    TRANSFER_REQUEST_HARDWARE = 1      ///< Transfer is triggered by hardware request
} transfer_request_t;

/** Transfer mode describes what will happen when a transfer request occurs. */
typedef enum e_transfer_mode
{
    /** In Normal speed mode, there is no configurable delay between consecutive DMA transfers.
     *  The transfer count is decremented and the source and address pointers are
     *  updated according to transfer_addr_mode_t.  After the transfer length reaches 0, transfer requests
     *  will not cause any further transfers. */
    TRANSFER_MODE_NORMAL = 0,                   // SLM = 0000: No delay (normal mode)

    /** In slow speed mode, there is a configurable delay between consecutive DMA transfers.
     * When one DMA channel operating in slow speed mode is waiting between DMA transfers,
     * another DMA channel operating in slow speed mode can perform DMA transfer.
     * Delay_time (seconds) = SLM_value × (1 / DMA_clock_frequency)
     */
    TRANSFER_MODE_SLOW_256_CLOCK_CYCLE   = 0x8, // SLM = 1000
    TRANSFER_MODE_SLOW_512_CLOCK_CYCLE   = 0x9, // SLM = 1001
    TRANSFER_MODE_SLOW_1024_CLOCK_CYCLE  = 0xA, // SLM = 1010
    TRANSFER_MODE_SLOW_2048_CLOCK_CYCLE  = 0xB, // SLM = 1011
    TRANSFER_MODE_SLOW_4096_CLOCK_CYCLE  = 0xC, // SLM = 1100
    TRANSFER_MODE_SLOW_8192_CLOCK_CYCLE  = 0xD, // SLM = 1101
    TRANSFER_MODE_SLOW_16384_CLOCK_CYCLE = 0xE, // SLM = 1110
    TRANSFER_MODE_SLOW_32768_CLOCK_CYCLE = 0xF  // SLM = 1111
} transfer_mode_t;

/** Transfer DTSC mode describes what will happen when a DTSC request occurs. */
typedef enum e_transfer_dtsc_mode
{
    TRANSFER_DTSC_MODE_SINGLE  = 0,    ///< Single transfer mode
    TRANSFER_DTSC_MODE_BLOCK_1 = 1,    ///< Block transfer mode 1
    TRANSFER_DTSC_MODE_BLOCK_2 = 2     ///< Block transfer mode 2
} transfer_dtsc_mode_t;

/** Interrupt options. */
typedef enum e_transfer_irq
{
    /** Interrupt is disable */
    TRANSFER_IRQ_DISABLE = 0,

    /** The transfer completion interrupt is set when the last transfer is complete */
    TRANSFER_IRQ_ENABLE = 1
} transfer_irq_t;

/** Address mode specifies whether to modify (increment or fixed) pointer after each transfer. */
typedef enum e_transfer_addr_mode
{
    /** Address pointer remains fixed after each transfer. */
    TRANSFER_ADDR_MODE_FIXED = 0,

    /** Address pointer is incremented by associated transfer_size_t after each transfer. */
    TRANSFER_ADDR_MODE_INCREMENTED = 1,
} transfer_addr_mode_t;

/** Address mode specifies whether to modify (increment, decrement or fixed) pointer after each transfer. */
typedef enum e_transfer_dtsc_addr_mode
{
    /** Address pointer is incremented by associated transfer_size_t after each transfer. */
    TRANSFER_DTSC_ADDR_MODE_INCREMENTED = 0,

    /** Address pointer is decremented by associated transfer_size_t after each transfer. */
    TRANSFER_DTSC_ADDR_MODE_DECREMENTED = 1,

    /** Address pointer remains fixed after each transfer. */
    TRANSFER_DTSC_ADDR_MODE_FIXED = 2,
} transfer_dtsc_addr_mode_t;

/** Chain transfer mode options */
typedef enum e_transfer_chain_mode
{
    /** Chain mode not used. */
    TRANSFER_CHAIN_MODE_DISABLED = 0,

    /** Switch to next transfer after a single transfer from this transfer_info_t. */
    TRANSFER_CHAIN_MODE_ALWAYS = 3,

    /** Complete the entire transfer defined in this transfer_info_t before chaining to next transfer. */
    TRANSFER_CHAIN_MODE_LAST_TRANSFER = 1
} transfer_chain_mode_t;

/** Restrict chain function*/
typedef enum e_transfer_chain_restriction
{
    TRANSFER_CHAIN_RESTRICTION_ENABLE  = 0, ///< restrict chain function
    TRANSFER_CHAIN_RESTRICTION_DISABLE = 1, ///< do not restrict chain function
} transfer_chain_restriction_t;

/** Chain Restriction Config*/
typedef struct st_transfer_chain_restrict_cfg
{
    transfer_chain_restriction_t chain_restriction_mode;
    uint8_t master_spid;
    uint8_t chain_spid;
    uint8_t master_um;
    uint8_t chain_um;
} transfer_chain_restrict_cfg_t;

/** Transfer event */
typedef enum e_transfer_event_t
{
    TRANSFER_EVENT_CHANNEL_BUSY_FLAG     = 0x01 << 0,  ///< Channel status: idle or busy.
    TRANSFER_EVENT_TRANS_END_FLAG        = 0x01 << 1,  ///< Hardware/Software DMA/DTSC transfer status: in progress, transfer ended.
    TRANSFER_EVENT_DESCRIPTOR_FLAG       = 0x01 << 2,  ///< DMA transfer status and shows whether the descriptor step has finished.
    TRANSFER_EVENT_ADDR_ERR_FLAG         = 0x01 << 3,  ///< Indicates that an address error interrupt occurred in the DMA transfer.
    TRANSFER_EVENT_TRANS_COMPLETION      = 0x01 << 4,  ///< Completion of the last transfer and shows whether the DMA transfer is complete.
    TRANSFER_EVENT_TRANS_COUNT_MATCH     = 0x01 << 5,  ///< Remaining transfer count is the same as the value set in transfer compare register.
    TRANSFER_EVENT_ERR_TRANS_READ_CYCLE  = 0x01 << 7,  ///< Transfer error occurs at read cycle.
    TRANSFER_EVENT_ERR_TRANS_WRITE_CYCLE = 0x01 << 11, ///< Transfer error occurs at write cycle.
    TRANSFER_EVENT_HW_REQ_STATUS         = 0x01 << 12, ///< DMA Hardware transfer request status.
    TRANSFER_EVENT_HW_REQ_OVERFLOW       = 0x01 << 13, ///< DMA Hardware transfer request overflow flag.
} transfer_event_t;

/** Callback function parameter data. */
typedef struct st_transfer_callback_args
{
    transfer_event_t event;            ///< Event code
    void const     * p_context;        ///< Placeholder for user data.  Set in @ref transfer_api_t::open function in ::transfer_cfg_t.
} transfer_callback_args_t;

/** Transfer size specifies the size of each individual transfer.
 *  Total transfer length = transfer_size_t * transfer_length_t
 */
typedef enum e_transfer_size
{
    TRANSFER_SIZE_1_BYTE  = 0,         ///< Each transfer transfers a 8-bit value
    TRANSFER_SIZE_2_BYTE  = 1,         ///< Each transfer transfers a 16-bit value
    TRANSFER_SIZE_4_BYTE  = 2,         ///< Each transfer transfers a 32-bit value
    TRANSFER_SIZE_8_BYTE  = 3,         ///< Each transfer transfers a 64-bit value
    TRANSFER_SIZE_16_BYTE = 4,         ///< Each transfer transfers a 128-bit value
    TRANSFER_SIZE_32_BYTE = 5,         ///< Each transfer transfers a 256-bit value
    TRANSFER_SIZE_64_BYTE = 6          ///< Each transfer transfers a 512-bit value
} transfer_size_t;

/** Redundant function */
typedef enum e_transfer_switchable_redundant
{
    TRANSFER_SWITCHABLE_REDUNDANT_OPERATION_ENABLE  = 0, ///< Switchable redundant function
    TRANSFER_SWITCHABLE_REDUNDANT_OPERATION_DISABLE = 1, ///< do not Switchable redundant function
} transfer_switchable_redundant_t;

/** Reload function 1 enables the transfer count (number of transfer) reloads at the end of DTS cycle,
 * and source/destination address reload is also attached in reload function 2 settings */
typedef enum e_transfer_reload_function_1
{
    TRANSFER_RELOAD_FUNCTION_1_DISABLE          = 0, ///< Reload function_1 disabled
    TRANSFER_RELOAD_FUNCTION_1_SRC_ADDR_RELOAD  = 1, ///< Reload source address
    TRANSFER_RELOAD_FUNCTION_1_DEST_ADDR_RELOAD = 2, ///< Reload destination address
    TRANSFER_RELOAD_FUNCTION_1_ALL_ADDR_RELOAD  = 3  ///< Reload both source and destination addresses
} transfer_reload_function_1_t;

/** Reload function 2 enables the address reload count reloads at the end of DTS cycle, and source/destination address
 * reload is also attached in reload function 2 settings */
typedef enum e_transfer_reload_function_2
{
    TRANSFER_RELOAD_FUNCTION_2_DISABLE          = 0, ///< Reload function_2 disabled
    TRANSFER_RELOAD_FUNCTION_2_SRC_ADDR_RELOAD  = 1, ///< Reload source address
    TRANSFER_RELOAD_FUNCTION_2_DEST_ADDR_RELOAD = 2, ///< Reload destination address
    TRANSFER_RELOAD_FUNCTION_2_ALL_ADDR_RELOAD  = 3  ///< Reload both source and destination addresses
} transfer_reload_function_2_t;

/** This structure specifies the properties of the transfer.
 *  @warning  When using DTC, this structure corresponds to the descriptor block registers required by the DTC.
 *            The following components may be modified by the driver: p_src, p_dest, num_blocks, and length.
 *  @warning  When using DTC, do NOT reuse this structure to configure multiple transfers.  Each transfer must
 *            have a unique transfer_info_t.
 *  @warning  When using DTC, this structure must not be allocated in a temporary location.  Any instance of this
 *            structure must remain in scope until the transfer it is used for is closed.
 *  @note     When using DTC, consider placing instances of this structure in a protected section of memory. */
typedef struct st_transfer_info
{
    void const * volatile p_src;       ///< Source pointer

    void * volatile p_dest;            ///< Destination pointer

    /** Select transfer data size of each transfer */
    uint32_t number_transfer;

    /** Transfer Count Match Register settings, only support DTSC */
    uint16_t number_transfer_count_match;

    /** Set the transfer limit for each hardware-triggered DMA request */
    uint8_t transfer_limit_per_hw_request;

    /** Set the Transfer count per hardware request */
    uint16_t transfer_count_per_hw_request;

    /** Enable/Disable Pre-load function */
    uint16_t transfer_pre_load;

    /** Enable/Disable First pre-load trigger */
    uint16_t transfer_first_pre_load;

    /** Specify the number of transfers until the address reload when using TRANSFER_MODE_BLOCK_2 */
    uint16_t addr_reload_count;

    /** Transfer Count Register settings transfer_reload_function_1_t */
    uint16_t reload_number_transfer;

    /** Specify the number of transfers until the address reload when using transfer_reload_function_2_t */
    uint16_t reload_addr_reload_count;

    /* Transfer Mode Register settings*/
    union
    {
        struct
        {
            uint32_t : 13;

            /** Configure DMA transfer speed: Normal or Specific Clock mode. */
            transfer_mode_t transfer_mode : 5;

            /** Select transfer request type (Auto Software/Hardware). */
            transfer_request_t transfer_request_type : 1;

            /** Transfer completion interrupt enable  */
            uint32_t transfer_completion_irq : 1;

            /** Transfer descriptor interrupt enable  */
            uint32_t transfer_descriptor_irq : 1;

            /** Channel address error interrupt enable  */
            uint32_t channel_address_error_irq : 1;

            /** Destination address mode */
            transfer_addr_mode_t des_addr_mode : 1;

            /** Source address mode */
            transfer_addr_mode_t src_addr_mode : 1;

            /** DMA destination transaction size */
            transfer_size_t des_trans_size : 4;

            /** DMA source transaction size */
            transfer_size_t src_trans_size : 4;
        } transfer_mode_b;

        uint32_t transfer_mode;        ///< Transfer mode register
    };

    /* Transfer Control Register settings*/
    union
    {
        struct
        {
            /** Select DTSC mode  */
            transfer_dtsc_mode_t transfer_mode : 2;

            /** Select transfer data size of each transfer */
            transfer_size_t transfer_data_size : 3;

            /** Source address count direction */
            transfer_dtsc_addr_mode_t src_addr_direction : 2;

            /** Destination address count direction */
            transfer_dtsc_addr_mode_t des_addr_direction : 2;

            /** Reload function 1 Setting  */
            transfer_reload_function_1_t reload_func_1 : 2;

            /** Reload function 2 Setting  */
            transfer_reload_function_2_t reload_func_2 : 2;

            /** Select whether to enable continuous transfer */
            uint32_t continuous_transfer : 1;

            /** Transfer completion interrupt enable  */
            uint32_t transfer_completion_irq : 1;

            /** Transfer count match interrupt enable, only support DTSC */
            uint32_t transfer_count_match_irq : 1;

            /** Select the chain function */
            transfer_chain_mode_t chain_function : 2;

            /** Next channel to chain. NOTE: the next channel should be another channel in the same DMAC unit */
            uint32_t next_channel_to_chain : 3;

            uint32_t : 5;

            /** Select transfer request type (Software/Hardware). */
            transfer_request_t transfer_request_type : 1;

            /** If this bit is set to 1, the following DMA cycles are not executed when the PDMAjDCSTn.ER bit is set
             *  due to DMA transfer error */
            uint32_t transfer_disable_in_error : 1;

            uint32_t : 4;
        } transfer_control_b;

        uint32_t transfer_control;     ///< Transfer control register
    };
    void * volatile p_reload_src_addr; ///< Source address for reload
    void * volatile p_reload_des_addr; ///< Destination address for reload
} transfer_info_t;

/** Reliability Function */
typedef struct st_transfer_switchable_redundant_cfg
{
    transfer_switchable_redundant_t redundant_operation;
    uint8_t master_spid;
    uint8_t master_um;
} transfer_switchable_redundant_cfg_t;

/** WDT time-out periods. */
typedef enum e_wdt_timeout
{
    WDT_TIMEOUT_128 = 0,               ///< 128 clock cycles
    WDT_TIMEOUT_512,                   ///< 512 clock cycles
    WDT_TIMEOUT_1024,                  ///< 1024 clock cycles
    WDT_TIMEOUT_2048,                  ///< 2048 clock cycles
    WDT_TIMEOUT_4096,                  ///< 4096 clock cycles
    WDT_TIMEOUT_8192,                  ///< 8192 clock cycles
    WDT_TIMEOUT_16384,                 ///< 16384 clock cycles
} wdt_timeout_t;

/** WDT clock division ratio. */
typedef enum e_wdt_clock_division
{
    WDT_CLOCK_DIVISION_1    = 0,       ///< CLK/1
    WDT_CLOCK_DIVISION_4    = 1,       ///< CLK/4
    WDT_CLOCK_DIVISION_16   = 2,       ///< CLK/16
    WDT_CLOCK_DIVISION_32   = 3,       ///< CLK/32
    WDT_CLOCK_DIVISION_64   = 4,       ///< CLK/64
    WDT_CLOCK_DIVISION_128  = 15,      ///< CLK/128
    WDT_CLOCK_DIVISION_256  = 5,       ///< CLK/256
    WDT_CLOCK_DIVISION_512  = 6,       ///< CLK/512
    WDT_CLOCK_DIVISION_2048 = 7,       ///< CLK/2048
    WDT_CLOCK_DIVISION_8192 = 8,       ///< CLK/8192
} wdt_clock_division_t;

/***********************************************************************************************************************
 *                          MSPI
 **********************************************************************************************************************/

/** MSPI data bit width */
typedef enum e_spi_bit_width
{
    SPI_BIT_WIDTH_2_BITS   = (0x2),    ///< Selects the data length is 2 bits
    SPI_BIT_WIDTH_3_BITS   = (0x3),    ///< Selects the data length is 3 bits
    SPI_BIT_WIDTH_4_BITS   = (0x4),    ///< Selects the data length is 4 bits
    SPI_BIT_WIDTH_5_BITS   = (0x5),    ///< Selects the data length is 5 bits
    SPI_BIT_WIDTH_6_BITS   = (0x6),    ///< Selects the data length is 6 bits
    SPI_BIT_WIDTH_7_BITS   = (0x7),    ///< Selects the data length is 7 bits
    SPI_BIT_WIDTH_8_BITS   = (0x8),    ///< Selects the data length is 8 bits
    SPI_BIT_WIDTH_9_BITS   = (0x9),    ///< Selects the data length is 9 bits
    SPI_BIT_WIDTH_10_BITS  = (0xA),    ///< Selects the data length is 10 bits
    SPI_BIT_WIDTH_11_BITS  = (0xB),    ///< Selects the data length is 11 bits
    SPI_BIT_WIDTH_12_BITS  = (0xC),    ///< Selects the data length is 12 bits
    SPI_BIT_WIDTH_13_BITS  = (0xD),    ///< Selects the data length is 13 bits
    SPI_BIT_WIDTH_14_BITS  = (0xE),    ///< Selects the data length is 14 bits
    SPI_BIT_WIDTH_15_BITS  = (0xF),    ///< Selects the data length is 15 bits
    SPI_BIT_WIDTH_16_BITS  = (0x10),   ///< Selects the data length is 16 bits
    SPI_BIT_WIDTH_17_BITS  = (0x11),   ///< Selects the data length is 17 bits
    SPI_BIT_WIDTH_18_BITS  = (0x12),   ///< Selects the data length is 18 bits
    SPI_BIT_WIDTH_19_BITS  = (0x13),   ///< Selects the data length is 19 bits
    SPI_BIT_WIDTH_20_BITS  = (0x14),   ///< Selects the data length is 20 bits
    SPI_BIT_WIDTH_21_BITS  = (0x15),   ///< Selects the data length is 21 bits
    SPI_BIT_WIDTH_22_BITS  = (0x16),   ///< Selects the data length is 22 bits
    SPI_BIT_WIDTH_23_BITS  = (0x17),   ///< Selects the data length is 23 bits
    SPI_BIT_WIDTH_24_BITS  = (0x18),   ///< Selects the data length is 24 bits
    SPI_BIT_WIDTH_25_BITS  = (0x19),   ///< Selects the data length is 25 bits
    SPI_BIT_WIDTH_26_BITS  = (0x1A),   ///< Selects the data length is 26 bits
    SPI_BIT_WIDTH_27_BITS  = (0x1B),   ///< Selects the data length is 27 bits
    SPI_BIT_WIDTH_28_BITS  = (0x1C),   ///< Selects the data length is 28 bits
    SPI_BIT_WIDTH_29_BITS  = (0x1D),   ///< Selects the data length is 29 bits
    SPI_BIT_WIDTH_30_BITS  = (0x1E),   ///< Selects the data length is 30 bits
    SPI_BIT_WIDTH_31_BITS  = (0x1F),   ///< Selects the data length is 31 bits
    SPI_BIT_WIDTH_32_BITS  = (0x20),   ///< Selects the data length is 32 bits
    SPI_BIT_WIDTH_33_BITS  = (0x21),   ///< Selects the data length is 33 bits
    SPI_BIT_WIDTH_34_BITS  = (0x22),   ///< Selects the data length is 34 bits
    SPI_BIT_WIDTH_35_BITS  = (0x23),   ///< Selects the data length is 35 bits
    SPI_BIT_WIDTH_36_BITS  = (0x24),   ///< Selects the data length is 36 bits
    SPI_BIT_WIDTH_37_BITS  = (0x25),   ///< Selects the data length is 37 bits
    SPI_BIT_WIDTH_38_BITS  = (0x26),   ///< Selects the data length is 38 bits
    SPI_BIT_WIDTH_39_BITS  = (0x27),   ///< Selects the data length is 39 bits
    SPI_BIT_WIDTH_40_BITS  = (0x28),   ///< Selects the data length is 40 bits
    SPI_BIT_WIDTH_41_BITS  = (0x29),   ///< Selects the data length is 41 bits
    SPI_BIT_WIDTH_42_BITS  = (0x2A),   ///< Selects the data length is 42 bits
    SPI_BIT_WIDTH_43_BITS  = (0x2B),   ///< Selects the data length is 43 bits
    SPI_BIT_WIDTH_44_BITS  = (0x2C),   ///< Selects the data length is 44 bits
    SPI_BIT_WIDTH_45_BITS  = (0x2D),   ///< Selects the data length is 45 bits
    SPI_BIT_WIDTH_46_BITS  = (0x2E),   ///< Selects the data length is 46 bits
    SPI_BIT_WIDTH_47_BITS  = (0x2F),   ///< Selects the data length is 47 bits
    SPI_BIT_WIDTH_48_BITS  = (0x30),   ///< Selects the data length is 48 bits
    SPI_BIT_WIDTH_49_BITS  = (0x31),   ///< Selects the data length is 49 bits
    SPI_BIT_WIDTH_50_BITS  = (0x32),   ///< Selects the data length is 50 bits
    SPI_BIT_WIDTH_51_BITS  = (0x33),   ///< Selects the data length is 51 bits
    SPI_BIT_WIDTH_52_BITS  = (0x34),   ///< Selects the data length is 52 bits
    SPI_BIT_WIDTH_53_BITS  = (0x35),   ///< Selects the data length is 53 bits
    SPI_BIT_WIDTH_54_BITS  = (0x36),   ///< Selects the data length is 54 bits
    SPI_BIT_WIDTH_55_BITS  = (0x37),   ///< Selects the data length is 55 bits
    SPI_BIT_WIDTH_56_BITS  = (0x38),   ///< Selects the data length is 56 bits
    SPI_BIT_WIDTH_57_BITS  = (0x39),   ///< Selects the data length is 57 bits
    SPI_BIT_WIDTH_58_BITS  = (0x3A),   ///< Selects the data length is 58 bits
    SPI_BIT_WIDTH_59_BITS  = (0x3B),   ///< Selects the data length is 59 bits
    SPI_BIT_WIDTH_60_BITS  = (0x3C),   ///< Selects the data length is 60 bits
    SPI_BIT_WIDTH_61_BITS  = (0x3D),   ///< Selects the data length is 61 bits
    SPI_BIT_WIDTH_62_BITS  = (0x3E),   ///< Selects the data length is 62 bits
    SPI_BIT_WIDTH_63_BITS  = (0x3F),   ///< Selects the data length is 63 bits
    SPI_BIT_WIDTH_64_BITS  = (0x40),   ///< Selects the data length is 64 bits
    SPI_BIT_WIDTH_65_BITS  = (0x41),   ///< Selects the data length is 65 bits
    SPI_BIT_WIDTH_66_BITS  = (0x42),   ///< Selects the data length is 66 bits
    SPI_BIT_WIDTH_67_BITS  = (0x43),   ///< Selects the data length is 67 bits
    SPI_BIT_WIDTH_68_BITS  = (0x44),   ///< Selects the data length is 68 bits
    SPI_BIT_WIDTH_69_BITS  = (0x45),   ///< Selects the data length is 69 bits
    SPI_BIT_WIDTH_70_BITS  = (0x46),   ///< Selects the data length is 70 bits
    SPI_BIT_WIDTH_71_BITS  = (0x47),   ///< Selects the data length is 71 bits
    SPI_BIT_WIDTH_72_BITS  = (0x48),   ///< Selects the data length is 72 bits
    SPI_BIT_WIDTH_73_BITS  = (0x49),   ///< Selects the data length is 73 bits
    SPI_BIT_WIDTH_74_BITS  = (0x4A),   ///< Selects the data length is 74 bits
    SPI_BIT_WIDTH_75_BITS  = (0x4B),   ///< Selects the data length is 75 bits
    SPI_BIT_WIDTH_76_BITS  = (0x4C),   ///< Selects the data length is 76 bits
    SPI_BIT_WIDTH_77_BITS  = (0x4D),   ///< Selects the data length is 77 bits
    SPI_BIT_WIDTH_78_BITS  = (0x4E),   ///< Selects the data length is 78 bits
    SPI_BIT_WIDTH_79_BITS  = (0x4F),   ///< Selects the data length is 79 bits
    SPI_BIT_WIDTH_80_BITS  = (0x50),   ///< Selects the data length is 80 bits
    SPI_BIT_WIDTH_81_BITS  = (0x51),   ///< Selects the data length is 81 bits
    SPI_BIT_WIDTH_82_BITS  = (0x52),   ///< Selects the data length is 82 bits
    SPI_BIT_WIDTH_83_BITS  = (0x53),   ///< Selects the data length is 83 bits
    SPI_BIT_WIDTH_84_BITS  = (0x54),   ///< Selects the data length is 84 bits
    SPI_BIT_WIDTH_85_BITS  = (0x55),   ///< Selects the data length is 85 bits
    SPI_BIT_WIDTH_86_BITS  = (0x56),   ///< Selects the data length is 86 bits
    SPI_BIT_WIDTH_87_BITS  = (0x57),   ///< Selects the data length is 87 bits
    SPI_BIT_WIDTH_88_BITS  = (0x58),   ///< Selects the data length is 88 bits
    SPI_BIT_WIDTH_89_BITS  = (0x59),   ///< Selects the data length is 89 bits
    SPI_BIT_WIDTH_90_BITS  = (0x5A),   ///< Selects the data length is 90 bits
    SPI_BIT_WIDTH_91_BITS  = (0x5B),   ///< Selects the data length is 91 bits
    SPI_BIT_WIDTH_92_BITS  = (0x5C),   ///< Selects the data length is 92 bits
    SPI_BIT_WIDTH_93_BITS  = (0x5D),   ///< Selects the data length is 93 bits
    SPI_BIT_WIDTH_94_BITS  = (0x5E),   ///< Selects the data length is 94 bits
    SPI_BIT_WIDTH_95_BITS  = (0x5F),   ///< Selects the data length is 95 bits
    SPI_BIT_WIDTH_96_BITS  = (0x60),   ///< Selects the data length is 96 bits
    SPI_BIT_WIDTH_97_BITS  = (0x61),   ///< Selects the data length is 97 bits
    SPI_BIT_WIDTH_98_BITS  = (0x62),   ///< Selects the data length is 98 bits
    SPI_BIT_WIDTH_99_BITS  = (0x63),   ///< Selects the data length is 99 bits
    SPI_BIT_WIDTH_100_BITS = (0x64),   ///< Selects the data length is 100 bits
    SPI_BIT_WIDTH_101_BITS = (0x65),   ///< Selects the data length is 101 bits
    SPI_BIT_WIDTH_102_BITS = (0x66),   ///< Selects the data length is 102 bits
    SPI_BIT_WIDTH_103_BITS = (0x67),   ///< Selects the data length is 103 bits
    SPI_BIT_WIDTH_104_BITS = (0x68),   ///< Selects the data length is 104 bits
    SPI_BIT_WIDTH_105_BITS = (0x69),   ///< Selects the data length is 105 bits
    SPI_BIT_WIDTH_106_BITS = (0x6A),   ///< Selects the data length is 106 bits
    SPI_BIT_WIDTH_107_BITS = (0x6B),   ///< Selects the data length is 107 bits
    SPI_BIT_WIDTH_108_BITS = (0x6C),   ///< Selects the data length is 108 bits
    SPI_BIT_WIDTH_109_BITS = (0x6D),   ///< Selects the data length is 109 bits
    SPI_BIT_WIDTH_110_BITS = (0x6E),   ///< Selects the data length is 110 bits
    SPI_BIT_WIDTH_111_BITS = (0x6F),   ///< Selects the data length is 111 bits
    SPI_BIT_WIDTH_112_BITS = (0x70),   ///< Selects the data length is 112 bits
    SPI_BIT_WIDTH_113_BITS = (0x71),   ///< Selects the data length is 113 bits
    SPI_BIT_WIDTH_114_BITS = (0x72),   ///< Selects the data length is 114 bits
    SPI_BIT_WIDTH_115_BITS = (0x73),   ///< Selects the data length is 115 bits
    SPI_BIT_WIDTH_116_BITS = (0x74),   ///< Selects the data length is 116 bits
    SPI_BIT_WIDTH_117_BITS = (0x75),   ///< Selects the data length is 117 bits
    SPI_BIT_WIDTH_118_BITS = (0x76),   ///< Selects the data length is 118 bits
    SPI_BIT_WIDTH_119_BITS = (0x77),   ///< Selects the data length is 119 bits
    SPI_BIT_WIDTH_120_BITS = (0x78),   ///< Selects the data length is 120 bits
    SPI_BIT_WIDTH_121_BITS = (0x79),   ///< Selects the data length is 121 bits
    SPI_BIT_WIDTH_122_BITS = (0x7A),   ///< Selects the data length is 122 bits
    SPI_BIT_WIDTH_123_BITS = (0x7B),   ///< Selects the data length is 123 bits
    SPI_BIT_WIDTH_124_BITS = (0x7C),   ///< Selects the data length is 124 bits
    SPI_BIT_WIDTH_125_BITS = (0x7D),   ///< Selects the data length is 125 bits
    SPI_BIT_WIDTH_126_BITS = (0x7E),   ///< Selects the data length is 126 bits
    SPI_BIT_WIDTH_127_BITS = (0x7F),   ///< Selects the data length is 127 bits
    SPI_BIT_WIDTH_128_BITS = (0x80)    ///< Selects the data length is 128 bits
} spi_bit_width_t;

/** Events that can trigger a callback function */
typedef enum e_timer_event
{
    TIMER_EVENT_CYCLE_END,             ///< Requested timer delay has expired or timer has wrapped around
    TIMER_EVENT_INPUT_CAPTURE,         ///< A capture has occurred on the input signal pin.
    TIMER_EVENT_COMPARE_MATCH_0,       ///< A Period/Compare Match 0 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_1,       ///< A Compare Match 1 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_2,       ///< A Compare Match 2 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_3,       ///< A Compare Match 3 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_4,       ///< A Compare Match 4 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_5,       ///< A Compare Match 5 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_6,       ///< A Compare Match 6 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_7,       ///< A Compare Match 7 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_8,       ///< A Compare Match 8 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_9,       ///< A Compare Match 9 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_10,      ///< A Compare Match 10 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_11,      ///< A Compare Match 11 interrupt event occurs
    TIMER_EVENT_COMPARE_MATCH_12,      ///< A Compare Match 12 interrupt event occurs
    TIMER_EVENT_PEAK_TIMING,           ///< A Peak interrupt event occurs
    TIMER_EVENT_VALLEY_TIMING,         ///< A Valley interrupt event occurs
    TIMER_EVENT_ERROR_DETECTION,       ///< A Error interrupt event occurs
    TIMER_EVENT_WARNING_DETECTION,     ///< A Warning interrupt event occurs
    TIMER_EVENT_OUTPUT_COMPARE_0,      ///< An output has occurred on signal 0
    TIMER_EVENT_OUTPUT_COMPARE_1,      ///< An output has occurred on signal 1
    TIMER_EVENT_DEAD_TIME,             ///< Dead time event
    TIMER_EVENT_CAPTURE_U,             ///< A capture has occurred on signal U
    TIMER_EVENT_CAPTURE_V,             ///< A capture has occurred on signal V
    TIMER_EVENT_CAPTURE_W,             ///< A capture has occurred on signal W
    TIMER_EVENT_PEAK_INTERRUPT,        ///< Peak interrupt request, apply only for TAPA
    TIMER_EVENT_VALLEY_INTERRUPT       ///< Valley interrupt request, apply only for TAPA
} timer_event_t;

/** Callback function parameter data */
typedef struct st_timer_callback_args
{
    /** Placeholder for user data.  Set in @ref timer_api_t::open function in @ref timer_cfg_t. */
    void const  * p_context;
    timer_event_t event;               ///< The event can be used to identify what caused the callback.

    /** Most recent capture, only valid if event is TIMER_EVENT_INPUT_CAPTURE. */
    uint64_t capture;
} timer_callback_args_t;

/** Condition that will trigger an interrupt when detected. */
typedef enum e_external_irq_trigger
{
    EXTERNAL_IRQ_TRIGGER_DISABLE    = 0, ///< Disable trigger
    EXTERNAL_IRQ_TRIGGER_RISING     = 1, ///< Rising edge trigger
    EXTERNAL_IRQ_TRIGGER_FALLING    = 2, ///< Falling edge trigger
    EXTERNAL_IRQ_TRIGGER_BOTH_EDGE  = 3, ///< Both edges trigger
    EXTERNAL_IRQ_TRIGGER_LEVEL_LOW  = 4, ///< Low level trigger
    EXTERNAL_IRQ_TRIGGER_LEVEL_HIGH = 5  ///< High level trigger
} external_irq_trigger_t;

/***********************************************************************************************************************
 *                          UART
 **********************************************************************************************************************/

/** UART Event codes */
typedef enum e_uart_event
{
    UART_EVENT_ERR_BIT     = 0x01 << 0, ///< Bit error event
    UART_EVENT_ERR_OVERRUN = 0x01 << 2, ///< Overrun error event
    UART_EVENT_ERR_FRAMING = 0x01 << 3, ///< Frame error event
    UART_EVENT_ERR_EXPAND  = 0x01 << 4, ///< Expansion bit error event
    UART_EVENT_ERR_ID      = 0x01 << 5, ///< ID match error event
    UART_EVENT_ERR_PARITY  = 0x01 << 6, ///< Parity error event
    UART_EVENT_TX_COMPLETE = 0x00,      ///< Transmit complete event
    UART_EVENT_RX_CHAR     = 0xFE,      ///< Character received
    UART_EVENT_RX_COMPLETE = 0xFF,      ///< Receive complete event
    UART_EVENT_ID_MATCH    = 0x30       ///< Receive data matched the ID value
} uart_event_t;

/** UART Data bit length definition */
typedef enum e_uart_data_bits
{
    UART_DATA_BITS_9 = 2U,             ///< Data bits 9-bits
    UART_DATA_BITS_8 = 0U,             ///< Data bits 8-bits
    UART_DATA_BITS_7 = 1U              ///< Data bits 7-bits
} uart_data_bits_t;

/** UART Parity definition */
typedef enum e_uart_parity
{
    UART_PARITY_OFF  = 0U,             ///< No parity
    UART_PARITY_EVEN = 1U,             ///< Even parity
    UART_PARITY_ZERO = 2U,             ///< Zero parity
    UART_PARITY_ODD  = 3U              ///< Odd parity
} uart_parity_t;

/***********************************************************************************************************************
 *                          ADCK
 **********************************************************************************************************************/

/** ADC callback event definitions  */
typedef enum e_adc_event
{
    ADC_EVENT_SCAN_COMPLETE_GROUP_0,   ///< Scan group 0 scan completed
    ADC_EVENT_SCAN_COMPLETE_GROUP_1,   ///< Scan group 1 scan completed
    ADC_EVENT_SCAN_COMPLETE_GROUP_2,   ///< Scan group 2 scan completed
    ADC_EVENT_SCAN_COMPLETE_GROUP_3,   ///< Scan group 3 scan completed
    ADC_EVENT_SCAN_COMPLETE_GROUP_4,   ///< Scan group 4 scan completed
    ADC_EVENT_SCAN_COMPLETE_SG_DIAG,   ///< SG-Diag scan completed
    ADC_EVENT_ID_ERROR,                ///< An ID event had occurred
    ADC_EVENT_OVERWRITE_ERROR,         ///< An overwrite event had occcured
    ADC_EVENT_TRIGGER_OVERLAP_ERROR,   ///< An trigger overlap event had occurred
    ADC_EVENT_MPX_CONVERSION,          ///< Even called when MPX interrupt occurred
    ADC_EVENT_UPPER_LOWER_LIMIT_ERROR  ///< An upper/lower limit check event had occured
} adc_event_t;

/* ADC channels for ADCK */
typedef enum e_adc_channel
{
    ADC_CHANNEL_0                 = 0U,   ///< ADCKn channel 0
    ADC_CHANNEL_1                 = 1U,   ///< ADCKn channel 1
    ADC_CHANNEL_2                 = 2U,   ///< ADCKn channel 2
    ADC_CHANNEL_3                 = 3U,   ///< ADCKn channel 3
    ADC_CHANNEL_4                 = 4U,   ///< ADCKn channel 4
    ADC_CHANNEL_5                 = 5U,   ///< ADCKn channel 5
    ADC_CHANNEL_6                 = 6U,   ///< ADCKn channel 6
    ADC_CHANNEL_7                 = 7U,   ///< ADCKn channel 7
    ADC_CHANNEL_8                 = 8U,   ///< ADCKn channel 8
    ADC_CHANNEL_9                 = 9U,   ///< ADCKn channel 9
    ADC_CHANNEL_10                = 10U,  ///< ADCKn channel 10
    ADC_CHANNEL_11                = 11U,  ///< ADCKn channel 11
    ADC_CHANNEL_12                = 12U,  ///< ADCKn channel 12
    ADC_CHANNEL_13                = 13U,  ///< ADCKn channel 13
    ADC_CHANNEL_14                = 14U,  ///< ADCKn channel 14
    ADC_CHANNEL_15                = 15U,  ///< ADCKn channel 15
    ADC_CHANNEL_16                = 16U,  ///< ADCKn channel 16
    ADC_CHANNEL_17                = 17U,  ///< ADCKn channel 17
    ADC_CHANNEL_18                = 18U,  ///< ADCKn channel 18
    ADC_CHANNEL_19                = 19U,  ///< ADCKn channel 19
    ADC_CHANNEL_20                = 20U,  ///< ADCKn channel 20
    ADC_CHANNEL_21                = 21U,  ///< ADCKn channel 21
    ADC_CHANNEL_22                = 22U,  ///< ADCKn channel 22
    ADC_CHANNEL_23                = 23U,  ///< ADCKn channel 23
    ADC_CHANNEL_24                = 24U,  ///< ADCKn channel 24
    ADC_CHANNEL_25                = 25U,  ///< ADCKn channel 25
    ADC_CHANNEL_26                = 26U,  ///< ADCKn channel 26
    ADC_CHANNEL_27                = 27U,  ///< ADCKn channel 27
    ADC_CHANNEL_28                = 28U,  ///< ADCKn channel 28
    ADC_CHANNEL_29                = 29U,  ///< ADCKn channel 29
    ADC_CHANNEL_30                = 30U,  ///< ADCKn channel 30
    ADC_CHANNEL_31                = 31U,  ///< ADCKn channel 31
    ADC_CHANNEL_32                = 32U,  ///< ADCKn channel 32
    ADC_CHANNEL_33                = 33U,  ///< ADCKn channel 33
    ADC_CHANNEL_34                = 34U,  ///< ADCKn channel 34
    ADC_CHANNEL_35                = 35U,  ///< ADCKn channel 35
    ADC_CHANNEL_36                = 36U,  ///< ADCKn channel 36
    ADC_CHANNEL_37                = 37U,  ///< ADCKn channel 37
    ADC_CHANNEL_38                = 38U,  ///< ADCKn channel 38
    ADC_CHANNEL_39                = 39U,  ///< ADCKn channel 39
    ADC_CHANNEL_40                = 40U,  ///< ADCKn channel 40
    ADC_CHANNEL_41                = 41U,  ///< ADCKn channel 41
    ADC_CHANNEL_42                = 42U,  ///< ADCKn channel 42
    ADC_CHANNEL_43                = 43U,  ///< ADCKn channel 43
    ADC_CHANNEL_44                = 44U,  ///< ADCKn channel 44
    ADC_CHANNEL_45                = 45U,  ///< ADCKn channel 45
    ADC_CHANNEL_46                = 46U,  ///< ADCKn channel 46
    ADC_CHANNEL_47                = 47U,  ///< ADCKn channel 47
    ADC_CHANNEL_48                = 48U,  ///< ADCKn channel 48
    ADC_CHANNEL_49                = 49U,  ///< ADCKn channel 49
    ADC_CHANNEL_50                = 50U,  ///< ADCKn channel 50
    ADC_CHANNEL_51                = 51U,  ///< ADCKn channel 51
    ADC_CHANNEL_52                = 52U,  ///< ADCKn channel 52
    ADC_CHANNEL_53                = 53U,  ///< ADCKn channel 53
    ADC_CHANNEL_54                = 54U,  ///< ADCKn channel 54
    ADC_CHANNEL_55                = 55U,  ///< ADCKn channel 55
    ADC_CHANNEL_56                = 56U,  ///< ADCKn channel 56
    ADC_CHANNEL_57                = 57U,  ///< ADCKn channel 57
    ADC_CHANNEL_58                = 58U,  ///< ADCKn channel 58
    ADC_CHANNEL_59                = 59U,  ///< ADCKn channel 59
    ADC_CHANNEL_60                = 60U,  ///< ADCKn channel 60
    ADC_CHANNEL_61                = 61U,  ///< ADCKn channel 61
    ADC_CHANNEL_62                = 62U,  ///< ADCKn channel 62
    ADC_CHANNEL_63                = 63U,  ///< ADCKn channel 63
    ADC_CHANNEL_PWM_DIAG          = 80U,  ///< ADCKn PWWM-Diag channel
    ADC_CHANNEL_SG_DIAG_CHANNEL_0 = 120U, ///< ADCKn SG-Diag channel 0
    ADC_CHANNEL_SG_DIAG_CHANNEL_1 = 121U, ///< ADCKn SG-Diag channel 1
    ADC_CHANNEL_SG_DIAG_CHANNEL_2 = 122U  ///< ADCKn SG-Diag channel 2
} adc_channel_t;

/** ADC Information Structure for Transfer Interface */
typedef struct st_adc_info
{
    const volatile void * p_address[BSP_OVERRIDE_ADC_NUMBER_OF_SG]; ///< The address to start reading the data from
    uint32_t              length[BSP_OVERRIDE_ADC_NUMBER_OF_SG];    ///< The total number of transfers to read
} adc_info_t;

/***********************************************************************************************************************
 *                          SFMA
 **********************************************************************************************************************/

/** Number of bytes in the address. */
typedef enum e_spi_flash_address_bytes
{
    SPI_FLASH_ADDRESS_BYTES_3 = 0x07,                ///< 3 address bytes

    SPI_FLASH_ADDRESS_BYTES_4 = 0x0F,                ///< 4 address bytes

    SPI_FLASH_ADDRESS_BYTES_4_4BYTE_READ_CODE = 0x13 ///< 4 address bytes with 4-byte read command
} spi_flash_address_bytes_t;

/***********************************************************************************************************************
 *                          ETHER
 **********************************************************************************************************************/

/** Event code of callback function */
typedef enum e_ether_event
{
    ETHER_EVENT_WAKEON_LAN,              ///< Magic packet detection event
    ETHER_EVENT_LINK_ON,                 ///< Link up detection event
    ETHER_EVENT_LINK_OFF,                ///< Link down detection event
    ETHER_EVENT_RX_COMPLETE,             ///< Receive complete event.
    ETHER_EVENT_RX_MESSAGE_LOST,         ///< Receive FIFO overflow or Receive descriptor is full.
    ETHER_EVENT_TX_COMPLETE,             ///< Transmit complete event.
    ETHER_EVENT_TX_BUFFER_EMPTY,         ///< Transmit descriptor or FIFO is empty.
    ETHER_EVENT_TX_ABORTED,              ///< Transmit abort event.
    ETHER_EVENT_ERR_GLOBAL,              ///< Global error has occurred.
    ETHER_EVENT_GET_NIC_INFO,            ///< Get NIC Info.
    ETHER_LINK_VERIFICATION_SUCCEED,     ///< Link verification succeed event
    ETHER_LINK_VERIFICATION_FAIL,        ///< Link verification fail event
    ETHER_EVENT_RX_INTERRUPT,            ///< Rx Interrupt event
    ETHER_EVENT_TX_INTERRUPT,            ///< Tx Interrupt event
    ETHER_CLOSE_SUCCEED,                 ///< Close event
    ETHER_EVENT_ERROR_LACK_OF_RESOURCES, ///< Allocation memory error due to lack of resources
    ETHER_EVENT_CONFIG_PHY_SUCCESSFUL    ///< Physical event
} ether_event_t;

/***********************************************************************************************************************
 *                          ETHER_PHY
 **********************************************************************************************************************/

/** Phy LSI */
typedef enum e_ether_phy_lsi_type
{
    ETHER_PHY_LSI_TYPE_DEFAULT            = 0,    ///< Select default configuration. This type dose not change Phy LSI default setting by strapping option.
    ETHER_PHY_LSI_TYPE_LAN8700            = 1,    ///< Select configuration for LAN8700.
    ETHER_PHY_LSI_TYPE_10BASE_T1S_LAN8680 = 2,    ///< Select configuration for LAN8680.
    ETHER_PHY_LSI_TYPE_LAN88Q2112         = 4,    ///< Select configuration for LAN88Q2112.
    ETHER_PHY_LSI_TYPE_CUSTOM             = 0xFFU ///< Select configuration for User custom.
} ether_phy_lsi_type_t;

/***********************************************************************************************************************
 *                          CRC
 **********************************************************************************************************************/

/** CRC Calculation Switching (LMS) */
typedef enum e_crc_bit_order
{
    CRC_BIT_ORDER_LMS_MSB = 0,         ///< Generates CRC for MSB first communication
    CRC_BIT_ORDER_LMS_LSB = 1          ///< Generates CRC for LSB first communication
} crc_bit_order_t;

/** CRC result type */
typedef uint64_t crc_result_t;

/***********************************************************************************************************************
 *                          IOPORT
 **********************************************************************************************************************/

/** Input signal */
typedef enum e_ioport_filter_signal
{
    IOPORT_FILTER_SIGNAL_RLIN30RX = 0,         ///< Select Enable Noise Filter RLIN30RX
    IOPORT_FILTER_SIGNAL_RLIN31RX,             ///< Select Enable Noise Filter RLIN31RX
    IOPORT_FILTER_SIGNAL_CAN0RX,               ///< Select Enable Noise Filter CAN0RX
    IOPORT_FILTER_SIGNAL_CAN1RX,               ///< Select Enable Noise Filter CAN1RX
    IOPORT_FILTER_SIGNAL_FLXA0RXDA,            ///< Select Enable Noise Filter FLXA0RXDA

    IOPORT_FILTER_SIGNAL_SENT0RX,              ///< Select Enable Noise Filter SENT0RX
    IOPORT_FILTER_SIGNAL_SENT1RX,              ///< Select Enable Noise Filter SENT1RX
    IOPORT_FILTER_SIGNAL_SENT2RX,              ///< Select Enable Noise Filter SENT2RX
    IOPORT_FILTER_SIGNAL_SENT3RX,              ///< Select Enable Noise Filter SENT3RX
    IOPORT_FILTER_SIGNAL_SENT4RX,              ///< Select Enable Noise Filter SENT4RX
    IOPORT_FILTER_SIGNAL_SENT5RX,              ///< Select Enable Noise Filter SENT5RX
    IOPORT_FILTER_SIGNAL_SENT6RX,              ///< Select Enable Noise Filter SENT6RX
    IOPORT_FILTER_SIGNAL_SENT7RX,              ///< Select Enable Noise Filter SENT7RX
    IOPORT_FILTER_SIGNAL_SENT8RX,              ///< Select Enable Noise Filter SENT8RX
    IOPORT_FILTER_SIGNAL_SENT9RX,              ///< Select Enable Noise Filter SENT9RX
    IOPORT_FILTER_SIGNAL_SENT10RX,             ///< Select Enable Noise Filter SENT10RX
    IOPORT_FILTER_SIGNAL_SENT11RX,             ///< Select Enable Noise Filter SENT11RX
    IOPORT_FILTER_SIGNAL_SENT12RX,             ///< Select Enable Noise Filter SENT12RX
    IOPORT_FILTER_SIGNAL_SENT13RX,             ///< Select Enable Noise Filter SENT13RX
    IOPORT_FILTER_SIGNAL_SENT14RX,             ///< Select Enable Noise Filter SENT14RX
    IOPORT_FILTER_SIGNAL_SENT15RX,             ///< Select Enable Noise Filter SENT15RX
    IOPORT_FILTER_SIGNAL_SENT16RX,             ///< Select Enable Noise Filter SENT16RX
    IOPORT_FILTER_SIGNAL_SENT17RX,             ///< Select Enable Noise Filter SENT17RX
    IOPORT_FILTER_SIGNAL_SENT18RX,             ///< Select Enable Noise Filter SENT18RX
    IOPORT_FILTER_SIGNAL_SENT19RX,             ///< Select Enable Noise Filter SENT19RX
    IOPORT_FILTER_SIGNAL_SENT20RX,             ///< Select Enable Noise Filter SENT20RX
    IOPORT_FILTER_SIGNAL_SENT21RX,             ///< Select Enable Noise Filter SENT21RX
    IOPORT_FILTER_SIGNAL_SENT22RX,             ///< Select Enable Noise Filter SENT22RX
    IOPORT_FILTER_SIGNAL_SENT23RX,             ///< Select Enable Noise Filter SENT23RX
    IOPORT_FILTER_SIGNAL_SENT24RX,             ///< Select Enable Noise Filter SENT24RX
    IOPORT_FILTER_SIGNAL_SENT25RX,             ///< Select Enable Noise Filter SENT25RX
    IOPORT_FILTER_SIGNAL_SENT26RX,             ///< Select Enable Noise Filter SENT26RX
    IOPORT_FILTER_SIGNAL_SENT27RX,             ///< Select Enable Noise Filter SENT27RX
    IOPORT_FILTER_SIGNAL_SENT28RX,             ///< Select Enable Noise Filter SENT28RX
    IOPORT_FILTER_SIGNAL_SENT29RX,             ///< Select Enable Noise Filter SENT29RX

    IOPORT_FILTER_SIGNAL_PSI50RX,              ///< Select Enable Noise Filter PSI50RX
    IOPORT_FILTER_SIGNAL_PSI51RX,              ///< Select Enable Noise Filter PSI51RX
    IOPORT_FILTER_SIGNAL_PSI52RX,              ///< Select Enable Noise Filter PSI52RX
    IOPORT_FILTER_SIGNAL_PSI53RX,              ///< Select Enable Noise Filter PSI53RX

    IOPORT_FILTER_SIGNAL_INTTAPA0ESO_TAPA0ESO, ///< Select Enable Noise Filter INTTAPA0ESO_TAPA0ESO
    IOPORT_FILTER_SIGNAL_INTTAPA1ESO_TAPA1ESO, ///< Select Enable Noise Filter INTTAPA1ESO_TAPA1ESO
    IOPORT_FILTER_SIGNAL_INTTAPA2ESO_TAPA2ESO, ///< Select Enable Noise Filter INTTAPA2ESO_TAPA2ESO
    IOPORT_FILTER_SIGNAL_INTTAPA3ESO_TAPA3ESO, ///< Select Enable Noise Filter INTTAPA3ESO_TAPA3ESO
    IOPORT_FILTER_SIGNAL_INTTAPA4ESO_TAPA4ESO, ///< Select Enable Noise Filter INTTAPA4ESO_TAPA4ESO
    IOPORT_FILTER_SIGNAL_INTTAPA5ESO_TAPA5ESO, ///< Select Enable Noise Filter INTTAPA5ESO_TAPA5ESO

    IOPORT_FILTER_SIGNAL_ADCK0TRG0,            ///< Select Enable Noise Filter ADCK0TRG0
    IOPORT_FILTER_SIGNAL_ADCK0TRG1,            ///< Select Enable Noise Filter ADCK0TRG1
    IOPORT_FILTER_SIGNAL_ADCK0TRG2,            ///< Select Enable Noise Filter ADCK0TRG2
    IOPORT_FILTER_SIGNAL_ADCK0TRG3,            ///< Select Enable Noise Filter ADCK0TRG3
    IOPORT_FILTER_SIGNAL_ADCK0TRG4,            ///< Select Enable Noise Filter ADCK0TRG4
    IOPORT_FILTER_SIGNAL_ADCK1TRG0,            ///< Select Enable Noise Filter ADCK1TRG0
    IOPORT_FILTER_SIGNAL_ADCK1TRG1,            ///< Select Enable Noise Filter ADCK1TRG1
    IOPORT_FILTER_SIGNAL_ADCK1TRG2,            ///< Select Enable Noise Filter ADCK1TRG2
    IOPORT_FILTER_SIGNAL_ADCK1TRG3,            ///< Select Enable Noise Filter ADCK1TRG3
    IOPORT_FILTER_SIGNAL_ADCK1TRG4,            ///< Select Enable Noise Filter ADCK1TRG4
    IOPORT_FILTER_SIGNAL_ADCK2TRG0,            ///< Select Enable Noise Filter ADCK2TRG0
    IOPORT_FILTER_SIGNAL_ADCK2TRG1,            ///< Select Enable Noise Filter ADCK2TRG1
    IOPORT_FILTER_SIGNAL_ADCK2TRG2,            ///< Select Enable Noise Filter ADCK2TRG2
    IOPORT_FILTER_SIGNAL_ADCK2TRG3,            ///< Select Enable Noise Filter ADCK2TRG3
    IOPORT_FILTER_SIGNAL_ADCK2TRG4,            ///< Select Enable Noise Filter ADCK2TRG4
    IOPORT_FILTER_SIGNAL_ADCK3TRG0,            ///< Select Enable Noise Filter ADCK3TRG0
    IOPORT_FILTER_SIGNAL_ADCK3TRG1,            ///< Select Enable Noise Filter ADCK3TRG1
    IOPORT_FILTER_SIGNAL_ADCK3TRG2,            ///< Select Enable Noise Filter ADCK3TRG2
    IOPORT_FILTER_SIGNAL_ADCK3TRG3,            ///< Select Enable Noise Filter ADCK3TRG3
    IOPORT_FILTER_SIGNAL_ADCK3TRG4,            ///< Select Enable Noise Filter ADCK3TRG4
    IOPORT_FILTER_SIGNAL_ADCKATRG0,            ///< Select Enable Noise Filter ADCKATRG0
    IOPORT_FILTER_SIGNAL_ADCKATRG1,            ///< Select Enable Noise Filter ADCKATRG1
    IOPORT_FILTER_SIGNAL_ADCKATRG2,            ///< Select Enable Noise Filter ADCKATRG2
    IOPORT_FILTER_SIGNAL_ADCKATRG3,            ///< Select Enable Noise Filter ADCKATRG3
    IOPORT_FILTER_SIGNAL_ADCKATRG4,            ///< Select Enable Noise Filter ADCKATRG4

    IOPORT_FILTER_SIGNAL_RHSB0EMRG,            ///< Select Enable Noise Filter RHSB0EMRG
    IOPORT_FILTER_SIGNAL_RHSB1EMRG,            ///< Select Enable Noise Filter RHSB1EMRG
    IOPORT_FILTER_SIGNAL_RHSB2EMRG,            ///< Select Enable Noise Filter RHSB2EMRG
    IOPORT_FILTER_SIGNAL_RHSB3EMRG,            ///< Select Enable Noise Filter RHSB3EMRG

    IOPORT_FILTER_SIGNAL_ENCA0TIN0,            ///< Select Enable Noise Filter ENCA0TIN0
    IOPORT_FILTER_SIGNAL_ENCA0TIN1,            ///< Select Enable Noise Filter ENCA0TIN1
    IOPORT_FILTER_SIGNAL_ENCA1TIN0,            ///< Select Enable Noise Filter ENCA1TIN0
    IOPORT_FILTER_SIGNAL_ENCA1TIN1,            ///< Select Enable Noise Filter ENCA1TIN1

    IOPORT_FILTER_SIGNAL_TAUD0I0,              ///< Select Enable Noise Filter TAUD0I0
    IOPORT_FILTER_SIGNAL_TAUD0I1,              ///< Select Enable Noise Filter TAUD0I1
    IOPORT_FILTER_SIGNAL_TAUD0I2,              ///< Select Enable Noise Filter TAUD0I2
    IOPORT_FILTER_SIGNAL_TAUD0I3,              ///< Select Enable Noise Filter TAUD0I3
    IOPORT_FILTER_SIGNAL_TAUD0I4,              ///< Select Enable Noise Filter TAUD0I4
    IOPORT_FILTER_SIGNAL_TAUD0I5,              ///< Select Enable Noise Filter TAUD0I5
    IOPORT_FILTER_SIGNAL_TAUD0I6,              ///< Select Enable Noise Filter TAUD0I6
    IOPORT_FILTER_SIGNAL_TAUD0I7,              ///< Select Enable Noise Filter TAUD0I7
    IOPORT_FILTER_SIGNAL_TAUD0I8,              ///< Select Enable Noise Filter TAUD0I8
    IOPORT_FILTER_SIGNAL_TAUD0I9,              ///< Select Enable Noise Filter TAUD0I9
    IOPORT_FILTER_SIGNAL_TAUD0I10,             ///< Select Enable Noise Filter TAUD0I10
    IOPORT_FILTER_SIGNAL_TAUD0I11,             ///< Select Enable Noise Filter TAUD0I11
    IOPORT_FILTER_SIGNAL_TAUD0I12,             ///< Select Enable Noise Filter TAUD0I12
    IOPORT_FILTER_SIGNAL_TAUD0I13,             ///< Select Enable Noise Filter TAUD0I13
    IOPORT_FILTER_SIGNAL_TAUD0I14,             ///< Select Enable Noise Filter TAUD0I14
    IOPORT_FILTER_SIGNAL_TAUD0I15,             ///< Select Enable Noise Filter TAUD0I15

    IOPORT_FILTER_SIGNAL_TAUD1I0,              ///< Select Enable Noise Filter TAUD1I0
    IOPORT_FILTER_SIGNAL_TAUD1I1,              ///< Select Enable Noise Filter TAUD1I1
    IOPORT_FILTER_SIGNAL_TAUD1I2,              ///< Select Enable Noise Filter TAUD1I2
    IOPORT_FILTER_SIGNAL_TAUD1I3,              ///< Select Enable Noise Filter TAUD1I3
    IOPORT_FILTER_SIGNAL_TAUD1I4,              ///< Select Enable Noise Filter TAUD1I4
    IOPORT_FILTER_SIGNAL_TAUD1I5,              ///< Select Enable Noise Filter TAUD1I5
    IOPORT_FILTER_SIGNAL_TAUD1I6,              ///< Select Enable Noise Filter TAUD1I6
    IOPORT_FILTER_SIGNAL_TAUD1I7,              ///< Select Enable Noise Filter TAUD1I7
    IOPORT_FILTER_SIGNAL_TAUD1I8,              ///< Select Enable Noise Filter TAUD1I8
    IOPORT_FILTER_SIGNAL_TAUD1I9,              ///< Select Enable Noise Filter TAUD1I9
    IOPORT_FILTER_SIGNAL_TAUD1I10,             ///< Select Enable Noise Filter TAUD1I10
    IOPORT_FILTER_SIGNAL_TAUD1I11,             ///< Select Enable Noise Filter TAUD1I11
    IOPORT_FILTER_SIGNAL_TAUD1I12,             ///< Select Enable Noise Filter TAUD1I12
    IOPORT_FILTER_SIGNAL_TAUD1I13,             ///< Select Enable Noise Filter TAUD1I13
    IOPORT_FILTER_SIGNAL_TAUD1I14,             ///< Select Enable Noise Filter TAUD1I14
    IOPORT_FILTER_SIGNAL_TAUD1I15,             ///< Select Enable Noise Filter TAUD1I15

    IOPORT_FILTER_SIGNAL_TAUD2I0,              ///< Select Enable Noise Filter TAUD2I0
    IOPORT_FILTER_SIGNAL_TAUD2I1,              ///< Select Enable Noise Filter TAUD2I1
    IOPORT_FILTER_SIGNAL_TAUD2I2,              ///< Select Enable Noise Filter TAUD2I2
    IOPORT_FILTER_SIGNAL_TAUD2I3,              ///< Select Enable Noise Filter TAUD2I3
    IOPORT_FILTER_SIGNAL_TAUD2I4,              ///< Select Enable Noise Filter TAUD2I4
    IOPORT_FILTER_SIGNAL_TAUD2I5,              ///< Select Enable Noise Filter TAUD2I5
    IOPORT_FILTER_SIGNAL_TAUD2I6,              ///< Select Enable Noise Filter TAUD2I6
    IOPORT_FILTER_SIGNAL_TAUD2I7,              ///< Select Enable Noise Filter TAUD2I7
    IOPORT_FILTER_SIGNAL_TAUD2I8,              ///< Select Enable Noise Filter TAUD2I8
    IOPORT_FILTER_SIGNAL_TAUD2I9,              ///< Select Enable Noise Filter TAUD2I9
    IOPORT_FILTER_SIGNAL_TAUD2I10,             ///< Select Enable Noise Filter TAUD2I10
    IOPORT_FILTER_SIGNAL_TAUD2I11,             ///< Select Enable Noise Filter TAUD2I11
    IOPORT_FILTER_SIGNAL_TAUD2I12,             ///< Select Enable Noise Filter TAUD2I12
    IOPORT_FILTER_SIGNAL_TAUD2I13,             ///< Select Enable Noise Filter TAUD2I13
    IOPORT_FILTER_SIGNAL_TAUD2I14,             ///< Select Enable Noise Filter TAUD2I14
    IOPORT_FILTER_SIGNAL_TAUD2I15,             ///< Select Enable Noise Filter TAUD2I15

    IOPORT_FILTER_SIGNAL_TAUD3I0,              ///< Select Enable Noise Filter TAUD3I0
    IOPORT_FILTER_SIGNAL_TAUD3I1,              ///< Select Enable Noise Filter TAUD3I1
    IOPORT_FILTER_SIGNAL_TAUD3I2,              ///< Select Enable Noise Filter TAUD3I2
    IOPORT_FILTER_SIGNAL_TAUD3I3,              ///< Select Enable Noise Filter TAUD3I3
    IOPORT_FILTER_SIGNAL_TAUD3I4,              ///< Select Enable Noise Filter TAUD3I4
    IOPORT_FILTER_SIGNAL_TAUD3I5,              ///< Select Enable Noise Filter TAUD3I5
    IOPORT_FILTER_SIGNAL_TAUD3I6,              ///< Select Enable Noise Filter TAUD3I6
    IOPORT_FILTER_SIGNAL_TAUD3I7,              ///< Select Enable Noise Filter TAUD3I7
    IOPORT_FILTER_SIGNAL_TAUD3I8,              ///< Select Enable Noise Filter TAUD3I8
    IOPORT_FILTER_SIGNAL_TAUD3I9,              ///< Select Enable Noise Filter TAUD3I9
    IOPORT_FILTER_SIGNAL_TAUD3I10,             ///< Select Enable Noise Filter TAUD3I10
    IOPORT_FILTER_SIGNAL_TAUD3I11,             ///< Select Enable Noise Filter TAUD3I11
    IOPORT_FILTER_SIGNAL_TAUD3I12,             ///< Select Enable Noise Filter TAUD3I12
    IOPORT_FILTER_SIGNAL_TAUD3I13,             ///< Select Enable Noise Filter TAUD3I13
    IOPORT_FILTER_SIGNAL_TAUD3I14,             ///< Select Enable Noise Filter TAUD3I14
    IOPORT_FILTER_SIGNAL_TAUD3I15,             ///< Select Enable Noise Filter TAUD3I15

    IOPORT_FILTER_SIGNAL_TSG30PTSI0,           ///< Select Enable Noise Filter TSG30PTSI0_ENCA0E0
    IOPORT_FILTER_SIGNAL_TSG30PTSI1,           ///< Select Enable Noise Filter TSG30PTSI1_ENCA0E1
    IOPORT_FILTER_SIGNAL_TSG30PTSI2,           ///< Select Enable Noise Filter TSG30PTSI2_ENCA0EC
    IOPORT_FILTER_SIGNAL_TSG30CLKI,            ///< Select Enable Noise Filter TSG30CLKI
    IOPORT_FILTER_SIGNAL_TSG31PTSI0,           ///< Select Enable Noise Filter TSG31PTSI0_ENCA1E0
    IOPORT_FILTER_SIGNAL_TSG31PTSI1,           ///< Select Enable Noise Filter TSG31PTSI1_ENCA1E1
    IOPORT_FILTER_SIGNAL_TSG31PTSI2,           ///< Select Enable Noise Filter TSG31PTSI2_ENCA1EC
    IOPORT_FILTER_SIGNAL_TSG31CLKI,            ///< Select Enable Noise Filter TSG31CLKI
    IOPORT_FILTER_SIGNAL_TSG32CLKI,            ///< Select Enable Noise Filter TSG32CLKI

    IOPORT_FILTER_SIGNAL_IRQ0,                 ///< Select Enable Noise Filter IRQ0
    IOPORT_FILTER_SIGNAL_IRQ1,                 ///< Select Enable Noise Filter IRQ1
    IOPORT_FILTER_SIGNAL_IRQ2,                 ///< Select Enable Noise Filter IRQ2
    IOPORT_FILTER_SIGNAL_IRQ3,                 ///< Select Enable Noise Filter IRQ3
    IOPORT_FILTER_SIGNAL_IRQ4,                 ///< Select Enable Noise Filter IRQ4
    IOPORT_FILTER_SIGNAL_IRQ5,                 ///< Select Enable Noise Filter IRQ5
    IOPORT_FILTER_SIGNAL_IRQ6,                 ///< Select Enable Noise Filter IRQ6
    IOPORT_FILTER_SIGNAL_IRQ7,                 ///< Select Enable Noise Filter IRQ7
    IOPORT_FILTER_SIGNAL_IRQ8,                 ///< Select Enable Noise Filter IRQ8
    IOPORT_FILTER_SIGNAL_IRQ9,                 ///< Select Enable Noise Filter IRQ9
    IOPORT_FILTER_SIGNAL_IRQ10,                ///< Select Enable Noise Filter IRQ10
    IOPORT_FILTER_SIGNAL_IRQ11,                ///< Select Enable Noise Filter IRQ11
    IOPORT_FILTER_SIGNAL_IRQ12,                ///< Select Enable Noise Filter IRQ12
    IOPORT_FILTER_SIGNAL_IRQ13,                ///< Select Enable Noise Filter IRQ13
    IOPORT_FILTER_SIGNAL_IRQ14,                ///< Select Enable Noise Filter IRQ14
    IOPORT_FILTER_SIGNAL_IRQ15,                ///< Select Enable Noise Filter IRQ15
    IOPORT_FILTER_SIGNAL_IRQ16,                ///< Select Enable Noise Filter IRQ16
    IOPORT_FILTER_SIGNAL_IRQ17,                ///< Select Enable Noise Filter IRQ17
    IOPORT_FILTER_SIGNAL_IRQ18,                ///< Select Enable Noise Filter IRQ18
    IOPORT_FILTER_SIGNAL_IRQ19,                ///< Select Enable Noise Filter IRQ19
    IOPORT_FILTER_SIGNAL_IRQ20,                ///< Select Enable Noise Filter IRQ20
    IOPORT_FILTER_SIGNAL_IRQ21,                ///< Select Enable Noise Filter IRQ21
    IOPORT_FILTER_SIGNAL_IRQ22,                ///< Select Enable Noise Filter IRQ22
    IOPORT_FILTER_SIGNAL_IRQ23,                ///< Select Enable Noise Filter IRQ23
    IOPORT_FILTER_SIGNAL_IRQ24,                ///< Select Enable Noise Filter IRQ24
    IOPORT_FILTER_SIGNAL_IRQ25,                ///< Select Enable Noise Filter IRQ25
    IOPORT_FILTER_SIGNAL_IRQ26,                ///< Select Enable Noise Filter IRQ26
    IOPORT_FILTER_SIGNAL_IRQ27,                ///< Select Enable Noise Filter IRQ27
    IOPORT_FILTER_SIGNAL_IRQ28,                ///< Select Enable Noise Filter IRQ28
    IOPORT_FILTER_SIGNAL_IRQ29,                ///< Select Enable Noise Filter IRQ29
    IOPORT_FILTER_SIGNAL_IRQ30,                ///< Select Enable Noise Filter IRQ30
    IOPORT_FILTER_SIGNAL_IRQ31,                ///< Select Enable Noise Filter IRQ31
    IOPORT_FILTER_SIGNAL_IRQ32,                ///< Select Enable Noise Filter IRQ32
    IOPORT_FILTER_SIGNAL_IRQ33,                ///< Select Enable Noise Filter IRQ33
    IOPORT_FILTER_SIGNAL_IRQ34,                ///< Select Enable Noise Filter IRQ34
    IOPORT_FILTER_SIGNAL_IRQ35,                ///< Select Enable Noise Filter IRQ35
    IOPORT_FILTER_SIGNAL_IRQ36,                ///< Select Enable Noise Filter IRQ36
    IOPORT_FILTER_SIGNAL_IRQ37,                ///< Select Enable Noise Filter IRQ37
    IOPORT_FILTER_SIGNAL_IRQ38,                ///< Select Enable Noise Filter IRQ38
    IOPORT_FILTER_SIGNAL_IRQ39,                ///< Select Enable Noise Filter IRQ39
    IOPORT_FILTER_SIGNAL_NMI,                  ///< Select Enable Noise Filter NMI
    IOPORT_FILTER_SIGNAL_IRQ40,                ///< Select Enable Noise Filter IRQ40
    IOPORT_FILTER_SIGNAL_IRQ41,                ///< Select Enable Noise Filter IRQ41
    IOPORT_FILTER_SIGNAL_IRQ42,                ///< Select Enable Noise Filter IRQ42
    IOPORT_FILTER_SIGNAL_IRQ43,                ///< Select Enable Noise Filter IRQ43

    IOPORT_FILTER_SIGNAL_ERRORIN0,             ///< Select Enable Noise Filter ERRORIN0
    IOPORT_FILTER_SIGNAL_ERRORIN1,             ///< Select Enable Noise Filter ERRORIN1
    IOPORT_FILTER_SIGNAL_ERRORIN2,             ///< Select Enable Noise Filter ERRORIN2
    IOPORT_FILTER_SIGNAL_ERRORIN3,             ///< Select Enable Noise Filter ERRORIN3
    IOPORT_FILTER_SIGNAL_ERRORIN4,             ///< Select Enable Noise Filter ERRORIN4
    IOPORT_FILTER_SIGNAL_ERRORIN5,             ///< Select Enable Noise Filter ERRORIN5
    IOPORT_FILTER_SIGNAL_ERRORIN6,             ///< Select Enable Noise Filter ERRORIN6
    IOPORT_FILTER_SIGNAL_ERRORIN7,             ///< Select Enable Noise Filter ERRORIN7

    IOPORT_FILTER_SIGNAL_TAUJ2I0,              ///< Select Enable Noise Filter TAUJ2I0
    IOPORT_FILTER_SIGNAL_TAUJ2I1,              ///< Select Enable Noise Filter TAUJ2I1
    IOPORT_FILTER_SIGNAL_TAUJ2I2,              ///< Select Enable Noise Filter TAUJ2I2
    IOPORT_FILTER_SIGNAL_TAUJ2I3,              ///< Select Enable Noise Filter TAUJ2I3

    IOPORT_FILTER_SIGNAL_TAUJ3I0,              ///< Select Enable Noise Filter TAUJ3I0
    IOPORT_FILTER_SIGNAL_TAUJ3I1,              ///< Select Enable Noise Filter TAUJ3I1
    IOPORT_FILTER_SIGNAL_TAUJ3I2,              ///< Select Enable Noise Filter TAUJ3I2
    IOPORT_FILTER_SIGNAL_TAUJ3I3,              ///< Select Enable Noise Filter TAUJ3I3

    IOPORT_FILTER_SIGNAL_TSGTRG,               ///< Select Enable Noise Filter TSGTRG

    IOPORT_FILTER_SIGNAL_EM00_IN_A,            ///< Select Enable Noise Filter EM00_IN_A
    IOPORT_FILTER_SIGNAL_EM00_IN_B,            ///< Select Enable Noise Filter EM00_IN_B
    IOPORT_FILTER_SIGNAL_EM00_IN_Z,            ///< Select Enable Noise Filter EM00_IN_Z
    IOPORT_FILTER_SIGNAL_EM10_IN_A,            ///< Select Enable Noise Filter EM10_IN_A
    IOPORT_FILTER_SIGNAL_EM10_IN_B,            ///< Select Enable Noise Filter EM10_IN_B
    IOPORT_FILTER_SIGNAL_EM10_IN_Z,            ///< Select Enable Noise Filter EM10_IN_Z
    IOPORT_FILTER_SIGNAL_MAX                   ///< The number of element
} ioport_filter_signal_t;

/***********************************************************************************************************************
 *                          MOTOR SPEED
 **********************************************************************************************************************/

/** Events that can trigger a callback function */
typedef enum e_motor_speed_event
{
    MOTOR_SPEED_EVENT_FORWARD = 1,     ///< Event forward speed control
    MOTOR_SPEED_EVENT_BACKWARD,        ///< Event backward speed control
    MOTOR_SPEED_EVENT_ENCODER_CYCLIC,  ///< Event encoder cyclic
    MOTOR_SPEED_EVENT_ENCODER_ADJUST,  ///< Event encoder adjust
    MOTOR_SPEED_EVENT_RESOLVER_CYCLIC, ///< Event resolver cyclic
} motor_speed_event_t;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

#endif

/** @} (end addtogroup BSP_MCU_RH850U2BX) */
