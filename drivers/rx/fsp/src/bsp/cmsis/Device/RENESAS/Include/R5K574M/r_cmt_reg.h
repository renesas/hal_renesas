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

#ifndef R_CMT_REG_H
#define R_CMT_REG_H

/* =========================================================================================================================== */
/* ================                              Device Specific Cluster Section                              ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief CMT_UNT_CM [CM] (2 Timer Start Register Pairs)
 */
typedef struct
{
    union
    {
        __IOM uint16_t CR;             /*!< (@ 0x00000000) Compare Match Timer Control Register                       */

        struct
        {
            __IOM uint16_t CKS  : 2;   /*!< [1..0] CKS                                                                */
            uint16_t            : 4;
            __IOM uint16_t CMIE : 1;   /*!< [6..6] CMIE                                                               */
            uint16_t            : 9;
        } CR_b;
    };
    __IOM uint16_t CNT;                /*!< (@ 0x00000002) Compare Match Timer Counter                                */
    __IOM uint16_t COR;                /*!< (@ 0x00000004) Compare Match Timer Constant Register                      */
} R_CMT_UNT_CM_Type;                   /*!< Size = 6 (0x6)                                                            */

/**
 * @brief CMT_UNT [UNT] (2 Timer Start Register Units)
 */
typedef struct
{
    union
    {
        __IOM uint16_t CMSTR0;         /*!< (@ 0x00000000) Compare Match Timer Start Register                         */

        struct
        {
            __IOM uint16_t STR0 : 1;   /*!< [0..0] Count Start 1                                                      */
            __IOM uint16_t STR1 : 1;   /*!< [1..1] Count Start 0                                                      */
            uint16_t            : 14;
        } CMSTR0_b;
    };
    __IOM R_CMT_UNT_CM_Type CM[2];     /*!< (@ 0x00000002) 2 Timer Start Register Pairs                               */
    __IM uint16_t           RESERVED;
} R_CMT_UNT_Type;                      /*!< Size = 16 (0x10)                                                          */

/** @} */ /* End of group Device_Peripheral_clusters */

/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                           R_CMT                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Compare Match Timer (R_CMT)
 */

typedef struct                         /*!< (@ 0x87826000) R_CMT Structure                                            */
{
    __IOM R_CMT_UNT_Type UNT[2];       /*!< (@ 0x00000000) 2 Timer Start Register Units                               */
} R_CMT_Type;                          /*!< Size = 32 (0x20)                                                          */

/** @} */ /* End of group Device_Peripheral_peripherals */

#endif                                 /* R_CMT_REG_H */
