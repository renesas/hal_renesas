/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R8A78000_H
#define R8A78000_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CMSIS-style access qualifiers, in case no CMSIS core header is in scope. */
#ifndef __IOM
 #define __IOM    volatile
#endif
#ifndef __IM
 #define __IM     volatile const
#endif
#ifndef __OM
 #define __OM     volatile
#endif

/***********************************************************************************************************************
 * Instance base addresses
 **********************************************************************************************************************/

/* Base addresses taken from the X5H hardware manual */
#define R_OSTM0_BASE    (0x1C060000UL)     /*!< OSTM channel 0 base */
#define R_OSTM1_BASE    (0x1C061000UL)     /*!< OSTM channel 1 base */
#define R_OSTM2_BASE    (0x1C062000UL)     /*!< OSTM channel 2 base */
#define R_OSTM3_BASE    (0x1C063000UL)     /*!< OSTM channel 3 base */
#define R_OSTM4_BASE    (0x1C064000UL)     /*!< OSTM channel 4 base */
#define R_OSTM5_BASE    (0x1C065000UL)     /*!< OSTM channel 5 base */

/** Number of OSTM channels on X5H. */
#define R_OSTM_CHANNEL_COUNT     (6U)

/*
 * Stride between consecutive OSTM instances. X5H lays the six instances out
 * at a constant 4 KB stride.
 *
 * Note that only the first 0x24 bytes of each 4 KB slot are registers; the
 * rest of the slot is reserved padding.
 */
#define R_OSTM_CHANNEL_STRIDE    (0x00001000UL)

/***********************************************************************************************************************
 * Register layout
 **********************************************************************************************************************/

typedef struct                             /*!< OSTM channel register block (size 0x24) */
{
    union
    {
        __IOM uint32_t OSTMnCMP;           /*!< (@ 0x00) Compare register                                */

        struct
        {
            __IOM uint32_t OSTMnCMP : 32;  /*!< [31..0] Interval mode: down-counter start value.
                                            *           Free-run mode: compare value.                    */
        } OSTMnCMP_b;
    };

    union
    {
        __IOM uint32_t OSTMnCNT;           /*!< (@ 0x04) Counter register                                */

        struct
        {
            __IOM uint32_t OSTMnCNT : 32;  /*!< [31..0] 32-bit counter value                             */
        } OSTMnCNT_b;
    };

    __IM uint32_t RESERVED_08;             /*!< (@ 0x08) reserved on X5H                                 */
    __IM uint32_t RESERVED_0C;             /*!< (@ 0x0C) reserved on X5H                                 */

    union
    {
        __IM uint8_t OSTMnTE;              /*!< (@ 0x10) Count enable status register (read-only)        */

        struct
        {
            __IM uint8_t OSTMnTE : 1;      /*!< [0..0] 0: counter stopped, 1: counter running            */
            uint8_t              : 7;
        } OSTMnTE_b;
    };
    __IM uint8_t  RESERVED4;
    __IM uint16_t RESERVED5;

    union
    {
        __OM uint8_t OSTMnTS;              /*!< (@ 0x14) Count start trigger register (write-only)       */

        struct
        {
            __OM uint8_t OSTMnTS : 1;      /*!< [0..0] Write 1 to start the counter                      */
            uint8_t              : 7;
        } OSTMnTS_b;
    };
    __IM uint8_t  RESERVED6;
    __IM uint16_t RESERVED7;

    union
    {
        __OM uint8_t OSTMnTT;              /*!< (@ 0x18) Count stop trigger register (write-only)        */

        struct
        {
            __OM uint8_t OSTMnTT : 1;      /*!< [0..0] Write 1 to stop the counter                       */
            uint8_t              : 7;
        } OSTMnTT_b;
    };
    __IM uint8_t  RESERVED8;
    __IM uint16_t RESERVED9;
    __IM uint32_t RESERVED10;              /*!< (@ 0x1C) reserved                                        */

    union
    {
        __IOM uint8_t OSTMnCTL;            /*!< (@ 0x20) Control register                                */

        struct
        {
            __IOM uint8_t OSTMnMD0 : 1;    /*!< [0..0] Generate interrupt when the timer starts          */
            __IOM uint8_t OSTMnMD1 : 1;    /*!< [1..1] 0: interval timer mode, 1: free-run compare mode  */
            __IOM uint8_t OSTMnMD2 : 1;    /*!< [2..2] Counter reload at start disable                   */
            uint8_t                : 4;
            __IOM uint8_t OSTMnIE  : 1;    /*!< [7..7] Interrupt enable                                  */
        } OSTMnCTL_b;
    };
} R_OSTMn_Type;                            /*!< Size = 36 (0x24)                                         */

/***********************************************************************************************************************
 * Bitfield masks
 **********************************************************************************************************************/
#define R_OSTMn_OSTMnCTL_OSTMnMD0_Msk    (0x01UL)
#define R_OSTMn_OSTMnCTL_OSTMnMD1_Msk    (0x02UL)
#define R_OSTMn_OSTMnCTL_OSTMnMD2_Msk    (0x04UL)
#define R_OSTMn_OSTMnCTL_OSTMnIE_Msk     (0x80UL)

#ifdef __cplusplus
}
#endif

#endif                                 /* R8A78000_H */
