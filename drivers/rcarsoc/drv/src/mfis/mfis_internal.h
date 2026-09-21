/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef MFIS_INTERNAL_H
#define MFIS_INTERNAL_H

#include <stdint.h>

#define MFIS_BASE           (0x18800000)
#define MFIS_COMMON_BASE    (0x189E0000UL)

#define MFIS_LOCK_0_7_OFF   (0x00C0UL)
#define MFIS_LOCK_8_63_OFF  (0x0704UL)
#define MFIS_LOCK_0_7_BASE  (MFIS_COMMON_BASE + MFIS_LOCK_0_7_OFF)
#define MFIS_LOCK_8_63_BASE (MFIS_COMMON_BASE + MFIS_LOCK_8_63_OFF)

#define MFIS_LOCK_IS_ACQUIRED   (0x00000001)
#define MFIS_LOCK_RELEASE       (0x00000000)

/* Sender: CR52 - Receiver: CA720 */
#define IICR(i)     (MFIS_BASE + 0x1000 * (i))        // Common communication control register Sender core to Receiver core ch[i]
#define EICR(i)     (MFIS_BASE + 0x1000 * (i) + 0x04) // Common communication control register Receiver core to Sender core ch[i]
#define IMBR(i)     (MFIS_BASE + 0x1000 * (i) + 0x40) // Common communication message register Sender core to Receiver core ch[i]
#define EMBR(i)     (MFIS_BASE + 0x1000 * (i) + 0x44) // Common communication message register Receiver core to Sender core ch[i]

#define MFIS_UNLOCK_WRITE   (0x189e0900)
#define MFIS_ACCESS_CONTROL (MFIS_COMMON_BASE + 0x0904)

/* Interrupt ID of MFIS, i=[0-63] */
#define INTID_S_R(i)    (0x0056 + (i) * 2) // Common INTID ch[i] from Sender to Receiver, unused
#define INTID_R_S(i)    (0x0057 + (i) * 2) // Common INTID ch[i] from Receiver to Sender
#define MFIS_INTID(i, type) ((uint32_t)(0x0057U) + (((uint32_t)(i)) * 2U) - ((uint32_t)(type))) // Common INTID ch[i] from Sender to Receiver, unused

#endif // MFIS_INTERNAL_H
