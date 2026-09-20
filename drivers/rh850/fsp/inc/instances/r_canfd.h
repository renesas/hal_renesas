/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_CANFD_H
#define R_CANFD_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_canfd_cfg.h"
#include "r_can_api.h"
#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE  /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
 #include "r_transfer_api.h"
#endif

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup CANFD
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define CANFD_TXMB_INT_REG_MAX            (3U)
#define CANFD_TX_QUEUE_NUM                (6U)
#define CANFD_PNF_DATA_PER_REG_NUM        (4U)
#if BSP_FEATURE_CANFD_NUM_CHANNELS > (8U)
 #define CANFD_NUM_CHANNEL_PER_UNIT       (8U)
#else
 #define CANFD_NUM_CHANNEL_PER_UNIT       (BSP_FEATURE_CANFD_NUM_CHANNELS)
#endif
#define CANFD_MAX_CHANNEL_PER_UNIT        (8UL)
#define CANFD_CHANNEL_MAX_TRANSFER_CFG    (5U) /* PRQA S 1534 # Advisory R-2.5 */
#define CANFD_SIZE_DATA_WORD_BUFFER       (19) /* PRQA S 1534 # Advisory R-2.5 */

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** CANFD Status */
typedef enum e_canfd_status                                      /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_STATUS_RESET_MODE    = 0x001,                          ///< Channel in Reset mode
    CANFD_STATUS_HALT_MODE     = 0x002,                          ///< Channel in Halt mode
    CANFD_STATUS_SLEEP_MODE    = 0x004,                          ///< Channel in Sleep mode
    CANFD_STATUS_ERROR_PASSIVE = 0x008,                          ///< Channel in error-passive state
    CANFD_STATUS_BUS_OFF       = 0x010,                          ///< Channel in bus-off state
    CANFD_STATUS_TRANSMITTING  = 0x020,                          ///< Channel is transmitting
    CANFD_STATUS_RECEIVING     = 0x040,                          ///< Channel is receiving
    CANFD_STATUS_READY         = 0x080,                          ///< Channel is ready for communication
    CANFD_STATUS_ESI           = 0x100,                          ///< At least one CAN-FD message was received with the ESI flag set
} canfd_status_t;                                                /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Error Code */
typedef enum e_canfd_error                                       /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_ERROR_CHANNEL_BUS                        = 0x00000001, ///< Bus Error
    CANFD_ERROR_CHANNEL_WARNING                    = 0x00000002, ///< Error Warning (TX/RX error count over 0x5F)
    CANFD_ERROR_CHANNEL_PASSIVE                    = 0x00000004, ///< Error Passive (TX/RX error count over 0x7F)
    CANFD_ERROR_CHANNEL_BUS_OFF_ENTRY              = 0x00000008, ///< Bus-Off State Entry
    CANFD_ERROR_CHANNEL_BUS_OFF_RECOVERY           = 0x00000010, ///< Recovery from Bus-Off State
    CANFD_ERROR_CHANNEL_OVERLOAD                   = 0x00000020, ///< Overload
    CANFD_ERROR_CHANNEL_BUS_LOCK                   = 0x00000040, ///< Bus Locked
    CANFD_ERROR_CHANNEL_ARBITRATION_LOSS           = 0x00000080, ///< Arbitration Lost
    CANFD_ERROR_CHANNEL_STUFF                      = 0x00000100, ///< Stuff Error
    CANFD_ERROR_CHANNEL_FORM                       = 0x00000200, ///< Form Error
    CANFD_ERROR_CHANNEL_ACK                        = 0x00000400, ///< ACK Error
    CANFD_ERROR_CHANNEL_CRC                        = 0x00000800, ///< CRC Error
    CANFD_ERROR_CHANNEL_BIT_RECESSIVE              = 0x00001000, ///< Bit Error (recessive) Error
    CANFD_ERROR_CHANNEL_BIT_DOMINANT               = 0x00002000, ///< Bit Error (dominant) Error
    CANFD_ERROR_CHANNEL_ACK_DELIMITER              = 0x00004000, ///< ACK Delimiter Error
    CANFD_ERROR_CHANNEL_DELAY_VIOLATE              = 0x00010000, ///< Transmitter Delay Compensation Violate
    CANFD_ERROR_CHANNEL_ERR_OCCUR_OVF              = 0x00020000, ///< Error Occurrence overflow
    CANFD_ERROR_CHANNEL_SUC_OCCUR_OVF              = 0x00040000, ///< Successful Occurrence overflow
    CANFD_ERROR_GLOBAL_DLC                         = 0x00100000, ///< DLC Error
    CANFD_ERROR_GLOBAL_MESSAGE_LOST                = 0x00200000, ///< Message Lost
    CANFD_ERROR_GLOBAL_TX_ENTRY_LOST               = 0x00400000, ///< TX History List Entry Lost
    CANFD_ERROR_GLOBAL_PAYLOAD_OVERFLOW            = 0x00800000, ///< FD Payload Overflow
    CANFD_ERROR_GLOBAL_TXQ_OVERWRITE               = 0x01000000, ///< TX Queue Message Overwrite
    CANFD_ERROR_GLOBAL_TXQ_MESSAGE_LOST            = 0x04000000, ///< TX Queue Message Lost
    CANFD_ERROR_GLOBAL_MESSAGE_OVERWRITE           = 0x08000000, ///< Transmit/Receive FIFO is overwrite
    CANFD_ERROR_CHANNEL_VM_FIFO_MESSAGE_LOST       = 0x02000000, ///< RXFIFO is overrun
    CANFD_ERROR_CHANNEL_VM_CFIFO_MESSAGE_OVERWRITE = 0x10000000, ///< Transmit/Receive FIFO is overwrite
    CANFD_ERROR_CHANNEL_VM_TXQ_CFIFO_MESSAGE_LOST  = 0x20000000, ///< Transmit Queue is overrun
    CANFD_ERROR_CHANNEL_VM_TXQ_MESSAGE_OVERWRITE   = 0x40000000, ///< Transmit Queue is overwrite

    /* PRQA S 4543, 0499 ++ # DP_MSR_R-10.1_UC1 , DP_MSR_R-12.2_UC1 */
    CANFD_ERROR_CHANNEL_VM_TX_ENTRY_LOST = (1 << 31),            ///< TX History List Entry Lost

    /* PRQA S 4543, 0499 -- */
} canfd_error_t;                                                 /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Transmit Buffer Type */

typedef enum e_canfd_tx_buffer_type                              /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_TX_BUFFER_TYPE_MB    = 0x0000,                         ///< Transmit Message Buffer
    CANFD_TX_BUFFER_TYPE_QUEUE = 0x0100,                         ///< Transmit Queue
    CANFD_TX_BUFFER_TYPE_CFIFO = 0x0200                          ///< Common FIFO (Transmit/Receive FIFO)
} canfd_tx_buffer_type_t;                                        /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Receive Buffer Type */

typedef enum e_canfd_rx_buffer_type                              /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_RX_BUFFER_TYPE_MB    = 0x0000,                         ///< Receive Message Buffer
    CANFD_RX_BUFFER_TYPE_FIFO  = 0x0100,                         ///< Receive FIFO
    CANFD_RX_BUFFER_TYPE_CFIFO = 0x0200                          ///< Common FIFO (Transmit/Receive FIFO)
} canfd_rx_buffer_type_t;                                        /* PRQA S 1535 # Advisory R-2.3 */

typedef enum e_canfd_rx_mb                                       /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_RX_MB_NONE = 0,
    CANFD_RX_MB_0    = 0x80 + 0,
    CANFD_RX_MB_1    = 0x80 + 1,
    CANFD_RX_MB_2    = 0x80 + 2,
    CANFD_RX_MB_3    = 0x80 + 3,
    CANFD_RX_MB_4    = 0x80 + 4,
    CANFD_RX_MB_5    = 0x80 + 5,
    CANFD_RX_MB_6    = 0x80 + 6,
    CANFD_RX_MB_7    = 0x80 + 7,
    CANFD_RX_MB_8    = 0x80 + 8,
    CANFD_RX_MB_9    = 0x80 + 9,
    CANFD_RX_MB_10   = 0x80 + 10,
    CANFD_RX_MB_11   = 0x80 + 11,
    CANFD_RX_MB_12   = 0x80 + 12,
    CANFD_RX_MB_13   = 0x80 + 13,
    CANFD_RX_MB_14   = 0x80 + 14,
    CANFD_RX_MB_15   = 0x80 + 15,
    CANFD_RX_MB_16   = 0x80 + 16,
    CANFD_RX_MB_17   = 0x80 + 17,
    CANFD_RX_MB_18   = 0x80 + 18,
    CANFD_RX_MB_19   = 0x80 + 19,
    CANFD_RX_MB_20   = 0x80 + 20,
    CANFD_RX_MB_21   = 0x80 + 21,
    CANFD_RX_MB_22   = 0x80 + 22,
    CANFD_RX_MB_23   = 0x80 + 23,
    CANFD_RX_MB_24   = 0x80 + 24,
    CANFD_RX_MB_25   = 0x80 + 25,
    CANFD_RX_MB_26   = 0x80 + 26,
    CANFD_RX_MB_27   = 0x80 + 27,
    CANFD_RX_MB_28   = 0x80 + 28,
    CANFD_RX_MB_29   = 0x80 + 29,
    CANFD_RX_MB_30   = 0x80 + 30,
    CANFD_RX_MB_31   = 0x80 + 31,
    CANFD_RX_MB_32   = 0x80 + 32,
    CANFD_RX_MB_33   = 0x80 + 33,
    CANFD_RX_MB_34   = 0x80 + 34,
    CANFD_RX_MB_35   = 0x80 + 35,
    CANFD_RX_MB_36   = 0x80 + 36,
    CANFD_RX_MB_37   = 0x80 + 37,
    CANFD_RX_MB_38   = 0x80 + 38,
    CANFD_RX_MB_39   = 0x80 + 39,
    CANFD_RX_MB_40   = 0x80 + 40,
    CANFD_RX_MB_41   = 0x80 + 41,
    CANFD_RX_MB_42   = 0x80 + 42,
    CANFD_RX_MB_43   = 0x80 + 43,
    CANFD_RX_MB_44   = 0x80 + 44,
    CANFD_RX_MB_45   = 0x80 + 45,
    CANFD_RX_MB_46   = 0x80 + 46,
    CANFD_RX_MB_47   = 0x80 + 47,
    CANFD_RX_MB_48   = 0x80 + 48,
    CANFD_RX_MB_49   = 0x80 + 49,
    CANFD_RX_MB_50   = 0x80 + 50,
    CANFD_RX_MB_51   = 0x80 + 51,
    CANFD_RX_MB_52   = 0x80 + 52,
    CANFD_RX_MB_53   = 0x80 + 53,
    CANFD_RX_MB_54   = 0x80 + 54,
    CANFD_RX_MB_55   = 0x80 + 55,
    CANFD_RX_MB_56   = 0x80 + 56,
    CANFD_RX_MB_57   = 0x80 + 57,
    CANFD_RX_MB_58   = 0x80 + 58,
    CANFD_RX_MB_59   = 0x80 + 59,
    CANFD_RX_MB_60   = 0x80 + 60,
    CANFD_RX_MB_61   = 0x80 + 61,
    CANFD_RX_MB_62   = 0x80 + 62,
    CANFD_RX_MB_63   = 0x80 + 63,
    CANFD_RX_MB_64   = 0x80 + 64,
    CANFD_RX_MB_65   = 0x80 + 65,
    CANFD_RX_MB_66   = 0x80 + 66,
    CANFD_RX_MB_67   = 0x80 + 67,
    CANFD_RX_MB_68   = 0x80 + 68,
    CANFD_RX_MB_69   = 0x80 + 69,
    CANFD_RX_MB_70   = 0x80 + 70,
    CANFD_RX_MB_71   = 0x80 + 71,
    CANFD_RX_MB_72   = 0x80 + 72,
    CANFD_RX_MB_73   = 0x80 + 73,
    CANFD_RX_MB_74   = 0x80 + 74,
    CANFD_RX_MB_75   = 0x80 + 75,
    CANFD_RX_MB_76   = 0x80 + 76,
    CANFD_RX_MB_77   = 0x80 + 77,
    CANFD_RX_MB_78   = 0x80 + 78,
    CANFD_RX_MB_79   = 0x80 + 79,
    CANFD_RX_MB_80   = 0x80 + 80,
    CANFD_RX_MB_81   = 0x80 + 81,
    CANFD_RX_MB_82   = 0x80 + 82,
    CANFD_RX_MB_83   = 0x80 + 83,
    CANFD_RX_MB_84   = 0x80 + 84,
    CANFD_RX_MB_85   = 0x80 + 85,
    CANFD_RX_MB_86   = 0x80 + 86,
    CANFD_RX_MB_87   = 0x80 + 87,
    CANFD_RX_MB_88   = 0x80 + 88,
    CANFD_RX_MB_89   = 0x80 + 89,
    CANFD_RX_MB_90   = 0x80 + 90,
    CANFD_RX_MB_91   = 0x80 + 91,
    CANFD_RX_MB_92   = 0x80 + 92,
    CANFD_RX_MB_93   = 0x80 + 93,
    CANFD_RX_MB_94   = 0x80 + 94,
    CANFD_RX_MB_95   = 0x80 + 95,
    CANFD_RX_MB_96   = 0x80 + 96,
    CANFD_RX_MB_97   = 0x80 + 97,
    CANFD_RX_MB_98   = 0x80 + 98,
    CANFD_RX_MB_99   = 0x80 + 99,
    CANFD_RX_MB_100  = 0x80 + 100,
    CANFD_RX_MB_101  = 0x80 + 101,
    CANFD_RX_MB_102  = 0x80 + 102,
    CANFD_RX_MB_103  = 0x80 + 103,
    CANFD_RX_MB_104  = 0x80 + 104,
    CANFD_RX_MB_105  = 0x80 + 105,
    CANFD_RX_MB_106  = 0x80 + 106,
    CANFD_RX_MB_107  = 0x80 + 107,
    CANFD_RX_MB_108  = 0x80 + 108,
    CANFD_RX_MB_109  = 0x80 + 109,
    CANFD_RX_MB_110  = 0x80 + 110,
    CANFD_RX_MB_111  = 0x80 + 111,
    CANFD_RX_MB_112  = 0x80 + 112,
    CANFD_RX_MB_113  = 0x80 + 113,
    CANFD_RX_MB_114  = 0x80 + 114,
    CANFD_RX_MB_115  = 0x80 + 115,
    CANFD_RX_MB_116  = 0x80 + 116,
    CANFD_RX_MB_117  = 0x80 + 117,
    CANFD_RX_MB_118  = 0x80 + 118,
    CANFD_RX_MB_119  = 0x80 + 119,
    CANFD_RX_MB_120  = 0x80 + 120,
    CANFD_RX_MB_121  = 0x80 + 121,
    CANFD_RX_MB_122  = 0x80 + 122,
    CANFD_RX_MB_123  = 0x80 + 123,
    CANFD_RX_MB_124  = 0x80 + 124,
    CANFD_RX_MB_125  = 0x80 + 125,
    CANFD_RX_MB_126  = 0x80 + 126,
    CANFD_RX_MB_127  = 0x80 + 127,
} canfd_rx_mb_t;                                                  /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD AFL Select Stored FIFO (RX FIFO + COMMON FIFO) */
typedef enum e_canfd_rx_target                                    /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_RX_TARGET_FIFO_0 = (1U),                                ///< CANFD RX FIFO 0
    CANFD_RX_TARGET_FIFO_1 = (1U << 1),                           ///< CANFD RX FIFO 1
    CANFD_RX_TARGET_FIFO_2 = (1U << 2),                           ///< CANFD RX FIFO 2
    CANFD_RX_TARGET_FIFO_3 = (1U << 3),                           ///< CANFD RX FIFO 3
    CANFD_RX_TARGET_FIFO_4 = (1U << 4),                           ///< CANFD RX FIFO 4
    CANFD_RX_TARGET_FIFO_5 = (1U << 5),                           ///< CANFD RX FIFO 5
    CANFD_RX_TARGET_FIFO_6 = (1U << 6),                           ///< CANFD RX FIFO 6
    CANFD_RX_TARGET_FIFO_7 = (1U << 7),                           ///< CANFD RX FIFO 7

    CANFD_RX_TARGET_CH0_COMFIFO_TXQ_0 = ((uint32_t) 1U << 8),     ///< CANFD Channel 0 Common FIFO/ TX Queue 0
    CANFD_RX_TARGET_CH0_COMFIFO_TXQ_1 = ((uint32_t) 1U << 9),     ///< CANFD Channel 0 Common FIFO/ TX Queue 1
    CANFD_RX_TARGET_CH0_COMFIFO_TXQ_2 = ((uint32_t) 1U << 10),    ///< CANFD Channel 0 Common FIFO/ TX Queue 2

    CANFD_RX_TARGET_CH1_COMFIFO_TXQ_0 = ((uint32_t) 1U << 11),    ///< CANFD Channel 1 Common FIFO/ TX Queue 0
    CANFD_RX_TARGET_CH1_COMFIFO_TXQ_1 = ((uint32_t) 1U << 12),    ///< CANFD Channel 1 Common FIFO/ TX Queue 1
    CANFD_RX_TARGET_CH1_COMFIFO_TXQ_2 = ((uint32_t) 1U << 13),    ///< CANFD Channel 1 Common FIFO/ TX Queue 2

    CANFD_RX_TARGET_CH2_COMFIFO_TXQ_0 = ((uint32_t) 1U << 14),    ///< CANFD Channel 2 Common FIFO/ TX Queue 0
    CANFD_RX_TARGET_CH2_COMFIFO_TXQ_1 = ((uint32_t) 1U << 15),    ///< CANFD Channel 2 Common FIFO/ TX Queue 1
    CANFD_RX_TARGET_CH2_COMFIFO_TXQ_2 = ((uint32_t) 1U << 16),    ///< CANFD Channel 2 Common FIFO/ TX Queue 2

    CANFD_RX_TARGET_CH3_COMFIFO_TXQ_0 = ((uint32_t) 1U << 17),    ///< CANFD Channel 3 Common FIFO/ TX Queue 0
    CANFD_RX_TARGET_CH3_COMFIFO_TXQ_1 = ((uint32_t) 1U << 18),    ///< CANFD Channel 3 Common FIFO/ TX Queue 1
    CANFD_RX_TARGET_CH3_COMFIFO_TXQ_2 = ((uint32_t) 1U << 19),    ///< CANFD Channel 3 Common FIFO/ TX Queue 2

    CANFD_RX_TARGET_CH4_COMFIFO_TXQ_0 = ((uint32_t) 1U << 20),    ///< CANFD Channel 4 Common FIFO/ TX Queue 0
    CANFD_RX_TARGET_CH4_COMFIFO_TXQ_1 = ((uint32_t) 1U << 21),    ///< CANFD Channel 4 Common FIFO/ TX Queue 1
    CANFD_RX_TARGET_CH4_COMFIFO_TXQ_2 = ((uint32_t) 1U << 22),    ///< CANFD Channel 4 Common FIFO/ TX Queue 2

    CANFD_RX_TARGET_CH5_COMFIFO_TXQ_0 = ((uint32_t) 1U << 23),    ///< CANFD Channel 5 Common FIFO/ TX Queue 0
    CANFD_RX_TARGET_CH5_COMFIFO_TXQ_1 = ((uint32_t) 1U << 24),    ///< CANFD Channel 5 Common FIFO/ TX Queue 1
    CANFD_RX_TARGET_CH5_COMFIFO_TXQ_2 = ((uint32_t) 1U << 25),    ///< CANFD Channel 5 Common FIFO/ TX Queue 2

    CANFD_RX_TARGET_CH6_COMFIFO_TXQ_0 = ((uint32_t) 1U << 26),    ///< CANFD Channel 6 Common FIFO/ TX Queue 0
    CANFD_RX_TARGET_CH6_COMFIFO_TXQ_1 = ((uint32_t) 1U << 27),    ///< CANFD Channel 6 Common FIFO/ TX Queue 1
    CANFD_RX_TARGET_CH6_COMFIFO_TXQ_2 = ((uint32_t) 1U << 28),    ///< CANFD Channel 6 Common FIFO/ TX Queue 2

    CANFD_RX_TARGET_CH7_COMFIFO_TXQ_0 = ((uint32_t) 1U << 29),    ///< CANFD Channel 7 Common FIFO/ TX Queue 0
    CANFD_RX_TARGET_CH7_COMFIFO_TXQ_1 = ((uint32_t) 1U << 30),    ///< CANFD Channel 7 Common FIFO/ TX Queue 1
    CANFD_RX_TARGET_CH7_COMFIFO_TXQ_2 = ((int32_t) 0x80000000UL), ///< CANFD Channel 7 Common FIFO/ TX Queue 2
} canfd_rx_target_t;                                              /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD AFL Minimum DLC settings */
typedef enum e_canfd_minimum_dlc                                  /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_MINIMUM_DLC_0 = 0,
    CANFD_MINIMUM_DLC_1,
    CANFD_MINIMUM_DLC_2,
    CANFD_MINIMUM_DLC_3,
    CANFD_MINIMUM_DLC_4,
    CANFD_MINIMUM_DLC_5,
    CANFD_MINIMUM_DLC_6,
    CANFD_MINIMUM_DLC_7,
    CANFD_MINIMUM_DLC_8,
    CANFD_MINIMUM_DLC_12,
    CANFD_MINIMUM_DLC_16,
    CANFD_MINIMUM_DLC_20,
    CANFD_MINIMUM_DLC_24,
    CANFD_MINIMUM_DLC_32,
    CANFD_MINIMUM_DLC_48,
    CANFD_MINIMUM_DLC_64,
} canfd_minimum_dlc_t;                 /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Receive Target Message */
typedef enum e_canfd_rule_target       /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_RULE_TARGET_OTHER = 0,
    CANFD_RULE_TARGET_SELF
} canfd_rule_target_t;

/** CANFD Receive Rule receive buffer enable **/
typedef enum e_canfd_rx_mb_options       /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_RX_MB_DISABLE,
    CANFD_RX_MB_ENABLE
} canfd_rx_mb_options_t;                 /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD FFI Mode enable **/
typedef enum e_canfd_ffi_mode_options    /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_FFI_MODE_OPTIONS_DISABLE,
    CANFD_FFI_MODE_OPTIONS_ENABLE
} canfd_ffi_mode_options_t;              /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD FFI Virtual Machine **/
typedef enum e_canfd_ffi_virtual_machine /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_FFI_VIRTUAL_MACHINE_NONE = 0x0,
    CANFD_FFI_VIRTUAL_MACHINE_0    = 0x8,
    CANFD_FFI_VIRTUAL_MACHINE_1    = 0x9,
    CANFD_FFI_VIRTUAL_MACHINE_2    = 0xA,
    CANFD_FFI_VIRTUAL_MACHINE_3    = 0xB,
    CANFD_FFI_VIRTUAL_MACHINE_4    = 0xC,
    CANFD_FFI_VIRTUAL_MACHINE_5    = 0xD,
    CANFD_FFI_VIRTUAL_MACHINE_6    = 0xE,
    CANFD_FFI_VIRTUAL_MACHINE_7    = 0xF
} canfd_ffi_virtual_machine_t;                      /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD FFI CFIFO Virtual Machine **/
/* PRQA S 0635 ++ # DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
typedef union u_canfd_ffi_txq_cfifo_virtual_machine /* PRQA S 1536 # Advisory R-2.4 */
{                                                   /* PRQA S 0750 # Advisory R-19.2 */
    uint32_t ffi_txq_cfifo_cfg_u32;
    struct
    {
        uint8_t txq0   : 4;                         ///< TXQ0 of VM
        uint8_t txq1   : 4;                         ///< TXQ1 of VM
        uint8_t txq2   : 4;                         ///< TXQ2 of VM
        uint8_t txq3   : 4;                         ///< TXQ3 of VM
        uint8_t cfifo0 : 4;                         ///< CFIFO 0 of VM
        uint8_t cfifo1 : 4;                         ///< CFIFO 1 of VM
        uint8_t cfifo2 : 4;                         ///< CFIFO 2 of VM
        uint8_t        : 4;                         ///< Bit Reserved
    } ffi_txq_cfifo_virtual_machine_b;
} canfd_ffi_txq_cfifo_virtual_machine_t;            /* PRQA S 1535 # Advisory R-2.3 */
/* PRQA S 0635 -- */

/** CANFD FFI Error Virtual Machine **/
/* PRQA S 0635 ++ # DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
typedef union u_canfd_ffi_error_virtual_machine /* PRQA S 1536 # Advisory R-2.4 */
{                                               /* PRQA S 0750 # Advisory R-19.2 */
    uint32_t ffi_error_cfg_u32;
    struct
    {
        uint8_t fifo_msg_lost  : 1;             ///< FIFO message lost
        uint8_t txq_msg_lost   : 1;             ///< TXQ message lost
        uint8_t thl_entry_lost : 1;             ///< Transmit history list entry lost
        uint8_t                : 1;             ///< Bit Reserved
        uint8_t cfifo_msg_ow   : 1;             ///< Common FIFO message overwrite
        uint8_t txq_msg_ow     : 1;             ///< TXQ message overwrite
    } ffi_error_virtual_machine_b;
} canfd_ffi_error_virtual_machine_t;            /* PRQA S 1535 # Advisory R-2.3 */
/* PRQA S 0635 -- */

/** CANFD Timestamp clock source select */
typedef enum e_canfd_ts_clk_src                 /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_TS_CLK_SRC_PCLK = 0UL,                ///< Peripheral clock
    CANFD_TS_CLK_SRC_CH0  = 1UL,                ///< Channel 0 Bit time clock
    CANFD_TS_CLK_SRC_CH1  = 3UL,                ///< Channel 1 Bit time clock
    CANFD_TS_CLK_SRC_CH2  = 5UL,                ///< Channel 2 Bit time clock
    CANFD_TS_CLK_SRC_CH3  = 7UL,                ///< Channel 3 Bit time clock
    CANFD_TS_CLK_SRC_CH4  = 9UL,                ///< Channel 4 Bit time clock
    CANFD_TS_CLK_SRC_CH5  = 11UL,               ///< Channel 5 Bit time clock
    CANFD_TS_CLK_SRC_CH6  = 13UL,               ///< Channel 6 Bit time clock
    CANFD_TS_CLK_SRC_CH7  = 15UL,               ///< Channel 7 Bit time clock
} canfd_ts_clk_src_t;

/** CANFD Protocol mode */
typedef enum e_canfd_prt_mode                   /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_PRT_MODE_FD      = 0U,                ///< CANFD, Classical CAN Mode
    CANFD_PRT_MODE_FD_ONLY = 1U,                ///< CANFD Only Mode
    CANFD_PRT_MODE_CC_ONLY = 4U                 ///< Classical CAN Only Mode
} canfd_prt_mode_t;

/** CANFD Timestamp clock prescaler select*/
typedef enum e_canfd_ts_clk_div                 /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_TS_CLK_DIV_1 = 0,                     ///< Timestamp Prescaler = 1
    CANFD_TS_CLK_DIV_2,                         ///< Timestamp Prescaler = 2
    CANFD_TS_CLK_DIV_4,                         ///< Timestamp Prescaler = 4
    CANFD_TS_CLK_DIV_8,                         ///< Timestamp Prescaler = 8
    CANFD_TS_CLK_DIV_16,                        ///< Timestamp Prescaler = 16
    CANFD_TS_CLK_DIV_32,                        ///< Timestamp Prescaler = 32
    CANFD_TS_CLK_DIV_64,                        ///< Timestamp Prescaler = 64
    CANFD_TS_CLK_DIV_128,                       ///< Timestamp Prescaler = 128
    CANFD_TS_CLK_DIV_256,                       ///< Timestamp Prescaler = 256
    CANFD_TS_CLK_DIV_512,                       ///< Timestamp Prescaler = 512
    CANFD_TS_CLK_DIV_1024,                      ///< Timestamp Prescaler = 1024
    CANFD_TS_CLK_DIV_2048,                      ///< Timestamp Prescaler = 2048
    CANFD_TS_CLK_DIV_4096,                      ///< Timestamp Prescaler = 4096
    CANFD_TS_CLK_DIV_8192,                      ///< Timestamp Prescaler = 8192
    CANFD_TS_CLK_DIV_16384,                     ///< Timestamp Prescaler = 16384
    CANFD_TS_CLK_DIV_32768,                     ///< Timestamp Prescaler = 32768
} canfd_ts_clk_div_t;

/** Timestamp capture configuration */
typedef enum e_canfd_ts_capture                 /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_TS_CAPTURE_SOF = 0,                   ///< Timestamp capture at the sample point of SOF (start of frame)
    CANFD_TS_CAPTURE_VALID,                     ///< Timestamp capture at frame valid indication
    CANFD_TS_CAPUTURE_RES,                      ///< Timestamp capture at the sample point of RES bit
} canfd_ts_capture_t;

/** CANFD Protocol Exception selection */
typedef enum e_canfd_prt_exception              /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_PRT_EXCEPTION_ENABLE  = 0,            ///< Protocol exception event detection enabled.
    CANFD_PRT_EXCEPTION_DISABLE = 1,            ///< Protocol exception event detection disabled.
} canfd_prt_exception;

/** CANFD Transmit Queue Configuration*/

typedef union u_canfd_tx_queue_cfg           /* PRQA S 1536 # Advisory R-2.4 */
{                                            /* PRQA S 0750 # Advisory R-19.2 */
    uint32_t tx_queue_cfg_u32;
    struct
    {
        uint32_t enable                 : 1; ///< Transmit Queue Enable option
        uint32_t queue_gw_mode_en       : 1; ///< Transmit Queue Gateway Enable option
        uint32_t queue_ow_mode_en       : 1; ///< Transmit Queue Overwrite Enable option
        uint32_t                        : 2; ///< Bit Reserved
        uint32_t interrupt_enable       : 1; ///< Transmit Queue Interrupt Enable option
        uint32_t                        : 1; ///< Bit Reserved
        uint32_t interrupt_select       : 1; ///< Transmit Queue Interrupt source selection
        uint32_t depth                  : 5; ///< Transmit Queue Depth
        uint32_t                        : 3; ///< Bit Reserved
        uint32_t full_isr_mode_en       : 1; ///< Transmit Queue Full Interrupt Enable option
        uint32_t one_frame_rx_isr_en    : 1; ///< Transmit Queue One Frame RX Interrupt Enable option
        uint32_t one_frame_tx_isr_en    : 1; ///< Transmit Queue One Frame TX Interrupt Enable option
        uint32_t                        : 5; ///< Bit Reserved
        uint32_t ffi_txq_lost_er_isr_en : 1; ///< Transmit Queue Message Lost Error Interrupt Enabled
        uint32_t ffi_txq_ow_er_isr_en   : 1; ///< Transmit Queue Message Overwrite Error Interrupt Enabled
        uint32_t                        : 2; ///< Bit Reserved
        uint32_t dma_enable             : 1; ///< DMA Transfer Request Enable option
        uint32_t                        : 3; ///< Bit Reserved
    } tx_queue_cfg_b;
} canfd_tx_queue_cfg_t;

/** CANFD Common Fifo Mode */
typedef enum e_canfd_cfifo_mode        /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_CFIFO_MODE_RECEIVE  = 0x00,  ///< Common FIFO Receive mode
    CANFD_CFIFO_MODE_TRANSMIT = 0x01,  ///< Common FIFO Transmit mode
    CANFD_CFIFO_MODE_GATEWAY  = 0x02,  ///< Common FIFO Gateway mode
} canfd_cfifo_mode_t;

/** CANFD Interval Timer Clock Source */

typedef enum e_canfd_timer_clk                /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_TIMER_CLK_REF_CLK           = 0x00, ///< Interval Timer Clock is reference clock
    CANFD_TIMER_CLK_REF_CLK_X10       = 0x02, ///< Interval Timer Clock is reference clock x10
    CANFD_TIMER_CLK_NOMINAL_BIT_CLOCK = 0x01, ///< Interval Timer Clock is nominal bit time clock of related channel
} canfd_timer_clk_t;

/** CANFD Error Occurrence counting method */
typedef enum e_canfd_eoc_count                /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_EOC_COUNT_ALL_TX_RX       = 0x0,    ///< Error Occurrence count all transmitted and received messages
    CANFD_EOC_COUNT_ALL_TX          = 0x01,   ///< Error Occurrence count all transmitted messages
    CANFD_EOC_COUNT_ALL_RX          = 0x02,   ///< Error Occurrence count all received messages
    CANFD_EOC_COUNT_DATAPHASE_TX_RX = 0x04,   ///< Error Occurrence count only data phase of transmitted and received CAN FD message
    CANFD_EOC_COUNT_DATAPHASE_TX    = 0x05,   ///< Error Occurrence count only data phase of transmitted CAN FD message
    CANFD_EOC_COUNT_DATAPHASE_RX    = 0x06,   ///< Error Occurrence count only data phase of received CAN FD messages
} canfd_eoc_count_t;

/** Error State Indicator (ESI) bit state when in error active state */
typedef enum e_canfd_esi_mode                 /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_ESI_MODE_NODE_STATE   = 0,          ///< ESI bit display node error state
    CANFD_ESI_MODE_BUFFER_STATE = 1,          ///< ESI bit display message buffer error state
} canfd_esi_mode_t;

/**AFLRAM MRAM or PFLRAM (for Pretended Network filter) in RAM Test Mode  */
typedef enum e_canfd_ramtest_field            /* PRQA S 1536 # Advisory R-2.4 */
{
    CAN_RAMTEST_FIELD_AFL_MRAM = 0,           ///< Field RAM of AFL MRAM area
    CAN_RAMTEST_FIELD_PFL                     ///< Field RAM of Prevented Network Filter List area
} canfd_ramtest_field_t;

/**Data which chose to fill into RAM */
typedef enum e_canfd_ramtest_data             /* PRQA S 1536 # Advisory R-2.4 */
{
    CAN_RAMTEST_DATA_0 = 0,                   ///< Fill RAM with logic 0
    CAN_RAMTEST_DATA_1                        ///< Fill RAM with logic 1
} canfd_ramtest_data_t;

/**Data walk type to test RAM */
typedef enum e_canfd_ramtest_walk_type        /* PRQA S 1536 # Advisory R-2.4 */
{
    CAN_RAMTEST_WALK_TYPE_0 = 0,              ///< Walking 0 test pattern
    CAN_RAMTEST_WALK_TYPE_1                   ///< Walking 1 test pattern
} canfd_ramtest_walk_type_t;

/** CANFD RX FIFO Configuration */
typedef union u_canfd_rx_fifo_cfg             /* PRQA S 1536 # Advisory R-2.4 */
{                                             /* PRQA S 0750 # Advisory R-19.2 */
    volatile uint32_t rx_fifo_cfg;
    struct
    {
        volatile uint32_t RFE      : 1;       ///< RX FIFO Enable
        volatile uint32_t RFIE     : 1;       ///< RX FIFO Interrupt Enable
        volatile          uint32_t : 2;       ///< Bit Reserved
        volatile uint32_t RFPLS    : 3;       ///< RX FIFO Payload Data Size configuration
        volatile          uint32_t : 1;       ///< Bit Reserved
        volatile uint32_t RFDC     : 3;       ///< RX FIFO Depth Configuration
        volatile          uint32_t : 1;       ///< Bit Reserved
        volatile uint32_t RFIM     : 1;       ///< RX FIFO Interrupt Mode
        volatile uint32_t RFIGCV   : 3;       ///< RX FIFO Interrupt Generation Counter Value
        volatile uint32_t RFFIE    : 1;       ///< RX FIFO Full interrupt Enable
        volatile          uint32_t : 15;      ///< Bit Reserved
    } rx_fifo_cfg_b;
} canfd_rx_fifo_cfg_t;

/** CANFD Common Fifo Config */
typedef union u_canfd_cfifo_cfg                    /* PRQA S 1536 # Advisory R-2.4 */
{                                                  /* PRQA S 0750 # Advisory R-19.2 */
    uint32_t cfifo_cfg_u32;
    struct
    {
        uint32_t enable        : 1;                ///< Common FIFO Enable
        uint32_t rx_int_enable : 1;                ///< Transmit Interrupt Enable
        uint32_t tx_int_enable : 1;                ///< Receive Interrupt Enable
        uint32_t               : 1;                ///< Bit Reserved
        uint32_t payload_size  : 3;                ///< Common FIFO payload size
        uint32_t               : 1;                ///< Bit Reserved

        /* PRQA S 0634, 0635 ++ # DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        canfd_cfifo_mode_t mode               : 2; ///< Common FIFO mode
        canfd_timer_clk_t  interval_clock_src : 2; ///< Interval Timer clock source select

        /* PRQA S 0634, 0635 -- */
        uint32_t interrupt_select : 1;             ///< Common FIFO interrupt source select
        uint32_t rx_int_threshold : 3;             ///< Common FIFO receive interrupt timing select
        uint32_t txmb_link        : 5;             ///< Transmit buffer link configuration
        uint32_t depth            : 3;             ///< Common FIFO buffer depth
        uint32_t tx_interval      : 8;             ///< Transmission Interval
    } cfifo_cfg_b;
} canfd_cfifo_cfg_t;

/** CANFD Common Fifo Config Advance*/
typedef union u_canfd_cfifo_cfg_enh             /* PRQA S 1536 # Advisory R-2.4 */
{                                               /* PRQA S 0750 # Advisory R-19.2 */
    uint32_t cfifo_cfg_enh_u32;
    struct
    {
        uint32_t                          : 8;  ///< Bit Reserved
        uint32_t cfifo_ow_en              : 1;  ///< Common FIFO overwrite mode
        uint32_t                          : 15; ///< Bit Reserved
        uint32_t ffi_cfifo_lost_er_isr_en : 1;  ///< Error Message Lost Interrupt Enable Of FFI Mode
        uint32_t ffi_cfifo_ow_er_isr_en   : 1;  ///< Error Message Overwrite Interrupt Enable Of FFI Mode
        uint32_t                          : 6;
    } cfifo_cfg_enh_b;
} canfd_cfifo_cfg_enh_t;

typedef struct st_canfd_gateway_cfg                    /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t enable : 1;                               ///< Gateway Enable
    uint32_t frame  : 1;                               ///< Gateway FD Frame
    uint32_t brs    : 1;                               ///< Gateway CANFD Bit Rate Switching
} canfd_gateway_cfg;

/* CAN Instance Control Block   */
typedef struct st_canfd_instance_ctrl                  /* PRQA S 1536 # Advisory R-2.4 */
{
    /* Parameters to control CAN peripheral device */
    can_cfg_t const    * p_cfg;                        // Pointer to the configuration structure
    uint32_t             open;                         // Open status of channel.
    can_operation_mode_t operation_mode;               // Can operation mode.
    can_test_mode_t      test_mode;                    // Can operation mode.
#if defined(BSP_TZ_SECURE_BUILD) && (BSP_TZ_SECURE_BUILD)
    bool callback_is_secure;                           // If the callback is in non-secure memory then a security state transistion is required to call p_callback (BLXNS)
#endif
    void (* p_callback)(can_callback_args_t * p_args); // Pointer to callback
    can_callback_args_t * p_callback_memory;           // Pointer to optional callback argument memory
    void                * p_context;                   // Pointer to context to be passed into callback function
    uint8_t               channel_index;               // CAN channel index.
    uint8_t               txmb_range_min;              // Minimum valid transmit buffer of the channel;
    uint8_t               txmb_range_max;              // Maximum valid transmit buffer of the channel;
    uint8_t               txqueue_range_min;           // Minimum valid transmit queue of the channel;
    uint8_t               txqueue_range_max;           // Maximum valid transmit queue of the channel;
} canfd_instance_ctrl_t;

/** AFL Entry (based on R_CANFD0_CFDGAFL_Type in renesas.h) */
typedef struct st_canfd_afl_entry_t                    /* PRQA S 1536 , 3630 # Advisory R-2.4 , Advisory D-4.8 */
{
    union
    {                                                  /* PRQA S 0750 # Advisory R-19.2 */
        uint32_t id_u32;

        struct
        {
            uint32_t id : 29;                   ///< ID to match against

            /* PRQA S 3660, 0634, 0635 ++ # DP_MSR_R-6.2_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            canfd_rule_target_t target     : 1; ///< Target Message
            can_frame_type_t    frame_type : 1; ///< Frame type (Data or Remote)
            can_id_mode_t       id_mode    : 1; ///< ID mode (Standard or Extended)

            /* PRQA S 3660, 0634, 0635 -- */
        } id_b;                                 /* PRQA S 1710 # Advisory D-4.5 */
    } id;

    union
    {                                           /* PRQA S 0750 # Advisory R-19.2 */
        uint32_t mask_u32;
        struct
        {
            uint32_t mask_id         : 29;      ///< ID Mask
            uint32_t                 : 1;
            uint32_t mask_frame_type : 1;       ///< Only accept frames with the configured frame type
            uint32_t mask_id_mode    : 1;       ///< Only accept frames with the configured ID mode
        } mask_b;
    } mask;

    union
    {                                     /* PRQA S 0750 # Advisory R-19.2 */
        uint32_t destination_u32[2];
        struct
        {
            uint32_t minimum_dlc    : 4;  ///< Minimum DLC value to accept (valid when DLC Check is enabled)
            uint32_t routing_target : 3;  ///< Global Acceptance Filter List Select Routing destination
            uint32_t                : 1;
            uint32_t rx_buffer      : 8;  ///< RX Message Buffer to receive messages accepted by this rule
            uint32_t label          : 16; ///< Receive Rule label
            uint32_t rx_select_flags;     ///< RX FIFO(s) to receive messages accepted by this rule
        } destination_b;
    } destination;
} canfd_afl_entry_t;

/** CANFD Global Timestamp config*/
typedef struct st_canfd_global_ts_cfg  /* PRQA S 1536 # Advisory R-2.4 */
{
    canfd_ts_capture_t capture_cfg;    ///< Timestamp capture config
    canfd_ts_clk_src_t clock_source;   ///< Timestamp clock source
    canfd_ts_clk_div_t clock_div;      ///< Timestamp clock prescaler
} canfd_global_ts_cfg_t;

/*CANFD RAM Test contain value*/
typedef struct st_canfd_ramtest        /* PRQA S 1536 , 3630 # Advisory R-2.4 , Advisory D-4.8 */
{
    uint32_t              ram_max_page;
    uint32_t              unaccessed_ram_entry;
    canfd_ramtest_field_t ram_field;
} canfd_ramtest_t;

/** CANFD TX history config*/
typedef struct st_canfd_tx_history_cfg           /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t enable;                             ///< Transmit History Enable
    uint8_t  isr_mode;                           ///< Transmit History Interrupt Mode
    uint8_t  thl_entry_lost;                     ///< Transmit History List Entry Lost
} canfd_tx_history_cfg_t;

/** Select flexible can channel to connect */
typedef enum e_canfd_flex_can                    /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_FLEX_CAN_CH0_CH1 = (1U << 0),          ///< Channel 0 and 1 are connected and use terminal of Channel 0
    CANFD_FLEX_CAN_CH2_CH3 = (1U << 1),          ///< Channel 2 and 3 are connected and use terminal of Channel 2
    CANFD_FLEX_CAN_CH4_CH5 = (1U << 2),          ///< Channel 4 and 5 are connected and use terminal of Channel 4
    CANFD_FLEX_CAN_CH6_CH7 = (1U << 3),          ///< Channel 6 and 7 are connected and use terminal of Channel 6
} canfd_flex_can_t;                              /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Protocol Exception selection */
typedef enum e_canfd_filter_mode                 /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_FILTER_MODE_AFL = 0,                   ///< Acceptance Filter Mode request
    CANFD_FILTER_MODE_ID  = 1,                   ///< Pretended Network Filter ID only and Acceptance Filter Mode request
    CANFD_FILTER_MODE_MIX = 2,                   ///< Pretended Network Filter and Acceptance Filter Mode request
    CANFD_FILTER_MODE_PNF = 3,                   ///< Pretended Network Filter Mode request
} canfd_filter_mode_t;

/** CANFD PNF comparison conditions of the filter */
typedef enum e_canfd_pnf_filter_compare          /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_PNF_FILTER_COMPARE_MATCH = 0,          ///< Payload data match filter
    CANFD_PNF_FILTER_COMPARE_RANGE = 1,          ///< Compare upper / lower filter
} canfd_pnf_filter_compare_t;

/** CANFD PNF filter conditions of upper / lower filter */
typedef enum e_canfd_pnf_filter_range            /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_PNF_FILTER_RANGE_WITHIN  = 0,          ///< Within the range of upper limit and lower limi
    CANFD_PNF_FILTER_RANGE_OUTSIDE = 1,          ///< Outside of the range of upper limit and lower limit
} canfd_pnf_filter_range_t;

/** CANFD PNF filter conditions of the filters */
typedef enum e_canfd_pnf_filter_condition        /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_PNF_FILTER_CONDITION_BOTH_SUCCESS = 0, ///< Both of filters 0 and 1 are successful
    CANFD_PNF_FILTER_CONDITION_ONE_SUCCESS  = 1, ///< One of the filter 0 or 1 is successful
} canfd_pnf_filter_condition_t;

/** CANFD Transmit Buffer Type of TX History List */
typedef enum e_canfd_thl_buff_type               /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_THL_BUFF_TYPE_TXMB    = (1U << 0),     ///< Flat TX Message Buffer
    CANFD_THL_BUFF_TYPE_TXFIFO  = (1U << 1),     ///< TX FIFO MB Number and GW FIFO MB Number
    CANFD_THL_BUFF_TYPE_TXQUEUE = (1U << 2),     ///< TX Queue MB Number
} canfd_thl_buff_type_t;                         /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Transmit History Interrupt Type */
typedef enum e_canfd_thl_isr_mode                /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_THL_ISR_MODE_THRESHOLD = 0,            ///< Interrupt generated if TX History List level reaches three-quarters of the TX History List depth
    CANFD_THL_ISR_MODE_EVERYTIME = 1,            ///< Interrupt generated for every successfully stored entry.
} canfd_thl_isr_mode_t;                          /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Transmit History List Dedicated */
typedef enum e_canfd_thl_dedicate                /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_THL_DEDICATED_TX = (1U << 0),          ///< TX History List Dedicated TX Enable
    CANFD_THL_DEDICATED_GW = (1U << 1),          ///< TX History List Dedicated GW Enable
} canfd_thl_dedicate_t;                          /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Transmit History List Dedicated */
typedef enum e_canfd_thl_entry_lost              /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_THL_ENTRY_LOST_DISABLE = 0,            ///< TX History List Entry Message Lost Disable
    CANFD_THL_ENTRY_LOST_ENABLE  = 1,            ///< TX History List Entry Message Lost Enable
} canfd_thl_entry_lost_t;                        /* PRQA S 1535 # Advisory R-2.3 */

/** Select channel to connect to can hub*/
typedef enum e_canfd_can_hub                     /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_CAN_HUB_CH0 = ((uint32_t) 1U << 16),   ///< Channel 0 connect to CAN HUB
    CANFD_CAN_HUB_CH1 = ((uint32_t) 1U << 17),   ///< Channel 1 connect to CAN HUB
    CANFD_CAN_HUB_CH2 = ((uint32_t) 1U << 18),   ///< Channel 2 connect to CAN HUB
    CANFD_CAN_HUB_CH3 = ((uint32_t) 1U << 19),   ///< Channel 3 connect to CAN HUB
    CANFD_CAN_HUB_CH4 = ((uint32_t) 1U << 20),   ///< Channel 4 connect to CAN HUB
    CANFD_CAN_HUB_CH5 = ((uint32_t) 1U << 21),   ///< Channel 5 connect to CAN HUB
    CANFD_CAN_HUB_CH6 = ((uint32_t) 1U << 22),   ///< Channel 6 connect to CAN HUB
    CANFD_CAN_HUB_CH7 = ((uint32_t) 1U << 23),   ///< Channel 7 connect to CAN HUB
} canfd_can_hub_t;                               /* PRQA S 1535 # Advisory R-2.3 */

/** Select flexible buffer number even channel rent odd channel */
typedef enum e_canfd_flex_buffer_lend            /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_FLEX_BUFFER_LEND_0  = 0,               ///< Disable flexible buffer assignment mode
    CANFD_FLEX_BUFFER_LEND_4  = 1,               ///< Even channel rent 4 buffer next odd channel
    CANFD_FLEX_BUFFER_LEND_8  = 2,               ///< Even channel rent 8 buffer next odd channel
    CANFD_FLEX_BUFFER_LEND_12 = 3,               ///< Even channel rent 12 buffer next odd channel
    CANFD_FLEX_BUFFER_LEND_16 = 4,               ///< Even channel rent 16 buffer next odd channel
    CANFD_FLEX_BUFFER_LEND_20 = 5,               ///< Even channel rent 20 buffer next odd channel
    CANFD_FLEX_BUFFER_LEND_24 = 6,               ///< Even channel rent 24 buffer next odd channel
    CANFD_FLEX_BUFFER_LEND_28 = 7,               ///< Even channel rent 28 buffer next odd channel
    CANFD_FLEX_BUFFER_LEND_32 = 8,               ///< Even channel rent 32 buffer next odd channel
} canfd_flex_buffer_lend_t;

/** Select the output destination for the FFI error interrupt */
typedef enum e_canfd_ffi_err                     /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_FFI_ERR_GLOBAL = 0,                    ///< Select Global error interrupt
    CANFD_FFI_ERR_VM     = 1,                    ///< Select VM error interrupt
} canfd_ffi_err_t;                               /* PRQA S 1535 # Advisory R-2.3 */

typedef enum e_canfd_transfer_buffer             /* PRQA S 1536 # Advisory R-2.4 */
{
    CANFD_TRANSFER_BUFFER_RX_FIFO_0     = 0,
    CANFD_TRANSFER_BUFFER_RX_FIFO_1     = 1,
    CANFD_TRANSFER_BUFFER_RX_FIFO_2     = 2,
    CANFD_TRANSFER_BUFFER_RX_FIFO_3     = 3,
    CANFD_TRANSFER_BUFFER_RX_FIFO_4     = 4,
    CANFD_TRANSFER_BUFFER_RX_FIFO_5     = 5,
    CANFD_TRANSFER_BUFFER_RX_FIFO_6     = 6,
    CANFD_TRANSFER_BUFFER_RX_FIFO_7     = 7,
    CANFD_TRANSFER_BUFFER_COMMON_FIFO_0 = 8,
    CANFD_TRANSFER_BUFFER_TXQ_0         = 9,
    CANFD_TRANSFER_BUFFER_TXQ_3         = 10,
    CANFD_TRANSFER_BUFFER_COMMON_FIFO_2 = 11,
} canfd_transfer_buffer_t;                      /* PRQA S 1535 # Advisory R-2.3 */

/** PNF Entry (based on R_CANFD0_CFDGPFL_Type in renesas.h) */
typedef struct st_canfd_pnf_entry_t             /* PRQA S 1536, 3630 # Advisory R-2.4 , Advisory D-4.8 */
{
    union
    {                                           /* PRQA S 0750 # Advisory R-19.2 */
        uint32_t id_u32;
        struct
        {
            uint32_t id : 29;                   ///< ID to match against

            /* PRQA S 3660, 0634, 0635 ++ # DP_MSR_R-6.2_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            canfd_rule_target_t target     : 1; ///< Target Message
            can_frame_type_t    frame_type : 1; ///< Frame type (Data or Remote)
            can_id_mode_t       id_mode    : 1; ///< ID mode (Standard or Extended)

            /* PRQA S 3660, 0634, 0635 -- */
        } id_b;
    } id;

    union
    {                                      /* PRQA S 0750 # Advisory R-19.2 */
        uint32_t mask_u32;
        struct
        {
            uint32_t mask_id         : 29; ///< ID Mask
            uint32_t                 : 1;
            uint32_t mask_frame_type : 1;  ///< Only accept frames with the configured frame type
            uint32_t mask_id_mode    : 1;  ///< Only accept frames with the configured ID mode
        } mask_b;
    } mask;

    union
    {                                     /* PRQA S 0750 # Advisory R-19.2 */
        uint32_t destination_u32[2];
        struct
        {
            uint32_t minimum_dlc    : 4;  ///< Minimum DLC value to accept (valid when DLC Check is enabled)
            uint32_t routing_target : 3;  ///< Global Acceptance Filter List Select Routing destination
            uint32_t                : 1;
            uint32_t rx_buffer      : 8;  ///< RX Message Buffer to receive messages accepted by this rule
            uint32_t label          : 16; ///< Receive Rule label
            uint32_t rx_select_flags;     ///< RX FIFO(s) to receive messages accepted by this rule
        } destination_b;
    } destination;

    union
    {                                                           /* PRQA S 0750 # Advisory R-19.2 */
        uint32_t payload_type_u32;
        struct
        {
            uint32_t offset_filter_1 : 4;                       ///< Filter offset value of the filter1 (Offset can be specified per 4 bytes.)
            uint32_t                 : 9;

            /* PRQA S 3660, 0634, 0635 ++ # DP_MSR_R-6.2_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            canfd_pnf_filter_range_t   range_filter_1   : 1;    ///< The conditions of upper / lower filter of the filter1 (Filter conditions of upper / lower filter of the filter1)
            canfd_pnf_filter_compare_t compare_filter_1 : 1;    ///< The comparison conditions of the filter1 (Filter comparison conditions of the filter1)

            /* PRQA S 3660, 0634, 0635 -- */
            uint32_t                 : 1;
            uint32_t offset_filter_0 : 4;                       ///< Filter offset value of the filter1 (Offset can be specified per 4 bytes.)
            uint32_t                 : 9;

            /* PRQA S 3660, 0634, 0635 ++ # DP_MSR_R-6.2_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 , DP_MSR_R-6.1_UC1 & DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            canfd_pnf_filter_range_t     range_filter_0    : 1; ///< The conditions of upper / lower filter of the filter0 (Filter conditions of upper / lower filter of the filter0)
            canfd_pnf_filter_compare_t   compare_filter_0  : 1; ///< The comparison conditions of the filter0 (Filter comparison conditions of the filter0)
            canfd_pnf_filter_condition_t match_filter_mode : 1; ///< Condition of the filters 0 and 1 (One of the filter or Both are successful)

            /* PRQA S 3660, 0634, 0635 -- */
        } payload_type_b;
    } payload_type;

    union
    {                                                        /* PRQA S 0750 # Advisory R-19.2 */
        uint32_t payload_compare_u32[4];
        struct
        {
            uint8_t data0_value[CANFD_PNF_DATA_PER_REG_NUM]; ///< Data 0 for comparision or Upper Limit value when comparison condition is upper / lower filter
            uint8_t data0_mask[CANFD_PNF_DATA_PER_REG_NUM];  ///< Mask data 0  comparision or Lower Limit value when comparison condition is upper / lower filter
            uint8_t data1_value[CANFD_PNF_DATA_PER_REG_NUM]; ///< Data 1 for comparision or Upper Limit value when comparison condition is upper / lower filter
            uint8_t data1_mask[CANFD_PNF_DATA_PER_REG_NUM];  ///< Mask data 1  comparision or Lower Limit value when comparison condition is upper / lower filter
        } payload_compare_b;
    } payload_compare;
} canfd_pnf_entry_t;

/** CANFD RX FIFO Data Structure */
typedef struct st_canfd_rx_fifo_data         /* PRQA S 1536 # Advisory R-2.4 */
{
    union
    {                                        /* PRQA S 0750 # Advisory R-19.2 */
        volatile uint32_t ID;                ///< RX FIFO Access ID Register

        struct
        {
            volatile uint32_t RFID     : 29; ///< RX FIFO Buffer ID FieldSTD-ID / EXT-ID fields
            volatile          uint32_t : 1;
            volatile uint32_t RFRTR    : 1;  ///< RX FIFO Buffer RTR Bit0B: Data Frame1B: Remote Frame
            volatile uint32_t RFIDE    : 1;  ///< RX FIFO Buffer IDE Bit0B: STD-ID has been received1B: EXT-ID has been received
        } ID_b;
    };                                       /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    union
    {                                        /* PRQA S 0750 # Advisory R-19.2 */
        /* PRQA S 1710 ++ # Advisory D-4.5 */
        volatile uint32_t PTR;               ///< RX FIFO Access Pointer Register

        /* PRQA S 1710 -- */

        struct
        {
            volatile uint32_t RFTS     : 16; ///< RX FIFO Timestamp ValueTimestamp value of the received CAN Frame
            volatile          uint32_t : 12;
            volatile uint32_t RFDLC    : 4;  ///< RX FIFO Buffer DLC FieldNo. of Data Bytes received in a CAN Frame
        } PTR_b;
    };                                       /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    union
    {                                        /* PRQA S 0750 # Advisory R-19.2 */
        /* PRQA S 1710 ++ # Advisory D-4.5 */
        volatile uint32_t FDSTS;             ///< RX FIFO Access CAN-FD Status Register

        /* PRQA S 1710 -- */

        struct
        {
            volatile uint32_t RFESI    : 1;  ///< Error State Indicator
            volatile uint32_t RFBRS    : 1;  ///< Bit Rate Switch
            volatile uint32_t RFFDF    : 1;  ///< CANFD Format
            volatile          uint32_t : 5;
            volatile uint32_t RFIFL    : 2;  ///< RX FIFO Buffer Information label FieldRX FIFO Buffer Information Label
            volatile          uint32_t : 6;
            volatile uint32_t RFPTR    : 16; ///< RX FIFO Buffer Pointer FieldFIFO Buffer Pointer
        } FDSTS_b;
    };                                       /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    volatile uint32_t DF[16];                ///< Receive FIFO Buffer Access Data Field Register
} canfd_rx_fifo_data_t;                      /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Common FIFO Data Structure */
typedef struct st_canfd_cfifo_data           /* PRQA S 1536 # Advisory R-2.4 */
{
    union
    {                                        /* PRQA S 0750 # Advisory R-19.2 */
        volatile uint32_t ID;                ///< Common FIFO Access ID Register

        struct
        {
            volatile uint32_t CFID  : 29;    ///< Transmit/Receive FIFO Buffer ID FieldSTD-ID / EXT-ID fields
            volatile uint32_t THLEN : 1;     ///< THL Entry enableTX FIFO Mode
            volatile uint32_t CFRTR : 1;     ///< Transmit/Receive FIFO Buffer RTR
            volatile uint32_t CFIDE : 1;     ///< Transmit/Receive FIFO Buffer IDE
        } ID_b;
    };                                       /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    union
    {                                        /* PRQA S 0750 # Advisory R-19.2 */
        volatile uint32_t PTR;               ///< Common FIFO Access Pointer Register

        struct
        {
            volatile uint32_t CFTS     : 16; ///< Transmit/Receive FIFO TimestampValue Timestamp value of the received CAN Frame (FIFO in RX Mode)
            volatile          uint32_t : 12;
            volatile uint32_t CFDLC    : 4;  ///< Transmit/Receive FIFO Buffer DLC FieldNo. of Data Bytes received in a CAN Frame, or to be transmitted in a CAN Frame
        } PTR_b;
    };                                       /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    union
    {                                        /* PRQA S 0750 # Advisory R-19.2 */
        volatile uint32_t FDCSTS;            ///< Common FIFO Access CAN-FD Control/Status Register

        struct
        {
            volatile uint32_t CFESI    : 1;  ///< Error State Indicator
            volatile uint32_t CFBRS    : 1;  ///< Bit Rate Switch
            volatile uint32_t CFFDF    : 1;  ///< CANFD Format
            volatile          uint32_t : 5;
            volatile uint32_t CFIFL    : 2;  ///< COMMON FIFO Buffer Information label Field COMMON FIFO Buffer Information Label
            volatile          uint32_t : 6;
            volatile uint32_t CFPTR    : 16; ///< Transmit/Receive FIFO Buffer Pointer FieldFIFO Message Buffer Pointer
        } FDCSTS_b;
    };                                       /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    volatile uint32_t DF[16];                ///< Common FIFO Access Data Field Register
} canfd_cfifo_data_t;                        /* PRQA S 1535 # Advisory R-2.3 */

/** CANFD Transmit Data Structure */
typedef struct st_canfd_tx_data              /* PRQA S 1536 # Advisory R-2.4 */
{
    union
    {                                        /* PRQA S 0750 # Advisory R-19.2 */
        volatile uint32_t ID;                ///< TX Message Buffer ID Register

        struct
        {
            volatile uint32_t TMID  : 29;    ///< TX Message Buffer ID FieldSTD-ID / EXT-ID fields
            volatile uint32_t THLEN : 1;     ///< TX History List Entry
            volatile uint32_t TMRTR : 1;     ///< TMRTR TX Message Buffer RTR
            volatile uint32_t TMIDE : 1;     ///< TX Message Buffer IDE
        } ID_b;
    };                                       /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    union
    {                                        /* PRQA S 0750 # Advisory R-19.2 */
        volatile uint32_t PTR;               ///< TX Message Buffer Pointer Register

        struct
        {
            volatile          uint32_t : 28;
            volatile uint32_t TMDLC    : 4; ///< TX Message Buffer DLC FieldNo. of Data Bytes to be transmitted in a CAN Frame
        } PTR_b;
    };                                      /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    union
    {                                       /* PRQA S 0750 # Advisory R-19.2 */
        /* PRQA S 1710 ++ # Advisory D-4.5 */
        volatile uint32_t FDCTR;            ///< TX Message Buffer CAN-FD Control Register

        /* PRQA S 1710 -- */

        struct
        {
            volatile uint32_t TMESI    : 1;  ///< Error State Indicator
            volatile uint32_t TMBRS    : 1;  ///< Bit Rate Switch
            volatile uint32_t TMFDF    : 1;  ///< CANFD Format
            volatile          uint32_t : 5;
            volatile uint32_t TMIFL    : 2;  ///< TMIFL TX Message Buffer Information Label FieldTX Message Buffer Information Label
            volatile          uint32_t : 6;
            volatile uint32_t TMPTR    : 16; ///< TX Message Buffer Pointer FieldTX Message Buffer Pointer
        } FDCTR_b;
    };                                       /* PRQA S 0660 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    volatile uint32_t DF[16];                ///< TX Message Buffer Data Field Register
} canfd_tx_data_t;                           /* PRQA S 1535 # Advisory R-2.3 */

#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE        /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */

/** CANFD Transfer Configuration */
typedef struct st_canfd_transfer_cfg         /* PRQA S 1536 # Advisory R-2.4 */
{
    transfer_instance_t   * p_transfer;      ///< Transfer instance for Message Buffers Transfer. Set to NULL if unused.
    void * volatile         p_dest;          ///< Destination memory address for transfer operation.
    void * volatile         p_src;           ///< Source memory address for transfer operation.
    canfd_transfer_buffer_t type;            ///< Type of CANFD message buffer
    int16_t                 front;           ///< Front Pointer for Queue Table
    int16_t                 rear;            ///< Rear Pointer for Queue Table

/* PRQA S 1710 ++ # Advisory D-4.5 */
    int16_t size;                            ///< Size of Queue Table

/* PRQA S 1710 -- */
} canfd_transfer_cfg_t;
#endif

/** CANFD Global Configuration */
typedef struct st_canfd_global_cfg                                                                 /* PRQA S 1536 # Advisory R-2.4 */
{
    volatile R_CANFD0_Type * p_canfd_reg;                                                          ///< Base address of the CANFD register block
    uint8_t               unit;                                                                    ///< CANFD Unit number
    uint8_t               channel_error;                                                           ///< Channel to notify global error
    uint32_t              global_interrupts;                                                       ///< Global control options (CFDGCTR register setting)
    uint32_t              global_config;                                                           ///< Global configuration options (CFDGCFG register setting)
    canfd_global_ts_cfg_t timestamp_config;                                                        ///< Timestamp configuration

    canfd_rx_fifo_cfg_t rx_fifo_config[BSP_FEATURE_CANFD_NUM_RXFIFO];                              ///< RX FIFO configuration (CFDRFCCn register settings)
#ifdef BSP_FEATURE_CANFD_FFI_MODE
    canfd_ffi_mode_options_t              canfd_ffi_mode;                                          ///< Freedom from interference
    uint32_t                              ffi_fifo_vm;                                             ///< Receive FIFO Virtual Machine
    canfd_ffi_txq_cfifo_virtual_machine_t ffi_txq_cfifo_vm[CANFD_MAX_CHANNEL_PER_UNIT];            ///< TxQ Common FIFO Virtual Machine
    canfd_ffi_error_virtual_machine_t     ffi_err_vm;                                              ///< Error interrupt Virtual Machine
#endif                                                                                             ///< TXQ and Common FIFO VM
    uint32_t                 rx_fifo_cb_channel[BSP_FEATURE_CANFD_NUM_RXFIFO];                     ///< Notified channel for RX FIFO
    canfd_cfifo_cfg_t        common_fifo_config[BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT];             ///< Common Fifo configuration (CFDCFCC register settings)
    canfd_cfifo_cfg_enh_t    common_fifo_config_enhancement[BSP_FEATURE_CANFD_NUM_CFIFO_PER_UNIT]; ///< Common FIFO Configuration Enhancements (CFDCFCCE register settings)
    uint32_t                 rx_mb_config;                                                         ///< Number and size of RX Message Buffers (CFDRMNB register setting)
    canfd_flex_buffer_lend_t flex_tx_buffer[CANFD_NUM_CHANNEL_PER_UNIT >> 1];                      ///< Number of lend TXMB in flexible buffer mode
    uint8_t             flexible_mode;                                                             ///< Enable flexible CAN mode
    uint32_t            can_hub_mode;                                                              ///< Enable CAN HUB mode
    canfd_prt_exception prt_exception;                                                             ///< Protocol Exception event
    uint8_t             global_err_ipl;                                                            ///< Global Error interrupt priority
    uint8_t             rx_fifo_ipl;                                                               ///< RX FIFO interrupt priority
    canfd_ramtest_t   * p_ram_infor;                                                               ///< Contain ram information
#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE                                                              /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    canfd_transfer_cfg_t transfer_rx_fifo_cfg[BSP_FEATURE_CANFD_NUM_RXFIFO];                       ///< Transfer RX FIFO Configuration
    canfd_transfer_cfg_t transfer_flex_tx_cfg[CANFD_NUM_CHANNEL_PER_UNIT >> 1];                    ///< Transfer Flexible Buffer Configuration
    uint8_t              status_transfer_cfg;                                                      ///< Status of Transfer Configuration
#endif
    uint8_t   afl_rule_num[CANFD_MAX_CHANNEL_PER_UNIT];                                            ///< Number of receive rule exclusively used for each channel
    uint8_t   pnf_rule_num[CANFD_MAX_CHANNEL_PER_UNIT];                                            ///< Number of prevent network filter entry
    IRQn_Type err_glb_irq;                                                                         ///< Global Error IRQ number
    IRQn_Type rx_fifo_irq;                                                                         ///< Receive FIFO IRQ number
} canfd_global_cfg_t;

/** CANFD Extended Configuration */
typedef struct st_canfd_extended_cfg                                                               /* PRQA S 1536 # Advisory R-2.4 */
{
    canfd_afl_entry_t const * p_afl;                                                               ///< AFL rules list
    canfd_pnf_entry_t const * p_pnf;                                                               ///< PNF rules list
    uint32_t               txmb_txi_enable[CANFD_TXMB_INT_REG_MAX];                                ///< Array of TX Message Buffer enable bits
    uint32_t               error_interrupts;                                                       ///< Error interrupt enable bits
    can_bit_timing_cfg_t * p_data_timing;                                                          ///< FD Data Rate (when bitrate switching is used)
    uint8_t                delay_compensation;                                                     ///< FD Transceiver Delay Compensation (enable or disable)
    canfd_tx_queue_cfg_t   tx_queue_cfg[CANFD_TX_QUEUE_NUM];                                       ///< Transmit queue configuration
    canfd_gateway_cfg      gateway_cfg;                                                            ///< Gateway mode configuration
    canfd_eoc_count_t      eoc_count_method;                                                       ///< Error Occurrence count method
    canfd_esi_mode_t       esi_mode;                                                               ///< Error state indicator mode
    canfd_global_cfg_t   * p_global_cfg;                                                           ///< Global configuration (global error callback channel only)
    canfd_filter_mode_t    pnf_mode;                                                               ///< Pretended Network Filter Mode Control
    canfd_tx_history_cfg_t tx_history_cfg;                                                         ///< Transmit History configuration
    uint8_t                tx_padding_data;                                                        ///< Value inserted by the controller when the DLC requires more payload bytes
    canfd_prt_mode_t       prt_mode;                                                               ///< Protocol mode
#if CANFD_CFG_TRANSFER_SUPPORT_ENABLE                                                              /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    canfd_transfer_cfg_t * p_transfer_cfg[CANFD_CHANNEL_MAX_TRANSFER_CFG];                         ///< Transfer CFIFO/TXQ FIFO Configuration
#endif
#if BSP_FEATURE_DEVICE_HAS_INTSEL_IRQ
    bsp_intsel_t intsel_irq_cfg;                                                                   ///< Interrupt source
#endif
#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
    bsp_ored_t tx_ored_int;                                                                        ///< ORed Interrupt source for transmit events
    bsp_ored_t rx_ored_int;                                                                        ///< ORed Interrupt source for receive events
    bsp_ored_t err_ored_int;                                                                       ///< ORed Interrupt source for error events
    bsp_ored_t vmtx_ored_int;                                                                      ///< ORed Interrupt source for virtual machine transmit events
    bsp_ored_t vmrx_ored_int;                                                                      ///< ORed Interrupt source for virtual machine receive events
    bsp_ored_t vmerr_ored_int;                                                                     ///< ORed Interrupt source for virtual machine error events
#endif
} canfd_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const can_api_t g_can_on_canfd;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_CANFD_Open(can_ctrl_t * const p_api_ctrl, can_cfg_t const * const p_cfg);
fsp_err_t R_CANFD_Close(can_ctrl_t * const p_api_ctrl);
fsp_err_t R_CANFD_Write(can_ctrl_t * const p_api_ctrl, can_buffer_t buffer, can_frame_t * const p_frame);
fsp_err_t R_CANFD_Read(can_ctrl_t * const p_api_ctrl, can_buffer_t buffer, can_frame_t * const p_frame);

fsp_err_t R_CANFD_ModeTransition(can_ctrl_t * const   p_api_ctrl,     /* PRQA S 1710 # Advisory D-4.5 */
                                 can_operation_mode_t operation_mode, /* PRQA S 1710 # Advisory D-4.5 */
                                 can_test_mode_t      test_mode);

fsp_err_t R_CANFD_InfoGet(can_ctrl_t * const p_api_ctrl, can_info_t * const p_info);
fsp_err_t R_CANFD_CallbackSet(can_ctrl_t * const          p_api_ctrl,
                              void (                    * p_callback)(can_callback_args_t * p_args),
                              void * const                p_context,
                              can_callback_args_t * const p_callback_memory);
fsp_err_t R_CANFD_StartBusIdleCnt(can_ctrl_t * const p_api_ctrl);
fsp_err_t R_CANFD_StopBusIdleCnt(can_ctrl_t * const p_api_ctrl, uint32_t * p_idle_time_ns);
fsp_err_t R_CANFD_PNFTransition(can_ctrl_t * const p_api_ctrl); /* PRQA S 1753 # DP_MSR_R-8.6_UC1 */
fsp_err_t R_CANFD_RAMTest(can_ctrl_t * const      p_api_ctrl,
                          can_cfg_t const * const p_cfg,
                          canfd_ramtest_field_t   ram_test_field,
                          uint32_t                page_id);

/*******************************************************************************************************************//**
 * @} (end defgroup CAN)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_CANFD_H */
