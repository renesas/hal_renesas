/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_CAN_API_H
#define R_CAN_API_H

/*******************************************************************************************************************//**
 * @ingroup RENESAS_CONNECTIVITY_INTERFACES
 * @defgroup CAN_API CAN Interface
 * @brief Interface for CAN peripheral
 *
 * @section CAN_INTERFACE_SUMMARY Summary
 * The CAN interface provides common APIs for CAN HAL drivers. CAN interface supports following features.
 * - Full-duplex CAN communication
 * - Generic CAN parameter setting
 * - Interrupt driven transmit/receive processing
 * - Callback function support with returning event code
 * - Hardware resource locking during a transaction
 * - Frame settings
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Includes board and MCU related header files. */
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if BSP_FEATURE_CANXL_NUM_CHANNELS
 #define CAN_DATA_BUFFER_LENGTH    (2048)
#elif BSP_FEATURE_CANFD_NUM_CHANNELS
 #define CAN_DATA_BUFFER_LENGTH    (64)
#else
 #define CAN_DATA_BUFFER_LENGTH    (8)
#endif

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

#ifndef BSP_OVERRIDE_CAN_EVENT_T

/** CAN event codes */
typedef enum e_can_event
{
    CAN_EVENT_ERR_WARNING          = 0x0002, ///< Error Warning event.
    CAN_EVENT_ERR_PASSIVE          = 0x0004, ///< Error Passive event.
    CAN_EVENT_ERR_BUS_OFF          = 0x0008, ///< Bus Off event.
    CAN_EVENT_BUS_RECOVERY         = 0x0010, ///< Bus Off Recovery event.
    CAN_EVENT_MAILBOX_MESSAGE_LOST = 0x0020, ///< Mailbox has been overrun.
    CAN_EVENT_ERR_BUS_LOCK         = 0x0080, ///< Bus lock detected (32 consecutive dominant bits).
    CAN_EVENT_ERR_CHANNEL          = 0x0100, ///< Channel error has occurred.
    CAN_EVENT_TX_ABORTED           = 0x0200, ///< Transmit abort event.
    CAN_EVENT_RX_COMPLETE          = 0x0400, ///< Receive complete event.
    CAN_EVENT_TX_COMPLETE          = 0x0800, ///< Transmit complete event.
    CAN_EVENT_ERR_GLOBAL           = 0x1000, ///< Global error has occurred.
    CAN_EVENT_TX_FIFO_EMPTY        = 0x2000, ///< Transmit FIFO is empty.
    CAN_EVENT_FIFO_MESSAGE_LOST    = 0x4000, ///< Receive FIFO overrun.
} can_event_t;

#endif

/** CAN Operation modes */
typedef enum e_can_operation_mode
{
    CAN_OPERATION_MODE_NORMAL = 0,              ///< CAN Normal Operation Mode
    CAN_OPERATION_MODE_RESET,                   ///< CAN Reset Operation Mode
    CAN_OPERATION_MODE_HALT,                    ///< CAN Halt Operation Mode
    CAN_OPERATION_MODE_SLEEP            = 5,    ///< CAN Sleep Operation Mode
    CAN_OPERATION_MODE_GLOBAL_OPERATION = 0x80, ///< CANFD Global Operation Mode
    CAN_OPERATION_MODE_GLOBAL_RESET,            ///< CANFD Global Reset Mode
    CAN_OPERATION_MODE_GLOBAL_HALT,             ///< CANFD Global Halt Mode
    CAN_OPERATION_MODE_GLOBAL_SLEEP = 0x85      ///< CANFD Global Sleep Mode
} can_operation_mode_t;

/** CAN Test modes */
typedef enum e_can_test_mode
{
    CAN_TEST_MODE_DISABLED          = 0,   ///< CAN Test Mode Disabled.
    CAN_TEST_MODE_LISTEN            = 3,   ///< CAN Test Listen Mode.
    CAN_TEST_MODE_LOOPBACK_EXTERNAL = 5,   ///< CAN Test External Loopback Mode.
    CAN_TEST_MODE_LOOPBACK_INTERNAL = 7,   ///< CAN Test Internal Loopback Mode.
    CAN_TEST_MODE_INTERNAL_BUS      = 0x80 ///< CANFD Internal CAN Bus Communication Test Mode.
} can_test_mode_t;

/** CAN Transmit Buffer Type */
typedef enum e_can_buffer_type
{
    CAN_BUFFER_TYPE_TX_SINGLE   = 0x0000, ///< Transmit message buffer. Buffer contain single frame.
    CAN_BUFFER_TYPE_TX_FIFO     = 0x0100, ///< Transmit FIFO-base queue. Buffer contain multiple frames and transmit base on First-in First-out.
    CAN_BUFFER_TYPE_TX_PRIORITY = 0x0200, ///< Transmit Priority-base queue. Buffer contain multiple frames and transmit base on ID priority.
    CAN_BUFFER_TYPE_RX_SINGLE   = 0x1000, ///< Receive message buffer. Buffer contain single frame.
    CAN_BUFFER_TYPE_RX_FIFO     = 0x1100, ///< Receive FIFO-base queue. Buffer contain multiple frames base on First-in First-out.
} can_buffer_type_t;

/** CAN Buffer (TX MB + TX QUEUE) */
typedef enum e_can_buffer
{
    CAN_BUFFER_TX_SINGLE_0  = CAN_BUFFER_TYPE_TX_SINGLE + 0,    ///< CAN Transmit Single Buffer 0
    CAN_BUFFER_TX_SINGLE_1  = CAN_BUFFER_TYPE_TX_SINGLE + 1,    ///< CAN Transmit Single Buffer 1
    CAN_BUFFER_TX_SINGLE_2  = CAN_BUFFER_TYPE_TX_SINGLE + 2,    ///< CAN Transmit Single Buffer 2
    CAN_BUFFER_TX_SINGLE_3  = CAN_BUFFER_TYPE_TX_SINGLE + 3,    ///< CAN Transmit Single Buffer 3
    CAN_BUFFER_TX_SINGLE_4  = CAN_BUFFER_TYPE_TX_SINGLE + 4,    ///< CAN Transmit Single Buffer 4
    CAN_BUFFER_TX_SINGLE_5  = CAN_BUFFER_TYPE_TX_SINGLE + 5,    ///< CAN Transmit Single Buffer 5
    CAN_BUFFER_TX_SINGLE_6  = CAN_BUFFER_TYPE_TX_SINGLE + 6,    ///< CAN Transmit Single Buffer 6
    CAN_BUFFER_TX_SINGLE_7  = CAN_BUFFER_TYPE_TX_SINGLE + 7,    ///< CAN Transmit Single Buffer 7
    CAN_BUFFER_TX_SINGLE_8  = CAN_BUFFER_TYPE_TX_SINGLE + 8,    ///< CAN Transmit Single Buffer 8
    CAN_BUFFER_TX_SINGLE_9  = CAN_BUFFER_TYPE_TX_SINGLE + 9,    ///< CAN Transmit Single Buffer 9
    CAN_BUFFER_TX_SINGLE_10 = CAN_BUFFER_TYPE_TX_SINGLE + 10,   ///< CAN Transmit Single Buffer 10
    CAN_BUFFER_TX_SINGLE_11 = CAN_BUFFER_TYPE_TX_SINGLE + 11,   ///< CAN Transmit Single Buffer 11
    CAN_BUFFER_TX_SINGLE_12 = CAN_BUFFER_TYPE_TX_SINGLE + 12,   ///< CAN Transmit Single Buffer 12
    CAN_BUFFER_TX_SINGLE_13 = CAN_BUFFER_TYPE_TX_SINGLE + 13,   ///< CAN Transmit Single Buffer 13
    CAN_BUFFER_TX_SINGLE_14 = CAN_BUFFER_TYPE_TX_SINGLE + 14,   ///< CAN Transmit Single Buffer 14
    CAN_BUFFER_TX_SINGLE_15 = CAN_BUFFER_TYPE_TX_SINGLE + 15,   ///< CAN Transmit Single Buffer 15
    CAN_BUFFER_TX_SINGLE_16 = CAN_BUFFER_TYPE_TX_SINGLE + 16,   ///< CAN Transmit Single Buffer 16
    CAN_BUFFER_TX_SINGLE_17 = CAN_BUFFER_TYPE_TX_SINGLE + 17,   ///< CAN Transmit Single Buffer 17
    CAN_BUFFER_TX_SINGLE_18 = CAN_BUFFER_TYPE_TX_SINGLE + 18,   ///< CAN Transmit Single Buffer 18
    CAN_BUFFER_TX_SINGLE_19 = CAN_BUFFER_TYPE_TX_SINGLE + 19,   ///< CAN Transmit Single Buffer 19
    CAN_BUFFER_TX_SINGLE_20 = CAN_BUFFER_TYPE_TX_SINGLE + 20,   ///< CAN Transmit Single Buffer 20
    CAN_BUFFER_TX_SINGLE_21 = CAN_BUFFER_TYPE_TX_SINGLE + 21,   ///< CAN Transmit Single Buffer 21
    CAN_BUFFER_TX_SINGLE_22 = CAN_BUFFER_TYPE_TX_SINGLE + 22,   ///< CAN Transmit Single Buffer 22
    CAN_BUFFER_TX_SINGLE_23 = CAN_BUFFER_TYPE_TX_SINGLE + 23,   ///< CAN Transmit Single Buffer 23
    CAN_BUFFER_TX_SINGLE_24 = CAN_BUFFER_TYPE_TX_SINGLE + 24,   ///< CAN Transmit Single Buffer 24
    CAN_BUFFER_TX_SINGLE_25 = CAN_BUFFER_TYPE_TX_SINGLE + 25,   ///< CAN Transmit Single Buffer 25
    CAN_BUFFER_TX_SINGLE_26 = CAN_BUFFER_TYPE_TX_SINGLE + 26,   ///< CAN Transmit Single Buffer 26
    CAN_BUFFER_TX_SINGLE_27 = CAN_BUFFER_TYPE_TX_SINGLE + 27,   ///< CAN Transmit Single Buffer 27
    CAN_BUFFER_TX_SINGLE_28 = CAN_BUFFER_TYPE_TX_SINGLE + 28,   ///< CAN Transmit Single Buffer 28
    CAN_BUFFER_TX_SINGLE_29 = CAN_BUFFER_TYPE_TX_SINGLE + 29,   ///< CAN Transmit Single Buffer 29
    CAN_BUFFER_TX_SINGLE_30 = CAN_BUFFER_TYPE_TX_SINGLE + 30,   ///< CAN Transmit Single Buffer 30
    CAN_BUFFER_TX_SINGLE_31 = CAN_BUFFER_TYPE_TX_SINGLE + 31,   ///< CAN Transmit Single Buffer 31
    CAN_BUFFER_TX_SINGLE_32 = CAN_BUFFER_TYPE_TX_SINGLE + 32,   ///< CAN Transmit Single Buffer 32
    CAN_BUFFER_TX_SINGLE_33 = CAN_BUFFER_TYPE_TX_SINGLE + 33,   ///< CAN Transmit Single Buffer 33
    CAN_BUFFER_TX_SINGLE_34 = CAN_BUFFER_TYPE_TX_SINGLE + 34,   ///< CAN Transmit Single Buffer 34
    CAN_BUFFER_TX_SINGLE_35 = CAN_BUFFER_TYPE_TX_SINGLE + 35,   ///< CAN Transmit Single Buffer 35
    CAN_BUFFER_TX_SINGLE_36 = CAN_BUFFER_TYPE_TX_SINGLE + 36,   ///< CAN Transmit Single Buffer 36
    CAN_BUFFER_TX_SINGLE_37 = CAN_BUFFER_TYPE_TX_SINGLE + 37,   ///< CAN Transmit Single Buffer 37
    CAN_BUFFER_TX_SINGLE_38 = CAN_BUFFER_TYPE_TX_SINGLE + 38,   ///< CAN Transmit Single Buffer 38
    CAN_BUFFER_TX_SINGLE_39 = CAN_BUFFER_TYPE_TX_SINGLE + 39,   ///< CAN Transmit Single Buffer 39
    CAN_BUFFER_TX_SINGLE_40 = CAN_BUFFER_TYPE_TX_SINGLE + 40,   ///< CAN Transmit Single Buffer 40
    CAN_BUFFER_TX_SINGLE_41 = CAN_BUFFER_TYPE_TX_SINGLE + 41,   ///< CAN Transmit Single Buffer 41
    CAN_BUFFER_TX_SINGLE_42 = CAN_BUFFER_TYPE_TX_SINGLE + 42,   ///< CAN Transmit Single Buffer 42
    CAN_BUFFER_TX_SINGLE_43 = CAN_BUFFER_TYPE_TX_SINGLE + 43,   ///< CAN Transmit Single Buffer 43
    CAN_BUFFER_TX_SINGLE_44 = CAN_BUFFER_TYPE_TX_SINGLE + 44,   ///< CAN Transmit Single Buffer 44
    CAN_BUFFER_TX_SINGLE_45 = CAN_BUFFER_TYPE_TX_SINGLE + 45,   ///< CAN Transmit Single Buffer 45
    CAN_BUFFER_TX_SINGLE_46 = CAN_BUFFER_TYPE_TX_SINGLE + 46,   ///< CAN Transmit Single Buffer 46
    CAN_BUFFER_TX_SINGLE_47 = CAN_BUFFER_TYPE_TX_SINGLE + 47,   ///< CAN Transmit Single Buffer 47
    CAN_BUFFER_TX_SINGLE_48 = CAN_BUFFER_TYPE_TX_SINGLE + 48,   ///< CAN Transmit Single Buffer 48
    CAN_BUFFER_TX_SINGLE_49 = CAN_BUFFER_TYPE_TX_SINGLE + 49,   ///< CAN Transmit Single Buffer 49
    CAN_BUFFER_TX_SINGLE_50 = CAN_BUFFER_TYPE_TX_SINGLE + 50,   ///< CAN Transmit Single Buffer 50
    CAN_BUFFER_TX_SINGLE_51 = CAN_BUFFER_TYPE_TX_SINGLE + 51,   ///< CAN Transmit Single Buffer 51
    CAN_BUFFER_TX_SINGLE_52 = CAN_BUFFER_TYPE_TX_SINGLE + 52,   ///< CAN Transmit Single Buffer 52
    CAN_BUFFER_TX_SINGLE_53 = CAN_BUFFER_TYPE_TX_SINGLE + 53,   ///< CAN Transmit Single Buffer 53
    CAN_BUFFER_TX_SINGLE_54 = CAN_BUFFER_TYPE_TX_SINGLE + 54,   ///< CAN Transmit Single Buffer 54
    CAN_BUFFER_TX_SINGLE_55 = CAN_BUFFER_TYPE_TX_SINGLE + 55,   ///< CAN Transmit Single Buffer 55
    CAN_BUFFER_TX_SINGLE_56 = CAN_BUFFER_TYPE_TX_SINGLE + 56,   ///< CAN Transmit Single Buffer 56
    CAN_BUFFER_TX_SINGLE_57 = CAN_BUFFER_TYPE_TX_SINGLE + 57,   ///< CAN Transmit Single Buffer 57
    CAN_BUFFER_TX_SINGLE_58 = CAN_BUFFER_TYPE_TX_SINGLE + 58,   ///< CAN Transmit Single Buffer 58
    CAN_BUFFER_TX_SINGLE_59 = CAN_BUFFER_TYPE_TX_SINGLE + 59,   ///< CAN Transmit Single Buffer 59
    CAN_BUFFER_TX_SINGLE_60 = CAN_BUFFER_TYPE_TX_SINGLE + 60,   ///< CAN Transmit Single Buffer 60
    CAN_BUFFER_TX_SINGLE_61 = CAN_BUFFER_TYPE_TX_SINGLE + 61,   ///< CAN Transmit Single Buffer 61
    CAN_BUFFER_TX_SINGLE_62 = CAN_BUFFER_TYPE_TX_SINGLE + 62,   ///< CAN Transmit Single Buffer 62
    CAN_BUFFER_TX_SINGLE_63 = CAN_BUFFER_TYPE_TX_SINGLE + 63,   ///< CAN Transmit Single Buffer 63
    CAN_BUFFER_TX_SINGLE_64 = CAN_BUFFER_TYPE_TX_SINGLE + 64,   ///< CAN Transmit Single Buffer 64
    CAN_BUFFER_TX_SINGLE_65 = CAN_BUFFER_TYPE_TX_SINGLE + 65,   ///< CAN Transmit Single Buffer 65
    CAN_BUFFER_TX_SINGLE_66 = CAN_BUFFER_TYPE_TX_SINGLE + 66,   ///< CAN Transmit Single Buffer 66
    CAN_BUFFER_TX_SINGLE_67 = CAN_BUFFER_TYPE_TX_SINGLE + 67,   ///< CAN Transmit Single Buffer 67
    CAN_BUFFER_TX_SINGLE_68 = CAN_BUFFER_TYPE_TX_SINGLE + 68,   ///< CAN Transmit Single Buffer 68
    CAN_BUFFER_TX_SINGLE_69 = CAN_BUFFER_TYPE_TX_SINGLE + 69,   ///< CAN Transmit Single Buffer 69
    CAN_BUFFER_TX_SINGLE_70 = CAN_BUFFER_TYPE_TX_SINGLE + 70,   ///< CAN Transmit Single Buffer 70
    CAN_BUFFER_TX_SINGLE_71 = CAN_BUFFER_TYPE_TX_SINGLE + 71,   ///< CAN Transmit Single Buffer 71
    CAN_BUFFER_TX_SINGLE_72 = CAN_BUFFER_TYPE_TX_SINGLE + 72,   ///< CAN Transmit Single Buffer 72
    CAN_BUFFER_TX_SINGLE_73 = CAN_BUFFER_TYPE_TX_SINGLE + 73,   ///< CAN Transmit Single Buffer 73
    CAN_BUFFER_TX_SINGLE_74 = CAN_BUFFER_TYPE_TX_SINGLE + 74,   ///< CAN Transmit Single Buffer 74
    CAN_BUFFER_TX_SINGLE_75 = CAN_BUFFER_TYPE_TX_SINGLE + 75,   ///< CAN Transmit Single Buffer 75
    CAN_BUFFER_TX_SINGLE_76 = CAN_BUFFER_TYPE_TX_SINGLE + 76,   ///< CAN Transmit Single Buffer 76
    CAN_BUFFER_TX_SINGLE_77 = CAN_BUFFER_TYPE_TX_SINGLE + 77,   ///< CAN Transmit Single Buffer 77
    CAN_BUFFER_TX_SINGLE_78 = CAN_BUFFER_TYPE_TX_SINGLE + 78,   ///< CAN Transmit Single Buffer 78
    CAN_BUFFER_TX_SINGLE_79 = CAN_BUFFER_TYPE_TX_SINGLE + 79,   ///< CAN Transmit Single Buffer 79
    CAN_BUFFER_TX_SINGLE_80 = CAN_BUFFER_TYPE_TX_SINGLE + 80,   ///< CAN Transmit Single Buffer 80
    CAN_BUFFER_TX_SINGLE_81 = CAN_BUFFER_TYPE_TX_SINGLE + 81,   ///< CAN Transmit Single Buffer 81
    CAN_BUFFER_TX_SINGLE_82 = CAN_BUFFER_TYPE_TX_SINGLE + 82,   ///< CAN Transmit Single Buffer 82
    CAN_BUFFER_TX_SINGLE_83 = CAN_BUFFER_TYPE_TX_SINGLE + 83,   ///< CAN Transmit Single Buffer 83
    CAN_BUFFER_TX_SINGLE_84 = CAN_BUFFER_TYPE_TX_SINGLE + 84,   ///< CAN Transmit Single Buffer 84
    CAN_BUFFER_TX_SINGLE_85 = CAN_BUFFER_TYPE_TX_SINGLE + 85,   ///< CAN Transmit Single Buffer 85
    CAN_BUFFER_TX_SINGLE_86 = CAN_BUFFER_TYPE_TX_SINGLE + 86,   ///< CAN Transmit Single Buffer 86
    CAN_BUFFER_TX_SINGLE_87 = CAN_BUFFER_TYPE_TX_SINGLE + 87,   ///< CAN Transmit Single Buffer 87
    CAN_BUFFER_TX_SINGLE_88 = CAN_BUFFER_TYPE_TX_SINGLE + 88,   ///< CAN Transmit Single Buffer 88
    CAN_BUFFER_TX_SINGLE_89 = CAN_BUFFER_TYPE_TX_SINGLE + 89,   ///< CAN Transmit Single Buffer 89
    CAN_BUFFER_TX_SINGLE_90 = CAN_BUFFER_TYPE_TX_SINGLE + 90,   ///< CAN Transmit Single Buffer 90
    CAN_BUFFER_TX_SINGLE_91 = CAN_BUFFER_TYPE_TX_SINGLE + 91,   ///< CAN Transmit Single Buffer 91
    CAN_BUFFER_TX_SINGLE_92 = CAN_BUFFER_TYPE_TX_SINGLE + 92,   ///< CAN Transmit Single Buffer 92
    CAN_BUFFER_TX_SINGLE_93 = CAN_BUFFER_TYPE_TX_SINGLE + 93,   ///< CAN Transmit Single Buffer 93
    CAN_BUFFER_TX_SINGLE_94 = CAN_BUFFER_TYPE_TX_SINGLE + 94,   ///< CAN Transmit Single Buffer 94
    CAN_BUFFER_TX_SINGLE_95 = CAN_BUFFER_TYPE_TX_SINGLE + 95,   ///< CAN Transmit Single Buffer 95

    CAN_BUFFER_TX_FIFO_0 = CAN_BUFFER_TYPE_TX_FIFO + 0,         ///< CAN Transmit FIFO buffer 0
    CAN_BUFFER_TX_FIFO_1 = CAN_BUFFER_TYPE_TX_FIFO + 1,         ///< CAN Transmit FIFO buffer 1
    CAN_BUFFER_TX_FIFO_2 = CAN_BUFFER_TYPE_TX_FIFO + 2,         ///< CAN Transmit FIFO buffer 2
    CAN_BUFFER_TX_FIFO_3 = CAN_BUFFER_TYPE_TX_FIFO + 3,         ///< CAN Transmit FIFO buffer 3
    CAN_BUFFER_TX_FIFO_4 = CAN_BUFFER_TYPE_TX_FIFO + 4,         ///< CAN Transmit FIFO buffer 4
    CAN_BUFFER_TX_FIFO_5 = CAN_BUFFER_TYPE_TX_FIFO + 5,         ///< CAN Transmit FIFO buffer 5
    CAN_BUFFER_TX_FIFO_6 = CAN_BUFFER_TYPE_TX_FIFO + 6,         ///< CAN Transmit FIFO buffer 6
    CAN_BUFFER_TX_FIFO_7 = CAN_BUFFER_TYPE_TX_FIFO + 7,         ///< CAN Transmit FIFO buffer 7

    CAN_BUFFER_TX_PRIORITY_0 = CAN_BUFFER_TYPE_TX_PRIORITY + 0, ///< CAN Transmit Priority buffer 0
    CAN_BUFFER_TX_PRIORITY_1 = CAN_BUFFER_TYPE_TX_PRIORITY + 1, ///< CAN Transmit Priority buffer 1
    CAN_BUFFER_TX_PRIORITY_2 = CAN_BUFFER_TYPE_TX_PRIORITY + 2, ///< CAN Transmit Priority buffer 2
    CAN_BUFFER_TX_PRIORITY_3 = CAN_BUFFER_TYPE_TX_PRIORITY + 3, ///< CAN Transmit Priority buffer 3
    CAN_BUFFER_TX_PRIORITY_4 = CAN_BUFFER_TYPE_TX_PRIORITY + 4, ///< CAN Transmit Priority buffer 4
    CAN_BUFFER_TX_PRIORITY_5 = CAN_BUFFER_TYPE_TX_PRIORITY + 5, ///< CAN Transmit Priority buffer 5

    CAN_BUFFER_RX_SINGLE_0   = CAN_BUFFER_TYPE_RX_SINGLE + 0,   ///< CAN Receive Single Buffer 0
    CAN_BUFFER_RX_SINGLE_1   = CAN_BUFFER_TYPE_RX_SINGLE + 1,   ///< CAN Receive Single Buffer 1
    CAN_BUFFER_RX_SINGLE_2   = CAN_BUFFER_TYPE_RX_SINGLE + 2,   ///< CAN Receive Single Buffer 2
    CAN_BUFFER_RX_SINGLE_3   = CAN_BUFFER_TYPE_RX_SINGLE + 3,   ///< CAN Receive Single Buffer 3
    CAN_BUFFER_RX_SINGLE_4   = CAN_BUFFER_TYPE_RX_SINGLE + 4,   ///< CAN Receive Single Buffer 4
    CAN_BUFFER_RX_SINGLE_5   = CAN_BUFFER_TYPE_RX_SINGLE + 5,   ///< CAN Receive Single Buffer 5
    CAN_BUFFER_RX_SINGLE_6   = CAN_BUFFER_TYPE_RX_SINGLE + 6,   ///< CAN Receive Single Buffer 6
    CAN_BUFFER_RX_SINGLE_7   = CAN_BUFFER_TYPE_RX_SINGLE + 7,   ///< CAN Receive Single Buffer 7
    CAN_BUFFER_RX_SINGLE_8   = CAN_BUFFER_TYPE_RX_SINGLE + 8,   ///< CAN Receive Single Buffer 8
    CAN_BUFFER_RX_SINGLE_9   = CAN_BUFFER_TYPE_RX_SINGLE + 9,   ///< CAN Receive Single Buffer 9
    CAN_BUFFER_RX_SINGLE_10  = CAN_BUFFER_TYPE_RX_SINGLE + 10,  ///< CAN Receive Single Buffer 10
    CAN_BUFFER_RX_SINGLE_11  = CAN_BUFFER_TYPE_RX_SINGLE + 11,  ///< CAN Receive Single Buffer 11
    CAN_BUFFER_RX_SINGLE_12  = CAN_BUFFER_TYPE_RX_SINGLE + 12,  ///< CAN Receive Single Buffer 12
    CAN_BUFFER_RX_SINGLE_13  = CAN_BUFFER_TYPE_RX_SINGLE + 13,  ///< CAN Receive Single Buffer 13
    CAN_BUFFER_RX_SINGLE_14  = CAN_BUFFER_TYPE_RX_SINGLE + 14,  ///< CAN Receive Single Buffer 14
    CAN_BUFFER_RX_SINGLE_15  = CAN_BUFFER_TYPE_RX_SINGLE + 15,  ///< CAN Receive Single Buffer 15
    CAN_BUFFER_RX_SINGLE_16  = CAN_BUFFER_TYPE_RX_SINGLE + 16,  ///< CAN Receive Single Buffer 16
    CAN_BUFFER_RX_SINGLE_17  = CAN_BUFFER_TYPE_RX_SINGLE + 17,  ///< CAN Receive Single Buffer 17
    CAN_BUFFER_RX_SINGLE_18  = CAN_BUFFER_TYPE_RX_SINGLE + 18,  ///< CAN Receive Single Buffer 18
    CAN_BUFFER_RX_SINGLE_19  = CAN_BUFFER_TYPE_RX_SINGLE + 19,  ///< CAN Receive Single Buffer 19
    CAN_BUFFER_RX_SINGLE_20  = CAN_BUFFER_TYPE_RX_SINGLE + 20,  ///< CAN Receive Single Buffer 20
    CAN_BUFFER_RX_SINGLE_21  = CAN_BUFFER_TYPE_RX_SINGLE + 21,  ///< CAN Receive Single Buffer 21
    CAN_BUFFER_RX_SINGLE_22  = CAN_BUFFER_TYPE_RX_SINGLE + 22,  ///< CAN Receive Single Buffer 22
    CAN_BUFFER_RX_SINGLE_23  = CAN_BUFFER_TYPE_RX_SINGLE + 23,  ///< CAN Receive Single Buffer 23
    CAN_BUFFER_RX_SINGLE_24  = CAN_BUFFER_TYPE_RX_SINGLE + 24,  ///< CAN Receive Single Buffer 24
    CAN_BUFFER_RX_SINGLE_25  = CAN_BUFFER_TYPE_RX_SINGLE + 25,  ///< CAN Receive Single Buffer 25
    CAN_BUFFER_RX_SINGLE_26  = CAN_BUFFER_TYPE_RX_SINGLE + 26,  ///< CAN Receive Single Buffer 26
    CAN_BUFFER_RX_SINGLE_27  = CAN_BUFFER_TYPE_RX_SINGLE + 27,  ///< CAN Receive Single Buffer 27
    CAN_BUFFER_RX_SINGLE_28  = CAN_BUFFER_TYPE_RX_SINGLE + 28,  ///< CAN Receive Single Buffer 28
    CAN_BUFFER_RX_SINGLE_29  = CAN_BUFFER_TYPE_RX_SINGLE + 29,  ///< CAN Receive Single Buffer 29
    CAN_BUFFER_RX_SINGLE_30  = CAN_BUFFER_TYPE_RX_SINGLE + 30,  ///< CAN Receive Single Buffer 30
    CAN_BUFFER_RX_SINGLE_31  = CAN_BUFFER_TYPE_RX_SINGLE + 31,  ///< CAN Receive Single Buffer 31
    CAN_BUFFER_RX_SINGLE_32  = CAN_BUFFER_TYPE_RX_SINGLE + 32,  ///< CAN Receive Single Buffer 32
    CAN_BUFFER_RX_SINGLE_33  = CAN_BUFFER_TYPE_RX_SINGLE + 33,  ///< CAN Receive Single Buffer 33
    CAN_BUFFER_RX_SINGLE_34  = CAN_BUFFER_TYPE_RX_SINGLE + 34,  ///< CAN Receive Single Buffer 34
    CAN_BUFFER_RX_SINGLE_35  = CAN_BUFFER_TYPE_RX_SINGLE + 35,  ///< CAN Receive Single Buffer 35
    CAN_BUFFER_RX_SINGLE_36  = CAN_BUFFER_TYPE_RX_SINGLE + 36,  ///< CAN Receive Single Buffer 36
    CAN_BUFFER_RX_SINGLE_37  = CAN_BUFFER_TYPE_RX_SINGLE + 37,  ///< CAN Receive Single Buffer 37
    CAN_BUFFER_RX_SINGLE_38  = CAN_BUFFER_TYPE_RX_SINGLE + 38,  ///< CAN Receive Single Buffer 38
    CAN_BUFFER_RX_SINGLE_39  = CAN_BUFFER_TYPE_RX_SINGLE + 39,  ///< CAN Receive Single Buffer 39
    CAN_BUFFER_RX_SINGLE_40  = CAN_BUFFER_TYPE_RX_SINGLE + 40,  ///< CAN Receive Single Buffer 40
    CAN_BUFFER_RX_SINGLE_41  = CAN_BUFFER_TYPE_RX_SINGLE + 41,  ///< CAN Receive Single Buffer 41
    CAN_BUFFER_RX_SINGLE_42  = CAN_BUFFER_TYPE_RX_SINGLE + 42,  ///< CAN Receive Single Buffer 42
    CAN_BUFFER_RX_SINGLE_43  = CAN_BUFFER_TYPE_RX_SINGLE + 43,  ///< CAN Receive Single Buffer 43
    CAN_BUFFER_RX_SINGLE_44  = CAN_BUFFER_TYPE_RX_SINGLE + 44,  ///< CAN Receive Single Buffer 44
    CAN_BUFFER_RX_SINGLE_45  = CAN_BUFFER_TYPE_RX_SINGLE + 45,  ///< CAN Receive Single Buffer 45
    CAN_BUFFER_RX_SINGLE_46  = CAN_BUFFER_TYPE_RX_SINGLE + 46,  ///< CAN Receive Single Buffer 46
    CAN_BUFFER_RX_SINGLE_47  = CAN_BUFFER_TYPE_RX_SINGLE + 47,  ///< CAN Receive Single Buffer 47
    CAN_BUFFER_RX_SINGLE_48  = CAN_BUFFER_TYPE_RX_SINGLE + 48,  ///< CAN Receive Single Buffer 48
    CAN_BUFFER_RX_SINGLE_49  = CAN_BUFFER_TYPE_RX_SINGLE + 49,  ///< CAN Receive Single Buffer 49
    CAN_BUFFER_RX_SINGLE_50  = CAN_BUFFER_TYPE_RX_SINGLE + 50,  ///< CAN Receive Single Buffer 50
    CAN_BUFFER_RX_SINGLE_51  = CAN_BUFFER_TYPE_RX_SINGLE + 51,  ///< CAN Receive Single Buffer 51
    CAN_BUFFER_RX_SINGLE_52  = CAN_BUFFER_TYPE_RX_SINGLE + 52,  ///< CAN Receive Single Buffer 52
    CAN_BUFFER_RX_SINGLE_53  = CAN_BUFFER_TYPE_RX_SINGLE + 53,  ///< CAN Receive Single Buffer 53
    CAN_BUFFER_RX_SINGLE_54  = CAN_BUFFER_TYPE_RX_SINGLE + 54,  ///< CAN Receive Single Buffer 54
    CAN_BUFFER_RX_SINGLE_55  = CAN_BUFFER_TYPE_RX_SINGLE + 55,  ///< CAN Receive Single Buffer 55
    CAN_BUFFER_RX_SINGLE_56  = CAN_BUFFER_TYPE_RX_SINGLE + 56,  ///< CAN Receive Single Buffer 56
    CAN_BUFFER_RX_SINGLE_57  = CAN_BUFFER_TYPE_RX_SINGLE + 57,  ///< CAN Receive Single Buffer 57
    CAN_BUFFER_RX_SINGLE_58  = CAN_BUFFER_TYPE_RX_SINGLE + 58,  ///< CAN Receive Single Buffer 58
    CAN_BUFFER_RX_SINGLE_59  = CAN_BUFFER_TYPE_RX_SINGLE + 59,  ///< CAN Receive Single Buffer 59
    CAN_BUFFER_RX_SINGLE_60  = CAN_BUFFER_TYPE_RX_SINGLE + 60,  ///< CAN Receive Single Buffer 60
    CAN_BUFFER_RX_SINGLE_61  = CAN_BUFFER_TYPE_RX_SINGLE + 61,  ///< CAN Receive Single Buffer 61
    CAN_BUFFER_RX_SINGLE_62  = CAN_BUFFER_TYPE_RX_SINGLE + 62,  ///< CAN Receive Single Buffer 62
    CAN_BUFFER_RX_SINGLE_63  = CAN_BUFFER_TYPE_RX_SINGLE + 63,  ///< CAN Receive Single Buffer 63
    CAN_BUFFER_RX_SINGLE_64  = CAN_BUFFER_TYPE_RX_SINGLE + 64,  ///< CAN Receive Single Buffer 64
    CAN_BUFFER_RX_SINGLE_65  = CAN_BUFFER_TYPE_RX_SINGLE + 65,  ///< CAN Receive Single Buffer 65
    CAN_BUFFER_RX_SINGLE_66  = CAN_BUFFER_TYPE_RX_SINGLE + 66,  ///< CAN Receive Single Buffer 66
    CAN_BUFFER_RX_SINGLE_67  = CAN_BUFFER_TYPE_RX_SINGLE + 67,  ///< CAN Receive Single Buffer 67
    CAN_BUFFER_RX_SINGLE_68  = CAN_BUFFER_TYPE_RX_SINGLE + 68,  ///< CAN Receive Single Buffer 68
    CAN_BUFFER_RX_SINGLE_69  = CAN_BUFFER_TYPE_RX_SINGLE + 69,  ///< CAN Receive Single Buffer 69
    CAN_BUFFER_RX_SINGLE_70  = CAN_BUFFER_TYPE_RX_SINGLE + 70,  ///< CAN Receive Single Buffer 70
    CAN_BUFFER_RX_SINGLE_71  = CAN_BUFFER_TYPE_RX_SINGLE + 71,  ///< CAN Receive Single Buffer 71
    CAN_BUFFER_RX_SINGLE_72  = CAN_BUFFER_TYPE_RX_SINGLE + 72,  ///< CAN Receive Single Buffer 72
    CAN_BUFFER_RX_SINGLE_73  = CAN_BUFFER_TYPE_RX_SINGLE + 73,  ///< CAN Receive Single Buffer 73
    CAN_BUFFER_RX_SINGLE_74  = CAN_BUFFER_TYPE_RX_SINGLE + 74,  ///< CAN Receive Single Buffer 74
    CAN_BUFFER_RX_SINGLE_75  = CAN_BUFFER_TYPE_RX_SINGLE + 75,  ///< CAN Receive Single Buffer 75
    CAN_BUFFER_RX_SINGLE_76  = CAN_BUFFER_TYPE_RX_SINGLE + 76,  ///< CAN Receive Single Buffer 76
    CAN_BUFFER_RX_SINGLE_77  = CAN_BUFFER_TYPE_RX_SINGLE + 77,  ///< CAN Receive Single Buffer 77
    CAN_BUFFER_RX_SINGLE_78  = CAN_BUFFER_TYPE_RX_SINGLE + 78,  ///< CAN Receive Single Buffer 78
    CAN_BUFFER_RX_SINGLE_79  = CAN_BUFFER_TYPE_RX_SINGLE + 79,  ///< CAN Receive Single Buffer 79
    CAN_BUFFER_RX_SINGLE_80  = CAN_BUFFER_TYPE_RX_SINGLE + 80,  ///< CAN Receive Single Buffer 80
    CAN_BUFFER_RX_SINGLE_81  = CAN_BUFFER_TYPE_RX_SINGLE + 81,  ///< CAN Receive Single Buffer 81
    CAN_BUFFER_RX_SINGLE_82  = CAN_BUFFER_TYPE_RX_SINGLE + 82,  ///< CAN Receive Single Buffer 82
    CAN_BUFFER_RX_SINGLE_83  = CAN_BUFFER_TYPE_RX_SINGLE + 83,  ///< CAN Receive Single Buffer 83
    CAN_BUFFER_RX_SINGLE_84  = CAN_BUFFER_TYPE_RX_SINGLE + 84,  ///< CAN Receive Single Buffer 84
    CAN_BUFFER_RX_SINGLE_85  = CAN_BUFFER_TYPE_RX_SINGLE + 85,  ///< CAN Receive Single Buffer 85
    CAN_BUFFER_RX_SINGLE_86  = CAN_BUFFER_TYPE_RX_SINGLE + 86,  ///< CAN Receive Single Buffer 86
    CAN_BUFFER_RX_SINGLE_87  = CAN_BUFFER_TYPE_RX_SINGLE + 87,  ///< CAN Receive Single Buffer 87
    CAN_BUFFER_RX_SINGLE_88  = CAN_BUFFER_TYPE_RX_SINGLE + 88,  ///< CAN Receive Single Buffer 88
    CAN_BUFFER_RX_SINGLE_89  = CAN_BUFFER_TYPE_RX_SINGLE + 89,  ///< CAN Receive Single Buffer 89
    CAN_BUFFER_RX_SINGLE_90  = CAN_BUFFER_TYPE_RX_SINGLE + 90,  ///< CAN Receive Single Buffer 90
    CAN_BUFFER_RX_SINGLE_91  = CAN_BUFFER_TYPE_RX_SINGLE + 91,  ///< CAN Receive Single Buffer 91
    CAN_BUFFER_RX_SINGLE_92  = CAN_BUFFER_TYPE_RX_SINGLE + 92,  ///< CAN Receive Single Buffer 92
    CAN_BUFFER_RX_SINGLE_93  = CAN_BUFFER_TYPE_RX_SINGLE + 93,  ///< CAN Receive Single Buffer 93
    CAN_BUFFER_RX_SINGLE_94  = CAN_BUFFER_TYPE_RX_SINGLE + 94,  ///< CAN Receive Single Buffer 94
    CAN_BUFFER_RX_SINGLE_95  = CAN_BUFFER_TYPE_RX_SINGLE + 95,  ///< CAN Receive Single Buffer 95
    CAN_BUFFER_RX_SINGLE_96  = CAN_BUFFER_TYPE_RX_SINGLE + 96,  ///< CAN Receive Single Buffer 96
    CAN_BUFFER_RX_SINGLE_97  = CAN_BUFFER_TYPE_RX_SINGLE + 97,  ///< CAN Receive Single Buffer 97
    CAN_BUFFER_RX_SINGLE_98  = CAN_BUFFER_TYPE_RX_SINGLE + 98,  ///< CAN Receive Single Buffer 98
    CAN_BUFFER_RX_SINGLE_99  = CAN_BUFFER_TYPE_RX_SINGLE + 99,  ///< CAN Receive Single Buffer 99
    CAN_BUFFER_RX_SINGLE_100 = CAN_BUFFER_TYPE_RX_SINGLE + 100, ///< CAN Receive Single Buffer 100
    CAN_BUFFER_RX_SINGLE_101 = CAN_BUFFER_TYPE_RX_SINGLE + 101, ///< CAN Receive Single Buffer 101
    CAN_BUFFER_RX_SINGLE_102 = CAN_BUFFER_TYPE_RX_SINGLE + 102, ///< CAN Receive Single Buffer 102
    CAN_BUFFER_RX_SINGLE_103 = CAN_BUFFER_TYPE_RX_SINGLE + 103, ///< CAN Receive Single Buffer 103
    CAN_BUFFER_RX_SINGLE_104 = CAN_BUFFER_TYPE_RX_SINGLE + 104, ///< CAN Receive Single Buffer 104
    CAN_BUFFER_RX_SINGLE_105 = CAN_BUFFER_TYPE_RX_SINGLE + 105, ///< CAN Receive Single Buffer 105
    CAN_BUFFER_RX_SINGLE_106 = CAN_BUFFER_TYPE_RX_SINGLE + 106, ///< CAN Receive Single Buffer 106
    CAN_BUFFER_RX_SINGLE_107 = CAN_BUFFER_TYPE_RX_SINGLE + 107, ///< CAN Receive Single Buffer 107
    CAN_BUFFER_RX_SINGLE_108 = CAN_BUFFER_TYPE_RX_SINGLE + 108, ///< CAN Receive Single Buffer 108
    CAN_BUFFER_RX_SINGLE_109 = CAN_BUFFER_TYPE_RX_SINGLE + 109, ///< CAN Receive Single Buffer 109
    CAN_BUFFER_RX_SINGLE_110 = CAN_BUFFER_TYPE_RX_SINGLE + 110, ///< CAN Receive Single Buffer 110
    CAN_BUFFER_RX_SINGLE_111 = CAN_BUFFER_TYPE_RX_SINGLE + 111, ///< CAN Receive Single Buffer 111
    CAN_BUFFER_RX_SINGLE_112 = CAN_BUFFER_TYPE_RX_SINGLE + 112, ///< CAN Receive Single Buffer 112
    CAN_BUFFER_RX_SINGLE_113 = CAN_BUFFER_TYPE_RX_SINGLE + 113, ///< CAN Receive Single Buffer 113
    CAN_BUFFER_RX_SINGLE_114 = CAN_BUFFER_TYPE_RX_SINGLE + 114, ///< CAN Receive Single Buffer 114
    CAN_BUFFER_RX_SINGLE_115 = CAN_BUFFER_TYPE_RX_SINGLE + 115, ///< CAN Receive Single Buffer 115
    CAN_BUFFER_RX_SINGLE_116 = CAN_BUFFER_TYPE_RX_SINGLE + 116, ///< CAN Receive Single Buffer 116
    CAN_BUFFER_RX_SINGLE_117 = CAN_BUFFER_TYPE_RX_SINGLE + 117, ///< CAN Receive Single Buffer 117
    CAN_BUFFER_RX_SINGLE_118 = CAN_BUFFER_TYPE_RX_SINGLE + 118, ///< CAN Receive Single Buffer 118
    CAN_BUFFER_RX_SINGLE_119 = CAN_BUFFER_TYPE_RX_SINGLE + 119, ///< CAN Receive Single Buffer 119
    CAN_BUFFER_RX_SINGLE_120 = CAN_BUFFER_TYPE_RX_SINGLE + 120, ///< CAN Receive Single Buffer 120
    CAN_BUFFER_RX_SINGLE_121 = CAN_BUFFER_TYPE_RX_SINGLE + 121, ///< CAN Receive Single Buffer 121
    CAN_BUFFER_RX_SINGLE_122 = CAN_BUFFER_TYPE_RX_SINGLE + 122, ///< CAN Receive Single Buffer 122
    CAN_BUFFER_RX_SINGLE_123 = CAN_BUFFER_TYPE_RX_SINGLE + 123, ///< CAN Receive Single Buffer 123
    CAN_BUFFER_RX_SINGLE_124 = CAN_BUFFER_TYPE_RX_SINGLE + 124, ///< CAN Receive Single Buffer 124
    CAN_BUFFER_RX_SINGLE_125 = CAN_BUFFER_TYPE_RX_SINGLE + 125, ///< CAN Receive Single Buffer 125
    CAN_BUFFER_RX_SINGLE_126 = CAN_BUFFER_TYPE_RX_SINGLE + 126, ///< CAN Receive Single Buffer 126
    CAN_BUFFER_RX_SINGLE_127 = CAN_BUFFER_TYPE_RX_SINGLE + 127, ///< CAN Receive Single Buffer 127
    CAN_BUFFER_RX_SINGLE_128 = CAN_BUFFER_TYPE_RX_SINGLE + 128, ///< CAN Receive Single Buffer 128
    CAN_BUFFER_RX_SINGLE_129 = CAN_BUFFER_TYPE_RX_SINGLE + 129, ///< CAN Receive Single Buffer 129
    CAN_BUFFER_RX_SINGLE_130 = CAN_BUFFER_TYPE_RX_SINGLE + 130, ///< CAN Receive Single Buffer 130
    CAN_BUFFER_RX_SINGLE_131 = CAN_BUFFER_TYPE_RX_SINGLE + 131, ///< CAN Receive Single Buffer 131
    CAN_BUFFER_RX_SINGLE_132 = CAN_BUFFER_TYPE_RX_SINGLE + 132, ///< CAN Receive Single Buffer 132
    CAN_BUFFER_RX_SINGLE_133 = CAN_BUFFER_TYPE_RX_SINGLE + 133, ///< CAN Receive Single Buffer 133
    CAN_BUFFER_RX_SINGLE_134 = CAN_BUFFER_TYPE_RX_SINGLE + 134, ///< CAN Receive Single Buffer 134
    CAN_BUFFER_RX_SINGLE_135 = CAN_BUFFER_TYPE_RX_SINGLE + 135, ///< CAN Receive Single Buffer 135
    CAN_BUFFER_RX_SINGLE_136 = CAN_BUFFER_TYPE_RX_SINGLE + 136, ///< CAN Receive Single Buffer 136
    CAN_BUFFER_RX_SINGLE_137 = CAN_BUFFER_TYPE_RX_SINGLE + 137, ///< CAN Receive Single Buffer 137
    CAN_BUFFER_RX_SINGLE_138 = CAN_BUFFER_TYPE_RX_SINGLE + 138, ///< CAN Receive Single Buffer 138
    CAN_BUFFER_RX_SINGLE_139 = CAN_BUFFER_TYPE_RX_SINGLE + 139, ///< CAN Receive Single Buffer 139
    CAN_BUFFER_RX_SINGLE_140 = CAN_BUFFER_TYPE_RX_SINGLE + 140, ///< CAN Receive Single Buffer 140
    CAN_BUFFER_RX_SINGLE_141 = CAN_BUFFER_TYPE_RX_SINGLE + 141, ///< CAN Receive Single Buffer 141
    CAN_BUFFER_RX_SINGLE_142 = CAN_BUFFER_TYPE_RX_SINGLE + 142, ///< CAN Receive Single Buffer 142
    CAN_BUFFER_RX_SINGLE_143 = CAN_BUFFER_TYPE_RX_SINGLE + 143, ///< CAN Receive Single Buffer 143
    CAN_BUFFER_RX_SINGLE_144 = CAN_BUFFER_TYPE_RX_SINGLE + 144, ///< CAN Receive Single Buffer 144
    CAN_BUFFER_RX_SINGLE_145 = CAN_BUFFER_TYPE_RX_SINGLE + 145, ///< CAN Receive Single Buffer 145
    CAN_BUFFER_RX_SINGLE_146 = CAN_BUFFER_TYPE_RX_SINGLE + 146, ///< CAN Receive Single Buffer 146
    CAN_BUFFER_RX_SINGLE_147 = CAN_BUFFER_TYPE_RX_SINGLE + 147, ///< CAN Receive Single Buffer 147
    CAN_BUFFER_RX_SINGLE_148 = CAN_BUFFER_TYPE_RX_SINGLE + 148, ///< CAN Receive Single Buffer 148
    CAN_BUFFER_RX_SINGLE_149 = CAN_BUFFER_TYPE_RX_SINGLE + 149, ///< CAN Receive Single Buffer 149
    CAN_BUFFER_RX_SINGLE_150 = CAN_BUFFER_TYPE_RX_SINGLE + 150, ///< CAN Receive Single Buffer 150
    CAN_BUFFER_RX_SINGLE_151 = CAN_BUFFER_TYPE_RX_SINGLE + 151, ///< CAN Receive Single Buffer 151
    CAN_BUFFER_RX_SINGLE_152 = CAN_BUFFER_TYPE_RX_SINGLE + 152, ///< CAN Receive Single Buffer 152
    CAN_BUFFER_RX_SINGLE_153 = CAN_BUFFER_TYPE_RX_SINGLE + 153, ///< CAN Receive Single Buffer 153
    CAN_BUFFER_RX_SINGLE_154 = CAN_BUFFER_TYPE_RX_SINGLE + 154, ///< CAN Receive Single Buffer 154
    CAN_BUFFER_RX_SINGLE_155 = CAN_BUFFER_TYPE_RX_SINGLE + 155, ///< CAN Receive Single Buffer 155
    CAN_BUFFER_RX_SINGLE_156 = CAN_BUFFER_TYPE_RX_SINGLE + 156, ///< CAN Receive Single Buffer 156
    CAN_BUFFER_RX_SINGLE_157 = CAN_BUFFER_TYPE_RX_SINGLE + 157, ///< CAN Receive Single Buffer 157
    CAN_BUFFER_RX_SINGLE_158 = CAN_BUFFER_TYPE_RX_SINGLE + 158, ///< CAN Receive Single Buffer 158
    CAN_BUFFER_RX_SINGLE_159 = CAN_BUFFER_TYPE_RX_SINGLE + 159, ///< CAN Receive Single Buffer 159
    CAN_BUFFER_RX_SINGLE_160 = CAN_BUFFER_TYPE_RX_SINGLE + 160, ///< CAN Receive Single Buffer 160
    CAN_BUFFER_RX_SINGLE_161 = CAN_BUFFER_TYPE_RX_SINGLE + 161, ///< CAN Receive Single Buffer 161
    CAN_BUFFER_RX_SINGLE_162 = CAN_BUFFER_TYPE_RX_SINGLE + 162, ///< CAN Receive Single Buffer 162
    CAN_BUFFER_RX_SINGLE_163 = CAN_BUFFER_TYPE_RX_SINGLE + 163, ///< CAN Receive Single Buffer 163
    CAN_BUFFER_RX_SINGLE_164 = CAN_BUFFER_TYPE_RX_SINGLE + 164, ///< CAN Receive Single Buffer 164
    CAN_BUFFER_RX_SINGLE_165 = CAN_BUFFER_TYPE_RX_SINGLE + 165, ///< CAN Receive Single Buffer 165
    CAN_BUFFER_RX_SINGLE_166 = CAN_BUFFER_TYPE_RX_SINGLE + 166, ///< CAN Receive Single Buffer 166
    CAN_BUFFER_RX_SINGLE_167 = CAN_BUFFER_TYPE_RX_SINGLE + 167, ///< CAN Receive Single Buffer 167
    CAN_BUFFER_RX_SINGLE_168 = CAN_BUFFER_TYPE_RX_SINGLE + 168, ///< CAN Receive Single Buffer 168
    CAN_BUFFER_RX_SINGLE_169 = CAN_BUFFER_TYPE_RX_SINGLE + 169, ///< CAN Receive Single Buffer 169
    CAN_BUFFER_RX_SINGLE_170 = CAN_BUFFER_TYPE_RX_SINGLE + 170, ///< CAN Receive Single Buffer 170
    CAN_BUFFER_RX_SINGLE_171 = CAN_BUFFER_TYPE_RX_SINGLE + 171, ///< CAN Receive Single Buffer 171
    CAN_BUFFER_RX_SINGLE_172 = CAN_BUFFER_TYPE_RX_SINGLE + 172, ///< CAN Receive Single Buffer 172
    CAN_BUFFER_RX_SINGLE_173 = CAN_BUFFER_TYPE_RX_SINGLE + 173, ///< CAN Receive Single Buffer 173
    CAN_BUFFER_RX_SINGLE_174 = CAN_BUFFER_TYPE_RX_SINGLE + 174, ///< CAN Receive Single Buffer 174
    CAN_BUFFER_RX_SINGLE_175 = CAN_BUFFER_TYPE_RX_SINGLE + 175, ///< CAN Receive Single Buffer 175
    CAN_BUFFER_RX_SINGLE_176 = CAN_BUFFER_TYPE_RX_SINGLE + 176, ///< CAN Receive Single Buffer 176
    CAN_BUFFER_RX_SINGLE_177 = CAN_BUFFER_TYPE_RX_SINGLE + 177, ///< CAN Receive Single Buffer 177
    CAN_BUFFER_RX_SINGLE_178 = CAN_BUFFER_TYPE_RX_SINGLE + 178, ///< CAN Receive Single Buffer 178
    CAN_BUFFER_RX_SINGLE_179 = CAN_BUFFER_TYPE_RX_SINGLE + 179, ///< CAN Receive Single Buffer 179
    CAN_BUFFER_RX_SINGLE_180 = CAN_BUFFER_TYPE_RX_SINGLE + 180, ///< CAN Receive Single Buffer 180
    CAN_BUFFER_RX_SINGLE_181 = CAN_BUFFER_TYPE_RX_SINGLE + 181, ///< CAN Receive Single Buffer 181
    CAN_BUFFER_RX_SINGLE_182 = CAN_BUFFER_TYPE_RX_SINGLE + 182, ///< CAN Receive Single Buffer 182
    CAN_BUFFER_RX_SINGLE_183 = CAN_BUFFER_TYPE_RX_SINGLE + 183, ///< CAN Receive Single Buffer 183
    CAN_BUFFER_RX_SINGLE_184 = CAN_BUFFER_TYPE_RX_SINGLE + 184, ///< CAN Receive Single Buffer 184
    CAN_BUFFER_RX_SINGLE_185 = CAN_BUFFER_TYPE_RX_SINGLE + 185, ///< CAN Receive Single Buffer 185
    CAN_BUFFER_RX_SINGLE_186 = CAN_BUFFER_TYPE_RX_SINGLE + 186, ///< CAN Receive Single Buffer 186
    CAN_BUFFER_RX_SINGLE_187 = CAN_BUFFER_TYPE_RX_SINGLE + 187, ///< CAN Receive Single Buffer 187
    CAN_BUFFER_RX_SINGLE_188 = CAN_BUFFER_TYPE_RX_SINGLE + 188, ///< CAN Receive Single Buffer 188
    CAN_BUFFER_RX_SINGLE_189 = CAN_BUFFER_TYPE_RX_SINGLE + 189, ///< CAN Receive Single Buffer 189
    CAN_BUFFER_RX_SINGLE_190 = CAN_BUFFER_TYPE_RX_SINGLE + 190, ///< CAN Receive Single Buffer 190
    CAN_BUFFER_RX_SINGLE_191 = CAN_BUFFER_TYPE_RX_SINGLE + 191, ///< CAN Receive Single Buffer 191
    CAN_BUFFER_RX_SINGLE_192 = CAN_BUFFER_TYPE_RX_SINGLE + 192, ///< CAN Receive Single Buffer 192
    CAN_BUFFER_RX_SINGLE_193 = CAN_BUFFER_TYPE_RX_SINGLE + 193, ///< CAN Receive Single Buffer 193
    CAN_BUFFER_RX_SINGLE_194 = CAN_BUFFER_TYPE_RX_SINGLE + 194, ///< CAN Receive Single Buffer 194
    CAN_BUFFER_RX_SINGLE_195 = CAN_BUFFER_TYPE_RX_SINGLE + 195, ///< CAN Receive Single Buffer 195
    CAN_BUFFER_RX_SINGLE_196 = CAN_BUFFER_TYPE_RX_SINGLE + 196, ///< CAN Receive Single Buffer 196
    CAN_BUFFER_RX_SINGLE_197 = CAN_BUFFER_TYPE_RX_SINGLE + 197, ///< CAN Receive Single Buffer 197
    CAN_BUFFER_RX_SINGLE_198 = CAN_BUFFER_TYPE_RX_SINGLE + 198, ///< CAN Receive Single Buffer 198
    CAN_BUFFER_RX_SINGLE_199 = CAN_BUFFER_TYPE_RX_SINGLE + 199, ///< CAN Receive Single Buffer 199
    CAN_BUFFER_RX_SINGLE_200 = CAN_BUFFER_TYPE_RX_SINGLE + 200, ///< CAN Receive Single Buffer 200
    CAN_BUFFER_RX_SINGLE_201 = CAN_BUFFER_TYPE_RX_SINGLE + 201, ///< CAN Receive Single Buffer 201
    CAN_BUFFER_RX_SINGLE_202 = CAN_BUFFER_TYPE_RX_SINGLE + 202, ///< CAN Receive Single Buffer 202
    CAN_BUFFER_RX_SINGLE_203 = CAN_BUFFER_TYPE_RX_SINGLE + 203, ///< CAN Receive Single Buffer 203
    CAN_BUFFER_RX_SINGLE_204 = CAN_BUFFER_TYPE_RX_SINGLE + 204, ///< CAN Receive Single Buffer 204
    CAN_BUFFER_RX_SINGLE_205 = CAN_BUFFER_TYPE_RX_SINGLE + 205, ///< CAN Receive Single Buffer 205
    CAN_BUFFER_RX_SINGLE_206 = CAN_BUFFER_TYPE_RX_SINGLE + 206, ///< CAN Receive Single Buffer 206
    CAN_BUFFER_RX_SINGLE_207 = CAN_BUFFER_TYPE_RX_SINGLE + 207, ///< CAN Receive Single Buffer 207
    CAN_BUFFER_RX_SINGLE_208 = CAN_BUFFER_TYPE_RX_SINGLE + 208, ///< CAN Receive Single Buffer 208
    CAN_BUFFER_RX_SINGLE_209 = CAN_BUFFER_TYPE_RX_SINGLE + 209, ///< CAN Receive Single Buffer 209
    CAN_BUFFER_RX_SINGLE_210 = CAN_BUFFER_TYPE_RX_SINGLE + 210, ///< CAN Receive Single Buffer 210
    CAN_BUFFER_RX_SINGLE_211 = CAN_BUFFER_TYPE_RX_SINGLE + 211, ///< CAN Receive Single Buffer 211
    CAN_BUFFER_RX_SINGLE_212 = CAN_BUFFER_TYPE_RX_SINGLE + 212, ///< CAN Receive Single Buffer 212
    CAN_BUFFER_RX_SINGLE_213 = CAN_BUFFER_TYPE_RX_SINGLE + 213, ///< CAN Receive Single Buffer 213
    CAN_BUFFER_RX_SINGLE_214 = CAN_BUFFER_TYPE_RX_SINGLE + 214, ///< CAN Receive Single Buffer 214
    CAN_BUFFER_RX_SINGLE_215 = CAN_BUFFER_TYPE_RX_SINGLE + 215, ///< CAN Receive Single Buffer 215
    CAN_BUFFER_RX_SINGLE_216 = CAN_BUFFER_TYPE_RX_SINGLE + 216, ///< CAN Receive Single Buffer 216
    CAN_BUFFER_RX_SINGLE_217 = CAN_BUFFER_TYPE_RX_SINGLE + 217, ///< CAN Receive Single Buffer 217
    CAN_BUFFER_RX_SINGLE_218 = CAN_BUFFER_TYPE_RX_SINGLE + 218, ///< CAN Receive Single Buffer 218
    CAN_BUFFER_RX_SINGLE_219 = CAN_BUFFER_TYPE_RX_SINGLE + 219, ///< CAN Receive Single Buffer 219
    CAN_BUFFER_RX_SINGLE_220 = CAN_BUFFER_TYPE_RX_SINGLE + 220, ///< CAN Receive Single Buffer 220
    CAN_BUFFER_RX_SINGLE_221 = CAN_BUFFER_TYPE_RX_SINGLE + 221, ///< CAN Receive Single Buffer 221
    CAN_BUFFER_RX_SINGLE_222 = CAN_BUFFER_TYPE_RX_SINGLE + 222, ///< CAN Receive Single Buffer 222
    CAN_BUFFER_RX_SINGLE_223 = CAN_BUFFER_TYPE_RX_SINGLE + 223, ///< CAN Receive Single Buffer 223
    CAN_BUFFER_RX_SINGLE_224 = CAN_BUFFER_TYPE_RX_SINGLE + 224, ///< CAN Receive Single Buffer 224
    CAN_BUFFER_RX_SINGLE_225 = CAN_BUFFER_TYPE_RX_SINGLE + 225, ///< CAN Receive Single Buffer 225
    CAN_BUFFER_RX_SINGLE_226 = CAN_BUFFER_TYPE_RX_SINGLE + 226, ///< CAN Receive Single Buffer 226
    CAN_BUFFER_RX_SINGLE_227 = CAN_BUFFER_TYPE_RX_SINGLE + 227, ///< CAN Receive Single Buffer 227
    CAN_BUFFER_RX_SINGLE_228 = CAN_BUFFER_TYPE_RX_SINGLE + 228, ///< CAN Receive Single Buffer 228
    CAN_BUFFER_RX_SINGLE_229 = CAN_BUFFER_TYPE_RX_SINGLE + 229, ///< CAN Receive Single Buffer 229
    CAN_BUFFER_RX_SINGLE_230 = CAN_BUFFER_TYPE_RX_SINGLE + 230, ///< CAN Receive Single Buffer 230
    CAN_BUFFER_RX_SINGLE_231 = CAN_BUFFER_TYPE_RX_SINGLE + 231, ///< CAN Receive Single Buffer 231
    CAN_BUFFER_RX_SINGLE_232 = CAN_BUFFER_TYPE_RX_SINGLE + 232, ///< CAN Receive Single Buffer 232
    CAN_BUFFER_RX_SINGLE_233 = CAN_BUFFER_TYPE_RX_SINGLE + 233, ///< CAN Receive Single Buffer 233
    CAN_BUFFER_RX_SINGLE_234 = CAN_BUFFER_TYPE_RX_SINGLE + 234, ///< CAN Receive Single Buffer 234
    CAN_BUFFER_RX_SINGLE_235 = CAN_BUFFER_TYPE_RX_SINGLE + 235, ///< CAN Receive Single Buffer 235
    CAN_BUFFER_RX_SINGLE_236 = CAN_BUFFER_TYPE_RX_SINGLE + 236, ///< CAN Receive Single Buffer 236
    CAN_BUFFER_RX_SINGLE_237 = CAN_BUFFER_TYPE_RX_SINGLE + 237, ///< CAN Receive Single Buffer 237
    CAN_BUFFER_RX_SINGLE_238 = CAN_BUFFER_TYPE_RX_SINGLE + 238, ///< CAN Receive Single Buffer 238
    CAN_BUFFER_RX_SINGLE_239 = CAN_BUFFER_TYPE_RX_SINGLE + 239, ///< CAN Receive Single Buffer 239
    CAN_BUFFER_RX_SINGLE_240 = CAN_BUFFER_TYPE_RX_SINGLE + 240, ///< CAN Receive Single Buffer 240
    CAN_BUFFER_RX_SINGLE_241 = CAN_BUFFER_TYPE_RX_SINGLE + 241, ///< CAN Receive Single Buffer 241
    CAN_BUFFER_RX_SINGLE_242 = CAN_BUFFER_TYPE_RX_SINGLE + 242, ///< CAN Receive Single Buffer 242
    CAN_BUFFER_RX_SINGLE_243 = CAN_BUFFER_TYPE_RX_SINGLE + 243, ///< CAN Receive Single Buffer 243
    CAN_BUFFER_RX_SINGLE_244 = CAN_BUFFER_TYPE_RX_SINGLE + 244, ///< CAN Receive Single Buffer 244
    CAN_BUFFER_RX_SINGLE_245 = CAN_BUFFER_TYPE_RX_SINGLE + 245, ///< CAN Receive Single Buffer 245
    CAN_BUFFER_RX_SINGLE_246 = CAN_BUFFER_TYPE_RX_SINGLE + 246, ///< CAN Receive Single Buffer 246
    CAN_BUFFER_RX_SINGLE_247 = CAN_BUFFER_TYPE_RX_SINGLE + 247, ///< CAN Receive Single Buffer 247
    CAN_BUFFER_RX_SINGLE_248 = CAN_BUFFER_TYPE_RX_SINGLE + 248, ///< CAN Receive Single Buffer 248
    CAN_BUFFER_RX_SINGLE_249 = CAN_BUFFER_TYPE_RX_SINGLE + 249, ///< CAN Receive Single Buffer 249
    CAN_BUFFER_RX_SINGLE_250 = CAN_BUFFER_TYPE_RX_SINGLE + 250, ///< CAN Receive Single Buffer 250
    CAN_BUFFER_RX_SINGLE_251 = CAN_BUFFER_TYPE_RX_SINGLE + 251, ///< CAN Receive Single Buffer 251
    CAN_BUFFER_RX_SINGLE_252 = CAN_BUFFER_TYPE_RX_SINGLE + 252, ///< CAN Receive Single Buffer 252
    CAN_BUFFER_RX_SINGLE_253 = CAN_BUFFER_TYPE_RX_SINGLE + 253, ///< CAN Receive Single Buffer 253
    CAN_BUFFER_RX_SINGLE_254 = CAN_BUFFER_TYPE_RX_SINGLE + 254, ///< CAN Receive Single Buffer 254
    CAN_BUFFER_RX_SINGLE_255 = CAN_BUFFER_TYPE_RX_SINGLE + 255, ///< CAN Receive Single Buffer 255

    CAN_BUFFER_RX_FIFO_0 = CAN_BUFFER_TYPE_RX_FIFO + 0,         ///< CAN Receive FIFO buffer 0
    CAN_BUFFER_RX_FIFO_1 = CAN_BUFFER_TYPE_RX_FIFO + 1,         ///< CAN Receive FIFO buffer 1
    CAN_BUFFER_RX_FIFO_2 = CAN_BUFFER_TYPE_RX_FIFO + 2,         ///< CAN Receive FIFO buffer 2
    CAN_BUFFER_RX_FIFO_3 = CAN_BUFFER_TYPE_RX_FIFO + 3,         ///< CAN Receive FIFO buffer 3
    CAN_BUFFER_RX_FIFO_4 = CAN_BUFFER_TYPE_RX_FIFO + 4,         ///< CAN Receive FIFO buffer 4
    CAN_BUFFER_RX_FIFO_5 = CAN_BUFFER_TYPE_RX_FIFO + 5,         ///< CAN Receive FIFO buffer 5
    CAN_BUFFER_RX_FIFO_6 = CAN_BUFFER_TYPE_RX_FIFO + 6,         ///< CAN Receive FIFO buffer 6
    CAN_BUFFER_RX_FIFO_7 = CAN_BUFFER_TYPE_RX_FIFO + 7,         ///< CAN Receive FIFO buffer 7

    CAN_BUFFER_RX_FIFO_8  = CAN_BUFFER_TYPE_RX_FIFO + 8,        ///< CAN Receive FIFO buffer 8
    CAN_BUFFER_RX_FIFO_9  = CAN_BUFFER_TYPE_RX_FIFO + 9,        ///< CAN Receive FIFO buffer 9
    CAN_BUFFER_RX_FIFO_10 = CAN_BUFFER_TYPE_RX_FIFO + 10,       ///< CAN Receive FIFO buffer 10
} can_buffer_t;

/** CAN frame format  */
typedef enum e_can_frame_format
{
    CAN_FRAME_FORMAT_CC = 0,           ///< Classical CAN Frame format
    CAN_FRAME_FORMAT_FD,               ///< FD Frame Format
    CAN_FRAME_FORMAT_XL,               ///< XL Frame format
} can_frame_format_t;

/** CAN frame options */
typedef enum e_can_frame_options
{
    CAN_FRAME_OPTIONS_NONE = 0,        ///< No options set.
    CAN_FRAME_OPTIONS_BRS  = (1 << 0), ///< Bit Rate Switch (BRS) option. Only used in FD Frame
    CAN_FRAME_OPTIONS_ESI  = (1 << 1), ///< Error State Indicator (ESI) option. Only used in FD Frame
    CAN_FRAME_OPTIONS_SEC  = (1 << 2), ///< Simple extended content (SEC) option. Only used in XL Frame
} can_frames_options_t;

#ifndef BSP_OVERRIDE_CAN_INFO_T

/** CAN status info */
typedef struct st_can_info
{
    uint32_t status;                   ///< Useful information from the CAN status register.
    uint32_t rx_single_status[4];      ///< RX Single Buffer New Data flags. (1: buffer contain data, 0: buffer doesn't have data)
    uint32_t rx_fifo_status;           ///< RX FIFO Empty flags.
    uint8_t  error_count_transmit;     ///< Transmit error count.
    uint8_t  error_count_receive;      ///< Receive error count.
    uint32_t error_code;               ///< Error code, cleared after reading.
} can_info_t;

#endif

#ifndef BSP_OVERRIDE_CAN_ID_MODE_T

/** CAN ID modes */
typedef enum e_can_id_mode
{
    CAN_ID_MODE_STANDARD,              ///< Standard IDs of 11 bits used.
    CAN_ID_MODE_EXTENDED,              ///< Extended IDs of 29 bits used.
} can_id_mode_t;

#endif

#ifndef BSP_OVERRIDE_CAN_FRAME_TYPE_T

/** CAN frame types */
typedef enum e_can_frame_type
{
    CAN_FRAME_TYPE_DATA,               ///< Data frame.
    CAN_FRAME_TYPE_REMOTE,             ///< Remote frame.
} can_frame_type_t;

#endif

/** CAN bit rate configuration. */
typedef struct st_can_bit_timing_cfg
{
    uint32_t baud_rate_prescaler;        ///< Baud rate prescaler.
    uint32_t time_segment_1;             ///< Time segment 1 control.
    uint32_t time_segment_2;             ///< Time segment 2 control.
    uint32_t synchronization_jump_width; ///< Synchronization jump width.
} can_bit_timing_cfg_t;

#ifndef BSP_OVERRIDE_CAN_FRAME_T

/** CAN data Frame */
typedef struct st_can_frame
{
    uint32_t             id;                           ///< CAN ID.
    can_id_mode_t        id_mode;                      ///< Standard or Extended ID (IDE).
    can_frame_type_t     type;                         ///< Frame type (RTR). Remote request substitution (RRS) in FD and XL Frame
    can_frame_format_t   format;                       ///< Frame format (CC, FD, XL).
    can_frames_options_t options;                      ///< Frame options.
    uint16_t             data_length_code;             ///< Data Length.
    uint8_t              vcid;                         ///< Virtual CAN Network ID (VCID). Only used in XL frame
    uint8_t              sdt;                          ///< SDU Type (SDT). Only used in XL Frame
    uint32_t             accept_field;                 ///< CANXL Acceptance Field (AF). Only used in XL Frame
    uint32_t             labels;                       ///< Frame Label
    uint64_t             timestamp;                    ///< Frame time stamp
    uint8_t              data[CAN_DATA_BUFFER_LENGTH]; ///< Frame data.
} can_frame_t;

#endif

#ifndef BSP_OVERRIDE_CAN_CALLBACK_ARGS_T

/** CAN callback parameter definition */
typedef struct st_can_callback_args
{
    uint32_t    channel;               ///< Device channel number.
    can_event_t event;                 ///< Event code.
    uint32_t    error;                 ///< Error code.
    union
    {
        uint32_t     mailbox;          ///< Mailbox number of interrupt source.
        can_buffer_t buffer;           ///< Buffer number of interrupt source.
    };
    uint32_t label;                    ///< Label of frame
    void   * p_context;                ///< Context provided to user during callback.
} can_callback_args_t;

#endif

/** CAN Configuration */
typedef struct st_can_cfg
{
    /* CAN generic configuration */
    uint32_t               channel;                    ///< CAN channel.
    can_bit_timing_cfg_t * p_bit_timing;               ///< CAN bit timing.

    /* Configuration for CAN Event processing */
    void (* p_callback)(can_callback_args_t * p_args); ///< Pointer to callback function
    void * p_context;                                  ///< User defined callback context.

    /* Pointer to CAN peripheral specific configuration */
    void const * p_extend;                             ///< CAN hardware dependent configuration.
    uint8_t      ipl;                                  ///< Error/Transmit/Receive interrupt priority.
    IRQn_Type    error_irq;                            ///< Error IRQ number.
    IRQn_Type    rx_irq;                               ///< Receive IRQ number.
    IRQn_Type    tx_irq;                               ///< Transmit IRQ number.

    uint8_t   vm_ipl;                                  ///< Error/Transmit/Receive virtual machine interrupt priority.
    IRQn_Type tx_vm_irq;                               ///< Transmit Virtual machine IRQ number
    IRQn_Type rx_vm_irq;                               ///< Receive Virtual machine IRQ number
    IRQn_Type err_vm_irq;                              ///< Error Virtual machine IRQ number
} can_cfg_t;

/** CAN control block.  Allocate an instance specific control block to pass into the CAN API calls.
 */
typedef void can_ctrl_t;

/** Shared Interface definition for CAN */
typedef struct st_can_api
{
    /** Open function for CAN device
     *
     * @param[in,out]  p_ctrl     Pointer to the CAN control block. Must be declared by user. Value set here.
     * @param[in]      p_cfg      Pointer to CAN configuration structure. All elements of this structure must be set by
     *                            user.
     */
    fsp_err_t (* open)(can_ctrl_t * const p_ctrl, can_cfg_t const * const p_cfg);

    /** Write function for CAN device
     * @param[in]   p_ctrl          Pointer to the CAN control block.
     * @param[in]   buffer_number   Buffer number (mailbox or message buffer) to write to.
     * @param[in]   p_frame         Pointer for frame of CAN ID, DLC, data and frame type to write.
     */
    fsp_err_t (* write)(can_ctrl_t * const p_ctrl, can_buffer_t buffer_number, can_frame_t * const p_frame);

    /** Read function for CAN device
     * @param[in]   p_ctrl          Pointer to the CAN control block.
     * @param[in]   buffer_number   Message buffer (number) to read from.
     * @param[in]   p_frame         Pointer to store the CAN ID, DLC, data and frame type.
     */
    fsp_err_t (* read)(can_ctrl_t * const p_ctrl, can_buffer_t buffer_number, can_frame_t * const p_frame);

    /** Close function for CAN device
     * @param[in]   p_ctrl     Pointer to the CAN control block.
     */
    fsp_err_t (* close)(can_ctrl_t * const p_ctrl);

    /** Mode Transition function for CAN device
     * @param[in]   p_ctrl               Pointer to the CAN control block.
     * @param[in]   operation_mode       Destination CAN operation state.
     * @param[in]   test_mode            Destination CAN test state.
     */
    fsp_err_t (* modeTransition)(can_ctrl_t * const p_ctrl, can_operation_mode_t operation_mode,
                                 can_test_mode_t test_mode);

    /** Get CAN channel info.
     *
     * @param[in]   p_ctrl  Handle for channel (pointer to channel control block)
     * @param[out]  p_info  Memory address to return channel specific data to.
     */
    fsp_err_t (* infoGet)(can_ctrl_t * const p_ctrl, can_info_t * const p_info);

    /** Specify callback function and optional context pointer and working memory pointer.
     *
     * @param[in]   p_ctrl                   Control block set in @ref can_api_t::open call.
     * @param[in]   p_callback               Callback function to register
     * @param[in]   p_context                Pointer to send to callback function
     * @param[in]   p_working_memory         Pointer to volatile memory where callback structure can be allocated.
     *                                       Callback arguments allocated here are only valid during the callback.
     */
    fsp_err_t (* callbackSet)(can_ctrl_t * const p_ctrl, void (* p_callback)(can_callback_args_t *),
                              void * const p_context, can_callback_args_t * const p_callback_memory);
} can_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_can_instance
{
    can_ctrl_t      * p_ctrl;          ///< Pointer to the control structure for this instance
    can_cfg_t const * p_cfg;           ///< Pointer to the configuration structure for this instance
    can_api_t const * p_api;           ///< Pointer to the API structure for this instance
} can_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup CAN_API)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_CAN_API_H */
