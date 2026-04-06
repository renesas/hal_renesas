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

#ifndef R_TSN_REG_H
#define R_TSN_REG_H

/* =========================================================================================================================== */
/* ================                                         R_TSN_CAL                                         ================ */
/* =========================================================================================================================== */

/**
 * @brief Temperature Sensor (R_TSN_CAL)
 */

typedef struct                         /*!< (@ 0x86C12DA0) R_TSN_CAL Structure                                        */
{
    union
    {
        __IM uint32_t TSCDR;           /*!< (@ 0x00000000) Temperature Sensor 32 bit Calibration Data Register        */

        struct
        {
            __IM uint32_t TSCDR : 32;  /*!< [31..0] The 32 bit TSCDR register stores temperature sensor
                                        *   calibration converted value.                                              */
        } TSCDR_b;
    };
} R_TSN_CAL_Type;                      /*!< Size = 4 (0x4)                                                            */

/* =========================================================================================================================== */
/* ================                                          R_TSN_CTRL                                          ================ */
/* =========================================================================================================================== */

/**
 * @brief Temperature Sensor (R_TSN_CTRL)
 */

typedef struct                         /*!< (@ 0x87835000) R_TSN_CTRL Structure                                          */
{
    union
    {
        __IOM uint8_t TSCR;            /*!< (@ 0x00000000) Temperature Sensor Control Register                        */

        struct
        {
            uint8_t            : 4;
            __IOM uint8_t TSOE : 1;    /*!< [4..4] Temperature Sensor Output Enable                                   */
            uint8_t            : 2;
            __IOM uint8_t TSEN : 1;    /*!< [7..7] Temperature Sensor Enable                                          */
        } TSCR_b;
    };
} R_TSN_CTRL_Type;                     /*!< Size = 1 (0x1)                                                            */

#endif                                 /* R_TSN_REG_H */
