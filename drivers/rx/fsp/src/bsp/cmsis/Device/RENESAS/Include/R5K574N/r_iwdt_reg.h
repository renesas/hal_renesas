/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_IWDT_REG_H
#define R_IWDT_REG_H

/* =========================================================================================================================== */
/* ================                                          R_IWDT                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Independent Watchdog Timer (R_IWDT)
 */

typedef struct                         /*!< (@ 0x87802200) R_IWDT Structure                                           */
{
    __IOM uint8_t IWDTRR;              /*!< (@ 0x00000000) IWDT Refresh Register                                      */
    __IM uint8_t  RESERVED;

    union
    {
        __IOM uint16_t IWDTCR;         /*!< (@ 0x00000002) IWDT Control Register                                      */

        struct
        {
            __IOM uint16_t TOPS : 2;   /*!< [1..0] Timeout Period Select                                              */
            uint16_t            : 2;
            __IOM uint16_t CKS  : 4;   /*!< [7..4] Clock Division Ratio Select                                        */
            __IOM uint16_t RPES : 2;   /*!< [9..8] Window End Position Select                                         */
            uint16_t            : 2;
            __IOM uint16_t RPSS : 2;   /*!< [13..12] Window Start Position Select                                     */
            uint16_t            : 2;
        } IWDTCR_b;
    };

    union
    {
        __IOM uint16_t IWDTSR;          /*!< (@ 0x00000004) IWDT Status Register                                       */

        struct
        {
            __IM uint16_t  CNTVAL : 14; /*!< [13..0] Counter Value                                                     */
            __IOM uint16_t UNDFF  : 1;  /*!< [14..14] Underflow Flag                                                   */
            __IOM uint16_t REFEF  : 1;  /*!< [15..15] Refresh Error Flag                                               */
        } IWDTSR_b;
    };

    union
    {
        __IOM uint8_t IWDTRCR;         /*!< (@ 0x00000006) IWDT Reset Control Register                                */

        struct
        {
            uint8_t               : 7;
            __IOM uint8_t RSTIRQS : 1; /*!< [7..7] Reset Interrupt Request Select                                     */
        } IWDTRCR_b;
    };
    __IM uint8_t RESERVED1;

    union
    {
        __IOM uint8_t IWDTCSTPR;       /*!< (@ 0x00000008) IWDT Count Stop Control Register                           */

        struct
        {
            uint8_t              : 7;
            __IOM uint8_t SLCSTP : 1;  /*!< [7..7] Sleep Mode Count Stop Control                                      */
        } IWDTCSTPR_b;
    };
    __IM uint8_t  RESERVED2;
    __IM uint16_t RESERVED3;
} R_IWDT_Type;                         /*!< Size = 12 (0xc)                                                           */

#endif                                 /* R_IWDT_REG_H */
