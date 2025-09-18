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

#ifndef R_DAC_B_REG_H
#define R_DAC_B_REG_H

/* =========================================================================================================================== */
/* ================                                          R_DAC_B0                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief 12-bit D/A converter (R_DAC_B0)
 */

typedef struct                         /*!< (@ 0x87833000) R_DAC_B0 Structure                                           */
{
    __IOM uint16_t DADR;               /*!< (@ 0x00000000) D/A Data Register 0                                        */
    __IM uint16_t  RESERVED;

    union
    {
        __IOM uint32_t DACR0;            /*!< (@ 0x00000004) D/A Control Register 0                                     */

        struct
        {
            __IOM uint32_t DACEN    : 1; /*!< [0..0] DA Conversion Enable                                               */
            uint32_t                : 14;
            __IOM uint32_t DAE      : 1; /*!< [15..15] DA Batch Conversion Control                                      */
            uint32_t                : 12;
            __IOM uint32_t DAOE     : 1; /*!< [28..28] D/A Output Enable                                                */
            uint32_t                : 1;
            __IOM uint32_t DAOUTDIS : 2; /*!< [31..30] D/A Output Selection                                             */
        } DACR0_b;
    };

    union
    {
        __IOM uint32_t DACR1;          /*!< (@ 0x00000008) D/A Control Register 1                                     */

        struct
        {
            uint32_t             : 16;
            __IOM uint32_t DPSEL : 1;  /*!< [16..16] Format Select                                                    */
            uint32_t             : 15;
        } DACR1_b;
    };

    union
    {
        __IOM uint32_t DACR2;          /*!< (@ 0x0000000C) D/A Control Register 2                                     */

        struct
        {
            uint32_t              : 8;
            __IOM uint32_t OFSSEL : 1; /*!< [8..8] DAC-HM Operating Voltage Mode Selection                            */
            uint32_t              : 22;
            __IOM uint32_t DAAMP  : 1; /*!< [31..31] Amplifier Control                                                */
        } DACR2_b;
    };
} R_DAC_B0_Type;                       /*!< Size = 16 (0x10)                                                          */

#endif /* R_DAC_B_REG_H */
