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

#ifndef R_TSN_POSMSK_H
#define R_TSN_POSMSK_H

/* =========================================================================================================================== */
/* ================                                         R_TSN_CAL                                         ================ */
/* =========================================================================================================================== */

/* =========================================================  TSCDR  ========================================================= */
#define R_TSN_CAL_TSCDR_TSCDR_Pos    (0UL)          /*!< TSCDR (Bit 0)                                         */
#define R_TSN_CAL_TSCDR_TSCDR_Msk    (0xffffffffUL) /*!< TSCDR (Bitfield-Mask: 0xffffffff)                     */

/* =========================================================================================================================== */
/* ================                                          R_TSN_CTRL                                          ================ */
/* =========================================================================================================================== */

/* =========================================================  TSCR  ========================================================== */
#define R_TSN_CTRL_TSCR_TSEN_Pos    (7UL)    /*!< TSEN (Bit 7)                                          */
#define R_TSN_CTRL_TSCR_TSEN_Msk    (0x80UL) /*!< TSEN (Bitfield-Mask: 0x01)                            */
#define R_TSN_CTRL_TSCR_TSOE_Pos    (4UL)    /*!< TSOE (Bit 4)                                          */
#define R_TSN_CTRL_TSCR_TSOE_Msk    (0x10UL) /*!< TSOE (Bitfield-Mask: 0x01)                            */

#endif /* R_TSN_POSMSK_H */
