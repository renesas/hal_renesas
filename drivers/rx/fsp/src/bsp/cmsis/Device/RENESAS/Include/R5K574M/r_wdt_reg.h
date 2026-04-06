/***********************************************************************************************************************
 * Copyright [2020-2026] Renesas Electronics Corporation and/or its affiliates. All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates. and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas"). No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale. Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability. No license, express or implied, to any
 * intellectual property right is granted by Renesas. This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION. RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#ifndef R_WDT_REG_H
#define R_WDT_REG_H

/* =========================================================================================================================== */
/* ================                                           R_WDT                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Watchdog Timer (R_WDT)
 */

typedef struct                         /*!< (@ 0x87802600) R_WDT Structure                                            */
{
    union
    {
        __IOM uint8_t WDTRR;           /*!< (@ 0x00000000) WDT Refresh Register                                       */

        struct
        {
            __IOM uint8_t WDTRR : 8;   /*!< [7..0] WDTRR is an 8-bit register that refreshes the down-counter
                                        *   of the WDT                                                                */
        } WDTRR_b;
    };
    __IM uint8_t RESERVED;

    union
    {
        __IOM uint16_t WDTCR;          /*!< (@ 0x00000002) WDT Control Register                                       */

        struct
        {
            __IOM uint16_t TOPS : 2;   /*!< [1..0] Timeout Period Select                                              */
            uint16_t            : 2;
            __IOM uint16_t CKS  : 4;   /*!< [7..4] Clock Division Ratio Select                                        */
            __IOM uint16_t RPES : 2;   /*!< [9..8] Window End Position Select                                         */
            uint16_t            : 2;
            __IOM uint16_t RPSS : 2;   /*!< [13..12] Window Start Position Select                                     */
            uint16_t            : 2;
        } WDTCR_b;
    };

    union
    {
        __IOM uint16_t WDTSR;           /*!< (@ 0x00000004) WDT Status Register                                        */

        struct
        {
            __IM uint16_t  CNTVAL : 14; /*!< [13..0] Counter Value                                                     */
            __IOM uint16_t UNDFF  : 1;  /*!< [14..14] Underflow Flag                                                   */
            __IOM uint16_t REFEF  : 1;  /*!< [15..15] Refresh Error Flag                                               */
        } WDTSR_b;
    };

    union
    {
        __IOM uint8_t WDTRCR;          /*!< (@ 0x00000006) WDT Reset Control Register                                 */

        struct
        {
            uint8_t               : 7;
            __IOM uint8_t RSTIRQS : 1; /*!< [7..7] Reset Interrupt Request Select                                     */
        } WDTRCR_b;
    };
    __IM uint8_t RESERVED1;

    union
    {
        __IOM uint8_t WDTCSTPR;        /*!< (@ 0x00000008) WDT Count Stop Control Register                            */

        struct
        {
            uint8_t              : 7;
            __IOM uint8_t SLCSTP : 1;  /*!< [7..7] Sleep Mode Count Stop Control                                      */
        } WDTCSTPR_b;
    };
    __IM uint8_t  RESERVED2;
    __IM uint16_t RESERVED3;
} R_WDT_Type;                          /*!< Size = 12 (0xc)                                                           */

#endif /* R_WDT_REG_H */
