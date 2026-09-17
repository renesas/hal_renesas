/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_LPM_REG_H
#define R_LPM_REG_H

/* ================                              Device Specific Cluster Section                              ================ */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/* =========================================================================================================================== */
/* ================                                          R_LPM                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Low Power Mode (R_LPM)
 */
typedef struct                         /*!< (@ 0x87019000) R_LPM Structure                                           */
{
    __IM uint32_t RESERVED[3];

    union
    {
        __IOM uint8_t SBYCR;           /*!< (@ 0x0000000C) Standby Control Register                                   */

        struct
        {
            uint8_t           : 6;
            __IOM uint8_t OPE : 1;     /*!< [6..6] Output Port Enable                                                 */
            uint8_t           : 1;
        } SBYCR_b;
    };
    __IM uint8_t RESERVED1;

    union
    {
        __IOM uint8_t SSCR2;           /*!< (@ 0x0000000E) Software Standby Control Register 2                        */

        struct
        {
            __IM uint8_t SS2RSF : 1;   /*!< [0..0] Software Standby mode 2 regulator status flag                      */
            uint8_t             : 7;
        } SSCR2_b;
    };
    __IM uint8_t  RESERVED2;
    __IM uint32_t RESERVED3;

    union
    {
        __IOM uint8_t VSCR;            /*!< (@ 0x00000014) Voltage Scaling Control Register                           */

        struct
        {
            __IOM uint8_t VSCM   : 3;  /*!< [2..0] Voltage Scaling Control Mode Bit                                   */
            uint8_t              : 1;
            __IM uint8_t VSCMTSF : 1;  /*!< [4..4] Voltage Scaling Control Mode Transition Status Flag                */
            uint8_t              : 3;
        } VSCR_b;
    };
    __IM uint8_t  RESERVED4;
    __IM uint16_t RESERVED5;
    __IM uint32_t RESERVED6[34];

    union
    {
        __IOM uint8_t OPCCR;           /*!< (@ 0x000000A0) Operating Power Control Register                           */

        struct
        {
            __IOM uint8_t OPCM   : 2;  /*!< [1..0] Operating Power Control Mode Select                                */
            uint8_t              : 2;
            __IM uint8_t OPCMTSF : 1;  /*!< [4..4] Operating Power Control Mode Transition Status Flag                */
            uint8_t              : 3;
        } OPCCR_b;
    };
    __IM uint8_t  RESERVED7;
    __IM uint16_t RESERVED8;
    __IM uint32_t RESERVED9[23];

    union
    {
        __IOM uint8_t CPUDSCR;         /*!< (@ 0x00000100) Power Gating Shift Control Register                        */

        struct
        {
            __IOM uint8_t PGD0 : 1;    /*!< [0..0] Power Gating Disable for CPU0                                      */
            uint8_t            : 7;
        } CPUDSCR_b;
    };
    __IM uint8_t  RESERVED10;
    __IM uint16_t RESERVED11;
    __IM uint32_t RESERVED12[5];

    union
    {
        __IOM uint8_t PDCTRESWM;       /*!< (@ 0x00000118) General Power Domain 2 Control Register                    */

        struct
        {
            __IOM uint8_t PDDE  : 1;   /*!< [0..0] Power control enable                                               */
            uint8_t             : 5;
            __IM uint8_t PDCSF  : 1;   /*!< [6..6] Power control status flag                                          */
            __IM uint8_t PDPGSF : 1;   /*!< [7..7] Power gating status flag                                           */
        } PDCTRESWM_b;
    };
    __IM uint8_t  RESERVED13;
    __IM uint16_t RESERVED14;
    __IM uint32_t RESERVED15[9];

    union
    {
        __IOM uint16_t PDRAMSCR0;       /*!< (@ 0x00000140) SRAM power domain Standby Control Register 0               */

        struct
        {
            __IOM uint16_t RKEEP0  : 1; /*!< [0..0] RAM Retention                                                      */
            __IOM uint16_t RKEEP1  : 1; /*!< [1..1] RAM Retention                                                      */
            __IOM uint16_t RKEEP2  : 1; /*!< [2..2] RAM Retention                                                      */
            __IOM uint16_t RKEEP3  : 1; /*!< [3..3] RAM Retention                                                      */
            __IOM uint16_t RKEEP4  : 1; /*!< [4..4] RAM Retention                                                      */
            __IOM uint16_t RKEEP5  : 1; /*!< [5..5] RAM Retention                                                      */
            __IOM uint16_t RKEEP6  : 1; /*!< [6..6] RAM Retention                                                      */
            __IOM uint16_t RKEEP7  : 1; /*!< [7..7] RAM Retention                                                      */
            __IOM uint16_t RKEEP8  : 1; /*!< [8..8] RAM Retention                                                      */
            __IOM uint16_t RKEEP9  : 1; /*!< [9..9] RAM Retention                                                      */
            __IOM uint16_t RKEEP10 : 1; /*!< [10..10] RAM Retention                                                    */
            __IOM uint16_t RKEEP11 : 1; /*!< [11..11] RAM Retention                                                    */
            uint16_t               : 4;
        } PDRAMSCR0_b;
    };
    __IM uint8_t RESERVED16;

    union
    {
        __IOM uint8_t PDRAMSCR2;       /*!< (@ 0x00000143) SRAM power domain Standby Control Register 2               */

        struct
        {
            __IOM uint8_t RKEEP0 : 1;  /*!< [0..0] RAM Retention                                                      */
            uint8_t              : 7;
        } PDRAMSCR2_b;
    };
    __IM uint32_t RESERVED17[161];

    union
    {
        __IOM uint32_t LPMSAR;           /*!< (@ 0x000003C8) Low Power Mode Security Attribution Register               */

        struct
        {
            __IOM uint32_t NONSEC0 : 1;  /*!< [0..0] Non-secure Attribute bit 0 Target register: OPCCR, MLSMCR          */
            __IOM uint32_t NONSEC1 : 1;  /*!< [1..1] Non-secure Attribute bit 1 Target register: LPSCR, DPSWCR          */
            __IOM uint32_t NONSEC2 : 1;  /*!< [2..2] Non-secure Attribute bit 2 Target register: SBYCR, SSCR1,
                                          *   SVSCR                                                                     */
            uint32_t               : 1;
            __IOM uint32_t NONSEC4 : 1;  /*!< [4..4] Non-secure Attribute bit 4 Target register: VSCR                   */
            uint32_t               : 3;
            __IOM uint32_t NONSEC8 : 1;  /*!< [8..8] Non-secure Attribute bit 8 Target register: DPSBYCR,
                                          *   DCCR                                                                      */
            uint32_t                : 8;
            __IOM uint32_t NONSEC17 : 1; /*!< [17..17] Non-secure Attribute bit 17 Target register: HOCOLDOCR           */
            __IOM uint32_t NONSEC18 : 1; /*!< [18..18] Non-secure Attribute bit 18 Target register: PLL1LDOCR           */
            __IOM uint32_t NONSEC19 : 1; /*!< [19..19] Non-secure Attribute bit 19 Target register: PLL2LDOCR           */
            uint32_t                : 1;
            __IOM uint32_t NONSEC21 : 1; /*!< [21..21] Non-secure Attribute bit 21 Target register: LVOCR,
                                          *   MWMCR                                                                     */
            uint32_t : 10;
        } LPMSAR_b;
    };
    __IM uint32_t RESERVED18[3];

    union
    {
        __IOM uint32_t PGCSAR;         /*!< (@ 0x000003D8) Power Gating Control Security Attribution Register         */

        struct
        {
            uint32_t               : 6;
            __IOM uint32_t NONSEC6 : 1; /*!< [6..6] Non-secure Attribute bit 6 Target register: PDCTRGEN2              */
            __IOM uint32_t NONSEC7 : 1; /*!< [7..7] Non-secure Attribute bit 7 Target register: CPUDSCR                */
            uint32_t               : 24;
        } PGCSAR_b;
    };
    __IM uint32_t RESERVED19;

    union
    {
        __IOM uint32_t DPFSAR;          /*!< (@ 0x000003E0) Deep Standby Interrupt Factor Security Attribution
                                         *                  Register                                                   */

        struct
        {
            __IOM uint32_t DPFSA0 : 1;  /*!< [0..0] Deep Standby Interrupt Factor Security Attribute bit
                                         *   0 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:0)Target factor : IRQn-DS Pin (n:0)                                    */
            __IOM uint32_t DPFSA1 : 1;  /*!< [1..1] Deep Standby Interrupt Factor Security Attribute bit
                                         *   1 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:1)Target factor : IRQn-DS Pin (n:1)                                    */
            __IOM uint32_t DPFSA2 : 1;  /*!< [2..2] Deep Standby Interrupt Factor Security Attribute bit
                                         *   2 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:2)Target factor : IRQn-DS Pin (n:2)                                    */
            __IOM uint32_t DPFSA3 : 1;  /*!< [3..3] Deep Standby Interrupt Factor Security Attribute bit
                                         *   3 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:3)Target factor : IRQn-DS Pin (n:3)                                    */
            __IOM uint32_t DPFSA4 : 1;  /*!< [4..4] Deep Standby Interrupt Factor Security Attribute bit
                                         *   4 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:4)Target factor : IRQn-DS Pin (n:4)                                    */
            __IOM uint32_t DPFSA5 : 1;  /*!< [5..5] Deep Standby Interrupt Factor Security Attribute bit
                                         *   5 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:5)Target factor : IRQn-DS Pin (n:5)                                    */
            __IOM uint32_t DPFSA6 : 1;  /*!< [6..6] Deep Standby Interrupt Factor Security Attribute bit
                                         *   6 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:6)Target factor : IRQn-DS Pin (n:6)                                    */
            __IOM uint32_t DPFSA7 : 1;  /*!< [7..7] Deep Standby Interrupt Factor Security Attribute bit
                                         *   7 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:7)Target factor : IRQn-DS Pin (n:7)                                    */
            __IOM uint32_t DPFSA8 : 1;  /*!< [8..8] Deep Standby Interrupt Factor Security Attribute bit
                                         *   8 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:0)Target factor : IRQn-DS Pin (n:8)                                    */
            __IOM uint32_t DPFSA9 : 1;  /*!< [9..9] Deep Standby Interrupt Factor Security Attribute bit
                                         *   9 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:1)Target factor : IRQn-DS Pin (n:9)                                    */
            __IOM uint32_t DPFSA10 : 1; /*!< [10..10] Deep Standby Interrupt Factor Security Attribute bit
                                         *   10 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:2)Target factor : IRQn-DS Pin (n:10)                                   */
            __IOM uint32_t DPFSA11 : 1; /*!< [11..11] Deep Standby Interrupt Factor Security Attribute bit
                                         *   11 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:3)Target factor : IRQn-DS Pin (n:11)                                   */
            __IOM uint32_t DPFSA12 : 1; /*!< [12..12] Deep Standby Interrupt Factor Security Attribute bit
                                         *   12 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:4)Target factor : IRQn-DS Pin (n:12)                                   */
            __IOM uint32_t DPFSA13 : 1; /*!< [13..13] Deep Standby Interrupt Factor Security Attribute bit
                                         *   13 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:5)Target factor : IRQn-DS Pin (n:13)                                   */
            __IOM uint32_t DPFSA14 : 1; /*!< [14..14] Deep Standby Interrupt Factor Security Attribute bit
                                         *   14 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:6)Target factor : IRQn-DS Pin (n:14)                                   */
            __IOM uint32_t DPFSA15 : 1; /*!< [15..15] Deep Standby Interrupt Factor Security Attribute bit
                                         *   15 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:7)Target factor : IRQn-DS Pin (n:15)                                   */
            __IOM uint32_t DPFSA16 : 1; /*!< [16..16] Deep Standby Interrupt Factor Security Attribute bit
                                         *   16 Target register: DPSIER2.b0, DPSIFR2.b0, DPSIEGR2.b0Target
                                         *   factor : PVD1                                                             */
            __IOM uint32_t DPFSA17 : 1; /*!< [17..17] Deep Standby Interrupt Factor Security Attribute bit
                                         *   17 Target register: DPSIER2.b1, DPSIFR2.b1, DPSIEGR2.b1Target
                                         *   factor : PVD2                                                             */
            __IOM uint32_t DPFSA18 : 1; /*!< [18..18] Deep Standby Interrupt Factor Security Attribute bit
                                         *   18 Target register: DPSIER2.b2, DPSIFR2.b2Target factor
                                         *   : RTC Interval                                                            */
            __IOM uint32_t DPFSA19 : 1; /*!< [19..19] Deep Standby Interrupt Factor Security Attribute bit
                                         *   19 Target register: DPSIER2.b3, DPSIFR2.b3Target factor
                                         *   : RTC Alarm                                                               */
            __IOM uint32_t DPFSA20 : 1; /*!< [20..20] Deep Standby Interrupt Factor Security Attribute bit
                                         *   20 Target register: DPSIER2.b4, DPSIFR2.b4, DPSIEGR2.b4Target
                                         *   factor : NMI Pin                                                          */
            uint32_t               : 3;
            __IOM uint32_t DPFSA24 : 1; /*!< [24..24] Deep Standby Interrupt Factor Security Attribute bit
                                         *   24 Target register: DPSIER3.b0, DPSIFR3.b0Target factor
                                         *   : USBFS Suspend/Resume                                                    */
            __IOM uint32_t DPFSA25 : 1; /*!< [25..25] Deep Standby Interrupt Factor Security Attribute bit
                                         *   25 Target register: DPSIER3.b1, DPSIFR3.b1Target factor
                                         *   : USBHS Suspend/Resume                                                    */
            uint32_t               : 3;
            __IOM uint32_t DPFSA29 : 1; /*!< [29..29] Deep Standby Interrupt Factor Security Attribute bit
                                         *   29 Target register: DPSIER3.b5, DPSIFR3.b5Target factor
                                         *   : IWDT Overflow                                                           */
            __IOM uint32_t DPFSA30 : 1; /*!< [30..30] Deep Standby Interrupt Factor Security Attribute bit
                                         *   30 Target register: DPSIER3.b6, DPSIFR3.b6Target factor
                                         *   : Sub-clock Oscillation stop detection                                    */
            __IOM uint32_t DFPSA31 : 1; /*!< [31..31] Deep Standby Interrupt Factor Security Attribute bit
                                         *   31 Target register: DPSIER3.b7, DPSIFR3.b7Target factor
                                         *   : Anti-Tamper Detection                                                   */
        } DPFSAR_b;
    };

    union
    {
        __IOM uint32_t RSCSAR;          /*!< (@ 0x000003E4) RAM Standby Control Security Attribution Register          */

        struct
        {
            __IOM uint32_t RSCSA0 : 1;  /*!< [0..0] RAM Standby Control Security Attribute bit 0 Target register:
                                         *   PDRAMSCR0.bn (n:0)                                                        */
            __IOM uint32_t RSCSA1 : 1;  /*!< [1..1] RAM Standby Control Security Attribute bit 1 Target register:
                                         *   PDRAMSCR0.bn (n:1)                                                        */
            __IOM uint32_t RSCSA2 : 1;  /*!< [2..2] RAM Standby Control Security Attribute bit 2 Target register:
                                         *   PDRAMSCR0.bn (n:2)                                                        */
            __IOM uint32_t RSCSA3 : 1;  /*!< [3..3] RAM Standby Control Security Attribute bit 3 Target register:
                                         *   PDRAMSCR0.bn (n:3)                                                        */
            __IOM uint32_t RSCSA4 : 1;  /*!< [4..4] RAM Standby Control Security Attribute bit 4 Target register:
                                         *   PDRAMSCR0.bn (n:4)                                                        */
            __IOM uint32_t RSCSA5 : 1;  /*!< [5..5] RAM Standby Control Security Attribute bit 5 Target register:
                                         *   PDRAMSCR0.bn (n:5)                                                        */
            __IOM uint32_t RSCSA6 : 1;  /*!< [6..6] RAM Standby Control Security Attribute bit 6 Target register:
                                         *   PDRAMSCR0.bn (n:6)                                                        */
            __IOM uint32_t RSCSA7 : 1;  /*!< [7..7] RAM Standby Control Security Attribute bit 7 Target register:
                                         *   PDRAMSCR0.bn (n:7)                                                        */
            __IOM uint32_t RSCSA8 : 1;  /*!< [8..8] RAM Standby Control Security Attribute bit 8 Target register:
                                         *   PDRAMSCR0.bn (n:8)                                                        */
            __IOM uint32_t RSCSA9 : 1;  /*!< [9..9] RAM Standby Control Security Attribute bit 9 Target register:
                                         *   PDRAMSCR0.bn (n:9)                                                        */
            __IOM uint32_t RSCSA10 : 1; /*!< [10..10] RAM Standby Control Security Attribute bit 10 Target
                                         *   register: PDRAMSCR0.bn (n:10)                                             */
            __IOM uint32_t RSCSA11 : 1; /*!< [11..11] RAM Standby Control Security Attribute bit 11 Target
                                         *   register: PDRAMSCR0.bn (n:11)                                             */
            __IOM uint32_t RSCSA12 : 1; /*!< [12..12] RAM Standby Control Security Attribute bit 12 Target
                                         *   register: PDRAMSCR0.bn (n:12)                                             */
            __IOM uint32_t RSCSA13 : 1; /*!< [13..13] RAM Standby Control Security Attribute bit 13 Target
                                         *   register: PDRAMSCR0.bn (n:13)                                             */
            __IOM uint32_t RSCSA14 : 1; /*!< [14..14] RAM Standby Control Security Attribute bit 14 Target
                                         *   register: PDRAMSCR0.bn (n:14)                                             */
            uint32_t               : 9;
            __IOM uint32_t RSCSA24 : 1; /*!< [24..24] RAM Standby Control Security Attribute bit 24 Target
                                         *   register: PDRAMSCR2.bn (n:0)                                              */
            __IOM uint32_t RSCSA25 : 1; /*!< [25..25] RAM Standby Control Security Attribute bit 25 Target
                                         *   register: PDRAMSCR2.bn (n:1)                                              */
            uint32_t : 6;
        } RSCSAR_b;
    };

    union
    {
        __IOM uint32_t DPFSAR1;         /*!< (@ 0x000003E8) Deep Standby Interrupt Factor Security Attribution
                                         *                  Register 1                                                 */

        struct
        {
            __IOM uint32_t DPFSA0 : 1;  /*!< [0..0] Deep Standby Interrupt Factor Security Attribute bit
                                         *   0 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:0)Target factor : IRQm-DS Pin (m:16)                                   */
            __IOM uint32_t DPFSA1 : 1;  /*!< [1..1] Deep Standby Interrupt Factor Security Attribute bit
                                         *   1 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:1)Target factor : IRQm-DS Pin (m:17)                                   */
            __IOM uint32_t DPFSA2 : 1;  /*!< [2..2] Deep Standby Interrupt Factor Security Attribute bit
                                         *   2 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:2)Target factor : IRQm-DS Pin (m:18)                                   */
            __IOM uint32_t DPFSA3 : 1;  /*!< [3..3] Deep Standby Interrupt Factor Security Attribute bit
                                         *   3 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:3)Target factor : IRQm-DS Pin (m:19)                                   */
            __IOM uint32_t DPFSA4 : 1;  /*!< [4..4] Deep Standby Interrupt Factor Security Attribute bit
                                         *   4 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:4)Target factor : IRQm-DS Pin (m:20)                                   */
            __IOM uint32_t DPFSA5 : 1;  /*!< [5..5] Deep Standby Interrupt Factor Security Attribute bit
                                         *   5 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:5)Target factor : IRQm-DS Pin (m:21)                                   */
            __IOM uint32_t DPFSA6 : 1;  /*!< [6..6] Deep Standby Interrupt Factor Security Attribute bit
                                         *   6 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:6)Target factor : IRQm-DS Pin (m:22)                                   */
            __IOM uint32_t DPFSA7 : 1;  /*!< [7..7] Deep Standby Interrupt Factor Security Attribute bit
                                         *   7 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:7)Target factor : IRQm-DS Pin (m:23)                                   */
            __IOM uint32_t DPFSA8 : 1;  /*!< [8..8] Deep Standby Interrupt Factor Security Attribute bit
                                         *   8 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 0)Target factor : IRQm-DS Pin (m:24)                                  */
            __IOM uint32_t DPFSA9 : 1;  /*!< [9..9] Deep Standby Interrupt Factor Security Attribute bit
                                         *   9 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 1)Target factor : IRQm-DS Pin (m:25)                                  */
            __IOM uint32_t DPFSA10 : 1; /*!< [10..10] Deep Standby Interrupt Factor Security Attribute bit
                                         *   10 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 2)Target factor : IRQm-DS Pin (m:26)                                  */
            __IOM uint32_t DPFSA11 : 1; /*!< [11..11] Deep Standby Interrupt Factor Security Attribute bit
                                         *   11 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 3)Target factor : IRQm-DS Pin (m:27)                                  */
            __IOM uint32_t DPFSA12 : 1; /*!< [12..12] Deep Standby Interrupt Factor Security Attribute bit
                                         *   12 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 4)Target factor : IRQm-DS Pin (m:28)                                  */
            __IOM uint32_t DPFSA13 : 1; /*!< [13..13] Deep Standby Interrupt Factor Security Attribute bit
                                         *   13 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 5)Target factor : IRQm-DS Pin (m:29)                                  */
            __IOM uint32_t DPFSA14 : 1; /*!< [14..14] Deep Standby Interrupt Factor Security Attribute bit
                                         *   14 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 6)Target factor : IRQm-DS Pin (m:30)                                  */
            __IOM uint32_t DPFSA15 : 1; /*!< [15..15] Deep Standby Interrupt Factor Security Attribute bit
                                         *   15 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 7)Target factor : IRQm-DS Pin (m:31)                                  */
            uint32_t : 16;
        } DPFSAR1_b;
    };
    __IM uint32_t RESERVED20[389];

    union
    {
        __IOM uint8_t DPSBYCR;           /*!< (@ 0x00000A00) Deep Standby Control Register                              */

        struct
        {
            __IOM uint8_t LDOSSMODE : 2; /*!< [1..0] LDO SSMODESet the time required to recover from the Deep
                                          *   Software Standby mode in LDO mode.                                        */
            __IOM uint8_t DCSSMODE : 2;  /*!< [3..2] DCDC SSMODESet the time required to recover from the
                                          *   Deep Software Standby mode in DCDC mode.                                  */
            uint8_t              : 2;
            __IOM uint8_t IOKEEP : 1;    /*!< [6..6] I/O Port Retention                                                 */
            uint8_t              : 1;
        } DPSBYCR_b;
    };
    __IM uint8_t  RESERVED21;
    __IM uint16_t RESERVED22;
    __IM uint32_t RESERVED23;

    union
    {
        __IOM uint8_t DPSIER0;         /*!< (@ 0x00000A08) Deep Standby Interrupt Enable Register 0                   */

        struct
        {
            __IOM uint8_t DIRQ0E : 1;  /*!< [0..0] IRQ0-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ1E : 1;  /*!< [1..1] IRQ1-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ2E : 1;  /*!< [2..2] IRQ2-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ3E : 1;  /*!< [3..3] IRQ3-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ4E : 1;  /*!< [4..4] IRQ4-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ5E : 1;  /*!< [5..5] IRQ5-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ6E : 1;  /*!< [6..6] IRQ6-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ7E : 1;  /*!< [7..7] IRQ7-DS Pin Enable                                                 */
        } DPSIER0_b;
    };
    __IM uint8_t  RESERVED24;
    __IM uint16_t RESERVED25;

    union
    {
        __IOM uint8_t DPSIER1;         /*!< (@ 0x00000A0C) Deep Standby Interrupt Enable Register 1                   */

        struct
        {
            __IOM uint8_t DIRQ8E  : 1; /*!< [0..0] IRQ8-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ9E  : 1; /*!< [1..1] IRQ9-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ10E : 1; /*!< [2..2] IRQ10-DS Pin Enable                                                */
            __IOM uint8_t DIRQ11E : 1; /*!< [3..3] IRQ11-DS Pin Enable                                                */
            __IOM uint8_t DIRQ12E : 1; /*!< [4..4] IRQ12-DS Pin Enable                                                */
            __IOM uint8_t DIRQ13E : 1; /*!< [5..5] IRQ13-DS Pin Enable                                                */
            __IOM uint8_t DIRQ14E : 1; /*!< [6..6] IRQ14-DS Pin Enable                                                */
            __IOM uint8_t DIRQ15E : 1; /*!< [7..7] IRQ15-DS Pin Enable                                                */
        } DPSIER1_b;
    };
    __IM uint8_t  RESERVED26;
    __IM uint16_t RESERVED27;

    union
    {
        __IOM uint8_t DPSIER2;         /*!< (@ 0x00000A10) Deep Standby Interrupt Enable Register 2                   */

        struct
        {
            __IOM uint8_t DPVD1IE : 1; /*!< [0..0] PVD1 Deep Standby Cancel Signal Enable                             */
            __IOM uint8_t DPVD2IE : 1; /*!< [1..1] PVD2 Deep Standby Cancel Signal Enable                             */
            __IOM uint8_t DRTCIIE : 1; /*!< [2..2] RTC Interval interrupt Deep Standby Cancel Signal Enable           */
            __IOM uint8_t DRTCAIE : 1; /*!< [3..3] RTC Alarm interrupt Deep Standby Cancel Signal Enable              */
            __IOM uint8_t DNMIE   : 1; /*!< [4..4] NMI Pin Deep Standby Cancel Signal Enable                          */
            uint8_t               : 3;
        } DPSIER2_b;
    };
    __IM uint8_t  RESERVED28;
    __IM uint16_t RESERVED29;

    union
    {
        __IOM uint8_t DPSIER3;            /*!< (@ 0x00000A14) Deep Standby Interrupt Enable Register 3                   */

        struct
        {
            __IOM uint8_t DUSBFSIE : 1;   /*!< [0..0] USBFS Suspend/Resume Deep Standby Cancel Signal Enable             */
            __IOM uint8_t DUSBHSIE : 1;   /*!< [1..1] USBHS Suspend/Resume Deep Standby Cancel Signal Enable             */
            uint8_t                : 3;
            __IOM uint8_t DIWDTIE  : 1;   /*!< [5..5] IWDT Overflow Deep Standby Cancel Signal Enable                    */
            __IOM uint8_t DSOSTDIE : 1;   /*!< [6..6] Sub-clock Oscillation stop detection Deep Standby Cancel
                                           *   Signal Enable                                                             */
            __IOM uint8_t DVBATTADIE : 1; /*!< [7..7] VBATT Tamper Detection Deep Standby Cancel Signal Enable           */
        } DPSIER3_b;
    };
    __IM uint8_t  RESERVED30;
    __IM uint16_t RESERVED31;

    union
    {
        __IOM uint8_t DPSIFR0;         /*!< (@ 0x00000A18) Deep Standby Interrupt Flag Register 0                     */

        struct
        {
            __IOM uint8_t DIRQ0F : 1;  /*!< [0..0] IRQ0-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ1F : 1;  /*!< [1..1] IRQ1-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ2F : 1;  /*!< [2..2] IRQ2-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ3F : 1;  /*!< [3..3] IRQ3-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ4F : 1;  /*!< [4..4] IRQ4-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ5F : 1;  /*!< [5..5] IRQ5-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ6F : 1;  /*!< [6..6] IRQ6-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ7F : 1;  /*!< [7..7] IRQ7-DS Pin Deep Standby Cancel Flag                               */
        } DPSIFR0_b;
    };
    __IM uint8_t  RESERVED32;
    __IM uint16_t RESERVED33;

    union
    {
        __IOM uint8_t DPSIFR1;         /*!< (@ 0x00000A1C) Deep Standby Interrupt Flag Register 1                     */

        struct
        {
            __IOM uint8_t DIRQ8F  : 1; /*!< [0..0] IRQ8-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ9F  : 1; /*!< [1..1] IRQ9-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ10F : 1; /*!< [2..2] IRQ10-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ11F : 1; /*!< [3..3] IRQ11-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ12F : 1; /*!< [4..4] IRQ12-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ13F : 1; /*!< [5..5] IRQ13-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ14F : 1; /*!< [6..6] IRQ14-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ15F : 1; /*!< [7..7] IRQ15-DS Pin Deep Standby Cancel Flag                              */
        } DPSIFR1_b;
    };
    __IM uint8_t  RESERVED34;
    __IM uint16_t RESERVED35;

    union
    {
        __IOM uint8_t DPSIFR2;         /*!< (@ 0x00000A20) Deep Standby Interrupt Flag Register 2                     */

        struct
        {
            __IOM uint8_t DPVD1IF : 1; /*!< [0..0] PVD1 Deep Standby Cancel Flag                                      */
            __IOM uint8_t DPVD2IF : 1; /*!< [1..1] PVD2 Deep Standby Cancel Flag                                      */
            __IOM uint8_t DRTCIIF : 1; /*!< [2..2] RTC Interval Interrupt Deep Standby Cancel Flag                    */
            __IOM uint8_t DRTCAIF : 1; /*!< [3..3] RTC Alarm Interrupt Deep Standby Cancel Flag                       */
            __IOM uint8_t DNMIF   : 1; /*!< [4..4] NMI Pin Deep Standby Cancel Flag                                   */
            uint8_t               : 3;
        } DPSIFR2_b;
    };
    __IM uint8_t  RESERVED36;
    __IM uint16_t RESERVED37;

    union
    {
        __IOM uint8_t DPSIFR3;            /*!< (@ 0x00000A24) Deep Standby Interrupt Flag Register 3                     */

        struct
        {
            __IOM uint8_t DUSBFSIF : 1;   /*!< [0..0] USBFS Suspend/Resume Deep Standby Cancel Flag                      */
            __IOM uint8_t DUSBHSIF : 1;   /*!< [1..1] USBHS Suspend/Resume Deep Standby Cancel Flag                      */
            uint8_t                : 3;
            __IOM uint8_t DIWDTIF  : 1;   /*!< [5..5] IWDT Overflow Deep Standby Cancel Flag                             */
            __IOM uint8_t DSOSTDIF : 1;   /*!< [6..6] Sub-clock Oscillation stop detection Deep Standby Cancel
                                           *   Flag                                                                      */
            __IOM uint8_t DVBATTADIF : 1; /*!< [7..7] VBATT Tamper Detection Deep Standby Cancel Flag                    */
        } DPSIFR3_b;
    };
    __IM uint8_t  RESERVED38;
    __IM uint16_t RESERVED39;

    union
    {
        __IOM uint8_t DPSIEGR0;        /*!< (@ 0x00000A28) Deep Standby Interrupt Edge Register 0                     */

        struct
        {
            __IOM uint8_t DIRQ0EG : 1; /*!< [0..0] IRQ0-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ1EG : 1; /*!< [1..1] IRQ1-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ2EG : 1; /*!< [2..2] IRQ2-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ3EG : 1; /*!< [3..3] IRQ3-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ4EG : 1; /*!< [4..4] IRQ4-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ5EG : 1; /*!< [5..5] IRQ5-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ6EG : 1; /*!< [6..6] IRQ6-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ7EG : 1; /*!< [7..7] IRQ7-DS Pin Edge Select                                            */
        } DPSIEGR0_b;
    };
    __IM uint8_t  RESERVED40;
    __IM uint16_t RESERVED41;

    union
    {
        __IOM uint8_t DPSIEGR1;         /*!< (@ 0x00000A2C) Deep Standby Interrupt Edge Register 1                     */

        struct
        {
            __IOM uint8_t DIRQ8EG  : 1; /*!< [0..0] IRQ8-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ9EG  : 1; /*!< [1..1] IRQ9-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ10EG : 1; /*!< [2..2] IRQ10-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ11EG : 1; /*!< [3..3] IRQ11-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ12EG : 1; /*!< [4..4] IRQ12-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ13EG : 1; /*!< [5..5] IRQ13-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ14EG : 1; /*!< [6..6] IRQ14-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ15EG : 1; /*!< [7..7] IRQ15-DS Pin Edge Select                                           */
        } DPSIEGR1_b;
    };
    __IM uint8_t  RESERVED42;
    __IM uint16_t RESERVED43;

    union
    {
        __IOM uint8_t DPSIEGR2;        /*!< (@ 0x00000A30) Deep Standby Interrupt Edge Register 2                     */

        struct
        {
            __IOM uint8_t DPVD1EG : 1; /*!< [0..0] PVD1 Edge Select                                                   */
            __IOM uint8_t DPVD2EG : 1; /*!< [1..1] PVD2 Edge Select                                                   */
            uint8_t               : 2;
            __IOM uint8_t DNMIEG  : 1; /*!< [4..4] NMI Pin Edge Select                                                */
            uint8_t               : 3;
        } DPSIEGR2_b;
    };
    __IM uint8_t  RESERVED44;
    __IM uint16_t RESERVED45;

    union
    {
        __IOM uint8_t DPSIEGR3;         /*!< (@ 0x00000A34) Deep Standby Interrupt Edge Register 3                     */

        struct
        {
            __IOM uint8_t DIRQ16EG : 1; /*!< [0..0] IRQ16-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ17EG : 1; /*!< [1..1] IRQ17-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ18EG : 1; /*!< [2..2] IRQ18-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ19EG : 1; /*!< [3..3] IRQ19-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ20EG : 1; /*!< [4..4] IRQ20-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ21EG : 1; /*!< [5..5] IRQ21-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ22EG : 1; /*!< [6..6] IRQ22-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ23EG : 1; /*!< [7..7] IRQ23-DS Pin Edge Select                                           */
        } DPSIEGR3_b;
    };
    __IM uint8_t  RESERVED46;
    __IM uint16_t RESERVED47;
    __IM uint32_t RESERVED48[22];

    union
    {
        __IOM uint8_t LPSCR;           /*!< (@ 0x00000A90) Low Power State Control Register                           */

        struct
        {
            __IOM uint8_t LPMD : 4;    /*!< [3..0] Low power mode setting bit                                         */
            uint8_t            : 4;
        } LPSCR_b;
    };
    __IM uint8_t  RESERVED49;
    __IM uint16_t RESERVED50;
    __IM uint32_t RESERVED51;

    union
    {
        __IOM uint8_t SSCR1;           /*!< (@ 0x00000A98) Software Standby Control Register 1                        */

        struct
        {
            __IOM uint8_t SS2FR : 1;   /*!< [0..0] Software Standby 2 Fast Return                                     */
            uint8_t             : 1;
            __IOM uint8_t SS2LP : 2;   /*!< [3..2] Software Standby 2 low power select                                */
            uint8_t             : 4;
        } SSCR1_b;
    };
    __IM uint8_t  RESERVED52;
    __IM uint16_t RESERVED53;

    union
    {
        __IOM uint8_t SVSCR;           /*!< (@ 0x00000A9C) SSTBY Voltage Scaling Control Register                     */

        struct
        {
            __IOM uint8_t SVSCM : 3;   /*!< [2..0] SSTBY Voltage Scaling Control Mode Bit                             */
            uint8_t             : 5;
        } SVSCR_b;
    };
    __IM uint8_t  RESERVED54;
    __IM uint16_t RESERVED55;
    __IM uint32_t RESERVED56[4];

    union
    {
        __IOM uint8_t LVOCR;           /*!< (@ 0x00000AB0) Low Power State Control Register                           */

        struct
        {
            __IOM uint8_t LVO0E : 1;   /*!< [0..0] Low Voltage Operation 0 Enable                                     */
            uint8_t             : 7;
        } LVOCR_b;
    };
    __IM uint8_t  RESERVED57;
    __IM uint16_t RESERVED58;

    union
    {
        __IOM uint8_t MWMCR;           /*!< (@ 0x00000AB4) MRAM-OTP Write Mode Control Register                       */

        struct
        {
            __IOM uint8_t MWM : 2;     /*!< [1..0] MRAM-OTP Write Mode [1:0]                                          */
            uint8_t           : 6;
        } MWMCR_b;
    };
    __IM uint8_t  RESERVED59;
    __IM uint16_t RESERVED60;

    union
    {
        __IOM uint8_t DCCR;             /*!< (@ 0x00000AB8) DSTBY CVMR Control Register                                */

        struct
        {
            __IOM uint8_t DSTBYCON : 1; /*!< [0..0] DSTBY Control                                                      */
            __IOM uint8_t WCODE    : 7; /*!< [7..1] Write CODE                                                         */
        } DCCR_b;
    };
    __IM uint8_t  RESERVED61;
    __IM uint16_t RESERVED62;
    __IM uint32_t RESERVED63[14];

    union
    {
        __IOM uint8_t LDOECR;          /*!< (@ 0x00000AF4) LDO Efficiency Control Register                            */

        struct
        {
            __IOM uint8_t EFUPE  : 1;  /*!< [0..0] LDO Efficiency Up Enable                                           */
            uint8_t              : 6;
            __IM uint8_t EFUPTSF : 1;  /*!< [7..7] LDO Efficiency Up Transition Status Flag                           */
        } LDOECR_b;
    };
    __IM uint8_t  RESERVED64;
    __IM uint16_t RESERVED65;
    __IM uint32_t RESERVED66[3];

    union
    {
        __IOM uint8_t PLL1LDOCR;       /*!< (@ 0x00000B04) PLL1-LDO Control Register                                  */

        struct
        {
            __IOM uint8_t LDOSTP : 1;  /*!< [0..0] LDO Stop                                                           */
            __IOM uint8_t SKEEP  : 1;  /*!< [1..1] STBY Keep                                                          */
            uint8_t              : 6;
        } PLL1LDOCR_b;
    };
    __IM uint8_t  RESERVED67;
    __IM uint16_t RESERVED68;

    union
    {
        __IOM uint8_t PLL2LDOCR;       /*!< (@ 0x00000B08) PLL2-LDO Control Register                                  */

        struct
        {
            __IOM uint8_t LDOSTP : 1;  /*!< [0..0] LDO Stop                                                           */
            __IOM uint8_t SKEEP  : 1;  /*!< [1..1] STBY Keep                                                          */
            uint8_t              : 6;
        } PLL2LDOCR_b;
    };
    __IM uint8_t  RESERVED69;
    __IM uint16_t RESERVED70;

    union
    {
        __IOM uint8_t HOCOLDOCR;       /*!< (@ 0x00000B0C) HOCO-LDO Control Register                                  */

        struct
        {
            __IOM uint8_t LDOSTP : 1;  /*!< [0..0] LDO Stop                                                           */
            __IOM uint8_t SKEEP  : 1;  /*!< [1..1] STBY Keep                                                          */
            uint8_t              : 6;
        } HOCOLDOCR_b;
    };
    __IM uint8_t  RESERVED71;
    __IM uint16_t RESERVED72;
    __IM uint32_t RESERVED73[12];

    union
    {
        __IOM uint8_t DPSIER4;         /*!< (@ 0x00000B40) Deep Standby Interrupt Enable Register 4                   */

        struct
        {
            __IOM uint8_t DIRQ16E : 1; /*!< [0..0] IRQ16-DS Pin Enable                                                */
            __IOM uint8_t DIRQ17E : 1; /*!< [1..1] IRQ17-DS Pin Enable                                                */
            __IOM uint8_t DIRQ18E : 1; /*!< [2..2] IRQ18-DS Pin Enable                                                */
            __IOM uint8_t DIRQ19E : 1; /*!< [3..3] IRQ19-DS Pin Enable                                                */
            __IOM uint8_t DIRQ20E : 1; /*!< [4..4] IRQ20-DS Pin Enable                                                */
            __IOM uint8_t DIRQ21E : 1; /*!< [5..5] IRQ21-DS Pin Enable                                                */
            __IOM uint8_t DIRQ22E : 1; /*!< [6..6] IRQ22-DS Pin Enable                                                */
            __IOM uint8_t DIRQ23E : 1; /*!< [7..7] IRQ23-DS Pin Enable                                                */
        } DPSIER4_b;
    };
    __IM uint8_t  RESERVED74;
    __IM uint16_t RESERVED75;

    union
    {
        __IOM uint8_t DPSIER5;         /*!< (@ 0x00000B44) Deep Standby Interrupt Enable Register 5                   */

        struct
        {
            __IOM uint8_t DIRQ24E : 1; /*!< [0..0] IRQ24-DS Pin Enable                                                */
            __IOM uint8_t DIRQ25E : 1; /*!< [1..1] IRQ25-DS Pin Enable                                                */
            __IOM uint8_t DIRQ26E : 1; /*!< [2..2] IRQ26-DS Pin Enable                                                */
            __IOM uint8_t DIRQ27E : 1; /*!< [3..3] IRQ27-DS Pin Enable                                                */
            __IOM uint8_t DIRQ28E : 1; /*!< [4..4] IRQ28-DS Pin Enable                                                */
            __IOM uint8_t DIRQ29E : 1; /*!< [5..5] IRQ29-DS Pin Enable                                                */
            __IOM uint8_t DIRQ30E : 1; /*!< [6..6] IRQ30-DS Pin Enable                                                */
            __IOM uint8_t DIRQ31E : 1; /*!< [7..7] IRQ31-DS Pin Enable                                                */
        } DPSIER5_b;
    };
    __IM uint8_t  RESERVED76;
    __IM uint16_t RESERVED77;

    union
    {
        __IOM uint8_t DPSIFR4;         /*!< (@ 0x00000B48) Deep Standby Interrupt Flag Register 4                     */

        struct
        {
            __IOM uint8_t DIRQ16F : 1; /*!< [0..0] IRQ16-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ17F : 1; /*!< [1..1] IRQ17-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ18F : 1; /*!< [2..2] IRQ18-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ19F : 1; /*!< [3..3] IRQ19-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ20F : 1; /*!< [4..4] IRQ20-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ21F : 1; /*!< [5..5] IRQ21-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ22F : 1; /*!< [6..6] IRQ22-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ23F : 1; /*!< [7..7] IRQ23-DS Pin Deep Standby Cancel Flag                              */
        } DPSIFR4_b;
    };
    __IM uint8_t  RESERVED78;
    __IM uint16_t RESERVED79;

    union
    {
        __IOM uint8_t DPSIFR5;         /*!< (@ 0x00000B4C) Deep Standby Interrupt Flag Register 5                     */

        struct
        {
            __IOM uint8_t DIRQ24F : 1; /*!< [0..0] IRQ24-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ25F : 1; /*!< [1..1] IRQ25-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ26F : 1; /*!< [2..2] IRQ26-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ27F : 1; /*!< [3..3] IRQ27-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ28F : 1; /*!< [4..4] IRQ28-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ29F : 1; /*!< [5..5] IRQ29-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ30F : 1; /*!< [6..6] IRQ30-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ31F : 1; /*!< [7..7] IRQ31-DS Pin Deep Standby Cancel Flag                              */
        } DPSIFR5_b;
    };
    __IM uint8_t  RESERVED80;
    __IM uint16_t RESERVED81;

    union
    {
        __IOM uint8_t DPSIEGR4;         /*!< (@ 0x00000B50) Deep Standby Interrupt Edge Register 4                     */

        struct
        {
            __IOM uint8_t DIRQ24EG : 1; /*!< [0..0] IRQ24-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ25EG : 1; /*!< [1..1] IRQ25-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ26EG : 1; /*!< [2..2] IRQ26-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ27EG : 1; /*!< [3..3] IRQ27-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ28EG : 1; /*!< [4..4] IRQ28-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ29EG : 1; /*!< [5..5] IRQ29-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ30EG : 1; /*!< [6..6] IRQ30-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ31EG : 1; /*!< [7..7] IRQ31-DS Pin Edge Select                                           */
        } DPSIEGR4_b;
    };
    __IM uint8_t  RESERVED82;
    __IM uint16_t RESERVED83;
} R_LPM_Type;                          /*!< Size = 2900 (0xb54)                                                       */

/** @} */ /* End of group Device_Peripheral_peripherals */

#endif /* R_LPM_REG_H */
