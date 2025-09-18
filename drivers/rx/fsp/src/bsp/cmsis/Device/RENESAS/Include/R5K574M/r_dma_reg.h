/***********************************************************************************************************************
 * Copyright [2020-2025] Renesas Electronics Corporation and/or its affiliates. All Rights Reserved.
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

#ifndef R_DMA_REG_H
#define R_DMA_REG_H

/* =========================================================================================================================== */
/* ================                                           R_DMA                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief DMA Controller Common (R_DMA)
 */

typedef struct                         /*!< (@ 0x87007800) R_DMA Structure                                            */
{
    union
    {
        __IOM uint8_t DMAST;           /*!< (@ 0x00000000) DMAC Module Start Register                                 */

        struct
        {
            __IOM uint8_t DMST : 1;    /*!< [0..0] DMAC Module Start                                                  */
            uint8_t            : 7;
        } DMAST_b;
    };
    __IM uint8_t  RESERVED;
    __IM uint16_t RESERVED1;

    union
    {
        __IM uint8_t DMIST;            /*!< (@ 0x00000004) DMAC74 Interrupt Status Monitor Register                   */

        struct
        {
            uint8_t            : 4;
            __IM uint8_t DMIS4 : 1;    /*!< [4..4] DMAC4 Interrupt Status Flag                                        */
            __IM uint8_t DMIS5 : 1;    /*!< [5..5] DMAC5 Interrupt Status Flag                                        */
            __IM uint8_t DMIS6 : 1;    /*!< [6..6] DMAC6 Interrupt Status Flag                                        */
            __IM uint8_t DMIS7 : 1;    /*!< [7..7] DMAC7 Interrupt Status Flag                                        */
        } DMIST_b;
    };
    __IM uint8_t  RESERVED2;
    __IM uint16_t RESERVED3;
    __IM uint32_t RESERVED4[2];

    union
    {
        __IOM uint8_t DMCTL;           /*!< (@ 0x00000010) DMAC Control Register                                      */

        struct
        {
            __IOM uint8_t PR   : 1;    /*!< [0..0] Priority Control Select                                            */
            uint8_t            : 3;
            __IOM uint8_t ERCH : 1;    /*!< [4..4] Clear Channel Select. Select whether to stop all DMAC
                                        *   channels or only the channel where the error occurred when
                                        *   an error occurs.                                                          */
            uint8_t : 3;
        } DMCTL_b;
    };
    __IM uint8_t  RESERVED5;
    __IM uint16_t RESERVED6;
    __IM uint32_t RESERVED7[11];

    union
    {
        __IOM uint32_t DMECHR;          /*!< (@ 0x00000040) DMAC Error Channel Register                                */

        struct
        {
            __IM uint32_t DMECH : 4;    /*!< [3..0] DMAC Error channel Flag. Indicates the channel number
                                         *   causing the error.                                                        */
            uint32_t               : 4;
            __IM uint32_t DMECHSAM : 1; /*!< [8..8] DMAC Error channel Security Attribution Monitor Flag.
                                         *   Indicates the security attribution of a channel causing
                                         *   the error.                                                                */
            uint32_t             : 3;
            __IM uint32_t DMERRW : 1;   /*!< [12..12] DMAC Error access Read/Write Attribute Flag. Indicates
                                         *   the read/write attribute of the error access.                             */
            uint32_t              : 3;
            __IOM uint32_t DMESTA : 1;  /*!< [16..16] DMAC Error Status Flag                                           */
            uint32_t              : 15;
        } DMECHR_b;
    };
} R_DMA_Type;                           /*!< Size = 68 (0x44)                                                          */

#endif /* R_DMA_REG_H */
