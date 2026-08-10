/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

/**
 * @defgroup Interrupt_ID_AIACC AIACC
 * @brief Interrupt IDs for AIACC.
 * @ingroup Interrupt_ID
 * @{
 */

#ifndef INTERRUPT_ID_H
#define INTERRUPT_ID_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**
 * @brief ARM Generic Timer Interrupt number
 */
#define R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM 30

/**
 * @brief HSCIF Interrupt number
 */
#define SCIF0_INT_ID    619
#define SCIF1_INT_ID    620
#define SCIF3_INT_ID    621
#define SCIF4_INT_ID    622
#define HSCIF0_INT_ID   623
#define HSCIF1_INT_ID   624
#define HSCIF2_INT_ID   625
#define HSCIF3_INT_ID   626

/**
 * @brief SCP2CR
 */
#define SCP2CR_INT_BASE_ID 214

/**
 * @brief Reserved priority bit
 */
#define RESERVED_PRIORITY_BIT 3

/**
 * @brief Max priority value
 */
#define MAX_PRIORITY_VALUE 31

/**
 * @brief Macro to check if the priority exceeds the maximum value
 */
#define IPRIORITY(x) (((x <= MAX_PRIORITY_VALUE && x >= 0) ? x : MAX_PRIORITY_VALUE) << RESERVED_PRIORITY_BIT)

/**
 * @brief Interrupt IDs of GPIO group
 */
#define INTID_GPIO_GRP0                0x0020
#define INTID_GPIO_GRP1                0x0024
#define INTID_GPIO_GRP2                0x0028
#define INTID_GPIO_GRP3                0x002C
#define INTID_GPIO_GRP4                0x0030
#define INTID_GPIO_GRP5                0x0034
#define INTID_GPIO_GRP6                0x0038
#define INTID_GPIO_GRP7                0x003C
#define INTID_GPIO_GRP8                0x0040
#define INTID_GPIO_GRP9                0x0044
#define INTID_GPIO_GRP10       0x0048
#define INTID_GPIO_NO_EXIST    0x0

/**
 * @brief Interrupt IDs of I2C driver
 */
#define INTID_I2C_IF0		0x0392
#define INTID_I2C_IF1		0x025D
#define INTID_I2C_IF2		0x025E
#define INTID_I2C_IF3		0x025F
#define INTID_I2C_IF4		0x0260
#define INTID_I2C_IF5		0x0261
#define INTID_I2C_IF6		0x0262
#define INTID_I2C_IF7		0x0263
#define INTID_I2C_IF8		0x0264
#define INTID_NO_EXIST		0x0

/**
 * @brief Interrupt IDs of SYS-DMAC group 0
 */
#define INTID_SYSDMA0_CH0                0x013A
#define INTID_SYSDMA0_CH1                0x013A
#define INTID_SYSDMA0_CH2                0x013B
#define INTID_SYSDMA0_CH3                0x013B
#define INTID_SYSDMA0_CH4                0x013C
#define INTID_SYSDMA0_CH5                0x013C
#define INTID_SYSDMA0_CH6                0x013D
#define INTID_SYSDMA0_CH7                0x013D
#define INTID_SYSDMA0_CH8                0x013E
#define INTID_SYSDMA0_CH9                0x013E
#define INTID_SYSDMA0_CH10               0x013F
#define INTID_SYSDMA0_CH11               0x013F
#define INTID_SYSDMA0_CH12               0x0140
#define INTID_SYSDMA0_CH13               0x0140
#define INTID_SYSDMA0_CH14               0x0141
#define INTID_SYSDMA0_CH15               0x0141
#define INTID_SYSDMA0_NO_EXIST           0x0

/**
 * @brief Interrupt IDs of SYS-DMAC group 1
 */
#define INTID_SYSDMA1_CH0                0x0142
#define INTID_SYSDMA1_CH1                0x0142
#define INTID_SYSDMA1_CH2                0x0143
#define INTID_SYSDMA1_CH3                0x0143
#define INTID_SYSDMA1_CH4                0x0144
#define INTID_SYSDMA1_CH5                0x0144
#define INTID_SYSDMA1_CH6                0x0145
#define INTID_SYSDMA1_CH7                0x0145
#define INTID_SYSDMA1_CH8                0x0146
#define INTID_SYSDMA1_CH9                0x0146
#define INTID_SYSDMA1_CH10               0x0147
#define INTID_SYSDMA1_CH11               0x0147
#define INTID_SYSDMA1_CH12               0x0148
#define INTID_SYSDMA1_CH13               0x0148
#define INTID_SYSDMA1_CH14               0x0149
#define INTID_SYSDMA1_CH15               0x0149
#define INTID_SYSDMA1_NO_EXIST           0x0

/**
 * @brief Interrupt IDs of SYS-DMAC group 2
 */
#define INTID_SYSDMA2_CH0                0x014A
#define INTID_SYSDMA2_CH1                0x014A
#define INTID_SYSDMA2_CH2                0x014B
#define INTID_SYSDMA2_CH3                0x014B
#define INTID_SYSDMA2_CH4                0x014C
#define INTID_SYSDMA2_CH5                0x014C
#define INTID_SYSDMA2_CH6                0x014D
#define INTID_SYSDMA2_CH7                0x014D
#define INTID_SYSDMA2_CH8                0x014E
#define INTID_SYSDMA2_CH9                0x014E
#define INTID_SYSDMA2_CH10               0x014F
#define INTID_SYSDMA2_CH11               0x014F
#define INTID_SYSDMA2_CH12               0x0150
#define INTID_SYSDMA2_CH13               0x0150
#define INTID_SYSDMA2_CH14               0x0151
#define INTID_SYSDMA2_CH15               0x0151
#define INTID_SYSDMA2_NO_EXIST           0x0

/**
 * @brief Interrupt IDs of SYS-DMAC group 3
 */
#define INTID_SYSDMA3_CH0                0x0152
#define INTID_SYSDMA3_CH1                0x0152
#define INTID_SYSDMA3_CH2                0x0153
#define INTID_SYSDMA3_CH3                0x0153
#define INTID_SYSDMA3_CH4                0x0154
#define INTID_SYSDMA3_CH5                0x0154
#define INTID_SYSDMA3_CH6                0x0155
#define INTID_SYSDMA3_CH7                0x0155
#define INTID_SYSDMA3_CH8                0x0156
#define INTID_SYSDMA3_CH9                0x0156
#define INTID_SYSDMA3_CH10               0x0157
#define INTID_SYSDMA3_CH11               0x0157
#define INTID_SYSDMA3_CH12               0x0158
#define INTID_SYSDMA3_CH13               0x0158
#define INTID_SYSDMA3_CH14               0x0159
#define INTID_SYSDMA3_CH15               0x0159
#define INTID_SYSDMA3_NO_EXIST           0x0

/**
 * @brief Interrupt IDs of RT-DMAC group 0
 */
#define INTID_RTDMA0_CH0                0x0112
#define INTID_RTDMA0_CH1                0x0112
#define INTID_RTDMA0_CH2                0x0113
#define INTID_RTDMA0_CH3                0x0113
#define INTID_RTDMA0_CH4                0x0114
#define INTID_RTDMA0_CH5                0x0114
#define INTID_RTDMA0_CH6                0x0115
#define INTID_RTDMA0_CH7                0x0115
#define INTID_RTDMA0_CH8                0x0116
#define INTID_RTDMA0_CH9                0x0116
#define INTID_RTDMA0_CH10               0x0117
#define INTID_RTDMA0_CH11               0x0117
#define INTID_RTDMA0_CH12               0x0118
#define INTID_RTDMA0_CH13               0x0118
#define INTID_RTDMA0_CH14               0x0119
#define INTID_RTDMA0_CH15               0x0119
#define INTID_RTDMA0_NO_EXIST           0x0

/**
 * @brief Interrupt IDs of RT-DMAC group 1
 */
#define INTID_RTDMA1_CH0                0x011A
#define INTID_RTDMA1_CH1                0x011A
#define INTID_RTDMA1_CH2                0x011B
#define INTID_RTDMA1_CH3                0x011B
#define INTID_RTDMA1_CH4                0x011C
#define INTID_RTDMA1_CH5                0x011C
#define INTID_RTDMA1_CH6                0x011D
#define INTID_RTDMA1_CH7                0x011D
#define INTID_RTDMA1_CH8                0x011E
#define INTID_RTDMA1_CH9                0x011E
#define INTID_RTDMA1_CH10               0x011F
#define INTID_RTDMA1_CH11               0x011F
#define INTID_RTDMA1_CH12               0x0120
#define INTID_RTDMA1_CH13               0x0120
#define INTID_RTDMA1_CH14               0x0121
#define INTID_RTDMA1_CH15               0x0121
#define INTID_RTDMA1_NO_EXIST           0x0

/**
 * @brief Interrupt IDs of RT-DMAC group 2
 */
#define INTID_RTDMA2_CH0                0x0122
#define INTID_RTDMA2_CH1                0x0122
#define INTID_RTDMA2_CH2                0x0123
#define INTID_RTDMA2_CH3                0x0123
#define INTID_RTDMA2_CH4                0x0124
#define INTID_RTDMA2_CH5                0x0124
#define INTID_RTDMA2_CH6                0x0125
#define INTID_RTDMA2_CH7                0x0125
#define INTID_RTDMA2_CH8                0x0126
#define INTID_RTDMA2_CH9                0x0126
#define INTID_RTDMA2_CH10               0x0127
#define INTID_RTDMA2_CH11               0x0127
#define INTID_RTDMA2_CH12               0x0128
#define INTID_RTDMA2_CH13               0x0128
#define INTID_RTDMA2_CH14               0x0128
#define INTID_RTDMA2_CH15               0x0129
#define INTID_RTDMA2_NO_EXIST           0x0

/**
 * @brief Interrupt IDs of RT-DMAC group 3
 */
#define INTID_RTDMA3_CH0                0x012A
#define INTID_RTDMA3_CH1                0x012A
#define INTID_RTDMA3_CH2                0x012B
#define INTID_RTDMA3_CH3                0x012B
#define INTID_RTDMA3_CH4                0x012C
#define INTID_RTDMA3_CH5                0x012C
#define INTID_RTDMA3_CH6                0x012D
#define INTID_RTDMA3_CH7                0x012D
#define INTID_RTDMA3_CH8                0x012E
#define INTID_RTDMA3_CH9                0x012E
#define INTID_RTDMA3_CH10               0x012F
#define INTID_RTDMA3_CH11               0x012F
#define INTID_RTDMA3_CH12               0x0130
#define INTID_RTDMA3_CH13               0x0130
#define INTID_RTDMA3_CH14               0x0131
#define INTID_RTDMA3_CH15               0x0131
#define INTID_RTDMA3_NO_EXIST           0x0

/**
 * @brief Interrupt IDs of TAUD unit 0
 */
#define INTID_TAUD0_CH0                 0x0397
#define INTID_TAUD0_CH1                 0x0398
#define INTID_TAUD0_CH2                 0x0399
#define INTID_TAUD0_CH3                 0x039A
#define INTID_TAUD0_CH4                 0x039B
#define INTID_TAUD0_CH5                 0x039C
#define INTID_TAUD0_CH6                 0x039D
#define INTID_TAUD0_CH7                 0x039E
#define INTID_TAUD0_CH8                 0x039F
#define INTID_TAUD0_CH9                 0x03A0
#define INTID_TAUD0_CH10                0x03A1
#define INTID_TAUD0_CH11                0x03A2
#define INTID_TAUD0_CH12                0x03A3
#define INTID_TAUD0_CH13                0x03A4
#define INTID_TAUD0_CH14                0x03A5
#define INTID_TAUD0_CH15                0x03A6

/**
 * @brief Interrupt IDs of TAUD unit 1
 */
#define INTID_TAUD1_CH0                 0x03A7
#define INTID_TAUD1_CH1                 0x03A8
#define INTID_TAUD1_CH2                 0x03A9
#define INTID_TAUD1_CH3                 0x03AA
#define INTID_TAUD1_CH4                 0x03AB
#define INTID_TAUD1_CH5                 0x03AC
#define INTID_TAUD1_CH6                 0x03AD
#define INTID_TAUD1_CH7                 0x03AE
#define INTID_TAUD1_CH8                 0x03AF
#define INTID_TAUD1_CH9                 0x03B0
#define INTID_TAUD1_CH10                0x03B1
#define INTID_TAUD1_CH11                0x03B2
#define INTID_TAUD1_CH12                0x03B3
#define INTID_TAUD1_CH13                0x03B4
#define INTID_TAUD1_CH14                0x03B5
#define INTID_TAUD1_CH15                0x03B6

/**
 * @brief Interrupt IDs of ECM
 */
#define INTID_ECMERR_MERGE              0x0326

#ifdef __cplusplus
}
#endif

/** @} */ // end of Interrupt_ID_AIACC

#endif /* INTERRUPT_ID_H*/
