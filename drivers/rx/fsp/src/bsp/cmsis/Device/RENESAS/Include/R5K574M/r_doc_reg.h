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

#ifndef R_DOC_B_REG_H
#define R_DOC_B_REG_H

/* =========================================================================================================================== */
/* ================                                          R_DOC_B                                          ================ */
/* =========================================================================================================================== */

/**
 * @brief Data Operation Circuit (R_DOC_B)
 */

typedef struct                         /*!< (@ 0x87411000) R_DOC_B Structure                                          */
{
    union
    {
        __IOM uint8_t DOCR;            /*!< (@ 0x00000000) DOC Control Register                                       */

        struct
        {
            __IOM uint8_t OMS    : 2;  /*!< [1..0] Operating Mode Select                                              */
            uint8_t              : 1;
            __IOM uint8_t DOPSZ  : 1;  /*!< [3..3] Data Operation Size Select                                         */
            __IOM uint8_t DCSEL  : 3;  /*!< [6..4] Detection Condition Select                                         */
            __IOM uint8_t DOPCIE : 1;  /*!< [7..7] Data Operation Circuit Interrput Enable                            */
        } DOCR_b;
    };
    __IM uint8_t  RESERVED;
    __IM uint16_t RESERVED1;

    union
    {
        __IM uint8_t DOSR;             /*!< (@ 0x00000004) DOC Status Register                                        */

        struct
        {
            __IM uint8_t DOPCF : 1;    /*!< [0..0] Data Operation Result Flag Indicates the result of an
                                        *   operation.                                                                */
            uint8_t : 7;
        } DOSR_b;
    };
    __IM uint8_t  RESERVED2;
    __IM uint16_t RESERVED3;

    union
    {
        __OM uint8_t DOSCR;            /*!< (@ 0x00000008) DOC Status Clear Register                                  */

        struct
        {
            __OM uint8_t DOPCFCL : 1;  /*!< [0..0] Data Operation Result Clear                                        */
            uint8_t              : 7;
        } DOSCR_b;
    };
    __IM uint8_t   RESERVED4;
    __IM uint16_t  RESERVED5;
    __IOM uint32_t DODIR;              /*!< (@ 0x0000000C) DOC Data Input Register                                    */
    __IOM uint32_t DODSR0;             /*!< (@ 0x00000010) DOC Data Setting Register 0                                */
    __IOM uint32_t DODSR1;             /*!< (@ 0x00000014) DOC Data Setting Register 1                                */
} R_DOC_B_Type;                        /*!< Size = 24 (0x18)                                                        */

#endif /* R_DOC_B_REG_H */
