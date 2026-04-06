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

#ifndef R_SAU_REG_H
#define R_SAU_REG_H

/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                           R_SAU                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Secure Attribution Unit (R_SAU)
 */

typedef struct                         /*!< (@ 0x81000400) R_SAU Structure                                            */
{
    union
    {
        __IOM uint32_t AAS;            /*!< (@ 0x00000000) Secure Address-Space Attribute Setting Register            */

        struct
        {
            __IOM uint32_t NS : 1;     /*!< [0..0] Non-secure Address Space Setting                                   */
            uint32_t          : 31;
        } AAS_b;
    };

    union
    {
        __IOM uint32_t SESTS;          /*!< (@ 0x00000004) Secure Exception Status Register                           */

        struct
        {
            __IM uint32_t ISER : 1;    /*!< [0..0] Instruction Access Secure Exception Flag                           */
            __IM uint32_t DSER : 1;    /*!< [1..1] Data Access Secure Exception Occurrence Flag                       */
            uint32_t           : 30;
        } SESTS_b;
    };

    union
    {
        __IOM uint32_t SECLR;          /*!< (@ 0x00000008) Secure Exception Status Clear Register                     */

        struct
        {
            __IOM uint32_t CLR : 1;    /*!< [0..0] Exception Status Clear                                             */
            uint32_t           : 31;
        } SECLR_b;
    };

    union
    {
        __IM uint32_t SEA;             /*!< (@ 0x0000000C) Secure Exception Address Register                          */

        struct
        {
            __IM uint32_t SEA : 32;    /*!< [31..0] Secure Exception Address                                          */
        } SEA_b;
    };
} R_SAU_Type;                          /*!< Size = 16 (0x10)                                                          */

/** @} */ /* End of group Device_Peripheral_peripherals */

#endif /* R_SAU_REG_H */
