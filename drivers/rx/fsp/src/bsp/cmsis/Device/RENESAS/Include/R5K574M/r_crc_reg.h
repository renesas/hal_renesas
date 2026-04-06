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

#ifndef R_CRC_REG_H
#define R_CRC_REG_H

/* =========================================================================================================================== */
/* ================                                           R_CRC                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief CRC Calculator (R_CRC)
 */

typedef struct                         /*!< (@ 0x87410000) R_CRC Structure                                            */
{
    union
    {
        __IOM uint8_t CRCCR0;          /*!< (@ 0x00000000) CRC Control Register                                       */

        struct
        {
            __IOM uint8_t GPS    : 3;  /*!< [2..0] CRC Generating Polynomial Switching                                */
            uint8_t              : 3;
            __IOM uint8_t LMS    : 1;  /*!< [6..6] CRC Calculation Switching                                          */
            __IOM uint8_t DORCLR : 1;  /*!< [7..7] CRCDOR Register Clear                                              */
        } CRCCR_b;
    };
    __IM uint8_t  RESERVED;
    __IM uint16_t RESERVED1;

    union
    {
        union
        {
            __IOM uint32_t CRCDIR;          /*!< (@ 0x00000004) CRC Data Input Register                                    */

            struct
            {
                __IOM uint32_t CRCDIR : 32; /*!< [31..0] Calculation input Data (Case of CRC-32, CRC-32C)                  */
            } CRCDIR_b;
        };

        union
        {
            __IOM uint8_t CRCDIR_BY;         /*!< (@ 0x00000004) CRC Data Input Register (byte access)                      */

            struct
            {
                __IOM uint8_t CRCDIR_BY : 8; /*!< [7..0] Calculation input Data (Case of CRC-8, CRC-16 or CRC-CCITT)        */
            } CRCDIR_BY_b;
        };
    };

    union
    {
        union
        {
            __IOM uint32_t CRCDOR;          /*!< (@ 0x00000008) CRC Data Output Register                                   */

            struct
            {
                __IOM uint32_t CRCDOR : 32; /*!< [31..0] Calculation output Data (Case of CRC-32, CRC-32C)                 */
            } CRCDOR_b;
        };

        union
        {
            __IOM uint16_t CRCDOR_HA;          /*!< (@ 0x00000008) CRC Data Output Register (halfword access)                 */

            struct
            {
                __IOM uint16_t CRCDOR_HA : 16; /*!< [15..0] Calculation output Data (Case of CRC-16 or CRC-CCITT)             */
            } CRCDOR_HA_b;
        };

        union
        {
            __IOM uint8_t CRCDOR_BY;         /*!< (@ 0x00000008) CRC Data Output Register(byte access)                      */

            struct
            {
                __IOM uint8_t CRCDOR_BY : 8; /*!< [7..0] Calculation output Data (Case of CRC-8)                            */
            } CRCDOR_BY_b;
        };
    };
} R_CRC_Type;                                /*!< Size = 12 (0xc)                                                           */

#endif /* R_CRC_REG_H */
