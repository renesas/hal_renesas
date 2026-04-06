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

#ifndef _R_TMR_REG_H
#define _R_TMR_REG_H

/* =========================================================================================================================== */
/* ================                                           R_TMR                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief 8-Bit Timer (R_TMR)
 */

typedef struct                         /*!< (@ 0x00088200) R_TMR Structure                                            */
{
    union
    {
        __IOM uint8_t TCR[2];          /*!< (@ 0x00000000) Timer Control Register                                     */

        struct
        {
            uint8_t             : 3;
            __IOM uint8_t CCLR  : 2;   /*!< [4..3] Counter Clear                                                      */
            __IOM uint8_t OVIE  : 1;   /*!< [5..5] Overflow Interrupt Enable                                          */
            __IOM uint8_t CMIEA : 1;   /*!< [6..6] Compare Match A Interrupt Enable                                   */
            __IOM uint8_t CMIEB : 1;   /*!< [7..7] Compare Match B Interrupt Enable                                   */
        } TCR_b[2];
    };

    union
    {
        __IOM uint8_t TCSR[2];         /*!< (@ 0x00000002) Timer Control/Status Register                              */

        struct
        {
            __IOM uint8_t OSA : 2;     /*!< [1..0] Output Select A                                                    */
            __IOM uint8_t OSB : 2;     /*!< [3..2] Output Select B                                                    */
            uint8_t           : 4;
        } TCSR_b[2];
    };

    union
    {
        __IOM uint16_t TCORA;
        struct
        {
            __IOM uint8_t TCORA;
        } TCORA_BY[2];
    };

    union
    {
        __IOM uint16_t TCORB;
        struct
        {
            __IOM uint8_t TCORB;
        } TCORB_BY[2];
    };

    union
    {
        __IOM uint16_t TCNT;
        struct
        {
            __IOM uint8_t TCNT;
        } TCNT_BY[2];
    };

    union
    {
        __IOM uint8_t TCCR[2];         /*!< (@ 0x0000000A) Timer Counter Control Register                             */

        struct
        {
            __IOM uint8_t CKS   : 3;   /*!< [2..0] Clock Select                                                       */
            __IOM uint8_t CSS   : 2;   /*!< [4..3] Clock Source Select                                                */
            uint8_t             : 2;
            __IOM uint8_t TMRIS : 1;   /*!< [7..7] Timer Reset Detection Condition Select                             */
        } TCCR_b[2];
    };
    union
    {
        __IOM uint8_t TCSTR[2];        /*!< (@ 0x0000000C) Timer Counter Start Register                               */

        struct
        {
            __IOM uint8_t TCS : 1;     /*!< [0..0] Timer Counter Status                                               */
            uint8_t           : 7;
        } TCSTR_b[2];
    };
} R_TMR_Type;                          /*!< Size = 13 (0xd)                                                           */

#endif  /* R_TMR_REG_H */
